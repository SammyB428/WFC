/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_MAKE_HEADER_FILE )

#define STL_MAKE_HEADER_FILE

#define wfc_to_upper( _x ) ( ( _x >= 0x61 and _x <= 0x7A ) ? _x - 0x20 : _x )

inline _Check_return_ wchar_t make_wide_character_lower_case(_In_ wchar_t const existing_character) noexcept
{
    return(static_cast<wchar_t>(towlower(existing_character)));
}

inline void make_lower(_Inout_ std::wstring& s) noexcept
{
    if (s.empty() == false)
    {
        std::transform(std::begin(s), std::end(s), std::begin(s), make_wide_character_lower_case);
    }
}

inline _Check_return_ wchar_t make_wide_character_upper_case(_In_ wchar_t const existing_character) noexcept
{
    return(static_cast<wchar_t>(towupper(existing_character)));
}

inline void make_upper(_Inout_ std::wstring& s) noexcept
{
    if (s.empty() == false)
    {
        std::transform(std::begin(s), std::end(s), std::begin(s), make_wide_character_upper_case);
    }
}

inline void make_reverse(_Inout_ std::wstring& value) noexcept
{
    if (value.empty() == true)
    {
        return;
    }

    std::size_t last_character_index = value.length() - 1;
    std::size_t loop_index = 0;

    while (loop_index < last_character_index)
    {
        wchar_t const character_to_move = value.at(loop_index);
        value.at(loop_index) = value.at(last_character_index);
        value.at(last_character_index) = character_to_move;
        loop_index++;
        last_character_index--;
    }
}

#endif // STL_MAKE_HEADER_FILE