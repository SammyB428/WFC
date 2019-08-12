/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2011, Samuel R. Blackburn
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
** $Workfile: TCRAS.CPP $
** $Revision: 4 $
** $Modtime: 2/01/00 8:08p $
*/

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#if ! defined( WFC_NO_RAS )

VOID PASCAL dial_monitor( HRASCONN     connection_handle, 
                          UINT         message, 
                          RASCONNSTATE connection_state,
                          DWORD        error_code,
                          DWORD        extended_error_code )
{
   switch( connection_state )
   {
      case RASCS_OpenPort:

         OutputDebugString( TEXT( "RASCS_OpenPort : The communication port is about to be opened.\n" ) );
         break;

      case RASCS_PortOpened:

         OutputDebugString( TEXT( "RASCS_PortOpened : The communication port has been opened successfully.\n" ) );
         break;

      case RASCS_ConnectDevice:

         OutputDebugString( TEXT( "RASCS_ConnectDevice : A device is about to be connected. RasGetConnectStatus can be called to determine the name and type of the device being connected.\n" ) );
         break;

      case RASCS_DeviceConnected:

         OutputDebugString( TEXT( "RASCS_DeviceConnected : A device has connected successfully. RasGetConnectStatus can be called to determine the name and type of the device being connected.\n" ) );
         break;

      case RASCS_AllDevicesConnected:

         OutputDebugString( TEXT( "RASCS_AllDevicesConnected : All devices in the device chain have successfully connected. At this point, the physical link is established.\n" ) );
         break;

      case RASCS_Authenticate:

         OutputDebugString( TEXT( "RASCS_Authenticate : 	The authentication process is starting. Remote access does not allow the remote client to generate any traffic on the LAN until authentication has been successfully completed. \n" ) );
         break;

      case RASCS_AuthNotify:

         OutputDebugString( TEXT( "RASCS_AuthNotify : An authentication event has occurred. If dwError is zero, this event will be immediately followed by one of the more specific authentication states following. If dwError is nonzero, authentication has failed, and the error value indicates why.\n" ) );
         break;

      case RASCS_AuthRetry:

         OutputDebugString( TEXT( "RASCS_AuthRetry : The client has requested another validation attempt with a new username/password/domain. This state does not occur in Windows NT version 3.1.\n" ) );
         break;

      case RASCS_AuthCallback:

         OutputDebugString( TEXT( "RASCS_AuthCallback : The remote access server has requested a callback number. This occurs only if the user has \"Set By Caller\" callback privilege on the server.\n" ) );
         break;

      case RASCS_AuthChangePassword:

         OutputDebugString( TEXT( "RASCS_AuthChangePassword : The client has requested to change the password on the account. This state does not occur in Windows NT version 3.1.\n" ) );
         break;

      case RASCS_AuthProject:

         OutputDebugString( TEXT( "RASCS_AuthProject : The projection phase is starting.\n" ) );
         break;

      case RASCS_AuthLinkSpeed:

         OutputDebugString( TEXT( "RASCS_AuthLinkSpeed : The link-speed calculation phase is starting.\n" ) );
         break;

      case RASCS_AuthAck:

         OutputDebugString( TEXT( "RASCS_AuthAck : An authentication request is being acknowledged.\n" ) );
         break;

      case RASCS_ReAuthenticate:

         OutputDebugString( TEXT( "RASCS_ReAuthenticate : Reauthentication (after callback) is starting.\n" ) );
         break;

      case RASCS_Authenticated:

         OutputDebugString( TEXT( "RASCS_Authenticated : The client has successfully completed authentication.\n" ) );
         break;

      case RASCS_PrepareForCallback:

         OutputDebugString( TEXT( "RASCS_PrepareForCallback : The line is about to disconnect in preparation for callback.\n" ) );
         break;

      case RASCS_WaitForModemReset:

         OutputDebugString( TEXT( "RASCS_WaitForModemReset : The client is delaying in order to give the modem time to reset itself in preparation for callback.\n" ) );
         break;

      case RASCS_WaitForCallback:

         OutputDebugString( TEXT( "RASCS_WaitForCallback : The client is waiting for an incoming call from the remote access server.\n" ) );
         break;

      case RASCS_Projected:

         OutputDebugString( TEXT( "RASCS_Projected : This state occurs after the RASCS_AuthProject state. It indicates that projection result information is available. You can access the projection result information by calling RasGetProjectionInfo.\n" ) );
         break;

#if (WINVER >= 0x400)

      case RASCS_StartAuthentication:    // Windows 95 only 

         OutputDebugString( TEXT( "RASCS_StartAuthentication : Windows 95 only: Indicates that user authentication is being initiated or retried.\n" ) );
         break;

      case RASCS_CallbackComplete:       // Windows 95 only 

         OutputDebugString( TEXT( "RASCS_CallbackComplete : Windows 95 only: Indicates that the client has been called back and is about to resume authentication.\n" ) );
         break;

      case RASCS_LogonNetwork:           // Windows 95 only 

         OutputDebugString( TEXT( "RASCS_LogonNetwork : Windows 95 only: Indicates that the client is logging on to the network.\n" ) );
         break;

#endif 
 
      case RASCS_Interactive:

         OutputDebugString( TEXT( "RASCS_Interactive : This state corresponds to the terminal state supported by RASPHONE.EXE. This state does not occur in Windows NT version 3.1.\n" ) );
         break;

      case RASCS_RetryAuthentication:

         OutputDebugString( TEXT( "RASCS_RetryAuthentication : This state corresponds to the retry authentication state supported by RASPHONE.EXE. This state does not occur in Windows NT version 3.1.\n" ) );
         break;

      case RASCS_CallbackSetByCaller:

         OutputDebugString( TEXT( "RASCS_CallbackSetByCaller : This state corresponds to the Callback state supported by RASPHONE.EXE. This state does not occur in Windows NT version 3.1.\n" ) );
         break;

      case RASCS_PasswordExpired:

         OutputDebugString( TEXT( "RASCS_PasswordExpired : This state corresponds to the change password state supported by RASPHONE.EXE. This state does not occur in Windows NT version 3.1.\n" ) );
         break;

      case RASCS_Connected:

         OutputDebugString( TEXT( "RASCS_Connected : Successful connection.\n" ) );
         break;

      case RASCS_Disconnected:

         OutputDebugString( TEXT( "RASCS_Disconnected : Disconnection or failed connection.\n" ) );
         break;

      default:

         if ( connection_state != 0 )
         {
            TCHAR debug_string[ 99 ];

            _stprintf( debug_string, (LPCTSTR) TEXT( "Unknown connection state %d\n" ), connection_state );
            OutputDebugString( debug_string );
         }

         break;
   }

   switch( extended_error_code )
   {
      case ERROR_SERVER_NOT_RESPONDING:

         OutputDebugString( TEXT( "   Extended Error: Server not responding\n" ) );
         break;

      case ERROR_NETBIOS_ERROR:

         OutputDebugString( TEXT( "   Extended Error: NetBIOS error\n" ) );
         break;

      case ERROR_AUTH_INTERNAL:

         OutputDebugString( TEXT( "   Extended Error: Internal Diagnostics Code\n" ) );
         break;

      case ERROR_CANNOT_GET_LANA:

         OutputDebugString( TEXT( "   Extended Error: Routing error code\n" ) );
         break;

      default:

         if ( extended_error_code != 0 )
         {
            TCHAR debug_string[ 99 ];

            _stprintf( debug_string, (LPCTSTR) TEXT( "Unknown extended error code %lu\n" ), extended_error_code );
            OutputDebugString( debug_string );
         }

         break;
   }
}

