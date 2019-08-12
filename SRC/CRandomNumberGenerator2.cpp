/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2017, Samuel R. Blackburn
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
** $Workfile: CRandomNumberGenerator2.cpp $
** $Revision: 12 $
** $Modtime: 6/26/01 10:50a $
** $Reuse Tracing Code: 1 $
*/

// Algorithm provided by Yehuda Hahn (ygh@cfsnet.com)
// Source adapted from http://www.math.keio.ac.jp/matumoto/cokus.c

// This is the ``Mersenne Twister'' random number generator MT19937, which
// generates pseudorandom integers uniformly distributed in 0..(2^32 - 1)
// starting from any odd seed in 0..(2^32 - 1).  This version is a recode
// by Shawn Cokus (Cokus@math.washington.edu) on March 8, 1998 of a version by
// Takuji Nishimura (who had suggestions from Topher Cooper and Marc Rieffel in
// July-August 1997).
//
// Effectiveness of the recoding (on Goedel2.math.washington.edu, a DEC Alpha
// running OSF/1) using GCC -O3 as a compiler: before recoding: 51.6 sec. to
// generate 300 million random numbers; after recoding: 24.0 sec. for the same
// (i.e., 46.5% of original time), so speed is now about 12.5 million random
// number generations per second on this machine.
//
// According to the URL <http://www.math.keio.ac.jp/~matumoto/emt.html>
// (and paraphrasing a bit in places), the Mersenne Twister is ``designed
// with consideration of the flaws of various existing generators,'' has
// a period of 2^19937 - 1, gives a sequence that is 623-dimensionally
// equidistributed, and ``has passed many stringent tests, including the
// die-hard test of G. Marsaglia and the load test of P. Hellekalek and
// S. Wegenkittl.''  It is efficient in memory usage (typically using 2506
// to 5012 bytes of static data, depending on data type sizes, and the code
// is quite short as well).  It generates random numbers in batches of 624
// at a time, so the caching and pipelining of modern systems is exploited.
// It is also divide- and mod-free.
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation (either version 2 of the License or, at your
// option, any later version).  This library is distributed in the hope that
// it will be useful, but WITHOUT ANY WARRANTY, without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
// the GNU Library General Public License for more details.  You should have
// received a copy of the GNU Library General Public License along with this
// library; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place, Suite 330, Boston, MA 02111-1307, USA.
//
// The code as Shawn received it included the following notice:
//
//   Copyright (C) 1997 Makoto Matsumoto and Takuji Nishimura.  When
//   you use this, send an e-mail to <matumoto@math.keio.ac.jp> with
//   an appropriate reference to your work.
//
// It would be nice to CC: <Cokus@math.washington.edu> when you write.
//

#include <wfc.h>
#pragma hdrstop

#include <intrin.h>

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

using GET_RANDOM_DWORD_FUNCTION = VOID (*)( DWORD * );

typedef unsigned long uint32;

#define N              (624)                 // length of state vector
#define M              (397)                 // a period parameter
#define K              (0x9908B0DFU)         // a magic constant
#define hiBit(u)       ((u) & 0x80000000U)   // mask all but highest   bit of u
#define loBit(u)       ((u) & 0x00000001U)   // mask all but lowest    bit of u
#define loBits(u)      ((u) & 0x7FFFFFFFU)   // mask     the highest   bit of u
#define mixBits(u, v)  (hiBit(u)|loBits(v))  // move hi bit of u to hi bit of v

_Check_return_ uint32_t CRandomNumberGenerator2::m_LoadMersenneTwister( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

    uint32_t *p0 = m_State;
    uint32_t *p2 = m_State + 2;
    uint32_t *pM = m_State + M;
    uint32_t s0 = 0;
    uint32_t s1 = 0;
    int    j = 0;

    if ( m_Left < (-1) )
    {
       InitializeSeed();
    }

    m_Left = N - 1;
    m_Next = m_State + 1;

    for( s0 = m_State[ 0 ], s1 = m_State[ 1 ], j = N - M + 1; --j; s0 = s1, s1 = *p2++ )
    {
        *p0++ = *pM++ ^ (mixBits(s0, s1) >> 1) ^ (loBit(s1) ? K : 0U);
    }

    for( pM = m_State, j = M; --j; s0 = s1, s1 = *p2++ )
    {
        *p0++ = *pM++ ^ (mixBits(s0, s1) >> 1) ^ (loBit(s1) ? K : 0U);
    }

    s1 = m_State[0], *p0 = *pM ^ (mixBits(s0, s1) >> 1) ^ (loBit(s1) ? K : 0U);
    s1 ^= (s1 >> 11);
    s1 ^= (s1 <<  7) & 0x9D2C5680U;
    s1 ^= (s1 << 15) & 0xEFC60000U;
    return(s1 ^ (s1 >> 18));
 }

CRandomNumberGenerator2::CRandomNumberGenerator2() noexcept
{
   WFC_VALIDATE_POINTER( this );
   ZeroMemory( m_State, sizeof( m_State ) );
   m_Disabled = false;
   m_Left = 0;
   m_Next = nullptr;

   InitializeSeed();
}

CRandomNumberGenerator2::CRandomNumberGenerator2( _In_ uint32_t const new_seed ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ZeroMemory( m_State, sizeof( m_State ) );
   m_Disabled = false;
   m_Left = 0;
   m_Next = nullptr;

   SetSeed( new_seed );
}

CRandomNumberGenerator2::~CRandomNumberGenerator2() noexcept
{
   WFC_VALIDATE_POINTER( this );
}

void CRandomNumberGenerator2::Disable( _In_ bool const disable ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Disabled = ( disable == false ) ? false : true;
}

_Check_return_ bool CRandomNumberGenerator2::Fill( __out_bcount( number_of_bytes ) uint8_t * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( buffer );

   if ( buffer == nullptr )
   {
      return( false );
   }

   std::size_t const number_of_dwords_in_buffer = number_of_bytes / sizeof(uint32_t);

   auto dwords = reinterpret_cast<uint32_t *>(buffer);

   std::size_t array_index = 0;

   while( array_index < number_of_dwords_in_buffer ) // Cannot be converted to a Range loop
   {
      dwords[ array_index ] = GetInteger();
      array_index++;
   }

   array_index *= sizeof(uint32_t);

   while( array_index < number_of_bytes ) // Cannot be converted to a Range loop
   {
      buffer[ array_index ] = static_cast<uint8_t>(GetInteger());
      array_index++;
   }

   return( true );
}

void CRandomNumberGenerator2::InitializeSeed( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Disabled == true)
   {
      SetSeed( m_State[ 0 ] );
      return;
   }

   // We need to seed our algorithm with something random from the operating system

   uint16_t low_word = 0;

   // GetTickCount() returns the number of milliseconds the machine has been turned on

   uint16_t high_word = LOWORD( ::GetTickCount64() );

   uint32_t number_of_sectors_per_cluster = 0;
   uint32_t number_of_bytes_per_sector    = 0;
   uint32_t number_of_free_clusters       = 0;
   uint32_t total_number_of_clusters      = 0;

   FILETIME a = { 0, 0 };
   FILETIME b = { 0, 0 };
   FILETIME c = { 0, 0 };

   uint32_t totaler = ::GetCurrentProcessId();

   totaler <<= 3;

   totaler += (uint32_t) ::GetTickCount64();

   if ( ::GetSystemTimes( &a, &b, &c ) != 0 )
   {
       totaler += a.dwLowDateTime;
       totaler += b.dwLowDateTime;
       totaler += c.dwLowDateTime;
   }

   // Different folks have different amounts of free space on their hard drives

   if ( ::GetDiskFreeSpace( nullptr,
                 (LPDWORD) &number_of_sectors_per_cluster,
                 (LPDWORD) &number_of_bytes_per_sector,
                 (LPDWORD) &number_of_free_clusters,
                 (LPDWORD) &total_number_of_clusters ) != FALSE )
   {
      low_word = LOWORD( number_of_free_clusters );

      // This is a nice number but not nice enough. It won't vary enough
      // from call to call. So, let's scramble it a bit (no pun intented)
      // Anther call to GetTickCount() should do the trick because the
      // GetDiskFreeSpace() will take a varying amount of time.

      low_word  ^= ( (uint16_t) ::GetTickCount64() );

      totaler += number_of_sectors_per_cluster;
      totaler <<= 1;
      totaler += number_of_bytes_per_sector;
      totaler <<= 1;
      totaler += number_of_free_clusters;
      totaler <<= 1;
      totaler += total_number_of_clusters;
   }
   else
   {
      // This is a fail-safe condition. We resort to time because it is kinda
      // random but it is guessable.

      SYSTEMTIME system_time;

      ZeroMemory( &system_time, sizeof( system_time ) );

      ::GetSystemTime( &system_time );

      // The most random part of the system time is the milliseconds then seconds

      BYTE high_byte = 0;
      BYTE low_byte  = 0;

      high_byte = LOBYTE( system_time.wMilliseconds );
      low_byte  = (BYTE) ( ( system_time.wMilliseconds >> 8 ) + system_time.wSecond );

      low_word = MAKEWORD( low_byte, high_byte );
   }

   // 2000-05-26
   // Added GetCurrentThreadId() so two threads won't initialize to the same seed

   uint32_t value_1 = ( MAKELONG( low_word, high_word ) ^ ( ::GetCurrentThreadId() << 11 ) );

   value_1 += totaler;

   LARGE_INTEGER li;

   QueryPerformanceCounter( &li );

   value_1 += li.LowPart;
   value_1 += li.HighPart;

#if 0
   // Now top it off with a random number from the CPU

   uint32_t random_integer = 0;

   while( _rdrand32_step( &random_integer ) == 0 )
   {
      value_1++;
   }

   value_1 ^= random_integer;
#endif

   SetSeed( value_1 );
}

