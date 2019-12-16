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

__checkReturn bool test_CByteArray( __out std::string& class_name, __out int& test_number_that_failed ) noexcept
{
    class_name.assign( "CByteArray" );

    CByteArray array_1;

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
        return( failure() );
    }

    //   int         Add( ARRAY_TYPE new_element );

    if ( array_1.Add( 47 ) != 0 )
    {
        test_number_that_failed = 2;
        return( failure() );
    }

    if ( array_1.GetSize() != 1 )
    {
        test_number_that_failed = 3;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 0 )
    {
        test_number_that_failed = 4;
        return( failure() );
    }

    if ( array_1.Add( 50 ) != 1 )
    {
        test_number_that_failed = 5;
        return( failure() );
    }

    if ( array_1.GetSize() != 2 )
    {
        test_number_that_failed = 6;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 1 )
    {
        test_number_that_failed = 7;
        return( failure() );
    }

    if ( array_1.Add( 57 ) != 2 )
    {
        test_number_that_failed = 8;
        return( failure() );
    }

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 9;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 10;
        return( failure() );
    }

    //   ARRAY_TYPE  GetAt( int array_index ) const;
    if ( array_1.GetAt( 0 ) != 47 )
    {
        test_number_that_failed = 11;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 50 )
    {
        test_number_that_failed = 12;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 57 )
    {
        test_number_that_failed = 13;
        return( failure() );
    }

    //   ARRAY_TYPE& ElementAt( int array_index );

    // Element in the middle test
    array_1.ElementAt( 1 ) = 80;

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 14;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 15;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 47 )
    {
        test_number_that_failed = 16;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 80 )
    {
        test_number_that_failed = 17;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 57 )
    {
        test_number_that_failed = 18;
        return( failure() );
    }

    // Element at the beginning test

    array_1.ElementAt( 0 ) = 90;

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 19;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 20;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 90 )
    {
        test_number_that_failed = 21;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 80 )
    {
        test_number_that_failed = 22;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 57 )
    {
        test_number_that_failed = 23;
        return( failure() );
    }

    // Element at the end test

    array_1.ElementAt( 2 ) = 100;

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 24;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 25;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 90 )
    {
        test_number_that_failed = 26;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 80 )
    {
        test_number_that_failed = 27;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 100 )
    {
        test_number_that_failed = 28;
        return( failure() );
    }

    //   ARRAY_TYPE& operator[]( int array_index );

    array_1[ 0 ] = 1;

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 29;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 30;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 31;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 80 )
    {
        test_number_that_failed = 32;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 100 )
    {
        test_number_that_failed = 33;
        return( failure() );
    }

    array_1[ 1 ] = 2;

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 34;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 35;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 36;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 2 )
    {
        test_number_that_failed = 37;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 100 )
    {
        test_number_that_failed = 38;
        return( failure() );
    }

    array_1[ 2 ] = 3;

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 39;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 40;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 41;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 2 )
    {
        test_number_that_failed = 42;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 3 )
    {
        test_number_that_failed = 43;
        return( failure() );
    }

    //   ARRAY_TYPE operator[]( int array_index ) const;

    if ( array_1[ 0 ] != 1 )
    {
        test_number_that_failed = 44;
        return( failure() );
    }

    if ( array_1[ 1 ] != 2 )
    {
        test_number_that_failed = 45;
        return( failure() );
    }

    if ( array_1[ 2 ] != 3 )
    {
        test_number_that_failed = 46;
        return( failure() );
    }

    //   void        SetAt( int array_index, ARRAY_TYPE new_element );

    array_1.SetAt( 0, 11 );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 47;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 48;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 49;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 2 )
    {
        test_number_that_failed = 50;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 3 )
    {
        test_number_that_failed = 51;
        return( failure() );
    }

    array_1.SetAt( 1, 22 );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 52;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 53;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 54;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 22 )
    {
        test_number_that_failed = 55;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 3 )
    {
        test_number_that_failed = 56;
        return( failure() );
    }

    array_1.SetAt( 2, 33 );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 57;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 58;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 59;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 22 )
    {
        test_number_that_failed = 60;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 33 )
    {
        test_number_that_failed = 61;
        return( failure() );
    }

    //   void        RemoveAt( int array_index, int number_of_elements_to_remove = 1 );

    (void) array_1.Add( 44 );

    // Remove at the end
    array_1.RemoveAt( array_1.GetUpperBound() );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 62;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 63;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 64;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 22 )
    {
        test_number_that_failed = 65;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 33 )
    {
        test_number_that_failed = 66;
        return( failure() );
    }

    (void) array_1.Add( 44 );

    if ( array_1.GetAt( array_1.GetUpperBound() ) != 44 )
    {
        test_number_that_failed = 66;
        return( failure() );
    }

    // Remove at the beginning

    array_1.RemoveAt( 0 );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 67;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 2 )
    {
        test_number_that_failed = 68;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 22 )
    {
        test_number_that_failed = 69;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 33 )
    {
        test_number_that_failed = 70;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 44 )
    {
        test_number_that_failed = 71;
        return( failure() );
    }

    // Remove in the middle

    array_1.RemoveAt( 1 );

    if ( array_1.GetSize() != 2 )
    {
        test_number_that_failed = 72;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 1 )
    {
        test_number_that_failed = 73;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 22 )
    {
        test_number_that_failed = 74;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 44 )
    {
        test_number_that_failed = 75;
        return( failure() );
    }

    // Remove Multiple

    array_1.RemoveAt( 0, 2 );

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 76;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != (-1) )
    {
        test_number_that_failed = 77;
        return( failure() );
    }

    (void) array_1.Add( 11 );
    (void) array_1.Add( 22 );
    (void) array_1.Add( 33 );
    (void) array_1.Add( 44 );

    array_1.RemoveAt( 1, 2 );

    if ( array_1.GetSize() != 2 )
    {
        test_number_that_failed = 78;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 1 )
    {
        test_number_that_failed = 79;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 80;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 44 )
    {
        test_number_that_failed = 81;
        return( failure() );
    }

    (void) array_1.Add( 55 );

    // Remove Multiple at the end

    array_1.RemoveAt( 1, 2 );

    if ( array_1.GetSize() != 1 )
    {
        test_number_that_failed = 82;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != 0 )
    {
        test_number_that_failed = 83;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 84;
        return( failure() );
    }

    //   void        RemoveAll( void );
    array_1.RemoveAll();

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 85;
        return( failure() );
    }

    if ( array_1.GetUpperBound() != (-1) )
    {
        test_number_that_failed = 86;
        return( failure() );
    }

    //   void        InsertAt( int array_index, ARRAY_TYPE new_element, int number_of_times_to_insert_it = 1 );

    array_1.InsertAt( 0, 11, 3 );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 87;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 88;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 11 )
    {
        test_number_that_failed = 89;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 11 )
    {
        test_number_that_failed = 90;
        return( failure() );
    }

    array_1.InsertAt( array_1.GetSize(), 22, 2 );

    if ( array_1.GetSize() != 5 )
    {
        test_number_that_failed = 91;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 92;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 11 )
    {
        test_number_that_failed = 93;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 11 )
    {
        test_number_that_failed = 94;
        return( failure() );
    }

    if ( array_1.GetAt( 3 ) != 22 )
    {
        test_number_that_failed = 95;
        return( failure() );
    }

    if ( array_1.GetAt( 4 ) != 22 )
    {
        test_number_that_failed = 96;
        return( failure() );
    }

    array_1.InsertAt( 3, 33, 2 );

    if ( array_1.GetSize() != 7 )
    {
        test_number_that_failed = 97;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 98;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 11 )
    {
        test_number_that_failed = 99;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 11 )
    {
        test_number_that_failed = 100;
        return( failure() );
    }

    if ( array_1.GetAt( 3 ) != 33 )
    {
        test_number_that_failed = 101;
        return( failure() );
    }

    if ( array_1.GetAt( 4 ) != 33 )
    {
        test_number_that_failed = 102;
        return( failure() );
    }

    if ( array_1.GetAt( 5 ) != 22 )
    {
        test_number_that_failed = 103;
        return( failure() );
    }

    if ( array_1.GetAt( 6 ) != 22 )
    {
        test_number_that_failed = 104;
        return( failure() );
    }

    array_1.RemoveAll();

    //   void        Append( const BYTE * byte_buffer, int number_of_bytes );

    BYTE bytes[ 5 ];

    bytes[ 0 ] = 1;
    bytes[ 1 ] = 2;
    bytes[ 2 ] = 3;
    bytes[ 3 ] = 4;
    bytes[ 4 ] = 5;

