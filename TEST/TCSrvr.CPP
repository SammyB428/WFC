/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2003, Samuel R. Blackburn
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
** $Workfile: TCSrvr.CPP $
** $Revision: 2 $
** $Modtime: 3/08/98 3:32p $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void test_CServer( _In_z_ LPCTSTR machine_name ) noexcept
{
   WFCTRACEINIT( TEXT( "test_CServer()" ) );

   CServer server;

   server.Open( machine_name );

   std::wstring name;
   std::wstring comment;
   std::wstring path;
   std::wstring os;
   std::wstring domain;

   DWORD major_version   = 0;
   DWORD minor_version   = 0;
   DWORD type            = 0;
   DWORD users           = 0;
   DWORD number_of_users = 0;
   DWORD number_of_opens = 0;

   server.GetUserPath( path );
   server.GetName( name );
   server.GetComment( comment );
   server.GetPlatformName( os );
   server.GetDomain( domain );

   major_version   = server.GetMajorVersion();
   minor_version   = server.GetMinorVersion();
   type            = server.GetType();
   users           = server.GetUsers();
   number_of_users = server.GetNumberOfUsers();
   number_of_opens = server.GetNumberOfOpens();

   _tprintf( TEXT( "Server Name:      \"%s\"\n" ), name.c_str()    );
   _tprintf( TEXT( "Server Domain:    \"%s\"\n" ), domain.c_str()  );
   _tprintf( TEXT( "Server Comment:   \"%s\"\n" ), comment.c_str() );
   _tprintf( TEXT( "Server User Path: \"%s\"\n" ), path.c_str()    );
   _tprintf( TEXT( "Server OS:        \"%s\"\n" ), os.c_str()      );
   _tprintf( TEXT( "Server Version:    %d.%d\n" ), major_version, minor_version );
   _tprintf( TEXT( "Server Users:      %d\n" ),    users );
   _tprintf( TEXT( "Number Of Users:   %d\n" ),    number_of_users );
   _tprintf( TEXT( "Number Of Opens:   %d\n" ),    number_of_opens );
}

