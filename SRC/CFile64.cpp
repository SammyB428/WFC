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
**
** $Workfile: CFile64.cpp $
** $Revision: 17 $
** $Modtime: 6/26/01 10:46a $
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

#pragma warning( disable : 4302 )

// Helper functions

static inline constexpr [[nodiscard]] _Check_return_ bool __is_directory_separation_character( _In_ wchar_t const character_to_test ) noexcept
{
    return( character_to_test == '\\' or character_to_test == '/' );
}

static inline constexpr [[nodiscard]] _Check_return_ bool __is_wide_directory_separation_character( _In_ wchar_t const character_to_test ) noexcept
{
    return( character_to_test == L'\\' or character_to_test == L'/' );
}

static inline _Check_return_ UINT __GetFileName( _In_z_ LPCTSTR path_name_parameter, __out_ecount_opt( maximum_number_of_characters ) LPTSTR title_parameter, _In_ UINT const maximum_number_of_characters ) noexcept
{
    WFC_VALIDATE_POINTER( path_name_parameter );
    WFC_VALIDATE_POINTER_NULL_OK( title_parameter );

    // always capture the complete file name including extension (if present)
    auto temporary_writable_pointer{ const_cast<LPTSTR>(path_name_parameter) };

    LPCTSTR temporary_pointer{ nullptr };

    for ( temporary_pointer = path_name_parameter; *temporary_pointer not_eq '\0'; temporary_pointer = _tcsinc( temporary_pointer ) )
    {
        // remember last directory/drive separator
        if ( *temporary_pointer == '\\' or *temporary_pointer == '/' or *temporary_pointer == ':')
        {
            temporary_writable_pointer = (LPTSTR) _tcsinc( temporary_pointer );
        }
    }

    // title_parameter can be NULL which just returns the number of bytes
    if ( title_parameter == nullptr )
    {
        return( lstrlen( temporary_writable_pointer ) + 1 );
    }

    // otherwise copy it into the buffer provided
    std::ignore = lstrcpyn( title_parameter, temporary_writable_pointer, maximum_number_of_characters );

    return( 0 );
}

static inline void __GetRoot( _In_z_ wchar_t const * path_name_parameter, _Inout_ std::wstring& root_string ) noexcept
{
    WFC_VALIDATE_POINTER( path_name_parameter );

    // determine the root name of the volume

    wchar_t root_name[ _MAX_PATH ];

    wchar_t* root_pointer{ root_name };

    ZeroMemory( root_pointer, _MAX_PATH );

    std::ignore = lstrcpynW( root_pointer, path_name_parameter, _MAX_PATH );

    wchar_t* temporary_pointer{ nullptr };

    for ( temporary_pointer = root_pointer; *temporary_pointer not_eq '\0'; temporary_pointer = _wcsinc( temporary_pointer ) )
    {
        // find first double slash and stop
        if ( __is_directory_separation_character( temporary_pointer[ 0 ] ) and
            __is_directory_separation_character( temporary_pointer[ 1 ] ) )
        {
            break;
        }
    }

    if ( *temporary_pointer not_eq '\0' )
    {
        // it is a UNC name, find second slash past '\\'
        ASSERT( __is_directory_separation_character( temporary_pointer[ 0 ] ) );
        ASSERT( __is_directory_separation_character( temporary_pointer[ 1 ] ) );

        temporary_pointer += 2;

        while ( *temporary_pointer not_eq '\0' and (not __is_directory_separation_character( *temporary_pointer ) ) )
        {
            temporary_pointer = _wcsinc( temporary_pointer );
        }

        if ( *temporary_pointer not_eq '\0' )
        {
            temporary_pointer = _wcsinc( temporary_pointer );
        }

        while ( *temporary_pointer not_eq '\0' and (not __is_directory_separation_character( *temporary_pointer ) ) )
        {
            temporary_pointer = _wcsinc( temporary_pointer );
        }

        // terminate it just after the UNC root (ie. '\\server\share\')

        if ( *temporary_pointer not_eq '\0' )
        {
            temporary_pointer[ 1 ] = '\0';
        }
    }
    else
    {
        // not a UNC, look for just the first slash
        temporary_pointer = root_pointer;

        while ( *temporary_pointer not_eq '\0' and (not __is_directory_separation_character( *temporary_pointer ) ) )
        {
            temporary_pointer = _wcsinc( temporary_pointer );
        }

        // terminate it just after root (ie. 'x:\')
        if ( *temporary_pointer not_eq '\0' )
        {
            temporary_pointer[ 1 ] = '\0';
        }
    }

    root_string.assign( root_name );
}

#define __wfc_next_value(_value)    ((_value)+1)

static inline _Check_return_ bool __FullPath( _Out_ wchar_t * lpszPathOut, _In_z_ wchar_t const * lpszFileIn ) noexcept
{
    WFC_VALIDATE_POINTER( lpszPathOut );

    // first, fully qualify the path name
    wchar_t* lpszFilePart{ nullptr };

    if (not GetFullPathNameW( lpszFileIn, _MAX_PATH, lpszPathOut, &lpszFilePart ) )
    {
        std::ignore = lstrcpyn(lpszPathOut, lpszFileIn, _MAX_PATH); // take it literally
        return( false );
    }

    std::wstring strRoot;

    // determine the root name of the volume
    __GetRoot(lpszPathOut, strRoot);

    // get file system information for the volume

    DWORD dwFlags{ 0 };
    DWORD dwDummy{ 0 };

    if (not GetVolumeInformationW(strRoot.c_str(), nullptr, 0, nullptr, &dwDummy, &dwFlags, nullptr, 0 ) )
    {
        return( false );   // preserving case may not be correct
    }

    // not all characters have complete uppercase/lowercase

    if (is_flagged( dwFlags, FS_CASE_IS_PRESERVED ) == false )
    {
        CharUpper( lpszPathOut );
    }

    // assume non-UNICODE file systems, use OEM character set

    if (is_flagged( dwFlags, FS_UNICODE_STORED_ON_DISK ) == false )
    {
        WIN32_FIND_DATA data;

        auto const h{ FindFirstFile(lpszFileIn, &data) };

        if ( h not_eq static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
        {
            FindClose( h );
            lstrcpy( lpszFilePart, data.cFileName );
        }
    }

    return( true );
}

static inline _Check_return_ std::size_t __GetFileTitle( _In_z_ wchar_t const * path_name, __out_ecount_opt( nMax ) wchar_t * title, _In_ UINT const nMax) noexcept
{
    WFC_VALIDATE_POINTER( path_name );
    WFC_VALIDATE_POINTER_NULL_OK( title );

    // use a temporary to avoid bugs in ::GetFileTitle when lpszTitle is NULL
    wchar_t szTemp[_MAX_PATH];
    wchar_t* lpszTemp{ title };

    UINT maximum_length{ nMax };

    if (lpszTemp == nullptr)
    {
        lpszTemp = szTemp;
        maximum_length = static_cast<UINT>(std::size(szTemp));
    }

    if ( ::GetFileTitleW( path_name, lpszTemp, (WORD) maximum_length ) not_eq 0 )
    {
        // when ::GetFileTitle fails, use cheap imitation
        return( __GetFileName( path_name, title, nMax ) );
    }

    return( ( title == nullptr ) ? _tcslen( lpszTemp ) + 1 : 0 );
}

void Win32FoundationClasses::CFile64::CreatePathTo(_In_ std::filesystem::path const& full_pathname) noexcept
{
    wchar_t drive[_MAX_DRIVE];
    wchar_t directory[_MAX_DIR];
    wchar_t filename[_MAX_FNAME];
    wchar_t extension_including_period[_MAX_EXT];

    ZeroMemory(drive, sizeof(drive));
    ZeroMemory(directory, sizeof(directory));
    ZeroMemory(filename, sizeof(filename));
    ZeroMemory(extension_including_period, sizeof(extension_including_period));

    if (_wsplitpath_s(full_pathname.c_str(),
        drive, std::size(drive),
        directory, std::size(directory),
        filename, std::size(filename),
        extension_including_period, std::size(extension_including_period)) == 0)
    {
        if (directory[0] not_eq 0x00 and directory[1] not_eq '\\')
        {
            std::wstring full_directory_path( drive );

            full_directory_path.append(directory);

            std::ignore = CFileDirectory::Create(full_directory_path);
        }
    }
}

Win32FoundationClasses::CFile64::CFile64() noexcept
{
    WFC_VALIDATE_POINTER( this );

    m_FileHandle           = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
    m_SecurityAttributes_p = nullptr;
    m_SecurityDescriptor_p = nullptr;
    m_SafeWriteCallback    = nullptr;
    m_SafeWriteContext     = nullptr;
    m_LastError = ERROR_SUCCESS;

#if ! defined( WFC_STL )

    m_BeStupidAndUseExceptions = false;

#endif

    m_Initialize();
}

Win32FoundationClasses::CFile64::CFile64( _In_ HANDLE const file_handle ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    _ASSERTE(file_handle not_eq INVALID_HANDLE_VALUE);
    _ASSERTE(file_handle not_eq NULL);

    m_FileHandle           = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
    m_SecurityAttributes_p = nullptr;
    m_SecurityDescriptor_p = nullptr;
    m_SafeWriteCallback    = nullptr;
    m_SafeWriteContext     = nullptr;

    m_Initialize();

    m_FileHandle    = file_handle;
    m_CloseOnDelete = false;
}

Win32FoundationClasses::CFile64::CFile64(_In_ std::filesystem::path const& filename, _In_ UINT const open_flags ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    m_FileHandle           = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
    m_SecurityAttributes_p = nullptr;
    m_SecurityDescriptor_p = nullptr;
    m_SafeWriteCallback    = nullptr;
    m_SafeWriteContext     = nullptr;

    m_Initialize();

    std::ignore = Open( filename, open_flags );
}

Win32FoundationClasses::CFile64::~CFile64()
{
    WFC_VALIDATE_POINTER( this );

    if ( m_FileHandle not_eq static_cast< HANDLE >( INVALID_HANDLE_VALUE ) and m_CloseOnDelete == true)
    {
        Close();
    }

    m_Uninitialize();

    m_SecurityAttributes_p = nullptr;
    m_SecurityDescriptor_p = nullptr;
    m_SafeWriteCallback    = nullptr;
    m_SafeWriteContext     = nullptr;
}

void Win32FoundationClasses::CFile64::Abort( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_FileHandle not_eq static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        std::ignore = Win32FoundationClasses::wfc_close_handle(m_FileHandle);
        m_FileHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
    }

    m_FileName.clear();
    m_PathName.clear();
    m_FileTitle.clear();
    m_CloseOnDelete = false;
}

