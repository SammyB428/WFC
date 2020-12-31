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

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

_Check_return_ bool test_is( _Out_ std::string& class_name, _Out_ int& test_number_that_failed ) noexcept
{
   class_name.assign(STRING_VIEW("test_is"));

   auto good_plain_ascii_guid = STRING_VIEW("0AEE2A9F-BCBB-11d0-8C72-00C04FC2B085");
   auto good_plain_wide_guid  = WSTRING_VIEW(L"0aee2A9F-BCBB-11d0-8C72-00C04FC2B085");
   auto good_curly_ascii_guid = STRING_VIEW("{0AEE2A9F-BCBB-11d0-8C72-00C04FC2B085}");
   auto good_curly_wide_guid  = WSTRING_VIEW(L"{0aee2A9F-BCBB-11d0-8C72-00C04FC2B085}");

   auto bad1_plain_ascii_guid = STRING_VIEW("0AEE2A9F-BCBB-11d0-8C72 00C04FC2B085");
   auto bad1_plain_wide_guid  = WSTRING_VIEW(L"0aee2A9F-BCBB-11d0-8C72 00C04FC2B085");
   auto bad1_curly_ascii_guid = STRING_VIEW("{0AEE2A9F-BCBB-11d0-8C72 00C04FC2B085}");
   auto bad1_curly_wide_guid  = WSTRING_VIEW(L"{0aee2A9F-BCBB-11d0-8C72 00C04FC2B085}");

   auto bad2_plain_ascii_guid = STRING_VIEW("0AEE2A9F-BCBB-11d0-8C72-00C04FC2B08O");
   auto bad2_plain_wide_guid  = WSTRING_VIEW(L"0aee2A9F-BCBB-11d0-8C72-00C04FC2B08O");
   auto bad2_curly_ascii_guid = STRING_VIEW("{0AEE2A9F-BCBB-11d0-8C72-00C04FC2B08O}");
   auto bad2_curly_wide_guid  = WSTRING_VIEW(L"{0aee2A9F-BCBB-11d0-8C72-00C04FC2B08O}");

   if (Win32FoundationClasses::wfc_is_guid( good_plain_ascii_guid ) == false )
   {
       class_name.assign(STRING_VIEW("wfc_is_guid( ascii )"));
       test_number_that_failed = 1;
       return( failure() );
   }

   if (Win32FoundationClasses::wfc_is_guid( bad1_plain_ascii_guid ) == true )
   {
       class_name.assign(STRING_VIEW("wfc_is_guid( ascii )"));
       test_number_that_failed = 2;
       return( failure() );
   }

   if (Win32FoundationClasses::wfc_is_guid( bad2_plain_ascii_guid ) == true )
   {
       class_name.assign(STRING_VIEW("wfc_is_guid( ascii )"));
       test_number_that_failed = 3;
       return( failure() );
   }

   if (Win32FoundationClasses::wfc_is_guid( good_plain_wide_guid ) == false )
   {
       class_name.assign(STRING_VIEW("wfc_is_guid( unicode )"));
       test_number_that_failed = 4;
       return( failure() );
   }

   if (Win32FoundationClasses::wfc_is_guid( bad1_plain_wide_guid ) == true )
   {
       class_name.assign(STRING_VIEW("wfc_is_guid( unicode )"));
       test_number_that_failed = 5;
       return( failure() );
   }

   if (Win32FoundationClasses::wfc_is_guid( bad2_plain_wide_guid ) == true )
   {
       class_name.assign(STRING_VIEW("wfc_is_guid( unicode )"));
       test_number_that_failed = 6;
       return( failure() );
   }

   if (Win32FoundationClasses::wfc_is_guid_with_curlies( good_curly_ascii_guid ) == false )
   {
       class_name.assign(STRING_VIEW("wfc_is_guid_with_curlies( ascii )"));
       test_number_that_failed = 7;
       return( failure() );
   }

   if (Win32FoundationClasses::wfc_is_guid_with_curlies( bad1_curly_ascii_guid ) == true )
   {
       class_name.assign(STRING_VIEW("wfc_is_guid_with_curlies( ascii )"));
       test_number_that_failed = 8;
       return( failure() );
   }

   if (Win32FoundationClasses::wfc_is_guid_with_curlies( bad2_curly_ascii_guid ) == true )
   {
       class_name.assign(STRING_VIEW("wfc_is_guid_with_curlies( ascii )"));
       test_number_that_failed = 9;
       return( failure() );
   }

   if (Win32FoundationClasses::wfc_is_guid_with_curlies( good_curly_wide_guid ) == false )
   {
       class_name.assign(STRING_VIEW("wfc_is_guid_with_curlies( unicode )"));
       test_number_that_failed = 10;
       return( failure() );
   }

   if (Win32FoundationClasses::wfc_is_guid_with_curlies( bad1_curly_wide_guid ) == true )
   {
       class_name.assign(STRING_VIEW("wfc_is_guid_with_curlies( unicode )"));
       test_number_that_failed = 11;
       return( failure() );
   }

   if (Win32FoundationClasses::wfc_is_guid_with_curlies( bad2_curly_wide_guid ) == true )
   {
       class_name.assign(STRING_VIEW("wfc_is_guid_with_curlies( unicode2 )"));
       test_number_that_failed = 12;
       return( failure() );
   }

   if (Win32FoundationClasses::wfc_find_curly_guid( reinterpret_cast<uint8_t const *>(good_curly_ascii_guid.data()), good_curly_ascii_guid.length() ) not_eq 0 )
   {
       class_name.assign(STRING_VIEW("wfc_find_curly_guid()"));
       test_number_that_failed = 13;
       return( failure() );
   }

   if (Win32FoundationClasses::wfc_find_wide_curly_guid( reinterpret_cast<uint8_t const *>(good_curly_wide_guid.data()), good_curly_wide_guid.length() * sizeof( wchar_t ) ) not_eq 0 )
   {
       class_name.assign(STRING_VIEW("wfc_find_wide_curly_guid()"));
       test_number_that_failed = 14;
       return( failure() );
   }

   if (Win32FoundationClasses::is_bad_handle( (HANDLE) NULL ) == false )
   {
       class_name.assign(STRING_VIEW("NULL is not a valid handle"));
       test_number_that_failed = 15;
       return( failure() );
   }

   if (Win32FoundationClasses::is_bad_handle( (HANDLE) INVALID_HANDLE_VALUE ) == false )
   {
       class_name.assign(STRING_VIEW("INVALID_HANDLE_VALUE is not a valid handle"));
       test_number_that_failed = 16;
       return( failure() );
   }

   if (Win32FoundationClasses::round_down_to_a_multiple_of(1025, 512) not_eq 1024)
   {
       class_name.assign(STRING_VIEW("round_down_to_a_multiple_of"));
       test_number_that_failed = 17;
       return(failure());
   }

   if (Win32FoundationClasses::round_down_to_a_multiple_of(512, 512) not_eq 512)
   {
       class_name.assign(STRING_VIEW("round_down_to_a_multiple_of"));
       test_number_that_failed = 18;
       return(failure());
   }

   if (Win32FoundationClasses::round_down_to_a_multiple_of(1023, 512) not_eq 512)
   {
       class_name.assign(STRING_VIEW("round_down_to_a_multiple_of"));
       test_number_that_failed = 19;
       return(failure());
   }

   if (Win32FoundationClasses::round_down_to_a_multiple_of(1024, 512) not_eq 1024)
   {
       class_name.assign(STRING_VIEW("round_down_to_a_multiple_of"));
       test_number_that_failed = 20;
       return(failure());
   }

   if (Win32FoundationClasses::round_down_to_a_multiple_of(1024, 0) not_eq 0)
   {
       class_name.assign(STRING_VIEW("round_down_to_a_multiple_of"));
       test_number_that_failed = 21;
       return(failure());
   }

   if (Win32FoundationClasses::round_down_to_a_multiple_of(511, 512) not_eq 0)
   {
       class_name.assign(STRING_VIEW("round_down_to_a_multiple_of()"));
       test_number_that_failed = 22;
       return(failure());
   }

   if (Win32FoundationClasses::wfc_is_dotted_ip_address(STRING_VIEW("1.1.1.1")) == false)
   {
       class_name.assign(STRING_VIEW("wfc_is_dotted_ip_address()"));
       test_number_that_failed = 23;
       return(failure());
   }

   if (Win32FoundationClasses::wfc_is_dotted_ip_address(STRING_VIEW("1.11.111.256")) == true)
   {
       class_name.assign(STRING_VIEW("wfc_is_dotted_ip_address()"));
       test_number_that_failed = 24;
       return(failure());
   }

   if (Win32FoundationClasses::wfc_is_dotted_ip_address(WSTRING_VIEW(L"1.1.1.1")) == false)
   {
       class_name.assign(STRING_VIEW("wfc_is_dotted_ip_address()"));
       test_number_that_failed = 25;
       return(failure());
   }

   if (Win32FoundationClasses::wfc_is_dotted_ip_address(WSTRING_VIEW(L"1.11.111.256")) == true)
   {
       class_name.assign(STRING_VIEW("wfc_is_dotted_ip_address()"));
       test_number_that_failed = 26;
       return(failure());
   }

   std::filesystem::path p;

   p.append(L"AA");
   p.append(L"BB");
   p.append(L"CC");

   std::wstring test_string;

   Win32FoundationClasses::format(test_string, L"%s.txt", p);

   if (test_string.compare(WSTRING_VIEW(L"AA\\BB\\CC.txt")) not_eq I_AM_EQUAL_TO_THAT)
   {
       test_number_that_failed = 27;
       return(failure());
   }

   wchar_t const* ss = L"12,34,56,78,";

   std::vector<std::wstring_view> views;

   Win32FoundationClasses::split_view(ss, wcslen(ss), ',', views);

   if (views.size() not_eq 5)
   {
       test_number_that_failed = 28;
       return(failure());
   }

   if (views[4].empty() == false)
   {
       test_number_that_failed = 29;
       return(failure());
   }

   if (views[0].compare(WSTRING_VIEW(L"12")) not_eq I_AM_EQUAL_TO_THAT)
   {
       test_number_that_failed = 30;
       return(failure());
   }

   if (views[1].compare(WSTRING_VIEW(L"34")) not_eq I_AM_EQUAL_TO_THAT)
   {
       test_number_that_failed = 31;
       return(failure());
   }

   if (views[2].compare(WSTRING_VIEW(L"56")) not_eq I_AM_EQUAL_TO_THAT)
   {
       test_number_that_failed = 32;
       return(failure());
   }

   if (views[3].compare(WSTRING_VIEW(L"78")) not_eq I_AM_EQUAL_TO_THAT)
   {
       test_number_that_failed = 33;
       return(failure());
   }

   ss = L"78,56,34,12";

   Win32FoundationClasses::split_view(ss, wcslen(ss), ',', views);

   if (views.size() not_eq 4)
   {
       test_number_that_failed = 34;
       return(failure());
   }

   if (views[0].compare(WSTRING_VIEW(L"78")) not_eq I_AM_EQUAL_TO_THAT)
   {
       test_number_that_failed = 35;
       return(failure());
   }

   if (views[1].compare(WSTRING_VIEW(L"56")) not_eq I_AM_EQUAL_TO_THAT)
   {
       test_number_that_failed = 36;
       return(failure());
   }

   if (views[2].compare(WSTRING_VIEW(L"34")) not_eq I_AM_EQUAL_TO_THAT)
   {
       test_number_that_failed = 37;
       return(failure());
   }

   if (views[3].compare(WSTRING_VIEW(L"12")) not_eq I_AM_EQUAL_TO_THAT)
   {
       test_number_that_failed = 38;
       return(failure());
   }

   std::vector<std::string_view> ascii_views;

   Win32FoundationClasses::split("A|B|C|", '|', ascii_views);

   if (ascii_views.size() not_eq 4)
   {
       test_number_that_failed = 39;
       return(failure());
   }

   if (ascii_views[0].compare(STRING_VIEW("A")) not_eq I_AM_EQUAL_TO_THAT)
   {
       test_number_that_failed = 40;
       return(failure());
   }

   if (ascii_views[1].compare(STRING_VIEW("B")) not_eq I_AM_EQUAL_TO_THAT)
   {
       test_number_that_failed = 41;
       return(failure());
   }

   if (ascii_views[2].compare(STRING_VIEW("C")) not_eq I_AM_EQUAL_TO_THAT)
   {
       test_number_that_failed = 42;
       return(failure());
   }

   if (ascii_views[3].empty() == false)
   {
       test_number_that_failed = 43;
       return(failure());
   }

   test_number_that_failed = 43;
   return( true );
}
