/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2022, Samuel R. Blackburn
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

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#define new DEBUG_NEW
#endif // _DEBUG

static void _to_hex( __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes, __out_ecount_z( number_of_characters ) wchar_t * destination_string, _In_ std::size_t const number_of_characters ) noexcept
{
    static constexpr wchar_t const* static_hex_digits{ L"0123456789abcdef" };

    destination_string[ 0 ] = 0x00;

    std::size_t string_index{ 0 };

    uint8_t value{ 0 };

    for ( auto const buffer_index : Range(number_of_bytes) )
    {
        if ( string_index + 2 < ( number_of_characters - 1 ) )
        {
            value = buffer[ buffer_index ];
            destination_string[ string_index     ] = static_hex_digits[ value >> 4 ];
            destination_string[ string_index + 1 ] = static_hex_digits[ value bitand 0x0F ];
            destination_string[ string_index + 2 ] = 0x00;

            string_index += 2;
        }
        else
        {
            break;
        }
    }
}

// http://forum.sysinternals.com/howto-verify-the-digital-signature-of-a-file_topic19247.html

_Check_return_ bool Win32FoundationClasses::wfc_check_digital_signature( __in_z LPCTSTR filename ) noexcept
{
    WFC_VALIDATE_POINTER(filename);

    if ( filename == nullptr or filename[ 0 ] == 0x0 )
    {
       return( false );
    }

    auto context{ static_cast<HCATADMIN>(NULL) };

    if ( ::CryptCATAdminAcquireContext( &context, nullptr, 0 ) == FALSE )
    {
        //WFCTRACE( TEXT( "Can't CryptCATAdminAcquireContext()" ) );
        return( false );
    }

    Win32FoundationClasses::CFile64 file_to_check;

    if ( file_to_check.Open( filename, (UINT)( (UINT)Win32FoundationClasses::CFile64::OpenFlags::modeRead bitor (UINT)Win32FoundationClasses::CFile64::OpenFlags::shareDenyNone )) == false )
    {
        //WFCTRACEERROR( ::GetLastError() );
        //WFCTRACEVAL( TEXT( "Can't open " ), filename );
        ::CryptCATAdminReleaseContext( context, 0 );
        return( false );
    }

    DWORD number_of_bytes_in_hash{ 0 };

    if ( ::CryptCATAdminCalcHashFromFileHandle( file_to_check.GetHandle(), &number_of_bytes_in_hash, nullptr, 0 ) == FALSE )
    {
        //WFCTRACEERROR( ::GetLastError() );
        //WFCTRACE( TEXT( "Can't CryptCATAdminCalcHashFromFileHandle()" ) );
        ::CryptCATAdminReleaseContext( context, 0 );
        return( false );
    }

    if ( number_of_bytes_in_hash == 0 )
    {
        //WFCTRACE( TEXT( "number of hash bytes is zero" ) );
        ::CryptCATAdminReleaseContext( context, 0 );
        return( false );
    }

    auto hash_buffer{ static_cast<uint8_t*>(_alloca(number_of_bytes_in_hash)) };

    ZeroMemory( hash_buffer, number_of_bytes_in_hash );

    if ( ::CryptCATAdminCalcHashFromFileHandle( file_to_check.GetHandle(), &number_of_bytes_in_hash, hash_buffer, 0 ) == FALSE )
    {
        //WFCTRACEERROR( ::GetLastError() );
        //WFCTRACE( TEXT( "Can't CryptCATAdminCalcHashFromFileHandle()" ) );

        ::CryptCATAdminReleaseContext( context, 0 );
        return( false );
    }

    auto hash_string{ static_cast<wchar_t*>(_alloca(((number_of_bytes_in_hash + 1) * 2) * 2)) };

    _to_hex( hash_buffer, number_of_bytes_in_hash, hash_string, number_of_bytes_in_hash * 2 );

    CATALOG_INFO catalog_information;

    ZeroMemory( &catalog_information, sizeof( catalog_information ) );
    catalog_information.cbStruct = sizeof( catalog_information );

    //Get catalog for our context.
    auto catalog_context{ ::CryptCATAdminEnumCatalogFromHash(context, hash_buffer, number_of_bytes_in_hash, 0, nullptr) };

    if ( catalog_context not_eq static_cast< HCATINFO >( NULL ) )
    {
        //If we couldn't get information
        if ( ::CryptCATCatalogInfoFromContext( catalog_context, &catalog_information, 0 ) not_eq FALSE )
        {
            //Release the context and set the context to null so it gets picked up below.
            ::CryptCATAdminReleaseCatalogContext( context, catalog_context, 0 );
            catalog_context = static_cast< HCATINFO >( NULL );
        }
    }
    else
    {
        //WFCTRACEERROR( ::GetLastError() );
        //WFCTRACE( TEXT( "Can't CryptCATAdminEnumCatalogFromHash()" ) );
    }

    WINTRUST_FILE_INFO    file_information;
    WINTRUST_DATA         trust_data;
    WINTRUST_CATALOG_INFO wintrust_catalog_information;

    ZeroMemory( &file_information, sizeof( file_information ) );
    ZeroMemory( &trust_data, sizeof( trust_data ) );
    ZeroMemory( &wintrust_catalog_information, sizeof( wintrust_catalog_information ) );

    if ( catalog_context == static_cast< HCATINFO >( NULL ) )
    {
        file_information.cbStruct       = sizeof( WINTRUST_FILE_INFO );
        file_information.pcwszFilePath  = filename;
        file_information.hFile          = static_cast< HANDLE >( NULL );
        file_information.pgKnownSubject = nullptr;

        trust_data.cbStruct            = sizeof(WINTRUST_DATA);
        trust_data.dwUnionChoice       = WTD_CHOICE_FILE;
        trust_data.pFile               = &file_information;
        trust_data.dwUIChoice          = WTD_UI_NONE;
        trust_data.fdwRevocationChecks = WTD_REVOKE_NONE;
        trust_data.dwStateAction       = WTD_STATEACTION_IGNORE;
        trust_data.dwProvFlags         = WTD_SAFER_FLAG bitor WTD_CACHE_ONLY_URL_RETRIEVAL;
        trust_data.hWVTStateData       = static_cast< HANDLE >( NULL );
        trust_data.pwszURLReference    = nullptr;
    }
    else
    {
        //If we get here, we have catalog info!  Verify it.
        trust_data.cbStruct            = sizeof( WINTRUST_DATA );
        trust_data.pPolicyCallbackData = 0;
        trust_data.pSIPClientData      = 0;
        trust_data.dwUIChoice          = WTD_UI_NONE;
        trust_data.fdwRevocationChecks = WTD_REVOKE_NONE;
        trust_data.dwUnionChoice       = WTD_CHOICE_CATALOG;
        trust_data.pCatalog            = &wintrust_catalog_information;
        trust_data.dwStateAction       = WTD_STATEACTION_VERIFY;
        trust_data.hWVTStateData       = static_cast< HANDLE >( NULL );
        trust_data.pwszURLReference    = nullptr;
        trust_data.dwProvFlags         = WTD_SAFER_FLAG bitor WTD_CACHE_ONLY_URL_RETRIEVAL;
        trust_data.dwUIContext         = WTD_UICONTEXT_EXECUTE;

        //Fill in catalog info structure.
        wintrust_catalog_information.cbStruct             = sizeof( WINTRUST_CATALOG_INFO );
        wintrust_catalog_information.dwCatalogVersion     = 0;
        wintrust_catalog_information.pcwszCatalogFilePath = catalog_information.wszCatalogFile;
        wintrust_catalog_information.pcwszMemberTag       = hash_string;
        wintrust_catalog_information.pcwszMemberFilePath  = filename;
        wintrust_catalog_information.hMemberFile          = static_cast< HANDLE >( NULL );
        wintrust_catalog_information.pbCalculatedFileHash = hash_buffer;
        wintrust_catalog_information.cbCalculatedFileHash = number_of_bytes_in_hash;
    }

    GUID action_id = WINTRUST_ACTION_GENERIC_VERIFY_V2;

    auto return_value{ ::WinVerifyTrust(0, &action_id, &trust_data) };

    if ( return_value == TRUST_E_NOSIGNATURE )
    {
        //WFCTRACE( TEXT( "No signature in file" ) );
    }

    BOOLEAN return_flag{ SUCCEEDED(return_value) };

    if ( catalog_context not_eq static_cast< HCATINFO >( NULL ) )
    {
        ::CryptCATAdminReleaseCatalogContext( context, catalog_context, 0 );
        catalog_context = static_cast< HCATINFO >( NULL );
    }

    trust_data.dwStateAction = WTD_STATEACTION_CLOSE;
    ::WinVerifyTrust( 0, &action_id, &trust_data );

    ::CryptCATAdminReleaseContext( context, 0 );

    return( return_flag == FALSE ? false : true );
}

