#if ! defined( STL_STRING_HELPERS_HEADER_FILE )

#define STL_STRING_HELPERS_HEADER_FILE

#define STRING_VIEW(_x)  std::string_view(  _x, std::size(_x) - 1)
#define WSTRING_VIEW(_x) std::wstring_view( _x, std::size(_x) - 1)

// sprintf replacement code
// https://msdn.microsoft.com/en-us/magazine/dn913181.aspx
// http://en.cppreference.com/w/cpp/io/c/fprintf

template <typename T>
T PrintfArgument(T value) noexcept
{
    return value;
}

template <typename T>
T const* PrintfArgument(std::basic_string<T> const& value) noexcept
{
    return value.c_str();
}

inline wchar_t const* PrintfArgument(std::filesystem::path const& value) noexcept
{
    return(value.c_str());
}

template <typename ... Args>
void format(_Inout_ std::wstring& result, _In_z_ _Printf_format_string_ wchar_t const* format_string, Args const& ... args) noexcept
{
#pragma warning( push )
#pragma warning( disable : 4996 )
    int const number_of_characters_to_allocate = _snwprintf(nullptr, 0, format_string, PrintfArgument(args) ...);
#pragma warning( pop )

    if (number_of_characters_to_allocate < 1)
    {
        result.clear();
        return;
    }

    result.resize(static_cast<std::size_t>(number_of_characters_to_allocate) + 1);

    std::size_t const number_of_characters_written = _snwprintf_s(&result[0], static_cast<std::size_t>(number_of_characters_to_allocate) + 1, _TRUNCATE, format_string, PrintfArgument(args) ...);

    if (number_of_characters_written >= 0)
    {
        // Get rid of the trailing null
        result.resize(number_of_characters_written);
    }
}

template <typename ... Args>
void format(_Inout_ std::string& result, _In_z_ _Printf_format_string_ char const* format_string, Args const& ... args) noexcept
{
#pragma warning( push )
#pragma warning( disable : 4996 )
    int const number_of_characters_to_allocate = _snprintf(nullptr, 0, format_string, PrintfArgument(args) ...);
#pragma warning( pop )

    if (number_of_characters_to_allocate < 1)
    {
        result.clear();
        return;
    }

    result.resize(number_of_characters_to_allocate + 1);

    int const number_of_characters_written = _snprintf_s(&result[0], number_of_characters_to_allocate + 1, _TRUNCATE, format_string, PrintfArgument(args) ...);

    if (number_of_characters_written >= 0)
    {
        // Get rid of the trailing null
        result.resize(number_of_characters_written);
    }
}

inline void append(_Inout_ std::vector<uint8_t>& s, _In_reads_bytes_(number_of_bytes) uint8_t const* __restrict byte_buffer, _In_ std::size_t const number_of_bytes) noexcept
{
    if (byte_buffer == nullptr or number_of_bytes == 0)
    {
        return;
    }

    std::size_t const original_size = s.size();

    s.resize(original_size + number_of_bytes);

    uint8_t * __restrict memory_address = &s[original_size];

    CopyMemory(memory_address, byte_buffer, number_of_bytes);
}

inline constexpr _Check_return_ bool is_space_character(_In_ wchar_t const character) noexcept
{
    if (character == 0x20 or
        character == 0x0D or
        character == 0x0A or
        character == 0x09 or
        character == 0x0B or
        character == 0x0C)
    {
        return(true);
    }

    return(false);
}

#define wfc_to_upper( _x ) ( ( _x >= 0x61 and _x <= 0x7A ) ? _x - 0x20 : _x )

// Avoid annoying out of range exceptions
inline constexpr _Check_return_ wchar_t get_at(_In_ std::wstring_view s, _In_ std::size_t const character_index) noexcept
{
    return (character_index >= s.length()) ? 0 : s.at(character_index);
}

inline _Check_return_ wchar_t make_wide_character_lower_case(_In_ wchar_t const existing_character) noexcept
{
    return(static_cast<wchar_t>(towlower(existing_character)));
}

inline void make_lower(_Inout_ std::wstring& s) noexcept
{
    if (s.empty() == false)
    {
        std::transform(std::begin(s), std::end(s), std::begin(s), make_wide_character_lower_case);
    }
}

inline _Check_return_ wchar_t make_wide_character_upper_case(_In_ wchar_t const existing_character) noexcept
{
    return(static_cast<wchar_t>(towupper(existing_character)));
}

inline void make_upper(_Inout_ std::wstring& s) noexcept
{
    if (s.empty() == false)
    {
        std::transform(std::begin(s), std::end(s), std::begin(s), make_wide_character_upper_case);
    }
}

inline void copy(_Inout_ std::wstring& s, std::string_view ascii_string, _In_ SSIZE_T number_of_characters = (-1), _In_ std::size_t const beginning_at = (0)) noexcept
{
    s.clear();

    if (ascii_string.length() == 0)
    {
        return;
    }

    if (number_of_characters < 0) // used to be -1 but that opened us up to errors
    {
        std::size_t loop_index = beginning_at;
        std::size_t string_size = ascii_string.length();

        if (loop_index >= string_size)
        {
            return;
        }

        _ASSERTE(loop_index <= string_size);

        string_size -= loop_index;

        s.reserve(string_size);

        while (loop_index < ascii_string.length() and ascii_string[loop_index] != 0)
        {
            if (ascii_string[loop_index] == 0x00)
            {
                return;
            }

            s.push_back(ascii_string[loop_index]);
            loop_index++;
        }
    }
    else
    {
        // Only append a finite number of characters...

        std::size_t const ascii_string_length = ascii_string.length();

        if (beginning_at > 0)
        {
            if (beginning_at >= ascii_string_length)
            {
                return;
            }
        }

        if (number_of_characters > static_cast<SSIZE_T>(ascii_string_length - beginning_at))
        {
            number_of_characters = (ascii_string_length - beginning_at);
        }

        s.resize(number_of_characters);

        std::size_t destination_string_index = 0;

        wchar_t* pointer = &s[0];

        for (auto const loop_index : Range(number_of_characters))
        {
            pointer[destination_string_index] = static_cast<uint8_t>(ascii_string[beginning_at + loop_index]);
            _ASSERTE(pointer[destination_string_index] > 0 and pointer[destination_string_index] < 256);
            destination_string_index++;
        }
    }
}

inline void copy_not_null(_Inout_ std::wstring& s, _In_z_ char const* ascii_string, _In_ SSIZE_T number_of_characters = (-1), _In_ std::size_t const beginning_at = (0)) noexcept
{
    if (ascii_string == nullptr)
    {
        s.clear();
        return;
    }

    copy(s, std::string_view(ascii_string), number_of_characters, beginning_at);
}

inline void copy(_Inout_ std::wstring& s, _In_ std::wstring_view string_view, _In_ SSIZE_T const number_of_characters = (-1), _In_ std::size_t const beginning_at = 0) noexcept
{
    s.clear();

    if (string_view.length() == 0)
    {
        return;
    }

    if (number_of_characters < 0)
    {
        s.append(string_view);
    }
    else
    {
        s.reserve(string_view.length() - number_of_characters);
        // Only append a finite number of characters...

        for (auto const loop_index : Range(number_of_characters))
        {
            if ((beginning_at + loop_index) >= string_view.length())
            {
                return;
            }

            if (string_view[beginning_at + loop_index] == 0x00)
            {
                return;
            }

            s.push_back(string_view[beginning_at + loop_index]);
        }
    }
}

inline void copy_not_null(_Inout_ std::wstring& s, _In_z_ wchar_t const* wide_string, _In_ SSIZE_T number_of_characters = (-1), _In_ std::size_t const beginning_at = (0)) noexcept
{
    if (wide_string == nullptr)
    {
        s.clear();
        return;
    }

    copy(s, std::wstring_view(wide_string), number_of_characters, beginning_at);
}

inline void copy(_Inout_ std::wstring& s, _In_ int const code_page, _In_reads_bytes_(number_of_bytes) uint8_t const* buffer, _In_ std::size_t const number_of_bytes) noexcept
{
    if (buffer == nullptr or number_of_bytes < 1)
    {
        s.resize(0);
        return;
    }

    std::size_t const new_buffer_size = (number_of_bytes * 3) + 2;

    auto output_buffer = std::make_unique<wchar_t[]>(new_buffer_size);

    ZeroMemory(output_buffer.get(), new_buffer_size * 2);

    std::size_t const number_of_characters_copied = ::MultiByteToWideChar(code_page, 0, reinterpret_cast<LPCCH>(buffer), static_cast<int>(number_of_bytes), output_buffer.get(), static_cast<int>(new_buffer_size));

    if (number_of_characters_copied > 0)
    {
        s.assign(output_buffer.get());
    }
    else
    {
        s.resize(0);
    }
}

