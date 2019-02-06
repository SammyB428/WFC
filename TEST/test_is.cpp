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
*/

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

_Check_return_ bool test_is( _Out_ std::string& class_name, _Out_ int& test_number_that_failed ) noexcept
{
   class_name.assign( "test_is" );

   const char *    good_plain_ascii_guid =  "0AEE2A9F-BCBB-11d0-8C72-00C04FC2B085";
   const wchar_t * good_plain_wide_guid  = L"0aee2A9F-BCBB-11d0-8C72-00C04FC2B085";
   const char *    good_curly_ascii_guid =  "{0AEE2A9F-BCBB-11d0-8C72-00C04FC2B085}";
   const wchar_t * good_curly_wide_guid  = L"{0aee2A9F-BCBB-11d0-8C72-00C04FC2B085}";

   const char *    bad1_plain_ascii_guid =  "0AEE2A9F-BCBB-11d0-8C72 00C04FC2B085";
   const wchar_t * bad1_plain_wide_guid  = L"0aee2A9F-BCBB-11d0-8C72 00C04FC2B085";
   const char *    bad1_curly_ascii_guid =  "{0AEE2A9F-BCBB-11d0-8C72 00C04FC2B085}";
   const wchar_t * bad1_curly_wide_guid  = L"{0aee2A9F-BCBB-11d0-8C72 00C04FC2B085}";

   const char *    bad2_plain_ascii_guid =  "0AEE2A9F-BCBB-11d0-8C72-00C04FC2B08O";
   const wchar_t * bad2_plain_wide_guid  = L"0aee2A9F-BCBB-11d0-8C72-00C04FC2B08O";
   const char *    bad2_curly_ascii_guid =  "{0AEE2A9F-BCBB-11d0-8C72-00C04FC2B08O}";
   const wchar_t * bad2_curly_wide_guid  = L"{0aee2A9F-BCBB-11d0-8C72-00C04FC2B08O}";

   if ( wfc_is_guid( good_plain_ascii_guid ) == false )
   {
       class_name.assign( "wfc_is_guid( ascii )" );
       test_number_that_failed = 1;
       return( failure() );
   }

   if ( wfc_is_guid( bad1_plain_ascii_guid ) != false )
   {
       class_name.assign( "wfc_is_guid( ascii )" );
       test_number_that_failed = 2;
       return( failure() );
   }

   if ( wfc_is_guid( bad2_plain_ascii_guid ) == false )
   {
       class_name.assign( "wfc_is_guid( ascii )" );
       test_number_that_failed = 3;
       return( failure() );
   }

   if ( wfc_is_guid( good_plain_wide_guid ) == false )
   {
       class_name.assign( "wfc_is_guid( unicode )" );
       test_number_that_failed = 4;
       return( failure() );
   }

   if ( wfc_is_guid( bad1_plain_wide_guid ) != false )
   {
       class_name.assign( "wfc_is_guid( unicode )" );
       test_number_that_failed = 5;
       return( failure() );
   }

   if ( wfc_is_guid( bad2_plain_wide_guid ) == false )
   {
       class_name.assign( "wfc_is_guid( unicode )" );
       test_number_that_failed = 6;
       return( failure() );
   }

   if ( wfc_is_guid_with_curlies( good_curly_ascii_guid ) == false )
   {
       class_name.assign( "wfc_is_guid_with_curlies( ascii )" );
       test_number_that_failed = 7;
       return( failure() );
   }

   if ( wfc_is_guid_with_curlies( bad1_curly_ascii_guid ) != false )
   {
       class_name.assign( "wfc_is_guid_with_curlies( ascii )" );
       test_number_that_failed = 8;
       return( failure() );
   }

   if ( wfc_is_guid_with_curlies( bad2_curly_ascii_guid ) != false )
   {
       class_name.assign( "wfc_is_guid_with_curlies( ascii )" );
       test_number_that_failed = 9;
       return( failure() );
   }

   if ( wfc_is_guid_with_curlies( good_curly_wide_guid ) == false )
   {
       class_name.assign( "wfc_is_guid_with_curlies( unicode )" );
       test_number_that_failed = 10;
       return( failure() );
   }

   if ( wfc_is_guid_with_curlies( bad1_curly_wide_guid ) != false )
   {
       class_name.assign( "wfc_is_guid_with_curlies( unicode )" );
       test_number_that_failed = 11;
       return( failure() );
   }

   if ( wfc_is_guid_with_curlies( bad2_curly_wide_guid ) != false )
   {
       class_name.assign( "wfc_is_guid_with_curlies( unicode2 )" );
       test_number_that_failed = 12;
       return( failure() );
   }

   if ( wfc_find_curly_guid( (const uint8_t *) good_curly_ascii_guid, strlen( good_curly_ascii_guid ) ) != 0 )
   {
       class_name.assign( "wfc_find_curly_guid()" );
       test_number_that_failed = 13;
       return( failure() );
   }

   if ( wfc_find_wide_curly_guid( (const uint8_t *) good_curly_wide_guid, wcslen( good_curly_wide_guid ) * sizeof( wchar_t ) ) != 0 )
   {
       class_name.assign( "wfc_find_wide_curly_guid()" );
       test_number_that_failed = 14;
       return( failure() );
   }

   if ( is_bad_handle( (HANDLE) NULL ) == false )
   {
       class_name.assign( "NULL is not a valid handle" );
       test_number_that_failed = 15;
       return( failure() );
   }

   if ( is_bad_handle( (HANDLE) INVALID_HANDLE_VALUE ) == false )
   {
       class_name.assign( "INVALID_HANDLE_VALUE is not a valid handle" );
       test_number_that_failed = 16;
       return( failure() );
   }

   if (round_down_to_a_multiple_of(1025, 512) != 1024)
   {
       class_name.assign("round_down_to_a_multiple_of");
       test_number_that_failed = 17;
       return(failure());
   }

   if (round_down_to_a_multiple_of(512, 512) != 512)
   {
       class_name.assign("round_down_to_a_multiple_of");
       test_number_that_failed = 18;
       return(failure());
   }

   if (round_down_to_a_multiple_of(1023, 512) != 512)
   {
       class_name.assign("round_down_to_a_multiple_of");
       test_number_that_failed = 19;
       return(failure());
   }

   if (round_down_to_a_multiple_of(1024, 512) != 1024)
   {
       class_name.assign("round_down_to_a_multiple_of");
       test_number_that_failed = 20;
       return(failure());
   }

   if (round_down_to_a_multiple_of(1024, 0) != 0)
   {
       class_name.assign("round_down_to_a_multiple_of");
       test_number_that_failed = 21;
       return(failure());
   }

   if (round_down_to_a_multiple_of(511, 512) != 0)
   {
       class_name.assign("round_down_to_a_multiple_of");
       test_number_that_failed = 22;
       return(failure());
   }
   test_number_that_failed = 22;
   return( true );
}
