/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_WITH_HEADER_FILE )

#define STL_WITH_HEADER_FILE

#include "stl_compare.h"

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
        if (s[loop_index] not_eq beginning[loop_index])
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
        if (s[loop_index] not_eq beginning[loop_index])
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

        if (this_character not_eq that_character)
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

        if (this_character not_eq that_character)
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

#endif // STL_WITH_HEADER_FILE