/// This will copy the Unicode string to a UTF-8 string.
inline void copy_to(_In_ std::wstring_view s, _Out_ char * ascii_buffer, _In_ std::size_t const buffer_size) noexcept
{
    WFC_VALIDATE_POINTER(ascii_buffer);

    if (buffer_size == 0 or ascii_buffer == nullptr)
    {
        // No buffer to write to, just give up
        return;
    }

    // NULL terminate the buffer
    ascii_buffer[0] = 0x00;
    ascii_buffer[buffer_size - 1] = 0x00;

    if (buffer_size == 1)
    {
        // No room for a single character, give up
        return;
    }

    // Looks like we may succeed, even for a single character.

    int const size_of_ascii_buffer = static_cast<int>(buffer_size);

    int const number_of_bytes_written = ::WideCharToMultiByte(CP_UTF8,
        0,
        s.data(),
        static_cast<int>(s.length()),
        ascii_buffer,
        static_cast<int>(size_of_ascii_buffer),
        nullptr,
        nullptr);

#if defined( _DEBUG )
    if (number_of_bytes_written == 0)
    {
        DWORD const last_error = ::GetLastError();
        WFCTRACEINIT(L"ss");
        WFCTRACEERROR(last_error);
    }
#endif

    // Make sure we are null terminated
    ascii_buffer[number_of_bytes_written] = 0x00;
}

inline void copy_to(_In_ std::wstring_view s, wchar_t * wide_string, _In_ std::size_t const maximum_length_in_characters_including_null_terminator) noexcept
{
    WFC_VALIDATE_POINTER(wide_string);

    if (maximum_length_in_characters_including_null_terminator == 0)
    {
        // No buffer to write to, just give up
        return;
    }

    (void)wcsncpy_s(wide_string, maximum_length_in_characters_including_null_terminator, s.data(), std::min((maximum_length_in_characters_including_null_terminator - 1), s.length()));
}

inline void trim_right(_Inout_ std::wstring& s, _In_ wchar_t const character) noexcept
{
    if (s.empty() == false)
    {
        while (character == s.at(s.length() - 1))
        {
            s.erase(s.length() - 1, 1);

            if (s.empty() == true)
            {
                return;
            }
        }
    }
}

inline void trim_right(_Inout_ std::string& s, _In_ char const character) noexcept
{
    if (s.empty() == false)
    {
        while (character == s.at(s.length() - 1))
        {
            s.erase(s.length() - 1, 1);

            if (s.empty() == true)
            {
                return;
            }
        }
    }
}

inline void trim_right(_Inout_ std::wstring& the_string) noexcept
{
    if (the_string.empty() == false)
    {
        while (iswspace(the_string.at(the_string.length() - 1)))
        {
            the_string.resize(the_string.length() - 1);

            if (the_string.length() <= 0)
            {
                return;
            }
        }
    }
}

inline void trim_right(_Inout_ std::string& the_string) noexcept
{
    if (the_string.empty() == false)
    {
        while (is_space_character(the_string.at(the_string.length() - 1)))
        {
            the_string.resize(the_string.length() - 1);

            if (the_string.length() <= 0)
            {
                return;
            }
        }
    }
}

inline void trim_left(_Inout_ std::wstring& the_string) noexcept
{
    std::size_t const string_length = the_string.length();

    if (string_length > 0)
    {
        std::size_t number_of_elements_to_erase = 0;

        while (number_of_elements_to_erase < string_length and
            iswspace(the_string.at(number_of_elements_to_erase)))
        {
            number_of_elements_to_erase++;
        }

        if (number_of_elements_to_erase >= string_length)
        {
            the_string.resize(0);
        }
        else
        {
            the_string.erase(0, number_of_elements_to_erase);
        }
    }
}

inline void trim_left(_Inout_ std::string& the_string) noexcept
{
    std::size_t const string_length = the_string.length();

    if (string_length > 0)
    {
        std::size_t number_of_elements_to_erase = 0;

        while (number_of_elements_to_erase < string_length and
            is_space_character(the_string.at(number_of_elements_to_erase)))
        {
            number_of_elements_to_erase++;
        }

        if (number_of_elements_to_erase >= string_length)
        {
            the_string.resize(0);
        }
        else
        {
            the_string.erase(0, number_of_elements_to_erase);
        }
    }
}

inline void trim_left(_Inout_ std::wstring& s, _In_ wchar_t const character) noexcept
{
    std::size_t const string_length = s.length();

    if (string_length > 0)
    {
        std::size_t number_of_elements_to_erase = 0;

        while (number_of_elements_to_erase < string_length and
            character == (s.at(number_of_elements_to_erase)))
        {
            number_of_elements_to_erase++;
        }

        if (number_of_elements_to_erase >= string_length)
        {
            s.resize(0);
        }
        else
        {
            s.erase(0, number_of_elements_to_erase);
        }
    }
}

inline void trim_left(_Inout_ std::string& s, _In_ char const character) noexcept
{
    std::size_t const string_length = s.length();

    if (string_length > 0)
    {
        std::size_t number_of_elements_to_erase = 0;

        while (number_of_elements_to_erase < string_length and
            character == (s.at(number_of_elements_to_erase)))
        {
            number_of_elements_to_erase++;
        }

        if (number_of_elements_to_erase >= string_length)
        {
            s.resize(0);
        }
        else
        {
            s.erase(0, number_of_elements_to_erase);
        }
    }
}

inline void trim(_Inout_ std::wstring& the_string) noexcept
{
    trim_right(the_string);
    trim_left(the_string);
}

inline constexpr _Check_return_ bool is_this_character_ascii(_In_ wchar_t const character) noexcept
{
    if (character == 13 or
        character == 10 or
        character == 9)
    {
        return(true);
    }

    if (character >= 0x20 and character <= 0x7E)
    {
        return(true);
    }

    return(false);
}

inline constexpr _Check_return_ bool is_all_ascii(_In_ std::wstring_view the_string) noexcept
{
    for (auto const& character : the_string)
    {
        if (is_this_character_ascii(character) == false)
        {
            return(false);
        }
    }

    return(true);
}

inline constexpr _Check_return_ bool is_all_ascii(_In_ std::string_view the_string) noexcept
{
    for (auto const character : the_string)
    {
        if (is_this_character_ascii(static_cast<wchar_t>(character)) == false)
        {
            return(false);
        }
    }

    return(true);
}

inline _Check_return_ bool is_all_ascii(_In_ std::vector<uint8_t> const& the_string) noexcept
{
    for (auto const character : the_string)
    {
        if (is_this_character_ascii(static_cast<wchar_t>(character)) == false)
        {
            return(false);
        }
    }

    return(true);
}

inline void trim(_Inout_ std::string& the_string) noexcept
{
    trim_right(the_string);
    trim_left(the_string);
}

inline void from_flags(_Inout_ std::wstring& s, _In_ uint64_t const flags, _In_reads_z_(number_of_names) wchar_t const* const names_of_bits[], _In_ std::size_t const number_of_names) noexcept
{
    s.resize(0);

    for ( auto const bit_number : Range(64) )
    {
        if (bit_test(flags, bit_number) == 1)
        {
            if (bit_number < number_of_names and names_of_bits != nullptr and names_of_bits[bit_number] != nullptr and names_of_bits[bit_number][0] != 0x00)
            {
                s.append(names_of_bits[bit_number]);
            }
            else
            {
                wchar_t temp_string[64];

                int const number_of_characters = swprintf_s(temp_string, std::size(temp_string), L"Undefined(%zd)", bit_number);
                s.append(temp_string, number_of_characters);
            }

            s.push_back(',');
            s.push_back(' ');
        }
    }

    trim(s);
    trim_right(s, ',');
}

inline _Check_return_ int compare_no_case(_In_ std::wstring const& s, _In_opt_z_ wchar_t const* string_p) noexcept
{
    WFC_VALIDATE_POINTER_NULL_OK(string_p);

    if (string_p != nullptr)
    {
        return(_wcsicmp(s.c_str(), string_p));
    }
    else
    {
        // We are greater than nothing (NULL)
        return(I_AM_GREATER_THAN_THAT);
    }
}

inline _Check_return_ int compare_no_case(_In_ std::wstring const& s1, _In_ std::wstring const& s2) noexcept
{
    return(_wcsicmp(s1.c_str(), s2.c_str()));
}

inline bool compare_character_ignore_case(wchar_t const& c1, wchar_t const& c2) noexcept
{
    if (c1 == c2)
        return true;
    else if (std::toupper(c1) == std::toupper(c2))
        return true;
    return false;
}

inline _Check_return_ int compare_no_case(_In_ std::wstring_view s1, _In_ std::wstring_view s2) noexcept
{
    if (s1.length() < s2.length())
    {
        return(I_AM_LESS_THAN_THAT);
    }

    if (s1.length() > s2.length())
    {
        return(I_AM_GREATER_THAN_THAT);
    }

    std::size_t index = 0;

    while (index < s2.length())
    {
        auto const s1_character = std::toupper(s1.at(index));
        auto const s2_character = std::toupper(s2.at(index));

        if (s1_character < s2_character)
        {
            return(I_AM_LESS_THAN_THAT);
        }
        else if (s1_character > s2_character)
        {
            return(I_AM_GREATER_THAN_THAT);
        }

        index++;
    }

    return( I_AM_EQUAL_TO_THAT );
}

