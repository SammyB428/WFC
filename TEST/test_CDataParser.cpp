/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2019, Samuel R. Blackburn
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
** $Workfile: test_CBitArray.cpp $
** $Revision: 3 $
** $Modtime: 8/06/00 3:25p $
*/

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#define NUMBER_OF_TESTS_IN_CDATAPARSER_TEST_CLASS (23)

static _Check_return_ bool test_class(_Inout_ CDataParser& parser, _Inout_ int& test_number_that_failed ) noexcept
{
    CParsePoint parse_point;
    CParsePoint found_at;

    if ( parser.IsTextASCII() != false )
    {
        _ASSERTE( parser.GetSize() == ( 6 * 1 ) );

        if ( parser.GetSize() != ( 6 * 1 ) )
        {
            test_number_that_failed = 1;
            return( failure() );
        }
    }
    else if ( parser.IsTextUCS4() != false )
    {
        _ASSERTE( parser.GetSize() == ( 6 * 4 ) );
        if ( parser.GetSize() != ( 6 * 4 ) )
        {
            test_number_that_failed = 1;
            return( failure() );
        }
    }
    else
    {
        _ASSERTE( parser.GetSize() == ( 6 * 2 ) );
        if ( parser.GetSize() != ( 6 * 2 ) )
        {
            test_number_that_failed = 1;
            return( failure() );
        }
    }

    uint32_t character = 0;

    if ( parser.PeekAtCharacter( parse_point, character, 0 ) == false )
    {
        test_number_that_failed = 2;
        return( failure() );
    }

    _ASSERTE( character == 'A' );

    if ( character != 'A' )
    {
        test_number_that_failed = 3;
        return( failure() );
    }

    if ( parser.PeekAtCharacter( parse_point, character ) == false )
    {
        test_number_that_failed = 4;
        return( failure() );
    }

    _ASSERTE( character == 'B' );

    if ( character != 'B' )
    {
        test_number_that_failed = 5;
        return( failure() );
    }

    if ( parser.PeekAtCharacter( parse_point, character, 2 ) == false )
    {
        test_number_that_failed = 6;
        return( failure() );
    }

    _ASSERTE( character == 'C' );

    if ( character != 'C' )
    {
        test_number_that_failed = 7;
        return( failure() );
    }

    if ( parser.Find( parse_point, TEXT( "DEF" ), found_at ) == false )
    {
        test_number_that_failed = 8;
        return( failure() );
    }

    if ( parser.IsTextASCII() != false )
    {
        _ASSERTE( found_at.GetIndex() == ( 3 * 1 ) );
        if ( found_at.GetIndex() != ( 3 * 1 ) )
        {
            test_number_that_failed = 9;
            return( failure() );
        }
    }
    else if ( parser.IsTextUCS4() != false )
    {
        _ASSERTE( found_at.GetIndex() == ( 3 * 4 ) );
        if ( found_at.GetIndex() != ( 3 * 4 ) )
        {
            test_number_that_failed = 9;
            return( failure() );
        }
    }
    else
    {
        // Must be Unicode
        _ASSERTE( found_at.GetIndex() == ( 3 * 2 ) );
        if ( found_at.GetIndex() != ( 3 * 2 ) )
        {
            test_number_that_failed = 9;
            return( failure() );
        }
    }

    found_at.Empty();

    if ( parser.FindNoCase( parse_point, TEXT( "dEf" ), found_at ) == false )
    {
        test_number_that_failed = 9;
        return( failure() );
    }

    if ( parser.IsTextASCII() != false )
    {
        _ASSERTE( found_at.GetIndex() == ( 3 * 1 ) );
        if ( found_at.GetIndex() != ( 3 * 1 ) )
        {
            test_number_that_failed = 11;
            return( failure() );
        }
    }
    else if ( parser.IsTextUCS4() != false )
    {
        _ASSERTE( found_at.GetIndex() == ( 3 * 4 ) );
        if ( found_at.GetIndex() != ( 3 * 4 ) )
        {
            test_number_that_failed = 11;
            return( failure() );
        }
    }
    else
    {
        // Must be Unicode
        _ASSERTE( found_at.GetIndex() == ( 3 * 2 ) );
        if ( found_at.GetIndex() != ( 3 * 2 ) )
        {
            test_number_that_failed = 11;
            return( failure() );
        }
    }

    found_at.Empty();

    if ( parser.Find( parse_point, (BYTE) 'D', found_at ) == false )
    {
        test_number_that_failed = 12;
        return( failure() );
    }

    if ( parser.IsTextASCII() != false )
    {
        _ASSERTE( found_at.GetIndex() == ( 3 * 1 ) );
        if ( found_at.GetIndex() != ( 3 * 1 ) )
        {
            test_number_that_failed = 13;
            return( failure() );
        }
    }
    else if ( parser.IsTextUCS4() != false )
    {
        if ( parser.GetUCS4Order() == 1234 )
        {
            _ASSERTE( found_at.GetIndex() == 15 );

            if ( found_at.GetIndex() != 15 )
            {
                test_number_that_failed = 13;
                return( failure() );
            }
        }
        else if ( parser.GetUCS4Order() == 2143 )
        {
            _ASSERTE( found_at.GetIndex() == 14 );

            if ( found_at.GetIndex() != 14 )
            {
                test_number_that_failed = 13;
                return( failure() );
            }
        }
        else if ( parser.GetUCS4Order() == 3412 )
        {
            _ASSERTE( found_at.GetIndex() == 13 );

            if ( found_at.GetIndex() != 13 )
            {
                test_number_that_failed = 13;
                return( failure() );
            }
        }
        else if ( parser.GetUCS4Order() == 4321 )
        {
            _ASSERTE( found_at.GetIndex() == 12 );

            if ( found_at.GetIndex() != 12 )
            {
                test_number_that_failed = 13;
                return( failure() );
            }
        }
        else
        {
            // Text is UCS4 but not a valid order!
            DWORD ucs_order = parser.GetUCS4Order();
            test_number_that_failed = 13;
            return( failure() );
        }
    }
    else
    {
        // Must be Unicode

        WFC_VALIDATE_bool( parser.IsTextBigEndian() );

        if ( parser.IsTextBigEndian() != false )
        {
            _ASSERTE( found_at.GetIndex() == 7 );

            if ( found_at.GetIndex() != 7 )
            {
                test_number_that_failed = 13;
                return( failure() );
            }
        }
        else
        {
            _ASSERTE( found_at.GetIndex() == 6 );

            if ( found_at.GetIndex() != 6 )
            {
                test_number_that_failed = 13;
                return( failure() );
            }
        }
    }

    parse_point.Empty();
    found_at.Empty();

    _ASSERTE( parse_point.GetLineIndex() == 1 );

    if ( parse_point.GetLineIndex() != 1 )
    {
        test_number_that_failed = 14;
        return( failure() );
    }

    parser.AdvanceByOneCharacter( parse_point );

    _ASSERTE( parse_point.GetLineIndex() == 2 );

    if ( parse_point.GetLineIndex() != 2 )
    {
        test_number_that_failed = 15;
        return( failure() );
    }

    parse_point.Empty();

    _ASSERTE( parse_point.GetLineIndex() == 1 );

    if ( parse_point.GetLineIndex() != 1 )
    {
        test_number_that_failed = 16;
        return( failure() );
    }

    parser.AdvanceByOneCharacter( parse_point, '\n' );

    _ASSERTE( parse_point.GetLineIndex() == 1 );

    // We advanced by one line, that means we should be at column 1 of line 2
    if ( parse_point.GetLineIndex() != 1 )
    {
        test_number_that_failed = 17;
        return( failure() );
    }

    _ASSERTE( parse_point.GetLineNumber() == 2 );

    if ( parse_point.GetLineNumber() != 2 )
    {
        test_number_that_failed = 18;
        return( failure() );
    }

    parser.Empty();
    parse_point.Empty();

    parser.AdvanceByOneCharacter( parse_point, '\n' );

    _ASSERTE( parse_point.GetLineIndex() == 1 );

    if ( parse_point.GetLineIndex() != 1 )
    {
        test_number_that_failed = 19;
        return( failure() );
    }

    _ASSERTE( parse_point.GetLineNumber() == 1 );

    if ( parse_point.GetLineNumber() != 1 )
    {
        test_number_that_failed = 20;
        return( failure() );
    }

    _ASSERTE( parse_point.GetIndex() == 0 );

    if ( parse_point.GetIndex() != 0 )
    {
        test_number_that_failed = 21;
        return( failure() );
    }

    parser.SetUnicodeToASCIITranslationFailureCharacter( '~' );

    _ASSERTE( parser.GetUnicodeToASCIITranslationFailureCharacter() == '~' );

    if ( parser.GetUnicodeToASCIITranslationFailureCharacter() != '~' )
    {
        test_number_that_failed = 22;
        return( failure() );
    }

    parser.SetUnicodeToASCIITranslationFailureCharacter( '?' );

    _ASSERTE( parser.GetUnicodeToASCIITranslationFailureCharacter() == '?' );

    if ( parser.GetUnicodeToASCIITranslationFailureCharacter() != '?' )
    {
        test_number_that_failed = 23;
        return( failure() );
    }

    test_number_that_failed = 23;

    return( true );
}

