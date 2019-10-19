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
** $Workfile: wfc_parse_iso_8601_string.cpp $
** $Revision: 10 $
** $Modtime: 6/26/01 10:59a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

static inline _Check_return_ bool __parse_ymdhmsf( _In_ std::wstring_view time_string,
    _Out_ int&    year,
    _Out_ int&    month,
    _Out_ int&    day,
    _Out_ int&    hours,
    _Out_ int&    minutes,
    _Out_ int&    seconds,
    _Out_ TCHAR&  offset_character,
    _Out_ int&    offset_hours,
    _Out_ int&    offset_minutes,
    _Out_ double& fraction ) noexcept
{
    // Here's a sample ISO8601 date string
    //
    // 1969-07-20T22:56:15-04:00

    // Do a little idiot checking

    if ( time_string.empty() == true )
    {
        //WFCTRACE( TEXT( "time string is NULL!" ) );
        return( false );
    }

    // Start with some believable defaults

    year             = 0;
    month            = 1;
    day              = 1;
    hours            = 0;
    minutes          = 0;
    seconds          = 0;
    offset_character = TEXT( 'Z' );
    offset_hours     = 0;
    offset_minutes   = 0;
    fraction = 0;

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        if ( time_string.length() < 4 ) 
        {
            // There ain't enough characters to even attempt to parse

            //WFCTRACE( TEXT( "Not enough character to even attempt to parse" ) );
            return(false);
        }

        // OK, let's start parsing

        if ( _istdigit( time_string[ 0 ] ) == 0 )
        {
            //WFCTRACE( TEXT( "First character is not a digit" ) );
            return(false);
        }

        if ( _istdigit( time_string[ 1 ] ) == 0 )
        {
            //WFCTRACE( TEXT( "Second character is not a digit" ) );
            return(false);
        }

        if ( _istdigit( time_string[ 2 ] ) == 0 )
        {
            //WFCTRACE( TEXT( "Third character is not a digit" ) );
            return(false);
        }

        if ( _istdigit( time_string[ 3 ] ) == 0 )
        {
            //WFCTRACE( TEXT( "Fourth character is not a digit" ) );
            return(false);
        }

        // If we get here, it means we've got 4 good digits

        char temp_string[ 11 ];

        temp_string[ 0 ] = static_cast<char>(time_string[ 0 ]);
        temp_string[ 1 ] = static_cast<char>(time_string[ 1 ]);
        temp_string[ 2 ] = static_cast<char>(time_string[ 2 ]);
        temp_string[ 3 ] = static_cast<char>(time_string[ 3 ]);
        temp_string[ 4 ] = 0x00;
        temp_string[ 5 ] = 0x00;
        temp_string[ 6 ] = 0x00;
        temp_string[ 7 ] = 0x00;
        temp_string[ 8 ] = 0x00;
        temp_string[ 9 ] = 0x00;
        temp_string[ 10 ] = 0x00;

        (void)std::from_chars(temp_string, &temp_string[4], year, 10);

        SIZE_T index = 4;

        // index should be pointing here
        //     |
        //     v
        // 1969-07-20T22:56:15-04:00

        if ( index >= time_string.length() )
        {
            // We're at the end of the string

            return( true );
        }

        if ( time_string[ index ] != L'-' )
        {
            //WFCTRACE( TEXT( "The separator between year and month is not -" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //      |
        //      v
        // 1969-07-20T22:56:15-04:00

        if (index >= time_string.length())
        {
            // We're at the end of the string
            return( true );
        }

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "First digit of month field isn't a digit at all" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //       |
        //       v
        // 1969-07-20T22:56:15-04:00

        // This test is safe because we could be sitting on a NULL

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "Second digit of month field isn't a digit at all" ) );
            return(false);
        }

        temp_string[ 0 ] = static_cast<char>(time_string[ index - 1 ]);
        temp_string[ 1 ] = static_cast<char>(time_string[ index ]);

        (void)std::from_chars(temp_string, &temp_string[2], month, 10);

        // Do a little idiot proofing

        if ( month < 1 or month > 12 )
        {
            //WFCTRACE( TEXT( "Funky month" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //        |
        //        v
        // 1969-07-20T22:56:15-04:00

        if (index >= time_string.length())
        {
            return( true );
        }

        if ( time_string[ index ] != L'-' )
        {
            //WFCTRACE( TEXT( "The separator between month and day fields is not -" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //         |
        //         v
        // 1969-07-20T22:56:15-04:00

        if (index >= time_string.length())
        {
            // Odd, but not illegal
            return( true );
        }

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "First digit of day field isn't a digit at all" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //          |
        //          v
        // 1969-07-20T22:56:15-04:00

        // This test is safe because we could be sitting on a NULL

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "Second digit of month field isn't a digit at all" ) );
            return(false);
        }

        temp_string[ 0 ] = static_cast<char>(time_string[ index - 1 ]);
        temp_string[ 1 ] = static_cast<char>(time_string[ index ]);

        // We don't need to set temp_string[ 2 ] because we did that at line 158 above

        (void)std::from_chars(temp_string, &temp_string[2], day, 10);

        // Do a very little bit of error checking

        if ( day < 1 or day > 31 )
        {
            //WFCTRACE( TEXT( "Funky day" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //           |
        //           v
        // 1969-07-20T22:56:15-04:00

        if (index >= time_string.length())
        {
            // Odd, but not illegal
            return( true );
        }

        if ( time_string[ index ] != L'T' )
        {
            //WFCTRACE( TEXT( "Separator between date and time ain't T" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //            |
        //            v
        // 1969-07-20T22:56:15-04:00

        if (index >= time_string.length())
        {
            // Odd, but not illegal
            return( true );
        }

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "First digit of hours field isn't a digit at all" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //             |
        //             v
        // 1969-07-20T22:56:15-04:00

        // This test is safe because we could be sitting on a NULL

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "Second digit of month field isn't a digit at all" ) );
            return(false);
        }

        temp_string[ 0 ] = static_cast<char>(time_string[ index - 1 ]);
        temp_string[ 1 ] = static_cast<char>(time_string[ index ]);

        // We don't need to set temp_string[ 2 ] because we did that at line 158 above

        (void)std::from_chars(temp_string, &temp_string[2], hours, 10);

        index++;

        // index should be pointing here
        //              |
        //              v
        // 1969-07-20T22:56:15-04:00

        if (index >= time_string.length())
        {
            // Odd, but not illegal
            return( true );
        }

        if ( time_string[ index ] != TEXT( ':' ) )
        {
            //WFCTRACE( TEXT( "Separator between hours and minutes ain't :" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //               |
        //               v
        // 1969-07-20T22:56:15-04:00

        if (index >= time_string.length())
        {
            // Odd, but not illegal
            return( true );
        }

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "First digit of time ain't no digit" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //                |
        //                v
        // 1969-07-20T22:56:15-04:00

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "First digit of time ain't no digit" ) );
            return(false);
        }

        temp_string[ 0 ] = static_cast<char>(time_string[ index - 1 ]);
        temp_string[ 1 ] = static_cast<char>(time_string[ index ]);

        (void)std::from_chars(temp_string, &temp_string[2], minutes, 10);

        index++;

        // index should be pointing here
        //                 |
        //                 v
        // 1969-07-20T22:56:15-04:00

        if (index >= time_string.length())
        {
            return( true );
        }

        if ( time_string[ index ] != L':' )
        {
            //WFCTRACE( TEXT( "Separator between minutes and seconds ain't :" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //                  |
        //                  v
        // 1969-07-20T22:56:15-04:00

        if (index >= time_string.length())
        {
            // Odd, but not illegal
            return( true );
        }

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "First digit of seconds ain't no digit" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //                   |
        //                   v
        // 1969-07-20T22:56:15-04:00

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "Second digit of seconds ain't no digit" ) );
            return(false);
        }

        temp_string[ 0 ] = static_cast<char>(time_string[ index - 1 ]);
        temp_string[ 1 ] = static_cast<char>(time_string[ index ]);

        (void)std::from_chars(temp_string, &temp_string[2], seconds, 10);

        index++;

        // index could be pointing at a fractional second
        //                    |
        //                    v
        // 2018-09-05T15:48:56.722622200Z

        if (time_string[index] == '.')
        {
            index++;

            temp_string[0] = '.';
            temp_string[1] = 0x00;
            temp_string[2] = 0x00;
            temp_string[3] = 0x00;
            temp_string[4] = 0x00;
            temp_string[5] = 0x00;
            temp_string[6] = 0x00;
            temp_string[7] = 0x00;
            temp_string[8] = 0x00;
            temp_string[9] = 0x00;
            temp_string[10] = 0x00;

            std::size_t temp_string_index = 1;

            while( _istdigit(time_string[index]) != 0)
            {
                if (temp_string_index < (std::size(temp_string) - 1))
                {
                    temp_string[temp_string_index] = static_cast<char>(time_string[index]);
                    temp_string_index++;
                }

                index++;
            }

            (void)std::from_chars(temp_string, &temp_string[temp_string_index], fraction);
        }

        temp_string[0] = 0x00;
        temp_string[1] = 0x00;
        temp_string[2] = 0x00;
        temp_string[3] = 0x00;
        temp_string[4] = 0x00;
        temp_string[5] = 0x00;
        temp_string[6] = 0x00;
        temp_string[7] = 0x00;
        temp_string[8] = 0x00;
        temp_string[9] = 0x00;
        temp_string[10] = 0x00;

        // index should be pointing here
        //                    |
        //                    v
        // 1969-07-20T22:56:15-04:00

        if (index >= time_string.length() or
            time_string[ index ] == L'Z' )
        {
            return( true );
        }

        // If we get here, we should be sitting on a Z, + or -

        if ( time_string[ index ] == L'Z' )
        {
            return( true );
        }

        if ( time_string[ index ] != L'+' and
            time_string[ index ] != L'-' )
        {
            //WFCTRACE( TEXT( "Time zone designator ain't beginning with + or -" ) );
            return(false);
        }

        offset_character = time_string[ index ];

        index++;

        // index should be pointing here
        //                     |
        //                     v
        // 1969-07-20T22:56:15-04:00

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "first digit of hours offset ain't a digit" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //                      |
        //                      v
        // 1969-07-20T22:56:15-04:00

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "second digit of hours offset ain't a digit" ) );
            return(false);
        }

        temp_string[ 0 ] = static_cast<char>(time_string[ index - 1 ]);
        temp_string[ 1 ] = static_cast<char>(time_string[ index ]);

        (void)std::from_chars(temp_string, &temp_string[2], offset_hours, 10);

        index++;

        // index should be pointing here
        //                       |
        //                       v
        // 1969-07-20T22:56:15-04:00

        if ( time_string[ index ] != L':' )
        {
            //WFCTRACE( TEXT( "Separator between offset hours and minutes ain't a :" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //                        |
        //                        v
        // 1969-07-20T22:56:15-04:00

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "First digit of minutes offset ain't a digit" ) );
            return(false);
        }

        index++;

        // index should be pointing here
        //                        |
        //                        v
        // 1969-07-20T22:56:15-04:00

        if ( _istdigit( time_string[ index ] ) == 0 )
        {
            //WFCTRACE( TEXT( "Second digit of minutes offset ain't a digit" ) );
            return(false);
        }

        temp_string[ 0 ] = static_cast<char>(time_string[ index - 1 ]);
        temp_string[ 1 ] = static_cast<char>(time_string[ index ]);

        (void)std::from_chars(temp_string, &temp_string[2], offset_minutes, 10);

        return( true );
    }
    WFC_CATCH_ALL
    {
        return(false);
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_parse_iso_8601_string(_In_ std::wstring_view time_string, __out Win32FoundationClasses::CFileTime& the_time ) noexcept
{
    the_time.Empty();

    CSystemTime system_time;

    if ( Win32FoundationClasses::wfc_parse_iso_8601_string( time_string, system_time ) == false )
    {
        return( false );
    }

    the_time.Copy( system_time );

    // Now to pick up any fractional seconds...

    int year = 0;
    int month = 0;
    int day = 0;
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    double fraction = 0;

    TCHAR offset_character = 0;

    int offset_hours = 0;
    int offset_minutes = 0;

    if (__parse_ymdhmsf(time_string, year, month, day, hours, minutes, seconds, offset_character, offset_hours, offset_minutes, fraction) == false)
    {
        //WFCTRACE( TEXT( "Can't parse string" ) );
        return(true);
    }

    double const ticks = static_cast<double>(CFileTime::NumberOfFiletimeTicksInOneSecond) * fraction;

    uint32_t const number_of_ticks = static_cast<uint32_t>(ticks);

    if (ticks > 0)
    {
        the_time.AddTicks(number_of_ticks);
    }

    return( true );
}

#if ! defined( WFC_STL )

BOOL PASCAL Win32FoundationClasses::wfc_parse_iso_8601_string( LPCTSTR time_string, COleDateTime& the_time )
{
    the_time = COleDateTime( static_cast< time_t >( 0 ) );

    // Do a little idiot checking

    if ( time_string == nullptr )
    {
        WFCTRACE( TEXT( "time_string is NULL!" ) );
        return( FALSE );
    }

    int year    = 0;
    int month   = 0;
    int day     = 0;
    int hours   = 0;
    int minutes = 0;
    int seconds = 0;

    TCHAR offset_character = 0;

    int offset_hours   = 0;
    int offset_minutes = 0;

    if ( __parse_ymdhms( time_string, year, month, day, hours, minutes, seconds, offset_character, offset_hours, offset_minutes ) == FALSE )
    {
        WFCTRACE( TEXT( "Can't parse string" ) );
        return( FALSE );
    }

    the_time = COleDateTime( year, month, day, hours, minutes, seconds );

    if ( offset_character != TEXT( 'Z' ) )
    {
        COleDateTimeSpan time_zone_offset( 0, offset_hours, offset_minutes, 0 );

        if ( offset_character == TEXT( '-' ) )
        {
            the_time += time_zone_offset;
        }
        else if ( offset_character == TEXT( '+' ) )
        {
            the_time -= time_zone_offset;
        }
    }

    return( TRUE );
}

#endif // WFC_STL

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_parse_iso_8601_string(_In_ std::wstring_view time_string, __out CSystemTime& the_time ) noexcept
{
    the_time.Empty();

    // Do a little idiot checking

    if ( time_string.empty() == true )
    {
        //WFCTRACE( TEXT( "time_string is NULL!" ) );
        return( false );
    }

    int year    = 0;
    int month   = 0;
    int day     = 0;
    int hours   = 0;
    int minutes = 0;
    int seconds = 0;
    double fraction = 0;

    TCHAR offset_character = 0;

    int offset_hours   = 0;
    int offset_minutes = 0;

    if ( __parse_ymdhmsf( time_string, year, month, day, hours, minutes, seconds, offset_character, offset_hours, offset_minutes, fraction ) == false )
    {
        //WFCTRACE( TEXT( "Can't parse string" ) );
        return( false );
    }

    CTime ole_time = CTime( year, month, day, hours, minutes, seconds );

    if ( offset_character != L'Z' )
    {
        CTimeSpan time_zone_offset( 0, offset_hours, offset_minutes, 0 );

        if ( offset_character == L'-' )
        {
            ole_time += time_zone_offset;
        }
        else if ( offset_character == L'+' )
        {
            ole_time -= time_zone_offset;
        }
    }

    the_time.Copy( ole_time );

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_parse_iso_8601_string(_In_ std::wstring_view time_string, __out CTime& the_time ) noexcept
{
    the_time = CTime( static_cast< time_t >( 0 ) );

    // Do a little idiot checking

    if ( time_string.empty() == true )
    {
        //WFCTRACE( TEXT( "time_string is NULL!" ) );
        return( false );
    }

    int year    = 0;
    int month   = 0;
    int day     = 0;
    int hours   = 0;
    int minutes = 0;
    int seconds = 0;
    double fraction = 0;

    TCHAR offset_character = 0;

    int offset_hours   = 0;
    int offset_minutes = 0;

    if ( __parse_ymdhmsf( time_string, year, month, day, hours, minutes, seconds, offset_character, offset_hours, offset_minutes, fraction) == false )
    {
        //WFCTRACE( TEXT( "Can't parse string" ) );
        return( false );
    }

    the_time = CTime( year, month, day, hours, minutes, seconds );

    if ( offset_character != L'Z' )
    {
        CTimeSpan time_zone_offset( 0, offset_hours, offset_minutes, 0 );

        if ( offset_character == L'-' )
        {
            the_time += time_zone_offset;
        }
        else if ( offset_character == L'+' )
        {
            the_time -= time_zone_offset;
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_parse_xml( __in_z LPCTSTR filename, __inout CExtensibleMarkupLanguageDocument& xml ) noexcept
{
    WFC_VALIDATE_POINTER( filename );

    xml.Empty();

    if ( filename == nullptr or filename[ 0 ] == 0x00 )
    {
        return( false );
    }

    CMemoryFile memory_mapped_file;

    if ( memory_mapped_file.Open( filename ) == false )
    {
        return( false );
    }

    auto pointer = reinterpret_cast<uint8_t const *>(memory_mapped_file.GetPointer());

    CDataParser parser;

    if ( parser.Initialize( pointer, memory_mapped_file.GetLength() ) == false)
    {
        return( false );
    }

    CParsePoint here;

    parser.DetectText( here );

    if ( xml.Parse( parser ) == false)
    {
        return( false );
    }

    return( true );
}

static constexpr uint8_t const g_hex_values[256] =
{
    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, //  0 - 15
    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 16 - 31
    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 32 - 47
    0, 1, 2, 3,  4, 5, 6, 7,  8, 9, 0, 0,  0, 0, 0, 0, // 48 - 63
    0, 10, 11, 12,  13, 14, 15, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 64 - 79
    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 80 - 95
    0, 10, 11, 12,  13, 14, 15, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 96 
    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0
};

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_parse_hex_string(__in_z wchar_t const * hex_string, _In_ std::size_t const number_of_characters, __out_bcount(buffer_size) uint8_t * buffer, _In_ std::size_t const buffer_size) noexcept
{
    WFC_VALIDATE_POINTER(hex_string);
    WFC_VALIDATE_POINTER(buffer);
    _ASSERTE(number_of_characters <= (buffer_size * 2));

    if (number_of_characters > (buffer_size * 2))
    {
        return(false);
    }

    std::size_t string_index = 0;

    for ( auto const buffer_index : Range(buffer_size) )
    {
        buffer[buffer_index] = ((g_hex_values[(uint8_t)hex_string[string_index]]) << 4) bitor g_hex_values[(uint8_t)hex_string[string_index + 1]];
        string_index += 2;
    }

    return(true);
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_parse_hex_string(__in_z char const * hex_string, _In_ std::size_t const number_of_characters, __out_bcount(buffer_size) uint8_t * buffer, _In_ std::size_t const buffer_size) noexcept
{
    WFC_VALIDATE_POINTER(hex_string);
    WFC_VALIDATE_POINTER(buffer);
    _ASSERTE(number_of_characters <= (buffer_size * 2));

    if (number_of_characters > (buffer_size * 2))
    {
        return(false);
    }

    std::size_t string_index = 0;

    for ( auto const buffer_index : Range(buffer_size) )
    {
        buffer[buffer_index] = ((g_hex_values[hex_string[string_index]]) << 4) bitor g_hex_values[hex_string[string_index + 1]];
        string_index += 2;
    }

    return(true);
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_parse_iso_8601_string</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that parses a time string (in ISO8601 format) into a CTime object.">
</HEAD>

<BODY>

<H1>wfc_parse_iso_8601_string</H1>
$Revision: 10 $<HR>

<H2>Declaration</H2>
<PRE>BOOL <B>wfc_parse_iso_8601_string</B>( LPCTSTR time_string, CFileTime&amp;    the_time )
BOOL <B>wfc_parse_iso_8601_string</B>( LPCTSTR time_string, COleDateTime&amp; the_time )
BOOL <B>wfc_parse_iso_8601_string</B>( LPCTSTR time_string, CSystemTime&amp;  the_time )
BOOL <B>wfc_parse_iso_8601_string</B>( LPCTSTR time_string, CTime&amp;        the_time )</PRE>

<H2>Description</H2>
This function parses <CODE>time_string</CODE> and puts the results into <CODE>the_time</CODE>.

<H2>Example</H2>

<PRE><CODE>void test_time( void )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;test_time()&quot; ) );

CTime the_time;

if ( <B>wfc_parse_iso_8601_string</B>( the_time ) == FALSE )
{
_tprintf( TEXT( &quot;Can't parse\n&quot; ) );
}
else
{
_tprintf( TEXT( &quot;Parsed OK\n&quot; ) );
}

}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_get_operating_system_version_string</B>() uses the following API's:
<UL>
<LI>GetVersionEx
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_parse_iso_8601_string.cpp $<BR>
$Modtime: 6/26/01 10:59a $
</BODY>

</HTML>
*/
