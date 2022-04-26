/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_TRIM_CHAR16_HEADER_FILE )

#define STL_TRIM_CHAR16_HEADER_FILE

inline void trim_right(_Inout_ std::u16string& s, _In_ char16_t const character) noexcept
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

inline void trim_right(_Inout_ std::u16string& the_string) noexcept
{
    if (the_string.empty() == false)
    {
        while (iswspace(the_string.back()))
        {
            the_string.resize(the_string.length() - 1);

            if (the_string.length() <= 0)
            {
                return;
            }
        }
    }
}

inline void trim_left(_Inout_ std::u16string& the_string) noexcept
{
    auto const string_length{ the_string.length() };

    if (string_length > 0)
    {
        std::size_t number_of_elements_to_erase{ 0 };

        while (number_of_elements_to_erase < string_length and
            iswspace(the_string.at(number_of_elements_to_erase)))
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

inline void trim_left(_Inout_ std::u16string& s, _In_ char16_t const character) noexcept
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

inline void trim(_Inout_ std::u16string& the_string) noexcept
{
    trim_right(the_string);
    trim_left(the_string);
}

inline void despace(_Inout_ std::u16string& s) noexcept
{
    // First see if we need to do anything

    auto number_of_characters{ s.length() };

    if (number_of_characters < 1)
    {
        return;
    }

    bool needs_despacing{ false };

    auto character{ s[0] };

    if (character == 0x20 or
        character == TAB_CHARACTER or
        character == CARRIAGE_RETURN or
        character == LINE_FEED)
    {
        needs_despacing = true;
    }
    else
    {
        character = s[number_of_characters - 1];

        if (character == 0x20 or
            character == TAB_CHARACTER or
            character == CARRIAGE_RETURN or
            character == LINE_FEED)
        {
            needs_despacing = true;
        }
        else
        {
            std::size_t string_index{ 1 };

            while (needs_despacing == false and string_index < (number_of_characters - 1))
            {
                character = s[string_index];

                if (character == 0x20 or
                    character == TAB_CHARACTER or
                    character == CARRIAGE_RETURN or
                    character == LINE_FEED)
                {
                    string_index++;

                    character = s[string_index];

                    if (character == 0x20 or
                        character == TAB_CHARACTER or
                        character == CARRIAGE_RETURN or
                        character == LINE_FEED)
                    {
                        needs_despacing = true;
                    }
                }

                string_index++;
            }
        }
    }

    if (needs_despacing == false)
    {
        return;
    }

    // Well finally, we have something to do.

    trim(s); // Get rid of the easy case

    number_of_characters = s.length();

    bool last_character_added_was_a_space{ false };

    std::u16string other_string;

    for (auto const string_index : Range(number_of_characters))
    {
        character = s[string_index];

        if (character == 0x20 or
            character == TAB_CHARACTER or
            character == CARRIAGE_RETURN or
            character == LINE_FEED)
        {
            if (last_character_added_was_a_space == false)
            {
                other_string.push_back(character);
                last_character_added_was_a_space = true;
            }
        }
        else
        {
            other_string.push_back(character);
            last_character_added_was_a_space = false;
        }
    }

    s = other_string;
}

inline _Check_return_ bool trim_quotes(_Inout_ std::u16string& s) noexcept
{
    if (s.length() > 1)
    {
        auto const first_character{ s.front() };
        auto const last_character{ s.back() };

        if (first_character == last_character)
        {
            // The string starts and ends with the same character.

            if (first_character == '\"')
            {
                trim_right(s, '\"');
                trim_left(s, '\"');
                trim(s);
                return(true);
            }

            if (first_character == '\'')
            {
                trim_right(s, '\'');
                trim_left(s, '\'');
                trim(s);
                return(true);
            }
        }

        return(false); // Nothing was trimmed
    }

    return(false); // Nothing was trimmed
}

inline void trim_all_quotes(_Inout_ std::u16string& s) noexcept
{
    while (trim_quotes(s) == true)
    {
        ;
    }
}

inline constexpr [[nodiscard]] std::u16string_view trim_left_view(_In_ std::u16string_view the_string) noexcept
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
            return(std::u16string_view(the_string.data(), 0));
        }
        else
        {
            return(std::u16string_view(&the_string.data()[number_of_elements_to_erase], string_length - number_of_elements_to_erase));
        }
    }

    return(std::u16string_view(the_string.data(), 0));
}

inline constexpr [[nodiscard]] std::u16string_view trim_left_view(_In_ std::u16string_view s, _In_ char16_t const character) noexcept
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
            return(std::u16string_view(s.data(), 0));
        }
        else
        {
            return(std::u16string_view(&s.data()[number_of_elements_to_erase], string_length - number_of_elements_to_erase));
        }
    }

    return(std::u16string_view(s.data(), 0));
}

inline constexpr [[nodiscard]] std::u16string_view trim_right_view(_In_ std::u16string_view the_string) noexcept
{
    if (the_string.empty() == false)
    {
        auto string_length{ the_string.length() };

        while (Win32FoundationClasses::is_space_character(the_string.at(string_length - 1)))
        {
            string_length--;

            if (string_length <= 0)
            {
                return(std::u16string_view(the_string.data(), 0));
            }
        }

        return(std::u16string_view(the_string.data(), string_length));
    }

    return(the_string);
}

inline constexpr [[nodiscard]] std::u16string_view trim_view(_In_ std::u16string_view the_string) noexcept
{
    return(trim_left_view(trim_right_view(the_string)));
}

#endif // STL_TRIM_CHAR16_HEADER_FILE
