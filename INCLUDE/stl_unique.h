/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_STRING_UNIQUE_SORTED_VECTOR_HEADER_FILE )

#define STL_STRING_UNIQUE_SORTED_VECTOR_HEADER_FILE

inline _Check_return_ SSIZE_T add_to_unique_sorted_vector(_In_ std::wstring_view value_to_add, _Inout_ std::vector<std::wstring>& values) noexcept
{
    auto const lower = std::lower_bound(std::cbegin(values), std::cend(values), value_to_add);
    auto const return_value = std::distance(std::cbegin(values), lower);

    if (not (lower not_eq std::cend(values) and not (value_to_add < *lower)))
    {
        values.emplace(lower, value_to_add);
    }

    return(return_value);
}

inline _Check_return_ SSIZE_T add_to_unique_sorted_vector(_In_ std::string_view value_to_add, _Inout_ std::vector<std::string>& values) noexcept
{
    auto const lower = std::lower_bound(std::cbegin(values), std::cend(values), value_to_add);
    auto const return_value = std::distance(std::cbegin(values), lower);

    if (not (lower not_eq std::cend(values) and not (value_to_add < *lower)))
    {
        values.emplace(lower, value_to_add);
    }

    return(return_value);
}

inline _Check_return_ bool compare_strings_ignoring_case(_In_ std::string_view left, _In_ std::string_view right) noexcept
{
    return(compare_no_case(left, right) < I_AM_EQUAL_TO_THAT);
}

inline _Check_return_ SSIZE_T add_to_unique_sorted_vector_ignore_case(_In_ std::string_view value_to_add, _Inout_ std::vector<std::string>& values) noexcept
{
    auto const lower = std::lower_bound(std::cbegin(values), std::cend(values), value_to_add, compare_strings_ignoring_case);
    auto const return_value = std::distance(std::cbegin(values), lower);

    if (not (lower not_eq std::cend(values) and not (value_to_add < *lower)))
    {
        values.emplace(lower, value_to_add);
    }

    return(return_value);
}

inline _Check_return_ bool contains_no_case_sorted(_In_ std::vector<std::string> const& s, _In_z_ char const* the_string) noexcept
{
    if (s.empty() == true or the_string == nullptr)
    {
        return(false);
    }

    std::string_view right_hand_side(the_string);

    auto const lower = std::lower_bound(std::cbegin(s), std::cend(s), right_hand_side, compare_strings_ignoring_case);

    return (lower not_eq std::cend(s) and not (right_hand_side < *lower));
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

#endif // STL_STRING_UNIQUE_SORTED_VECTOR_HEADER_FILE