void Win32FoundationClasses::CFile64::Close( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_FileHandle not_eq static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        std::ignore = Win32FoundationClasses::wfc_close_handle(m_FileHandle);
        m_FileHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
    }

    m_FileName.clear();
    m_PathName.clear();
    m_FileTitle.clear();
    m_CloseOnDelete = false;

#if ! defined( WFC_STL )

    if ( return_value == FALSE and m_BeStupidAndUseExceptions == true )
    {
        CFileException::ThrowOsError( (LONG) ::GetLastError() );
    }

#endif // WFC_STL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CFile64::Dump( CDumpContext& dump_context ) const
{
    WFC_VALIDATE_POINTER( this );
    dump_context << TEXT( "a CFile64 at " ) << (VOID *) this << TEXT( "\n{\n" );

    dump_context << TEXT( "   m_PathName             is \"" ) << m_PathName  << TEXT( "\"\n" );
    dump_context << TEXT( "   m_FileName             is \"" ) << m_FileName  << TEXT( "\"\n" );
    dump_context << TEXT( "   m_FileTitle            is \"" ) << m_FileTitle << TEXT( "\"\n" );
    dump_context << TEXT( "   m_CloseOnDelete        is "   );

    if ( m_CloseOnDelete == false )
    {
        dump_context << TEXT( "false" );
    }
    else
    {
        dump_context << TEXT( "true" );
    }

    dump_context << TEXT( "\n" );

    dump_context << TEXT( "   m_SecurityAttributes_p is " );

    if ( m_SecurityAttributes_p not_eq nullptr )
    {
        dump_context << (VOID *) m_SecurityDescriptor_p;
        dump_context << TEXT( "\n   {\n" );
        dump_context << TEXT( "      nLength              is " ) << m_SecurityAttributes_p->nLength << TEXT( "\n" );
        dump_context << TEXT( "      lpSecurityDescriptor is " ) << m_SecurityAttributes_p->lpSecurityDescriptor << TEXT( "\n" );
        dump_context << TEXT( "      bInheritHandle       is " );

        if ( m_SecurityAttributes_p->bInheritHandle == FALSE )
        {
            dump_context << TEXT( "FALSE" );
        }
        else
        {
            dump_context << TEXT( "TRUE" );
        }

        dump_context << TEXT( "\n   }\n" );
    }
    else
    {
        dump_context << TEXT( "NULL" );
    }

    dump_context << TEXT( "\n" );

    dump_context << TEXT( "   m_SecurityDescriptor_p is " );

    if ( m_SecurityDescriptor_p not_eq nullptr )
    {
        dump_context << (VOID *) m_SecurityDescriptor_p;
    }
    else
    {
        dump_context << TEXT( "NULL" );
    }

    dump_context << TEXT( "\n" );

    dump_context << TEXT( "   m_FileHandle           is " );

    if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        dump_context << TEXT( "INVALID_HANDLE_VALUE" );
    }
    else
    {
        dump_context << m_FileHandle;
    }

    dump_context << TEXT( "\n" );

    if ( dump_context.GetDepth() > 0 and m_FileHandle not_eq static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        BY_HANDLE_FILE_INFORMATION information;

        ::GetFileInformationByHandle( m_FileHandle, &information );

        dump_context << TEXT( "\n   extended information from BY_HANDLE_FILE_INFORMATION\n   {\n" );
        dump_context << TEXT( "      dwFileAttributes     is " ) << information.dwFileAttributes << TEXT( " (" );

        std::wstring attributes;

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_ARCHIVE )
        {
            attributes.assign(WSTRING_VIEW(L"FILE_ATTRIBUTE_ARCHIVE"));
        }

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_COMPRESSED )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_COMPRESSED" );
        }

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_ENCRYPTED )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_ENCRYPTED" );
        }

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_DIRECTORY )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_DIRECTORY" );
        }

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_HIDDEN )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_HIDDEN" );
        }

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_OFFLINE )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_OFFLINE" );
        }

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_READONLY )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_READONLY" );
        }

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_REPARSE_POINT )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_REPARSE_POINT" );
        }

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_SPARSE_FILE )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_SPARSE_FILE" );
        }

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_SYSTEM )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_SYSTEM" );
        }

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_TEMPORARY )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_TEMPORARY" );
        }

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_NOT_CONTENT_INDEXED )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_NOT_CONTENT_INDEXED" );
        }

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_NORMAL )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_NORMAL" );
        }

        if ( information.dwFileAttributes bitand FILE_ATTRIBUTE_DEVICE )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_DEVICE" );
        }

        dump_context << attributes << TEXT( ")\n" );

        attributes.Empty();

        CFileTime file_time;

        CTime normal_time;

        LARGE_INTEGER large_integer;

        dump_context << TEXT( "      ftCreationTime       is " );

        large_integer.LowPart  = information.ftCreationTime.dwLowDateTime;
        large_integer.HighPart = information.ftCreationTime.dwHighDateTime;

        if ( large_integer.QuadPart == 0 )
        {
            dump_context << TEXT( " 0 (file system doesn't support this)" );
        }
        else
        {
            file_time.Copy( &information.ftCreationTime );
            file_time.CopyTo( normal_time );

            attributes = normal_time.Format( WFC_STANDARD_TIME_FORMAT );

            dump_context << attributes;
        }

        dump_context << TEXT( "\n" );

        dump_context << TEXT( "      ftLastAccessTime     is " );

        large_integer.LowPart  = information.ftLastAccessTime.dwLowDateTime;
        large_integer.HighPart = information.ftLastAccessTime.dwHighDateTime;

        if ( large_integer.QuadPart == 0 )
        {
            dump_context << TEXT( " 0 (file system doesn't support this)" );
        }
        else
        {
            file_time.Copy( &information.ftLastAccessTime );
            file_time.CopyTo( normal_time );

            attributes = normal_time.Format( WFC_STANDARD_TIME_FORMAT );

            dump_context << attributes;
        }

        dump_context << TEXT( "\n" );

        dump_context << TEXT( "      ftLastWriteTime      is " );

        large_integer.LowPart  = information.ftLastWriteTime.dwLowDateTime;
        large_integer.HighPart = information.ftLastWriteTime.dwHighDateTime;

        if ( large_integer.QuadPart == 0 )
        {
            dump_context << TEXT( " 0 (file system doesn't support this)" );
        }
        else
        {
            file_time.Copy( &information.ftLastWriteTime );
            file_time.CopyTo( normal_time );

            attributes = normal_time.Format( WFC_STANDARD_TIME_FORMAT );

            dump_context << attributes;
        }

        dump_context << TEXT( "\n" );

        dump_context << TEXT( "      dwVolumeSerialNumber is " ) << information.dwVolumeSerialNumber << TEXT( "(0x" );

        TCHAR temp_string[ 31 ];
        _ultot( information.dwVolumeSerialNumber, temp_string, 16 );

        dump_context << temp_string << TEXT( ")\n" );

        large_integer.LowPart  = information.nFileSizeLow;
        large_integer.HighPart = information.nFileSizeHigh;

        _ui64tot( large_integer.QuadPart, temp_string, 10 );

        dump_context << TEXT( "      nFileSize            is " ) << temp_string;
        dump_context << TEXT( " or MAKEDWORDLONG( " ) << large_integer.LowPart << TEXT( ", " ) << large_integer.HighPart << TEXT( " )\n" );

        dump_context << TEXT( "      nNumberOfLinks       is " ) << information.nNumberOfLinks << TEXT( "\n" );

        large_integer.LowPart  = information.nFileIndexLow;
        large_integer.HighPart = information.nFileIndexHigh;

        _ui64tot( large_integer.QuadPart, temp_string, 10 );

        dump_context << TEXT( "      nFileIndex           is " ) << temp_string;
        dump_context << TEXT( " (0x" );
        _ui64tot( large_integer.QuadPart, temp_string, 16 );
        dump_context << temp_string << TEXT( ")" );

        dump_context << TEXT( " or MAKEDWORDLONG( " ) << large_integer.LowPart;

        _ultot( large_integer.LowPart, temp_string, 16 );

        dump_context << TEXT( " (0x" ) << temp_string << TEXT( "), " ) << large_integer.HighPart << TEXT( " (0x" );

        _ultot( large_integer.HighPart, temp_string, 16 );

        dump_context << temp_string << TEXT( ") )\n" );

        dump_context << TEXT( "   }\n" );

        dump_context << TEXT( "\n File type is " );

        large_integer.LowPart = ::GetFileType( m_FileHandle );

        switch( large_integer.LowPart )
        {
        case FILE_TYPE_UNKNOWN:

            dump_context << TEXT( "FILE_TYPE_UNKNOWN" );
            break;

        case FILE_TYPE_DISK:

            dump_context << TEXT( "FILE_TYPE_DISK" );
            break;

        case FILE_TYPE_CHAR:

            dump_context << TEXT( "FILE_TYPE_CHAR" );
            break;

        case FILE_TYPE_PIPE:

            dump_context << TEXT( "FILE_TYPE_PIPE" );
            break;

        case FILE_TYPE_REMOTE:

            dump_context << TEXT( "FILE_TYPE_REMOTE" );
            break;

        default:

            dump_context << TEXT( "Undocumened value of " ) << large_integer.LowPart;
            break;
        }

        dump_context << TEXT( "\n" );
    }

    dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ Win32FoundationClasses::CFile64 * Win32FoundationClasses::CFile64::Duplicate( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        return( (CFile64 *) nullptr );
    }

    auto return_value{ new Win32FoundationClasses::CFile64() };

    auto duplicate_file_handle{ static_cast<HANDLE>(INVALID_HANDLE_VALUE) };

    if ( ::DuplicateHandle( ::GetCurrentProcess(), m_FileHandle, ::GetCurrentProcess(), &duplicate_file_handle, 0, FALSE, DUPLICATE_SAME_ACCESS ) == FALSE )
    {
        //WFCTRACEERROR( ::GetLastError() );

        delete return_value;
        return_value = nullptr;

#if ! defined( WFC_STL )

        if ( m_BeStupidAndUseExceptions == true )
        {
            CFileException::ThrowOsError( (LONG) ::GetLastError() );
        }

#endif // WFC_STL
    }
    else
    {
        return_value->m_FileHandle    = duplicate_file_handle;
        return_value->m_CloseOnDelete = m_CloseOnDelete;
        return_value->m_PathName.assign( m_PathName );
        return_value->m_FileName.assign( m_FileName );
        return_value->m_FileTitle.assign( m_FileTitle );
    }

    return( return_value );
}

