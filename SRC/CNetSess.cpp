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
** $Workfile: cnetsess.cpp $
** $Revision: 24 $
** $Modtime: 6/26/01 10:49a $
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

CNetworkSessions::CNetworkSessions(_In_ std::wstring_view machine_name) noexcept : CNetwork(machine_name)
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CNetworkSessions::~CNetworkSessions() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Close();
}

void CNetworkSessions::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   CNetwork::Close();
   m_Initialize();
}

_Check_return_ bool CNetworkSessions::Delete( __inout CNetworkSessionInformation& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   WCHAR wide_user_name[ 256 ];
   WCHAR wide_client_name[ 256 ];

   ::ZeroMemory( wide_user_name,   sizeof( wide_user_name   ) );
   ::ZeroMemory( wide_client_name, sizeof( wide_client_name ) );

   LPWSTR user   = nullptr;
   LPWSTR client = nullptr;

   if ( information.UserName.empty() == false)
   {
#if ! defined( UNICODE )
      ::ASCII_to_UNICODE( information.UserName, wide_user_name );
#else
      _tcscpy_s( wide_user_name, std::size( wide_user_name ), information.UserName.c_str() );
#endif // UNICODE

      user = wide_user_name;
   }

   if ( information.ClientName.empty() == false )
   {
#if ! defined( UNICODE )
      ::ASCII_to_UNICODE( information.ClientName, wide_client_name );
#else
      _tcscpy_s( wide_client_name, std::size( wide_client_name ), information.ClientName.c_str() );
#endif // UNICODE

      client = wide_client_name;
   }

   m_ErrorCode = ::NetSessionDel( (LMSTR) m_WideMachineName.get(),
                                  (LMSTR) client,
                                  (LMSTR) user );
   
   if ( m_ErrorCode == NERR_Success )
   {
      return( true );
   }
   else
   {
      return( false );
   }
}

_Check_return_ bool CNetworkSessions::Enumerate( __inout CNetworkSessionInformation& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   SESSION_INFO_502 *information_p = nullptr;

   SESSION_INFO_502 s;

   ::ZeroMemory( &s, sizeof( s ) );

   information_p = &s;

   DWORD prefered_maximum_length = sizeof( SESSION_INFO_502 ) * 128;
   DWORD number_of_entries_read  = 0;
   DWORD total_number_of_entries = 0;

   m_ErrorCode = ::NetSessionEnum( (LMSTR) m_WideMachineName.get(),
                                   (LMSTR) nullptr,
                                   (LMSTR) nullptr,
                                           502,
                               (LPBYTE *) &information_p,
                                           prefered_maximum_length,
                                          &number_of_entries_read,
                                          &total_number_of_entries,
                                          &m_ResumeHandle );

   if ( information_p != (SESSION_INFO_502 *) nullptr )
   {
      information.Copy( information_p );
      return( true );
   }

   return( false );
}

_Check_return_ bool CNetworkSessions::GetNext( __inout CNetworkSessionInformation& information ) noexcept
{
   return( Enumerate( information ) );
}

void CNetworkSessions::m_Get_0_Data( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   LPBYTE buffer = nullptr;

   /*
   ** One of the children got loose in the header files again...
   **
   ** Also, we can't get 101 information because it doesn't work if you supply
   ** a machine name... Go Figure...
   */

   (void) ::NetSessionGetInfo( (LMSTR) m_WideMachineName.get(), nullptr, nullptr, 0, &buffer );

   if ( buffer != nullptr )
   {
      auto information_p = reinterpret_cast<SESSION_INFO_0 const *>(buffer);

      /*
      ** Now store the info we want...
      */

      m_ServerName.assign( information_p->sesi0_cname );
      m_Retrieved0 = true;
   }
}

void CNetworkSessions::m_Get_1_Data( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
}

void CNetworkSessions::m_Get_2_Data( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
}

void CNetworkSessions::m_Get_10_Data( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
}

void CNetworkSessions::m_Get_502_Data( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
}

void CNetworkSessions::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_ServerName.clear();
   m_ClientName.clear();

   m_Retrieved0   = false;
   m_Retrieved1   = false;
   m_Retrieved2   = false;
   m_Retrieved10  = false;
   m_Retrieved502 = false;

   m_ErrorCode    = 0;
   m_ResumeHandle = 0;
}

// End of source