inline _Check_return_ int compare_no_case(_In_ std::string const& s, _In_opt_z_ char const* string_p) noexcept
{
    WFC_VALIDATE_POINTER_NULL_OK(string_p);

    if (string_p != nullptr)
    {
        return(_stricmp(s.c_str(), string_p));
    }
    else
    {
        // We are greater than nothing (NULL)
        return(I_AM_GREATER_THAN_THAT);
    }
}

inline _Check_return_ int compare_no_case(_In_ std::string const& s1, _In_ std::string const& s2) noexcept
{
    return(_stricmp(s1.c_str(), s2.c_str()));
}

inline _Check_return_ std::wstring right(_In_ std::wstring const& s, _In_ std::size_t const count) noexcept
{
    if (s.length() > count)
    {
        return(s.substr(s.length() - count));
    }
    else
    {
        std::wstring return_value(s);

        return(return_value);
    }
}

inline _Check_return_ std::wstring right(_In_ std::wstring_view s, _In_ std::size_t const count) noexcept
{
    if (s.length() > count)
    {
        return(std::wstring(s.substr(s.length() - count)));
    }
    else
    {
        std::wstring return_value(s);

        return(return_value);
    }
}

inline _Check_return_ std::string right(_In_ std::string const& s, _In_ std::size_t const count) noexcept
{
    if (s.length() > count)
    {
        return(s.substr(s.length() - count));
    }
    else
    {
        std::string return_value(s);

        return(return_value);
    }
}

inline _Check_return_ std::string right(_In_ std::string_view s, _In_ std::size_t const count) noexcept
{
    if (s.length() > count)
    {
        return(std::string(s.substr(s.length() - count)));
    }
    else
    {
        std::string return_value(s);

        return(return_value);
    }
}

inline _Check_return_ std::wstring left(_In_ std::wstring const& s, _In_ std::size_t count) noexcept
{
    return(s.substr(0, count));
}

inline _Check_return_ std::string left(_In_ std::string const& s, _In_ std::size_t count) noexcept
{
    return(s.substr(0, count));
}

inline _Check_return_ bool ends_with(_In_ std::wstring_view s, _In_ wchar_t const ending) noexcept
{
    std::size_t const string_length = s.length();

    if (string_length < 1)
    {
        return(false);
    }

    wchar_t const character = s.at(string_length - 1);

    return((ending == character) ? true : false);
}

inline _Check_return_ bool ends_with(_In_ std::wstring_view s, _In_ std::wstring_view ending) noexcept
{
    if (s.empty() == true or ending.empty() == true)
    {
        return(false);
    }

    if (ending.length() > s.length())
    {
        return(false);
    }

    auto const our_ending(s.substr(s.length() - ending.length()));

    int const comparison_result = our_ending.compare(ending);

    if (comparison_result == I_AM_EQUAL_TO_THAT)
    {
        return(true);
    }

    return(false);
}

inline _Check_return_ bool ends_with_no_case(_In_ std::wstring_view s, _In_ std::wstring_view ending) noexcept
{
    if (s.empty() == true or ending.empty() == true)
    {
        return(false);
    }

    if (ending.length() > s.length())
    {
        return(false);
    }

    auto const our_ending(s.substr(s.length() -ending.length()));

    int const comparison_result = compare_no_case(our_ending, ending);

    if (comparison_result == I_AM_EQUAL_TO_THAT)
    {
        return(true);
    }

    return(false);
}

inline _Check_return_ bool starts_with(_In_ std::string_view s, _In_ char const beginning) noexcept
{
    if (s.empty() == true)
    {
        return(false);
    }

    auto character = s.at(0);

    return((beginning == character) ? true : false);
}

inline _Check_return_ bool starts_with(_In_ std::string_view s, _In_ std::string_view beginning) noexcept
{
    if (beginning.empty() == true or beginning.length() > s.length())
    {
        return(false);
    }

    for (auto const loop_index : Range(beginning.length()))
    {
        if (s[loop_index] != beginning[loop_index])
        {
            return(false);
        }
    }

    return(true);
}

inline _Check_return_ bool starts_with(_In_ std::string_view s, _In_z_ char const* beginning) noexcept
{
    return(starts_with(s, std::string_view(beginning, strlen(beginning))));
}

inline _Check_return_ bool starts_with(_In_ std::string_view s, _In_ std::string const& beginning) noexcept
{
    return(starts_with(s, beginning));
}

inline _Check_return_ bool starts_with(_In_ std::wstring_view s, _In_ wchar_t const beginning) noexcept
{
    if (s.empty() == true)
    {
        return(false);
    }

    auto character = s.at(0);

    return((beginning == character) ? true : false);
}

inline _Check_return_ bool starts_with(_In_ std::wstring_view s, std::wstring_view beginning ) noexcept
{
    if (beginning.empty() == true or beginning.length() > s.length())
    {
        return(false);
    }

    for (auto const loop_index : Range(beginning.length()))
    {
        if (s[loop_index] != beginning[loop_index])
        {
            return(false);
        }
    }

    return(true);
}

inline _Check_return_ bool starts_with(_In_ std::wstring const& s, _In_z_ wchar_t const* beginning) noexcept
{
    return(starts_with(s, std::wstring_view(beginning, wcslen(beginning))));
}

inline _Check_return_ bool starts_with(_In_ std::wstring const& s, _In_ std::wstring const& beginning) noexcept
{
    return(starts_with(s, beginning));
}

inline _Check_return_ bool starts_with_no_case(_In_ std::wstring const& s, _In_ std::wstring_view beginning) noexcept
{
    if (beginning.empty() == true or beginning.length() > s.length())
    {
        return(false);
    }

    auto our_string = s.c_str();

    for (auto const loop_index : Range(beginning.length()))
    {
        auto const this_character = wfc_to_upper(our_string[loop_index]);
        auto const that_character = wfc_to_upper(beginning[loop_index]);

        if (this_character != that_character)
        {
            return(false);
        }
    }

    return(true);
}

inline _Check_return_ bool starts_with_no_case(_In_ std::string const& s, _In_ std::string_view beginning) noexcept
{
    if (beginning.empty() == true or beginning.length() > s.length())
    {
        return(false);
    }

    auto our_string = s.c_str();

    for (auto const loop_index : Range(beginning.length()))
    {
        auto const this_character = wfc_to_upper(our_string[loop_index]);
        auto const that_character = wfc_to_upper(beginning[loop_index]);

        if (this_character != that_character)
        {
            return(false);
        }
    }

    return(true);
}

inline _Check_return_ bool starts_with_no_case(_In_ std::wstring const& s, _In_z_ wchar_t const* beginning) noexcept
{
    return(starts_with_no_case(s, std::wstring_view(beginning, wcslen(beginning))));
}

inline _Check_return_ bool starts_with_no_case(_In_ std::string const& s, _In_z_ char const* beginning) noexcept
{
    return(starts_with_no_case(s, std::string_view(beginning, strlen(beginning))));
}

inline constexpr _Check_return_ std::size_t number_of_decimal_digits(_In_ std::wstring_view s) noexcept
{
    std::size_t return_value = 0;

    for (auto const this_character : s)
    {
        if (this_character >= '0' and this_character <= '9')
        {
            return_value++;
        }
    }

    return(return_value);
}

inline constexpr _Check_return_ std::size_t number_of_decimal_digits(_In_ std::string_view s) noexcept
{
    std::size_t return_value = 0;

    for (auto const this_character : s)
    {
        if (this_character >= '0' and this_character <= '9')
        {
            return_value++;
        }
    }

    return(return_value);
}

inline constexpr _Check_return_ std::size_t number_of_hex_digits(_In_ std::wstring_view s) noexcept
{
    std::size_t return_value = 0;

    for (auto const this_character : s)
    {
        if (WFC_IS_HEXADECIMAL(this_character))
        {
            return_value++;
        }
    }

    return(return_value);
}

inline constexpr _Check_return_ std::size_t number_of_lower_case_letters_AZ(_In_ std::wstring_view s) noexcept
{
    std::size_t return_value = 0;

    for (auto const this_character : s)
    {
        if (this_character >= 'a' and this_character <= 'z')
        {
            return_value++;
        }
    }

    return(return_value);
}

inline constexpr _Check_return_ std::size_t number_of_upper_case_letters_AZ(_In_ std::wstring_view s) noexcept
{
    std::size_t return_value = 0;

    for (auto const this_character : s)
    {
        if (this_character >= 'A' and this_character <= 'Z')
        {
            return_value++;
        }
    }

    return(return_value);
}

inline _Check_return_ bool is_all_decimal_digits(_In_ std::wstring_view s) noexcept
{
    return((number_of_decimal_digits(s) == s.length()) ? true : false);
}

inline _Check_return_ bool is_all_decimal_digits(_In_ std::string_view s) noexcept
{
    return((number_of_decimal_digits(s) == s.length()) ? true : false);
}

