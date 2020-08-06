/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2020, Samuel R. Blackburn
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
** $Workfile: CServiceApplication.cpp $
** $Revision: 9 $
** $Modtime: 6/26/01 10:51a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CServiceApplication::CServiceApplication(_In_ int const number_of_command_line_arguments,
    __in_ecount_z_opt(number_of_command_line_arguments) LPCTSTR command_line_arguments[],
    __in_z LPCTSTR service_name,
    __in_z LPCTSTR friendly_service_name,
    __in_z LPCTSTR executable_path,
    __callback LPTHREAD_START_ROUTINE worker_thread,
    __callback SHOW_USAGE_ROUTINE show_usage,
    __callback SET_DEFAULTS_ROUTINE set_defaults,
    _In_ DWORD const installation_configuration)
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER(worker_thread);

    (void)m_Main(number_of_command_line_arguments,
        command_line_arguments,
        service_name,
        friendly_service_name,
        executable_path,
        installation_configuration,
        worker_thread,
        show_usage,
        set_defaults);
}

Win32FoundationClasses::CServiceApplication::~CServiceApplication(void)
{
    WFC_VALIDATE_POINTER(this);
}

_Check_return_ int Win32FoundationClasses::CServiceApplication::m_Main(_In_ int const number_of_command_line_arguments,
    __in_ecount_z_opt(number_of_command_line_arguments) LPCTSTR command_line_arguments[],
    __in_z LPCTSTR service_name,
    __in_z LPCTSTR friendly_service_name,
    __in_z LPCTSTR executable_path,
    _In_ DWORD const installation_configuration,
    __callback LPTHREAD_START_ROUTINE worker_thread,
    __callback SHOW_USAGE_ROUTINE show_usage,
    __callback SET_DEFAULTS_ROUTINE set_defaults) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (number_of_command_line_arguments == 1)
    {
        CService service(worker_thread);

        (void)service.Initialize(service_name);

        return(EXIT_SUCCESS);
    }

    if (number_of_command_line_arguments > 1 and command_line_arguments not_eq nullptr)
    {
        if (_tcsicmp(command_line_arguments[1], TEXT("install")) == 0)
        {
            Win32FoundationClasses::CServiceControlManager service_control_manager;

            if (service_control_manager.Open() == false)
            {
                //WFCTRACEERROR( service_control_manager.GetErrorCode() );
                _tprintf(TEXT("Can't open service control manager, INSTALL FAILED!\n"));
                return(EXIT_FAILURE);
            }

            if (service_control_manager.Install(service_name, friendly_service_name, executable_path) == false)
            {
                //WFCTRACEERROR( service_control_manager.GetErrorCode() );
                _tprintf(TEXT("Install failed. Please see Application Log for details.\n"));
                return(EXIT_FAILURE);
            }

            if (service_control_manager.SetConfiguration(service_name,
                installation_configuration,
                SERVICE_NO_CHANGE,
                SERVICE_NO_CHANGE,
                executable_path) == false)
            {
                //WFCTRACEERROR( service_control_manager.GetErrorCode() );
                _tprintf(TEXT("Can't set service configuration.\n"));
                return(EXIT_FAILURE);
            }

            if (set_defaults not_eq nullptr)
            {
                // We were passed a pointer, don't trust it...

                WFC_TRY
                {
                   set_defaults();
                }
                    WFC_CATCH_ALL
                {
                }
                WFC_END_CATCH_ALL
            }

            return(EXIT_SUCCESS);
        }
        else if (_tcsicmp(command_line_arguments[1], TEXT("remove")) == I_AM_EQUAL_TO_THAT)
        {
            CServiceControlManager service_control_manager;

            if (service_control_manager.Open() == false)
            {
                //WFCTRACEERROR( service_control_manager.GetErrorCode() );
                _tprintf(TEXT("Can't open service control manager, attempting to carry on...\n"));
            }

            if (service_control_manager.Remove(service_name) == false)
            {
                //WFCTRACEERROR( service_control_manager.GetErrorCode() );
                std::wstring error_message;

                service_control_manager.GetErrorMessage(error_message);

                _tprintf(TEXT("Can't remove service, see Application Log for details. Error code %lu, %s\n"), service_control_manager.GetErrorCode(), error_message.c_str());
                return(EXIT_FAILURE);
            }

            return(EXIT_SUCCESS);
        }
        else if (_tcsicmp(command_line_arguments[1], TEXT("run")) == 0)
        {
            worker_thread((LPVOID)1);
            return(EXIT_SUCCESS);
        }
        else
        {
            if (show_usage not_eq nullptr)
            {
                // We were passed a pointer, don't trust it...

                WFC_TRY
                {
                   show_usage(command_line_arguments[0]);
                }
                    WFC_CATCH_ALL
                {
                }
                WFC_END_CATCH_ALL
            }
        }
    }
    else
    {
        if (show_usage not_eq nullptr)
        {
            // We were passed a pointer, don't trust it...

            WFC_TRY
            {
                if (command_line_arguments not_eq nullptr)
                {
                   show_usage(command_line_arguments[0]);
                }
            }
                WFC_CATCH_ALL
            {
            }
            WFC_END_CATCH_ALL
        }
    }

    return(EXIT_SUCCESS);
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CServiceApplication</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that makes it easy to create NT services.">
</HEAD>