void Win32FoundationClasses::wfc_get_version(_In_ std::filesystem::path const& file_name, _Out_ uint16_t& major, _Out_ uint16_t& minor, _Out_ uint16_t& build, _Out_ uint16_t& revision ) noexcept
{
   major = 0;
   minor = 0;
   build = 0;
   revision = 0;

   DWORD handle{ 0 };

   uint32_t const number_of_bytes_to_allocate{ GetFileVersionInfoSizeW(file_name.native().c_str(), &handle) };

   if ( number_of_bytes_to_allocate > 0 )
   {
       auto byte_buffer{ std::make_unique<uint8_t[]>(number_of_bytes_to_allocate) };

      if ( byte_buffer.get() not_eq nullptr )
      {
         uint8_t * pointer{ nullptr };

         UINT number_of_bytes_at_pointer{ 0 };

         if ( ::GetFileVersionInfoW( file_name.native().c_str(), 0, number_of_bytes_to_allocate, byte_buffer.get() ) not_eq 0 )
         {
            if ( VerQueryValueW( byte_buffer.get(), L"\\", reinterpret_cast<LPVOID *>(&pointer), &number_of_bytes_at_pointer ) not_eq 0 )
            {
               auto version_information{ reinterpret_cast<VS_FIXEDFILEINFO*>(pointer) };

               if ( version_information->dwSignature == VS_FFI_SIGNATURE )
               {
                  major = HIWORD(version_information->dwFileVersionMS);
                  minor = LOWORD(version_information->dwFileVersionMS);
                  build = HIWORD(version_information->dwFileVersionLS);
                  revision = LOWORD(version_information->dwFileVersionLS);
               }
            }
         }
      }
   }
}

