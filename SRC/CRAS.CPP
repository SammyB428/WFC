/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2014, Samuel R. Blackburn
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
** $Workfile: cras.cpp $
** $Revision: 42 $
** $Modtime: 6/26/01 10:50a $
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

#if ! defined( WFC_NO_RAS )

// Let's define a little function to ease debugging

#if defined( _DEBUG )

static inline __out_z _Check_return_ LPCTSTR __convert_RASCONNSTATE_to_string( __in DWORD ras_state )
{
   switch( ras_state )
   {
      case RASCS_OpenPort:

         return( TEXT( "RASCS_OpenPort - The communication port is about to be opened." ) );

      case RASCS_PortOpened:

         return( TEXT( "RASCS_PortOpened - The communication port has been opened successfully." ) );

      case RASCS_ConnectDevice:

         return( TEXT( "RASCS_ConnectDevice - A device is about to be connected. RasGetConnectStatus can be called to determine the name and type of the device being connected." ) );

      case RASCS_DeviceConnected:

         return( TEXT( "RASCS_DeviceConnected - A device has connected successfully. RasGetConnectStatus can be called to determine the name and type of the device being connected." ) );

      case RASCS_AllDevicesConnected:

         return( TEXT( "RASCS_AllDevicesConnected - All devices in the device chain have successfully connected. At this point, the physical link is established." ) );
  
      case RASCS_Authenticate:

         return( TEXT( "RASCS_Authenticate - The authentication process is starting. Remote access does not allow the remote client to generate any traffic on the LAN until authentication has been successfully completed." ) );
 
      case RASCS_AuthNotify:

         return( TEXT( "RASCS_AuthNotify - An authentication event has occurred. If dwError is zero, this event will be immediately followed by one of the more specific authentication states following. If dwError is nonzero, authentication has failed, and the error value indicates why." ) );

      case RASCS_AuthRetry:

         return( TEXT( "RASCS_AuthRetry - The client has requested another validation attempt with a new user name/password/domain. This state does not occur in Windows NT version 3.1." ) );
 
      case RASCS_AuthCallback:

         return( TEXT( "RASCS_AuthCallback - The remote access server has requested a callback number. This occurs only if the user has “Set By Caller” callback privilege on the server." ) );
 
      case RASCS_AuthChangePassword:

         return( TEXT( "RASCS_AuthChangePassword - The client has requested to change the password on the account. This state does not occur in Windows NT version 3.1." ) );
 
      case RASCS_AuthProject:

         return( TEXT( "RASCS_AuthProject - The projection phase is starting." ) );
 
      case RASCS_AuthLinkSpeed:

         return( TEXT( "RASCS_AuthLinkSpeed - The link-speed calculation phase is starting." ) );
 
      case RASCS_AuthAck:

         return( TEXT( "RASCS_AuthAck - An authentication request is being acknowledged." ) );
 
      case RASCS_ReAuthenticate:

         return( TEXT( "RASCS_ReAuthenticate - Reauthentication (after callback) is starting." ) );
 
      case RASCS_Authenticated:

         return( TEXT( "RASCS_Authenticated - The client has successfully completed authentication." ) );
 
      case RASCS_PrepareForCallback:

         return( TEXT( "RASCS_PrepareForCallback - The line is about to disconnect in preparation for callback." ) );
 
      case RASCS_WaitForModemReset:

         return( TEXT( "RASCS_WaitForModemReset - The client is delaying in order to give the modem time to reset itself in preparation for callback." ) );
 
      case RASCS_WaitForCallback:

         return( TEXT( "RASCS_WaitForCallback - The client is waiting for an incoming call from the remote access server." ) );
 
      case RASCS_Projected:

         return( TEXT( "RASCS_Projected - This state occurs after the RASCS_AuthProject state. It indicates that projection result information is available. You can access the projection result information by calling RasGetProjectionInfo." ) );
 
      case RASCS_StartAuthentication:

         return( TEXT( "RASCS_StartAuthentication - Windows 95 only: Indicates that user authentication is being initiated or retried." ) );
 
      case RASCS_CallbackComplete:

         return( TEXT( "RASCS_CallbackComplete - Windows 95 only: Indicates that the client has been called back and is about to resume authentication." ) );
 
      case RASCS_LogonNetwork:

         return( TEXT( "RASCS_LogonNetwork - Windows 95 only: Indicates that the client is logging on to the network." ) );
 
      case RASCS_SubEntryConnected:

         return( TEXT( "RASCS_SubEntryConnected - When dialing a multilink phone-book entry, this state indicates that a subentry has been connected during the dialing process. The dwSubEntry parameter of a RasDialFunc2 callback function indicates the index of the subentry. When the final state of all subentries in the phone-book entry has been determined, the connection state is RASCS_Connected if one or more subentries have been connected successfully." ) );
 
      case RASCS_SubEntryDisconnected:

         return( TEXT( "RASCS_SubEntryDisconnected - When dialing a multilink phone-book entry, this state indicates that a subentry has been disconnected during the dialing process. The dwSubEntry parameter of a RasDialFunc2 callback function indicates the index of the subentry." ) );
 
      case RASCS_Interactive:

         return( TEXT( "RASCS_Interactive - This state corresponds to the terminal state supported by RASPHONE.EXE. This state does not occur in Windows NT version 3.1." ) );
 
      case RASCS_RetryAuthentication:

         return( TEXT( "RASCS_RetryAuthentication - This state corresponds to the retry authentication state supported by RASPHONE.EXE. This state does not occur in Windows NT version 3.1." ) );
 
      case RASCS_CallbackSetByCaller:

         return( TEXT( "RASCS_CallbackSetByCaller - This state corresponds to the callback state supported by RASPHONE.EXE. This state does not occur in Windows NT version 3.1." ) );
 
      case RASCS_PasswordExpired:

         return( TEXT( "RASCS_PasswordExpired - This state corresponds to the change password state supported by RASPHONE.EXE. This state does not occur in Windows NT version 3.1." ) );

      case RASCS_Connected:

         return( TEXT( "RASCS_Connected - Successful connection." ) );

      case RASCS_Disconnected:

         return( TEXT( "RASCS_Disconnected - Disconnection or failed connection." ) );

#if ( WINVER >= 0x500 )

      case RASCS_InvokeEapUI:

         return( TEXT( "RASCS_InvokeEapUI - Invoke Authentication User Interface." ) );

#endif // WINVER >= 0x500

      default:

         return( TEXT( "Unknown state." ) );
   }
}

#endif // _DEBUG

/*
** CRASAuthenticationMessageBlock
*/