<BODY>

<H1>CServiceApplication</H1>
$Revision: 9 $
<HR>

<H2>Description</H2>

This class encapsulates the basic logic behind a service application. It
takes care of installing, removing and starting a service as a console
application.

<H2>Construction</H2>

<DL COMPACT>

<DT><PRE><B>CServiceApplication</B>( int number_of_command_line_arguments,
                           LPCTSTR command_line_arguments[],
                           LPCTSTR service_name,
                           LPCTSTR friendly_service_name,
                           LPCTSTR executable_path,
                           LPTHREAD_START_ROUTINE worker_thread,
                           SHOW_USAGE_ROUTINE show_usage,
                           SET_DEFAULTS_ROUTINE set_defaults,
                           DWORD   installation_configuration )</PRE><DD>
That's pretty much it.

</DL>

<H2>Data Members</H2>

None.

<H2>Methods</H2>

None.

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;include &quot;messages.h&quot;
&#35;pragma hdrstop

DWORD WINAPI worker_thread( LPVOID pointer_to_parent_CService_class );
VOID set_default_parameters( void );

int __cdecl _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   <B>CServiceApplication</B> application( number_of_command_line_arguments,
                                    command_line_arguments,
                                    TEXT( &quot;WatchDog&quot; ),
                                    TEXT( &quot;Service Watch Dog&quot; ),
                                    command_line_arguments[ 0 ],
                                    worker_thread,
                                    nullptr,
                                    set_default_parameters );

   return( EXIT_SUCCESS );
}