inline _Check_return_ double as_double(_In_z_ char const * const buffer, _In_ std::size_t buffer_size) noexcept
{
    double return_value = 0.0;

    (void)std::from_chars(buffer, &buffer[buffer_size], return_value);

    return(return_value);
}

inline _Check_return_ double as_double(_In_ std::string_view s) noexcept
{
   return(as_double(s.data(), s.length()));
}

inline _Check_return_ double as_double(_In_ std::wstring_view s) noexcept
{
    // See if we can optimize for small strings

    std::size_t const string_length = s.length();

    if (string_length < 26)
    {
        char ascii_string[27]{ 0 }; // Deliberately NOT initializing for speed, we don't need null termination

        for (auto const string_index : Range(string_length))
        {
            ascii_string[string_index] = static_cast<char>(s[string_index]);
        }

        return(as_double(ascii_string, string_length));
    }
    else
    {
        std::string ascii_string;

        ascii_string.reserve(string_length);

        for (auto const string_index : Range(string_length))
        {
            ascii_string.push_back(static_cast<char>(s[string_index]));
        }

        return(as_double(ascii_string.c_str(), ascii_string.length()));
    }
}

inline _Check_return_ int64_t ascii_string_to_integer( _In_ char const * ascii_string, _In_ std::size_t string_length, _In_ int radix ) noexcept
{
    int64_t return_value = 0;

    (void) std::from_chars(ascii_string, &ascii_string[string_length], return_value, radix);

    return(return_value);
}

inline _Check_return_ int64_t ascii_string_to_integer(_In_ std::string_view ascii_string) noexcept
{
    return(ascii_string_to_integer(ascii_string.data(), ascii_string.length(), 10));
}

inline _Check_return_ uint64_t ascii_string_to_unsigned_integer(_In_ char const * ascii_string, _In_ std::size_t string_length, _In_ int radix) noexcept
{
    uint64_t return_value = 0;

    (void)std::from_chars(ascii_string, &ascii_string[string_length], return_value, radix);

    return(return_value);
}

inline _Check_return_ uint64_t ascii_string_to_unsigned_integer(_In_ std::string_view ascii_string) noexcept
{
    return(ascii_string_to_unsigned_integer(ascii_string.data(), ascii_string.length(), 10));
}

inline _Check_return_ int64_t as_integer(_In_z_ char const * const buffer, _In_ std::size_t const buffer_size) noexcept
{
    int radix = 10;

    std::size_t offset_of_first_digit = 0;

    while (is_space_character(buffer[offset_of_first_digit]) == true)
    {
        offset_of_first_digit++;
    }

    if (offset_of_first_digit >= buffer_size)
    {
        return(0);
    }

    if ((buffer_size - offset_of_first_digit) > 1)
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

    return(ascii_string_to_integer(&buffer[offset_of_first_digit], buffer_size - offset_of_first_digit, radix));
}

inline _Check_return_ uint64_t as_unsigned_integer(_In_z_ char const* const buffer, _In_ std::size_t const buffer_size) noexcept
{
    int radix = 10;

    std::size_t offset_of_first_digit = 0;

    while (is_space_character(buffer[offset_of_first_digit]) == true)
    {
        offset_of_first_digit++;
    }

    if (offset_of_first_digit >= buffer_size)
    {
        return(0);
    }

    if ((buffer_size - offset_of_first_digit) > 1)
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

    return(ascii_string_to_unsigned_integer(&buffer[offset_of_first_digit], buffer_size - offset_of_first_digit, radix));
}

inline _Check_return_ int64_t as_integer(_In_ std::string_view s) noexcept
{
    return(as_integer(s.data(), s.length()));
}

inline _Check_return_ int64_t as_unsigned_integer(_In_ std::string_view s) noexcept
{
    return(as_unsigned_integer(s.data(), s.length()));
}

inline _Check_return_ int64_t as_integer(_In_ std::wstring_view s) noexcept
{
    // See if we can optimize for small strings

    std::size_t const string_length = s.length();

    if (string_length < 26)
    {
        char ascii_string[27]{ 0 }; // Deliberately NOT initializing for speed, we don't need null termination

        for (auto const string_index : Range(string_length))
        {
            ascii_string[string_index] = static_cast<char>(s[string_index]);
        }

        return(as_integer(ascii_string, string_length));
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

    wchar_t * unused = nullptr;

    int64_t const return_value = _wcstoi64(value.c_str(), &unused, radix);

    return(return_value);
}

inline _Check_return_ uint64_t as_unsigned_integer(_In_ std::wstring_view s) noexcept
{
    // See if we can optimize for small strings

    std::size_t const string_length = s.length();

    if (string_length < 26)
    {
        char ascii_string[27]{ 0 }; // Deliberately NOT initializing for speed, we don't need null termination

        for (auto const string_index : Range(string_length))
        {
            ascii_string[string_index] = static_cast<char>(s[string_index]);
        }

        return(as_unsigned_integer(ascii_string, string_length));
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

    wchar_t * unused = nullptr;

    uint64_t const return_value = _wcstoui64(value.c_str(), &unused, radix);

    return(return_value);
}

inline _Check_return_ int64_t as_hexadecimal_integer(_In_ std::wstring const& s) noexcept
{
    wchar_t* unused = nullptr;

    int64_t const return_value = _wcstoi64(s.c_str(), &unused, 16);

    return(return_value);
}

inline _Check_return_ std::size_t insert(_Inout_ std::wstring& s, _In_ std::size_t const string_index, _In_ wchar_t const character_to_insert) noexcept
{
    if (string_index >= s.length())
    {
        s.push_back(character_to_insert);
    }
    else
    {
        s.insert(string_index, 1, character_to_insert);
    }

    return(s.length());
}

inline _Check_return_ std::size_t insert(_Inout_ std::wstring& s, _In_ std::size_t string_index, _In_z_ wchar_t const* string_to_insert) noexcept
{
    WFC_VALIDATE_POINTER_NULL_OK(string_to_insert);

    if (string_to_insert != nullptr)
    {
        if (string_index < 0)
        {
            string_index = 0;
        }

        if (string_index >= s.length())
        {
            s.append(string_to_insert);
        }
        else
        {
            s.insert(string_index, string_to_insert);
        }
    }

    return(s.length());
}

/// <summary>Returns a string skipping the first 'count' characters specified.</summary>
inline _Check_return_ std::wstring skip_first(_In_ std::wstring const& s, _In_ std::size_t const count) noexcept
{
    std::size_t const string_length = s.length();

    if (count == 0 or count > string_length)
    {
        std::wstring return_value(s);

        return(return_value);
    }

    return(right(s, string_length - count));
}

/// <summary>Returns a string skipping the first 'count' characters specified.</summary>
inline _Check_return_ std::string skip_first(_In_ std::string const& s, _In_ std::size_t const count) noexcept
{
    std::size_t const string_length = s.length();

    if (count == 0 or count > string_length)
    {
        std::string return_value(s);

        return(return_value);
    }

    return(right(s, string_length - count));
}

inline void append(_Inout_ std::wstring& s, _In_ uint64_t const value) noexcept
{
    wchar_t number_string[64];

    _ui64tow_s(value, number_string, std::size(number_string), 10);

    s.append(number_string);
}

inline void append(_Inout_ std::wstring& s, _In_ int64_t const value) noexcept
{
    wchar_t number_string[64];

    _i64tow_s(value, number_string, std::size(number_string), 10);

    s.append(number_string);
}

inline void format_number(_Inout_ std::wstring& s, _In_ uint64_t const source) noexcept
{
    wchar_t number_string[64];

    swprintf_s(number_string, std::size(number_string), L"%I64u", source);

    wchar_t formatted_string[65];

    formatted_string[0] = 0x00;
    formatted_string[64] = 0x00;

    NUMBERFMT number_format;

    ZeroMemory(&number_format, sizeof(number_format));

    number_format.Grouping = 3;
    number_format.NegativeOrder = 1;

    wchar_t decimal_separator[16];

    decimal_separator[0] = 0x00;
    decimal_separator[15] = 0x00;

    ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_SDECIMAL, decimal_separator, static_cast<int>(std::size(decimal_separator)));

    number_format.lpDecimalSep = decimal_separator;

    wchar_t thousands_separator[16];

    thousands_separator[0] = 0x00;
    thousands_separator[15] = 0x00;

    ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_STHOUSAND, thousands_separator, static_cast<int>(std::size(thousands_separator)));

    number_format.lpThousandSep = thousands_separator;

    ::GetNumberFormatEx(LOCALE_NAME_USER_DEFAULT,
        0,
        number_string,
        &number_format,
        formatted_string,
        static_cast<int>(std::size(formatted_string) - 1));

    s.assign(formatted_string);
}

