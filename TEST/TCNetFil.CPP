/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2011, Samuel R. Blackburn
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
** $Workfile: TCNetFil.CPP $
** $Revision: 2 $
** $Modtime: 3/08/98 3:31p $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

// Why don't cannabals eat clowns? They taste funny.

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#define new DEBUG_NEW
#endif // _DEBUG

void test_CNetworkFiles( __in_z LPCTSTR machine_name ) noexcept
{
   WFCTRACEINIT( TEXT( "test_CNetworkFiles()" ) );

   Win32FoundationClasses::CNetworkFiles network_files( machine_name );

   Win32FoundationClasses::CNetworkFileInformation network_files_information;

   if ( network_files.Enumerate() == true )
   {
      _tprintf( TEXT( "Network Files Information:\n" ) );

      while( network_files.GetNext( network_files_information ) == true )
      {
         _tprintf( TEXT( " ID            - %d\n" ),     network_files_information.ID            );
         _tprintf( TEXT( " PathName      - \"%s\"\n" ), network_files_information.PathName.c_str());
         _tprintf( TEXT( " UserName      - \"%s\"\n" ), network_files_information.UserName.c_str());
         _tprintf( TEXT( " Permissions   - %d\n" ),     network_files_information.Permissions   );
         _tprintf( TEXT( " NumberOfLocks - %d\n" ),     network_files_information.NumberOfLocks );
      }
   }
   else
   {
      auto error_code{ network_files.GetErrorCode() };

      std::wstring error_message;

      Win32FoundationClasses::Convert_NERR_Code_to_String( error_code, error_message );

      _tprintf( TEXT( "CNetworkFiles.Enumerate( CNetworkFileInformation ), ErrorCode == %d \"%s\"\n" ), error_code, error_message.c_str());
   }
}
