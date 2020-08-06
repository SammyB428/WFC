/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2020, Samuel R. Blackburn
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

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

/*
** CWorkstationUser stuff
*/

Win32FoundationClasses::CWorkstationUser::CWorkstationUser() noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CWorkstationUser::CWorkstationUser( _In_ WKSTA_USER_INFO_1 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CWorkstationUser::CWorkstationUser( _In_ CWorkstationUser const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

/*
** Can't make Copy take a const pointer because Microsoft screwed up the 
** net API header files...
*/

void Win32FoundationClasses::CWorkstationUser::Copy( _In_ WKSTA_USER_INFO_1 const * source ) noexcept
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

void Win32FoundationClasses::CWorkstationUser::Copy( _In_ Win32FoundationClasses::CWorkstationUser const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this not_eq &source );

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

void Win32FoundationClasses::CWorkstationUser::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void Win32FoundationClasses::CWorkstationUser::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   UserName.clear();
   LogonDomain.clear();
   OtherDomains.clear();
   LogonServer.clear();
}

Win32FoundationClasses::CWorkstationUser const& Win32FoundationClasses::CWorkstationUser::operator = ( _In_ Win32FoundationClasses::CWorkstationUser const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** CWorkstationInformation stuff
*/

Win32FoundationClasses::CWorkstationInformation::CWorkstationInformation() noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CWorkstationInformation::CWorkstationInformation( _In_ WKSTA_INFO_100 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CWorkstationInformation::CWorkstationInformation( _In_ WKSTA_INFO_101 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CWorkstationInformation::CWorkstationInformation(_In_ WKSTA_INFO_102 const* source) noexcept
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER(source);
    Copy(source);
}

Win32FoundationClasses::CWorkstationInformation::CWorkstationInformation( _In_ Win32FoundationClasses::CWorkstationInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

void Win32FoundationClasses::CWorkstationInformation::Copy( _In_ WKSTA_INFO_100 const * source ) noexcept
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

void Win32FoundationClasses::CWorkstationInformation::Copy( _In_ WKSTA_INFO_101 const * source ) noexcept
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

void Win32FoundationClasses::CWorkstationInformation::Copy( _In_ WKSTA_INFO_102 const * source ) noexcept
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

void Win32FoundationClasses::CWorkstationInformation::Copy( _In_ Win32FoundationClasses::CWorkstationInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this not_eq &source );

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

void Win32FoundationClasses::CWorkstationInformation::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void Win32FoundationClasses::CWorkstationInformation::m_Initialize( void ) noexcept
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

Win32FoundationClasses::CWorkstationInformation const& Win32FoundationClasses::CWorkstationInformation::operator = ( _In_ Win32FoundationClasses::CWorkstationInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** CWorkstationTransport stuff
*/

Win32FoundationClasses::CWorkstationTransport::CWorkstationTransport() noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CWorkstationTransport::CWorkstationTransport( _In_ WKSTA_TRANSPORT_INFO_0 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CWorkstationTransport::CWorkstationTransport( _In_ Win32FoundationClasses::CWorkstationTransport const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

void Win32FoundationClasses::CWorkstationTransport::Copy( _In_ WKSTA_TRANSPORT_INFO_0 const * source ) noexcept
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

void Win32FoundationClasses::CWorkstationTransport::Copy( _In_ Win32FoundationClasses::CWorkstationTransport const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this not_eq &source );

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

void Win32FoundationClasses::CWorkstationTransport::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void Win32FoundationClasses::CWorkstationTransport::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   QualityOfService        = 0;
   NumberOfVirtualCircuits = 0;
   Name.clear();
   Address.clear();
   WANish = false;
}

Win32FoundationClasses::CWorkstationTransport const& Win32FoundationClasses::CWorkstationTransport::operator = ( _In_ Win32FoundationClasses::CWorkstationTransport const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** CNetWorkstation Stuff
*/

Win32FoundationClasses::CNetWorkstation::CNetWorkstation(_In_ std::wstring_view machine_name) noexcept : CNetwork(machine_name)
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CNetWorkstation::~CNetWorkstation() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Close();
}

void Win32FoundationClasses::CNetWorkstation::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   CNetwork::Close();

   if ( m_InformationBuffer100 not_eq nullptr )
   {
      ::NetApiBufferFree( m_InformationBuffer100 );
      m_InformationBuffer100 = nullptr;
   }

   if ( m_InformationBuffer101 not_eq nullptr )
   {
      ::NetApiBufferFree( m_InformationBuffer101 );
      m_InformationBuffer101 = nullptr;
   }

   if ( m_InformationBuffer102 not_eq nullptr )
   {
      ::NetApiBufferFree( m_InformationBuffer102 );
      m_InformationBuffer102 = nullptr;
   }

   if ( m_TransportBuffer not_eq nullptr )
   {
      ::NetApiBufferFree( m_TransportBuffer );
      m_TransportBuffer = nullptr;
   }

   if ( m_UserBuffer not_eq nullptr )
   {
      ::NetApiBufferFree( m_UserBuffer );
      m_UserBuffer = nullptr;
   }

   m_TransportBuffer = nullptr;
   m_UserBuffer      = nullptr;
}

void Win32FoundationClasses::CNetWorkstation::m_Initialize( void ) noexcept
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

_Check_return_ bool Win32FoundationClasses::CNetWorkstation::EnumerateInformation( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_InformationBuffer100 not_eq nullptr )
   {
      ::NetApiBufferFree( m_InformationBuffer100 );
      m_InformationBuffer100 = nullptr;
   }

   if ( m_InformationBuffer101 not_eq nullptr )
   {
      ::NetApiBufferFree( m_InformationBuffer101 );
      m_InformationBuffer101 = nullptr;
   }

   if ( m_InformationBuffer102 not_eq nullptr )
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

         if ( m_ErrorCode not_eq NERR_Success or m_InformationBuffer100 == nullptr )
         {
            return( false );
         }
      }
      else if ( m_ErrorCode not_eq NERR_Success or m_InformationBuffer101 == nullptr )
      {
         return( false );
      }
   }
   else if ( m_ErrorCode not_eq NERR_Success or m_InformationBuffer102 == nullptr )
   {
      return( false );
   }

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CNetWorkstation::EnumerateTransports( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_TransportBuffer not_eq nullptr )
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

   if ( m_ErrorCode not_eq NERR_Success or m_TransportBuffer == nullptr or m_TransportNumberOfEntriesRead == 0 )
   {
      return( false );
   }

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CNetWorkstation::EnumerateUsers( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_UserBuffer not_eq nullptr )
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

   if ( m_ErrorCode not_eq NERR_Success or m_UserBuffer == nullptr or m_UserNumberOfEntriesRead == 0 )
   {
      return( false );
   }

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CNetWorkstation::GetCurrentUser( __inout Win32FoundationClasses::CWorkstationUser& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   WKSTA_USER_INFO_1 * buffer = nullptr;

   m_ErrorCode = ::NetWkstaUserGetInfo( nullptr, 1, (LPBYTE *) &buffer );

   if ( m_ErrorCode not_eq NERR_Success or buffer == nullptr )
   {
      information.Empty();
      return( false );
   }

   information.Copy( buffer );

   (void) ::NetApiBufferFree( buffer );

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CNetWorkstation::GetNext( __inout Win32FoundationClasses::CWorkstationInformation& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_InformationBuffer102 not_eq nullptr )
   {
      information.Copy( m_InformationBuffer102 );
      ::NetApiBufferFree( m_InformationBuffer102 );
      m_InformationBuffer102 = nullptr;
      return( true );
   }

   if ( m_InformationBuffer101 not_eq nullptr )
   {
      information.Copy( m_InformationBuffer101 );
      ::NetApiBufferFree( m_InformationBuffer101 );
      m_InformationBuffer101 = nullptr;
      return( true );
   }

   if ( m_InformationBuffer100 not_eq nullptr )
   {
      information.Copy( m_InformationBuffer100 );
      ::NetApiBufferFree( m_InformationBuffer100 );
      m_InformationBuffer100 = nullptr;
      return( true );
   }

   information.Empty();

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CNetWorkstation::GetNext( __inout Win32FoundationClasses::CWorkstationTransport& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_TransportCurrentEntryNumber < m_TransportNumberOfEntriesRead )
   {
      information.Copy( &m_TransportBuffer[ m_TransportCurrentEntryNumber ] );
      m_TransportCurrentEntryNumber++;
      return( true );
   }

   information.Empty();

   // Go ahead and clean up

   if ( m_TransportBuffer not_eq nullptr )
   {
      ::NetApiBufferFree( m_TransportBuffer );
      m_TransportBuffer = nullptr;
   }

   m_TransportCurrentEntryNumber   = 0;
   m_TransportNumberOfEntriesRead  = 0;
   m_TransportTotalNumberOfEntries = 0;

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CNetWorkstation::GetNext( _Inout_ Win32FoundationClasses::CWorkstationUser& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_UserCurrentEntryNumber < m_UserTotalNumberOfEntries )
   {
      information.Copy( &m_UserBuffer[ m_UserCurrentEntryNumber ] );
      m_UserCurrentEntryNumber++;
      return( true );
   }

   information.Empty();
   return( false );
}

// End of source