inline void format_number(_Inout_ std::wstring& s, _In_ int64_t const source) noexcept
{
    wchar_t number_string[64];

    swprintf_s(number_string, std::size(number_string), L"%I64d", source);

    wchar_t formatted_string[65];

    formatted_string[0] = 0x00;
    formatted_string[64] = 0x00;

    NUMBERFMT number_format;

    ZeroMemory(&number_format, sizeof(number_format));

    number_format.Grouping = 3;
    number_format.NegativeOrder = 1;

    wchar_t decimal_separator[16];

    decimal_separator[0] = 0x00;
    decimal_separator[15] = 0x00;

    ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_SDECIMAL, decimal_separator, static_cast<int>(std::size(decimal_separator)));

    number_format.lpDecimalSep = decimal_separator;

    wchar_t thousands_separator[16];

    thousands_separator[0] = 0x00;
    thousands_separator[15] = 0x00;

    ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_STHOUSAND, thousands_separator, static_cast<int>(std::size(thousands_separator)));

    number_format.lpThousandSep = thousands_separator;

    ::GetNumberFormatEx(LOCALE_NAME_USER_DEFAULT,
        0,
        number_string,
        &number_format,
        formatted_string,
        static_cast<int>(std::size(formatted_string) - 1));

    s.assign(formatted_string);
}

inline void format_number(_Inout_ std::wstring& s, _In_ double const source, _In_ std::size_t const number_of_decimal_places) noexcept
{
    wchar_t number_string[64];

    swprintf_s(number_string, std::size(number_string), L"%f", source);

    wchar_t formatted_string[65];

    formatted_string[0] = 0x00;
    formatted_string[64] = 0x00;

    NUMBERFMT number_format;

    ZeroMemory(&number_format, sizeof(number_format));

    number_format.NumDigits = static_cast<UINT>(number_of_decimal_places);
    number_format.Grouping = 3;
    number_format.NegativeOrder = 1;

    wchar_t decimal_separator[16];

    decimal_separator[0] = 0x00;
    decimal_separator[15] = 0x00;

    ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_SDECIMAL, decimal_separator, static_cast<int>(std::size(decimal_separator)));

    number_format.lpDecimalSep = decimal_separator;

    wchar_t thousands_separator[16];

    thousands_separator[0] = 0x00;
    thousands_separator[15] = 0x00;

    ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_STHOUSAND, thousands_separator, static_cast<int>(std::size(thousands_separator)));

    number_format.lpThousandSep = thousands_separator;

    ::GetNumberFormatEx(LOCALE_NAME_USER_DEFAULT,
        0,
        number_string,
        &number_format,
        formatted_string,
        static_cast<int>(std::size(formatted_string) - 1));

    s.assign(formatted_string);
}

inline void friendly_byte_count(_Inout_ std::wstring& s, _In_ uint64_t const byte_count) noexcept
{
    if (byte_count < ONE_KILOBYTE)
    {
        format_number(s, byte_count);
        s.append(WSTRING_VIEW(L" bytes"));
    }
    else if (byte_count < ONE_MEGABYTE)
    {
        double const units = (double)((double)byte_count / (double)ONE_KILOBYTE);

        format(s, L"%.02lfKB", units);
    }
    else if (byte_count < ONE_GIGABYTE)
    {
        double const units = (double)((double)byte_count / (double)ONE_MEGABYTE);

        format(s, L"%.02lfMB", units);
    }
    else if (byte_count < ONE_TERABYTE)
    {
        double const units = (double)((double)byte_count / (double)ONE_GIGABYTE);

        format(s, L"%.02lfGB", units);
    }
    else if (byte_count < ONE_PETABYTE)
    {
        double const units = (double)((double)byte_count / (double)ONE_TERABYTE);

        format(s, L"%.02lfTB", units);
    }
    else if (byte_count < ONE_EXABYTE)
    {
        double const units = (double)((double)byte_count / (double)ONE_PETABYTE);

        format(s, L"%.02lfPB", units);
    }
    else
    {
        double const units = (double)((double)byte_count / (double)ONE_EXABYTE);

        format(s, L"%.02lfEB", units);
    }
}

inline void despace(_Inout_ std::wstring& s) noexcept
{
    // First see if we need to do anything

    std::size_t number_of_characters = s.length();

    if (number_of_characters < 1)
    {
        return;
    }

    bool needs_despacing = false;

    wchar_t character = s[0];

    if (character == 0x20 or
        character == TAB_CHARACTER or
        character == CARRIAGE_RETURN or
        character == LINE_FEED)
    {
        needs_despacing = true;
    }
    else
    {
        character = s[number_of_characters - 1];

        if (character == 0x20 or
            character == TAB_CHARACTER or
            character == CARRIAGE_RETURN or
            character == LINE_FEED)
        {
            needs_despacing = true;
        }
        else
        {
            std::size_t string_index = 1;

            while (needs_despacing == false and string_index < (number_of_characters - 1))
            {
                character = s[string_index];

                if (character == 0x20 or
                    character == TAB_CHARACTER or
                    character == CARRIAGE_RETURN or
                    character == LINE_FEED)
                {
                    string_index++;

                    character = s[string_index];

                    if (character == 0x20 or
                        character == TAB_CHARACTER or
                        character == CARRIAGE_RETURN or
                        character == LINE_FEED)
                    {
                        needs_despacing = true;
                    }
                }

                string_index++;
            }
        }
    }

    if (needs_despacing == false)
    {
        return;
    }

    // Well finally, we have something to do.

    trim(s); // Get rid of the easy case

    number_of_characters = s.length();

    bool last_character_added_was_a_space = false;

    std::wstring other_string;

    for (auto const string_index : Range(number_of_characters))
    {
        character = s[string_index];

        if (character == 0x20 or
            character == TAB_CHARACTER or
            character == CARRIAGE_RETURN or
            character == LINE_FEED)
        {
            if (last_character_added_was_a_space == false)
            {
                other_string.push_back(character);
                last_character_added_was_a_space = true;
            }
        }
        else
        {
            other_string.push_back(character);
            last_character_added_was_a_space = false;
        }
    }

    s = other_string;
}

inline _Check_return_ std::size_t replace(_Inout_ std::wstring& s, _In_ wchar_t const old_character, _In_ wchar_t const new_character) noexcept
{
    std::size_t number_of_replacements = 0;

    for (auto const string_index : Range(s.length()))
    {
        if (s[string_index] == old_character)
        {
            s[string_index] = new_character;
            number_of_replacements++;
        }
    }

    return(number_of_replacements);
}

inline _Check_return_ bool trim_quotes(_Inout_ std::wstring& s) noexcept
{
    if (s.length() > 1)
    {
        wchar_t const first_character = s[0];
        wchar_t const last_character = s[s.length() - 1];

        if (first_character == last_character)
        {
            // The string starts and ends with the same character.

            if (first_character == '\"')
            {
                trim_right(s, '\"');
                trim_left(s, '\"');
                trim(s);
                return(true);
            }

            if (first_character == '\'')
            {
                trim_right(s, '\'');
                trim_left(s, '\'');
                trim(s);
                return(true);
            }
        }

        return(false); // Nothing was trimmed
    }

    return(false); // Nothing was trimmed
}

inline void trim_all_quotes(_Inout_ std::wstring& s) noexcept
{
    while (trim_quotes(s) == true)
    {
        ;
    }
}

inline constexpr _Check_return_ std::size_t number_of(_In_ std::wstring_view s, _In_ wchar_t const character) noexcept
{
    std::size_t return_value = 0;

    for (auto const this_character : s)
    {
        if (this_character == character)
        {
            return_value++;
        }
    }

    return(return_value);
}

inline constexpr _Check_return_ bool is_all_hex_digits(_In_ std::wstring_view s) noexcept
{
    for (wchar_t const character : s)
    {
        if (!WFC_IS_HEXADECIMAL(character))
        {
            return(false);
        }
    }

    return(true);
}

inline _Check_return_ bool is_all_space(_In_ std::wstring_view s) noexcept
{
    for (auto const character : s)
    {
        if (iswspace(character) == 0)
        {
            return(false);
        }
    }

    return(true);
}

inline constexpr _Check_return_ bool is_space(_In_ std::wstring_view s, _In_ std::size_t const character_index) noexcept
{
    if (s.empty() == true or character_index >= s.length())
    {
        return(false);
    }

    wchar_t const character_to_test = get_at(s, character_index);

    if (character_to_test == 0x20 or
        character_to_test == TAB_CHARACTER or
        character_to_test == CARRIAGE_RETURN or
        character_to_test == LINE_FEED)
    {
        return(true);
    }

    return(false);
}

inline void copy_utf8(_Inout_ std::wstring& s, _In_opt_z_ char const* ascii_string_p) noexcept
{
    if (ascii_string_p == nullptr or ascii_string_p[0] == 0x00)
    {
        s.resize(0);
        return;
    }

    // WARNING!!!! The C++ way will cause your program to blow up with an exception
    //std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_converter;
    //s = utf8_converter.from_bytes(ascii_string_p);

    copy(s, CP_UTF8, reinterpret_cast<uint8_t const*>(ascii_string_p), strlen(ascii_string_p));
}

