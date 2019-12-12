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
** $Workfile: serial.cpp $
** $Revision: 61 $
** $Modtime: 6/26/01 10:56a $
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

#pragma warning( disable : 4302 )

CSerialFile::CSerialFile()
{
   WFC_VALIDATE_POINTER( this );

   FileHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
   ::ZeroMemory( &m_CommunicationsStatus, sizeof( m_CommunicationsStatus ) );
   m_IsOpen             = FALSE;
   m_PurgeBufferOnError = FALSE;
   Name.clear();
   m_InputBufferSize  = 4096;
   m_OutputBufferSize = 4096;
}

CSerialFile::~CSerialFile()
{
   WFC_VALIDATE_POINTER( this );
   Close();
}

_Check_return_ BOOL CSerialFile::Attach( __in HANDLE new_handle, __out_opt HANDLE * old_handle_p ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( old_handle_p );

   if (old_handle_p != nullptr)
   {
       *old_handle_p = NULL;
   }

   // A new method as suggested by Fabio Angelini (fabioang@iol.it or nico.sw@ibm.net)
   // 1998-07-14

   ASSERT( new_handle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
#else
   ASSERT( m_hFile != reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) );
#endif

   if ( new_handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) or
#if defined( WFC_STL )
        m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
        m_hFile    == reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) )
#endif
   {
      return( FALSE );
   }

   if ( old_handle_p != nullptr )
   {
      WFC_TRY
      {
         // We were sent a pointer, don't trust it
         *old_handle_p = m_FileHandle;
      }
      WFC_CATCH_ALL
      {
         // Do Nothing
      }
      WFC_END_CATCH_ALL
   }

   m_IsOpen   = TRUE;
#if defined( WFC_STL )
   m_FileHandle = new_handle;
#else
   m_hFile    = reinterpret_cast< CFILE_HFILE >( new_handle );
#endif
   FileHandle = new_handle;

   return( TRUE );
}

_Check_return_ BOOL CSerialFile::CancelWaitFor( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   BOOL return_value = ::SetCommMask( m_FileHandle, 0 );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACE( TEXT( "Can't set comm mask" ) );
      //WFCTRACEERROR( m_LastError );
   }

   return( return_value );
}

_Check_return_ BOOL CSerialFile::ClearBreak( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
#endif

#if defined( WFC_STL )
   if ( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   if ( m_hFile == reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( FALSE );
   }

   BOOL return_value = ::ClearCommBreak( m_FileHandle );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value );
}

void CSerialFile::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   if ( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   if ( m_hFile == reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) )
#endif
   {
      m_IsOpen = FALSE;
      return;
   }

   m_ClearError( __LINE__ );

   (void) Purge( purgeAll );

   ::EscapeCommFunction( m_FileHandle, CLRDTR );

#if defined( WFC_STL )
   if ( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   if ( m_hFile != reinterpret_cast< CFILE_HFILE >( CFile::hFileNull ) )
#endif
   {
      CDummyFile::Close();
   }

   FileHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
   m_IsOpen   = FALSE;
}

_Check_return_ bool CSerialFile::ConfigurationDialog( __inout CCommunicationsConfiguration& configuration, _In_ bool const save_changes, _In_ HWND const parent_window_handle ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   if ( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   if ( m_hFile == reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( false );
   }

   // First, find out how large a buffer we need for the configuration data...

   CCommunicationProperties properties;

   BOOL return_value = GetProperties( properties );

   if ( return_value == FALSE )
   {
      return( false );
   }

   return_value = GetConfiguration( configuration );

   if ( return_value == FALSE )
   {
      return( FALSE );
   }

   std::size_t colon_location = Name.find( TEXT( ':' ) );

   if ( colon_location == std::wstring::npos )
   {
      return( false );
   }

   std::wstring device_name( Name.substr(0, colon_location ) );

   auto allocated_bytes = std::make_unique<uint8_t[]>(8192);

   auto buffer = allocated_bytes.get();

   if ( buffer == nullptr )
   {
      return( false );
   }

   // Always, always always zeroize the buffer
   ZeroMemory( buffer, 8192 );

   auto communications_configuration_p = reinterpret_cast< LPCOMMCONFIG >( buffer );

   communications_configuration_p->dwSize        = 8192;
   communications_configuration_p->dcb.DCBlength = sizeof( DCB );

   return_value = ::CommConfigDialogW( device_name.c_str(), parent_window_handle, reinterpret_cast< LPCOMMCONFIG >( buffer ) );

   // Alexander Kiesl [Alexander.Kiesl@mch.sni.de] found a bug here.
   // Apparently someone at Microsoft left the door to the day care
   // center and a child got into the source code again. CommConfigDialog()
   // is defined and documented as a BOOL function but it returns
   // something other than TRUE and FALSE. The grown ups at Microsoft
   // should be more careful... This caused me to edit this entire
   // class. Children should learn the meaning of BOOL.

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      return( false );
   }

   configuration.Copy( *( reinterpret_cast< LPCOMMCONFIG >( buffer ) ) );

   if ( save_changes != FALSE )
   {
      communications_configuration_p->dcb.DCBlength = sizeof( DCB );

      CDeviceControlBlock device_control_block;

      device_control_block.Copy( communications_configuration_p->dcb );

      // I don't know why this doesn't work. It keeps returning ERROR_INVALID_PARAMETER but
      // there's no way to tell *WHICH* parameter was invalid.

      return_value = ::SetCommConfig( m_FileHandle, communications_configuration_p, 8192 );

      if ( return_value == FALSE )
      {
         m_LastError = ::GetLastError();
         //WFCTRACEERROR( m_LastError );

         // Heck, let's give SetState() a try.

         if ( SetState( device_control_block ) == FALSE )
         {
            m_LastError = ::GetLastError();
            //WFCTRACEERROR( m_LastError );

            return( false );
         }

         return( true );
      }
   }

   return( true );
}

_Check_return_ HANDLE CSerialFile::Detach( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // A new method as suggested by Fabio Angelini (fabioang@iol.it or nico.sw@ibm.net)
   // 1998-07-14

   HANDLE return_value = m_FileHandle;

   m_IsOpen   = FALSE;
#if defined( WFC_STL )
   m_FileHandle = reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE );
#else
   m_hFile    = reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE );
