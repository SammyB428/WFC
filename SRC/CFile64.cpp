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
** $Workfile: CFile64.cpp $
** $Revision: 17 $
** $Modtime: 6/26/01 10:46a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

#pragma warning( disable : 4302 )

// Helper functions

static inline constexpr _Check_return_ bool __is_directory_separation_character( _In_ wchar_t const character_to_test ) noexcept
{
    return( character_to_test == '\\' || character_to_test == '/' );
}

static inline constexpr _Check_return_ bool __is_wide_directory_separation_character( _In_ wchar_t const character_to_test ) noexcept
{
    return( character_to_test == L'\\' || character_to_test == L'/' );
}

static inline _Check_return_ UINT __GetFileName( _In_z_ LPCTSTR path_name_parameter, __out_ecount_opt( maximum_number_of_characters ) LPTSTR title_parameter, _In_ UINT const maximum_number_of_characters ) noexcept
{
    WFC_VALIDATE_POINTER( path_name_parameter );
    WFC_VALIDATE_POINTER_NULL_OK( title_parameter );

    // always capture the complete file name including extension (if present)
    LPTSTR temporary_writable_pointer = const_cast< LPTSTR >( path_name_parameter );

    LPCTSTR temporary_pointer = nullptr;

    for ( temporary_pointer = path_name_parameter; *temporary_pointer != '\0'; temporary_pointer = _tcsinc( temporary_pointer ) )
    {
        // remember last directory/drive separator
        if ( *temporary_pointer == '\\' || *temporary_pointer == '/' || *temporary_pointer == ':')
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
    (void) lstrcpyn( title_parameter, temporary_writable_pointer, maximum_number_of_characters );

    return( 0 );
}

static inline void __GetRoot( _In_z_ wchar_t const * path_name_parameter, _Inout_ std::wstring& root_string ) noexcept
{
    WFC_VALIDATE_POINTER( path_name_parameter );

    // determine the root name of the volume

    wchar_t root_name[ _MAX_PATH ];

    wchar_t * root_pointer = root_name;

    ZeroMemory( root_pointer, _MAX_PATH );

    (void) lstrcpynW( root_pointer, path_name_parameter, _MAX_PATH );

    wchar_t * temporary_pointer = nullptr;

    for ( temporary_pointer = root_pointer; *temporary_pointer != '\0'; temporary_pointer = _wcsinc( temporary_pointer ) )
    {
        // find first double slash and stop
        if ( __is_directory_separation_character( temporary_pointer[ 0 ] ) &&
            __is_directory_separation_character( temporary_pointer[ 1 ] ) )
        {
            break;
        }
    }

    if ( *temporary_pointer != '\0' )
    {
        // it is a UNC name, find second slash past '\\'
        ASSERT( __is_directory_separation_character( temporary_pointer[ 0 ] ) );
        ASSERT( __is_directory_separation_character( temporary_pointer[ 1 ] ) );

        temporary_pointer += 2;

        while ( *temporary_pointer != '\0' && ( ! __is_directory_separation_character( *temporary_pointer ) ) )
        {
            temporary_pointer = _wcsinc( temporary_pointer );
        }

        if ( *temporary_pointer != '\0' )
        {
            temporary_pointer = _wcsinc( temporary_pointer );
        }

        while ( *temporary_pointer != '\0' && ( ! __is_directory_separation_character( *temporary_pointer ) ) )
        {
            temporary_pointer = _wcsinc( temporary_pointer );
        }

        // terminate it just after the UNC root (ie. '\\server\share\')

        if ( *temporary_pointer != '\0' )
        {
            temporary_pointer[ 1 ] = '\0';
        }
    }
    else
    {
        // not a UNC, look for just the first slash
        temporary_pointer = root_pointer;

        while ( *temporary_pointer != '\0' && ( ! __is_directory_separation_character( *temporary_pointer ) ) )
        {
            temporary_pointer = _wcsinc( temporary_pointer );
        }

        // terminate it just after root (ie. 'x:\')
        if ( *temporary_pointer != '\0' )
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
    wchar_t * lpszFilePart = nullptr;

    if ( ! GetFullPathNameW( lpszFileIn, _MAX_PATH, lpszPathOut, &lpszFilePart ) )
    {
        (void) lstrcpyn(lpszPathOut, lpszFileIn, _MAX_PATH); // take it literally
        return( false );
    }

    std::wstring strRoot;

    // determine the root name of the volume
    __GetRoot(lpszPathOut, strRoot);

    // get file system information for the volume

    DWORD dwFlags = 0;
    DWORD dwDummy = 0;

    if ( ! GetVolumeInformationW(strRoot.c_str(), nullptr, 0, nullptr, &dwDummy, &dwFlags, nullptr, 0 ) )
    {
        return( false );   // preserving case may not be correct
    }

    // not all characters have complete uppercase/lowercase

    if ( ! ( dwFlags & FS_CASE_IS_PRESERVED ) )
    {
        CharUpper( lpszPathOut );
    }

    // assume non-UNICODE file systems, use OEM character set

    if ( ! ( dwFlags & FS_UNICODE_STORED_ON_DISK ) )
    {
        WIN32_FIND_DATA data;

        auto const h = FindFirstFile( lpszFileIn, &data );

        if ( h != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
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
    wchar_t * lpszTemp = title;

    UINT maximum_length = nMax;

    if (lpszTemp == nullptr)
    {
        lpszTemp = szTemp;
        maximum_length = static_cast<UINT>(std::size(szTemp));
    }

    if ( ::GetFileTitleW( path_name, lpszTemp, (WORD) maximum_length ) != 0 )
    {
        // when ::GetFileTitle fails, use cheap imitation
        return( __GetFileName( path_name, title, nMax ) );
    }

    return( ( title == nullptr ) ? _tcslen( lpszTemp ) + 1 : 0 );
}

void CFile64::CreatePathTo(_In_z_ LPCWSTR full_pathname) noexcept
{
    wchar_t drive[_MAX_DRIVE];
    wchar_t directory[_MAX_DIR];
    wchar_t filename[_MAX_FNAME];
    wchar_t extension_including_period[_MAX_EXT];

    ZeroMemory(drive, sizeof(drive));
    ZeroMemory(directory, sizeof(directory));
    ZeroMemory(filename, sizeof(filename));
    ZeroMemory(extension_including_period, sizeof(extension_including_period));

    if (_wsplitpath_s(full_pathname,
        drive, std::size(drive),
        directory, std::size(directory),
        filename, std::size(filename),
        extension_including_period, std::size(extension_including_period)) == 0)
    {
        if (directory[0] != 0x00 && directory[1] != '\\')
        {
            std::wstring full_directory_path( drive );

            full_directory_path.append(directory);

            CFileDirectory::Create(full_directory_path);
        }
    }
}

CFile64::CFile64() noexcept
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

CFile64::CFile64( _In_ HANDLE const file_handle ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    _ASSERTE(file_handle != INVALID_HANDLE_VALUE);
    _ASSERTE(file_handle != NULL);

    m_FileHandle           = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
    m_SecurityAttributes_p = nullptr;
    m_SecurityDescriptor_p = nullptr;
    m_SafeWriteCallback    = nullptr;
    m_SafeWriteContext     = nullptr;

    m_Initialize();

    m_FileHandle    = file_handle;
    m_CloseOnDelete = false;
}

CFile64::CFile64( _In_z_ LPCTSTR filename, _In_ UINT const open_flags ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( filename );

    m_FileHandle           = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
    m_SecurityAttributes_p = nullptr;
    m_SecurityDescriptor_p = nullptr;
    m_SafeWriteCallback    = nullptr;
    m_SafeWriteContext     = nullptr;

    m_Initialize();

    (void) Open( filename, open_flags );
}

CFile64::~CFile64() noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) && m_CloseOnDelete != false )
    {
        Close();
    }

    m_Uninitialize();

    m_SecurityAttributes_p = nullptr;
    m_SecurityDescriptor_p = nullptr;
    m_SafeWriteCallback    = nullptr;
    m_SafeWriteContext     = nullptr;
}

void CFile64::Abort( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        (void)Win32FoundationClasses::wfc_close_handle(m_FileHandle);
        m_FileHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
    }

    m_FileName.clear();
    m_PathName.clear();
    m_FileTitle.clear();
    m_CloseOnDelete = false;
}