inline void copy_utf8(_Inout_ std::wstring& s, _In_ std::vector<uint8_t> const& bytes) noexcept
{
    if (bytes.empty() == true)
    {
        s.resize(0);
        return;
    }

    copy(s, CP_UTF8, bytes.data(), bytes.size());
}

inline void copy_utf8(_Inout_ std::wstring& s, _In_ uint8_t const* bytes, _In_ std::size_t const number_of_bytes) noexcept
{
    if (bytes == nullptr or number_of_bytes < 1)
    {
        s.resize(0);
        return;
    }

    copy(s, CP_UTF8, bytes, number_of_bytes);
}

inline _Check_return_ std::string to_utf8(_In_ std::wstring const& s) noexcept
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_converter;
    return utf8_converter.to_bytes(s);
}

inline void split(_In_z_ wchar_t const* source, _In_z_ wchar_t const* terminator, _Inout_ std::vector<std::wstring>& string_array) noexcept
{
    string_array.clear();

    if (source == nullptr or source[0] == 0x00 or
        terminator == nullptr or terminator[0] == 0x00)
    {
        return;
    }

    std::size_t const terminator_length = wcslen(terminator);

    int source_index = 0;

    std::wstring string_to_add;

    while (source[source_index] != 0x00)
    {
        if (_wcsnicmp(&source[source_index], terminator, terminator_length) == 0)
        {
            string_array.push_back(string_to_add);
            string_to_add.clear();
            source_index += static_cast<int>(terminator_length - 1);
        }
        else
        {
            string_to_add.push_back(source[source_index]);
        }

        source_index++;
    }

    string_array.push_back(string_to_add);
}

inline void split(_In_z_ char const* source, _In_z_ char const* terminator, _Inout_ std::vector<std::string>& string_array) noexcept
{
    string_array.clear();

    if (source == nullptr or source[0] == 0x00 or
        terminator == nullptr or terminator[0] == 0x00)
    {
        return;
    }

    std::size_t const terminator_length = strlen(terminator);

    int source_index = 0;

    std::string string_to_add;

    while (source[source_index] != 0x00)
    {
        if (_strnicmp(&source[source_index], terminator, terminator_length) == 0)
        {
            string_array.push_back(string_to_add);
            string_to_add.clear();
            source_index += static_cast<int>(terminator_length - 1);
        }
        else
        {
            string_to_add.push_back(source[source_index]);
        }

        source_index++;
    }

    string_array.push_back(string_to_add);
}

inline void split(_In_z_ char const* source, _In_ std::size_t const length, char const character, _Inout_ std::vector<std::string>& string_array) noexcept
{
    string_array.clear();

    if (source == nullptr or source[0] == 0x00 or character == 0x00)
    {
        return;
    }

    std::string string_to_add;

    for (auto const string_index : Range(length))
    {
        if (source[string_index] == character)
        {
            string_array.push_back(string_to_add);
            string_to_add.clear();
        }
        else
        {
            string_to_add.push_back(source[string_index]);
        }
    }

    string_array.push_back(string_to_add);
}

inline void split(_In_z_ char const* source, _In_ char const character, _Inout_ std::vector<std::string>& string_array) noexcept
{
    string_array.clear();

    if (source == nullptr or source[0] == 0x00 or character == 0x00)
    {
        return;
    }

    split(source, strlen(source), character, string_array);
}

inline void split(_In_ std::string_view source, char const separation_character, _Inout_ std::vector<std::string_view>& string_array) noexcept
{
    string_array.clear();

    if (source.empty() == true || separation_character == 0x00)
    {
        return;
    }

    char const* here = nullptr;
    std::size_t length = 0;

    for (auto const source_index : Range(source.length()))
    {
        if (source[source_index] == separation_character)
        {
            string_array.push_back(std::string_view(here, length));
            here = nullptr;
            length = 0;
        }
        else
        {
            if (here == nullptr)
            {
                here = &source[source_index];
            }

            length++;
        }
    }

    if (length > 0 && here != nullptr)
    {
        string_array.push_back(std::string_view(here, length));
    }
}

inline void split(_In_z_ wchar_t const* source, _In_ std::size_t const length, _In_ wchar_t const character, _Inout_ std::vector<std::wstring>& string_array) noexcept
{
    string_array.clear();

    if (source == nullptr or source[0] == 0x00 or character == 0x00)
    {
        return;
    }

    std::wstring string_to_add;

    for (auto const string_index : Range(length))
    {
        if (source[string_index] == character)
        {
            string_array.push_back(string_to_add);
            string_to_add.clear();
        }
        else
        {
            string_to_add.push_back(source[string_index]);
        }
    }

    string_array.push_back(string_to_add);
}

inline void split(_In_z_ wchar_t const* source, _In_ wchar_t const character, _Inout_ std::vector<std::wstring>& string_array) noexcept
{
    string_array.clear();

    if (source == nullptr or source[0] == 0x00 or character == 0x00)
    {
        return;
    }

    split(source, wcslen(source), character, string_array);
}

inline void split(_In_ std::wstring_view source, _In_ wchar_t const character, _Inout_ std::vector<std::wstring>& string_array) noexcept
{
    split(source.data(), source.length(), character, string_array);
}

inline void split(_In_ std::string_view source, _In_ char const character, _Inout_ std::vector<std::string>& string_array) noexcept
{
    split(source.data(), source.length(), character, string_array);
}

inline void split_and_trim(_In_ std::wstring_view source, _In_ wchar_t const character, _Inout_ std::vector<std::wstring>& string_array) noexcept
{
    string_array.clear();

    if (source.empty() == true)
    {
        return;
    }

    std::wstring string_to_add;

    for (auto const source_character : source)
    {
        if (source_character == character)
        {
            trim(string_to_add);

            if (string_to_add.empty() == false)
            {
                string_array.push_back(string_to_add);
                string_to_add.clear();
            }
        }
        else
        {
            string_to_add.push_back(source_character);
        }
    }

    trim(string_to_add);

    if (string_to_add.empty() == false)
    {
        string_array.push_back(string_to_add);
    }
}

inline _Check_return_ int delete_characters(_Inout_ std::wstring& s, _In_ int const index, _In_ int number_of_characters_to_delete = 1) noexcept
{
    if (index < 0 or number_of_characters_to_delete < 1)
    {
        return(static_cast<int>(s.length()));
    }

    int const number_of_characters_remaining = static_cast<int>(static_cast<int>(s.length()) - index);

    if (number_of_characters_to_delete > number_of_characters_remaining)
    {
        number_of_characters_to_delete = number_of_characters_remaining;
    }

    s = s.erase(index, number_of_characters_to_delete);

    return(static_cast<int>(s.length()));
}

using WSTRING_CONST_ITERATOR = std::vector<std::wstring>::const_iterator;
using WSTRING_CONST_ITERATOR_PAIR = std::pair<WSTRING_CONST_ITERATOR, WSTRING_CONST_ITERATOR>;

static inline constexpr _Check_return_ bool it_wasnt_found(_In_ WSTRING_CONST_ITERATOR_PAIR const& p) noexcept
{
    return((p.first == p.second) ? true : false);
}

static inline constexpr _Check_return_ bool it_was_found(_In_ WSTRING_CONST_ITERATOR_PAIR const& p) noexcept
{
    return((p.first != p.second) ? true : false);
}

inline _Check_return_ SSIZE_T add_to_unique_sorted_vector(_In_ std::wstring_view value_to_add, _Inout_ std::vector<std::wstring>& values) noexcept
{
    auto const p = std::equal_range(std::cbegin(values), std::cend(values), value_to_add);

    if (it_was_found(p) == false)
    {
        SSIZE_T const return_value = std::distance(std::cbegin(values), p.second);

        values.emplace(p.second, value_to_add);

        // Return the index where we inserted
        return(return_value);
    }

    // Return the index of the existing entry
    return(std::distance(std::cbegin(values), p.first));
}

inline _Check_return_ SSIZE_T add_to_unique_sorted_vector(_In_ std::wstring const& value_to_add, _Inout_ std::vector<std::wstring>& values) noexcept
{
    auto const p = std::equal_range(std::cbegin(values), std::cend(values), value_to_add);

    if (it_was_found(p) == false)
    {
        SSIZE_T const return_value = std::distance(std::cbegin(values), p.second);

        values.emplace(p.second, value_to_add);

        // Return the index where we inserted
        return(return_value);
    }

    // Return the index of the existing entry
    return(std::distance(std::cbegin(values), p.first));
}

using STRING_CONST_ITERATOR = std::vector<std::string>::const_iterator;
using STRING_CONST_ITERATOR_PAIR = std::pair<STRING_CONST_ITERATOR, STRING_CONST_ITERATOR>;

static inline constexpr _Check_return_ bool it_wasnt_found(_In_ STRING_CONST_ITERATOR_PAIR const& p) noexcept
{
    return((p.first == p.second) ? true : false);
}

static inline constexpr _Check_return_ bool it_was_found(_In_ STRING_CONST_ITERATOR_PAIR const& p) noexcept
{
    return((p.first != p.second) ? true : false);
}