#endif
   FileHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );

   return( return_value );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CSerialFile::Dump( CDumpContext& dump_context ) const
{
   CDummyFile::Dump( dump_context );

   dump_context << TEXT( "m_IsOpen                  = " );
   
   if ( m_IsOpen == FALSE )
   {
      dump_context << TEXT( "FALSE\n" );
   }
   else
   {
      dump_context << TEXT( "TRUE\n" );
   }

   dump_context << TEXT( "m_PurgeBufferOnError      = " );
   
   if ( m_PurgeBufferOnError == FALSE )
   {
      dump_context << TEXT( "FALSE\n" );
   }
   else
   {
      dump_context << TEXT( "TRUE\n" );
   }

   dump_context << TEXT( "m_CommunicationErrorCodes = " ) << m_CommunicationErrorCodes;

   if ( m_CommunicationErrorCodes != 0 )
   {
      dump_context << TEXT( " ( " );

      if ( m_CommunicationErrorCodes bitand CE_BREAK )
      {
         dump_context << TEXT( "CE_BREAK " );
      }

      if ( m_CommunicationErrorCodes bitand CE_FRAME )
      {
         dump_context << TEXT( "CE_FRAME " );
      }

      if ( m_CommunicationErrorCodes bitand CE_IOE )
      {
         dump_context << TEXT( "CE_IOE " );
      }

      if ( m_CommunicationErrorCodes bitand CE_MODE )
      {
         dump_context << TEXT( "CE_MODE " );
      }

      if ( m_CommunicationErrorCodes bitand CE_OVERRUN )
      {
         dump_context << TEXT( "CE_OVERRUN " );
      }

      if ( m_CommunicationErrorCodes bitand CE_RXOVER )
      {
         dump_context << TEXT( "CE_RXOVER " );
      }

      if ( m_CommunicationErrorCodes bitand CE_RXPARITY )
      {
         dump_context << TEXT( "CE_RXPARITY " );
      }

      if ( m_CommunicationErrorCodes bitand CE_TXFULL )
      {
         dump_context << TEXT( "CE_TXFULL " );
      }

      if ( m_CommunicationErrorCodes bitand CE_DNS )
      {
         dump_context << TEXT( "CE_DNS " );
      }

      if ( m_CommunicationErrorCodes bitand CE_PTO )
      {
         dump_context << TEXT( "CE_PTO " );
      }

      if ( m_CommunicationErrorCodes bitand CE_OOP )
      {
         dump_context << TEXT( "CE_OOP " );
      }

      dump_context << TEXT( ")\n" );
   }
   else
   {
      dump_context << TEXT( "\n" );
   }

   dump_context << TEXT( "m_CommunicationsStatus is a COMSTAT:\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   fCtsHold  = " ) << m_CommunicationsStatus.fCtsHold  << TEXT( "\n" );
   dump_context << TEXT( "   fDsrHold  = " ) << m_CommunicationsStatus.fDsrHold  << TEXT( "\n" );
   dump_context << TEXT( "   fRlsdHold = " ) << m_CommunicationsStatus.fRlsdHold << TEXT( "\n" );
   dump_context << TEXT( "   fXoffHold = " ) << m_CommunicationsStatus.fXoffHold << TEXT( "\n" );
   dump_context << TEXT( "   fXoffSent = " ) << m_CommunicationsStatus.fXoffSent << TEXT( "\n" );
   dump_context << TEXT( "   fEof      = " ) << m_CommunicationsStatus.fEof      << TEXT( "\n" );
   dump_context << TEXT( "   fTxim     = " ) << m_CommunicationsStatus.fTxim     << TEXT( "\n" );
   dump_context << TEXT( "   fReserved = " ) << m_CommunicationsStatus.fReserved << TEXT( "\n" );
   dump_context << TEXT( "   cbInQue   = " ) << m_CommunicationsStatus.cbInQue   << TEXT( "\n" );
   dump_context << TEXT( "   cbOutQue  = " ) << m_CommunicationsStatus.cbOutQue  << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
   dump_context << TEXT( "m_InputBufferSize  = " ) << m_InputBufferSize  << TEXT( "\n" );
   dump_context << TEXT( "m_OutputBufferSize = " ) << m_OutputBufferSize << TEXT( "\n" );
}

#endif // _DEBUG

_Check_return_ DWORD CSerialFile::GetBaudRate( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) );
   if ( m_hFile == reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( 0 );
   }

   CDeviceControlBlock device_control_block;

   if ( GetState( device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      ASSERT( FALSE );
      return( 0 );
   }

   return( device_control_block.BaudRate );
}

_Check_return_ bool CSerialFile::GetConfiguration( __inout CCommunicationsConfiguration& configuration ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) );
   if ( m_hFile == reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( false );
   }

   configuration.Empty();

   DWORD buffer_size = 8 * 1024;

   auto buffer = std::make_unique<uint8_t[]>(buffer_size);

   auto buffer_p = buffer.get();

   if ( buffer_p == nullptr )
   {
      return( false );
   }

   // Always, always always zeroize the buffer
   ZeroMemory( buffer_p, buffer_size );

   auto communications_configuration_p = reinterpret_cast< LPCOMMCONFIG >( buffer_p );

   communications_configuration_p->dwSize        = buffer_size;
   communications_configuration_p->dcb.DCBlength = sizeof( DCB );

   BOOL const return_value = ::GetCommConfig( m_FileHandle, communications_configuration_p, &buffer_size );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      return( false );
   }

   configuration.Copy( *( reinterpret_cast< LPCOMMCONFIG >( buffer_p ) ) );

   return( true );
}

_Check_return_ DWORD CSerialFile::GetFlowControl( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) );
   if ( m_hFile == reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( 0 );
   }

   DWORD flow_control = 0;

   CDeviceControlBlock device_control_block;

   if ( GetState( device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      ASSERT( FALSE );
      return( 0 );
   }

   if ( device_control_block.fOutxDsrFlow != FALSE and
        device_control_block.fOutxCtsFlow != FALSE )
   {
      flow_control = flowHardware;
   }

   if ( device_control_block.fInX  != FALSE and
        device_control_block.fOutX != FALSE )
   {
      flow_control or_eq flowSoftware;
   }

   return( flow_control );
}

_Check_return_ DWORD CSerialFile::GetInputBufferSize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // A better solution supplied by Fabio Angelini (fabioang@iol.it or nico.sw@ibm.net)
   // 1998-07-14

   CCommunicationProperties properties;

   if ( GetProperties( properties ) != FALSE )
   {
      return( properties.dwCurrentRxQueue );
   }
   else
   {
      return( 0 );
   }
}

_Check_return_ DWORD CSerialFile::GetModemStatus( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) );
   if ( m_hFile == reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( 0 );
   }

   DWORD modem_status = 0;

   BOOL return_value = ::GetCommModemStatus( m_FileHandle, &modem_status );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
   }
   else
   {
      return( modem_status );
   }

   return( 0 );
}

_Check_return_ DWORD CSerialFile::GetOutputBufferSize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // A better solution supplied by Fabio Angelini (fabioang@iol.it or nico.sw@ibm.net)
   // 1998-07-14

   CCommunicationProperties properties;

   if ( GetProperties( properties ) != FALSE )
   {
      return( properties.dwCurrentTxQueue );
   }
   else
   {
      return( 0 );
   }
}

