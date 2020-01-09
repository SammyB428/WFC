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
** $Workfile: CRASADM.CPP $
** $Revision: 23 $
** $Modtime: 6/26/01 10:51a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if ! defined( WFC_NO_RAS )

// 2000-10-30
// Thanks go to Danny Smith (danny_r_smith_2001@yahoo.co.nz) for
// making this compilable in GCC (added WFC_HAVE_RASSAPI).
// It was pretty stupid of me to assume that everyone uses Visual C++.

#if ( _MSC_VER >= 1020 ) || defined( WFC_HAVE_RASSAPI )

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

/*
** CRemoteAccessServiceAdministration
*/

CRemoteAccessServiceAdministration::CRemoteAccessServiceAdministration()
{
   WFC_VALIDATE_POINTER( this );
   m_PortArray = nullptr;
   m_Initialize();
}

CRemoteAccessServiceAdministration::~CRemoteAccessServiceAdministration()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

BOOL CRemoteAccessServiceAdministration::ClearPortStatistics( std::wstring const& port_name, LPCTSTR server_name )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( server_name );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CNetwork network_server_name( server_name );

      if ( server_name == nullptr )
      {
         // OOPS! We need to use the name of this machine...

         TCHAR this_machine_name[ MAX_COMPUTERNAME_LENGTH + 1 ];

         DWORD machine_name_length = MAX_COMPUTERNAME_LENGTH + 1;
   
         ::ZeroMemory( this_machine_name, sizeof( this_machine_name ) );

         if ( ::GetComputerNameEx(COMPUTER_NAME_FORMAT::ComputerNameDnsHostname, this_machine_name, &machine_name_length ) == FALSE )
         {
            WFCTRACE( TEXT( "Can't get computer name\n" ) );
            return( FALSE );
         }

         network_server_name.Open( this_machine_name );
      }

      WCHAR unicode_port_name[ UNCLEN + 1 ];
      ::ZeroMemory( unicode_port_name, sizeof( unicode_port_name ) );

#if ! defined( UNICODE )

      ::ASCII_to_UNICODE( port_name, unicode_port_name );

#else

      ::wcscpy( unicode_port_name, port_name );

#endif // UNICODE

      m_ErrorCode = ::RasAdminPortClearStatistics( (LPCWSTR) network_server_name, unicode_port_name );

      if ( m_ErrorCode != ERROR_SUCCESS )
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

BOOL CRemoteAccessServiceAdministration::DisconnectPort( std::wstring const& port_name, LPCTSTR server_name )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( server_name );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CNetwork network_server_name( server_name );

      if ( server_name == nullptr )
      {
         // OOPS! We need to use the name of this machine...

         TCHAR this_machine_name[ MAX_COMPUTERNAME_LENGTH + 1 ];

         DWORD machine_name_length = MAX_COMPUTERNAME_LENGTH + 1;

         ::ZeroMemory( this_machine_name, sizeof( this_machine_name ) );

         if ( ::GetComputerNameEx(COMPUTER_NAME_FORMAT::ComputerNameDnsHostname, this_machine_name, &machine_name_length ) == FALSE )
         {
            WFCTRACE( TEXT( "Can't get computer name\n" ) );
            return( FALSE );
         }

         network_server_name.Open( this_machine_name );
      }

      WCHAR unicode_port_name[ UNCLEN + 1 ];
      ::ZeroMemory( unicode_port_name, sizeof( unicode_port_name ) );

#if ! defined( UNICODE )

      ::ASCII_to_UNICODE( port_name, unicode_port_name );

#else

      ::wcscpy( unicode_port_name, port_name );

#endif // UNICODE

      m_ErrorCode = ::RasAdminPortDisconnect( (LPCWSTR) network_server_name, unicode_port_name );

      if ( m_ErrorCode != ERROR_SUCCESS )
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

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CRemoteAccessServiceAdministration::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );
}

#endif // _DEBUG

