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
** $Workfile: test_CFileTransferProtocol.cpp $
** $Revision: 4 $
** $Modtime: 1/31/00 7:46p $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#define new DEBUG_NEW
#endif // _DEBUG

void test_CFileTransferProtocol( void ) noexcept
{
   WFCTRACEINIT( TEXT( "test_CFileTransferProtocol()" ) );

#if ! defined( WFC_STL )

   // This registry stuff is here so I can keep private bits of
   // information about myself hidden from the WFC user group.
   // I trust you with WFC but not with my passwords ;-)

   CRegistry registry;

   if ( registry.Connect( (HKEY) CRegistry::keyLocalMachine ) == FALSE )
   {
      WFCTRACE( TEXT( "Can't connect to registry" ) );
      WFCTRACEVAL( TEXT( "At Line " ), __LINE__ );
      ReportError( registry.GetErrorCode() );
      return;
   }

   // Open HKEY_LOCAL_MACHINE\SOFTWARE\WFC\TEST\CFileTransferProtocol

   if ( registry.Open( TEXT( "SOFTWARE\\WFC\\TEST\\CFileTransferProtocol" ), (CRegistry::CreatePermissions)(CRegistry::permissionRead) ) == FALSE )
   {
      WFCTRACE( TEXT( "Can't open key" ) );
      WFCTRACEVAL( TEXT( "At line " ), __LINE__ );
      ReportError( registry.GetErrorCode() );
      return;
   }

   std::wstring url_string;
   std::wstring user;
   std::wstring password;

   registry.GetValue( TEXT( "url"      ), url_string );
   registry.GetValue( TEXT( "user"     ), user       );
   registry.GetValue( TEXT( "password" ), password   );

   if ( url_string.GetLength() == 0 )
   {
      WFCTRACE( TEXT( "You haven't set HKEY_LOCAL_MACHINE\\SOFTWARE\\WFC\\TEST\\CFileTransferProtocol\\url to the URL for the server to test." ) );
      return;
   }

   if ( user.GetLength() == 0 )
   {
      WFCTRACE( TEXT( "You haven't set HKEY_LOCAL_MACHINE\\SOFTWARE\\WFC\\TEST\\CFileTransferProtocol\\user to the user name for the server to test." ) );
      return;
   }

   if ( password.GetLength() == 0 )
   {
      WFCTRACE( TEXT( "You haven't set HKEY_LOCAL_MACHINE\\SOFTWARE\\WFC\\TEST\\CFileTransferProtocol\\password to the password for the server to test." ) );
      return;
   }

   CFileTransferProtocol ftp;

   ftp.SetUserName( user );
   ftp.SetPassword( password );

   std::vector<std::wstring> directory;

   CUniformResourceLocator url( url_string );

   ftp.GetDirectory( url, directory, "*.*" );

   _tprintf( TEXT( "Directory for %s\n" ), (LPCTSTR) url );

   for ( auto const index : Range(directory.GetSize()) )
   {
      WFCTRACE( directory.GetAt( index ) );
      _tprintf( TEXT( "%s\n" ), (LPCTSTR) directory.GetAt( index ) );
   }
#endif // WFC_STL
}

