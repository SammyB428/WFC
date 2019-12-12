/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2019, Samuel R. Blackburn
**
** "You can get credit for something or get it done, but not both."
** Dr. Richard Garwin
**
** BSD License follows.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** Redistributions of source code must retain the above copyright notice,
** this list of conditions and the following disclaimer. Redistributions
** in binary form must reproduce the above copyright notice, this list
** of conditions and the following disclaimer in the documentation and/or
** other materials provided with the distribution. Neither the name of
** the WFC nor the names of its contributors may be used to endorse or
** promote products derived from this software without specific prior
** written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** $Workfile: CDataParser.hpp $
** $Revision: 26 $
** $Modtime: 10/03/01 4:18a $
*/

#if ! defined ( DATA_PARSER_CLASS_HEADER )

#define DATA_PARSER_CLASS_HEADER

#if defined( SCREW_PERFORMANCE_AND_USE_SET_LAST_ERROR )
#define WFC_SET_LAST_ERROR( _x ) ::SetLastError( _x )
#else
#define WFC_SET_LAST_ERROR( _x )
#endif

#define MAKE_uint16_t( least_significant, most_significant ) \
    ( (uint16_t) (uint8_t) (least_significant) bitor ( (uint16_t) (uint8_t) (most_significant) << 8 ) )

#define MAKE_uint32_t( least_significant, byte_2, byte_3, most_significant ) \
    ( (uint32_t) (uint8_t) (least_significant) bitor ( (uint32_t) (uint8_t) (byte_2) <<  8 ) bitor \
    ( (uint32_t) (uint8_t) (byte_3) << 16 )    bitor ( (uint32_t) (uint8_t) (most_significant) << 24 ) )

#define MAKE_uint64_t( least_significant, byte_2, byte_3, byte_4, byte_5, byte_6, byte_7, most_significant ) \
    ( (uint64_t) (uint8_t) (least_significant) bitor ( (uint64_t) (uint8_t) (byte_2) <<  8 ) bitor \
    ( (uint64_t) (uint8_t) (byte_3) << 16 )    bitor ( (uint64_t) (uint8_t) (byte_4) << 24 ) bitor \
    ( (uint64_t) (uint8_t) (byte_5) << 32 )    bitor ( (uint64_t) (uint8_t) (byte_6) << 40 ) bitor \
    ( (uint64_t) (uint8_t) (byte_7) << 48 )    bitor ( (uint64_t) (uint8_t) (most_significant) << 56 ) )

class CMatchingCharacters
{
public:

    uint64_t Open{ 0 };
    uint64_t Close{ 0 };
    uint64_t BackToBack{ 0 };

    CMatchingCharacters(CMatchingCharacters const&) = delete;
    CMatchingCharacters& operator=(CMatchingCharacters const&) = delete;

    inline constexpr CMatchingCharacters() noexcept
    {
        Empty();
    }

    inline constexpr CMatchingCharacters(uint64_t o, uint64_t c, uint64_t b) noexcept : Open(o), Close(c), BackToBack(b)
    {
    }

    inline constexpr void Empty( void ) noexcept
    {
        Open = 0;
        Close = 0;
        BackToBack = 0;
    }
};

inline constexpr _Check_return_ bool wfc_is_punctuation( _In_ uint32_t const character ) noexcept
{
    if ( character >= 0x21 and character <= 0x2F )
    {
        return( true );
    }

    if ( character >= 0x3A and character <= 0x40 )
    {
        return( true );
    }

    if ( character >= 0x5B and character <= 0x60 )
    {
        return( true );
    }

    if ( character >= 0x7B and character <= 0x7E )
    {
        return( true );
    }

    return( false );
}

class CTextStatistics
{
public:

    uint64_t NumberOfCharacters{ 0 };
    uint64_t NumberOfCharactersOutsideASCII{ 0 };
    uint64_t NumberOfSpaces{ 0 };
    uint64_t NumberOfLetters{ 0 };
    uint64_t NumberOfFunctionLetters{ 0 };
    uint64_t NumberOfDigits{ 0 };
    uint64_t NumberOfPunctuation{ 0 };
    uint64_t NumberOfDoubleQuotes{ 0 };
    uint64_t NumberOfSingleQuotes{ 0 };
    uint64_t NumberOfLines{ 0 };
    uint64_t NumberOfEmptyLines{ 0 };
    uint64_t NumberOfArrows{ 0 };
    uint64_t NumberOfEquals{ 0 };
    uint64_t NumberOfSemiColons{ 0 };
    uint64_t NumberOfColons{ 0 };
    uint64_t LongestLine{ 0 };

    CMatchingCharacters Parentheses;
    CMatchingCharacters CurlyBraces;
    CMatchingCharacters Brackets;
    CMatchingCharacters GreaterThanLessThan;

    CTextStatistics(_In_ CTextStatistics const&) = delete;
    _Check_return_ CTextStatistics& operator=(_In_ CTextStatistics const&) = delete;

    inline constexpr CTextStatistics() noexcept
    {
        Empty();
    }

    inline constexpr _Check_return_ double AverageLineLength( void ) const noexcept
    {
        uint64_t const number_of_non_new_line_characters = NumberOfCharactersOutsideASCII + NumberOfSpaces + NumberOfLetters + NumberOfDigits + NumberOfPunctuation;

        if ( NumberOfLines == 0 )
        {
            return( (double) number_of_non_new_line_characters );
        }

        double const return_value = static_cast<double>( (double) number_of_non_new_line_characters / (double) NumberOfLines );

        return( return_value );
    }

    inline constexpr void Empty() noexcept
    {
        NumberOfCharacters      = 0;
        NumberOfCharactersOutsideASCII = 0;
        NumberOfSpaces          = 0;
        NumberOfLetters         = 0;
        NumberOfFunctionLetters = 0;
        NumberOfDigits          = 0;
        NumberOfPunctuation     = 0;
        NumberOfDoubleQuotes    = 0;
        NumberOfSingleQuotes    = 0;
        NumberOfLines           = 0;
        NumberOfEmptyLines      = 0;
        NumberOfArrows          = 0; // ->
        LongestLine             = 0;
        NumberOfEquals          = 0;
        NumberOfSemiColons      = 0;
        NumberOfColons          = 0;

        Parentheses.Empty();
        CurlyBraces.Empty();
        Brackets.Empty();
        GreaterThanLessThan.Empty();
    }
};

/*
** Sorry this is so huge but I had to get performance up...
*/

class CDataParser
{
protected:

#if defined( _DEBUG )

    uint64_t m_LastIndex{ 0 };
    uint64_t m_NumberOfTimesWeHaveBeenAskedForTheSameIndex{ 0 };

#endif // _DEBUG

    bool            m_AutomaticallyDelete{ false };
    std::vector<uint8_t> * m_Data{ nullptr };
    uint8_t const * m_Bytes{ nullptr };
    uint64_t        m_NumberOfBytes{ 0 };
    bool            m_IsASCII{ true };
    bool            m_IsBigEndian{ false };
    bool            m_IsUCS4{ false };
    uint32_t        m_UCS4Order{ 4321 };
    uint8_t         m_UnicodeToASCIIErrorCharacter{ 'q' };

public:

    CDataParser(_In_ CDataParser const&) = delete;
    _Check_return_ CDataParser& operator=(_In_ CDataParser const&) = delete;

    inline constexpr CDataParser() noexcept
    {
        m_AutomaticallyDelete          = false;
        m_Data                         = nullptr;
        m_Bytes                        = nullptr;
        m_NumberOfBytes                = 0;
        m_IsASCII                      = true;
        m_IsBigEndian                  = false;
        m_IsUCS4                       = false;
        m_UCS4Order                    = 4321;
        m_UnicodeToASCIIErrorCharacter = 'q';

#if defined( _DEBUG )
        m_LastIndex                                   = 0;
        m_NumberOfTimesWeHaveBeenAskedForTheSameIndex = 0;
#endif // _DEBUG
    }

    inline CDataParser( _In_reads_bytes_( buffer_size ) uint8_t const * buffer, _In_ uint64_t const buffer_size ) noexcept
    {
        m_AutomaticallyDelete          = false;
        m_Data                         = nullptr;
        m_Bytes                        = nullptr;
        m_NumberOfBytes                = 0;
        m_IsASCII                      = true;
        m_IsBigEndian                  = false;
        m_IsUCS4                       = false;
        m_UCS4Order                    = 4321;
        m_UnicodeToASCIIErrorCharacter = 'q';

#if defined( _DEBUG )
        m_LastIndex                                   = 0;
        m_NumberOfTimesWeHaveBeenAskedForTheSameIndex = 0;
#endif // _DEBUG

        (void) Initialize( buffer, buffer_size );
    }

    inline ~CDataParser()
    {
        Empty();
    }

    inline constexpr _Check_return_ std::size_t NumberOfBytesPerCharacter(void) const noexcept
    {
        if (IsTextUCS4())
        {
            return(4);
        }
        else if (IsTextASCII() == false)
        {
            return(2);
        }

        return(1);
    }