BOOL CRemoteAccessServiceAdministration::EnumeratePorts( LPCTSTR server_name )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( server_name );

   m_Initialize();

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CNetwork network_server_name( server_name );

      if ( server_name == nullptr )
      {
         // OOPS! We need to use the name of this machine...

         TCHAR this_machine_name[ MAX_COMPUTERNAME_LENGTH + 1 ];

         DWORD machine_name_length = MAX_COMPUTERNAME_LENGTH + 1;

         ::ZeroMemory( this_machine_name, sizeof( this_machine_name ) );

         if ( ::GetComputerNameEx(COMPUTER_NAME_FORMAT::ComputerNameDnsHostname, this_machine_name, &machine_name_length ) == FALSE )
         {
            WFCTRACE( TEXT( "Can't get computer name\n" ) );
            return( FALSE );
         }

         network_server_name.Open( this_machine_name );
      }

      WCHAR wide_machine_name[ MAX_COMPUTERNAME_LENGTH + 3 ];

      ZeroMemory( wide_machine_name, sizeof( wide_machine_name ) );

      ::wcsncpy_s( wide_machine_name, std::size( wide_machine_name ), L"\\\\", _TRUNCATE );
      ::wcscat_s( wide_machine_name, std::size( wide_machine_name ), (LPCWSTR) network_server_name );

      DWORD return_value = 0;

#if defined( _DEBUG )      
      WFCTRACEVAL( TEXT( "Network server name is " ), wide_machine_name);
#endif // _DEBUG

      return_value = ::RasAdminPortEnum( (LPCWSTR) wide_machine_name, &m_PortArray, &m_TotalNumberOfEntries );

      if ( return_value != ERROR_SUCCESS )
      {
         WFCTRACEVAL( TEXT( "RasAdminPortEnum failed with " ), return_value );
         m_Initialize();
         m_ErrorCode = return_value;
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

_Check_return_ DWORD CRemoteAccessServiceAdministration::GetErrorCode( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( m_ErrorCode );
}

void CRemoteAccessServiceAdministration::GetErrorString( const UINT error_code, std::wstring& error_string ) const
{
   WFC_VALIDATE_POINTER( this );

   WCHAR temp_string[ 512 ];

   ::ZeroMemory( temp_string, sizeof( temp_string ) );

#if 1
   // 2002-09-30
   // In the July 2002 Platform SDK, Microsoft got rid of the RasAdminGetErrorString() API
   Win32FoundationClasses::wfc_get_error_string( error_code, error_string );
#else
   DWORD return_value = 0;

   return_value = ::RasAdminGetErrorString( error_code, temp_string, 511 );

   if ( return_value == ERROR_SUCCESS )
   {
      ::wfc_convert_lpcwstr_to_cstring( temp_string, error_string );
   }
   else
   {
      error_string.Empty();
   }
#endif
}

BOOL CRemoteAccessServiceAdministration::GetNext( CRemoteAccessServicePort& port )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_PortArray );

   port.Empty();

   if ( m_PortArray == nullptr )
   {
      return( FALSE );
   }

   if ( m_CurrentEntryNumber < m_TotalNumberOfEntries )
   {
      port.Copy( &m_PortArray[ m_CurrentEntryNumber ] );
      m_CurrentEntryNumber++;
   }
   else
   {
      return( FALSE );
   }

   return( TRUE );
}

