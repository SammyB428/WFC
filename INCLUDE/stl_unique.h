/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_STRING_UNIQUE_SORTED_VECTOR_HEADER_FILE )

#define STL_STRING_UNIQUE_SORTED_VECTOR_HEADER_FILE

using WSTRING_CONST_ITERATOR = std::vector<std::wstring>::const_iterator;
using WSTRING_CONST_ITERATOR_PAIR = std::pair<WSTRING_CONST_ITERATOR, WSTRING_CONST_ITERATOR>;

#if defined( WE_ARE_BUILDING_WFC_ON_UNIX )
inline _Check_return_ bool it_wasnt_found(_In_ WSTRING_CONST_ITERATOR_PAIR const& p) noexcept
#else
inline constexpr _Check_return_ bool it_wasnt_found(_In_ WSTRING_CONST_ITERATOR_PAIR const& p) noexcept
#endif
{
    return((p.first == p.second) ? true : false);
}

#if defined( WE_ARE_BUILDING_WFC_ON_UNIX )
inline _Check_return_ bool it_was_found(_In_ WSTRING_CONST_ITERATOR_PAIR const& p) noexcept
#else
inline constexpr _Check_return_ bool it_was_found(_In_ WSTRING_CONST_ITERATOR_PAIR const& p) noexcept
#endif
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

#if defined( WE_ARE_BUILDING_WFC_ON_UNIX )
static inline _Check_return_ bool it_wasnt_found(_In_ STRING_CONST_ITERATOR_PAIR const& p) noexcept
#else
static inline constexpr _Check_return_ bool it_wasnt_found(_In_ STRING_CONST_ITERATOR_PAIR const& p) noexcept
#endif
{
    return((p.first == p.second) ? true : false);
}

#if defined( WE_ARE_BUILDING_WFC_ON_UNIX )
static inline _Check_return_ bool it_was_found(_In_ STRING_CONST_ITERATOR_PAIR const& p) noexcept
#else
static inline constexpr _Check_return_ bool it_was_found(_In_ STRING_CONST_ITERATOR_PAIR const& p) noexcept
#endif
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