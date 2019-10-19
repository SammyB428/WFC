/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2017, Samuel R. Blackburn
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
** $Workfile: cnetwork.cpp $
** $Revision: 44 $
** $Modtime: 6/26/01 10:49a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

USING_WFC_NAMESPACE

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

/*
** CNetwork stuff
*/

CNetwork::CNetwork(_In_opt_z_ LPCWSTR machine_name )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( machine_name );
   m_Initialize();
   Open( machine_name );
}

CNetwork::~CNetwork()
{
   WFC_VALIDATE_POINTER( this );
   Close();
   m_Initialize();
}

_Check_return_ bool CNetwork::AbortShutdown( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto token_handle = static_cast< HANDLE >( NULL );

   if ( ::OpenProcessToken( ::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES bitor TOKEN_QUERY, &token_handle ) == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACE( TEXT( "Can't OpenProcessToken()" ) );
      //WFCTRACEERROR( m_ErrorCode );
      return( false );
   }

   TOKEN_PRIVILEGES token_privileges;

   ::ZeroMemory( &token_privileges, sizeof( token_privileges ) );

   LPCSTR machine_name = nullptr;

   // Check to see if we've opened ourselves...

   TCHAR this_machine_name[ 1024 ];

   ::ZeroMemory( this_machine_name, sizeof( this_machine_name ) );

   DWORD temp_dword = 1024;

   if ( ::GetComputerNameEx(COMPUTER_NAME_FORMAT::ComputerNameDnsHostname, this_machine_name, &temp_dword ) == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACE( TEXT( "Can't GetComputerName" ) );
      //WFCTRACEERROR( m_ErrorCode );
      return(false);
   }

   // Something changed in service pack 3 for NT 3.51. You used to be able
   // to specify the local machine name and get SE_REMOTE_SHUTDOWN privilege
   // and shut down the machine. Not any more. You can no longer remotely
   // shut down your local machine. And no, I couldn't find this documented
   // anywhere...

   if (_wcsicmp( m_MachineName.c_str(), this_machine_name ) == I_AM_EQUAL_TO_THAT )
   {
      machine_name = nullptr;
   }

   if ( machine_name != nullptr )
   {
      // Shutdown a machine somewhere on the network

      if ( ::LookupPrivilegeValue( m_MachineName.c_str(), SE_REMOTE_SHUTDOWN_NAME, &token_privileges.Privileges[ 0 ].Luid ) == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACE( TEXT( "Can't LookupPrivilegeValue for remote machine" ) );
         //WFCTRACEERROR( m_ErrorCode );
         return(false);
      }
   }
   else
   {
      // Shutdown this computer

      if ( ::LookupPrivilegeValue( nullptr, SE_SHUTDOWN_NAME, &token_privileges.Privileges[ 0 ].Luid ) == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACE( TEXT( "Can't LookupPrivilege for local machine" ) );
         //WFCTRACEERROR( m_ErrorCode );
         return(false);
      }
   }

   token_privileges.PrivilegeCount             = 1;
   token_privileges.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED;

   if ( ::AdjustTokenPrivileges( token_handle, FALSE, &token_privileges, 0, nullptr, 0 ) == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACE( TEXT( "Can't AdjustTokenPrivileges" ) );
      //WFCTRACEERROR( m_ErrorCode );
      return(false);
   }

   (void) Win32FoundationClasses::wfc_close_handle( token_handle );

   if ( machine_name != nullptr )
   {
      // AbortSystemShutdown() is not const correct
      // The first parameter of AbortSystemShutdown() is a LPTSTR when it should be 
      // a LPCTSTR (const). Because it ain't, we gotta code around it.

      _tcscpy_s( this_machine_name, std::size( this_machine_name ), m_MachineName.c_str() );

      if ( ::AbortSystemShutdown( this_machine_name ) == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACE( TEXT( "Can't AbortSystemShutdown on remote machine" ) );
         //WFCTRACEERROR( m_ErrorCode );
         return(false);
      }
   }
   else
   {
      if ( ::AbortSystemShutdown( nullptr ) == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACE( TEXT( "Can't AbortSystemShutdown on local machine" ) );
         //WFCTRACEERROR( m_ErrorCode );
         return(false);
      }
   }

   return( true );
}

