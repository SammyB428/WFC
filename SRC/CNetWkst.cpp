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
** $Workfile: cnetwkst.cpp $
** $Revision: 21 $
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

/*
** CWorkstationUser stuff
*/

CWorkstationUser::CWorkstationUser()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CWorkstationUser::CWorkstationUser( __in WKSTA_USER_INFO_1 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CWorkstationUser::CWorkstationUser( __in CWorkstationUser const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CWorkstationUser::~CWorkstationUser()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

/*
** Can't make Copy take a const pointer because Microsoft screwed up the 
** net API header files...
*/

void CWorkstationUser::Copy( __in WKSTA_USER_INFO_1 const * source ) noexcept
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
      UserName.assign( source->wkui1_username );
      LogonDomain.assign( source->wkui1_logon_domain );
      OtherDomains.assign( source->wkui1_oth_domains );
      LogonServer.assign( source->wkui1_logon_server );
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void CWorkstationUser::Copy( __in CWorkstationUser const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this != &source );

   /*
   ** Make sure we ain't copying ourselves
   */

   if ( this == &source )
   {
      return;
   }

   UserName.assign(source.UserName);
   LogonDomain.assign(source.LogonDomain);
   OtherDomains.assign(source.OtherDomains);
   LogonServer.assign(source.LogonServer);
}

void CWorkstationUser::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void CWorkstationUser::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   UserName.clear();
   LogonDomain.clear();
   OtherDomains.clear();
   LogonServer.clear();
}

