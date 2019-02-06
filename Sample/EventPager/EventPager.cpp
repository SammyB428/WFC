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
#include "messages.h"
#pragma hdrstop

USING_WFC_NAMESPACE

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

#define APPLICATION_NAME TEXT( "EventPager" )

DWORD WINAPI worker_thread( LPVOID unused_parameter );
VOID set_default_parameters( void );

int __cdecl _tmain( int argc, LPCTSTR argv[] )
{
   if ( argc == 1 )
   {
      CService service( worker_thread );
      service.Initialize( APPLICATION_NAME );

      return( EXIT_SUCCESS );
   }

   if ( argc == 2 )
   {
      if ( _tcsicmp( argv[ 1 ], TEXT( "install" ) ) == 0 )
      {
         CServiceControlManager service_control_manager;

         service_control_manager.Open();

         if ( service_control_manager.Install( APPLICATION_NAME, APPLICATION_NAME, TEXT( "%SystemRoot%\\System32\\EventPager.exe" ) ) == FALSE )
         {
            _tprintf( TEXT( "Install failed, please see Application Log for details\n" ) );
         }

         set_default_parameters();

         return( EXIT_SUCCESS );
      }
      else if ( _tcsicmp( argv[ 1 ], TEXT( "remove" ) ) == 0 )
      {
         CServiceControlManager service_control_manager;

         service_control_manager.Open();

         if ( service_control_manager.Remove( APPLICATION_NAME ) == FALSE )
         {
            _tprintf( TEXT( "Removing failed, please see Application Log for details\n" ) );
         }

         return( EXIT_SUCCESS );
      }
      else if ( _tcsicmp( argv[ 1 ], TEXT( "run" ) ) == 0 )
      {
         worker_thread( (LPVOID) 1 );
         return( EXIT_SUCCESS );
      }
      else
      {
         _tprintf( TEXT( "Samuel R. Blackburn, EventPager\nUsage: %s [install|remove]\n" ), argv[ 0 ] );
      }
   }
   else
   {
      _tprintf( TEXT( "Samuel R. Blackburn, EventPager\nUsage: %s [install|remove]\n" ), argv[ 0 ] );
   }

   return( EXIT_SUCCESS );
}


DWORD WINAPI worker_thread( LPVOID )
{
   {
      CEventLog log( APPLICATION_NAME );

      log.Report( CEventLog::eventInformation, 0, MSG_SERVICE_STARTED );
   }

   CStringArray names_of_services_to_watch;

   CString machine_name( TEXT( "" ) );
   CString pager_com_port( TEXT( "" ) );
   CString pager_id( TEXT( "" ) );
   CString message( TEXT( "" ) );

   TCHAR this_computer_name[ MAX_PATH ];

   DWORD size = sizeof( this_computer_name );

   GetComputerName( this_computer_name, &size );

   {
      CRegistry registry;

      if ( registry.Connect( (HKEY) CRegistry::keyLocalMachine ) == FALSE )
      {
         CEventLog log( APPLICATION_NAME );

         CString error_message;

         error_message.Format( TEXT( "%s: %d" ), __FILE__, __LINE__ );

		   log.ReportError( error_message );

         return( 0 );
      }

      CString key_name( TEXT( "SYSTEM\\CurrentControlSet\\Services\\EventPager\\Parameters" ) );

      if ( registry.Open( key_name ) == FALSE )
      {
         CEventLog log( APPLICATION_NAME );

         CString error_message;

         error_message.Format( TEXT( "%s: %d" ), __FILE__, __LINE__ );

         log.ReportError( error_message );

         return( 0 );
      }

      registry.GetValue( TEXT( "PagerID" ), pager_id );
      registry.GetValue( TEXT( "PagerCOMPort" ), pager_com_port );
   }

   CString message_machine_name;

   if ( machine_name.IsEmpty() )
   {
      message_machine_name = this_computer_name;
   }
   else
   {
      message_machine_name = machine_name;
   }

   CSkyWord pager;

   pager.SetAccount( pager_id );
   pager.SetPhoneNumber( TEXT( "1-800-759-9673" ) );
   pager.SetSerialPort( pager_com_port );

   // Let's open the registry

   CEventLog event_log;

   return( 0 );
}