_Check_return_ BOOL CSerialFile::GetProperties( __inout CCommunicationProperties& properties ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Choose to live

#if defined( WFC_STL )
   ASSERT( m_FileHandle != reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) );
   if ( m_hFile == reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) )
#endif
   {
      properties.Empty();
      m_LastError = ERROR_INVALID_HANDLE;
      return( FALSE );
   }

   BOOL return_value = ::GetCommProperties( m_FileHandle, &properties );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      properties.Empty();
      return( FALSE );
   }

   return( TRUE );
}

_Check_return_ BOOL CSerialFile::GetState( __inout CDeviceControlBlock& device_control_block ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
#else
   ASSERT( m_hFile != reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) );
#endif

   if ( ::GetCommState( m_FileHandle, &device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      return( FALSE );
   }

   return( TRUE );
}

_Check_return_ BOOL CSerialFile::GetTimeouts( __inout COMMTIMEOUTS& timeouts ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Choose to live

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      ::ZeroMemory( &timeouts, sizeof( COMMTIMEOUTS ) );
      m_LastError = ERROR_INVALID_HANDLE;
      return( FALSE );
   }

   BOOL return_value = ::GetCommTimeouts( m_FileHandle, &timeouts );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value );
}

#if defined( _DEBUG )
void CSerialFile::m_ClearError( __in int const line_number ) noexcept
#else
void CSerialFile::m_ClearError(__in int const ) noexcept
#endif // _DEBUG
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return;
   }

   if ( ::ClearCommError( m_FileHandle, &m_CommunicationErrorCodes, &m_CommunicationsStatus ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );

      if ( m_LastError == ERROR_INVALID_HANDLE )
      {
#if ! defined( WFC_STL )
         m_hFile    = (CFILE_HFILE) INVALID_HANDLE_VALUE;
#endif
         FileHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
      }

      return;
   }

   if ( m_CommunicationErrorCodes != 0 )
   {
#if defined( _DEBUG )
      TCHAR error_message[ 2048 ];

      ::ZeroMemory( error_message, sizeof( error_message ) );

      if ( m_CommunicationErrorCodes bitand CE_BREAK )
      {
         _tcscat_s( error_message, std::size( error_message ), TEXT( "\r\nThe hardware detected a break condition. (CE_BREAK);" ) );
      }

#endif // _DEBUG

      if ( is_flagged(m_CommunicationErrorCodes, CE_FRAME) == true )
      {
#if defined( _DEBUG )
         _tcscat_s( error_message, std::size( error_message ), TEXT( "\r\nThe hardware detected a framing error. (CE_FRAME);" ) );
#endif // _DEBUG

         if ( m_PurgeBufferOnError == true )
         {
            (void) Purge( purgeClearInputBuffer );

            // Thanks go to Fabio Angelini (fabioang@iol.it or nico.sw@ibm.net)
            // for suggesting this change
            // 1998-07-14

            m_CommunicationsStatus.cbInQue = 0;
         }
      }

#if defined( _DEBUG )

      if ( m_CommunicationErrorCodes bitand CE_IOE )
      {
         _tcscat_s( error_message, std::size( error_message ), TEXT( "\r\nAn I/O error occurred during communications with the device. (CE_IOE);" ) );
      }

      if ( m_CommunicationErrorCodes bitand CE_MODE )
      {
         _tcscat_s( error_message, std::size( error_message ), TEXT( "\r\nThe requested mode is not supported or the hCommDev parameter is invalid. (CE_MODE);" ) );
      }

      if ( m_CommunicationErrorCodes bitand CE_OVERRUN )
      {
         _tcscat_s( error_message, std::size( error_message ), TEXT( "\r\nA character-buffer overrun has occurred. The next character is lost. (CE_OVERRUN);" ) );
      }

#endif // _DEBUG

      if ( is_flagged(m_CommunicationErrorCodes, CE_RXOVER) == true )
      {
         m_LastError = CE_RXOVER;
#if defined( _DEBUG )
         _tcscat_s( error_message, std::size( error_message ), TEXT( "\r\nAn input buffer overflow has occurred. There is either no room in the input buffer, or a character was received after the end-of-file (EOF) character. (CE_RXOVER);" ) );
#endif // _DEBUG
      }

#if defined( _DEBUG )

      if ( m_CommunicationErrorCodes bitand CE_RXPARITY )
      {
         _tcscat_s( error_message, std::size( error_message ), TEXT( "\r\n.The hardware detected a parity error. (CE_RXPARITY);" ) );
      }

      if ( m_CommunicationErrorCodes bitand CE_TXFULL )
      {
         _tcscat_s( error_message, std::size( error_message ), TEXT( "\r\nThe application tried to transmit a character, but the output buffer was full. (CE_TXFULL);" ) );
      }

      if ( m_CommunicationErrorCodes bitand CE_DNS )
      {
         _tcscat_s( error_message, std::size( error_message ), TEXT( "\r\nThe parallel device is not selected. (CE_DNS);" ) );
      }

      if ( m_CommunicationErrorCodes bitand CE_PTO )
      {
         _tcscat_s( error_message, std::size( error_message ), TEXT( "\r\nA time-out occurred on the parallel device. (CE_PTO);" ) );
      }

      if ( m_CommunicationErrorCodes bitand CE_OOP )
      {
         _tcscat_s( error_message, std::size( error_message ), TEXT( "\r\nThe parallel device signaled that it is out of paper. (CE_OOP);" ) );
      }

      TCHAR temp_string[ 100 ];

      _stprintf_s( temp_string, std::size( temp_string ), TEXT( "%d (%d in input_queue)" ), line_number, (int) m_CommunicationsStatus.cbInQue );

      //WFCTRACE( error_message );
      //WFCTRACE( temp_string   );

#endif // _DEBUG

   }
}

_Check_return_ bool CSerialFile::IsDataWaiting( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( false );
   }

   m_ClearError( __LINE__ );

   if ( m_CommunicationsStatus.cbInQue > 0 )
   {
      return( true );
   }
   else
   {
      return( false );
   }
}

_Check_return_ DWORD CSerialFile::NumberOfBytesWaitingToBeRead( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( 0 );
   }

   m_ClearError( __LINE__ );

   return( m_CommunicationsStatus.cbInQue );
}

_Check_return_ DWORD CSerialFile::NumberOfBytesWaitingToBeWritten( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( 0 );
   }

   m_ClearError( __LINE__ );

   return( m_CommunicationsStatus.cbOutQue );
}

#pragma warning( disable : 4100 )

