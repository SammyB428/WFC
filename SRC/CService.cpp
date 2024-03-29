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
** $Workfile: cservice.cpp $
** $Revision: 41 $
** $Modtime: 6/26/01 10:51a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#define new DEBUG_NEW
#endif // _DEBUG

CRITICAL_SECTION Win32FoundationClasses::g_ServiceCriticalSection;

Win32FoundationClasses::CService* Win32FoundationClasses::CService::m_StaticService_p{ nullptr };

Win32FoundationClasses::CService::CService( __callback LPTHREAD_START_ROUTINE thread_start_routine, _In_ DWORD const controls_accepted, _In_ DWORD const wait_hint ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( thread_start_routine );

   ::InitializeCriticalSection( &g_ServiceCriticalSection );

   m_ThreadStartRoutine  = thread_start_routine;
   m_ThreadHandle        = static_cast< HANDLE >( NULL );
   m_ExitEventHandle     = static_cast< HANDLE >( NULL );
   m_ServiceStatusHandle = 0;
   m_ErrorCode           = NO_ERROR;
   m_Running             = false;
   m_Paused              = false;
   m_Exiting             = false;
   m_Debugging           = false;
   m_ControlsAccepted    = controls_accepted;
   m_WaitHint            = wait_hint;
   m_CurrentState        = SERVICE_START_PENDING;
   m_StaticService_p     = this;
   m_ThreadId            = 0;

   m_OnShutdown = nullptr;
   m_OnStop = nullptr;
   m_OnPause = nullptr;
   m_OnContinue = nullptr;
   m_ServiceControlManagerCallbackParameter = nullptr;

   CommandLineParameters.clear();

   ZeroMemory( m_ServiceTable, sizeof( m_ServiceTable ) );

   m_ServiceName = static_cast<wchar_t *>(_aligned_malloc((SERVICE_NAME_LEN + 1) * sizeof( wchar_t ), 4096));

   ZeroMemory( m_ServiceName, (SERVICE_NAME_LEN + 1) * sizeof(wchar_t));
}

Win32FoundationClasses::CService::~CService( void )
{
   WFC_VALIDATE_POINTER( this );

   ::DeleteCriticalSection( &g_ServiceCriticalSection );

   if ( m_ExitEventHandle not_eq static_cast< HANDLE >( NULL ) )
   {
       std::ignore = Win32FoundationClasses::wfc_close_handle( m_ExitEventHandle );
      m_ExitEventHandle = static_cast< HANDLE >( NULL );
   }

   if ( m_ThreadHandle not_eq static_cast< HANDLE >( NULL ) )
   {
       std::ignore = Win32FoundationClasses::wfc_close_handle( m_ThreadHandle );
      m_ThreadHandle = static_cast< HANDLE >( NULL );
   }

   CommandLineParameters.clear();

   if (m_ServiceName not_eq nullptr)
   {
       _aligned_free(m_ServiceName);
       m_ServiceName = nullptr;
   }
}

#if defined( _DEBUG )

void Win32FoundationClasses::CService::AssertValid( void ) const
{
   ASSERT( m_Exiting             == false );
   ASSERT( m_ExitEventHandle     not_eq static_cast< HANDLE >( NULL )  );
   ASSERT( m_ServiceStatusHandle not_eq 0     );
   ASSERT( m_ThreadHandle        not_eq static_cast< HANDLE >( NULL )  );
   ASSERT( m_StaticService_p     not_eq 0     );
}

#endif // _DEBUG

_Check_return_ HANDLE Win32FoundationClasses::CService::CreateConfigurationFile( __in_z LPCTSTR filename )  noexcept // static
{
   WFC_VALIDATE_POINTER( filename );

   // We need to create a file that *ANY* process on the system can
   // open. To do this we, we need to create the file with a NULL DACL.
   // Please read Knowledge Base article Q106387.

   SECURITY_ATTRIBUTES security_attributes;

   security_attributes.nLength              = sizeof( security_attributes );
   security_attributes.lpSecurityDescriptor = (void *)Win32FoundationClasses::wfc_create_null_dacl();
   security_attributes.bInheritHandle       = TRUE;

   if ( security_attributes.lpSecurityDescriptor == nullptr )
   {
      return( INVALID_HANDLE_VALUE );
   }

   auto const file_handle{ ::CreateFile(filename,
                                    GENERIC_WRITE,
                                    FILE_SHARE_READ,
                                   &security_attributes,
                                    CREATE_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL,
                                    nullptr) };
   
   wfc_destroy_null_dacl( security_attributes.lpSecurityDescriptor );

   return( file_handle );
}