CRemoteAccessServiceAuthenticationMessageBlock::CRemoteAccessServiceAuthenticationMessageBlock()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CRemoteAccessServiceAuthenticationMessageBlock::CRemoteAccessServiceAuthenticationMessageBlock( RASAMB const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CRemoteAccessServiceAuthenticationMessageBlock::CRemoteAccessServiceAuthenticationMessageBlock( CRemoteAccessServiceAuthenticationMessageBlock const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CRemoteAccessServiceAuthenticationMessageBlock::~CRemoteAccessServiceAuthenticationMessageBlock()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void CRemoteAccessServiceAuthenticationMessageBlock::m_Initialize( void )
{
   WFC_VALIDATE_POINTER( this );

   ::ZeroMemory( (RASAMB *) this, sizeof( RASAMB ) );
   dwSize = sizeof( RASAMB );
}

void CRemoteAccessServiceAuthenticationMessageBlock::Copy( const RASAMB *source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      m_Initialize();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      dwError = source->dwError;
      bLana   = source->bLana;
      _tcscpy_s( szNetBiosError, std::size( szNetBiosError ), source->szNetBiosError );
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void CRemoteAccessServiceAuthenticationMessageBlock::Copy( CRemoteAccessServiceAuthenticationMessageBlock const& source )
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   dwError = source.dwError;
   bLana   = source.bLana;
   _tcscpy_s( szNetBiosError, std::size( szNetBiosError ), source.szNetBiosError );
}

BYTE CRemoteAccessServiceAuthenticationMessageBlock::GetLana( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( bLana );
}

DWORD CRemoteAccessServiceAuthenticationMessageBlock::GetErrorCode( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( dwError );
}

void CRemoteAccessServiceAuthenticationMessageBlock::GetErrorString(std::wstring &return_string ) const
{
   WFC_VALIDATE_POINTER( this );
   return_string = szNetBiosError;
}

CRemoteAccessServiceAuthenticationMessageBlock& CRemoteAccessServiceAuthenticationMessageBlock::operator=( CRemoteAccessServiceAuthenticationMessageBlock const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** CRemoteAccessServiceNetBEUIFramer
*/

CRemoteAccessServiceNetBEUIFramer::CRemoteAccessServiceNetBEUIFramer()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CRemoteAccessServiceNetBEUIFramer::CRemoteAccessServiceNetBEUIFramer( RASPPPNBF const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CRemoteAccessServiceNetBEUIFramer::CRemoteAccessServiceNetBEUIFramer( CRemoteAccessServiceNetBEUIFramer const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CRemoteAccessServiceNetBEUIFramer::~CRemoteAccessServiceNetBEUIFramer()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void CRemoteAccessServiceNetBEUIFramer::m_Initialize( void )
{
   WFC_VALIDATE_POINTER( this );

   ::ZeroMemory( (RASPPPNBF *) this, sizeof( RASPPPNBF ) );
   dwSize = sizeof( RASPPPNBF );
}

void CRemoteAccessServiceNetBEUIFramer::Copy( const RASPPPNBF *source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      m_Initialize();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      dwError        = source->dwError;
      dwNetBiosError = source->dwNetBiosError;
      bLana          = source->bLana;
      _tcscpy_s( szNetBiosError,    std::size( szNetBiosError    ), source->szNetBiosError    );
      _tcscpy_s( szWorkstationName, std::size( szWorkstationName ), source->szWorkstationName );
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void CRemoteAccessServiceNetBEUIFramer::Copy( CRemoteAccessServiceNetBEUIFramer const& source )
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   dwError        = source.dwError;
   dwNetBiosError = source.dwNetBiosError;
   bLana          = source.bLana;                      
   _tcscpy_s( szNetBiosError,    std::size( szNetBiosError    ), source.szNetBiosError    );
   _tcscpy_s( szWorkstationName, std::size( szWorkstationName ), source.szWorkstationName );
}

BYTE CRemoteAccessServiceNetBEUIFramer::GetLana( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( bLana );
}

_Check_return_ DWORD CRemoteAccessServiceNetBEUIFramer::GetErrorCode( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( dwError );
}

_Check_return_ DWORD CRemoteAccessServiceNetBEUIFramer::GetNetBiosErrorCode( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( dwNetBiosError );
}

void CRemoteAccessServiceNetBEUIFramer::GetErrorString(std::wstring& return_string ) const
{
   WFC_VALIDATE_POINTER( this );
   return_string = szNetBiosError;
}

void CRemoteAccessServiceNetBEUIFramer::GetWorkstationName(std::wstring& return_string ) const
{
   WFC_VALIDATE_POINTER( this );
   return_string = szWorkstationName;
}

CRemoteAccessServiceNetBEUIFramer& CRemoteAccessServiceNetBEUIFramer::operator=( CRemoteAccessServiceNetBEUIFramer const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** CRemoteAccessServiceInternetworkPacketExchange
*/

CRemoteAccessServiceInternetworkPacketExchange::CRemoteAccessServiceInternetworkPacketExchange()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CRemoteAccessServiceInternetworkPacketExchange::CRemoteAccessServiceInternetworkPacketExchange( RASPPPIPX const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CRemoteAccessServiceInternetworkPacketExchange::CRemoteAccessServiceInternetworkPacketExchange( CRemoteAccessServiceInternetworkPacketExchange const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CRemoteAccessServiceInternetworkPacketExchange::~CRemoteAccessServiceInternetworkPacketExchange()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void CRemoteAccessServiceInternetworkPacketExchange::m_Initialize( void )
{
   WFC_VALIDATE_POINTER( this );
   ::ZeroMemory( (RASPPPIPX *) this, sizeof( RASPPPIPX ) );
   dwSize = sizeof( RASPPPIPX );
}

void CRemoteAccessServiceInternetworkPacketExchange::Copy( const RASPPPIPX * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      m_Initialize();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      dwError = source->dwError;
      _tcscpy_s( szIpxAddress, std::size( szIpxAddress ), source->szIpxAddress );
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void CRemoteAccessServiceInternetworkPacketExchange::Copy( CRemoteAccessServiceInternetworkPacketExchange const& source )
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   dwError = source.dwError;
   _tcscpy_s( szIpxAddress, std::size( szIpxAddress ), source.szIpxAddress );
}

DWORD CRemoteAccessServiceInternetworkPacketExchange::GetErrorCode( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( dwError );
}

void CRemoteAccessServiceInternetworkPacketExchange::GetIPXAddress(std::wstring& return_string ) const
{
   WFC_VALIDATE_POINTER( this );
   return_string = szIpxAddress;
}

CRemoteAccessServiceInternetworkPacketExchange& CRemoteAccessServiceInternetworkPacketExchange::operator=( CRemoteAccessServiceInternetworkPacketExchange const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** CRemoteAccessServiceInternetProtocol
*/

CRemoteAccessServiceInternetProtocol::CRemoteAccessServiceInternetProtocol()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CRemoteAccessServiceInternetProtocol::CRemoteAccessServiceInternetProtocol( RASPPPIP const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CRemoteAccessServiceInternetProtocol::CRemoteAccessServiceInternetProtocol( CRemoteAccessServiceInternetProtocol const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CRemoteAccessServiceInternetProtocol::~CRemoteAccessServiceInternetProtocol()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void CRemoteAccessServiceInternetProtocol::m_Initialize( void )
{
   WFC_VALIDATE_POINTER( this );
   ::ZeroMemory( (RASPPPIP *) this, sizeof( RASPPPIP ) );
   dwSize = sizeof( RASPPPIP );
}

void CRemoteAccessServiceInternetProtocol::Copy( const RASPPPIP * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      m_Initialize();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      dwError = source->dwError;
      _tcscpy_s( szIpAddress, std::size( szIpAddress ), source->szIpAddress );

#if ! defined( WINNT35COMPATIBLE )

      _tcscpy_s( szServerIpAddress, std::size( szServerIpAddress ), source->szServerIpAddress );

#endif // WINNT35COMPATIBLE
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void CRemoteAccessServiceInternetProtocol::Copy( CRemoteAccessServiceInternetProtocol const& source )
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   dwError = source.dwError;
   _tcscpy_s( szIpAddress, std::size( szIpAddress ), source.szIpAddress );

#if ! defined( WINNT35COMPATIBLE )

   _tcscpy_s( szServerIpAddress, std::size( szServerIpAddress ), source.szServerIpAddress );

#endif // WINNT35COMPATIBLE
}

DWORD CRemoteAccessServiceInternetProtocol::GetErrorCode( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( dwError );
}

void CRemoteAccessServiceInternetProtocol::GetIPAddress(std::wstring& return_string ) const
{
   WFC_VALIDATE_POINTER( this );
   return_string = szIpAddress;
}

#if ! defined( WINNT35COMPATIBLE )

void CRemoteAccessServiceInternetProtocol::GetServerIPAddress(std::wstring& return_string ) const
{
   WFC_VALIDATE_POINTER( this );
   return_string = szServerIpAddress;
}

#endif // WINNT35COMPATIBLE

CRemoteAccessServiceInternetProtocol& CRemoteAccessServiceInternetProtocol::operator=( CRemoteAccessServiceInternetProtocol const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** CRemoteAccessService
*/

CRemoteAccessService::CRemoteAccessService()
{
   WFC_VALIDATE_POINTER( this );

   m_Thunk_p = nullptr;
   m_Initialize();
   m_InitializeThunk();
   m_AutomaticallyClose = false;
}

CRemoteAccessService::~CRemoteAccessService()
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AutomaticallyClose == true )
   {
      Close();
   }

   m_Initialize();

   if ( m_Thunk_p != nullptr )
   {
      if ( m_Thunk_p->dll_instance_handle != static_cast< HINSTANCE >( NULL ) )
      {
         ::FreeLibrary( m_Thunk_p->dll_instance_handle );
      }

      ZeroMemory( m_Thunk_p, sizeof( WFC_RAS_THUNK ) );
      delete m_Thunk_p;
      m_Thunk_p = nullptr;
   }
}

BOOL CRemoteAccessService::Close( LPCTSTR name_of_connection )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( name_of_connection );

   std::wstring connection_name;

#if defined( _DEBUG )

   DWORD number_of_tries = 0;

#endif

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( name_of_connection == nullptr )
      {
         connection_name = m_Name;

         m_ErrorCode = m_Thunk_p->RasHangUp( m_ConnectionHandle );

         if ( m_ErrorCode == 0 )
         {
            // Code around the NT bug

            while( m_Thunk_p->RasGetConnectStatus( m_ConnectionHandle, &m_ConnectionStatus ) != ERROR_INVALID_HANDLE )
            {
#if defined( _DEBUG )
               number_of_tries++;
#endif
               // Surrender the rest of our thread's time slice and a little more
               std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            WFCTRACEVAL( TEXT( "Hung up, it took this number of tries: " ), number_of_tries );

            m_ConnectionHandle = static_cast< HRASCONN >( NULL );
         }
      }
      else
      {
         HRASCONN temp_handle = GetConnection( name_of_connection );

         connection_name = name_of_connection;

         m_ErrorCode = m_Thunk_p->RasHangUp( temp_handle );

         if ( m_ErrorCode == 0 )
         {
            WFCTRACEVAL( TEXT( "Waiting until ras really hangs up on " ), name_of_connection );

            // Code around he NT bug

            while( m_Thunk_p->RasGetConnectStatus( temp_handle, &m_ConnectionStatus ) != ERROR_INVALID_HANDLE )
            {
#if defined( _DEBUG )
               number_of_tries++;
#endif
               // Surrender the rest of our thread's time slice
               std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            WFCTRACEVAL( TEXT( "Hung up, it took this number of tries: " ), number_of_tries );
         }
      }

      // By now you're wondering, "Why the heck is he closing the connection again?"
      // Because I have an application that *STILL* doesn't hangup! Gawl dang children...

      auto temp_handle = static_cast< HRASCONN >( NULL );

      do
      {
         temp_handle = GetConnection( connection_name );

         if ( temp_handle != static_cast< HRASCONN >( NULL ) )
         {
            WFCTRACE( TEXT( "Well whaddya know, the connection is still there..." ) );
            m_Thunk_p->RasHangUp( temp_handle );
            std::this_thread::sleep_for(std::chrono::seconds(1));
         }
      }
      while( temp_handle != static_cast< HRASCONN >( NULL ) );

      if ( m_ErrorCode == 0 )
      {
         return( TRUE );
      }
      else
      {
         return( FALSE );
      }
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

BOOL CRemoteAccessService::Dial( LPCTSTR who_to_dial )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( who_to_dial );

   if ( who_to_dial == nullptr )
   {
      m_ErrorCode = ERROR_INVALID_PARAMETER;
      return( FALSE );
   }

   // Disconnect the current connection asynchronously, we don't care when
   // it gets done.

   HangUp();

#if ( WINVER >= 0x401 )

   // Thanks go to Richard Wirth (r.wirth@wirthware.da-net.de) for
   // finding and fixing an endless loop bug here.

   RASCREDENTIALS credentials;

   ::ZeroMemory( &credentials, sizeof( credentials ) );
   credentials.dwSize = sizeof( credentials );
   credentials.dwMask = RASCM_UserName bitor RASCM_Password bitor RASCM_Domain;

   // get the default credentials
   m_Thunk_p->RasGetCredentials( nullptr, (TCHAR *) who_to_dial, &credentials );

#endif // WINVER >= 0x401

   RASDIALPARAMS dialing_parameters;

   ::ZeroMemory( &dialing_parameters, sizeof( dialing_parameters ) );

   dialing_parameters.dwSize = sizeof( dialing_parameters );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      _tcscpy_s( dialing_parameters.szEntryName, std::size( dialing_parameters.szEntryName ), who_to_dial );
#if ( WINVER >= 0x401 )
      // These are thanks to Richard Wirth...
      _tcscpy_s( dialing_parameters.szUserName, std::size( dialing_parameters.szUserName ), credentials.szUserName );
      _tcscpy_s( dialing_parameters.szPassword, std::size( dialing_parameters.szPassword ), credentials.szPassword );
      _tcscpy_s( dialing_parameters.szDomain,   std::size( dialing_parameters.szDomain   ), credentials.szDomain   );
#endif // WINVER >= 0x401

      m_ConnectionHandle = static_cast< HRASCONN >( NULL );

      m_ErrorCode = m_Thunk_p->RasDial( &m_DialExtensions,
                                nullptr,
                               &dialing_parameters,
                                m_NotifierType,
                                m_ConnectionCallbackFunctionPointer,
                               &m_ConnectionHandle );

      if ( m_ErrorCode == 0 )
      {
         m_Name = who_to_dial;
         return( TRUE );
      }
      else
      {
         m_Name.Empty();
         return( FALSE );
      }
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

BOOL CRemoteAccessService::EnableLogging( BOOL enable_logging, LPCTSTR machine_name )
{
   WFC_VALIDATE_POINTER_NULL_OK( machine_name );

   CRegistry registry;

   if ( registry.Connect( HKEY_LOCAL_MACHINE, machine_name ) == FALSE )
   {
      WFCTRACE( TEXT( "Can't connect to HKEY_LOCAL_MACHINE" ) );
      return( FALSE );
   }

   if ( registry.Open( TEXT( "SYSTEM\\CurrentControlSet\\Services\\RasMan\\Parameters" ) ) == FALSE )
   {
      WFCTRACE( TEXT( "Can't open \"SYSTEM\\CurrentControlSet\\Services\\RasMan\\Parameters\"" ) );
      return( FALSE );
   }

   DWORD value_to_set = 0;

   if ( enable_logging == FALSE )
   {
      value_to_set = 0;
   }
   else
   {
      value_to_set = 1;
   }

   if ( registry.SetValue( TEXT( "Logging" ), value_to_set ) == false)
   {
      return( FALSE );
   }

   return( TRUE );
}

HRASCONN CRemoteAccessService::GetConnection( LPCTSTR name_of_connection )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( name_of_connection );

   if ( name_of_connection == nullptr )
   {
      m_ErrorCode = ERROR_INVALID_PARAMETER;
      return( static_cast< HRASCONN >( NULL ) );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      LPRASCONN connections       = nullptr;
      DWORD     number_of_entries = 0;

      m_GetConnectionsIntoMemory( connections, number_of_entries );

      auto return_value = static_cast< HRASCONN >( NULL );

      if ( m_ErrorCode == 0 )
      {
         for ( auto const index : Range(number_of_entries) )
         {
            if ( _tcsicmp( name_of_connection, connections[ index ].szEntryName ) == 0 )
            {
               return_value = connections[ index ].hrasconn;
               break;
            }
         }
      }

      delete [] connections;

      return( return_value );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

BOOL CRemoteAccessService::GetConnections( std::vector<std::wstring>& connection_names )
{
   WFC_VALIDATE_POINTER( this );

   connection_names.RemoveAll();

   LPRASCONN connections       = nullptr;
   DWORD     number_of_entries = 0;

   m_GetConnectionsIntoMemory( connections, number_of_entries );

   BOOL return_value = FALSE;

   if ( m_ErrorCode == 0 )
   {
      return_value = TRUE;

      DWORD index = 0;

      while( index < number_of_entries )
      {
         connection_names.Add( connections[ index ].szEntryName );
         index++;
      }
   }

   delete [] connections;
   connections = NULL;

   return( return_value );
}

BOOL CRemoteAccessService::GetConnectionStatus( void )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   m_ErrorCode = m_Thunk_p->RasGetConnectStatus( m_ConnectionHandle, &m_ConnectionStatus );

   if ( m_ErrorCode == 0 )
   {
      return( TRUE );
   }
   else
   {
      return( FALSE );
   }
}

LONG CRemoteAccessService::GetErrorCode( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( m_ErrorCode );
}

BOOL CRemoteAccessService::GetErrorString(std::wstring& return_string )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   TCHAR string[ 4096 ];

   ::ZeroMemory( string, sizeof( string ) );

   if ( m_Thunk_p->RasGetErrorString( m_ErrorCode, string, std::size( string ) ) == 0 )
   {
      return_string = string;
      return( TRUE );
   }
   else
   {
      return_string.Empty();
      return( FALSE );
   }
}

BOOL CRemoteAccessService::GetKeepConnectionsAfterLogoff( LPCTSTR machine_name )
{
   WFC_VALIDATE_POINTER_NULL_OK( machine_name );

   // Thanks to Erik Strom (estrom@rcich.com) for getting
   // the information out of Microsoft

   CRegistry registry;

   if ( registry.Connect( HKEY_LOCAL_MACHINE, machine_name ) == FALSE )
   {
      return( FALSE );
   }

   if ( registry.Open( TEXT( "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon" ) ) == FALSE )
   {
      WFCTRACE( TEXT( "Can't open \"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\"" ) );
      return( FALSE );
   }

   std::wstring keep_connections();

   if ( registry.GetValue( TEXT( "KeepRasConnections" ), keep_connections ) == FALSE )
   {
      // The value doesn't exist, this means the RAS connections will be dropped at logoff
      return( FALSE );
   }

   int keep_connections_value = 0;

   keep_connections_value = _ttoi( keep_connections );

   if ( keep_connections_value == 0 )
   {
      return( FALSE );
   }

   return( TRUE );
}

void CRemoteAccessService::GetName(std::wstring& name ) const
{
   WFC_VALIDATE_POINTER( this );
   name = m_Name;
}

BOOL CRemoteAccessService::GetPhoneBookEntries( std::vector<std::wstring>& phone_book_entries )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   phone_book_entries.RemoveAll();

   DWORD size_of_buffer         = 64 * sizeof( RASENTRYNAME );
   DWORD number_of_bytes_needed = size_of_buffer;
   DWORD number_of_entries      = 0;

   LPRASENTRYNAME memory_buffer = (LPRASENTRYNAME) new BYTE[ size_of_buffer ]; // Get room for 64 entries (to begin with)

   if ( memory_buffer == nullptr )
   {
      m_ErrorCode = ERROR_NOT_ENOUGH_MEMORY;
      return( FALSE );
   }

   ::ZeroMemory( memory_buffer, size_of_buffer );

   memory_buffer->dwSize = sizeof( RASENTRYNAME );

   m_ErrorCode = m_Thunk_p->RasEnumEntries( nullptr, nullptr, memory_buffer, &number_of_bytes_needed, &number_of_entries );

   if ( m_ErrorCode != 0 )
   {
      if ( m_ErrorCode == ERROR_BUFFER_TOO_SMALL or m_ErrorCode == ERROR_NOT_ENOUGH_MEMORY )
      {
         delete [] memory_buffer;

         memory_buffer = (LPRASENTRYNAME) new BYTE[ number_of_bytes_needed ];

         if ( memory_buffer == nullptr )
         {
            m_ErrorCode = ERROR_NOT_ENOUGH_MEMORY;
            return( FALSE );
         }

         ::ZeroMemory( memory_buffer, number_of_bytes_needed );

         memory_buffer->dwSize = sizeof( RASENTRYNAME );
         m_ErrorCode = m_Thunk_p->RasEnumEntries( nullptr, nullptr, memory_buffer, &number_of_bytes_needed, &number_of_entries );
      }
   }

   BOOL return_value = FALSE;

   if ( m_ErrorCode == 0 )
   {
      return_value = TRUE;

      DWORD index = 0;

      while( index < number_of_entries )
      {
         phone_book_entries.Add( memory_buffer[ index ].szEntryName );
         index++;
      }
   }

   delete [] memory_buffer;

   return( return_value );
}

BOOL CRemoteAccessService::GetProtocolInformation( CRemoteAccessServiceAuthenticationMessageBlock& data_to_get )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   CRemoteAccessServiceAuthenticationMessageBlock return_data;

   // Thanks to Chad Stevens (c-stephens1@ti.com) for finding a Symantec 7.5 casting problem here

   if ( m_Thunk_p->RasGetProjectionInfo( m_ConnectionHandle, static_cast< enum tagRASPROJECTION >( RASP_Amb ), (LPVOID) (RASAMB *) &return_data, &return_data.dwSize ) == 0 )
   {
      data_to_get.Copy( return_data );

      return( TRUE );
   }
   else
   {
      return( FALSE );
   }
}

BOOL CRemoteAccessService::GetProtocolInformation( CRemoteAccessServiceNetBEUIFramer& data_to_get )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   CRemoteAccessServiceNetBEUIFramer return_data;

   // Thanks to Chad Stevens (c-stephens1@ti.com) for finding a Symantec 7.5 casting problem here

   if ( m_Thunk_p->RasGetProjectionInfo( m_ConnectionHandle, static_cast< enum tagRASPROJECTION >( RASP_PppNbf ), (LPVOID) (RASPPPNBF *) &return_data, &return_data.dwSize ) == 0 )
   {
      data_to_get.Copy( return_data );

      return( TRUE );
   }
   else
   {
      return( FALSE );
   }
}

BOOL CRemoteAccessService::GetProtocolInformation( CRemoteAccessServiceInternetworkPacketExchange& data_to_get )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   CRemoteAccessServiceInternetworkPacketExchange return_data;

   // Thanks to Chad Stevens (c-stephens1@ti.com) for finding a Symantec 7.5 casting problem here

   if ( m_Thunk_p->RasGetProjectionInfo( m_ConnectionHandle, static_cast< enum tagRASPROJECTION >( RASP_PppIpx ), (LPVOID) (RASPPPIPX *) &return_data, &return_data.dwSize ) == 0 )
   {
      data_to_get.Copy( return_data );

      return( TRUE );
   }
   else
   {
      return( FALSE );
   }
}

BOOL CRemoteAccessService::GetProtocolInformation( CRemoteAccessServiceInternetProtocol& data_to_get )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   CRemoteAccessServiceInternetProtocol return_data;

   // Thanks to Chad Stevens (c-stephens1@ti.com) for finding a Symantec 7.5 casting problem here

   if ( m_Thunk_p->RasGetProjectionInfo( m_ConnectionHandle, static_cast< enum tagRASPROJECTION >( RASP_PppIp ), (LPVOID) (RASPPPIP *) &return_data, &return_data.dwSize ) == 0 )
   {
      data_to_get.Copy( return_data );

      return( TRUE );
   }
   else
   {
      return( FALSE );
   }
}

BOOL CRemoteAccessService::HangUp( LPCTSTR name_of_connection )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );
   WFC_VALIDATE_POINTER_NULL_OK( name_of_connection );

   /*
   ** RasHangUp is mis-documented. It is an asynchronous call (although they don't bother
   ** to tell you that). Don't assume that when RasHangUp returns, the connection is
   ** terminates. THAT AIN'T THE CASE!
   **
   ** RasHangUp should be documented as follows:
   **
   ** DWORD RasPostHangUpRequest( HRASCONN ras_connection_handle );
   **
   ** Return Values
   ** If the function succeeds, zero if a hangup message was put in the RAS command queue, 
   ** If the function fails, the return value is a nonzero error value listed in the RAS header file, or ERROR_INVALID_HANDLE.
   **
   ** Remarks
   ** RasPostHangUpRequest (formerly named RasHangUp) issues a request to RAS to terminate
   ** the session. If you exit your process or thread before the request is processed, the
   ** RAS port will remain open. You must put code in your application to work around
   ** this little gem.
   */

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( name_of_connection == nullptr )
      {
         m_ErrorCode = m_Thunk_p->RasHangUp( m_ConnectionHandle );

         // m_ConnectionHandle is left intact because RAS doesn't really hangup.
      }
      else
      {
         HRASCONN temp_handle = GetConnection( name_of_connection );

         m_ErrorCode = m_Thunk_p->RasHangUp( temp_handle );
      }

      if ( m_ErrorCode == 0 )
      {
         return( TRUE );
      }
      else
      {
         return( FALSE );
      }
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

BOOL CRemoteAccessService::IsConnected( void )
{
   WFC_VALIDATE_POINTER( this );

   GetConnectionStatus();

   if ( m_ConnectionStatus.rasconnstate == RASCS_Connected )
   {
      return( TRUE );
   }
   else
   {
      return( FALSE );
   }
}
                                                              
void CRemoteAccessService::m_GetConnectionsIntoMemory( LPRASCONN& connections, DWORD& number_of_connections )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   DWORD size_of_buffer         = 64 * sizeof( RASCONN );
   DWORD number_of_bytes_needed = size_of_buffer;
   DWORD number_of_entries      = 0;

   LPRASCONN memory_buffer = (LPRASCONN) new BYTE[ size_of_buffer ]; // Get room for 64 entries (to begin with)

   if ( memory_buffer == nullptr )
   {
      m_ErrorCode = ERROR_NOT_ENOUGH_MEMORY;
      return;
   }

   ::ZeroMemory( memory_buffer, size_of_buffer );

   memory_buffer->dwSize = sizeof( RASCONN );

   m_ErrorCode = m_Thunk_p->RasEnumConnections( memory_buffer, &number_of_bytes_needed, &number_of_entries );

   if ( m_ErrorCode != 0 )
   {
      if ( m_ErrorCode == ERROR_BUFFER_TOO_SMALL or m_ErrorCode == ERROR_NOT_ENOUGH_MEMORY )
      {
         delete [] memory_buffer;

         memory_buffer = (LPRASCONN) new BYTE[ number_of_bytes_needed ];

         if ( memory_buffer == nullptr )
         {
            m_ErrorCode = ERROR_NOT_ENOUGH_MEMORY;
            return;
         }

         ::ZeroMemory( memory_buffer, number_of_bytes_needed );

         memory_buffer->dwSize = sizeof( RASCONN );
         m_ErrorCode = m_Thunk_p->RasEnumConnections( memory_buffer, &number_of_bytes_needed, &number_of_entries );
      }
   }

   connections           = memory_buffer;
   number_of_connections = number_of_entries;
}

void CRemoteAccessService::m_Initialize( void )
{
   WFC_VALIDATE_POINTER( this );

   m_ErrorCode                         = 0;
   m_ConnectionHandle                  = static_cast< HRASCONN >( NULL );
   m_ConnectionCallbackFunctionPointer = nullptr;
   m_NotifierType                      = 0;
   m_Name.Empty();

   // Always play with member structures last, Microsoft likes to 
   // change the size of them so when you ZeroMemory() them, things
   // break.

   // Here's a stack corruptor if WINVER is less than 0x401
   ::ZeroMemory( &m_ConnectionStatus, sizeof( m_ConnectionStatus ) );
   m_ConnectionStatus.dwSize = sizeof( m_ConnectionStatus );

   ::ZeroMemory( &m_DialExtensions, sizeof( m_DialExtensions ) );
   m_DialExtensions.dwSize = sizeof( m_DialExtensions );
}

// 2000-06-11
// Release 47, many many thanks to Anand Rajiva [mailto:anand@rajiva.net]
// for fixing a stack error with my thunking. The bug only showed itself
// when the executable ran on a machine that didn't have RAS installed.

#define WFC_LOAD_RAS_THUNK( dll_instance_handle, structure_pointer, function_name, need_params ) \
{\
   *( (FARPROC *) &structure_pointer->function_name ) = GetProcAddress( dll_instance_handle, #function_name );\
   if ( structure_pointer->function_name == nullptr ) \
{\
   *( (FARPROC *) &structure_pointer->function_name ) = ( (FARPROC) _wfc_ras_thunk_not_implemented##need_params );\
}\
}

static DWORD APIENTRY _wfc_ras_thunk_not_implemented1( PVOID )
{
   return( ERROR_CALL_NOT_IMPLEMENTED );
}

static DWORD APIENTRY _wfc_ras_thunk_not_implemented2( PVOID, PVOID )
{
   return( ERROR_CALL_NOT_IMPLEMENTED );
}

static DWORD APIENTRY _wfc_ras_thunk_not_implemented3( PVOID, PVOID, PVOID )
{
   return( ERROR_CALL_NOT_IMPLEMENTED );
}

static DWORD APIENTRY _wfc_ras_thunk_not_implemented4( PVOID, PVOID, PVOID, PVOID )
{
   return( ERROR_CALL_NOT_IMPLEMENTED );
}

static DWORD APIENTRY _wfc_ras_thunk_not_implemented5( PVOID, PVOID, PVOID, PVOID, PVOID )
{
   return( ERROR_CALL_NOT_IMPLEMENTED );
}

static DWORD APIENTRY _wfc_ras_thunk_not_implemented6( PVOID, PVOID, PVOID, PVOID, PVOID, PVOID )
{
   return( ERROR_CALL_NOT_IMPLEMENTED );
}

static DWORD APIENTRY _wfc_ras_thunk_not_implemented7( PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID )
{
   return( ERROR_CALL_NOT_IMPLEMENTED );
}

void CRemoteAccessService::m_InitializeThunk( void )
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Thunk_p != nullptr )
   {
      return;
   }

   WFC_TRY
   {
      m_Thunk_p = new WFC_RAS_THUNK;
   }
   WFC_CATCH_ALL
   {
      m_Thunk_p = nullptr;
   }
   WFC_END_CATCH_ALL

   if ( m_Thunk_p == nullptr )
   {
      WFCTRACE( TEXT( "Out of memory" ) );
      return;
   }

   ZeroMemory( m_Thunk_p, sizeof( WFC_RAS_THUNK ) );

   // Now is the time for all good programmers to detect the operating system

   _tcsncpy_s( m_Thunk_p->dll_name, std::size( m_Thunk_p->dll_name ), TEXT( "RASAPI32.DLL" ), std::size( m_Thunk_p->dll_name ) );

   m_Thunk_p->dll_instance_handle = ::LoadLibrary( m_Thunk_p->dll_name );

   if ( m_Thunk_p->dll_instance_handle == static_cast< HINSTANCE >( NULL ) )
   {
      WFCTRACEVAL( TEXT( "Can't load " ), m_Thunk_p->dll_name );
   }

   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasDialA, 6 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasDialW, 6 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasEnumConnectionsA, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasEnumConnectionsW, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasEnumEntriesA, 5 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasEnumEntriesW, 5 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetConnectStatusA, 2 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetConnectStatusW, 2 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetErrorStringA, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetErrorStringW, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasHangUpA, 1 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasHangUpW, 1 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetProjectionInfoA, 4 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetProjectionInfoW, 4 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasCreatePhonebookEntryA, 2 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasCreatePhonebookEntryW, 2 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasEditPhonebookEntryA, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasEditPhonebookEntryW, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetEntryDialParamsA, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetEntryDialParamsW, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetEntryDialParamsA, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetEntryDialParamsW, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasEnumDevicesA, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasEnumDevicesW, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetCountryInfoA, 2 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetCountryInfoW, 2 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetEntryPropertiesA, 6 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetEntryPropertiesW, 6 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetEntryPropertiesA, 6 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetEntryPropertiesW, 6 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasRenameEntryA, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasRenameEntryW, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasDeleteEntryA, 2 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasDeleteEntryW, 2 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasValidateEntryNameA, 2 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasValidateEntryNameW, 2 );

#if ( WINVER >= 0x401 )

   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetSubEntryHandleA, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetSubEntryHandleW, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetCredentialsA, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetCredentialsW, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetCredentialsA, 4 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetCredentialsW, 4 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasConnectionNotificationA, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasConnectionNotificationW, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetSubEntryPropertiesA, 7 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetSubEntryPropertiesW, 7 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetSubEntryPropertiesA, 7 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetSubEntryPropertiesW, 7 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetAutodialAddressA, 5 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetAutodialAddressW, 5 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetAutodialAddressA, 5 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetAutodialAddressW, 5 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasEnumAutodialAddressesA, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasEnumAutodialAddressesW, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetAutodialEnableA, 2 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetAutodialEnableW, 2 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetAutodialEnableA, 2 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetAutodialEnableW, 2 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetAutodialParamA, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasGetAutodialParamW, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetAutodialParamA, 3 );
   WFC_LOAD_RAS_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, RasSetAutodialParamW, 3 );

#endif
}

BOOL CRemoteAccessService::Open( LPCTSTR who_to_dial )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( who_to_dial );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   if ( who_to_dial == nullptr )
   {
      m_ErrorCode = ERROR_INVALID_PARAMETER;
      return( FALSE );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // Sometimes Open() would go into an endless loop.
      // Luckily, Richard Wirth (r.wirth@wirthware.da-net.de)
      // was smart enough to catch it.

#if ( WINVER >= 0x401 )
      RASCREDENTIALS credentials;

      ::ZeroMemory( &credentials, sizeof( credentials ) );
      credentials.dwSize = sizeof( credentials );
      credentials.dwMask = RASCM_UserName bitor RASCM_Password bitor RASCM_Domain;

      // get the default credentials
      m_Thunk_p->RasGetCredentials( nullptr, (TCHAR *) who_to_dial, &credentials );

#endif // WINVER >= 0x401

      RASDIALPARAMS dialing_parameters;

      ::ZeroMemory( &dialing_parameters, sizeof( dialing_parameters ) );

      dialing_parameters.dwSize = sizeof( dialing_parameters );
      _tcscpy_s( dialing_parameters.szEntryName, std::size( dialing_parameters.szEntryName ), who_to_dial );

#if ( WINVER >= 0x401 )
      // These are thanks to Richard Wirth...
      _tcscpy_s( dialing_parameters.szUserName, std::size( dialing_parameters.szUserName ), credentials.szUserName );
      _tcscpy_s( dialing_parameters.szPassword, std::size( dialing_parameters.szPassword ), credentials.szPassword );
      _tcscpy_s( dialing_parameters.szDomain,   std::size( dialing_parameters.szDomain   ), credentials.szDomain   );
#endif // WINVER >= 0x401

      m_ConnectionHandle = static_cast< HRASCONN >( NULL );
      m_Name.Empty();

      m_ErrorCode = m_Thunk_p->RasDial( &m_DialExtensions,
                                nullptr,
                               &dialing_parameters,
                                m_NotifierType,
                                m_ConnectionCallbackFunctionPointer,
                               &m_ConnectionHandle );

      if ( m_ErrorCode == 0 )
      {
         m_Name = who_to_dial;

         // Now we've got a connection handle, with it, we can wait until we're connected

         // First, let's sleep for a while. This let's RAS dial the phone instead of
         // respond to all of our GetConnectionStatus() calls. We are hammering RAS
         // when we call that function.

         std::this_thread::sleep_for(std::chrono::seconds(6));

         // Now start hammering RAS for the status

         while( 1 )
         {
            if ( GetConnectionStatus() != FALSE )
            {
               if ( m_ConnectionStatus.rasconnstate == RASCS_Connected )
               {
                  WFCTRACE( TEXT( "Connected!" ) );
                  return( TRUE );
               }

               if ( m_ConnectionStatus.rasconnstate == RASCS_Disconnected )
               {
                  WFCTRACE( TEXT( "Disconnected" ) );
                  return( FALSE );
               }

               // only ask RAS about 4 times per second... (i.e. be nice)

               std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
            else
            {
               WFCTRACEVAL( TEXT( "Not Connected Error: " ), m_ErrorCode );
               WFCTRACEERROR( m_ErrorCode );

               WFCTRACEVAL( TEXT( "Connection Status Error: " ), m_ConnectionStatus.dwError );
               WFCTRACEERROR( m_ConnectionStatus.dwError );

               WFCTRACEVAL( TEXT( "rasconnstate " ), __convert_RASCONNSTATE_to_string( m_ConnectionStatus.rasconnstate ) );

               return( FALSE );
            }
         }
      }
      else
      {
        return( FALSE );
      }
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

bool CRemoteAccessService::SetAutomaticallyClose( bool auto_close )
{
   WFC_VALIDATE_POINTER( this );

   auto return_value = m_AutomaticallyClose;

   m_AutomaticallyClose = auto_close;

   return( return_value );
}

LPVOID CRemoteAccessService::SetConnectionCallback( LPVOID function_pointer )
{
   WFC_VALIDATE_POINTER( this );

   LPVOID return_value = nullptr;

   if ( m_NotifierType == 1 )
   {
      return_value = m_ConnectionCallbackFunctionPointer;
   }

   m_ConnectionCallbackFunctionPointer = function_pointer;
   m_NotifierType                      = 1;

   return( return_value );
}

HWND CRemoteAccessService::SetConnectionCallbackWindow( HWND window_handle )
{
   WFC_VALIDATE_POINTER( this );

   auto return_value = static_cast< HWND >( NULL );

   if ( m_NotifierType == 0xFFFFFFFF )
   {
      return_value = (HWND) m_ConnectionCallbackFunctionPointer;
   }

   m_ConnectionCallbackFunctionPointer = (LPVOID) window_handle;
   m_NotifierType = 0xFFFFFFFF;

   return( return_value );
}

DWORD CRemoteAccessService::SetDialOptions( DWORD dial_options )
{
   WFC_VALIDATE_POINTER( this );

   DWORD return_value = m_DialExtensions.dwfOptions;

   m_DialExtensions.dwfOptions = dial_options;

   return( return_value );
}

BOOL CRemoteAccessService::SetKeepConnectionsAfterLogoff( BOOL keep_connections, LPCTSTR machine_name )
{
   WFC_VALIDATE_POINTER_NULL_OK( machine_name );

   // Thanks to Erik Strom (estrom@rcich.com) for getting
   // the information out of Microsoft

   CRegistry registry;

   if ( registry.Connect( HKEY_LOCAL_MACHINE, machine_name ) == false)
   {
      WFCTRACE( TEXT( "Can't connect to HKEY_LOCAL_MACHINE" ) );
      return( FALSE );
   }

   if ( registry.Open( TEXT( "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon" ) ) == false)
   {
      WFCTRACE( TEXT( "Can't open \"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\"" ) );
      return( FALSE );
   }

   std::wstring keep_connections_string;

   if ( keep_connections == FALSE )
   {
      keep_connections_string = TEXT( "0" );
   }
   else
   {
      keep_connections_string = TEXT( "1" );
   }

   if ( registry.SetValue( TEXT( "KeepRasConnections" ), keep_connections_string ) == false)
   {
      // The value doesn't exist, this means the RAS connections will be dropped at logoff
      return( FALSE );
   }

   return( TRUE );
}

#endif // WFC_NO_RAS

/*
** How to get rid of the "Done" button on the terminal screen
**
** 1. Get your macro in SWITCH.INF working so the only thing left to do
**    is click on the Done button in the terminal window
**
** 2. Set "Authenticate using current username and password" ON.
**
** 3. Choose "Authenticate using any method including clear text" under
**    the "Security" button.
**
** 4. ENJOY!
*/

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CRemoteAccessService</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, RAS, Remote Access Service, Dialup networking, source code">
<META name="description" content="The C++ class that encapsulates Remote Access Service (Dialup networking).">
</HEAD>

<BODY>

<H1>CRemoteAccessService : CObject</H1>
$Revision: 42 $
<HR>

<H2>Description</H2>

This class makes RAS a little easier. From a programming perspective, RAS is like most Microsoft API's: almost useful.
What's lacking from RAS is the ability to play with ports. I had to write software for a fault-tolerant mail postoffice
and wrote a little program to page a system administrator when something went wrong. I had to dedicate one modem to
this task because there's no way to tell RAS to release a port for other use. There's a RasPortxxx API that is not
exposed so we lowly developer's can't use it. In short, it is impossible to write the RASPHONE application using
the documented RAS API.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CRemoteAccessService</B>()
<B>CRemoteAccessService</B>( DWORD input_buffer_size )
<B>CRemoteAccessService</B>( DWORD input_buffer_size, output_buffer_size )</PRE><DD>
Constructs the object.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="Close">Close</A></B>( LPCTSTR name_of_connection = NULL )</PRE><DD>
Same as <B><A HREF="#HangUp">HangUp()</A></B>
except it will not return until RAS really hangs up.

<DT><PRE>BOOL <B><A NAME="Dial">Dial</A></B>( LPCTSTR who_to_call )</PRE><DD>
Dials a phone book entry. Makes a connection.

<DT><PRE>static BOOL <B><A NAME="EnableLogging">EnableLogging</A></B>( BOOL enable_logging, LPCTSTR machine_name = NULL )</PRE><DD>
This method allows you to enable logging of the connection. If set to
TRUE, it will create a DEVICE.LOG file that holds the commands and
responses during a RAS connection (when you dial).

<DT><PRE>HRASCONN <B><A NAME="GetConnection">GetConnection</A></B>( LPCTSTR name_of_connection )</PRE><DD>
Gives you the HRASONN for the named connection.

<DT><PRE>BOOL <B><A NAME="GetConnections">GetConnections</A></B>( std::vector&lt;std::wstring&gt;&amp; connection_names )</PRE><DD>
Gives you a list of connection names. These are the names of the
phone book entries that are currently connected. It is a list
of the active RAS connections.

<DT><PRE>BOOL <B><A NAME="GetConnectionStatus">GetConnectionStatus</A></B>( void )</PRE><DD>
Returns TRUE is you're connected.

<DT><PRE>LONG <B><A NAME="GetErrorCode">GetErrorCode</A></B>( void ) const</PRE><DD>
Returns the error code should any function return FALSE.

<DT><PRE>BOOL <B><A NAME="GetErrorString">GetErrorString</A></B>( std::wstring&amp; error_string )</PRE><DD>
Same as <B>GetErrorCode</B>() but it gives you a human
readable string instead of a number.

<DT><PRE>BOOL <B><A NAME="GetKeepConnectionsAfterLogoff">GetKeepConnectionsAfterLogoff</A></B>( LPCTSTR machine_name = NULL )</PRE><DD>
Tells you if your RAS connections
will be dropped (disconnected) when the user logs off.
If <CODE></CODE> is not NULL,

<DT><PRE>void <B><A NAME="GetName">GetName</A></B>( std::wstring&amp; name )</PRE><DD>
Gives you the name of the connection that was successfully
<B><A HREF="#Open">Open()</A></B>'d or
<B><A HREF="#Dial">Dial()</A></B>'d.

<DT><PRE>BOOL <B><A NAME="GetPhoneBookEntries">GetPhoneBookEntries</A></B>( std::vector&lt;std::wstring&gt;&amp; phone_book_entries )</PRE><DD>
Gives you back a list of phone book entry
names. You can use these names to pass to <B><A HREF="#Dial">Dial()</A></B>.

<DT><PRE>BOOL <B><A NAME="GetProtocolInformation">GetProtocolInformation</A></B>( CRemoteAccessServiceAuthenticationMessageBlock&amp; data_to_get )
BOOL <B>GetProtocolInformation</B>( CRemoteAccessServiceNetBEUIFramer&amp; data_to_get )
BOOL <B>GetProtocolInformation</B>( CRemoteAccessServiceInternetworkPacketExchange&amp; data_to_get )
BOOL <B>GetProtocolInformation</B>( CRemoteAccessServiceInternetProtocol&amp; data_to_get )</PRE><DD>
Gives you back information about the networking protocol used on the active connection.

<DT><PRE>BOOL <B><A NAME="HangUp">HangUp</A></B>( LPCTSTR name_of_connection = NULL )</PRE><DD>
Tells RAS to terminate the connection. <B>NOTE!</B>
When this function returns, <B><I>THE CONNECTION HAS NOT BEEN TERMINATED!!</I></B>
If you immediately exit from your thread/process after calling <B>HangUp()</B>, the connection
will be left in a random state. It is much safer to use
<B><A HREF="#Close">Close()</A></B> instead.

<DT><PRE>BOOL <B><A NAME="IsConnected">IsConnected</A></B>( void )</PRE><DD>
Returns TRUE if you're connected.

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( LPCTSTR who_to_call )</PRE><DD>
Same as Dial but lets you treat RAS like most other object in WFC.

<DT><PRE>BOOL <B><A NAME="SetAutomaticallyClose">SetAutomaticallyClose</A></B>( BOOL automatically_close )</PRE><DD>
When this property is TRUE, the destructor will terminate the connection if there is one. 

<DT><PRE>LPVOID <B><A NAME="SetConnectionCallback">SetConnectionCallback</A></B>( LPVOID RasDialFunc1_function_pointer )</PRE><DD>
Lets you pass a pointer to a function so you can monitor the progress of RAS.

<DT><PRE>HWND <B><A NAME="SetConnectionCallbackWindow">SetConnectionCallbackWindow</A></B>( HWND window_handle )</PRE><DD>
Lets you pass a window handle that will
receive <CODE>WM_RASDIALEVENT</CODE> messages so you can monitor the progress of RAS.

<DT><PRE>DWORD <B><A NAME="SetDialOptions">SetDialOptions</A></B>( DWORD dial_options )</PRE><DD>
Let's you set the dialing options.

<DT><PRE>BOOL <B><A NAME="SetKeepConnectionsAfterLogoff">SetKeepConnectionsAfterLogoff</A></B>( BOOL keep_connections = TRUE, LPCTSTR machine_name )</PRE><DD>
Lets you tell NT whether or not it
should drop all RAS connections when the current user logs off.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

void test_CRAS( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;test_CRAS()&quot; ) );

   <B>CRemoteAccessService</B> ras;

   std::vector&lt;std::wstring&gt; strings;

   ras.<A HREF="#SetDialOptions">SetDialOptions</A>( CRemoteAccessService::dialAcceptPausedStates );

   if ( ras.<A HREF="#GetPhoneBookEntries">GetPhoneBookEntries</A>( strings ) != FALSE )
   {
      DWORD index = 0;
      DWORD number_of_entries = strings.GetSize();

      TRACE( TEXT( &quot;Phone Book Entries:\n&quot; ) );

      while( index &lt; number_of_entries )
      {
         TRACE1( TEXT( &quot;\&quot;%s\&quot;\n&quot; ), (LPCTSTR) strings[ index ] );
         index++;
      }
   }

   strings.RemoveAll();

   if ( ras.<A HREF="#GetConnections">GetConnections</A>( strings ) != FALSE )
   {
      DWORD index = 0;
      DWORD number_of_entries = strings.GetSize();

      TRACE( TEXT( &quot;\nCurrent Connections:\n&quot; ) );

      while( index &lt; number_of_entries )
      {
         TRACE1( TEXT( &quot;\&quot;%s\&quot;\n&quot; ), (LPCTSTR) strings[ index ] );
         index++;
      }
   }

   if ( ras.<A HREF="#Open">Open</A>( TEXT( &quot;EROLS&quot; ) ) == FALSE )
   {
      std::wstring error_string;
      ras.<A HREF="#GetErrorString">GetErrorString</A>( error_string );
      TRACE1( TEXT( &quot;Ras Open failed with \&quot;%s\&quot;!\n&quot; ), (LPCTSTR) error_string );
   }
   else
   {
      TRACE( TEXT( &quot;RAS Open!\n&quot; ) );
   }

   strings.RemoveAll();

   if ( ras.<A HREF="#GetConnections">GetConnections</A>( strings ) != FALSE )
   {
      DWORD index = 0;
      DWORD number_of_entries = strings.GetSize();

      TRACE( TEXT( &quot;\nCurrent Connections:\n&quot; ) );

      while( index &lt; number_of_entries )
      {
         TRACE1( TEXT( &quot;\&quot;%s\&quot;\n&quot; ), (LPCTSTR) strings[ index ] );
         index++;
      }
   }

   if ( ras.<A HREF="#HangUp">HangUp</A>( TEXT( &quot;EROLS&quot; ) ) != FALSE )
   {
      TRACE( TEXT( &quot;Hangup OK\n&quot; ) );
   }
}</CODE></PRE>

<H2>API's Used</H2>

<UL>
<LI>RasDial
<LI>RasEnumConnections
<LI>RasEnumEntries
<LI>RasGetConnectStatus
<LI>RasGetErrorString
<LI>RasGetProjectionInfo
<LI>RasHangUp
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: cras.cpp $<BR>
$Modtime: 6/26/01 10:50a $
</BODY>

</HTML>
*/