void test_CRAS( void )
{
   WFCTRACEINIT( TEXT( "test_CRAS()" ) );

   CRemoteAccessService ras;

   std::vector<std::wstring> strings;

   ras.SetConnectionCallback( dial_monitor );
   ras.SetDialOptions( CRemoteAccessService::dialAcceptPausedStates );

   if ( ras.GetPhoneBookEntries( strings ) != FALSE )
   {
      WFCTRACE( TEXT( "Phone Book Entries:" ) );

      for ( auto const index : Range(strings.GetSize()))
      {
         WFCTRACE( strings.GetAt( index ) );
      }
   }

   strings.RemoveAll();

   if ( ras.GetConnections( strings ) != FALSE )
   {
      DWORD index = 0;
      DWORD number_of_entries = strings.GetSize();

      WFCTRACE( TEXT( "\nCurrent Connections:" ) );

      for ( auto const index : Range(strings.GetSize()))
      {
         WFCTRACE( strings[ index ] );
      }
   }

   if ( ras.Open( TEXT( "Erols" ) ) == FALSE )
   {
       std::wstring error_string;
      ras.GetErrorString( error_string );
      WFCTRACEVAL( TEXT( "Ras Open failed with " ), error_string );
   }
   else
   {
      WFCTRACE( TEXT( "RAS Open!\n" ) );
   }

   strings.RemoveAll();

   if ( ras.GetConnections( strings ) != FALSE )
   {
      WFCTRACE( TEXT( "Current Connections:" ) );

      for ( auto const index : Range(strings.GetSize()) )
      {
         WFCTRACE( strings[ index ] );
         index++;
      }
   }

   Sleep( 5000 );

   if ( ras.Close( TEXT( "Erols" ) ) != FALSE )
   {
      WFCTRACE( TEXT( "Close OK\n" ) );
   }
} 

#endif // WFC_NO_RAS