void Win32FoundationClasses::CFile64::Flush( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        return;
    }

    if ( ::FlushFileBuffers( m_FileHandle ) == FALSE )
    {
        //WFCTRACEERROR( ::GetLastError() );

#if ! defined( WFC_STL )

        if ( m_BeStupidAndUseExceptions == true )
        {
            CFileException::ThrowOsError( (LONG) ::GetLastError() );
        }

#endif // WFC_STL
    }
}

_Check_return_ std::wstring Win32FoundationClasses::CFile64::GetFileName( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( m_FileName.c_str() );
}

_Check_return_ std::wstring Win32FoundationClasses::CFile64::GetFilePath( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( m_PathName.c_str() );
}

_Check_return_ std::wstring Win32FoundationClasses::CFile64::GetFileTitle( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    // 2000-05-23
    // You pay an enourmous performance penalty for calling GetFilePath().
    // The children at Microsoft actually create a COM object and
    // read tons of registry settings when you call GetFilePath().
    // This is not a good thing when you are trying to be fast.
    // We will delay the penality as long as possible.

    if ( m_FileTitle.empty() == true and m_PathName.empty() == false )
    {
        TCHAR file_title[ _MAX_FNAME ];

        if ( ::__GetFileTitle( m_PathName.c_str(), file_title, _MAX_FNAME ) == 0 )
        {
            const_cast< std::wstring& >( m_FileTitle ).assign( file_title );
        }
        else
        {
            const_cast< std::wstring& >( m_FileTitle ).clear();
        }
    }

    return( m_FileTitle );
}

_Check_return_ HANDLE Win32FoundationClasses::CFile64::GetHandle( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( m_FileHandle );
}

_Check_return_ bool Win32FoundationClasses::CFile64::GetInformation( _Inout_ BY_HANDLE_FILE_INFORMATION& information ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    ZeroMemory( &information, sizeof( information ) );

    if ( ::GetFileInformationByHandle( m_FileHandle, &information ) == FALSE )
    {
        //WFCTRACEERROR( ::GetLastError() );
        return( false );
    }

    return( true );
}

_Check_return_ uint64_t Win32FoundationClasses::CFile64::GetLength( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    LARGE_INTEGER length{ 0, 0 };

    if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        return( length.QuadPart );
    }

    if ( ::GetFileSizeEx( m_FileHandle, &length ) == FALSE )
    {
        //WFCTRACEERROR( ::GetLastError() );
        length.QuadPart = 0;
    }

    return( length.QuadPart );
}

_Check_return_ uint64_t Win32FoundationClasses::CFile64::GetPosition( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        return( (uint64_t) -1 );
    }

    LARGE_INTEGER return_value{ 0,0 };
    LARGE_INTEGER zero{ 0, 0 };

    // There's a bug in NT where SetFilePointer() doesn't set last error correctly or at all...

    if ( ::SetFilePointerEx( m_FileHandle, zero, &return_value, FILE_CURRENT ) == 0 )
    {
        // An Error Occurred
        //WFCTRACEERROR( ::GetLastError() );

#if ! defined( WFC_STL )

        if ( m_BeStupidAndUseExceptions == true )
        {
            CFileException::ThrowOsError( (LONG) ::GetLastError() );
        }
        else
        {
            return_value.QuadPart = 0;
        }

#else // WFC_STL
        return_value.QuadPart = 0;
#endif // WFC_STL
    }

    return( return_value.QuadPart );
}

_Check_return_ SECURITY_ATTRIBUTES * Win32FoundationClasses::CFile64::GetSecurityAttributes( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( m_SecurityAttributes_p );
    return( m_SecurityAttributes_p );
}

