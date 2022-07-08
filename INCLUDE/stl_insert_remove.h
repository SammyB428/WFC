/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_INSERT_REMOVE_HEADER_FILE )

#define STL_INSERT_REMOVE_HEADER_FILE

#include "stl_compare.h"

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

    if (string_to_insert not_eq nullptr)
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

inline _Check_return_ std::size_t replace(_Inout_ std::wstring& s, _In_ wchar_t const old_character, _In_ wchar_t const new_character) noexcept
{
    std::size_t number_of_replacements{ 0 };

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

inline _Check_return_ int delete_characters(_Inout_ std::wstring& s, _In_ int const index, _In_ int number_of_characters_to_delete = 1) noexcept
{
    if (index < 0 or number_of_characters_to_delete < 1)
    {
        return(static_cast<int>(s.length()));
    }

    auto const number_of_characters_remaining{ static_cast<int>(static_cast<int>(s.length()) - index) };

    if (number_of_characters_to_delete > number_of_characters_remaining)
    {
        number_of_characters_to_delete = number_of_characters_remaining;
    }

    s = s.erase(index, number_of_characters_to_delete);

    return(static_cast<int>(s.length()));
}

inline void replace(_Inout_ std::wstring& s, _In_ std::wstring_view what_to_replace, _In_ std::wstring_view replace_it_with) noexcept
{
    std::wstring translated_string;

    auto location{ s.find(what_to_replace) };

    while (location not_eq s.npos)
    {
        // Append the data up to the what_to_replace to our translated string

        translated_string.append(s.substr(0, location));
        translated_string.append(replace_it_with);

        std::ignore = s.erase(0, location + what_to_replace.length());

        location = s.find(what_to_replace);
    }

    translated_string.append(s);
    s = translated_string;
}

inline void replace(_Inout_ std::string& s, _In_ std::string_view what_to_replace, _In_ std::string_view replace_it_with) noexcept
{
    std::string translated_string;

    auto location{ s.find(what_to_replace) };

    while (location not_eq s.npos)
    {
        // Append the data up to the what_to_replace to our translated string

        translated_string.append(s.substr(0, location));
        translated_string.append(replace_it_with);

        std::ignore = s.erase(0, location + what_to_replace.length());

        location = s.find(what_to_replace);
    }

    translated_string.append(s);
    s = translated_string;
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

    std::size_t loop_index{ 0 };

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

inline void remove(_Inout_ std::wstring& the_string, _In_ wchar_t const character_to_remove) noexcept
{
    if (the_string.empty() == true)
    {
        return;
    }

    std::wstring new_string;

    for (auto const& character_to_test : the_string)
    {
        if (character_to_test not_eq character_to_remove)
        {
            new_string.push_back(character_to_test);
        }
    }

    the_string.assign(new_string);
}

#endif // STL_INSERT_REMOVE_HEADER_FILE