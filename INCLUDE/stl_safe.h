/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_SAFE_ACCESS_HEADER_FILE )

#define STL_SAFE_ACCESS_HEADER_FILE

// Avoid annoying out of range exceptions
inline constexpr _Check_return_ wchar_t get_at(_In_ std::wstring_view s, _In_ std::size_t const character_index) noexcept
{
    return (character_index >= s.length()) ? 0 : s.at(character_index);
}

inline _Check_return_ wchar_t last_character(_In_ std::wstring const& s) noexcept
{
    if (s.empty() == false)
    {
        return(s.at(s.length() - 1));
    }

    return(0);
}

inline _Check_return_ std::wstring get_last(_In_ std::vector<std::wstring> const& s) noexcept
{
    std::wstring return_value;

    auto const count = s.size();

    if (count > 0)
    {
        return_value.assign(s.at(count - 1));
    }

    return (return_value);
}

#endif // STL_SAFE_ACCESS_HEADER_FILE