    inline constexpr void AdvanceByOneCharacter( _Inout_ CParsePoint& parse_point, _In_ uint32_t const character_parameter = 0 ) const noexcept
    {
            if ( m_Bytes == nullptr )
            {
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return;
            }

            if ( parse_point.GetIndex() >= m_NumberOfBytes )
            {
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return;
            }

            uint64_t const last_index = ( parse_point.GetIndex() + NumberOfBytesPerCharacter() );

            if ( last_index >= m_NumberOfBytes )
            {
                parse_point.SetIndex( last_index );
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return;
            }

            uint32_t character = 0;

            uint8_t const little_end = m_Bytes[ parse_point.GetIndex() ];

            if ( IsTextASCII() == false)
            {
                if ( IsTextUCS4() == false )
                {
                    // We are UNICODE, let's decipher that Endian thing

                    if ( ( parse_point.GetIndex() + 1 ) >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return;
                    }

                    uint8_t const big_end = m_Bytes[ parse_point.GetIndex() + 1 ];

                    // 2000-10-28
                    // Thanks go to Danny Smith (danny_r_smith_2001@yahoo.co.nz) for
                    // finding a bug in gcc

#if defined( _MSC_VER )
                    // We're using a commercial compiler
                    character = IsTextBigEndian() ? MAKEWORD( big_end, little_end ) : MAKEWORD( little_end, big_end );
#else
                    // This version is for the GCC compiler. The above line was too
                    // complicated for it (i.e. exposed a bug). This version does the
                    // same thing.

                    if ( IsTextBigEndian() == false)
                    {
                        // Intel Format
                        character = MAKEWORD( little_end, big_end );
                    }
                    else
                    {
                        // Sun (stupid) Format
                        character = MAKEWORD( big_end, little_end );
                    }

#endif // _MSC_VER

                    WFC_SET_LAST_ERROR( NO_ERROR );

                    if ( character_parameter == 0 )
                    {
                        parse_point.AutoIncrement( static_cast< uint8_t >( character ) );
                    }
                    else
                    {
                        parse_point.AutoIncrement( static_cast< uint8_t >( character_parameter ) );
                    }

                    // We are UNICODE so advance the byte index by 2
                    // The AutoIncrement() call above will advance the index
                    // by only one byte.

                    parse_point.SetIndex( parse_point.GetIndex() + 1 );
                    return;
                }
                else
                {
                    // The most complicated... UCS4!

                    if ( ( parse_point.GetIndex() + 3 ) >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return;
                    }

                    uint8_t const byte_2 = m_Bytes[ parse_point.GetIndex() + 1 ];
                    uint8_t const byte_3 = m_Bytes[ parse_point.GetIndex() + 2 ];
                    uint8_t const byte_4 = m_Bytes[ parse_point.GetIndex() + 3 ];

                    // Now figure out the order

                    switch( GetUCS4Order() )
                    {
                    case 4321:

                        character = ( byte_4 << 24 ) + ( byte_3 << 16 ) + ( byte_2 << 8 ) + little_end;
                        break;

                    case 1234:

                        character = ( little_end << 24 ) + ( byte_2 << 16 ) + ( byte_3 << 8 ) + byte_4;
                        break;

                    case 2143:

                        character = ( byte_2 << 24 ) + ( little_end << 16 ) + ( byte_4 << 8 ) + byte_3;
                        break;

                    case 3412:

                        character = ( byte_3 << 24 ) + ( byte_4 << 16 ) + ( little_end << 8 ) + byte_2;
                        break;

                    default:

                        WFC_SET_LAST_ERROR( ERROR_MAPPED_ALIGNMENT );
                        return;
                    }

                    WFC_SET_LAST_ERROR( NO_ERROR );

                    if ( character_parameter == 0 )
                    {
                        parse_point.AutoIncrement( static_cast< uint8_t >( character ) );
                    }
                    else
                    {
                        parse_point.AutoIncrement( static_cast< uint8_t >( character_parameter ) );
                    }

                    // We are UCS4 so advance the byte index by 2
                    // The AutoIncrement() call above will advance the index
                    // by only one byte.

                    parse_point.SetIndex( parse_point.GetIndex() + 3 );
                    return;
                }
            }

            WFC_SET_LAST_ERROR( NO_ERROR );

            if ( character_parameter == 0 )
            {
                parse_point.AutoIncrement( little_end );
            }
            else
            {
                parse_point.AutoIncrement( static_cast< uint8_t >( character_parameter ) );
            }
    }

    inline void Empty( void ) noexcept
    {
        try
        {
            if ( m_AutomaticallyDelete == true )
            {
                if ( m_Data != nullptr )
                {
                    delete m_Data;
                    m_Data = nullptr;
                }
            }

            m_AutomaticallyDelete          = false;
            m_Data                         = nullptr;
            m_Bytes                        = nullptr;
            m_NumberOfBytes                = 0;
            m_IsASCII                      = true;
            m_IsBigEndian                  = false;
            m_IsUCS4                       = false;
            m_UCS4Order                    = 4321;
            m_UnicodeToASCIIErrorCharacter = 'q';

#if defined( _DEBUG )
            m_LastIndex                                   = 0;
            m_NumberOfTimesWeHaveBeenAskedForTheSameIndex = 0;
#endif // _DEBUG
        }
        catch( ... )
        {
            // We can't do anything in here because this may be nullptr!
            return;
        }
    }

    inline constexpr _Check_return_ bool Find( _In_ CParsePoint const& parse_point, _In_ uint8_t const byte_to_find, _Inout_ CParsePoint& found_at ) const noexcept
    {
            found_at.Copy( parse_point );

            if ( m_Bytes == nullptr )
            {
                found_at.Empty();
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( false );
            }

            if ( found_at.GetIndex() >= m_NumberOfBytes )
            {
                found_at.Empty();
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( false );
            }

            while( found_at.GetIndex() < m_NumberOfBytes )
            {
                if ( m_Bytes[ found_at.GetIndex() ] == byte_to_find )
                {
                    return( true );
                }

                found_at.AutoIncrement( m_Bytes[ found_at.GetIndex() ] );
            }

            found_at.Empty();

            WFC_SET_LAST_ERROR( NO_ERROR );
            return( false );
    }

    inline constexpr _Check_return_ bool Find(_In_ CParsePoint const& parse_point, _In_ CParsePoint const& end_parse_point, _In_ wchar_t const * string_to_find, _In_ std::size_t const number_of_characters, _Inout_ CParsePoint& found_at) const noexcept
    {
            found_at.Copy(parse_point);

            if (m_Bytes == nullptr)
            {
                found_at.Empty();
                WFC_SET_LAST_ERROR(ERROR_INVALID_ADDRESS);
                return(false);
            }

            if (found_at.GetIndex() >= end_parse_point.GetIndex())
            {
                found_at.Empty();
                WFC_SET_LAST_ERROR(ERROR_ALLOTTED_SPACE_EXCEEDED);
                return(false);
            }

            if (number_of_characters < 1)
            {
                found_at.Empty();
                WFC_SET_LAST_ERROR(ERROR_ALLOTTED_SPACE_EXCEEDED);
                return(false);
            }

            uint32_t character_to_test = 0;

            while ((found_at.GetIndex() + number_of_characters) <= end_parse_point.GetIndex())
            {
                if (PeekAtCharacter(found_at, character_to_test, 0) == true)
                {
                    if (character_to_test == static_cast< uint32_t >(string_to_find[0]))
                    {
                        bool string_was_found = true;

                        uint32_t other_character = 0;

                        for (auto const loop_index : Range(number_of_characters, 1))
                        {
                            // We peek at string_length - 1 because the GetNextCharacter() call above
                            // advances the character index

                            if (PeekAtCharacter(found_at, character_to_test, loop_index) == false)
                            {
                                found_at.Empty();
                                return(false);
                            }

                            other_character = string_to_find[loop_index];

                            if (character_to_test != other_character)
                            {
                                string_was_found = false;
                                break;
                            }
                        }

                        if (string_was_found == true)
                        {
                            return(true);
                        }
                    }

                    if (GetNextCharacter(found_at, character_to_test) == false)
                    {
                        return(false);
                    }
                }
                else
                {
                    return(false);
                }
            }

            found_at.Empty();

            WFC_SET_LAST_ERROR(NO_ERROR);
            return(false);
    }

    inline _Check_return_ bool Find(_In_ CParsePoint const& parse_point, _In_ CParsePoint const& end_parse_point, _In_ std::wstring const& string_to_find, _Inout_ CParsePoint& found_at) const noexcept
    {
        return(Find(parse_point, end_parse_point, string_to_find.c_str(), string_to_find.length(), found_at));
    }

    inline _Check_return_ bool Find(_In_ CParsePoint const& parse_point, _In_ std::wstring const& string_to_find, _Inout_ CParsePoint& found_at) const noexcept
    {
        CParsePoint end_parse_point;

        end_parse_point.SetIndex(m_NumberOfBytes);

        return(Find(parse_point, end_parse_point, string_to_find, found_at));
    }

    inline _Check_return_ bool Find(_In_ CParsePoint const& parse_point, _In_ wchar_t const * string_to_find, _Inout_ CParsePoint& found_at) const noexcept
    {
        CParsePoint end_parse_point;

        end_parse_point.SetIndex(m_NumberOfBytes);

        return(Find(parse_point, end_parse_point, string_to_find, wcslen(string_to_find), found_at));
    }

