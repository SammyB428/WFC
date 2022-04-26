/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_SPLIT_JOIN_CHAR16_HEADER_FILE )

#define STL_SPLIT_JOIN_CHAR16_HEADER_FILE

inline void split(_In_z_ char16_t const * source, _In_z_ char16_t const * field_delimiter, _Inout_ std::vector<std::u16string>& string_array) noexcept
{
    string_array.clear();

    if (source == nullptr or source[0] == 0x00 or
        field_delimiter == nullptr or field_delimiter[0] == 0x00)
    {
        return;
    }

    auto const terminator_length{ std::char_traits<char16_t>::length(field_delimiter) };

    int source_index{ 0 };

    std::u16string string_to_add;

    while (source[source_index] not_eq 0x00)
    {
        if (_wcsnicmp((LPCWSTR) &source[source_index], (LPCWSTR) field_delimiter, terminator_length) == I_AM_EQUAL_TO_THAT)
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

inline void split(_In_z_ char16_t const * source, _In_ std::size_t const length, _In_ char16_t const field_delimiter, _Inout_ std::vector<std::u16string>& string_array) noexcept
{
    string_array.clear();

    if (source == nullptr or source[0] == 0x00 or field_delimiter == 0x00)
    {
        return;
    }

    std::u16string string_to_add;

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

inline void split(_In_z_ char16_t const * source, _In_ char16_t const field_delimiter, _Inout_ std::vector<std::u16string>& string_array) noexcept
{
    string_array.clear();

    if (source == nullptr or source[0] == 0x00 or field_delimiter == 0x00)
    {
        return;
    }

    split(source, std::char_traits<char16_t>::length(source), field_delimiter, string_array);
}

inline void split(_In_ std::u16string_view source, _In_ char16_t const field_delimiter, _Inout_ std::vector<std::u16string>& string_array) noexcept
{
    split(source.data(), source.length(), field_delimiter, string_array);
}

inline void split_view(_In_z_ char16_t const * source, _In_ std::size_t const length, _In_ char16_t const field_delimiter, _Inout_ std::vector<std::u16string_view>& view_array) noexcept
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
            view_array.push_back(std::u16string_view(&source[beginning_index_of_string], string_index - beginning_index_of_string));
            beginning_index_of_string = string_index + 1;
        }
    }

    if (beginning_index_of_string <= length)
    {
        view_array.push_back(std::u16string_view(&source[beginning_index_of_string], length - beginning_index_of_string));
    }
}

inline void split_view(_In_ std::u16string_view source, _In_ char16_t const field_delimiter, _Inout_ std::vector<std::u16string_view>& view_array) noexcept
{
    split_view(source.data(), source.length(), field_delimiter, view_array);
}

inline void split_and_trim(_In_ std::u16string_view source, _In_ char16_t const field_delimiter, _Inout_ std::vector<std::u16string>& string_array) noexcept
{
    string_array.clear();

    if (source.empty() == true)
    {
        return;
    }

    std::u16string string_to_add;

    for (auto const source_character : source)
    {
        if (source_character == field_delimiter)
        {
            Win32FoundationClasses::trim(string_to_add);

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

    Win32FoundationClasses::trim(string_to_add);

    if (string_to_add.empty() == false)
    {
        string_array.push_back(string_to_add);
    }
}

inline void join(_In_ std::vector<std::u16string> const& s, _In_ std::u16string_view field_delimiter, _Inout_ std::u16string& result) noexcept
{
    result.clear();

    auto const number_of_elements{ s.size() };

    if (number_of_elements == 0)
    {
        return;
    }

    if (number_of_elements == 1)
    {
        result.assign(s.front());
        return;
    }

    std::size_t array_index{ 0 };

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

inline void split_path(_In_ std::u16string const& source, _Inout_ std::vector<std::u16string>& pieces) noexcept
{
    pieces.clear();

    if (source.empty() == true)
    {
        return;
    }

    int source_index{ 0 };

    std::u16string string_to_add;

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

inline void split_unique_non_empty_lines(_In_ std::u16string_view text, _Inout_ std::vector<std::u16string>& lines) noexcept
{
    lines.clear();

    std::size_t string_index{ 0 };
    std::size_t beginning_of_line{ 0 };

    while (string_index < text.size())
    {
        auto const this_character{ text[string_index] };

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

            std::u16string string_to_add(text.substr(beginning_of_line, string_index - beginning_of_line));

            Win32FoundationClasses::trim(string_to_add);

            if (string_to_add.empty() == false)
            {
                std::ignore = Win32FoundationClasses::add_to_unique_sorted_vector(string_to_add, lines);
            }

            beginning_of_line = string_index + 1;
        }

        string_index++;
    }

    std::u16string string_to_add(text.substr(beginning_of_line, string_index - beginning_of_line));

    Win32FoundationClasses::trim(string_to_add);

    if (string_to_add.empty() == false)
    {
        std::ignore = Win32FoundationClasses::add_to_unique_sorted_vector(string_to_add, lines);
    }
}

inline _Check_return_ std::u16string join(_In_ std::vector<std::u16string> const& strings, _In_ std::u16string_view field_delimiter) noexcept
{
    std::u16string return_value;

    if (strings.empty() == true)
    {
        return(return_value);
    }

    std::size_t reserve_length{ 0 };

    for (auto const& this_string : strings)
    {
        reserve_length += this_string.length();
        reserve_length += field_delimiter.length();
    }

    reserve_length -= field_delimiter.length();

    return_value.reserve(reserve_length);

    for (auto const& this_string : strings)
    {
        if (return_value.empty() == false)
        {
            return_value.append(field_delimiter);
        }

        return_value.append(this_string);
    }

    return(return_value);
}

#endif // STL_SPLIT_JOIN_CHAR16_HEADER_FILE