_Check_return_ bool CSerialFile::Open(_In_ std::wstring_view channel_name, __in UINT const open_flags ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Choose to live

   if ( channel_name.empty() == true )
   {
      m_LastError = ERROR_INVALID_PARAMETER;
      return( false );
   }

   if ( m_IsOpen != FALSE )
   {
      Close();
   }

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      Name.assign( channel_name );

      return( Open() );
   }
   WFC_CATCH_ALL
   {
      m_LastError = ERROR_EXCEPTION_IN_SERVICE;
      return( false );
   }
   WFC_END_CATCH_ALL
}

#pragma warning( default : 4100 )

_Check_return_ bool CSerialFile::Open( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_IsOpen != FALSE )
   {
      Close();
   }

   // Gawl dang children... They coded CreateFile to accept COM1: through COM9:
   // the only problem is NT can have up to 255 com ports. Let's see... that
   // means CreateFile will fail 246 out of 255 times. To get around this, we
   // have to call the com port by a different name, "COM1:" becomes "\\.\COM1"
   // Why? I dunno, it was probably coded to originally take names of "\\.\COMx"
   // but someone cried until it was hacked to take "COMx:". It sure looks like
   // a compromise to me.

   std::wstring com_port_name(WSTRING_VIEW(L"\\\\.\\"));

   std::size_t index = 0;
   std::size_t string_length = Name.length();

   while( index < string_length and Name.at( index ) != ':' )
   {
      com_port_name.push_back( Name.at( index ) );
      index++;
   }

   make_upper(com_port_name );

   std::wstring settings_string( right( Name, ( string_length - index ) - 1 ) );

   // Win95 doesn't seem to like \\.\ so let's trim it if we can.
   // 0123456
   // \\.\COMxxx

   index = 7;

   std::wstring number_string;

   std::size_t com_port_name_string_length = com_port_name.length();

   while( index < com_port_name_string_length )
   {
      if ( com_port_name.at( index ) >= '0' and
           com_port_name.at( index ) <= '9' )
      {
         number_string.push_back( com_port_name.at( index ) );
         index++;
      }
      else
      {
         index = com_port_name_string_length;
      }
   }

   int const port_number = static_cast<int>(as_integer(number_string));

   if ( port_number > 0 and port_number < 10 )
   {
      // Yep, we've got a COM1 through COM9 so strip our \\.\ prefix

      // com_port_name = com_port_name.Right( com_port_name_string_length - 4 );
      format( com_port_name, L"COM%d:", port_number );
   }

   // Thanks go to Fabio Angelini (fabioang@iol.it or nico.sw@ibm.net)
   // for suggesting the functions instead of contants.
   // 1998-07-14

   FileHandle = ::CreateFileW( com_port_name.c_str(),
                              m_CreateDesiredAccess(),
                              m_CreateShareMode(),
                              m_CreateSecurityAttributes(),
                              m_CreateCreationDistribution(),
                              m_CreateFlagsAndAttributes(),
                              m_CreateTemplateHandle() );

   if ( FileHandle == INVALID_HANDLE_VALUE )
   {
      m_LastError = ::GetLastError();

      m_FileHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );

      //WFCTRACEERROR( m_LastError );
      return( false );
   }

   m_FileHandle = FileHandle;

   // 2000-08-11
   // Thanks go to Harold Yang (harold@digitcom.net) for finding a bug
   // in the STL build. My STL version of CFile uses m_FileHandle instead
   // of m_hFile. I wasn't setting this handle. DOH!

   if ( SetTimeouts() == FALSE )
   {
      //WFCTRACE( TEXT( "SetTimeouts failed." ) );
   }

   if ( Purge( purgeAll ) == FALSE )
   {
      //WFCTRACE( TEXT( "Purge failed." ) );
   }

   std::wstring baud_string;

   index = 0;
   string_length = settings_string.length();

   while( index < string_length and settings_string.at( index ) != ',' )
   {
      baud_string.push_back( settings_string.at( index ) );
      index++;
   }

   index++; // Skip over the comma

   std::wstring parity_string;

   while( index < string_length and settings_string.at( index ) != ',' )
   {
      parity_string.push_back( settings_string.at( index ) );
      index++;
   }

   index++; // skip the comma

   std::wstring data_bits_string;

   while( index < string_length and settings_string.at( index ) != ',' )
   {
      data_bits_string.push_back( settings_string.at( index ) );
      index++;
   }

   index++; // another dang comma

   std::wstring stop_bits_string;

   while( index < string_length )
   {
      stop_bits_string.push_back( settings_string.at( index ) );
      index++;
   }

   // Need to add idiot proofing here...
   // check each setting for validity...

   std::wstring dcb_string(WSTRING_VIEW(L"baud="));
   dcb_string.append( baud_string );
   dcb_string.append(WSTRING_VIEW(L" parity="));
   dcb_string.append( parity_string );
   dcb_string.append(WSTRING_VIEW(L" data="));
   dcb_string.append( data_bits_string );
   dcb_string.append(WSTRING_VIEW(L" stop="));
   dcb_string.append( stop_bits_string );

   CDeviceControlBlock device_control_block;

   if ( GetState( device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      Close();
      return( FALSE );
   }

   if ( ::BuildCommDCBW( dcb_string.c_str(), &device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      Close();
      return( false );
   }

   device_control_block.fAbortOnError = FALSE; // Terminate Reads & Writes if there's an error
   device_control_block.fErrorChar    = TRUE;  // Replace any garbled bytes with ErrorChar
   device_control_block.ErrorChar     = (char) ' '; // Garbage bytes are spaces
   device_control_block.fBinary       = TRUE;  // Ignore EOF

   if ( SetState( device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      Close();
      return( false );
   }

   if ( ::SetupComm( m_FileHandle, m_InputBufferSize, m_OutputBufferSize ) == FALSE )
   {
      //WFCTRACE( TEXT( "SetupComm failed." ) );
   }

   // assert DTR

   (void) SetDataTerminalReady( true );

   m_IsOpen = true;

#if ! defined( WFC_STL )
   m_strFileName = Name;
#else // WFC_STL
   m_FileName.assign( Name );
#endif // WFC_STL

   return( true );
}

_Check_return_ UINT CSerialFile::Read( __out_bcount( length ) void * buffer, __in UINT const length ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( buffer );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( buffer == nullptr or
       m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( buffer == nullptr or
	   m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( 0 );
   }

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      m_ClearError( __LINE__ );

      return( CDummyFile::Read( buffer, length ) );
   }
   WFC_CATCH_ALL
   {
      m_LastError = ERROR_EXCEPTION_IN_SERVICE;
      return( 0 );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ BOOL CSerialFile::Purge( __in DWORD const what_to_purge ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( FALSE );
   }

   BOOL return_value = ::PurgeComm( m_FileHandle, what_to_purge );

   if ( return_value == FALSE )
   {
      // 1998-09-15 Thanks go to Jim Peng (jimp@datalink.net) for
      // finding a type here. I was setting return_value to the
      // value returned by GetLastError(). I should have been
      // setting m_LastError. DOH!

      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value );
}

void CSerialFile::ReplaceGarbledCharacter( __in bool const yes_or_no, __in uint8_t const character_to_replace_the_garbled_one_with ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   CDeviceControlBlock device_control_block;

   if ( GetState( device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      return;
   }

   device_control_block.fErrorChar = yes_or_no == true ? 1 : 0;  // Replace any garbled bytes with ErrorChar
   device_control_block.ErrorChar  = character_to_replace_the_garbled_one_with;

   if ( SetState( device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      return;
   }
}

_Check_return_ bool CSerialFile::SetBaudRate( __in uint32_t const baud_rate ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( false );
   }

   CDeviceControlBlock device_control_block;

   if ( GetState( device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      return( false );
   }

   device_control_block.BaudRate = baud_rate;

   if ( SetState( device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      return( false );
   }

   return( true );
}

_Check_return_ BOOL CSerialFile::SetBreak( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( FALSE );
   }

   BOOL return_value = ::SetCommBreak( m_FileHandle );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value );
}

_Check_return_ BOOL CSerialFile::SetCharacterToWaitFor( __in uint8_t const character_to_wait_for ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( FALSE );
   }

   CDeviceControlBlock device_control_block;

   if ( GetState( device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      return( FALSE );
   }

   device_control_block.EvtChar = character_to_wait_for;

   if ( SetState( device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      return( FALSE );
   }

   return( TRUE );
}

_Check_return_ BOOL CSerialFile::SetDataTerminalReady( _In_ bool const set_DTR_on ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( FALSE );
   }

   BOOL return_value = FALSE;

   if ( set_DTR_on == true )
   {
      //WFCTRACE( TEXT( "Setting DTR on" ) );
      return_value = ::EscapeCommFunction( m_FileHandle, SETDTR );
   }
   else
   {
      //WFCTRACE( TEXT( "Setting DTR off" ) );
      return_value = ::EscapeCommFunction( m_FileHandle, CLRDTR );
   }

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
   }
   else
   {
      return_value = TRUE;
   }

   //WFCTRACEVAL( TEXT( "returning " ), return_value );

   return( return_value );
}

void CSerialFile::SetFlowControl( _In_ DWORD const flow_control ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return;
   }

   CDeviceControlBlock device_control_block;

   if ( GetState( device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      ASSERT( FALSE );
      return;
   }

   // Changes by Fabio Angelini (fabioang@iol.it or nico.sw@ibm.net)
   // to get hardware flow control work properly
   // 1998-07-14

   if ( is_flagged( flow_control, flowRtsCts ) == true )
   {
      device_control_block.fRtsControl  = RTS_CONTROL_HANDSHAKE;
      device_control_block.fOutxCtsFlow = TRUE;
   }
   else
   {
      device_control_block.fRtsControl  = RTS_CONTROL_DISABLE;
      device_control_block.fOutxCtsFlow = FALSE;
   }

   if ( is_flagged(flow_control, flowDtrDsr) == true )
   {
      device_control_block.fDtrControl  = DTR_CONTROL_HANDSHAKE;
      device_control_block.fOutxDsrFlow = TRUE;
   }
   else
   {
      device_control_block.fDtrControl  = DTR_CONTROL_DISABLE;
      device_control_block.fOutxDsrFlow = FALSE;
   }

   if ( is_flagged(flow_control, flowSoftware) == true )
   {
      device_control_block.fInX     = TRUE;
      device_control_block.fOutX    = TRUE;
      device_control_block.XonChar  = 0x11;
      device_control_block.XoffChar = 0x13;
      device_control_block.XonLim   = 100;
      device_control_block.XoffLim  = 100;
   }
   else
   {
      device_control_block.fInX     = FALSE;
      device_control_block.fOutX    = FALSE;
      device_control_block.XonChar  = 0;
      device_control_block.XoffChar = 0;
      device_control_block.XonLim   = 0;
      device_control_block.XoffLim  = 0;
   }

   if ( SetState( device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      ASSERT( FALSE );
      return;
   }
}

_Check_return_ bool CSerialFile::SetPurgeBufferOnError( __in bool const purge_buffer ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto return_value = m_PurgeBufferOnError;

   m_PurgeBufferOnError = purge_buffer;

   return( return_value );
}

_Check_return_ BOOL CSerialFile::SetRequestToSend( __in BOOL const set_RTS_on ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( FALSE );
   }

   BOOL return_value = FALSE;

   if ( set_RTS_on != FALSE )
   {
      //WFCTRACE( TEXT( "Setting RTS on" ) );
      return_value = ::EscapeCommFunction( m_FileHandle, SETRTS );
   }
   else
   {
      //WFCTRACE( TEXT( "Setting RTS off" ) );
      return_value = ::EscapeCommFunction( m_FileHandle, CLRRTS );
   }

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value );
}

_Check_return_ BOOL CSerialFile::SetState(__inout CDeviceControlBlock& device_control_block ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( ::SetCommState( m_FileHandle, &device_control_block ) == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
      return( FALSE );
   }

   return( TRUE );
}

_Check_return_ BOOL CSerialFile::SetTimeouts( __in_opt COMMTIMEOUTS const * timeouts_p ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( timeouts_p );

   // Choose to live

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( FALSE );
   }

   COMMTIMEOUTS communications_timeouts;

   ::ZeroMemory( &communications_timeouts, sizeof( communications_timeouts ) );

   if ( timeouts_p != nullptr )
   {
      // We were passed a pointer, do not trust it

      WFC_TRY
      {
         communications_timeouts.ReadIntervalTimeout         = timeouts_p->ReadIntervalTimeout;
         communications_timeouts.ReadTotalTimeoutMultiplier  = timeouts_p->ReadTotalTimeoutMultiplier;
         communications_timeouts.ReadTotalTimeoutConstant    = timeouts_p->ReadTotalTimeoutConstant;
         communications_timeouts.WriteTotalTimeoutMultiplier = timeouts_p->WriteTotalTimeoutMultiplier;
         communications_timeouts.WriteTotalTimeoutConstant   = timeouts_p->WriteTotalTimeoutConstant;
      }
      WFC_CATCH_ALL
      {
         communications_timeouts.ReadIntervalTimeout         = MAXDWORD;
         communications_timeouts.ReadTotalTimeoutMultiplier  = 0;
         communications_timeouts.ReadTotalTimeoutConstant    = 0;
         communications_timeouts.WriteTotalTimeoutMultiplier = 0;
         communications_timeouts.WriteTotalTimeoutConstant   = 0;
      }
      WFC_END_CATCH_ALL
   }
   else
   {
      // Use our defaults
      communications_timeouts.ReadIntervalTimeout         = MAXDWORD;
      communications_timeouts.ReadTotalTimeoutMultiplier  = 0;
      communications_timeouts.ReadTotalTimeoutConstant    = 0;
      communications_timeouts.WriteTotalTimeoutMultiplier = 0;
      communications_timeouts.WriteTotalTimeoutConstant   = 0;
   }

   BOOL return_value = ::SetCommTimeouts( m_FileHandle, &communications_timeouts );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value );
}