void CFile64::Close( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        (void)Win32FoundationClasses::wfc_close_handle(m_FileHandle);
        m_FileHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
    }

    m_FileName.clear();
    m_PathName.clear();
    m_FileTitle.clear();
    m_CloseOnDelete = false;

#if ! defined( WFC_STL )

    if ( return_value == FALSE && m_BeStupidAndUseExceptions == true )
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

    if ( m_SecurityAttributes_p != nullptr )
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

    if ( m_SecurityDescriptor_p != nullptr )
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

    if ( dump_context.GetDepth() > 0 && m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        BY_HANDLE_FILE_INFORMATION information;

        ::GetFileInformationByHandle( m_FileHandle, &information );

        dump_context << TEXT( "\n   extended information from BY_HANDLE_FILE_INFORMATION\n   {\n" );
        dump_context << TEXT( "      dwFileAttributes     is " ) << information.dwFileAttributes << TEXT( " (" );

        std::wstring attributes;

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE )
        {
            attributes.assign(L"FILE_ATTRIBUTE_ARCHIVE" );
        }

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_COMPRESSED" );
        }

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_ENCRYPTED" );
        }

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_DIRECTORY" );
        }

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_HIDDEN" );
        }

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_OFFLINE" );
        }

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_READONLY )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_READONLY" );
        }

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_REPARSE_POINT" );
        }

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_SPARSE_FILE" );
        }

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_SYSTEM" );
        }

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_TEMPORARY" );
        }

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_NOT_CONTENT_INDEXED" );
        }

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_NORMAL )
        {
            if ( attributes.GetLength() > 0 )
            {
                attributes += TEXT( " | " );
            }

            attributes += TEXT( "FILE_ATTRIBUTE_NORMAL" );
        }

        if ( information.dwFileAttributes & FILE_ATTRIBUTE_DEVICE )
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

