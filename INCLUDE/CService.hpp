#if ! defined( SERVICE_CLASS_HEADER )

/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2019, Samuel R. Blackburn
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
** $Workfile: cservice.hpp $
** $Revision: 21 $
** $Modtime: 6/26/01 11:04a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#define SERVICE_CLASS_HEADER

extern CRITICAL_SECTION g_ServiceCriticalSection;

#define SERVICE_NAME_LEN (256)
#define ACCEPT_FLAGS ( SERVICE_ACCEPT_STOP bitor SERVICE_ACCEPT_PAUSE_CONTINUE bitor SERVICE_ACCEPT_SHUTDOWN )

using SERVICE_CONTROL_MANAGER_MESSAGE_CALLBACK = void (WINAPI *)( LPVOID service_class_pointer );

class CService
{
   private:

      SERVICE_CONTROL_MANAGER_MESSAGE_CALLBACK m_OnShutdown{ nullptr };
      SERVICE_CONTROL_MANAGER_MESSAGE_CALLBACK m_OnStop{ nullptr };
      SERVICE_CONTROL_MANAGER_MESSAGE_CALLBACK m_OnPause{ nullptr };
      SERVICE_CONTROL_MANAGER_MESSAGE_CALLBACK m_OnContinue{ nullptr };

      VOID * m_ServiceControlManagerCallbackParameter{ nullptr };

   public:

       static CService* m_StaticService_p;

      CService(_In_ CService const&) = delete;
      CService& operator=(_In_ CService const&) = delete;

      CService( __callback LPTHREAD_START_ROUTINE thread_start_routine, _In_ DWORD const accepted_controls = ACCEPT_FLAGS, _In_ DWORD const wait_hint = 30011 ) noexcept;

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CService( void );

      std::vector<std::wstring> CommandLineParameters;

      static _Check_return_ HANDLE CreateConfigurationFile( __in_z LPCTSTR filename ) noexcept;
      static _Check_return_ bool   SpawnProcess( __in_z LPCTSTR command_line, __in_z LPCTSTR current_directory, _Out_ DWORD& last_error ) noexcept;

      virtual _Check_return_ bool Initialize( __in_z LPCWSTR name_of_service ) noexcept;
      void LogEvent( _In_ WORD event_type = EVENTLOG_ERROR_TYPE, __in_z_opt LPCTSTR message_string = nullptr, _In_ DWORD error_code = NO_ERROR ) noexcept;
      virtual _Check_return_ INT_PTR DialogBoxParam( _In_ HINSTANCE instance, __in_z LPCTSTR template_name, _In_ HWND parent_window, __callback DLGPROC dialogbox_procedure, _In_ LPARAM lParam = 0 ) noexcept;

      // 2012-05-128 - MessageBox gets #defined to MessageBoxW which causes problems in managed C++ builds
      virtual _Check_return_ int ShowMessageBox( __in_z_opt LPCTSTR text, __in_z_opt LPCTSTR caption, _In_ UINT type ) noexcept;

      /// <summary>Sets the parameter used in the OnxxxCallback methods</summary>
      inline constexpr void SetCallbackParameter( _In_ void * parameter ) noexcept
      {
         m_ServiceControlManagerCallbackParameter = parameter;
      }

      inline constexpr void SetOnShutdownCallback( _In_ SERVICE_CONTROL_MANAGER_MESSAGE_CALLBACK callback ) noexcept
      {
         m_OnShutdown = callback;
      }

      inline constexpr void SetOnStopcallback( _In_ SERVICE_CONTROL_MANAGER_MESSAGE_CALLBACK callback ) noexcept
      {
         m_OnStop = callback;
      }

      inline constexpr void SetOnPauseCallback( _In_ SERVICE_CONTROL_MANAGER_MESSAGE_CALLBACK callback ) noexcept
      {
         m_OnPause = callback;
      }

      inline constexpr void SetOnContinueCallback( _In_ SERVICE_CONTROL_MANAGER_MESSAGE_CALLBACK callback ) noexcept
      {
         m_OnContinue = callback;
      }

      virtual _Check_return_ bool SendStatusToServiceControlManager(_In_ DWORD current_state,
          _In_ DWORD win32_exit_code = NO_ERROR,
          _In_ DWORD check_point = 0,
          _In_ DWORD wait_hint = 30011,
          _In_ DWORD service_specific_code = NO_ERROR) noexcept;

protected:

      HANDLE m_ExitEventHandle{ INVALID_HANDLE_VALUE };
      HANDLE m_ThreadHandle{ INVALID_HANDLE_VALUE };

      DWORD m_ControlsAccepted{ 0 };
      DWORD m_CurrentState{ 0 };
      DWORD m_ErrorCode{ 0 };
      DWORD m_ThreadId{ 0 };
      DWORD m_WaitHint{ 0 };

      LPTHREAD_START_ROUTINE m_ThreadStartRoutine{ nullptr };

      SERVICE_STATUS_HANDLE m_ServiceStatusHandle{ nullptr };

      bool m_Debugging{ false };
      bool m_Running{ false };
      bool m_Paused{ false };
      bool m_Exiting{ false };

      __declspec(align(16)) SERVICE_TABLE_ENTRYW m_ServiceTable[ 2 ];

      wchar_t * m_ServiceName{ nullptr };

      static void CALLBACK ServiceControlManagerHandler( _In_ DWORD control_code );
      static void CALLBACK ServiceMain( _In_ DWORD const argc, _In_reads_z_( argc ) LPCTSTR argv[] );

      virtual void ParseCommandLineParameters( _In_ DWORD const argc, _In_reads_z_( argc ) LPCTSTR argv[] ) noexcept;
      virtual void OnControlCode( _In_ DWORD control_code ) noexcept;
      virtual void OnShutdown( void ) noexcept;
      virtual void OnStop( void ) noexcept;
      virtual _Check_return_ bool OnPrepareServiceThread( void ) noexcept;
      virtual void OnPause( void ) noexcept;
      virtual void OnContinue( void ) noexcept;

      /*
      ** Thanks go to Juha Jalovaara (Juha@compuserve.com) for
      ** finding the flaw of Exit() not being virtual.
      */

      virtual void Exit( void ) noexcept;

#if defined ( _DEBUG )
      virtual void AssertValid( void ) const;
      void DumpStatus( _Inout_ SERVICE_STATUS * status_p ) const;
#endif _DEBUG
};

#endif // SERVICE_CLASS_HEADER
