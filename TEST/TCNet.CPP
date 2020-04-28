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
** $Workfile: TCNet.CPP $
** $Revision: 2 $
** $Modtime: 3/08/98 3:30p $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

_Check_return_ bool test_CNetWorkstation( _Out_ std::string& class_name, _Out_ int& test_number_that_failed ) noexcept
{
   WFCTRACEINIT( TEXT( "test_CNetWorkstation()" ) );

   class_name.assign(STRING_VIEW("CNetWorkstation"));

   test_number_that_failed = 1;

   Win32FoundationClasses::CNetWorkstation workstation; // Test this workstation

   Win32FoundationClasses::CWorkstationInformation workstation_information;

   if ( workstation.EnumerateInformation() == true )
   {
      //_tprintf( TEXT( "Workstation Machine Information:\n" ) );

      while( workstation.GetNext( workstation_information ) == true )
      {
#if 0
         _tprintf( TEXT( " PlatformID            - %d\n" ),               workstation_information.PlatformID            );
         _tprintf( TEXT( " Name                  - \"%s\"\n" ), (LPCTSTR) workstation_information.ComputerName          );
         _tprintf( TEXT( " LANGroup              - \"%s\"\n" ), (LPCTSTR) workstation_information.LANGroup              );
         _tprintf( TEXT( " MajorVersion          - %d\n" ),               workstation_information.MajorVersion          );
         _tprintf( TEXT( " MinorVersion          - %d\n" ),               workstation_information.MinorVersion          );
         _tprintf( TEXT( " LANRoot               - \"%s\"\n" ), (LPCTSTR) workstation_information.LANRoot               );
         _tprintf( TEXT( " NumberOfLoggedOnUsers - %d\n" ),               workstation_information.NumberOfLoggedOnUsers );
#endif
      }
   }
#if 0
   else
   {
      DWORD error_code = workstation.GetErrorCode();

      std::wstring error_message;

      Convert_NERR_Code_to_String( error_code, error_message );

      _tprintf( TEXT( "CNetWorkstation.Enumerate( workstation_information ), ErrorCode == %d \"%s\"\n" ), error_code, error_message.c_str() );
   }
#endif

   Win32FoundationClasses::CWorkstationUser user_information;

   if ( workstation.EnumerateUsers() == true )
   {
      // _tprintf( TEXT( "\nWorkstation User Information:\n" ) );

      WCHAR wide_machine_name[ 256 ];
      WCHAR wide_domain_name[ 256 ];

      LPSTR buffer = NULL;

      ZeroMemory( wide_machine_name, sizeof( wide_machine_name ) );
      ZeroMemory( wide_domain_name,  sizeof( wide_domain_name  ) );

      while( workstation.GetNext( user_information ) == true )
      {
#if 0
         ASCII_to_UNICODE( (LPCTSTR) user_information.LogonServer, wide_machine_name );
         ASCII_to_UNICODE( (LPCTSTR) user_information.LogonDomain, wide_domain_name  );

         NetGetDCName( wide_machine_name, wide_domain_name, (LPBYTE *) &buffer );
         UNICODE_to_ASCII( (LPCWSTR) buffer, buffer );

         _tprintf( TEXT( " User Name     - \"%s\"\n" ),      (LPCTSTR) user_information.UserName     );
         _tprintf( TEXT( " Logon Domain  - \"%s\" [%s]\n" ), (LPCTSTR) user_information.LogonDomain, (LPSTR) buffer );
         _tprintf( TEXT( " Other Domains - \"%s\"\n" ),      (LPCTSTR) user_information.OtherDomains );
         _tprintf( TEXT( " Logon Server  - \"%s\"\n\n" ),    (LPCTSTR) user_information.LogonServer  );
#endif
      }
   }
#if 0
   else
   {
      DWORD error_code = workstation.GetErrorCode();

      std::wstring error_message;

      Convert_NERR_Code_to_String( error_code, error_message );

      _tprintf( TEXT( "CNetWorkstation.Enumerate( user_information ), ErrorCode == %d \"%s\"\n" ), error_code, error_message.c_str() );
   }
#endif

   user_information.Empty();

   if ( workstation.GetCurrentUser( user_information ) not_eq FALSE )
   {
#if 0
      _tprintf( TEXT( "\nWorkstation Current User Information:\n" ) );
      _tprintf( TEXT( " User Name     - \"%s\"\n" ), (LPCTSTR) user_information.UserName     );
      _tprintf( TEXT( " Logon Domain  - \"%s\"\n" ), (LPCTSTR) user_information.LogonDomain  );
      _tprintf( TEXT( " Other Domains - \"%s\"\n" ), (LPCTSTR) user_information.OtherDomains );
      _tprintf( TEXT( " Logon Server  - \"%s\"\n" ), (LPCTSTR) user_information.LogonServer  );
#endif
   }
   else
   {
      DWORD error_code = workstation.GetErrorCode();

      std::wstring error_message;

      Win32FoundationClasses::Convert_NERR_Code_to_String( error_code, error_message );

      _tprintf( TEXT( "CNetWorkstation.GetCurrentUser( user_information ), ErrorCode == %d \"%s\"\n" ), error_code, error_message.c_str());
   }

   Win32FoundationClasses::CWorkstationTransport transport_information;

   if ( workstation.EnumerateTransports() == true )
   {
      //_tprintf( TEXT( "\nWorkstation Transport Information:\n" ) );

      while( workstation.GetNext( transport_information ) == true )
      {
#if 0
         _tprintf( TEXT( " Quality of Service         - %d\n" ), transport_information.QualityOfService        );
         _tprintf( TEXT( " Number Of Virtual Circuits - %d\n" ), transport_information.NumberOfVirtualCircuits );
         _tprintf( TEXT( " Name                       - \"%s\"\n" ), (LPCTSTR) transport_information.Name      );
         _tprintf( TEXT( " Address                    - \"%s\"\n" ), (LPCTSTR) transport_information.Address   );
   
         if ( transport_information.WANish not_eq FALSE )
         {
            _tprintf( TEXT( " WANish                     - TRUE\n\n" ) );
         }
         else
         {
            _tprintf( TEXT( " WANish                     - FALSE\n\n" ) );
         }
#endif
      }
   }
#if 0
   else
   {
      DWORD error_code = workstation.GetErrorCode();

      std::wstring error_message;

      Convert_NERR_Code_to_String( error_code, error_message );

      _tprintf( TEXT( "CNetWorkstation.Enumerate( transport_information ), ErrorCode == %d \"%s\"\n" ), error_code, error_message.c_str() );
   }
#endif

   return( true );
}
