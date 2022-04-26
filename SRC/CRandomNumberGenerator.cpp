/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2022, Samuel R. Blackburn
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
** $Workfile: CRandomNumberGenerator.cpp $
** $Revision: 29 $
** $Modtime: 6/26/01 10:50a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

// Algorithm provided by Guy Gascoigne - Piggford (ggp@informix.com)
// Guy got it from:
/************************************************************************
This random number generator originally appeared in "Toward a Universal
Random Number Generator" by George Marsaglia and Arif Zaman.
Florida State University Report: FSU-SCRI-87-50 (1987)
It was later modified by F. James and published in "A Review of Pseudo-
random Number Generators"
Converted from FORTRAN to C by Phil Linttell, James F. Hickling
Management Consultants Ltd, Aug. 14, 1989.
THIS IS THE BEST KNOWN RANDOM NUMBER GENERATOR AVAILABLE.
       (However, a newly discovered technique can yield
         a period of 10^600. But that is still in the development stage.)
It passes ALL of the tests for random number generators and has a period
   of 2^144, is completely portable (gives bit identical results on all
   machines with at least 24-bit mantissas in the floating point
   representation).
The algorithm is a combination of a Fibonacci sequence (with lags of 97
   and 33, and operation "subtraction plus one, modulo one") and an
   "arithmetic sequence" (using subtraction).
On a Vax 11/780, this random number generator can produce a number in
    13 microseconds.
************************************************************************/

/************************************************************************
 This is the initialization routine for the random number generator RANMAR()
 NOTE: The seed variables can have values between:    0 <= IJ <= 31328
                                                      0 <= KL <= 30081
 The random number sequences created by these two seeds are of sufficient
 length to complete an entire calculation with. For example, if several
 different groups are working on different parts of the same calculation,
 each group could be assigned its own IJ seed. This would leave each group
 with 30000 choices for the second seed. That is to say, this random
 number generator can create 900 million different subsequences -- with
 each subsequence having a length of approximately 10^30.

 Use IJ = 1802 & KL = 9373 to test the random number generator. The
 subroutine RANMAR should be used to generate 20000 random numbers.
 Then display the next six random numbers generated multiplied by 4096*4096
 If the random number generator is working properly, the random numbers
 should be:
           6533892.0  14220222.0   7275067.0
           6172232.0   8354498.0  10633180.0
************************************************************************/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CRandomNumberGenerator::CRandomNumberGenerator() noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_C       = 0.0;
   m_Cd      = 0.0;
   m_Cm      = 0.0;
   m_Index_1 = 0;
   m_Index_2 = 0;

   ZeroMemory( m_Array, sizeof( m_Array ) );
   InitializeSeed();
}

Win32FoundationClasses::CRandomNumberGenerator::CRandomNumberGenerator( _In_ DWORD new_seed ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_C       = 0.0;
   m_Cd      = 0.0;
   m_Cm      = 0.0;
   m_Index_1 = 0;
   m_Index_2 = 0;

   ZeroMemory( m_Array, sizeof( m_Array ) );
   SetSeed( new_seed );
}

