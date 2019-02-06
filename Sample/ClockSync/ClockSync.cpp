#define WFC_STL

#if ! defined( WINVER )
#define WINVER 0x0401
#endif

#define _WIN32_WINNT 0x0401
#include <wfc.h>
#pragma hdrstop

USING_WFC_NAMESPACE

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
**
** $Workfile: ClockSync.cpp $
** $Revision: 6 $
** $Modtime: 2/11/00 4:24a $
*/

#if defined( _DEBUG )
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD WINAPI worker_thread( LPVOID parameter );

#define APPLICATION_NAME TEXT( "ClockSync" )
#define HALF_AN_HOUR      1800
#define ONE_HOUR          3600
#define FOUR_HOURS        14400
#define EIGHT_HOURS       28800
#define FORTY_NINE_DAYS   4233600
#define MINIMUM_INTERVAL  HALF_AN_HOUR
#define MAXIMUM_INTERVAL  FORTY_NINE_DAYS

void set_time_server( const CString& server_name )
{
   WFCTRACEINIT( TEXT( "set_time_server()" ) );

   CRegistry registry;

   if ( registry.Connect( HKEY_LOCAL_MACHINE ) != FALSE )
   {
      if ( registry.Open( TEXT( "SYSTEM\\CurrentControlSet\\Services\\ClockSync\\Parameters" ) ) != FALSE )
      {
         WFCTRACEVAL( TEXT( "Setting server to " ), server_name );
         registry.SetValue( TEXT( "Server" ), server_name );
      }
      else
      {
         WFCTRACE( TEXT( "Can't open SYSTEM\\CurrentControlSet\\Services\\ClockSync\\Parameters" ) );
      }
   }
   else
   {
      WFCTRACE( TEXT( "Can't Connect to local machine's registry" ) );
   }
}

int __cdecl _tmain( int argc, LPCTSTR argv[] )
{
   WFCTRACEINIT( TEXT( "main()" ) );

   if ( argc < 2 )
   {
      CService service( worker_thread );
      service.Initialize( APPLICATION_NAME );
      return( EXIT_SUCCESS );
   }

   if ( _tcsicmp( argv[ 1 ], TEXT( "install" ) ) == 0 )
   {
      WFCTRACE( TEXT( "install" ) );

      CServiceControlManager service_control_manager;

      if ( service_control_manager.Open() == FALSE )
      {
         WFCTRACEVAL( TEXT( "service_control_manager.Open() returned " ), service_control_manager.GetErrorCode() );
         _tprintf( TEXT( "Can't open service control manager failed, reason %lu\n" ), service_control_manager.GetErrorCode() );
         return( EXIT_SUCCESS );
      }

      if ( service_control_manager.Install( APPLICATION_NAME, APPLICATION_NAME, TEXT( "%SystemRoot%\\System32\\ClockSync.exe" ) ) == FALSE )
      {
         WFCTRACEVAL( TEXT( "service_control_manager.Install() returned " ), service_control_manager.GetErrorCode() );
         _tprintf( TEXT( "Install failed. See Application Log for details\n" ) );
         return( EXIT_SUCCESS );
      }

      set_time_server( TEXT( "You Need To Specify A Machine Name Here" ) );

      if ( argc > 2 )
      {
         CString server_name( argv[ 2 ] );
         set_time_server( server_name );
      }
      else
      {
         _tprintf( TEXT( "WARNING! No server name was specified. You will nee to edit SYSTEM\\CurrentControlSet\\Services\\ClockSync\\Parameters\\Server\n" ) );
      }

      return( EXIT_SUCCESS );
   }
   else if ( _tcsicmp( argv[ 1 ], TEXT( "remove" ) ) == 0 )
   {
      WFCTRACE( TEXT( "remove" ) );

      CServiceControlManager service_control_manager;

      if ( service_control_manager.Open() == FALSE )
      {
         WFCTRACEVAL( TEXT( "service_control_manager.Open() returned " ), service_control_manager.GetErrorCode() );
         _tprintf( TEXT( "Can't open service control manager failed, reason %lu\n" ), service_control_manager.GetErrorCode() );
         return( EXIT_SUCCESS );
      }

      if ( service_control_manager.Remove( APPLICATION_NAME ) == FALSE )
      {
         WFCTRACEVAL( TEXT( "service_control_manager.Remove() returned " ), service_control_manager.GetErrorCode() );
         _tprintf( TEXT( "Remove failed. See Application Log for details\n" ) );
      }

      return( EXIT_SUCCESS );
   }
   else if ( _tcsicmp( argv[ 1 ], TEXT( "run" ) ) == 0 )
   {
      WFCTRACE( TEXT( "run" ) );
      worker_thread( reinterpret_cast<VOID *>( 99 ) );
   }
   else
   {
      _tprintf( TEXT( "Usage: %s [install [time server name]|remove|run]\n" ), argv[ 0 ] );
   }

   return( EXIT_SUCCESS );
}