void CNetwork::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_WideMachineName.get() != nullptr )
   {
       m_WideMachineName.release();
   }

   if ( m_WideDoubleBackslashPreceededMachineName.get() != nullptr )
   {
       m_WideDoubleBackslashPreceededMachineName.release();
   }

   if ( m_PortBuffer.get() != nullptr )
   {
      m_PortBuffer.release();
   }

   m_MachineName.clear();
   m_FriendlyMachineName.clear();
   m_PortNumber    = 0;
   m_NumberOfPorts = 0;
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CNetwork::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );

   dump_context << TEXT( "m_MachineName         = \"" ) << m_MachineName         << TEXT( "\"\n" );
   dump_context << TEXT( "m_FriendlyMachineName = \"" ) << m_FriendlyMachineName << TEXT( "\"\n" );
   dump_context << TEXT( "m_PortNumber          = "   ) << m_PortNumber          << TEXT( "\n"   );
   dump_context << TEXT( "m_NumberOfPorts       = "   ) << m_NumberOfPorts       << TEXT( "\n"   );
   dump_context << TEXT( "m_ErrorCode           = "   ) << m_ErrorCode           << TEXT( "\n"   );
}

#endif // _DEBUG

_Check_return_ bool CNetwork::EnumeratePorts( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_PortNumber = 0;

   if ( m_PortBuffer.get() != nullptr )
   {
      m_PortBuffer.release();
      m_NumberOfPorts = 0;
   }

   // First, we call EnumPorts to find out how big our buffer needs to be...

   DWORD number_of_bytes_needed = 0;

   BYTE pseudo_buffer[10]{ 0 };

   // EnumPorts is not const correct, therefore we have to code around it...

   wchar_t machine_name[ MAX_PATH ];

   ::ZeroMemory( machine_name, sizeof( machine_name ) );

   wcscpy_s( machine_name, std::size( machine_name ), m_WideDoubleBackslashPreceededMachineName.get() );

   auto result = ::EnumPortsW( machine_name,
                               1,
                               pseudo_buffer,
                               sizeof( pseudo_buffer ),
                              &number_of_bytes_needed,
                              &m_NumberOfPorts );

   m_ErrorCode = ::GetLastError();

   if (result != FALSE )
   {
      //WFCTRACEERROR( m_ErrorCode );
      // Something bad wrong here...

      return( false );
   }

   if ( m_ErrorCode != ERROR_INSUFFICIENT_BUFFER )
   {
      //WFCTRACEERROR( m_ErrorCode );
      return( false );
   }

   m_PortBuffer = std::make_unique<uint8_t[]>(number_of_bytes_needed);

   WFC_VALIDATE_POINTER_NULL_OK( m_PortBuffer.get() );

   if ( m_PortBuffer.get() == nullptr )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
      return( false );
   }

   result = ::EnumPortsW( machine_name,
                          1,
                          m_PortBuffer.get(),
                          number_of_bytes_needed,
                         &number_of_bytes_needed,
                         &m_NumberOfPorts );

   if ( result == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
      m_PortBuffer.release();
      m_NumberOfPorts = 0;
      return(false);
   }

   return( true );
}

_Check_return_ bool CNetwork::GetNext( __inout CPortInformation& port ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_PortBuffer == nullptr )
   {
      return( false );
   }

   if ( m_PortNumber < m_NumberOfPorts )
   {
      auto port_p = reinterpret_cast<PORT_INFO_1 *>(m_PortBuffer.get());

      port.Copy( &port_p[ m_PortNumber ] );
      m_PortNumber++;
      return( true );
   }
   else
   {
      // We've retrieved the last one so clean up...

      m_PortBuffer.release();
      m_PortNumber    = 0;
      m_NumberOfPorts = 0;
      return( false );
   }
}

_Check_return_ DWORD CNetwork::GetErrorCode( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_ErrorCode );
}

