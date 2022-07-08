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
** $Workfile: TCNetshs.CPP $
** $Revision: 2 $
** $Modtime: 3/08/98 3:32p $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#define new DEBUG_NEW
#endif // _DEBUG

void test_CNetworkShares( __in_z LPCTSTR machine_name ) noexcept
{
   WFCTRACEINIT( TEXT( "test_CNetworkShares()" ) );

   Win32FoundationClasses::CNetworkShares shares( machine_name );

   Win32FoundationClasses::CNetworkShareInformation share_information;

   if ( shares.Enumerate() == true )
   {
      _tprintf( TEXT( "Share Information for %s:\n" ), (LPCTSTR) shares.GetMachineName() );

      while( shares.GetNext( share_information ) == true )
      {
         _tprintf( TEXT( " NetworkName         - \"%s\"\n" ), share_information.NetworkName.c_str() );
         _tprintf( TEXT( " Type                - %lu " ),     share_information.Type );

         switch( share_information.Type )
         {
            case STYPE_DISKTREE:

               _tprintf( TEXT( "Disk" ) );
               break;

            case STYPE_PRINTQ:

               _tprintf( TEXT( "Printer" ) );
               break;

            case STYPE_DEVICE:

               _tprintf( TEXT( "Communications Device" ) );
               break;

            case STYPE_IPC:

               _tprintf( TEXT( "IPC" ) );
               break;

#if 0
            case STYPE_DFS:

               _tprintf( TEXT( "Distributed File System" ) );
               break;
#endif
            case STYPE_SPECIAL:

               _tprintf( TEXT( "Special" ) );
               break;

            default:

               _tprintf( TEXT( "Unknown Type" ) );
               break;
         }

         _tprintf( TEXT( "\n" ) );
         _tprintf( TEXT( " Remark              - \"%s\"\n" ), (LPCTSTR) share_information.Remark.c_str()              );
         _tprintf( TEXT( " Permissions         - %lu\n" ),              share_information.Permissions         );
         _tprintf( TEXT( " MaximumNumberOfUses - %d\n" ),               share_information.MaximumNumberOfUses );
         _tprintf( TEXT( " CurrentNumberOfUses - %d\n" ),               share_information.CurrentNumberOfUses );
         _tprintf( TEXT( " PathName            - \"%s\"\n" ), (LPCTSTR) share_information.PathName.c_str()            );
         _tprintf( TEXT( " Password            - \"%s\"\n\n" ), (LPCTSTR) share_information.Password.c_str()          );
      }
   }
   else
   {
      auto error_code{ shares.GetErrorCode() };

      std::wstring error_message;

      Win32FoundationClasses::Convert_NERR_Code_to_String( error_code, error_message );

      _tprintf( TEXT( "CNetworkShares.Enumerate( share_information ), ErrorCode == %d \"%s\"\n" ), error_code, error_message.c_str());
   }
}
