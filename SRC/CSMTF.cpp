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
** $Workfile: csmtf.cpp $
** $Revision: 22 $
** $Modtime: 6/26/01 10:51a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

/*
** CUINT64
*/

// Construction

Win32FoundationClasses::CUINT64::CUINT64() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CUINT64::CUINT64( _In_ Win32FoundationClasses::CUINT64 const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

Win32FoundationClasses::CUINT64::CUINT64( _In_ SIXTY_FOUR_BIT_UNSIGNED_INTEGER const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Empty();
   Copy( source );
}

Win32FoundationClasses::CUINT64::~CUINT64() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

// Methods

void Win32FoundationClasses::CUINT64::Copy(_In_ Win32FoundationClasses::CUINT64 const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( static_cast<SIXTY_FOUR_BIT_UNSIGNED_INTEGER const *>(&source) );
}

void Win32FoundationClasses::CUINT64::Copy(_In_ SIXTY_FOUR_BIT_UNSIGNED_INTEGER const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( &source );
}

void Win32FoundationClasses::CUINT64::Copy(_In_ SIXTY_FOUR_BIT_UNSIGNED_INTEGER const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      lsw = source->lsw;
      msw = source->msw;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CUINT64::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CUINT64 at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   lsw = " ) << lsw << TEXT( "\n" );
   dump_context << TEXT( "   msw = " ) << msw << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CUINT64::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   lsw = 0;
   msw = 0;
}

// Operators

_Check_return_ Win32FoundationClasses::CUINT64& Win32FoundationClasses::CUINT64::operator=( _In_ Win32FoundationClasses::CUINT64 const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

_Check_return_ Win32FoundationClasses::CUINT64& Win32FoundationClasses::CUINT64::operator=( _In_ SIXTY_FOUR_BIT_UNSIGNED_INTEGER const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** CMTFDateTime
*/

// Construction

Win32FoundationClasses::CMTFDateTime::CMTFDateTime() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CMTFDateTime::CMTFDateTime( _In_ Win32FoundationClasses::CMTFDateTime const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

Win32FoundationClasses::CMTFDateTime::CMTFDateTime( _In_ MTF_DATE_TIME const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Empty();
   Copy( source );
}

Win32FoundationClasses::CMTFDateTime::CMTFDateTime( _In_ MTF_DATE_TIME const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( &source );
}

Win32FoundationClasses::CMTFDateTime::CMTFDateTime( _In_ Win32FoundationClasses::CTime const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

Win32FoundationClasses::CMTFDateTime::~CMTFDateTime() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

// Methods

void Win32FoundationClasses::CMTFDateTime::Convert( _Out_ Win32FoundationClasses::CTime& destination ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   int year    = 0;
   int month   = 0;
   int day     = 0;
   int hours   = 0;
   int minutes = 0;
   int seconds = 0;

   uint8_t temp_byte = 0;

   /*
   ** Compressed date structure for storing dates in minimal space on tape
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   */

   /*
   ** Year
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   ** ^^^^^^^^  ^^^^^^
   */

   temp_byte = dt_field[ 1 ];

   bit_clear( temp_byte, 0 );
   bit_clear( temp_byte, 1 );

   year = MAKEWORD( temp_byte, dt_field[ 0 ] );

   year >>= 2;

   /*
   ** Month
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   **                 ^^  ^^
   */

   temp_byte = (BYTE) ( dt_field[ 1 ] % 4 );

   temp_byte <<= 2;

   month = temp_byte + ( dt_field[ 2 ] >> 6 );

   /*
   ** Day
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   **                       ^^^^^
   */

   temp_byte = dt_field[ 2 ];

   bit_clear( temp_byte, 7 );
   bit_clear( temp_byte, 6 );

   day = temp_byte >> 1;

   /*
   ** Hours
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   **                            ^  ^^^^
   */

   temp_byte = dt_field[ 3 ];

   hours = temp_byte >> 4;

   if ( bit_test( dt_field[ 2 ], 0 ) == 1 )
   {
      hours += 16;
   }

   /*
   ** Minutes
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   **                                   ^^^^  ^^
   */

   minutes = dt_field[ 4 ] >> 6;

   temp_byte = (BYTE) ( dt_field[ 3 ] % 16 );

   minutes += ( temp_byte << 2 );

   /*
   ** Seconds
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   **                                           ^^^^^^
   */

   seconds = dt_field[ 4 ];

   bit_clear( seconds, 7 );
   bit_clear( seconds, 6 );

   destination = CTime( year, month, day, hours, minutes, seconds );
}

void Win32FoundationClasses::CMTFDateTime::Copy( _In_ Win32FoundationClasses::CTime const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   uint8_t byte_0 = 0;
   uint8_t byte_1 = 0;
   uint8_t byte_2 = 0;
   uint8_t byte_3 = 0;
   uint8_t byte_4 = 0;

   uint16_t temp_word = 0;

   /*
   ** Compressed date structure for storing dates in minimal space on tape
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   */

   /*
   ** Year
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   ** ^^^^^^^^  ^^^^^^
   */

   temp_word = (uint16_t) source.GetYear();

   temp_word <<= 2;

   byte_0 = HIBYTE( temp_word );
   byte_1 = LOBYTE( temp_word );

   /*
   ** Month
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   **                 ^^  ^^
   */

   uint32_t temp_dword = static_cast<uint32_t>(source.GetMonth());

   if ( _bittest( reinterpret_cast<LONG const *>(&temp_dword), 3 ) == 1 )
   {
      bit_set( byte_1, 1 );
   }
   else
   {
      bit_clear( byte_1, 1 );
   }

   if ( _bittest(reinterpret_cast<LONG const *>(&temp_dword), 2 ) == 1 )
   {
      bit_set( byte_1, 0 );
   }
   else
   {
      bit_clear( byte_1, 0 );
   }

   _bittestandreset( (LONG *) &temp_dword, 3 );
   _bittestandreset( (LONG *) &temp_dword, 2 );

   byte_2 = (uint8_t) temp_dword;

   byte_2 <<= 6;

   /*
   ** Day
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   **                       ^^^^^
   */

   temp_word = (uint16_t) source.GetDay();

   temp_word <<= 1;

   byte_2 = (uint8_t) ( byte_2 + temp_word );

   /*
   ** Hours
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   **                            ^  ^^^^
   */

   temp_dword = static_cast<uint32_t>(source.GetHour());

   if ( _bittest(reinterpret_cast<LONG const *>(&temp_dword), 4 ) == 1 )
   {
      bit_set( byte_2, 0 );
   }
   else
   {
      bit_clear( byte_2, 1 );
   }

   _bittestandreset( (LONG *) &temp_dword, 4 );

   byte_3 = (uint8_t) temp_dword;

   byte_3 <<= 4;

   /*
   ** Minutes
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   **                                   ^^^^  ^^
   */

   temp_dword = static_cast<uint32_t>(source.GetMinute());

   if ( _bittest(reinterpret_cast<LONG const *>(&temp_dword), 5 ) == 1 )
   {
      bit_set( byte_3, 3 );
   }
   else
   {
      bit_clear( byte_3, 3 );
   }

   if ( _bittest(reinterpret_cast<LONG const *>(&temp_dword), 4 ) == 1 )
   {
      bit_set( byte_3, 2 );
   }
   else
   {
      bit_clear( byte_3, 2 );
   }

   if ( _bittest(reinterpret_cast<LONG const *>(&temp_dword), 3 ) == 1 )
   {
      bit_set( byte_3, 1 );
   }
   else
   {
      bit_clear( byte_3, 1 );
   }

   if ( _bittest(reinterpret_cast<LONG const *>(&temp_dword), 2 ) == 1 )
   {
      bit_set( byte_3, 0 );
   }
   else
   {
      bit_clear( byte_3, 0 );
   }

   _bittestandreset(reinterpret_cast<LONG *>(&temp_dword), 2 );
   _bittestandreset(reinterpret_cast<LONG *>(&temp_dword), 3 );
   _bittestandreset(reinterpret_cast<LONG *>(&temp_dword), 4 );
   _bittestandreset(reinterpret_cast<LONG *>(&temp_dword), 5 );

   byte_4 = static_cast<uint8_t>(temp_dword);

   byte_4 <<= 6;

   /*
   ** Seconds
   **  BYTE 0    BYTE 1    BYTE 2    BYTE 3    BYTE 4
   ** 76543210  76543210  76543210  76543210  76543210
   ** yyyyyyyy  yyyyyymm  mmdddddh  hhhhmmmm  mmssssss
   **                                           ^^^^^^
   */

   temp_word = static_cast<uint16_t>(source.GetSecond());

   byte_4 = static_cast<uint8_t>( byte_4 + temp_word );

   // Finally, save the data

   dt_field[ 0 ] = byte_0;
   dt_field[ 1 ] = byte_1;
   dt_field[ 2 ] = byte_2;
   dt_field[ 3 ] = byte_3;
   dt_field[ 4 ] = byte_4;
}

void Win32FoundationClasses::CMTFDateTime::Copy(_In_ Win32FoundationClasses::CMTFDateTime const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( static_cast<MTF_DATE_TIME const *>(&source) );
}

void Win32FoundationClasses::CMTFDateTime::Copy(_In_ MTF_DATE_TIME const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      for ( auto const dt_field_index : Range(sizeof( dt_field )) )
      {
         dt_field[ dt_field_index ] = source->dt_field[ dt_field_index ];
      }
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CMTFDateTime::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CMTFDateTime at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   dt_field[ 0 ] = " ) << dt_field[ 0 ] << TEXT( "\n" );
   dump_context << TEXT( "   dt_field[ 1 ] = " ) << dt_field[ 1 ] << TEXT( "\n" );
   dump_context << TEXT( "   dt_field[ 2 ] = " ) << dt_field[ 2 ] << TEXT( "\n" );
   dump_context << TEXT( "   dt_field[ 3 ] = " ) << dt_field[ 3 ] << TEXT( "\n" );
   dump_context << TEXT( "   dt_field[ 4 ] = " ) << dt_field[ 4 ] << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CMTFDateTime::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   dt_field[ 0 ] = 0;
   dt_field[ 1 ] = 0;
   dt_field[ 2 ] = 0;
   dt_field[ 3 ] = 0;
   dt_field[ 4 ] = 0;
}

// Operators

Win32FoundationClasses::CMTFDateTime& Win32FoundationClasses::CMTFDateTime::operator=( _In_ Win32FoundationClasses::CMTFDateTime const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CMTFDateTime& Win32FoundationClasses::CMTFDateTime::operator=( _In_ MTF_DATE_TIME const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CMTFDateTime& Win32FoundationClasses::CMTFDateTime::operator=( _In_ Win32FoundationClasses::CTime const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** CMTFTapeAddress
*/

Win32FoundationClasses::CMTFTapeAddress::CMTFTapeAddress()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CMTFTapeAddress::CMTFTapeAddress( _In_ Win32FoundationClasses::CMTFTapeAddress const& source )
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

Win32FoundationClasses::CMTFTapeAddress::CMTFTapeAddress( _In_ MTF_TAPE_ADDRESS const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Empty();
   Copy( source );
}

Win32FoundationClasses::CMTFTapeAddress::~CMTFTapeAddress()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

void Win32FoundationClasses::CMTFTapeAddress::Copy(_In_ Win32FoundationClasses::CMTFTapeAddress const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( static_cast<MTF_TAPE_ADDRESS const *>(&source) );
}

void Win32FoundationClasses::CMTFTapeAddress::Copy(_In_ MTF_TAPE_ADDRESS const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      elmdata_size = source->elmdata_size;
      data_offset  = source->data_offset;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CMTFTapeAddress::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CMTFTapeAddress at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   elmdata_size = " ) << elmdata_size << TEXT( "\n" );
   dump_context << TEXT( "   data_offset  = " ) << data_offset  << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CMTFTapeAddress::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   elmdata_size = 0;
   data_offset  = 0;
}

/*
** CMTFDataBlockHeader
*/

Win32FoundationClasses::CMTFDataBlockHeader::CMTFDataBlockHeader()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CMTFDataBlockHeader::CMTFDataBlockHeader( _In_ Win32FoundationClasses::CMTFDataBlockHeader const& source )
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

Win32FoundationClasses::CMTFDataBlockHeader::CMTFDataBlockHeader( _In_ MTF_DB_HDR const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Empty();
   Copy( source );
}

Win32FoundationClasses::CMTFDataBlockHeader::~CMTFDataBlockHeader()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

void Win32FoundationClasses::CMTFDataBlockHeader::Copy(_In_ Win32FoundationClasses::CMTFDataBlockHeader const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( static_cast<MTF_DB_HDR const *>(&source) );
}

void Win32FoundationClasses::CMTFDataBlockHeader::Copy(_In_ MTF_DB_HDR const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      block_type[ 0 ] = source->block_type[ 0 ];
      block_type[ 1 ] = source->block_type[ 1 ];
      block_type[ 2 ] = source->block_type[ 2 ];
      block_type[ 3 ] = source->block_type[ 3 ];

      block_attribs      = source->block_attribs;
      offset_to_data     = source->offset_to_data;
      machine_os_id      = source->machine_os_id;
      machine_os_version = source->machine_os_version;

      displayable_size.lsw = source->displayable_size.lsw;
      displayable_size.msw = source->displayable_size.msw;

      logical_block_address.lsw = source->logical_block_address.lsw;
      logical_block_address.msw = source->logical_block_address.msw;

      session_id.lsw = source->session_id.lsw;
      session_id.msw = source->session_id.msw;

      control_block_id = source->control_block_id;
   
      string_storage.elmdata_size = source->string_storage.elmdata_size;
      string_storage.data_offset  = source->string_storage.data_offset;

      os_specific_data.elmdata_size = source->os_specific_data.elmdata_size;
      os_specific_data.data_offset  = source->os_specific_data.data_offset;

      string_type = source->string_type;
      reserved    = source->reserved;
      hdr_chksm   = source->hdr_chksm;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CMTFDataBlockHeader::Dump( CDumpContext& dump_context ) const
{
   TCHAR block_type_string[ 5 ];
   CMTFTapeAddress tape_address;
   CUINT64 temp_uint;

   block_type_string[ 0 ] = block_type[ 0 ];
   block_type_string[ 1 ] = block_type[ 1 ];
   block_type_string[ 2 ] = block_type[ 2 ];
   block_type_string[ 3 ] = block_type[ 3 ];
   block_type_string[ 4 ] = 0x00;

   dump_context << TEXT( " a CMTFDataBlockHeader at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   block_type[4]      = " ) << block_type_string  << TEXT( "\n" );
   dump_context << TEXT( "   block_attribs      = " ) << block_attribs      << TEXT( "\n" );
   dump_context << TEXT( "   offset_to_data     = " ) << offset_to_data     << TEXT( "\n" );
   dump_context << TEXT( "   machine_os_id      = " ) << machine_os_id      << TEXT( "\n" );
   dump_context << TEXT( "   machine_os_version = " ) << machine_os_version << TEXT( "\n" );

   dump_context << TEXT( "   displayable_size is" );
   temp_uint.Copy( &displayable_size );
   temp_uint.Dump( dump_context );

   dump_context << TEXT( "   logical_block_address is" );
   temp_uint.Copy( &logical_block_address );
   temp_uint.Dump( dump_context );

   dump_context << TEXT( "   session_id is" );
   temp_uint.Copy( &session_id );
   temp_uint.Dump( dump_context );
   dump_context << TEXT( "   control_block_id = " ) << control_block_id << TEXT( "\n" );
   dump_context << TEXT( "   string_storage is" );
   tape_address.Copy( &string_storage );
   tape_address.Dump( dump_context );
   
   dump_context << TEXT( "   os_specific_data is" );
   tape_address.Copy( &string_storage );
   tape_address.Dump( dump_context );

   dump_context << TEXT( "   string_type = " ) << string_type << TEXT( "\n" );
   dump_context << TEXT( "   reserved    = " ) << reserved    << TEXT( "\n" );
   dump_context << TEXT( "   hdr_chksm   = " ) << hdr_chksm   << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CMTFDataBlockHeader::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   block_type[ 0 ] = 0;
   block_type[ 1 ] = 0;
   block_type[ 2 ] = 0;
   block_type[ 3 ] = 0;

   block_attribs      = 0;
   offset_to_data     = 0;
   machine_os_id      = 0;
   machine_os_version = 0;

   displayable_size.lsw = 0;
   displayable_size.msw = 0;

   logical_block_address.lsw = 0;
   logical_block_address.msw = 0;

   session_id.lsw = 0;
   session_id.msw = 0;

   control_block_id = 0;
   
   string_storage.elmdata_size = 0;
   string_storage.data_offset  = 0;

   os_specific_data.elmdata_size = 0;
   os_specific_data.data_offset  = 0;

   string_type = 0;
   reserved    = 0;
   hdr_chksm   = 0;
}

/*
** CMTFTape
*/

Win32FoundationClasses::CMTFTape::CMTFTape()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CMTFTape::CMTFTape( _In_ Win32FoundationClasses::CMTFTape const& source )
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

Win32FoundationClasses::CMTFTape::CMTFTape( _In_ MTF_TAPE const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Empty();
   Copy( source );
}

Win32FoundationClasses::CMTFTape::~CMTFTape()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

void Win32FoundationClasses::CMTFTape::Copy(_In_ Win32FoundationClasses::CMTFTape const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( static_cast<MTF_TAPE const *>(&source) );
}

void Win32FoundationClasses::CMTFTape::Copy(_In_ MTF_TAPE const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      block_header.block_type[ 0 ] = source->block_header.block_type[ 0 ];
      block_header.block_type[ 1 ] = source->block_header.block_type[ 1 ];
      block_header.block_type[ 2 ] = source->block_header.block_type[ 2 ];
      block_header.block_type[ 3 ] = source->block_header.block_type[ 3 ];

      block_header.block_attribs      = source->block_header.block_attribs;
      block_header.offset_to_data     = source->block_header.offset_to_data;
      block_header.machine_os_id      = source->block_header.machine_os_id;
      block_header.machine_os_version = source->block_header.machine_os_version;

      block_header.displayable_size.lsw = source->block_header.displayable_size.lsw;
      block_header.displayable_size.msw = source->block_header.displayable_size.msw;

      block_header.logical_block_address.lsw = source->block_header.logical_block_address.lsw;
      block_header.logical_block_address.msw = source->block_header.logical_block_address.msw;

      block_header.session_id.lsw = source->block_header.session_id.lsw;
      block_header.session_id.msw = source->block_header.session_id.msw;

      block_header.control_block_id = source->block_header.control_block_id;
   
      block_header.string_storage.elmdata_size = source->block_header.string_storage.elmdata_size;
      block_header.string_storage.data_offset  = source->block_header.string_storage.data_offset;

      block_header.os_specific_data.elmdata_size = source->block_header.os_specific_data.elmdata_size;
      block_header.os_specific_data.data_offset  = source->block_header.os_specific_data.data_offset;

      block_header.string_type = source->block_header.string_type;
      block_header.reserved    = source->block_header.reserved;
      block_header.hdr_chksm   = source->block_header.hdr_chksm;

      tape_id_number            = source->tape_id_number;
      tape_attributes           = source->tape_attributes;
      tape_seq_number           = source->tape_seq_number;
      password_encryption_algor = source->password_encryption_algor;
      ecc_alg                   = source->ecc_alg;
      otc_type                  = source->otc_type;

      tape_name.elmdata_size = source->tape_name.elmdata_size;
      tape_name.data_offset  = source->tape_name.data_offset;

      tape_description.elmdata_size = source->tape_description.elmdata_size;
      tape_description.data_offset  = source->tape_description.data_offset;

      tape_password.elmdata_size = source->tape_password.elmdata_size;
      tape_password.data_offset  = source->tape_password.data_offset;

      software_name.elmdata_size = source->software_name.elmdata_size;
      software_name.data_offset  = source->software_name.data_offset;

      logical_block_size = source->logical_block_size;
      software_vendor_id = source->software_vendor_id;
   
      tape_date.dt_field[ 0 ] = source->tape_date.dt_field[ 0 ];
      tape_date.dt_field[ 1 ] = source->tape_date.dt_field[ 1 ];
      tape_date.dt_field[ 2 ] = source->tape_date.dt_field[ 2 ];
      tape_date.dt_field[ 3 ] = source->tape_date.dt_field[ 3 ];
      tape_date.dt_field[ 4 ] = source->tape_date.dt_field[ 4 ];

      tape_format_version_major = source->tape_format_version_major;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CMTFTape::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CMTFDataBlockHeader at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   CMTFDataBlockHeader header( &block_header );
   dump_context << TEXT( "   block_header is" );
   header.Dump( dump_context );
   dump_context << TEXT( "   tape_id_number                = " ) << tape_id_number                << TEXT( "\n" );
   dump_context << TEXT( "   tape_attributes               = " ) << tape_attributes               << TEXT( "\n" );
   dump_context << TEXT( "   tape_seq_number               = " ) << tape_seq_number               << TEXT( "\n" );
   dump_context << TEXT( "   password_encryption_algor     = " ) << password_encryption_algor     << TEXT( "\n" );
   dump_context << TEXT( "   ecc_alg                       = " ) << ecc_alg                       << TEXT( "\n" );
   dump_context << TEXT( "   otc_type                      = " ) << otc_type                      << TEXT( "\n" );
   dump_context << TEXT( "   tape_name.elmdata_size        = " ) << tape_name.elmdata_size        << TEXT( "\n" );
   dump_context << TEXT( "   tape_name.data_offset         = " ) << tape_name.data_offset         << TEXT( "\n" );
   dump_context << TEXT( "   tape_description.elmdata_size = " ) << tape_description.elmdata_size << TEXT( "\n" );
   dump_context << TEXT( "   tape_description.data_offset  = " ) << tape_description.data_offset  << TEXT( "\n" );
   dump_context << TEXT( "   tape_password.elmdata_size    = " ) << tape_password.elmdata_size    << TEXT( "\n" );
   dump_context << TEXT( "   tape_password.data_offset     = " ) << tape_password.data_offset     << TEXT( "\n" );
   dump_context << TEXT( "   software_name.elmdata_size    = " ) << software_name.elmdata_size    << TEXT( "\n" );
   dump_context << TEXT( "   software_name.data_offset     = " ) << software_name.data_offset     << TEXT( "\n" );
   dump_context << TEXT( "   logical_block_size            = " ) << logical_block_size            << TEXT( "\n" );
   dump_context << TEXT( "   software_vendor_id            = " ) << software_vendor_id            << TEXT( "\n" );
   dump_context << TEXT( "   tape_date.dt_field[ 0 ]       = " ) << tape_date.dt_field[ 0 ]       << TEXT( "\n" );
   dump_context << TEXT( "   tape_date.dt_field[ 1 ]       = " ) << tape_date.dt_field[ 1 ]       << TEXT( "\n" );
   dump_context << TEXT( "   tape_date.dt_field[ 2 ]       = " ) << tape_date.dt_field[ 2 ]       << TEXT( "\n" );
   dump_context << TEXT( "   tape_date.dt_field[ 3 ]       = " ) << tape_date.dt_field[ 3 ]       << TEXT( "\n" );
   dump_context << TEXT( "   tape_date.dt_field[ 4 ]       = " ) << tape_date.dt_field[ 4 ]       << TEXT( "\n" );
   dump_context << TEXT( "   tape_format_version_major     = " ) << tape_format_version_major     << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CMTFTape::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   block_header.block_type[ 0 ] = 0;
   block_header.block_type[ 1 ] = 0;
   block_header.block_type[ 2 ] = 0;
   block_header.block_type[ 3 ] = 0;

   block_header.block_attribs      = 0;
   block_header.offset_to_data     = 0;
   block_header.machine_os_id      = 0;
   block_header.machine_os_version = 0;

   block_header.displayable_size.lsw = 0;
   block_header.displayable_size.msw = 0;

   block_header.logical_block_address.lsw = 0;
   block_header.logical_block_address.msw = 0;

   block_header.session_id.lsw = 0;
   block_header.session_id.msw = 0;

   block_header.control_block_id = 0;
   
   block_header.string_storage.elmdata_size = 0;
   block_header.string_storage.data_offset  = 0;

   block_header.os_specific_data.elmdata_size = 0;
   block_header.os_specific_data.data_offset  = 0;
   block_header.string_type = 0;
   block_header.reserved    = 0;
   block_header.hdr_chksm   = 0;

   tape_id_number            = 0;
   tape_attributes           = 0;
   tape_seq_number           = 0;
   password_encryption_algor = 0;
   ecc_alg                   = 0;
   otc_type                  = 0;

   tape_name.elmdata_size = 0;
   tape_name.data_offset  = 0;

   tape_description.elmdata_size = 0;
   tape_description.data_offset  = 0;

   tape_password.elmdata_size = 0;
   tape_password.data_offset  = 0;

   software_name.elmdata_size = 0;
   software_name.data_offset  = 0;

   logical_block_size = 0;
   software_vendor_id = 0;
   
   tape_date.dt_field[ 0 ] = 0;
   tape_date.dt_field[ 1 ] = 0;
   tape_date.dt_field[ 2 ] = 0;
   tape_date.dt_field[ 3 ] = 0;
   tape_date.dt_field[ 4 ] = 0;

   tape_format_version_major = 0;
}

// End of source
