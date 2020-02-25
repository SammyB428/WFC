/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_COMPARE_HEADER_FILE )

#define STL_COMPARE_HEADER_FILE

#if defined(WE_ARE_BUILDING_WFC_ON_UNIX)

#endif // WE_ARE_BUILDING_WFC_ON_UNIX

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

    return(I_AM_EQUAL_TO_THAT);
}

inline _Check_return_ int compare_no_case(_In_ std::wstring const& s, _In_opt_z_ wchar_t const* string_p) noexcept
{
    WFC_VALIDATE_POINTER_NULL_OK(string_p);

    if (string_p != nullptr)
    {
        return(compare_no_case(s, std::wstring_view(string_p, ::wcslen(string_p))));
    }
    else
    {
        // We are greater than nothing (NULL)
        return(I_AM_GREATER_THAN_THAT);
    }
}

inline _Check_return_ int compare_no_case(_In_ std::string_view s1, _In_ std::string_view s2) noexcept
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

    return(I_AM_EQUAL_TO_THAT);
}


inline _Check_return_ int compare_no_case(_In_ std::string const& s, _In_opt_z_ char const* string_p) noexcept
{
    WFC_VALIDATE_POINTER_NULL_OK(string_p);

    if (string_p != nullptr)
    {
        return(compare_no_case(s, std::string_view(string_p, ::strlen(string_p))));
    }
    else
    {
        // We are greater than nothing (NULL)
        return(I_AM_GREATER_THAN_THAT);
    }
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