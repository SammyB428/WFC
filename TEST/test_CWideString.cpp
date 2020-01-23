/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2020, Samuel R. Blackburn
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
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#if ! defined( INTERNAL_STRING_BUFFER_SIZE )
#define INTERNAL_STRING_BUFFER_SIZE (16)
#endif // INTERNAL_STRING_BUFFER_SIZE

_Check_return_ bool test_CWideString( _Out_ std::string& class_name, _Out_ int& test_number_that_failed ) noexcept
{
    class_name.assign(STRING_VIEW("string helpers"));

    std::wstring string_1;

    if ( string_1.length() != 0 )
    {
        test_number_that_failed = 1;
        return( failure() );
    }

    string_1.assign(WSTRING_VIEW(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"));

    if ( string_1.length() != 36 )
    {
        test_number_that_failed = 2;
        return( failure() );
    }

    std::wstring string_2( string_1 );

    if ( string_1.compare( string_2 ) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 3;
        return( failure() );
    }

    make_lower( string_1 );

    if ( string_1.compare(WSTRING_VIEW(L"abcdefghijklmnopqrstuvwxyz0123456789")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 4;
        return( failure() );
    }

    make_upper(string_1);

    if ( string_1.compare( string_2 ) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 5;
        return( failure() );
    }

    string_2.assign( left(string_1, 26 ) );

    if ( string_2.compare(WSTRING_VIEW(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 6;
        return( failure() );
    }

    if ( compare_no_case( string_2, WSTRING_VIEW(L"abcDEFGHIJKLMNOPQRSTUVWXYZ")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 7;
        return( failure() );
    }

    string_2.assign( right( string_1, 10 ) );

    if ( string_2.compare(WSTRING_VIEW(L"0123456789")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 8;
        return( failure() );
    }

    // OK, we've got the basic tests out of the way, now let's methodically
    // go through the interface and make sure we test everything.

    // Now test to see if we can add more than one character...

    std::wstring string_3(WSTRING_VIEW(L"ABCDE"));

    if ( string_3.length() != 5 )
    {
        test_number_that_failed = 13;
        return( failure() );
    }

    if ( string_3.at( 4 ) != L'E' )
    {
        test_number_that_failed = 14;
        return( failure() );
    }

    if ( string_3.compare(WSTRING_VIEW(L"ABCDE")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 15;
        return( failure() );
    }

    unsigned short byte_buffer[ 5 ];

    byte_buffer[ 0 ] = 'a';
    byte_buffer[ 1 ] = 'b';
    byte_buffer[ 2 ] = 'c';
    byte_buffer[ 3 ] = 'd';
    byte_buffer[ 4 ] = 0xFF; // NOT NULL TERMINATED!

    std::wstring string_4( reinterpret_cast<wchar_t const *>(byte_buffer), 4 );

    if ( string_4.length() != 4 )
    {
        test_number_that_failed = 16;
        return( failure() );
    }

    if ( string_4.at( 3 ) != L'd' )
    {
        test_number_that_failed = 17;
        return( failure() );
    }

    if ( string_4.compare(WSTRING_VIEW(L"abcd")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 18;
        return( failure() );
    }

    byte_buffer[ 4 ] = 0x00; // NULL TERMINATE

    std::wstring string_5( reinterpret_cast<wchar_t const *>(byte_buffer) );

    if ( string_5.length() != 4 )
    {
        test_number_that_failed = 19;
        return( failure() );
    }

    if ( string_5.at( 3 ) != L'd' )
    {
        test_number_that_failed = 20;
        return( failure() );
    }

    if ( string_5.compare(WSTRING_VIEW(L"abcd")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 21;
        return( failure() );
    }

    string_5.clear();

    if ( string_5.length() != 0 )
    {
        test_number_that_failed = 22;
        return( failure() );
    }

    if ( string_5.empty() != true )
    {
        test_number_that_failed = 23;
        return( failure() );
    }

    // Be careful to use only functions that have already been tested above

    string_1.assign(WSTRING_VIEW(L"ABC"));
    string_2.assign(WSTRING_VIEW(L"DEF"));

    string_1.append( string_2 );

    if ( string_1.compare(WSTRING_VIEW(L"ABCDEF")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 23;
        return( failure() );
    }

    string_1.append(WSTRING_VIEW(L"GHI"));

    if ( string_1.compare(WSTRING_VIEW(L"ABCDEFGHI")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 24;
        return( failure() );
    }

    string_1.push_back( 'J' );

    if ( string_1.compare(WSTRING_VIEW(L"ABCDEFGHIJ")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 25;
        return( failure() );
    }

    string_1.append( reinterpret_cast<wchar_t const *>(byte_buffer) );

    if ( string_1.compare(WSTRING_VIEW(L"ABCDEFGHIJabcd")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 26;
        return( failure() );
    }

    string_2.assign(WSTRING_VIEW(L"WXYZ"));

    SSIZE_T found_at_index = 0;

    string_1.assign(WSTRING_VIEW(L"WXYZ"));

    found_at_index = string_1.find( 'Z' );

    if ( found_at_index != 3 )
    {
        test_number_that_failed = 34;
        return( failure() );
    }

    found_at_index = string_1.find(WSTRING_VIEW(L"XY"));

    if ( found_at_index != 1 )
    {
        test_number_that_failed = 35;
        return( failure() );
    }

    if ( string_1.length() != 4 )
    {
        test_number_that_failed = 37;
        return( failure() );
    }

    string_1.assign(WSTRING_VIEW(L"ABCD12345")); // An Odd-numbered length

    make_reverse(string_1);

    if ( string_1.compare(WSTRING_VIEW(L"54321DCBA")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 38;
        return( failure() );
    }

    make_reverse(string_1);

    if ( string_1.compare(WSTRING_VIEW(L"ABCD12345")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 39;
        return( failure() );
    }

    string_1.assign(WSTRING_VIEW(L"ABCD1234")); // An Even-numbered length

    make_reverse(string_1);

    if ( string_1.compare(WSTRING_VIEW(L"4321DCBA")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 40;
        return( failure() );
    }

    make_reverse(string_1);

    if ( string_1.compare(WSTRING_VIEW(L"ABCD1234")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 41;
        return( failure() );
    }

    string_2.assign( string_1.substr(2, 4 ) );

    if ( string_2.compare(WSTRING_VIEW(L"CD12")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 42;
        return( failure() );
    }

    string_2.assign( string_1.substr( 2 ) );

    if ( string_2.compare(WSTRING_VIEW(L"CD1234")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 43;
        return( failure() );
    }

    string_1.assign(WSTRING_VIEW(L"AZA"));

    string_1.at( 1 ) =  'A';

    if ( string_1.compare(WSTRING_VIEW(L"AAA")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 45;
        return( failure() );
    }

    // CWideString::SpanExcluding()

    string_1.assign(WSTRING_VIEW(L"AABBCCDDYYZZ"));

    // CWideString::TrimLeft()

    string_1.assign(WSTRING_VIEW(L" \t\r  ABCD"));

    trim_left( string_1 );

    if ( string_1.compare(WSTRING_VIEW(L"ABCD")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 48;
        return( failure() );
    }

    // CWideString::TrimRight()

    string_1.assign(WSTRING_VIEW(L"ABCD \t\r "));

    trim_right( string_1 );

    if ( string_1.compare(WSTRING_VIEW(L"ABCD")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 49;
        return( failure() );
    }

    // Whew, those were the easy functions, now let's do the easy operators

    // CWideString operator=( const CWideString& )

    string_2 = string_1;

    if ( string_2.compare(WSTRING_VIEW(L"ABCD")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 50;
        return( failure() );
    }

    string_2 = L'A';

    if ( string_2.compare(WSTRING_VIEW(L"A")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 51;
        return( failure() );
    }

    if ( string_1[ 1 ] != L'B' )
    {
        test_number_that_failed = 52;
        return( failure() );
    }

    string_2 = reinterpret_cast<wchar_t const *>(byte_buffer);

    if ( string_2.compare(WSTRING_VIEW(L"abcd")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 53;
        return( failure() );
    }

    // CWideString::operator += ( CWideString& )

    string_1.assign(WSTRING_VIEW(L"ABC"));
    string_2.assign(WSTRING_VIEW(L"DEF"));

    string_1 += string_2;

    if ( string_1.compare(WSTRING_VIEW(L"ABCDEF")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 54;
        return( failure() );
    }

    // CWideString::operator += ( char )

    string_1 += L'G';

    if ( string_1.compare(WSTRING_VIEW(L"ABCDEFG")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 55;
        return( failure() );
    }

    // CWideString::operator += ( char const * )

    string_1 += L"HIJ";

    if ( string_1.compare(WSTRING_VIEW(L"ABCDEFGHIJ")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 56;
        return( failure() );
    }

    string_1.assign(WSTRING_VIEW(L"ABC"));
    string_2.assign(WSTRING_VIEW(L"DEF"));

    string_3.assign( string_1 + string_2 );

    if ( string_3.compare(WSTRING_VIEW(L"ABCDEF")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 57;
        return( failure() );
    }

    string_3.assign( string_1 + L'D' );

    if ( string_3.compare(WSTRING_VIEW(L"ABCD")) != I_AM_EQUAL_TO_THAT)
    {
        wprintf( L"String should have been \"ABCD\" but it is \"%s\"\n", string_3.c_str() );
        test_number_that_failed = 58;
        return( failure() );
    }

    string_3.assign( L'C' + string_2 );

    if ( string_3.compare(WSTRING_VIEW(L"CDEF")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 59;
        return( failure() );
    }

    string_3.assign( string_1 + L"DEF" );

    if ( string_3.compare(WSTRING_VIEW(L"ABCDEF")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 60;
        return( failure() );
    }

    string_3.assign( L"ABC" + string_2 );

    if ( string_3.compare(WSTRING_VIEW(L"ABCDEF")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 61;
        return( failure() );
    }

    // Enough with the friends aleady. Now is the time for helper operators

    string_1.assign(WSTRING_VIEW(L"ABC"));
    string_2.assign(WSTRING_VIEW(L"ABC"));

    if (not ( string_1 == string_2 ) )
    {
        test_number_that_failed = 62;
        return( failure() );
    }

    if (not ( string_1 == L"ABC" ) )
    {
        test_number_that_failed = 63;
        return( failure() );
    }

    if (not ( L"ABC" == string_1 ) )
    {
        test_number_that_failed = 64;
        return( failure() );
    }

    string_2.assign(WSTRING_VIEW(L"abc"));

    if (not ( string_1 != string_2 ) )
    {
        test_number_that_failed = 65;
        return( failure() );
    }

    // operator != ( CWideString&, char const * )

    if (not ( string_1 != L"abc" ) )
    {
        test_number_that_failed = 66;
        return( failure() );
    }

    // operator != ( char const *, CWideString& )

    if (not ( L"abc" != string_1 ) )
    {
        test_number_that_failed = 67;
        return( failure() );
    }

    // operator < ( CWideString&, CWideString& )

    string_1.assign(WSTRING_VIEW(L"ABC"));
    string_2.assign(WSTRING_VIEW(L"ABc"));

    if (not ( string_1 < string_2 ) )
    {
        test_number_that_failed = 68;
        return( failure() );
    }

    // operator < ( CWideString&, char const * )

    if (not ( string_1 < L"ABc" ) )
    {
        test_number_that_failed = 69;
        return( failure() );
    }

    // operator < ( char const *, CWideString& )

    if (not ( L"AAB" < string_1 ) )
    {
        test_number_that_failed = 70;
        return( failure() );
    }

    // operator > ( CWideString&, CWideString& )

    string_1.assign(WSTRING_VIEW(L"ABc"));
    string_2.assign(WSTRING_VIEW(L"ABC"));

    if (not ( string_1 > string_2 ) )
    {
        test_number_that_failed = 71;
        return( failure() );
    }

    // operator > ( CWideString&, char const * )

    if (not ( string_1 > L"ABC" ) )
    {
        test_number_that_failed = 72;
        return( failure() );
    }

    // operator > ( char const *, CWideString& )

    string_1.assign(WSTRING_VIEW(L"ABC"));

    if (not ( L"ABc" > string_1 ) )
    {
        test_number_that_failed = 73;
        return( failure() );
    }

    // operator <= ( CWideString&, CWideString& )

    // Test the "less than" part of "less than or equal to"

    string_1.assign(WSTRING_VIEW(L"ABC"));
    string_2.assign(WSTRING_VIEW(L"ABc"));

    if (not ( string_1 <= string_2 ) )
    {
        test_number_that_failed = 74;
        return( failure() );
    }

    // operator <= ( CWideString&, char const * )

    if (not ( string_1 <= L"ABc" ) )
    {
        test_number_that_failed = 75;
        return( failure() );
    }

    // operator <= ( char const *, CWideString& )

    if (not ( L"AAB" <= string_1 ) )
    {
        test_number_that_failed = 76;
        return( failure() );
    }

    // Now test the "equal to" part of "less than or equal to"
    // operator <= ( CWideString&, CWideString& )

    string_1.assign(WSTRING_VIEW(L"ABC"));
    string_2.assign(WSTRING_VIEW(L"ABC"));

    if (not ( string_1 <= string_2 ) )
    {
        test_number_that_failed = 77;
        return( failure() );
    }

    // operator <= ( CWideString&, char const * )

    if (not ( string_1 <= L"ABC" ) )
    {
        test_number_that_failed = 78;
        return( failure() );
    }

    // operator <= ( char const *, CWideString& )

    if (not ( L"ABC" <= string_1 ) )
    {
        test_number_that_failed = 79;
        return( failure() );
    }

    // Test the "greater than" part of "greater than or equal to"
    // operator >= ( CWideString&, CWideString& )

    string_1.assign(WSTRING_VIEW(L"ABc"));
    string_2.assign(WSTRING_VIEW(L"ABC"));

    if (not ( string_1 >= string_2 ) )
    {
        test_number_that_failed = 80;
        return( failure() );
    }

    // operator >= ( CWideString&, char const * )

    if (not ( string_1 >= L"ABC" ) )
    {
        test_number_that_failed = 81;
        return( failure() );
    }

    // operator >= ( char const *, CWideString& )

    string_1.assign(WSTRING_VIEW(L"ABC"));

    if (not ( L"ABc" >= string_1 ) )
    {
        test_number_that_failed = 82;
        return( failure() );
    }

    // Now test the "equal to" part of "greater than or equal to"
    // operator >= ( CWideString&, CWideString& )

    string_1.assign(WSTRING_VIEW(L"ABC"));
    string_2.assign(WSTRING_VIEW(L"ABC"));

    if (not ( string_1 >= string_2 ) )
    {
        test_number_that_failed = 83;
        return( failure() );
    }

    // operator >= ( CWideString&, char const * )

    if (not ( string_1 >= L"ABC" ) )
    {
        test_number_that_failed = 84;
        return( failure() );
    }

    // operator >= ( char const *, CWideString& )

    if (not ( L"ABC" >= string_1 ) )
    {
        test_number_that_failed = 85;
        return( failure() );
    }

    string_1.clear();

    if ( string_1.length() != 0 )
    {
        test_number_that_failed = 86;
        return( failure() );
    }

    long number_of_characters_to_test = INTERNAL_STRING_BUFFER_SIZE * 2;

    std::size_t previous_length = 0;

    for ( auto const loop_index : Range(number_of_characters_to_test) )
    {
        previous_length = string_1.length();

        string_1.push_back( 'A' );

        if ( previous_length != (string_1.length() - 1 ) )
        {
            printf( "previous length (%lu) failed, length in error is %lu\n",
                (unsigned long) previous_length,
                (unsigned long) string_1.length() - 1 );

            test_number_that_failed = 87;
            return( failure() );
        }
    }

    if ( string_1.length() != number_of_characters_to_test )
    {
        printf( "Length is %lu while it should have been %lu\n",
            (unsigned long) string_1.length(),
            (unsigned long) number_of_characters_to_test );

        test_number_that_failed = 88;
        return( failure() );
    }

    copy( string_1, STRING_VIEW("0123456789") );

    if ( string_1.length() != 10 )
    {
        test_number_that_failed = 89;
        return( failure() );
    }

    if ( string_1.compare(WSTRING_VIEW(L"0123456789")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 90;
        return( failure() );
    }

    string_1.resize( 9 );

    if ( string_1.length() != 9 )
    {
        printf( "Length should have been 9 but it was %lu\n", (unsigned long) string_1.length() );
        test_number_that_failed = 91;
        return( failure() );
    }

    if ( string_1.compare(WSTRING_VIEW(L"012345678")) != I_AM_EQUAL_TO_THAT)
    {
        printf( "String is \"%S\" when is should have been \"012345678\"\n", string_1.c_str() );
        test_number_that_failed = 92;
        return( failure() );
    }

    string_1.clear();

    if ( string_1.length() != 0 )
    {
        test_number_that_failed = 93;
        return( failure() );
    }

    format( string_1, L"This is the %d", 1 );

    if ( string_1.compare(WSTRING_VIEW(L"This is the 1")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 94;
        return( failure() );
    }

    format( string_1, L"This is the %d, %lu, %02d \"%s\"", 1, static_cast<unsigned long>(2), 3, L"Sam Loves Laura" );

    if ( string_1.compare(WSTRING_VIEW(L"This is the 1, 2, 03 \"Sam Loves Laura\"")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 95;
        return( failure() );
    }

    wchar_t bytes[ 5 ];

    bytes[ 0 ] = 'A';
    bytes[ 1 ] = 'B';
    bytes[ 2 ] = 'C';
    bytes[ 3 ] = 'D';
    bytes[ 4 ] = 0x00;

    string_1 = bytes;

    if ( string_1.compare(WSTRING_VIEW(L"ABCD")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 96;
        return( failure() );
    }

    std::wstring string_6(WSTRING_VIEW(L"ABCD"));

    if ( string_6.compare(WSTRING_VIEW(L"ABCD")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 97;
        return( failure() );
    }

    std::wstring string_7( L"ABCD", 2 );

    if ( string_7.compare( WSTRING_VIEW(L"AB")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 98;
        return( failure() );
    }

    string_7.assign(WSTRING_VIEW(L"WXYZ"));

    if ( string_7.compare(WSTRING_VIEW( L"WXYZ") ) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 99;
        return( failure() );
    }

    // 2000-08-05
    // Thanks go to Tony Lam (lamt@alumni.utexas.net) for finding a bug in STL_CWideString()
    // According to MSDN, CWideString should
    // Extracts the last (that is, rightmost) nCount characters from this CWideString
    // object and returns a copy of the extracted substring. If nCount exceeds the
    // string length, then the entire string is extracted. Right is similar to the
    // Basic RIGHT$ function (except that indexes are zero-based).

    string_6.clear();

    string_6.assign( right( string_7, string_7.length() ) );

    if ( string_6.length() != string_7.length() )
    {
        test_number_that_failed = 100;
        return( failure() );
    }

    if ( string_6.compare( string_7 ) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 101;
        return( failure() );
    }

    string_6.clear();

    string_6.assign( right( string_7, string_7.length() + 99 ) );

    if ( string_6.length() != string_7.length() )
    {
        test_number_that_failed = 102;
        return( failure() );
    }

    if ( string_6.compare( string_7 ) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 103;
        return( failure() );
    }

    string_6.clear();

    string_6.assign( string_7.substr(0, string_7.length() ) );

    if ( string_6.length() != string_7.length() )
    {
        test_number_that_failed = 104;
        return( failure() );
    }

    if ( string_6.compare( string_7 ) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 105;
        return( failure() );
    }

    string_6.clear();

    string_6.assign( string_7.substr( 0, string_7.length() + 99 ) );

    if ( string_6.length() != string_7.length() )
    {
        test_number_that_failed = 106;
        return( failure() );
    }

    if ( string_6.compare( string_7 ) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 107;
        return( failure() );
    }

    string_6.clear();

    string_6.assign(WSTRING_VIEW(L"     ")); // 5 spaces

    trim_right( string_6 );

    if ( string_6.length() != 0 )
    {
        test_number_that_failed = 108;
        return( failure() );
    }

    string_6.assign(WSTRING_VIEW(L"     ")); // 5 spaces

    trim_left(string_6);

    if ( string_6.length() != 0 )
    {
        test_number_that_failed = 109;
        return( failure() );
    }

    string_6.assign(WSTRING_VIEW(L" \t\r\n")); // all space-like characters

    trim_left(string_6);

    if ( string_6.length() != 0 )
    {
        test_number_that_failed = 110;
        return( failure() );
    }

    string_6.assign(WSTRING_VIEW(L" \t\r\n")); // all space-like characters

    trim_right(string_6);

    if ( string_6.length() != 0 )
    {
        test_number_that_failed = 111;
        return( failure() );
    }

    trim_right(string_6 ); // Shouldn't blow up, test 112
    trim_left(string_6); // Shouldn't blow up, test 113

    string_6.assign(WSTRING_VIEW(L"111"));

    trim_right( string_6, L'1');

    if ( string_6.length() != 0 )
    {
        test_number_that_failed = 114;
        return( failure() );
    }

    string_6.assign(WSTRING_VIEW(L"111"));

    trim_left( string_6, L'1' );

    if ( string_6.length() != 0 )
    {
        WFCTRACEINIT( TEXT( "test_CWideString()" ) );
        WFCTRACEVAL( TEXT( "string_6.length() is " ), string_6.length() );
        WFCTRACEVAL( TEXT( "string_6 is " ), string_6.c_str() );
        test_number_that_failed = 115;
        return( failure() );
    }

    string_5.clear();

    string_6.assign(WSTRING_VIEW(L"0123456789"));

    string_5.assign( right( string_6, string_6.length() + 1 ) );

    if ( string_5.compare(WSTRING_VIEW(L"0123456789")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 116;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"BEF"));

    if ( string_5.length() != 3 )
    {
        test_number_that_failed = 118;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"CF"));

    if ( string_5.length() != 2 )
    {
        test_number_that_failed = 133;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"This is a test"));

    remove( string_5, 't' );

    if ( string_5.compare(WSTRING_VIEW(L"This is a es")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 149;
        return( failure() );
    }

    remove( string_5, 'T' );

    if ( string_5.compare(WSTRING_VIEW(L"his is a es")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 151;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"ttt"));

    remove( string_5, 't' );

    if ( string_5.length() !=  0 )
    {
        test_number_that_failed = 153;
        return( failure() );
    }

    remove( string_5, 't' );

    string_5.assign(WSTRING_VIEW(L"ABCDEFGHIJ"));

    remove( string_5, 'K' );

    // Replace()

    string_5.clear();

    if ( string_5.length() !=  0 )
    {
        test_number_that_failed = 156;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"a b c d e f g a b c a"));

    // replace a character at the beginning, in the middle and at the end
    if ( replace( string_5, 'a', 'z' ) != 3 )
    {
        test_number_that_failed = 157;
        return( failure() );
    }

    if ( string_5.compare(WSTRING_VIEW(L"z b c d e f g z b c z")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 158;
        return( failure() );
    }

    string_5.clear();

    if ( string_5.length() !=  0 )
    {
        test_number_that_failed = 159;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"beginbeginbegin"));

    // replace a longer string with a shorter one
    replace(string_5, WSTRING_VIEW(L"begin"), WSTRING_VIEW(L"end"));

    if ( string_5.compare(WSTRING_VIEW(L"endendend")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 161;
        return( failure() );
    }

    string_5.clear();

    if ( string_5.length() !=  0 )
    {
        test_number_that_failed = 162;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"beginbeginbegin"));

    // replace a shorter string with a longer one
    replace(string_5, WSTRING_VIEW(L"begin"), WSTRING_VIEW(L"theend"));

    if ( string_5.compare(WSTRING_VIEW(L"theendtheendtheend")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 164;
        return( failure() );
    }

    string_5.clear();

    if ( string_5.length() !=  0 )
    {
        test_number_that_failed = 165;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"beginbeginbegin"));

    // replace a string with a same-sized one
    replace(string_5, WSTRING_VIEW(L"begin"), WSTRING_VIEW(L"start"));

    if ( string_5.compare(WSTRING_VIEW(L"startstartstart")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 167;
        return( failure() );
    }

    string_5.clear();

    if ( string_5.length() !=  0 )
    {
        test_number_that_failed = 168;
        return( failure() );
    }

    // Now let's get a little tricky, replace one string with another that
    // produces the first string.

    string_5.assign(WSTRING_VIEW(L"begineginbeginegin"));

    replace(string_5, WSTRING_VIEW(L"begin"), WSTRING_VIEW(L"b"));

    if ( string_5.compare(WSTRING_VIEW(L"beginbegin")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 170;
        return( failure() );
    }

    test_number_that_failed = 170;

    string_5.clear();

    if ( string_5.length() != 0 )
    {
        test_number_that_failed = 171;
        return( failure() );
    }

    copy( string_5, STRING_VIEW("ABCDEFGHIJK") );

    if ( string_5.length() != 11 )
    {
        test_number_that_failed = 172;
        return( failure() );
    }

    if ( string_5.compare(WSTRING_VIEW(L"ABCDEFGHIJK")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 173;
        return( failure() );
    }

    string_5.clear();

    if ( string_5.length() != 0 )
    {
        test_number_that_failed = 174;
        return( failure() );
    }

    copy( string_5, STRING_VIEW("ABCDEFGHIJKL"), 11 );

    if ( string_5.length() != 11 )
    {
        test_number_that_failed = 175;
        return( failure() );
    }

    if ( string_5.compare(WSTRING_VIEW(L"ABCDEFGHIJK")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 176;
        return( failure() );
    }

    string_5.clear();

    if ( string_5.length() != 0 )
    {
        test_number_that_failed = 177;
        return( failure() );
    }

    copy( string_5, STRING_VIEW("ABCDEFGHIJKL"), 11, 1 );

    if ( string_5.length() != 11 )
    {
        test_number_that_failed = 178;
        return( failure() );
    }

    if ( string_5.compare(WSTRING_VIEW(L"BCDEFGHIJKL")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 179;
        return( failure() );
    }

    string_5.clear();

    if ( string_5.length() != 0 )
    {
        test_number_that_failed = 180;
        return( failure() );
    }

    copy( string_5, STRING_VIEW("ABCDEFGHIJKL"), 99 );

    if ( string_5.length() != 12 )
    {
        test_number_that_failed = 181;
        return( failure() );
    }

    string_5.clear();

    if ( string_5.length() != 0 )
    {
        test_number_that_failed = 183;
        return( failure() );
    }

    copy( string_5, STRING_VIEW("ABC") );

    if ( string_5.length() != 3 )
    {
        test_number_that_failed = 184;
        return( failure() );
    }

    if ( string_5.compare(WSTRING_VIEW(L"ABC")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 185;
        return( failure() );
    }

    copy( string_5, STRING_VIEW("ABCDEFGHIJKL"), 0, 99 );

    if ( string_5.length() != 0 )
    {
        test_number_that_failed = 186;
        return( failure() );
    }

    string_5.clear();

    if ( string_5.length() != 0 )
    {
        test_number_that_failed = 187;
        return( failure() );
    }

    copy( string_5, STRING_VIEW("ABCDEFGHIJKL"), 99, 99 );

    if ( string_5.length() != 0 )
    {
        test_number_that_failed = 188;
        return( failure() );
    }

    string_5.clear();

    if ( string_5.length() != 0 )
    {
        test_number_that_failed = 189;
        return( failure() );
    }

    copy( string_5, STRING_VIEW("ABCDEFGHIJKL"), 1, 99 );

    if ( string_5.length() != 0 )
    {
        test_number_that_failed = 190;
        return( failure() );
    }

    int64_t integer_value = 0;

    string_5.assign(WSTRING_VIEW(L" 16 "));

    int64_t test_value = as_integer(string_5);

    if ( test_value != 16 )
    {
        test_number_that_failed = 191;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L" x16 "));

    test_value = as_integer(string_5);

    if ( test_value != 22 )
    {
        wprintf( L"\"%s\" evaluated to %I64d not 22\n", string_5.c_str(), test_value );
        test_number_that_failed = 192;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L" 0x17 "));

    test_value = as_integer(string_5);

    if ( test_value != 23 )
    {
        test_number_that_failed = 193;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"  1  2   3    4     5 "));

    despace(string_5);

    if ( string_5.compare(WSTRING_VIEW(L"1 2 3 4 5")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 194;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"1 2   3    4     5 "));

    despace(string_5);

    if ( string_5.compare(WSTRING_VIEW(L"1 2 3 4 5")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 195;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"1 2   3    4     5"));

    despace(string_5);

    if ( string_5.compare(WSTRING_VIEW(L"1 2 3 4 5")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 196;
        return( failure() );
    }

    string_5.clear();

    string_5.assign(WSTRING_VIEW(L"123"));

    if ( as_integer(string_5) != 123 )
    {
        test_number_that_failed = 197;
        return( failure() );
    }

    string_5.clear();

    string_5.push_back( ' ' );
    string_5.push_back( TAB_CHARACTER );
    string_5.push_back( CARRIAGE_RETURN );
    string_5.push_back( LINE_FEED );

    if ( is_all_space(string_5) == false )
    {
        test_number_that_failed = 198;
        return( failure() );
    }

    string_5.push_back( '.' );

    if ( is_all_space(string_5) != false )
    {
        test_number_that_failed = 199;
        return( failure() );
    }

    static constexpr DEFINED_WIDE_STRING defines[] =
    {
        { 1, L"1" },
        { 2, L"2" },
        { 3, L"3" },
        { 4, L"4" },
        { 5, L"5" },
    };

    string_5.clear();

    from_defined( string_5, 1, defines, 1 );

    if ( string_5.compare(WSTRING_VIEW(L"1")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 200;
        return( failure() );
    }

    from_defined( string_5, 1, defines, 2 );

    if ( string_5.compare(WSTRING_VIEW(L"1")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 201;
        return( failure() );
    }

    from_defined( string_5, 1, defines, std::size( defines ) );

    if ( string_5.compare(WSTRING_VIEW(L"1")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 202;
        return( failure() );
    }

    from_defined( string_5, 3, defines, std::size( defines ) );

    if ( string_5.compare(WSTRING_VIEW(L"3")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 203;
        return( failure() );
    }

    from_defined( string_5, 5, defines, std::size( defines ) );

    if ( string_5.compare(WSTRING_VIEW(L"5")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 204;
        return( failure() );
    }

    static constexpr wchar_t const * flags[] =
    {
        L"0",
        nullptr,
        L"2",
        L"3",
        L"4",
        L"5",
        L"6",
        L"7",
        L"8",
        L"9",
        L"10",
        L"11",
        L"12",
        L"13",
        L"14",
        L"15",
        L"16",
        L"17",
        L"18",
        L"19",

        L"20",
        L"21",
        L"22",
        L"23",
        L"24",
        L"25",
        L"26",
        L"27",
        L"28",
        L"29",

        L"30",
        L"31",
        L"32",
    };

    string_5.clear();

    from_flags( string_5, 0x00, flags, std::size( flags ) );

    if ( string_5.empty() == false )
    {
        test_number_that_failed = 205;
        return( failure() );
    }

    from_flags(string_5, 0x01, flags, std::size( flags ) );

    if ( string_5.compare(WSTRING_VIEW(L"0")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 206;
        return( failure() );
    }

    from_flags( string_5, 0x07, flags, std::size( flags ) );

    if ( string_5.compare(WSTRING_VIEW(L"0, Undefined(1), 2")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 207;
        return( failure() );
    }

    uint64_t test_flags = 1;

    test_flags <<= 32;

    from_flags( string_5, test_flags, flags, std::size( flags ) );

    if ( string_5.compare(WSTRING_VIEW(L"32")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 208;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"'\"'\"5\"'\"'"));

    (void)trim_quotes( string_5 );

    if ( string_5.compare(WSTRING_VIEW(L"\"'\"5\"'\"")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 209;
        return( failure() );
    }

    trim_all_quotes(string_5);

    if ( string_5.compare(WSTRING_VIEW(L"5")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 210;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L" 1 2 3 4 5 a b c z Y"));

    string_5.assign(WSTRING_VIEW(L"ABC123"));

    string_4.assign( skip_first( string_5, 3 ) );

    if ( string_4.compare(WSTRING_VIEW(L"123")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 215;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"123ABc"));

    if ( ends_with( string_5, L"ABc") == false )
    {
        test_number_that_failed = 218;
        return( failure() );
    }

    if ( ends_with( string_5, L"abc" ) == true )
    {
        test_number_that_failed = 219;
        return( failure() );
    }

    if ( ends_with_no_case( string_5, L"abc" ) != true )
    {
        test_number_that_failed = 220;
        return( failure() );
    }

    if ( starts_with( string_5, WSTRING_VIEW(L"123") ) != true )
    {
        test_number_that_failed = 221;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"AbC123"));

    if ( starts_with_no_case( string_5, WSTRING_VIEW(L"abc") ) != true )
    {
        test_number_that_failed = 222;
        return( failure() );
    }

    wchar_t output_string[ 128 ];

    ZeroMemory( output_string, sizeof( output_string ) );
    memset( output_string, 'T', sizeof( output_string ) - 2 );

    string_5.assign(WSTRING_VIEW(L"SRBLLB"));

    copy_to( string_5, output_string, std::size( output_string ) );

    if ( wcslen( output_string ) != 6 )
    {
        test_number_that_failed = 223;
        return( failure() );
    }

    if ( string_5.compare( output_string ) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 224;
        return( failure() );
    }

    string_5.assign(WSTRING_VIEW(L"BBD"));

    if ( ends_with_no_case(string_5, L"bbd" ) != true )
    {
        test_number_that_failed = 225;
        return( failure() );
    }

    std::vector<std::wstring> string_array;

    (void) string_array.push_back(L"1a1");
    (void) string_array.push_back(L"2b2");
    (void) string_array.push_back(L"1a1");

    if ( string_array.size() != 3 )
    {
        test_number_that_failed = 226;
        return( failure() );
    }

    remove_no_case( string_array, WSTRING_VIEW(L"1A1") );

    if ( string_array.size() != 1 )
    {
        test_number_that_failed = 227;
        return( failure() );
    }

    string_5.assign( string_array.at(0) );

    if ( string_5.compare(WSTRING_VIEW(L"2b2")) != I_AM_EQUAL_TO_THAT )
    {
        test_number_that_failed = 228;
        return( failure() );
    }

    string_1.clear();
    string_2.clear();

    string_1.assign(WSTRING_VIEW(L"AAAB"));

    found_at_index = string_1.find( 'B', 3 );

    if ( found_at_index != 3 )
    {
        test_number_that_failed = 228;
        return( failure() );
    }

    string_1.assign(WSTRING_VIEW(L"YZYZTZ"));

    found_at_index = string_1.find( L"TZ", 1 );

    if ( found_at_index != 4 )
    {
        test_number_that_failed = 229;
        return( failure() );
    }

    string_2.assign(WSTRING_VIEW(L"0123456789"));

    string_1.append( string_2, 7 );

    if ( string_1.compare(WSTRING_VIEW(L"YZYZTZ789")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 230;
        return( failure() );
    }

    string_array.clear();

    if ( string_array.size() != 0 )
    {
        test_number_that_failed = 231;
        return( failure() );
    }

    string_array.push_back(L"0");
    string_array.push_back(L"Aa");
    string_array.push_back(L"Bb");
    string_array.push_back(L"Cc");

    if ( string_array.size() != 4 )
    {
        test_number_that_failed = 232;
        return( failure() );
    }

    std::vector<std::wstring> other_array;

    other_array.push_back(L"One");
    other_array.push_back(L"Two");
    other_array.push_back(L"Three");

    if ( other_array.size() != 3 )
    {
        test_number_that_failed = 238;
        return( failure() );
    }

    join( other_array, std::wstring_view(nullptr,0), string_1 );

    if ( string_1.compare(WSTRING_VIEW(L"OneTwoThree")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 239;
        return( failure() );
    }

    string_1.clear();

    if ( string_1.length() != 0 )
    {
        test_number_that_failed = 240;
        return( failure() );
    }

    join( other_array, WSTRING_VIEW(L""), string_1 );

    if ( string_1.compare(WSTRING_VIEW(L"OneTwoThree")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 241;
        return( failure() );
    }

    string_1.clear();

    if ( string_1.length() != 0 )
    {
        test_number_that_failed = 242;
        return( failure() );
    }

    join( other_array, WSTRING_VIEW(L"."), string_1 );

    if ( string_1.compare(WSTRING_VIEW(L"One.Two.Three")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 243;
        return( failure() );
    }

    string_1.clear();

    if ( string_1.length() != 0 )
    {
        test_number_that_failed = 244;
        return( failure() );
    }

    join( other_array, WSTRING_VIEW(L" and a "), string_1 );

    if ( string_1.compare(WSTRING_VIEW(L"One and a Two and a Three")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 245;
        return( failure() );
    }

    string_1.assign(WSTRING_VIEW(L"3.14"));

    double const test_double = as_double( string_1 );

    if ( test_double < 3.14 or test_double > 3.141 )
    {
        test_number_that_failed = 246;
        return(failure());
    }

    if (is_all_decimal_digits( string_1 ) == true)
    {
        test_number_that_failed = 247;
        return(failure());
    }

    string_1.assign(WSTRING_VIEW(L"01293784529346"));

    if (is_all_decimal_digits( string_1 ) == false)
    {
        test_number_that_failed = 248;
        return(failure());
    }

    if ( is_all_hex_digits(string_1) == false)
    {
        test_number_that_failed = 249;
        return(failure());
    }

    string_1.assign(WSTRING_VIEW(L"012aBcDEf9"));

    if (is_all_hex_digits( string_1 ) == false)
    {
        test_number_that_failed = 250;
        return(failure());
    }

    string_1.assign(WSTRING_VIEW(L"ABCDEFG"));

    if (is_all_hex_digits( string_1) == true)
    {
        test_number_that_failed = 251;
        return(failure());
    }

    // Test the search routines for failure...

    found_at_index = 0;

    string_1.assign(WSTRING_VIEW(L"WXYZ"));

    found_at_index = string_1.find(L'U');

    if (found_at_index != std::wstring::npos)
    {
        test_number_that_failed = 252;
        return(failure());
    }

    found_at_index = string_1.find(WSTRING_VIEW(L"Z1"));

    if (found_at_index != std::wstring::npos)
    {
        test_number_that_failed = 253;
        return(failure());
    }

    found_at_index = string_1.find(WSTRING_VIEW(L"YZ"));

    if (found_at_index != 2)
    {
        test_number_that_failed = 254;
        return(failure());
    }

    string_1.assign(WSTRING_VIEW(L"BCDEF"));

    found_at_index = string_1.find_first_of(WSTRING_VIEW(L"AbZ"));

    if (found_at_index != std::wstring::npos)
    {
        test_number_that_failed = 256;
        return(failure());
    }

    std::vector<std::wstring> sorted;

    SSIZE_T where_it_was_inserted = add_to_unique_sorted_vector(WSTRING_VIEW(L"&amp;"), sorted);

    if (sorted.size() != 1)
    {
        test_number_that_failed = 257;
        return(failure());
    }

    if (where_it_was_inserted != 0)
    {
        test_number_that_failed = 258;
        return(failure());
    }

    where_it_was_inserted = add_to_unique_sorted_vector(WSTRING_VIEW(L"&apos;"), sorted);

    if (sorted.size() != 2)
    {
        test_number_that_failed = 259;
        return(failure());
    }

    if (where_it_was_inserted != 1)
    {
        test_number_that_failed = 260;
        return(failure());
    }

    if (sorted[0].compare(WSTRING_VIEW(L"&amp;")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 261;
        return(failure());
    }

    if (sorted[1].compare(WSTRING_VIEW(L"&apos;")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 262;
        return(failure());
    }

    where_it_was_inserted = add_to_unique_sorted_vector(WSTRING_VIEW(L"&amp;"), sorted);

    if (sorted.size() != 2)
    {
        test_number_that_failed = 263;
        return(failure());
    }

    if (sorted[0].compare(WSTRING_VIEW(L"&amp;")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 264;
        return(failure());
    }

    if (sorted[1].compare(WSTRING_VIEW(L"&apos;")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 265;
        return(failure());
    }

    if (where_it_was_inserted != 0)
    {
        test_number_that_failed = 266;
        return(failure());
    }

    where_it_was_inserted = add_to_unique_sorted_vector(WSTRING_VIEW(L"&apos;"), sorted);

    if (sorted.size() != 2)
    {
        test_number_that_failed = 267;
        return(failure());
    }

    if (sorted[0].compare(WSTRING_VIEW(L"&amp;")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 268;
        return(failure());
    }

    if (sorted[1].compare(WSTRING_VIEW(L"&apos;")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 269;
        return(failure());
    }

    if (where_it_was_inserted != 1)
    {
        test_number_that_failed = 270;
        return(failure());
    }

    where_it_was_inserted = add_to_unique_sorted_vector(WSTRING_VIEW(L"&aaaa;"), sorted);

    if (sorted.size() != 3)
    {
        test_number_that_failed = 271;
        return(failure());
    }

    if (sorted[0].compare(WSTRING_VIEW(L"&aaaa;")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 272;
        return(failure());
    }

    if (sorted[1].compare(WSTRING_VIEW(L"&amp;")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 273;
        return(failure());
    }

    if (sorted[2].compare(WSTRING_VIEW(L"&apos;")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 274;
        return(failure());
    }

    if (where_it_was_inserted != 0)
    {
        test_number_that_failed = 275;
        return(failure());
    }

    std::wstring ss1(WSTRING_VIEW(L"ABCDEF"));
    std::wstring ss2(WSTRING_VIEW(L"abcdef"));

    if (compare_no_case(ss1, ss2) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 276;
        return(failure());
    }

    std::filesystem::path p1(L"C:\\t1\\t2\\t3.txt");

    format(ss2, L"%s==", p1);

    if (ss2.compare(WSTRING_VIEW(L"C:\\t1\\t2\\t3.txt==")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 277;
        return(failure());
    }

    // Now test the ASCII as_integer

    std::string a_string(" x16 ");

    test_value = as_integer(a_string);

    if (test_value != 22)
    {
        wprintf(L"\"%S\" evaluated to %I64d not 22\n", a_string.c_str(), test_value);
        test_number_that_failed = 278;
        return(failure());
    }

    a_string.assign(STRING_VIEW(" 0x17 "));

    test_value = as_integer(a_string);

    if (test_value != 23)
    {
        wprintf(L"\"%S\" evaluated to %I64d not 23\n", a_string.c_str(), test_value);
        test_number_that_failed = 279;
        return(failure());
    }

    a_string.assign(STRING_VIEW("24"));

    test_value = as_integer(a_string);

    if (test_value != 24)
    {
        wprintf(L"\"%S\" evaluated to %I64d not 24\n", a_string.c_str(), test_value);
        test_number_that_failed = 280;
        return(failure());
    }

    test_value = ascii_string_to_integer("567", 2, 10);

    if (test_value != 56)
    {
        wprintf(L"\"567\" evaluated to %I64d not 56\n", test_value);
        test_number_that_failed = 281;
        return(failure());
    }

    a_string.assign(STRING_VIEW("X24"));

    test_value = as_integer(a_string);

    if (test_value != 36)
    {
        wprintf(L"\"%S\" evaluated to %I64d not 36\n", a_string.c_str(), test_value);
        test_number_that_failed = 282;
        return(failure());
    }

    uint8_t test_bytes[]{ 1, 2, 129, 200, 201 };

    copy(string_5, test_bytes, sizeof(test_bytes));

    if (string_5.length() != sizeof(test_bytes))
    {
        test_number_that_failed = 283;
        return(failure());
    }

    if (string_5[0] != 1)
    {
        test_number_that_failed = 284;
        return(failure());
    }

    if (string_5[1] != 2)
    {
        test_number_that_failed = 285;
        return(failure());
    }

    if (string_5[2] != 129)
    {
        test_number_that_failed = 286;
        return(failure());
    }

    if (string_5[3] != 200)
    {
        test_number_that_failed = 287;
        return(failure());
    }

    if (string_5[4] != 201)
    {
        test_number_that_failed = 288;
        return(failure());
    }

    // 2014-05-10T19:21:18-0400

    CFileTime ft;

    wfc_parse_iso_8601_string(WSTRING_VIEW(L"2014-05-10T19:21:18-0400"), ft);

    test_number_that_failed = 288;
    return( true );
}