    inline _Check_return_ bool Find( _In_ CParsePoint const& parse_point, _In_ CParsePoint const& end_parse_point, _In_ uint8_t const * pattern_buffer, _In_ std::size_t const pattern_length, _Inout_ CParsePoint& found_at ) const noexcept
    {
        try
        {
            found_at.Copy( parse_point );

            if ( m_Bytes == nullptr )
            {
                found_at.Empty();
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( false );
            }

            if ( found_at.GetIndex() >= end_parse_point.GetIndex())
            {
                found_at.Empty();
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( false );
            }

            // 2000-02-19
            // Thanks go to Arek Kotas (arekk@metrosoft-us.com) for finding a bug
            // here. I was using < when it should have been <=

            while( ( found_at.GetIndex() + pattern_length ) <= end_parse_point.GetIndex())
            {
                if ( ::memcmp( &m_Bytes[ found_at.GetIndex() ], pattern_buffer, pattern_length ) == I_AM_EQUAL_TO_THAT )
                {
                    return( true );
                }

                found_at.AutoIncrement( m_Bytes[ found_at.GetIndex() ] );
            }

            found_at.Empty();

            WFC_SET_LAST_ERROR( NO_ERROR );
            return( false );
        }
        catch( ... )
        {
            return( false );
        }
    }

    inline _Check_return_ bool Find(_In_ CParsePoint const& parse_point, _In_ CParsePoint const& end_parse_point, _In_ std::vector<uint8_t> const& bytes_to_find, _Inout_ CParsePoint& found_at) const noexcept
    {
        return(Find(parse_point, end_parse_point, bytes_to_find.data(), bytes_to_find.size(), found_at));
    }

    inline _Check_return_ bool Find(_In_ CParsePoint const& parse_point, _In_ std::vector<uint8_t> const& bytes_to_find, _Inout_ CParsePoint& found_at) const noexcept
    {
        CParsePoint end_parse_point;

        end_parse_point.SetIndex(m_NumberOfBytes);

        return(Find(parse_point, end_parse_point, bytes_to_find, found_at));
    }

    inline _Check_return_ bool Find(_In_ CParsePoint const& parse_point, _In_ uint8_t const * bytes_to_find, _In_ std::size_t const number_of_bytes_to_find, CParsePoint& found_at) const noexcept
    {
        CParsePoint end_parse_point;

        end_parse_point.SetIndex(m_NumberOfBytes);

        return(Find(parse_point, end_parse_point, bytes_to_find, number_of_bytes_to_find, found_at));
    }

    inline _Check_return_ bool FindNoCase( _In_ CParsePoint const& parse_point, _In_ std::wstring const& string_to_find, _Inout_ CParsePoint& found_at ) const noexcept
    {
        try
        {
            if ( m_Bytes == nullptr )
            {
                found_at.Empty();
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( false );
            }

            if ( found_at.GetIndex() >= m_NumberOfBytes )
            {
                found_at.Empty();
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( false );
            }

            std::size_t const string_length = string_to_find.length();

            if ( string_length == 0 )
            {
                found_at.Empty();
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( false );
            }

            found_at.Copy( parse_point );

            uint32_t character_to_test         = 0;
            uint32_t const first_character_of_string = tolower( string_to_find.at( 0 ) );

            while( ( found_at.GetIndex() + string_length ) <= m_NumberOfBytes )
            {
                if ( PeekAtCharacter( found_at, character_to_test, 0 ) == true )
                {
                    if ( tolower( character_to_test ) == first_character_of_string )
                    {
                        bool string_was_found = true;

                        uint32_t other_character = 0;

                        for ( auto const loop_index : Range(string_length, 1) )
                        {
                            if ( PeekAtCharacter( found_at, character_to_test, loop_index ) == false)
                            {
                                found_at.Empty();
                                return( false );
                            }

                            other_character = tolower( string_to_find.at( loop_index ) );

                            if ( other_character != tolower( character_to_test ) )
                            {
                                string_was_found = false;
                                break;
                            }
                        }

                        if ( string_was_found == true )
                        {
                            return( true );
                        }
                    }

                    if ( GetNextCharacter( found_at, character_to_test ) == false )
                    {
                        return( false );
                    }
                }
                else
                {
                    found_at.Empty();
                    return( false );
                }
            }

            found_at.Empty();
            WFC_SET_LAST_ERROR( NO_ERROR );
            return( false );
        }
        catch( ... )
        {
            return( false );
        }
    }

    inline _Check_return_ bool FindNoCase( _In_ CParsePoint const& parse_point, _In_ std::vector<uint8_t> const& bytes_to_find, _Inout_ CParsePoint& found_at ) const noexcept
    {
        try
        {
            if ( m_Bytes == nullptr )
            {
                found_at.Empty();
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( false );
            }

            if ( found_at.GetIndex() >= m_NumberOfBytes )
            {
                found_at.Empty();
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( false );
            }

            found_at.Copy( parse_point );

            auto const pattern_length = bytes_to_find.size();

            auto pattern_buffer = bytes_to_find.data();

            while( ( found_at.GetIndex() + pattern_length ) <= m_NumberOfBytes )
            {
                if ( ::_memicmp( &m_Bytes[ found_at.GetIndex() ], pattern_buffer, pattern_length ) == I_AM_EQUAL_TO_THAT)
                {
                    return( true );
                }

                found_at.AutoIncrement( m_Bytes[ found_at.GetIndex() ] );
            }

            found_at.Empty();

            WFC_SET_LAST_ERROR( NO_ERROR );
            return( false );
        }
        catch( ... )
        {
            return( false );
        }
    }

    inline _Check_return_ bool Get(_Inout_ CParsePoint& parse_point, _In_ uint32_t const length, _Inout_ std::vector<uint8_t>& bytes_to_get) const noexcept
    {
        try
        {
            bytes_to_get.clear();

            if (m_Bytes == nullptr)
            {
                WFC_SET_LAST_ERROR(ERROR_INVALID_ADDRESS);
                return(false);
            }

            // 1999-01-30 - Josh Mayfield (mayfielj@squared.com) found an off-by-one error here

            if ((parse_point.GetIndex() + length) > m_NumberOfBytes)
            {
                WFC_SET_LAST_ERROR(ERROR_ALLOTTED_SPACE_EXCEEDED);
                return(false);
            }

            for (auto const index : Range(length))
            {
                bytes_to_get.push_back(m_Bytes[parse_point.GetIndex()]);
                parse_point.AutoIncrement(m_Bytes[parse_point.GetIndex()]);
            }

            WFC_SET_LAST_ERROR(NO_ERROR);
            return(true);
        }
        catch (...)
        {
            return(false);
        }
    }

    inline _Check_return_ bool Get( _Inout_ CParsePoint& parse_point, _In_ uint32_t const length, _Out_writes_bytes_( length ) uint8_t * bytes_to_get ) const noexcept
    {
        try
        {
            if ( m_Bytes == nullptr )
            {
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( false );
            }

            // 1999-01-30 - Josh Mayfield (mayfielj@squared.com) found an off-by-one error here

            if ( ( parse_point.GetIndex() + length ) > m_NumberOfBytes )
            {
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( false );
            }

            for ( auto const index : Range(length) )
            {
                bytes_to_get[ index ] = m_Bytes[ parse_point.GetIndex() ];
                parse_point.AutoIncrement( m_Bytes[ parse_point.GetIndex() ] );
            }

            WFC_SET_LAST_ERROR( NO_ERROR );
            return( true );
        }
        catch( ... )
        {
            return( false );
        }
    }

    inline _Check_return_ bool Get( _Inout_ CParsePoint& parse_point, _In_ uint32_t const length, _Inout_ std::wstring& string_to_get ) const noexcept
    {
        try
        {
            string_to_get.clear();

            if ( m_Bytes == nullptr )
            {
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( false );
            }

            // 1999-01-30 - Josh Mayfield (mayfielj@squared.com) found an off-by-one error here

            if ( ( parse_point.GetIndex() + length ) > m_NumberOfBytes )
            {
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( false );
            }

            for ( auto const loop_index : Range(length) )
            {
                string_to_get.push_back( static_cast< TCHAR > ( m_Bytes[ parse_point.GetIndex() ] ) );
                parse_point.AutoIncrement( m_Bytes[ parse_point.GetIndex() ] );
            }

            WFC_SET_LAST_ERROR( NO_ERROR );
            return( true );
        }
        catch( ... )
        {
            return( false );
        }
    }

    void GetTextStatistics( _In_ CParsePoint& parse_point, _Inout_ CTextStatistics& statistics ) const noexcept;

    inline constexpr _Check_return_ uint8_t ReadByte( _Inout_ CParsePoint& parse_point ) const noexcept
    {
        uint8_t const byte_1 = GetAt( parse_point.GetIndex() );
        parse_point.AutoIncrement( byte_1 );

        return( byte_1 );
    }

    inline constexpr void ReadBytes( _Inout_ CParsePoint& parse_point, _Out_writes_bytes_( number_of_bytes ) uint8_t * buffer, _In_ uint64_t const number_of_bytes ) const noexcept
    {
        WFC_VALIDATE_POINTER( buffer );

        for( auto const loop_index : Range(number_of_bytes) )
        {
            buffer[ loop_index ] = GetAt( parse_point.GetIndex() );
            parse_point.AutoIncrement( buffer[ loop_index ] );
        }
    }

    inline constexpr _Check_return_ bool ReadUInt32BigEndian( _Inout_ CParsePoint& parse_point, _Inout_ uint32_t& value ) const noexcept
    {
        uint8_t const byte_1 = ReadByte( parse_point );
        uint8_t const byte_2 = ReadByte( parse_point );
        uint8_t const byte_3 = ReadByte( parse_point );
        uint8_t const byte_4 = ReadByte( parse_point );

        value = MAKE_uint32_t( byte_4, byte_3, byte_2, byte_1 );

        return( true );
    }

    inline constexpr _Check_return_ uint32_t ReadUInt32BigEndian( _Inout_ CParsePoint& parse_point ) const noexcept
    {
        uint32_t return_value = 0;

        if ( ReadUInt32BigEndian( parse_point, return_value ) == false )
        {
            return( 0 );
        }

        return( return_value );
    }