_Check_return_ uint32_t CRandomNumberGenerator2::GetInteger( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   uint32_t return_value = 0;

   if ( --m_Left < 0 )
   {
      return( m_LoadMersenneTwister() );
   }

   return_value  = *m_Next++;
   return_value ^= (return_value >> 11);
   return_value ^= (return_value <<  7) & 0x9D2C5680U;
   return_value ^= (return_value << 15) & 0xEFC60000U;

   return( return_value ^ (return_value >> 18) );
}

_Check_return_ double CRandomNumberGenerator2::GetFloat( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   union
   {
      double return_value;
      unsigned char bytes[ 8 ];
   }
   x;

   x.return_value = GetInteger();

   uint32_t temp_integer = GetInteger();

   // This works for Intel-Endian machines
   x.bytes[ 0 ] ^= HIBYTE( HIWORD( temp_integer ) );
   x.bytes[ 1 ] ^= HIBYTE( LOWORD( temp_integer ) );
   x.bytes[ 2 ] ^= LOBYTE( HIWORD( temp_integer ) );

   // This works for Sun-Endian machines
   //x.bytes[ 7 ] ^= HIBYTE( HIWORD( temp_integer ) );
   //x.bytes[ 6 ] ^= HIBYTE( LOWORD( temp_integer ) );
   //x.bytes[ 5 ] ^= LOBYTE( HIWORD( temp_integer ) );

   return( x.return_value );
}

