/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_IS_HEADER_FILE )

#define STL_IS_HEADER_FILE

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
        if (not WFC_IS_HEXADECIMAL(character))
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

inline _Check_return_ bool constexpr is_null_or_empty(_In_opt_z_ wchar_t const * s) noexcept
{
    return (s == nullptr or s[0] == 0x00) ? true : false;
}

#endif // STL_IS_HEADER_FILE