_Check_return_ bool Win32FoundationClasses::CService::SpawnProcess( __in_z LPCTSTR command_line, __in_z LPCTSTR current_directory, _Out_ DWORD& last_error ) noexcept // static
{
   TCHAR non_const_command_line[ 2048 ];

   _tcsncpy_s( non_const_command_line, std::size( non_const_command_line ), command_line, std::size( non_const_command_line ) );

   DWORD creation_flags{ CREATE_NO_WINDOW bitor CREATE_NEW_CONSOLE };

   CStartupInformationW startup_information;

   wchar_t empty_string[1]{ 0 };

   startup_information.lpDesktop = empty_string;
   startup_information.HideWindow();

   PROCESS_INFORMATION process_information;

   ZeroMemory( &process_information, sizeof( process_information ) );

   SECURITY_ATTRIBUTES security_attributes;

   security_attributes.nLength              = sizeof( security_attributes );
   security_attributes.lpSecurityDescriptor = (void *) wfc_create_null_dacl();
   security_attributes.bInheritHandle       = TRUE;

   if ( ::CreateProcess( nullptr, // THIS MUST BE NULL IN ORDER TO WORK!
                       non_const_command_line,
                      &security_attributes,
                      &security_attributes,
                       TRUE,
                       creation_flags,
                       nullptr,
                       current_directory,
                      &startup_information,
                      &process_information ) == FALSE )
   {
      last_error = ::GetLastError();
      wfc_destroy_null_dacl( security_attributes.lpSecurityDescriptor );
      return( false );
   }

   wfc_destroy_null_dacl( security_attributes.lpSecurityDescriptor );
   CloseHandle( process_information.hProcess );
   CloseHandle( process_information.hThread );

   last_error = NO_ERROR;

   return( true );
}

_Check_return_ INT_PTR Win32FoundationClasses::CService::DialogBoxParam( _In_ HINSTANCE instance, __in_z LPCTSTR template_name, _In_ HWND parent_window, __callback DLGPROC dialogbox_procedure, _In_ LPARAM lParam ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // You must have SERVICE_INTERACTIVE_PROCESS for this to work

   auto old_desktop_handle{ ::GetThreadDesktop(::GetCurrentThreadId()) };
   auto desktop_handle{ ::OpenInputDesktop(0, FALSE, DESKTOP_CREATEWINDOW) };

   ::SetThreadDesktop( desktop_handle );

   auto return_value{ ::DialogBoxParam(instance,
       template_name,
       parent_window,
       dialogbox_procedure,
       lParam) };

   ::SetThreadDesktop( old_desktop_handle );

   return( return_value );
}

#if defined( _DEBUG )

