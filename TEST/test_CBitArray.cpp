/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2018, Samuel R. Blackburn
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

__checkReturn bool test_CBitArray( __out std::string& class_name, __out int& test_number_that_failed ) noexcept
{
    class_name.assign( "CBitArray" );

    //   CBitArray();
    CBitArray bits;

    //   virtual void      SetSize( DWORD number_of_bits );
    bits.SetSize( 2 );

    //   virtual DWORD     GetSize( void ) const;
    if ( bits.GetSize() != 2 )
    {
        test_number_that_failed = 1;
        return( failure() );
    }

    // When the size of an array is set, the members are initialized to zero

    //   virtual DWORD     GetAt( DWORD index ) const;
    if ( bits.GetAt( 0 ) != 0 )
    {
        test_number_that_failed = 2;
        return( failure() );
    }

    if ( bits.GetAt( 1 ) != 0 )
    {
        test_number_that_failed = 3;
        return( failure() );
    }

    //   virtual void      SetAt( DWORD index, DWORD value );
    bits.SetAt( 0, 1 );
    bits.SetAt( 1, 0 );

    if ( bits.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 4;
        return( failure() );
    }

    if ( bits.GetAt( 1 ) != 0 )
    {
        test_number_that_failed = 5;
        return( failure() );
    }

    //   virtual void      Add( DWORD bit_to_add, DWORD count = 1 );
    bits.Add( 1, 34 );

    if ( bits.GetSize() != 36 )
    {
        test_number_that_failed = 6;
        return( failure() );
    }

    // The bit array should now be:
    // 10 1111111111 1111111111 1111111111 1111

    if ( bits.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 7;
        return( failure() );
    }

    if ( bits.GetAt( 1 ) != 0 )
    {
        test_number_that_failed = 8;
        return( failure() );
    }

    for ( auto const bit_index : Range( 36, 2 ) )
    {
        if ( bits.GetAt( bit_index ) != 1 )
        {
            test_number_that_failed = 9;
            return( failure() );
        }
    }

    //   virtual void      LeftTrim( DWORD number_of_bits );
    bits.LeftTrim( 4 );

    // The bit array should now be:
    // 11111111 11111111 11111111 11111111

    if ( bits.GetSize() != 32 )
    {
        test_number_that_failed = 10;
        return( failure() );
    }

    for ( auto const array_index : Range(32) )
    {
        if ( bits.GetAt(array_index) != 1 )
        {
            test_number_that_failed = 11;
            return( failure() );
        }
    }

    bits.SetSize( 10 );

    if ( bits.GetSize() != 10 )
    {
        test_number_that_failed = 12;
        return( failure() );
    }

    bits.SetSize( 32 );

    if ( bits.GetSize() != 32 )
    {
        test_number_that_failed = 13;
        return( failure() );
    }

    //   virtual void      SetAll( DWORD value );

    bits.SetAll( 1 );

    for ( auto const array_index : Range(32) )
    {
        if ( bits.GetAt(array_index) != 1 )
        {
            test_number_that_failed = 14;
            return( failure() );
        }
    }

    bits.SetAll( 0 );

    // Array should be zeroeized

    for ( auto const array_index : Range(32) )
    {
        if ( bits.GetAt(array_index) != 0 )
        {
            test_number_that_failed = 15;
            return( failure() );
        }
    }

    // Setup our bytes test, the array is 32 bits wide now, let's
    // setup the test so four bytes will come out with the values of:
    // Byte 0 - 130 (0x82 or 10000010)
    // Byte 1 -  65 (0x41 or 01000001)
    // Byte 2 - 125 (0x7D or 01111101)
    // Byte 3 - 190 (0xBE or 10111110)

    // Array Index: 01234567
    // Byte 1 bits: 10000010

    bits.SetAt( 0, 1 );
    bits.SetAt( 6, 1 );

    //                111111
    // Array Index: 89012345
    // Byte 2 bits: 01000001

    bits.SetAt(  9, 1 );
    bits.SetAt( 15, 1 );

    //              11112222
    // Array Index: 67890123
    // Byte 3 bits: 01111101

    bits.SetAt( 17, 1 );
    bits.SetAt( 18, 1 );
    bits.SetAt( 19, 1 );
    bits.SetAt( 20, 1 );
    bits.SetAt( 21, 1 );
    bits.SetAt( 23, 1 );

    //              22222233
    // Array Index: 45678901
    // Byte 4 bits: 10111110

    bits.SetAt( 24, 1 );
    bits.SetAt( 26, 1 );
    bits.SetAt( 27, 1 );
    bits.SetAt( 28, 1 );
    bits.SetAt( 29, 1 );
    bits.SetAt( 30, 1 );

    std::vector<uint8_t> bytes;

    bits.CopyTo( bytes );

    if ( bytes.size() != 4 )
    {
        test_number_that_failed = 16;
        return( failure() );
    }

    if ( bytes.at( 0 ) != 0x82 )
    {
        test_number_that_failed = 17;
        return( failure() );
    }

    if ( bytes.at( 1 ) != 0x41 )
    {
        test_number_that_failed = 17;
        return( failure() );
    }

    if ( bytes.at( 2 ) != 0x7D )
    {
        test_number_that_failed = 18;
        return( failure() );
    }

    if ( bytes.at( 3 ) != 0xBE )
    {
        test_number_that_failed = 19;
        return( failure() );
    }

    //   virtual void      Complement( void );
    bits.Complement();

    bits.CopyTo( bytes );

    if ( bytes.size() != 4 )
    {
        test_number_that_failed = 20;
        return( failure() );
    }

    if ( bytes.at( 0 ) != 0x7D )
    {
        test_number_that_failed = 21;
        return( failure() );
    }

    if ( bytes.at( 1 ) != 0xBE )
    {
        test_number_that_failed = 22;
        return( failure() );
    }

    if ( bytes.at( 2 ) != 0x82 )
    {
        test_number_that_failed = 23;
        return( failure() );
    }

    if ( bytes.at( 3 ) != 0x41 )
    {
        test_number_that_failed = 24;
        return( failure() );
    }

    bits.Complement();

    // We should be back where we started

    bits.CopyTo( bytes );

    if ( bytes.size() != 4 )
    {
        test_number_that_failed = 25;
        return( failure() );
    }

    if ( bytes.at( 0 ) != 0x82 )
    {
        test_number_that_failed = 26;
        return( failure() );
    }

    if ( bytes.at( 1 ) != 0x41 )
    {
        test_number_that_failed = 27;
        return( failure() );
    }

    if ( bytes.at( 2 ) != 0x7D )
    {
        test_number_that_failed = 28;
        return( failure() );
    }

    if ( bytes.at( 3 ) != 0xBE )
    {
        test_number_that_failed = 29;
        return( failure() );
    }

    //   virtual void      InsertAt( DWORD index, DWORD value );
    bits.InsertAt( 0, 1 ); // 33 bits now

    if ( bits.GetSize() != 33 )
    {
        test_number_that_failed = 30;
        return( failure() );
    }

    bits.CopyTo( bytes );

    if ( bytes.size() != 5 )
    {
        test_number_that_failed = 31;
        return( failure() );
    }

    ASSERT( bytes.at( 0 ) == 0xC1 );

    if ( bytes.at( 0 ) != 0xC1 )
    {
        test_number_that_failed = 32;
        return( failure() );
    }

    //   virtual void      RemoveAt( DWORD index );
    bits.RemoveAt( 1 );

    if ( bits.GetSize() != 32 )
    {
        test_number_that_failed = 33;
        return( failure() );
    }

    bits.CopyTo( bytes );

    if ( bytes.size() != 4 )
    {
        test_number_that_failed = 34;
        return( failure() );
    }

    if ( bytes.at( 0 ) != 0x82 )
    {
        test_number_that_failed = 35;
        return( failure() );
    }

    if ( bytes.at( 1 ) != 0x41 )
    {
        test_number_that_failed = 36;
        return( failure() );
    }

    if ( bytes.at( 2 ) != 0x7D )
    {
        test_number_that_failed = 37;
        return( failure() );
    }

    if ( bytes.at( 3 ) != 0xBE )
    {
        test_number_that_failed = 38;
        return( failure() );
    }

    //   virtual DWORD     GetValue( DWORD index, DWORD length ) const;
    if ( bits.GetValue( 0, 8 ) != 0x82 )
    {
        test_number_that_failed = 39;
        return( failure() );
    }

    if ( bits.GetValue( 8, 8 ) != 0x41 )
    {
        test_number_that_failed = 40;
        return( failure() );
    }

    if ( bits.GetValue( 16, 8 ) != 0x7D )
    {
        test_number_that_failed = 41;
        return( failure() );
    }

    if ( bits.GetValue( 24, 8 ) != 0xBE )
    {
        test_number_that_failed = 42;
        return( failure() );
    }

    bits.InsertAt( 0, 1 );

    if ( bits.GetSize() != 33 )
    {
        test_number_that_failed = 43;
        return( failure() );
    }

    if ( bits.GetValue( 0, 8 ) != 0xC1 )
    {
        test_number_that_failed = 44;
        return( failure() );
    }

    if ( bits.GetValue( 1, 8 ) != 0x82 )
    {
        test_number_that_failed = 45;
        return( failure() );
    }

    if ( bits.GetValue( 24, 8 ) != 0xDF )
    {
        test_number_that_failed = 46;
        return( failure() );
    }

    if ( bits.GetValue( 25, 8 ) != 0xBE )
    {
        test_number_that_failed = 47;
        return( failure() );
    }

    //   virtual void      RemoveAll( void );
    bits.RemoveAll();

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 48;
        return( failure() );
    }

    //   virtual void      AddValue( DWORD value_to_add, DWORD number_of_bits_in_value );
    DWORD added_value = 0x55;

    bits.AddValue( added_value, 8 );

    if ( bits.GetSize() != 8 )
    {
        test_number_that_failed = 49;
        return( failure() );
    }

    // I always like to use prime numbers for testing sizes. I don't know
    // why it just seems to work better for me. It avoids problems like
    // "the code works when the index is a multiple of 2". So, I like
    // primes. A list of primes can be found at
    // http://www.utm.edu/research/primes/index.html

    int number_of_bits_to_insert = 10007;

    for ( auto const array_index : Range(number_of_bits_to_insert) )
    {
        bits.InsertAt( 0, 1 );
    }

    if ( bits.GetSize() != (DWORD) ( number_of_bits_to_insert + 8 ) )
    {
        test_number_that_failed = 50;
        return( failure() );
    }

    DWORD value_to_check = bits.GetValue( bits.GetSize() - 8, 8 );

    if ( added_value != value_to_check )
    {
        test_number_that_failed = 51;
        return( failure() );
    }

    //   virtual void      AddByte( BYTE byte_to_add );

    bits.AddByte( 0xAA );

    if ( bits.GetSize() != (DWORD) ( number_of_bits_to_insert + 16 ) )
    {
        test_number_that_failed = 52;
        return( failure() );
    }

    value_to_check = bits.GetValue( bits.GetSize() - 8, 8 );

    if ( value_to_check != 0xAA )
    {
        test_number_that_failed = 53;
        return( failure() );
    }

    //   CBitArray( DWORD initial_size );

    CBitArray bits2( 999 );

    if ( bits2.GetSize() != 999 )
    {
        test_number_that_failed = 54;
        return( FALSE);
    }

    for ( auto const array_index : Range(999) )
    {
        if ( bits2.GetAt(array_index) != 0 )
        {
            test_number_that_failed = 55;
            return( failure() );
        }
    }

    //   CBitArray( const CBitArray&  source );

    CBitArray bits3( bits );

    if ( bits3.GetSize() != bits.GetSize() )
    {
        test_number_that_failed = 56;
        return( FALSE);
    }

    value_to_check = bits3.GetValue( bits3.GetSize() - 8, 8 );

    if ( value_to_check != 0xAA )
    {
        test_number_that_failed = 57;
        return( failure() );
    }

    bytes.clear();

    (void) bytes.push_back(   1 );
    (void) bytes.push_back(  64 );
    (void) bytes.push_back( 128 );
    (void) bytes.push_back( 255 );

    CBitArray bits4( bytes );

    if ( bits4.GetSize() != 32 )
    {
        test_number_that_failed = 58;
        return( failure() );
    }

    if ( bits4.GetValue( 0, 8 ) != bytes.at( 0 ) )
    {
        test_number_that_failed = 59;
        return( failure() );
    }

    if ( bits4.GetValue( 8, 8 ) != bytes.at( 1 ) )
    {
        test_number_that_failed = 60;
        return( failure() );
    }

    if ( bits4.GetValue( 16, 8 ) != bytes.at( 2 ) )
    {
        test_number_that_failed = 61;
        return( failure() );
    }

    if ( bits4.GetValue( 24, 8 ) != bytes.at( 3 ) )
    {
        test_number_that_failed = 62;
        return( failure() );
    }

    //   virtual void      Append( const CBitArray& source );

    auto const number_of_bits = bits.GetSize();

    bits.Append( bits4 );

    if ( bits.GetSize() != (number_of_bits + bits4.GetSize() ) )
    {
        test_number_that_failed = 63;
        return( failure() );
    }

    bits.RemoveAll();

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 64;
        return( failure() );
    }

    //   virtual void      Append( const BYTE * data, DWORD number_of_bytes );

    BYTE byte_buffer[ 5 ];

    byte_buffer[ 0 ] = 1;
    byte_buffer[ 1 ] = 2;
    byte_buffer[ 2 ] = 3;
    byte_buffer[ 3 ] = 255;
    byte_buffer[ 4 ] = 0;

    bits.Append( byte_buffer, 5 );

    if ( bits.GetSize() != 40 )
    {
        test_number_that_failed = 64;
        return( failure() );
    }

    if ( bits.GetValue( 0, 8 ) != 1 )
    {
        test_number_that_failed = 65;
        return( failure() );
    }

    if ( bits.GetValue( 8, 8 ) != 2 )
    {
        test_number_that_failed = 66;
        return( failure() );
    }

    if ( bits.GetValue( 16, 8 ) != 3 )
    {
        _tprintf( TEXT( "Value was %d when it should have been 3.\n" ), (int) bits.GetValue( 16, 8 ) );
        test_number_that_failed = 67;
        return( failure() );
    }

    if ( bits.GetValue( 24, 8 ) != 255 )
    {
        test_number_that_failed = 68;
        return( failure() );
    }

    if ( bits.GetValue( 32, 8 ) != 0 )
    {
        test_number_that_failed = 69;
        return( failure() );
    }

    //   virtual void      Copy( const CBitArray& source );

    bits2.RemoveAll();

    bits2.Copy( bits );

    if ( bits2.GetSize() != bits.GetSize() )
    {
        test_number_that_failed = 70;
        return( failure() );
    }

    for ( auto const array_index : Range(bits.GetSize()) )
    {
        if ( bits.GetAt(array_index) != bits2.GetAt(array_index) )
        {
            test_number_that_failed = 71;
            return( failure() );
        }
    }

    bytes.clear();

    (void) bytes.push_back(   1 );
    (void) bytes.push_back(  16 );
    (void) bytes.push_back(  63 );
    (void) bytes.push_back( 127 );
    (void) bytes.push_back( 255 );

    bits.Copy( bytes );

    if ( bits.GetSize() != (DWORD) ( bytes.size() * 8 ) )
    {
        test_number_that_failed = 72;
        return( failure() );
    }

    if ( bits.GetValue( 0, 8 ) != bytes.at( 0 ) )
    {
        test_number_that_failed = 73;
        return( failure() );
    }

    if ( bits.GetValue( 8, 8 ) != bytes.at( 1 ) )
    {
        test_number_that_failed = 74;
        return( failure() );
    }

    if ( bits.GetValue( 16, 8 ) != bytes.at( 2 ) )
    {
        test_number_that_failed = 75;
        return( failure() );
    }

    if ( bits.GetValue( 24, 8 ) != bytes.at( 3 ) )
    {
        test_number_that_failed = 76;
        return( failure() );
    }

    if ( bits.GetValue( 32, 8 ) != bytes.at( 4 ) )
    {
        test_number_that_failed = 77;
        return( failure() );
    }

    std::vector<uint32_t> dword_array;

    CRandomNumberGenerator2 random;

    int number_of_array_entries = ( random.GetInteger() % 100 ) + 25;

    for ( auto const array_index : Range(number_of_array_entries) )
    {
        (void) dword_array.push_back( random.GetInteger() );
    }

    bits.RemoveAll();

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 78;
        return( failure() );
    }

    bits.Copy( dword_array );

    if ( bits.GetSize() != (DWORD) ( dword_array.size() * 32 ) )
    {
        test_number_that_failed = 79;
        return( failure() );
    }

    std::vector<uint32_t> dword_array2;

    bits.CopyTo( dword_array2 );

    if ( dword_array2.size() != dword_array.size() )
    {
        test_number_that_failed = 80;
        return( failure() );
    }

    for( auto const array_index : Range(dword_array.size()) )
    {
        if ( dword_array.at(array_index) != dword_array2.at(array_index) )
        {
            test_number_that_failed = 81;
            return( failure() );
        }
    }

    dword_array2.clear();
    dword_array.clear();
    bits.RemoveAll();

    //   virtual BOOL      EnumerateOnes( DWORD& enumerator ) const;

    for( auto const array_index : Range(101) )
    {
        bits.AddByte( 0x55 ); // Bit pattern of 01010101
    }

    std::size_t enumerator = 0;

    if ( bits.EnumerateOnes( enumerator ) == false )
    {
        test_number_that_failed = 82;
        return( failure() );
    }

    //   virtual BOOL      GetNextOne( DWORD& enumerator ) const;

    while( bits.GetNextOne( enumerator ) != false )
    {
        if ( ( enumerator % 2 ) == 0 )
        {
            test_number_that_failed = 83;
            return( failure() );
        }
    }

    //   virtual BOOL      EnumerateZeroes( DWORD& enumerator ) const;

    if ( bits.EnumerateZeroes( enumerator ) == false )
    {
        test_number_that_failed = 84;
        return( failure() );
    }

    //   virtual BOOL      GetNextZero( DWORD& enumerator ) const;

    while( bits.GetNextZero( enumerator ) != false )
    {
        if ( ( enumerator % 2 ) == 1 )
        {
            test_number_that_failed = 85;
            return( failure() );
        }
    }

    //   virtual DWORD     GetNumberOfOnes( void ) const;

    if ( bits.GetNumberOfOnes() != ( bits.GetSize() / 2 ) )
    {
        test_number_that_failed = 86;
        return( failure() );
    }

    //   virtual DWORD     GetNumberOfZeroes( void ) const;

    if ( bits.GetNumberOfZeroes() != ( bits.GetSize() / 2 ) )
    {
        test_number_that_failed = 87;
        return( failure() );
    }

    //   virtual DWORD     GetUpperBound( void ) const;

    if ( bits.GetUpperBound() != ( bits.GetSize() - 1 ) )
    {
        test_number_that_failed = 88;
        return( failure() );
    }

    bits2.RemoveAll();

    //   virtual CBitArray Left( DWORD number_of_bits ) const;

    bits2.Copy( bits.Left( bits.GetSize() / 2 ) );

    if ( bits2.GetSize() != bits.GetSize() / 2 )
    {
        test_number_that_failed = 89;
        return( failure() );
    }

    // Size is 404 bits, that means there should be 202 1's and 202 0's

    for( auto const array_index : Range(50) )
    {
        if ( bits2.GetValue(array_index * 8, 8 ) != 0x55 )
        {
            test_number_that_failed = 90;
            return( failure() );
        }
    }

    if ( bits2.GetNumberOfOnes() != bits2.GetNumberOfZeroes() )
    {
        test_number_that_failed = 91;
        return( failure() );
    }

    bits2.RemoveAll();

    if ( bits2.GetSize() != 0 )
    {
        test_number_that_failed = 92;
        return( failure() );
    }

    //   virtual CBitArray Right( DWORD number_of_bits ) const;

    bits2.Copy( bits.Right( bits.GetSize() / 2 ) );

    if ( bits2.GetSize() != bits.GetSize() / 2 )
    {
        test_number_that_failed = 93;
        return( failure() );
    }

    // Size is 404 bits, that means there should be 202 1's and 202 0's

    for ( auto const array_index : Range(50) )
    {
        if ( bits2.GetValue(array_index * 8, 8 ) != 0x55 )
        {
            test_number_that_failed = 94;
            return( failure() );
        }
    }

    if ( bits2.GetNumberOfOnes() != bits2.GetNumberOfZeroes() )
    {
        test_number_that_failed = 95;
        return( failure() );
    }

    bits2.RemoveAll();

    if ( bits2.GetSize() != 0 )
    {
        test_number_that_failed = 96;
        return( failure() );
    }

    //   virtual BOOL      Find( const CBitArray& value, DWORD& found_at, DWORD starting_at = 0 ) const;

    for ( auto const array_index : Range(100) )
    {
        bits2.AddByte( 0xAA ); // Bit pattern of 10101010
    }

    std::size_t found_at = 0;

    if ( bits.Find( bits2, found_at ) != true )
    {
        test_number_that_failed = 97;
        return( failure() );
    }

    if ( found_at != 1 )
    {
        test_number_that_failed = 98;
        return( failure() );
    }

    std::size_t old_found_at = found_at;

    if ( bits.Find( bits2, found_at, found_at ) != true )
    {
        test_number_that_failed = 99;
        return( failure() );
    }

    if ( found_at != old_found_at )
    {
        test_number_that_failed = 100;
        return( failure() );
    }

    if ( bits.Find( bits2, found_at, found_at + 1 ) != true )
    {
        test_number_that_failed = 101;
        return( failure() );
    }

    if ( found_at != 3 )
    {
        test_number_that_failed = 102;
        return( failure() );
    }

    if ( bits.Find( bits2, found_at, found_at + 1 ) != true )
    {
        test_number_that_failed = 103;
        return( failure() );
    }

    if ( found_at != 5 )
    {
        test_number_that_failed = 104;
        return( failure() );
    }

    if ( bits.Find( bits2, found_at, found_at + 1 ) != true )
    {
        test_number_that_failed = 105;
        return( failure() );
    }

    if ( found_at != 7 )
    {
        test_number_that_failed = 106;
        return( failure() );
    }

    if ( bits.Find( bits2, found_at, found_at + 1 ) != false )
    {
        test_number_that_failed = 107;
        return( failure() );
    }

    bits.RemoveAll();

    bits.Add( 1 );
    bits.AddValue( 0x12345678, 32 );

    //   virtual BOOL      Find( DWORD value, DWORD number_of_bits_in_value, DWORD& found_at_index, DWORD starting_at = 0 ) const;

    if ( bits.Find( 0x12345678, 32, found_at ) == false )
    {
        test_number_that_failed = 108;
        return( failure() );
    }

    if ( found_at != 1 )
    {
        test_number_that_failed = 109;
        return( failure() );
    }

    if ( bits.Find( 0x12345678, 32, found_at, found_at + 1 ) != false )
    {
        test_number_that_failed = 110;
        return( failure() );
    }

    bits.RemoveAll();

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 111;
        return( failure() );
    }

    bits.AddValue( 0x12345678, 32 );

    if ( bits.Find( 0x12345678, 32, found_at ) == false )
    {
        test_number_that_failed = 112;
        return( failure() );
    }

    if ( found_at != 0 )
    {
        test_number_that_failed = 113;
        return( failure() );
    }

    //   virtual void      FreeExtra( void );
    //   virtual CBitArray Mid( DWORD starting_at_index, DWORD number_of_bits ) const;

    bits2.RemoveAll();

    if ( bits2.GetSize() != 0 )
    {
        test_number_that_failed = 114;
        return( failure() );
    }

    bits.RemoveAll();

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 115;
        return( failure() );
    }

    bits.Add( 1 );
    bits.AddValue( 0x12345678, 32 );
    bits.Add( 0 );

    bits2.Copy( bits.Mid( 1, 32 ) );

    DWORD value = bits2.GetValue( 0, 32 );

    if ( value != 0x12345678 )
    {
        test_number_that_failed = 116;
        return( failure() );
    }

    //   virtual void      RightTrim( DWORD number_of_bits );

    bits.RightTrim( 9 );

    if ( bits.GetSize() != ( 34 - 9 ) )
    {
        test_number_that_failed = 117;
        return( failure() );
    }

    //   virtual void      SetValue( DWORD index, DWORD value, DWORD length );

    bits.RemoveAll();

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 118;
        return( failure() );
    }

    bits.Add( 1 );
    bits.AddValue( 0x12345678, 32 );
    bits.Add( 0 );

    if ( bits.GetValue( 1, 32 ) != 0x12345678 )
    {
        test_number_that_failed = 119;
        return( failure() );
    }

    bits.SetValue( 1, 0x87654321, 32 );

    if ( bits.GetValue( 1, 32 ) != 0x87654321 )
    {
        test_number_that_failed = 120;
        return( failure() );
    }

    bits.RemoveAll();
    bits2.RemoveAll();

    for ( auto const array_index : Range(51) )
    {
        bits.AddValue( random.GetInteger(), 32 );
    }

    //   virtual CBitArray& operator =  ( const CBitArray&  source );

    bits2 = bits;

    if ( bits2.GetSize() != bits.GetSize() )
    {
        test_number_that_failed = 121;
        return( failure() );
    }

    for ( auto const array_index : Range(bits.GetSize()) )
    {
        if ( bits.GetAt(array_index) != bits2.GetAt(array_index) )
        {
            test_number_that_failed = 122;
            return( failure() );
        }
    }

    //   virtual DWORD      operator [] ( int index ) const;

    for ( auto const array_index : Range(bits.GetSize()) )
    {
        if ( bits[array_index] != bits2[array_index] )
        {
            test_number_that_failed = 123;
            return( failure() );
        }
    }

    bits.SetSize( 0 );

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 124;
        return( failure() );
    }

    bits.Add( 1 );

    if ( bits.GetSize() != 1 )
    {
        test_number_that_failed = 125;
        return( failure() );
    }

    if ( bits.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 126;
        return( failure() );
    }

    bits.SetValue( 0, 0, 0 );

    if ( bits.GetAt( 0 ) != 1 )
    {
        test_number_that_failed = 127;
        return( failure() );
    }

    bits.SetAt( 1, 0 );

    if ( bits.GetSize() != 1 )
    {
        test_number_that_failed = 128;
        return( failure() );
    }

    bits.RightTrim( 99 );

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 130;
        return( failure() );
    }

    bits.Add( 1 );
    bits.Add( 0 );

    bits2.Copy( bits.Right( 99 ) );

    if ( bits2.GetSize() != 2 )
    {
        test_number_that_failed = 131;
        return( failure() );
    }

    bits2.RemoveAt( 0 );

    if ( bits2.GetSize() != 1 )
    {
        test_number_that_failed = 132;
        return( failure() );
    }

    bits2.RemoveAt( 99 );

    if ( bits2.GetSize() != 1 )
    {
        test_number_that_failed = 133;
        return( failure() );
    }

    bits2.Copy( bits.Mid( 99, 99 ) );

    if ( bits2.GetSize() != 0 )
    {
        test_number_that_failed = 134;
        return( failure() );
    }

    bits2.Copy( bits.Mid( 0, 99 ) );

    if ( bits2.GetSize() != 0 )
    {
        test_number_that_failed = 135;
        return( failure() );
    }

    bits2.Copy( bits.Left( 99 ) );

    if ( bits2.GetSize() != 2 )
    {
        test_number_that_failed = 136;
        return( failure() );
    }

    bits.LeftTrim( 99 );

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 137;
        return( failure() );
    }

    bits.RemoveAll();

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 138;
        return( failure() );
    }

    bits.SetSize( 30 );

    if ( bits.GetSize() != 30 )
    {
        test_number_that_failed = 139;
        return( failure() );
    }

    bits.SetRange( 1, 28 );

    if ( bits.GetAt( 0 ) != 0 )
    {
        test_number_that_failed = 140;
        return( failure() );
    }

    if ( bits.GetAt( 29 ) != 0 )
    {
        test_number_that_failed = 141;
        return( failure() );
    }

    for ( auto const array_index : Range(29, 1) )
    {
        if ( bits.GetAt(array_index) != 1 )
        {
            test_number_that_failed = 142;
            return( failure() );
        }
    }

    bits.RemoveAll();

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 143;
        return( failure() );
    }

    bits.SetSize( 65 );

    if ( bits.GetSize() != 65 )
    {
        test_number_that_failed = 144;
        return( failure() );
    }

    bits.SetAt( 0, 0 );
    bits.SetAt( 64, 0 );

    bits.SetRange( 1, 63 );

    if ( bits.GetAt( 0 ) != 0 )
    {
        test_number_that_failed = 145;
        return( failure() );
    }

    if ( bits.GetAt( 64 ) != 0 )
    {
        test_number_that_failed = 146;
        return( failure() );
    }

    for ( auto const array_index : Range(64, 1) )
    {
        if ( bits.GetAt(array_index) != 1 )
        {
            test_number_that_failed = 147;
            return( failure() );
        }
    }

    bits.RemoveAll();

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 148;
        return( failure() );
    }

    bits.SetSize( 32 );

    if ( bits.GetSize() != 32 )
    {
        test_number_that_failed = 149;
        return( failure() );
    }

    bits.SetRange( 0, 32 );

    for ( auto const array_index : Range(32) )
    {
        if ( bits.GetAt(array_index) != 1 )
        {
            test_number_that_failed = 150;
            return( failure() );
        }
    }

    bits.ClearRange( 0, 32 );

    for ( auto const array_index : Range(32) )
    {
        if ( bits.GetAt(array_index) != 0 )
        {
            test_number_that_failed = 151;
            return( failure() );
        }
    }

    bits.RemoveAll();

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 152;
        return( failure() );
    }

    bits.SetSize( 33 );

    if ( bits.GetSize() != 33 )
    {
        test_number_that_failed = 153;
        return( failure() );
    }

    bits.SetRange( 0, 32 );

    if ( bits.GetAt( 32 ) != 0 )
    {
        test_number_that_failed = 154;
        return( failure() );
    }

    for ( auto const array_index : Range(32) )
    {
        if ( bits.GetAt(array_index) != 1 )
        {
            test_number_that_failed = 155;
            return( failure() );
        }
    }

    bits.RemoveAll();

    if ( bits.GetSize() != 0 )
    {
        test_number_that_failed = 156;
        return( failure() );
    }

    bits.SetSize( 64 );

    if ( bits.GetSize() != 64 )
    {
        test_number_that_failed = 157;
        return( failure() );
    }

    bits.SetRange( 0, 64 );

    for ( auto const array_index : Range(64) )
    {
        if ( bits.GetAt(array_index) != 1 )
        {
            test_number_that_failed = 158;
            return( failure() );
        }
    }

    test_number_that_failed = 158;

    return( true );
}