void set_default_parameters( void )
{
   CRegistry registry;

   if ( registry.Connect( (HKEY) CRegistry::keyLocalMachine ) != FALSE )
   {
      if ( registry.Create( TEXT( "SYSTEM\\CurrentControlSet\\Services\\EventPager\\Parameters" ) ) != FALSE )
      {
         CString unknown( TEXT( "Unknown" ) );

         if ( registry.SetValue( TEXT( "PagerID" ), unknown ) == FALSE )
         {
            LPVOID message_buffer = (LPVOID) NULL;

            ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                             NULL,
                             registry.GetErrorCode(),
                             MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
                   (LPTSTR) &message_buffer,
                             0,
                             NULL );

            LPCTSTR string_array[ 2 ];

            string_array[ 0 ] = TEXT( "SYSTEM\\CurrentControlSet\\Services\\EventPager\\Parameters\\PegerID" );
            string_array[ 1 ] = reinterpret_cast<LPCTSTR>( message_buffer );

            CEventLog event_log( APPLICATION_NAME );
            event_log.Report( CEventLog::eventError,
                              0,
                              MSG_CANT_SET_REGISTRY_ENTRY,
                              2,
                              reinterpret_cast<LPCTSTR *>( string_array ) );

            if ( message_buffer != NULL )
            {
               ::LocalFree( message_buffer );
            }
         }

         unknown = TEXT( "COM1:2400,n,8,1" );

         if ( registry.SetValue( TEXT( "PagerCOMPort" ), unknown ) == FALSE )
         {
            LPVOID message_buffer = (LPVOID) NULL;

            ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                             NULL,
                             registry.GetErrorCode(),
                             MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
                   (LPTSTR) &message_buffer,
                             0,
                             NULL );

            LPCTSTR string_array[ 2 ];

            string_array[ 0 ] = TEXT( "SYSTEM\\CurrentControlSet\\Services\\EventPager\\Parameters\\PagerCOMPort" );
            string_array[ 1 ] = (LPCTSTR) message_buffer;

            CEventLog event_log( APPLICATION_NAME );
            event_log.Report( CEventLog::eventError, 0, MSG_CANT_SET_REGISTRY_ENTRY, 2, (LPCTSTR *) string_array );

            if ( message_buffer != NULL )
            {
               ::LocalFree( message_buffer );
            }
         }
      }
      else
      {
         LPVOID message_buffer = (LPVOID) NULL;

         ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                          NULL,
                          registry.GetErrorCode(),
                          MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
                (LPTSTR) &message_buffer,
                          0,
                          NULL );

         LPCTSTR string_array[ 2 ];

         string_array[ 0 ] = TEXT( "SYSTEM\\CurrentControlSet\\Services\\Watch\\Parameters" );
         string_array[ 1 ] = (LPCTSTR) message_buffer;

         CEventLog event_log( APPLICATION_NAME );
         event_log.Report( CEventLog::eventError, 0, MSG_CANT_CREATE_REGISTRY_KEY, 2, (LPCTSTR *) string_array );

         if ( message_buffer != NULL )
         {
            ::LocalFree( message_buffer );
         }
      }
   }
   else
   {
      LPVOID message_buffer = (LPVOID) NULL;

      ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                       NULL,
                       registry.GetErrorCode(),
                       MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
             (LPTSTR) &message_buffer,
                       0,
                       NULL );

      LPCTSTR string_array[ 2 ];

      string_array[ 0 ] = TEXT( "keyLocalMachine" );
      string_array[ 1 ] = (LPCTSTR) message_buffer;

      CEventLog event_log( APPLICATION_NAME );
      event_log.Report( CEventLog::eventError, 0, MSG_CANT_CONNECT_TO_REGISTRY, 2, (LPCTSTR *) string_array );

      if ( message_buffer != NULL )
      {
         ::LocalFree( message_buffer );
      }
   }
}