_Check_return_ SECURITY_DESCRIPTOR * Win32FoundationClasses::CFile64::GetSecurityDescriptor( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( m_SecurityDescriptor_p );
    return( m_SecurityDescriptor_p );
}

#if ! defined( WFC_STL )

_Check_return_ bool CFile64::GetStatus( CFileStatus& status ) const
{
    WFC_VALIDATE_POINTER( this );
    ZeroMemory( &status, sizeof( status ) );

    BY_HANDLE_FILE_INFORMATION information;

    if ( GetInformation( information ) == FALSE )
    {
        return( FALSE );
    }

    _tcsncpy( status.m_szFullName, m_FileName, std::size( status.m_szFullName ) );

    if ( information.nFileSizeHigh not_eq 0 )
    {
        // File is too large to return information about
        return( FALSE );
    }

    if ( m_FileName.IsEmpty() )
    {
        status.m_attribute = 0;
    }
    else
    {
        status.m_attribute = (BYTE) information.dwFileAttributes;

        // convert times as appropriate

#if ( _MFC_VER < 0x0700 )
        status.m_ctime = CTime( information.ftCreationTime   );
        status.m_atime = CTime( information.ftLastAccessTime );
        status.m_mtime = CTime( information.ftLastWriteTime  );
#else
        // MFC 7 version...
        status.m_ctime = ATL::CTime( information.ftCreationTime   );
        status.m_atime = ATL::CTime( information.ftLastAccessTime );
        status.m_mtime = ATL::CTime( information.ftLastWriteTime  );
#endif

        if ( status.m_ctime.GetTime() == 0 )
        {
            status.m_ctime = status.m_mtime;
        }

        if ( status.m_atime.GetTime() == 0 )
        {
            status.m_atime = status.m_mtime;
        }
    }

    return( true );
}

_Check_return_ BOOL CFile64::GetStatus( _In_z_ LPCTSTR filename, _Out_ CFileStatus& status )
{
    WFC_VALIDATE_POINTER( filename );

    if ( ::__FullPath( status.m_szFullName, filename ) == FALSE )
    {
        status.m_szFullName[ 0 ] = TEXT( '\0' );
        return( FALSE );
    }

    WIN32_FIND_DATA find_data;

    auto const find_handle{ ::FindFirstFile((LPTSTR)filename, &find_data) };

    if ( find_handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        return( FALSE );
    }

    FindClose( find_handle );

    status.m_attribute = (BYTE) ( find_data.dwFileAttributes bitand (compl FILE_ATTRIBUTE_NORMAL) );

    status.m_size = (LONG) find_data.nFileSizeLow;

    // convert times as appropriate
#if ( _MFC_VER < 0x0700 )
    status.m_ctime = CTime( find_data.ftCreationTime   );
    status.m_atime = CTime( find_data.ftLastAccessTime );
    status.m_mtime = CTime( find_data.ftLastWriteTime  );
#else
    // MFC 7 version
    status.m_ctime = ATL::CTime( find_data.ftCreationTime   );
    status.m_atime = ATL::CTime( find_data.ftLastAccessTime );
    status.m_mtime = ATL::CTime( find_data.ftLastWriteTime  );
#endif

    if ( status.m_ctime.GetTime() == 0 )
    {
        status.m_ctime = status.m_mtime;
    }

    if ( status.m_atime.GetTime() == 0 )
    {
        status.m_atime = status.m_mtime;
    }

    return( TRUE );
}

#endif // WFC_STL

void Win32FoundationClasses::CFile64::m_Initialize( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    m_CloseOnDelete = false;
    m_Attributes    = FILE_ATTRIBUTE_NORMAL;
    m_TemplateFile  = static_cast< HANDLE >( NULL );

    WFC_TRY
    {
        m_SecurityAttributes_p = new SECURITY_ATTRIBUTES;
    }
    WFC_CATCH_ALL
    {
        m_SecurityAttributes_p = nullptr;
    }
    WFC_END_CATCH_ALL

    if ( m_SecurityAttributes_p == nullptr )
    {
        return;
    }

    m_SecurityDescriptor_p = Win32FoundationClasses::wfc_create_null_dacl();

    if ( m_SecurityDescriptor_p == nullptr )
    {
        delete m_SecurityAttributes_p;
        m_SecurityAttributes_p = nullptr;

        return;
    }

    m_SecurityAttributes_p->nLength              = sizeof( SECURITY_ATTRIBUTES );
    m_SecurityAttributes_p->lpSecurityDescriptor = m_SecurityDescriptor_p;
    m_SecurityAttributes_p->bInheritHandle       = TRUE;
}

/*
Generating SDDL strings is a royal pain. The easiest way to do it is to use
explorer to set the permissions on a file as you want the SDDL for. Then
spawn a command prompt and issue the command:
cacls myfile.txt /S
It will print out the SDDL string that you can cut and paste into your code.
*/
void Win32FoundationClasses::CFile64::SetSecurity(_In_z_ wchar_t const * sddl) noexcept
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER(sddl);
    _ASSERTE(sddl[0] not_eq 0x00);

    if (sddl == nullptr or sddl[0] == 0x00)
    {
        return;
    }

    if (m_SecurityAttributes_p not_eq nullptr)
    {
        delete m_SecurityAttributes_p;
        m_SecurityAttributes_p = nullptr;
    }

    if (m_SecurityDescriptor_p not_eq nullptr)
    {
        wfc_destroy_null_dacl(m_SecurityDescriptor_p);
        m_SecurityDescriptor_p = nullptr;
    }

    _ASSERTE(m_SecurityAttributes_p == nullptr);
    _ASSERTE(m_SecurityDescriptor_p == nullptr);

    m_SecurityAttributes_p = new SECURITY_ATTRIBUTES;
    m_SecurityAttributes_p->nLength = sizeof(SECURITY_ATTRIBUTES);
    m_SecurityAttributes_p->bInheritHandle = TRUE;

    if (ConvertStringSecurityDescriptorToSecurityDescriptor(sddl, SDDL_REVISION_1, &m_SecurityAttributes_p->lpSecurityDescriptor, nullptr) == FALSE)
    {
        m_LastError = ::GetLastError();
    }

    m_SecurityDescriptor_p = static_cast<SECURITY_DESCRIPTOR *>(m_SecurityAttributes_p->lpSecurityDescriptor);
}

void Win32FoundationClasses::CFile64::m_Uninitialize( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_SecurityAttributes_p not_eq nullptr )
    {
        delete m_SecurityAttributes_p;
        m_SecurityAttributes_p = nullptr;
    }

    if ( m_SecurityDescriptor_p not_eq nullptr )
    {
        Win32FoundationClasses::wfc_destroy_null_dacl( m_SecurityDescriptor_p );
        m_SecurityDescriptor_p = nullptr;
    }
}

