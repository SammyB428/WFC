/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2016, Samuel R. Blackburn
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

#if ! defined( INTERNAL_STRING_BUFFER_SIZE )
#define INTERNAL_STRING_BUFFER_SIZE (16)
#endif // INTERNAL_STRING_BUFFER_SIZE

__checkReturn bool test_CString( __out std::string& class_name, __out int& test_number_that_failed ) noexcept
{
    class_name.assign( "CString" );

    return(true);

    CString string_1;

    if ( string_1.GetLength() != 0 )
    {
        test_number_that_failed = 1;
        return( failure() );
    }

#if defined( WFC_STL )

    string_1.Copy( TEXT( "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" ) );

#else // WFC_STL

    string_1 = TEXT( "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" );

#endif // WFC_STL

    if ( string_1.GetLength() != 36 )
    {
        test_number_that_failed = 2;
        return(failure());
    }

    CString string_2( string_1 );

    if ( string_1.Compare( string_2 ) != 0 )
    {
        test_number_that_failed = 3;
        return(failure());
    }

    string_1.MakeLower();

    if ( string_1.Compare( TEXT( "abcdefghijklmnopqrstuvwxyz0123456789" ) ) != 0 )
    {
        test_number_that_failed = 4;
        return(failure());
    }

    string_1.MakeUpper();

    if ( string_1.Compare( string_2 ) != 0 )
    {
        test_number_that_failed = 5;
        return(failure());
    }

    string_2.Copy( string_1.Left( 26 ) );

    if ( string_2.Compare( TEXT( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" ) ) != 0 )
    {
        test_number_that_failed = 6;
        return(failure());
    }

    if ( string_2.CompareNoCase( TEXT( "abcDEFGHIJKLMNOPQRSTUVWXYZ" ) ) != 0 )
    {
        test_number_that_failed = 7;
        return(failure());
    }

    string_2.Copy( string_1.Right( 10 ) );

    if ( string_2.Compare( TEXT( "0123456789" ) ) != 0 )
    {
        test_number_that_failed = 8;
        return(failure());
    }

    // OK, we've got the basic tests out of the way, now let's methodically
    // go through the interface and make sure we test everything.

    string_2.Copy( CString( TEXT( 'A' ) ) );

    if ( string_2.GetLength() != 1 )
    {
        test_number_that_failed = 9;
        return(failure());
    }

    if ( string_2.GetAt( 0 ) != TEXT( 'A' ) )
    {
        test_number_that_failed = 10;
        return(failure());
    }

    // Now test to see if we can add more than one character...

    string_2.Copy( CString( TEXT( 'A' ), 5 ) );

    if ( string_2.GetLength() != 5 )
    {
        test_number_that_failed = 11;
        return(failure());
    }

    if ( string_2.GetAt( 4 ) != TEXT( 'A' ) )
    {
        test_number_that_failed = 12;
        return(failure());
    }

    CString string_3( TEXT( "ABCDE" ) );

    if ( string_3.GetLength() != 5 )
    {
        test_number_that_failed = 13;
        return(failure());
    }

    if ( string_3.GetAt( 4 ) != TEXT( 'E' ) )
    {
        test_number_that_failed = 14;
        return(failure());
    }

    if ( string_3.Compare( TEXT( "ABCDE" ) ) != 0 )
    {
        test_number_that_failed = 15;
        return(failure());
    }

    // CString::CString( unsigned char *, long )

    unsigned char byte_buffer[ 5 ];

    byte_buffer[ 0 ] = 'a';
    byte_buffer[ 1 ] = 'b';
    byte_buffer[ 2 ] = 'c';
    byte_buffer[ 3 ] = 'd';
    byte_buffer[ 4 ] = 0xFF; // NOT NULL TERMINATED!

    CString string_4( (const char *) byte_buffer, 4 );

    if ( string_4.GetLength() != 4 )
    {
        test_number_that_failed = 16;
        return(failure());
    }

    if ( string_4.GetAt( 3 ) != 'd' )
    {
        test_number_that_failed = 17;
        return(failure());
    }

    if ( string_4.Compare( TEXT( "abcd" ) ) != 0 )
    {
        test_number_that_failed = 18;
        return(failure());
    }

    byte_buffer[ 4 ] = 0x00; // NULL TERMINATE

    CString string_5( (const char *) byte_buffer );

    if ( string_5.GetLength() != 4 )
    {
        test_number_that_failed = 19;
        return(failure());
    }

    if ( string_5.GetAt( 3 ) != 'd' )
    {
        test_number_that_failed = 20;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "abcd" ) ) != 0 )
    {
        test_number_that_failed = 21;
        return(failure());
    }

    string_5.Empty();

    if ( string_5.GetLength() != 0 )
    {
        test_number_that_failed = 22;
        return(failure());
    }

    if ( string_5.IsEmpty() != true )
    {
        test_number_that_failed = 23;
        return(failure());
    }

    // Be careful to use only functions that have already been tested above

#if defined( WFC_STL )

    string_1.Copy( TEXT( "ABC" ) );
    string_2.Copy( TEXT( "DEF" ) );

    string_1.Append( string_2 );

#else // WFC_STL

    string_1 = TEXT( "ABC" );
    string_2 = TEXT( "DEF" );

    string_1 += string_2;

#endif // WFC_STL

    if ( string_1.Compare( TEXT( "ABCDEF" ) ) != 0 )
    {
        test_number_that_failed = 23;
        return(failure());
    }

#if defined( WFC_STL )

    string_1.Append( TEXT( "GHI" ) );

#else // WFC_STL

    string_1 += TEXT( "GHI" );

#endif // WFC_STL

    if ( string_1.Compare( TEXT( "ABCDEFGHI" ) ) != 0 )
    {
        test_number_that_failed = 24;
        return(failure());
    }

#if defined( WFC_STL )

    string_1.Append( TEXT( 'J' ) );

#else // WFC_STL

    string_1 += TEXT( 'J' );

#endif // WFC_STL

    if ( string_1.Compare( TEXT( "ABCDEFGHIJ" ) ) != 0 )
    {
        test_number_that_failed = 25;
        return(failure());
    }

#if defined( WFC_STL )

    string_1.Append( (const char *) byte_buffer );

#else // WFC_STL

    string_1 += (const char *) byte_buffer;

#endif // WFC_STL

    if ( string_1.Compare( TEXT( "ABCDEFGHIJabcd" ) ) != 0 )
    {
        test_number_that_failed = 26;
        return(failure());
    }

    if ( string_1.Collate( TEXT( "ABCDEFGHIJabcd" ) ) != 0 )
    {
        test_number_that_failed = 27;
        return(failure());
    }

#if defined( WFC_STL )

    string_1.Copy( (const char *) byte_buffer, 2, 1 );

    if ( string_1.GetLength() != 2 )
    {
        test_number_that_failed = 28;
        return(failure());
    }

    if ( string_1.Compare( TEXT( "bc" ) ) != 0 )
    {
        test_number_that_failed = 29;
        return(failure());
    }

    // Test the non-unsigned version of Copy()

    string_1.Copy( (const char *) "abcd", 2, 1 );

    if ( string_1.GetLength() != 2 )
    {
        test_number_that_failed = 30;
        return(failure());
    }

    if ( string_1.Compare( TEXT( "bc" ) ) != 0 )
    {
        test_number_that_failed = 31;
        return(failure());
    }

    // CString::Copy( const CString&, long, long )

    string_2.Copy( TEXT( "WXYZ" ) );

    string_1.Copy( string_2, 2, 1 );

    if ( string_1.GetLength() != 2 )
    {
        test_number_that_failed = 32;
        return(failure());
    }

    if ( string_1.Compare( TEXT( "XY" ) ) != 0 )
    {
        test_number_that_failed = 33;
        return(failure());
    }

#endif // WFC_STL

    // CString::Find( char )

    SSIZE_T found_at_index = 0;

    string_1.Copy( TEXT( "WXYZ" ) );

    found_at_index = string_1.Find( TEXT( 'Z' ) );

    if ( found_at_index != 3 )
    {
        test_number_that_failed = 34;
        return(failure());
    }

    // CString::Find( char * )

    found_at_index = string_1.Find( TEXT( "XY" ) );

    if ( found_at_index != 1 )
    {
        test_number_that_failed = 35;
        return(failure());
    }

    // CString::FindOneOf()

    found_at_index = string_1.FindOneOf( TEXT( "AbZ" ) );

    if ( found_at_index != 3 )
    {
        test_number_that_failed = 36;
        return(failure());
    }

#if defined( WFC_STL )

    if ( string_1.GetSize() != 4 )

#else // WFC_STL

    if ( string_1.GetLength() != 4 )

#endif // WFC_STL
    {
        test_number_that_failed = 37;
        return(failure());
    }

    string_1.Copy( TEXT( "ABCD12345" ) ); // An Odd-numbered length

    string_1.MakeReverse();

    if ( string_1.Compare( TEXT( "54321DCBA" ) ) != 0 )
    {
        test_number_that_failed = 38;
        return(failure());
    }

    string_1.MakeReverse();

    if ( string_1.Compare( TEXT( "ABCD12345" ) ) != 0 )
    {
        test_number_that_failed = 39;
        return(failure());
    }

    string_1.Copy( TEXT( "ABCD1234" ) ); // An Even-numbered length

    string_1.MakeReverse();

    if ( string_1.Compare( TEXT( "4321DCBA" ) ) != 0 )
    {
        test_number_that_failed = 40;
        return(failure());
    }

    string_1.MakeReverse();

    if ( string_1.Compare( TEXT( "ABCD1234" ) ) != 0 )
    {
        test_number_that_failed = 41;
        return(failure());
    }

    // CString::Mid( long, long )

    string_2.Copy( string_1.Mid( 2, 4 ) );

    if ( string_2.Compare( TEXT( "CD12" ) ) != 0 )
    {
        test_number_that_failed = 42;
        return(failure());
    }

    // CString::Mid( long )

    string_2.Copy( string_1.Mid( 2 ) );

    if ( string_2.Compare( TEXT( "CD1234" ) ) != 0 )
    {
        test_number_that_failed = 43;
        return(failure());
    }

    // CString::ReverseFind()

    string_1.Copy( "AZA" );

    found_at_index = string_1.ReverseFind( 'A' );

    if ( found_at_index != 2 )
    {
        test_number_that_failed = 44;
        return(failure());
    }

    // CString::SetAt()

    string_1.SetAt( 1, 'A' );

    if ( string_1.Compare( TEXT( "AAA" ) ) != 0 )
    {
        test_number_that_failed = 45;
        return(failure());
    }

    // CString::SpanExcluding()

    string_1.Copy( TEXT( "AABBCCDDYYZZ" ) );

    string_2.Copy( string_1.SpanExcluding( TEXT( "ZY" ) ) );

    if ( string_2.Compare( TEXT( "AABBCCDD" ) ) != 0 )
    {
        test_number_that_failed = 46;
        return(failure());
    }

    // CString::SpanIncluding()

    string_2.Copy( string_1.SpanIncluding( TEXT( "BAC" ) ) );

    if ( string_2.Compare( TEXT( "AABBCC" ) ) != 0 )
    {
        test_number_that_failed = 47;
        return(failure());
    }

    // CString::TrimLeft()

    string_1.Copy( TEXT( " \t\r  ABCD" ) );

    string_1.TrimLeft();

    if ( string_1.Compare( TEXT( "ABCD" ) ) != 0 )
    {
        test_number_that_failed = 48;
        return(failure());
    }

    // CString::TrimRight()

    string_1.Copy( TEXT( "ABCD \t\r " ) );

    string_1.TrimRight();

    if ( string_1.Compare( TEXT( "ABCD" ) ) != 0 )
    {
        test_number_that_failed = 49;
        return(failure());
    }

    // Whew, those were the easy functions, now let's do the easy operators

    // CString operator=( const CString& )

    string_2 = string_1;

    if ( string_2.Compare( TEXT( "ABCD" ) ) != 0 )
    {
        test_number_that_failed = 50;
        return(failure());
    }

    // CString::operator=( char )

    string_2 = 'A';

    if ( string_2.Compare( TEXT( "A" ) ) != 0 )
    {
        test_number_that_failed = 51;
        return(failure());
    }

    // CString::operator[]

#if defined( WFC_STL )
    if ( string_1[ (SIZE_T) 1 ] != 'B' )
    {
        test_number_that_failed = 52;
        return(failure());
    }
#endif

    // CString::operator=( const unsigned char )

    string_2 = byte_buffer;

    if ( string_2.Compare( TEXT( "abcd" ) ) != 0 )
    {
        test_number_that_failed = 53;
        return(failure());
    }

    // CString::operator += ( CString& )

    string_1.Copy( TEXT( "ABC" ) );
    string_2.Copy( TEXT( "DEF" ) );

    string_1 += string_2;

    if ( string_1.Compare( TEXT( "ABCDEF" ) ) != 0 )
    {
        test_number_that_failed = 54;
        return(failure());
    }

    // CString::operator += ( char )

    string_1 += 'G';

    if ( string_1.Compare( TEXT( "ABCDEFG" ) ) != 0 )
    {
        test_number_that_failed = 55;
        return(failure());
    }

    // CString::operator += ( const char * )

    string_1 += TEXT( "HIJ" );

    if ( string_1.Compare( TEXT( "ABCDEFGHIJ" ) ) != 0 )
    {
        test_number_that_failed = 56;
        return(failure());
    }

    // friend + ( CString&, CString& )

    string_1.Copy( TEXT( "ABC" ) );
    string_2.Copy( TEXT( "DEF" ) );

    string_3 = string_1 + string_2;

    if ( string_3.Compare( TEXT( "ABCDEF" ) ) != 0 )
    {
        test_number_that_failed = 57;
        return(failure());
    }

    // friend + ( CString&, char )

    string_3.Copy( string_1 + TEXT( 'D' ) );

    if ( string_3.Compare( TEXT( "ABCD" ) ) != 0 )
    {
        _tprintf( TEXT( "String should have been \"ABCD\" but it is \"%s\"\n" ), (LPCTSTR) string_3 );
        test_number_that_failed = 58;
        return(failure());
    }

    // friend + ( char, CString& )

    string_3 = TEXT( 'C' ) + string_2;

    if ( string_3.Compare( TEXT( "CDEF" ) ) != 0 )
    {
        test_number_that_failed = 59;
        return(failure());
    }

    // friend + ( CString&, const char * )

    string_3.Copy( string_1 + TEXT( "DEF" ) );

    if ( string_3.Compare( TEXT( "ABCDEF" ) ) != 0 )
    {
        test_number_that_failed = 60;
        return(failure());
    }

    // friend + ( const char *, CString )

    string_3.Copy( TEXT( "ABC" ) + string_2 );

    if ( string_3.Compare( TEXT( "ABCDEF" ) ) != 0 )
    {
        test_number_that_failed = 61;
        return(failure());
    }

    // Enough with the friends aleady. Now is the time for helper operators

    // operator == ( CString&, CString& )

    string_1.Copy( TEXT( "ABC" ) );
    string_2.Copy( TEXT( "ABC" ) );

    if ( ! ( string_1 == string_2 ) )
    {
        test_number_that_failed = 62;
        return(failure());
    }

    // operator == ( CString&, const char * )

    if ( ! ( string_1 == TEXT( "ABC" ) ) )
    {
        test_number_that_failed = 63;
        return(failure());
    }

    // operator == ( const char *, CString& )

    if ( ! ( TEXT( "ABC" ) == string_1 ) )
    {
        test_number_that_failed = 64;
        return(failure());
    }

    // operator != ( CString&, CString& )

    string_2.Copy( TEXT( "abc" ) );

    if ( ! ( string_1 != string_2 ) )
    {
        test_number_that_failed = 65;
        return(failure());
    }

    // operator != ( CString&, const char * )

    if ( ! ( string_1 != TEXT( "abc" ) ) )
    {
        test_number_that_failed = 66;
        return(failure());
    }

    // operator != ( const char *, CString& )

    if ( ! ( TEXT( "abc" ) != string_1 ) )
    {
        test_number_that_failed = 67;
        return(failure());
    }

    // operator < ( CString&, CString& )

    string_1.Copy( TEXT( "ABC" ) );
    string_2.Copy( TEXT( "ABc" ) );

    if ( ! ( string_1 < string_2 ) )
    {
        test_number_that_failed = 68;
        return(failure());
    }

    // operator < ( CString&, const char * )

    if ( ! ( string_1 < TEXT( "ABc" ) ) )
    {
        test_number_that_failed = 69;
        return(failure());
    }

    // operator < ( const char *, CString& )

    if ( ! ( TEXT( "AAB" ) < string_1 ) )
    {
        test_number_that_failed = 70;
        return(failure());
    }

    // operator > ( CString&, CString& )

    string_1.Copy( TEXT( "ABc" ) );
    string_2.Copy( TEXT( "ABC" ) );

    if ( ! ( string_1 > string_2 ) )
    {
        test_number_that_failed = 71;
        return(failure());
    }

    // operator > ( CString&, const char * )

    if ( ! ( string_1 > TEXT( "ABC" ) ) )
    {
        test_number_that_failed = 72;
        return(failure());
    }

    // operator > ( const char *, CString& )

    string_1.Copy( TEXT( "ABC" ) );

    if ( ! ( TEXT( "ABc" ) > string_1 ) )
    {
        test_number_that_failed = 73;
        return(failure());
    }

    // operator <= ( CString&, CString& )

    // Test the "less than" part of "less than or equal to"

    string_1.Copy( TEXT( "ABC" ) );
    string_2.Copy( TEXT( "ABc" ) );

    if ( ! ( string_1 <= string_2 ) )
    {
        test_number_that_failed = 74;
        return(failure());
    }

    // operator <= ( CString&, const char * )

    if ( ! ( string_1 <= "ABc" ) )
    {
        test_number_that_failed = 75;
        return(failure());
    }

    // operator <= ( const char *, CString& )

    if ( ! ( "AAB" <= string_1 ) )
    {
        test_number_that_failed = 76;
        return(failure());
    }

    // Now test the "equal to" part of "less than or equal to"
    // operator <= ( CString&, CString& )

    string_1.Copy( "ABC" );
    string_2.Copy( "ABC" );

    if ( ! ( string_1 <= string_2 ) )
    {
        test_number_that_failed = 77;
        return(failure());
    }

    // operator <= ( CString&, const char * )

    if ( ! ( string_1 <= "ABC" ) )
    {
        test_number_that_failed = 78;
        return(failure());
    }

    // operator <= ( const char *, CString& )

    if ( ! ( "ABC" <= string_1 ) )
    {
        test_number_that_failed = 79;
        return(failure());
    }

    // Test the "greater than" part of "greater than or equal to"
    // operator >= ( CString&, CString& )

    string_1.Copy( "ABc" );
    string_2.Copy( "ABC" );

    if ( ! ( string_1 >= string_2 ) )
    {
        test_number_that_failed = 80;
        return(failure());
    }

    // operator >= ( CString&, const char * )

    if ( ! ( string_1 >= "ABC" ) )
    {
        test_number_that_failed = 81;
        return(failure());
    }

    // operator >= ( const char *, CString& )

    string_1.Copy( "ABC" );

    if ( ! ( "ABc" >= string_1 ) )
    {
        test_number_that_failed = 82;
        return(failure());
    }

    // Now test the "equal to" part of "greater than or equal to"
    // operator >= ( CString&, CString& )

    string_1.Copy( "ABC" );
    string_2.Copy( "ABC" );

    if ( ! ( string_1 >= string_2 ) )
    {
        test_number_that_failed = 83;
        return(failure());
    }

    // operator >= ( CString&, const char * )

    if ( ! ( string_1 >= "ABC" ) )
    {
        test_number_that_failed = 84;
        return(failure());
    }

    // operator >= ( const char *, CString& )

    if ( ! ( "ABC" >= string_1 ) )
    {
        test_number_that_failed = 85;
        return(failure());
    }

    string_1.Empty();

    if ( string_1.GetLength() != 0 )
    {
        test_number_that_failed = 86;
        return(failure());
    }

    long loop_index                   = 0;
    long number_of_characters_to_test = INTERNAL_STRING_BUFFER_SIZE * 2;

    SIZE_T previous_length = 0;

    while( loop_index < number_of_characters_to_test )
    {
        previous_length = string_1.GetLength();

#if defined( WFC_STL )

        string_1.Append( 'A' );

#else // WFC_STL

        string_1 += TEXT( 'A' );

#endif // WFC_STL

        if ( previous_length != (string_1.GetLength() - 1 ) )
        {
            printf( "previous length (%lu) failed, length in error is %lu\n",
                (unsigned long) previous_length,
                (unsigned long) string_1.GetLength() - 1 );

            test_number_that_failed = 87;
            return(failure());
        }

        loop_index++;
    }

    if ( string_1.GetLength() != number_of_characters_to_test )
    {
        printf( "Length is %lu while it should have been %lu\n",
            (unsigned long) string_1.GetLength(),
            (unsigned long) number_of_characters_to_test );

        test_number_that_failed = 88;
        return(failure());
    }

    string_1.Copy( "0123456789" );

    if ( string_1.GetLength() != 10 )
    {
        test_number_that_failed = 89;
        return(failure());
    }

    if ( string_1.Compare( TEXT( "0123456789" ) ) != 0 )
    {
        test_number_that_failed = 90;
        return(failure());
    }

#if defined( WFC_STL )

    string_1.SetLength( 9 );

    if ( string_1.GetLength() != 9 )
    {
        printf( "Length should have been 9 but it was %lu\n", (unsigned long) string_1.GetLength() );
        test_number_that_failed = 91;
        return(failure());
    }

#else

    string_1 = "012345678";

#endif // WFC_STL

    if ( string_1.Compare( TEXT( "012345678" ) ) != 0 )
    {
        _tprintf( TEXT( "String is \"%s\" when is should have been \"012345678\"\n" ), (LPCTSTR) string_1 );
        test_number_that_failed = 92;
        return(failure());
    }

    string_1.Empty();

    if ( string_1.GetLength() != 0 )
    {
        test_number_that_failed = 93;
        return(failure());
    }

    // CString::Format()

    string_1.Format( TEXT( "This is the %d" ), 1 );

    if ( string_1.Compare( TEXT( "This is the 1" ) ) != 0 )
    {
        test_number_that_failed = 94;
        return(failure());
    }

    string_1.Format( TEXT( "This is the %d, %lu, %02d \"%s\"" ), 1, (unsigned long) 2, 3, TEXT( "Sam Loves Laura" ) );

    if ( string_1.Compare( TEXT( "This is the 1, 2, 03 \"Sam Loves Laura\"" ) ) != 0 )
    {
        test_number_that_failed = 95;
        return(failure());
    }

    unsigned char bytes[ 5 ];

    bytes[ 0 ] = 'A';
    bytes[ 1 ] = 'B';
    bytes[ 2 ] = 'C';
    bytes[ 3 ] = 'D';
    bytes[ 4 ] = 0x00;

    // const CString& operator=( const unsigned char * source );

    string_1 = bytes;

    if ( string_1.Compare( TEXT( "ABCD" ) ) != 0 )
    {
        test_number_that_failed = 96;
        return(failure());
    }

    // CString( LPCWSTR lpsz );

    CString string_6( L"ABCD" );

    if ( string_6.Compare( TEXT( "ABCD" ) ) != 0 )
    {
        test_number_that_failed = 97;
        return(failure());
    }

    // CString( LPCWSTR lpsz, int length );

    CString string_7( L"ABCD", 2 );

    if ( string_7.Compare( TEXT( "AB" ) ) != 0 )
    {
        test_number_that_failed = 98;
        return(failure());
    }

    // const CString& operator=( LPCWSTR source );

    string_7 = L"WXYZ";

    if ( string_7.Compare( TEXT( "WXYZ" ) ) != 0 )
    {
        test_number_that_failed = 99;
        return(failure());
    }

    // 2000-08-05
    // Thanks go to Tony Lam (lamt@alumni.utexas.net) for finding a bug in STL_CString()
    // According to MSDN, CString should
    // Extracts the last (that is, rightmost) nCount characters from this CString
    // object and returns a copy of the extracted substring. If nCount exceeds the
    // string length, then the entire string is extracted. Right is similar to the
    // Basic RIGHT$ function (except that indexes are zero-based).

    string_6.Empty();

    string_6 = string_7.Right( string_7.GetLength() );

    if ( string_6.GetLength() != string_7.GetLength() )
    {
        test_number_that_failed = 100;
        return(failure());
    }

    if ( string_6.Compare( string_7 ) != 0 )
    {
        test_number_that_failed = 101;
        return(failure());
    }

    string_6.Empty();

    string_6 = string_7.Right( string_7.GetLength() + 99 );

    if ( string_6.GetLength() != string_7.GetLength() )
    {
        test_number_that_failed = 102;
        return(failure());
    }

    if ( string_6.Compare( string_7 ) != 0 )
    {
        test_number_that_failed = 103;
        return(failure());
    }

    string_6.Empty();

    string_6.Copy( string_7.Left( string_7.GetLength() ) );

    if ( string_6.GetLength() != string_7.GetLength() )
    {
        test_number_that_failed = 104;
        return(failure());
    }

    if ( string_6.Compare( string_7 ) != 0 )
    {
        test_number_that_failed = 105;
        return(failure());
    }

    string_6.Empty();

    string_6.Copy( string_7.Left( string_7.GetLength() + 99 ) );

    if ( string_6.GetLength() != string_7.GetLength() )
    {
        test_number_that_failed = 106;
        return(failure());
    }

    if ( string_6.Compare( string_7 ) != 0 )
    {
        test_number_that_failed = 107;
        return(failure());
    }

    string_6.Empty();

    string_6.Copy( TEXT( "     " ) ); // 5 spaces

    string_6.TrimRight();

    if ( string_6.GetLength() != 0 )
    {
        test_number_that_failed = 108;
        return(failure());
    }

    string_6.Copy( TEXT( "     " ) ); // 5 spaces

    string_6.TrimLeft();

    if ( string_6.GetLength() != 0 )
    {
        test_number_that_failed = 109;
        return(failure());
    }

    string_6.Copy( TEXT( " \t\r\n" ) ); // all space-like characters

    string_6.TrimLeft();

    if ( string_6.GetLength() != 0 )
    {
        test_number_that_failed = 110;
        return(failure());
    }

    string_6.Copy( TEXT( " \t\r\n" ) ); // all space-like characters

    string_6.TrimRight();

    if ( string_6.GetLength() != 0 )
    {
        test_number_that_failed = 111;
        return(failure());
    }

    string_6.TrimRight(); // Shouldn't blow up, test 112
    string_6.TrimLeft(); // Shouldn't blow up, test 113

    string_6.Copy( "111" );

    string_6.TrimRight( '1' );

    if ( string_6.GetLength() != 0 )
    {
        test_number_that_failed = 114;
        return(failure());
    }

    string_6.Copy( "111" );

    string_6.TrimLeft( '1' );

    if ( string_6.GetLength() != 0 )
    {
        WFCTRACEINIT( TEXT( "test_CString()" ) );
        WFCTRACEVAL( TEXT( "string_6.GetLength() is " ), string_6.GetLength() );
        WFCTRACEVAL( TEXT( "string_6 is " ), string_6 );
        test_number_that_failed = 115;
        return(failure());
    }

    string_5.Empty();

    string_6.Copy( TEXT( "0123456789" ) );

    string_5.Copy( string_6.Right( string_6.GetLength() + 1 ) );

    if ( string_5.Compare( TEXT( "0123456789" ) ) != 0 )
    {
        test_number_that_failed = 116;
        return(failure());
    }

    string_5 = string_6.Left( ( string_6.GetLength() * 2 ) + 1 );

    if ( string_5.Compare( TEXT( "0123456789" ) ) != 0 )
    {
        test_number_that_failed = 117;
        return(failure());
    }

    // Now test inserting, this function has lots of edge conditions...

    string_5.Copy( TEXT( "BEF" ) );

    if ( string_5.GetLength() != 3 )
    {
        test_number_that_failed = 118;
        return(failure());
    }

    // Try inserting before the first character in the string

    if ( string_5.Insert( -1, TEXT( 'A' ) ) != 4 )
    {
        test_number_that_failed = 119;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "ABEF" ) ) != 0 )
    {
        test_number_that_failed = 120;
        return( FALSE );
    }

    string_5.Copy( "ABEF" );

    // Now insert after the end of the string

    if ( string_5.Insert( 999, TEXT( 'G' ) ) != 5 )
    {
        test_number_that_failed = 121;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "ABEFG" ) ) != 0 )
    {
        test_number_that_failed = 122;
        return(failure());
    }

    if ( string_5.Insert( 2, TEXT( 'C' ) ) != 6 )
    {
        test_number_that_failed = 123;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "ABCEFG" ) ) != 0 )
    {
        test_number_that_failed = 124;
        return(failure());
    }

    if ( string_5.Insert( 0, TEXT( 'Z' ) ) != 7 )
    {
        test_number_that_failed = 125;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "ZABCEFG" ) ) != 0 )
    {
        test_number_that_failed = 126;
        return(failure());
    }

    if ( string_5.Insert( 1, TEXT( 'Y' ) ) != 8 )
    {
        test_number_that_failed = 127;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "ZYABCEFG" ) ) != 0 )
    {
        test_number_that_failed = 128;
        return(failure());
    }

    if ( string_5.Insert( 7, TEXT( 'H' ) ) != 9 )
    {
        test_number_that_failed = 129;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "ZYABCEFHG" ) ) != 0 )
    {
        test_number_that_failed = 130;
        return(failure());
    }

    if ( string_5.Insert( 9, TEXT( 'I' ) ) != 10 )
    {
        test_number_that_failed = 131;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "ZYABCEFHGI" ) ) != 0 )
    {
        test_number_that_failed = 132;
        return(failure());
    }

    // Now test inserting strings

    string_5.Copy( TEXT( "CF" ) );

    if ( string_5.GetLength() != 2 )
    {
        test_number_that_failed = 133;
        return(failure());
    }

    // Try inserting before the first character in the string

    if ( string_5.Insert( -1, TEXT( "AB" ) ) != 4 )
    {
        test_number_that_failed = 134;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "ABCF" ) ) != 0 )
    {
        test_number_that_failed = 135;
        return(failure());
    }

    string_5.Copy( "ABCF" );

    // Now insert after the end of the string

    if ( string_5.Insert( 999, TEXT( "GH" ) ) != 6 )
    {
        test_number_that_failed = 136;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "ABCFGH" ) ) != 0 )
    {
        test_number_that_failed = 137;
        return(failure());
    }

    if ( string_5.Insert( 3, TEXT( "DE" ) ) != 8 )
    {
        test_number_that_failed = 138;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "ABCDEFGH" ) ) != 0 )
    {
        test_number_that_failed = 139;
        return(failure());
    }

    if ( string_5.Insert( 0, TEXT( "YZ" ) ) != 10 )
    {
        test_number_that_failed = 140;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "YZABCDEFGH" ) ) != 0 )
    {
        test_number_that_failed = 141;
        return(failure());
    }

    if ( string_5.Insert( 1, TEXT( "WX" ) ) != 12 )
    {
        test_number_that_failed = 142;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "YWXZABCDEFGH" ) ) != 0 )
    {
        test_number_that_failed = 143;
        return(failure());
    }

    if ( string_5.Insert( 11, TEXT( "IJ" ) ) != 14 )
    {
        test_number_that_failed = 144;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "YWXZABCDEFGIJH" ) ) != 0 )
    {
        test_number_that_failed = 145;
        return(failure());
    }

    if ( string_5.Insert( 14, TEXT( "SR" ) ) != 16 )
    {
        test_number_that_failed = 146;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "YWXZABCDEFGIJHSR" ) ) != 0 )
    {
        test_number_that_failed = 147;
        return(failure());
    }

    // Remove()

    string_5.Copy( TEXT( "This is a test" ) );

    SIZE_T number_of_characters_removed = string_5.Remove( TEXT( 't' ) );

    if ( number_of_characters_removed != 2 )
    {
        test_number_that_failed = 148;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "This is a es" ) ) != 0 )
    {
        test_number_that_failed = 149;
        return(failure());
    }

    number_of_characters_removed = string_5.Remove( TEXT( 'T' ) );

    if ( number_of_characters_removed != 1 )
    {
        test_number_that_failed = 150;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "his is a es" ) ) != 0 )
    {
        test_number_that_failed = 151;
        return(failure());
    }

    string_5.Copy( TEXT( "ttt" ) );

    number_of_characters_removed = string_5.Remove( TEXT( 't' ) );

    if ( number_of_characters_removed != 3 )
    {
        test_number_that_failed = 152;
        return(failure());
    }

    if ( string_5.GetLength() !=  0 )
    {
        test_number_that_failed = 153;
        return(failure());
    }

    number_of_characters_removed = string_5.Remove( TEXT( 't' ) );

    if ( number_of_characters_removed != 0 )
    {
        test_number_that_failed = 154;
        return(failure());
    }

    string_5.Copy( TEXT( "ABCDEFGHIJ" ) );

    number_of_characters_removed = string_5.Remove( TEXT( 'K' ) );

    if ( number_of_characters_removed != 0 )
    {
        test_number_that_failed = 155;
        return(failure());
    }

    // Replace()

    string_5.Empty();

    if ( string_5.GetLength() !=  0 )
    {
        test_number_that_failed = 156;
        return(failure());
    }

    string_5.Copy( TEXT( "a b c d e f g a b c a" ) );

    // replace a character at the beginning, in the middle and at the end
    if ( string_5.Replace( 'a', 'z' ) != 3 )
    {
        test_number_that_failed = 157;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "z b c d e f g z b c z" ) ) != 0 )
    {
        test_number_that_failed = 158;
        return(failure());
    }

    string_5.Empty();

    if ( string_5.GetLength() !=  0 )
    {
        test_number_that_failed = 159;
        return(failure());
    }

    string_5.Copy( TEXT( "beginbeginbegin" ) );

    // replace a longer string with a shorter one
    if ( string_5.Replace( TEXT( "begin" ), TEXT( "end" ) ) != 3 )
    {
        test_number_that_failed = 160;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "endendend" ) ) != 0 )
    {
        test_number_that_failed = 161;
        return(failure());
    }

    string_5.Empty();

    if ( string_5.GetLength() !=  0 )
    {
        test_number_that_failed = 162;
        return(failure());
    }

    string_5.Copy( TEXT( "beginbeginbegin" ) );

    // replace a shorter string with a longer one
    if ( string_5.Replace( TEXT( "begin" ), TEXT( "theend" ) ) != 3 )
    {
        test_number_that_failed = 163;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "theendtheendtheend" ) ) != 0 )
    {
        test_number_that_failed = 164;
        return(failure());
    }

    string_5.Empty();

    if ( string_5.GetLength() !=  0 )
    {
        test_number_that_failed = 165;
        return(failure());
    }

    string_5.Copy( TEXT( "beginbeginbegin" ) );

    // replace a string with a same-sized one
    if ( string_5.Replace( TEXT( "begin" ), TEXT( "start" ) ) != 3 )
    {
        test_number_that_failed = 166;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "startstartstart" ) ) != 0 )
    {
        test_number_that_failed = 167;
        return(failure());
    }

    string_5.Empty();

    if ( string_5.GetLength() !=  0 )
    {
        test_number_that_failed = 168;
        return(failure());
    }

    // Now let's get a little tricky, replace one string with another that
    // produces the first string.

    string_5.Copy( TEXT( "begineginbeginegin" ) );

    if ( string_5.Replace( TEXT( "begin" ), TEXT( "b" ) ) != 2 )
    {
        test_number_that_failed = 169;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "beginbegin" ) ) != 0 )
    {
        test_number_that_failed = 170;
        return(failure());
    }

    test_number_that_failed = 170;

