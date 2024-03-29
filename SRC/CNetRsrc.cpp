/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2022, Samuel R. Blackburn
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
** $Workfile: cnetrsrc.cpp $
** $Revision: 20 $
** $Modtime: 6/26/01 10:49a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#endif // _DEBUG

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

// TO DO: Add "net use" capability, WNetAddConnection

Win32FoundationClasses::CNetworkResources::CNetworkResources(_In_ std::wstring_view machine_name) : CNetwork(machine_name)
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CNetworkResources::~CNetworkResources()
{
   WFC_VALIDATE_POINTER( this );

   if ( m_ResumeHandle not_eq static_cast< HANDLE >( NULL ) )
   {
      //WFCTRACE( TEXT( "Closing Enumeration Handle" ) );
      std::ignore = ::WNetCloseEnum( m_ResumeHandle );
      m_ResumeHandle = static_cast< HANDLE >( NULL );
   }

   m_Initialize();
}

void Win32FoundationClasses::CNetworkResources::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_ErrorCode    = 0;
   m_ResumeHandle = static_cast< HANDLE >( NULL );
   ::ZeroMemory( &m_NetResource, sizeof( m_NetResource ) );
}

_Check_return_ bool Win32FoundationClasses::CNetworkResources::Enumerate( __inout Win32FoundationClasses::CNetworkResourceInformation& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_ResumeHandle not_eq static_cast< HANDLE >( NULL ) )
   {
      //WFCTRACE( TEXT( "Closing Enumeration Handle" ) );
      std::ignore = ::WNetCloseEnum( m_ResumeHandle );
      m_ResumeHandle = static_cast< HANDLE >( NULL );
   }

   ::ZeroMemory( &m_NetResource, sizeof( m_NetResource ) );
   m_NetResource.dwUsage = usageContainer;

   NETRESOURCE * net_resource_parameter{ nullptr };

   /*
   ** Let's see what we want to enumerate
   */

   switch( information.Scope )
   {
   case static_cast<DWORD>(Scope::Connected): // information.Usage is ignored

         break;
            
      case static_cast<DWORD>(Scope::Persistent):  // information.Usage is ignored

         break;

      case static_cast<DWORD>(Scope::All):

         break;

      default:

         m_ErrorCode = ERROR_INVALID_PARAMETER;
         return( false );
   }

   switch( information.Usage )
   {
      case usageAll:

         net_resource_parameter = nullptr;
         break;

      case usageConnectable:
      case usageContainer:
      case usageConnectable bitor usageContainer:

         net_resource_parameter = &m_NetResource;
         break;

      default:

         m_ErrorCode = ERROR_INVALID_PARAMETER;
         return( false );
   }

   m_ErrorCode = ::WNetOpenEnum( information.Scope, 
                                 information.Type, 
                                 information.Usage, 
                                 net_resource_parameter,
                                &m_ResumeHandle );

   if ( m_ErrorCode == NO_ERROR )
   {
      return( true );
   }
   else if ( m_ErrorCode == ERROR_EXTENDED_ERROR )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CNetworkResources::GetNext( __inout Win32FoundationClasses::CNetworkResourceInformation& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD number_of_entries{ 1 };
   DWORD size_of_buffer{ sizeof(m_NetResource) };

   m_ErrorCode = ::WNetEnumResource( m_ResumeHandle,
                                    &number_of_entries,
                                    &m_NetResource,
                                    &size_of_buffer );

   if ( m_ErrorCode == NO_ERROR )
   {
      information.Copy( &m_NetResource );
      return( true );
   }
   else if ( m_ErrorCode == ERROR_EXTENDED_ERROR )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
   }

   return( false );
}

// End of source