_Check_return_ BOOL CSerialFile::TransmitCharacter( __in char const character_to_transmit ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Choose to live

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( FALSE );
   }

   BOOL return_value = ::TransmitCommChar( m_FileHandle, character_to_transmit );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACE( TEXT( "Can't TransmitCommChar()" ) );
      //WFCTRACEERROR( m_LastError );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value );
}

_Check_return_ BOOL CSerialFile::WaitFor(__inout DWORD& what_to_wait_for ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Choose to live

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
   if ( m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( FALSE );
   }

   BOOL return_value = ::SetCommMask( m_FileHandle, what_to_wait_for );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACE( TEXT( "Can't set comm mask" ) );
      //WFCTRACEERROR( m_LastError );
      return( return_value );
   }

   return_value = ::WaitCommEvent( m_FileHandle, &what_to_wait_for, nullptr );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACE( TEXT( "Can't wait comm event" ) );
      //WFCTRACEERROR( m_LastError );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value );
}

_Check_return_ bool CSerialFile::WaitForString( __in std::string const& string_to_wait_for, __in DWORD const seconds_to_wait, __inout_opt std::string * string_p ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( _DEBUG )

   std::string debug_string(STRING_VIEW("\"") );

   debug_string.append( string_to_wait_for );
   debug_string.push_back('\"');

   //WFCTRACEVAL( TEXT( "Waiting for " ), debug_string );

#endif // _DEBUG

   std::string response( "" );

   BYTE buffer[1]{ 0 };

   bool time_expired = false;

   std::size_t const length_of_string_to_wait_for = string_to_wait_for.length();

   CTime current_time = CTime::GetCurrentTime();

   CTime expiration_time = current_time;

   expiration_time += CTimeSpan( 0, 0, 0, seconds_to_wait + 1 );

   while( time_expired == false )
   {
      if ( IsDataWaiting() == true )
      {
         (void) Read( buffer, 1 );

         response.push_back( buffer[ 0 ] );

         // Only compare the last x characters of what came in

         if ( response.length() >= length_of_string_to_wait_for )
         {
            if ( string_to_wait_for.compare( right( response, length_of_string_to_wait_for ) ) == I_AM_EQUAL_TO_THAT )
            {
               //WFCTRACE( TEXT( "Returning TRUE" ) );

               if ( string_p != nullptr )
               {
                  // We were passed a pointer, do not trust it

                  WFC_TRY
                  {
                     string_p->assign( response );
                  }
                  WFC_CATCH_ALL
                  {
                     ;
                  }
                  WFC_END_CATCH_ALL
               }

               return( true );
            }
         }
      }
      else
      {
         // This is a cheat. The right way to do it would be to use overlapped
         // I/O but MFC doesn't support overlapped I/O because only NT
         // supports it.

          std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }

      current_time = CTime::GetCurrentTime();

      if ( current_time > expiration_time )
      {
         //WFCTRACE( TEXT( "timing out..." ) );
         time_expired = true;
      }
   }

   if ( string_p != nullptr )
   {
      // We were passed a pointer, do not trust it

      WFC_TRY
      {
         string_p->assign( response );
      }
      WFC_CATCH_ALL
      {
         ;
      }
      WFC_END_CATCH_ALL
   }

   //WFCTRACE( TEXT( "Returning FALSE" ) );

   return( false );
}

