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
** $Workfile: dump_mime_mappings.cpp $
** $Revision: 1 $
** $Modtime: 3/22/00 4:31a $
** $Reuse Tracing Code: 1 $
*/

#include "MIME2HTML.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void dump_mime_mappings( void )
{
   WFCTRACEINIT( TEXT( "dump_mime_mappings()" ) );

   CRegistry registry;

   if ( registry.Connect( (HKEY) CRegistry::keyClassesRoot ) == FALSE )
   {
      WFCTRACEERROR( registry.GetErrorCode() );
      WFCTRACE( TEXT( "Can't connect to registry" ) );
      return;
   }

   if ( registry.Open( TEXT( "MIME\\Database\\Content Type" ),
      (CRegistry::CreatePermissions)(CRegistry::permissionRead) ) == FALSE )
   {
      WFCTRACEERROR( registry.GetErrorCode() );
      WFCTRACE( TEXT( "Can't open key \"MIME\\Database\\Content Type\"" ) );
      return;
   }

   CString subkey_name;
   CString class_name;

   _tprintf( TEXT( "<HTML>\n\n" ) );
   _tprintf( TEXT( "<HEAD>\n" ) );
   _tprintf( TEXT( "<TITLE>Microsoft MIME Mappings</TITLE>\n" ) );
   _tprintf( TEXT( "</HEAD>\n\n" ) );
   _tprintf( TEXT( "<BODY>\n\n" ) );
   _tprintf( TEXT( "<H1>Microsoft MIME Mappings</H1>\n\n" ) );
   _tprintf( TEXT( "<TABLE BORDER=1>" ) );
   _tprintf( TEXT( "<TR>\n" ) );
   _tprintf( TEXT( "   <TD ALIGN=CENTER>MIME Type</TD>\n" ) );
   _tprintf( TEXT( "   <TD ALIGN=CENTER>Associated Filename Extension</TD>\n" ) );
   _tprintf( TEXT( "   <TD ALIGN=CENTER>OLE Object Interface Name</TD>\n" ) );
   _tprintf( TEXT( "   <TD ALIGN=CENTER>Command Line Executed</TD>\n" ) );
   _tprintf( TEXT( "</TR>\n\n" ) );

   DWORD key_index = 0;

   CString output_string;

   while( registry.EnumerateKeys( key_index, subkey_name, class_name ) != FALSE )
   {
      _tprintf( TEXT( "<TR>\n" ) );

      output_string = subkey_name;

      normalize_string( output_string );
      _tprintf( TEXT( "   <TD>%s</TD>\n" ), (LPCTSTR) output_string );
      dump_mime_type( subkey_name );
      _tprintf( TEXT( "</TR>\n\n" ) );

      key_index++;
   }

   _tprintf( TEXT( "</BODY>\n\n" ) );
   _tprintf( TEXT( "</HTML>\n" ) );
}