inline _Check_return_ SSIZE_T add_to_unique_sorted_vector(_In_ std::string const& value_to_add, _Inout_ std::vector<std::string>& values) noexcept
{
    auto const p = std::equal_range(std::cbegin(values), std::cend(values), value_to_add);

    if (it_was_found(p) == false)
    {
        SSIZE_T const return_value = std::distance(std::cbegin(values), p.second);

        values.emplace(p.second, value_to_add);

        // Return the index where we inserted
        return(return_value);
    }

    // Return the index of the existing entry
    return(std::distance(std::cbegin(values), p.first));
}

inline void replace(_Inout_ std::wstring& s, _In_ std::wstring_view what_to_replace, _In_ std::wstring_view replace_it_with) noexcept
{
    std::wstring translated_string;

    std::size_t location = s.find(what_to_replace);

    while (location != std::wstring::npos)
    {
        // Append the data up to the what_to_replace to our translated string

        translated_string.append(s.substr(0, location));
        translated_string.append(replace_it_with);

        (void) s.erase(0, location + what_to_replace.length());

        location = s.find(what_to_replace);
    }

    translated_string.append(s);
    s = translated_string;
}

inline void replace(_Inout_ std::string& s, _In_ std::string_view what_to_replace, _In_ std::string_view replace_it_with) noexcept
{
    std::string translated_string;

    std::size_t location = s.find(what_to_replace);

    while (location != std::string::npos)
    {
        // Append the data up to the what_to_replace to our translated string

        translated_string.append(s.substr(0, location));
        translated_string.append(replace_it_with);

        (void) s.erase(0, location + what_to_replace.length());

        location = s.find(what_to_replace);
    }

    translated_string.append(s);
    s = translated_string;
}

inline _Check_return_ bool contains_no_case(_In_ std::vector<std::wstring> const& s, _In_ std::wstring_view the_string) noexcept
{
    if (s.empty() == true or the_string.empty() == true)
    {
        return(false);
    }

    for (auto const& entry : s)
    {
        if (compare_no_case(entry, the_string) == I_AM_EQUAL_TO_THAT)
        {
            return(true);
        }
    }

    return(false);
}

inline _Check_return_ bool contains(_In_ std::vector<std::wstring> const& s, _In_ std::wstring_view the_string) noexcept
{
    if (s.empty() == true or the_string.empty() == true)
    {
        return(false);
    }

    for (auto const& entry : s)
    {
        if (entry.compare(the_string) == I_AM_EQUAL_TO_THAT)
        {
            return(true);
        }
    }

    return(false);
}

inline _Check_return_ bool contains_no_case(_In_ std::vector<std::string> const& s, _In_ std::string the_string) noexcept
{
    if (s.empty() == true or the_string.empty() == true)
    {
        return(false);
    }

    for (auto const& entry : s)
    {
        if (compare_no_case(entry, the_string) == I_AM_EQUAL_TO_THAT)
        {
            return(true);
        }
    }

    return(false);
}

inline _Check_return_ bool compare_strings_ignoring_case(_In_ std::string const& left, _In_ std::string const& right) noexcept
{
    return(compare_no_case(left, right) < I_AM_EQUAL_TO_THAT);
}

inline _Check_return_ SSIZE_T add_to_unique_sorted_vector_ignore_case(_In_ std::string const& value_to_add, _Inout_ std::vector<std::string>& values) noexcept
{
    auto const p = std::equal_range(std::cbegin(values), std::cend(values), value_to_add, compare_strings_ignoring_case);

    if (it_was_found(p) == false)
    {
        SSIZE_T const return_value = std::distance(std::cbegin(values), p.second);

        values.emplace(p.second, value_to_add);

        // Return the index where we inserted
        return(return_value);
    }

    // Return the index of the existing entry
    return(std::distance(std::cbegin(values), p.first));
}

inline _Check_return_ bool contains_no_case_sorted(_In_ std::vector<std::string> const& s, _In_z_ char const* the_string) noexcept
{
    if (s.empty() == true or the_string == nullptr)
    {
        return(false);
    }

    std::string const right_hand_side(the_string);

    auto const p = std::equal_range(std::cbegin(s), std::cend(s), right_hand_side, compare_strings_ignoring_case);

    return(it_was_found(p));
}

inline _Check_return_ bool contains(_In_ std::vector<std::string> const& s, _In_z_ char const* the_string) noexcept
{
    if (s.empty() == true or the_string == nullptr)
    {
        return(false);
    }

    for (auto const& entry : s)
    {
        if (entry.compare(the_string) == I_AM_EQUAL_TO_THAT)
        {
            return(true);
        }
    }

    return(false);
}

inline _Check_return_ bool add_unique(_Inout_ std::vector<std::wstring>& s, _In_ std::wstring_view new_element) noexcept
{
    if (contains_no_case(s, new_element) == true)
    {
        return(false);
    }

    s.push_back(std::wstring(new_element));
    return(true);
}

inline void add_unique(_Inout_ std::vector<std::wstring>& destination, _In_ std::vector<std::wstring> const& source) noexcept
{
    for (auto const& new_string : source)
    {
        (void)add_unique(destination, new_string);
    }
}

inline void join(_In_ std::vector<std::wstring> const& s, _In_ std::wstring_view divider, _Inout_ std::wstring& result) noexcept
{
    result.clear();

    std::size_t const number_of_elements = s.size();

    if (number_of_elements == 0)
    {
        return;
    }

    if (number_of_elements == 1)
    {
        result.assign(s.at(0));
        return;
    }

    std::size_t array_index = 0;

    while (array_index < (number_of_elements - 1))
    {
        result.append(s.at(array_index));

        if (divider.empty() == false)
        {
            result.append(divider);
        }

        array_index++;
    }

    // Add the last entry without a divider.
    result.append(s.at(array_index));
}

inline void join(_In_ std::vector<std::string> const& s, _In_ std::string_view delimiter, _Inout_ std::string& result) noexcept
{
    result.clear();

    std::size_t const number_of_elements = s.size();

    if (number_of_elements == 0)
    {
        return;
    }

    if (number_of_elements == 1)
    {
        result.assign(s.at(0));
        return;
    }

    std::size_t array_index = 0;

    while (array_index < (number_of_elements - 1))
    {
        result.append(s.at(array_index));

        if (delimiter.empty() == false)
        {
            result.append(delimiter);
        }

        array_index++;
    }

    // Add the last entry without a divider.
    result.append(s.at(array_index));
}

inline void remove_at(_Inout_ std::vector<uint8_t>& s, _In_ std::size_t const array_index, _In_ std::size_t const number_of_elements_to_remove = 1) noexcept
{
    if (s.size() == 0)
    {
        return;
    }

    s.erase(std::begin(s) + array_index, std::begin(s) + array_index + number_of_elements_to_remove);
}

inline void remove_no_case(std::vector<std::wstring>& s, _In_ std::wstring_view entry) noexcept
{
    if (s.empty() == true)
    {
        return;
    }

    std::size_t loop_index = 0;

    while (loop_index < s.size())
    {
        if (compare_no_case(s.at(loop_index), entry) == I_AM_EQUAL_TO_THAT)
        {
            s.erase(std::begin(s) + loop_index, std::begin(s) + loop_index + 1);
            loop_index--;
        }

        loop_index++;
    }
}

inline void insert_at(_Inout_ std::vector<std::wstring>& s, _In_ std::size_t const array_index, _In_ std::wstring_view new_element, _In_ std::size_t const number_of_times_to_insert_it = 1) noexcept
{
    if (number_of_times_to_insert_it < 1 or new_element.empty() == true)
    {
        return;
    }

    if (s.empty() == true)
    {
        s.reserve(number_of_times_to_insert_it);

        for (auto const loop_index : Range(number_of_times_to_insert_it))
        {
            s.push_back(std::wstring(new_element));
        }

        return;
    }

    if (array_index > (s.size() - 1))
    {
        s.reserve(s.size() + number_of_times_to_insert_it);

        for (auto const loop_index : Range(number_of_times_to_insert_it))
        {
            s.push_back(std::wstring(new_element));
        }

        return;
    }

    s.insert(std::begin(s) + array_index, number_of_times_to_insert_it, std::wstring(new_element));
}

inline void copy_beginning_at(_Inout_ std::vector<std::wstring>& destination, _In_ std::vector<std::wstring> const& source, _In_ std::size_t const starting_index) noexcept
{
    destination.clear();

    for ( auto const index : Range(source.size(), starting_index) )
    {
        destination.push_back(source[index]);
    }
}

inline void append_to_bytes(_In_ std::wstring_view source, _Inout_ std::vector<uint8_t>& bytes) noexcept
{
    std::size_t const number_of_bytes_in_string = source.length() * sizeof(wchar_t);
    std::size_t const original_array_size = bytes.size();

    bytes.resize(number_of_bytes_in_string + original_array_size);

    auto buffer = bytes.data();

    CopyMemory(&buffer[original_array_size], source.data(), number_of_bytes_in_string);
}