CWorkstationUser const& CWorkstationUser::operator = ( __in CWorkstationUser const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** CWorkstationInformation stuff
*/

CWorkstationInformation::CWorkstationInformation()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CWorkstationInformation::CWorkstationInformation( __in WKSTA_INFO_100 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CWorkstationInformation::CWorkstationInformation( __in WKSTA_INFO_101 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CWorkstationInformation::CWorkstationInformation( __in CWorkstationInformation const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CWorkstationInformation::~CWorkstationInformation()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void CWorkstationInformation::Copy( __in WKSTA_INFO_100 const * source ) noexcept
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
      ComputerName.assign( source->wki100_computername );
      LANGroup.assign( source->wki100_langroup );

      PlatformID            = source->wki100_platform_id;
      MajorVersion          = source->wki100_ver_major;
      MinorVersion          = source->wki100_ver_minor;
      LANRoot.clear();
      NumberOfLoggedOnUsers = 0;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void CWorkstationInformation::Copy( __in WKSTA_INFO_101 const * source ) noexcept
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
      ComputerName.assign( source->wki101_computername );
      LANGroup.assign( source->wki101_langroup );
      LANRoot.assign( source->wki101_lanroot );

      PlatformID            = source->wki101_platform_id;
      MajorVersion          = source->wki101_ver_major;
      MinorVersion          = source->wki101_ver_minor;
      NumberOfLoggedOnUsers = 0;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void CWorkstationInformation::Copy( __in WKSTA_INFO_102 const * source ) noexcept
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
      ComputerName.assign( source->wki102_computername );
      LANGroup.assign( source->wki102_langroup );
      LANRoot.assign( source->wki102_lanroot );

      PlatformID            = source->wki102_platform_id;
      MajorVersion          = source->wki102_ver_major;
      MinorVersion          = source->wki102_ver_minor;
      NumberOfLoggedOnUsers = source->wki102_logged_on_users;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void CWorkstationInformation::Copy( __in CWorkstationInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this != &source );

   /*
   ** Make sure we ain't copying ourselves
   */

   if ( this == &source )
   {
      return;
   }

   PlatformID            = source.PlatformID;
   ComputerName.assign( source.ComputerName );
   LANGroup.assign( source.LANGroup );
   MajorVersion          = source.MajorVersion;
   MinorVersion          = source.MinorVersion;
   LANRoot.assign(source.LANRoot);
   NumberOfLoggedOnUsers = source.NumberOfLoggedOnUsers;
}

void CWorkstationInformation::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void CWorkstationInformation::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   PlatformID            = 0;
   ComputerName.clear();
   LANGroup.clear();
   MajorVersion          = 0;
   MinorVersion          = 0;
   LANRoot.clear();
   NumberOfLoggedOnUsers = 0;
}

CWorkstationInformation const& CWorkstationInformation::operator = ( __in CWorkstationInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** CWorkstationTransport stuff
*/

CWorkstationTransport::CWorkstationTransport() noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CWorkstationTransport::CWorkstationTransport( __in WKSTA_TRANSPORT_INFO_0 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CWorkstationTransport::CWorkstationTransport( __in CWorkstationTransport const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CWorkstationTransport::~CWorkstationTransport()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void CWorkstationTransport::Copy( __in WKSTA_TRANSPORT_INFO_0 const * source ) noexcept
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
      Name.assign( source->wkti0_transport_name );
      Address.assign( source->wkti0_transport_address );

      QualityOfService        = source->wkti0_quality_of_service;
      NumberOfVirtualCircuits = source->wkti0_number_of_vcs;
      WANish                  = source->wkti0_wan_ish == TRUE ? true : false;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void CWorkstationTransport::Copy( __in CWorkstationTransport const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this != &source );

   /*
   ** Make sure we ain't copying ourselves
   */

   if ( this == &source )
   {
      return;
   }

   QualityOfService        = source.QualityOfService;
   NumberOfVirtualCircuits = source.NumberOfVirtualCircuits;
   Name.assign( source.Name );
   Address.assign( source.Address );
   WANish                  = source.WANish;
}

void CWorkstationTransport::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void CWorkstationTransport::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   QualityOfService        = 0;
   NumberOfVirtualCircuits = 0;
   Name.clear();
   Address.clear();
   WANish = false;
}

CWorkstationTransport const& CWorkstationTransport::operator = ( __in CWorkstationTransport const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** CNetWorkstation Stuff
*/

CNetWorkstation::CNetWorkstation()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CNetWorkstation::CNetWorkstation(__in_z_opt LPCTSTR machine_name )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( machine_name );

   m_Initialize();
   Open( machine_name );
}

CNetWorkstation::~CNetWorkstation()
{
   WFC_VALIDATE_POINTER( this );
   Close();
}

void CNetWorkstation::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   CNetwork::Close();

   if ( m_InformationBuffer100 != nullptr )
   {
      ::NetApiBufferFree( m_InformationBuffer100 );
      m_InformationBuffer100 = nullptr;
   }

   if ( m_InformationBuffer101 != nullptr )
   {
      ::NetApiBufferFree( m_InformationBuffer101 );
      m_InformationBuffer101 = nullptr;
   }

   if ( m_InformationBuffer102 != nullptr )
   {
      ::NetApiBufferFree( m_InformationBuffer102 );
      m_InformationBuffer102 = nullptr;
   }

   if ( m_TransportBuffer != nullptr )
   {
      ::NetApiBufferFree( m_TransportBuffer );
      m_TransportBuffer = nullptr;
   }

   if ( m_UserBuffer != nullptr )
   {
      ::NetApiBufferFree( m_UserBuffer );
      m_UserBuffer = nullptr;
   }

   m_TransportBuffer = nullptr;
   m_UserBuffer      = nullptr;
}

void CNetWorkstation::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_ErrorCode                     = 0;
   m_InformationBuffer100          = nullptr;
   m_InformationBuffer101          = nullptr;
   m_InformationBuffer102          = nullptr;
   m_TransportBuffer               = nullptr;
   m_TransportResumeHandle         = 0;
   m_TransportCurrentEntryNumber   = 0;
   m_TransportNumberOfEntriesRead  = 0;
   m_TransportTotalNumberOfEntries = 0;
   m_UserBuffer                    = nullptr;
   m_UserResumeHandle              = 0;
   m_UserCurrentEntryNumber        = 0;
   m_UserNumberOfEntriesRead       = 0;
   m_UserTotalNumberOfEntries      = 0;
}

_Check_return_ BOOL CNetWorkstation::EnumerateInformation( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_InformationBuffer100 != nullptr )
   {
      ::NetApiBufferFree( m_InformationBuffer100 );
      m_InformationBuffer100 = nullptr;
   }

   if ( m_InformationBuffer101 != nullptr )
   {
      ::NetApiBufferFree( m_InformationBuffer101 );
      m_InformationBuffer101 = nullptr;
   }

   if ( m_InformationBuffer102 != nullptr )
   {
      ::NetApiBufferFree( m_InformationBuffer102 );
      m_InformationBuffer102 = nullptr;
   }

   m_ErrorCode = ::NetWkstaGetInfo( (LMSTR) m_WideMachineName.get(), 102, (LPBYTE *) &m_InformationBuffer102 );

   if ( m_ErrorCode == ERROR_ACCESS_DENIED )
   {
      m_ErrorCode = ::NetWkstaGetInfo( (LMSTR) m_WideMachineName.get(), 101, (LPBYTE *) &m_InformationBuffer101 );

      if ( m_ErrorCode == ERROR_ACCESS_DENIED )
      {
         m_ErrorCode = ::NetWkstaGetInfo( (LMSTR) m_WideMachineName.get(), 100, (LPBYTE *) &m_InformationBuffer100 );

         if ( m_ErrorCode != NERR_Success || m_InformationBuffer100 == nullptr )
         {
            return( FALSE );
         }
      }
      else if ( m_ErrorCode != NERR_Success || m_InformationBuffer101 == nullptr )
      {
         return( FALSE );
      }
   }
   else if ( m_ErrorCode != NERR_Success || m_InformationBuffer102 == nullptr )
   {
      return( FALSE );
   }

   return( TRUE );
}

