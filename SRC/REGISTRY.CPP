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
** $Workfile: registry.cpp $
** $Revision: 45 $
** $Modtime: 6/26/01 10:55a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

static _Check_return_ LONG _recursively_delete_all_sub_keys( _In_ HKEY key_handle, __in_z wchar_t const * key_name ) noexcept
{
    WFC_VALIDATE_POINTER( key_name );

    auto child_key_handle = static_cast< HKEY >( NULL );

    LONG return_value = RegOpenKeyEx( key_handle, key_name, 0, KEY_ALL_ACCESS, &child_key_handle );

    if ( return_value not_eq ERROR_SUCCESS )
    {
        return( return_value );
    }

    auto temporary_key_name = std::make_unique<wchar_t[]>(MAX_PATH);

    if ( temporary_key_name.get() == nullptr )
    {
        return( ERROR_NOT_ENOUGH_MEMORY );
    }

    return_value = RegEnumKeyW( child_key_handle, 0, temporary_key_name.get(), MAX_PATH );

    while( return_value == ERROR_SUCCESS )
    {
        (void) _recursively_delete_all_sub_keys( child_key_handle, temporary_key_name.get() );

        return_value = RegEnumKeyW( child_key_handle, 0, temporary_key_name.get(), MAX_PATH );
    }

    RegCloseKey( child_key_handle );

    return_value = RegDeleteKeyW( key_handle, key_name );

    return( return_value );
}

Win32FoundationClasses::CRegistry::CRegistry()
{
    WFC_VALIDATE_POINTER( this );
    m_Initialize();
}

Win32FoundationClasses::CRegistry::~CRegistry()
{
    WFC_VALIDATE_POINTER( this );

    if ( m_RegistryHandle not_eq static_cast< HKEY >( NULL ) )
    {
        (void) Close();
    }

    m_Initialize();
}

