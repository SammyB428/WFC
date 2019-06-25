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
** $Workfile: CBitArray.hpp $
** $Revision: 12 $
** $Modtime: 6/26/01 11:00a $
*/

#if ! defined( BIT_ARRAY_CLASS_HEADER )

#define BIT_ARRAY_CLASS_HEADER

class CBitArray
{
   protected:

      std::vector<uint32_t> m_Bits;

      std::size_t m_TotalNumberOfBits{ 0 };
      std::size_t m_IndexOfFirstBit{ 0 };

#if ! defined( _DEBUG )

      inline

#endif // _DEBUG

      __checkReturn bool m_GetElementIndexOfBitLocation( __in std::size_t const bit_location, __out std::size_t& index, __out uint32_t& bit_number_in_element ) const noexcept;

#if defined( _DEBUG )

      int m_NumberOfBitsPerGroup{ 0 };
      int m_NumberOfGroupsPerLine{ 0 };

#endif // _DEBUG

   public:

      // Construction

      CBitArray();
      CBitArray( _In_ std::size_t const initial_size );
      CBitArray(_In_ CBitArray const&  source );
      CBitArray(_In_ std::vector<uint8_t> const& source );

      virtual ~CBitArray();

      static inline constexpr int SizeOfBitRepresentation(void) noexcept { return(32); }
      static inline constexpr int MostSignificantBitLocation(void) noexcept { return(31); }

      // Methods

      void                    Add( __in uint32_t const bit_to_add, __in std::size_t const count = 1 ) noexcept;
      void                    AddByte( __in uint8_t const byte_to_add ) noexcept;
      void                    AddValue( __in uint32_t const value_to_add, __in uint32_t const number_of_bits_in_value ) noexcept;
      void                    Append( __in CBitArray const& source ) noexcept;
      void                    Append( __in_bcount( number_of_bytes ) uint8_t const * data, __in std::size_t const number_of_bytes ) noexcept;
      void                    Complement( void ) noexcept;
      void                    Copy( __in CBitArray const& source ) noexcept;
      void                    Copy( __in std::vector<uint8_t> const& source ) noexcept;
      void                    Copy( __in std::vector<uint32_t> const& source ) noexcept;
      void                    CopyTo( __inout std::vector<uint8_t>& destination ) noexcept;
      void                    CopyTo( __inout std::vector<uint32_t>& destination ) noexcept;
      __checkReturn bool      EnumerateOnes( __inout std::size_t& enumerator ) const noexcept;
      __checkReturn bool      EnumerateZeroes( __inout std::size_t& enumerator ) const noexcept;
      __checkReturn bool      Find( __in CBitArray const& value, __inout std::size_t& found_at, __in std::size_t const starting_at = 0 ) const noexcept;
      __checkReturn bool      Find( __in uint32_t const value, __in std::size_t const number_of_bits_in_value, __inout std::size_t& found_at_index, __in std::size_t const starting_at = 0 ) const noexcept;
      void                    FreeExtra( void ) noexcept;
      
      inline __checkReturn uint32_t GetAt( __in std::size_t const bit_location ) const noexcept
      {
         std::size_t const size = GetSize();

         ASSERT( bit_location < size );

         if ( bit_location >= size )
         {
            return( 0 );
         }

         std::size_t array_index = 0;

         uint32_t bit_number = 0;

         if ( m_GetElementIndexOfBitLocation( bit_location, array_index, bit_number ) == false )
         {
            ASSERT( FALSE );
            return( 0 );
         }

         uint32_t const temp_bits = m_Bits.at( array_index ); // Safe to NOT check out of bounds because m_Bits.GetAt does that (code review)

         if ( _bittest( reinterpret_cast<LONG const *>(&temp_bits), bit_number ) == 1 )
         {
            return( 1 );
         }

         return( 0 );
      }

      __checkReturn bool      GetNextOne( __inout std::size_t& enumerator ) const noexcept;
      __checkReturn bool      GetNextZero( __inout std::size_t& enumerator ) const noexcept;
      __checkReturn std::size_t GetNumberOfOnes( void ) const noexcept;
      __checkReturn std::size_t GetNumberOfZeroes( void ) const noexcept;
      __checkReturn std::size_t GetSize( void ) const noexcept;
      __checkReturn std::size_t GetUpperBound( void ) const noexcept;
      __checkReturn uint32_t    GetValue( __in std::size_t const index, __in std::size_t const length ) const noexcept;
      void                    InsertAt( __in std::size_t const index, __in uint32_t const value ) noexcept;
      __checkReturn CBitArray Left( __in std::size_t const number_of_bits ) const noexcept;
      void                    LeftTrim( __in std::size_t const number_of_bits ) noexcept;
      __checkReturn CBitArray Mid( __in std::size_t const starting_at_index, __in std::size_t const number_of_bits ) const noexcept;

      inline void RemoveAll( void ) noexcept
      {
         m_Bits.clear();
         m_TotalNumberOfBits = 0;
         m_IndexOfFirstBit   = 0;
      }

      void                    RemoveAt( __in std::size_t const array_index ) noexcept;
      __checkReturn CBitArray Right( __in std::size_t const number_of_bits ) const noexcept;
      void                    RightTrim( __in std::size_t const number_of_bits ) noexcept;
      void      SetAll( __in uint32_t const value ) noexcept;
      void      SetAt( __in std::size_t const array_index, __in uint32_t const value ) noexcept;

      /// <summary>Sets the bits to 1 in the given range.</summary>
      void      SetRange( __in std::size_t const array_index, __in std::size_t const length ) noexcept;

      /// <summary>Sets the bits to 0 in the given range.</summary>
      void      ClearRange( __in std::size_t const array_index, __in std::size_t const length ) noexcept;
      void      SetSize( __in std::size_t const number_of_bits ) noexcept;
      void      SetValue( __in std::size_t const array_index, __in uint32_t const value, __in uint32_t const length ) noexcept;

      // Operators

      inline __checkReturn CBitArray& operator =  ( __in CBitArray const&  source ) noexcept { Copy( source );   return( *this ); }
      inline __checkReturn CBitArray& operator =  ( __in std::vector<uint8_t> const& source ) noexcept { Copy( source );   return( *this ); }
      inline __checkReturn CBitArray& operator += ( __in CBitArray const&  source ) noexcept { Append( source ); return( *this ); }
      inline __checkReturn CBitArray& operator += ( __in std::vector<uint8_t> const& source ) noexcept { Append( source ); return( *this ); }
      inline __checkReturn uint32_t      operator [] ( __in std::size_t index ) const noexcept       { return( GetAt( index ) );          }

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      void Dump( CDumpContext& dump_context ) const;
      void SetDumpOptions( int number_of_bits_per_group, int number_of_groups_per_line ) noexcept;

#endif // _DEBUG
};

#if ! defined( _DEBUG )

inline __checkReturn bool CBitArray::m_GetElementIndexOfBitLocation( __in std::size_t const bit_location, __out std::size_t& index, __out uint32_t& bit_number ) const noexcept
{
   // index = ( bit_location + m_IndexOfFirstBit ) / m_SizeOfBitRepresentation;

   // This formula is optimized for m_SizeOfBitRepresentation == 32
   index = ( bit_location + m_IndexOfFirstBit ) >> 5;

   bit_number = (uint32_t) ( MostSignificantBitLocation() - ( ( bit_location + m_IndexOfFirstBit ) % SizeOfBitRepresentation() ) );
   return( true );
}

#endif // _DEBUG

#endif // BIT_ARRAY_CLASS_HEADER