void Win32FoundationClasses::CService::DumpStatus( __inout SERVICE_STATUS *status_p ) const
{
   // The user passed us a pointer, don't trust it

   WFC_TRY
   {
      switch( status_p->dwServiceType )
      {
         case SERVICE_WIN32_OWN_PROCESS:

            //WFCTRACE( TEXT( "dwServiceType SERVICE_WIN32_OWN_PROCESS" ) );
            break;

         case SERVICE_WIN32_SHARE_PROCESS:

            //WFCTRACE( TEXT( "dwServiceType SERVICE_WIN32_SHARE_PROCESS" ) );
            break;

         case SERVICE_KERNEL_DRIVER:

            //WFCTRACE( TEXT( "dwServiceType SERVICE_KERNEL_DRIVER" ) );
            break;

         case SERVICE_FILE_SYSTEM_DRIVER:

            //WFCTRACE( TEXT( "dwServiceType SERVICE_FILE_SYSTEM_DRIVER" ) );
            break;

         case SERVICE_INTERACTIVE_PROCESS:

            //WFCTRACE( TEXT( "dwServiceType SERVICE_INTERACTIVE_PROCESS" ) );
            break;

         default:

            //WFCTRACEVAL( TEXT( "Unknown dwServiceType " ), status_p->dwServiceType );
            break;
      }

      //WFCTRACE( TEXT( "   SERVICE_CONTROL_INTERROGATE" ) );

      if ( is_flagged(status_p->dwControlsAccepted, SERVICE_ACCEPT_STOP) == true )
      {
         //WFCTRACE( TEXT( "   SERVICE_CONTROL_STOP" ) );
      }

      if (is_flagged(status_p->dwControlsAccepted, SERVICE_ACCEPT_PAUSE_CONTINUE) == true )
      {
         //WFCTRACE( TEXT( "   SERVICE_CONTROL_PAUSE" ) );
         //WFCTRACE( TEXT( "   SERVICE_CONTROL_CONTINUE" ) );
      }

      if (is_flagged(status_p->dwControlsAccepted, SERVICE_ACCEPT_SHUTDOWN) == true )
      {
         //WFCTRACE( TEXT( "   SERVICE_CONTROL_SHUTDOWN" ) );
      }

      switch( status_p->dwCurrentState )
      {
         case SERVICE_STOPPED:

            //WFCTRACE( TEXT( "dwCurrentState SERVICE_STOPPED" ) );
            break;

         case SERVICE_START_PENDING:

            //WFCTRACE( TEXT( "dwCurrentState SERVICE_START_PENDING" ) );
            break;

         case SERVICE_STOP_PENDING:

            //WFCTRACE( TEXT( "dwCurrentState SERVICE_STOP_PENDING" ) );
            break;

         case SERVICE_RUNNING:

            //WFCTRACE( TEXT( "dwCurrentState SERVICE_RUNNING" ) );
            break;

         case SERVICE_CONTINUE_PENDING:

            //WFCTRACE( TEXT( "dwCurrentState SERVICE_CONTINUE_PENDING" ) );
            break;

         case SERVICE_PAUSE_PENDING:

            //WFCTRACE( TEXT( "dwCurrentState SERVICE_PAUSE_PENDING" ) );
            break;

         case SERVICE_PAUSED:

            //WFCTRACE( TEXT( "dwCurrentState SERVICE_PAUSED" ) );
            break;

         default:

            //WFCTRACEVAL( TEXT( "dwCurrentState " ), status_p->dwCurrentState );
            break;
      }

      if ( status_p->dwWin32ExitCode == ERROR_SERVICE_SPECIFIC_ERROR )
      {
         //WFCTRACEVAL( TEXT( "dwServiceSpecificExitCode " ), status_p->dwServiceSpecificExitCode );
      }
      else
      {
         //WFCTRACEVAL( TEXT( "dwWin32ExitCode " ), status_p->dwWin32ExitCode );
      }

      //WFCTRACEVAL( TEXT( "dwCheckPoint " ), status_p->dwCheckPoint );
      //WFCTRACEVAL( TEXT( "dwWaitHint " ), status_p->dwWaitHint );
   }
   WFC_CATCH_ALL
   {
      ; // Do Nothing
   }
   WFC_END_CATCH_ALL
}

#endif // _DEBUG

void Win32FoundationClasses::CService::Exit( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ::EnterCriticalSection( &g_ServiceCriticalSection );

   m_Running      = false;
   m_CurrentState = SERVICE_STOPPED;
   m_Exiting      = true;

   ::LeaveCriticalSection( &g_ServiceCriticalSection );

   if ( m_ExitEventHandle not_eq static_cast< HANDLE >( NULL ) )
   {
      ::SetEvent( m_ExitEventHandle );
   }
}

_Check_return_ bool Win32FoundationClasses::CService::Initialize( __in_z LPCWSTR name_of_service ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( name_of_service );

   /*
   ** Thank you Rob Williams (CI$ 73740,774) for fixing this function
   */

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // initialize m_ServiceTable

      _tcsncpy_s( m_ServiceName, SERVICE_NAME_LEN, name_of_service, SERVICE_NAME_LEN );

      m_ServiceTable[ 0 ].lpServiceName = m_ServiceName;
      m_ServiceTable[ 0 ].lpServiceProc = (LPSERVICE_MAIN_FUNCTIONW) CService::ServiceMain;
      m_ServiceTable[ 1 ].lpServiceName = nullptr;
      m_ServiceTable[ 1 ].lpServiceProc = nullptr;

      // initiate conversation with SCM

      if ( ::StartServiceCtrlDispatcherW( m_ServiceTable ) == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACEERROR( m_ErrorCode );
         LogEvent();
         return(false);
      }

      return( true );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( false );
   }
   WFC_END_CATCH_ALL
}

#pragma warning( disable : 4100 )

void Win32FoundationClasses::CService::LogEvent( _In_ WORD event_type, __in_z_opt LPCTSTR message_string, _In_ DWORD error_code ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Win32FoundationClasses::CEventLog log( m_ServiceName );

   LPCTSTR strings[1]{ message_string };

   std::ignore = log.Report( (Win32FoundationClasses::CEventLog::EventType) event_type, 0, 0, 1, (LPCTSTR *) strings );
}