void Win32FoundationClasses::CRandomNumberGenerator::InitializeSeed( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // We need to seed our algorithm with something random from the operating system

   // GetTickCount64() returns the number of milliseconds the machine has been turned on

   WORD high_word{ LOWORD(::GetTickCount64()) };
   WORD low_word{ 0 };

   DWORD number_of_sectors_per_cluster{ 0 };
   DWORD number_of_bytes_per_sector{ 0 };
   DWORD number_of_free_clusters{ 0 };
   DWORD total_number_of_clusters{ 0 };

   // Different folks have different amounts of free space on their hard drives

   if ( ::GetDiskFreeSpace( nullptr,
                           &number_of_sectors_per_cluster,
                           &number_of_bytes_per_sector,
                           &number_of_free_clusters,
                           &total_number_of_clusters ) not_eq FALSE )
   {
      low_word = LOWORD( number_of_free_clusters );

      // This is a nice number but not nice enough. It won't vary enough
      // from call to call. So, let's scramble it a bit (no pun intented)
      // Anther call to GetTickCount() should do the trick because the
      // GetDiskFreeSpace() will take a varying amount of time.

      low_word xor_eq ( static_cast<WORD>( ::GetTickCount64() ) );
   }
   else
   {
      // This is a fail-safe condition. We resort to time because it is kinda
      // random but it is guessable.

      SYSTEMTIME system_time;

      ZeroMemory( &system_time, sizeof( system_time ) );

      ::GetSystemTime( &system_time );

      // The most random part of the system time is the milliseconds then seconds

      BYTE high_byte{ LOBYTE(system_time.wMilliseconds) };
      BYTE low_byte{ static_cast<BYTE>((system_time.wMilliseconds >> 8) + system_time.wSecond) };

      low_word = MAKEWORD( low_byte, high_byte );
   }

   // 2000-05-26
   // Added GetCurrentThreadId() so two threads won't initialize to the same seed

   SetSeed( ( MAKELONG( low_word, high_word ) xor ( ::GetCurrentThreadId() << 11 ) ) );
}

_Check_return_ DWORD Win32FoundationClasses::CRandomNumberGenerator::GetInteger( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   double value{ GetFloat() };

   value *= static_cast< double >( 0xFFFFFFFF );

   // Now, the last 8 bits are probably going to be 1
   // To get rid of this flaw in the stream we will
   // do a quick and dirty hash of the bytes that make 
   // up the double value

   auto buffer{ reinterpret_cast<BYTE const*>(&value) };

   DWORD hash_value{ static_cast<DWORD>(value) };
   DWORD temp_value{ 0 };

   for ( auto const index : Range(sizeof( value )) )
   {
      hash_value = ( hash_value << 4 ) + buffer[ index ];

      temp_value = hash_value bitand 0xF0000000L;

      if ( temp_value )
      {
         hash_value xor_eq temp_value >> 24;
      }

      hash_value and_eq compl temp_value;
   }

   DWORD return_value{ static_cast<DWORD>(value) };

   return_value += ( ( hash_value >> 5 ) % 256 );

   return( return_value );
}

_Check_return_ double Win32FoundationClasses::CRandomNumberGenerator::GetFloat( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   double return_value{ m_Array[m_Index_1] - m_Array[m_Index_2] };

   if ( return_value < 0.0 )
   {
      return_value += 1.0;
   }

   m_Array[ m_Index_1 ] = return_value;

   m_Index_1--;

   if ( m_Index_1 < 0 )
   {
      m_Index_1 = 96;
   }

   m_Index_2--;

   if ( m_Index_2 < 0 )
   {
      m_Index_2 = 96;
   }

   m_C -= m_Cd;

   if ( m_C < 0.0 )
   {
      m_C += m_Cm;
   }

   return_value -= m_C;

   if ( return_value < 0.0 )
   {
      return_value += 1.0;
   }

   return( return_value );
}

void Win32FoundationClasses::CRandomNumberGenerator::SetSeed( _In_ DWORD new_seed ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   WORD seed_1{ LOWORD(new_seed) };
   WORD seed_2{ HIWORD(new_seed) };

   while( seed_1 > 31328 ) // Cannot be converted to a Range loop
   {
      seed_1 %= 31329;
   }

   while( seed_2 > 30081 ) // Cannot be converted to a Range loop
   {
      seed_2 %= 30082;
   }

   int i{ static_cast<int>(::fmod(seed_1 / 177.0, 177.0) + 2) };
   int j{ static_cast<int>(::fmod(seed_1        , 177.0) + 2) };
   int k{ static_cast<int>(::fmod(seed_2 / 169.0, 178.0) + 1) };
   int l{ static_cast<int>(::fmod(seed_2        , 169.0)) };

   for( auto const outer_loop_index : Range(97) )
   {
      double seed{ 0.0 };
      double t{ 0.5 };

      for( auto const inner_loop_index : Range(24) )
      {
         double const m{ ::fmod(::fmod(i * j, 179.0) * k , 179.0) };
         i = j;
         j = k;
         k = static_cast< int >( m );
         l = static_cast< int >( ::fmod( 53.0 * l + 1.0 , 169.0 ) );

         if ( ::fmod( l * m, 64.0 ) > 31 )
         {
            seed += t;
         }

         t = static_cast< double >( 0.5 * t );
      }

      m_Array[ outer_loop_index ] = seed;
   }

   m_C  = static_cast< double >(   362436.0 / 16777216.0 );
   m_Cd = static_cast< double >(  7654321.0 / 16777216.0 );
   m_Cm = static_cast< double >( 16777213.0 / 16777216.0 );

   m_Index_1 = 96;
   m_Index_2 = 32;
}