_Check_return_ CFile64 * CFile64::Duplicate( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        return( (CFile64 *) nullptr );
    }

    CFile64 * return_value = nullptr;

    WFC_TRY
    {
        return_value = new CFile64;
    }
    WFC_CATCH_ALL
    {
        return( nullptr );
    }
    WFC_END_CATCH_ALL

    auto duplicate_file_handle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );

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

void CFile64::Flush( void ) noexcept
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

_Check_return_ std::wstring CFile64::GetFileName( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( m_FileName );
}

_Check_return_ std::wstring CFile64::GetFilePath( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( m_PathName );
}

_Check_return_ std::wstring CFile64::GetFileTitle( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    // 2000-05-23
    // You pay an enourmous performance penalty for calling GetFilePath().
    // The children at Microsoft actually create a COM object and
    // read tons of registry settings when you call GetFilePath().
    // This is not a good thing when you are trying to be fast.
    // We will delay the penality as long as possible.

    if ( m_FileTitle.empty() == true && m_PathName.empty() == false )
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

_Check_return_ HANDLE CFile64::GetHandle( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( m_FileHandle );
}

_Check_return_ bool CFile64::GetInformation( _Inout_ BY_HANDLE_FILE_INFORMATION& information ) const noexcept
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

_Check_return_ uint64_t CFile64::GetLength( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    LARGE_INTEGER length = { 0, 0 };

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

_Check_return_ uint64_t CFile64::GetPosition( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        return( (uint64_t) -1 );
    }

    LARGE_INTEGER return_value;
    LARGE_INTEGER zero = { 0, 0 };

    return_value.QuadPart = 0;

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

_Check_return_ SECURITY_ATTRIBUTES * CFile64::GetSecurityAttributes( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( m_SecurityAttributes_p );
    return( m_SecurityAttributes_p );
}

_Check_return_ SECURITY_DESCRIPTOR * CFile64::GetSecurityDescriptor( void ) const noexcept
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

    if ( information.nFileSizeHigh != 0 )
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

_Check_return_ BOOL PASCAL CFile64::GetStatus( _In_z_ LPCTSTR filename, __out CFileStatus& status )
{
    WFC_VALIDATE_POINTER( filename );

    if ( ::__FullPath( status.m_szFullName, filename ) == FALSE )
    {
        status.m_szFullName[ 0 ] = TEXT( '\0' );
        return( FALSE );
    }

    WIN32_FIND_DATA find_data;

    auto const find_handle = ::FindFirstFile( (LPTSTR) filename, &find_data );

    if ( find_handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        return( FALSE );
    }

    FindClose( find_handle );

    status.m_attribute = (BYTE) ( find_data.dwFileAttributes & ~FILE_ATTRIBUTE_NORMAL );

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

void CFile64::m_Initialize( void )
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

        m_SecurityDescriptor_p = wfc_create_null_dacl();

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
void CFile64::SetSecurity(_In_z_ wchar_t const * sddl) noexcept
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER(sddl);
    _ASSERTE(sddl[0] != 0x00);

    if (sddl == nullptr || sddl[0] == 0x00)
    {
        return;
    }

    if (m_SecurityAttributes_p != nullptr)
    {
        delete m_SecurityAttributes_p;
        m_SecurityAttributes_p = nullptr;
    }

    if (m_SecurityDescriptor_p != nullptr)
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

    m_SecurityDescriptor_p = (SECURITY_DESCRIPTOR *) m_SecurityAttributes_p->lpSecurityDescriptor;
}

void CFile64::m_Uninitialize( void )
{
    WFC_VALIDATE_POINTER( this );

    if ( m_SecurityAttributes_p != nullptr )
    {
        delete m_SecurityAttributes_p;
        m_SecurityAttributes_p = nullptr;
    }

    if ( m_SecurityDescriptor_p != nullptr )
    {
        wfc_destroy_null_dacl( m_SecurityDescriptor_p );
        m_SecurityDescriptor_p = nullptr;
    }
}

_Check_return_ bool CFile64::LockRange( _In_ uint64_t const position, _In_ uint64_t const number_of_bytes_to_lock ) noexcept
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

_Check_return_ bool CFile64::Open( _In_z_ LPCTSTR filename, _In_ UINT const open_flags_parameter ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( filename );

    Close();

    UINT open_flags = open_flags_parameter;

    WFC_TRY
    {
        m_FileName.assign( filename );

        TCHAR full_path[_MAX_PATH]{ 0 };

        (void) ::__FullPath( full_path, filename );

        m_PathName.assign( full_path );

        m_FileTitle.clear ();

        open_flags &= ~ (UINT) OpenFlags::typeBinary;

        DWORD access = 0;

        switch ( open_flags & 3 )
        {
        case (UINT) OpenFlags::modeRead:

            access = GENERIC_READ;
            break;

        case (UINT) OpenFlags::modeWrite:

            access = GENERIC_WRITE;
            break;

        case (UINT) OpenFlags::modeReadWrite:

            access = GENERIC_READ | GENERIC_WRITE;
            break;

        default:

            ASSERT( FALSE );  // invalid share mode
        }

        DWORD share_mode = 0;

        switch ( static_cast<OpenFlags>(open_flags & 0x70) )    // map compatibility mode to exclusive
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

            share_mode = FILE_SHARE_WRITE|FILE_SHARE_READ;
            break;

        default:

            ASSERT( FALSE );  // invalid share mode?
        }

        if (is_flagged(open_flags, (UINT)OpenFlags::shareAllowDelete))
        {
            share_mode |= FILE_SHARE_DELETE;
        }

        if ( m_SecurityAttributes_p != nullptr )
        {
            m_SecurityAttributes_p->bInheritHandle = ( ( open_flags & (UINT) OpenFlags::modeNoInherit ) == 0 ) ? TRUE : FALSE;
        }

        DWORD creation_flags = 0;

        if ( open_flags & (UINT) OpenFlags::modeCreate )
        {
            if ( open_flags & (UINT) OpenFlags::modeNoTruncate )
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

        if ( open_flags & (UINT) OpenFlags::osNoBuffer )
        {
            m_Attributes |= FILE_FLAG_NO_BUFFERING;
        }

        if ( open_flags & (UINT) OpenFlags::osRandomAccess )
        {
            m_Attributes |= FILE_FLAG_RANDOM_ACCESS;
        }

        if ( open_flags & (UINT) OpenFlags::osSequentialScan )
        {
            m_Attributes |= FILE_FLAG_SEQUENTIAL_SCAN;
        }

        if ( open_flags & (UINT) OpenFlags::osWriteThrough )
        {
            m_Attributes |= FILE_FLAG_WRITE_THROUGH;
        }

        if ( open_flags & (UINT) OpenFlags::wfcDeleteOnClose )
        {
            m_Attributes |= FILE_FLAG_DELETE_ON_CLOSE;
        }

        m_FileHandle = ::CreateFile( filename,
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

            if ( exception_p != nullptr )
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

_Check_return_ bool CFile64::OpenWide( _In_z_ wchar_t const * unicode_filename, _In_ UINT const open_flags_parameter ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( unicode_filename );

    Close();

    UINT open_flags = open_flags_parameter;

    WFC_TRY
    {
        m_FileName.assign( unicode_filename );

        wchar_t unicode_full_path[ _MAX_PATH ];

        (void) ::__FullPath( unicode_full_path, unicode_filename );

        m_PathName.assign( unicode_full_path );

        m_FileTitle.clear();

        open_flags &= ~ (UINT) OpenFlags::typeBinary;

        DWORD access = 0;

        switch ( (OpenFlags)(open_flags & 3 ))
        {
        case OpenFlags::modeRead:

            access = GENERIC_READ;
            break;

        case OpenFlags::modeWrite:

            access = GENERIC_WRITE;
            break;

        case OpenFlags::modeReadWrite:

            access = GENERIC_READ | GENERIC_WRITE;
            break;

        default:

        _ASSERT_EXPR( FALSE, _CRT_WIDE( "Invalid flags." ) );
        }

        DWORD share_mode = 0;

        switch ( (OpenFlags)(open_flags & 0x70))    // map compatibility mode to exclusive
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

            share_mode = FILE_SHARE_WRITE|FILE_SHARE_READ;
            break;

        default:

        _ASSERT_EXPR( FALSE, _CRT_WIDE( "Invalid share mode." ) );
        }

        if ( m_SecurityAttributes_p != nullptr )
        {
            m_SecurityAttributes_p->bInheritHandle = ( ( open_flags & (UINT) OpenFlags::modeNoInherit ) == 0 ) ? TRUE : FALSE;
        }

        DWORD creation_flags = 0;

        if ( open_flags & (UINT) OpenFlags::modeCreate )
        {
            if ( open_flags & (UINT) OpenFlags::modeNoTruncate )
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

        if ( open_flags & (UINT)OpenFlags::osNoBuffer )
        {
            m_Attributes |= FILE_FLAG_NO_BUFFERING;
        }

        if ( open_flags & (UINT)OpenFlags::osRandomAccess )
        {
            m_Attributes |= FILE_FLAG_RANDOM_ACCESS;
        }

        if ( open_flags & (UINT)OpenFlags::osSequentialScan )
        {
            m_Attributes |= FILE_FLAG_SEQUENTIAL_SCAN;
        }

        if ( open_flags & (UINT) OpenFlags::osWriteThrough )
        {
            m_Attributes |= FILE_FLAG_WRITE_THROUGH;
        }

        if ( open_flags & (UINT)OpenFlags::wfcDeleteOnClose )
        {
            m_Attributes |= FILE_FLAG_DELETE_ON_CLOSE;
        }

        m_FileHandle = ::CreateFileW( unicode_filename,
            access,
            share_mode,
            GetSecurityAttributes(),
            creation_flags,
            GetAttributes(),
            GetTemplateFile() );

        if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
        {
            m_LastError = ::GetLastError();
            //WFCTRACEERROR(m_LastError);

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

_Check_return_ bool CFile64::Read(_Inout_ std::wstring& string_from_file) noexcept
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

_Check_return_ bool CFile64::Read( _Inout_ std::string& string_from_file ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    string_from_file.clear();

    BYTE byte_from_file = 0x00;

    bool exit_loop = false;

    while( exit_loop == false )
    {
        if ( Read( &byte_from_file, 1 ) != 1 )
        {
            return( ( string_from_file.length() > 0 ) ? true : false );
        }

        if ( byte_from_file == 0x00 ||
            byte_from_file == LINE_FEED )
        {
            return( true );
        }

        if ( byte_from_file == CARRIAGE_RETURN )
        {
            // See if next character is  a line feed...

            uint64_t current_position = GetPosition();

            if ( Read( &byte_from_file, 1 ) != 1 )
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

_Check_return_ bool CFile64::Read( _Inout_ std::vector<std::string>& array ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    std::string string_from_file;

    while( Read( string_from_file ) != false )
    {
        (void) array.push_back( string_from_file );
    }

    return( true );
}

_Check_return_ bool CFile64::Read(_Inout_ std::vector<std::wstring>& array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    std::wstring string_from_file;

    while (Read(string_from_file) != false)
    {
        array.push_back(string_from_file);
    }

    return(true);
}

_Check_return_ bool CFile64::Read(_Inout_ std::vector<uint8_t>& bytes, _In_ uint32_t const number_of_bytes_to_read) noexcept
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

_Check_return_ uint32_t CFile64::Read( __out_bcount( number_of_bytes_to_read ) void * buffer, _In_ uint32_t const number_of_bytes_to_read ) noexcept
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

    DWORD number_of_bytes_read = 0;

    if ( ::ReadFile( m_FileHandle, buffer, number_of_bytes_to_read, &number_of_bytes_read, nullptr ) == FALSE )
    {
        m_LastError = ::GetLastError();
    }

    return( number_of_bytes_read );
}

_Check_return_ uint32_t CFile64::AtomicRead( _In_ uint64_t const file_offset, __out_bcount( number_of_bytes_to_read ) void * buffer, _In_ uint32_t const number_of_bytes_to_read ) const noexcept
{
    //WFCTRACEVAL( TEXT( "Reading " ), number_of_bytes_to_read );
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( buffer );

#if defined( _DEBUG )
    if ( number_of_bytes_to_read == 1 )
    {
        WFC_COVERAGE( 29 );
    }
#endif // _DEBUG

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

    OVERLAPPED overlapped;
    // Use the Union magic to set the file offset to read in a single assignment rather than convert to ULARGE_INTEGER.

    overlapped.Pointer      = reinterpret_cast<PVOID>(file_offset);
    overlapped.hEvent       = static_cast< HANDLE >( NULL );
    overlapped.Internal     = 0;
    overlapped.InternalHigh = 0;

    DWORD number_of_bytes_read = 0;

    if ( ::ReadFile( m_FileHandle, buffer, number_of_bytes_to_read, &number_of_bytes_read, &overlapped ) == FALSE )
    {
#if defined( _DEBUG )
        uint32_t last_error = ::GetLastError();

        LARGE_INTEGER file_size{ 0, 0 };

        _ASSERTE(::GetFileSizeEx(m_FileHandle, &file_size) != 0);
        _ASSERTE((LONGLONG)(file_offset + number_of_bytes_to_read) <= file_size.QuadPart);
#endif
        //_ASSERT_EXPR(FALSE, _CRT_WIDE("ReadFile returned FALSE"));
    }

    //_ASSERTE(number_of_bytes_read > 0);

    return( number_of_bytes_read );
}

_Check_return_ uint32_t CFile64::AtomicWrite(_In_ uint64_t const file_offset, __in_bcount(number_of_bytes_to_write) void const * buffer, _In_ uint32_t const number_of_bytes_to_write) const noexcept
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
    overlapped.hEvent = static_cast< HANDLE >(NULL);
    overlapped.Internal = 0;
    overlapped.InternalHigh = 0;

    DWORD number_of_bytes_read = 0;

    // Unfortunately, atomic writes don't work. They reset the file pointer to the offset
    // after the last byte of the write. DOH! Halfway useful...
    uint64_t const current_file_position = GetPosition();

    if (::WriteFile(m_FileHandle, buffer, (DWORD) number_of_bytes_to_write, &number_of_bytes_read, &overlapped) == FALSE)
    {
        //m_LastError = ::GetLastError();
        //WFCTRACEERROR( ::GetLastError() );
    }

    LARGE_INTEGER return_value = { 0, 0 };
    LARGE_INTEGER distance_to_move;

    distance_to_move.QuadPart = current_file_position;

    ::SetFilePointerEx(m_FileHandle, distance_to_move, &return_value, FILE_BEGIN);

    return(number_of_bytes_read);
}

_Check_return_ uint32_t CFile64::ReadHuge( __out_bcount( number_of_bytes_to_read ) void * buffer, _In_ uint32_t const number_of_bytes_to_read ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( buffer );
    return( Read( buffer, number_of_bytes_to_read ) );
}

void PASCAL CFile64::Rename( _In_z_ LPCTSTR old_name, _In_z_ LPCTSTR new_name ) noexcept // static
{
    WFC_VALIDATE_POINTER( old_name );
    WFC_VALIDATE_POINTER( new_name );

    if ( ::MoveFile( (LPTSTR) old_name, (LPTSTR) new_name ) == FALSE )
    {
    }
}

void PASCAL CFile64::Remove( _In_z_ LPCTSTR filename ) noexcept // static
{
    WFC_VALIDATE_POINTER( filename );

    if ( ::DeleteFile( (LPTSTR) filename ) == FALSE )
    {
        CFile64 file;

        // Try to delete the file with DELETE ON CLOSE semantics.

        if (file.Open(filename, (UINT)((UINT)CFile64::OpenFlags::modeRead | (UINT)CFile64::OpenFlags::shareDenyNone | (UINT)CFile64::OpenFlags::wfcDeleteOnClose)) != false)
        {
            file.Close();
        }
    }
}

_Check_return_ uint64_t CFile64::Seek( _In_ int64_t const offset, _In_ SeekPosition const from ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        return( (uint64_t) -1 );
    }

    DWORD move_method = 0;

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

        return( (uint64_t) -1 );
    }

    LARGE_INTEGER return_value = { 0, 0 };
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

void CFile64::SeekToBegin( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    (void) Seek( (int64_t) 0, SeekPosition::begin );
}

_Check_return_ uint64_t CFile64::SeekToEnd( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( Seek( (int64_t) 0, SeekPosition::end ) );
}

_Check_return_ bool CFile64::SetEndOfFile( _In_ uint64_t const length ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    (void) Seek( length, SeekPosition::begin );

    _ASSERTE( GetPosition() == length );

    if ( ::SetEndOfFile( m_FileHandle ) == FALSE )
    {
        m_LastError = ::GetLastError();
        //WFCTRACEERROR( ::GetLastError() );
        return( false );
    }

    return( true );
}

void CFile64::SetFilePath( _In_z_ LPCTSTR new_name ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( new_name );
    m_FileName.assign( new_name );
}

void CFile64::SetLength( _In_ uint64_t const new_length ) noexcept
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

_Check_return_ bool CFile64::SetPosition( _In_ uint64_t const position ) noexcept
{
    return( Seek( position, SeekPosition::begin ) == position ? true : false );
}

#if ! defined( WFC_STL )

void PASCAL CFile64::SetStatus( LPCTSTR filename, CFileStatus& const status )
{
    CFile::SetStatus( filename, status );
}

#endif // WFC_STL

#if ! defined( FSCTL_SET_ZERO_DATA )
#define FSCTL_SET_ZERO_DATA             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 50, METHOD_BUFFERED, FILE_WRITE_DATA) // FILE_ZERO_DATA_INFORMATION,
#endif

// WFC version of the FILE_ZERO_DATA_INFORMATION structure
typedef struct _wfc_sparse_region
{
    LARGE_INTEGER file_offset;
    LARGE_INTEGER beyond_final_zero;
}
WFC_SPARSE_REGION, *WFC_SPARSE_REGION_P;

_Check_return_ bool CFile64::SparsifyRegion( _In_ uint64_t const position, _In_ uint64_t const number_of_bytes_to_mark_as_sparse ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    WFC_SPARSE_REGION region_to_sparsify;

    region_to_sparsify.file_offset.QuadPart = position;
    region_to_sparsify.beyond_final_zero.QuadPart = position;
    region_to_sparsify.beyond_final_zero.QuadPart += number_of_bytes_to_mark_as_sparse;
    region_to_sparsify.beyond_final_zero.QuadPart++;

    OVERLAPPED overlapped;

    ZeroMemory( &overlapped, sizeof( overlapped ) );

    DWORD number_of_bytes_returned = 0;

    BOOL return_value = ::DeviceIoControl( m_FileHandle,
        FSCTL_SET_ZERO_DATA,
        &region_to_sparsify,
        sizeof( region_to_sparsify ),
        nullptr,
        0,
        &number_of_bytes_returned,
        &overlapped );

    if ( return_value != 0 )
    {
        return( true );
    }

    return( false );
}

void CFile64::UnlockRange( _In_ uint64_t const position, _In_ uint64_t const number_of_bytes_to_unlock ) noexcept
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

_Check_return_ bool CFile64::ProcessContent( _In_ std::size_t const step_size, _Inout_ PROCESS_BUFFER_CALLBACK function_to_call, __inout_opt void * callback_context ) const noexcept
{
    if ( function_to_call == nullptr )
    {
        return( false );
    }

    auto buffer = reinterpret_cast<uint8_t *>(_aligned_malloc( step_size, 4096 ));

    if ( buffer == nullptr )
    {
        return( false );
    }

    int64_t const number_of_bytes_to_process = GetLength();
    int64_t number_of_bytes_processed = 0;

    while( number_of_bytes_processed < number_of_bytes_to_process )
    {
        int64_t number_of_bytes_to_process_in_this_call = number_of_bytes_to_process - number_of_bytes_processed;

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
        if ( AtomicRead( static_cast<uint64_t const>(number_of_bytes_processed), (void *) buffer, (uint32_t const) number_of_bytes_to_process_in_this_call ) != number_of_bytes_to_process_in_this_call)
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

void CFile64::RegisterSafeWrite( _Inout_ void * context, __callback SAFE_WRITE_CALLBACK callback ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( context );
    WFC_VALIDATE_POINTER_NULL_OK( callback );

    m_SafeWriteContext = context;
    m_SafeWriteCallback = callback;
}

void CFile64::Write( __in_bcount( number_of_bytes_to_write ) void const * buffer, _In_ UINT const number_of_bytes_to_write ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( number_of_bytes_to_write == 0 )
    {
        //WFCTRACE( TEXT( "No bytes to write" ) );
        return;
    }

#if defined( _DEBUG )

    std::wstring debug_message;

    uint64_t file_position = GetPosition();

    format( debug_message, L"Writing at offset %I64d", (int64_t) file_position );

    //WFCTRACE( debug_message );

#endif

    WFC_VALIDATE_POINTER( buffer );

    DWORD number_of_bytes_written = 0;

    while ( ::WriteFile( m_FileHandle, buffer, number_of_bytes_to_write, &number_of_bytes_written, nullptr ) == FALSE )
    {
        DWORD error_code = ::GetLastError();
        //WFCTRACEERROR( error_code );

        if ( m_SafeWriteCallback != nullptr )
        {
            int const return_value = m_SafeWriteCallback( m_SafeWriteContext, error_code, m_FileName.c_str() );

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
    if ( number_of_bytes_written != number_of_bytes_to_write )
    {
        if ( m_BeStupidAndUseExceptions == true )
        {
            ::AfxThrowFileException( CFileException::diskFull, -1, m_FileName );
        }
    }
#endif // WFC_STL
}

void CFile64::WriteHuge( __in_bcount( number_of_bytes_to_write ) void const * buffer, _In_ DWORD const number_of_bytes_to_write ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( buffer );
    Write( buffer, number_of_bytes_to_write );
}

void CFile64::Write( _In_ std::wstring const& string_to_write ) noexcept
{
    Write( string_to_write.c_str(), static_cast<UINT>(string_to_write.length() * sizeof( wchar_t ) ) );
}

void CFile64::Write( _In_ std::string const& string_to_write ) noexcept
{
    Write( string_to_write.c_str(), static_cast<UINT>(string_to_write.length()) );
}

void CFile64::Write( _In_ std::vector<std::string> const& strings, _In_ bool const include_end_of_lines ) noexcept
{
    for ( auto const& entry : strings )
    {
        Write( entry );

        if ( include_end_of_lines != false )
        {
            uint8_t end_of_line[ 2 ] = { CARRIAGE_RETURN, LINE_FEED };
            Write( end_of_line, 2 );
        }
    }
}

void CFile64::Write(_In_ std::vector<std::wstring> const& strings, _In_ bool const include_end_of_lines) noexcept
{
    for( auto const& entry : strings )
    {
        Write(entry.c_str(), static_cast<UINT>(entry.length() * sizeof(wchar_t)));

        if (include_end_of_lines != false)
        {
            wchar_t end_of_line[2] = { CARRIAGE_RETURN, LINE_FEED };
            Write(end_of_line, 2);
        }
    }
}

_Check_return_ bool CFile64::SetShortName( _In_z_ LPCTSTR new_short_name )  noexcept// New for 73
{
    return( SetFileShortName( m_FileHandle, new_short_name ) == 0 ? false : true );
}

_Check_return_ bool CFile64::SetValidData( _In_ int64_t const valid_data_length )  noexcept// New for 73
{
    int return_value = SetFileValidData( m_FileHandle, valid_data_length );

    if ( return_value == 0 )
    {
        uint32_t const last_error = ::GetLastError();

        if ( last_error == ERROR_PRIVILEGE_NOT_HELD )
        {
            if ( Win32FoundationClasses::wfc_enable_privilege( SE_MANAGE_VOLUME_NAME ) != false )
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