inline _Check_return_ bool constexpr is_null_or_empty(_In_opt_z_ wchar_t const * s) noexcept
{
    return (s == nullptr or s[0] == 0x00) ? true : false;
}

inline void append(_Inout_ std::wstring& s, _In_ int const integer) noexcept
{
    wchar_t integer_string[40]{ 0 };

    _itow_s(integer, integer_string, std::size(integer_string), 10);

    s.append(integer_string);
}

inline void append(_Inout_ std::wstring& s, _In_ long const integer) noexcept
{
    wchar_t integer_string[40]{ 0 };

    _ltow_s(integer, integer_string, std::size(integer_string), 10);

    s.append(integer_string);
}

inline void append(_Inout_ std::wstring& s, _In_ unsigned long const integer) noexcept
{
    wchar_t integer_string[40]{ 0 };

    _ultow_s(integer, integer_string, std::size(integer_string), 10);

    s.append(integer_string);
}

inline _Check_return_ wchar_t last_character(_In_ std::wstring const& s) noexcept
{
    if (s.empty() == false)
    {
        return(s.at(s.length() - 1));
    }

    return(0);
}

inline void append_integer(_Inout_ std::wstring& s, _In_ uint64_t const value) noexcept
{
    wchar_t number_string[65]{ 0 };
    (void)_ui64tow_s(value, number_string, std::size(number_string), 10);

    s.append(number_string);
}

inline void append_integer(_Inout_ std::wstring& s, _In_ int64_t const value) noexcept
{
    wchar_t number_string[65];
    (void)_i64tow_s(value, number_string, std::size(number_string), 10);

    s.append(number_string);
}

inline _Check_return_ std::wstring get_last(_In_ std::vector<std::wstring> const& s) noexcept
{
    std::wstring return_value;

    std::size_t const count = s.size();

    if (count > 0)
    {
        return_value.assign(s.at(count - 1));
    }

    return (return_value);
}

inline void split_path(_In_ std::wstring const& source, _Inout_ std::vector<std::wstring>& pieces) noexcept
{
    pieces.clear();

    if (source.empty() == true)
    {
        return;
    }

    int source_index = 0;

    std::wstring string_to_add;

    while (source[source_index] != 0x00)
    {
        if (source[source_index] == '/' or
            source[source_index] == '\\')
        {
            if (string_to_add.empty() == false)
            {
                pieces.push_back(string_to_add);
                string_to_add.clear();
            }
        }
        else
        {
            string_to_add.push_back(source[source_index]);
        }

        source_index++;
    }

    if (string_to_add.empty() == false)
    {
        pieces.push_back(string_to_add);
    }
}

inline void copy_to_clipboard(_In_ std::wstring_view the_string) noexcept
{
    if (OpenClipboard(NULL) != FALSE)
    {
        EmptyClipboard();

        auto global_memory_handle = GlobalAlloc(GHND, (the_string.length() * sizeof(wchar_t)) + 2);

        if (global_memory_handle != NULL)
        {
            auto output_string = static_cast<wchar_t*>(GlobalLock(global_memory_handle));

            if (output_string != nullptr)
            {
                copy_to(the_string, output_string, the_string.length() + 1);

                GlobalUnlock(global_memory_handle);

                SetClipboardData(CF_UNICODETEXT, global_memory_handle);
            }
        }

        CloseClipboard();
    }
}

inline void end_each_entry_with(_Inout_ std::vector<std::wstring>& strings, _In_ std::wstring_view ending) noexcept
{
    if (ending.empty() == true)
    {
        return;
    }

    for (auto& entry : strings)
    {
        if (ends_with_no_case(entry, ending) == false)
        {
            entry.append(ending);
        }
    }
}

inline void remove(_Inout_ std::wstring& the_string, _In_ wchar_t const character_to_remove) noexcept
{
    if (the_string.empty() == true)
    {
        return;
    }

    std::wstring new_string;

    for (auto const& character_to_test : the_string)
    {
        if (character_to_test != character_to_remove)
        {
            new_string.push_back(character_to_test);
        }
    }

    the_string.assign(new_string);
}

struct DEFINED_WIDE_STRING
{
    uint64_t Constant{ 0 };
    wchar_t const* StringValue{ nullptr };
};

inline _Check_return_ int compare_defines(_In_ void const* a, _In_ void const* b) noexcept
{
    if (a == nullptr or b == nullptr)
    {
        return(I_AM_EQUAL_TO_THAT);
    }

    auto defined_a = static_cast<Win32FoundationClasses::DEFINED_WIDE_STRING const*>(a);
    auto defined_b = static_cast<Win32FoundationClasses::DEFINED_WIDE_STRING const*>(b);

    if (defined_a->Constant < defined_b->Constant)
    {
        return(I_AM_LESS_THAN_THAT);
    }
    else if (defined_a->Constant > defined_b->Constant)
    {
        return(I_AM_GREATER_THAN_THAT);
    }

    return(I_AM_EQUAL_TO_THAT);
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

inline void make_reverse(_Inout_ std::wstring& value) noexcept
{
    if (value.empty() == true)
    {
        return;
    }

    std::size_t last_character_index = value.length() - 1;
    std::size_t loop_index = 0;

    while (loop_index < last_character_index)
    {
        wchar_t const character_to_move = value.at(loop_index);
        value.at(loop_index) = value.at(last_character_index);
        value.at(last_character_index) = character_to_move;
        loop_index++;
        last_character_index--;
    }
}

inline void split_unique_non_empty_lines(_In_ std::wstring_view text, _Inout_ std::vector<std::wstring>& lines) noexcept
{
    lines.clear();

    std::size_t string_index = 0;
    std::size_t beginning_of_line = 0;

    while (string_index < text.size())
    {
        wchar_t const this_character = text[string_index];

        if (this_character == CARRIAGE_RETURN or this_character == LINE_FEED)
        {
            if (this_character == CARRIAGE_RETURN)
            {
                if (string_index < (text.size() - 1))
                {
                    if (text[string_index + 1] == LINE_FEED)
                    {
                        string_index++;
                    }
                }
            }

            std::wstring string_to_add(text.substr(beginning_of_line, string_index - beginning_of_line));

            trim(string_to_add);

            if (string_to_add.empty() == false)
            {
                (void)add_to_unique_sorted_vector(string_to_add, lines);
            }

            beginning_of_line = string_index + 1;
        }

        string_index++;
    }

    std::wstring string_to_add(text.substr(beginning_of_line, string_index - beginning_of_line));

    trim(string_to_add);

    if (string_to_add.empty() == false)
    {
        (void)add_to_unique_sorted_vector(string_to_add, lines);
    }
}

inline _Check_return_ std::wstring join(_In_ std::vector<std::wstring> const& strings, _In_ std::wstring_view delimiter) noexcept
{
    std::wstring return_value;

    if (strings.empty() == true)
    {
        return(return_value);
    }

    std::size_t reserve_length = 0;

    for (auto const& this_string : strings)
    {
        reserve_length += this_string.length();
        reserve_length += delimiter.length();
    }

    reserve_length -= delimiter.length();

    return_value.reserve(reserve_length);

    for (auto const& this_string : strings)
    {
        if (return_value.empty() == false)
        {
            return_value.append(delimiter);
        }

        return_value.append(this_string);
    }

    return(return_value);
}

inline void append(_Inout_ std::vector<uint8_t>& bytes, _In_z_ char const* string, _In_ std::size_t const string_length) noexcept
{
    if (string_length < 1 or string == nullptr or string[0] == 0x00)
    {
        return;
    }

    if (bytes.empty() == true)
    {
        bytes.resize(string_length);
        memcpy_s(bytes.data(), bytes.size(), string, string_length);
        return;
    }

    std::size_t const original_byte_length = bytes.size();

    bytes.resize(original_byte_length + string_length);
    memcpy_s(&bytes.data()[original_byte_length], string_length, string, string_length);
}

inline void append(_Inout_ std::vector<uint8_t>& bytes, _In_z_ char const* string) noexcept
{
    if (string == nullptr or string[0] == 0x00)
    {
        return;
    }

    append(bytes, string, strlen(string));
}

inline void append_string_to_byte_array(_In_ char const* the_string, _Inout_ std::vector<uint8_t>& bytes) noexcept
{
    std::size_t const original_array_size = bytes.size();
    std::size_t const string_length = strlen(the_string);

    bytes.resize(string_length + original_array_size);

    auto buffer = bytes.data();

    CopyMemory(&buffer[original_array_size], the_string, string_length);
}

inline void append_string_to_byte_array(_In_ std::string_view the_string, _Inout_ std::vector<uint8_t>& bytes) noexcept
{
    std::size_t const original_array_size = bytes.size();

    bytes.resize(the_string.length() + original_array_size);

    auto buffer = bytes.data();

    CopyMemory(&buffer[original_array_size], the_string.data(), the_string.length());
}

#endif // STL_STRING_HELPERS_HEADER_FILE