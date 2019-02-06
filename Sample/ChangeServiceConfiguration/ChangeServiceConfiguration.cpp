#define WFC_STL
#define WINVER 0x0401
#define _WIN32_WINNT 0x0401
#include "wfc.h"
#pragma hdrstop

USING_WFC_NAMESPACE

/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1996-2003, Samuel R. Blackburn
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
** $Workfile: ChangeServiceConfiguration.cpp $
** $Revision: 3 $
** $Modtime: 4/02/98 7:30a $
*/

void print_error( LPCTSTR, DWORD );
void usage( void );

#if defined( UNICODE )
extern "C"
#endif // UNICODE

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   WFCTRACEINIT( TEXT( "_tmain()" ) );

   if ( number_of_command_line_arguments < 3 )
   {
      usage();
      return( EXIT_SUCCESS );
   }

   CServiceControlManager manager;

   if ( manager.Open() == FALSE )
   {
      print_error( TEXT( "Can't open service control manager" ), manager.GetErrorCode() );
      return( EXIT_SUCCESS );
   }

   if ( _tcsicmp( command_line_arguments[ 2 ], TEXT( "Auto" ) ) == 0 )
   {
      if ( manager.SetConfiguration( command_line_arguments[ 1 ], SERVICE_AUTO_START ) == FALSE )
      {
         print_error( TEXT( "Auto" ), manager.GetErrorCode() );
      }
      else
      {
         _tprintf( TEXT( "%s set to auto\n" ), command_line_arguments[ 1 ] );
      }
   }
   else if ( _tcsicmp( command_line_arguments[ 2 ], TEXT( "Manual" ) ) == 0 )
   {
      if ( manager.SetConfiguration( command_line_arguments[ 1 ], SERVICE_DEMAND_START ) == FALSE )
      {
         print_error( TEXT( "Manual" ), manager.GetErrorCode() );
      }
      else
      {
         _tprintf( TEXT( "%s set to manual\n" ), command_line_arguments[ 1 ] );
      }
   }
   else
   {
      usage();
   }

   return( EXIT_SUCCESS );
}

void print_error( LPCTSTR message, DWORD error_code )
{
   LPVOID message_buffer = (LPVOID) NULL;

   FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  error_code,
                  MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
        (LPTSTR) &message_buffer,
                  0,
                  NULL );

   if ( message_buffer != NULL )
   {
      _tprintf( TEXT( "%s\nError is %s\n" ), message, (LPCTSTR) message_buffer );

      LocalFree( message_buffer );
   }
   else
   {
      _tprintf( TEXT( "%s\nError Code is %d\n" ), message, error_code );
   }
}

void usage( void )
{
   _tprintf( TEXT( "ChangeServiceConfiguration name_of_service Auto|Manual\n" ) );
}
