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
** $Workfile: test_CRandomNumberGenerator.cpp $
** $Revision: 4 $
** $Modtime: 5/26/98 5:00a $
*/

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

static _Check_return_ bool count_ones_and_zeroes( _In_ DWORD const number_of_dwords_to_test ) noexcept
{
   DWORD number_of_ones                     = 0;
   DWORD number_of_zeroes                   = 0;
   DWORD length_of_longest_series_of_ones   = 0;
   DWORD length_of_longest_series_of_zeroes = 0;
   DWORD number_of_consecutive_ones         = 0;
   DWORD number_of_consecutive_zeroes       = 0;
   DWORD last_bit                           = 0;
   DWORD random_value                       = 0;
   DWORD bit                                = 0;
   DWORD maximum_number_of_ones             = 0;
   DWORD minimum_number_of_ones             = 0xFFFFFFFF;

   DWORD one_bits[ 32 ]; // Count the one-bits in the locations

   for ( auto const bit_number : Range(std::size( one_bits )) )
   {
      one_bits[ bit_number ] = 0;
   }

   CRandomNumberGenerator2 generator;
   //CRandomNumberGenerator generator;
   //CSuperRandomNumberGenerator generator;

   _tprintf( TEXT( "Testing %lu bits\n" ), number_of_dwords_to_test * 32 );

   for ( auto const loop_index : Range(number_of_dwords_to_test) )
   {
      random_value = generator.GetInteger();

      for ( auto const bit_number : Range(32) )
      {
         bit = _bittest( reinterpret_cast<LONG const *>(&random_value), static_cast<LONG>(bit_number) );

         if ( bit == 0 )
         {
            if ( bit == last_bit )
            {
               number_of_consecutive_zeroes++;

               if ( number_of_consecutive_zeroes > length_of_longest_series_of_zeroes )
               {
                  length_of_longest_series_of_zeroes = number_of_consecutive_zeroes;
               }
            }
            else
            {
               number_of_consecutive_zeroes = 1;
            }

            number_of_zeroes++;
         }
         else
         {
            // Bit is a 1

            one_bits[ bit_number ]++; // Increment our count

            if ( bit == last_bit )
            {
               number_of_consecutive_ones++;

               if ( number_of_consecutive_ones > length_of_longest_series_of_ones )
               {
                  length_of_longest_series_of_ones = number_of_consecutive_ones;
               }
            }
            else
            {
               number_of_consecutive_ones = 1;
            }

            number_of_ones++;
         }

         last_bit = bit;
      }
   }

   // Now that we've collected our statistics, let's report them

   //WFCTRACEVAL( TEXT( "Number of Ones " ),           number_of_ones                     );
   //WFCTRACEVAL( TEXT( "Number of Zeroes " ),         number_of_zeroes                   );
   //WFCTRACEVAL( TEXT( "Longest series of ones " ),   length_of_longest_series_of_ones   );
   //WFCTRACEVAL( TEXT( "Longest series of zeroes " ), length_of_longest_series_of_zeroes );

  // _tprintf( TEXT( "Number of ones           %lu\n" ), number_of_ones                     );
  // _tprintf( TEXT( "Number of zeroes         %lu\n" ), number_of_zeroes                   );
  // _tprintf( TEXT( "Longest series of ones   %lu\n" ), length_of_longest_series_of_ones   );
  // _tprintf( TEXT( "Longest series of zeroes %lu\n" ), length_of_longest_series_of_zeroes );

  // _tprintf( TEXT( "\nDistribution of 1's in location:\n" ), length_of_longest_series_of_zeroes );

   std::wstring output_string;

   for( auto const bit_number : Range(std::size( one_bits )) )
   {
      if ( one_bits[ bit_number ] > maximum_number_of_ones )
      {
         maximum_number_of_ones = one_bits[ bit_number ];
      }

      if ( one_bits[ bit_number ] < minimum_number_of_ones )
      {
         minimum_number_of_ones = one_bits[ bit_number ];
      }

      format( output_string, L"bit%lu = %lu", bit_number, one_bits[ bit_number ] );
    //  _tprintf( TEXT( "%s\n" ), (LPCTSTR) output_string );
      //WFCTRACEVAL( TEXT( " " ), output_string );
   }

   // The last test we perform is to determine an even distribution of ones over the 32 bit positions.
   // Arbitrarily chosen to be a deviation of no more than 1%

   DWORD const distance = maximum_number_of_ones - minimum_number_of_ones;

   // We had two choices for the value of the bit, 1 or 0. We can assume that roughly
   // half the values will turn out to be 1. This means our distance should be
   // calculated using the assumed total number of 1's.
   DWORD const half_the_sample_size = number_of_dwords_to_test / 2; // We had two choices for the

   double percentage = (double) ( (double) distance / (double) half_the_sample_size ) * (double) 100.0;

   if ( percentage > (double) 1.0 )
   {
      return( false );
   }

   return( true );
}

