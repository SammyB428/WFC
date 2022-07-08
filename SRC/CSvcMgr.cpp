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
** $Workfile: csvcmgr.cpp $
** $Revision: 47 $
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

Win32FoundationClasses::CServiceControlManager::CServiceControlManager() noexcept
{
    WFC_VALIDATE_POINTER( this );
    m_Initialize();
}

Win32FoundationClasses::CServiceControlManager::~CServiceControlManager()
{
    WFC_VALIDATE_POINTER( this );

    Close();

    m_BufferSize = 0;
}

void Win32FoundationClasses::CServiceControlManager::Close( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    // We don't want to close while the database is locked

    std::ignore = UnlockDatabase();

    if ( m_ManagerHandle not_eq static_cast< SC_HANDLE >( NULL ) )
    {
        if ( ::CloseServiceHandle( m_ManagerHandle ) == FALSE )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
        }

        m_ManagerHandle = reinterpret_cast< SC_HANDLE >( NULL );
    }
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::Continue( _In_z_ wchar_t const * service_name ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( service_name );

    if ( m_ManagerHandle == static_cast< SC_HANDLE >( NULL ) )
    {
        m_ErrorCode = ERROR_INVALID_HANDLE;
        Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }

    if ( service_name == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_HANDLE;
        Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        auto service_handle { ::OpenServiceW(m_ManagerHandle, service_name, SERVICE_PAUSE_CONTINUE)};

        if ( service_handle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }

        SERVICE_STATUS service_status{ 0, 0, 0, 0, 0, 0, 0 };

        auto return_value{ ::ControlService(service_handle, SERVICE_CONTROL_CONTINUE, &service_status) };

        if ( return_value == FALSE )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
        }
        else
        {
            return_value = TRUE;
        }

        ::CloseServiceHandle( service_handle );

        // 2000-03-17
        // Thanks go to Steve Loughran (steve@iseran.com) for finding a bug
        // where I wasn't casting INVALID_HANDLE_VALUE to SC_HANDLE.

        service_handle = static_cast<SC_HANDLE>(INVALID_HANDLE_VALUE);

        return( return_value == FALSE ? false : true );
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::EnableInteractiveServices( _In_ bool enable_interactive_services ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    bool return_value{ true };

    CRegistry registry;

    if ( m_MachineName.empty() == true )
    {
        return_value = registry.Connect( HKEY_LOCAL_MACHINE );
    }
    else
    {
        return_value = registry.Connect( HKEY_LOCAL_MACHINE, m_MachineName.c_str() );
    }

    if ( return_value == false )
    {
        m_ErrorCode = registry.GetErrorCode();
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }

    return_value = registry.Open( L"System\\CurrentControlSet\\Control\\Windows" );

    if ( return_value == false )
    {
        m_ErrorCode = registry.GetErrorCode();
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }

    DWORD no_interactive_services{ 0 };

    if ( enable_interactive_services == true )
    {
        // Allow interactive services
        no_interactive_services = 0;
    }
    else
    {
        // Do not allow interactive services
        no_interactive_services = 1;
    }

    if ( registry.SetValue( TEXT( "NoInteractiveServices" ), no_interactive_services ) == false)
    {
        m_ErrorCode = registry.GetErrorCode();
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::EnumerateStatus( _In_ uint32_t state, _In_ uint32_t type ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    /*
    ** For GetNext() calls
    */

    if ( m_ManagerHandle == static_cast< SC_HANDLE >( NULL ) )
    {
        m_ErrorCode = ERROR_INVALID_HANDLE;
        Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }

    if ( m_Buffer.get() == nullptr )
    {
        m_BufferSize = 2 * sizeof( ENUM_SERVICE_STATUS );

        m_Buffer = std::make_unique<uint8_t []>(m_BufferSize);

        if ( m_Buffer.get() == nullptr )
        {
            m_BufferSize = 0;
            m_ErrorCode = ::GetLastError();
            Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
    }

    DWORD number_of_bytes_needed{ 0 };
    DWORD number_of_services_returned{ 0 };
    DWORD resume_handle{ 0 };

    if ( ::EnumServicesStatus( m_ManagerHandle,
        type,
        state,
        reinterpret_cast<ENUM_SERVICE_STATUS *>(m_Buffer.get()),
        m_BufferSize,
        &number_of_bytes_needed,
        &number_of_services_returned,
        &resume_handle ) not_eq FALSE )
    {
        m_CurrentEntryNumber = 0;
        m_NumberOfEntries    = number_of_services_returned;

        return( true );
    }

    m_ErrorCode = ::GetLastError();
    Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
    //WFCTRACEERROR( m_ErrorCode );

    if ( m_ErrorCode == ERROR_MORE_DATA )
    {
        m_Buffer.release();

        // Thanks go to Ullrich Pollaehne [Ullrich_Pollaehne@ka2.maus.de]
        // for finding a serious flaw in my logic here.

        m_Buffer = std::make_unique<uint8_t[]>(number_of_bytes_needed + m_BufferSize);

        if ( m_Buffer not_eq nullptr )
        {
            m_BufferSize += number_of_bytes_needed;
        }
        else
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            return( false );
        }

        number_of_bytes_needed      = 0;
        number_of_services_returned = 0;
        resume_handle               = 0;

        if ( ::EnumServicesStatus( m_ManagerHandle,
            type,
            state,
            reinterpret_cast<ENUM_SERVICE_STATUS *>(m_Buffer.get()),
            m_BufferSize,
            &number_of_bytes_needed,
            &number_of_services_returned,
            &resume_handle ) not_eq FALSE )
        {
            m_CurrentEntryNumber = 0;
            m_NumberOfEntries    = number_of_services_returned;

            return( true );
        }
        else
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );

            m_CurrentEntryNumber = 0;
            m_NumberOfEntries    = 0;

            return( false );
        }
    }

    return( false );
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::GetConfiguration( __in_z wchar_t const * service_name, _Out_ Win32FoundationClasses::CServiceConfigurationW& configuration ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( service_name );

    configuration.Empty();

    if ( service_name == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        ASSERT( m_ManagerHandle not_eq static_cast< SC_HANDLE >( NULL ) );

        if ( m_ManagerHandle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ERROR_INVALID_HANDLE;
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            return( false );
        }

        configuration.Empty();

        auto service_handle{ ::OpenServiceW(m_ManagerHandle, service_name, SERVICE_QUERY_CONFIG) };

        if ( service_handle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }

        auto buffer{ std::make_unique<uint8_t[]>(4000) };

        DWORD number_of_bytes_needed{ 0 };

        auto return_value{ ::QueryServiceConfig(service_handle,
                reinterpret_cast<QUERY_SERVICE_CONFIG*>(buffer.get()),
                4000,
                &number_of_bytes_needed) };

        if ( return_value == FALSE )
        {
            m_ErrorCode = ::GetLastError();
            Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );

            if ( m_ErrorCode == ERROR_INSUFFICIENT_BUFFER )
            {
#if ! defined( WFC_STL )
                    TRACE( TEXT( "CServiceControlManager::GetConfiguration() line %d, Sent in a buffer of %lu bytes, needed %lu bytes (%lu more)\n" ),
                        __LINE__,
                        4000,
                        (DWORD) number_of_bytes_needed,
                        (DWORD) number_of_bytes_needed - 4000 ); 
#endif
            }
        }
        else
        {
            return_value = TRUE;
            auto service_configuration_p{ reinterpret_cast<QUERY_SERVICE_CONFIG const*>(buffer.get()) };
            configuration.Copy( service_configuration_p );
        }

        ::CloseServiceHandle( service_handle );

        return( return_value == FALSE ? false : true );
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::GetDependencies( _In_z_ wchar_t const * service_name, _Out_ std::vector<std::wstring>& dependencies ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    ASSERT( m_ManagerHandle not_eq static_cast< SC_HANDLE >( NULL ) );

    dependencies.clear();

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        auto service_handle { ::OpenServiceW(m_ManagerHandle, service_name, SERVICE_ENUMERATE_DEPENDENTS)};

        if ( service_handle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }

        DWORD number_of_services_returned{ 0 };
        DWORD number_of_bytes_needed{ 0 };
        DWORD buffer_size{ 0 };

        auto status_array{ std::make_unique<ENUM_SERVICE_STATUS[]>(512) }; // an insane amount

        buffer_size = 512 * sizeof( ENUM_SERVICE_STATUS );

        ZeroMemory( status_array.get(), buffer_size );

        auto return_value{ ::EnumDependentServices(service_handle,
                SERVICE_STATE_ALL, // Both running and stopped services
                status_array.get(),
                buffer_size,
                &number_of_bytes_needed,
                &number_of_services_returned) };

        if ( return_value not_eq FALSE )
        {
            for ( auto const index : Range(number_of_services_returned) )
            {
                dependencies.push_back( status_array[ index ].lpServiceName );
            }
        }

        ::CloseServiceHandle( service_handle );

        return( return_value == FALSE ? false : true );
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::GetDisplayName( _In_z_ wchar_t const * key_name, _Out_ std::wstring& display_name ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    ASSERT( m_ManagerHandle not_eq static_cast< SC_HANDLE >( NULL ) );

    display_name.clear();

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        DWORD buffer_size { 8192}; // insanely large buffer

        auto display_string{ std::make_unique<wchar_t[]>(buffer_size) };

        auto return_value{ ::GetServiceDisplayName(m_ManagerHandle,
                key_name,
                display_string.get(),
                &buffer_size) };

            if ( return_value not_eq FALSE )
            {
                display_name.assign( display_string.get() );
            }
            else
            {
                m_ErrorCode = ::GetLastError();
                Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
                //WFCTRACEERROR( m_ErrorCode );
            }

            if ( return_value == FALSE )
            {
                return( false );
            }

            return( true );
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ SC_HANDLE Win32FoundationClasses::CServiceControlManager::GetHandle( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( m_ManagerHandle );
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::GetKeyName( __in_z wchar_t const * display_name, _Out_ std::wstring& key_name ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    ASSERT( m_ManagerHandle not_eq static_cast< SC_HANDLE >( NULL ) );

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        key_name.clear();

        DWORD buffer_size{ 8192 }; // insanely large buffer

        auto key_string{ std::make_unique<wchar_t[]>(buffer_size) };

        auto return_value{ ::GetServiceKeyNameW(m_ManagerHandle,
                display_name,
                key_string.get(),
                &buffer_size) };

        if ( return_value not_eq FALSE )
        {
            key_name.assign( key_string.get(), buffer_size );
        }
        else
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
        }

        if ( return_value == FALSE )
        {
            return( false );
        }

        return( true );
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
    Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( FALSE );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::GetNext( _Out_ Win32FoundationClasses::CServiceNameAndStatusW& status ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    status.Empty();

    if ( m_CurrentEntryNumber < m_NumberOfEntries )
    {
        auto status_array{ reinterpret_cast<ENUM_SERVICE_STATUS*>(m_Buffer.get()) };
        status.Copy( &status_array[ m_CurrentEntryNumber ] );
        m_CurrentEntryNumber++;
        return( true );
    }

    return( false );
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::Install( __in_z wchar_t const * service_name, __in_z wchar_t const * friendly_name, __in_z_opt wchar_t const * executable_file ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( service_name );
    WFC_VALIDATE_POINTER( friendly_name );

    if ( service_name  == nullptr or friendly_name == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }

    // We were passed pointers, don't trust them

    WFC_TRY
    {
        TCHAR name_of_executable_file[ MAX_PATH ];

        ZeroMemory( name_of_executable_file, sizeof( name_of_executable_file ) );

        if ( executable_file == nullptr )
        {
            ::GetModuleFileName( static_cast< HMODULE >( NULL ), name_of_executable_file, static_cast<DWORD>(std::size( name_of_executable_file )) );
        }
        else
        {
            _tcsncpy_s( name_of_executable_file, std::size( name_of_executable_file ), executable_file, std::size( name_of_executable_file ) );
        }

        // Microsoft has a bug in NT. I know what you're thinking, "THAT'S IMPOSSIBLE"
        // The programmers at Microsoft are only human so things slip through.
        // A programmer named Lee Ott (not of Microsoft) discovered a bug while
        // debugging Tom Horsley's NTPTime program (highly recommended by me).
        // If your executable name has a space in it (like "c:\Program files\myservice.exe"
        // The service control manager will fail with helpful message of "Event ID 7000:
        // %1 Not A Valid Windows NT Application" Lee discovered that if you put
        // the executable name in quotes, it will work properly.

        std::wstring normalized_executable_name( name_of_executable_file );

        if ( normalized_executable_name.empty() == true )
        {
            m_ErrorCode = ERROR_INVALID_PARAMETER;
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            return( false );
        }

        if ( normalized_executable_name.find(' ') not_eq normalized_executable_name.npos )
        {
            // The name contains a space. We must make sure the name is in quotes

            if ( normalized_executable_name.at( 0 ) not_eq '\"' )
            {
                std::wstring new_name( WSTRING_VIEW(L"\"") );

                new_name.append( normalized_executable_name );
                normalized_executable_name.assign( new_name );
            }

            if ( normalized_executable_name.at( normalized_executable_name.length() - 1 ) not_eq '\"' )
            {
                normalized_executable_name.push_back( '\"' );
            }
        }

        ASSERT( m_ManagerHandle not_eq static_cast< SC_HANDLE >( NULL ) );

        if ( m_ManagerHandle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ERROR_INVALID_HANDLE;
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            return( false );
        }

        DWORD const supported_types{ EVENTLOG_ERROR_TYPE bitor
            EVENTLOG_WARNING_TYPE bitor
            EVENTLOG_INFORMATION_TYPE bitor
            EVENTLOG_AUDIT_SUCCESS bitor
            EVENTLOG_AUDIT_FAILURE };

        // Thanks go to Patrik Sjoberg (pasjo@wmdata.com) for finding a bug here.
        // Install() would fail in unattended installations. Event logging
        // is now a non-fatal error

        auto event_log{ std::make_unique<CEventLog>() };

        if ( event_log.get() not_eq nullptr )
        {
            if ( event_log->CreateApplicationLog( service_name, normalized_executable_name.c_str(), supported_types ) == FALSE )
            {
                event_log.release();
            }
        }

        if ( event_log.get() not_eq nullptr )
        {
            if ( event_log->RegisterSource( service_name ) == FALSE )
            {
                event_log.release();
            }
        }

        auto service_handle{ ::CreateServiceW(m_ManagerHandle,
            service_name,
            friendly_name,
            SERVICE_ALL_ACCESS,
            SERVICE_WIN32_OWN_PROCESS,
            SERVICE_DEMAND_START,
            SERVICE_ERROR_NORMAL,
            normalized_executable_name.c_str(),
            nullptr,
            nullptr,
            L"EventLog\0\0",
            nullptr,
            nullptr) };

        if ( service_handle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );

            if ( event_log not_eq nullptr )
            {
                LPVOID message_buffer{ nullptr };

                ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER bitor FORMAT_MESSAGE_FROM_SYSTEM,
                    nullptr,
                    m_ErrorCode,
                    MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
                    (LPTSTR) &message_buffer,
                    0,
                    nullptr );

                if ( message_buffer not_eq nullptr )
                {
                    std::wstring temp_string;

                    format( temp_string, L"Can't create service because %s at line %d of CSvcMgr.cpp", reinterpret_cast<TCHAR const *>(message_buffer), __LINE__ );

                    event_log->ReportError( temp_string.c_str() );

                    ::LocalFree( message_buffer );
                    message_buffer = nullptr;
                }
            }

            return( false );
        }

        ::CloseServiceHandle( service_handle );
        service_handle = reinterpret_cast< SC_HANDLE >( NULL );

        /*
        ** We successfully installed a new service, this is something we should log
        */

        if ( event_log not_eq nullptr )
        {
            TCHAR user_name[ 2048 ];
            TCHAR temp_string[ 2100 ];

            auto size_of_user_name{ static_cast<DWORD>(std::size(user_name)) };

            ::ZeroMemory( user_name,   sizeof( user_name   ) );
            ::ZeroMemory( temp_string, sizeof( temp_string ) );

            ::GetUserName( user_name, &size_of_user_name );

            _stprintf_s( temp_string, std::size( temp_string ), TEXT( "Service successfully installed by %s" ), user_name );

            event_log->ReportInformation( temp_string );
        }

        return( true );
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::IsDatabaseLocked( _Out_ std::wstring& who_locked_it, _Out_ Win32FoundationClasses::CTimeSpan& how_long_it_has_been_locked ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    ASSERT( m_ManagerHandle not_eq static_cast< SC_HANDLE >( NULL ) );

    // First, initialize the return values

    who_locked_it.clear();
    how_long_it_has_been_locked = CTimeSpan( 0 );

    if ( m_ManagerHandle == static_cast< SC_HANDLE >( NULL ) )
    {
        return( false );
    }

    DWORD buffer_size{ 8192 }; // insanely large buffer
    DWORD number_of_bytes_needed{ 0 };

    auto buffer{ std::make_unique<uint8_t[]>(buffer_size) };

    // Always always always zero buffers

    ::ZeroMemory( buffer.get(), buffer_size );

    auto return_value{ ::QueryServiceLockStatus(m_ManagerHandle,
            reinterpret_cast<LPQUERY_SERVICE_LOCK_STATUS>(buffer.get()),
            buffer_size,
            &number_of_bytes_needed) };

    if ( return_value not_eq FALSE )
    {
        // Get the data from the structure
        auto status_p{ reinterpret_cast<LPQUERY_SERVICE_LOCK_STATUS>(buffer.get()) };

        if ( status_p->fIsLocked not_eq 0 )
        {
            // The database is locked
            return_value = TRUE;
            who_locked_it.assign( status_p->lpLockOwner );
            how_long_it_has_been_locked = CTimeSpan( status_p->dwLockDuration );
        }
        else
        {
            // The database is not locked
            return_value = FALSE;
        }
    }

    return( return_value == FALSE ? false : true );
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::LockDatabase( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_DatabaseLockHandle not_eq static_cast< SC_LOCK >( NULL ) )
    {
        // The database is already locked, unlock it
        std::ignore = UnlockDatabase();
    }

    m_DatabaseLockHandle = ::LockServiceDatabase( m_ManagerHandle );

    if ( m_DatabaseLockHandle == static_cast< SC_LOCK >( NULL ) )
    {
        m_ErrorCode = ::GetLastError();
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);

        //WFCTRACEERROR( m_ErrorCode );

        return( false );
    }

    return( true );
}

void Win32FoundationClasses::CServiceControlManager::m_Initialize( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    m_ManagerHandle      = reinterpret_cast< SC_HANDLE >( NULL );
    m_ErrorCode          = 0;
    m_Buffer             = nullptr;
    m_BufferSize         = 0;
    m_CurrentEntryNumber = 0;
    m_NumberOfEntries    = 0;
    m_DatabaseLockHandle = reinterpret_cast< SC_LOCK >( NULL );
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::Open( _In_ uint32_t const what_to_open, __in_z_opt wchar_t const * database_name, __in_z_opt wchar_t const * machine_name ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    /*
    ** database_name can be nullptr
    */

    if ( m_ManagerHandle not_eq static_cast< SC_HANDLE >( NULL ) )
    {
        Close();
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        if ( machine_name == nullptr )
        {
            m_MachineName.clear(); // Should go and get our machine's name
        }
        else
        {
            m_MachineName.assign( machine_name );
        }

        m_ManagerHandle = ::OpenSCManagerW( machine_name, database_name, what_to_open );

        if ( m_ManagerHandle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
        else
        {
            return( true );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::Pause( __in_z wchar_t const * service_name ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( service_name );

    if ( service_name == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        ASSERT( m_ManagerHandle not_eq static_cast< SC_HANDLE >( NULL ) );

        if ( m_ManagerHandle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ERROR_INVALID_HANDLE;
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            return( false );
        }

        auto service_handle{ ::OpenServiceW(m_ManagerHandle, service_name, SERVICE_PAUSE_CONTINUE) };

        if ( service_handle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }

        SERVICE_STATUS service_status{ 0, 0, 0, 0, 0, 0, 0 };

        auto return_value{ ::ControlService(service_handle, SERVICE_CONTROL_PAUSE, &service_status) };

        if ( return_value == FALSE )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
        }
        else
        {
            return_value = TRUE;
        }

        ::CloseServiceHandle( service_handle );

        return( return_value == FALSE ? false : true );
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::Remove( __in_z wchar_t const * service_name ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( service_name );

    if ( service_name == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        ASSERT( m_ManagerHandle not_eq static_cast< SC_HANDLE >( NULL ) );

        if ( m_ManagerHandle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ERROR_INVALID_HANDLE;
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            return( false );
        }

        auto service_handle{ ::OpenServiceW(m_ManagerHandle, service_name, SERVICE_ALL_ACCESS) };

        if ( service_handle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            return( false );
        }

        /*
        ** We're gonna delete the service, this is something we should record
        */

        {
            TCHAR user_name[ 2048 ];
            TCHAR temp_string[ 2100 ];

            DWORD size_of_user_name{ static_cast<DWORD>(std::size(user_name)) };

            ::ZeroMemory( user_name,   sizeof( user_name   ) );
            ::ZeroMemory( temp_string, sizeof( temp_string ) );

            ::GetUserName( user_name, &size_of_user_name );

            _stprintf_s( temp_string, std::size( temp_string ), TEXT( "Service being removed by %s" ), user_name );

            CEventLog event_log( service_name );

            event_log.ReportInformation( temp_string );
        }

        BOOL return_value{ ::DeleteService(service_handle) };

        if ( return_value == FALSE )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );

            /*
            ** We couldn't delete the service, let's record why...
            */

            LPVOID message_buffer{ nullptr };

            ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER bitor FORMAT_MESSAGE_FROM_SYSTEM,
                nullptr,
                m_ErrorCode,
                MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
                (LPTSTR) &message_buffer,
                0,
                nullptr );

            if ( message_buffer not_eq nullptr )
            {
                TCHAR temp_string[ 255 ];

                _stprintf_s( temp_string, std::size( temp_string ), TEXT( "Can't delete service because %s" ), reinterpret_cast<TCHAR const *>(message_buffer) );

                m_ErrorMessage.assign(temp_string);

                CEventLog event_log( service_name );
                event_log.ReportError( temp_string );

                ::LocalFree( message_buffer );
            }

            ::CloseServiceHandle(service_handle);
            return( false );
        }

        // We no longer need the service handle...

        std::ignore = ::CloseServiceHandle( service_handle );
        service_handle = static_cast< SC_HANDLE >( NULL );

        /*
        ** Now that we've deleted the service, we need to remove it from the event logger
        */

        CEventLog event_log;

        std::ignore = event_log.DeleteApplicationLog( service_name );

        return( true );
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        m_ErrorMessage.assign(WSTRING_VIEW(L"Exception occurred."));
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::SetConfiguration( __in_z wchar_t const * service_name,
                                                            _In_ uint32_t const  when_to_start,
                                                            _In_ uint32_t const  type_of_service,
                                                            _In_ uint32_t const  error_control,
                                                            __in_z_opt wchar_t const * name_of_executable_file,
                                                            __in_z_opt wchar_t const * load_order_group,
                                                            __in_z_opt wchar_t const * dependencies,
                                                            __in_z_opt wchar_t const * start_name,
                                                            __in_z_opt wchar_t const * password,
                                                            __in_z_opt wchar_t const * display_name ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( service_name );

    if ( service_name == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        m_ErrorMessage.append(WSTRING_VIEW(L" because service_name parameter is null."));
        return( false );
    }

    ASSERT( m_ManagerHandle not_eq static_cast< SC_HANDLE >( NULL ) );

    if ( m_ManagerHandle == static_cast< SC_HANDLE >( NULL ) )
    {
        m_ErrorCode = ERROR_INVALID_HANDLE;
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        m_ErrorMessage.append(WSTRING_VIEW(L" because the manager handle is invalid."));
        return( false );
    }

    // We were passed a bunch of pointers, don't trust them

    WFC_TRY
    {
        auto const service_handle { ::OpenServiceW(m_ManagerHandle, service_name, SERVICE_CHANGE_CONFIG)};

        if ( service_handle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
            m_ErrorMessage.append(WSTRING_VIEW(L" because the manager handle is invalid Can't open for config Access."));
            return( false );
        }

        // Lock the database so we can safely make the changes

        bool return_value{ LockDatabase() };

        if ( return_value == true )
        {
            // Microsoft has a bug in NT. I know what you're thinking, "THAT'S IMPOSSIBLE"
            // The programmers at Microsoft are only human so things slip through.
            // A programmer named Lee Ott (not of Microsoft) discovered a bug while
            // debugging Tom Horsley's NTPTime program (highly recommended by me).
            // If your executable name has a space in it (like "c:\Program files\myservice.exe"
            // The service control manager will fail with helpful message of "Event ID 7000:
            // %1 Not A Valid Windows NT Application" Lee discovered that if you put
            // the exeuteable anme in quotes, it will work properly.

            std::wstring normalized_executable_name;

            if ( name_of_executable_file not_eq nullptr )
            {
                normalized_executable_name.assign( name_of_executable_file );
            }

            if ( normalized_executable_name.find(' ') not_eq normalized_executable_name.npos )
            {
                // The name contains a space. We must make sure the name is in quotes

                if ( normalized_executable_name.at( 0 ) not_eq '\"' )
                {
                    std::wstring new_name(WSTRING_VIEW(L"\""));

                    new_name.append( normalized_executable_name );
                    normalized_executable_name.assign( new_name );
                }

                if ( normalized_executable_name.at( normalized_executable_name.length() - 1 ) not_eq '\"' )
                {
                    normalized_executable_name.push_back( '\"' );
                }
            }

            auto const api_return_value{ ::ChangeServiceConfigW(service_handle,
                type_of_service,
                when_to_start,
                error_control,
                (LPCWSTR)((normalized_executable_name.empty() == true) ? (LPCWSTR) nullptr : normalized_executable_name.c_str()),
                load_order_group,
                nullptr,
                dependencies,
                start_name,
                password,
                display_name) };

            if (api_return_value == FALSE )
            {
                m_ErrorCode = ::GetLastError();
                wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
                //WFCTRACEERROR( m_ErrorCode );
                m_ErrorMessage.append(WSTRING_VIEW(L" while calling ChangeServiceConfigW API."));
                return_value = false;
            }
            else
            {
                return_value = true;
            }

            std::ignore = UnlockDatabase();

            ::CloseServiceHandle( service_handle );
            return( return_value );
        }
        else
        {
            // LockDatabase already set m_ErrorCode

            ::CloseServiceHandle( service_handle );
            m_ErrorMessage.append(WSTRING_VIEW(L" while locking database to change service config."));
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::Start( __in_z wchar_t const * service_name, _In_ uint32_t const service_argc, __in_ecount_z_opt( service_argc ) wchar_t const * * service_argv ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( service_name );

    if ( service_name == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        ASSERT( m_ManagerHandle not_eq static_cast< SC_HANDLE >( NULL ) );

        if ( m_ManagerHandle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ERROR_INVALID_HANDLE;
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            return( false );
        }

        auto service_handle{ ::OpenServiceW(m_ManagerHandle, service_name, SERVICE_START) };

        if ( service_handle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }

        auto return_value{ ::StartService(service_handle, service_argc, service_argv) };

        if ( return_value == FALSE )
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
        }
        else
        {
            return_value = TRUE;
        }

        ::CloseServiceHandle( service_handle );

        return( return_value == FALSE ? false : true);
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::Stop( _In_z_ wchar_t const * service_name ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( service_name );

    if ( service_name == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        ASSERT( m_ManagerHandle not_eq static_cast< SC_HANDLE >( NULL ) );

        if ( m_ManagerHandle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ERROR_INVALID_HANDLE;
            Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            return( false );
        }

        // Thanks go to Rainer Kukasch (rk@sicom.de) for finding a bug here.
        // I was not opening the service with permission to query it's status.
        // That caused the QueryServiceStatus() later on to fail.

        auto service_handle{ ::OpenServiceW(m_ManagerHandle, service_name, SERVICE_STOP bitor SERVICE_QUERY_STATUS) };

        if ( service_handle == static_cast< SC_HANDLE >( NULL ) )
        {
            m_ErrorCode = ::GetLastError();
            Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }

        SERVICE_STATUS service_status{ 0, 0, 0, 0, 0, 0, 0 };

        auto return_value{ ::ControlService(service_handle, SERVICE_CONTROL_STOP, &service_status) };

        if ( return_value == FALSE )
        {
            m_ErrorCode = ::GetLastError();
            Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
            ::CloseServiceHandle( service_handle );

            return( false );
        }

        DWORD old_checkpoint{ 0 };
        DWORD number_of_tries{ 0 };

        while( service_status.dwCurrentState not_eq SERVICE_STOPPED )
        {
            old_checkpoint = service_status.dwCheckPoint;

            // Stop pestering the service with status requests

            std::this_thread::sleep_for(std::chrono::milliseconds(service_status.dwWaitHint));

            if ( ::QueryServiceStatus( service_handle, &service_status ) == FALSE )
            {
                ::CloseServiceHandle( service_handle );
                return( false );
            }

            if ( old_checkpoint >= service_status.dwCheckPoint )
            {
                number_of_tries++;

                if ( number_of_tries >= 100 )
                {
                    //WFCTRACE( TEXT( "Service hung while stopping" ) );
                    ::CloseServiceHandle( service_handle );
                    return( false );
                }
            }
            else
            {
                number_of_tries = 0;
            }
        }

        ::CloseServiceHandle( service_handle );
        return( true );
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::UnlockDatabase( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_DatabaseLockHandle == static_cast< SC_LOCK >( NULL ) )
    {
        // Already unlocked
        return( true );
    }

    auto return_value{ ::UnlockServiceDatabase(m_DatabaseLockHandle) };

    if ( return_value == FALSE )
    {
        m_ErrorCode = ::GetLastError();
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        //WFCTRACEERROR( m_ErrorCode );
    }
    else
    {
        return_value = TRUE;
    }

    m_DatabaseLockHandle = reinterpret_cast< SC_LOCK >( NULL );

    return( return_value == FALSE ? false : true );
}

static VOID CALLBACK __stop_notification_callback(PVOID parameter) noexcept
{
    auto notify{ reinterpret_cast<SERVICE_NOTIFYW*>(parameter) };
    auto event_handle{ reinterpret_cast<HANDLE>(notify->pContext) };

    SetEvent(event_handle);
}

_Check_return_ bool Win32FoundationClasses::CServiceControlManager::WaitForStop(_In_z_ wchar_t const * service_name) noexcept
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER(service_name);

    if (service_name == nullptr)
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return(false);
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        ASSERT(m_ManagerHandle not_eq static_cast<SC_HANDLE>(NULL));

        if (m_ManagerHandle == static_cast<SC_HANDLE>(NULL))
        {
            m_ErrorCode = ERROR_INVALID_HANDLE;
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            return(false);
        }

        auto service_handle{ ::OpenServiceW(m_ManagerHandle, service_name, SERVICE_QUERY_STATUS) };

        if (service_handle == static_cast<SC_HANDLE>(NULL))
        {
            m_ErrorCode = ::GetLastError();
            wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
            //WFCTRACEERROR( m_ErrorCode );
            return(false);
        }

        SERVICE_STATUS service_status{ 0, 0, 0, 0, 0, 0, 0 };

        if (::QueryServiceStatus(service_handle, &service_status) == FALSE)
        {
            ::CloseServiceHandle(service_handle);
            return(false);
        }

        if (service_status.dwCurrentState == SERVICE_STOPPED)
        {
            ::CloseServiceHandle(service_handle);
            return(true);
        }

        auto const event_handle{ MANUAL_RESET_EVENT() };

        // The service is running

        SERVICE_NOTIFYW notify;

        ZeroMemory(&notify, sizeof(notify));

        notify.dwVersion = SERVICE_NOTIFY_STATUS_CHANGE;
        notify.pfnNotifyCallback = __stop_notification_callback;
        notify.pContext = event_handle;

        m_ErrorCode = NotifyServiceStatusChangeW(service_handle, SERVICE_NOTIFY_STOPPED, &notify);

        if (m_ErrorCode not_eq ERROR_SUCCESS)
        {
            ::CloseServiceHandle(service_handle);
            std::ignore = Win32FoundationClasses::wfc_close_handle(event_handle);
            return(false);
        }
        else
        {
            Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        }

        WaitForSingleObjectEx(event_handle, INFINITE, TRUE);
        std::ignore = Win32FoundationClasses::wfc_close_handle(event_handle);
        ::CloseServiceHandle(service_handle);
        return(true);
    }
        WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        Win32FoundationClasses::wfc_get_error_string(m_ErrorCode, m_ErrorMessage);
        return(false);
    }
        WFC_END_CATCH_ALL
}

Win32FoundationClasses::CServiceNameAndStatusA::CServiceNameAndStatusA() noexcept
{
    WFC_VALIDATE_POINTER( this );
    Empty();
}

void Win32FoundationClasses::CServiceNameAndStatusA::Copy( _In_ _ENUM_SERVICE_STATUSA const * source_p ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( source_p );

    if ( source_p == nullptr )
    {
        Empty();
        return;
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        lpServiceName                           = source_p->lpServiceName;
        lpDisplayName                           = source_p->lpDisplayName;
        ServiceStatus.dwServiceType             = source_p->ServiceStatus.dwServiceType;
        ServiceStatus.dwCurrentState            = source_p->ServiceStatus.dwCurrentState;
        ServiceStatus.dwControlsAccepted        = source_p->ServiceStatus.dwControlsAccepted;
        ServiceStatus.dwWin32ExitCode           = source_p->ServiceStatus.dwWin32ExitCode;
        ServiceStatus.dwServiceSpecificExitCode = source_p->ServiceStatus.dwServiceSpecificExitCode;
        ServiceStatus.dwCheckPoint              = source_p->ServiceStatus.dwCheckPoint;
        ServiceStatus.dwWaitHint                = source_p->ServiceStatus.dwWaitHint;
    }
    WFC_CATCH_ALL
    {
        Empty();
    }
    WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CServiceNameAndStatusA::Dump( CDumpContext& dump_context ) const
{
    dump_context << TEXT( "a CServiceNameAndStatusA at " ) << (VOID *) this << TEXT( "\n{\n" );
    dump_context << TEXT( "   lpServicename                           = " ) << lpServiceName << TEXT( "\n" );
    dump_context << TEXT( "   lpDisplayName                           = " ) << lpDisplayName << TEXT( "\n" );
    dump_context << TEXT( "   ServiceStatus.dwServiceType             = " ) << ServiceStatus.dwServiceType << TEXT( " (" );

    std::wstring temp_string;

    if (is_flagged(ServiceStatus.dwServiceType, SERVICE_WIN32_OWN_PROCESS) == true )
    {
        temp_string = TEXT( "SERVICE_WIN32_OWN_PROCESS" );
    }

    if (is_flagged( ServiceStatus.dwServiceType, SERVICE_WIN32_SHARE_PROCESS) == true )
    {
        if ( temp_string.GetLength() > 0 )
        {
            temp_string += TEXT( " | " );
        }

        temp_string += TEXT( "SERVICE_WIN32_SHARE_PROCESS" );
    }

    if (is_flagged(ServiceStatus.dwServiceType, SERVICE_KERNEL_DRIVER) == true )
    {
        if ( temp_string.GetLength() > 0 )
        {
            temp_string += TEXT( " | " );
        }

        temp_string += TEXT( "SERVICE_KERNEL_DRIVER" );
    }

    if (is_flagged(ServiceStatus.dwServiceType, SERVICE_FILE_SYSTEM_DRIVER) == true )
    {
        if ( temp_string.GetLength() > 0 )
        {
            temp_string += TEXT( " | " );
        }

        temp_string += TEXT( "SERVICE_FILE_SYSTEM_DRIVER" );
    }

    if (is_flagged(ServiceStatus.dwServiceType, SERVICE_INTERACTIVE_PROCESS) == true )
    {
        if ( temp_string.GetLength() > 0 )
        {
            temp_string += TEXT( " | " );
        }

        temp_string += TEXT( "SERVICE_INTERACTIVE_PROCESS" );
    }

    dump_context << temp_string << TEXT( ")\n" );

    dump_context << TEXT( "   ServiceStatus.dwCurrentState            = " ) << ServiceStatus.dwCurrentState << TEXT( " (" );

    temp_string = TEXT( "" );

    switch( ServiceStatus.dwCurrentState )
    {
    case SERVICE_STOPPED:

        temp_string = TEXT( "SERVICE_STOPPED" );
        break;

    case SERVICE_START_PENDING:

        temp_string = TEXT( "SERVICE_START_PENDING" );
        break;

    case SERVICE_STOP_PENDING:

        temp_string = TEXT( "SERVICE_STOP_PENDING" );
        break;

    case SERVICE_RUNNING:

        temp_string = TEXT( "SERVICE_RUNNING" );
        break;

    case SERVICE_CONTINUE_PENDING:

        temp_string = TEXT( "SERVICE_CONTINUE_PENDING" );
        break;

    case SERVICE_PAUSE_PENDING:

        temp_string = TEXT( "SERVICE_PAUSE_PENDING" );
        break;

    case SERVICE_PAUSED:

        temp_string = TEXT( "SERVICE_PAUSED" );
        break;

    default:

        temp_string = TEXT( "Unknown" );
        break;
    }

    dump_context << temp_string << TEXT( ")\n" );

    dump_context << TEXT( "   ServiceStatus.dwControlsAccepted        = " ) << ServiceStatus.dwControlsAccepted << TEXT( " (" );

    temp_string = TEXT( "" );

    if ( ServiceStatus.dwControlsAccepted bitand SERVICE_ACCEPT_STOP )
    {
        temp_string = TEXT( "SERVICE_ACCEPT_STOP" );
    }

    if ( ServiceStatus.dwControlsAccepted bitand SERVICE_ACCEPT_PAUSE_CONTINUE )
    {
        if ( temp_string.GetLength() > 0 )
        {
            temp_string += TEXT( " | " );
        }

        temp_string += TEXT( "SERVICE_ACCEPT_PAUSE_CONTINUE" );
    }

    if ( ServiceStatus.dwControlsAccepted bitand SERVICE_ACCEPT_SHUTDOWN )
    {
        if ( temp_string.GetLength() > 0 )
        {
            temp_string += TEXT( " | " );
        }

        temp_string += TEXT( "SERVICE_ACCEPT_SHUTDOWN" );
    }

    dump_context << temp_string << TEXT( ")\n" );

    dump_context << TEXT( "   ServiceStatus.dwWin32ExitCode           = " ) << ServiceStatus.dwWin32ExitCode           << TEXT( "\n" );
    dump_context << TEXT( "   ServiceStatus.dwServiceSpecificExitCode = " ) << ServiceStatus.dwServiceSpecificExitCode << TEXT( "\n" );
    dump_context << TEXT( "   ServiceStatus.dwCheckPoint              = " ) << ServiceStatus.dwCheckPoint              << TEXT( "\n" );
    dump_context << TEXT( "   ServiceStatus.dwWaitHint                = " ) << ServiceStatus.dwWaitHint                << TEXT( "\n" );
    dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CServiceNameAndStatusA::Empty( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    lpServiceName                           = nullptr;
    lpDisplayName                           = nullptr;
    ServiceStatus.dwServiceType             = 0;
    ServiceStatus.dwCurrentState            = 0;
    ServiceStatus.dwControlsAccepted        = 0;
    ServiceStatus.dwWin32ExitCode           = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint              = 0;
    ServiceStatus.dwWaitHint                = 0;
}

Win32FoundationClasses::CServiceNameAndStatusW::CServiceNameAndStatusW() noexcept
{
    WFC_VALIDATE_POINTER( this );
    Empty();
}

void Win32FoundationClasses::CServiceNameAndStatusW::Copy( _In_ _ENUM_SERVICE_STATUSW const * source_p ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( source_p );

    if ( source_p == nullptr )
    {
        Empty();
        return;
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        lpServiceName                           = source_p->lpServiceName;
        lpDisplayName                           = source_p->lpDisplayName;
        ServiceStatus.dwServiceType             = source_p->ServiceStatus.dwServiceType;
        ServiceStatus.dwCurrentState            = source_p->ServiceStatus.dwCurrentState;
        ServiceStatus.dwControlsAccepted        = source_p->ServiceStatus.dwControlsAccepted;
        ServiceStatus.dwWin32ExitCode           = source_p->ServiceStatus.dwWin32ExitCode;
        ServiceStatus.dwServiceSpecificExitCode = source_p->ServiceStatus.dwServiceSpecificExitCode;
        ServiceStatus.dwCheckPoint              = source_p->ServiceStatus.dwCheckPoint;
        ServiceStatus.dwWaitHint                = source_p->ServiceStatus.dwWaitHint;
    }
    WFC_CATCH_ALL
    {
        Empty();
    }
    WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CServiceNameAndStatusW::Dump( CDumpContext& dump_context ) const
{
    dump_context << TEXT( "a CServiceNameAndStatusW at " ) << (VOID *) this << TEXT( "\n{\n" );
    dump_context << TEXT( "   lpServicename                           = " ) << lpServiceName << TEXT( "\n" );
    dump_context << TEXT( "   lpDisplayName                           = " ) << lpDisplayName << TEXT( "\n" );
    dump_context << TEXT( "   ServiceStatus.dwServiceType             = " ) << ServiceStatus.dwServiceType << TEXT( " (" );

    std::wstring temp_string;

    if ( ServiceStatus.dwServiceType bitand SERVICE_WIN32_OWN_PROCESS )
    {
        temp_string = TEXT( "SERVICE_WIN32_OWN_PROCESS" );
    }

    if ( ServiceStatus.dwServiceType bitand SERVICE_WIN32_SHARE_PROCESS )
    {
        if ( temp_string.GetLength() > 0 )
        {
            temp_string += TEXT( " | " );
        }

        temp_string += TEXT( "SERVICE_WIN32_SHARE_PROCESS" );
    }

    if ( ServiceStatus.dwServiceType bitand SERVICE_KERNEL_DRIVER  )
    {
        if ( temp_string.GetLength() > 0 )
        {
            temp_string += TEXT( " | " );
        }

        temp_string += TEXT( "SERVICE_KERNEL_DRIVER" );
    }

    if ( ServiceStatus.dwServiceType bitand SERVICE_FILE_SYSTEM_DRIVER )
    {
        if ( temp_string.GetLength() > 0 )
        {
            temp_string += TEXT( " | " );
        }

        temp_string += TEXT( "SERVICE_FILE_SYSTEM_DRIVER" );
    }

    if ( ServiceStatus.dwServiceType bitand SERVICE_INTERACTIVE_PROCESS )
    {
        if ( temp_string.GetLength() > 0 )
        {
            temp_string += TEXT( " | " );
        }

        temp_string += TEXT( "SERVICE_INTERACTIVE_PROCESS" );
    }

    dump_context << temp_string << TEXT( ")\n" );

    dump_context << TEXT( "   ServiceStatus.dwCurrentState            = " ) << ServiceStatus.dwCurrentState << TEXT( " (" );

    temp_string = TEXT( "" );

    switch( ServiceStatus.dwCurrentState )
    {
    case SERVICE_STOPPED:

        temp_string = TEXT( "SERVICE_STOPPED" );
        break;

    case SERVICE_START_PENDING:

        temp_string = TEXT( "SERVICE_START_PENDING" );
        break;

    case SERVICE_STOP_PENDING:

        temp_string = TEXT( "SERVICE_STOP_PENDING" );
        break;

    case SERVICE_RUNNING:

        temp_string = TEXT( "SERVICE_RUNNING" );
        break;

    case SERVICE_CONTINUE_PENDING:

        temp_string = TEXT( "SERVICE_CONTINUE_PENDING" );
        break;

    case SERVICE_PAUSE_PENDING:

        temp_string = TEXT( "SERVICE_PAUSE_PENDING" );
        break;

    case SERVICE_PAUSED:

        temp_string = TEXT( "SERVICE_PAUSED" );
        break;

    default:

        temp_string = TEXT( "Unknown" );
        break;
    }

    dump_context << temp_string << TEXT( ")\n" );

    dump_context << TEXT( "   ServiceStatus.dwControlsAccepted        = " ) << ServiceStatus.dwControlsAccepted << TEXT( " (" );

    temp_string = TEXT( "" );

    if ( ServiceStatus.dwControlsAccepted bitand SERVICE_ACCEPT_STOP )
    {
        temp_string = TEXT( "SERVICE_ACCEPT_STOP" );
    }

    if ( ServiceStatus.dwControlsAccepted bitand SERVICE_ACCEPT_PAUSE_CONTINUE )
    {
        if ( temp_string.GetLength() > 0 )
        {
            temp_string += TEXT( " | " );
        }

        temp_string += TEXT( "SERVICE_ACCEPT_PAUSE_CONTINUE" );
    }

    if ( ServiceStatus.dwControlsAccepted bitand SERVICE_ACCEPT_SHUTDOWN )
    {
        if ( temp_string.GetLength() > 0 )
        {
            temp_string += TEXT( " | " );
        }

        temp_string += TEXT( "SERVICE_ACCEPT_SHUTDOWN" );
    }

    dump_context << temp_string << TEXT( ")\n" );

    dump_context << TEXT( "   ServiceStatus.dwWin32ExitCode           = " ) << ServiceStatus.dwWin32ExitCode           << TEXT( "\n" );
    dump_context << TEXT( "   ServiceStatus.dwServiceSpecificExitCode = " ) << ServiceStatus.dwServiceSpecificExitCode << TEXT( "\n" );
    dump_context << TEXT( "   ServiceStatus.dwCheckPoint              = " ) << ServiceStatus.dwCheckPoint              << TEXT( "\n" );
    dump_context << TEXT( "   ServiceStatus.dwWaitHint                = " ) << ServiceStatus.dwWaitHint                << TEXT( "\n" );
    dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CServiceNameAndStatusW::Empty( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    lpServiceName                           = nullptr;
    lpDisplayName                           = nullptr;
    ServiceStatus.dwServiceType             = 0;
    ServiceStatus.dwCurrentState            = 0;
    ServiceStatus.dwControlsAccepted        = 0;
    ServiceStatus.dwWin32ExitCode           = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint              = 0;
    ServiceStatus.dwWaitHint                = 0;
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CServiceControlManager</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that makes it easy to manage NT services. Let's you start/stop/install or remove services.">
</HEAD>

<BODY>

<H1>CServiceControlManager</H1>
$Revision: 47 $
<HR>

<H2>Description</H2>
This class handles playing with NT's Service Control Manager (SCM).
The SCM manages the starting/stopping/installing/removing/etc. of services.

<H2>Data Members</H2>
None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes your session with the SCM.

<DT><PRE>BOOL <B><A NAME="Continue">Continue</A></B>( LPCTSTR service_name )</PRE><DD>
You give this function the name of a paused service. It will
tell the named service to continue.

<DT><PRE>BOOL <B><A NAME="EnableInteractiveServices">EnableInteractiveServices</A></B>( BOOL enable_interactive_services = TRUE )</PRE><DD>
This function returns TRUE if the setting was set, FALSE if there was a problem.
Calling <B>EnableInteractiveServices</B>() with <CODE>enable_interactive_services</CODE>
set to FALSE will prevent any service from interacting with the desktop/user.

<DT><PRE>BOOL <B><A NAME="EnumerateStatus">EnumerateStatus</A></B>( DWORD state = SERVICE_ACTIVE, type = SERVICE_WIN32 )</PRE><DD>
This function allows you to enumerate services that have a particular status.
The <CODE>state</CODE> parameter may be one of:

<UL>
<LI>SERVICE_ACTIVE
<LI>SERVICE_INACTIVE
<LI>SERVICE_STATE_ALL
</UL>

The <CODE>type</CODE> parameter may be one of:

<UL>
<LI>SERVICE_WIN32
<LI>SERVICE_DRIVER
</UL>

<DT><PRE>BOOL <B><A NAME="GetConfiguration">GetConfiguration</A></B>( LPCTSTR service_name, CServiceConfiguration&amp; configuration )</PRE><DD>
This function retrieves useful information about how a service is configured.

<DT><PRE>BOOL <B><A NAME="GetDependencies">GetDependencies</A></B>( LPCTSTR service_name, std::vector&lt;std::wstring&gt;&amp; dependencies )</PRE><DD>
Fills <CODE>dependencies</CODE> with the names of services that must be running before
the <CODE>service_name</CODE> service can run.

<DT><PRE>DWORD <B><A NAME="GetErrorCode">GetErrorCode</A></B>( void ) const</PRE><DD>
Retrieves the error code. Call this function to find out why any other class member returned FALSE.

<DT><PRE>SC_HANDLE <B><A NAME="GetHandle">GetHandle</A></B>( void ) const</PRE><DD>
Returns the encapsulated SC_HANDLE.
Use this method if you want to call the Win32 API directly
(i.e. you don't want to use WFC).

<DT><PRE>BOOL <B><A NAME="GetKeyName">GetKeyName</A></B>( LPCTSTR display_name, std::wstring&amp; key_name )</PRE><DD>
Retrieves a service's internal name (real name) from the user-friendly name.

<DT><PRE>BOOL <B><A NAME="GetNext">GetNext</A></B>( CServiceNameAndStatus&amp; status )</PRE><DD>
This function is called to retrieve the next name and status (CServiceNameAndStatus)
being enumerated via <B>EnumerateStatus</B>. It will return FALSE
when the last enumerated service has been retrieved.

<DT><PRE>BOOL <B><A NAME="Install">Install</A></B>( LPCTSTR service_name, LPCTSTR friendly_name, LPCTSTR name_of_executable_file )</PRE><DD>
This function will install a new service.

<DT><PRE>BOOL <B><A NAME="IsDatabaseLocked">IsDatabaseLocked</A></B>( std::wstring&amp; who_locked_it, CTimeSpan&amp; how_long_it_has_been_locked )</PRE><DD>
If <B>IsDatabaseLocked</B>() returns TRUE, the database is locked, <CODE>who_locked_it</CODE>
will be filled with the name of the user who locked it, <CODE>how_long_it_has_been_locked</CODE>
will be filled with how long that user has had the database locked. If
<B>IsDatabaseLocked</B>() returns FALSE, <CODE>who_locked_it</CODE> will be
empty and <CODE>how_long_it_has_been_locked</CODE> will be zero.

<DT><PRE>BOOL <B><A NAME="LockDatabase">LockDatabase</A></B>( void )</PRE><DD>
Locks the service control database. Only one process can lock the database at
any given time. You cannot start a service when the database is locked.The lock
prevents the service control manager from starting a service while it is
being reconfigured. 

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( DWORD   what_to_open  = SC_MANAGER_ALL_ACCESS,
LPCTSTR database_name = nullptr,
LPCTSTR machine_name  = nullptr )</PRE><DD>
Opens a connection to the SCM on a machine.

<DT><PRE>BOOL <B><A NAME="Pause">Pause</A></B>( LPCTSTR service_name )</PRE><DD>
Pauses the named service.

<DT><PRE>BOOL <B><A NAME="Remove">Remove</A></B>( LPCTSTR service_name )</PRE><DD>
Un-installs a service. <B>WARNING!</B> This does a lot
of stuff for you. One of the things it will screw up is the Event Log. It removes the source of messages so you will no longer be
able to read entries in the log that the removed service made.

<DT><PRE><B><A NAME="SetConfiguration">SetConfiguration</A></B>( LPCTSTR service_name,
DWORD   when_to_start           = SERVICE_NO_CHANGE,
DWORD   type_of_service         = SERVICE_NO_CHANGE,
DWORD   error_control           = SERVICE_NO_CHANGE,
LPCTSTR name_of_executable_file = nullptr,
LPCTSTR load_order_group        = nullptr,
LPCTSTR dependencies            = nullptr,
LPCTSTR start_name              = nullptr,
LPCTSTR password                = nullptr,
LPCTSTR display_name            = nullptr )</PRE><DD>
Changes a service's configuration. You can change 
when a service is to start, type, name of the executable file, etc.

<DT><PRE>BOOL <B><A NAME="Start">Start</A></B>( LPCTSTR service_name, DWORD service_argc = 0, LPCTSTR *service_argv = nullptr )</PRE><DD>
Starts a service by name. You supply the startup parameters.

<DT><PRE>BOOL <B><A NAME="Stop">Stop</A></B>( LPCTSTR service_name )</PRE><DD>
Stops a service by name.

<DT><PRE>BOOL <B><A NAME="UnlockDatabase">UnlockDatabase</A></B>( void )</PRE><DD>
Unlocks the service control database.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;include &quot;messages.h&quot;
&#35;pragma hdrstop

&#35;if defined( _DEBUG )
&#35;undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
&#35;endif

DWORD WINAPI worker_thread( LPVOID pointer_to_parent_CService_class );
VOID set_default_parameters( void );

int __cdecl _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

if ( number_of_command_line_arguments == 1 )
{
<A HREF="CService.htm">CService</A> service( worker_thread );
service.Initialize( TEXT( &quot;WatchDog&quot; ) );
return( EXIT_SUCCESS );
}

if ( number_of_command_line_arguments == 2 )
{
if ( _tcsicmp( command_line_arguments[ 1 ], TEXT( &quot;install&quot; ) ) == 0 )
{
<B>CServiceControlManager</B> service_control_manager;

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
<B>CServiceControlManager</B> service_control_manager;

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

if ( registry.Connect( <A HREF="REGISTRY.htm">CRegistry</A>::keyLocalMachine ) == FALSE )
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

<B>CServiceControlManager</B> service_control_manager;

/&#42;
** Sleep for one minute, this is in case we are starting during boot-up. We want
** to give the service control manager time to start all necessary services before
** we begin restarting stopped services.
&#42;/

std::this_thread::sleep_for(std::chrono::milliseconds(60));

do
{
if ( machine_name.IsEmpty() )
{
return_value = service_control_manager.Open( GENERIC_READ, nullptr, (LPCTSTR) nullptr );
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
if ( registry.Create( TEXT( &quot;SYSTEM\\CurrentControlSet\\Services\\WatchDog\\Parameters&quot; ) not_eq FALSE ) )
{
DWORD default_sleep_time {60};

if ( registry.SetValue( TEXT( &quot;NumberOfSecondsBetweenChecks&quot; ), default_sleep_time ) == FALSE )
{
LPVOID message_buffer{ nullptr };

::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
nullptr,
registry.GetErrorCode(),
MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
(LPTSTR) &amp;message_buffer,
0,
nullptr
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
LPVOID message_buffer{ nullptr };

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
LPVOID message_buffer{ nullptr };

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

<A HREF="CEVNTLOG.htm">CEventLog</A> event_log( TEXT( &quot;WatchDog&quot; ) );

event_log.Report( <A HREF="CEVNTLOG.htm">CEventLog</A>::eventError, 0, MSG_CANT_CREATE_REGISTRY_KEY, 2, (LPCTSTR *) string_array );

if ( message_buffer not_eq nullptr )
{
::LocalFree( message_buffer );
}
}
}
else
{
LPVOID message_buffer{ nullptr };

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

<B>CServiceControlManager</B> encapsulates the following API's:
<UL>
<LI>ChangeServiceConfig
<LI>CloseServiceHandle
<LI>ControlService
<LI>CreateService
<LI>DeleteService
<LI>EnumDependentServices
<LI>EnumServicesStatus
<LI>FormatMessage
<LI>GetLastError
<LI>GetServiceDisplayName
<LI>GetServiceKeyName
<LI>LocalFree
<LI>LockServiceDatabase
<LI>OpenSCManager
<LI>OpenService
<LI>QueryServiceConfig
<LI>QueryServiceLockStatus
<LI>Sleep
<LI>StartService
<LI>UnlockServiceDatabase
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: csvcmgr.cpp $<BR>
$Modtime: 6/26/01 10:51a $
</BODY>

</HTML>
*/
