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
*/

#define WINVER 0x0401
#define _WIN32_WINNT 0x0401
#define WFC_STL
#include <wfc.h>
#include <imagehlp.h>
#pragma hdrstop

USING_WFC_NAMESPACE

// include the right library in the linker stage
#pragma comment( lib, "imagehlp.lib" )

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments )
{
   WFCTRACEINIT( TEXT( "_tmain()" ) );

   TCHAR undecorated_name[ 513 ];

   ZeroMemory( undecorated_name, sizeof( undecorated_name ) );

   // It is unclear if the third parameter should be the size of the buffer or
   // the number of characters undecorated_name can hold. Documentation is shoddy

   CString decorated_name( TEXT( "?DT_GetCur@@YAJXZ" ) );

   if ( number_of_command_line_arguments < 2 )
   {
      if ( UnDecorateSymbolName( decorated_name, undecorated_name, sizeof( undecorated_name ), UNDNAME_COMPLETE ) > 0 )
      {
         WFCTRACEVAL( TEXT( "Undecorated name " ), CString( undecorated_name ) );
         _tprintf( TEXT( "%s\n" ), undecorated_name );
      }
      else
      {
         WFCTRACEVAL( TEXT( "This doesn't demangle " ), decorated_name );
         _tprintf( TEXT( "Unknown\n" ) );
      }
   }
   else
   {
      int index = 1;

      while( index < number_of_command_line_arguments )
      {
         decorated_name = command_line_arguments[ index ];

         if ( UnDecorateSymbolName( decorated_name, undecorated_name, sizeof( undecorated_name ), UNDNAME_COMPLETE ) > 0 )
         {
            WFCTRACEVAL( TEXT( "Undecorated name " ), CString( undecorated_name ) );
            _tprintf( TEXT( "%s\n" ), undecorated_name );
         }
         else
         {
            WFCTRACEVAL( TEXT( "This doesn't demangle " ), decorated_name );
            _tprintf( TEXT( "Unknown\n" ) );
         }

         index++;
      }
   }

   return( EXIT_SUCCESS );
}