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

void create_huge_character_reference( LPCTSTR filename, DWORD size, CRandomNumberGenerator2& random )
{
   WFCTRACEINIT( TEXT( "create_huge_character_reference()" ) );

   CFile file;

   if ( file.Open( filename, CFile::modeCreate | CFile::modeWrite ) == FALSE )
   {
      WFCTRACEERROR( GetLastError() );
      WFCTRACE( TEXT( "Can't create file." ) );
      return;
   }

   BYTE buffer[ 100 ];

   ZeroMemory( buffer, sizeof( buffer ) );

   strcpy( (char *) buffer, "<?xml version=\"1.0\" ?>" );

   file.Write( (const char *) buffer, strlen( (const char *) buffer ) );

   buffer[ 0 ] = CARRIAGE_RETURN;
   buffer[ 1 ] = LINE_FEED;
   buffer[ 2 ] = '<';
   buffer[ 3 ] = 'S';
   buffer[ 4 ] = '>';

   file.Write( buffer, 5 );

   DWORD loop_index = 0;

   CByteArray element_text;

   element_text.SetSize( 0, 8192 );

   DWORD end_tick_count = 0;
   DWORD start_tick_count = GetTickCount();
   element_text.Add( '&' );
   element_text.Add( '#' );
   
   while( loop_index < size )
   {
      element_text.Add( '0' );
      loop_index++;
   }

   element_text.Add( '6' );
   element_text.Add( '5' );
   element_text.Add( ';' );

   end_tick_count = GetTickCount();

   WFCTRACEVAL( TEXT( "Milliseconds is " ), (DWORD)( end_tick_count - start_tick_count ) );

   file.Write( element_text.GetData(), element_text.GetSize() );

   loop_index = 0;

   element_text.SetSize( 0, 8192 );

   element_text.Add( '&' );
   element_text.Add( '#' );
   element_text.Add( 'x' );
   
   while( loop_index < size )
   {
      element_text.Add( '0' );
      loop_index++;
   }

   element_text.Add( '4' );
   element_text.Add( '1' );
   element_text.Add( ';' );

   file.Write( element_text.GetData(), element_text.GetSize() );

   buffer[ 0 ] = '<';
   buffer[ 1 ] = '/';
   buffer[ 2 ] = 'S';
   buffer[ 3 ] = '>';
   buffer[ 4 ] = CARRIAGE_RETURN;
   buffer[ 5 ] = LINE_FEED;

   file.Write( buffer, 6 );
   file.Close();
}