_Check_return_ bool Win32FoundationClasses::CFile64::LockRange( _In_ uint64_t const position, _In_ uint64_t const number_of_bytes_to_lock ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    LARGE_INTEGER parameter_1;
    LARGE_INTEGER parameter_2;

    parameter_1.QuadPart = position;
    parameter_2.QuadPart = number_of_bytes_to_lock;

    if ( ::LockFile( m_FileHandle, parameter_1.LowPart, parameter_1.HighPart, parameter_2.LowPart, parameter_2.HighPart ) == FALSE )
    {
        m_LastError = ::GetLastError();

#if ! defined( WFC_STL )
        if ( m_BeStupidAndUseExceptions == true )
        {
            CFileException::ThrowOsError( (LONG) ::GetLastError() );
        }
#endif // WFC_STL

        return( false );
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CFile64::Open(_In_ std::filesystem::path const& filename, _In_ UINT const open_flags_parameter ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    Close();

    UINT open_flags{ open_flags_parameter };

    WFC_TRY
    {
        m_FileName.assign( filename );

        TCHAR full_path[_MAX_PATH]{ 0 };

        std::ignore = ::__FullPath( full_path, m_FileName.c_str());

        m_PathName.assign( full_path );

        m_FileTitle.clear();

        open_flags and_eq compl (UINT) OpenFlags::typeBinary;

        DWORD access{ 0 };

        switch ( open_flags bitand 3 )
        {
        case (UINT) OpenFlags::modeRead:

            access = GENERIC_READ;
            break;

        case (UINT) OpenFlags::modeWrite:

            access = GENERIC_WRITE;
            break;

        case (UINT) OpenFlags::modeReadWrite:

            access = GENERIC_READ bitor GENERIC_WRITE;
            break;

        default:

            ASSERT( FALSE );  // invalid share mode
        }

        DWORD share_mode{ 0 };

        switch ( static_cast<OpenFlags>(open_flags bitand 0x70) )    // map compatibility mode to exclusive
        {
        case OpenFlags::shareCompat:
        case OpenFlags::shareExclusive:

            share_mode = 0;
            break;

        case OpenFlags::shareDenyWrite:

            share_mode = FILE_SHARE_READ;
            break;

        case OpenFlags::shareDenyRead:

            share_mode = FILE_SHARE_WRITE;
            break;

        case OpenFlags::shareDenyNone:

            share_mode = FILE_SHARE_WRITE bitor FILE_SHARE_READ;
            break;

        default:

            ASSERT( FALSE );  // invalid share mode?
        }

        if (is_flagged(open_flags, (UINT)OpenFlags::shareAllowDelete))
        {
            share_mode or_eq FILE_SHARE_DELETE;
        }

        if ( m_SecurityAttributes_p not_eq nullptr )
        {
            m_SecurityAttributes_p->bInheritHandle = ( ( open_flags bitand (UINT) OpenFlags::modeNoInherit ) == 0 ) ? TRUE : FALSE;
        }

        DWORD creation_flags{ 0 };

        if ( is_flagged(open_flags, (uint64_t) OpenFlags::modeCreate) == true )
        {
            if ( is_flagged( open_flags, (uint64_t) OpenFlags::modeNoTruncate) == true )
            {
                creation_flags = OPEN_ALWAYS;
            }
            else
            {
                creation_flags = CREATE_ALWAYS;
            }
        }
        else
        {
            creation_flags = OPEN_EXISTING;
        }

        if ( is_flagged( open_flags, (uint64_t) OpenFlags::osNoBuffer) == true )
        {
            m_Attributes or_eq FILE_FLAG_NO_BUFFERING;
        }

        if ( is_flagged( open_flags, (uint64_t) OpenFlags::osRandomAccess) == true )
        {
            m_Attributes or_eq FILE_FLAG_RANDOM_ACCESS;
        }

        if ( is_flagged( open_flags, (uint64_t) OpenFlags::osSequentialScan) == true )
        {
            m_Attributes or_eq FILE_FLAG_SEQUENTIAL_SCAN;
        }

        if ( is_flagged( open_flags, (uint64_t) OpenFlags::osWriteThrough) == true )
        {
            m_Attributes or_eq FILE_FLAG_WRITE_THROUGH;
        }

        if ( is_flagged( open_flags, (uint64_t) OpenFlags::wfcDeleteOnClose) == true )
        {
            m_Attributes or_eq FILE_FLAG_DELETE_ON_CLOSE;
        }

        if (is_flagged(open_flags, (uint64_t)OpenFlags::osOverlapped) == true)
        {
            m_Attributes or_eq FILE_FLAG_OVERLAPPED;
        }

        m_FileHandle = ::CreateFile(m_FileName.c_str(),
            access,
            share_mode,
            GetSecurityAttributes(),
            creation_flags,
            GetAttributes(),
            GetTemplateFile() );

        if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
        {
            m_LastError = ::GetLastError();

            //WFCTRACEERROR( ::GetLastError() );

#if ! defined( WFC_STL )

            if ( exception_p not_eq nullptr )
            {
                exception_p->m_lOsError    = ::GetLastError();
                exception_p->m_cause       = CFileException::OsErrorToException( exception_p->m_lOsError );
                exception_p->m_strFileName = filename;
            }

#endif // WFC_STL

            Close();
            return( false );
        }

        m_CloseOnDelete = true;

        return( true );
    }
    WFC_CATCH_ALL
    {
        return( false );
    }
    WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CFile64::Read(_Inout_ std::wstring& string_from_file) noexcept
{
    std::string s;

    if (Read(s) == true)
    {
        copy(string_from_file, s.c_str(), s.length());
        return(true);
    }

    string_from_file.clear();
    return(false);
}

_Check_return_ bool Win32FoundationClasses::CFile64::Read( _Inout_ std::string& string_from_file ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    string_from_file.clear();

    BYTE byte_from_file{ 0x00 };

    bool exit_loop{ false };

    while( exit_loop == false )
    {
        if ( Read( &byte_from_file, 1 ) not_eq 1 )
        {
            return( ( string_from_file.length() > 0 ) ? true : false );
        }

        if ( byte_from_file == 0x00 or
             byte_from_file == LINE_FEED )
        {
            return( true );
        }

        if ( byte_from_file == CARRIAGE_RETURN )
        {
            // See if next character is  a line feed...

            uint64_t current_position{ GetPosition() };

            if ( Read( &byte_from_file, 1 ) not_eq 1 )
            {
                return( true );
            }

            if ( byte_from_file == LINE_FEED )
            {
                return( true );
            }

            // Not a CRLF pair, put the character back....

            return( SetPosition( current_position ) );
        }

        string_from_file.push_back( byte_from_file );
    }

    return( false );
}

_Check_return_ bool Win32FoundationClasses::CFile64::Read( _Inout_ std::vector<std::string>& array ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    std::string string_from_file;

    while( Read( string_from_file ) == true )
    {
        array.push_back( string_from_file );
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CFile64::Read(_Inout_ std::vector<std::wstring>& array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    std::wstring string_from_file;

    while (Read(string_from_file) == true)
    {
        array.push_back(string_from_file);
    }

    return(true);
}

_Check_return_ bool Win32FoundationClasses::CFile64::Read(_Inout_ std::vector<uint8_t>& bytes, _In_ uint32_t const number_of_bytes_to_read) noexcept
{
    WFC_VALIDATE_POINTER(this);

    bytes.resize(number_of_bytes_to_read);

    if (Read(bytes.data(), static_cast<UINT>(bytes.size())) == bytes.size())
    {
        return(true);
    }

    bytes.clear();
    return(false);
}

_Check_return_ uint32_t Win32FoundationClasses::CFile64::Read( __out_bcount( number_of_bytes_to_read ) void * buffer, _In_ uint32_t const number_of_bytes_to_read ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( buffer );

#if defined( _DEBUG )
    if ( number_of_bytes_to_read == 1 )
    {
        WFC_COVERAGE( 29 );
    }
#endif // _DEBUG

    if ( number_of_bytes_to_read == 0 )
    {
        return( 0 );
    }

    if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        return( 0 );
    }

    DWORD number_of_bytes_read{ 0 };

    if ( ::ReadFile( m_FileHandle, buffer, number_of_bytes_to_read, &number_of_bytes_read, nullptr ) == FALSE )
    {
        m_LastError = ::GetLastError();
    }

    return( number_of_bytes_read );
}

_Check_return_ uint32_t Win32FoundationClasses::CFile64::AtomicRead( _In_ HANDLE manual_reset_event_handle, _In_ uint64_t const file_offset, __out_bcount( number_of_bytes_to_read ) void * buffer, _In_ uint32_t const number_of_bytes_to_read ) const noexcept
{
    //WFCTRACEVAL( TEXT( "Reading " ), number_of_bytes_to_read );
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( buffer );

    if ( number_of_bytes_to_read < 1 )
    {
        ::SetLastError(ERROR_SUCCESS);
        return( 0 );
    }

    if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        ::SetLastError(ERROR_INVALID_HANDLE);
        return( 0 );
    }

    // 2021-03-23 SRB - Another bad day. Raymond Chen describes why, after 20 years, this no longer works.
    // https://devblogs.microsoft.com/oldnewthing/20121012-00/?p=6343

    OVERLAPPED overlapped;

    overlapped.Pointer      = reinterpret_cast<PVOID>(file_offset);
    overlapped.hEvent       = manual_reset_event_handle;
    overlapped.Internal     = 0;
    overlapped.InternalHigh = 0;

    DWORD number_of_bytes_read{ 0 };

    if ( ::ReadFile( m_FileHandle, buffer, number_of_bytes_to_read, &number_of_bytes_read, &overlapped ) == FALSE )
    {
        auto const last_error{ ::GetLastError() };

        if (last_error == ERROR_IO_PENDING)
        {
            // Wait until our IO completes
            if ( ::GetOverlappedResult(m_FileHandle, &overlapped, &number_of_bytes_read, TRUE ) == FALSE )
            {
                return(0);
            }
            else
            {
                const_cast<Win32FoundationClasses::CFile64 *>(this)->m_AtomicWaitSucceeded = true;
            }
        }
        else
        {
            return(0);
        }
    }

    return( number_of_bytes_read );
}

_Check_return_ uint32_t Win32FoundationClasses::CFile64::AtomicWrite(_In_ HANDLE manual_reset_event_handle, _In_ uint64_t const file_offset, __in_bcount(number_of_bytes_to_write) void const * buffer, _In_ uint32_t const number_of_bytes_to_write) const noexcept
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER_NULL_OK(buffer);

    if (number_of_bytes_to_write < 1)
    {
        return(0);
    }

    if (m_FileHandle == static_cast< HANDLE >(INVALID_HANDLE_VALUE))
    {
        return(0);
    }

    //ULARGE_INTEGER file_offset_large_integer;

    //file_offset_large_integer.QuadPart = file_offset;

    OVERLAPPED overlapped;
    // Use the Union magic to set the file offset to read in a single assignment rather than convert to ULARGE_INTEGER.

    overlapped.Pointer = (PVOID)file_offset;
    overlapped.hEvent = manual_reset_event_handle;
    overlapped.Internal = 0;
    overlapped.InternalHigh = 0;

    DWORD number_of_bytes_written{ 0 };

    // Unfortunately, atomic writes don't work. They reset the file pointer to the offset
    // after the last byte of the write. DOH! Halfway useful...
    auto const current_file_position{ GetPosition() };

    if (::WriteFile(m_FileHandle, buffer, (DWORD) number_of_bytes_to_write, &number_of_bytes_written, &overlapped) == FALSE)
    {
        auto const last_error{ ::GetLastError() };

        if (last_error == ERROR_IO_PENDING)
        {
            // Wait until our IO completes
            if (::GetOverlappedResult(m_FileHandle, &overlapped, &number_of_bytes_written, TRUE) == FALSE)
            {
                return(0);
            }
        }
        else
        {
            return(0);
        }
    }

    LARGE_INTEGER return_value{ 0, 0 };
    LARGE_INTEGER distance_to_move;

    distance_to_move.QuadPart = current_file_position;

    ::SetFilePointerEx(m_FileHandle, distance_to_move, &return_value, FILE_BEGIN);

    return(number_of_bytes_written);
}

_Check_return_ uint32_t Win32FoundationClasses::CFile64::ReadHuge( __out_bcount( number_of_bytes_to_read ) void * buffer, _In_ uint32_t const number_of_bytes_to_read ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( buffer );
    return( Read( buffer, number_of_bytes_to_read ) );
}

void Win32FoundationClasses::CFile64::Rename(_In_ std::filesystem::path const& old_name, _In_ std::filesystem::path const& new_name ) noexcept // static
{
    if ( ::MoveFileW( old_name.c_str(), new_name.c_str() ) == FALSE )
    {
    }
}

void Win32FoundationClasses::CFile64::Remove(_In_ std::filesystem::path const& filename ) noexcept // static
{
    if ( ::DeleteFileW( filename.c_str() ) == FALSE )
    {
        CFile64 file;

        // Try to delete the file with DELETE ON CLOSE semantics.

        if (file.Open(filename, read_delete_on_close()) == true)
        {
            file.Close();
        }
    }
}

_Check_return_ uint64_t Win32FoundationClasses::CFile64::Seek( _In_ int64_t const offset, _In_ SeekPosition const from ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        return(std::numeric_limits<std::uint64_t>::max());
    }

    DWORD move_method{ 0 };

    switch( from )
    {
    case CFile64::SeekPosition::begin:

        //WFCTRACEVAL( TEXT( "From beginning to " ), offset );
        move_method = FILE_BEGIN;
        break;

    case CFile64::SeekPosition::current:

        //WFCTRACEVAL( TEXT( "From current to " ), offset );
        move_method = FILE_CURRENT;
        break;

    case CFile64::SeekPosition::end:

        //WFCTRACEVAL( TEXT( "From end to " ), offset );
        move_method = FILE_END;
        break;

    default:

        return(std::numeric_limits<std::uint64_t>::max());
    }

    LARGE_INTEGER return_value{ 0, 0 };
    LARGE_INTEGER distance_to_move;

    distance_to_move.QuadPart = offset;

    // There's a bug in NT where SetFilePointer() doesn't set last error correctly or at all...
    if ( ::SetFilePointerEx( m_FileHandle, distance_to_move, &return_value, move_method ) == 0 )
    {
#if ! defined( WFC_STL )

        if ( m_BeStupidAndUseExceptions == true )
        {
            CFileException::ThrowOsError( (LONG) ::GetLastError() );
        }

#endif // WFC_STL
    }

    return( return_value.QuadPart );
}

void Win32FoundationClasses::CFile64::SeekToBegin( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    std::ignore = Seek( (int64_t) 0, SeekPosition::begin );
}

_Check_return_ uint64_t Win32FoundationClasses::CFile64::SeekToEnd( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( Seek( (int64_t) 0, SeekPosition::end ) );
}

_Check_return_ bool Win32FoundationClasses::CFile64::SetEndOfFile( _In_ uint64_t const length ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    std::ignore = Seek( length, SeekPosition::begin );

    _ASSERTE( GetPosition() == length );

    if ( ::SetEndOfFile( m_FileHandle ) == FALSE )
    {
        m_LastError = ::GetLastError();
        //WFCTRACEERROR( ::GetLastError() );
        return( false );
    }

    return( true );
}

void Win32FoundationClasses::CFile64::SetFilePath(_In_ std::filesystem::path const& new_name ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    m_FileName.assign( new_name );
}

void Win32FoundationClasses::CFile64::SetLength( _In_ uint64_t const new_length ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( SetEndOfFile( new_length ) == false )
    {
#if ! defined( WFC_STL )

        if ( m_BeStupidAndUseExceptions == true )
        {
            CFileException::ThrowOsError( (LONG)::GetLastError() );
        }

#endif // WFC_STL
    }
}

_Check_return_ bool Win32FoundationClasses::CFile64::SetPosition( _In_ uint64_t const position ) noexcept
{
    return( Seek( position, SeekPosition::begin ) == position ? true : false );
}

#if ! defined( WFC_STL )

void CFile64::SetStatus( LPCTSTR filename, CFileStatus& const status )
{
    CFile::SetStatus( filename, status );
}

#endif // WFC_STL

#if ! defined( FSCTL_SET_ZERO_DATA )
#define FSCTL_SET_ZERO_DATA             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 50, METHOD_BUFFERED, FILE_WRITE_DATA) // FILE_ZERO_DATA_INFORMATION,
#endif

// WFC version of the FILE_ZERO_DATA_INFORMATION structure
struct WFC_SPARSE_REGION
{
    LARGE_INTEGER file_offset{ 0,0 };
    LARGE_INTEGER beyond_final_zero{ 0,0 };
};

_Check_return_ bool Win32FoundationClasses::CFile64::SparsifyRegion( _In_ uint64_t const position, _In_ uint64_t const number_of_bytes_to_mark_as_sparse ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    WFC_SPARSE_REGION region_to_sparsify;

    region_to_sparsify.file_offset.QuadPart = position;
    region_to_sparsify.beyond_final_zero.QuadPart = position;
    region_to_sparsify.beyond_final_zero.QuadPart += number_of_bytes_to_mark_as_sparse;
    region_to_sparsify.beyond_final_zero.QuadPart++;

    OVERLAPPED overlapped;

    ZeroMemory( &overlapped, sizeof( overlapped ) );

    DWORD number_of_bytes_returned{ 0 };

    auto return_value{ ::DeviceIoControl(m_FileHandle,
        FSCTL_SET_ZERO_DATA,
        &region_to_sparsify,
        sizeof(region_to_sparsify),
        nullptr,
        0,
        &number_of_bytes_returned,
        &overlapped) };

    if ( return_value not_eq 0 )
    {
        return( true );
    }

    return( false );
}

void Win32FoundationClasses::CFile64::UnlockRange( _In_ uint64_t const position, _In_ uint64_t const number_of_bytes_to_unlock ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    LARGE_INTEGER parameter_1;
    LARGE_INTEGER parameter_2;

    parameter_1.QuadPart = position;
    parameter_2.QuadPart = number_of_bytes_to_unlock;

    if ( ::UnlockFile( m_FileHandle, parameter_1.LowPart, parameter_1.HighPart, parameter_2.LowPart, parameter_2.HighPart ) == FALSE )
    {
        //WFCTRACEERROR( ::GetLastError() );

#if ! defined( WFC_STL )

        if ( m_BeStupidAndUseExceptions == true )
        {
            CFileException::ThrowOsError( (LONG) ::GetLastError() );
        }

#endif // WFC_STL
    }
}

_Check_return_ bool Win32FoundationClasses::CFile64::ProcessContent(_In_ HANDLE manual_reset_event_handle, _In_ std::size_t const step_size, _Inout_ Win32FoundationClasses::PROCESS_BUFFER_CALLBACK function_to_call, __inout_opt void * callback_context ) const noexcept
{
    if ( function_to_call == nullptr )
    {
        return( false );
    }

    auto buffer{ static_cast<uint8_t*>(_aligned_malloc(step_size, 4096)) };

    if ( buffer == nullptr )
    {
        return( false );
    }

    int64_t const number_of_bytes_to_process{ static_cast<int64_t>(GetLength()) };
    int64_t number_of_bytes_processed{ 0 };

    while( number_of_bytes_processed < number_of_bytes_to_process ) // Cannot be converted to a Range loop
    {
        int64_t number_of_bytes_to_process_in_this_call{ number_of_bytes_to_process - number_of_bytes_processed };

        if ( number_of_bytes_to_process_in_this_call <= 0 )
        {
            // No more bytes to processs
            _aligned_free( buffer );
            return( true );
        }

        if ( number_of_bytes_to_process_in_this_call > (int64_t) step_size )
        {
            number_of_bytes_to_process_in_this_call = step_size;
        }

        // Now read the data.
        if ( AtomicRead(manual_reset_event_handle, static_cast<uint64_t const>(number_of_bytes_processed), (void *) buffer, (uint32_t const) number_of_bytes_to_process_in_this_call ) not_eq number_of_bytes_to_process_in_this_call)
        {
            _aligned_free( buffer );
            return( false );
        }

        if ( function_to_call( callback_context, buffer, number_of_bytes_to_process_in_this_call ) == false )
        {
            // They told us to stop.
            _aligned_free( buffer );
            return( true );
        }

        number_of_bytes_processed += number_of_bytes_to_process_in_this_call;
    }

    _aligned_free( buffer );
    return( true );
}

void Win32FoundationClasses::CFile64::RegisterSafeWrite( _Inout_ void * context, __callback Win32FoundationClasses::SAFE_WRITE_CALLBACK callback ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( context );
    WFC_VALIDATE_POINTER_NULL_OK( callback );

    m_SafeWriteContext = context;
    m_SafeWriteCallback = callback;
}

void Win32FoundationClasses::CFile64::Write( __in_bcount( number_of_bytes_to_write ) void const * buffer, _In_ uint32_t const number_of_bytes_to_write ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( number_of_bytes_to_write == 0 )
    {
        //WFCTRACE( TEXT( "No bytes to write" ) );
        return;
    }

#if defined( _DEBUG )

    std::wstring debug_message;

    auto file_position{ GetPosition() };

    format( debug_message, L"Writing at offset %" PRIu64, file_position );

    //WFCTRACE( debug_message );

#endif

    WFC_VALIDATE_POINTER( buffer );

    DWORD number_of_bytes_written{ 0 };

    while ( ::WriteFile( m_FileHandle, buffer, number_of_bytes_to_write, &number_of_bytes_written, nullptr ) == FALSE )
    {
        auto error_code{ ::GetLastError() };
        //WFCTRACEERROR( error_code );

        if ( m_SafeWriteCallback not_eq nullptr )
        {
            auto const return_value{ m_SafeWriteCallback(m_SafeWriteContext, error_code, m_FileName.c_str()) };

            switch( return_value )
            {
            case SAFE_WRITE_WAIT:

#if defined( _DEBUG )
                std::this_thread::sleep_for(std::chrono::milliseconds(15233));

#else
                std::this_thread::sleep_for(std::chrono::milliseconds(120011));
#endif
                break;

            case SAFE_WRITE_OK:

                // Try again immediately.
                break;

            case SAFE_WRITE_FAIL:
            default:

                return;
            }
        }
        else
        {

#if ! defined( WFC_STL )

            if ( m_BeStupidAndUseExceptions == true )
            {
                CFileException::ThrowOsError( (LONG) ::GetLastError(), m_FileName );
            }

#endif // WFC_STL

            return;
        }
    }

#if ! defined( WFC_STL )
    if ( number_of_bytes_written not_eq number_of_bytes_to_write )
    {
        if ( m_BeStupidAndUseExceptions == true )
        {
            ::AfxThrowFileException( CFileException::diskFull, -1, m_FileName );
        }
    }
#endif // WFC_STL
}

void Win32FoundationClasses::CFile64::WriteHuge( __in_bcount( number_of_bytes_to_write ) void const * buffer, _In_ DWORD const number_of_bytes_to_write ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( buffer );
    Write( buffer, number_of_bytes_to_write );
}

void Win32FoundationClasses::CFile64::Write( _In_ std::wstring_view string_to_write ) noexcept
{
    Write( string_to_write.data(), static_cast<UINT>(string_to_write.length() * sizeof( wchar_t ) ) );
}

void Win32FoundationClasses::CFile64::Write( _In_ std::string_view string_to_write ) noexcept
{
    Write( string_to_write.data(), static_cast<UINT>(string_to_write.length()) );
}

void Win32FoundationClasses::CFile64::Write( _In_ std::vector<std::string> const& strings, _In_ bool const include_end_of_lines ) noexcept
{
    for ( auto const& entry : strings )
    {
        Write( entry );

        if ( include_end_of_lines == true)
        {
            uint8_t end_of_line[ 2 ]{ CARRIAGE_RETURN, LINE_FEED };
            Write( end_of_line, 2 );
        }
    }
}

void Win32FoundationClasses::CFile64::Write(_In_ std::vector<std::wstring> const& strings, _In_ bool const include_end_of_lines) noexcept
{
    for( auto const& entry : strings )
    {
        Write(entry.c_str(), static_cast<UINT>(entry.length() * sizeof(wchar_t)));

        if (include_end_of_lines == true)
        {
            wchar_t end_of_line[2]{ CARRIAGE_RETURN, LINE_FEED };
            Write(end_of_line, 2);
        }
    }
}

_Check_return_ bool Win32FoundationClasses::CFile64::SetShortName( _In_z_ LPCTSTR new_short_name )  noexcept// New for 73
{
    return( SetFileShortName( m_FileHandle, new_short_name ) == 0 ? false : true );
}

_Check_return_ bool Win32FoundationClasses::CFile64::SetValidData( _In_ int64_t const valid_data_length )  noexcept// New for 73
{
    auto return_value{ ::SetFileValidData(m_FileHandle, valid_data_length) };

    if ( return_value == 0 )
    {
        auto const last_error{ ::GetLastError() };

        if ( last_error == ERROR_PRIVILEGE_NOT_HELD )
        {
            if ( Win32FoundationClasses::wfc_enable_privilege( SE_MANAGE_VOLUME_NAME ) == true)
            {
                return( SetFileValidData( m_FileHandle, valid_data_length ) == 0 ? false : true );
            }
        }

        return( false );
    }

    return( true );
}

#pragma warning( default : 4302 )

// End of source

/*
<HTML>

<HEAD>
<TITLE>CFile64</TITLE>
</HEAD>

<BODY>

<H1>CFile64</H1>

$Revision: 17 $<BR><HR>

<H2>Description</H2>

This class gives you the ability to read very large files
(up to 18 million terabytes). It is designed to be a drop in
replacement for MFC's CFile class.<BR>

<B>WARNING!</B> - I've spent too much time writing crappy exception
handling code to deal with weird situations that I had disabled the
MFC compatibility mode in MFC builds. CFile64 <I>will never throw
an exception.</I> If you want MFC exceptions, you will have to write
your own file class that inherits from CFile64 and sets the protected
member variable <CODE>m_BeStupidAndUseExceptions</CODE> to non-false.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CFile64</B>()
<B>CFile64</B>( int file_handle );
<B>CFile64</B>( LPCTSTR filename, UINT open_flags );
<DD>
Constructs this object.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Abort">Abort</A></B>( void )</PRE><DD>
Closes the file. It will <I>not</I> throw any exceptions.

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
This closes the file.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>void <B><A NAME="Dump">Dump</A></B>( CDumpContext&amp; dump_context ) const</PRE><DD>
Will dump the contents of this object in human readable form.
It is present only in debug builds of the class.

<DT><PRE>CFile64 * <B><A NAME="Duplicate">Duplicate</A></B>( void ) const</PRE><DD>
Will duplicate the handle to the file and return a new CFile64
object to you.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>void <B><A NAME="Flush">Flush</A></B>( void )</PRE><DD>
Flushes the data to the disk.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>DWORD <B><A NAME="GetAttributes">GetAttributes</A></B>( void ) const</PRE><DD>
Retrieves the attributes used in the wrapped CreateFile() API. This value is used in
the sixth parameter to CreateFile(). This method is called by the
<A HREF="#Open">Open</A> method. The default value is FILE_ATTRIBUTE_NORMAL.

<DT><PRE>std::wstring <B><A NAME="GetFileName">GetFileName</A></B>( void ) const</PRE><DD>
Returns the name of the file that is open.

<DT><PRE>std::wstring <B><A NAME="GetFilePath">GetFilePath</A></B>( void ) const</PRE><DD>
Returns the complete path of the file that is open.

<DT><PRE>std::wstring <B><A NAME="GetFileTitle">GetFileTitle</A></B>( void ) const</PRE><DD>
Returns the title of the file. This is usually the filename without
the path or extension.

<DT><PRE>BOOL <B><A NAME="GetInformation">GetInformation</A></B>( BY_HANDLE_FILE_INFORMATION&amp; information ) const</PRE><DD>
Gives you all kinds of neat information about an open file.

<DT><PRE>uint64_t <B><A NAME="GetLength">GetLength</A></B>( void ) const</PRE><DD>
Returns the length of the file. It will return zero if it fails.

<DT><PRE>uint64_t <B><A NAME="GetPosition">GetPosition</A></B>( void ) const</PRE><DD>
Returns the current position in the file of the file pointer.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>SECURITY_ATTRIBUTES * <B><A NAME="GetSecurityAttributes">GetSecurityAttributes</A></B>( void ) const</PRE><DD>
Returns the pointer to the security attributes for this file.

<DT><PRE>SECURITY_DESCRIPTOR * <B><A NAME="GetSecurityDescriptor">GetSecurityDescriptor</A></B>( void ) const</PRE><DD>
Returns the pointer to the security descriptor for this file.

<DT><PRE>BOOL <B><A NAME="GetStatus">GetStatus</A></B>( CFileStatus&amp; status ) const
static BOOL PASCAL <B>GetStatus</B>)( LPCTSTR filename, CFileStatus&amp; status )</PRE><DD>
Will fill <CODE>status</CODE> with information about the file.

<DT><PRE>HANDLE <B><A NAME="GetTemplateFile">GetTemplateFile</A></B>( void ) const</PRE><DD>
Retrieves the handle to the file that will be used as a template in the
<A HREF="#Open">Open</A> method. This handle is used as the seventh parameter
to the wrapped CreateFile() API.

<DT><PRE>void <B><A NAME="LockRange">LockRange</A></B>( uint64_t position, uint64_t number_of_bytes_to_lock )</PRE><DD>
Locks the range of bytes in the file.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( LPCTSTR filename, UINT open_flags, CFileException * exception_p = nullptr )</PRE><DD>
Opens the file. Read CFile's documentation for the <CODE>open_flags</CODE> documentation.

<DT><PRE>DWORD <B><A NAME="Read">Read</A></B>( void * buffer, DWORD number_of_bytes_to_read )
DWORD <B>Read</B>( std::vector&lt;uint8_t&gt;&amp; buffer, DWORD number_of_bytes_to_read )</PRE><DD>
Reads data from the file. It will return the number of bytes read.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>DWORD <B><A NAME="ReadHuge">ReadHuge</A></B>( void * buffer, DWORD number_of_bytes_to_read )</PRE><DD>
Simpy calls <B>Read()</B>.

<DT><PRE>static void PASCAL <B><A NAME="Rename">Rename</A></B>( LPCTSTR old_name, LPCTSTR new_name )</PRE><DD>
Renames a file.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>static void PASCAL <B><A NAME="Remove">Remove</A></B>( LPCTSTR old_name, LPCTSTR new_name )</PRE><DD>
Renames a file.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>uint64_t <B><A NAME="Seek">Seek</A></B>( uint64_t offset, UINT from )</PRE><DD>
Seeks to a position in a file.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>void <B><A NAME="SeekToBegin">SeekToBegin</A></B>( void )</PRE><DD>
Seeks to the beginning of the file.

<DT><PRE>uint64_t <B><A NAME="SeekToEnd">SeekToEnd</A></B>( void )</PRE><DD>
Seeks to the end of the file.

<DT><PRE>void <B><A NAME="SetAttributes">SetAttributes</A></B>( DWORD attributes )</PRE><DD>
Sets the attributes used in the wrapped CreateFile() API. This value is used in
the sixth parameter to CreateFile().

<DT><PRE>BOOL <B><A NAME="SetEndOfFile">SetEndOfFile</A></B>( uint64_t length )</PRE><DD>
Sets the end of the file at the specified location.

<DT><PRE>void <B><A NAME="SetFilePath">SetFilePath</A></B>( LPCTSTR new_name )</PRE><DD>
Sets the file name.

<DT><PRE>void <B><A NAME="SetLength">SetLength</A></B>( uint64_t new_length )</PRE><DD>
Sets the end of the file at the specified location.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>static void PASCAL <B><A NAME="SetStatus">SetStatus</A></B>( LPCTSTR filename, const CFileStatus&amp; status )</PRE><DD>
Basically calls CFile's implementation of this function.

<DT><PRE>void <B><A NAME="SetTemplateFile">SetTemplateFile</A></B>( HANDLE template_file_handle )</PRE><DD>
Sets the handle to the file that will be used as a template in the
<A HREF="#Open">Open</A> method. This handle is used as the seventh parameter
to the wrapped CreateFile() API.

<DT><PRE>void <B><A NAME="UnlockRange">UnlockRange</A></B>( uint64_t position, uint64_t number_of_bytes_to_lock )</PRE><DD>
Unlocks a range of bytes in a file.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>void <B><A NAME="Write">Write</A></B>( const void * buffer, uint32_t number_of_bytes )</PRE><DD>
Writes the bytes to the file.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>void <B><A NAME="WriteHuge">WriteHuge</A></B>( const void * buffer, uint32_t number_of_bytes )</PRE><DD>
Writes the bytes to the file.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

<B>CFile64</B> file;

if ( file.<A HREF="#Open">Open</A>( command_line_arguments[ 1 ], CFile64::modeRead ) == FALSE )
{
return( EXIT_FAILURE );
}

file.<A HREF="#SetLength">SetLength</A>( _ttoi64( command_line_arguments[ 2 ] ) );

file.<A HREF="#Close">Close</A>();

return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>

<UL>
<LI>CreateFile
<LI>DeleteFile
<LI>DuplicateHandle
<LI>FindClose
<LI>FindFirstFile
<LI>FlushFileBuffers
<LI>GetCurrentProcess
<LI>GetFileSize
<LI>GetFileInformationByHandle
<LI>GetLastError
<LI>GetVolumeInformation
<LI>InitializeSecurityDescriptor
<LI>MoveFile
<LI>ReadFile
<LI>SetEndOfFile
<LI>SetFilePointerEx
<LI>SetLastError
<LI>SetSecurityDescriptorDacl
<LI>UnlockFile
<LI>WriteFile
<LI>ZeroMemory
</UL>

<HR><I><A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CFile64.cpp $<BR>
$Modtime: 6/26/01 10:46a $
</BODY>

</HTML>
*/
