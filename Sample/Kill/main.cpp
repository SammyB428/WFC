/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1997-2003, Samuel R. Blackburn
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

#define WINVER 0x401
#define WFC_STL
#include <wfc.h>
#include <psapi.h>
#pragma hdrstop

#pragma comment( lib, "psapi.lib" )

USING_WFC_NAMESPACE;

int _tmain( int argc, LPCTSTR argv[] )
{
   WFCTRACEINIT( TEXT( "main()" ) );

   if ( argc < 2 )
   {
      _tprintf( TEXT( "Usage: Kill process_id [process_id [...]]\n" ) );
      return( EXIT_FAILURE );
   }

   wfc_enable_all_privileges();

   size_t loop_index = 1;

   long process_id = 0;

   while( loop_index < argc )
   {
      process_id = _ttol( argv[ loop_index ] );

      if ( process_id > 0 )
      {
         HANDLE process_handle = OpenProcess( PROCESS_TERMINATE, FALSE, process_id );

         if ( process_handle != NULL )
         {
            TCHAR image_filename[ 2048 ];

            image_filename[ 0 ] = 0x00;

            if ( GetProcessImageFileName( process_handle, image_filename, sizeof( image_filename ) ) == 0 )
            {
               CString message_string;
               wfc_get_error_string( GetLastError(), message_string );
               WFCTRACEERROR( GetLastError() );
               _tprintf( TEXT( "%s\n" ), (LPCTSTR) message_string );
            }
            else
            {
               _tprintf( TEXT( "Terminating %s\n" ), image_filename );
            }

            WFCTRACEVAL( TEXT( "Terminating " ), CString( image_filename ) );

            if ( TerminateProcess( process_handle, 0 ) == FALSE )
            {
               CString error_message;

               wfc_get_error_string( GetLastError(), error_message );

               _tprintf( TEXT( "Can't kill process %ld, %lu - %s\n" ), process_id, GetLastError(), (LPCTSTR) error_message );
            }
            else
            {
               _tprintf( TEXT( "Process %ld killed.\n" ), process_id );
            }

            wfc_close_handle( process_handle );
         }
         else
         {
            CString error_message;

            wfc_get_error_string( GetLastError(), error_message );

            _tprintf( TEXT( "Can't open pro%scess %ld, error code %s, \n" ), process_id, GetLastError(), (LPCTSTR) error_message );
         }
      }

      loop_index++;
   }

   return( EXIT_SUCCESS );
}