void CSerialFile::Write( __in BYTE const byte_to_write ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Write( &byte_to_write, 1 );
}

void CSerialFile::Write( __in_bcount( length ) void const * buffer, __in UINT const length ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( buffer );

#if defined( WFC_STL )
   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
#else
   ASSERT( m_hFile != (CFILE_HFILE) INVALID_HANDLE_VALUE );
#endif

   if ( buffer == nullptr or
#if defined( WFC_STL )
       m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
#else
       m_hFile == (CFILE_HFILE) INVALID_HANDLE_VALUE )
#endif
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return;
   }

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      m_ClearError( __LINE__ );

      if ( length == 0 )
      {
         return;
      }

      // Windows 95 sucks!

      DWORD number_of_bytes_written       = 0;
      DWORD total_number_of_bytes_written = 0;
      DWORD number_of_bytes_to_write      = length;

      auto temp_buffer = static_cast<uint8_t const *>(buffer);

      while( total_number_of_bytes_written < length )
      {
         if ( ::WriteFile( m_FileHandle,
                          &temp_buffer[ total_number_of_bytes_written ],
                           number_of_bytes_to_write,
                          &number_of_bytes_written,
                           nullptr ) == FALSE and
              ::GetLastError() != 0 )
         {
            // Something bad happened...
            return;
         }

         number_of_bytes_to_write      -= number_of_bytes_written;
         total_number_of_bytes_written += number_of_bytes_written;

         // 1998-09-10
         // Thanks go to Glenn Brude (d40480@algonet.se) for finding
         // a serious bug here. I was setting number_of_bytes_to_write
         // to zero which meant if everything didn't go in the first
         // call to WriteFile() nothing else would get sent.

         number_of_bytes_written = 0;
      }
   }
   WFC_CATCH_ALL
   {
      m_LastError = ERROR_EXCEPTION_IN_SERVICE;
   }
   WFC_END_CATCH_ALL
}