void CNetwork::GetFriendlyMachineName( _Out_ std::wstring& machine_name ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   machine_name = m_FriendlyMachineName;
}

_Check_return_ LPCWSTR CNetwork::GetMachineName( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_MachineName.c_str() );
}

_Check_return_ bool CNetwork::GetTime(_Inout_ CTime& machine_time ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   TIME_OF_DAY_INFO * time_of_day = nullptr;

   if ( ::NetRemoteTOD( m_WideMachineName.get(), reinterpret_cast<LPBYTE *>(&time_of_day) ) == NERR_Success )
   {
      machine_time = CTime( time_of_day->tod_elapsedt );
      return( true );
   }
   else
   {
      m_ErrorCode = ::GetLastError();
      machine_time.Empty();
      //WFCTRACEERROR( m_ErrorCode );
      return( false );
   }
}

_Check_return_ bool CNetwork::GetTime(__inout CSystemTime& machine_time ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   TIME_OF_DAY_INFO * time_of_day = nullptr;

   if ( ::NetRemoteTOD( m_WideMachineName.get(), reinterpret_cast<LPBYTE *>(&time_of_day) ) == NERR_Success )
   {
      machine_time.Copy( time_of_day );
      return( true );
   }
   else
   {
       m_ErrorCode = ::GetLastError();
       machine_time.Empty();
      //WFCTRACEERROR( m_ErrorCode );
      return( false );
   }
}

_Check_return_ bool CNetwork::IsRebootable( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto token_handle = static_cast< HANDLE >( NULL );

   if ( ::OpenProcessToken( ::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES bitor TOKEN_QUERY, &token_handle ) == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
      return( false );
   }

   TOKEN_PRIVILEGES token_privileges;

   ::ZeroMemory( &token_privileges, sizeof( token_privileges ) );

   LPCSTR machine_name = nullptr;

   // Check to see if we've opened ourselves...

   TCHAR this_machine_name[ 1024 ];

   ::ZeroMemory( this_machine_name, sizeof( this_machine_name ) );

   DWORD temp_dword = 1024;

   if ( ::GetComputerNameEx(COMPUTER_NAME_FORMAT::ComputerNameDnsHostname, this_machine_name, &temp_dword ) == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
      return( false );
   }

   // Something changed in service pack 3 for NT 3.51. You used to be able
   // to specify the local machine name and get SE_REMOTE_SHUTDOWN privilege
   // and shut down the machine. Not any more. You can no longer remotely
   // shut down your local machine. And no, I couldn't find this documented
   // anywhere...

   if (_wcsicmp( m_MachineName.c_str(), this_machine_name ) == I_AM_EQUAL_TO_THAT )
   {
      machine_name = nullptr;
   }

   if ( machine_name != nullptr )
   {
      // Shutdown a machine somewhere on the network

      if ( ::LookupPrivilegeValue( m_MachineName.c_str(), SE_REMOTE_SHUTDOWN_NAME, &token_privileges.Privileges[ 0 ].Luid ) == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACEERROR( m_ErrorCode );
         return( false );
      }
   }
   else
   {
      // Shutdown this computer

      if ( ::LookupPrivilegeValue( nullptr, SE_SHUTDOWN_NAME, &token_privileges.Privileges[ 0 ].Luid ) == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACEERROR( m_ErrorCode );
         return( false );
      }
   }

   token_privileges.PrivilegeCount             = 1;
   token_privileges.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED;

   if ( ::AdjustTokenPrivileges( token_handle, FALSE, &token_privileges, 0, nullptr, 0 ) == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
      return( false );
   }

   // If we get this far it means we can reboot the machine...

   return( true );
}

void CNetwork::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_MachineName.clear();
   m_FriendlyMachineName.clear();
   m_WideMachineName.release();
   m_WideDoubleBackslashPreceededMachineName.release();
   m_ErrorCode       = 0;
   m_PortBuffer      = nullptr;
   m_NumberOfPorts   = 0;
   m_PortNumber      = 0;
}