void CRandomNumberGenerator2::SetSeed( _In_ uint32_t const new_seed ) noexcept
{
   WFC_VALIDATE_POINTER( this );

    //
    // We initialize state[0..(N-1)] via the generator
    //
    //   x_new = (69069 * x_old) mod 2^32
    //
    // from Line 15 of Table 1, p. 106, Sec. 3.3.4 of Knuth's
    // _The Art of Computer Programming_, Volume 2, 3rd ed.
    //
    // Notes (SJC): I do not know what the initial state requirements
    // of the Mersenne Twister are, but it seems this seeding generator
    // could be better.  It achieves the maximum period for its modulus
    // (2^30) iff x_initial is odd (p. 20-21, Sec. 3.2.1.2, Knuth); if
    // x_initial can be even, you have sequences like 0, 0, 0, ...;
    // 2^31, 2^31, 2^31, ...; 2^30, 2^30, 2^30, ...; 2^29, 2^29 + 2^31,
    // 2^29, 2^29 + 2^31, ..., etc. so I force seed to be odd below.
    //
    // Even if x_initial is odd, if x_initial is 1 mod 4 then
    //
    //   the          lowest bit of x is always 1,
    //   the  next-to-lowest bit of x is always 0,
    //   the 2nd-from-lowest bit of x alternates      ... 0 1 0 1 0 1 0 1 ... ,
    //   the 3rd-from-lowest bit of x 4-cycles        ... 0 1 1 0 0 1 1 0 ... ,
    //   the 4th-from-lowest bit of x has the 8-cycle ... 0 0 0 1 1 1 1 0 ... ,
    //    ...
    //
    // and if x_initial is 3 mod 4 then
    //
    //   the          lowest bit of x is always 1,
    //   the  next-to-lowest bit of x is always 1,
    //   the 2nd-from-lowest bit of x alternates      ... 0 1 0 1 0 1 0 1 ... ,
    //   the 3rd-from-lowest bit of x 4-cycles        ... 0 0 1 1 0 0 1 1 ... ,
    //   the 4th-from-lowest bit of x has the 8-cycle ... 0 0 1 1 1 1 0 0 ... ,
    //    ...
    //
    // The generator's potency (min. s>=0 with (69069-1)^s = 0 mod 2^32) is
    // 16, which seems to be alright by p. 25, Sec. 3.2.1.3 of Knuth.  It
    // also does well in the dimension 2..5 spectral tests, but it could be
    // better in dimension 6 (Line 15, Table 1, p. 106, Sec. 3.3.4, Knuth).
    //
    // Note that the random number user does not see the values generated
    // here directly since reloadMT() will always munge them first, so maybe
    // none of all of this matters.  In fact, the seed values made here could
    // even be extra-special desirable if the Mersenne Twister theory says
    // so-- that's why the only change I made is to restrict to odd seeds.
    //

    uint32_t x = (new_seed | 1) & 0xFFFFFFFF;
    uint32_t *s = m_State;
    int    j = 0;

    for(m_Left=0, *s++=x, j=N; --j;
        *s++ = (x*=69069U) & 0xFFFFFFFF);
}

