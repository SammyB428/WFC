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
** $Workfile: talksock.cpp $
** $Revision: 23 $
** $Modtime: 6/26/01 10:57a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CTalkingSocket::CTalkingSocket() noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CTalkingSocket::CTalkingSocket(_In_ std::wstring_view a_host_name, _In_ std::wstring const& p_name ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();

   SetAddress( a_host_name );
   SetPort( p_name.c_str() );
}

Win32FoundationClasses::CTalkingSocket::CTalkingSocket(_In_ std::wstring_view address, _In_ short const p_number ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();

   SetAddress( address );
   SetPort( p_number );
}

Win32FoundationClasses::CTalkingSocket::~CTalkingSocket()
{
   WFC_VALIDATE_POINTER( this );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CTalkingSocket::Dump( CDumpContext &dump_context ) const
{
   CSimpleSocket::Dump( dump_context );
}

#endif // _DEBUG

void Win32FoundationClasses::CTalkingSocket::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
}

/*
** The Socket manipulation routines
*/

_Check_return_ bool Win32FoundationClasses::CTalkingSocket::Open( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   if ( Address.empty() == true )
   {
      /*
      ** We don't have an address
      */

      return( false );
   }

   if ( m_SocketID not_eq INVALID_SOCKET )
   {
      // Someone (the user) is trying to open us twice!

      Close();
   }

   SOCKADDR_IN server_address;

   /*
   ** Create the socket
   */

   m_SocketID = m_Thunk_p->socket( AF_INET, SOCK_STREAM, 0 );

   if ( m_SocketID == INVALID_SOCKET )
   {
      m_LastError = m_Thunk_p->WSAGetLastError();
      //WFCTRACESOCKETERROR( m_ErrorCode );
      return( false );
   }

   /*
   ** Now fill in a socket address structure with the necessary information about the remote
   ** server node (remote node IP address and port for incoming connections) and attempt to
   ** connect to the server. This connect call will block until the remote server has accepted
   ** the connection or the connection request times out.
   */

   server_address.sin_family = AF_INET;
   server_address.sin_port   = m_PortNumberInNetworkByteOrder;

#if ! defined( UNICODE )

   server_address.sin_addr.s_addr = m_Thunk_p->inet_addr( reinterpret_cast<char const *>(Address) );

#else

   char ascii_address_string[ 100 ];

   ::ZeroMemory( ascii_address_string, sizeof( ascii_address_string ) );

   Win32FoundationClasses::UNICODE_to_ASCII( Address.c_str(), ascii_address_string );

   server_address.sin_addr.s_addr = m_Thunk_p->inet_addr( ascii_address_string );

#endif // UNICODE

   int connection_status = m_Thunk_p->connect( m_SocketID, (LPSOCKADDR) &server_address, sizeof( server_address ) );

   if ( connection_status == SOCKET_ERROR )
   {
      m_LastError = m_Thunk_p->WSAGetLastError();
      //WFCTRACESOCKETERROR( m_ErrorCode );

      Close();
      return( false );
   }

#if defined( WFC_STL )
   m_FileHandle = (HANDLE) m_SocketID;
#else
   m_hFile = (CFILE_HFILE) m_SocketID;
#endif

   // The filename should be the host name followed by IP address then port number.
   // Example: "Sammy (195.99.72.1) Port: 23"

   short port_number = 0;

   GetPort( port_number );

#if ! defined( WFC_STL )
   m_strFileName.Format( TEXT( "%s (%s) Port: %d" ), (LPCTSTR) Name, (LPCTSTR) Address, (int) port_number );
#else // WFC_STL

   std::wstring temp_name;

   format(temp_name, L"%s (%s) Port: %d", Name, Address, (int) port_number );
   m_FileName.assign(temp_name);
#endif // WFC_STL

   return( true );
}

#pragma warning( disable : 4100 )

_Check_return_ bool Win32FoundationClasses::CTalkingSocket::Open(_In_ std::filesystem::path const& a, _In_ UINT const port_number ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( a.empty() == true )
   {
      m_LastError = ERROR_INVALID_PARAMETER;
      return( FALSE );
   }

   SetAddress( a.c_str() );
   SetPort( (short) port_number );

   return( Open() );
}

#pragma warning( default : 4100 )

_Check_return_ bool Win32FoundationClasses::CTalkingSocket::Open(_In_ std::wstring_view a, _In_ short const p ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( p > 0 );

   SetAddress( a );
   SetPort( p );

   return( Open() );
}

_Check_return_ bool Win32FoundationClasses::CTalkingSocket::Open(_In_ std::wstring_view _host_name, _In_ std::wstring const& _port_name ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   SetAddress( _host_name );
   SetPort( _port_name.c_str() );

   return( Open() );
}

// End of source