    inline constexpr _Check_return_ bool ReadtInt32BigEndian( _Inout_ CParsePoint& parse_point, _Inout_ int32_t& value ) const noexcept
    {
        uint32_t first_value = 0;

        bool const return_value = ReadUInt32BigEndian( parse_point, first_value );

        value = (int32_t) first_value;

        return( return_value );
    }

    inline constexpr _Check_return_ int32_t ReadInt32BigEndian( _Inout_ CParsePoint& parse_point ) const noexcept
    {
        int32_t return_value = 0;

        if ( ReadtInt32BigEndian( parse_point, return_value ) == false )
        {
            return( 0 );
        }

        return( return_value );
    }

    inline constexpr _Check_return_ bool ReadUInt16LittleEndian( _Inout_ CParsePoint& parse_point, _Inout_ uint16_t& value ) const noexcept
    {
        uint8_t const byte_1 = ReadByte( parse_point );
        uint8_t const byte_2 = ReadByte( parse_point );

        value = MAKE_uint16_t( byte_1, byte_2 );

        return( true );
    }

    inline constexpr _Check_return_ uint16_t ReadUInt16LittleEndian( _Inout_ CParsePoint& parse_point ) const noexcept
    {
        uint16_t return_value = 0;

        if ( ReadUInt16LittleEndian( parse_point, return_value ) == false )
        {
            return( 0 );
        }

        return( return_value );
    }

    inline constexpr _Check_return_ int16_t ReadInt16LittleEndian( _Inout_ CParsePoint& parse_point ) const noexcept
    {
        uint16_t return_value = 0;

        if ( ReadUInt16LittleEndian( parse_point, return_value ) == false )
        {
            return( 0 );
        }

        return( return_value );
    }

    inline constexpr _Check_return_ bool ReadInt16LittleEndian( _Inout_ CParsePoint& parse_point, _Inout_ int16_t& value ) const noexcept
    {
        uint8_t const byte_1 = ReadByte( parse_point );
        uint8_t const byte_2 = ReadByte( parse_point );

        value = (int16_t) MAKE_uint16_t( byte_1, byte_2 );

        return( true );
    }

    inline constexpr _Check_return_ bool ReadUInt32LittleEndian( _Inout_ CParsePoint& parse_point, _Inout_ uint32_t& value ) const noexcept
    {
        uint8_t const byte_1 = ReadByte( parse_point );
        uint8_t const byte_2 = ReadByte( parse_point );
        uint8_t const byte_3 = ReadByte( parse_point );
        uint8_t const byte_4 = ReadByte( parse_point );

        value = MAKE_uint32_t( byte_1, byte_2, byte_3, byte_4 );

        return( true );
    }

    inline constexpr _Check_return_ uint32_t ReadUInt32LittleEndian( _Inout_ CParsePoint& parse_point ) const noexcept
    {
        uint32_t return_value = 0;

        if ( ReadUInt32LittleEndian( parse_point, return_value ) == false )
        {
            return( 0 );
        }

        return( return_value );
    }

    inline constexpr _Check_return_ bool ReadInt32LittleEndian( _Inout_ CParsePoint& parse_point, _Inout_ int32_t& value ) const noexcept
    {
        uint32_t first_value = 0;

        bool const return_value = ReadUInt32LittleEndian( parse_point, first_value );

        value = static_cast<int32_t>(first_value);

        return( return_value );
    }

    inline constexpr _Check_return_ uint32_t ReadInt32LittleEndian( _Inout_ CParsePoint& parse_point ) const noexcept
    {
        int32_t return_value = 0;

        if ( ReadInt32LittleEndian( parse_point, return_value ) == false )
        {
            return( 0 );
        }

        return( return_value );
    }

    inline constexpr _Check_return_ bool ReadUInt64BigEndian( _Inout_ CParsePoint& parse_point, _Inout_ uint64_t& value ) const noexcept
    {
        uint8_t const byte_1 = ReadByte( parse_point );
        uint8_t const byte_2 = ReadByte( parse_point );
        uint8_t const byte_3 = ReadByte( parse_point );
        uint8_t const byte_4 = ReadByte( parse_point );
        uint8_t const byte_5 = ReadByte( parse_point );
        uint8_t const byte_6 = ReadByte( parse_point );
        uint8_t const byte_7 = ReadByte( parse_point );
        uint8_t const byte_8 = ReadByte( parse_point );

        value = MAKE_uint64_t( byte_8, byte_7, byte_6, byte_5, byte_4, byte_3, byte_2, byte_1 );

        return( true );
    }

    inline constexpr _Check_return_ bool ReadInt64BigEndian( _Inout_ CParsePoint& parse_point, _Inout_ int64_t& value ) const noexcept
    {
        uint64_t first_value = 0;

        bool const return_value = ReadUInt64BigEndian( parse_point, first_value );

        value = static_cast<int64_t>(first_value);

        return( return_value );
    }

    inline constexpr _Check_return_ bool ReadUInt64LittleEndian( _Inout_ CParsePoint& parse_point, _Inout_ uint64_t& value ) const noexcept
    {
        uint8_t const byte_1 = ReadByte( parse_point );
        uint8_t const byte_2 = ReadByte( parse_point );
        uint8_t const byte_3 = ReadByte( parse_point );
        uint8_t const byte_4 = ReadByte( parse_point );
        uint8_t const byte_5 = ReadByte( parse_point );
        uint8_t const byte_6 = ReadByte( parse_point );
        uint8_t const byte_7 = ReadByte( parse_point );
        uint8_t const byte_8 = ReadByte( parse_point );

        value = MAKE_uint64_t( byte_1, byte_2, byte_3, byte_4, byte_5, byte_6, byte_7, byte_8 );

        return( true );
    }

    inline constexpr _Check_return_ uint64_t ReadUInt64LittleEndian( _Inout_ CParsePoint& parse_point ) const noexcept
    {
        uint64_t return_value = 0;

        if ( ReadUInt64LittleEndian( parse_point, return_value ) == false )
        {
            return( 0 );
        }

        return( return_value );
    }

    inline constexpr _Check_return_ bool ReadInt64LittleEndian( _Inout_ CParsePoint& parse_point, _Inout_ int64_t& value ) const noexcept
    {
        uint64_t first_value = 0;

        bool const return_value = ReadUInt64LittleEndian( parse_point, first_value );

        value = static_cast<int64_t>(first_value);

        return( return_value );
    }

    inline constexpr _Check_return_ int64_t ReadInt64LittleEndian( _Inout_ CParsePoint& parse_point ) const noexcept
    {
        int64_t return_value = 0;

        if ( ReadInt64LittleEndian( parse_point, return_value ) == false )
        {
            return( 0 );
        }

        return( return_value );
    }

    inline _Check_return_ uint64_t ReadInteger( _Inout_ CParsePoint& parse_point, _In_ uint64_t const number_of_characters_in_integer ) const noexcept
    {
        uint64_t return_value = 0;

        std::wstring hex_string;

        uint32_t field_character = 0;

        for ( auto const loop_index : Range(number_of_characters_in_integer) )
        {
            (void) GetNextCharacter( parse_point, field_character );
            hex_string.push_back( static_cast<wchar_t>(field_character) );
        }

        wchar_t * stopped_at_address = nullptr;

        return_value = wcstol( hex_string.c_str(), &stopped_at_address, 10 );

        return( return_value );
    }

    inline _Check_return_ uint64_t ReadHexadecimalInteger( _Inout_ CParsePoint& parse_point, _In_ uint64_t const number_of_characters_in_integer ) const noexcept
    {
        uint64_t return_value = 0;

        std::wstring hex_string;

        uint32_t field_character = 0;

        for ( auto const loop_index : Range(number_of_characters_in_integer) )
        {
            (void) GetNextCharacter( parse_point, field_character );
            hex_string.push_back( static_cast<wchar_t>(field_character) );
        }

        wchar_t * stopped_at_address = nullptr;

        return_value = wcstol( hex_string.c_str(), &stopped_at_address, 16 );

        return( return_value );
    }

    inline _Check_return_ GUID ReadGUID( _Inout_ CParsePoint& parse_point ) const noexcept
    {
        GUID return_value{ 0 };

        auto buffer = reinterpret_cast<uint8_t *>(&return_value);

        buffer[  0 ] = ReadByte( parse_point );
        buffer[  1 ] = ReadByte( parse_point );
        buffer[  2 ] = ReadByte( parse_point );
        buffer[  3 ] = ReadByte( parse_point );
        buffer[  4 ] = ReadByte( parse_point );
        buffer[  5 ] = ReadByte( parse_point );
        buffer[  6 ] = ReadByte( parse_point );
        buffer[  7 ] = ReadByte( parse_point );
        buffer[  8 ] = ReadByte( parse_point );
        buffer[  9 ] = ReadByte( parse_point );
        buffer[ 10 ] = ReadByte( parse_point );
        buffer[ 11 ] = ReadByte( parse_point );
        buffer[ 12 ] = ReadByte( parse_point );
        buffer[ 13 ] = ReadByte( parse_point );
        buffer[ 14 ] = ReadByte( parse_point );
        buffer[ 15 ] = ReadByte( parse_point );

        return( return_value );
    }

