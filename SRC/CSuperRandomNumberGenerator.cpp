/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2016, Samuel R. Blackburn
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
** $Workfile: CSuperRandomNumberGenerator.cpp $
** $Revision: 14 $
** $Modtime: 6/26/01 10:51a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

CSuperRandomNumberGenerator::CSuperRandomNumberGenerator( __in DWORD seed )
{
   WFC_VALIDATE_POINTER( this );
   SetSeed( seed );
}

CSuperRandomNumberGenerator::~CSuperRandomNumberGenerator()
{
   WFC_VALIDATE_POINTER( this );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CSuperRandomNumberGenerator::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CSuperRandomNumberGenerator at " ) << (VOID *) this << TEXT( "\n{\n" );

   for ( auto const index : Range(25) )
   {
      dump_context << TEXT( "   m_IntegerSeeds[ " ) << index << TEXT( " ] is " ) << m_IntegerSeeds[ index ] << TEXT( "\n" );
   }

   for (auto const index : Range(25))
   {
      dump_context << TEXT( "   m_NextValue[ " ) << index << TEXT( " ] is " ) << m_NextValue[ index ] << TEXT( "\n" );
   }

   for (auto const index : Range(25))
   {
      dump_context << TEXT( "   m_Seeds[ " ) << index << TEXT( " ] is " ) << m_Seeds[ index ] << TEXT( "\n" );
   }

   dump_context << TEXT( "   m_Carry is     " ) << m_Carry     << TEXT( "\n" );
   dump_context << TEXT( "   m_TwoM12 is    " ) << m_TwoM12    << TEXT( "\n" );
   dump_context << TEXT( "   m_TwoM24 is    " ) << m_TwoM24    << TEXT( "\n" );
   dump_context << TEXT( "   m_Index_I24 is " ) << m_Index_I24 << TEXT( "\n" );
   dump_context << TEXT( "   m_Index_J24 is " ) << m_Index_J24 << TEXT( "\n" );
   dump_context << TEXT( "   m_Index24 is   " ) << m_Index24   << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ DWORD CSuperRandomNumberGenerator::GetInteger( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   double value = GetValue();

   value *= static_cast< double >( 0xFFFFFFFF );

   // Now, the last 8 bits are probably going to be 1
   // To get rid of this flaw in the stream we will
   // do a quick and dirty hash of the bytes that make 
   // up the double value

   auto buffer = reinterpret_cast<BYTE const *>( &value );

   DWORD hash_value = static_cast< DWORD >( value );
   DWORD temp_value = 0;

   for ( auto const index : Range(sizeof( value )) )
   {
      hash_value = ( hash_value << 4 ) + buffer[ index ];

      temp_value = hash_value & 0xF0000000L;

      if ( temp_value )
      {
         hash_value ^= temp_value >> 24;
      }

      hash_value &= ~temp_value;
   }

   DWORD return_value = static_cast< DWORD >( value );

   return_value += ( ( hash_value >> 5 ) % 256 );

   return( return_value );
}

_Check_return_ double CSuperRandomNumberGenerator::GetValue( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   double temporary_value = 0.0;
   double return_value    = 0.0;

   temporary_value = m_Seeds[ m_Index_J24 ] - m_Seeds[ m_Index_I24 ] - m_Carry;

   if ( temporary_value < (double) 0.0 )
   {
      temporary_value += 1.0;
      m_Carry = m_TwoM24; 
   }
   else
   {
      m_Carry = (double) 0.0;
   }

   m_Seeds[ m_Index_I24 ] = temporary_value;

   m_Index_I24 = m_NextValue[ m_Index_I24 ];
   m_Index_J24 = m_NextValue[ m_Index_J24 ];

   return_value = temporary_value;

   if ( temporary_value < m_TwoM12 )
   {
      return_value += m_TwoM24 * m_Seeds[ m_Index_J24 ];
   }

/* zero is forbidden in case user wants logarithms */

   if ( return_value == 0.0 )
   {
      return_value = m_TwoM24 * m_TwoM24;
   }

   m_Index24++;

   if ( m_Index24 == 24 )
   {
      m_Index24 = 0;

      for( auto const i : Range(NUMBER_OF_VALUES_TO_SKIP + 1, 1 ) )
      {
         temporary_value = m_Seeds[ m_Index_J24 ] - m_Seeds[ m_Index_I24 ] - m_Carry;

         if ( temporary_value < (float) 0.0 )
         {
            temporary_value += 1.0;
            m_Carry = m_TwoM24; 
         }
         else
         {
            m_Carry = (float) 0.0;
         }

         m_Seeds[ m_Index_I24 ] = temporary_value;

         m_Index_I24 = m_NextValue[ m_Index_I24 ];
         m_Index_J24 = m_NextValue[ m_Index_J24 ];
      }
   }

   return( return_value );
}

void CSuperRandomNumberGenerator::m_Initialize( void )
{
   WFC_VALIDATE_POINTER( this );
   m_Index_I24 = 24;
   m_Index_J24 = 10;
   m_Index24   = 0;
}

void CSuperRandomNumberGenerator::SetSeed( __in DWORD jseed ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( jseed == 0 )
   {
      CRandomNumberGenerator random;

      jseed = random;
   }

   m_TwoM24 = (double) 1.0;

   for( auto const i : Range( 25, 1 ) )
   {
      m_TwoM24 *= (double) 0.5;
      int k = jseed / 53668;
      jseed = 40014 * ( jseed - ( k * 53668 ) ) - ( k * 12211 );

      m_IntegerSeeds[ i ] = jseed % TWENTY_FOUR_BIT_VALUE;
   }

   m_TwoM12 = m_TwoM24 * (double) 4096.0;

   for( auto const i : Range( 25, 1 ) )
   {
      m_Seeds[ i ] = static_cast< double >( m_IntegerSeeds[ i ] * m_TwoM24 );
      m_NextValue[ i ] = i - 1;
   }

   m_NextValue[ 1 ] = 24;
   m_Index_I24 = 24;
   m_Index_J24 = 10;
   m_Carry = (double) 0.0;

   if ( m_Seeds[ 24 ] == 0.0 )
   {
      m_Carry = m_TwoM24;
   }
}

// End of source