void Win32FoundationClasses::wfc_get_my_version( _Out_ uint16_t& major, _Out_ uint16_t& minor, _Out_ uint16_t& build, _Out_ uint16_t& revision ) noexcept
{
   major = 0;
   minor = 0;
   build = 0;
   revision = 0;

   wchar_t path[ 4096 ];

   ZeroMemory( path, sizeof( path ) );

   if ( ::GetModuleFileNameW( static_cast< HMODULE >( NULL ), path, static_cast<DWORD>(std::size( path )) ) not_eq 0 )
   {
      wfc_get_version( path, major, minor, build, revision );
   }
}

void Win32FoundationClasses::wfc_get_my_version(_Inout_ std::wstring& version) noexcept
{
    uint16_t major{ 0 };
    uint16_t minor{ 0 };
    uint16_t build{ 0 };
    uint16_t revision{ 0 };

    wfc_get_my_version(major, minor, build, revision);

    format(version, L"%" PRIu16 ".%" PRIu16 ".%" PRIu16 ".%" PRIu16, major, minor, build, revision);
}

_Check_return_ uint64_t Win32FoundationClasses::wfc_get_my_packed_version( void ) noexcept
{
   uint16_t major{ 0 };
   uint16_t minor{ 0 };
   uint16_t patch{ 0 };
   uint16_t build{ 0 };

   Win32FoundationClasses::wfc_get_my_version( major, minor, patch, build );

   ULARGE_INTEGER ft;

   ft.HighPart = (DWORD) ( major << 16 ) + (DWORD) minor;
   ft.LowPart = (DWORD) ( patch << 16 ) + (DWORD) build;

   return( ft.QuadPart );
}
