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
** $Workfile: TCREG.CPP $
** $Revision: 5 $
** $Modtime: 2/01/00 8:08p $
*/

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

_Check_return_ bool test_CRegistry(_Out_ std::string& class_name, _Out_ int& test_number_that_failed ) noexcept
{
   class_name.assign( "CRegistry" );

   test_number_that_failed = 1;

   CRegistry registry;

   if ( registry.Connect( (HKEY) CRegistry::keyCurrentUser ) == false )
   {
      ReportError( registry.GetErrorCode() );
      return( failure() );
   }

   // Open HKEY_CURRENT_USER\Software

   if ( registry.Open( TEXT( "Software" ), (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue ) ) == false )
   {
      ReportError( registry.GetErrorCode() );
      test_number_that_failed = 2;
      return( failure() );
   }

   // Let's create a sub-key

   if ( registry.Create( TEXT( "Software\\WFC" ) ) == false )
   {
      ReportError( registry.GetErrorCode() );
      test_number_that_failed = 3;
      return( failure() );
   }

   // Let's create another sub-key

   if ( registry.Create( TEXT( "Software\\WFC\\Sub Key #1" ) ) == false )
   {
      ReportError( registry.GetErrorCode() );
      test_number_that_failed = 4;
      return( failure() );
   }

   // Let's create another sub-key

   if ( registry.Create( TEXT( "Software\\WFC\\Sub Key #2" ) ) == false )
   {
      ReportError( registry.GetErrorCode() );
      test_number_that_failed = 5;
      return( failure() );
   }

   // Now let's enumerate the sub keys.

   if ( registry.Open( TEXT( "Software\\WFC" ), (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue ) ) == false )
   {
      ReportError( registry.GetErrorCode() );
      test_number_that_failed = 6;
      return( failure() );
   }

   std::wstring sub_key_name;
   std::wstring registry_class_name;

   DWORD index = 0;

   while( registry.EnumerateKeys( index, sub_key_name, registry_class_name ) == true )
   {
      index++;
   }

   index = 0;

   BYTE buffer[ 4096 ];

   DWORD size_of_buffer = 4096;
   CRegistry::KeyValueTypes data_type;

   while( registry.EnumerateValues( index, sub_key_name, data_type, buffer, size_of_buffer ) == true )
   {
      index++;
   }

   std::vector<std::wstring> string_array;

   string_array.clear();

   if ( registry.GetStringArrayValue( L"Sources", string_array ) != false )
   {
      if ( registry.SetStringArrayValue( L"TempSources", string_array ) == false )
      {
         ReportError( registry.GetErrorCode() );
         test_number_that_failed = 7;
         return( failure() );
      }
   }

   if ( registry.SetValue( L"QFile", L"Sammy" ) == false )
   {
      ReportError( registry.GetErrorCode() );
      test_number_that_failed = 8;
      return( failure() );
   }

   std::wstring temp_sammy;

   if ( registry.GetValue( L"QFile", temp_sammy ) == false )
   {
      ReportError( registry.GetErrorCode() );
      test_number_that_failed = 9;
      return( failure() );
   }
   else
   {
      if ( temp_sammy.compare( L"Sammy" ) == I_AM_EQUAL_TO_THAT )
      {
         //WFCTRACE( TEXT( "GetValue OK" ) );
      }
      else
      {
         test_number_that_failed = 10;
         return( failure() );
      }
   }

   test_number_that_failed = 10;

   return( true );
}