#if defined( WFC_STL )

    array_1.Append( bytes, 5 );

    if ( array_1.GetSize() != 5 )
    {
        test_number_that_failed = 105;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 106;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 2 )
    {
        test_number_that_failed = 107;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 3 )
    {
        test_number_that_failed = 108;
        return( failure() );
    }

    if ( array_1.GetAt( 3 ) != 4 )
    {
        test_number_that_failed = 109;
        return( failure() );
    }

    if ( array_1.GetAt( 4 ) != 5 )
    {
        test_number_that_failed = 110;
        return( failure() );
    }
#else // WC_STL

    array_1.Add( 1 );
    array_1.Add( 2 );
    array_1.Add( 3 );
    array_1.Add( 4 );
    array_1.Add( 5 );

#endif // WFC_STL

    bytes[ 0 ] = 11;
    bytes[ 1 ] = 22;
    bytes[ 2 ] = 33;
    bytes[ 3 ] = 44;
    bytes[ 4 ] = 55;

#if defined( WFC_STL )

    array_1.Append( bytes, 5 );

    if ( array_1.GetSize() != 10 )
    {
        test_number_that_failed = 111;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 112;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 2 )
    {
        test_number_that_failed = 113;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 3 )
    {
        test_number_that_failed = 114;
        return( failure() );
    }

    if ( array_1.GetAt( 3 ) != 4 )
    {
        test_number_that_failed = 115;
        return( failure() );
    }

    if ( array_1.GetAt( 4 ) != 5 )
    {
        test_number_that_failed = 116;
        return( failure() );
    }

    if ( array_1.GetAt( 5 ) != 11 )
    {
        test_number_that_failed = 117;
        return( failure() );
    }

    if ( array_1.GetAt( 6 ) != 22 )
    {
        test_number_that_failed = 118;
        return( failure() );
    }

    if ( array_1.GetAt( 7 ) != 33 )
    {
        test_number_that_failed = 119;
        return( failure() );
    }

    if ( array_1.GetAt( 8 ) != 44 )
    {
        test_number_that_failed = 120;
        return( failure() );
    }

    if ( array_1.GetAt( 9 ) != 55 )
    {
        test_number_that_failed = 121;
        return( failure() );
    }