CRandomNumberGenerator2::operator char() noexcept
{
   WFC_VALIDATE_POINTER( this );

   uint32_t random_value = GetInteger();

   char return_value = static_cast< char >( random_value >> 9 );

   return( return_value );
}

CRandomNumberGenerator2::operator unsigned char() noexcept
{
   WFC_VALIDATE_POINTER( this );

   uint32_t random_value = GetInteger();
   
   unsigned char return_value = static_cast< unsigned char >( random_value >> 17 );

   return( return_value );
}

CRandomNumberGenerator2::operator int() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( GetInteger() );
}

CRandomNumberGenerator2::operator unsigned int() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( GetInteger() );
}

CRandomNumberGenerator2::operator short() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( static_cast< short >( GetInteger() ) );
}

CRandomNumberGenerator2::operator unsigned short() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( static_cast< unsigned short >( GetInteger() ) );
}

CRandomNumberGenerator2::operator long() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( static_cast< long >( GetInteger() ) );
}

CRandomNumberGenerator2::operator unsigned long() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( GetInteger() );
}

CRandomNumberGenerator2::operator float() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( static_cast< float >( GetFloat() ) );
}

CRandomNumberGenerator2::operator double() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( GetFloat() );
}

_Check_return_ double CRandomNumberGenerator2::Double(_In_ double const min_value, _In_ double const max_value) noexcept
{
    double minimum_value = min_value;
    double maximum_value = max_value;

    if (min_value == max_value)
    {
        return(min_value);
    }

    if (min_value > max_value)
    {
        minimum_value = max_value;
        maximum_value = min_value;
    }

    double const number_of_values = std::fabs( maximum_value - minimum_value);

    // Now randomly choose a value below number_of_values

    double return_value = fabs( GetFloat() );

    double integer_part = 0.0;
    double const fraction_part = modf(return_value, &integer_part);

    return_value = number_of_values * fraction_part;
    return_value += minimum_value;

    return(return_value);
}

