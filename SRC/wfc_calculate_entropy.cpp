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
*/

#include <wfc.h>
#pragma hdrstop

#if ! defined( WFC_STL )

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#endif // WFC_STL

USING_WFC_NAMESPACE

// Define log( 2.0 )
#define WFC_LOG2 (0.693147180559945309417232121458176568075500134360255254120680009493393621969L)

_Check_return_ double PASCAL Win32FoundationClasses::wfc_calculate_entropy( __in_ecount( number_of_counts ) uint64_t const * counts, _In_ std::size_t const number_of_counts ) noexcept
{
   WFC_VALIDATE_POINTER( counts );

   if ( counts           == nullptr ||
        number_of_counts < 2 )
   {
      //WFCTRACE( TEXT( "Nothing to do." ) );
      return( 0.0 );
   }

   WFC_TRY
   {
      double entropy{ 0.0 };

      int64_t sum{ 0 };

      std::size_t number_of_unique_values{ 0 };

      for ( auto const array_index : Range(number_of_counts) )
      {
         if ( counts[ array_index ] > 0 )
         {
            sum += counts[ array_index ];

            // 2001-08-23
            // We need all of this casting because VC6SP3 doesn't know how to
            // convert unsigned int64_t's to double
            entropy -= ( ((double) ((int64_t)counts[ array_index ])) * log( ((double) ((int64_t)counts[ array_index ])) ) );

            number_of_unique_values++;
         }
      }

      if ( sum <= 0 || number_of_unique_values < 2 ) // If there's only one unique value, the entropy is zero
      {
         return( 0.0 );
      }

      entropy /= sum;
      entropy += log( (double) sum );
      entropy /= WFC_LOG2;

      return( entropy );
   }
   WFC_CATCH_ALL
   {
      return( 0.0 );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ double PASCAL Win32FoundationClasses::wfc_calculate_entropy( __in_ecount( number_of_counts ) uint16_t const * counts, _In_ std::size_t const number_of_counts ) noexcept
{
   WFC_VALIDATE_POINTER( counts );

   if ( counts           == nullptr ||
        number_of_counts < 2 )
   {
      //WFCTRACE( TEXT( "Nothing to do." ) );
      return( 0.0 );
   }

   WFC_TRY
   {
      double entropy{ 0.0 };

      int64_t sum{ 0 };

      std::size_t number_of_unique_values{ 0 };

      for ( auto const array_index : Range(number_of_counts) )
      {
         if ( counts[ array_index ] > 0 )
         {
            sum += counts[ array_index ];
            entropy -= ( counts[ array_index ] * log( (double) counts[ array_index ] ) );
            number_of_unique_values++;
         }
      }

      if (sum <= 0 || number_of_unique_values < 2) // If there's only one unique value, the entropy is zero
      {
         return( 0.0 );
      }

      entropy /= sum;
      entropy += log( (double) sum );
      entropy /= WFC_LOG2;

      return( entropy );
   }
   WFC_CATCH_ALL
   {
      return( 0.0 );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ double PASCAL Win32FoundationClasses::wfc_calculate_entropy( __in_ecount( number_of_counts ) uint32_t const * counts, _In_ std::size_t const number_of_counts ) noexcept
{
   WFC_VALIDATE_POINTER( counts );

   if ( counts           == nullptr ||
        number_of_counts < 2 )
   {
      //WFCTRACE( TEXT( "Nothing to do." ) );
      return( 0.0 );
   }

   WFC_TRY
   {
      double entropy{0.0};

      int64_t sum{ 0 };

      std::size_t number_of_unique_values{ 0 };

      for ( auto const array_index : Range(number_of_counts) )
      {
         if ( counts[ array_index ] > 0 )
         {
            sum += counts[ array_index ];
            entropy -= ( counts[ array_index ] * log( (double) counts[ array_index ] ) );
            number_of_unique_values++;
         }
      }

      if (sum <= 0 || number_of_unique_values < 2) // If there's only one unique value, the entropy is zero
      {
         return( 0.0 );
      }

      entropy /= sum;
      entropy += log( (double) sum );
      entropy /= WFC_LOG2;

      return( entropy );
   }
   WFC_CATCH_ALL
   {
      return( 0.0 );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ double Win32FoundationClasses::wfc_get_buffer_entropy(__in_bcount(buffer_size) uint8_t const * buffer, _In_ std::size_t const buffer_size) noexcept
{
    WFC_VALIDATE_POINTER(buffer);

    if (buffer == nullptr || buffer_size < 2)
    {
        return(0.0);
    }

    uint64_t counts[256];

    ZeroMemory(counts, sizeof(counts));

    for ( auto const buffer_index : Range(buffer_size) )
    {
        counts[buffer[buffer_index]]++;
    }

    return(Win32FoundationClasses::wfc_calculate_entropy(counts, std::size(counts)));
}

__checkReturn bool Win32FoundationClasses::wfcGenRandom( _Out_writes_bytes_(RandomBufferLength) PVOID RandomBuffer, _In_ ULONG const RandomBufferLength ) noexcept
{
   static CRandomNumberGenerator2 * static_rng = nullptr;

   if ( RandomBuffer == nullptr || RandomBufferLength < 1 )
   {
       if ( RandomBuffer == nullptr && RandomBufferLength == 0 )
       {
           if ( static_rng != nullptr )
           {
               // Copy the pointer, this will maybe (not bloody likely) prevent multithreaded problems.
               // It keeps the current RNG valid for just a couple of nanoseconds longer.
               // If the destructor for the current RNG takes a while to clean up, another thread could
               // try to use it. If that happens, the RNG being used is in the middle of being destructed.
               // By immediately setting the static pointer to null, the other thread will create a new one
               // and use it while we destruct the old one. If this sitution happens, you've got other problems
               // to worry about...
               CRandomNumberGenerator2 * temp_rng = static_rng;
               static_rng = nullptr;
               delete temp_rng;
           }
       }

      return( false );
   }

   if ( static_rng == nullptr )
   {
      static_rng = new CRandomNumberGenerator2();
   }

   if ( static_rng != nullptr )
   {
      if ( RandomBufferLength == ULONG_MAX )
      {
         ((BYTE *)RandomBuffer)[ 0 ] = 0x00;
         // Fill nothing. 
         delete static_rng;
         static_rng = nullptr;
         return( true );
      }

      return( static_rng->Fill( (BYTE *) RandomBuffer, RandomBufferLength ) == false ? FALSE : TRUE );
   }

   return( false );
}

using FIND_BYTE_FUNCTION = int64_t (*)( __in uint8_t const byte_value, __in_bcount( buffer_size ) uint8_t const * buffer, __in int64_t const buffer_size );

#if 0
// This is the super optimized version 
// 2016-05-30 - SRB - I cannot get this version to work on our Jenkins build server.
// A stand-alone project with this code works fine but refuses to work during the build.
// This version fails the test where the desired byte was found at offset 16 in the 32-byte test buffer.
_Check_return_ static int64_t _find_byte_256(_In_ uint8_t const byte_value, _In_reads_bytes_(buffer_size) uint8_t const * buffer, _In_ int64_t const buffer_size) noexcept
{
    __declspec(align(32)) __m256i const byte_to_find = _mm256_set1_epi8(byte_value); // AVX2
    __declspec(align(32)) __m256i const zero = _mm256_setzero_si256(); // AVX2

    int64_t const last_buffer_index = buffer_size - 31;

    int64_t buffer_index = 0;

    if (buffer_size > 31)
    {
        if (_mm256_testc_si256(zero, _mm256_cmpeq_epi8(byte_to_find, _mm256_loadu_si256((__m256i const *) buffer))) != 0)
        {
            // The desired byte was not found in the first block.
            // Make sure the buffer is aligned on a 32 byte boundary so our main loop can avoid the _mm256_loadu_si256 (AVX2) call

            if (((uint64_t)buffer % 32) == 0)
            {
                buffer_index = 32;
            }
            else
            {
                buffer_index = (32 - ((uint64_t)buffer % 32));
            }
        }
        else
        {
            goto SEARCH_LAST_BLOCK;
        }
    }
    else
    {
        goto SEARCH_LAST_BLOCK;
    }

    while (buffer_index < last_buffer_index &&
        _mm256_testc_si256(zero, _mm256_cmpeq_epi8(byte_to_find, *((__m256i const *) &buffer[buffer_index]))) != 0) // _mm256_testc_si256 - AVX2
    {
        buffer_index += 32;
    }

SEARCH_LAST_BLOCK:
    while (buffer_index < buffer_size)
    {
        if (buffer[buffer_index] == byte_value)
        {
            return(buffer_index);
        }

        buffer_index++;
    }

    return(BYTES_NOT_FOUND);
}

#else

// This version works
_Check_return_ static int64_t _find_byte_256( _In_ uint8_t const byte_value, _In_reads_bytes_( buffer_size ) uint8_t const * buffer, _In_ int64_t const buffer_size ) noexcept
{
    __declspec(align(32)) __m256i const byte_to_find = _mm256_set1_epi8( byte_value ); // AVX2
    __declspec(align(32)) __m256i const zero = _mm256_setzero_si256(); // AVX2

    int64_t const last_buffer_index = buffer_size - 31;

    int64_t buffer_index = 0;

    while( buffer_index < last_buffer_index &&
        _mm256_testc_si256( zero, _mm256_cmpeq_epi8( byte_to_find, _mm256_loadu_si256((__m256i const *) &buffer[ buffer_index ]))) != 0 ) // _mm256_testc_si256 - AVX2
    {
        buffer_index += 32;
    }

    while( buffer_index < buffer_size )
    {
        if ( buffer[ buffer_index ] == byte_value )
        {
            return( buffer_index );
        }

        buffer_index++;
    }

    return( BYTES_NOT_FOUND );
}
#endif

_Check_return_ int64_t _find_byte_SSE41(_In_ uint8_t const byte_value, _In_reads_bytes_(buffer_size) uint8_t const * buffer, _In_ int64_t const buffer_size) noexcept
{
    __declspec(align(16)) __m128i const byte_to_find = _mm_set1_epi8(byte_value); // SSE2
    __declspec(align(16)) __m128i const zero = _mm_setzero_si128(); // SSE2

    int64_t const last_buffer_index = buffer_size - 15;

    int64_t buffer_index = 0;

    if (buffer_size > 15)
    {
        if (_mm_testc_si128(zero, _mm_cmpeq_epi8(byte_to_find, _mm_loadu_si128((__m128i const *) buffer))) != 0)
        {
            // The desired byte was not found in the first block.
            // Make sure the buffer is aligned on a 16 byte boundary so our main loop can avoid the _mm_loadu_si128 (SSE2) call

            if (((uint64_t)buffer % 16) == 0)
            {
                buffer_index = 16;
            }
            else
            {
                buffer_index = (16 - ((uint64_t)buffer % 16));
            }
        }
        else
        {
            goto SEARCH_LAST_BLOCK;
        }
    }
    else
    {
        goto SEARCH_LAST_BLOCK;
    }

    _ASSERTE((((int64_t)&buffer[buffer_index]) % 16) == 0);

    while (buffer_index < last_buffer_index &&
        _mm_testc_si128(zero, _mm_cmpeq_epi8(byte_to_find, *((__m128i const *) &buffer[buffer_index]))) != 0) // _mm_cmpeq_epi8-SSE2, _mm_testc_si128-SSE4.1
    {
        buffer_index += 16;
    }

SEARCH_LAST_BLOCK:
    while (buffer_index < buffer_size)
    {
        if (buffer[buffer_index] == byte_value)
        {
            return(buffer_index);
        }

        buffer_index++;
    }

    return(BYTES_NOT_FOUND);
}

_Check_return_ int64_t _find_byte_using_nothing_but_C( _In_ uint8_t const byte_value, _In_reads_bytes_( buffer_size ) uint8_t const * buffer, _In_ int64_t const buffer_size ) noexcept
{
    int64_t buffer_index = 0;

    while( buffer_index < buffer_size )
    {
        if ( buffer[ buffer_index ] == byte_value )
        {
            return( buffer_index );
        }

        buffer_index++;
    }

    return( BYTES_NOT_FOUND );
}

_Check_return_ int64_t Win32FoundationClasses::find_byte( _In_ uint8_t const byte_value, _In_reads_bytes_( buffer_size ) uint8_t const * __restrict buffer, _In_ int64_t const buffer_size ) noexcept
{
    static FIND_BYTE_FUNCTION find_byte_implementation = nullptr;

    if ( find_byte_implementation == nullptr )
    {
        if ( is_avx2_supported() == true )
        {
            // Woo hoo! We can search 32 bytes at a time
            find_byte_implementation = _find_byte_256;
        }
        else if ( is_sse41_supported() == true )
        {
            // Not as rockin' as 32 bytes at a time but we can still search 16 bytes at a time

            find_byte_implementation = _find_byte_SSE41;
        }
        else
        {
            // What the what?? No SSE4, geez. Just use plain old C
            find_byte_implementation = _find_byte_using_nothing_but_C;
        }
    }

    return( find_byte_implementation( byte_value, buffer, buffer_size ) );
}

__checkReturn bool Win32FoundationClasses::wfc_process_buffer( __in uint8_t const * buffer, __in std::size_t const number_of_bytes_in_buffer, __in std::size_t const step_size, __inout PROCESS_BUFFER_CALLBACK function_to_call, __inout_opt void * callback_context ) noexcept
{
    if (function_to_call == nullptr )
    {
        return( false );
    }

    int64_t const number_of_bytes_to_process = number_of_bytes_in_buffer;
    int64_t number_of_bytes_processed = 0;
    int64_t number_of_bytes_to_process_in_this_call = 0;

    while( number_of_bytes_processed < number_of_bytes_to_process )
    {
        number_of_bytes_to_process_in_this_call = number_of_bytes_to_process - number_of_bytes_processed;

        if ( number_of_bytes_to_process_in_this_call <= 0 )
        {
            // No more bytes to processs
            return( true );
        }

        if ( number_of_bytes_to_process_in_this_call > (int64_t) step_size )
        {
            number_of_bytes_to_process_in_this_call = step_size;
        }

        if ( function_to_call( callback_context, &buffer[ number_of_bytes_processed ], number_of_bytes_to_process_in_this_call ) == false )
        {
            // They told us to stop.
            return( true );
        }

        number_of_bytes_processed += number_of_bytes_to_process_in_this_call;
    }

    return( true );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_calculate_entropy</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that calculates entropy for an arbitrary length histogram of 64-bit values.">
</HEAD>

<BODY>

<H1>wfc_calculate_entropy</H1>
$Revision: 24 $<HR>

<H2>Declaration</H2>
<PRE><CODE>double <B>wfc_calculate_entropy</B>( uint64_t const * counts, std::size_t number_of_counts )</CODE></PRE>

<H2>Description</H2>
This function takes a url and retrieves it from Internet. What you get is an
array containing the source for that page. This function is meant
to provide the capability to pull data from Internet and chew on it inside
a program (as opposed to display it to a user).

<H2>Example</H2>

<PRE><CODE>void print_maryland_weather_report( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;print_maryland_weather_report()&quot; ) );

   CUniformResourceLocator url( TEXT( &quot;http://iwin.nws.noaa.gov/iwin/md/hourly.html&quot; ) );

   std::vector&lt;std::wstring&gt; weather_report;

   <B>wfc_get_web_page</B>( url, weather_report );

   int index = 0;
   int number_of_lines_in_report = weather_report.GetSize();

   while( index &lt; number_of_lines_in_report )
   {
      _tprintf( TEXT( &quot;%s\n&quot; ), (LPCTSTR) weather_report.GetAt( index ) );
      index++;
   }
}</CODE></PRE>

<HR><I>Copyright, 1995-2001, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_get_web_page.cpp $<BR>
$Modtime: 7/17/01 5:23a $
</BODY>

</HTML>
*/
