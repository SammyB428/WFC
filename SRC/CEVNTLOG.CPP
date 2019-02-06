/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2016, Samuel R. Blackburn
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
** $Workfile: cevntlog.cpp $
** $Revision: 43 $
** $Modtime: 6/26/01 10:45a $
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

CEventLog::CEventLog()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CEventLog::CEventLog( __in_z LPCTSTR source_name )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( source_name );

   m_Initialize();
   (void )RegisterSource( source_name );
}

CEventLog::~CEventLog()
{
   WFC_VALIDATE_POINTER( this );

   if ( m_EventSourceHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      (void) DeregisterSource();
   }

   if ( m_LogHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      (void) Close();
   }

   m_Initialize();
}

void CEventLog::m_Initialize( void )
{
   WFC_VALIDATE_POINTER( this );

   ComputerName.clear();
   LogName.clear();

   m_LogHandle                 = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
   m_EventSourceHandle         = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
   m_ErrorCode                 = 0;
   m_NumberOfBytesRead         = 0;
   m_NumberOfBytesInNextRecord = 0;
}

__checkReturn bool CEventLog::Backup( __in_z LPCTSTR name_of_backup_file ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( name_of_backup_file );

   if ( name_of_backup_file == nullptr )
   {
      m_ErrorCode = ERROR_INVALID_PARAMETER;
      return( false );
   }

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      BOOL return_value = ::BackupEventLog( m_LogHandle, name_of_backup_file );

      if ( return_value == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACEERROR( m_ErrorCode );
      }
      else
      {
         return_value = TRUE;
      }

      return( return_value == FALSE ? false : true );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

__checkReturn BOOL CEventLog::Clear( __in_z LPCTSTR name_of_backup_file ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( name_of_backup_file );

   /*
   ** name_of_backup_file can be nullptr
   */

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
#if defined( _DEBUG )

      if ( name_of_backup_file != nullptr )
      {
         //WFCTRACEVAL( TEXT( "name of backup file is " ), std::wstring( name_of_backup_file ) );
      }
      else
      {
         //WFCTRACE( TEXT( "name of backup file is NULL" ) );
      }

#endif // _DEBUG

      BOOL return_value = ::ClearEventLog( m_LogHandle, name_of_backup_file );

      if ( return_value == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACEERROR( m_ErrorCode );
      }
      else
      {
         return_value = TRUE;
      }

      return( return_value );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

__checkReturn BOOL CEventLog::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   BOOL return_value = ::CloseEventLog( m_LogHandle );

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
   }
   else
   {
      return_value = TRUE;
   }

   m_LogHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );

   return( return_value );
}

