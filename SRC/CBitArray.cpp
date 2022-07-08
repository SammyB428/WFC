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
** $Workfile: CBitArray.cpp $
** $Revision: 26 $
** $Modtime: 6/26/01 10:44a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#endif // _DEBUG

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

// Construction

Win32FoundationClasses::CBitArray::CBitArray() noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( _DEBUG )
   m_NumberOfBitsPerGroup  = 8;
   m_NumberOfGroupsPerLine = 5;
#endif // _DEBUG

   m_IndexOfFirstBit   = 0;
   m_TotalNumberOfBits = 0;

   RemoveAll();
}

Win32FoundationClasses::CBitArray::CBitArray( _In_ std::size_t const initial_size ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( _DEBUG )
   m_NumberOfBitsPerGroup  = 8;
   m_NumberOfGroupsPerLine = 5;
#endif // _DEBUG

   m_IndexOfFirstBit   = 0;
   m_TotalNumberOfBits = 0;

   RemoveAll();
   SetSize( initial_size );
}

Win32FoundationClasses::CBitArray::CBitArray( _In_ Win32FoundationClasses::CBitArray const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( _DEBUG )
   m_NumberOfBitsPerGroup  = 8;
   m_NumberOfGroupsPerLine = 5;
#endif // _DEBUG

   m_IndexOfFirstBit   = 0;
   m_TotalNumberOfBits = 0;

   Copy( source );   
}

Win32FoundationClasses::CBitArray::CBitArray( _In_ std::vector<uint8_t> const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( _DEBUG )
   m_NumberOfBitsPerGroup  = 8;
   m_NumberOfGroupsPerLine = 5;
#endif // _DEBUG

   m_IndexOfFirstBit   = 0;
   m_TotalNumberOfBits = 0;

   Copy( source );
}

// Methods

void Win32FoundationClasses::CBitArray::ClearRange( _In_ std::size_t const array_index, _In_ std::size_t const count ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto const total_number_of_bits{ GetSize() };

   if ( array_index >= total_number_of_bits or ( array_index + count ) > total_number_of_bits )
   {
       _ASSERT_EXPR(false, _CRT_WIDE("Array index is out of bounds."));
      // Array out of bounds
      return;
   }

   std::size_t loop_counter{ 0 };
   std::size_t bit_array_index{ 0 };
   std::size_t bit_location{ 0 };
   SSIZE_T number_of_bits_remaining{ 0 };
   uint32_t bit_number{ 0 };

   while( loop_counter < count ) // Cannot be converted to a Range loop
   {
      bit_location = array_index + loop_counter;
      number_of_bits_remaining = total_number_of_bits - bit_location;

      if ( ( ( bit_location % SizeOfBitRepresentation() ) == 0 ) and (number_of_bits_remaining >= SizeOfBitRepresentation()) )
      {
         // We are on a boundary! We can optimize
         std::ignore = m_GetElementIndexOfBitLocation( bit_location, bit_array_index, bit_number );
         m_Bits.at( bit_array_index ) = 0;

         loop_counter += SizeOfBitRepresentation();
      }
      else
      {
         SetAt( bit_location, 0 );
         loop_counter++;
      }
   }
}

void Win32FoundationClasses::CBitArray::SetRange( _In_ std::size_t const array_index, _In_ std::size_t const count ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto const total_number_of_bits{ GetSize() };

   if ( array_index >= total_number_of_bits or ( array_index + count ) > total_number_of_bits )
   {
       _ASSERT_EXPR(false, _CRT_WIDE("Array index is out of bounds."));
       // Array out of bounds
      return;
   }

   std::size_t loop_counter{ 0 };
   std::size_t bit_array_index{ 0 };
   std::size_t bit_location{ 0 };
   SSIZE_T number_of_bits_remaining{ 0 };
   uint32_t bit_number{ 0 };

   while( loop_counter < count ) // Cannot be converted to a Range loop
   {
      bit_location = array_index + loop_counter;
      number_of_bits_remaining = total_number_of_bits - bit_location;

      if ( ( ( bit_location % SizeOfBitRepresentation() ) == 0 ) and (number_of_bits_remaining >= SizeOfBitRepresentation()) )
      {
         // We are on a boundary! We can optimize
          std::ignore = m_GetElementIndexOfBitLocation( bit_location, bit_array_index, bit_number );
         m_Bits.at( bit_array_index ) = AN_ELEMENT_THAT_IS_ALL_ONES;

         loop_counter += SizeOfBitRepresentation();
      }
      else
      {
         SetAt( bit_location, 1 );
         loop_counter++;
      }
   }
}

void Win32FoundationClasses::CBitArray::Add( _In_ uint32_t const value, _In_ std::size_t const count ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   std::size_t loop_counter{ 0 };
   std::size_t bit_location{ 0 };
   std::size_t index{ 0 };

   uint32_t bit_number{ 0 };

   while( loop_counter < count ) // Cannot be converted to a Range loop
   {
      bit_location = GetSize();

      if ( m_GetElementIndexOfBitLocation( bit_location, index, bit_number ) == true)
      {
         if ( index >= static_cast< uint32_t >( m_Bits.size() ) )
         {
            // We need to grow the array

            if ( ( count - loop_counter ) > SizeOfBitRepresentation() )
            {
               if ( value not_eq 0 )
               {
                  m_Bits.push_back( AN_ELEMENT_THAT_IS_ALL_ONES );
               }
               else
               {
                  m_Bits.push_back( 0 );
               }

               loop_counter        += SizeOfBitRepresentation();
               m_TotalNumberOfBits += SizeOfBitRepresentation();

               continue;
            }
            else
            {
               // Add a zero bit (an element that is all zeroes)
               m_Bits.push_back( 0 );
            }
         }

         m_TotalNumberOfBits++;

         // 1998-12-09 Peter Ekberg (peda@sectra.se) found a really nasty bug here.
         // I had assumed that only zero bits would be added to the array. While
         // this is true, those zero-bits could get stomped on by SetAll() and
         // Complement(). Because of this, we have to set the bit value every time.

         SetAt( bit_location, value );
      }
      else
      {
         ASSERT( false );
      }

      loop_counter++;
   }
}

void Win32FoundationClasses::CBitArray::AddByte( _In_ uint8_t const value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   uint32_t dword_value{ value };

   Add( _bittest(reinterpret_cast<LONG const *>(&dword_value), 7 ) );
   Add( _bittest(reinterpret_cast<LONG const *>(&dword_value), 6 ) );
   Add( _bittest(reinterpret_cast<LONG const *>(&dword_value), 5 ) );
   Add( _bittest(reinterpret_cast<LONG const *>(&dword_value), 4 ) );
   Add( _bittest(reinterpret_cast<LONG const *>(&dword_value), 3 ) );
   Add( _bittest(reinterpret_cast<LONG const *>(&dword_value), 2 ) );
   Add( _bittest(reinterpret_cast<LONG const *>(&dword_value), 1 ) );
   Add( _bittest(reinterpret_cast<LONG const *>(&dword_value), 0 ) );
}

