/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_LEFT_RIGHT_HEADER_FILE )

#define STL_LEFT_RIGHT_HEADER_FILE

inline _Check_return_ std::wstring right(_In_ std::wstring const& s, _In_ std::size_t const count) noexcept
{
    if (s.length() > count)
    {
        return(s.substr(s.length() - count));
    }
    else
    {
        std::wstring return_value(s);

        return(return_value);
    }
}

inline _Check_return_ std::wstring right(_In_ std::wstring_view s, _In_ std::size_t const count) noexcept
{
    if (s.length() > count)
    {
        return(std::wstring(s.substr(s.length() - count)));
    }
    else
    {
        std::wstring return_value(s);

        return(return_value);
    }
}

inline _Check_return_ std::string right(_In_ std::string const& s, _In_ std::size_t const count) noexcept
{
    if (s.length() > count)
    {
        return(s.substr(s.length() - count));
    }
    else
    {
        std::string return_value(s);

        return(return_value);
    }
}

inline _Check_return_ std::string right(_In_ std::string_view s, _In_ std::size_t const count) noexcept
{
    if (s.length() > count)
    {
        return(std::string(s.substr(s.length() - count)));
    }
    else
    {
        std::string return_value(s);

        return(return_value);
    }
}

inline _Check_return_ std::wstring left(_In_ std::wstring const& s, _In_ std::size_t count) noexcept
{
    return(s.substr(0, count));
}

inline _Check_return_ std::string left(_In_ std::string const& s, _In_ std::size_t count) noexcept
{
    return(s.substr(0, count));
}

/// <summary>Returns a string skipping the first 'count' characters specified.</summary>
inline _Check_return_ std::wstring skip_first(_In_ std::wstring const& s, _In_ std::size_t const count) noexcept
{
    auto const string_length = s.length();

    if (count == 0 or count > string_length)
    {
        std::wstring return_value(s);

        return(return_value);
    }

    return(right(s, string_length - count));
}

/// <summary>Returns a string skipping the first 'count' characters specified.</summary>
inline _Check_return_ std::string skip_first(_In_ std::string const& s, _In_ std::size_t const count) noexcept
{
    auto const string_length = s.length();

    if (count == 0 or count > string_length)
    {
        std::string return_value(s);

        return(return_value);
    }

    return(right(s, string_length - count));
}

#endif // STL_LEFT_RIGHT_HEADER_FILE