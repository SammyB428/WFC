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
** $Revision: 3 $
** $Modtime: 3/08/98 3:20p $
*/

#include "my_xml.h"
#pragma hdrstop

BOOL parse_document( const CString& filename, CExtensibleMarkupLanguageDocument& document )
{
   WFCTRACEINIT( TEXT( "parse_document()" ) );

   CByteArray bytes;

   if ( get_bytes( filename, bytes ) != TRUE )
   {
      return( FALSE );
   }

   CDataParser parser;

   parser.Initialize( &bytes, DO_NOT_AUTOMATICALLY_DELETE);

   if ( document.Parse( parser ) != false )
   {
      WFCTRACE( TEXT( "Parsed OK" ) );


#if defined( _DEBUG )

      //document.Dump( afxDump );

#endif // _DEBUG

      return( TRUE );
   }
   else
   {
      WFCTRACE( TEXT( "Can't parse document" ) );
      return( FALSE );
   }

   return( TRUE );
}