void Win32FoundationClasses::CBitArray::AddValue( _In_ uint32_t const value, _In_ uint32_t const number_of_bits ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( number_of_bits < 33 );

   if ( number_of_bits > 32 )
   {
      return;
   }

   for ( auto const index : Range(number_of_bits) )
   {
      Add( 0 );
   }

   uint32_t offset{ number_of_bits - 1 };

   std::size_t starting_bit_location{ GetSize() - number_of_bits };

   for ( auto const index : Range(number_of_bits) )
   {
      if ( _bittest( reinterpret_cast<LONG const *>(&value), offset ) == 1 )
      {
         // Remember, we added only zeroes above, so we only
         // need to worry about setting one-values.

         SetAt( starting_bit_location + index, 1 );
      }

      offset--;
   }
}

void Win32FoundationClasses::CBitArray::Append( _In_ CBitArray const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // This method is a cheap and cheesy way of appending.
   // I should probably re-write it to take advantage
   // of m_SizeOfBitRepresentation and append things
   // an element at a time

   for ( auto const index : Range(source.GetSize()))
   {
      Add( source.GetAt( index ) );
   }
}

void Win32FoundationClasses::CBitArray::Append( __in_bcount( number_of_bytes ) uint8_t const * data, _In_ std::size_t const number_of_bytes ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( data );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      for ( auto const index : Range(number_of_bytes) )
      {
         AddByte( data[ index ] );
      }
   }
   WFC_CATCH_ALL
   {
      return;
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CBitArray::Complement( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   // Set all 1's to 0's and all 0's to 1's

#if ( AN_ELEMENT_THAT_IS_ALL_ONES == 0xFFFFFFFF )

   // If we are using 32 bits per element, we can optimize the
   // division to right shifting. Right shifting by 5 is the
   // same as dividing by 32 ( (x/32) == (x>>5) )

   std::size_t index{ m_IndexOfFirstBit >> 5 };

#else

   std::size_t index{ m_IndexOfFirstBit / SizeOfBitRepresentation() };

#endif // AN_ELEMENT_THAT_IS_ALL_ONES

   auto number_of_elements{ m_Bits.size() };

   // We set index to the first element that contains good bits.
   // We are skipping any elements at the beginning of m_Bits
   // that contain LeftTrim()'d bits. In other words, don't
   // complement unused bits.

   while( index < number_of_elements )  // Cannot be converted to a Range loop
   {
      m_Bits.at( index ) xor_eq AN_ELEMENT_THAT_IS_ALL_ONES;
      index++;
   }
}

void Win32FoundationClasses::CBitArray::Copy( _In_ Win32FoundationClasses::CBitArray const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Bits = source.m_Bits;
   m_TotalNumberOfBits = source.m_TotalNumberOfBits;
   m_IndexOfFirstBit   = source.m_IndexOfFirstBit;
}

void Win32FoundationClasses::CBitArray::Copy( _In_ std::vector<uint8_t> const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   RemoveAll();

   std::size_t index{ 0 };
   auto const number_of_bytes{ source.size() };

   if ( number_of_bytes > sizeof( uint32_t ) )
   {
      // We can add things 32 bits at a time

      uint32_t value_to_add{ 0 };

      while( index < (int) ( number_of_bytes - sizeof( uint32_t ) ) ) // Cannot be converted to a Range loop
      {
         value_to_add = MAKELONG( MAKEWORD( source.at( index + 3 ), source.at( index + 2 ) ), MAKEWORD( source.at( index + 1 ), source.at( index ) ) );
         m_Bits.push_back( value_to_add );
         m_TotalNumberOfBits += 32;
         index += sizeof( uint32_t );
      }
   }

   while( index < number_of_bytes )  // Cannot be converted to a Range loop
   {
      AddByte( source.at( index ) );
      index++;
   }
}

void Win32FoundationClasses::CBitArray::Copy( _In_ std::vector<uint32_t> const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_Bits = source;

#if ( AN_ELEMENT_THAT_IS_ALL_ONES == 0xFFFFFFFF )
   // We can optimize the calculation to a left shift if
   // our element size is a multiple of 2 (like 32)
   m_TotalNumberOfBits = source.size() << 5;
#else
   m_TotalNumberOfBits = source.size() * 32;
#endif // AN_ELEMENT_THAT_IS_ALL_ONES

   m_IndexOfFirstBit = 0;
}

void Win32FoundationClasses::CBitArray::CopyTo( __inout std::vector<uint8_t>& destination ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   if ( GetSize() == 0 )
   {
      destination.clear();
      return;
   }

   FreeExtra();

   uint32_t bits{ 0 };

   WORD word{ 0 };

   // 1998-12-09 Optimization suggested by Peter Ekberg (peda@sectra.se)

   auto const number_of_elements{ m_Bits.size() };

   destination.resize( number_of_elements * sizeof( uint32_t ) );

   // Get a raw pointer. This is the fastest way to access memory

   auto buffer_pointer{ destination.data() };

   WFC_VALIDATE_POINTER( buffer_pointer );

   uint32_t buffer_index{ 0 };

   for ( auto const index : Range(number_of_elements) )
   {
      bits = m_Bits.at( index );

      word = HIWORD( bits );

      buffer_pointer[ buffer_index ] = HIBYTE( word );
      buffer_index++;

      buffer_pointer[ buffer_index ] = LOBYTE( word );
      buffer_index++;

      word = LOWORD( bits );

      buffer_pointer[ buffer_index ] = HIBYTE( word );
      buffer_index++;

      buffer_pointer[ buffer_index ] = LOBYTE( word );
      buffer_index++;
   }

   // We have added a number of bytes that is divisible by four
   // We may not have needed to do such a thing. Let's see if
   // we can get rid of a couple of bytes.

   std::size_t const number_of_bytes{ (GetSize() + 7) >> 3 }; // ">> 3" is the same as saying "/ 8"

   destination.resize( number_of_bytes );

   _ASSERTE( (uint32_t) destination.size() == number_of_bytes );
}

void Win32FoundationClasses::CBitArray::CopyTo( __inout std::vector<uint32_t>& destination ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   FreeExtra();
   destination = m_Bits;
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CBitArray::SetDumpOptions( int number_of_bits_per_group, int number_of_groups_per_line )
{
   WFC_VALIDATE_POINTER( this );
   m_NumberOfBitsPerGroup  = number_of_bits_per_group;
   m_NumberOfGroupsPerLine = number_of_groups_per_line;
}

void Win32FoundationClasses::CBitArray::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );

   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   m_TotalNumberOfBits is " ) << m_TotalNumberOfBits << TEXT( "\n" );
   dump_context << TEXT( "   m_IndexOfFirstBit   is " ) << m_IndexOfFirstBit   << TEXT( "\n" );

   std::wstring debug_string;
   std::wstring line_number_string;

   std::size_t index{ 0 };
   std::size_t line_index{ 0 };
   std::size_t line_number{ 0 };
   std::size_t total_number_of_bits{ GetSize() };

   int digits_in_group{ 0 };

   while( index < total_number_of_bits )
   {
      if ( GetAt( index ) == 1 )
      {
         debug_string += TEXT( '1' );
      }
      else
      {
         debug_string += TEXT( '0' );
      }

      digits_in_group++;

      if ( digits_in_group >= (int) m_NumberOfBitsPerGroup )
      {
         debug_string += TEXT( ' ' );
         digits_in_group = 0;

         line_index++;

         if ( line_index == static_cast< uint32_t >( m_NumberOfGroupsPerLine ) )
         {
            line_number_string.Format( TEXT( "%05lu : " ), line_number );
            debug_string += TEXT( '\n' );
            line_number_string += debug_string;
            dump_context << line_number_string;
            debug_string.Empty();
            line_number_string.Empty();
            line_index = 0;
            line_number = index + 1;
         }
      }

      index++;
   }

   if ( debug_string.GetLength() > 0 )
   {
      debug_string += TEXT( '\n' );
      line_number_string.Format( TEXT( "%05lu : " ), line_number );
      line_number_string += debug_string;
      dump_context << line_number_string;
   }

   if ( dump_context.GetDepth() > 0 )
   {
      m_Bits.Dump( dump_context );
   }

   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ bool Win32FoundationClasses::CBitArray::EnumerateOnes( __inout std::size_t& enumerator ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   enumerator = 0xFFFFFFFF;

   if ( GetSize() > 0 )
   {
      return( true );
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CBitArray::EnumerateZeroes( __inout std::size_t& enumerator ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   enumerator = 0xFFFFFFFF;

   if ( GetSize() > 0 )
   {
      return( true );
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CBitArray::Find( _In_ Win32FoundationClasses::CBitArray const& value, _Inout_ std::size_t& found_at, _In_ std::size_t const starting_at ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   uint32_t value_to_search_for{ 0 };

   if ( value.GetSize() <= SizeOfBitRepresentation() )
   {
      // We can fit the bit array into a single uint32_t value, let's
      // use the fast version of this function

      value_to_search_for = value.GetValue( 0, (uint32_t) value.GetSize() );

      return( Find( value_to_search_for, value.GetSize(), found_at, starting_at ) );
   }

   // OK, we're bigger than a uint32_t, that means we have to do things the hard way

   value_to_search_for = value.GetValue( 0, SizeOfBitRepresentation() );

   bool exit_loop{ false };
   bool value_was_found{ false };

   auto starting_index{ starting_at };
   std::size_t found_index{ 0 };
   std::size_t loop_index{ 0 };

   auto ending_index{ GetSize() };
   auto size_of_value{ value.GetSize() };

   uint32_t this_bit{ 0 };
   uint32_t that_bit{ 0 };

   while( exit_loop == false )
   {
      if ( ( starting_index + size_of_value ) > ending_index )
      {
         // We ran out of room looking for the pattern
         return( false );
      }

      if ( Find( value_to_search_for, SizeOfBitRepresentation(), found_index, starting_index ) == true)
      {
         // We found the first 32 (sizeof(uint32_t)) bits, let's look for the
         // rest for them

         starting_index = found_index;

         loop_index = 0;

         value_was_found = true;

         this_bit = 0;
         that_bit = 0;

         while( loop_index < ( size_of_value - SizeOfBitRepresentation() ) ) // Cannot be converted to a Range loop
         {
            this_bit = value.GetAt( SizeOfBitRepresentation() + loop_index );

            if ( ( SizeOfBitRepresentation() + loop_index + found_index ) >= ending_index )
            {
               return( false );
            }

            that_bit = GetAt( SizeOfBitRepresentation() + loop_index + found_index );

            if ( this_bit == that_bit )
            {
               loop_index++;
            }
            else
            {
               value_was_found = false;
               starting_index++;
               loop_index = size_of_value; // Exit this loop
            }
         }

         if ( value_was_found == true)
         {
            found_at = found_index;
            return( true );
         }
      }
      else
      {
         // Can't find the bit-pattern
         return( false );
      }
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CBitArray::Find( _In_ uint32_t const value, _In_ std::size_t const number_of_bits_in_value, __inout std::size_t& found_at, _In_ std::size_t const starting_at ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   // Always start at a known state
   found_at = 0;

   ASSERT( number_of_bits_in_value <= GetSize() );

   if ( number_of_bits_in_value > GetSize() )
   {
      // There aren't enough bits in the array to possibly match the pattern.
      // There are more bits in the pattern than in the array.
      return( false );
   }

   auto upper_bound{ GetUpperBound() };
   std::size_t starting_at_index{ starting_at };

   // Let's do a Boyer-Moore'ish kind of search

   std::size_t desired_number_of_ones{ 0 };
   std::size_t desired_number_of_zeroes{ 0 };

   // Let's count the number of ones and zeroes in the pattern we're searching for

   for ( auto const array_index : Range(number_of_bits_in_value) )
   {
      if ( _bittest( reinterpret_cast<LONG const *>(&value), (LONG) array_index ) == 1 )
      {
         desired_number_of_ones++;
      }
      else
      {
         desired_number_of_zeroes++;
      }
   }

   // We reduce upper_bound because we can't possibly match
   // the pattern when there are fewer number of bits remaining
   // in the array than are in the pattern.

   upper_bound -= number_of_bits_in_value;

   uint32_t value_to_test{ 0 };
   std::size_t number_of_bits_to_skip{ 0 };
   std::size_t number_of_ones{ 0 };
   std::size_t number_of_zeroes{ 0 };

   // Finally! We're ready to start searching the array

   while( starting_at_index <= ( upper_bound + 1 ) )
   {
      value_to_test = GetValue( starting_at_index, number_of_bits_in_value );

      if ( value_to_test == value )
      {
         found_at = starting_at_index;
         return( true );
      }

      // OK, we haven't found the desired value (i.e. bit-pattern)
      // Let's use the Boyer-Moore'ish trick of jumping ahead a few
      // bits instead of incrementing by only one. The first step in
      // determining how many bits we can skip is to count the number
      // of ones and zeroes in the value we just pulled from the array.

      number_of_ones         = 0;
      number_of_zeroes       = 0;
      number_of_bits_to_skip = 1; // We can always skip one bit

      for ( auto const array_index : Range(number_of_bits_in_value) )
      {
         if ( _bittest( reinterpret_cast<LONG const *>(&value_to_test), (LONG) array_index ) == 1 )
         {
            number_of_ones++;
         }
         else
         {
            number_of_zeroes++;
         }
      }

      // Now we know how many ones and zeroes are in the value we
      // know isn't the right one, and we know how many ones and
      // zeroes are in the value we're searching for. Here's where
      // Boyer-Moore comes in. We know the number of ones in the
      // pattern we're searching for and the number of ones in the
      // portion of the array we get from the array must be equal.
      // Likewise, the number of zeroes must be equal. Given this
      // fact, we can calculate the difference between what we
      // need and what we found and skip ahead those number of bits.
      // For example, if the pattern we're searching for has 31 zeroes
      // and the number of zeroes in what we just pulled from the array
      // contained only 15 zeroes, we can safely skip the next 16 bits.
      // Kinda neato.

      if ( number_of_ones < desired_number_of_ones )
      {
         number_of_bits_to_skip = desired_number_of_ones - number_of_ones;
      }

      if ( number_of_zeroes < desired_number_of_zeroes )
      {
         // Let's see if this difference is greater than the current number of bits to skip

         if ( ( desired_number_of_zeroes - number_of_zeroes ) > number_of_bits_to_skip )
         {
            number_of_bits_to_skip = desired_number_of_zeroes - number_of_zeroes;
         }
      }

      starting_at_index += number_of_bits_to_skip;
   }

   return( false );
}

void Win32FoundationClasses::CBitArray::FreeExtra( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto number_of_bits{ m_IndexOfFirstBit };

   if ( number_of_bits >= m_TotalNumberOfBits )
   {
      RemoveAll();
      return;
   }

   while( number_of_bits >= SizeOfBitRepresentation() )
   {
      m_Bits.erase(std::begin(m_Bits));
      number_of_bits      -= SizeOfBitRepresentation();
      m_TotalNumberOfBits -= SizeOfBitRepresentation();
   }

   uint32_t this_set_of_bits{ 0 };
   uint32_t that_set_of_bits{ 0 };
   uint32_t bit_number{ 0 };

   SSIZE_T index{ 0 };

   while( number_of_bits > 0 )
   {
      this_set_of_bits = 0;
      that_set_of_bits = 0;
      bit_number       = MostSignificantBitLocation();

      // Always start at the beginning of the array

      this_set_of_bits = m_Bits.at( index );

      if ( bit_number > 0 )
      {
         // Only kill the bit if it ain't the LSB

         uint32_t bit_index{ bit_number - 1 };

         while( ( bit_index + 1 ) > 0 )
         {
            // Why can't I do a left shift here?

            if ( _bittest( reinterpret_cast<LONG const *>(&this_set_of_bits), bit_index ) not_eq 0 )
            {
               _bittestandset( reinterpret_cast<LONG *>(&this_set_of_bits), bit_index + 1 );
            }
            else
            {
               _bittestandreset( reinterpret_cast<LONG *>(&this_set_of_bits), bit_index + 1 );
            }

            bit_index--;
         }

         m_Bits.at( index ) = this_set_of_bits;
      }

      auto loop_index{ m_Bits.size() - 1 };

      for( auto const this_set_of_bits_index : Range(loop_index) )
      {
         this_set_of_bits = m_Bits.at(this_set_of_bits_index);
         that_set_of_bits = m_Bits.at(this_set_of_bits_index + 1 );

         if ( _bittest( reinterpret_cast<LONG const *>(&that_set_of_bits), MostSignificantBitLocation() ) == 1 )
         {
            _bittestandset( reinterpret_cast<LONG *>(&this_set_of_bits), 0 );
         }
         else
         {
            _bittestandreset( reinterpret_cast<LONG *>(&this_set_of_bits), 0 );
         }

         that_set_of_bits <<= 1;

         m_Bits.at(this_set_of_bits_index) = this_set_of_bits;
         m_Bits.at(this_set_of_bits_index + 1 ) = that_set_of_bits;
      }

      number_of_bits--;
      m_TotalNumberOfBits--;
   }

   m_IndexOfFirstBit = 0;
}

_Check_return_ bool Win32FoundationClasses::CBitArray::GetNextOne( __inout std::size_t& enumerator ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( enumerator == 0xFFFFFFFF )
   {
      // Starting at the beginning, skip all zero-filled members

      SSIZE_T element_index{ 0 };
      SSIZE_T const number_of_elements{ (SSIZE_T)m_Bits.size() };

      while( element_index < ( number_of_elements - 1 ) and
             m_Bits.at( element_index ) == 0 )
      {
         element_index++;
      }

#if ( AN_ELEMENT_THAT_IS_ALL_ONES == 0xFFFFFFFF )
   // We can optimize the calculation to a left shift if
   // our element size is a multiple of 2 (like 32)
      SSIZE_T bit_index{ element_index << 5 };
#else
      SSIZE_T bit_index{ element_index * m_SizeOfBitRepresentation };
#endif // AN_ELEMENT_THAT_IS_ALL_ONES

      SSIZE_T const number_of_bits_in_array{ (SSIZE_T)GetSize() };

      while( bit_index < number_of_bits_in_array and GetAt( bit_index ) == 0 )
      {
         bit_index++;
      }

      if ( bit_index >= number_of_bits_in_array )
      {
         return( false );
      }

      enumerator = bit_index;
      return( true );
   }

   // We're picking up where we left off from a previous call to GetNextOne().

   std::size_t bit_index{ enumerator + 1 }; // Always start after the one we previously found
   auto const number_of_bits_in_array{ GetSize() };

   if ( bit_index >= number_of_bits_in_array )
   {
      return( false );
   }

   while( bit_index < number_of_bits_in_array and
          ( bit_index % SizeOfBitRepresentation() ) not_eq 0 )
   {
      if ( GetAt( bit_index ) == 1 )
      {
         enumerator = bit_index;
         return( true );
      }

      bit_index++;
   }

#if ( AN_ELEMENT_THAT_IS_ALL_ONES == 0xFFFFFFFF )

   // If we are using 32 bits per element, we can optimize the
   // division to right shifting. Right shifting by 5 is the
   // same as dividing by 32 ( (x/32) == (x>>5) )
   std::size_t element_index{ bit_index / SizeOfBitRepresentation() };
#else
   std::size_t element_index{ bit_index >> 5 };
#endif // AN_ELEMENT_THAT_IS_ALL_ONES

   auto number_of_elements{ m_Bits.size() };

   while( element_index < ( number_of_elements - 1 ) and
          m_Bits.at( element_index ) == 0 )
   {
      element_index++;
   }

#if ( AN_ELEMENT_THAT_IS_ALL_ONES == 0xFFFFFFFF )
   // We can optimize the calculation to a left shift if
   // our element size is a multiple of 2 (like 32)
   bit_index = element_index << 5;
#else
   bit_index = element_index * m_SizeOfBitRepresentation;
#endif

   bit_index += ( ( enumerator + 1 ) % SizeOfBitRepresentation() );

   while( bit_index < number_of_bits_in_array and GetAt( bit_index ) == 0 )
   {
      bit_index++;
   }

   if ( bit_index >= number_of_bits_in_array )
   {
      return( false );
   }

   enumerator = bit_index;
   return( true );
}

_Check_return_ bool Win32FoundationClasses::CBitArray::GetNextZero( __inout std::size_t& enumerator ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( enumerator == 0xFFFFFFFF )
   {
      // Starting at the beginning, skip all zero-filled members

      SSIZE_T element_index{ 0 };
      SSIZE_T const number_of_elements{ (SSIZE_T)m_Bits.size() };

      while( element_index < ( number_of_elements - 1 ) and
             m_Bits.at( element_index ) == AN_ELEMENT_THAT_IS_ALL_ONES )
      {
         element_index++;
      }

#if ( AN_ELEMENT_THAT_IS_ALL_ONES == 0xFFFFFFFF )
   // We can optimize the calculation to a left shift if
   // our element size is a multiple of 2 (like 32)
      std::size_t bit_index{ static_cast<std::size_t>(element_index) << 5 };
#else
      std::size_t bit_index{ element_index * m_SizeOfBitRepresentation };
#endif // AN_ELEMENT_THAT_IS_ALL_ONES

      auto const number_of_bits_in_array{ GetSize() };

      while( bit_index < number_of_bits_in_array and GetAt( bit_index ) == 1 )
      {
         bit_index++;
      }

      if ( bit_index >= number_of_bits_in_array )
      {
         return( false );
      }

      enumerator = bit_index;
      return( true );
   }

   // We're picking up where we left off from a previous call to GetNextZero().

   std::size_t bit_index{ enumerator + 1 }; // Always start after the one we previously found
   auto const number_of_bits_in_array{ GetSize() };

   if ( bit_index >= number_of_bits_in_array )
   {
      return( false );
   }

   while( bit_index < number_of_bits_in_array and
          ( bit_index % SizeOfBitRepresentation() ) not_eq 0 )
   {
      if ( GetAt( bit_index ) == 0 )
      {
         enumerator = bit_index;
         return( true );
      }

      bit_index++;
   }

#if ( AN_ELEMENT_THAT_IS_ALL_ONES == 0xFFFFFFFF )

   // If we are using 32 bits per element, we can optimize the
   // division to right shifting. Right shifting by 5 is the
   // same as dividing by 32 ( (x/32) == (x>>5) )
   std::size_t element_index{ bit_index >> 5 };
#else
   std::size_t element_index{ bit_index / SizeOfBitRepresentation() };
#endif // AN_ELEMENT_THAT_IS_ALL_ONES

   auto number_of_elements{ m_Bits.size() };

   while( element_index < ( number_of_elements - 1 ) and
          m_Bits.at( element_index ) == AN_ELEMENT_THAT_IS_ALL_ONES )
   {
      element_index++;
   }

#if ( AN_ELEMENT_THAT_IS_ALL_ONES == 0xFFFFFFFF )
   // We can optimize the calculation to a left shift if
   // our element size is a multiple of 2 (like 32)
   bit_index = element_index << 5;
#else
   bit_index = element_index * SizeOfBitRepresentation();
#endif // AN_ELEMENT_THAT_IS_ALL_ONES

   bit_index += ( ( enumerator + 1 ) % SizeOfBitRepresentation() );

   while( bit_index < number_of_bits_in_array and GetAt( bit_index ) == 1 )
   {
      bit_index++;
   }

   if ( bit_index >= number_of_bits_in_array )
   {
      return( false );
   }

   enumerator = bit_index;
   return( true );
}

_Check_return_ std::size_t Win32FoundationClasses::CBitArray::GetNumberOfOnes( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   std::size_t number_of_ones{ 0 };

   for ( auto const index : Range(GetSize()) )
   {
      if ( GetAt( index ) == 1 )
      {
         number_of_ones++;
      }
   }

   return( number_of_ones );
}

_Check_return_ std::size_t Win32FoundationClasses::CBitArray::GetNumberOfZeroes( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( GetSize() - GetNumberOfOnes() );
}

_Check_return_ uint32_t Win32FoundationClasses::CBitArray::GetValue( _In_ std::size_t const starting_bit_location, _In_ std::size_t const number_of_bits ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( number_of_bits < 33 );

   if ( number_of_bits > SizeOfBitRepresentation() )
   {
      return( 0 );
   }

   if ( ( starting_bit_location + number_of_bits ) > GetSize() )
   {
      return( 0 );
   }

   uint32_t return_value{ 0 };
   uint32_t bit_value{ 0 };

   for ( auto const index : Range(number_of_bits) )
   {
      return_value <<= 1;

      bit_value = GetAt( starting_bit_location + index );

      return_value += bit_value;
   }

   return( return_value );
}

void Win32FoundationClasses::CBitArray::InsertAt( _In_ std::size_t const bit_location, _In_ uint32_t const value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( bit_location == 0 )
   {
      if ( m_IndexOfFirstBit > 0 )
      {
         m_IndexOfFirstBit--;
         SetAt( 0, value );
         return;
      }
   }

   if ( bit_location > GetUpperBound() )
   {
      // Attempting to insert passed the end of the array
      // we call this kind of thing Append().

      Add( value );
   }

   Add( 0 );

   uint32_t this_set_of_bits{ 0 };
   uint32_t that_set_of_bits{ 0 };
   uint32_t bit_number{ 0 };

   std::size_t array_index{ 0 };

   if ( m_GetElementIndexOfBitLocation( bit_location, array_index, bit_number ) == false )
   {
      ASSERT( false );
      return;
   }

   auto loop_index{ m_Bits.size() - 1 };

   if ( loop_index >= 1 )
   {
      this_set_of_bits = m_Bits.at( loop_index );
      this_set_of_bits >>= 1;

      m_Bits.at( loop_index ) = (uint32_t) this_set_of_bits;

      loop_index--;

      while( loop_index > array_index )
      {
         that_set_of_bits = m_Bits.at( loop_index + 1 );
         this_set_of_bits = m_Bits.at( loop_index );

         // The most significant bit of that set of bit should be equal to
         // the least significant bit of this set of bits.

         if ( _bittest( reinterpret_cast<LONG const *>(&this_set_of_bits), 0 ) == 1 )
         {
            _bittestandset( reinterpret_cast<LONG *>(&that_set_of_bits), MostSignificantBitLocation() );
         }
         else
         {
            _bittestandreset( reinterpret_cast<LONG *>(&that_set_of_bits), MostSignificantBitLocation() );
         }

         this_set_of_bits >>= 1;

         m_Bits.at( loop_index + 1 ) = static_cast<uint32_t>(that_set_of_bits);
         m_Bits.at( loop_index )     = static_cast<uint32_t>(this_set_of_bits);

         loop_index--;
      }

      this_set_of_bits = m_Bits.at( array_index );

      auto upper_bound{ m_Bits.size() - 1 };

      if ( bit_number not_eq 0 and array_index < upper_bound )
      {
         that_set_of_bits = m_Bits.at( array_index + 1 );

         if ( _bittest( reinterpret_cast<LONG const *>(&this_set_of_bits), 0 ) == 1 )
         {
            _bittestandset( reinterpret_cast<LONG *>(&that_set_of_bits), MostSignificantBitLocation() );
         }
         else
         {
            _bittestandreset( reinterpret_cast<LONG *>(&that_set_of_bits), MostSignificantBitLocation() );
         }

         m_Bits.at( array_index + 1 ) = that_set_of_bits;
         m_Bits.at( array_index )     = this_set_of_bits;
      }
   }

   // Now the least significant bit is safe

   this_set_of_bits = m_Bits.at( array_index );

   for ( auto const bit_index : Range(bit_number) )
   {
      if ( _bittest(reinterpret_cast<LONG const *>(&this_set_of_bits), static_cast<LONG>(bit_index + 1) ) == 1 )
      {
         _bittestandset(reinterpret_cast<LONG *>(&this_set_of_bits), static_cast<LONG>(bit_index) );
      }
      else
      {
         _bittestandreset(reinterpret_cast<LONG *>(&this_set_of_bits), static_cast<LONG>(bit_index) );
      }
   }

   if ( value not_eq 0 )
   {
      _bittestandset(reinterpret_cast<LONG *>(&this_set_of_bits), bit_number );
   }
   else
   {
      _bittestandreset(reinterpret_cast<LONG *>(&this_set_of_bits), bit_number );
   }

   m_Bits.at( array_index ) = this_set_of_bits;
}

_Check_return_ Win32FoundationClasses::CBitArray Win32FoundationClasses::CBitArray::Left( _In_ std::size_t const number_of_bits ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   Win32FoundationClasses::CBitArray return_value;

   if ( number_of_bits >= GetSize() )
   {
      return_value.Copy( *this );
   }
   else
   {
      for ( auto const array_index : Range(number_of_bits) )
      {
         return_value.Add( GetAt( array_index ) );
      }
   }

   return( return_value );
}

void Win32FoundationClasses::CBitArray::LeftTrim( _In_ std::size_t const number_of_bits ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( number_of_bits >= GetSize() )
   {
      RemoveAll();
      return;
   }

   m_IndexOfFirstBit += number_of_bits;
}

_Check_return_ Win32FoundationClasses::CBitArray Win32FoundationClasses::CBitArray::Mid( _In_ std::size_t const starting_at, _In_ std::size_t const number_of_bits ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   Win32FoundationClasses::CBitArray return_value;

   if ( starting_at > GetUpperBound() )
   {
      // There's nothing to return
      return( return_value );
   }

   if ( ( starting_at + number_of_bits ) > GetUpperBound() )
   {
      return( return_value );
   }

   for ( auto const index : Range(number_of_bits) )
   {
      return_value.Add( GetAt( index + starting_at ) );
   }

   return( return_value );
}

void Win32FoundationClasses::CBitArray::RemoveAt( _In_ std::size_t const bit_location ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( GetSize() > 0 );

   if ( GetSize() < 1 )
   {
      return;
   }

   // There one simple optimization we can perform, if the user
   // wants to remove the bit at the beginning, it is the same
   // as LeftTrim()'ing

   if ( bit_location == 0 )
   {
      LeftTrim( 1 );
      return;
   }

   if ( bit_location > GetUpperBound() )
   {
      return;
   }

   uint32_t this_set_of_bits{ 0 };
   uint32_t that_set_of_bits{ 0 };
   uint32_t bit_number{ 0 };

   std::size_t index{ 0 };

   if ( m_GetElementIndexOfBitLocation( bit_location, index, bit_number ) == false )
   {
      ASSERT( false );
      return;
   }

   this_set_of_bits = m_Bits.at( index );

   if ( bit_number > 0 )
   {
       std::size_t bit_index{ bit_number - 1 };

      while( ( bit_index + 1 ) > 0 )
      {
         if ( _bittest( reinterpret_cast<LONG const *>(&this_set_of_bits), (LONG) bit_index ) == 1 )
         {
            _bittestandset( reinterpret_cast<LONG *>(&this_set_of_bits), (LONG) (bit_index + 1 ) );
         }
         else
         {
            _bittestandreset( reinterpret_cast<LONG *>(&this_set_of_bits), (LONG) (bit_index + 1 ) );
         }

         bit_index--;
      }

      m_Bits.at( index ) = this_set_of_bits;
   }

   SSIZE_T loop_index{ static_cast<SSIZE_T>(m_Bits.size()) - 1 };

   while( (SSIZE_T) index < loop_index )
   {
      this_set_of_bits = m_Bits.at( index );
      that_set_of_bits = m_Bits.at( index + 1 );

      if ( _bittest( reinterpret_cast<const LONG *>(&that_set_of_bits), MostSignificantBitLocation() ) == 1 )
      {
         _bittestandset( reinterpret_cast<LONG *>(&this_set_of_bits), 0 );
      }
      else
      {
         _bittestandreset( reinterpret_cast<LONG *>(&this_set_of_bits), 0 );
      }

      that_set_of_bits <<= 1;

      m_Bits.at( index ) = this_set_of_bits;
      m_Bits.at( index + 1 ) = that_set_of_bits;

      index++;
   }

   m_TotalNumberOfBits--;
}

_Check_return_ Win32FoundationClasses::CBitArray Win32FoundationClasses::CBitArray::Right( _In_ std::size_t const number_of_bits ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   Win32FoundationClasses::CBitArray return_value;

   if ( number_of_bits >= GetSize() )
   {
      return_value.Copy( *this );
   }
   else
   {
      std::size_t get_at_index{ GetSize() - number_of_bits };

      for ( auto const index :Range(number_of_bits) )
      {
         return_value.Add( GetAt( get_at_index + index ) );
      }
   }

   return( return_value );
}

void Win32FoundationClasses::CBitArray::RightTrim( _In_ std::size_t const number_of_bits ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( number_of_bits >= GetSize() )
   {
      RemoveAll();
      return;
   }

   m_TotalNumberOfBits -= number_of_bits;

   // 1998-12-09 Peter Ekberg (peda@sectra.se) found a bug here. I was
   // having a bit of a rounding error (no, I couldn't resist the pun).

   auto number_of_elements{ GetSize() };

   number_of_elements += ( SizeOfBitRepresentation() - 1 );
   number_of_elements /= SizeOfBitRepresentation();

   m_Bits.resize( number_of_elements );
}

void Win32FoundationClasses::CBitArray::SetAll( _In_ uint32_t const value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( value not_eq 0 )
   {
      ::memset( m_Bits.data(), 0xFF, ( m_Bits.size() * sizeof( uint32_t ) ) );
   }
   else
   {
      ::memset( m_Bits.data(), 0x00, ( m_Bits.size() * sizeof( uint32_t ) ) );
   }
}

void Win32FoundationClasses::CBitArray::SetAt( _In_ std::size_t const bit_location, _In_ uint32_t const value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( bit_location > GetUpperBound() )
   {
      return;
   }

   std::size_t index{ 0 };

   uint32_t bit_number{ 0 };

   if ( m_GetElementIndexOfBitLocation( bit_location, index, bit_number ) == false )
   {
      ASSERT( false );
      return;
   }

   // Thanks go to Anne Davidson (davidsoa@nichols.com) for finding
   // a bug here. I should have been testing for greater than or
   // equal to, not just greater than. DOH!
   // 1998-07-21

   if ( index >= static_cast< uint32_t >( m_Bits.size() ) )
   {
      return;
   }

   uint32_t this_set_of_bits{ m_Bits.at(index) };

   if ( value not_eq 0 )
   {
      _bittestandset( reinterpret_cast<LONG *>(&this_set_of_bits), bit_number );
   }
   else
   {
      _bittestandreset( reinterpret_cast<LONG *>(&this_set_of_bits), bit_number );
   }

   m_Bits.at( index ) = this_set_of_bits;
}

void Win32FoundationClasses::CBitArray::SetSize( _In_ std::size_t const number_of_bits ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( number_of_bits == 0 )
   {
      RemoveAll();
      return;
   }

   // Thanks go to Jeff Winkler (winkler1@sprynet.com)
   // for finding a bug here where number_of_bits is
   // less than m_SizeOfBitRepresentation and GetSize()
   // is zero. It would not add. DOH!

   uint32_t number_of_elements{ static_cast<uint32_t>(m_Bits.size()) };

#if ( AN_ELEMENT_THAT_IS_ALL_ONES == 0xFFFFFFFF )
   // We can optimize the calculation to a left shift if
   // our element size is a multiple of 2 (like 32)
   if ( number_of_bits > (size_t) ( (size_t) number_of_elements << 5 ) )
#else
   if ( number_of_bits > ( number_of_elements * SizeOfBitRepresentation() ) )
#endif // AN_ELEMENT_THAT_IS_ALL_ONES
   {
      std::size_t number_of_elements_to_add{ number_of_bits - GetSize() };

      // Thanks go to Anne Davidson (davidsoa@nichols.com) for finding
      // a bug here in that caused a rounding error.
      // 1998-07-21

      number_of_elements_to_add += ( SizeOfBitRepresentation() - 1 );

      number_of_elements_to_add /= SizeOfBitRepresentation();

      if ( number_of_elements == 0 and number_of_elements_to_add == 0 )
      {
         // Dang math problem...
         number_of_elements_to_add++;
      }

      m_Bits.resize( number_of_elements + number_of_elements_to_add );
   }

   // 1998-12-09 Peter Ekberg (peda@sectra.se) found a bug here. I was
   // not adding m_IndexOfFirstBit to number_of_bits. This was stupid of me. :-(
   m_TotalNumberOfBits = number_of_bits + m_IndexOfFirstBit;
}

void Win32FoundationClasses::CBitArray::SetValue(_In_ std::size_t const starting_bit_location, _In_ uint32_t const value, _In_ uint32_t const number_of_bits ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( number_of_bits <= static_cast<uint32_t>(SizeOfBitRepresentation()) );

   if ( number_of_bits > static_cast<uint32_t>(SizeOfBitRepresentation()) or number_of_bits < 1 )
   {
      return;
   }

   uint32_t offset{ number_of_bits - 1 };

   for ( auto const index : Range(number_of_bits) )
   {
      // 1998-12-08 Thanks to Peter Ekberg (peda@sectra.se) for finding
      // a bug here. I had the arguments to bit_test() reversed.

      SetAt( starting_bit_location + index, _bittest( reinterpret_cast<LONG const *>(&value), offset ) );
      offset--;
   }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CBitArray</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, bits, bit array, array of bits, source code">
<META name="description" content="This C++ class let's you manipulate a set of bits like it was an array.">
</HEAD>


<BODY>

<H1>CBitArray</H1>
$Revision: 26 $<BR><HR>

<H2>Description</H2>

This class treats bits like they were an integral type.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CBitArray</B>()
<B>CBitArray</B>( uint32_t initial_size )
<B>CBitArray</B>( CBitArray const&amp; source )
<B>CBitArray</B>( std::vector&lt;uint8_t&gt; const&amp; source )<DD>
Constructs this object.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Add">Add</A></B>( uint32_t bit_to_add, uint32_t count = 1 )</PRE><DD>
Appends a new bit to the array. If <CODE>bit_to_add</CODE>
is not zero, a 1-bit is added.

<DT><PRE>void <B><A NAME="AddByte">AddByte</A></B>( uint8_t byte_to_add )</PRE><DD>
Appends byte (8 bits) to the array.

<DT><PRE>void <B><A NAME="AddValue">AddValue</A></B>( uint32_t value_to_add, uint32_t number_of_bits_in_value )</PRE><DD>
Appends a a value to the array. You specify how many bits are in <CODE>value_to_add</CODE>
(up to 32).

<DT><PRE>void <B><A NAME="Append">Append</A></B>( const CBitArray&amp; source  )
void <B>Append</B>( const BYTE * data, uint32_t number_of_bytes )</PRE><DD>
Appends a whole bunch of bits to the array.

<DT><PRE>void <B><A NAME="Complement">Complement</A></B>( void )</PRE><DD>
Turns every zero into a one and every one into a zero in the array.

<DT><PRE>void <B><A NAME="Copy">Copy</A></B>( const CBitArray&amp; source )
void <B>Copy</B>( const std::vector&lt;uint8_t&gt;&amp; source )</PRE><DD>
Copies bits from the specified <CODE>source</CODE>.

<DT><PRE>void <B><A NAME="CopyTo">CopyTo</A></B>( std::vector&lt;uint8_t&gt;&amp; destination )
void <B>CopyTo</B>( std::vector&lt;uint32_t&gt;&amp; destination )</PRE><DD>
Exports the bits to the <CODE>destination</CODE> supplied.

<DT><PRE>bool <B><A NAME="Find">Find</A></B>( const CBitArray&amp; value, uint32_t&amp; found_at, uint32_t starting_at = 0 ) const
bool <B>Find</B>( uint32_t value, uint32_t number_of_bits_in_value, uint32_t&amp; found_at, uint32_t starting_at = 0 ) const</PRE><DD>
Searches for a bit pattern in the array. If it returns true, <CODE>found_at</CODE>
will be filled with the index where the pattern begins.

<DT><PRE>void <B><A NAME="FreeExtra">FreeExtra</A></B>( void )</PRE><DD>
Compresses the memory used to store the bits.

<DT><PRE>uint32_t <B><A NAME="GetAt">GetAt</A></B>( uint32_t index ) const</PRE><DD>
Returns a 1 if the bit at that <CODE>index</CODE> is not zero.
It will return 0 if that bit is a zero. If <CODE>index</CODE>
is something wacky (i.e. passed the end of the array), it will
return 0.

<DT><PRE>uint32_t <B><A NAME="GetNumberOfOnes">GetNumberOfOnes</A></B>( void ) const</PRE><DD>
Returns the number of bits that are 1.

<DT><PRE>uint32_t <B><A NAME="GetNumberOfZeroes">GetNumberOfZeroes</A></B>( void ) const</PRE><DD>
Returns the number of bits that are 0.

<DT><PRE>uint32_t <B><A NAME="GetSize">GetSize</A></B>( void ) const</PRE><DD>
Returns the number of bits in the array.

<DT><PRE>uint32_t <B><A NAME="GetUpperBound">GetUpperBound</A></B>( void ) const</PRE><DD>
Returns the current upper bound of this array. Because array indexes are zero-based, this function returns a value 1 less than GetSize().

<DT><PRE>uint32_t <B><A NAME="GetValue">GetValue</A></B>( uint32_t index, uint32_t length ) const</PRE><DD>
Returns the value at the given location.

<DT><PRE>void <B><A NAME="InsertAt">InsertAt</A></B>( uint32_t index, uint32_t value )</PRE><DD>
Inserts a single bit at the given <CODE>index</CODE>.

<DT><PRE>CBitArray <B><A NAME="Left">Left</A></B>( uint32_t number_of_bits ) const</PRE><DD>
Returns the left-most <CODE>number_of_bits</CODE>.

<DT><PRE>void <B><A NAME="LeftTrim">LeftTrim</A></B>( uint32_t number_of_bits )</PRE><DD>
Trims the <CODE>number_of_bits</CODE> from the beginning
of the array.

<DT><PRE>CBitArray <B><A NAME="Mid">Mid</A></B>( uint32_t starting_at_index, uint32_t number_of_bits ) const</PRE><DD>
Returns <CODE>number_of_bits</CODE> beginning at <CODE>starting_at_index</CODE>.

<DT><PRE>void <B><A NAME="RemoveAll">RemoveAll</A></B>( void )</PRE><DD>
Clears the array. It removes all bits.

<DT><PRE>void <B><A NAME="RemoveAt">RemoveAt</A></B>( uint32_t index )</PRE><DD>
Removes a single bit at the given <CODE>index</CODE>.

<DT><PRE>CBitArray <B><A NAME="Right">Right</A></B>( uint32_t number_of_bits )</PRE><DD>
Returns the last <CODE>number_of_bits</CODE> in the array.

<DT><PRE>void <B><A NAME="RightTrim">RightTrim</A></B>( uint32_t number_of_bits )</PRE><DD>
Removes the last <CODE>number_of_bits</CODE> from the array.

<DT><PRE>void <B><A NAME="Serialize">Serialize</A></B>( CArchive&amp; archive )</PRE><DD>
After all, we are a CObject...

<DT><PRE>void <B><A NAME="SetAll">SetAll</A></B>( uint32_t value )</PRE><DD>
Sets all of the bits in the array to <CODE>value</CODE>.

<DT><PRE>void <B><A NAME="SetAt">SetAt</A></B>( uint32_t index, uint32_t value )</PRE><DD>
Sets the bits at the given <CODE>index</CODE> to <CODE>value</CODE>.

<DT><PRE>void <B><A NAME="SetSize">SetSize</A></B>( uint32_t size )</PRE><DD>
Allocates enough memory to hold <CODE>size</CODE> bits.

<DT><PRE>void <B><A NAME="SetValue">SetValue</A></B>( uint32_t index, uint32_t value, uint32_t length )</PRE><DD>
Beginning at <CODE>index</CODE>, the bits in <CODE>value</CODE> will
replace the bits in the array for <CODE>length</CODE> number of bits.

</DL>

<H2>Operators</H2>

<DL COMPACT>

<DT><PRE>CBitArray&amp; operator <B>=</B> ( const CBitArray&amp; source )
CBitArray&amp; operator <B>=</B> ( const std::vector&lt;uint8_t&gt;&amp; source )</PRE><DD>
Basically calls <B>Copy</B>().

<DT><PRE>CBitArray&amp; operator <B>+=</B> ( const CBitArray&amp; source )
CBitArray&amp; operator <B>+=</B> ( const std::vector&lt;uint8_t&gt;&amp; source )</PRE><DD>
Basically calls <B>Append</B>().

<DT><PRE>uint32_t operator <B>[]</B> ( int index ) const</PRE><DD>
Calls <B>GetAt</B>().

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   <B>CBitArray</B> bits;

   bits.Add( 0x55 );

   _tprintf( TEXT( &quot;Bit number 7 is %d\n&quot;, (int) bits.GetAt( 7 ) );

   bits.SetAt( 7, 0 ); // Turn bit 7 off
}</CODE></PRE>
<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CBitArray.cpp $<BR>
$Modtime: 6/26/01 10:44a $
</BODY>

</HTML>
*/