_Check_return_ bool test_CDataParser( _Out_ std::string& class_name, _Out_ int& test_number_that_failed ) noexcept
{
    class_name.assign( "CDataParser" );

    std::vector<uint8_t> bytes;

    CDataParser parser;

    if ( parser.GetSize() != 0 )
    {
        test_number_that_failed = 1000;
        return( failure() );
    }

    if ( parser.GetData() != NULL )
    {
        test_number_that_failed = 1001;
        return( failure() );
    }

    // ASCII Test

    (void) bytes.push_back( 'A' );
    (void) bytes.push_back( 'B' );
    (void) bytes.push_back( 'C' );
    (void) bytes.push_back( 'D' );
    (void) bytes.push_back( 'E' );
    (void) bytes.push_back( 'F' );

    (void) parser.Initialize( &bytes );

    if ( parser.GetSize() != 6 )
    {
        test_number_that_failed = 1002;
        return( failure() );
    }

    if ( parser.GetData() == nullptr )
    {
        test_number_that_failed = 1003;
        return( failure() );
    }

    (void) parser.SetTextToASCII( true );

    if ( test_class( parser, test_number_that_failed ) == false )
    {
        return( failure() );
    }

    parser.Empty();

    if ( parser.GetSize() != 0 )
    {
        test_number_that_failed = 1004;
        return( failure() );
    }

    if ( parser.GetData() != nullptr)
    {
        test_number_that_failed = 1005;
        return( failure() );
    }

    // UNICODE, Intel (little-endian)

    bytes.clear();

    (void) bytes.push_back( 'A' ); // little end
    (void) bytes.push_back(  0  ); // big end

    (void) bytes.push_back( 'B' );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back( 'C' );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back( 'D' );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back( 'E' );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back( 'F' );
    (void) bytes.push_back(  0  );

    (void) parser.Initialize( &bytes );

    if ( parser.GetSize() != 12 )
    {
        test_number_that_failed = 1006;
        return( failure() );
    }

    if ( parser.GetData() == nullptr)
    {
        test_number_that_failed = 1007;
        return( failure() );
    }

    (void) parser.SetTextToASCII( false );
    (void) parser.SetTextToBigEndian(false);

    if ( test_class( parser, test_number_that_failed ) == false )
    {
        test_number_that_failed += ( NUMBER_OF_TESTS_IN_CDATAPARSER_TEST_CLASS * 1 );
        return( failure() );
    }

    parser.Empty();

    if ( parser.GetSize() != 0 )
    {
        test_number_that_failed = 1008;
        return( failure() );
    }

    if ( parser.GetData() != nullptr)
    {
        test_number_that_failed = 1009;
        return( failure() );
    }

    // UNICODE, Sun (big-endian)

    bytes.clear();

    (void) bytes.push_back(  0  ); // big end
    (void) bytes.push_back( 'A' ); // little end

    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'B' );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'C' );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'D' );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'E' );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'F' );

    (void) parser.Initialize( &bytes );

    if ( parser.GetSize() != 12 )
    {
        test_number_that_failed = 1010;
        return( failure() );
    }

    if ( parser.GetData() == nullptr)
    {
        test_number_that_failed = 1011;
        return( failure() );
    }

    (void) parser.SetTextToASCII( false );
    (void) parser.SetTextToBigEndian( true );

    if ( test_class( parser, test_number_that_failed ) == false )
    {
        test_number_that_failed += ( NUMBER_OF_TESTS_IN_CDATAPARSER_TEST_CLASS * 2 );
        return( failure() );
    }

    parser.Empty();

    if ( parser.GetSize() != 0 )
    {
        test_number_that_failed = 1012;
        return( failure() );
    }

    if ( parser.GetData() != nullptr)
    {
        test_number_that_failed = 1013;
        return( failure() );
    }

    // UCS4-1234

    bytes.clear();

    (void) bytes.push_back(  0  ); // 1
    (void) bytes.push_back(  0  ); // 2
    (void) bytes.push_back(  0  ); // 3
    (void) bytes.push_back( 'A' ); // 4

    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'B' );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'C' );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'D' );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'E' );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'F' );

    (void) parser.Initialize( &bytes );

    if ( parser.GetSize() != 24 )
    {
        test_number_that_failed = 1014;
        return( failure() );
    }

    if ( parser.GetData() == nullptr)
    {
        test_number_that_failed = 1015;
        return( failure() );
    }

    (void) parser.SetTextToUCS4( true );
    (void) parser.SetUCS4Order( 1234 );

    if ( test_class( parser, test_number_that_failed ) == false )
    {
        test_number_that_failed += ( NUMBER_OF_TESTS_IN_CDATAPARSER_TEST_CLASS * 3 );
        return( failure() );
    }

    parser.Empty();

    if ( parser.GetSize() != 0 )
    {
        test_number_that_failed = 1016;
        return( failure() );
    }

    if ( parser.GetData() != nullptr)
    {
        test_number_that_failed = 1017;
        return( failure() );
    }

    // UCS4-2143

    bytes.clear();

    (void) bytes.push_back(  0  ); // 2
    (void) bytes.push_back(  0  ); // 1
    (void) bytes.push_back( 'A' ); // 4
    (void) bytes.push_back(  0  ); // 3

    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'B' );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'C' );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'D' );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'E' );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'F' );
    (void) bytes.push_back(  0  );

    (void) parser.Initialize( &bytes );

    if ( parser.GetSize() != 24 )
    {
        test_number_that_failed = 1018;
        return( failure() );
    }

    if ( parser.GetData() == nullptr)
    {
        test_number_that_failed = 1019;
        return( failure() );
    }

    (void) parser.SetTextToUCS4( true );
    (void) parser.SetUCS4Order( 2143 );

    if ( test_class( parser, test_number_that_failed ) == false )
    {
        test_number_that_failed += ( NUMBER_OF_TESTS_IN_CDATAPARSER_TEST_CLASS * 4 );
        return( failure() );
    }

    parser.Empty();

    if ( parser.GetSize() != 0 )
    {
        test_number_that_failed = 1020;
        return( failure() );
    }

    if ( parser.GetData() != nullptr)
    {
        test_number_that_failed = 1021;
        return( failure() );
    }

    // UCS4-3412

    bytes.clear();

    (void) bytes.push_back(  0  ); // 3
    (void) bytes.push_back( 'A' ); // 4
    (void) bytes.push_back(  0  ); // 1
    (void) bytes.push_back(  0  ); // 2

    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'B' );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'C' );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'D' );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'E' );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back(  0  );
    (void) bytes.push_back( 'F' );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );

    (void) parser.Initialize( &bytes );

    if ( parser.GetSize() != 24 )
    {
        test_number_that_failed = 1022;
        return( failure() );
    }

    if ( parser.GetData() == nullptr)
    {
        test_number_that_failed = 1023;
        return( failure() );
    }

    (void) parser.SetTextToUCS4( true );
    (void) parser.SetUCS4Order( 3412 );

    if ( test_class( parser, test_number_that_failed ) == false )
    {
        test_number_that_failed += ( NUMBER_OF_TESTS_IN_CDATAPARSER_TEST_CLASS * 5 );
        return( failure() );
    }

    parser.Empty();

    if ( parser.GetSize() != 0 )
    {
        test_number_that_failed = 1024;
        return( failure() );
    }

    if ( parser.GetData() != nullptr )
    {
        test_number_that_failed = 1025;
        return( failure() );
    }

    // UCS4-4321

    bytes.clear();

    (void) bytes.push_back( 'A' ); // 4
    (void) bytes.push_back(  0  ); // 3
    (void) bytes.push_back(  0  ); // 2
    (void) bytes.push_back(  0  ); // 1

    (void) bytes.push_back( 'B' );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back( 'C' );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back( 'D' );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back( 'E' );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );

    (void) bytes.push_back( 'F' );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );
    (void) bytes.push_back(  0  );

    (void) parser.Initialize( &bytes );

    if ( parser.GetSize() != 24 )
    {
        test_number_that_failed = 1026;
        return( failure() );
    }

    if ( parser.GetData() == nullptr )
    {
        test_number_that_failed = 1027;
        return( failure() );
    }

    (void) parser.SetTextToUCS4( true );
    (void) parser.SetUCS4Order( 4321 );

    if ( test_class( parser, test_number_that_failed ) == false )
    {
        test_number_that_failed += ( NUMBER_OF_TESTS_IN_CDATAPARSER_TEST_CLASS * 6 );
        return( failure() );
    }

    parser.Empty();

    if ( parser.GetSize() != 0 )
    {
        test_number_that_failed = 1028;
        return( failure() );
    }

    if ( parser.GetData() != nullptr )
    {
        test_number_that_failed = 1029;
        return( failure() );
    }

    bytes.clear();

    (void) bytes.push_back( 'A' );
    (void) bytes.push_back( 'B' );
    (void) bytes.push_back( 'C' );
    (void) bytes.push_back( 'D' );
    (void) bytes.push_back( 'E' );

    (void) parser.Initialize( &bytes );

    if ( parser.GetSize() != 5 )
    {
        test_number_that_failed = 1030;
        return( failure() );
    }

    std::vector<uint8_t> bytes_to_find;

    (void) bytes_to_find.push_back( 'A' );
    (void) bytes_to_find.push_back( 'B' );

    CParsePoint found_at;
    CParsePoint here;

    (void) parser.SetTextToASCII( true );

    if ( parser.Find( here, bytes_to_find, found_at ) == false )
    {
        test_number_that_failed = 1031;
        return( failure() );
    }

    if ( found_at.GetIndex() != 0 )
    {
        test_number_that_failed = 1032;
        return( failure() );
    }

    bytes_to_find.clear();

    (void) bytes_to_find.push_back( 'C' );
    (void) bytes_to_find.push_back( 'D' );
    (void) bytes_to_find.push_back( 'E' );

    if ( parser.Find( here, bytes_to_find, found_at ) == false )
    {
        test_number_that_failed = 1033;
        return( failure() );
    }

    if ( found_at.GetIndex() != 2 )
    {
        test_number_that_failed = 1034;
        return( failure() );
    }

    bytes_to_find.clear();

    (void) bytes_to_find.push_back( 'C' );
    (void) bytes_to_find.push_back( 'E' );

    if ( parser.Find( here, bytes_to_find, found_at ) != false )
    {
        test_number_that_failed = 1035;
        return( failure() );
    }

    bytes_to_find.clear();

    (void) bytes_to_find.push_back( 'C' );
    (void) bytes_to_find.push_back( 'd' );
    (void) bytes_to_find.push_back( 'E' );

    found_at.Empty();

    if ( parser.Find( here, bytes_to_find, found_at ) != false )
    {
        test_number_that_failed = 1036;
        return( failure() );
    }

    found_at.Empty();

    if ( parser.FindNoCase( here, bytes_to_find, found_at ) == false )
    {
        test_number_that_failed = 1037;
        return( failure() );
    }

    if ( found_at.GetIndex() != 2 )
    {
        test_number_that_failed = 1038;
        return( failure() );
    }

    std::wstring string_to_find( L"cdE" );

    found_at.Empty();

    if ( parser.FindNoCase( here, string_to_find, found_at ) == false )
    {
        test_number_that_failed = 1039;
        return( failure() );
    }

    if ( found_at.GetIndex() != 2 )
    {
        test_number_that_failed = 1040;
        return( failure() );
    }

    std::vector<uint8_t> bytes_gotten;

    here.SetIndex( 2 );

    if ( parser.Get( here, 3, bytes_gotten ) == false )
    {
        test_number_that_failed = 1041;
        return( failure() );
    }

    if ( bytes_gotten.size() != 3 )
    {
        test_number_that_failed = 1042;
        return( failure() );
    }

    if ( bytes_gotten.at( 0 ) != 'C' )
    {
        test_number_that_failed = 1043;
        return( failure() );
    }

    if ( bytes_gotten.at( 1 ) != 'D' )
    {
        test_number_that_failed = 1044;
        return( failure() );
    }

    if ( bytes_gotten.at( 2 ) != 'E' )
    {
        test_number_that_failed = 1045;
        return( failure() );
    }

    here.SetIndex( 3 );
    bytes_gotten.clear();

    // Try and get three bytes from a five byte array beginning at index 3
    // (this should fail)
    if ( parser.Get( here, 3, bytes_gotten ) != false )
    {
        test_number_that_failed = 1046;
        return( failure() );
    }

    if ( bytes_gotten.size() != 0 )
    {
        test_number_that_failed = 1047;
        return( failure() );
    }

    uint8_t our_bytes[ 2 ] = { 0x00, 0x00 };

    here.SetIndex( 1 );

    if ( parser.Get( here, sizeof( our_bytes ), our_bytes ) == false )
    {
        test_number_that_failed = 1048;
        return( failure() );
    }

    if ( our_bytes[ 0 ] != 'B' )
    {
        test_number_that_failed = 1049;
        return( failure() );
    }

    if ( our_bytes[ 1 ] != 'C' )
    {
        test_number_that_failed = 1050;
        return( failure() );
    }

    here.SetIndex( 4 );

    if ( parser.Get( here, sizeof( our_bytes ), our_bytes ) != false )
    {
        test_number_that_failed = 1051;
        return( failure() );
    }

    if ( parser.Get( here, 1, our_bytes ) == false )
    {
        test_number_that_failed = 1052;
        return( failure() );
    }

    if ( our_bytes[ 0 ] != 'E' )
    {
        test_number_that_failed = 1053;
        return( failure() );
    }

    std::wstring string_to_be_gotten;

    here.SetIndex( 0 );

    if ( parser.Get( here, 6, string_to_be_gotten ) != false )
    {
        test_number_that_failed = 1054;
        return( failure() );
    }

    if ( string_to_be_gotten.length() != 0 )
    {
        test_number_that_failed = 1055;
        return( failure() );
    }

    if ( parser.Get( here, 5, string_to_be_gotten ) == false )
    {
        test_number_that_failed = 1056;
        return( failure() );
    }

    if ( string_to_be_gotten.length() != 5 )
    {
        test_number_that_failed = 1057;
        return( failure() );
    }

    if ( string_to_be_gotten.compare( L"ABCDE" ) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 1058;
        return( failure() );
    }

    if ( parser.GetAt( 0 ) != 'A' )
    {
        test_number_that_failed = 1059;
        return( failure() );
    }

    if ( parser.GetAt( 4 ) != 'E' )
    {
        test_number_that_failed = 1060;
        return( failure() );
    }

    if ( parser.GetAt( 5 ) != 0 )
    {
        test_number_that_failed = 1061;
        return( failure() );
    }

    if ( parser.GetAt( 111111 ) != 0 )
    {
        test_number_that_failed = 1062;
        return( failure() );
    }

    if ( parser.GetSize() != 5 )
    {
        test_number_that_failed = 1063;
        return( failure() );
    }

    uint8_t hex_buffer[10]{ 0 };

    hex_buffer[ 0 ] = '1';
    hex_buffer[ 1 ] = '2';
    hex_buffer[ 2 ] = '3';
    hex_buffer[ 3 ] = '4';
    hex_buffer[ 4 ] = '5';

    parser.Initialize( hex_buffer, sizeof(hex_buffer) );

    here.Empty();
    here.SetIndex( 1 );

    uint64_t test_value = parser.ReadHexadecimalInteger( here, 2 );

    if ( test_value != 0x23 )
    {
        test_number_that_failed = 1064;
        return( failure() );
    }

    here.SetIndex( 1 );
    test_value = parser.ReadInteger( here, 2 );

    if ( test_value != 23 )
    {
        test_number_that_failed = 1065;
        return( failure() );
    }

    here.SetIndex( 1 );
    test_value = parser.ReadInteger( here, 3 );

    if ( test_value != 234 )
    {
        test_number_that_failed = 1066;
        return( failure() );
    }

    hex_buffer[0] = 'a';
    hex_buffer[1] = 'b';
    hex_buffer[2] = 'c';
    hex_buffer[3] = 'd';
    hex_buffer[4] = 'e';
    hex_buffer[5] = 'f';
    hex_buffer[6] = 'g';
    hex_buffer[7] = 'h';
    hex_buffer[8] = 'h';
    hex_buffer[9] = 'i';

    parser.Initialize(hex_buffer, sizeof(hex_buffer));

    here.Empty();
    found_at.Empty();

    constexpr uint8_t const needle[2] = { 'h', 'i' };

    if (parser.Find(here, needle, sizeof(needle), found_at) == false)
    {
        test_number_that_failed = 1067;
        return(failure());
    }

    if (found_at.GetIndex() != 8)
    {
        test_number_that_failed = 1068;
        return(failure());
    }

    test_number_that_failed = 1068;

    return( true );
}