void CSerialFile::Write( __in std::string const& string_to_write ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ASSERT( m_FileHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   if ( m_FileHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return;
   }

   if ( string_to_write.empty() == false )
   {
      Write( string_to_write.c_str(), static_cast<UINT>( string_to_write.length() ) );
   }
}

void CSerialFile::Write( __in std::vector<uint8_t> const& bytes ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   CDummyFile::Write( bytes );
}

#pragma warning( default : 4302 )

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CSerialFile</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles non-overlapped serial communications.">
</HEAD>

<BODY>

<H1>CSerialFile : <A HREF="DumyFile.htm">CDummyFile</A></H1>
$Revision: 61 $<BR>

<HR>

<H2>Description</H2>

This class makes it easy to play with serial ports in NT. The serial
ports in Windows 95 are screwed up so this class may not work the same
way under 95 as it does in NT. I haven't had time to play with the Win95
stuff yet.

<H2>Data Members</H2>

<B>FileHandle</B> - A handle that you can use to call the native Win32
API yourself. You can modify this value but <B>CSerialFile</B> will
ignore the change. In other words, you can't break the class by
modifying this variable.

<P>

<B>Name</B> - The name passed in to the <B>Open</B>() function.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="CancelWaitFor">CancelWaitFor</A></B>( void )</PRE><DD>
<B>WaitFor</B>() is a blocking call. The thread from which
you call <B><A HREF="#WaitFor">WaitFor</A></B>() will stop running until whatever
you're waiting for happens. Sometimes you will never get what
you wait for. This means your thread will never wake up. To
get out of this problem, you can call <B>CancelWaitFor</B>()
from another thread (usually the parent thread).

<DT><PRE>BOOL <B><A NAME="ClearBreak">ClearBreak</A></B>( void )</PRE><DD>
Resumes transmission of data.

<DT><PRE>BOOL <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the serial port.

<DT><PRE>BOOL <B><A NAME="ConfigurationDialog">ConfigurationDialog</A></B>( CCommunicationsConfiguration&amp; configuration, BOOL save_changes = TRUE, HWND parent_window_handle = nullptr )</PRE><DD>
It pops up a dialogbox letting you change the parameters of the serial port.

<DT><PRE>DWORD <B><A NAME="GetBaudRate">GetBaudRate</A></B>( void )</PRE><DD>
Returns the current baud rate.

<DT><PRE>BOOL <B><A NAME="GetConfiguration">GetConfiguration</A></B>( CCommunicationsConfiguration&amp; configuration )</PRE><DD>
Retrieves the configuration of the serial port.

<DT><PRE>DWORD <B><A NAME="GetFlowControl">GetFlowControl</A></B>( void )</PRE><DD>
Retrieves the type of flow control being used.

<DT><PRE>DWORD <B><A NAME="GetInputBufferSize">GetInputBufferSize</A></B>( void ) const</PRE><DD>
Retrieves the input buffer size set via <A HREF="#SetInputBufferSize">SetInputBufferSize()</A>.

<DT><PRE>DWORD <B><A NAME="GetModemStatus">GetModemStatus</A></B>( void )</PRE><DD>
Retrieves the status of the modem if there is one. The return value is
a 32-bit wide bit field. You must and the return value with one of these
flags (defined in <CODE>WINBASE.H</CODE> to see if that status is true:
<UL>
<LI><CODE>MS_CTS_ON</CODE> - To see if the Clear to Send (CTS) pin is high
<LI><CODE>MS_DSR_ON</CODE> - To see if the Data Set Ready (DSR) pin is high
<LI><CODE>MS_RING_ON</CODE> - To see if the modem has detected a ring
<LI><CODE>MS_RLSD_ON</CODE> - To see if the Receive Line Signal Detect (RLSD) signal is on
</UL>

<DT><PRE>DWORD <B><A NAME="GetOutputBufferSize">GetOutputBufferSize</A></B>( void ) const</PRE><DD>
Retrieves the output buffer size set via SetOutputBufferSize().

<DT><PRE>BOOL <B><A NAME="GetProperties">GetProperties</A></B>( CCommunicationProperties&amp; properties )</PRE><DD>
Gets properties about the provider
of the serial port. Returns the data in a CCommunicationProperties object.

<DT><PRE>BOOL <B><A NAME="GetState">GetState</A></B>( CDeviceControlBlock&amp; device_control_block )</PRE><DD>
Gets the state of the port.

<DT><PRE>BOOL <B><A NAME="GetTimeouts">GetTimeouts</A></B>( COMMTIMEOUTS&amp; timeouts )</PRE><DD>
Retrieves the timeouts (COMMTIMEOUTS).

<DT><PRE>BOOL <B><A NAME="IsDataWaiting">IsDataWaiting</A></B>( void )</PRE><DD>
Returns TRUE if there is data
waiting to be read. If the number of bytes in the input queue is zero
then it will return FALSE.

<DT><PRE>DWORD <B><A NAME="NumberOfBytesWaitingToBeRead">NumberOfBytesWaitingToBeRead</A></B>( void )</PRE><DD>Hhhhmmm, Gee I wonder...

<DT><PRE>DWORD <B><A NAME="NumberOfBytesWaitingToBeWritten">NumberOfBytesWaitingToBeWritten</A></B>( void )</PRE><DD>
Hhhhmmm, Gee I wonder...

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( void )
BOOL <B>Open</B>( LPCTSTR channel_name, UINT flags = 0, CFileException * exception_p = nullptr )</PRE><DD>
Opens the serial port. You pass the name of the
serial port in the &quot;COM1:9600,n,8,1&quot; format.

<DT><PRE>BOOL <B><A NAME="Purge">Purge</A></B>( DWORD what_to_purge = purgeAll )</PRE><DD>
Purges data in the system buffers. <CODE>what_to_purge</CODE> can be
one or a combination of the following:

<UL>
<LI>purgeTerminateWriteOperation
<LI>purgeTerminateReadOperation
<LI>purgeClearInputBuffer
<LI>purgeClearOutputBuffer
<LI>purgeAll
</UL>

<DT><PRE>UINT <B><A NAME="Read">Read</A></B>( void *buffer, UINT length )</PRE><DD>
Reads data from the serial port.

<DT><PRE>BOOL <B><A NAME="ReplaceGarbledCharacter">ReplaceGarbledCharacter</A></B>( yes_or_no = TRUE, BYTE replacement_character = ' ' )</PRE><DD>
You can tell the serial driver
to automagically replace garbled characters with a known character. The <B>Open</B>()
function sets this mode on replacing garbles with spaces (' ').

<DT><B><A NAME="Serialize">Serialize</A></B><DD>Saves/restores the object.

<DT><PRE>BOOL <B><A NAME="SetBaudRate">SetBaudRate</A></B>( DWORD baud_rate )</PRE><DD>
Changes the current baud rate to <CODE>baud_rate</CODE>.

<DT><PRE>BOOL <B><A NAME="SetBreak">SetBreak</A></B>( void )</PRE><DD>
Halts transmission of data. You must call
<B>ClearBreak</B>() to resume data IO. These are handy functions to use if you
have a lot of processing you need to perform on the data received from the
serial port.

<DT><PRE>BOOL <B><A NAME="SetCharacterToWaitFor">SetCharacterToWaitFor</A></B>( uint8_t character_to_wait_for )</PRE><DD>
Sets the character to wait for when you <B>WaitFor</B>() with
<CODE>waitParticularCharacterReceived</CODE> flag.

<DT><PRE>BOOL <B><A NAME="SetDataTerminalReady">SetDataTerminalReady</A></B>( BOOL set_DTR_on = TRUE )</PRE><DD>
Sets the DTR pin high (TRUE) or low (FALSE).

<DT><PRE>void <B><A NAME="SetFlowControl">SetFlowControl</A></B>( DWORD flow_control )</PRE><DD>
Sets the serial port to use the type of flow control you specify.

<DT><PRE>void <B><A NAME="SetInputBufferSize">SetInputBufferSize</A></B>( DWORD buffer_size )</PRE><DD>
Sets the size of the system input
buffer for the serial port. Make this call before you <B>Open</B>() the port. The default
is 4096.

<DT><PRE>void <B><A NAME="SetOutputBufferSize">SetOutputBufferSize</A></B>( DWORD buffer_size )</PRE><DD>
Sets the size of the system output
buffer for the serial port. Make this call before you <B>Open</B>() the port. The default
is 4096.

<DT><PRE>BOOL <B><A NAME="SetPurgeBufferOnError">SetPurgeBufferOnError</A></B>( BOOL purge_buffer = TRUE )</PRE><DD>
This method lets you specify whether or not the buffer will be
purged when an error occurs. This usually happened when the hardware
detected a framing error. The default is to NOT purge the buffer. The return
value is the previous state of the setting.

<DT><PRE>BOOL <B><A NAME="SetRequestToSend">SetRequestToSend</A></B>( BOOL set_RTS_on = TRUE )</PRE><DD>
Sets the RTS pin high (TRUE) or low (FALSE).

<DT><PRE>BOOL <B><A NAME="SetState">SetState</A></B>( CDeviceControlBlock&amp; device_control_block )</PRE><DD>
Sets the state of the port.

<DT><PRE>BOOL <B><A NAME="SetTimeouts">SetTimeouts</A></B>( COMMTIMEOUTS * timeouts_p = nullptr )</PRE><DD>
Sets the timeouts (COMMTIMEOUTS).

<DT><PRE>BOOL <B><A NAME="TransmitCharacter">TransmitCharacter</A></B>( char character_to_transmit )</PRE><DD>
Will interrupt the transmission of any buffered data and send
<CODE>character_to_transmit</CODE> immediately. This is kind of dangerous
to do.

<DT><PRE>BOOL <B><A NAME="WaitFor">WaitFor</A></B>( DWORD&amp; stuff_you_can_wait_for )</PRE><DD>
Stops execution of your thread until one of the events has occured. The 
<CODE>stuff_you_can_wait_for</CODE> parameter can be one of the following:

<UL>
<LI>waitBreakDetected
<LI>waitClearToSendChangedState
<LI>waitDataSetReadyChangedState
<LI>waitLineStatusError
<LI>waitRing
<LI>waitReceiveLineSignalDetectChangedState
<LI>waitAnyCharacterReceived
<LI>waitParticularCharacterReceived
<LI>waitTransmitBufferEmpty
</UL>

<DT><PRE>BOOL <B><A NAME="WaitForString">WaitForString</A></B>( const std::wstring&amp; string_to_wait_for, DWORD timeout_seconds = 5, std::wstring * what_was_read )</PRE><DD>
This handy dandy routine will wait
for a specified time for a specified string to come into the serial port.
It is good for dealing with text devices.

<DT><PRE>void <B><A NAME="Write">Write</A></B>( BYTE byte_to_write )
void <B>Write</B>( const std::vector&lt;uint8_t&gt;&amp; array_of_bytes )
void <B>Write</B>( std::wstring&amp; string_to_write )
void <B>Write</B>( const void * buffer, UINT number_of_bytes )</PRE><DD>
Sends data to the serial port. You can send
data using a std::wstring, std::vector&lt;uint8_t&gt; or plain old void pointer. Writing a single
BYTE differs from <B>TransmitCharacter</B>() in that writing the BYTE queues
the byte to be transmitted.
</DL>

<H2>Example</H2><PRE><CODE>void wait_for_ring( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;wait_for_ring()&quot; ) );

   <B>CSerialFile</B> serial;

   while( 1 )
   {
      _tprintf( TEXT( &quot;Opening serial port\n&quot; ) );

      if ( serial.Open( TEXT( &quot;COM1:57600,n,8,1&quot; ) ) != FALSE )
      {
         std::wstring response;

         response = TEXT( &quot;ATM0\r\n&quot; );

         serial.Write( response );

         // See if the phone rang

         while ( serial.WaitForString( TEXT( &quot;RING&quot; ), 2, &amp;response ) == FALSE )
         {
            if ( response.GetLength() &gt; 0 )
            {
               _tprintf( TEXT( &quot;Didn't get RING, going to sleep \&quot;%s\&quot;\n&quot; ), (LPCTSTR) response );
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
         }

         _tprintf( TEXT( &quot;Received a RING, waiting for them to end\n&quot; ) );

         while( serial.WaitForString( TEXT( &quot;RING&quot; ), 7 ) != FALSE )
         {
            // Do Nothing
         }

         _tprintf( TEXT( &quot;Closing serial port\n&quot; ) );

         serial.Close();

         CTime time_now( CTime::GetCurrentTime() );

         std::this_thread::sleep_for(std::chrono::milliseconds(1000));

         std::wstring message;

         message.Format( TEXT( &quot;The phone rang at %s&quot; ), (LPCTSTR) time_now.Format( &quot;%H:%M:%S %d %b %y&quot; ) );

         CSprintSpectrum pager;

         pager.SetAccount( TEXT( &quot;4101234567&quot; ) );
         pager.SetMessage( message );

         _tprintf( TEXT( &quot;Calling pager.Send()\n&quot; ) );
         pager.Send();
      }
      else
      {
         _tprintf( TEXT( &quot;Can't open serial port\n&quot; ) );
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   }
}</CODE></PRE>

<H2>API's Used</H2>

<B>CSerialFile</B> encapsulates the following API's:

<UL>
<LI>BuildCommDCB
<LI>ClearCommBreak
<LI>ClearCommError
<LI>CommConfigDialog
<LI>CreateFile
<LI>EscapeCommFunction
<LI>GetCommConfig
<LI>GetCommModemStatus
<LI>GetCommProperties
<LI>GetCommState
<LI>GetCommTimeouts
<LI>PurgeComm
<LI>SetCommBreak
<LI>SetCommConfig
<LI>SetCommMask
<LI>SetCommState
<LI>SetCommTimeouts
<LI>SetupComm
<LI>TransmitCommChar
<LI>WaitCommEvent
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: serial.cpp $<BR>
$Modtime: 6/26/01 10:56a $
</BODY>

</HTML>
The following line should go in AUTOEXP.DAT so the debugging tooltips will format properly
ToolTipFormatLine=CSerialFile=hFile=<m_hFile> name=<m_strFileName.m_pchData,s>
*/