DWORD WINAPI worker_thread( LPVOID )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;worker_thread()&quot; ) );

   {
      <A HREF="CEVNTLOG.htm">CEventLog</A> log( TEXT( &quot;WatchDog&quot; ) );
      log.Report( CEventLog::eventInformation, 0, MSG_WATCHDOG_SERVICE_STARTED );
   }

   std::vector&lt;std::wstring&gt; names_of_services_to_keep_alive;

   DWORD number_of_seconds_to_sleep = 0;

   std::wstring machine_name( TEXT( &quot;&quot; ) );

   BOOL return_value = TRUE;

   {
      <A HREF="REGISTRY.htm">CRegistry</A> registry;

      if ( registry.Connect( CRegistry::keyLocalMachine ) == FALSE )
      {
         return( 0 );
      }

      std::wstring key_name( TEXT( &quot;SYSTEM\\CurrentControlSet\\Services\\WatchDog\\Parameters&quot; ) );

      if ( registry.Open( key_name ) == FALSE )
      {
         return( 0 );
      }

      registry.GetValue( TEXT( &quot;Services&quot; ), names_of_services_to_keep_alive );
      registry.GetValue( TEXT( &quot;NumberOfSecondsBetweenChecks&quot; ), number_of_seconds_to_sleep );
      registry.GetValue( TEXT( &quot;MachineName&quot; ), machine_name );
   }

   DWORD sleep_time = 1000 * number_of_seconds_to_sleep;

   if ( sleep_time &lt; 2000 )
   {
      // Minimum sleep time is 2 seconds, this give the OS time to do other things
      sleep_time = 2000;
   }

   int number_of_services_to_keep_alive = names_of_services_to_keep_alive.GetSize();

   <A HREF="CSvcMgr.htm">CServiceControlManager</A> service_control_manager;

   /&#42;
   ** Sleep for one minute, this is in case we are starting during boot-up. We want
   ** to give the service control manager time to start all necessary services before
   ** we begin restarting stopped services.
   &#42;/

   std::this_thread::sleep_for(std::chrono::milliseconds(60000));

   do
   {
      if ( machine_name.IsEmpty() )
      {
         return_value = service_control_manager.Open( GENERIC_READ, nullptr, nullptr );
      }
      else
      {
         return_value = service_control_manager.Open( GENERIC_READ, nullptr, machine_name );
      }

      if ( return_value not_eq FALSE )
      {
         if ( service_control_manager.EnumerateStatus( SERVICE_INACTIVE ) not_eq FALSE )
         {
            std::vector&lt;std::wstring&gt; stopped_services;

            CServiceNameAndStatus status;

            while( service_control_manager.GetNext( status ) == true )
            {
               stopped_services.Add( status.lpServiceName );
            }

            // Now that we have the service names, we need to see which services need to be started

            int number_of_stopped_services = stopped_services.GetSize();
            int alive_index                = 0;
            int stopped_index              = 0;

            while( alive_index &lt; number_of_services_to_keep_alive )
            {
               stopped_index = 0;

               while( stopped_index &lt; number_of_stopped_services )
               {
                  if ( names_of_services_to_keep_alive[ alive_index ].CompareNoCase( stopped_services[ stopped_index ] ) == 0 )
                  {
                     // We found one that died, let's start it

                     service_control_manager.Start( names_of_services_to_keep_alive[ alive_index ] );

                     // We restarted a service, time to record the event

                     LPCTSTR string_array[ 1 ];

                     string_array[ 0 ] = (LPCTSTR) names_of_services_to_keep_alive[ alive_index ];

                     <A HREF="CEVNTLOG.htm">CEventLog</A> event_log( TEXT( &quot;WatchDog&quot; ) );

                     event_log.Report( <A HREF="CEVNTLOG.htm">CEventLog</A>::eventInformation,
                                       0,
                                       MSG_WATCHDOG_RESTARTING_SERVICE,
                                       1,
                           (LPCTSTR *) string_array );

                     // pop out of the loop

                     stopped_index = number_of_stopped_services;
                  }

                  stopped_index++;
               }

               alive_index++;
            }
         }
      }

      service_control_manager.Close();

      std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
   }
   while( 1 );

   return( 0 );
}

