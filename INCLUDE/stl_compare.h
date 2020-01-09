/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_COMPARE_HEADER_FILE )

#define STL_COMPARE_HEADER_FILE

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

#endif // STL_COMPARE_HEADER_FILE