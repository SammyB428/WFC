/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2020, Samuel R. Blackburn
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
** $Workfile: TEST.CPP $
** $Revision: 14 $
** $Modtime: 7/23/00 1:43p $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

// #include <mlang.h>

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#if defined( UNICODE )
extern "C"
#endif

using GET_RANDOM_DWORD_FUNCTION = VOID(*)(DWORD *);

_Check_return_ bool failure( void ) noexcept
{
   return( false );
}

inline void split_view(_In_z_ wchar_t const* source, _In_ std::size_t const length, _In_ wchar_t const character, _Inout_ std::vector<std::wstring_view>& string_array) noexcept
{
    string_array.clear();

    if (source == nullptr or source[0] == 0x00 or character == 0x00)
    {
        return;
    }

    std::size_t beginning_index_of_string = 0;

    for (auto const string_index : Range(length))
    {
        if (source[string_index] == character)
        {
            string_array.push_back(std::wstring_view(&source[beginning_index_of_string], string_index - beginning_index_of_string));
            beginning_index_of_string = string_index + 1;
        }
    }

    if (beginning_index_of_string < length)
    {
        string_array.push_back(std::wstring_view(&source[beginning_index_of_string], length - beginning_index_of_string));
    }
}

_Check_return_ int _tmain( _In_ int const number_of_command_line_arguments, _In_reads_z_(number_of_command_line_arguments) LPCTSTR command_line_arguments[] )
{
   WFCTRACEINIT( TEXT( "_tmain()" ) );

   wchar_t const * ss = L"12,34,56,78";

   std::vector<std::wstring_view> views;

   split_view(ss, wcslen(ss), ',', views);

   std::string s3(STRING_VIEW("   abc "));
   auto lt = Win32FoundationClasses::trim_left_view(s3);
   auto rt = Win32FoundationClasses::trim_right_view(lt);
   auto tt = Win32FoundationClasses::trim_view(s3);
   auto t3 = Win32FoundationClasses::trim_view(STRING_VIEW("      "));

   std::wstring s4(WSTRING_VIEW(L"   xyz "));
   auto wlt = Win32FoundationClasses::trim_left_view(s4);
   auto wrt = Win32FoundationClasses::trim_right_view(wlt);
   auto wtt = Win32FoundationClasses::trim_view(s4);
   auto wt3 = Win32FoundationClasses::trim_view(WSTRING_VIEW(L"      "));

   WFC_INITIALIZE_SINGLE_THREADED_COM();

   IMultiLanguage2 * i = nullptr;

   GUID const mlang_class_id = { 0x275c23e2, 0x3747, 0x11d0, 0x9f, 0xea, 0x00, 0xaa, 0x00, 0x3f, 0x86, 0x46 };
   GUID const interface_id   = { 0xdccfc164, 0x2b38, 0x11d2, 0xb7, 0xec, 0x00, 0xc0, 0x4f, 0x8f, 0x5d, 0x9a };

   auto result = CoCreateInstance( mlang_class_id, NULL,
      CLSCTX_INPROC_SERVER bitor CLSCTX_LOCAL_SERVER,
      interface_id,
      (LPVOID *) &i );

   // You can view the thread name by putting "(char*)((DW(@TIB)+0x14))"
   // as a variable in the debugger's watch window
   //wfc_set_thread_name( "_tmain()" );

   std::wstring operating_system_version_string;

   Win32FoundationClasses::wfc_get_operating_system_version_string( operating_system_version_string );

   WFCTRACE( operating_system_version_string.c_str() );

   bool saw_zero = false;
   bool saw_one = false;
   bool saw_two = false;

   for (auto const index : Range(3))
   {
       if (index == 0)
       {
           saw_zero = true;
       }
       else if (index == 1)
       {
           saw_one = true;
       }
       else if (index == 2)
       {
           saw_two = true;
       }
   }

   if (saw_zero == false or saw_one == false or saw_two == false)
   {
       printf("Failed Range test 1\n");
   }

   saw_zero = false;
   saw_one = false;
   saw_two = false;

   for (auto const index : Range(3, StartingRangePosition(1)))
   {
       if (index == 0)
       {
           saw_zero = true;
       }
       else if (index == 1)
       {
           saw_one = true;
       }
       else if (index == 2)
       {
           saw_two = true;
       }
   }

   if (saw_zero == true or saw_one == false or saw_two == false)
   {
       printf("Failed Range test 2\n");
   }

//   memory();
//   test_CloseHandle();
//   test_CEventLog();
//   test_CFileTransferProtocol();
//   test_CLZFile( WSTRING_VIEW(L"b:\\ctl3d32.dl_") );
//   test_CNetSession( NULL );
//   test_CNetwork( NULL );
//   test_CNetwork( TEXT( "ftp" ) );
//   test_CNetworkConnections( NULL );
//   test_CNetworkFiles( NULL );
//   test_CNetworkShares( TEXT( "ftp" ) );
//   test_CNetworkShares( NULL );
//   test_CNetworkUsers( NULL );
//   test_CNetworkUsers( TEXT( "ftp" ) );
//   test_CQueue();
//   test_CReedSolomonErrorCorrectionCode();
//   test_CRAS();
//   test_CSerialFile();
//   test_CServer( TEXT( "sammy" ) );
//   test_CServer( TEXT( "sammy" ) );
//   test_CServer( TEXT( "\\\\SAMMY" ) );
//   test_CServiceControlManager();
//   test_CXMLDocument();

   if (Win32FoundationClasses::wfc_does_file_exist( command_line_arguments[ 0 ] ) == false )
   {
      _tprintf( TEXT( "wfc_does_file_exist() FAILED!\n" ) );
      return( EXIT_FAILURE );
   }

   // The order in which you place test functions in this array
   // is important. The most basic classes should be first.
   // If one class uses another class internally, the first class
   // should appear above the other in the array. For example,
   // before CBase64Coding (higher in the list). This way, if
   // CBase64Coding fails, you know it is not a problem with

   constexpr TEST_FUNCTION const tests[] =
   {
      test_find,
      test_is,
      test_CSystemTime,
      test_CWideString,
      test_CBitArray,
      test_CBase64Coding,
      test_CQueue,
      test_CDataParser,
      test_CFile64,
      test_CJulianDate,
      test_CRandomNumberGenerator,
      test_CReedSolomonErrorCorrectionCode,
      test_CUniformResourceLocator,
      test_CUUCoding,
      test_wfc_protect,
      test_CNetWorkstation,
      test_CNetResource,
      test_CRegistry,
      test_CServiceControlManager,
      test_CXMLDocument,
      test_process_list,

      // The following tests take a while to complete.
      //CQueue_integrity_test, // The 
      //CQueue_stress_test,
   };

#if 0
   GET_RANDOM_DWORD_FUNCTION get_random = (GET_RANDOM_DWORD_FUNCTION) GetProcAddress( GetModuleHandle( "NTDLL.DLL" ), "RtlRandomEx" );

   if ( get_random not_eq NULL )
   {
      DWORD new_random_value = GetTickCount();

      get_random( &new_random_value );

      WFCTRACEVAL( "Value is now", new_random_value );
   }
#endif

   int test_number_that_failed = 0;

   std::string class_name;

   int total_number_of_tests = 0;

   std::wstring number_of_tests_string;

   for ( auto const loop_index : Range( std::size( tests ) ) )
   {
      auto test_function = tests[ loop_index ];

      if ( test_function( class_name, test_number_that_failed ) == FALSE )
      {
         WFCTRACEVAL( TEXT( "This class failed " ), class_name.c_str() );
         WFCTRACEVAL( TEXT( "Test number " ), test_number_that_failed );
         wprintf( L"%S failed test number %d.\n", class_name.c_str(), test_number_that_failed );

         return( EXIT_FAILURE );
      }
      else
      {
         if ( test_number_that_failed == 1 )
         {
            wprintf( L"%S passed 1 test.\n", class_name.c_str() );
         }
         else if ( test_number_that_failed < 1 )
         {
            wprintf( L"%S passed no tests.\n", class_name.c_str() );
         }
         else
         {
             Win32FoundationClasses::format_number( number_of_tests_string, static_cast<int64_t>(test_number_that_failed) );
            wprintf( L"%S passed %s tests.\n", class_name.c_str(), number_of_tests_string.c_str() );
         }

         total_number_of_tests += test_number_that_failed;
      }
   }

   std::wstring number_of_classes;

   Win32FoundationClasses::format_number( number_of_classes, std::size( tests ) );
   Win32FoundationClasses::format_number( number_of_tests_string, static_cast<int64_t>(total_number_of_tests) );

   wprintf( L"All %s classes PASSED %s tests!\n", number_of_classes.c_str(), number_of_tests_string.c_str() );
   return( EXIT_SUCCESS );
}