#else // WFC_STL

    array_1.Add( 11 );
    array_1.Add( 22 );
    array_1.Add( 33 );
    array_1.Add( 44 );
    array_1.Add( 55 );

#endif // WFC_STL

    //   void        Copy( const CByteArray& source );

    CByteArray array_2;

    array_2.Copy( array_1 );

    if ( array_2.GetSize() != 10 )
    {
        test_number_that_failed = 122;
        return( failure() );
    }

    if ( array_2.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 123;
        return( failure() );
    }

    if ( array_2.GetAt( 1 ) != 2 )
    {
        test_number_that_failed = 124;
        return( failure() );
    }

    if ( array_2.GetAt( 2 ) != 3 )
    {
        test_number_that_failed = 125;
        return( failure() );
    }

    if ( array_2.GetAt( 3 ) != 4 )
    {
        test_number_that_failed = 126;
        return( failure() );
    }

    if ( array_2.GetAt( 4 ) != 5 )
    {
        test_number_that_failed = 127;
        return( failure() );
    }

    if ( array_2.GetAt( 5 ) != 11 )
    {
        test_number_that_failed = 128;
        return( failure() );
    }

    if ( array_2.GetAt( 6 ) != 22 )
    {
        test_number_that_failed = 129;
        return( failure() );
    }

    if ( array_2.GetAt( 7 ) != 33 )
    {
        test_number_that_failed = 130;
        return( failure() );
    }

    if ( array_2.GetAt( 8 ) != 44 )
    {
        test_number_that_failed = 131;
        return( failure() );
    }

    if ( array_2.GetAt( 9 ) != 55 )
    {
        test_number_that_failed = 132;
        return( failure() );
    }

    //   void        Append( const CByteArray& source );

    array_2.RemoveAll();
    array_1.RemoveAll();

    (void) array_1.Add( 11 );
    (void) array_2.Add( 22 );

    array_1.Append( array_2 );

    if ( array_1.GetSize() != 2 )
    {
        test_number_that_failed = 133;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 134;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 22 )
    {
        test_number_that_failed = 135;
        return( failure() );
    }

    //   void        SetSize( int new_size, int x = 0 );

    array_1.SetSize( 3, 0x20 );

    if ( array_1.GetSize() != 3 )
    {
        test_number_that_failed = 136;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 137;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 22 )
    {
        test_number_that_failed = 138;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 0x20 )
    {
        test_number_that_failed = 139;
        return( failure() );
    }

    array_1.RemoveAll();
    array_1.SetSize( 5 );

    //   ARRAY_TYPE * GetData( void );
    CopyMemory( array_1.GetData(), bytes, array_1.GetSize() );

    if ( array_1.GetAt( 0 ) != 11 )
    {
        test_number_that_failed = 140;
        return( failure() );
    }

    if ( array_1.GetAt( 1 ) != 22 )
    {
        test_number_that_failed = 141;
        return( failure() );
    }

    if ( array_1.GetAt( 2 ) != 33 )
    {
        test_number_that_failed = 142;
        return( failure() );
    }

    if ( array_1.GetAt( 3 ) != 44 )
    {
        test_number_that_failed = 143;
        return( failure() );
    }

    if ( array_1.GetAt( 4 ) != 55 )
    {
        test_number_that_failed = 144;
        return( failure() );
    }

    //   const ARRAY_TYPE * GetData( void ) const;

    array_1.InsertAt( 0, (BYTE) 0 ); // Modify the buffer

    const BYTE * const_bytes = array_1.GetData();

    if ( const_bytes[ 0 ] != 0 )
    {
        test_number_that_failed = 145;
        return( failure() );
    }

    if ( const_bytes[ 1 ] != 11 )
    {
        test_number_that_failed = 146;
        return( failure() );
    }

    if ( const_bytes[ 2 ] != 22 )
    {
        test_number_that_failed = 147;
        return( failure() );
    }

    if ( const_bytes[ 3 ] != 33 )
    {
        test_number_that_failed = 148;
        return( failure() );
    }

    if ( const_bytes[ 4 ] != 44 )
    {
        test_number_that_failed = 149;
        return( failure() );
    }

    if ( const_bytes[ 5 ] != 55 )
    {
        test_number_that_failed = 150;
        return( failure() );
    }

    // InsertAt( array )

    // We must test three scenarios, inserting at the beginning,
    // inserting at the end, and inserting in the middle

    // Insert in the middle

    array_1.RemoveAll();

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 151;
        return( failure() );
    }

    array_2.RemoveAll();

    if ( array_2.GetSize() != 0 )
    {
        test_number_that_failed = 152;
        return( failure() );
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
        test_number_that_failed = 153;
        return( failure() );
    }

    // Insert at the beginning

    array_1.RemoveAll();

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 154;
        return( failure() );
    }

    array_2.RemoveAll();

    if ( array_2.GetSize() != 0 )
    {
        test_number_that_failed = 155;
        return( failure() );
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
        test_number_that_failed = 156;
        return( failure() );
    }

    // Insert at the end

    array_1.RemoveAll();

    if ( array_1.GetSize() != 0 )
    {
        test_number_that_failed = 157;
        return( failure() );
    }

    array_2.RemoveAll();

    if ( array_2.GetSize() != 0 )
    {
        test_number_that_failed = 158;
        return( failure() );
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
        WFCTRACEINIT( TEXT( "CB" ) );
        WFCTRACEVAL( TEXT( "array_1( 0 ) should be 1 when it is " ), array_1.GetAt( 0 ) );
        WFCTRACEVAL( TEXT( "array_1( 1 ) should be 2 when it is " ), array_1.GetAt( 1 ) );
        WFCTRACEVAL( TEXT( "array_1( 2 ) should be 3 when it is " ), array_1.GetAt( 2 ) );
        WFCTRACEVAL( TEXT( "array_1( 3 ) should be 4 when it is " ), array_1.GetAt( 3 ) );
        WFCTRACEVAL( TEXT( "array_1( 4 ) should be 5 when it is " ), array_1.GetAt( 4 ) );
        test_number_that_failed = 159;
        return( failure() );
    }

    // hex_val

    int hexadecimal_value = hex_val( '0' );

    if ( hexadecimal_value != 0 )
    {
        test_number_that_failed = 160;
        return( failure() );
    }

    hexadecimal_value = hex_val( '1' );

    if ( hexadecimal_value != 1 )
    {
        test_number_that_failed = 161;
        return( failure() );
    }

    hexadecimal_value = hex_val( '2' );

    if ( hexadecimal_value != 2 )
    {
        test_number_that_failed = 162;
        return( failure() );
    }

    hexadecimal_value = hex_val( '3' );

    if ( hexadecimal_value != 3 )
    {
        test_number_that_failed = 163;
        return( failure() );
    }

    hexadecimal_value = hex_val( '4' );

    if ( hexadecimal_value != 4 )
    {
        test_number_that_failed = 164;
        return( failure() );
    }

    hexadecimal_value = hex_val( '5' );

    if ( hexadecimal_value != 5 )
    {
        test_number_that_failed = 165;
        return( failure() );
    }

    hexadecimal_value = hex_val( '6' );

    if ( hexadecimal_value != 6 )
    {
        test_number_that_failed = 166;
        return( failure() );
    }

    hexadecimal_value = hex_val( '7' );

    if ( hexadecimal_value != 7 )
    {
        test_number_that_failed = 167;
        return( failure() );
    }

    hexadecimal_value = hex_val( '8' );

    if ( hexadecimal_value != 8 )
    {
        test_number_that_failed = 168;
        return( failure() );
    }

    hexadecimal_value = hex_val( '9' );

    if ( hexadecimal_value != 9 )
    {
        test_number_that_failed = 169;
        return( failure() );
    }

    hexadecimal_value = hex_val( 'a' );

    if ( hexadecimal_value != 10 )
    {
        test_number_that_failed = 170;
        return( failure() );
    }

    hexadecimal_value = hex_val( 'b' );

    if ( hexadecimal_value != 11 )
    {
        test_number_that_failed = 171;
        return( failure() );
    }

    hexadecimal_value = hex_val( 'c' );

    if ( hexadecimal_value != 12 )
    {
        test_number_that_failed = 172;
        return( failure() );
    }

    hexadecimal_value = hex_val( 'd' );

    if ( hexadecimal_value != 13 )
    {
        test_number_that_failed = 173;
        return( failure() );
    }

    hexadecimal_value = hex_val( 'e' );

    if ( hexadecimal_value != 14 )
    {
        test_number_that_failed = 174;
        return( failure() );
    }

    hexadecimal_value = hex_val( 'f' );

    if ( hexadecimal_value != 15 )
    {
        test_number_that_failed = 175;
        return( failure() );
    }

    hexadecimal_value = hex_val( 'A' );

    if ( hexadecimal_value != 10 )
    {
        test_number_that_failed = 176;
        return( failure() );
    }

    hexadecimal_value = hex_val( 'B' );

    if ( hexadecimal_value != 11 )
    {
        test_number_that_failed = 177;
        return( failure() );
    }

    hexadecimal_value = hex_val( 'C' );

    if ( hexadecimal_value != 12 )
    {
        test_number_that_failed = 178;
        return( failure() );
    }

    hexadecimal_value = hex_val( 'D' );

    if ( hexadecimal_value != 13 )
    {
        test_number_that_failed = 179;
        return( failure() );
    }

    hexadecimal_value = hex_val( 'E' );

    if ( hexadecimal_value != 14 )
    {
        test_number_that_failed = 180;
        return( failure() );
    }

    hexadecimal_value = hex_val( 'F' );

    if ( hexadecimal_value != 15 )
    {
        test_number_that_failed = 181;
        return( failure() );
    }

    array_1.FromHex( L"   a B c   D  112  2 " ); 

    if ( array_1.GetSize() != 4 )
    {
        test_number_that_failed = 182;
        return( failure() );
    }

    if ( array_1.GetAt( 0 ) != 0xAB or
        array_1.GetAt( 1 ) != 0xCD or
        array_1.GetAt( 2 ) != 0x11 or
        array_1.GetAt( 3 ) != 0x22 )
    {
        WFCTRACEINIT( TEXT( "CB" ) );
        WFCTRACEVAL( TEXT( "array_1( 0 ) should be 171 when it is " ), array_1.GetAt( 0 ) );
        WFCTRACEVAL( TEXT( "array_1( 1 ) should be 205 when it is " ), array_1.GetAt( 1 ) );
        WFCTRACEVAL( TEXT( "array_1( 2 ) should be 17 when it is " ), array_1.GetAt( 2 ) );
        WFCTRACEVAL( TEXT( "array_1( 3 ) should be 34 when it is " ), array_1.GetAt( 3 ) );
        test_number_that_failed = 183;
        return( failure() );
    }

    std::vector<uint8_t> bb;

    append(bb, "Sam");

    if (bb.size() != 3)
    {
        test_number_that_failed = 184;
        return(failure());
    }

    if (bb.at(0) != 'S' or bb.at(1) != 'a' or bb.at(2) != 'm')
    {
        test_number_that_failed = 185;
        return(failure());
    }

    append(bb, "my");

    if (bb.size() != 5)
    {
        test_number_that_failed = 186;
        return(failure());
    }

    if (bb.at(0) != 'S' or bb.at(1) != 'a' or bb.at(2) != 'm' or bb.at(3) != 'm' or bb.at(4) != 'y' )
    {
        test_number_that_failed = 187;
        return(failure());
    }

    test_number_that_failed = 187;

    return( true );
}
