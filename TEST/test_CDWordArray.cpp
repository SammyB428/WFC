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

__checkReturn bool test_CDWordArray( __out std::string& class_name, __out int& test_number_that_failed ) noexcept
{
    class_name.assign( "CDWordArray" );

    CDWordArray array_1;

    //   int         GetSize( void ) const;

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 1;
        return(failure());
    }

    //   int         GetUpperBound( void ) const;

    if ( array_1.GetUpperBound() != (-1) )
    {
        test_number_that_failed = 2;
        return(failure());
    }

    //   int         Add( ARRAY_TYPE new_element );

    if ( array_1.Add( 47 ) != 0 )
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

    if ( array_1.Add( 50 ) != 1 )
    {
        test_number_that_failed = 5;
        return(failure());
    }

    if ( array_1.GetSize() != 2 )
    {
        test_number_that_failed = 6;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 1 )
    {
        test_number_that_failed = 7;
        return(failure());
    }

    if ( array_1.Add( 57 ) != 2 )
    {
        test_number_that_failed = 8;
        return(failure());
    }

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 9;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 10;
        return(failure());
    }

    //   ARRAY_TYPE  GetAt( int array_index ) const;
    if ( array_1.GetAt( 0 ) != 47 )
    {
        test_number_that_failed = 11;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 50 )
    {
        test_number_that_failed = 12;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 57 )
    {
        test_number_that_failed = 13;
        return(failure());
    }

    //   ARRAY_TYPE& ElementAt( int array_index );

    // Element in the middle test
    array_1.ElementAt( 1 ) = 80;

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 14;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 15;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 47 )
    {
        test_number_that_failed = 16;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 80 )
    {
        test_number_that_failed = 17;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 57 )
    {
        test_number_that_failed = 18;
        return(failure());
    }

    // Element at the beginning test

    array_1.ElementAt( 0 ) = 90;

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 19;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 20;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 90 )
    {
        test_number_that_failed = 21;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 80 )
    {
        test_number_that_failed = 22;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 57 )
    {
        test_number_that_failed = 23;
        return(failure());
    }

    // Element at the end test

    array_1.ElementAt( 2 ) = 100;

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 24;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 25;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 90 )
    {
        test_number_that_failed = 26;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 80 )
    {
        test_number_that_failed = 27;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 100 )
    {
        test_number_that_failed = 28;
        return(failure());
    }

    //   ARRAY_TYPE& operator[]( int array_index );

    array_1[ 0 ] = 1;

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 29;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 30;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 31;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 80 )
    {
        test_number_that_failed = 32;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 100 )
    {
        test_number_that_failed = 33;
        return(failure());
    }

    array_1[ 1 ] = 2;

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 34;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 35;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 36;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 2 )
    {
        test_number_that_failed = 37;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 100 )
    {
        test_number_that_failed = 38;
        return(failure());
    }

    array_1[ 2 ] = 3;

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 39;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 40;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 41;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 2 )
    {
        test_number_that_failed = 42;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 3 )
    {
        test_number_that_failed = 43;
        return(failure());
    }

    //   ARRAY_TYPE operator[]( int array_index ) const;

    if ( array_1[ 0 ] != 1 )
    {
        test_number_that_failed = 44;
        return(failure());
    }

    if ( array_1[ 1 ] != 2 )
    {
        test_number_that_failed = 45;
        return(failure());
    }

    if ( array_1[ 2 ] != 3 )
    {
        test_number_that_failed = 46;
        return(failure());
    }

    //   void        SetAt( int array_index, ARRAY_TYPE new_element );

    array_1.SetAt( 0, 11 );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 47;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 48;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 49;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 2 )
    {
        test_number_that_failed = 50;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 3 )
    {
        test_number_that_failed = 51;
        return(failure());
    }

    array_1.SetAt( 1, 22 );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 52;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 53;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 54;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 22 )
    {
        test_number_that_failed = 55;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 3 )
    {
        test_number_that_failed = 56;
        return(failure());
    }

    array_1.SetAt( 2, 33 );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 57;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 58;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 59;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 22 )
    {
        test_number_that_failed = 60;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 33 )
    {
        test_number_that_failed = 61;
        return(failure());
    }

    //   void        RemoveAt( int array_index, int number_of_elements_to_remove = 1 );

    (void) array_1.Add( 44 );

    // Remove at the end
    array_1.RemoveAt( array_1.GetUpperBound() );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 62;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 63;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 64;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 22 )
    {
        test_number_that_failed = 65;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 33 )
    {
        test_number_that_failed = 66;
        return(failure());
    }

    (void) array_1.Add( 44 );

    if ( array_1.GetAt( array_1.GetUpperBound() ) != 44 )
    {
        test_number_that_failed = 66;
        return(failure());
    }

    // Remove at the beginning

    array_1.RemoveAt( 0 );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 67;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 68;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 22 )
    {
        test_number_that_failed = 69;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 33 )
    {
        test_number_that_failed = 70;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 44 )
    {
        test_number_that_failed = 71;
        return(failure());
    }

    // Remove in the middle

    array_1.RemoveAt( 1 );

    if ( array_1.GetSize() != 2 )
    {
        test_number_that_failed = 72;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 1 )
    {
        test_number_that_failed = 73;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 22 )
    {
        test_number_that_failed = 74;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 44 )
    {
        test_number_that_failed = 75;
        return(failure());
    }

    // Remove Multiple

    array_1.RemoveAt( 0, 2 );

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 76;
        return(failure());
    }

    if ( array_1.GetUpperBound() != (-1) )
    {
        test_number_that_failed = 77;
        return(failure());
    }

    (void) array_1.Add( 11 );
    (void) array_1.Add( 22 );
    (void) array_1.Add( 33 );
    (void) array_1.Add( 44 );

    array_1.RemoveAt( 1, 2 );

    if ( array_1.GetSize() != 2 )
    {
        test_number_that_failed = 78;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 1 )
    {
        test_number_that_failed = 79;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 80;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 44 )
    {
        test_number_that_failed = 81;
        return(failure());
    }

    (void) array_1.Add( 55 );

    // Remove Multiple at the end

    array_1.RemoveAt( 1, 2 );

    if ( array_1.GetSize() != 1 )
    {
        test_number_that_failed = 82;
        return(failure());
    }

    if ( array_1.GetUpperBound() != 0 )
    {
        test_number_that_failed = 83;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 84;
        return(failure());
    }

    //   void        RemoveAll( void );
    array_1.RemoveAll();

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 85;
        return(failure());
    }

    if ( array_1.GetUpperBound() != (-1) )
    {
        test_number_that_failed = 86;
        return(failure());
    }

    //   void        InsertAt( int array_index, ARRAY_TYPE new_element, int number_of_times_to_insert_it = 1 );

    array_1.InsertAt( 0, 11, 3 );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 87;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 88;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 11 )
    {
        test_number_that_failed = 89;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 11 )
    {
        test_number_that_failed = 90;
        return(failure());
    }

    array_1.InsertAt( array_1.GetSize(), 22, 2 );

    if ( array_1.GetSize() != 5 )
    {
        test_number_that_failed = 91;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 92;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 11 )
    {
        test_number_that_failed = 93;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 11 )
    {
        test_number_that_failed = 94;
        return(failure());
    }

    if ( array_1.GetAt( 3 ) != 22 )
    {
        test_number_that_failed = 95;
        return(failure());
    }

    if ( array_1.GetAt( 4 ) != 22 )
    {
        test_number_that_failed = 96;
        return(failure());
    }

    array_1.InsertAt( 3, 33, 2 );

    if ( array_1.GetSize() != 7 )
    {
        test_number_that_failed = 97;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 98;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 11 )
    {
        test_number_that_failed = 99;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 11 )
    {
        test_number_that_failed = 100;
        return(failure());
    }

    if ( array_1.GetAt( 3 ) != 33 )
    {
        test_number_that_failed = 101;
        return(failure());
    }

    if ( array_1.GetAt( 4 ) != 33 )
    {
        test_number_that_failed = 102;
        return(failure());
    }

    if ( array_1.GetAt( 5 ) != 22 )
    {
        test_number_that_failed = 103;
        return(failure());
    }

    if ( array_1.GetAt( 6 ) != 22 )
    {
        test_number_that_failed = 104;
        return(failure());
    }

    array_1.RemoveAll();

    (void) array_1.Add( 1 );
    (void) array_1.Add( 2 );
    (void) array_1.Add( 3 );

    CDWordArray array_2;

    array_2.Copy( array_1 );

    if ( array_2.GetSize() != 3 )
    {
        test_number_that_failed = 105;
        return(failure());
    }

    if ( array_2.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 106;
        return(failure());
    }

    if ( array_2.GetAt( 1 ) != 2 )
    {
        test_number_that_failed = 107;
        return(failure());
    }

    if ( array_2.GetAt( 2 ) != 3 )
    {
        test_number_that_failed = 108;
        return(failure());
    }

    array_2.RemoveAll();
    array_1.RemoveAll();

    (void) array_1.Add( 11 );
    (void) array_2.Add( 22 );

    array_1.Append( array_2 );

    if ( array_1.GetSize() != 2 )
    {
        test_number_that_failed = 109;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 110;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 22 )
    {
        test_number_that_failed = 111;
        return(failure());
    }

    //   void        SetSize( int new_size, int x = 32 );

    array_1.SetSize( 3, 99 );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 112;
        return(failure());
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 113;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 22 )
    {
        test_number_that_failed = 114;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 0 )
    {
        test_number_that_failed = 115;
        return(failure());
    }

    array_1.RemoveAll();
    array_1.SetSize( 5 );

    DWORD elements[ 5 ];

    elements[ 0 ] = 11;
    elements[ 1 ] = 22;
    elements[ 2 ] = 33;
    elements[ 3 ] = 44;
    elements[ 4 ] = 55;

    //   ARRAY_TYPE * GetData( void );
    CopyMemory( array_1.GetData(), elements, array_1.GetSize() * sizeof( DWORD ) );

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 116;
        return(failure());
    }

    if ( array_1.GetAt( 1 ) != 22 )
    {
        test_number_that_failed = 117;
        return(failure());
    }

    if ( array_1.GetAt( 2 ) != 33 )
    {
        test_number_that_failed = 118;
        return(failure());
    }

    if ( array_1.GetAt( 3 ) != 44 )
    {
        test_number_that_failed = 119;
        return(failure());
    }

    if ( array_1.GetAt( 4 ) != 55 )
    {
        test_number_that_failed = 120;
        return(failure());
    }

    //   const ARRAY_TYPE * GetData( void ) const;

    array_1.InsertAt( 0, (DWORD) 0 ); // Modify the buffer

    const DWORD * const_elements = array_1.GetData();

    if ( const_elements[ 0 ] != 0 )
    {
        test_number_that_failed = 121;
        return(failure());
    }

    if ( const_elements[ 1 ] != 11 )
    {
        test_number_that_failed = 122;
        return(failure());
    }

    if ( const_elements[ 2 ] != 22 )
    {
        test_number_that_failed = 123;
        return(failure());
    }

    if ( const_elements[ 3 ] != 33 )
    {
        test_number_that_failed = 124;
        return(failure());
    }

    if ( const_elements[ 4 ] != 44 )
    {
        test_number_that_failed = 125;
        return(failure());
    }

    if ( const_elements[ 5 ] != 55 )
    {
        test_number_that_failed = 126;
        return(failure());
    }

    // InsertAt( array )

    // We must test three scenarios, inserting at the beginning,
    // inserting at the end, and inserting in the middle

    // Insert in the middle

    array_1.RemoveAll();

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 127;
        return(failure());
    }

    array_2.RemoveAll();

    if ( array_2.GetSize() != 0 )
    {
        test_number_that_failed = 128;
        return(failure());
    }

    (void) array_1.Add( 1 );
    (void) array_1.Add( 4 );
    (void) array_1.Add( 5 );

    (void) array_2.Add( 2 );
    (void) array_2.Add( 3 );

    array_1.InsertAt( 1, &array_2 );

    if ( array_1.GetAt( 0 ) != 1 or
        array_1.GetAt( 1 ) != 2 or
        array_1.GetAt( 2 ) != 3 or
        array_1.GetAt( 3 ) != 4 or
        array_1.GetAt( 4 ) != 5 )
    {
        test_number_that_failed = 129;
        return(failure());
    }

    // Insert at the beginning

    array_1.RemoveAll();

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 130;
        return(failure());
    }

    array_2.RemoveAll();

    if ( array_2.GetSize() != 0 )
    {
        test_number_that_failed = 131;
        return(failure());
    }

    (void) array_1.Add( 3 );
    (void) array_1.Add( 4 );
    (void) array_1.Add( 5 );

    (void) array_2.Add( 1 );
    (void) array_2.Add( 2 );

    array_1.InsertAt( 0, &array_2 );

    if ( array_1.GetAt( 0 ) != 1 or
        array_1.GetAt( 1 ) != 2 or
        array_1.GetAt( 2 ) != 3 or
        array_1.GetAt( 3 ) != 4 or
        array_1.GetAt( 4 ) != 5 )
    {
        test_number_that_failed = 132;
        return(failure());
    }

    // Insert at the end

    array_1.RemoveAll();

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 133;
        return(failure());
    }

    array_2.RemoveAll();

    if ( array_2.GetSize() != 0 )
    {
        test_number_that_failed = 134;
        return(failure());
    }

    (void) array_1.Add( 1 );
    (void) array_1.Add( 2 );
    (void) array_1.Add( 3 );

    (void) array_2.Add( 4 );
    (void) array_2.Add( 5 );

    array_1.InsertAt( 3, &array_2 );

    if ( array_1.GetAt( 0 ) != 1 or
        array_1.GetAt( 1 ) != 2 or
        array_1.GetAt( 2 ) != 3 or
        array_1.GetAt( 3 ) != 4 or
        array_1.GetAt( 4 ) != 5 )
    {
        test_number_that_failed = 135;
        return(failure());
    }

    test_number_that_failed = 135;

    return( true );
}