Win32FoundationClasses::CRandomNumberGenerator::operator char() noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD const random_value{ GetInteger() };

   char const return_value{ static_cast<char>(random_value >> 9) };

   return( return_value );
}

Win32FoundationClasses::CRandomNumberGenerator::operator unsigned char() noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD const random_value{ GetInteger() };
   
   unsigned char const return_value{ static_cast<unsigned char>(random_value >> 17) };

   return( return_value );
}

Win32FoundationClasses::CRandomNumberGenerator::operator int() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( GetInteger() );
}

Win32FoundationClasses::CRandomNumberGenerator::operator unsigned int() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( GetInteger() );
}

Win32FoundationClasses::CRandomNumberGenerator::operator short() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( static_cast< short >( GetInteger() ) );
}

Win32FoundationClasses::CRandomNumberGenerator::operator unsigned short() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( static_cast< unsigned short >( GetInteger() ) );
}

Win32FoundationClasses::CRandomNumberGenerator::operator long() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( static_cast< long >( GetInteger() ) );
}

Win32FoundationClasses::CRandomNumberGenerator::operator unsigned long() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( GetInteger() );
}

Win32FoundationClasses::CRandomNumberGenerator::operator float() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( static_cast< float >( GetFloat() ) );
}

Win32FoundationClasses::CRandomNumberGenerator::operator double() noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( GetFloat() );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CRandomNumberGenerator</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, Random number generator, source code">
<META name="description" content="This C++ class gives you a really good random number generator.">
</HEAD>

<BODY>

<H1>CRandomNumberGenerator</H1>
$Revision: 29 $

<HR>

<H2>Description</H2>
This class implements a better random number generator than the C runtime
library. It is based on an algorithm provided by Guy Gascoigne - Piggford
(<A HREF="mailto:ggp@informix.com">ggp@informix.com</A>) who got it from
a generator that originally appeared in &quot;Toward a Universal
Random Number Generator&quot; by George Marsaglia and Arif Zaman.
Florida State University Report: FSU-SCRI-87-50 (1987).
It was later modified by F. James and published in &quot;A Review of
Pseudo-random Number Generators.&quot; Converted from FORTRAN to C by Phil
Linttell, James F. Hickling Management Consultants Ltd, Aug. 14, 1989. While
in FORTRAN, it was known as the RANMAR() generator. All I did was convert
it to a C++ class.

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

<H2>Example</H2><PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

void test_CRandomNumberGenerator( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;test_CRandomNumberGenerator()&quot; ) );

   <B>CRandomNumberGenerator</B> random_number;

   int index = 0;

   while( index &lt; 20 )
   {
      _tprintf( TEXT( &quot;DWORD  random number is %lu\n&quot; ), (DWORD)  random_number );
      _tprintf( TEXT( &quot;double random number is %lf\n&quot; ), (double) random_number );

      index++;
   }
}</CODE></PRE>

<H2>API's Used</H2>

<B>CNetworkUsers</B> uses the following API's:

<UL>
<LI>GetDiskFreeSpace
<LI>GetSystemTime
<LI>GetTickCount
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CRandomNumberGenerator.cpp $<BR>
$Modtime: 6/26/01 10:50a $
</BODY>

</HTML>
*/