void set_default_parameters( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;set_default_parameters()&quot; ) );

   <A HREF="REGISTRY.htm">CRegistry</A> registry;

   if ( registry.Connect( CRegistry::keyLocalMachine ) not_eq FALSE )
   {
      if ( registry.Create( TEXT( &quot;SYSTEM\\CurrentControlSet\\Services\\WatchDog\\Parameters&quot; ) ) not_eq FALSE )
      {
         DWORD default_sleep_time = 60;

         if ( registry.SetValue( TEXT( &quot;NumberOfSecondsBetweenChecks&quot; ), default_sleep_time ) == FALSE )
         {
            LPVOID message_buffer = (LPVOID) nullptr;

            ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                             nullptr,
                             registry.GetErrorCode(),
                             MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
                   (LPTSTR) &amp;message_buffer,
                             0,
                             nullptr );

            LPCTSTR string_array[ 2 ];

            string_array[ 0 ] = TEXT( &quot;SYSTEM\\CurrentControlSet\\Services\\WatchDog\\Parameters\\NumberOfSecondsBetweenChecks&quot; );
            string_array[ 1 ] = (LPCTSTR) message_buffer;

            <A HREF="CEVNTLOG.htm">CEventLog</A> event_log( TEXT( &quot;WatchDog&quot; ) );

            event_log.Report( <A HREF="CEVNTLOG.htm">CEventLog</A>::eventError,
                              0,
                              MSG_CANT_SET_REGISTRY_ENTRY,
                              2,
                  (LPCTSTR *) string_array );

            if ( message_buffer not_eq nullptr )
            {
               ::LocalFree( message_buffer );
            }
         }

         std::vector&lt;std::wstring&gt; strings;

         strings.RemoveAll();
         strings.Add( TEXT( &quot;&quot; ) );

         if ( registry.SetValue( TEXT( &quot;Services&quot; ), strings ) == FALSE )
         {
            LPVOID message_buffer = (LPVOID) nullptr;

            ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                             nullptr,
                             registry.GetErrorCode(),
                             MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
                   (LPTSTR) &amp;message_buffer,
                             0,
                             nullptr );

            LPCTSTR string_array[ 2 ];

            string_array[ 0 ] = TEXT( &quot;SYSTEM\\CurrentControlSet\\Services\\WatchDog\\Parameters\\Services&quot; );
            string_array[ 1 ] = (LPCTSTR) message_buffer;

            <A HREF="CEVNTLOG.htm">CEventLog</A> event_log( TEXT( &quot;WatchDog&quot; ) );

            event_log.Report( <A HREF="CEVNTLOG.htm">CEventLog</A>::eventError, 0, MSG_CANT_SET_REGISTRY_ENTRY, 2, (LPCTSTR *) string_array );

            if ( message_buffer not_eq nullptr )
            {
               ::LocalFree( message_buffer );
            }
         }
      }
      else
      {
         LPVOID message_buffer = (LPVOID) nullptr;

         ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                          nullptr,
                          registry.GetErrorCode(),
                          MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
                (LPTSTR) &amp;message_buffer,
                          0,
                          nullptr );

         LPCTSTR string_array[ 2 ];

         string_array[ 0 ] = TEXT( &quot;SYSTEM\\CurrentControlSet\\Services\\WatchDog\\Parameters&quot; );
         string_array[ 1 ] = (LPCTSTR) message_buffer;

         <A HREF="CEVNTLOG.htm">CEventLog</A> event_log( &quot;WatchDog&quot; );

         event_log.Report( <A HREF="CEVNTLOG.htm">CEventLog</A>::eventError, 0, MSG_CANT_CREATE_REGISTRY_KEY, 2, (LPCTSTR *) string_array );

         if ( message_buffer not_eq nullptr )
         {
            ::LocalFree( message_buffer );
         }
      }
   }
   else
   {
      LPVOID message_buffer = (LPVOID) nullptr;

      ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                       nullptr,
                       registry.GetErrorCode(),
                       MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
             (LPTSTR) &amp;message_buffer,
                       0,
                       nullptr );

      LPCTSTR string_array[ 2 ];

      string_array[ 0 ] = TEXT( &quot;keyLocalMachine&quot; );
      string_array[ 1 ] = (LPCTSTR) message_buffer;

      <A HREF="CEVNTLOG.htm">CEventLog</A> event_log( TEXT( &quot;WatchDog&quot; ) );

      event_log.Report( <A HREF="CEVNTLOG.htm">CEventLog</A>::eventError, 0, MSG_CANT_CONNECT_TO_REGISTRY, 2, (LPCTSTR *) string_array );

      if ( message_buffer not_eq nullptr )
      {
         ::LocalFree( message_buffer );
      }
   }
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CServiceApplication.cpp $<BR>
$Modtime: 6/26/01 10:51a $
</BODY>

</HTML>
*/