DWORD WINAPI worker_thread( LPVOID parameter )
{
   WFCTRACEINIT( TEXT( "worker_thread()" ) );

   CString server_name;
   CString log_message;

   DWORD poll_interval                   = 0;
   DWORD number_of_milliseconds_to_sleep = 0;
   DWORD span                            = 0;

   {
      CRegistry registry;

      if ( registry.Connect( HKEY_LOCAL_MACHINE ) == FALSE )
      {
         log_message.Format( TEXT( "Can't connect to registry at line %d" ), __LINE__ );
         CEventLog event_log;
         event_log.ReportError( log_message );
         return( 0 );
      }

      CString key_name( TEXT( "SYSTEM\\CurrentControlSet\\Services\\ClockSync\\Parameters" ) );

      if ( registry.Open( key_name ) == FALSE )
      {
         log_message.Format( TEXT( "Can't open SYSTEM\\CurrentControlSet\\Services\\ClockSync\\Parameters at line %d" ), __LINE__ );
         CEventLog event_log;
         event_log.ReportError( log_message );
         return( 0 );
      }

      registry.GetValue( TEXT( "Server" ), server_name );

      if ( server_name.IsEmpty() )
      {
         log_message.Format( TEXT( "SYSTEM\\CurrentControlSet\\Services\\ClockSync\\Parameters\\Server is empty" ) );
         CEventLog event_log;
         event_log.ReportError( log_message );
         return( 0 );
      }

      if ( server_name.CompareNoCase( TEXT( "You Need To Specify A Machine Name Here" ) ) == 0 )
      {
         log_message.Format( TEXT( "SYSTEM\\CurrentControlSet\\Services\\ClockSync\\Parameters\\Server is still set to the default. You need to change this." ) );
         CEventLog event_log;
         event_log.ReportError( log_message );
         return( 0 );
      }
   }

   CSystemTime server_time;
   CSystemTime local_machine_time;

   CTime local_ctime;
   CTime server_ctime;

   CTimeSpan difference;

   CNetwork server( server_name );

   while( 1 )
   {
      if ( server.GetTime( server_time ) != FALSE )
      {
         GetSystemTime( &local_machine_time );

         if ( SetSystemTime( &server_time ) == FALSE )
         {
            DWORD last_error = GetLastError();
            log_message.Format( TEXT( "Can't set the time! Error code %lu" ), last_error );
            CEventLog event_log;
            event_log.ReportError( log_message );
            poll_interval = ONE_HOUR;
         }
         else
         {
            server_time.CopyTo( server_ctime );
            local_machine_time.CopyTo( local_ctime );
            difference   = CTimeSpan( 0 );

            if ( local_ctime != server_ctime )
            {
               // We need to adjust the polling interval somehow

               if ( local_ctime > server_ctime )
               {
                  difference = local_ctime - server_ctime;
               }
               else
               {
                  difference = server_ctime - local_ctime;
               }

               span = difference.GetTotalSeconds();

               if ( span >= 2 )
               {
                  // 2 seconds, let's poll a little more often

                  if ( poll_interval > ONE_HOUR )
                  {
                     poll_interval -= ONE_HOUR;
                  }

                  // Don't poll more than once every thirty minutes

                  if ( poll_interval < HALF_AN_HOUR )
                  {
                     poll_interval = HALF_AN_HOUR;
                  }
               }
               else
               {
                  // only one second off, let's ask a little less frequently

                  poll_interval += FOUR_HOURS;
               }
            }
            else
            {
               // Times are EQUAL! WOW! Let's ask a little less frequently

               poll_interval += EIGHT_HOURS;
            }

            if ( poll_interval > MAXIMUM_INTERVAL )
            {
               poll_interval = MAXIMUM_INTERVAL;
            }

            if ( poll_interval < MINIMUM_INTERVAL )
            {
               poll_interval = MINIMUM_INTERVAL;
            }

            difference = CTimeSpan( poll_interval );

            local_ctime = CTime::GetCurrentTime();
            local_ctime += difference;

            {
               CEventLog event_log;

               log_message.Format( TEXT( "Difference with %s was %lu seconds, will synchronize again at %s" ),
                         (LPCTSTR) server_name,
                                   span,
                         (LPCTSTR) local_ctime.Format( TEXT( "%H:%M:%S %d %b %y" ) ) );

               WFCTRACE( log_message );

               event_log.ReportInformation( log_message );
            }
         }
      }
      else
      {
         CEventLog event_log;
         log_message.Format( TEXT( "Can't get time from %s, error code is %lu" ), (LPCTSTR) server_name, server.GetErrorCode() );
         event_log.ReportError( log_message );
         poll_interval = FOUR_HOURS;
      }

      number_of_milliseconds_to_sleep = poll_interval * 1000;

      Sleep( number_of_milliseconds_to_sleep );
   }
}
