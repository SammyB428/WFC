#if ! defined( STL_TRIM_HEADER_FILE )

#define STL_TRIM_HEADER_FILE

#include "stl_is.h"

inline void trim_right(_Inout_ std::wstring& s, _In_ wchar_t const character) noexcept
{
    if (s.empty() == false)
    {
        while (character == s.at(s.length() - 1))
        {
            s.erase(s.length() - 1, 1);

            if (s.empty() == true)
            {
                return;
            }
        }
    }
}

inline void trim_right(_Inout_ std::string& s, _In_ char const character) noexcept
{
    if (s.empty() == false)
    {
        while (character == s.at(s.length() - 1))
        {
            s.erase(s.length() - 1, 1);

            if (s.empty() == true)
            {
                return;
            }
        }
    }
}

inline void trim_right(_Inout_ std::wstring& the_string) noexcept
{
    if (the_string.empty() == false)
    {
        while (iswspace(the_string.at(the_string.length() - 1)))
        {
            the_string.resize(the_string.length() - 1);

            if (the_string.length() <= 0)
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
        while (is_space_character(the_string.at(the_string.length() - 1)))
        {
            the_string.resize(the_string.length() - 1);

            if (the_string.length() <= 0)
            {
                return;
            }
        }
    }
}

inline void trim_left(_Inout_ std::wstring& the_string) noexcept
{
    std::size_t const string_length = the_string.length();

    if (string_length > 0)
    {
        std::size_t number_of_elements_to_erase = 0;

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

inline void trim_left(_Inout_ std::string& the_string) noexcept
{
    std::size_t const string_length = the_string.length();

    if (string_length > 0)
    {
        std::size_t number_of_elements_to_erase = 0;

        while (number_of_elements_to_erase < string_length and
            is_space_character(the_string.at(number_of_elements_to_erase)))
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

inline void trim_left(_Inout_ std::wstring& s, _In_ wchar_t const character) noexcept
{
    std::size_t const string_length = s.length();

    if (string_length > 0)
    {
        std::size_t number_of_elements_to_erase = 0;

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

inline void trim_left(_Inout_ std::string& s, _In_ char const character) noexcept
{
    std::size_t const string_length = s.length();

    if (string_length > 0)
    {
        std::size_t number_of_elements_to_erase = 0;

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

inline void trim(_Inout_ std::wstring& the_string) noexcept
{
    trim_right(the_string);
    trim_left(the_string);
}

inline void trim(_Inout_ std::string& the_string) noexcept
{
    trim_right(the_string);
    trim_left(the_string);
}

inline void despace(_Inout_ std::wstring& s) noexcept
{
    // First see if we need to do anything

    std::size_t number_of_characters = s.length();

    if (number_of_characters < 1)
    {
        return;
    }

    bool needs_despacing = false;

    wchar_t character = s[0];

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
            std::size_t string_index = 1;

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

    bool last_character_added_was_a_space = false;

    std::wstring other_string;

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

inline _Check_return_ bool trim_quotes(_Inout_ std::wstring& s) noexcept
{
    if (s.length() > 1)
    {
        wchar_t const first_character = s[0];
        wchar_t const last_character = s[s.length() - 1];

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

inline void trim_all_quotes(_Inout_ std::wstring& s) noexcept
{
    while (trim_quotes(s) == true)
    {
        ;
    }
}

#endif // STL_TRIM_HEADER_FILE