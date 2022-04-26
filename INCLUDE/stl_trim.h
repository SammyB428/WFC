/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_TRIM_HEADER_FILE )

#define STL_TRIM_HEADER_FILE

#include <algorithm>

#include "stl_is.h"

#include "stl_trim_char16.h"
#include "stl_trim_wchar.h"

inline void trim_right(_Inout_ std::string& s, _In_ char const character) noexcept
{
    if (s.empty() == false)
    {
        while (character == s.back())
        {
            s.erase(s.length() - 1, 1);

            if (s.empty() == true)
            {
                return;
            }
        }
    }
}

inline void trim_right(_Inout_ std::string& the_string) noexcept
{
    if (the_string.empty() == false)
    {
        while (Win32FoundationClasses::is_space_character(the_string.back()))
        {
            the_string.resize(the_string.length() - 1);

            if (the_string.length() <= 0)
            {
                return;
            }
        }
    }
}

inline void trim_left(_Inout_ std::string& the_string) noexcept
{
    auto const string_length{ the_string.length() };

    if (string_length > 0)
    {
        std::size_t number_of_elements_to_erase{ 0 };

        while (number_of_elements_to_erase < string_length and
            Win32FoundationClasses::is_space_character(the_string.at(number_of_elements_to_erase)))
        {
            number_of_elements_to_erase++;
        }

        if (number_of_elements_to_erase >= string_length)
        {
            the_string.resize(0);
        }
        else
        {
            the_string.erase(0, number_of_elements_to_erase);
        }
    }
}

inline void trim_left(_Inout_ std::string& s, _In_ char const character) noexcept
{
    auto const string_length{ s.length() };

    if (string_length > 0)
    {
        std::size_t number_of_elements_to_erase{ 0 };

        while (number_of_elements_to_erase < string_length and
            character == (s.at(number_of_elements_to_erase)))
        {
            number_of_elements_to_erase++;
        }

        if (number_of_elements_to_erase >= string_length)
        {
            s.resize(0);
        }
        else
        {
            s.erase(0, number_of_elements_to_erase);
        }
    }
}

inline void trim(_Inout_ std::string& the_string) noexcept
{
    trim_right(the_string);
    trim_left(the_string);
}

// String View versions
inline constexpr [[nodiscard]] std::string_view trim_left_view(_In_ std::string_view the_string) noexcept
{
    auto const string_length{ the_string.length() };

    if (string_length > 0)
    {
        std::size_t number_of_elements_to_erase{ 0 };

        while (number_of_elements_to_erase < string_length and
            Win32FoundationClasses::is_space_character(the_string.at(number_of_elements_to_erase)))
        {
            number_of_elements_to_erase++;
        }

        if (number_of_elements_to_erase >= string_length)
        {
            return(std::string_view(the_string.data(), 0));
        }
        else
        {
            return(std::string_view(&the_string.data()[number_of_elements_to_erase], string_length - number_of_elements_to_erase));
        }
    }

    return(std::string_view(the_string.data(), 0));
}

inline constexpr [[nodiscard]] std::string_view trim_left_view(_In_ std::string_view s, _In_ char const character) noexcept
{
    auto const string_length{ s.length() };

    if (string_length > 0)
    {
        std::size_t number_of_elements_to_erase{ 0 };

        while (number_of_elements_to_erase < string_length and
            character == (s.at(number_of_elements_to_erase)))
        {
            number_of_elements_to_erase++;
        }

        if (number_of_elements_to_erase >= string_length)
        {
            return(std::string_view(s.data(), 0));
        }
        else
        {
            return(std::string_view(&s.data()[number_of_elements_to_erase], string_length - number_of_elements_to_erase));
        }
    }

    return(std::string_view(s.data(), 0));
}

inline constexpr [[nodiscard]] std::string_view trim_right_view(_In_ std::string_view the_string) noexcept
{
    if (the_string.empty() == false)
    {
        auto string_length{ the_string.length() };

        while (Win32FoundationClasses::is_space_character(the_string.at(string_length - 1)))
        {
            string_length--;

            if (string_length <= 0)
            {
                return(std::string_view(the_string.data(), 0));
            }
        }

        return(std::string_view(the_string.data(), string_length));
    }

    return(the_string);
}

inline constexpr [[nodiscard]] std::string_view trim_view(_In_ std::string_view the_string) noexcept
{
    return(trim_left_view(trim_right_view(the_string)));
}

#endif // STL_TRIM_HEADER_FILE
