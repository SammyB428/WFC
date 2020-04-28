/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_SPLIT_JOIN_HEADER_FILE )

#define STL_SPLIT_JOIN_HEADER_FILE

#include "stl_trim.h"

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

    while (source[source_index] not_eq 0x00)
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

    while (source[source_index] not_eq 0x00)
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

    if (source.empty() == true or separation_character == 0x00)
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

    if (length > 0 and here not_eq nullptr)
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

inline void split_path(_In_ std::wstring const& source, _Inout_ std::vector<std::wstring>& pieces) noexcept
{
    pieces.clear();

    if (source.empty() == true)
    {
        return;
    }

    int source_index = 0;

    std::wstring string_to_add;

    while (source[source_index] not_eq 0x00)
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

#endif // STL_SPLIT_JOIN_HEADER_FILE