BOOL CRemoteAccessServiceAdministration::GetUserAccountServer(std::wstring& user_account_server_name, LPCTSTR ras_server_name, LPCTSTR ras_server_domain_name )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( ras_server_name );
   WFC_VALIDATE_POINTER_NULL_OK( ras_server_domain_name );

   // Idiot proof the parameters

   if ( ras_server_domain_name == nullptr and ras_server_name == nullptr )
   {
      m_ErrorCode = ERROR_INVALID_PARAMETER;
      return( FALSE );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // OK, at least one of the parameters is not null. This is a good thing.

      // Get ready to receive PDC name

      WCHAR primary_domain_controller_name[ UNCLEN + 1 ];

      ::ZeroMemory( primary_domain_controller_name, sizeof( primary_domain_controller_name ) );

      DWORD return_value = 0;

      if ( ras_server_domain_name != nullptr and ras_server_name != nullptr )
      {
         // CNetwork is cool because it will convert the name to a real server name
         // taking into account UNICODE and prepending

         CNetwork network_server_name( ras_server_name );
         CNetwork network_domain_name( ras_server_domain_name );

         return_value = ::RasAdminGetUserAccountServer( (LPCWSTR) network_domain_name, (LPCWSTR) network_server_name, primary_domain_controller_name );
      }
      else if ( ras_server_domain_name == nullptr )
      {
         CNetwork network_server_name( ras_server_name );

         return_value = ::RasAdminGetUserAccountServer( nullptr, (LPCWSTR) network_server_name, primary_domain_controller_name );
      }
      else
      {
         CNetwork network_domain_name( ras_server_domain_name );

         return_value = ::RasAdminGetUserAccountServer( (LPCWSTR) network_domain_name, nullptr, primary_domain_controller_name );
      }

      if ( return_value == ERROR_SUCCESS )
      {
         ::wfc_convert_lpcwstr_to_cstring( primary_domain_controller_name, user_account_server_name );
         return( TRUE );
      }
 
      user_account_server_name.Empty();
      return( FALSE );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

void CRemoteAccessServiceAdministration::m_Initialize( void )
{
   WFC_VALIDATE_POINTER( this );

   m_ErrorCode            = 0;
   m_TotalNumberOfEntries = 0;
   m_CurrentEntryNumber   = 0;

   if ( m_PortArray != nullptr )
   {
      ::RasAdminFreeBuffer( m_PortArray );
   }

   m_PortArray = nullptr;
}

#endif // _MSC_VER
#endif // WFC_NO_RAS

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CRemoteAccessServiceAdministration</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, RAS, Remote Access Service, Dialup networking, source code">
<META name="description" content="The C++ class that encapsulates Remote Access Service (Dialup networking) administration.">
</HEAD>

<BODY>

<H1>CRemoteAccessServiceAdministration : CObject</H1>
$Revision: 23 $
<HR>

<H2>Description</H2>

This class handles Remote Access Service Administration. It is
still under development.

<H2>Data Members</H2><B>None.</B>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="GetErrorString">GetErrorString</A></B>( const UINT error_code, std::wstring&amp; error_string ) const</PRE><DD>

<DT><PRE>DWORD <B><A NAME="GetErrorCode">GetErrorCode</A></B>( void ) const</PRE><DD>
Returns the error code should any method return FALSE.

<DT><PRE>BOOL <B><A NAME="GetUserAccountServer">GetUserAccountServer</A></B>( std::wstring&amp; user_account_server_name, LPCTSTR ras_server_name, LPCTSTR ras_server_domain_name = NULL )</PRE><DD>

<DT><PRE>BOOL <B><A NAME="ClearPortStatistics">ClearPortStatistics</A></B>( const std::wstring&amp; port_name, LPCTSTR server_name = NULL )</PRE><DD>

<DT><PRE>BOOL <B><A NAME="DisconnectPort">DisconnectPort</A></B>( const std::wstring&amp; port_name, LPCTSTR server_name = NULL )</PRE><DD>

<DT><PRE>BOOL <B><A NAME="EnumeratePorts">EnumeratePorts</A></B>( LPCTSTR server_name = NULL )</PRE><DD>
Let's you enumerator the RAS ports on the local machine or on
a machine on the network.

<DT><PRE>BOOL <B><A NAME="GetNext">GetNext</A></B>( CRemoteAccessServicePort&amp; port )</PRE><DD>

</DL>

<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>

<H2>API's Used</H2>

<B>CRemoteAccessServiceAdministration</B> uses the following API's:

<UL>
<LI>GetComputerNameEx
<LI>RasAdminFreeBuffer
<LI>RasAdminGetErrorString
<LI>RasAdminGetUserAccountServer
<LI>RasAdminPortClearStatistics
<LI>RasAdminPortDisconnect
<LI>RasAdminPortEnum
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CRASADM.CPP $<BR>
$Modtime: 6/26/01 10:51a $
</BODY>

</HTML>
*/

