/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_SPLIT_JOIN_HEADER_FILE )

#define STL_SPLIT_JOIN_HEADER_FILE

#include "stl_trim.h"

#include "stl_split_join_wchar.h"
#include "stl_split_join_char16.h"

// ASCII API
inline void split(_In_z_ char const* source, _In_z_ char const* field_delimiter, _Inout_ std::vector<std::string>& string_array) noexcept
{
    string_array.clear();

    if (source == nullptr or source[0] == 0x00 or
        field_delimiter == nullptr or field_delimiter[0] == 0x00)
    {
        return;
    }

    auto const terminator_length{ strlen(field_delimiter) };

    int source_index{ 0 };

    std::string string_to_add;

    while (source[source_index] not_eq 0x00)
    {
        if (_strnicmp(&source[source_index], field_delimiter, terminator_length) == 0)
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

inline void split(_In_z_ char const* source, _In_ std::size_t const length, char const field_delimiter, _Inout_ std::vector<std::string>& string_array) noexcept
{
    string_array.clear();

    if (source == nullptr or source[0] == 0x00 or field_delimiter == 0x00)
    {
        return;
    }

    std::string string_to_add;

    for (auto const string_index : Range(length))
    {
        if (source[string_index] == field_delimiter)
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

inline void split(_In_z_ char const* source, _In_ char const field_delimiter, _Inout_ std::vector<std::string>& string_array) noexcept
{
    string_array.clear();

    if (source == nullptr or source[0] == 0x00 or field_delimiter == 0x00)
    {
        return;
    }

    split(source, strlen(source), field_delimiter, string_array);
}

inline void split_view(_In_z_ char const* source, _In_ std::size_t const length, _In_ char const field_delimiter, _Inout_ std::vector<std::string_view>& view_array) noexcept
{
    view_array.clear();

    if (source == nullptr or source[0] == 0x00 or field_delimiter == 0x00 or length < 1)
    {
        return;
    }

    std::size_t beginning_index_of_string{ 0 };

    for (auto const string_index : Range(length))
    {
        if (source[string_index] == field_delimiter)
        {
            view_array.push_back(std::string_view(&source[beginning_index_of_string], string_index - beginning_index_of_string));
            beginning_index_of_string = string_index + 1;
        }
    }

    if (beginning_index_of_string <= length)
    {
        view_array.push_back(std::string_view(&source[beginning_index_of_string], length - beginning_index_of_string));
    }
}

inline void split_view(_In_ std::string_view source, _In_ std::size_t const length, _In_ char const field_delimiter, _Inout_ std::vector<std::string_view>& view_array) noexcept
{
    split_view(source.data(), source.length(), field_delimiter, view_array);
}

inline void split(_In_ std::string_view source, char const field_delimiter, _Inout_ std::vector<std::string_view>& view_array) noexcept
{
    split_view(source.data(), source.length(), field_delimiter, view_array);
}

inline void split(_In_ std::string_view source, _In_ char const field_delimiter, _Inout_ std::vector<std::string>& string_array) noexcept
{
    split(source.data(), source.length(), field_delimiter, string_array);
}

inline void join(_In_ std::vector<std::string> const& s, _In_ std::string_view field_delimiter, _Inout_ std::string& result) noexcept
{
    result.clear();

    auto const number_of_elements = s.size();

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

        if (field_delimiter.empty() == false)
        {
            result.append(field_delimiter);
        }

        array_index++;
    }

    // Add the last entry without a divider.
    result.append(s.at(array_index));
}

#endif // STL_SPLIT_JOIN_HEADER_FILE