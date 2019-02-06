/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 1996-2003, Samuel R. Blackburn
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

#include "TortureXML.h"
#pragma hdrstop

void create_huge_version( LPCTSTR filename, DWORD size, CRandomNumberGenerator2& random )
{
   WFCTRACEINIT( TEXT( "create_huge_version()" ) );

   CFile file;

   if ( file.Open( filename, CFile::modeCreate | CFile::modeWrite ) == FALSE )
   {
      WFCTRACEERROR( GetLastError() );
      WFCTRACE( TEXT( "Can't create file." ) );
      return;
   }

   BYTE buffer[ 100 ];

   ZeroMemory( buffer, sizeof( buffer ) );

   strcpy( (char *) buffer, "<?xml version=\"" );

   file.Write( (const char *) buffer, strlen( (const char *) buffer ) );

   CString legal_characters( TEXT( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:_.-" ) );

   DWORD number_of_legal_characters = legal_characters.GetLength();
   DWORD loop_index                 = 0;

   while( loop_index < size )
   {
      buffer[ 0 ] = legal_characters.GetAt( random.GetInteger() % number_of_legal_characters );
      file.Write( buffer, 1 );
      loop_index++;
   }

   buffer[ 0 ] = '\"';
   buffer[ 1 ] = ' ';
   buffer[ 2 ] = '?';
   buffer[ 3 ] = '>';
   buffer[ 4 ] = CARRIAGE_RETURN;
   buffer[ 5 ] = LINE_FEED;

   file.Write( buffer, 6 );
   file.Close();
}

