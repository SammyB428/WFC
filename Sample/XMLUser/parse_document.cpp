/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 1998-2003, Samuel R. Blackburn
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
** $Workfile: parse_document.cpp $
** $Revision: 1 $
** $Modtime: 6/14/98 9:39a $
*/
#include "XMLUser.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void user_element_callback( void * parameter, CExtensibleMarkupLanguageElement * element_p )
{
   WFCTRACEINIT( TEXT( "user_element_callback()" ) );

   _tprintf( TEXT( "in user_element_callback\n" ) );

   // SCHWEET! We have a user element

   CByteArray names;

   CXMLArchive archive;

   archive.ReadFrom( element_p );

   if ( archive.Read( TEXT( "BINARY" ), names ) == NULL )
   {
      WFCTRACE( TEXT( "Can't find BINARY" ) );
      _tprintf( TEXT( "Can't find BINARY" ) );
      return;
   }

   WFCTRACEVAL( TEXT( "Number of strings is " ), names.GetSize() );

   CFile output_file;

   if ( output_file.Open( TEXT( "c:\\temp\\output.bin" ), CFile::modeCreate | CFile::modeWrite ) == FALSE )
   {
      WFCTRACE( TEXT( "Can't create output.bin" ) );
      return;
   }

   output_file.Write( names.GetData(), names.GetSize() );
}

void parse_document( void )
{
   WFCTRACEINIT( TEXT( "parse_document()" ) );

   CExtensibleMarkupLanguageDocument document;

   CFile file;

//   if ( file.Open( TEXT( "users.xml" ), CFile::modeRead ) == FALSE )
   if ( file.Open( TEXT( "c:\\temp\\stuff.xml" ), CFile::modeRead ) == FALSE )
   {
      WFCTRACE( TEXT( "Can't open stuff.xml" ) );
      _tprintf( TEXT( "Can't open users.xml\n" ) );
      return;
   }

   CByteArray bytes;

   bytes.SetSize( file.GetLength() );

   file.Read( bytes.GetData(), bytes.GetSize() );

   file.Close();

   CDataParser parser;

   parser.Initialize( &bytes, DO_NOT_AUTOMATICALLY_DELETE);

   document.AddCallback( TEXT( "USER" ), user_element_callback, NULL );

   if ( document.Parse( parser ) == FALSE )
   {
      WFCTRACE( TEXT( "Can't parse" ) );
   }
   else
   {
      WFCTRACE( TEXT( "Parsed OK" ) );
   }
}