void CNetwork::Open( __in_z_opt LPCTSTR machine_name ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( machine_name );

   // First, make sure we're closed...

   Close();

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( machine_name != nullptr )
      {
         // Thanks go to Ullrich Pollahne (u.pollaehne@online.de)
         // for finding a bug here. I was not correctly counting
         // the number of bytes needed for a buffer.

         int additional_characters = 1; // For the 0x00 at the end

         std::wstring temporary_machine_name( machine_name );

         // Strip off the \\ if present

         if ( temporary_machine_name.length() > 1 )
         {
            if ( temporary_machine_name.at( 0 ) == '\\' )
            {
               temporary_machine_name.erase(0, 1);
               additional_characters++;
            }

            if ( temporary_machine_name.at( 0 ) == '\\' )
            {
               temporary_machine_name.erase(0, 1);
               additional_characters++;
            }
         }

         // We now have a backslash-free name

         m_FriendlyMachineName.assign( temporary_machine_name );

         std::size_t const number_of_characters_in_wide_machine_name = temporary_machine_name.length() + additional_characters + 1;

         m_WideMachineName = std::make_unique<wchar_t[]>(number_of_characters_in_wide_machine_name);

         if ( m_WideMachineName.get() != nullptr )
         {
#if defined( UNICODE )
            ::wcscpy_s( m_WideMachineName.get(), number_of_characters_in_wide_machine_name, machine_name );
#else
            ::ASCII_to_UNICODE( machine_name, m_WideMachineName.get() );
#endif
         }

         m_MachineName.assign( machine_name );

         // Now, add those stinking backslashes

         temporary_machine_name.assign(WSTRING_VIEW(L"\\\\"));
         temporary_machine_name.append( m_FriendlyMachineName );

         std::size_t const number_of_characters_in_m_WideDoubleBackslashPreceededMachineName = temporary_machine_name.length() + 1;

         m_WideDoubleBackslashPreceededMachineName = std::make_unique<wchar_t[]>(number_of_characters_in_m_WideDoubleBackslashPreceededMachineName);

         if ( m_WideDoubleBackslashPreceededMachineName.get() != nullptr )
         {
#if defined( UNICODE )
            ::wcscpy_s( m_WideDoubleBackslashPreceededMachineName.get(), number_of_characters_in_m_WideDoubleBackslashPreceededMachineName, temporary_machine_name.c_str() );
#else
            ::ASCII_to_UNICODE( temporary_machine_name, m_WideDoubleBackslashPreceededMachineName );
#endif
         }
      }
      else
      {
         // The machine name is NULL, that means the user wants to play
         // with the local machine. That's OK, all we need to do is put
         // the name of the computer into m_FriendlyName

         TCHAR this_machine_name[ MAX_COMPUTERNAME_LENGTH + 1 ];

         DWORD string_size = MAX_COMPUTERNAME_LENGTH + 1;

         ZeroMemory( this_machine_name, sizeof( this_machine_name ) );

         if ( ::GetComputerNameEx(COMPUTER_NAME_FORMAT::ComputerNameDnsHostname, this_machine_name, &string_size ) == FALSE )
         {
            // CRAP! The computer name is too long.

            auto new_string = std::make_unique<wchar_t []>(string_size + 1);

            string_size++;

            if ( ::GetComputerNameExW(COMPUTER_NAME_FORMAT::ComputerNameDnsHostname, new_string.get(), &string_size ) == FALSE )
            {
               // DOUBLE CRAP!
               return;
            }

            m_FriendlyMachineName.assign( new_string.get() );
         }
         else
         {
            m_FriendlyMachineName.assign( this_machine_name );
         }
      }
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

_Check_return_ bool CNetwork::SetPrivilege( __in_z LPCTSTR privilege_name, __in bool const add_privilege ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( privilege_name );

   auto token_handle = static_cast< HANDLE >( NULL );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( ::OpenProcessToken( ::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES bitor TOKEN_QUERY, &token_handle ) == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACE( TEXT( "Can't OpenProcessToken" ) );
         //WFCTRACEERROR( m_ErrorCode );
         return( false );
      }

      TOKEN_PRIVILEGES token_privileges;
      TOKEN_PRIVILEGES previous_token_privileges;

      LUID locally_unique_identifier;

      ::ZeroMemory( &token_privileges, sizeof( token_privileges ) );
      ::ZeroMemory( &previous_token_privileges, sizeof( previous_token_privileges ) );
      ::ZeroMemory( &locally_unique_identifier, sizeof( locally_unique_identifier ) );
   
      if ( ::LookupPrivilegeValue( m_MachineName.c_str(), privilege_name, &locally_unique_identifier ) == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACE( TEXT( "Can't LookupPrivilegeValue for remote machine" ) );
         //WFCTRACEERROR( m_ErrorCode );
         (void) Win32FoundationClasses::wfc_close_handle( token_handle );
         token_handle = static_cast< HANDLE >( NULL );
         return( false );
      }

      token_privileges.PrivilegeCount             = 1;
      token_privileges.Privileges[ 0 ].Luid       = locally_unique_identifier;
      token_privileges.Privileges[ 0 ].Attributes = 0;

      DWORD sizeof_previous_token_privileges = sizeof( previous_token_privileges );

      if ( ::AdjustTokenPrivileges( token_handle,
                                    FALSE,
                                   &token_privileges,
                                    sizeof( token_privileges ),
                                   &previous_token_privileges,
                                   &sizeof_previous_token_privileges ) == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACE( TEXT( "Can't AdjustTokenPrivileges" ) );
         //WFCTRACEERROR( m_ErrorCode );
         (void) Win32FoundationClasses::wfc_close_handle( token_handle );
         token_handle = static_cast< HANDLE >( NULL );
         return( false );
      }

      previous_token_privileges.PrivilegeCount       = 1;
      previous_token_privileges.Privileges[ 0 ].Luid = locally_unique_identifier;
      
      if ( add_privilege == true)
      {
         previous_token_privileges.Privileges[ 0 ].Attributes or_eq (SE_PRIVILEGE_ENABLED);
      }
      else
      {
         previous_token_privileges.Privileges[ 0 ].Attributes xor_eq ( (SE_PRIVILEGE_ENABLED) bitand previous_token_privileges.Privileges[ 0 ].Attributes );
      }

      if ( ::AdjustTokenPrivileges( token_handle,
                                    FALSE,
                                   &previous_token_privileges,
                                    sizeof_previous_token_privileges,
                                    nullptr,
                                    nullptr ) == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACE( TEXT( "Can't AdjustTokenPrivileges for the second time" ) );
         //WFCTRACEERROR( m_ErrorCode );
         (void) Win32FoundationClasses::wfc_close_handle( token_handle );
         token_handle = static_cast< HANDLE >( NULL );
         return( false );
      }

      // YIPPEE! We succeeded!

      (void) Win32FoundationClasses::wfc_close_handle( token_handle );
      token_handle = static_cast< HANDLE >( NULL );

      return( true );
   }
   WFC_CATCH_ALL
   {
      // Let the caller know an exception occurred
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( false );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ bool CNetwork::Shutdown( __in bool const reboot, __in bool const force_applications_to_close, __in_z_opt LPCTSTR message_to_display, __in DWORD const number_of_seconds_before_shutdown ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto token_handle = static_cast< HANDLE >( NULL );

   if ( ::OpenProcessToken( ::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES bitor TOKEN_QUERY, &token_handle ) == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
      return( false );
   }

   TOKEN_PRIVILEGES token_privileges;

   ::ZeroMemory( &token_privileges, sizeof( token_privileges ) );

   // 1999-11-12
   // Thanks go to Karl Spaelti (karl.spaelti@qps.ch) for finding a bug
   // here. machine_name was a LPCSTR when it should have been LPCTSTR.

   LPCTSTR machine_name = nullptr;

   // Check to see if we've opened ourselves...

   TCHAR this_machine_name[ 1024 ];

   ::ZeroMemory( this_machine_name, sizeof( this_machine_name ) );

   DWORD temp_dword = static_cast<DWORD>(std::size(this_machine_name));

   if ( ::GetComputerNameEx(COMPUTER_NAME_FORMAT::ComputerNameDnsHostname, this_machine_name, &temp_dword ) == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
      (void) Win32FoundationClasses::wfc_close_handle( token_handle );
      token_handle = static_cast< HANDLE >( NULL );
      return( false );
   }

   // Something changed in service pack 3 for NT 3.51. You used to be able
   // to specify the local machine name and get SE_REMOTE_SHUTDOWN privilege
   // and shut down the machine. Not any more. You can no longer remotely
   // shut down your local machine. And no, I couldn't find this documented
   // anywhere...

   if (_wcsicmp( m_MachineName.c_str(), this_machine_name ) == I_AM_EQUAL_TO_THAT )
   {
      machine_name = nullptr;
   }
   else
   {
      machine_name = m_MachineName.c_str();
   }

   if ( machine_name != nullptr )
   {
      // Shutdown a machine somewhere on the network

      if ( ::LookupPrivilegeValue( m_MachineName.c_str(), SE_REMOTE_SHUTDOWN_NAME, &token_privileges.Privileges[ 0 ].Luid ) == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACEERROR( m_ErrorCode );
         (void) Win32FoundationClasses::wfc_close_handle( token_handle );
         token_handle = static_cast< HANDLE >( NULL );
         return( false );
      }
   }
   else
   {
      // Shutdown this computer

      if ( ::LookupPrivilegeValue( nullptr, SE_SHUTDOWN_NAME, &token_privileges.Privileges[ 0 ].Luid ) == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACEERROR( m_ErrorCode );
         (void) Win32FoundationClasses::wfc_close_handle( token_handle );
         token_handle = static_cast< HANDLE >( NULL );
         return( false );
      }
   }

   token_privileges.PrivilegeCount             = 1;
   token_privileges.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED;

   if ( ::AdjustTokenPrivileges( token_handle, FALSE, &token_privileges, 0, (PTOKEN_PRIVILEGES) nullptr, 0 ) == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
      (void) Win32FoundationClasses::wfc_close_handle( token_handle );
      token_handle = static_cast< HANDLE >( NULL );
      return( false );
   }

   // Whoever wrote InitiateSystemShutdown() (nor the QA/tester) didn't understand the
   // concept of const pointers. The way InitiateSystemShutdown() is prototyped, one
   // would expect it to be returning strings from the operating system. It doesn't
   // (or if it does it isn't documented but Microsoft would never ever ever not
   // document an API parameter) return any data in the string parameters. So, we are
   // left with either a programmer that didn't fully understand what they were doing
   // or it is yet another undocumented feature of the system.

   // InitiateSystemShutdown() is not const correct.
   // The second parameter of InitiateSystemShutdown() is a LPTSTR when it should be
   // a LPCTSTR (const). Because it ain't, we gotta code around it.

   TCHAR message_string[ 1024 ];

   ::ZeroMemory( message_string, sizeof( message_string ) );

   LPTSTR corrected_message_parameter = nullptr;

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( message_to_display != nullptr )
      {
         _tcsncpy_s( message_string, std::size( message_string ), message_to_display, _TRUNCATE );
         corrected_message_parameter = message_string;
      }
      else
      {
         corrected_message_parameter = nullptr;
      }

      if ( machine_name != nullptr )
      {
         // The first parameter of InitiateSystemShutdown() is a LPTSTR when it should be 
         // a LPCTSTR (const). Because it ain't, we gotta code around it.

         _tcscpy_s( this_machine_name, std::size( this_machine_name ), m_MachineName.c_str() );

         if ( ::InitiateSystemShutdown( this_machine_name, corrected_message_parameter, number_of_seconds_before_shutdown, force_applications_to_close, reboot ) == FALSE )
         {
            m_ErrorCode = ::GetLastError();
            //WFCTRACEERROR( m_ErrorCode );
            (void) Win32FoundationClasses::wfc_close_handle( token_handle );
            token_handle = static_cast< HANDLE >( NULL );
            return( false );
         }
      }
      else
      {
         if ( ::InitiateSystemShutdown( nullptr, corrected_message_parameter, number_of_seconds_before_shutdown, force_applications_to_close, reboot ) == FALSE )
         {
            m_ErrorCode = ::GetLastError();
            //WFCTRACEERROR( m_ErrorCode );
            (void) Win32FoundationClasses::wfc_close_handle( token_handle );
            token_handle = static_cast< HANDLE >( NULL );
            return( false );
         }
      }

      (void) Win32FoundationClasses::wfc_close_handle( token_handle );
      token_handle = static_cast< HANDLE >( NULL );
      return( true );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      (void) Win32FoundationClasses::wfc_close_handle( token_handle );
      token_handle = static_cast< HANDLE >( NULL );
      return( false );
   }
   WFC_END_CATCH_ALL
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CNetwork</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ base class for the networking classes.">
</HEAD>

<BODY>
<H1>CNetwork : CObject</H1>
$Revision: 44 $<BR><HR>

<H2>Description</H2>
This class is the base class for the WFC networking classes. It is not a pure
virtual base class in that it provides some interesting (and hopefully
useful) functions.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CNetwork</B>( LPCTSTR machine_name = nullptr )</PRE><DD>
Constructs the object.

</DL>

<H2>Data Members</H2>None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="AbortShutdown">AbortShutdown</A></B>( void )</PRE><DD>
Will abort a shutdown started by <B>Shutdown</B>().

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the connection to the machine.

<DT><PRE>BOOL <B><A NAME="EnumeratePorts">EnumeratePorts</A></B>( void )</PRE><DD>
Prepares to enumerate the ports on the machine.

<DT><PRE>DWORD <B><A NAME="GetErrorCode">GetErrorCode</A></B>( void ) const</PRE><DD>
Retrieves the error code. This is set when a
member function returns FALSE.

<DT><PRE>void <B><A NAME="GetFriendlyMachineName">GetFriendlyMachineName</A></B>( std::wstring&amp; machine_name )</PRE><DD>
Fills <CODE>machine_name</CODE> with the name of the machine
that has been <B>Open</B>()'ed. This names doesn't have those
annoying backslashes preceeding it.

<DT><PRE>LPCTSTR <B><A NAME="GetMachineName">GetMachineName</A></B>( void )</PRE><DD>
Retrieves the name of the machine this class
is working with.

<DT><PRE>BOOL <B><A NAME="GetNext">GetNext</A></B>( <A HREF="PortInformation.htm">CPortInformation</A>&amp; port_information )</PRE><DD>
Gets the next <B><A HREF="PortInformation.htm">CPortInformation</A></B> (used in conjunction with
<B>EnumeratePorts</B>()).

<DT><PRE>BOOL <B><A NAME="GetTime">GetTime</A></B>( CTime&amp; time )
BOOL <B>GetTime</B>( <A HREF="CSystemTime.htm">CSystemTime</A>&amp; time )</PRE><DD>
Gets the time from the machine as a CTime or a 
<A HREF="CSystemTime.htm">CSystemTime</A> (more accurate).

<DT><PRE>BOOL <B><A NAME="IsRebootable">IsRebootable</A></B>( void )</PRE><DD>
Returns TRUE if you have permissions to reboot the machine.
In other words, a call to <B>Shutdown</B>() should succeed.

<DT><PRE>void <B><A NAME="Open">Open</A></B>( LPCTSTR machine_name = nullptr )</PRE><DD>
Tells the class to work with the machine you specify.

<DT><PRE>void <B><A NAME="Serialize">Serialize</A></B>( CArchive&amp; archive )</PRE><DD>
After all we are a CObject...

<DT><PRE>BOOL <B><A NAME="SetPrivilege">SetPrivilege</A></B>( LPCTSTR privilege_name, BOOL add_privilege = TRUE )</PRE><DD>
Adds or removes a privilege. If <CODE>add_privilege</CODE> is FALSE, the 
privilege is removed, otherwise it is added. <CODE>privilege_name</CODE>
can be one of the following (as of NT4.0SP2/VC42b):

<UL>
<LI>SE_CREATE_TOKEN_NAME
<LI>SE_ASSIGNPRIMARYTOKEN_NAME
<LI>SE_LOCK_MEMORY_NAME
<LI>SE_INCREASE_QUOTA_NAME
<LI>SE_UNSOLICITED_INPUT_NAME
<LI>SE_MACHINE_ACCOUNT_NAME
<LI>SE_TCB_NAME
<LI>SE_SECURITY_NAME
<LI>SE_TAKE_OWNERSHIP_NAME
<LI>SE_LOAD_DRIVER_NAME
<LI>SE_SYSTEM_PROFILE_NAME
<LI>SE_SYSTEMTIME_NAME
<LI>SE_PROF_SINGLE_PROCESS_NAME
<LI>SE_INC_BASE_PRIORITY_NAME
<LI>SE_CREATE_PAGEFILE_NAME
<LI>SE_CREATE_PERMANENT_NAME
<LI>SE_BACKUP_NAME
<LI>SE_RESTORE_NAME
<LI>SE_SHUTDOWN_NAME
<LI>SE_DEBUG_NAME
<LI>SE_AUDIT_NAME
<LI>SE_SYSTEM_ENVIRONMENT_NAME
<LI>SE_CHANGE_NOTIFY_NAME
<LI>SE_REMOTE_SHUTDOWN_NAME
</UL>

<DT><PRE>BOOL <B><A NAME="Shutdown">Shutdown</A></B>( BOOL    reboot                            = TRUE,
               BOOL    force_applocations_to_close       = TRUE,
               LPCTSTR message_to_display                = nullptr,
               DWORD   number_of_seconds_before_shutdown = 0 )</PRE><DD>
Will reboot the machine set by <B>Open</B>().

</DL>

<H2>Example</H2><PRE><CODE>void test_CNetwork( LPCTSTR machine_name )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;test_CNetwork()&quot; ) );

   <B>CNetwork</B> machine( machine_name );

   if ( machine.EnumeratePorts() )
   {
      if ( machine_name != nullptr )
      {
         _tprintf( TEXT( &quot;Ports on %s\n&quot; ), machine_name );
      }
      else
      {
         _tprintf( TEXT( &quot;Ports on local machine\n&quot; ) );
      }

      <A HREF="PortInformation.htm">CPortInformation</A> port;

      while( machine.GetNext( port ) )
      {
         _tprintf( TEXT( &quot;Name:        %s\n&quot; ), (LPCTSTR) port.Name        );
         _tprintf( TEXT( &quot;Monitor:     %s\n&quot; ), (LPCTSTR) port.Monitor     );
         _tprintf( TEXT( &quot;Description: %s\n&quot; ), (LPCTSTR) port.Description );
         _tprintf( TEXT( &quot;Type:        %04lX\n&quot; ), port.Type );
         _tprintf( TEXT( &quot;TypeName:    %s\n\n&quot; ), (LPCTSTR) port.TypeName );
      }
   }
   else
   {
      DWORD error_code = machine.GetErrorCode();
      ReportError( error_code );
      _tprintf( TEXT( &quot;Can't EnumeratePorts, reason code %lu\n&quot; ), error_code );
   }

   if ( machine.IsRebootable() )
   {
      if ( machine_name != nullptr )
      {
         _tprintf( TEXT( &quot;Yes, I can reboot %s\n&quot; ), machine_name );
      }
      else
      {
         _tprintf( TEXT( &quot;Yes, I can reboot this machine\n&quot; ) );
      }
   }
}</CODE></PRE>

<H2>API's Used</H2>

<UL>
<LI>AbortSystemShutdown
<LI>AdjustTokenPrivileges
<LI>EnumPorts
<LI>GetComputerNameEx
<LI>GetCurrentProcess
<LI>GetLastError
<LI>InitiateSystemShutdown
<LI>LookupPrivilegeValue
<LI>NetRemoteTOD
<LI>OpenProcessToken
<LI>ZeroMemory
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: cnetwork.cpp $<BR>
$Modtime: 6/26/01 10:49a $
</BODY>

</HTML>
*/