_Check_return_ uint32_t CRandomNumberGenerator2::Uint32( _In_ uint32_t const min_value, _In_ uint32_t const max_value ) noexcept
{
    uint32_t minimum_value = min_value;
    uint32_t maximum_value = max_value;

    if ( min_value == max_value )
    {
        return( min_value );
    }

    if ( min_value > max_value )
    {
        minimum_value = max_value;
        maximum_value = min_value;
    }

    uint32_t const number_of_values = (maximum_value - minimum_value) + 1;

    uint32_t const return_value = ( GetInteger() % number_of_values ) + minimum_value;

    return( return_value );
}

_Check_return_ uint64_t CRandomNumberGenerator2::Uint64( _In_ uint64_t const min_value, _In_ uint64_t const max_value ) noexcept
{
    uint64_t minimum_value = min_value;
    uint64_t maximum_value = max_value;

    if ( min_value == max_value )
    {
        return( min_value );
    }

    if ( min_value > max_value )
    {
        minimum_value = max_value;
        maximum_value = min_value;
    }

    uint64_t const number_of_values = (maximum_value - minimum_value) + 1;

    uint64_t const return_value = ( Integer64() % number_of_values ) + minimum_value;

    return( return_value );
}

_Check_return_ uint64_t CRandomNumberGenerator2::Uint64(__in_ecount(number_of_values) VALUE_RANGE const * values, _In_ std::size_t const number_of_values) noexcept
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER(values);

    if (values == nullptr || number_of_values < 1)
    {
        return(Integer64());
    }

    std::size_t const value_index = Integer64() % number_of_values;

    return(Uint64(values[value_index].low, values[value_index].high));
}

_Check_return_ bool CRandomNumberGenerator2::IsTrue( _In_ double const percentage ) noexcept
{
    if ( percentage >= 100.0 )
    {
        return( true );
    }

    if ( percentage <= 0.0 )
    {
        return( false );
    }

#undef max
    double const number_of_true_values = ( (double) ( (double) percentage / (double) 100.0 ) * (double)std::numeric_limits<std::uint32_t>::max());

    if ( static_cast<uint32_t>(GetInteger()) <= static_cast<uint32_t>(number_of_true_values) )
    {
        return( true );
    }

    return( false );
}

void CRandomNumberGenerator2::Fill( _In_ FILETIME const& min_value, _In_ FILETIME const& max_value, _Out_ FILETIME& destination ) noexcept
{
    ULARGE_INTEGER a;
    ULARGE_INTEGER b;

    a.HighPart = min_value.dwHighDateTime;
    a.LowPart = min_value.dwLowDateTime;

    b.HighPart = max_value.dwHighDateTime;
    b.LowPart = max_value.dwLowDateTime;

    ULARGE_INTEGER return_value;

    return_value.QuadPart = Uint64( a.QuadPart, b.QuadPart );

    destination.dwHighDateTime = return_value.HighPart;
    destination.dwLowDateTime = return_value.LowPart;
}