__checkReturn LPTSTR BigNumToString( LONG lNum, LPTSTR szBuf ) noexcept
{
   WORD wNumDigits = 0, wNumChars = 0;

   do
   {
      szBuf[ wNumChars++ ] = (TCHAR) (lNum % 10 + TEXT( '0' ));
      wNumDigits++;
      
      if ( ( wNumDigits % 3 ) == 0 )
      {
         szBuf[ wNumChars++ ] = TEXT( ',' );
      }

      lNum /= 10;
   }
   while( lNum not_eq 0 );

   if ( szBuf[ wNumChars - 1 ] == TEXT( ',' ) )
   {
      szBuf[ wNumChars - 1 ] = 0x00;
   }

   szBuf[ wNumChars ] = 0x00;

   _tcsrev( szBuf );
   
   return( szBuf );
}

void memory( void )
{
    Win32FoundationClasses::CMemoryStatus memory_status;

   GlobalMemoryStatus( &memory_status );

#if ! defined( WFC_STL )
   TCHAR szBuf[ 50 ];

   TRACE1( TEXT( "Memory Load:         %-20s\n" ), BigNumToString( memory_status.dwMemoryLoad,    szBuf ) );
   TRACE1( TEXT( "Total Physical:      %-20s\n" ), BigNumToString( memory_status.dwTotalPhys,     szBuf ) );
   TRACE1( TEXT( "Available Physical:  %-20s\n" ), BigNumToString( memory_status.dwAvailPhys,     szBuf ) );
   TRACE1( TEXT( "Total Page File:     %-20s\n" ), BigNumToString( memory_status.dwTotalPageFile, szBuf ) );
   TRACE1( TEXT( "Available Page File: %-20s\n" ), BigNumToString( memory_status.dwAvailPageFile, szBuf ) );
   TRACE1( TEXT( "Total Virtual:       %-20s\n" ), BigNumToString( memory_status.dwTotalVirtual,  szBuf ) );
   TRACE1( TEXT( "Available Virtual:   %-20s\n" ), BigNumToString( memory_status.dwAvailVirtual,  szBuf ) );
#endif // WFC_STL
}
