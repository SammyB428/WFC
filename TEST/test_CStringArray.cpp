/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2017, Samuel R. Blackburn
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

_Check_return_ bool test_CStringArray( _Out_ std::string& class_name, _Out_ int& test_number_that_failed ) noexcept
{
    class_name.assign( "CStringArray" );
    return(true);

    CStringArray array_1;

    //   int         GetSize( void ) const;

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 1;
        return( failure() );
    }

    //   int         GetUpperBound( void ) const;

    if ( array_1.GetUpperBound() != (-1) )
    {
        test_number_that_failed = 2;
        return(failure());
    }

    //   int         Add( ARRAY_TYPE new_element );

    if ( array_1.Add( TEXT( "Sammy" ) ) != 0 )
    {
        test_number_that_failed = 2;
        return(failure());
    }

    if ( array_1.GetSize() != 1 )
    {
        test_number_that_failed = 3;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 0 )
    {
        test_number_that_failed = 4;
        return(failure());
    }

    array_1[ 0 ].SetAt( 1, 'A' );

    if ( array_1[ 0 ].Compare( TEXT( "SAmmy" ) ) != 0 )
    {
        test_number_that_failed = 5;
        return(failure());
    }

    array_1.RemoveAll();

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 6;
        return(failure());
    }

    (void) array_1.Add( TEXT( "ZZZ" ) );
    (void) array_1.Add( TEXT( "YYY" ) );
    (void) array_1.Add( TEXT( "BBB" ) );
    (void) array_1.Add( TEXT( "AAA" ) );

    array_1.Sort();

    if ( array_1[ 0 ].Compare( TEXT( "AAA" ) ) != 0 )
    {
        test_number_that_failed = 7;
        return(failure());
    }

    if ( array_1[ 1 ].Compare( TEXT( "BBB" ) ) != 0 )
    {
        test_number_that_failed = 8;
        return(failure());
    }

    if ( array_1[ 2 ].Compare( TEXT( "YYY" ) ) != 0 )
    {
        test_number_that_failed = 9;
        return(failure());
    }

    if ( array_1[ 3 ].Compare( TEXT( "ZZZ" ) ) != 0 )
    {
        test_number_that_failed = 10;
        return(failure());
    }

    array_1.RemoveAll();

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 11;
        return(failure());
    }

    (void) array_1.Add( TEXT( "ZZZ" ) );
    (void) array_1.Add( TEXT( "yyy" ) );
    (void) array_1.Add( TEXT( "bbb" ) );
    (void) array_1.Add( TEXT( "AAA" ) );

    array_1.SortNoCase();

    if ( array_1[ 0 ].Compare( TEXT( "AAA" ) ) != 0 )
    {
        test_number_that_failed = 12;
        return(failure());
    }

    if ( array_1[ 1 ].Compare( TEXT( "bbb" ) ) != 0 )
    {
        test_number_that_failed = 13;
        return(failure());
    }

    if ( array_1[ 2 ].Compare( TEXT( "yyy" ) ) != 0 )
    {
        test_number_that_failed = 14;
        return(failure());
    }

    if ( array_1[ 3 ].Compare( TEXT( "ZZZ" ) ) != 0 )
    {
        test_number_that_failed = 15;
        return(failure());
    }

    std::vector<std::wstring> a1;

    // Insert into invalid area of an empty array
    insert_at(a1, 42, L"Sam", 2);

    if (a1.size() != 2)
    {
        test_number_that_failed = 16;
        return(failure());
    }

    if (a1.at(0).compare(L"Sam") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 17;
        return(failure());
    }

    if (a1.at(1).compare(L"Sam") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 18;
        return(failure());
    }

    // Insert into invalid area of an array with elemenets
    insert_at(a1, 42, L"Laura", 2);

    if (a1.size() != 4)
    {
        test_number_that_failed = 19;
        return(failure());
    }

    if (a1.at(0).compare(L"Sam") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 20;
        return(failure());
    }

    if (a1.at(1).compare(L"Sam") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 21;
        return(failure());
    }

    if (a1.at(2).compare(L"Laura") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 22;
        return(failure());
    }

    if (a1.at(3).compare(L"Laura") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 23;
        return(failure());
    }

    // Insert into the beginning of an array with elemenets
    insert_at(a1, 0, L"David", 2);

    if (a1.size() != 6)
    {
        test_number_that_failed = 24;
        return(failure());
    }

    if (a1.at(0).compare(L"David") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 25;
        return(failure());
    }

    if (a1.at(1).compare(L"David") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 26;
        return(failure());
    }

    if (a1.at(2).compare(L"Sam") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 27;
        return(failure());
    }

    if (a1.at(3).compare(L"Sam") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 28;
        return(failure());
    }

    if (a1.at(4).compare(L"Laura") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 29;
        return(failure());
    }

    if (a1.at(5).compare(L"Laura") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 30;
        return(failure());
    }

    // Finally, Insert into the middle of an array with elemenets
    insert_at(a1, 2, L"Audrey", 2);

    if (a1.size() != 8)
    {
        test_number_that_failed = 31;
        return(failure());
    }

    if (a1.at(0).compare(L"David") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 32;
        return(failure());
    }

    if (a1.at(1).compare(L"David") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 32;
        return(failure());
    }

    if (a1.at(2).compare(L"Audrey") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 33;
        return(failure());
    }

    if (a1.at(3).compare(L"Audrey") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 34;
        return(failure());
    }

    if (a1.at(4).compare(L"Sam") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 35;
        return(failure());
    }

    if (a1.at(5).compare(L"Sam") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 36;
        return(failure());
    }

    if (a1.at(6).compare(L"Laura") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 37;
        return(failure());
    }

    if (a1.at(7).compare(L"Laura") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 38;
        return(failure());
    }

    const WORD word_buffer[] = 
    {
        'S',  'a', 'm', 0x00,
        'L',  'a',  'u',  'r',  'a', 0x00,
        0x00
    };

    std::vector<std::wstring> words;

    words.push_back(L"word");

    wfc_convert_double_null_terminated_UNICODE_string(word_buffer, words);

    if (words.size() != 2)
    {
        test_number_that_failed = 39;
        return(failure());
    }

    if (words.at(0).compare(L"Sam") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 40;
        return(failure());
    }

    if (words.at(1).compare(L"Laura") != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 41;
        return(failure());
    }

    test_number_that_failed = 41;
    return( true );
}