void CRandomNumberGenerator2::Fill( _In_ uint32_t const min_length, _In_ uint32_t const max_length, _Out_ std::wstring& destination ) noexcept
{
    uint32_t const length = Uint32( min_length, max_length );

    destination.resize( length );

    wchar_t const * alphabet = L" abcdefghijklmnopqrstuvwxyz 01234567890 .! ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    uint32_t const alphabet_length = static_cast<uint32_t>(wcslen( alphabet ));

    for ( auto const loop_index : Range(length) )
    {
        destination.at( loop_index ) = alphabet[ GetInteger() % alphabet_length ];
    }
}

void CRandomNumberGenerator2::Fill( _In_ uint32_t const seed, _Out_ std::wstring& destination ) noexcept // static
{
    uint32_t const length = ( seed / 131282527 ) + 3;

    destination.resize( length );

    wchar_t const * alphabet = L" abcdefghijklmnopqrstuvwxyz 01234567890 .! ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    uint32_t const alphabet_length = static_cast<uint32_t>(wcslen( alphabet ));

    uint32_t loop_index = 0;

    uint32_t new_seed = seed;

    while( loop_index < length ) // Can't be converted to range loop
    {
        destination.at( loop_index ) =  alphabet[ new_seed % alphabet_length ];

        new_seed = _rotl( new_seed, 1 );

        if ( destination.at( 0 ) != ' ' )
        {
           loop_index++;
        }
    }
}

_Check_return_ uint32_t CRandomNumberGenerator2::OneOf( _In_reads_( number_of_values ) uint32_t const * values, _In_ std::size_t const number_of_values ) noexcept
{
    return( values[ GetInteger() % number_of_values ] );
}

