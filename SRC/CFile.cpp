/*
** Author: Samuel R. Blackburn
**
** $Workfile: CFile.cpp $
** $Revision: 13 $
** $Modtime: 2/21/01 5:02a $
** $Reuse Tracing Code: 1001 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

#if defined( WFC_STL )

// Present only in STL builds

#if ! defined( DIMENSION_OF )
#define DIMENSION_OF( argument ) ( sizeof( argument ) / sizeof( *( argument ) ) )
#endif

// Helper functions

static inline __checkReturn bool __is_directory_separation_character( __in TCHAR temp_pointer )
{
   return( temp_pointer == '\\' || temp_pointer == '/' );
}

static inline UINT __GetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
{
   WFC_VALIDATE_POINTER( lpszPathName );
   WFC_VALIDATE_POINTER( lpszTitle );

   // always capture the complete file name including extension (if present)
   LPTSTR lpszTemp = (LPTSTR)lpszPathName;

   LPCTSTR temp_pointer = nullptr;

   for ( temp_pointer = lpszPathName; *temp_pointer != '\0'; temp_pointer = _tcsinc( temp_pointer ) )
   {
      // remember last directory/drive separator
      if (*temp_pointer == '\\' || *temp_pointer == '/' || *temp_pointer == ':')
      {
         lpszTemp = (LPTSTR) _tcsinc( temp_pointer );
      }
   }

   // lpszTitle can be NULL which just returns the number of bytes
   if (lpszTitle == nullptr)
   {
      return( lstrlen( lpszTemp ) + 1 );
   }

   // otherwise copy it into the buffer provided
   lstrcpyn( lpszTitle, lpszTemp, nMax );

   return( 0 );
}

static inline void __GetRoot( LPCTSTR path_name, CString& root_string )
{
   WFC_VALIDATE_POINTER( path_name );

   TCHAR root_name[ _MAX_PATH ];

   LPTSTR root_pointer = root_name;

   ZeroMemory( root_pointer, _MAX_PATH );

   lstrcpyn( root_pointer, path_name, _MAX_PATH );

   LPTSTR temp_pointer = nullptr;

   for ( temp_pointer = root_name; *temp_pointer != '\0'; temp_pointer = _tcsinc( temp_pointer ) )
   {
      // find first double slash and stop
      if ( __is_directory_separation_character( temp_pointer[ 0 ] ) &&
           __is_directory_separation_character( temp_pointer[ 1 ] ) )
      {
         break;
      }
   }

   if ( *temp_pointer != '\0' )
   {
      // it is a UNC name, find second slash past '\\'
      ASSERT( __is_directory_separation_character( temp_pointer[ 0 ] ) );
      ASSERT( __is_directory_separation_character( temp_pointer[ 1 ] ) );

      temp_pointer += 2;

      while ( *temp_pointer != '\0' && ( ! __is_directory_separation_character( *temp_pointer ) ) )
      {
         temp_pointer = _tcsinc( temp_pointer );
      }

      if ( *temp_pointer != '\0' )
      {
         temp_pointer = _tcsinc( temp_pointer );
      }

      while ( *temp_pointer != '\0' && ( ! __is_directory_separation_character( *temp_pointer ) ) )
      {
         temp_pointer = _tcsinc( temp_pointer );
      }

      // terminate it just after the UNC root (ie. '\\server\share\')

      if ( *temp_pointer != '\0' )
      {
         temp_pointer[ 1 ] = '\0';
      }
   }
   else
   {
      // not a UNC, look for just the first slash
      temp_pointer = root_pointer;

      while ( *temp_pointer != '\0' && ( ! __is_directory_separation_character( *temp_pointer ) ) )
      {
         temp_pointer = _tcsinc( temp_pointer );
      }

      // terminate it just after root (ie. 'x:\')
      if ( *temp_pointer != '\0' )
      {
         temp_pointer[ 1 ] = '\0';
      }
   }

   root_string = root_name;
}

BOOL __FullPath( LPTSTR path_out, LPCTSTR file_in )
{
   WFC_VALIDATE_POINTER( path_out );
   WFC_VALIDATE_POINTER( file_in );

   // first, fully qualify the path name
   LPTSTR file_part = nullptr;

   if ( ! GetFullPathName( file_in, _MAX_PATH, path_out, &file_part ) )
   {
      lstrcpyn( path_out, file_in, _MAX_PATH ); // take it literally
      return( FALSE );
   }

   CString strRoot;

   // determine the root name of the volume
   __GetRoot(path_out, strRoot);

   // get file system information for the volume

   DWORD dwFlags = 0;
   DWORD dwDummy = 0;

   if ( ! GetVolumeInformation(strRoot, NULL, 0, NULL, &dwDummy, &dwFlags, NULL, 0 ) )
   {
      return( FALSE );   // preserving case may not be correct
   }

   // not all characters have complete uppercase/lowercase

   if ( ! ( dwFlags & FS_CASE_IS_PRESERVED ) )
   {
      CharUpper( path_out );
   }

   // assume non-UNICODE file systems, use OEM character set

   if ( ! ( dwFlags & FS_UNICODE_STORED_ON_DISK ) )
   {
      WIN32_FIND_DATA data;

      HANDLE h = FindFirstFile( file_in, &data );

      if ( h != (HANDLE) INVALID_HANDLE_VALUE )
      {
         FindClose( h );
         lstrcpy( file_part, data.cFileName );
      }
   }

   return( TRUE );
}

UINT __GetFileTitle(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
{
   WFC_VALIDATE_POINTER( lpszPathName );
   WFC_VALIDATE_POINTER( lpszTitle );

   // use a temporary to avoid bugs in ::GetFileTitle when lpszTitle is NULL
   TCHAR temp_string[ _MAX_PATH ];

   LPTSTR lpszTemp = lpszTitle;

   if (lpszTemp == nullptr)
   {
      lpszTemp = temp_string;
      nMax = DIMENSION_OF( temp_string );
   }

   // 2000-05-23
   // There is a huge performance penality for calling GetFileTitle().
   // You should avoid it at all costs.

   if ( ::GetFileTitle( lpszPathName, lpszTemp, (WORD) nMax ) != 0 )
   {
      // when ::GetFileTitle fails, use cheap imitation
      return ( ::__GetFileName( lpszPathName, lpszTitle, nMax ) );
   }

   return lpszTitle == nullptr ? ::lstrlen( lpszTemp ) + 1 : 0;
}

CFile::CFile()
{
   WFCLTRACEINIT( TEXT( "CFile::CFile()" ) );
   WFC_VALIDATE_POINTER( this );

   m_FileHandle           = (HANDLE) INVALID_HANDLE_VALUE;
   m_SecurityAttributes_p = (SECURITY_ATTRIBUTES *) NULL;
   m_SecurityDescriptor_p = (SECURITY_DESCRIPTOR *) NULL;

   m_Initialize();
}

CFile::CFile( int file_handle )
{
   WFCLTRACEINIT( TEXT( "CFile::CFile( int )" ) );
   WFC_VALIDATE_POINTER( this );

   m_FileHandle           = (HANDLE) INVALID_HANDLE_VALUE;
   m_SecurityAttributes_p = (SECURITY_ATTRIBUTES *) NULL;
   m_SecurityDescriptor_p = (SECURITY_DESCRIPTOR *) NULL;

   m_Initialize();

   m_hFile         = (CFILE_HFILE) file_handle; // Stupid public member that is never used internally
   m_FileHandle    = (HANDLE) file_handle;
   m_CloseOnDelete = FALSE;
}

CFile::CFile( LPCTSTR filename, UINT open_flags )
{
   WFCLTRACEINIT( TEXT( "CFile::CFile( LPCTSTR )" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( filename );

   m_FileHandle           = (HANDLE) INVALID_HANDLE_VALUE;
   m_SecurityAttributes_p = (SECURITY_ATTRIBUTES *) NULL;
   m_SecurityDescriptor_p = (SECURITY_DESCRIPTOR *) NULL;

   m_Initialize();

   if ( Open( filename, open_flags ) == FALSE )
   {
      //AfxThrowFileException( exception.m_cause, exception.m_lOsError, exception.m_strFileName );
   }
}

CFile::~CFile()
{
   WFCLTRACEINIT( TEXT( "CFile::~CFile()" ) );
   WFC_VALIDATE_POINTER( this );

   if ( m_FileHandle != (HANDLE) INVALID_HANDLE_VALUE && m_CloseOnDelete != FALSE )
   {
      Close();
   }

   m_Uninitialize();

   m_SecurityAttributes_p = (SECURITY_ATTRIBUTES *) NULL;
   m_SecurityDescriptor_p = (SECURITY_DESCRIPTOR *) NULL;
}

void CFile::Abort( void )
{
   WFCLTRACEINIT( TEXT( "CFile::Abort()" ) );
   WFC_VALIDATE_POINTER( this );

   BOOL return_value = TRUE;

   if ( m_FileHandle != (HANDLE) INVALID_HANDLE_VALUE )
   {
      if ( Win32FoundationClasses::wfc_close_handle( m_FileHandle ) != 0 )
      {
         return_value = FALSE;
      }

      m_FileHandle = (HANDLE) INVALID_HANDLE_VALUE;
   }

   m_FileName.Empty();
   m_PathName.Empty();
   m_FileTitle.Empty();
   m_CloseOnDelete = FALSE;

   // Invalidate that stupid public attribute

   m_hFile = (CFILE_HFILE) hFileNull;
}

void CFile::Close( void )
{
   WFCLTRACEINIT( TEXT( "CFile::Close()" ) );
   WFC_VALIDATE_POINTER( this );

   BOOL return_value = TRUE;

   if ( m_FileHandle != (HANDLE) INVALID_HANDLE_VALUE )
   {
      if ( Win32FoundationClasses::wfc_close_handle( m_FileHandle ) != 0 )
      {
         return_value = FALSE;
      }

      m_FileHandle = (HANDLE) INVALID_HANDLE_VALUE;
   }

   m_FileName.Empty();
   m_PathName.Empty();
   m_FileTitle.Empty();
   m_CloseOnDelete = FALSE;

   // Invalidate that stupid public attribute

   m_hFile = (CFILE_HFILE) hFileNull;
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CFile::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   dump_context << TEXT( "a CFile at " ) << (VOID *) this << TEXT( "\n{\n" );

   dump_context << TEXT( "   m_PathName             is \"" ) << m_PathName  << TEXT( "\"\n" );
   dump_context << TEXT( "   m_FileName             is \"" ) << m_FileName  << TEXT( "\"\n" );
   dump_context << TEXT( "   m_FileTitle            is \"" ) << m_FileTitle << TEXT( "\"\n" );
   dump_context << TEXT( "   m_CloseOnDelete        is "   );

   if ( m_CloseOnDelete == FALSE )
   {
      dump_context << TEXT( "FALSE" );
   }
   else
   {
      dump_context << TEXT( "TRUE" );
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

   if ( m_FileHandle == (HANDLE) INVALID_HANDLE_VALUE )
   {
      dump_context << TEXT( "INVALID_HANDLE_VALUE" );
   }
   else
   {
      dump_context << m_FileHandle;
   }

   dump_context << TEXT( "\n" );

   if ( dump_context.GetDepth() > 0 && m_FileHandle != (HANDLE) INVALID_HANDLE_VALUE )
   {
      BY_HANDLE_FILE_INFORMATION information;

      ::GetFileInformationByHandle( m_FileHandle, &information );

      dump_context << TEXT( "\n   extended information from BY_HANDLE_FILE_INFORMATION\n   {\n" );
      dump_context << TEXT( "      dwFileAttributes     is " ) << information.dwFileAttributes << TEXT( " (" );

      CString attributes;

      if ( information.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE )
      {
         attributes = TEXT( "FILE_ATTRIBUTE_ARCHIVE" );
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

CFile * CFile::Duplicate( void ) const
{
   WFCLTRACEINIT( TEXT( "CFile::Duplicate()" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_COVERAGE( 2 );

   if ( m_FileHandle == (HANDLE) INVALID_HANDLE_VALUE )
   {
      return( (CFile *) nullptr );
   }

   CFile * return_value = nullptr;
   
   WFC_TRY
   {
      return_value = new CFile;
   }
   WFC_CATCH_ALL
   {
      return( NULL );
   }
   WFC_END_CATCH_ALL

   HANDLE duplicate_file_handle = (HANDLE) INVALID_HANDLE_VALUE;

   if ( ::DuplicateHandle( ::GetCurrentProcess(), m_FileHandle, ::GetCurrentProcess(), &duplicate_file_handle, 0, FALSE, DUPLICATE_SAME_ACCESS ) == FALSE )
   {
      WFCTRACEERROR( ::GetLastError() );

      delete return_value;
      return_value = nullptr;
   }
   else
   {
      return_value->m_hFile         = (CFILE_HFILE) duplicate_file_handle; // Stupid public attribute that will never be used internally
      return_value->m_FileHandle    = duplicate_file_handle;
      return_value->m_CloseOnDelete = m_CloseOnDelete;
      return_value->m_PathName      = m_PathName;
      return_value->m_FileName      = m_FileName;
      return_value->m_FileTitle     = m_FileTitle;
   }

   return( return_value );
}

void CFile::Flush( void )
{
   WFCLTRACEINIT( TEXT( "CFile::Flush()" ) );
   WFC_VALIDATE_POINTER( this );

   if ( m_FileHandle == (HANDLE) INVALID_HANDLE_VALUE )
   {
      return;
   }

   if ( ::FlushFileBuffers( m_FileHandle ) == FALSE )
   {
      WFCTRACEERROR( ::GetLastError() );
   }
}

CString CFile::GetFileName( void ) const
{
   WFCLTRACEINIT( TEXT( "CFile::GetFileName()" ) );
   WFC_VALIDATE_POINTER( this );

   return( m_FileName );
}

CString CFile::GetFilePath( void ) const
{
   WFCLTRACEINIT( TEXT( "CFile::GetFilePath()" ) );
   WFC_VALIDATE_POINTER( this );

   return( m_PathName );
}

CString CFile::GetFileTitle( void ) const
{
   WFCLTRACEINIT( TEXT( "CFile::GetFileTitle()" ) );
   WFC_VALIDATE_POINTER( this );

   // 2000-05-23
   // You pay an enourmous performance penalty for calling GetFilePath().
   // The children at Microsoft actually create a COM object and
   // read tons of registry settings when you call GetFilePath().
   // This is not a good thing when you are trying to be fast.
   // We will delay the penality as long as possible.

   if ( m_FileTitle.GetLength() == 0 &&
        m_PathName.GetLength() > 0 )
   {
      TCHAR file_title[ _MAX_FNAME ];

      if ( ::__GetFileTitle( m_PathName, file_title, _MAX_FNAME ) == 0 )
      {
         const_cast< CString& >( m_FileTitle ) = file_title;
      }
      else
      {
         const_cast< CString& >( m_FileTitle ).Empty();
      }
   }

   return( m_FileTitle );
}

HANDLE CFile::GetHandle( void ) const
{
   WFCLTRACEINIT( TEXT( "CFile::GetHandle()" ) );
   WFC_VALIDATE_POINTER( this );
   return( m_FileHandle );
}

BOOL CFile::GetInformation( BY_HANDLE_FILE_INFORMATION& information ) const
{
   WFCLTRACEINIT( TEXT( "CFile::GetInformation()" ) );
   WFC_VALIDATE_POINTER( this );

   if ( ::GetFileInformationByHandle( m_FileHandle, &information ) == FALSE )
   {
      WFCTRACEERROR( ::GetLastError() );
      return( FALSE );
   }

   return( TRUE );
}

CFILE_LENGTH CFile::GetLength( void ) const
{
   WFCLTRACEINIT( TEXT( "CFile::GetLength()" ) );
   WFC_VALIDATE_POINTER( this );

   LARGE_INTEGER length;

   length.QuadPart = 0;

   if ( m_FileHandle == (HANDLE) INVALID_HANDLE_VALUE )
   {
      return( length.QuadPart );
   }

   length.LowPart = ::GetFileSize( m_FileHandle, (DWORD *) &length.HighPart );

   if ( length.LowPart == 0xFFFFFFFF && ( ::GetLastError() != NO_ERROR ) )
   {
      WFCTRACEERROR( ::GetLastError() );
      length.QuadPart = 0;
   }

   return( length.QuadPart );
}

CFILE_POSITION CFile::GetPosition( void ) const
{
   WFCLTRACEINIT( TEXT( "CFile::GetHandle()" ) );
   WFC_VALIDATE_POINTER( this );

   if ( m_FileHandle == (HANDLE) INVALID_HANDLE_VALUE )
   {
      WFCTRACE( TEXT( "File is not open." ) );
      return( (CFILE_POSITION) -1 );
   }

   LARGE_INTEGER return_value;
   LARGE_INTEGER zero = { 0, 0 };

   return_value.QuadPart = 0;

   // There's a bug in NT where SetFilePointer() doesn't set last error correctly or at all...

   if ( ::SetFilePointerEx( m_FileHandle, zero, &return_value, FILE_CURRENT ) == 0 )
   {
      // An Error Occurred
      WFCTRACEERROR( ::GetLastError() );

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

SECURITY_ATTRIBUTES * CFile::GetSecurityAttributes( void ) const
{
   WFCLTRACEINIT( TEXT( "CFile::GetSecurityAttributes()" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_COVERAGE( 11 );
   return( m_SecurityAttributes_p );
}

SECURITY_DESCRIPTOR * CFile::GetSecurityDescriptor( void ) const
{
   WFCLTRACEINIT( TEXT( "CFile::GetSecurityDescriptor()" ) );
   WFC_VALIDATE_POINTER( this );
   return( m_SecurityDescriptor_p );
}

#if 0

BOOL CFile::GetStatus( CFileStatus& status ) const
{
   WFC_VALIDATE_POINTER( this );
   ZeroMemory( &status, sizeof( status ) );

   BY_HANDLE_FILE_INFORMATION information;

   if ( GetInformation( information ) == FALSE )
   {
      return( FALSE );
   }

   _tcsncpy( status.m_szFullName, m_FileName, DIMENSION_OF( status.m_szFullName ) );

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
      status.m_ctime = CTime( information.ftCreationTime   );
      status.m_atime = CTime( information.ftLastAccessTime );
      status.m_mtime = CTime( information.ftLastWriteTime  );

      if ( status.m_ctime.GetTime() == 0 )
      {
         status.m_ctime = status.m_mtime;
      }

      if ( status.m_atime.GetTime() == 0 )
      {
         status.m_atime = status.m_mtime;
      }
   }

   return( TRUE );
}

BOOL PASCAL CFile::GetStatus( LPCTSTR filename, CFileStatus& status )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( filename );

   if ( ::__FullPath( status.m_szFullName, filename ) == FALSE )
   {
      status.m_szFullName[ 0 ] = TEXT( '\0' );
      return( FALSE );
   }

   WIN32_FIND_DATA find_data;

   HANDLE find_handle = ::FindFirstFile( (LPTSTR) filename, &find_data );

   if ( find_handle == (HANDLE) INVALID_HANDLE_VALUE )
   {
      return( FALSE );
   }

   FindClose( find_handle );

   status.m_attribute = (BYTE) ( find_data.dwFileAttributes & ~FILE_ATTRIBUTE_NORMAL );

   status.m_size = (LONG) find_data.nFileSizeLow;

   // convert times as appropriate
   status.m_ctime = CTime( find_data.ftCreationTime   );
   status.m_atime = CTime( find_data.ftLastAccessTime );
   status.m_mtime = CTime( find_data.ftLastWriteTime  );

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

#endif

void CFile::m_Initialize( void )
{
   WFCLTRACEINIT( TEXT( "CFile::m_Initialize()" ) );
   WFC_VALIDATE_POINTER( this );

   m_CloseOnDelete = FALSE;

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

   WFC_TRY
   {
      m_SecurityDescriptor_p = new SECURITY_DESCRIPTOR;
   }
   WFC_CATCH_ALL
   {
      m_SecurityDescriptor_p = nullptr;
   }
   WFC_END_CATCH_ALL

   if ( m_SecurityDescriptor_p == nullptr )
   {
      delete m_SecurityAttributes_p;
      m_SecurityAttributes_p = nullptr;

      return;
   }

   if ( ::InitializeSecurityDescriptor( m_SecurityDescriptor_p, SECURITY_DESCRIPTOR_REVISION ) == FALSE )
   {
      WFCTRACEERROR( ::GetLastError() );
      WFCTRACE( TEXT( "InitializeSecurityDescriptor() failed with the above error." ) );

      delete m_SecurityAttributes_p;
      m_SecurityAttributes_p = nullptr;

      delete m_SecurityDescriptor_p;
      m_SecurityDescriptor_p = nullptr;

      return;
   }

   if ( ::SetSecurityDescriptorDacl( m_SecurityDescriptor_p, TRUE, NULL, FALSE ) == FALSE )
   {
      WFCTRACEERROR( ::GetLastError() );
      WFCTRACE( TEXT( "SetSecurityDescriptorDacl() failed with the above error." ) );

      delete m_SecurityAttributes_p;
      m_SecurityAttributes_p = nullptr;

      delete m_SecurityDescriptor_p;
      m_SecurityDescriptor_p = nullptr;

      return;
   }

   m_SecurityAttributes_p->nLength              = sizeof( SECURITY_ATTRIBUTES );
   m_SecurityAttributes_p->lpSecurityDescriptor = m_SecurityDescriptor_p;
   m_SecurityAttributes_p->bInheritHandle       = TRUE;
}

void CFile::m_Uninitialize( void )
{
   WFCLTRACEINIT( TEXT( "CFile::m_Uninitialize()" ) );
   WFC_VALIDATE_POINTER( this );

   if ( m_SecurityAttributes_p != nullptr )
   {
      delete m_SecurityAttributes_p;
      m_SecurityAttributes_p = nullptr;
   }

   if ( m_SecurityDescriptor_p != nullptr )
   {
      delete m_SecurityDescriptor_p;
      m_SecurityDescriptor_p = nullptr;
   }
}

void CFile::LockRange( CFILE_POSITION position, CFILE_LENGTH number_of_bytes_to_lock )
{
   WFCLTRACEINIT( TEXT( "CFile::LockRange()" ) );
   WFC_VALIDATE_POINTER( this );

   LARGE_INTEGER parameter_1;
   LARGE_INTEGER parameter_2;

   parameter_1.QuadPart = position;
   parameter_2.QuadPart = number_of_bytes_to_lock;

   if ( ::LockFile( m_FileHandle, parameter_1.LowPart, parameter_1.HighPart, parameter_2.LowPart, parameter_2.HighPart ) == FALSE )
   {
      WFCTRACEERROR( ::GetLastError() );
      //CFileException::ThrowOsError( (LONG) ::GetLastError() );
   }
}

BOOL CFile::Open( LPCTSTR filename, UINT open_flags )
{
   WFCLTRACEINIT( TEXT( "CFile::Open()" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( filename );

   Close();

   WFC_TRY
   {
      m_FileName = filename;

      TCHAR full_path[ _MAX_PATH ];

      ::__FullPath( full_path, filename );

      m_PathName = full_path;

      m_FileTitle.Empty();

      open_flags &= ~ (UINT) typeBinary;

      DWORD access = 0;

      switch ( open_flags & 3 )
      {
         case modeRead:

            access = GENERIC_READ;
            break;

         case modeWrite:

            access = GENERIC_WRITE;
            break;

         case modeReadWrite:

            access = GENERIC_READ | GENERIC_WRITE;
            break;

         default:

            ASSERT( FALSE );
      }

      DWORD share_mode = 0;

      switch ( open_flags & 0x70 )
      {
         case shareCompat:
         case shareExclusive:

            share_mode = 0;
            break;

         case shareDenyWrite:

            share_mode = FILE_SHARE_READ;
            break;

         case shareDenyRead:

            share_mode = FILE_SHARE_WRITE;
            break;

         case shareDenyNone:

            share_mode = FILE_SHARE_WRITE | FILE_SHARE_READ;
            break;

         default:

            ASSERT( FALSE );
      }

      if ( m_SecurityAttributes_p != nullptr )
      {
         m_SecurityAttributes_p->bInheritHandle = ( ( open_flags & modeNoInherit ) == 0 ) ? TRUE : FALSE;
      }

      DWORD creation_flags = 0;

      if ( open_flags & modeCreate )
      {
         if ( open_flags & modeNoTruncate )
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

      // 2001-02-21
      // Thanks go to Roman Mak (roman@matrix.spb.ru) for finding a bug here.
      // I was applying the flags to the create_flags parameter and not the
      // attributes parameter.

      DWORD attributes = FILE_ATTRIBUTE_NORMAL;

      if ( open_flags & osNoBuffer )
      {
         attributes |= FILE_FLAG_NO_BUFFERING;
      }

      if ( open_flags & osRandomAccess )
      {
         attributes |= FILE_FLAG_RANDOM_ACCESS;
      }

      if ( open_flags & osSequentialScan )
      {
         attributes |= FILE_FLAG_SEQUENTIAL_SCAN;
      }

      if ( open_flags & osWriteThrough )
      {
         attributes |= FILE_FLAG_WRITE_THROUGH;
      }

      if ( open_flags & wfcDeleteOnClose )
      {
         attributes |= FILE_FLAG_DELETE_ON_CLOSE;
      }

      m_FileHandle = ::CreateFile( filename,
                                   access,
                                   share_mode,
                                   GetSecurityAttributes(),
                                   creation_flags,
                                   attributes,
                                   NULL );

      if ( m_FileHandle == (HANDLE) INVALID_HANDLE_VALUE )
      {
         WFCTRACEERROR( ::GetLastError() );

         Close();
         return( FALSE );
      }

      m_hFile = (CFILE_HFILE) m_FileHandle; // Set the stupid public member that is never used internally

      m_CloseOnDelete = TRUE;

      return( TRUE );
   }
   WFC_CATCH_ALL
   {
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

DWORD CFile::Read(std::vector<uint8_t>& buffer, DWORD number_of_bytes_to_read )
{
   WFCLTRACEINIT( TEXT( "CFile::Read( std::vector<uint8_t> )" ) );
   WFC_VALIDATE_POINTER( this );

   buffer.resize( number_of_bytes_to_read, 8192 );

   return( Read( (void *) buffer.data(), buffer.size() ) );
}

UINT CFile::Read( void * buffer, UINT number_of_bytes_to_read )
{
   WFCLTRACEINIT( TEXT( "CFile::Read( void * )" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( buffer );
   WFCTRACEVAL( TEXT( "Reading " ), number_of_bytes_to_read );

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

   if ( m_FileHandle == (HANDLE) INVALID_HANDLE_VALUE )
   {
      return( 0 );
   }

   DWORD number_of_bytes_read = 0;

   if ( ::ReadFile( m_FileHandle, buffer, number_of_bytes_to_read, &number_of_bytes_read, NULL ) == FALSE )
   {
      WFCTRACEERROR( ::GetLastError() );
      WFCTRACE( TEXT( "Can't read from file because of above error." ) );
   }

   return( number_of_bytes_read );
}

DWORD CFile::ReadHuge( void * buffer, DWORD number_of_bytes_to_read )
{
   WFC_VALIDATE_POINTER( this );
   return( Read( buffer, number_of_bytes_to_read ) );
}

void PASCAL CFile::Rename( LPCTSTR old_name, LPCTSTR new_name )
{
   WFC_VALIDATE_POINTER( old_name );
   WFC_VALIDATE_POINTER( new_name );

   if ( ::MoveFile( (LPTSTR) old_name, (LPTSTR) new_name ) == FALSE )
   {
   }
}

void PASCAL CFile::Remove( LPCTSTR filename )
{
   WFC_VALIDATE_POINTER( filename );

   if ( ::DeleteFile( (LPTSTR) filename ) == FALSE )
   {
   }
}

CFILE_POSITION CFile::Seek( CFILE_SEEK_OFFSET offset, UINT from )
{
   WFCLTRACEINIT( TEXT( "CFile::Seek()" ) );
   WFC_VALIDATE_POINTER( this );

   if ( m_FileHandle == (HANDLE) INVALID_HANDLE_VALUE )
   {
      WFCTRACE( TEXT( "File is not open." ) );
      return( (DWORD) -1 );
   }

   DWORD move_method = 0;

   switch( from )
   {
     case CFile::begin:

        WFCTRACEVAL( TEXT( "From beginning to " ), offset );
        move_method = FILE_BEGIN;
        break;

     case CFile::current:

        WFCTRACEVAL( TEXT( "From current to " ), offset );
        move_method = FILE_CURRENT;
        break;

     case CFile::end:

        WFCTRACEVAL( TEXT( "From end to " ), offset );
        move_method = FILE_END;
        break;

     default:

        WFCTRACEVAL( TEXT( "Unknown from position (it wasn't CFile::begin, CFile::current or CFile::end " ), from );
        return( (DWORD) -1 );
   }

   LARGE_INTEGER return_value = { 0, 0 };
   LARGE_INTEGER distance_to_move;

   distance_to_move.QuadPart = offset;

   // There's a bug in NT where SetFilePointer() doesn't set last error correctly or at all...
   if (  ::SetFilePointerEx( m_FileHandle, distance_to_move, &return_value, move_method ) == 0 )
   {
      WFCTRACEERROR( ::GetLastError() );

#if ! defined( WFC_STL )

      if ( m_BeStupidAndUseExceptions == true )
      {
         CFileException::ThrowOsError( (LONG) ::GetLastError() );
      }

#endif // WFC_STL
   }


   return( return_value.QuadPart );
}

void CFile::SeekToBegin( void )
{
   WFCLTRACEINIT( TEXT( "CFile::SeekToBegin()" ) );
   WFC_VALIDATE_POINTER( this );

   Seek( (ULONGLONG) 0, CFile::begin );
}

CFILE_POSITION CFile::SeekToEnd( void )
{
   WFCLTRACEINIT( TEXT( "CFile::SeekToEnd()" ) );
   WFC_VALIDATE_POINTER( this );
   return( Seek( 0, CFile::end ) );
}

BOOL CFile::SetEndOfFile( CFILE_LENGTH length )
{
   WFCLTRACEINIT( TEXT( "CFile::SetEndOfFile()" ) );
   WFC_VALIDATE_POINTER( this );

   Seek( length, CFile::begin );

   _ASSERTE( GetPosition() == length );

   if ( ::SetEndOfFile( m_FileHandle ) == FALSE )
   {
      WFCTRACEERROR( ::GetLastError() );
      WFCTRACE( TEXT( "Can't set end of file because of above error." ) );
      return( FALSE );
   }

   return( TRUE );
}

void CFile::SetFilePath( LPCTSTR new_name )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( new_name );
   m_FileName = new_name;
}

void CFile::SetLength( CFILE_LENGTH new_length )
{
   WFC_VALIDATE_POINTER( this );

   if ( SetEndOfFile( new_length ) == FALSE )
   {
   }
}

#if 0
void PASCAL CFile::SetStatus( LPCTSTR filename, const CFileStatus& status )
{
   CFile::SetStatus( filename, status );
}
#endif

void CFile::UnlockRange( CFILE_POSITION position, CFILE_LENGTH number_of_bytes_to_unlock )
{
   WFCLTRACEINIT( TEXT( "CFile::UnlockRange()" ) );
   WFC_VALIDATE_POINTER( this );

   LARGE_INTEGER parameter_1;
   LARGE_INTEGER parameter_2;

   parameter_1.QuadPart = position;
   parameter_2.QuadPart = number_of_bytes_to_unlock;

   if ( ::UnlockFile( m_FileHandle, parameter_1.LowPart, parameter_1.HighPart, parameter_2.LowPart, parameter_2.HighPart ) == FALSE )
   {
      WFCTRACEERROR( ::GetLastError() );
   }
}

void CFile::Write( const void * buffer, UINT number_of_bytes_to_write )
{
   WFCLTRACEINIT( TEXT( "CFile::Write()" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( buffer );
   WFC_COVERAGE( 26 );

   if ( number_of_bytes_to_write == 0 )
   {
      return;
   }

   DWORD number_of_bytes_written = 0;

   if ( ::WriteFile( m_FileHandle, buffer, number_of_bytes_to_write, &number_of_bytes_written, NULL ) == FALSE )
   {
      WFCTRACEERROR( ::GetLastError() );
   }
}

void CFile::WriteHuge( const void * buffer, DWORD number_of_bytes_to_write )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( buffer );
   Write( buffer, number_of_bytes_to_write );
}

// Operators

CFile::operator HFILE ( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( (HFILE) m_FileHandle );
}

#endif // WFC_STL

// End of source

/*
<HTML>

<HEAD>
<TITLE>CFile</TITLE>
</HEAD>

<BODY>

<H1>CFile</H1>

$Revision: 13 $<BR><HR>

<H2>Description</H2>

This class gives you the ability to read very large files
(up to 18 million terabytes). It is designed to be a drop in
replacement for MFC's CFile class.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CFile</B>()
<B>CFile</B>( int file_handle );
<B>CFile</B>( LPCTSTR filename, UINT open_flags );
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

<DT><PRE>CFile * <B><A NAME="Duplicate">Duplicate</A></B>( void ) const</PRE><DD>
Will duplicate the handle to the file and return a new CFile
object to you.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>void <B><A NAME="Flush">Flush</A></B>( void )</PRE><DD>
Flushes the data to the disk.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>CString <B><A NAME="GetFileName">GetFileName</A></B>( void ) const</PRE><DD>
Returns the name of the file that is open.

<DT><PRE>CString <B><A NAME="GetFilePath">GetFilePath</A></B>( void ) const</PRE><DD>
Returns the complete path of the file that is open.

<DT><PRE>CString <B><A NAME="GetFileTitle">GetFileTitle</A></B>( void ) const</PRE><DD>
Returns the title of the file. This is usually the filename without
the path or extension.

<DT><PRE>BOOL <B><A NAME="GetInformation">GetInformation</A></B>( BY_HANDLE_FILE_INFORMATION&amp; information ) const</PRE><DD>
Gives you all kinds of neat information about an open file.

<DT><PRE>ULONGLONG <B><A NAME="GetLength">GetLength</A></B>( void ) const</PRE><DD>
Returns the length of the file. It will return zero if it fails.

<DT><PRE>ULONGLONG <B><A NAME="GetPosition">GetPosition</A></B>( void ) const</PRE><DD>
Returns the current position in the file of the file pointer.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>SECURITY_ATTRIBUTES * <B><A NAME="GetSecurityAttributes">GetSecurityAttributes</A></B>( void ) const</PRE><DD>
Returns the pointer to the security attributes for this file.

<DT><PRE>SECURITY_DESCRIPTOR * <B><A NAME="GetSecurityDescriptor">GetSecurityDescriptor</A></B>( void ) const</PRE><DD>
Returns the pointer to the security descriptor for this file.

<DT><PRE>BOOL <B><A NAME="GetStatus">GetStatus</A></B>( CFileStatus&amp; status ) const
static BOOL PASCAL <B>GetStatus</B>)( LPCTSTR filename, CFileStatus&amp; status )</PRE><DD>
Will fill <CODE>status</CODE> with information about the file.

<DT><PRE>void <B><A NAME="LockRange">LockRange</A></B>( ULONGLONG position, ULONGLONG number_of_bytes_to_lock )</PRE><DD>
Locks the range of bytes in the file.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( LPCTSTR filename, UINT open_flags, CFileException * exception_p = NULL )</PRE><DD>
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

<DT><PRE>ULONGLONG <B><A NAME="Seek">Seek</A></B>( ULONGLONG offset, UINT from )</PRE><DD>
Seeks to a position in a file.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>void <B><A NAME="SeekToBegin">SeekToBegin</A></B>( void )</PRE><DD>
Seeks to the beginning of the file.

<DT><PRE>ULONGLONG <B><A NAME="SeekToEnd">SeekToEnd</A></B>( void )</PRE><DD>
Seeks to the end of the file.

<DT><PRE>BOOL <B><A NAME="SetEndOfFile">SetEndOfFile</A></B>( ULONGLONG length )</PRE><DD>
Sets the end of the file at the specified location.

<DT><PRE>void <B><A NAME="SetFilePath">SetFilePath</A></B>( LPCTSTR new_name )</PRE><DD>
Sets the file name.

<DT><PRE>void <B><A NAME="SetLength">SetLength</A></B>( ULONGLONG new_length )</PRE><DD>
Sets the end of the file at the specified location.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>static void PASCAL <B><A NAME="SetStatus">SetStatus</A></B>( LPCTSTR filename, const CFileStatus&amp; status )</PRE><DD>
Basically calls CFile's implementation of this function.

<DT><PRE>void <B><A NAME="UnlockRange">UnlockRange</A></B>( ULONGLONG position, ULONGLONG number_of_bytes_to_lock )</PRE><DD>
Unlocks a range of bytes in a file.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>void <B><A NAME="Write">Write</A></B>( const void * buffer, DWORD number_of_bytes )</PRE><DD>
Writes the bytes to the file.
Like CFile, it will throw a CFileException if there is a problem.

<DT><PRE>void <B><A NAME="WriteHuge">WriteHuge</A></B>( const void * buffer, DWORD number_of_bytes )</PRE><DD>
Writes the bytes to the file.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   <B>CFile</B> file;

   if ( file.<A HREF="#Open">Open</A>( command_line_arguments[ 1 ], CFile::modeRead ) == FALSE )
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
<LI>SetFilePointer
<LI>SetLastError
<LI>SetSecurityDescriptorDacl
<LI>UnlockFile
<LI>WriteFile
<LI>ZeroMemory
</UL>

<HR><I><A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CFile.cpp $<BR>
$Modtime: 2/21/01 5:02a $
</BODY>

</HTML>
*/
