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
** $Workfile: TCNetUSR.CPP $
** $Revision: 3 $
** $Modtime: 1/31/00 7:33p $
*/

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void test_CNetworkUsers( __in_z LPCTSTR machine_name ) noexcept
{
   WFCTRACEINIT( TEXT( "test_CNetworkUsers()" ) );

   CNetworkUsers users( machine_name );

   CNetworkUserInformation user_information;

   if ( users.Enumerate() != FALSE )
   {
      _tprintf( TEXT( "User Information for %s:\n" ), (LPCTSTR) users.GetMachineName() );

      while( users.GetNext( user_information ) != FALSE )
      {
#if defined( _DEBUG ) && ! defined( WFC_STL )
         user_information.Dump( afxDump );
#endif // _DEBUG
         _tprintf( TEXT( " Name                 - \"%s\"\n" ), user_information.Name.c_str()          );
         _tprintf( TEXT( " Full Name            - \"%s\"\n" ), user_information.FullName.c_str()      );
         _tprintf( TEXT( " Comment              - \"%s\"\n" ), user_information.Comment.c_str()       );
         _tprintf( TEXT( " User Comment         - \"%s\"\n" ), user_information.UserComment.c_str()   );
         _tprintf( TEXT( " ID                   - %lu\n" ),              user_information.UserID        );
         _tprintf( TEXT( " Flags                - %lX\n" ),              user_information.Flags         );
         _tprintf( TEXT( " Privileges           - %lX\n" ),              user_information.Privileges    );
         _tprintf( TEXT( " Password             - \"%s\"\n" ), user_information.Password.c_str()      );
         _tprintf( TEXT( " Password Age         - %d Days %d Hours %d Minutes %d seconds\n" ),
                   user_information.PasswordAge.GetDays(),
                   user_information.PasswordAge.GetHours(),
                   user_information.PasswordAge.GetMinutes(),
                   user_information.PasswordAge.GetSeconds() );
         _tprintf( TEXT( " HomeDirectory        - \"%s\"\n" ), user_information.HomeDirectory.c_str() );
         _tprintf( TEXT( " Script Path          - \"%s\"\n" ), user_information.ScriptPath.c_str()    );
         _tprintf( TEXT( " Parameters           - \"%s\"\n" ), user_information.Parameters.c_str()    );
         _tprintf( TEXT( " Workstations         - \"%s\"\n" ), user_information.Workstations.c_str()  );
         //_tprintf( TEXT( " LastLogon            - \"%s\"\n" ), (LPCTSTR) user_information.LastLogon.Format( TEXT( "%a %d %b %y, %H:%M:%S" ) ) );
        // _tprintf( TEXT( " LastLogoff           - \"%s\"\n" ), (LPCTSTR) user_information.LastLogoff.Format( TEXT( "%a %d %b %y, %H:%M:%S" ) ) );
         //_tprintf( TEXT( " Account Expires      - \"%s\"\n" ), (LPCTSTR) user_information.AccountExpires.Format( TEXT( "%a %d %b %y, %H:%M:%S" ) ) );
         _tprintf( TEXT( " Maximum Storage      - %lu\n" ),              user_information.MaximumStorage     );
         _tprintf( TEXT( " Bad Password Count   - %lu\n" ),              user_information.BadPasswordCount   );
         _tprintf( TEXT( " Number Of Logons     - %lu\n" ),              user_information.NumberOfLogons     );
         _tprintf( TEXT( " Logon Server         - \"%s\"\n" ), user_information.LogonServer.c_str()        );
         _tprintf( TEXT( " Country Code         - %lX\n" ),              user_information.CountryCode        );
         _tprintf( TEXT( " Code Page            - %lX\n" ),              user_information.CodePage           );
         _tprintf( TEXT( " Primary Group ID     - %lu\n" ),              user_information.PrimaryGroupID     );
         _tprintf( TEXT( " Home Directory Drive - \"%s\"\n" ), user_information.HomeDirectoryDrive.c_str() );
         _tprintf( TEXT( " Profile              - \"%s\"\n" ), user_information.Profile.c_str()            );
         _tprintf( TEXT( " Password Has Expired - %lu\n" ),              user_information.PasswordHasExpired );
         _tprintf( TEXT( " Encrypted Password: " ) );

         for ( const auto index : Range(user_information.EncryptedPassword.size() ))
         {
            _tprintf( TEXT( "%02X" ), (int) user_information.EncryptedPassword.at( index ) );
         }

         _tprintf( TEXT( "\n\n" ) );
      }
   }
   else
   {
      DWORD error_code = users.GetErrorCode();

      std::wstring error_message;

      Convert_NERR_Code_to_String( error_code, error_message );

      _tprintf( TEXT( "CNetworkUsers.Enumerate( user_information ), ErrorCode == %d \"%s\"\n" ), error_code, error_message.c_str());
   }

   user_information.Empty();

   user_information.Name.assign( L"Laura" );
   user_information.Password.assign( L"LovesSammy" );

   user_information.SetAddDefaults();

   if ( users.Add( user_information ) != false )
   {
      _tprintf( TEXT( "Laura Added.\n" ) );
   }
   else
   {
      _tprintf( TEXT( "Can't Add User\n" ) );
   }

   if ( users.Delete( TEXT( "Laura" ) ) != FALSE )
   {
      _tprintf( TEXT( "Deleted Laura\n" ) );
   }
   else
   {
      _tprintf( TEXT( "Can't delete Laura\n" ) );
   }
}