    inline constexpr _Check_return_ uint8_t GetAt( _In_ uint64_t const byte_index ) const noexcept
    {
#if defined( _DEBUG )

            if ( byte_index == m_LastIndex )
            {
                const_cast< uint64_t& > ( m_NumberOfTimesWeHaveBeenAskedForTheSameIndex )++;

                if ( m_NumberOfTimesWeHaveBeenAskedForTheSameIndex > 25 )
                {
                    // We've asked for this same stinking character 25 times already!
                    // Looks like our caller is stuck in an endless loop

                    ASSERT( false );
                }
            }
            else
            {
                const_cast< uint64_t& > ( m_LastIndex                                   ) = byte_index;
                const_cast< uint64_t& > ( m_NumberOfTimesWeHaveBeenAskedForTheSameIndex ) = 0;
            }

#endif // _DEBUG

            if ( m_Bytes == nullptr )
            {
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( 0 );
            }

            if ( byte_index >= m_NumberOfBytes )
            {
                return( 0 );
            }

            return( m_Bytes[ byte_index ] );
    }

    // Take a look a the current character without advancing the parse point

    inline constexpr _Check_return_ DWORD GetCharacter( _In_ CParsePoint const& const_parse_point, _In_ uint32_t const number_of_characters_ahead = 0 ) const noexcept
    {
            if ( m_Bytes == nullptr )
            {
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( 0 );
            }

            CParsePoint parse_point(const_parse_point);

            if ( parse_point.GetIndex() >= m_NumberOfBytes )
            {
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( 0 );
            }

            uint64_t number_of_bytes_per_character = 1;

            if ( IsTextUCS4() )
            {
                number_of_bytes_per_character = 4;
            }
            else if ( IsTextASCII() == false )
            {
                number_of_bytes_per_character = 2;
            }

            uint64_t number_of_bytes_to_skip_ahead = number_of_bytes_per_character * number_of_characters_ahead;

            // We must now skip the rest of the bytes that make up one character

            number_of_bytes_to_skip_ahead += ( number_of_bytes_per_character - 1 );

            uint64_t const last_index = ( parse_point.GetIndex() + number_of_bytes_to_skip_ahead );

            if ( last_index >= m_NumberOfBytes )
            {
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( 0 );
            }

            number_of_bytes_per_character = 0;

            uint8_t little_end = 0;

            do
            {
                little_end = m_Bytes[ parse_point.GetIndex() ];
                parse_point.AutoIncrement( little_end );
                number_of_bytes_per_character++;
            }
            while( number_of_bytes_per_character < number_of_bytes_to_skip_ahead );

            uint32_t character = 0;

            if ( IsTextASCII() == false)
            {
                if ( IsTextUCS4() == false )
                {
                    // We are UNICODE, let's decipher that Endian thing

                    if ( parse_point.GetIndex() >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return( 0 );
                    }

                    uint8_t const big_end = m_Bytes[ parse_point.GetIndex() ];
                    parse_point.AutoIncrement( big_end );

#if defined( _MSC_VER )
                    // We're using a commercial compiler
                    character = IsTextBigEndian() ? MAKEWORD( big_end, little_end ) : MAKEWORD( little_end, big_end );
#else
                    // This version is for the GCC compiler. The above line was too
                    // complicated for it (i.e. exposed a bug). This version does the
                    // same thing.
                    if ( IsTextBigEndian() == false)
                    {
                        // Intel Format
                        character = MAKEWORD( little_end, big_end );
                    }
                    else
                    {
                        // Sun Format
                        character = MAKEWORD( big_end, little_end );
                    }
#endif

                    WFC_SET_LAST_ERROR( NO_ERROR );

#if ! defined( UNICODE )
                    // ASCII build, check for UNICODE translation problem

                    if ( character > 255 )
                    {
                        character = GetUnicodeToASCIITranslationFailureCharacter();
                    }
#endif // UNICODE

                    return( character );
                }
                else
                {
                    // The most complicated... UCS4!

                    if ( parse_point.GetIndex() >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return( 0 );
                    }

                    uint8_t const byte_2 = m_Bytes[ parse_point.GetIndex() ];
                    parse_point.AutoIncrement( byte_2 );

                    if ( parse_point.GetIndex() >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return( 0 );
                    }

                    uint8_t const byte_3 = m_Bytes[ parse_point.GetIndex() ];
                    parse_point.AutoIncrement( byte_3 );

                    if ( parse_point.GetIndex() >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return( 0 );
                    }

                    uint8_t const byte_4 = m_Bytes[ parse_point.GetIndex() ];
                    parse_point.AutoIncrement( byte_4 );

                    // Now figure out the order

                    switch( GetUCS4Order() )
                    {
                    case 4321:

                        character = ( byte_4 << 24 ) + ( byte_3 << 16 ) + ( byte_2 << 8 ) + little_end;
                        break;

                    case 1234:

                        character = ( little_end << 24 ) + ( byte_2 << 16 ) + ( byte_3 << 8 ) + byte_4;
                        break;

                    case 2143:

                        character = ( byte_2 << 24 ) + ( little_end << 16 ) + ( byte_4 << 8 ) + byte_3;
                        break;

                    case 3412:

                        character = ( byte_3 << 24 ) + ( byte_4 << 16 ) + ( little_end << 8 ) + byte_2;
                        break;

                    default:

                        WFC_SET_LAST_ERROR( ERROR_MAPPED_ALIGNMENT );
                        return( 0 );
                    }

                    WFC_SET_LAST_ERROR( NO_ERROR );

#if ! defined( UNICODE )
                    // ASCII build, check for UNICODE translation problem

                    if ( character > 255 )
                    {
                        character = GetUnicodeToASCIITranslationFailureCharacter();
                    }
#endif // UNICODE
                    return( character );
                }
            }

            WFC_SET_LAST_ERROR( NO_ERROR );
            return( little_end );
    }

    inline _Check_return_ bool GetField( _Inout_ CParsePoint& parse_point, _Inout_ std::vector<uint8_t>& field, _In_ wchar_t const field_separator ) const noexcept
    {
        field.clear();

        uint32_t field_character = 0;

        while( GetNextCharacter( parse_point, field_character ) == true )
        {
            if ( field_character == field_separator )
            {
                return( true );
            }

            (void) field.push_back( static_cast<uint8_t>(field_character) );
        }

        if ( field.empty() == false )
        {
            return( true );
        }

        return( false );
    }

    inline _Check_return_ bool GetField( _Inout_ CParsePoint& parse_point, _Inout_ std::wstring& field, _In_ wchar_t const field_separator ) const noexcept
    {
        field.clear();

        uint32_t field_character = 0;

        while( GetNextCharacter( parse_point, field_character ) == true )
        {
            if ( field_character == field_separator )
            {
                return( true );
            }

            field.push_back( field_character );
        }

        return (field.empty() == true ? false : true);
    }

    inline constexpr _Check_return_ bool GetNextCharacter( _Inout_ CParsePoint& parse_point, _Inout_ uint32_t& character ) const noexcept
    {
            if ( m_Bytes == nullptr )
            {
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( false );
            }

            if ( parse_point.GetIndex() >= m_NumberOfBytes )
            {
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( false );
            }

            uint8_t little_end = m_Bytes[ parse_point.GetIndex() ];
            parse_point.AutoIncrement( little_end );

            if ( IsTextASCII() == false)
            {
                if ( IsTextUCS4() == false )
                {
                    // We are UNICODE, let's decipher that Endian thing

                    if ( parse_point.GetIndex() >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return( false );
                    }

                    uint8_t const big_end = m_Bytes[ parse_point.GetIndex() ];
                    parse_point.AutoIncrement( big_end );

#if defined( _MSC_VER )
                    // We're using a commercial compiler
                    character = IsTextBigEndian() ? MAKEWORD( big_end, little_end ) : MAKEWORD( little_end, big_end );
#else
                    // This version is for the GCC compiler. The above line was too
                    // complicated for it (i.e. exposed a bug). This version does the
                    // same thing.

                    if ( IsTextBigEndian() == false)
                    {
                        // Intel Format

                        character = MAKEWORD( little_end, big_end );
                    }
                    else
                    {
                        character = MAKEWORD( big_end, little_end );
                    }
#endif

                    WFC_SET_LAST_ERROR( NO_ERROR );

#if ! defined( UNICODE )
                    // ASCII build, check for UNICODE translation problem

                    if ( character > 255 )
                    {
                        character = GetUnicodeToASCIITranslationFailureCharacter();
                    }
#endif // UNICODE

                    return( true );
                }
                else
                {
                    // The most complicated... UCS4!

                    if ( parse_point.GetIndex() >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return( false );
                    }

                    uint8_t byte_2 = m_Bytes[ parse_point.GetIndex() ];
                    parse_point.AutoIncrement( byte_2 );

                    if ( parse_point.GetIndex() >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return( false );
                    }

                    uint8_t byte_3 = m_Bytes[ parse_point.GetIndex() ];
                    parse_point.AutoIncrement( byte_3 );

                    if ( parse_point.GetIndex() >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return( false );
                    }

                    uint8_t byte_4 = m_Bytes[ parse_point.GetIndex() ];
                    parse_point.AutoIncrement( byte_4 );

                    // Now figure out the order

                    switch( GetUCS4Order() )
                    {
                    case 4321:

                        character = ( byte_4 << 24 ) + ( byte_3 << 16 ) + ( byte_2 << 8 ) + little_end;
                        break;

                    case 1234:

                        character = ( little_end << 24 ) + ( byte_2 << 16 ) + ( byte_3 << 8 ) + byte_4;
                        break;

                    case 2143:

                        character = ( byte_2 << 24 ) + ( little_end << 16 ) + ( byte_4 << 8 ) + byte_3;
                        break;

                    case 3412:

                        character = ( byte_3 << 24 ) + ( byte_4 << 16 ) + ( little_end << 8 ) + byte_2;
                        break;

                    default:

                        WFC_SET_LAST_ERROR( ERROR_MAPPED_ALIGNMENT );
                        character = 0;
                        return( false );
                    }

                    WFC_SET_LAST_ERROR( NO_ERROR );

#if ! defined( UNICODE )
                    // ASCII build, check for UNICODE translation problem

                    if ( character > 255 )
                    {
                        character = GetUnicodeToASCIITranslationFailureCharacter();
                    }
#endif // UNICODE

                    return( true );
                }
            }

            character = little_end;
            WFC_SET_LAST_ERROR( NO_ERROR );
            return( true );
    }