#if defined( _UNICODE )

    string_5.Empty();

    if ( string_5.GetLength() != 0 )
    {
        test_number_that_failed = 171;
        return(failure());
    }

    string_5.Copy( "ABCDEFGHIJK" );

    if ( string_5.GetLength() != 11 )
    {
        test_number_that_failed = 172;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "ABCDEFGHIJK" ) ) != 0 )
    {
        test_number_that_failed = 173;
        return(failure());
    }

    string_5.Empty();

    if ( string_5.GetLength() != 0 )
    {
        test_number_that_failed = 174;
        return(failure());
    }

    string_5.Copy( "ABCDEFGHIJKL", 11 );

    if ( string_5.GetLength() != 11 )
    {
        test_number_that_failed = 175;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "ABCDEFGHIJK" ) ) != 0 )
    {
        test_number_that_failed = 176;
        return(failure());
    }

    string_5.Empty();

    if ( string_5.GetLength() != 0 )
    {
        test_number_that_failed = 177;
        return(failure());
    }

    string_5.Copy( "ABCDEFGHIJKL", 11, 1 );

    if ( string_5.GetLength() != 11 )
    {
        test_number_that_failed = 178;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "BCDEFGHIJKL" ) ) != 0 )
    {
        test_number_that_failed = 179;
        return(failure());
    }

    string_5.Empty();

    if ( string_5.GetLength() != 0 )
    {
        test_number_that_failed = 180;
        return(failure());
    }

    string_5.Copy( "ABCDEFGHIJKL", 99 );

    if ( string_5.GetLength() != 12 )
    {
        _tprintf( TEXT( "CString failed test 181, length was %d when it should have been 12\n" ), (int) string_5.GetLength() );
        test_number_that_failed = 181;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "ABCDEFGHIJKL" ) ) != 0 )
    {
        test_number_that_failed = 182;
        return(failure());
    }

    string_5.Empty();

    if ( string_5.GetLength() != 0 )
    {
        test_number_that_failed = 183;
        return(failure());
    }

    string_5.Copy( "ABCDEFGHIJKL", 0, 99 );

    if ( string_5.GetLength() != 0 )
    {
        test_number_that_failed = 184;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "" ) ) != 0 )
    {
        test_number_that_failed = 185;
        return(failure());
    }

    string_5.Empty();

    if ( string_5.GetLength() != 0 )
    {
        test_number_that_failed = 186;
        return(failure());
    }

    string_5.Copy( "ABCDEFGHIJKL", 99, 99 );

    if ( string_5.GetLength() != 0 )
    {
        test_number_that_failed = 187;
        return(failure());
    }

    string_5.Empty();

    if ( string_5.GetLength() != 0 )
    {
        test_number_that_failed = 188;
        return(failure());
    }

    string_5.Copy( "ABCDEFGHIJKL", 1, 99 );

    if ( string_5.GetLength() != 0 )
    {
        test_number_that_failed = 189;
        return(failure());
    }

    if ( string_5.Compare( TEXT( "" ) ) != 0 )
    {
        test_number_that_failed = 190;
        return(failure());
    }

    test_number_that_failed = 190;