__checkReturn bool CEventLog::CreateApplicationLog( __in_z LPCTSTR application_name, __in_z LPCTSTR message_resource_file, __in DWORD supported_types ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( application_name );
   WFC_VALIDATE_POINTER( message_resource_file );

   if ( application_name == nullptr || message_resource_file == nullptr )
   {
      m_ErrorCode = ERROR_INVALID_PARAMETER;
      return( false );
   }

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      if ( application_name[ 0 ] == 0x00 || message_resource_file[ 0 ] == 0x00 )
      {
         return( false );
      }

      CRegistry registry;

      if ( registry.Connect( (HKEY) CRegistry::keyLocalMachine ) == false )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( false );
      }

      std::wstring log_key_name( L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\" );

      log_key_name.append( application_name );

      if ( registry.Create( log_key_name.c_str() ) == false )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( false );
      }

      // 1999-08-17
      // Thanks go to Donald Hartzog (donhartzog@usa.net) for finding a UNICODE
      // bug here. I was assuming _tcslen() == sizeof() a string. DOH!

      if ( registry.SetValue( L"EventMessageFile", CRegistry::typeUnexpandedString, (LPBYTE) message_resource_file, ( _tcslen( message_resource_file ) + 1 ) * sizeof( TCHAR ) ) == false )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( false );
      }

      if ( registry.SetValue( L"TypesSupported", supported_types ) == false )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( false );
      }

      /*
      ** Many thanks go to Juha Jalovaara (Juha@compuserve.com) for
      ** searching out the fact that I was not adding to the Sources value.
      ** I may be forgetful, but I thought the Win32 RegisterEventSource
      ** used to do this. Oh well, no harm no foul.
      */

      if ( registry.Open( L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application" ) != false )
      {
         std::vector<std::wstring> sources;

         if ( registry.GetValue( L"Sources", sources ) != false )
         {
            bool application_was_found = false;

            for ( const auto& entry : sources )
            {
               if ( entry.compare(application_name) == I_AM_EQUAL_TO_THAT )
               {
                  application_was_found = true;
                  break;
               }
            }

            if ( application_was_found == false )
            {
               // The application isn't listed as a source, we must add it

               (void) sources.push_back( application_name );
               (void) registry.SetValue( L"Sources", sources );
            }
         }
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

__checkReturn BOOL CEventLog::CreateCustomLog( __in_z LPCTSTR log_name_parameter, __in_z_opt LPCTSTR log_filename_parameter, __in DWORD maximum_size_in_bytes, __in DWORD overwrite_after_this_many_seconds ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( log_name_parameter );
   WFC_VALIDATE_POINTER_NULL_OK( log_filename_parameter );

   if ( log_name_parameter == nullptr )
   {
      m_ErrorCode = ERROR_INVALID_PARAMETER;
      return( FALSE );
   }

   // Let's normallize the file size to a 64K boundary (round up if needed)

   maximum_size_in_bytes += ( maximum_size_in_bytes % 0x10000 );

   if ( maximum_size_in_bytes == 0 )
   {
      maximum_size_in_bytes = 0x10000;
   }

   std::wstring log_name;
   std::wstring log_filename;

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      log_name.assign( log_name_parameter );

      if ( log_filename_parameter == nullptr )
      {
         log_filename.assign( L"%SystemRoot%\\System32\\config\\" );
         log_filename.append( log_name );
         log_filename.append( L".Evt" );
      }
      else
      {
         log_filename.assign( log_filename_parameter );
      }

      CRegistry registry;

      if ( registry.Connect( (HKEY) CRegistry::keyLocalMachine ) == FALSE )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( FALSE );
      }

      std::wstring log_key_name( L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\" );

      log_key_name.append( log_name );

      if ( registry.Create( log_key_name.c_str() ) == false )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( FALSE );
      }

      if ( registry.SetValue( L"File", CRegistry::typeUnexpandedString, reinterpret_cast<const uint8_t *>(log_filename.data()), log_filename.length() * sizeof(wchar_t) ) == FALSE )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( FALSE );
      }

      if ( registry.SetValue( TEXT( "MaxSize" ), maximum_size_in_bytes ) == FALSE )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( FALSE );
      }

      if ( registry.SetValue( TEXT( "Retention" ), overwrite_after_this_many_seconds ) == FALSE )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( FALSE );
      }

      log_key_name.append( L"\\WFC" );

      if ( registry.Create( log_key_name.c_str() ) == FALSE )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( FALSE );
      }

      return( TRUE );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ bool CEventLog::Delete( _In_z_ const wchar_t * log_name ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( log_name );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      std::wstring log_to_delete( log_name );

      if ( log_to_delete.empty() == true )
      {
         return( false );
      }

      //WFCTRACEVAL( TEXT( "Deleting " ), log_to_delete );

      // OK. If the caller is going to delete a default NT log, let's make
      // really sure they want to do this. By using ASSERT, Visual C++ will
      // popup an Abort, Retry, or Ignore window.

      ASSERT( compare_no_case( log_to_delete, L"Application" ) != I_AM_EQUAL_TO_THAT );
      ASSERT( compare_no_case( log_to_delete, L"Security"    ) != I_AM_EQUAL_TO_THAT );
      ASSERT( compare_no_case( log_to_delete, L"System"      ) != I_AM_EQUAL_TO_THAT );

      CRegistry registry;

      if ( registry.Connect( (HKEY) CRegistry::keyLocalMachine ) == false )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( false );
      }

      std::wstring log_key_name( L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\" );

      // Should I delete the EVT file? Let's be safe and not do that right now

      log_key_name.append( log_to_delete );

      if ( registry.DeleteKey( log_key_name.c_str() ) == false )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( false );
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

_Check_return_ bool CEventLog::DeleteApplicationLog( _In_z_ const wchar_t * application_name ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( application_name );

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      if ( application_name == nullptr )
      {
         m_ErrorCode = ERROR_INVALID_PARAMETER;
         return( false );
      }

      if ( application_name[ 0 ] == 0x00 )
      {
         return( false );
      }

      CRegistry registry;

      if ( registry.Connect( (HKEY) CRegistry::keyLocalMachine ) == false )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( false );
      }

      // Thanks to Bejan Aminifard <bejan@ix.netcom.com> for finding a bug here

      std::wstring log_key_name( L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\" );

      log_key_name.append( application_name );

      if ( registry.DeleteKey( log_key_name.c_str() ) == false )
      {
         m_ErrorCode = registry.GetErrorCode();
         return( false );
      }

      /*
      ** Microsoft has a bug in this area. Even though we deleted the application from the
      ** HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\EventLog\Application\
      ** registry area, they don't provide a way to delete the application from the 
      ** HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\EventLog\Application\Sources
      ** value. The application name is one of the strings in this REG_MULTI_SZ value. We
      ** still need to delete it from there. The names listed in this value appear in the 
      ** "Source" combobox of the Event Viewer application View->Filter Events... menu selection.
      */

      if ( registry.Open( L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application" ) != false )
      {
         std::vector<std::wstring> sources;

         if ( registry.GetValue( L"Sources", sources ) != false )
         {
            bool application_was_found = false;

            for ( const auto index : Range(sources.size()) )
            {
               if ( sources.at( index ).compare(application_name) == I_AM_EQUAL_TO_THAT )
               {
                  application_was_found = true;
                  sources.erase(sources.cbegin() + index );
                  break;
               }
            }

            if ( application_was_found != false )
            {
               (void) registry.SetValue( L"Sources", sources );
            }
         }
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

__checkReturn BOOL CEventLog::DeregisterSource( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   BOOL return_value = TRUE;

   if ( m_EventSourceHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      return_value = ::DeregisterEventSource( m_EventSourceHandle );

      if ( return_value == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACEERROR( m_ErrorCode );
      }
      else
      {
         return_value = TRUE;
      }

      m_EventSourceHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
   }

   return( return_value );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CEventLog::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );

   dump_context << TEXT( "m_LogHandle                 =  "  ) << m_LogHandle                 << TEXT( "\n" );
   dump_context << TEXT( "m_EventSourceHandle         =  "  ) << m_EventSourceHandle         << TEXT( "\n" );
   dump_context << TEXT( "m_ErrorCode                 =  "  ) << m_ErrorCode                 << TEXT( "\n" );
   dump_context << TEXT( "m_NumberOfBytesRead         =  "  ) << m_NumberOfBytesRead         << TEXT( "\n" );
   dump_context << TEXT( "m_NumberOfBytesInNextRecord =  "  ) << m_NumberOfBytesInNextRecord << TEXT( "\n" );
   dump_context << TEXT( "ComputerName                = \"" ) << ComputerName                << TEXT( "\"\n" );
   dump_context << TEXT( "LogName                     = \"" ) << LogName                     << TEXT( "\"\n" );
}

#endif // _DEBUG

__checkReturn BOOL CEventLog::EnumerateLogs( __out DWORD& enumerator ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   enumerator = 0;
   return( TRUE );
}

__checkReturn DWORD CEventLog::GetErrorCode( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_ErrorCode );
}

__checkReturn HANDLE CEventLog::GetHandle( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_LogHandle );
}

__checkReturn BOOL CEventLog::GetNextLog( __inout DWORD& enumerator, __out std::wstring& log_name ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   log_name.clear();

   CRegistry registry;

   BOOL return_value = FALSE;

   if ( ComputerName.empty() == false )
   {
       return_value = registry.Connect((HKEY)CRegistry::keyLocalMachine, ComputerName.c_str() );
   }
   else
   {
      return_value = registry.Connect( (HKEY) CRegistry::keyLocalMachine, nullptr );
   }

   if ( return_value == FALSE )
   {
      m_ErrorCode = registry.GetErrorCode();
      //WFCTRACEERROR( m_ErrorCode ); 
      return( FALSE );
   }

   DWORD permissions = CRegistry::permissionEnumerateSubKeys | CRegistry::permissionRead;

   return_value = registry.Open( TEXT( "SYSTEM\\CurrentControlSet\\Services\\EventLog" ), static_cast< CRegistry::CreatePermissions >( permissions ) );

   if ( return_value == FALSE )
   {
      m_ErrorCode = registry.GetErrorCode();
      //WFCTRACEERROR( m_ErrorCode ); 
      return( FALSE );
   }

   DWORD key_index = 0;

   std::wstring class_name;

   while( registry.EnumerateKeys( key_index, log_name, class_name ) != FALSE )
   {
      if ( key_index == enumerator )
      {
         enumerator++;
         return( TRUE );
      }

      key_index++;
   }

   enumerator++;

   return( FALSE );
}

__checkReturn BOOL CEventLog::GetNumberOfRecords( __out DWORD& number_of_records ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   number_of_records = 0; // Always start at a known state

   BOOL return_value = ::GetNumberOfEventLogRecords( m_LogHandle, &number_of_records );

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEVAL( TEXT( "GetNumberOfEventLogRecords() failed with " ), m_ErrorCode );
      //WFCTRACEERROR( m_ErrorCode );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value );
}

__checkReturn DWORD CEventLog::GetOldestRecordNumber( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD return_value = 0;

   if ( ::GetOldestEventLogRecord( m_LogHandle, &return_value ) != FALSE )
   {
      return( return_value );
   }

   m_ErrorCode = ::GetLastError();

   //WFCTRACE( TEXT( "GetOldestEventLogRecord() failed" ) );
   //WFCTRACEERROR( m_ErrorCode );

   return( 0 );
}

__checkReturn BOOL CEventLog::NotifyChange( __in HANDLE event_handle, __in_opt HANDLE user_log_handle ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( event_handle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );

   if ( event_handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      m_ErrorCode = ERROR_INVALID_PARAMETER;
      return( FALSE );
   }

   if ( is_bad_handle( user_log_handle ) == true )
   {
      user_log_handle = m_LogHandle;
   }

   BOOL return_value = ::NotifyChangeEventLog( user_log_handle, event_handle );

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value );
}

__checkReturn BOOL CEventLog::Open( __in_z LPCTSTR log_name, __in_z_opt LPCTSTR name_of_computer ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( log_name );
   WFC_VALIDATE_POINTER_NULL_OK( name_of_computer );

   /*
   ** name_of_computer can be nullptr
   */

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      if ( log_name == nullptr )
      {
         m_ErrorCode = ERROR_INVALID_PARAMETER;
         return( FALSE );
      }

      BOOL return_value = TRUE;

      m_LogHandle = ::OpenEventLog( name_of_computer, log_name );

      if ( m_LogHandle == static_cast< HANDLE >( NULL ) )
      {
         m_ErrorCode  = ::GetLastError();
         m_LogHandle  = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
         return_value = FALSE;

         //WFCTRACEERROR( m_ErrorCode );
      }
      else
      {
         if ( name_of_computer == nullptr )
         {
            wchar_t computer_name[ MAX_PATH ];

            ZeroMemory( computer_name, sizeof( computer_name ) );

            DWORD size = static_cast<DWORD>(std::size( computer_name ));

            if ( ::GetComputerNameExW(COMPUTER_NAME_FORMAT::ComputerNameDnsHostname, computer_name, &size ) != FALSE )
            {
               ComputerName.assign( computer_name );
            }
            else
            {
               ComputerName.clear();
            }
         }
         else
         {
           ComputerName.assign( name_of_computer );
         }
      }

      return( return_value );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

__checkReturn BOOL CEventLog::OpenBackup( __in_z LPCTSTR name_of_backup_file, __in_z_opt LPCTSTR name_of_computer ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( name_of_backup_file );

   /*
   ** name_of_computer can be NULL
   */

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      if ( name_of_backup_file == nullptr )
      {
         m_ErrorCode = ERROR_INVALID_PARAMETER;
         return( FALSE );
      }

      BOOL return_value = TRUE;

      m_LogHandle = ::OpenBackupEventLog( name_of_computer, name_of_backup_file );

      if ( m_LogHandle == static_cast< HDESK >( NULL ) )
      {
         m_LogHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
         m_ErrorCode = ::GetLastError();
         //WFCTRACEERROR( m_ErrorCode );
         return_value = FALSE;
      }
      else
      {
         if ( name_of_computer == nullptr )
         {
            wchar_t computer_name[ MAX_PATH ];

            ZeroMemory( computer_name, sizeof( computer_name ) );

            DWORD size = static_cast<DWORD>(std::size( computer_name ));

            if ( ::GetComputerNameExW(COMPUTER_NAME_FORMAT::ComputerNameDnsHostname, computer_name, &size ) != FALSE )
            {
               ComputerName.assign( computer_name );
            }
            else
            {
               ComputerName.clear();
            }
         }
         else
         {
            ComputerName.assign( name_of_computer );
         }
      }

      return( return_value );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

__checkReturn BOOL CEventLog::Read( __in DWORD record_number, __out CEventLogRecord& record, __in DWORD how_to_read ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( is_bad_handle( m_LogHandle ) == false );

   record.Empty();

   // We were passed a pointer, do not trust

   WFC_TRY
   {
      if ( is_bad_handle( m_LogHandle ) == true )
      {
         m_ErrorCode = ERROR_INVALID_PARAMETER;
         return( FALSE );
      }

      /*
      ** OK, time for a lecture.
      ** Microsoft coded ReadEventLog() so it would read as many records from
      ** the event log as would fit in the memory buffer you sent it. This is
      ** all well and good but it increases the number of calls when all you
      ** want to do is read one stinking little record at a time. Yes, this
      ** is an awfully inefficient way of doing it but it is simple.
      */

      BOOL return_value = FALSE;

      // First, find out how many bytes we need for this record

      BYTE temporary_buffer[ 1 ]; // a throw-away valid memory address

      DWORD number_of_bytes_to_read = 0;

      return_value = Read( record_number, temporary_buffer, number_of_bytes_to_read, how_to_read );

      m_ErrorCode = ::GetLastError();

      if ( return_value == FALSE && m_ErrorCode != ERROR_INSUFFICIENT_BUFFER )
      {
         //WFCTRACEERROR( m_ErrorCode );
         return( FALSE );
      }

      m_ErrorCode = 0;

      // m_NumberOfBytesInNextRecord now contains the size our buffer should be

      std::unique_ptr<uint8_t[]> allocated_buffer = std::make_unique<uint8_t[]>(m_NumberOfBytesInNextRecord);

      if (allocated_buffer.get() == nullptr )
      {
         return( FALSE );
      }

      // Always always always zero your buffers

      ZeroMemory(allocated_buffer.get(), m_NumberOfBytesInNextRecord );

      return_value = Read( record_number, allocated_buffer.get(), m_NumberOfBytesInNextRecord, how_to_read );

      if ( return_value == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACEERROR( m_ErrorCode );
      }
      else
      {
         record.Copy( reinterpret_cast<EVENTLOGRECORD *>(allocated_buffer.get()) );
      }

      return( return_value );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

__checkReturn BOOL CEventLog::Read( __in DWORD record_number, __out_bcount_part( number_of_bytes_to_read, number_of_bytes_to_read ) LPVOID buffer, __inout DWORD& number_of_bytes_to_read, __in DWORD how_to_read ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( buffer );
   ASSERT( m_LogHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );

   // We were passed a pointer, do not trust

   WFC_TRY
   {
      if ( buffer == nullptr || m_LogHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
      {
         m_ErrorCode = ERROR_INVALID_PARAMETER;
         return( FALSE );
      }

      BOOL return_value = ::ReadEventLog( m_LogHandle,
                                          how_to_read,
                                          record_number,
                                          buffer,
                                          number_of_bytes_to_read,
                                         &m_NumberOfBytesRead,
                                         &m_NumberOfBytesInNextRecord );

      if ( return_value == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACEERROR( m_ErrorCode );
      }
      else
      {
         return_value = TRUE;
      }

      return( return_value );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

__checkReturn BOOL CEventLog::RegisterSource( __in_z LPCTSTR source_name, __in_z_opt LPCTSTR name_of_computer ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source_name );
   WFC_VALIDATE_POINTER_NULL_OK( name_of_computer );

   /*
   ** name_of_computer can be nullptr
   */

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      if ( source_name == nullptr )
      {
         m_ErrorCode = ERROR_INVALID_PARAMETER;
         return( FALSE );
      }

      BOOL return_value = TRUE;

      if ( m_EventSourceHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
      {
         (void) DeregisterSource();
      }

      m_EventSourceHandle = ::RegisterEventSource( name_of_computer, source_name );

      if ( m_EventSourceHandle == static_cast< HDESK >( NULL ) )
      {
         m_ErrorCode         = ::GetLastError();
         //WFCTRACE( TEXT( "RegisterEventSource returned NULL" ) );
         //WFCTRACEERROR( m_ErrorCode );
         m_EventSourceHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
         return_value        = FALSE;
      }

      return( return_value );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

__checkReturn BOOL CEventLog::Report( __in EventType event_type,
                        __in WORD      category,
                        __in DWORD     event_identifier,
                        __in WORD      number_of_strings,
                        __in_ecount_z_opt( number_of_strings ) LPCTSTR * string_array,
                        __in DWORD     number_of_raw_data_bytes,
                        __in_bcount_opt( number_of_raw_data_bytes ) LPVOID raw_data_buffer,
                        __in_opt PSID      user_security_identifier ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( string_array );
   WFC_VALIDATE_POINTER_NULL_OK( raw_data_buffer );

   BYTE security_identifier_buffer[ 4096 ];

   DWORD size_of_security_identifier_buffer = sizeof( security_identifier_buffer );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( user_security_identifier == nullptr )
      {
         TCHAR user_name[ 256 ];
         DWORD size_of_user_name  = static_cast<DWORD>(std::size( user_name ));

         TCHAR domain_name[ 256 ];
         DWORD size_of_domain_name = static_cast<DWORD>(std::size( domain_name ));

         SID_NAME_USE type_of_security_identifier;

         ::ZeroMemory( user_name, sizeof( user_name ) );
         ::ZeroMemory( domain_name, sizeof( domain_name ) );
         ::ZeroMemory( security_identifier_buffer, size_of_security_identifier_buffer );

         ::GetUserName( user_name, &size_of_user_name );

         if ( ::LookupAccountName( nullptr,
                                   user_name,
                                  &security_identifier_buffer,
                                  &size_of_security_identifier_buffer,
                                   domain_name,
                                  &size_of_domain_name,
                                  &type_of_security_identifier ) != FALSE )
         {
            user_security_identifier = security_identifier_buffer;
         }
      }

      BOOL return_value = FALSE;

      if ( m_EventSourceHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
      {
         return_value = ::ReportEvent( m_EventSourceHandle,
                                (WORD) event_type,
                                       category,
                                       event_identifier,
                                       user_security_identifier,
                                       number_of_strings,
                                       number_of_raw_data_bytes,
                                       string_array,
                                       raw_data_buffer );

         if ( return_value == FALSE )
         {
            m_ErrorCode = ::GetLastError();
            //WFCTRACEERROR( m_ErrorCode );
         }
         else
         {
            return_value = TRUE;
         }
      }
      else
      {
         m_ErrorCode = ERROR_INVALID_HANDLE;
      }

     return( return_value );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

__checkReturn BOOL CEventLog::Report( __in_z LPCTSTR log_name, __in DWORD message_string_resource_identifier, __in WORD number_of_strings, __in_ecount_z_opt( number_of_strings ) LPCTSTR * string_array ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( log_name );
   WFC_VALIDATE_POINTER_NULL_OK( string_array );

   if ( log_name == nullptr )
   {
      m_ErrorCode = ERROR_INVALID_PARAMETER;
      return( FALSE );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( string_array == nullptr )
      {
         m_ErrorCode = ERROR_INVALID_HANDLE;
         return( FALSE );
      }

      if ( RegisterSource( log_name ) != FALSE )
      {
         if ( Report( eventError, 0, message_string_resource_identifier, number_of_strings, string_array ) == FALSE )
         {
            (void) DeregisterSource();
            return( FALSE );
         }

         (void) DeregisterSource();
      }
      else
      {
         return( FALSE );
      }

      return( TRUE );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

void CEventLog::ReportError( __in_z LPCTSTR string_to_report ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( string_to_report );

   LPCTSTR string_array[ 1 ];

   if ( string_to_report == nullptr )
   {
      string_array[ 0 ] = TEXT( "CEventLog::ReportError( NULL )" );
   }
   else
   {
      string_array[ 0 ] = string_to_report;
   }

   (void) Report( eventError, 0, messageNumberZero, 1, string_array );
}

void CEventLog::ReportInformation( __in_z LPCTSTR string_to_report ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( string_to_report );

   LPCTSTR string_array[ 1 ];

   if ( string_to_report == nullptr )
   {
      string_array[ 0 ] = TEXT( "CEventLog::ReportInformation( NULL )" );
   }
   else
   {
      string_array[ 0 ] = string_to_report;
   }

   (void) Report( eventInformation, 0, messageNumberZero, 1, string_array );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CEventLog</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles the Win32 event logger.">
</HEAD>

<BODY>

<H1>CEventLog</H1>
$Revision: 43 $
<HR>

<H2>Description</H2>

This class handles playing with NT's Event Logger.

<H2>Data Members</H2>

<B>ComputerName</B> - Name of the computer where the event log resides.<P>
<B>LogName</B> - Name of the log you're playing with.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="Backup">Backup</A></B>( LPCTSTR name_of_backup_file )</PRE><DD>
Saves the log to <CODE>name_of_backup_file</CODE>.

<DT><PRE>BOOL <B><A NAME="Clear">Clear</A></B>( LPCTSTR name_of_backup_file )</PRE><DD>
Empties the event log and will optionally save it to a backup file specified
by <CODE>name_of_backup_file</CODE>. If <CODE>name_of_backup_file</CODE>
is NULL, no backup is made.

<DT><PRE>BOOL <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes your session with the event log.

<DT><PRE>BOOL <B><A NAME="CreateApplicationLog">CreateApplicationLog</A></B>( LPCTSTR application_name, LPCTSTR file_containing_message_table_resource, DWORD supported_types )</PRE><DD>
This creates a source of strings for the event
log. Basically, it registers your executable with the event logger.
This allows the event logger to go to your executable's message
table resource for strings to display to the user via the Event Viewer.

<DT><PRE>BOOL <B><A NAME="CreateCustomLog">CreateCustomLog</A></B>( LPCTSTR log_name,
                      LPCTSTR log_filename = nullptr,
                      DWORD   maximum_file_size = 0x800000,
                      DWORD   overwrite_after_this_many_seconds = 604800 )</PRE><DD>
This allows you to create your own custom event log file. The
<CODE>maximum_file_size</CODE> specifies the maximum size of the event
log file. The <CODE>overwrite_after_this_many_seconds</CODE> let's
you specify the age of an entry before it will be overwritten. The
default for these parameters is 8MB file size and 7 days before
events will be overwritten.

<DT><PRE><B><A NAME="Delete">Delete</A></B>( LPCTSTR log_name )</PRE><DD>
This will delete an event log. <STRONG>WARNING!</STRONG>
This is a very dangerous method. It will delete any and all
event logs in the system. Use with caution.

<DT><PRE>BOOL <B><A NAME="DeleteApplicationLog">DeleteApplicationLog</A></B>( LPCTSTR application_name )</PRE><DD>
Disconnects the application from the Event Log.

<DT><PRE>BOOL <B><A NAME="DeregisterSource">DeregisterSource</A></B>( void )</PRE><DD>
Deregisters the source. It basically releases the source of event strings.

<DT><PRE>BOOL <B><A NAME="EnumerateLogs">EnumerateLogs</A></B>( DWORD&amp; enumerator ) const</PRE><DD>
Initializes <CODE>enumerator</CODE> so you can enumerate through
the logs that are installed on the system.

<DT><PRE>DWORD <B><A NAME="GetErrorCode">GetErrorCode</A></B>( void ) const</PRE><DD>
Retrieves the error code. Call this function
to find out why any other class member returned FALSE.

<DT><PRE>HANDLE <B><A NAME="GetHandle">GetHandle</A></B>( void ) const</PRE><DD>
Returns the encapsulated event log handle so you can
call the Win32 API directly (i.e. you don't have to use
this class).

<DT><PRE>BOOL <B><A NAME="GetNextLog">GetNextLog</A></B>( DWORD&amp; enumerator, std::wstring&amp; log_name )</PRE><DD>
Allows you to get the next log while enumerating them.
The <CODE>enumerator</CODE> must be initialized via the
<B><A HREF="#EnumerateLogs">EnumerateLogs</A></B>() method.
Normally, when you enumerate the logs, you will get the
default three logs:

<UL>
<LI>Application
<LI>Security
<LI>System
</UL>

However, when an application creates a custom log, it will show up
in this list.

<DT><PRE>BOOL <B><A NAME="GetNumberOfRecords">GetNumberOfRecords</A></B>( DWORD&amp; number_of_records )</PRE><DD>
This function is gets the number of records in the log.

<DT><PRE>DWORD <B><A NAME="GetOldestRecordNumber">GetOldestRecordNumber</A></B>( void )</PRE><DD>
Returns the record number of the oldest record in the log. If it returns zero,
the call failed.

<DT><PRE>BOOL <B><A NAME="NotifyChange">NotifyChange</A></B>( HANDLE event_handle, HANDLE log_handle = nullptr )</PRE><DD>
Allows you to watch a log and be notified if it changes.

<DT><PRE>BOOL <B><A NAME="OpenBackup">OpenBackup</A></B>( LPCTSTR name_of_backup_file, LPCTSTR name_of_computer = nullptr )</PRE><DD>
Opens a log that has been backed up to a file. If <CODE>name_of_computer</CODE>
is NULL, the local backup is openend.

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( LPCTSTR log_name, LPCTSTR name_of_computer = nullptr )</PRE><DD>
Opens a specified log on a specified machine. If <CODE>name_of_computer</CODE>
is NULL, the local log is openend.

<DT><PRE>BOOL <B><A NAME="Read">Read</A></B>( DWORD  record_number, 
           LPVOID buffer, 
           DWORD&amp; number_of_bytes_to_read, 
           DWORD  how_to_read = EVENTLOG_FORWARDS_READ | EVENTLOG_SEQUENTIAL_READ )
BOOL <B>Read</B>( DWORD record_number,
                  <A HREF="CEVENT~1.HTM">CEventLogRecord</A>&amp; record,
                  DWORD how_to_read = EVENTLOG_FORWARDS_READ | EVENTLOG_SEQUENTIAL_READ )</PRE><DD>
Reads a specific record from the log.

<DT><PRE>BOOL <B><A NAME="RegisterSource">RegisterSource</A></B>( LPCTSTR source_name, LPCTSTR name_of_computer )</PRE><DD>
Basically loads the event strings for <CODE>source_name</CODE>. If <CODE>name_of_computer</CODE>
is NULL, the local source is openend.

<DT><PRE>BOOL <B><A NAME="Report">Report</A></B>( EventType event_type,
             WORD      category,
             DWORD     event_id,
             WORD      number_of_strings   = 0,
             LPCTSTR * string_array        = nullptr,
             DWORD     number_of_raw_bytes = 0,
             LPVOID    raw_data_buffer     = nullptr,
             PSID      user_sid            = nullptr )</PRE><DD>
Makes an entry into the log. The <CODE>event_type</CODE> parameter
may be one of the following:

<UL>
<LI>eventError
<LI>eventWarning
<LI>eventInformation
<LI>eventSuccess
<LI>eventFailure
</UL>

<DT><PRE>BOOL <B><A NAME="ReportError">ReportError</A></B>( LPCTSTR string_to_report )</PRE><DD>
Makes an entry into the log flagged as an Error message. This is the lazy man's
way of writing to the event log. It calls <B><A HREF="#Report">Report</A></B>() with eventError.

<DT><PRE>BOOL <B><A NAME="ReportInformation">ReportInformation</A></B>( LPCTSTR string_to_report )</PRE><DD>
Makes an entry into the log flagged as an Informational message. This is the
lazy man's way of writing to the event log. It calls <B><A HREF="#Report">Report</A></B>() with
eventInformation.

</DL>

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;

void test_CEventLog( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;test_CEventLog()&quot; ) );

   <B>CEventLog</B> log( TEXT( &quot;Dodah&quot; ) );

   LPCTSTR string_array[ 1 ];

   string_array[ 0 ] = TEXT( &quot;Hello World&quot; );

   log.<A HREF="#Report">Report</A>( CEventLog::eventInformation, 0, 0, 1, string_array );
   log.<A HREF="#ReportInformation">ReportInformation</A>( TEXT( &quot;ReportInformation&quot; ) );
   log.<A HREF="#ReportError">ReportError</A>( TEXT( &quot;ReportError&quot; ) );
}</CODE></PRE>

<H2>API's Used</H2>

<UL>
<LI>BackupEventLog
<LI>ClearEventLog
<LI>CloseEventLog
<LI>DeregisterEventSource
<LI>GetComputerNameEx
<LI>GetNumberOfEventLogRecords
<LI>GetOldestEventLogRecord
<LI>GetUserName
<LI>LookupAccountName
<LI>NotifyChangeEventLog
<LI>OpenBackupEventLog
<LI>OpenEventLog
<LI>ReadEventLog
<LI>RegisterEventSource
<LI>ReportEvent
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: cevntlog.cpp $<BR>
$Modtime: 6/26/01 10:45a $

</BODY>

</HTML>
*/