_Check_return_ bool test_CRandomNumberGenerator( _Out_ std::string& class_name, _Out_ int& test_number_that_failed ) noexcept
{
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
 subroutine RANMAR should be used to generate 20,000 random numbers.
 Then display the next six random numbers generated multiplied by 4096*4096
 If the random number generator is working properly, the random numbers
 should be:
           6533892.0  14220222.0   7275067.0
           6172232.0   8354498.0  10633180.0
************************************************************************/

   CRandomNumberGenerator random_number( MAKELONG( 1802, 9373 ) );

   class_name.assign(STRING_VIEW("CRandomNumberGenerator"));

   double temp_double = 0.0;

   for ( auto const index : Range(20000) )
   {
      temp_double = random_number.GetFloat();
   }

   temp_double = 4096.0 * 4096.0;

   double test_number = 0.0;

   test_number = random_number.GetFloat() * temp_double;

   if ( test_number != (DWORD) 6533892 )
   {
      test_number_that_failed = 1;
      return(failure());
   }

   test_number = random_number.GetFloat() * temp_double;

   if ( test_number != (DWORD) 14220222 )
   {
      test_number_that_failed = 2;
      return(failure());
   }

   test_number = random_number.GetFloat() * temp_double;

   if ( test_number != (DWORD) 7275067 )
   {
      test_number_that_failed = 3;
      return(failure());
   }

   test_number = random_number.GetFloat() * temp_double;

   if ( test_number != (DWORD) 6172232 )
   {
      test_number_that_failed = 4;
      return(failure());
   }

   test_number = random_number.GetFloat() * temp_double;

   if ( test_number != (DWORD) 8354498 )
   {
      test_number_that_failed = 5;
      return(failure());
   }

   test_number = random_number.GetFloat() * temp_double;

   if ( test_number != (DWORD) 10633180 )
   {
      test_number_that_failed = 6;
      return(failure());
   }

   if ( count_ones_and_zeroes( 1000000 ) == false )
   {
      test_number_that_failed = 7;
      return(failure());
   }

   CRandomNumberGenerator2 generator;

   // Don't let the generator produce truely random values.
   generator.Disable(true);

   DWORD const seed = ::GetTickCount();

   generator.SetSeed(seed);

   std::size_t loop_index = 0;
   std::size_t const number_of_tests = 1000033;

   uint32_t test_integer = 0;
   double test_double = 0.0;

   uint32_t const minimum_integer = 137;
   uint32_t const maximum_integer = 2115485863;
   
   double const minimum_double = 1.33;
   double const maximum_double = 2115485863.19630502;

   while (loop_index < number_of_tests)
   {
       test_integer = generator.Uint32(minimum_integer, maximum_integer);

       if (test_integer < minimum_integer or test_integer > maximum_integer)
       {
           printf("CRandomNumberGenerator2 failed Uint32 test, seed %08X, iteration %lu\n", seed, (unsigned long) loop_index);
           test_number_that_failed = 8;
           return(failure());
       }

       loop_index++;
   }

   loop_index = 0;

   while (loop_index < number_of_tests)
   {
       test_double = generator.Double(minimum_double, maximum_double);

       if (test_double < minimum_double or test_double > maximum_double)
       {
           printf("CRandomNumberGenerator2 failed Double test, seed %08X, iteration %lu\n", seed, (unsigned long)(loop_index + number_of_tests));
           test_number_that_failed = 9;
           return(failure());
       }

       loop_index++;
   }

   test_number_that_failed = 9;
   return( true );
}