    inline constexpr _Check_return_ bool SkipLines(_Inout_ CParsePoint& parse_point, _In_ int const number_of_lines) const noexcept
    {
        int line_count = 0;

        uint32_t line_character = 0;

        while (line_count < number_of_lines  )
        {
            if (GetNextCharacter(parse_point, line_character) == false)
            {
                break;
            }

            if (line_character == CARRIAGE_RETURN)
            {
                line_character = GetCharacter(parse_point);

                if (line_character == LINE_FEED)
                {
                    (void) GetNextCharacter(parse_point, line_character);
                    line_count++;
                }
            }
            else if (line_character == LINE_FEED)
            {
                line_count++;
            }
        }

        return(line_count == number_of_lines);
    }

    inline _Check_return_ bool GetLine( _Inout_ CParsePoint& parse_point, _Inout_ std::wstring& line ) const noexcept
    {
        line.clear();

        uint32_t line_character = 0;

        while( GetNextCharacter( parse_point, line_character ) == true )
        {
            if ( line_character == CARRIAGE_RETURN )
            {
                // Now take a look a the current character without advancing the parse point

                line_character = GetCharacter( parse_point );

                if ( line_character == LINE_FEED )
                {
                    if ( GetNextCharacter( parse_point, line_character ) == false )
                    {
                        return( true );
                    }
                }

                return( true );
            }
            else if ( line_character == LINE_FEED )
            {
                return( true );
            }

            line.push_back( line_character );
        }

        if ( line.empty() == false )
        {
            return( true );
        }

        return( false );
    }

    inline _Check_return_ bool GetLine(_Inout_ CParsePoint& parse_point, _Inout_ std::vector<uint8_t>& line) const noexcept
    {
        line.clear();

        uint32_t line_character = 0;

        while (GetNextCharacter(parse_point, line_character) == true)
        {
            if (line_character == CARRIAGE_RETURN or line_character == 0x00)
            {
                // Now take a look a the current character without advancing the parse point

                if (PeekNextCharacter(parse_point, line_character) == true)
                {
                    if (line_character == LINE_FEED)
                    {
                        if (GetNextCharacter(parse_point, line_character) == false)
                        {
                            return(false);
                        }
                    }
                }

                return(true);
            }
            else if (line_character == LINE_FEED or line_character == 0x00)
            {
                return(true);
            }

            line.push_back(static_cast<uint8_t>(line_character));
        }

        if (line.empty() == false)
        {
            return(true);
        }

        return(false);
    }

    inline _Check_return_ bool GetLine( _Inout_ CParsePoint& parse_point, _Inout_ uint8_t const *& buffer, _Inout_ uint64_t& length ) const noexcept
    {
        buffer = &m_Bytes[ parse_point.GetIndex() ];
        length = 0;

        uint32_t line_character = 0;

        while( GetNextCharacter( parse_point, line_character ) == true )
        {
            if ( line_character == CARRIAGE_RETURN )
            {
                // Now take a look a the current character without advancing the parse point

                if ( PeekNextCharacter( parse_point, line_character ) == true )
                {
                    if ( line_character == LINE_FEED or line_character == 0x00 )
                    {
                        if ( GetNextCharacter( parse_point, line_character ) == false )
                        {
                            return( false );
                        }
                    }
                }

                return( true );
            }
            else if ( line_character == LINE_FEED or line_character == 0x00 )
            {
                return( true );
            }

            length++;
        }

        if ( length == 0 )
        {
            buffer = nullptr;
            return( false );
        }

        return( true );
    }

    inline constexpr _Check_return_ uint32_t GetUCS4Order( void ) const noexcept
    {
        return( m_UCS4Order );
    }

    inline constexpr _Check_return_ uint8_t GetUnicodeToASCIITranslationFailureCharacter( void ) const noexcept
    {
        return( m_UnicodeToASCIIErrorCharacter );
    }

    inline constexpr _Check_return_ uint64_t GetSize( void ) const noexcept
    {
        return( m_NumberOfBytes );
    }

    inline _Check_return_ bool GetUntilAndIncluding( _Inout_ CParsePoint& parse_point, _In_ uint8_t const termination_byte, _Inout_ std::vector<uint8_t>& bytes_to_get ) const noexcept
    {
        try
        {
            bytes_to_get.clear();

            if ( m_Bytes == nullptr )
            {
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( false );
            }

            if ( parse_point.GetIndex() >= m_NumberOfBytes )
            {
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( false );
            }

            uint8_t byte_to_test = 0;

            do
            {
                if ( parse_point.GetIndex() < m_NumberOfBytes )
                {
                    byte_to_test = m_Bytes[ parse_point.GetIndex() ];
                    (void) bytes_to_get.push_back( byte_to_test );
                    parse_point.AutoIncrement( byte_to_test );
                }
                else
                {
                    WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                    return( false );
                }
            }
            while( byte_to_test != termination_byte );

            WFC_SET_LAST_ERROR( NO_ERROR );
            return( true );
        }
        catch( ... )
        {
            return( false );
        }
    }

    inline _Check_return_ bool GetUntilAndIncluding(_Inout_ CParsePoint& parse_point, _In_ uint8_t const termination_byte, _Inout_ std::wstring& string_to_get) const noexcept
    {
        // 1999-07-01 - Optimized this routine

        try
        {
            // Start out with an empty string
            string_to_get.clear();

            if (m_Bytes == nullptr)
            {
                WFC_SET_LAST_ERROR(ERROR_INVALID_ADDRESS);
                return(false);
            }

            if (parse_point.GetIndex() >= m_NumberOfBytes)
            {
                WFC_SET_LAST_ERROR(ERROR_ALLOTTED_SPACE_EXCEEDED);
                return(false);
            }

            uint8_t byte_to_test = 0;

            TCHAR temp_buffer[257];
            temp_buffer[256] = 0x00; // zero Terminate

            int temp_buffer_index = 0;

            do
            {
                if (parse_point.GetIndex() < m_NumberOfBytes)
                {
                    byte_to_test = m_Bytes[parse_point.GetIndex()];

                    temp_buffer[temp_buffer_index] = byte_to_test;
                    temp_buffer_index++;

                    if (temp_buffer_index == 256)
                    {
                        string_to_get.append(temp_buffer, 256);
                        temp_buffer_index = 0;
                    }

                    parse_point.AutoIncrement(byte_to_test);
                }
                else
                {
                    string_to_get.clear();
                    WFC_SET_LAST_ERROR(ERROR_ALLOTTED_SPACE_EXCEEDED);
                    return(false);
                }
            } while (byte_to_test != termination_byte);

            if (temp_buffer_index < 256 and temp_buffer_index > 0)
            {
                string_to_get.append(temp_buffer, temp_buffer_index);
            }

            WFC_SET_LAST_ERROR(NO_ERROR);
            return(true);
        }
        catch (...)
        {
            return(false);
        }
    }

    // 1998-11-21, New for Release 38
    // The following method was added by Jeff Barczewski (jb@snowflight.com)
    // He added to fix a bug in the parsing of comment sections in XML
    inline _Check_return_ bool GetUntilAndIncluding(_Inout_ CParsePoint& parse_point, _In_ std::wstring const& termination_characters, _Inout_ std::wstring& string_to_get) const noexcept
    {
        try
        {
            string_to_get.clear();

            if (m_Bytes == nullptr)
            {
                WFC_SET_LAST_ERROR(ERROR_INVALID_ADDRESS);
                return(false);
            }

            if (parse_point.GetIndex() >= m_NumberOfBytes)
            {
                WFC_SET_LAST_ERROR(ERROR_ALLOTTED_SPACE_EXCEEDED);
                return(false);
            }

            std::vector<uint8_t> bytes;

            uint32_t character = 0;

            std::size_t const termination_characters_length = termination_characters.length();

#if defined( CSTRING_APPENDING_IS_FAST )

            while (GetNextCharacter(parse_point, character) == true)
            {
#if ! defined( UNICODE )
                // ASCII build, check for UNICODE translation problem

                if (character > 255)
                {
                    character = GetUnicodeToASCIITranslationFailureCharacter();
                }
#endif // UNICODE

                string_to_get += static_cast< TCHAR >(character);

                if (string_to_get.GetLength() >= termination_characters_length)
                {
                    if (string_to_get.Right(termination_characters_length).Compare(termination_characters) == I_AM_EQUAL_TO_THAT)
                    {
                        return(true);
                    }
                }
            }

#else // CSTRING_APPENDING_IS_FAST

            std::vector<uint8_t> byte_array;

            while (GetNextCharacter(parse_point, character) == true)
            {
#if ! defined( UNICODE )
                // ASCII build, check for UNICODE translation problem

                if (character > 255)
                {
                    character = GetUnicodeToASCIITranslationFailureCharacter();
                }

                byte_array.Add(static_cast< uint8_t >(character));
#else // UNICODE

                (void)byte_array.push_back(LOBYTE(character));
                (void)byte_array.push_back(HIBYTE(character));

#endif // UNICODE

                if (byte_array.size() >= termination_characters_length)
                {
                    auto const address = byte_array.data();
                    uint8_t const * address_to_compare = &address[byte_array.size() - (termination_characters_length * sizeof(TCHAR))];

                    // Don't forget to take into account that we may be a UNICODE build so we
                    // need to multiply the number of characters by the number of bytes per
                    // character to get the memcmp() to work properly.

                    if (::memcmp(address_to_compare, termination_characters.c_str(), termination_characters_length * sizeof(wchar_t)) == I_AM_EQUAL_TO_THAT)
                    {
#if defined( UNICODE )
                        (void)byte_array.push_back(0);
#endif // UNICODE
                        (void)byte_array.push_back(0);
                        string_to_get.assign(reinterpret_cast<wchar_t const *>(byte_array.data()));
                        return(true);
                    }
                }
            }

#endif // CSTRING_APPENDING_IS_FAST

            WFC_SET_LAST_ERROR(ERROR_ALLOTTED_SPACE_EXCEEDED);
            return(false);
        }
        catch (...)
        {
            return(false);
        }
    }