#endif // _UNICODE

#if defined( WFC_STL ) || (_MSV_VER >= 1300 )

    LPVOID environment = GetEnvironmentStrings();

    SIZE_T string_length = 0;

    std::vector<std::wstring> names;
    std::vector<std::wstring> values;

    LPTSTR current_string = (LPTSTR) environment;

    SSIZE_T location_of_equals = 0;

    CString name;
    CString value;

    string_length = _tcslen( current_string );

    while( string_length > 0 )
    {
        string_5.Copy( current_string );

        location_of_equals = string_5.Find( TEXT( '=' ) );
        _ASSERTE( location_of_equals != (-1) );

        if ( location_of_equals > 0 )
        {
            name.Copy( string_5.Left( location_of_equals ) );
            value.Copy( string_5.Right( string_5.GetLength() - ( location_of_equals + 1 ) ) );

            (void) names.Add( name );
            (void) values.Add( value );
        }

        current_string = &current_string[ string_length + 1 ];
        string_length = _tcslen( current_string );
    }

    FreeEnvironmentStrings( (LPTSTR) environment );

    string_length = 0;

#if 0

    while( string_length <  (int) names.GetSize() )
    {
        string_5.Empty();

        if ( string_5.GetEnvironmentVariable( names.GetAt( string_length ) ) == FALSE )
        {
            test_number_that_failed = 191;
            return(failure());
        }

        if ( string_5.Compare( values.GetAt( string_length ) ) != 0 )
        {
            test_number_that_failed = 192;
            return(failure());
        }

        string_length++;
    }
#endif

    string_5.Copy( TEXT( "WFC" ) );

    if ( string_5.GetEnvironmentVariable( TEXT( "WFCISCOOL1234567890" ) ) != FALSE )
    {
        test_number_that_failed = 193;
        return(failure());
    }

    if ( string_5.GetLength() != 0 )
    {
        test_number_that_failed = 194;
        return(failure());
    }

    test_number_that_failed = 194;

#endif

    return( true );
}