_Check_return_ uint64_t CRandomNumberGenerator2::OneOf(_In_reads_( number_of_values ) uint64_t const * values, _In_ std::size_t const number_of_values ) noexcept
{
    return( values[ GetInteger() % number_of_values ] );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CRandomNumberGenerator2</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, Random number generator, source code, Mersenne Prime Twister">
<META name="description" content="This C++ class gives you a really good random number generator based on the Mersenne Prime Twister algorithm.">
</HEAD>

<BODY>

<H1>CRandomNumberGenerator2</H1>
$Revision: 12 $
<HR>

<H2>Description</H2>

This class implements a better random number generator than the C runtime
library. It uses the Mersenne Twister algorithm. Yehuda Hahn
(<A HREF="mailto:ygh@cfsnet.com">ygh@cfsnet.com</A>) wrote the C version
adapted from
<A HREF="http://www.math.keio.ac.jp/matumoto/cokus.c">http://www.math.keio.ac.jp/matumoto/cokus.c</A>
All I did was wrap it in a C++ class.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>DWORD <B><A NAME="GetInteger">GetInteger</A></B>( void )</PRE><DD>
Returns a 32-bit random integer.

<DT><PRE>double <B><A NAME="GetFloat">GetFloat</A></B>( void )</PRE><DD>
Returns a random float between 0 and 1.

<DT><PRE>void <B><A NAME="InitializeSeed">InitializeSeed</A></B>( void )</PRE><DD>
Initializes the seed value for the generator. To increase randomness,
it uses a combination of the number of milliseconds the NT machine has
been on and the number of free clusters on the current drive to seed
the generator.

<DT><PRE>void <B><A NAME="SetSeed">SetSeed</A></B>( DWORD new_seed )</PRE><DD>
Sets the seed to this value. You can use this if you don't like my
method of seeding.

</DL>

<H2>Operators</H2>

These are what makes this class cool. You can use the class as a
base type.

<UL>
<LI><PRE><B>char</B></PRE>
<LI><PRE><B>unsigned char</B></PRE>
<LI><PRE><B>int</B></PRE>
<LI><PRE><B>unsigned int</B></PRE>
<LI><PRE><B>short</B></PRE>
<LI><PRE><B>unsigned short</B></PRE>
<LI><PRE><B>long</B></PRE>
<LI><PRE><B>unsigned long</B></PRE>
<LI><PRE><B>float</B></PRE>
<LI><PRE><B>double</B></PRE>
</UL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

void test_CRandomNumberGenerator( void )
{
   <B>CRandomNumberGenerator2</B> random_number;

   int index = 0;

   while( index &lt; 20 )
   {
      _tprintf( TEXT( &quot;DWORD  random number is %lu\n&quot; ), (DWORD)  random_number );
      _tprintf( TEXT( &quot;double random number is %lf\n&quot; ), (double) random_number );

      index++;
   }
}</CODE></PRE>

<H2>API's Used</H2>

<B>CRandomNumberGenerator2</B> uses the following API's:
<UL>
<LI>GetDiskFreeSpace
<LI>GetSystemTime
<LI>GetTickCount
</UL>

<H2>Randomness Test</H2>

I'm no mathematician. If you are, please have mercy on me when you
read the results of my randomness tests on this generator.<P>

I ran this generator through a few tests that I could think of for
randomness. Here's the results for generating 100,000,000 32-bit integers
(that makes 3,200,000,000 bits).

<TABLE BORDER="0">
<TR><TD>Number of 1-bits</TD><TD>1,600,024,395</TD></TR>
<TR><TD>Number of 0-bits</TD><TD>1,599,975,605</TD></TR>
<TR><TD>Longest series of 1-bits</TD><TD>31</TD></TR>
<TR><TD>Longest series of 0-bits</TD><TD>37</TD></TR>
</TABLE>

Here's a breakdown of the bits in each of the 32 slots that
make up a 32-bit integer. The count is the number of times
the bit at that location had a value of 1.

<TABLE>
<TR><TD>Bit Number</TD><TD>Number of 1's</TD></TR>
<TR><TD>0</TD><TD>50,001,806</TD></TR>
<TR><TD>1</TD><TD>49,996,289</TD></TR>
<TR><TD>2</TD><TD>49,998,465</TD></TR>
<TR><TD>3</TD><TD>49,995,963</TD></TR>
<TR><TD>4</TD><TD>50,008,188</TD></TR>
<TR><TD>5</TD><TD>49,991,291</TD></TR>
<TR><TD>6</TD><TD>50,010,640</TD></TR>
<TR><TD>7</TD><TD>50,004,980</TD></TR>
<TR><TD>8</TD><TD>50,002,946</TD></TR>
<TR><TD>9</TD><TD>49,997,908</TD></TR>
<TR><TD>10</TD><TD>49,993,413</TD></TR>
<TR><TD>11</TD><TD>50,004,827</TD></TR>
<TR><TD>12</TD><TD>49,998,652</TD></TR>
<TR><TD>13</TD><TD>49,993,790</TD></TR>
<TR><TD>14</TD><TD>50,000,819</TD></TR>
<TR><TD>15</TD><TD>49,995,340</TD></TR>
<TR><TD>16</TD><TD>50,000,634</TD></TR>
<TR><TD>17</TD><TD>50,004,450</TD></TR>
<TR><TD>18</TD><TD>49,999,081</TD></TR>
<TR><TD>19</TD><TD>50,003,646</TD></TR>
<TR><TD>20</TD><TD>50,001,295</TD></TR>
<TR><TD>21</TD><TD>50,005,936</TD></TR>
<TR><TD>22</TD><TD>50,001,090</TD></TR>
<TR><TD>23</TD><TD>50,003,166</TD></TR>
<TR><TD>24</TD><TD>50,003,137</TD></TR>
<TR><TD>25</TD><TD>49,999,151</TD></TR>
<TR><TD>26</TD><TD>49,998,465</TD></TR>
<TR><TD>27</TD><TD>50,001,554</TD></TR>
<TR><TD>28</TD><TD>50,005,453</TD></TR>
<TR><TD>29</TD><TD>50,010,749</TD></TR>
<TR><TD>30</TD><TD>49,995,459</TD></TR>
<TR><TD>31</TD><TD>49,995,812</TD></TR>
</TABLE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CRandomNumberGenerator2.cpp $<BR>
$Modtime: 6/26/01 10:50a $
</BODY>

</HTML>
*/