void Win32FoundationClasses::CRegistry::m_Initialize( void )
{
    WFC_VALIDATE_POINTER( this );

    /*
    ** Make sure everything is zeroed out
    */

    m_ClassName.clear();
    m_ComputerName.clear();
    m_KeyName.clear();
    m_RegistryName.clear();

    m_KeyHandle                    = static_cast< HKEY >( NULL );
    m_ErrorCode                    = 0L;
    m_NumberOfSubkeys              = 0;
    m_LongestSubkeyNameLength      = 0;
    m_LongestClassNameLength       = 0;
    m_NumberOfValues               = 0;
    m_LongestValueNameLength       = 0;
    m_LongestValueDataLength       = 0;
    m_SecurityDescriptorLength     = 0;
    m_LastWriteTime.dwLowDateTime  = 0;
    m_LastWriteTime.dwHighDateTime = 0;

    // Thanks go to Chad Christenson (chadc@cwcinc.com) for finding
    // the bug where m_RegistryHandle was never initialized

    m_RegistryHandle               = static_cast< HKEY >( NULL );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::Close( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    // Thanks go to Gareth Isaac (humbert@tcp.co.uk) for finding a resource leak here

    if ( m_KeyHandle not_eq static_cast< HKEY >( NULL ) )
    {
        ::RegCloseKey( m_KeyHandle );
        m_KeyHandle = static_cast< HKEY >( NULL );
    }

    if ( m_RegistryHandle == static_cast< HKEY >( NULL ) )
    {
        //WFCTRACE( TEXT( "Registry already closed" ) );
        return( true );
    }

    m_ErrorCode = ::RegCloseKey( m_RegistryHandle );

    if ( m_ErrorCode == ERROR_SUCCESS )
    {
        m_RegistryHandle = static_cast< HKEY >( NULL );
        m_Initialize();

        return( true );
    }
    else
    {
        //WFCTRACEERROR( m_ErrorCode );
        return( false );
    }
}

_Check_return_ bool Win32FoundationClasses::CRegistry::Connect( _In_ HKEY key_to_open, __in_z_opt wchar_t const * name_of_computer ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( name_of_computer );

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        /*
        ** name_of_computer can be nullptr
        */

        if ( key_to_open == (HKEY) keyClassesRoot or key_to_open == (HKEY) keyCurrentUser )
        {
            if ( name_of_computer == nullptr )
            {
                m_RegistryHandle = key_to_open;
                m_ErrorCode      = ERROR_SUCCESS;
            }
            else
            {
                /*
                ** NT won't allow you to connect to these hives via RegConnectRegistry so we'll just skip that step
                */

                //WFCTRACE( TEXT( "You cannot connect to keyClassesRoot or keyCurrentUser on a remote machine" ) );
                m_ErrorCode = ERROR_INVALID_HANDLE;
            }
        }
        else
        {
            //WFCTRACE( TEXT( "Connecting to registry" ) );

            // Thanks to Paul Ostrowski [postrowski@xantel.com] for finding UNICODE bug here

            // RegConnectRegistry is not const correct

            m_ErrorCode = ::RegConnectRegistry( (LPTSTR) name_of_computer, key_to_open, &m_RegistryHandle );
        }

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            if ( name_of_computer == nullptr )
            {
                wchar_t computer_name[MAX_PATH]{ 0 };
                DWORD size = MAX_PATH;

                if ( ::GetComputerNameEx(COMPUTER_NAME_FORMAT::ComputerNameDnsHostname, computer_name, &size ) == FALSE )
                {
                    m_ComputerName.clear();
                }
                else
                {
                    m_ComputerName.assign( computer_name );
                }
            }
            else
            {
                m_ComputerName.assign( name_of_computer );
            }

            /*
            ** It would be nice to use a switch statement here but I get a "not integral" error!
            */

            if ( key_to_open == HKEY_LOCAL_MACHINE )
            {
                m_RegistryName.assign(WSTRING_VIEW(L"HKEY_LOCAL_MACHINE"));
            }
            else if ( key_to_open == HKEY_CLASSES_ROOT )
            {
                m_RegistryName.assign(WSTRING_VIEW(L"HKEY_CLASSES_ROOT"));
            }
            else if ( key_to_open == HKEY_USERS )
            {
                m_RegistryName.assign(WSTRING_VIEW(L"HKEY_USERS"));
            }
            else if ( key_to_open == HKEY_CURRENT_USER )
            {
                m_RegistryName.assign(WSTRING_VIEW(L"HKEY_CURRENT_USER"));
            }
            else if ( key_to_open == HKEY_PERFORMANCE_DATA )
            {
                m_RegistryName.assign(WSTRING_VIEW(L"HKEY_PERFORMANCE_DATA"));
            }
#if ( WINVER >= 0x400 )
            else if ( key_to_open == HKEY_CURRENT_CONFIG )
            {
                m_RegistryName.assign(WSTRING_VIEW(L"HKEY_CURRENT_CONFIG"));
            }
            else if ( key_to_open == HKEY_DYN_DATA )
            {
                m_RegistryName.assign(WSTRING_VIEW(L"HKEY_DYN_DATA"));
            }
#endif
            else
            {
                m_RegistryName.assign(WSTRING_VIEW(L"Unknown"));
            }

            //WFCTRACEVAL( TEXT( "Connected to " ), m_ComputerName );
            //WFCTRACEVAL( TEXT( "Key " ), m_RegistryName );

            return( true );
        }
        else
        {
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CRegistry::Create( __in_z wchar_t const *               name_of_subkey,
                                     __in_z_opt wchar_t const *               name_of_class,
                                     _In_ CreateOptions         options, 
                                     _In_ CreatePermissions     permissions, 
                                     __in_opt LPSECURITY_ATTRIBUTES security_attributes_p,
                                     __in_opt CreationDisposition * disposition_p ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( name_of_subkey );

    if ( name_of_subkey == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        return( false );
    }

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        DWORD disposition = 0;

        if ( name_of_class == nullptr )
        {
            name_of_class = L""; // Paul Ostrowski [postrowski@xantel.com]
        }

        if ( m_KeyHandle not_eq static_cast< HKEY >( NULL ) )
        {
            ::RegCloseKey( m_KeyHandle );
            m_KeyHandle = static_cast< HKEY >( NULL );
        }

        m_ErrorCode = ::RegCreateKeyExW( m_RegistryHandle,
            name_of_subkey,
            (DWORD) 0,
            (LPTSTR) name_of_class, // Paul Ostrowski [postrowski@xantel.com]
            static_cast<DWORD>(options),
            permissions,
            security_attributes_p,
            &m_KeyHandle,
            &disposition );

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            if ( disposition_p not_eq nullptr )
            {
                *disposition_p = (CreationDisposition) disposition;
            }

            m_KeyName.assign( name_of_subkey );

            return( true );
        }
        else
        {
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CRegistry::DeleteKey( __in_z wchar_t const * name_of_key_to_delete ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( name_of_key_to_delete );

    if ( name_of_key_to_delete == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        return( false );
    }

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        /*
        ** You can't delete a key given a full path. What you have to do is back up one level and then do a delete
        */

        std::wstring full_key_name( name_of_key_to_delete );

        if ( full_key_name.find( '\\' ) == std::wstring::npos )
        {
            /*
            ** User had not given us a full path so assume the name of the key he passed us
            ** is a key off of the current key
            */

            m_ErrorCode = ::_recursively_delete_all_sub_keys( m_KeyHandle, name_of_key_to_delete );
        }
        else
        {
            std::size_t last_back_slash_location = full_key_name.length() - 1;

            /*
            ** We know this loop will succeed because a back slash was found in the above if statement
            */

            while( full_key_name.at( last_back_slash_location ) not_eq '\\' )
            {
                last_back_slash_location--;
            }

            std::wstring currently_opened_key_name = m_KeyName;

            std::wstring parent_key_name = full_key_name.substr( 0, last_back_slash_location );
            std::wstring child_key_name  = right( full_key_name, ( full_key_name.length() - last_back_slash_location ) - 1 );

            /*
            ** Now we open the parent key and delete the child
            */

            if ( Open( parent_key_name.c_str() ) == true )
            {
                m_ErrorCode = ::_recursively_delete_all_sub_keys( m_KeyHandle, child_key_name.c_str() );
            }
            else
            {
                m_KeyName.assign( currently_opened_key_name );
                return( false );
            }
        }

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            return( true );
        }
        else
        {
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CRegistry::DeleteValue( __in_z wchar_t const * name_of_value_to_delete ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    /*
    ** name_of_value_to_delete can be nullptr
    */

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        m_ErrorCode = ::RegDeleteValueW( m_KeyHandle, name_of_value_to_delete );

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            return( true );
        }
        else
        {
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CRegistry::Dump( CDumpContext& dump_context ) const
{
    CObject::Dump( dump_context );

    dump_context << TEXT( "m_KeyHandle = "                ) << m_KeyHandle                << TEXT( "\n" );
    dump_context << TEXT( "m_RegistryHandle = "           ) << m_RegistryHandle           << TEXT( "\n" );
    dump_context << TEXT( "m_ClassName = \""              ) << m_ClassName                << TEXT( "\"\n" );
    dump_context << TEXT( "m_ComputerName = \""           ) << m_ComputerName             << TEXT( "\"\n" );
    dump_context << TEXT( "m_KeyName = \""                ) << m_KeyName                  << TEXT( "\"\n" );
    dump_context << TEXT( "m_RegistryName = \""           ) << m_RegistryName             << TEXT( "\"\n" );
    dump_context << TEXT( "m_NumberOfSubkeys = "          ) << m_NumberOfSubkeys          << TEXT( "\n" );
    dump_context << TEXT( "m_NumberOfValues = "           ) << m_NumberOfValues           << TEXT( "\n" );
    dump_context << TEXT( "m_LongestSubkeyNameLength = "  ) << m_LongestSubkeyNameLength  << TEXT( "\n" );
    dump_context << TEXT( "m_LongestClassNameLength = "   ) << m_LongestClassNameLength   << TEXT( "\n" );
    dump_context << TEXT( "m_LongestValueNameLength = "   ) << m_LongestValueNameLength   << TEXT( "\n" );
    dump_context << TEXT( "m_LongestValueDataLength = "   ) << m_LongestValueDataLength   << TEXT( "\n" );
    dump_context << TEXT( "m_SecurityDescriptorLength = " ) << m_SecurityDescriptorLength << TEXT( "\n" );

    // Mike Berger (mberger@smg.seagatesoftware.com) found a bug where there was no
    // << operator for FILETIME structure. Color me stupid for not catching it...

    dump_context << TEXT( "m_LastWriteTime.dwLowDateTime  = " ) << m_LastWriteTime.dwLowDateTime  << TEXT( "\n" );
    dump_context << TEXT( "m_LastWriteTime.dwHighDateTime = " ) << m_LastWriteTime.dwHighDateTime << TEXT( "\n" );
}

#endif // _DEBUG

_Check_return_ bool Win32FoundationClasses::CRegistry::EnumerateKeys( _In_ DWORD const subkey_index, _Out_ std::wstring& subkey_name, _Out_ std::wstring& class_name ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    subkey_name.clear();
    class_name.clear();

    wchar_t subkey_name_string[ 2048 ];
    wchar_t class_name_string[ 2048 ];

    DWORD size_of_subkey_name_string = static_cast<DWORD>(std::size( subkey_name_string ) - 1);
    DWORD size_of_class_name_string  = static_cast<DWORD>(std::size( class_name_string  ) - 1);

    ::ZeroMemory( subkey_name_string, sizeof( subkey_name_string ) );
    ::ZeroMemory( class_name_string,  sizeof( class_name_string  ) );

    m_ErrorCode = ::RegEnumKeyExW( m_KeyHandle, 
        subkey_index, 
        subkey_name_string, 
        &size_of_subkey_name_string,
        nullptr,
        class_name_string,
        &size_of_class_name_string,
        &m_LastWriteTime );

    if ( m_ErrorCode == ERROR_SUCCESS )
    {
        subkey_name.assign( subkey_name_string );
        class_name.assign( class_name_string );

        return( true );
    }
    else
    {
        //WFCTRACEERROR( m_ErrorCode );
        return( false );
    }
}

_Check_return_ bool Win32FoundationClasses::CRegistry::EnumerateValues( _In_ DWORD const    value_index,
                                              _Out_ std::wstring&       name_of_value,
                                              _Out_ KeyValueTypes& type_code,
                                              __out_bcount_part( size_of_data_buffer, size_of_data_buffer) LPBYTE data_buffer,
                                              __inout DWORD&         size_of_data_buffer ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    /*
    ** data_buffer and size_of_data_buffer can be nullptr
    */

    type_code = KeyValueTypes::None;
    name_of_value.clear();

    DWORD temp_type_code = static_cast<DWORD>(type_code);

    wchar_t temp_name[ 2048 ];

    ::ZeroMemory( temp_name, sizeof( temp_name ) );

    DWORD temp_name_size = static_cast<DWORD>(std::size( temp_name ));

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        m_ErrorCode = ::RegEnumValueW( m_KeyHandle,
            value_index,
            temp_name,
            &temp_name_size,
            nullptr,
            &temp_type_code,
            data_buffer,
            &size_of_data_buffer );

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            type_code     = static_cast<KeyValueTypes>(temp_type_code);
            name_of_value.assign( temp_name );

            return( true );
        }
        else
        {
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CRegistry::Flush( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    m_ErrorCode = ::RegFlushKey( m_KeyHandle );

    if ( m_ErrorCode == ERROR_SUCCESS )
    {
        return( true );
    }
    else
    {
        //WFCTRACEERROR( m_ErrorCode );
        return( false );
    }
}

_Check_return_ bool Win32FoundationClasses::CRegistry::GetBinaryValue( __in_z wchar_t const * name_of_value, _Out_ std::vector<uint8_t>& return_array ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    // Thanks go to Chris Hines (ChrisHines@msn.com) for finding
    // a bug here. If you add entries to the key, then the
    // information retrieved via QueryInfo() may be invalid. This
    // will screw you here. So, we must make sure our information
    // is correct before we attempt to *use* the data.

    return_array.clear();

    (void) QueryInfo();

    std::size_t size_of_buffer = m_LongestValueDataLength;

    auto allocated_buffer = std::make_unique<uint8_t[]>(size_of_buffer);

    auto memory_buffer = allocated_buffer.get();

    if ( memory_buffer == nullptr )
    {
        m_ErrorCode = ::GetLastError();
        //WFCTRACEERROR( m_ErrorCode );
        return( false );
    }

    bool return_value = true;

    auto type = KeyValueTypes::Binary;

    if ( QueryValue( name_of_value, type, memory_buffer, size_of_buffer ) == true )
    {
        /*
        ** We've got data so give it back to the caller
        */

        return_array.clear();

        for ( auto const index : Range(size_of_buffer) )
        {
            return_array.push_back( memory_buffer[ index ] );
        }

        return_value = true;
    }
    else
    {
        return_value = false;
    }

    return( return_value );
}

void Win32FoundationClasses::CRegistry::GetTheClassName( _Out_ std::wstring& class_name ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    class_name.assign( m_ClassName );
}

void Win32FoundationClasses::CRegistry::GetComputerName( _Out_ std::wstring& computer_name ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    computer_name.assign( m_ComputerName );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::GetDoubleWordValue( _In_z_ wchar_t const * name_of_value, _Out_ DWORD& return_value ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    std::size_t size_of_buffer = sizeof( DWORD );

    auto type = KeyValueTypes::DoubleWord;

    return( QueryValue( name_of_value, type, reinterpret_cast<uint8_t *>(&return_value), size_of_buffer ) );
}

void Win32FoundationClasses::CRegistry::GetKeyName( _Out_ std::wstring& key_name ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    key_name.assign( m_KeyName );
}

void Win32FoundationClasses::CRegistry::GetRegistryName( _Out_ std::wstring& registry_name ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    registry_name.assign( m_RegistryName );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::GetSecurity( _In_ SECURITY_INFORMATION const what_you_want_to_know,
                                          __out_bcount_part( size_of_data_buffer, size_of_data_buffer ) PSECURITY_DESCRIPTOR data_buffer,
                                          __inout DWORD&                     size_of_data_buffer ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( data_buffer );

    if ( data_buffer == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        return( false );
    }

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        m_ErrorCode = ::RegGetKeySecurity( m_KeyHandle,
            what_you_want_to_know,
            data_buffer,
            &size_of_data_buffer );

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            return( true );
        }
        else
        {
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CRegistry::GetStringValue(_In_z_ wchar_t const * name_of_value, _Out_ std::wstring& return_string) noexcept
{
    WFC_VALIDATE_POINTER(this);

    wchar_t temp_string[2048];
    std::size_t size_of_buffer = std::size(temp_string);

    ::ZeroMemory(temp_string, sizeof(temp_string));

    auto type = KeyValueTypes::String;

    if (QueryValue(name_of_value, type, reinterpret_cast<uint8_t *>(temp_string), size_of_buffer) == true)
    {
        return_string.assign(temp_string);
        return(true);
    }
    else
    {
        return_string.clear();
        return(false);
    }
}

_Check_return_ bool Win32FoundationClasses::CRegistry::GetStringArrayValue(_In_z_ wchar_t const * name_of_value, _Out_ std::vector<std::wstring>& return_array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    return_array.clear();

    // Thanks go to Chris Hines (ChrisHines@msn.com) for finding
    // a bug here. If you add entries to the key, then the
    // information retrieved via QueryInfo() may be invalid. This
    // will screw you here. So, we must make sure our information
    // is correct before we attempt to *use* the data.

    (void)QueryInfo();

    std::size_t size_of_buffer = m_LongestValueDataLength;

    auto memory_buffer = std::make_unique<uint8_t[]>(size_of_buffer);

    if (memory_buffer.get() == nullptr)
    {
        m_ErrorCode = ::GetLastError();
        //WFCTRACEERROR( m_ErrorCode );
        return(false);
    }

    bool return_value = true;

    auto type = KeyValueTypes::MultipleString; // A double NULL terminated string

    if (QueryValue(name_of_value, type, memory_buffer.get(), size_of_buffer) == true)
    {
        /*
        ** We've got data so give it back to the caller
        */

        auto strings = reinterpret_cast<LPTSTR>(memory_buffer.get());

        return_array.clear();

        while (strings[0] not_eq 0x00)
        {
            (void)return_array.push_back(reinterpret_cast<wchar_t const *>(strings));
            strings += (_tcslen(reinterpret_cast<wchar_t const *>(strings)) + 1); // Paul Ostrowski [postrowski@xantel.com]
        }

        return_value = true;
    }
    else
    {
        return_value = false;
    }

    return(return_value);
}

_Check_return_ bool Win32FoundationClasses::CRegistry::GetValue( __in_z wchar_t const * name_of_value, _Out_ std::vector<uint8_t>& return_array ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( GetBinaryValue( name_of_value, return_array ) );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::GetValue( __in_z wchar_t const * name_of_value, _Out_ DWORD& return_value ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( GetDoubleWordValue( name_of_value, return_value ) );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::GetValue( __in_z wchar_t const * name_of_value, _Out_ std::wstring& return_string ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( GetStringValue( name_of_value, return_string ) );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::GetValue( __in_z wchar_t const * name_of_value, _Out_ std::vector<std::wstring>& return_array ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( GetStringArrayValue( name_of_value, return_array ) );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::Load( __in_z wchar_t const * name_of_subkey, __in_z wchar_t const * name_of_file_containing_information ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( name_of_subkey );
    WFC_VALIDATE_POINTER( name_of_file_containing_information );

    if ( name_of_subkey == nullptr or name_of_file_containing_information == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        return( false );
    }

    // We were passeda pointer, do not trust it

    WFC_TRY
    {
        m_ErrorCode = ::RegLoadKey( m_RegistryHandle, name_of_subkey, name_of_file_containing_information );

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            return( true );
        }
        else
        {
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CRegistry::NotifyChange( _In_ HANDLE const           event_handle,
                                           _In_ NotifyChangeFilter const changes_to_be_reported,
                                           _In_ NotifyChangeFlag const   this_subkey_or_all_subkeys,
                                           _In_ bool const               wait_for_change_or_signal_event ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    m_ErrorCode = ::RegNotifyChangeKeyValue( m_KeyHandle,
        static_cast<BOOL>(this_subkey_or_all_subkeys),
        changes_to_be_reported,
        event_handle,
        wait_for_change_or_signal_event );

    if ( m_ErrorCode == ERROR_SUCCESS )
    {
        return( true );
    }
    else
    {
        //WFCTRACEERROR( m_ErrorCode );
        return( false );
    }
}

_Check_return_ bool Win32FoundationClasses::CRegistry::Open( __in_z wchar_t const * name_of_subkey_to_open, _In_ CreatePermissions const security_access_mask ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    /*
    ** name_of_subkey_to_open can be nullptr
    */

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        if ( m_KeyHandle not_eq static_cast< HKEY >( NULL ) )
        {
            (void) ::RegCloseKey( m_KeyHandle );
            m_KeyHandle = static_cast< HKEY >( NULL );
        }

        m_ErrorCode = ::RegOpenKeyEx( m_RegistryHandle, name_of_subkey_to_open, 0, security_access_mask bitor KEY_WOW64_64KEY, &m_KeyHandle );

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            (void) QueryInfo();
            m_KeyName.assign( name_of_subkey_to_open );

            return( true );
        }
        else
        {
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CRegistry::QueryInfo( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    TCHAR class_name[ 2048 ];

    ::ZeroMemory( class_name, sizeof( class_name ) );

    DWORD size_of_class_name = static_cast<DWORD>(std::size( class_name ) - 1);

    m_ErrorCode = ::RegQueryInfoKeyW( m_KeyHandle,
        class_name,
        &size_of_class_name,
        (LPDWORD) nullptr,
        &m_NumberOfSubkeys,
        &m_LongestSubkeyNameLength,
        &m_LongestClassNameLength,
        &m_NumberOfValues,
        &m_LongestValueNameLength,
        &m_LongestValueDataLength,
        &m_SecurityDescriptorLength,
        &m_LastWriteTime );

    if ( m_ErrorCode == ERROR_SUCCESS )
    {
        m_ClassName.assign( class_name );

        return( true );
    }
    else
    {
        //WFCTRACEERROR( m_ErrorCode );
        return( false );
    }
}

_Check_return_ bool Win32FoundationClasses::CRegistry::QueryValue( __in_z wchar_t const * name_of_value,
                                         __inout KeyValueTypes& value_type,
                                         __out_bcount_part( size_of_buffer, size_of_buffer ) uint8_t * address_of_buffer,
                                         __inout std::size_t& size_of_buffer ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    /*
    ** address_of_buffer and size_of_buffer can be nullptr
    */

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        DWORD temp_data_type         = static_cast<DWORD>(value_type);
        DWORD the_size_of_the_buffer = static_cast<DWORD>(size_of_buffer);

        m_ErrorCode = ::RegQueryValueEx( m_KeyHandle,
            (LPTSTR) name_of_value,
            nullptr,
            &temp_data_type,
            address_of_buffer,
            &the_size_of_the_buffer );

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            value_type = static_cast<KeyValueTypes>(temp_data_type);
            return( true );
        }
        else
        {
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CRegistry::Replace( __in_z wchar_t const * name_of_subkey,
                                      __in_z wchar_t const * name_of_file_with_new_data,
                                      __in_z wchar_t const * name_of_backup_file ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( name_of_subkey );
    WFC_VALIDATE_POINTER( name_of_file_with_new_data );
    WFC_VALIDATE_POINTER( name_of_backup_file );

    if ( name_of_subkey             == nullptr or
        name_of_file_with_new_data == nullptr or
        name_of_backup_file        == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        return( false );
    }

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        m_ErrorCode = ::RegReplaceKey( m_KeyHandle, 
            name_of_subkey,
            name_of_file_with_new_data,
            name_of_backup_file );

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            return( true );
        }
        else
        {
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CRegistry::Restore( __in_z wchar_t const * name_of_file_holding_saved_tree, _In_ DWORD const volatility_flags ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( name_of_file_holding_saved_tree );

    if ( name_of_file_holding_saved_tree == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        return( false );
    }

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        m_ErrorCode = ::RegRestoreKey( m_KeyHandle,
            name_of_file_holding_saved_tree,
            volatility_flags );

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            return( true );
        }
        else
        {
            //WFCTRACEERROR( m_ErrorCode );

            // Maybe we need to get permission...
            HANDLE token_handle = static_cast< HANDLE >( NULL );

            if ( ::OpenProcessToken( ::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES bitor TOKEN_QUERY, &token_handle ) == FALSE )
            {
                m_ErrorCode = ::GetLastError();
                //WFCTRACEERROR( m_ErrorCode );
                return( false );
            }

            TOKEN_PRIVILEGES token_privileges;

            ::ZeroMemory( &token_privileges, sizeof( token_privileges ) );

            if ( ::LookupPrivilegeValue( nullptr, SE_RESTORE_NAME, &token_privileges.Privileges[ 0 ].Luid ) == FALSE )
            {
                m_ErrorCode = ::GetLastError();
                //WFCTRACEERROR( m_ErrorCode );
                (void) Win32FoundationClasses::wfc_close_handle( token_handle );
                token_handle = static_cast< HANDLE >( NULL );
                return( false );
            }

            token_privileges.PrivilegeCount             = 1;
            token_privileges.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED;

            if ( ::AdjustTokenPrivileges( token_handle, FALSE, &token_privileges, 0, nullptr, 0 ) == FALSE )
            {
                m_ErrorCode = ::GetLastError();
                //WFCTRACEERROR( m_ErrorCode );
                (void) Win32FoundationClasses::wfc_close_handle( token_handle );
                token_handle = static_cast< HANDLE >( NULL );
                return( false );
            }

            // Try the restore again

            m_ErrorCode = ::RegRestoreKey( m_KeyHandle,
                name_of_file_holding_saved_tree,
                volatility_flags );

            // Regardless of the turnout, we're through with the token handle

            (void) Win32FoundationClasses::wfc_close_handle( token_handle );
            token_handle = static_cast< HANDLE >( NULL );

            if ( m_ErrorCode not_eq ERROR_SUCCESS )
            {
                m_ErrorCode = ::GetLastError();

                //WFCTRACEERROR( m_ErrorCode );
                return( false );
            }

            return( true );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CRegistry::Save( __in_z wchar_t const * name_of_file_to_hold_tree, __in_opt LPSECURITY_ATTRIBUTES security_attributes_p ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( name_of_file_to_hold_tree );

    if ( name_of_file_to_hold_tree == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        return( false );
    }

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        m_ErrorCode = ::RegSaveKey( m_KeyHandle, name_of_file_to_hold_tree, security_attributes_p );

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            return( true );
        }
        else
        {
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CRegistry::SetBinaryValue( __in_z wchar_t const * name_of_value, _In_ std::vector<uint8_t> const& bytes_to_write ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    DWORD size_of_buffer = (DWORD) bytes_to_write.size();

    auto memory_buffer = std::make_unique<uint8_t[]>(size_of_buffer);

    if ( memory_buffer.get() == nullptr )
    {
        m_ErrorCode = ::GetLastError();
        //WFCTRACEERROR( m_ErrorCode );
        return( false );
    }

    for ( auto const index : Range(size_of_buffer) )
    {
        memory_buffer[ index ] = bytes_to_write.at( index );
    }

    bool const return_value = SetValue( name_of_value, KeyValueTypes::Binary, memory_buffer.get(), size_of_buffer );

    return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::SetDoubleWordValue( __in_z wchar_t const * name_of_value, _In_ DWORD value_to_write ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    return( SetValue( name_of_value, KeyValueTypes::DoubleWord, reinterpret_cast<PBYTE const>(&value_to_write), sizeof( DWORD ) ) );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::SetSecurity( _In_ SECURITY_INFORMATION const& security_information, _In_ PSECURITY_DESCRIPTOR const security_descriptor_p ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( security_descriptor_p );

    if ( security_descriptor_p == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        return( false );
    }

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        m_ErrorCode = ::RegSetKeySecurity( m_KeyHandle, security_information, security_descriptor_p );

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            return( true );
        }
        else
        {
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CRegistry::SetStringValue( _In_z_ wchar_t const * name_of_value, _In_ std::wstring const& string_value ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    // Thanks to Ravi Kiran (kravi@blr.sni.de) for finding a bug here in the UNICODE build

    return( SetValue( name_of_value, KeyValueTypes::String, reinterpret_cast<uint8_t const *>(string_value.c_str()), (DWORD) ( ( string_value.length() + 1 ) * sizeof( wchar_t ) ) ) );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::SetStringArrayValue(_In_z_ wchar_t const * name_of_value, _In_ std::vector<std::wstring> const& string_array ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    std::size_t size_of_buffer = 0;

    /*
    ** Find out how big our buffer needs to be...
    */

    std::size_t number_of_strings = string_array.size();

    std::wstring temp_string;

    for ( auto const index : Range(number_of_strings) )
    {
        temp_string.assign( string_array.at( index ) );

        // Thanks go to George Koukoulommatis (gemasoft@hol.gr)
        // for finding another UNICODE bug here...

        size_of_buffer += ( ( temp_string.length() + 1 ) * sizeof( wchar_t ) );
    }

    /*
    ** Don't forget the second zero needed for double null terminated strings...
    */

    // Thanks go to George Koukoulommatis (gemasoft@hol.gr)
    // for finding another UNICODE bug here...

    size_of_buffer += sizeof( TCHAR );

    auto memory_buffer = std::make_unique<uint8_t[]>(size_of_buffer);

    if ( memory_buffer.get() == nullptr )
    {
        m_ErrorCode = ::GetLastError();
        //WFCTRACEERROR( m_ErrorCode );
        return( false );
    }

    ::ZeroMemory( memory_buffer.get(), size_of_buffer );

    /*
    ** OK, now add the strings to the memory buffer
    */

    LPTSTR string = (LPTSTR) memory_buffer.get();

    SSIZE_T string_length = 0;

    for ( auto const index : Range(number_of_strings) )
    {
        temp_string.assign( string_array.at( index ) );
        wcscpy( &string[ string_length ], temp_string.c_str() );

        // Thanks go to George Koukoulommatis (gemasoft@hol.gr)
        // for finding another UNICODE bug here...

        string_length += ( ( temp_string.length() + 1 ) * sizeof( TCHAR ) );
    }

    // Thanks go to George Koukoulommatis (gemasoft@hol.gr)
    // for finding another UNICODE bug here...

    string_length += sizeof( wchar_t );

    bool return_value = true;

    auto type = KeyValueTypes::MultipleString; // A double NULL terminated string

    if ( SetValue( name_of_value, type, memory_buffer.get(), size_of_buffer ) == false )
    {
        return_value = false;
    }

    return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::SetValue( __in_z wchar_t const * name_of_value, _In_ std::vector<uint8_t> const& bytes_to_write ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    return( SetBinaryValue( name_of_value, bytes_to_write ) );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::SetValue( __in_z wchar_t const * name_of_value, _In_ DWORD value ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    return( SetDoubleWordValue( name_of_value, value ) );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::SetValue( __in_z wchar_t const * name_of_value, _In_ std::vector<std::wstring> const& strings_to_write ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    return( SetStringArrayValue( name_of_value, strings_to_write ) );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::SetValue( __in_z wchar_t const * name_of_value, _In_ std::wstring const& string_to_write ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    return( SetStringValue( name_of_value, string_to_write ) );
}

_Check_return_ bool Win32FoundationClasses::CRegistry::SetValue( __in_z wchar_t const * name_of_value,
                                       _In_ KeyValueTypes const type_of_value_to_set, 
                                       __in_bcount_opt( size_of_data ) uint8_t const* address_of_value_data,
                                       _In_ std::size_t const        size_of_data ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( name_of_value );
    WFC_VALIDATE_POINTER( address_of_value_data );

    if ( name_of_value == nullptr or address_of_value_data == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        return( false );
    }

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        _ASSERTE( size_of_data <= 0xFFFFFFFF ); // To detect 32-bit overruns...

        m_ErrorCode = ::RegSetValueEx( m_KeyHandle,
            name_of_value,
            0,
            static_cast<DWORD>(type_of_value_to_set),
            address_of_value_data,
            static_cast<DWORD>(size_of_data) );

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            return( true );
        }
        else
        {
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CRegistry::UnLoad( __in_z wchar_t const * name_of_subkey_to_unload ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( name_of_subkey_to_unload );

    if ( name_of_subkey_to_unload == nullptr )
    {
        m_ErrorCode = ERROR_INVALID_PARAMETER;
        return( false );
    }

    // We were passed a pointer, do not trust it

    WFC_TRY
    {
        // 2003-04-23, Thanks go to Anne Jobmann for finding that I was using the wrong handle...

        m_ErrorCode = ::RegUnLoadKey( m_RegistryHandle, name_of_subkey_to_unload );

        if ( m_ErrorCode == ERROR_SUCCESS )
        {
            return( true );
        }
        else
        {
            //WFCTRACEERROR( m_ErrorCode );
            return( false );
        }
    }
    WFC_CATCH_ALL
    {
        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        return( false );
    }
    WFC_END_CATCH_ALL
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CRegistry</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles the Win32 registry.">
</HEAD>

<BODY>

<H1>CRegistry</H1>
$Revision: 45 $
<HR>

<H2>Description</H2>

This class handles the registry API. The registry is a database
that allows applications to store stuff. Microsoft has weird terms
for dealing with the registry:

<DL COMPACT>

<DT>Hive<DD>Kinda like a virtual disk drive
<DT>Key<DD>Kinda like a directory
<DT>Value<DD>Kinda like a file 

</DL>

<H2>Data Members</H2>

None

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the connection to the registry.

<DT><PRE>BOOL <B><A NAME="Connect">Connect</A></B>( HKEY key_to_open = HKEY_CURRENT_USER, const wchar_t * computer_name = nullptr )</PRE><DD>
Connects to a registry on a computer. The default is to connect to
the local computer. If <CODE>computer_name</CODE> is not NULL,
it will connect to that machine on the network.

<DT><PRE>BOOL <B><A NAME="Create">Create</A></B>( wchar_t const *               name_of_subkey,
wchar_t const *               name_of_class         = nullptr,
CreateOptions         options               = optionsNonVolatile,
CreatePermissions     permissions           = permissionAllAccess,
LPSECURITY_ATTRIBUTES security_attributes_p = nullptr,
CreationDisposition * disposition_p         = nullptr )</PRE><DD>
Creates a subkey in a registry hive. <CODE>options</CODE> can
be one of:
<UL>
<LI>optionsNonVolatile
<LI>optionsVolatile
</UL>
<CODE>permissions</CODE> can be a combination of:
<UL>
<LI>permissionAllAccess
<LI>permissionCreateLink
<LI>permissionCreateSubKey
<LI>permissionEnumerateSubKeys
<LI>permissionExecute
<LI>permissionNotify
<LI>permissionQueryValue
<LI>permissionRead
<LI>permissionSetValue
<LI>permissionWrite
</UL>
<CODE>disposition_p</CODE> if not NULL, will be filled with either:
<UL>
<LI>dispositionCreatedNewKey
<LI>dispositionOpenedExistingKey
</UL>
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="DeleteKey">DeleteKey</A></B>( wchar_t const * name_of_subkey_to_delete )</PRE><DD>
Deletes a key and all subkeys (even in NT) of that key.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="DeleteValue">DeleteValue</A></B>( wchar_t const * name_of_value_to_delete )</PRE><DD>
Deletes a specific value.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="EnumerateKeys">EnumerateKeys</A></B>( DWORD const subkey_index,
std::wstring&amp;    subkey_name,
std::wstring&amp;    class_name )</PRE><DD>
Let's you list the names of the keys.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="EnumerateValues">EnumerateValues</A></B>( DWORD const value_index,
std::wstring&amp;       name_of_value,
KeyValueTypes&amp; type_code,
LPBYTE         data_buffer,
DWORD&amp;         size_of_data_buffer )</PRE><DD>
Let's you list the names of the values within a key. If you do not wish
to retrieve the data, set <CODE>data_buffer</CODE> to NULL.
<CODE>size_of_data_buffer</CODE> will be filled with the number of
bytes put into <CODE>data_buffer</CODE>.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().
<CODE>type_code</CODE> will be one of:
<UL>
<LI>typeBinary
<LI>typeDoubleWord
<LI>typeDoubleWordLittleEndian
<LI>typeDoubleWordBigEndian
<LI>typeUnexpandedString
<LI>typeSymbolicLink
<LI>typeMultipleString
<LI>typeNone
<LI>typeResourceList
<LI>typeString
</UL>

<B>NOTE:</B> Remember that <CODE>size_of_data_buffer</CODE> must
be reset within your enumerating loop because it is set by the
function call. If you do not, you will probably get an error code
of 234 (<CODE>ERROR_MORE_DATA</CODE>). Here's what a proper loop
should look like:

<PRE><CODE>BYTE buffer[ 4096 ];

DWORD size_of_buffer = 4096;

while( registry.EnumerateValues( enumerator, name, type, buffer, size_of_buffer ) == true )
{
_tprintf( TEXT( &quot;Found %s, length is %lu\n&quot; ), (wchar_t const *) name, size_of_buffer );
size_of_buffer = 4096;
enumerator++;
}</CODE></PRE>

<DT><PRE>BOOL <B><A NAME="Flush">Flush</A></B>( void )</PRE><DD>
Writes all cached changes to disk.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="GetBinaryValue">GetBinaryValue</A></B>( const wchar_t * name_of_value, std::vector&lt;uint8_t&gt;&amp; bytes )</PRE><DD>
Retrieves the binary data and puts it into <CODE>bytes</CODE>.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>void <B><A NAME="GetTheClassName">GetTheClassName</A></B>( std::wstring&amp; class_name ) const</PRE><DD>
Retrieves the name of the class of key you have opened.

<DT><PRE>void <B><A NAME="GetComputerName">GetComputerName</A></B>( std::wstring&amp; computer_name ) const</PRE><DD>
Retrieves the name of the computer where the hive is located.

<DT><PRE>BOOL <B><A NAME="GetDoubleWordValue">GetDoubleWordValue</A></B>( const wchar_t * name_of_value, DWORD&amp; value )</PRE><DD>
Retrieves the DWORD data value.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>DWORD <B><A NAME="GetErrorCode">GetErrorCode</A></B>( void ) const</PRE><DD>
Retrieves the error code. Call this function
if any other class method returns FALSE.

<DT><PRE>void <B><A NAME="GetKeyName">GetKeyName</A></B>( std::wstring&amp; key_name ) const</PRE><DD>
Retrieves the name of the current key.

<DT><PRE>DWORD <B><A NAME="GetNumberOfSubKeys">GetNumberOfSubKeys</A></B>( void ) const</PRE><DD>
Retrieves the number of keys contained within the current key.

<DT><PRE>DWORD <B><A NAME="GetNumberOfValues">GetNumberOfValues</A></B>( void ) const</PRE><DD>
Retrieves the number of values contained within the current key.

<DT><PRE>void <B><A NAME="GetRegistryName">GetRegistryName</A></B>( std::wstring&amp; registry_name ) const</PRE><DD>
Retrieves the name of the registry you are playing with.

<DT><PRE>BOOL <B><A NAME="GetSecurity">GetSecurity</A></B>( const SECURITY_INFORMATION what_you_want_to_know,
PSECURITY_DESCRIPTOR       data_buffer,
DWORD&amp;                     size_of_data_buffer )</PRE><DD>
Retrieves security information.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="GetStringValue">GetStringValue</A></B>( const wchar_t * name_of_value, std::wstring&amp; return_string )</PRE><DD>
Retrieves a string value (<CODE><B>REG_SZ</B></CODE>).
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="GetStringArrayValue">GetStringArrayValue</A></B>( const wchar_t * name_of_value, std::vector&lt;std::wstring&gt;&amp; return_array )</PRE><DD>
Retrieves a value containing multiple strings (REG_MULTI_SZ) and puts it into a std::vector&lt;std::wstring&gt;.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="GetValue">GetValue</A></B>( const wchar_t * name_of_value, std::vector&lt;uint8_t&gt;&amp; return_array )
BOOL <B>GetValue</B>( const wchar_t * name_of_value, DWORD&amp; return_value )
BOOL <B>GetValue</B>( const wchar_t * name_of_value, std::wstring&amp; return_string )
BOOL <B>GetValue</B>( const wchar_t * name_of_value, std::vector&lt;std::wstring&gt;&amp; return_array )</PRE><DD>
Overloaded function that calls the proper GetXValue function.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="Load">Load</A></B>( const wchar_t * name_of_subkey, const wchar_t * name_of_file_containg_information )</PRE><DD>
Loads a subkey from a file.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="NotifyChange">NotifyChange</A></B>( HANDLE const event_handle                       = nullptr,
const NotifyChangeFilter changes_to_be_reported = notifyLastSet,
const BOOL this_subkey_or_all_subkeys           = changeSpecifiedKeyOnly,
const BOOL wait_for_change_or_signal_event      = WAIT_FOR_CHANGE )</PRE><DD>
Lets you watch a key (with or without subkeys) to see if anything changes.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().
<CODE>changes_to_be_reported</CODE> can be one of:
<UL>
<LI>notifyName
<LI>notifyAttributes
<LI>notifyLastSet
<LI>notifySecurity
</UL>

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( const wchar_t * name_of_subkey_to_open,
CreatePermissions const security_access_mask = permissionAllAccess )</PRE><DD>
Opens a subkey.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().
<CODE>security_access_mask</CODE> can be a combination of:
<UL>
<LI>permissionAllAccess
<LI>permissionCreateLink
<LI>permissionCreateSubKey
<LI>permissionEnumerateSubKeys
<LI>permissionExecute
<LI>permissionNotify
<LI>permissionQueryValue
<LI>permissionRead
<LI>permissionSetValue
<LI>permissionWrite
</UL>

<DT><PRE>BOOL <B><A NAME="QueryInfo">QueryInfo</A></B>( void )</PRE><DD>
Tells <B>CRegistry</B> to load information because you're about to call <B>QueryValue</B>().
This loads information that can be retrieved via <B>GetNumberOfSubkeys</B>()
or <B>GetNumberOfValues</B>().
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="QueryValue">QueryValue</A></B>( const wchar_t *        name_of_value,
KeyValueTypes&amp; value_type,
LPBYTE         address_of_buffer,
DWORD&amp;         size_of_buffer )</PRE><DD>
Allows you to retrieve a value without knowing it's type.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().
<CODE>value_type</CODE> will be filled with one of the following values:
<UL>
<LI>typeBinary
<LI>typeDoubleWord
<LI>typeDoubleWordLittleEndian
<LI>typeDoubleWordBigEndian
<LI>typeUnexpandedString
<LI>typeSymbolicLink
<LI>typeMultipleString
<LI>typeNone
<LI>typeResourceList
<LI>typeString
</UL>

<DT><PRE>BOOL <B><A NAME="Replace">Replace</A></B>( wchar_t const * name_of_subkey,
wchar_t const * name_of_file_with_new_data,
wchar_t const * name_of_backup_file )</PRE><DD>
Allows you to replace a subkey with data from a file.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="Restore">Restore</A></B>( wchar_t const * name_of_file_holding_saved_tree, DWORD const volitility_flags = nullptr )</PRE><DD>
Restores a subkey that was backed up to a file.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="Save">Save</A></B>( const wchar_t * name_of_file_to_hold_tree, LPSECURITY_ATTRIBUTES security_attributes_p = nullptr )</PRE><DD>
Allows you to save a subtree to a file (so you can call <B>Restore</B>()).
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="SetBinaryValue">SetBinaryValue</A></B>( const wchar_t * name_of_value, const std::vector&lt;uint8_t&gt;&amp; bytes_to_write )</PRE><DD>
Allows you to write a binary (raw bytes) to a value in a key.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="SetDoubleWordValue">SetDoubleWordValue</A></B>( const wchar_t * name_of_value, DWORD value_to_write )</PRE><DD>
Allows you to write a DWORD value to a key.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="SetSecurity">SetSecurity</A></B>( const SECURITY_INFORMATION&amp; security_information,
const PSECURITY_DESCRIPTOR security_descriptor_p )</PRE><DD>
Allows you to put security onto a subkey.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="SetStringValue">SetStringValue</A></B>( const wchar_t * name_of_value, const std::wstring&amp; string_value )</PRE><DD>
Allows you to write a string value to a key.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="SetStringArrayValue">SetStringArrayValue</A></B>( const wchar_t * name_of_value, const std::vector&lt;std::wstring&gt;&amp; string_array )</PRE><DD>
Allows you to write a bunch of strings to a value in a key.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="SetValue">SetValue</A></B>( const wchar_t * name_of_value, const std::vector&lt;uint8_t&gt;&amp; bytes_to_write );
BOOL <B>SetValue</B>( wchar_t const * name_of_value, DWORD value );
BOOL <B>SetValue</B>( wchar_t const * name_of_value, const std::vector&lt;std::wstring&gt;&amp; strings_to_write );
BOOL <B>SetValue</B>( wchar_t const * name_of_value, const std::wstring&amp; string_to_write );
BOOL <B>SetValue</B>( wchar_t const *             name_of_subkey,
KeyValueTypes const type_of_value_to_set,
PBYTE         const address_of_value_data,
DWORD         const size_of_data )</PRE><DD>
Overloaded method that figures out what type of value you want to set.
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

<DT><PRE>BOOL <B><A NAME="UnLoad">UnLoad</A></B>( const wchar_t * name_of_subkey_to_unload )</PRE><DD>
Unloads a previously <B>Load</B>()'ed key. 
It returns TRUE on success or FALSE on failure.
The reason for the failure can be retrieved via <B>GetErrorCode</B>().

</DL>

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

void test_CRegistry( void )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;test_CRegistry()&quot; ) );

<B>CRegistry</B> registry;

if ( registry.Connect( CRegistry::keyLocalMachine ) == FALSE )
{
TRACE( TEXT( &quot;Can't connect to registry\n&quot; ) );
return;
}

if ( registry.Open( TEXT( &quot;SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application&quot; ),
(CRegistry::CreatePermissions)(CRegistry::permissionRead bitor CRegistry::permissionSetValue)
) == FALSE )
{
TRACE( TEXT( &quot;Can't open key\n&quot; ) );

ReportError( registry.GetErrorCode() );
return;
}

std::vector&lt;std::wstring&gt; string_array;

string_array.RemoveAll();

if ( registry.GetStringArrayValue( TEXT( &quot;Sources&quot; ), string_array ) not_eq FALSE )
{
int index = 0;
int number_of_strings = string_array.GetSize();

TRACE1( TEXT( &quot;There are %d strings\n&quot; ), number_of_strings );

std::wstring temp_string;

while( index &lt; number_of_strings )
{
temp_string = string_array[ index ];

TRACE2( TEXT( &quot;%d - \&quot;%s\&quot;\n&quot; ), index + 1, (wchar_t const *) temp_string );
index++;
}

if ( registry.SetStringArrayValue( TEXT( &quot;TempSources&quot; ), string_array ) == FALSE )
{
ReportError( registry.GetErrorCode() );
}
}

if ( registry.SetValue( TEXT( &quot;QFile&quot; ), &quot;Sammy&quot; ) == FALSE )
{
TRACE( TEXT( &quot;registry.SetValue failed\n&quot; ) );
ReportError( registry.GetErrorCode() );
}
else
{
TRACE( TEXT( &quot;Set QFile to Sammy\n&quot; ) );
}

std::wstring temp_sammy( TEXT( &quot;&quot; ) );

if ( registry.GetValue( TEXT( &quot;QFile&quot; ), temp_sammy ) == FALSE )
{
TRACE( TEXT( &quot;registry.GetValue failed\n&quot; ) );
ReportError( registry.GetErrorCode() );
}
else
{
if ( temp_sammy.Compare( TEXT( &quot;Sammy&quot; ) ) == 0 )
{
TRACE( TEXT( &quot;GetValue OK\n&quot; ) );
}
else
{
TRACE1( TEXT( &quot;FAIL! QFile is \&quot;%s\&quot;\n&quot; ), (wchar_t const *) temp_sammy );
}
}
}</CODE></PRE>

<H2>API's Used</H2>

<B>CRegistry</B> encapsulates the following API's:
<UL>
<LI>RegCloseKey
<LI>RegConnectRegistry
<LI>RegCreateKeyEx
<LI>RegDeleteValue
<LI>RegEnumKeyEx
<LI>RegEnumValue
<LI>RegFlushKey
<LI>RegGetKeySecurity
<LI>RegLoadKey
<LI>RegNotifyChangeKeyValue
<LI>RegOpenKeyEx
<LI>RegQueryInfoKey
<LI>RegQueryValueEx
<LI>RegReplaceKey
<LI>RegRestoreKey
<LI>RegSaveKey
<LI>RegSetKeySecurity
<LI>RegSetValueEx
<LI>RegUnLoadKey
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: registry.cpp $<BR>
$Modtime: 6/26/01 10:55a $
</BODY>

</HTML>
*/