// I shouldn't have to put this here but WINUSER.H wasn't doing it right...
#ifdef _WIN32_WINNT
#if (_WIN32_WINNT >= 0x0400)
#define MB_SERVICE_NOTIFICATION          0x00200000L
#else
#define MB_SERVICE_NOTIFICATION          0x00040000L
#endif
#define MB_SERVICE_NOTIFICATION_NT3X     0x00040000L
#endif

#pragma warning( default : 4100 )

_Check_return_ int Win32FoundationClasses::CService::ShowMessageBox(__in_z_opt LPCTSTR text, __in_z_opt LPCTSTR caption, _In_ UINT type ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   COperatingSystemVersionInformation version_information;

   if ( version_information.Fill() == true )
   {
      if ( version_information.dwMajorVersion <= 3 )
      {
         type or_eq MB_SERVICE_NOTIFICATION_NT3X;
      }
      else
      {
         type or_eq MB_SERVICE_NOTIFICATION;
      }
   }

   auto const return_value{ ::MessageBox(static_cast<HWND>(NULL), text, caption, type) };

   return( return_value );
}

void Win32FoundationClasses::CService::OnContinue( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK(m_OnContinue);

   if ( m_OnContinue not_eq nullptr )
   {
      WFC_TRY
      {
         m_OnContinue(this, m_ServiceControlManagerCallbackParameter);
      }
      WFC_CATCH_ALL
      {
         m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      }
      WFC_END_CATCH_ALL
   }

   CEventLog log( m_ServiceName );
   log.ReportInformation( L"Service Resumed" );
}

void Win32FoundationClasses::CService::OnControlCode( _In_ DWORD /* control_code */ ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   // default implementation
   // handle user-defined control codes (128 - 255 inclusive)
}

void Win32FoundationClasses::CService::OnPause( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK(m_OnPause);

   if ( m_OnPause not_eq nullptr )
   {
      WFC_TRY
      {
         m_OnPause(this, m_ServiceControlManagerCallbackParameter);
      }
      WFC_CATCH_ALL
      {
         m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      }
      WFC_END_CATCH_ALL
   }

   CEventLog log( m_ServiceName );
   log.ReportInformation( L"Service Paused" );
}

_Check_return_ bool Win32FoundationClasses::CService::OnPrepareServiceThread( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( true );
}

void Win32FoundationClasses::CService::OnShutdown( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK(m_OnShutdown);

   if ( m_OnShutdown not_eq nullptr )
   {
      WFC_TRY
      {
         m_OnShutdown(this, m_ServiceControlManagerCallbackParameter);
      }
      WFC_CATCH_ALL
      {
         m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      }
      WFC_END_CATCH_ALL
   }
}

void Win32FoundationClasses::CService::OnStop( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK(m_OnStop);

   if ( m_OnStop not_eq nullptr )
   {
      WFC_TRY
      {
         m_OnStop(this, m_ServiceControlManagerCallbackParameter);
      }
      WFC_CATCH_ALL
      {
         m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      }
      WFC_END_CATCH_ALL
   }
}

void Win32FoundationClasses::CService::ParseCommandLineParameters( _In_ DWORD const number_of_command_line_arguments, _In_reads_z_( number_of_command_line_arguments ) LPCTSTR command_line_arguments[] ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   CommandLineParameters.clear();

   for ( auto const argument_number : Range(number_of_command_line_arguments) )
   {
      CommandLineParameters.push_back( command_line_arguments[ argument_number ] );
   }

   // default implementation
   // parse command line parameters passed via SCM through ServiceMain

   for( auto const argument_number : Range(number_of_command_line_arguments, StartingRangePosition(1)) )
   {
      if ( command_line_arguments[ argument_number ][ 0 ] == '-' or
           command_line_arguments[ argument_number ][ 0 ] == '/' )
      {
         switch( command_line_arguments[ argument_number ][ 1 ] )
         {
            case TEXT( 'd' ):
            case TEXT( 'D' ):

               ::EnterCriticalSection( &g_ServiceCriticalSection );
               m_Debugging = true;
               ::LeaveCriticalSection( &g_ServiceCriticalSection );

               break;

            case TEXT( 'i' ):
            case TEXT( 'I' ):

               TCHAR message_string[ 80 ];

               _stprintf_s( message_string, std::size( message_string ), TEXT( "pid %#lx %lu" ), ::GetCurrentProcessId(), ::GetCurrentProcessId() );
               ::MessageBoxW( static_cast< HWND >( NULL ), message_string, m_ServiceName, MB_OK );

               break;

            default:

               break;
        }
      }
   }
}