    inline _Check_return_ bool GetUntilAndIncluding( _Inout_ CParsePoint& parse_point, _In_ std::vector<uint8_t> const& termination_bytes, _Inout_ std::wstring& string_to_get ) const noexcept
    {
        try
        {
            string_to_get.clear();

            if ( m_Bytes == nullptr )
            {
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( false );
            }

            if ( parse_point.GetIndex() >= m_NumberOfBytes )
            {
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( false );
            }

            wchar_t character_to_test = 0;

            auto termination_bytes_p = termination_bytes.data();

            std::size_t const termination_bytes_length = termination_bytes.size();

            do
            {
                if ( ( parse_point.GetIndex() + termination_bytes_length - 1 ) < m_NumberOfBytes )
                {
                    character_to_test = m_Bytes[ parse_point.GetIndex() ];
                    string_to_get.push_back( character_to_test );
                    parse_point.AutoIncrement( static_cast< uint8_t >( character_to_test ) );
                }
                else
                {
                    WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                    return( false );
                }
            }
            while(I_AM_EQUAL_TO_THAT != ::memcmp( &m_Bytes[ parse_point.GetIndex() ], termination_bytes_p, termination_bytes_length ) );

            for( auto const loop_index : Range(termination_bytes_length) )
            {
                //move to the end of the string
                character_to_test = m_Bytes[ parse_point.GetIndex() ];
                string_to_get.push_back( character_to_test );
                parse_point.AutoIncrement( static_cast< uint8_t >( character_to_test ) );
            }

            WFC_SET_LAST_ERROR( NO_ERROR );

            return( true );
        }
        catch( ... )
        {
            return( false );
        }
    }

    inline _Check_return_ bool Initialize(_In_ std::vector<uint8_t> const * data ) noexcept
    {
        WFC_VALIDATE_POINTER(data);

        try
        {
            Empty();

            m_AutomaticallyDelete = false;
            m_Data = nullptr;

            WFC_SET_LAST_ERROR(NO_ERROR);

            m_Bytes = data->data();
            m_NumberOfBytes = data->size();

            return(true);
        }
        catch (...)
        {
            return(false);
        }
    }

    inline _Check_return_ bool Initialize( _Inout_ std::vector<uint8_t> * data, _In_ bool const automatically_delete = false ) noexcept
    {
        WFC_VALIDATE_POINTER( data );

        try
        {
            Empty();

            m_AutomaticallyDelete = automatically_delete;
            m_Data                = data;

            WFC_SET_LAST_ERROR( NO_ERROR );

            if ( m_Data == nullptr )
            {
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( false );
            }

            m_Bytes         = data->data();
            m_NumberOfBytes = data->size();

            return( true );
        }
        catch( ... )
        {
            return( false );
        }
    }

    inline _Check_return_ bool Initialize( _In_reads_bytes_( number_of_bytes ) uint8_t const * data, _In_ uint64_t const number_of_bytes ) noexcept
    {
        WFC_VALIDATE_POINTER( data );

        try
        {
            Empty();

            WFC_SET_LAST_ERROR( NO_ERROR );

            if ( data == nullptr or number_of_bytes == 0 )
            {
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( false );
            }

            m_Bytes         = data;
            m_NumberOfBytes = number_of_bytes;

            return( true );
        }
        catch( ... )
        {
            return( false );
        }
    }

    inline _Check_return_ bool Initialize(_In_ std::vector<std::wstring> const& strings) noexcept
    {
        try
        {
            std::vector<uint8_t> * bytes_p = nullptr;

            try
            {
                bytes_p = new std::vector<uint8_t>;
            }
            catch (...)
            {
                bytes_p = nullptr;
            }

            if (bytes_p == nullptr)
            {
                return(false);
            }

            for (auto const& the_string : strings)
            {
                for (auto const& character : the_string)
                {
                    (void)bytes_p->push_back(static_cast<uint8_t>(character));
                }
            }

            (void)Initialize(bytes_p, true);

            return(true);
        }
        catch (...)
        {
            return(false);
        }
    }

    inline constexpr _Check_return_ bool IsTextASCII( void ) const noexcept     { return( ( m_IsASCII     == false ) ? false : ( m_IsUCS4 == true ) ? false : true ); }
    inline constexpr _Check_return_ bool IsTextBigEndian( void ) const noexcept { return( ( m_IsBigEndian == false ) ? false : true ); }
    inline constexpr _Check_return_ bool IsTextUCS4( void ) const noexcept      { return( ( m_IsUCS4      == false ) ? false : true ); }
    inline constexpr _Check_return_ uint8_t const * GetData( void ) const noexcept { return( m_Bytes ); }

    inline constexpr _Check_return_ bool PeekAtCharacter( _In_ CParsePoint const& const_parse_point, _Inout_ uint32_t& character, _In_ uint64_t const number_of_characters_ahead = 1 ) const noexcept
    {
            if ( m_Bytes == nullptr )
            {
                WFC_SET_LAST_ERROR( ERROR_INVALID_ADDRESS );
                return( false );
            }

            CParsePoint parse_point( const_parse_point );

            if ( parse_point.GetIndex() >= m_NumberOfBytes )
            {
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( false );
            }

            uint32_t number_of_bytes_per_character = 1;

            if ( IsTextUCS4() )
            {
                number_of_bytes_per_character = 4;
            }
            else if ( IsTextASCII() == false )
            {
                number_of_bytes_per_character = 2;
            }

            // We must calculate the number of bytes to skip until we reach
            // the first byte of the desired character

            uint64_t const number_of_bytes_to_skip_ahead = number_of_bytes_per_character * number_of_characters_ahead;
            uint64_t const last_index = ( parse_point.GetIndex() + number_of_bytes_to_skip_ahead );

            if ( last_index >= m_NumberOfBytes )
            {
                WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                return( false );
            }

            // Reuse the variable since we no longer need it
            number_of_bytes_per_character = 0;

            uint8_t little_end = m_Bytes[ parse_point.GetIndex() ];

            while( number_of_bytes_per_character <= number_of_bytes_to_skip_ahead )
            {
                little_end = m_Bytes[ parse_point.GetIndex() ];
                parse_point.AutoIncrement( little_end );
                number_of_bytes_per_character++;
            }

            // Get the first byte of the character

            if ( IsTextASCII() == false )
            {
                if ( IsTextUCS4() == false )
                {
                    // We are UNICODE, let's decipher that Endian thing

                    if ( parse_point.GetIndex() >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return( false );
                    }

                    uint8_t const big_end = m_Bytes[ parse_point.GetIndex() ];

                    character = IsTextBigEndian() ? MAKEWORD( big_end, little_end ) : MAKEWORD( little_end, big_end );

                    WFC_SET_LAST_ERROR( NO_ERROR );
                    return( true );
                }
                else
                {
                    // The most complicated... UCS4!

                    if ( parse_point.GetIndex() >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return( false );
                    }

                    uint8_t byte_2 = m_Bytes[ parse_point.GetIndex() ];
                    parse_point.AutoIncrement( byte_2 );

                    if ( parse_point.GetIndex() >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return( false );
                    }

                    uint8_t byte_3 = m_Bytes[ parse_point.GetIndex() ];
                    parse_point.AutoIncrement( byte_3 );

                    if ( parse_point.GetIndex() >= m_NumberOfBytes )
                    {
                        WFC_SET_LAST_ERROR( ERROR_ALLOTTED_SPACE_EXCEEDED );
                        return( false );
                    }

                    uint8_t byte_4 = m_Bytes[ parse_point.GetIndex() ];

                    // Shouldn't there be an AutoIncrement call here????? Write a test for this

                    // Now figure out the order

                    switch( GetUCS4Order() )
                    {
                    case 4321:

                        character = ( byte_4 << 24 ) + ( byte_3 << 16 ) + ( byte_2 << 8 ) + little_end;
                        break;

                    case 1234:

                        character = ( little_end << 24 ) + ( byte_2 << 16 ) + ( byte_3 << 8 ) + byte_4;
                        break;

                    case 2143:

                        character = ( byte_2 << 24 ) + ( little_end << 16 ) + ( byte_4 << 8 ) + byte_3;
                        break;

                    case 3412:

                        character = ( byte_3 << 24 ) + ( byte_4 << 16 ) + ( little_end << 8 ) + byte_2;
                        break;

                    default:

                        WFC_SET_LAST_ERROR( ERROR_MAPPED_ALIGNMENT );
                        character = 0;
                        return( false );
                    }

                    WFC_SET_LAST_ERROR( NO_ERROR );
                    return( true );
                }
            }

            character = little_end;
            WFC_SET_LAST_ERROR( NO_ERROR );
            return( true );
    }