_Check_return_ BOOL CNetWorkstation::EnumerateTransports( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_TransportBuffer != nullptr )
   {
      ::NetApiBufferFree( m_TransportBuffer );
      m_TransportBuffer = nullptr;
   }

   m_TransportCurrentEntryNumber   = 0;
   m_TransportNumberOfEntriesRead  = 0;
   m_TransportResumeHandle         = 0;
   m_TransportTotalNumberOfEntries = 0;

   m_ErrorCode = ::NetWkstaTransportEnum( (LMSTR) m_WideMachineName.get(),
                                                  0, 
                                      (LPBYTE *) &m_TransportBuffer,
                                                  65536,
                                                 &m_TransportNumberOfEntriesRead,
                                                 &m_TransportTotalNumberOfEntries,
                                                 &m_TransportResumeHandle );

   if ( m_ErrorCode != NERR_Success || m_TransportBuffer == nullptr || m_TransportNumberOfEntriesRead == 0 )
   {
      return( FALSE );
   }

   return( TRUE );
}

_Check_return_ BOOL CNetWorkstation::EnumerateUsers( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_UserBuffer != nullptr )
   {
      ::NetApiBufferFree( m_UserBuffer );
      m_UserBuffer = nullptr;
   }

   m_UserCurrentEntryNumber   = 0;
   m_UserNumberOfEntriesRead  = 0;
   m_UserTotalNumberOfEntries = 0;
   m_UserResumeHandle         = 0;

   m_ErrorCode = ::NetWkstaUserEnum( (LMSTR) m_WideMachineName.get(),
                                     1,
                         (LPBYTE *) &m_UserBuffer,
                                     65536,
                                    &m_UserNumberOfEntriesRead,
                                    &m_UserTotalNumberOfEntries,
                                    &m_UserResumeHandle );

   if ( m_ErrorCode != NERR_Success || m_UserBuffer == nullptr || m_UserNumberOfEntriesRead == 0 )
   {
      return( FALSE );
   }

   return( TRUE );
}

_Check_return_ BOOL CNetWorkstation::GetCurrentUser( __inout CWorkstationUser& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   WKSTA_USER_INFO_1 * buffer = nullptr;

   m_ErrorCode = ::NetWkstaUserGetInfo( nullptr, 1, (LPBYTE *) &buffer );

   if ( m_ErrorCode != NERR_Success || buffer == nullptr )
   {
      information.Empty();
      return( FALSE );
   }

   information.Copy( buffer );

   ::NetApiBufferFree( buffer );

   return( TRUE );
}

_Check_return_ BOOL CNetWorkstation::GetNext( __inout CWorkstationInformation& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_InformationBuffer102 != nullptr )
   {
      information.Copy( m_InformationBuffer102 );
      ::NetApiBufferFree( m_InformationBuffer102 );
      m_InformationBuffer102 = nullptr;
      return( TRUE );
   }

   if ( m_InformationBuffer101 != nullptr )
   {
      information.Copy( m_InformationBuffer101 );
      ::NetApiBufferFree( m_InformationBuffer101 );
      m_InformationBuffer101 = nullptr;
      return( TRUE );
   }

   if ( m_InformationBuffer100 != nullptr )
   {
      information.Copy( m_InformationBuffer100 );
      ::NetApiBufferFree( m_InformationBuffer100 );
      m_InformationBuffer100 = nullptr;
      return( TRUE );
   }

   information.Empty();

   return( FALSE );
}

_Check_return_ BOOL CNetWorkstation::GetNext( __inout CWorkstationTransport& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_TransportCurrentEntryNumber < m_TransportNumberOfEntriesRead )
   {
      information.Copy( &m_TransportBuffer[ m_TransportCurrentEntryNumber ] );
      m_TransportCurrentEntryNumber++;
      return( TRUE );
   }

   information.Empty();

   // Go ahead and clean up

   if ( m_TransportBuffer != nullptr )
   {
      ::NetApiBufferFree( m_TransportBuffer );
      m_TransportBuffer = nullptr;
   }

   m_TransportCurrentEntryNumber   = 0;
   m_TransportNumberOfEntriesRead  = 0;
   m_TransportTotalNumberOfEntries = 0;

   return( FALSE );
}

_Check_return_ BOOL CNetWorkstation::GetNext( __inout CWorkstationUser& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_UserCurrentEntryNumber < m_UserTotalNumberOfEntries )
   {
      information.Copy( &m_UserBuffer[ m_UserCurrentEntryNumber ] );
      m_UserCurrentEntryNumber++;
      return( TRUE );
   }

   information.Empty();
   return( FALSE );
}

// End of source