_Check_return_ bool Win32FoundationClasses::CService::SendStatusToServiceControlManager( _In_ DWORD current_state,
                                                                _In_ DWORD win32_exit_code,
                                                                _In_ DWORD check_point,
                                                                _In_ DWORD wait_hint,
                                                                _In_ DWORD service_specific_code ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   SERVICE_STATUS service_status;

   ::ZeroMemory( &service_status, sizeof( service_status ) );

   // initialize service_status and send it to SCM

   if ( current_state == SERVICE_START_PENDING )
   {
      service_status.dwControlsAccepted = 0;
   }
   else
   {
      service_status.dwControlsAccepted = m_ControlsAccepted;
   }

   if ( service_specific_code == 0 )
   {
      service_status.dwWin32ExitCode = win32_exit_code;
   }
   else
   {
      service_status.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
   }

   service_status.dwServiceType             = SERVICE_WIN32_OWN_PROCESS;
   service_status.dwCurrentState            = current_state;
   service_status.dwServiceSpecificExitCode = service_specific_code;
   service_status.dwCheckPoint              = check_point;
   service_status.dwWaitHint                = wait_hint;

#if defined( _DEBUG )
   DumpStatus( &service_status );
#endif

   if ( ::SetServiceStatus( m_ServiceStatusHandle, &service_status ) == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
      LogEvent();
      Exit();
      return(false);
   }

   return( true );
}

void CALLBACK Win32FoundationClasses::CService::ServiceControlManagerHandler( _In_ DWORD control_code )
{
   // entry point for service called by SCM after service is started

   WFC_VALIDATE_POINTER(m_StaticService_p);

   switch( control_code )
   {
      case SERVICE_CONTROL_STOP:

         std::ignore = m_StaticService_p->SendStatusToServiceControlManager( SERVICE_STOP_PENDING, NO_ERROR, 1, m_StaticService_p->m_WaitHint );

         ::EnterCriticalSection( &g_ServiceCriticalSection );
         m_StaticService_p->m_Running      = false;
         m_StaticService_p->m_CurrentState = SERVICE_STOPPED;
         ::LeaveCriticalSection( &g_ServiceCriticalSection );

         std::ignore = m_StaticService_p->SendStatusToServiceControlManager(SERVICE_STOP_PENDING, NO_ERROR, 2, m_StaticService_p->m_WaitHint);

         m_StaticService_p->OnStop();
         std::ignore = m_StaticService_p->SendStatusToServiceControlManager(SERVICE_STOPPED, NO_ERROR, 1, m_StaticService_p->m_WaitHint);
         m_StaticService_p->Exit();

         return;

      case SERVICE_CONTROL_PAUSE:

         if ( m_StaticService_p->m_Running == true and m_StaticService_p->m_Paused == false )
         {
            if ( m_StaticService_p->SendStatusToServiceControlManager( SERVICE_PAUSE_PENDING, NO_ERROR, 1, m_StaticService_p->m_WaitHint ) == FALSE )
            {
               return;
            }

            ::EnterCriticalSection( &g_ServiceCriticalSection );
            m_StaticService_p->m_Paused = true;
            ::LeaveCriticalSection( &g_ServiceCriticalSection );

            m_StaticService_p->OnPause();
            ::SuspendThread( m_StaticService_p->m_ThreadHandle );

            ::EnterCriticalSection( &g_ServiceCriticalSection );
            m_StaticService_p->m_CurrentState = SERVICE_PAUSED;
            ::LeaveCriticalSection( &g_ServiceCriticalSection );
         }

         break;

      case SERVICE_CONTROL_CONTINUE:

         if ( m_StaticService_p->m_Running == true and m_StaticService_p->m_Paused == true )
         {
            if ( m_StaticService_p->SendStatusToServiceControlManager( SERVICE_CONTINUE_PENDING, NO_ERROR, 1, m_StaticService_p->m_WaitHint ) == FALSE )
            {
               return;
            }

            ::EnterCriticalSection( &g_ServiceCriticalSection );
            m_StaticService_p->m_Paused = false;
            ::LeaveCriticalSection( &g_ServiceCriticalSection );

            ::ResumeThread( m_StaticService_p->m_ThreadHandle );
            m_StaticService_p->OnContinue();

            ::EnterCriticalSection( &g_ServiceCriticalSection );
            m_StaticService_p->m_CurrentState = SERVICE_RUNNING;
            ::LeaveCriticalSection( &g_ServiceCriticalSection );
         }

         break;

      case SERVICE_CONTROL_INTERROGATE:

         break;

      case SERVICE_CONTROL_SHUTDOWN:

         std::ignore = m_StaticService_p->SendStatusToServiceControlManager( SERVICE_STOP_PENDING, NO_ERROR, 1, m_StaticService_p->m_WaitHint );

         ::EnterCriticalSection( &g_ServiceCriticalSection );
         m_StaticService_p->m_Running      = false;
         m_StaticService_p->m_CurrentState = SERVICE_STOP_PENDING;
         ::LeaveCriticalSection( &g_ServiceCriticalSection );

         std::ignore = m_StaticService_p->SendStatusToServiceControlManager(SERVICE_STOP_PENDING, NO_ERROR, 2, m_StaticService_p->m_WaitHint);
         m_StaticService_p->OnShutdown();

         ::EnterCriticalSection(&g_ServiceCriticalSection);
         m_StaticService_p->m_Running = false;
         m_StaticService_p->m_CurrentState = SERVICE_STOPPED;
         ::LeaveCriticalSection(&g_ServiceCriticalSection);

         std::ignore = m_StaticService_p->SendStatusToServiceControlManager(SERVICE_STOPPED, NO_ERROR, 1, m_StaticService_p->m_WaitHint);
         m_StaticService_p->Exit();

         return;

      default:

         m_StaticService_p->OnControlCode( control_code );

         break;
   }

   std::ignore = m_StaticService_p->SendStatusToServiceControlManager( m_StaticService_p->m_CurrentState );
}

