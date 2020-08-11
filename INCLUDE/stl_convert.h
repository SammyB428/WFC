/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_CONVERT_HEADER_FILE )

#define STL_CONVERT_HEADER_FILE

#include "stl_trim.h"
#include "stl_with.h"

inline _Check_return_ double as_double(_In_ std::string_view buffer) noexcept
{
    if (buffer.empty() == true)
    {
        return(0);
    }

#if ! defined( WE_ARE_BUILDING_WFC_ON_UNIX )
    double return_value = 0.0;

    (void)std::from_chars(buffer.data(), buffer.data() + buffer.length(), return_value);

    return(return_value);
#else // WE_ARE_BUILDING_WFC_ON_UNIX
    // gcc doesn't support from_chars(double)
    return(std::stod(std::string(buffer, buffer_size)));
#endif // WE_ARE_BUILDING_WFC_ON_UNIX
}

inline _Check_return_ double as_double(_In_ std::wstring_view s) noexcept
{
    if (s.empty() == true)
    {
        return(0);
    }

    // See if we can optimize for small strings

    auto const string_length = s.length();

    if (string_length < 26)
    {
        char ascii_string[27]{ 0 }; // Deliberately NOT initializing for speed, we don't need null termination

        for (auto const string_index : Range(string_length))
        {
            ascii_string[string_index] = static_cast<char>(s[string_index]);
        }

        return(as_double(std::string_view(ascii_string, string_length)));
    }
    else
    {
        std::string ascii_string;

        ascii_string.reserve(string_length);

        for (auto const string_index : Range(string_length))
        {
            ascii_string.push_back(static_cast<char>(s[string_index]));
        }

        return(as_double(ascii_string));
    }
}

inline _Check_return_ int64_t ascii_string_to_integer(_In_ std::string_view ascii_string, _In_ int radix = 10 ) noexcept
{
    int64_t return_value = 0;

    (void) std::from_chars(ascii_string.data(), ascii_string.data() + ascii_string.length(), return_value, radix);

    return(return_value);
}

inline _Check_return_ uint64_t ascii_string_to_unsigned_integer(_In_ std::string_view ascii_string, _In_ int radix = 10) noexcept
{
    uint64_t return_value = 0;

    (void)std::from_chars(ascii_string.data(), ascii_string.data() + ascii_string.length(), return_value, radix);

    return(return_value);
}

inline _Check_return_ int64_t as_integer(_In_ std::string_view buffer) noexcept
{
    if (buffer.empty() == true)
    {
        return(0);
    }

    int radix = 10;

    std::size_t offset_of_first_digit = 0;

    while (is_space_character(buffer[offset_of_first_digit]) == true)
    {
        offset_of_first_digit++;
    }

    if (offset_of_first_digit >= buffer.length())
    {
        return(0);
    }

    if ((buffer.length() - offset_of_first_digit) > 1)
    {
        if (buffer[offset_of_first_digit] == '0' and
            (buffer[offset_of_first_digit + 1] == 'x' or buffer[offset_of_first_digit + 1] == 'X'))
        {
            radix = 16;
            offset_of_first_digit += 2;
        }
        else if (buffer[offset_of_first_digit] == 'x' or buffer[offset_of_first_digit] == 'X')
        {
            radix = 16;
            offset_of_first_digit++;
        }
    }

    return(ascii_string_to_integer(buffer.substr(offset_of_first_digit), radix));
}

inline _Check_return_ uint64_t as_unsigned_integer(_In_ std::string_view buffer) noexcept
{
    if (buffer.empty() == true)
    {
        return(0);
    }

    int radix = 10;

    std::size_t offset_of_first_digit = 0;

    while (is_space_character(buffer[offset_of_first_digit]) == true)
    {
        offset_of_first_digit++;
    }

    if (offset_of_first_digit >= buffer.length())
    {
        return(0);
    }

    if ((buffer.length() - offset_of_first_digit) > 1)
    {
        if (buffer[offset_of_first_digit] == '0' and
            (buffer[offset_of_first_digit + 1] == 'x' or buffer[offset_of_first_digit + 1] == 'X'))
        {
            radix = 16;
            offset_of_first_digit += 2;
        }
        else if (buffer[offset_of_first_digit] == 'x' or buffer[offset_of_first_digit] == 'X')
        {
            radix = 16;
            offset_of_first_digit++;
        }
    }

    return(ascii_string_to_unsigned_integer(buffer.substr(offset_of_first_digit), radix));
}

inline _Check_return_ int64_t as_integer(_In_ std::wstring_view s) noexcept
{
    if (s.empty() == true)
    {
        return(0);
    }

    // See if we can optimize for small strings

    auto const string_length = s.length();

    if (string_length < 26)
    {
        char ascii_string[27]; // Deliberately NOT initializing for speed, we don't need null termination

        for (auto const string_index : Range(string_length))
        {
            ascii_string[string_index] = static_cast<char>(s[string_index]);
        }

        return(as_integer(std::string_view(ascii_string, string_length)));
    }

    int radix = 10;

    std::wstring value(s);

    trim(value);

    if (starts_with_no_case(value, WSTRING_VIEW(L"0x")) == true)
    {
        radix = 16;
        value.erase(0, 2);
    }
    else if (value[0] == 'x')
    {
        radix = 16;
        value.erase(0, 1);
    }

    int64_t const return_value = std::stoll(value, nullptr, radix);

    return(return_value);
}

inline _Check_return_ uint64_t as_unsigned_integer(_In_ std::wstring_view s) noexcept
{
    if (s.empty() == true)
    {
        return(0);
    }

    // See if we can optimize for small strings

    auto const string_length = s.length();

    if (string_length < 26)
    {
        char ascii_string[27]; // Deliberately NOT initializing for speed, we don't need null termination

        for (auto const string_index : Range(string_length))
        {
            ascii_string[string_index] = static_cast<char>(s[string_index]);
        }

        return(as_unsigned_integer(std::string_view(ascii_string, string_length)));
    }

    int radix = 10;

    std::wstring value(s);

    trim(value);

    if (starts_with_no_case(value, WSTRING_VIEW(L"0x")) == true)
    {
        radix = 16;
        value.erase(0, 2);
    }
    else if (value[0] == 'x')
    {
        radix = 16;
        value.erase(0, 1);
    }

    uint64_t const return_value = std::stoull(value, nullptr, radix);

    return(return_value);
}

inline _Check_return_ int64_t as_hexadecimal_integer(_In_ std::wstring const& s) noexcept
{
    int64_t const return_value = std::stoll(s, nullptr, 16);

    return(return_value);
}

inline _Check_return_ wchar_t const * from_defined_constant(_In_ uint64_t const value, _In_reads_(number_of_definitions) const Win32FoundationClasses::DEFINED_WIDE_STRING definitions[], _In_ std::size_t const number_of_definitions) noexcept
{
    if (definitions == nullptr or number_of_definitions < 1)
    {
        return(nullptr);
    }

    Win32FoundationClasses::DEFINED_WIDE_STRING value_to_find;

    value_to_find.Constant = value;
    value_to_find.StringValue = nullptr;

    auto entry = static_cast<Win32FoundationClasses::DEFINED_WIDE_STRING const *>(std::bsearch(&value_to_find, definitions, number_of_definitions, sizeof(Win32FoundationClasses::DEFINED_WIDE_STRING), compare_defines));

    if (entry == nullptr)
    {
        return(nullptr);
    }

    return(entry->StringValue);
}

#endif // STL_CONVERT_HEADER_FILE