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
**
** $Workfile: dump_mime_type.cpp $
** $Revision: 1 $
** $Modtime: 3/21/00 5:59p $
** $Reuse Tracing Code: 1 $
*/

#include "MIME2HTML.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void dump_mime_type( const CString& mime_type_name )
{
   WFCTRACEINIT( TEXT( "dump_mime_type()" ) );

   CString key_name( TEXT( "MIME\\Database\\Content Type\\" ) );

   key_name += mime_type_name;

   CRegistry registry;

   if ( registry.Connect( (HKEY) CRegistry::keyClassesRoot ) == FALSE )
   {
      WFCTRACEERROR( registry.GetErrorCode() );
      WFCTRACE( TEXT( "Can't connect to registry" ) );
      return;
   }

   if ( registry.Open( key_name,
      (CRegistry::CreatePermissions)(CRegistry::permissionRead) ) == FALSE )
   {
      WFCTRACEERROR( registry.GetErrorCode() );
      WFCTRACEVAL( TEXT( "Can't open key " ), key_name );
      return;
   }

   CString extension;

   if ( registry.GetValue( TEXT( "Extension" ), extension ) == FALSE )
   {
      WFCTRACEERROR( registry.GetErrorCode() );
      WFCTRACE( TEXT( "Can't GetExtension Value " ) );
      return;
   }

   CString output_string( extension );

   normalize_string( output_string );

   _tprintf( TEXT( "   <TD>%s</TD>\n" ), (LPCTSTR) output_string );

   if ( registry.Open( extension,
      (CRegistry::CreatePermissions)(CRegistry::permissionRead) ) == FALSE )
   {
      WFCTRACEERROR( registry.GetErrorCode() );
      WFCTRACEVAL( TEXT( "Can't extension key " ), extension );
      return;
   }

   // Get the default value

   CString object_name;

   if ( registry.GetValue( NULL, object_name ) == FALSE )
   {
      WFCTRACEERROR( registry.GetErrorCode() );
      WFCTRACEVAL( TEXT( "Can't default value for " ), object_name );
      return;
   }

   output_string = object_name;

   normalize_string( output_string );

   _tprintf( TEXT( "   <TD>%s</TD>\n" ), (LPCTSTR) output_string );

   // Microsoft maps MIME types to filename extensions and
   // filename extensions to shell commands that are executed
   // when you double click on them.

   CString command_line;

   get_command_line( object_name, command_line );

   output_string = command_line;

   normalize_string( output_string );

   _tprintf( TEXT( "   <TD>%s</TD>\n" ), (LPCTSTR) output_string );
}