    inline constexpr _Check_return_ uint32_t PeekCharacter( _In_ CParsePoint const& const_parse_point, _In_ uint64_t const number_of_characters_ahead ) const noexcept
    {
        uint32_t return_value = 0;
        (void) PeekAtCharacter(const_parse_point, return_value, number_of_characters_ahead );
        return( return_value );
    }

    inline constexpr _Check_return_ bool PeekNextCharacter( _In_ CParsePoint const& const_parse_point, _Out_ uint32_t& character ) const noexcept
    {
        character = 0;
        CParsePoint peek_point(const_parse_point);
        return( GetNextCharacter( peek_point, character ) );
    }

    inline constexpr _Check_return_ bool PeekAtByte(_In_ CParsePoint const& const_parse_point, _Out_ uint8_t& value) const noexcept
    {
        uint64_t const index = const_parse_point.GetIndex();

        if (index >= GetSize())
        {
            value = 0;
            return(false);
        }

        value = GetAt(index);

        return(true);
    }

    inline constexpr _Check_return_ bool SetTextToASCII( _In_ bool const text_is_ascii = true ) noexcept
    {
        m_IsASCII = ( text_is_ascii == false ) ? false : true;
        m_IsUCS4 = false;
        return( true );
    }

    inline constexpr _Check_return_ bool SetTextToBigEndian( _In_ bool const unicode_is_big_endian = true ) noexcept
    {
        m_IsBigEndian = ( unicode_is_big_endian == false ) ? false : true;
        return( true );
    }

    inline constexpr _Check_return_ bool SetTextToUCS4( _In_ bool const text_is_ucs4 = true ) noexcept
    {
        if ( text_is_ucs4 == false )
        {
            m_IsUCS4 = false;
        }
        else
        {
            m_IsASCII = false;
            m_IsUCS4  = true;
        }

        return( true );
    }

    inline constexpr _Check_return_ bool SetUCS4Order( _In_ uint32_t const order = 4321 ) noexcept
    {
        switch( order )
        {
        case 4321:
        case 1234:
        case 2143:
        case 3412:

            m_UCS4Order = order;
            return( true );

        default:

            m_UCS4Order = 4321;
            return( false );
        }
    }

    inline constexpr void DetectText( _Inout_ CParsePoint& p ) noexcept
    {
        uint8_t const byte_1 = ReadByte( p );
        uint8_t const byte_2 = ReadByte( p );

        if ( byte_1 == 0xFF and byte_2 == 0xFE )
        {
            (void) SetTextToASCII( false );
            (void) SetTextToBigEndian( false );
            p.SetIndex( 2 );
            return;
        }

        if ( byte_1 == 0xFE and byte_2 == 0xFF )
        {
            (void) SetTextToASCII( false );
            (void) SetTextToBigEndian( true );
            p.SetIndex( 2 );
            return;
        }

        uint8_t const byte_3 = ReadByte( p );

        if ( byte_1 == 0xEF and
            byte_2 == 0xBB and
            byte_3 == 0xBF )
        {
            (void) SetTextToASCII( true );
            p.SetIndex( 3 );
            return;
        }

        // No easy signatures...

        uint8_t const byte_4 = ReadByte( p );

        if ( Win32FoundationClasses::wfc_is_ascii( byte_1 ) == true and
            Win32FoundationClasses::wfc_is_ascii( byte_2 ) == true and
            Win32FoundationClasses::wfc_is_ascii( byte_3 ) == true and
            Win32FoundationClasses::wfc_is_ascii( byte_4 ) == true)
        {
            // all 4 are ascii
            (void) SetTextToASCII( true );
            p.SetIndex( 0 );
            return;
        }

        if ( byte_2 == 0x00 and
            byte_4 == 0x00 and
            Win32FoundationClasses::wfc_is_ascii( byte_1 ) == true and
            Win32FoundationClasses::wfc_is_ascii( byte_3 ) == true)
        {
            // UTF-16, Big Endian
            (void) SetTextToASCII( false );
            (void) SetTextToBigEndian( false );
            p.SetIndex( 0 );
            return;
        }

        if ( byte_1 == 0x00 and
            Win32FoundationClasses::wfc_is_ascii( byte_2 ) == true and
            byte_3 == 0x00 and
            Win32FoundationClasses::wfc_is_ascii( byte_4 ) == true)
        {
            // UTF-16, Big Endian
            (void) SetTextToASCII( false );
            (void) SetTextToBigEndian( true );
            p.SetIndex( 0 );
            return;
        }

        if ( byte_1 == 0x00 and
            byte_2 == 0x00 and
            byte_3 == 0x00 and
            byte_4 != 0x00 )
        {
            // UCS-4 Big Endian
            (void) SetTextToUCS4( true );
            (void) SetUCS4Order( 1234 );
            p.SetIndex( 0 );
            return;
        }

        if ( byte_1 != 0x00 and
            byte_2 == 0x00 and
            byte_3 == 0x00 and
            byte_4 == 0x00 )
        {
            // UCS-4 Little Endian
            (void) SetTextToUCS4( true );
            (void) SetUCS4Order( 4321 );
            p.SetIndex( 0 );
            return;
        }

        if ( byte_1 == 0x00 and
            byte_2 == 0x00 and
            byte_3 != 0x00 and
            byte_4 == 0x00 )
        {
            // UCS-4 Little Endian
            (void) SetTextToUCS4( true );
            (void) SetUCS4Order( 2143 );
            p.SetIndex( 0 );
            return;
        }

        if ( byte_1 == 0x00 and
            byte_2 != 0x00 and
            byte_3 == 0x00 and
            byte_4 == 0x00 )
        {
            // UCS-4 Little Endian
            (void) SetTextToUCS4( true );
            (void) SetUCS4Order( 3412 );
            p.SetIndex( 0 );
            return;
        }

        // Default to ASCII
        (void) SetTextToASCII( true );
        p.SetIndex( 0 );
    }

    inline constexpr void SetUnicodeToASCIITranslationFailureCharacter( _In_ uint8_t const ascii_character ) noexcept
    {
        _ASSERTE( ascii_character != 0x00 );
        m_UnicodeToASCIIErrorCharacter = ascii_character;
    }

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

    virtual void  Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class BinaryReader
{
public:

    CDataParser Parser;
    CParsePoint Position;

    BinaryReader(_In_ BinaryReader const&) = delete;
    _Check_return_ BinaryReader& operator=(_In_ BinaryReader const&) = delete;

    inline constexpr BinaryReader() noexcept {};
    ~BinaryReader(){};

    inline constexpr _Check_return_ uint8_t ReadByte( void ) noexcept
    {
        return( Parser.ReadByte( Position ) );
    }

    inline constexpr _Check_return_ uint8_t ReadInt8( void ) noexcept
    {
        return( Parser.ReadByte( Position ) );
    }

    inline constexpr void ReadBytes(_Out_ uint8_t * buffer, _In_ uint64_t const number_of_bytes ) noexcept
    {
        if ( buffer == nullptr )
        {
            return;
        }

        Parser.ReadBytes( Position, buffer, number_of_bytes );
    }

    inline _Check_return_ GUID ReadGUID( void ) noexcept
    {
#pragma warning( push )
#pragma warning( disable: 6031 )
        return( Parser.ReadGUID( Position ) );
#pragma warning( pop )
    }

    inline constexpr _Check_return_ int16_t ReadInt16( void ) noexcept
    {
        return( Parser.ReadInt16LittleEndian( Position ) );
    }

    inline constexpr _Check_return_ int32_t ReadInt32( void ) noexcept
    {
        return( Parser.ReadInt32LittleEndian( Position ) );
    }

    inline constexpr _Check_return_ int64_t ReadInt64( void ) noexcept
    {
        return( Parser.ReadInt64LittleEndian( Position ) );
    }

    inline constexpr _Check_return_ uint16_t ReadUInt16( void ) noexcept
    {
        return( Parser.ReadUInt16LittleEndian( Position ) );
    }

    inline constexpr _Check_return_ uint32_t ReadUInt32( void ) noexcept
    {
        return( Parser.ReadUInt32LittleEndian( Position ) );
    }

    inline constexpr _Check_return_ uint64_t ReadUInt64( void ) noexcept
    {
        return( Parser.ReadUInt64LittleEndian( Position ) );
    }

    inline void ReadUnicodeLittleEndian(_In_ uint32_t const number_of_bytes, _Inout_ std::wstring& return_value) noexcept
    {
        return_value.clear();

        if (number_of_bytes < 2)
        {
            return;
        }

        uint32_t const buffer_size = number_of_bytes + 2; // Leave room for the zero terminator

        auto buffer = std::make_unique<uint8_t []>(buffer_size);

        if (buffer.get() == nullptr)
        {
            return;
        }

        ZeroMemory(buffer.get(), buffer_size);

        ReadBytes(buffer.get(), number_of_bytes);

        return_value.assign(reinterpret_cast<wchar_t const *>(buffer.get()));
    }
};

#endif // DATA_PARSER_CLASS_HEADER