void CALLBACK Win32FoundationClasses::CService::ServiceMain( _In_ DWORD const number_of_command_line_arguments, _In_reads_z_( number_of_command_line_arguments ) LPCTSTR command_line_arguments[] )
{
   // entry point for service called by SCM when service is started

   auto thread_handle{ static_cast<HANDLE>(NULL) };

   WFC_VALIDATE_POINTER( m_StaticService_p );

   ::EnterCriticalSection( &g_ServiceCriticalSection );
   m_StaticService_p->m_ServiceStatusHandle = ::RegisterServiceCtrlHandler( m_StaticService_p->m_ServiceName, ServiceControlManagerHandler );
   ::LeaveCriticalSection( &g_ServiceCriticalSection );

   if ( m_StaticService_p->m_ServiceStatusHandle == (SERVICE_STATUS_HANDLE) 0 )
   {
      m_StaticService_p->m_ErrorCode = ::GetLastError();
      m_StaticService_p->LogEvent();
      m_StaticService_p->Exit();
   }
   else
   {
      if (not m_StaticService_p->SendStatusToServiceControlManager( SERVICE_START_PENDING, NO_ERROR, 1, m_StaticService_p->m_WaitHint ) )
      {
         goto EXIT_GOTO;
      }

      ::EnterCriticalSection( &g_ServiceCriticalSection );
      m_StaticService_p->m_ExitEventHandle = ::CreateEvent( 0, TRUE, FALSE, 0 ); // DO NOT USE MACRO HERE FOR MANUAL RESET EVENT!
      ::LeaveCriticalSection( &g_ServiceCriticalSection );

      if ( m_StaticService_p->m_ExitEventHandle == static_cast< HANDLE >( NULL ) )
      {
         m_StaticService_p->m_ErrorCode = ::GetLastError();
         m_StaticService_p->LogEvent();
         m_StaticService_p->Exit();
      }
      else
      {
         if (not m_StaticService_p->SendStatusToServiceControlManager( SERVICE_START_PENDING, NO_ERROR, 2, m_StaticService_p->m_WaitHint ) )
         {
            goto EXIT_GOTO;
         }

         m_StaticService_p->ParseCommandLineParameters( number_of_command_line_arguments, command_line_arguments );

         if (not m_StaticService_p->SendStatusToServiceControlManager( SERVICE_START_PENDING, NO_ERROR, 3, m_StaticService_p->m_WaitHint ) )
         {
            goto EXIT_GOTO;
         }

         if ( m_StaticService_p->OnPrepareServiceThread() == true )
         {
            // Daniel Meyer (Daniel.Meyer@Digital.com) had an awsome
            // suggestion here. Change CreateThread() to _beginthreadex()
            // so the C Runtime will work correctly. Now why didn't I think
            // of that???

            thread_handle = reinterpret_cast< HANDLE >( ::_beginthreadex(
                DEFAULT_THREAD_SECURITY_CONTEXT,
                DEFAULT_THREAD_STACK_SIZE,
                reinterpret_cast< unsigned int (__stdcall *)( void *) >( m_StaticService_p->m_ThreadStartRoutine ),
                m_StaticService_p,
                DEFAULT_THREAD_INIT_FLAG,
                reinterpret_cast< unsigned int * >( &m_StaticService_p->m_ThreadId ) ) );

            ::EnterCriticalSection( &g_ServiceCriticalSection );
            m_StaticService_p->m_ThreadHandle = thread_handle;
            ::LeaveCriticalSection( &g_ServiceCriticalSection );

            if ( m_StaticService_p->m_ThreadHandle == static_cast< HANDLE >( NULL ) )
            {
               m_StaticService_p->m_ErrorCode = ::GetLastError();
               m_StaticService_p->LogEvent();
               m_StaticService_p->Exit();
            }
            else
            {
               ::EnterCriticalSection( &g_ServiceCriticalSection );
               m_StaticService_p->m_Running = true;
               ::LeaveCriticalSection( &g_ServiceCriticalSection );

               if ( m_StaticService_p->SendStatusToServiceControlManager( SERVICE_RUNNING ) == FALSE )
               {
                  return;
               }

               ::EnterCriticalSection( &g_ServiceCriticalSection );
               m_StaticService_p->m_CurrentState = SERVICE_RUNNING;
               ::LeaveCriticalSection( &g_ServiceCriticalSection );

               ::WaitForSingleObject( m_StaticService_p->m_ExitEventHandle, INFINITE );

               // Thanks to Daniel Meyer (Daniel.Meyer@Digital.com) for finding a bug here
               ::CloseHandle( m_StaticService_p->m_ThreadHandle );
            }
         }

EXIT_GOTO:

         // notify SCM that service has stopped

         WFC_VALIDATE_POINTER( m_StaticService_p );

         if ( m_StaticService_p->m_ServiceStatusHandle not_eq 0 )
         {
             std::ignore = m_StaticService_p->SendStatusToServiceControlManager( SERVICE_STOPPED, m_StaticService_p->m_ErrorCode );
         }
      }
   }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CService</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that makes it easy to create NT services.">
</HEAD>

<BODY>

<H1>CService</H1>
$Revision: 41 $
<HR>

<H2>Description</H2>

This class makes it easy to create Services. This class is not well suited
to handle multiple services in a single process space. This class assumes
one service per process.

<H2>Data Members</H2>

<B><A NAME="m_StaticService_p">m_StaticService_p</A></B> - A static variable that holds a pointer to
the class (this).<P>
<B><A NAME="CommandLineParameters">CommandLineParameters</A></B> - An array that holds the command
line arguments when the service was started.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>HANDLE <B><A NAME="CreateConfigurationFile">CreateConfigurationFile</A></B>( LPCTSTR filename )</PRE><DD>
Creates a file with a NULL security DACL. This allows the file to be opened
by anyone in the system.

<DT><PRE>int <B><A NAME="DialogBoxParam">DialogBoxParam</A></B>( HINSTANCE instance, LPCTSTR template_name, HWND parent_window, DLGPROC dialogbox_procedure, LPARAM lParam = 0 )</PRE><DD>
Displays a dialog box from an interactive service.

<DT><PRE>BOOL <B><A NAME="Initialize">Initialize</A></B>( LPCTSTR name_of_service )</PRE><DD>
Initializes the service.

<DT><PRE>void <B><A NAME="LogEvent">LogEvent</A></B>( WORD EventType = EVENTLOG_ERROR_TYPE, LPTSTR message_string = nullptr, DWORD error_code = NO_ERROR )</PRE><DD>
Makes an entry in the Event Log for this service.

<DT><PRE>int <B><A NAME="MessageBox">MessageBox</A></B>( LPCTSTR text, LPCTSTR caption, UINT type )</PRE><DD>
Displays a message box for interactive services. It handles all of the calls to the
security API to create the file.

<DT><PRE>bool <B><A NAME="SpawnProcess">SpawnProcess</A></B>( LPCTSTR command_line, LPCTSTR current_directory, DWORD&amp; last_error )</PRE><DD>
Let&#39;s you start a non-interactive process from within a service.

</DL>

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;include &quot;messages.h&quot;
&#35;pragma hdrstop

DWORD WINAPI worker_thread( LPVOID pointer_to_parent_CService_class );
VOID set_default_parameters( void );

int __cdecl _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   if ( number_of_command_line_arguments == 1 )
   {
      <B>CService</B> service( worker_thread );
      service.Initialize( TEXT( &quot;WatchDog&quot; ) );
      return( EXIT_SUCCESS );
   }

   if ( number_of_command_line_arguments == 2 )
   {
      if ( _tcsicmp( command_line_arguments[ 1 ], TEXT( &quot;install&quot; ) ) == 0 )
      {
         <A HREF="CSvcMgr.htm">CServiceControlManager</A> service_control_manager;

         service_control_manager.Open();

         if ( service_control_manager.Install( TEXT( &quot;WatchDog&quot; ),
                                               TEXT( &quot;WatchDog&quot; ),
            TEXT( &quot;%SystemRoot%\\System32\\WatchDog.exe&quot; ) ) == FALSE )
         {
            _tprintf( TEXT( &quot;Install failed, please see Application Log for details\n&quot; ) );
         }

         set_default_parameters();

         return( EXIT_SUCCESS );
      }
      else if ( _tcsicmp( command_line_arguments[ 1 ], TEXT( &quot;remove&quot; ) ) == 0 )
      {
         <A HREF="CSvcMgr.htm">CServiceControlManager</A> service_control_manager;

         service_control_manager.Open();

         if ( service_control_manager.Remove( TEXT( &quot;WatchDog&quot; ) ) == FALSE )
         {
            _tprintf( TEXT( &quot;Removing failed, please see Application Log for details\n&quot; ) );
         }

         return( EXIT_SUCCESS );
      }
      else if ( _tcsicmp( command_line_arguments[ 1 ], TEXT( &quot;run&quot; ) ) == 0 )
      {
         worker_thread( (LPVOID) 1 );
         return( EXIT_SUCCESS );
      }
      else
      {
         _tprintf( TEXT( &quot;Samuel R. Blackburn, WFC Sample Application\nUsage: %s [install|remove]\n&quot; ), command_line_arguments[ 0 ] );
      }
   }
   else
   {
      _tprintf( TEXT( &quot;Samuel R. Blackburn, WFC Sample Application\nUsage: %s [install|remove]\n&quot; ), command_line_arguments[ 0 ] );
   }

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

   DWORD number_of_seconds_to_sleep { 0 };

   std::wstring machine_name( TEXT( &quot;&quot; ) );

   BOOL return_value { TRUE };

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

   DWORD sleep_time { 1000 * number_of_seconds_to_sleep };

   if ( sleep_time &lt; 2000 )
   {
      // Minimum sleep time is 2 seconds, this give the OS time to do other things
      sleep_time = 2000;
   }

   int number_of_services_to_keep_alive { names_of_services_to_keep_alive.GetSize() };

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

            while( service_control_manager.GetNext( status ) not_eq FALSE )
            {
               stopped_services.Add( status.lpServiceName );
            }

            // Now that we have the service names, we need to see which services need to be started

            int number_of_stopped_services { stopped_services.GetSize() };
            int alive_index                { 0 };
            int stopped_index              { 0 };

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
         DWORD default_sleep_time { 60 };

         if ( registry.SetValue( TEXT( &quot;NumberOfSecondsBetweenChecks&quot; ), default_sleep_time ) == FALSE )
         {
            LPVOID message_buffer { nullptr };

            ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER bitor FORMAT_MESSAGE_FROM_SYSTEM,
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
            LPVOID message_buffer { nullptr };

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

            event_log.Report( CEventLog::eventError, 0, MSG_CANT_SET_REGISTRY_ENTRY, 2, (LPCTSTR *) string_array );

            if ( message_buffer not_eq nullptr )
            {
               ::LocalFree( message_buffer );
            }
         }
      }
      else
      {
         LPVOID message_buffer { nullptr };

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
      LPVOID message_buffer { nullptr };

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

<H2>API's Used</H2>

<DL COMPACT>
<LI>CreateEvent
<LI>CreateThread
<LI>DeleteCriticalSection
<LI>DialogBoxParam
<LI>EnterCriticalSection
<LI>GetCurrentThreadId
<LI>GetLastError
<LI>GetThreadDesktop
<LI>GetVersionEx
<LI>InitializeCriticalSection
<LI>LeaveCriticalSection
<LI>MessageBox
<LI>OpenInputDesktop
<LI>RegisterServiceCtrlHandler
<LI>ResumeThread
<LI>SetEvent
<LI>SetServiceStatus
<LI>SetThreadDesktop
<LI>StartServiceCtrlDispatcher
<LI>SuspendThread
<LI>WaitForSingleObject
</DL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: cservice.cpp $<BR>
$Modtime: 6/26/01 10:51a $
</BODY>

</HTML>
*/
