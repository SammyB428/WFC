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
** $Workfile: cnetwkst.hpp $
** $Revision: 14 $
** $Modtime: 6/26/01 11:03a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( NET_WORKSTATION_CLASS_HEADER )

#define NET_WORKSTATION_CLASS_HEADER

class CWorkstationUser
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CWorkstationUser() noexcept;

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      explicit CWorkstationUser( _In_ WKSTA_USER_INFO_1 const * source ) noexcept;
      explicit CWorkstationUser( _In_ CWorkstationUser const& source ) noexcept;

      /*
      ** Patterned after WKSTA_USER_INFO_1
      */

      std::wstring UserName;
      std::wstring LogonDomain;
      std::wstring OtherDomains;
      std::wstring LogonServer;

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      void Copy( _In_ WKSTA_USER_INFO_1 const * source ) noexcept;
      void Copy( _In_ CWorkstationUser const& source ) noexcept;
      void Empty( void ) noexcept;
      CWorkstationUser const& operator = ( _In_ CWorkstationUser const& source ) noexcept;
};

class CWorkstationInformation
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CWorkstationInformation() noexcept;

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      explicit CWorkstationInformation( _In_ WKSTA_INFO_100 const * source ) noexcept;
      explicit CWorkstationInformation( _In_ WKSTA_INFO_101 const * source ) noexcept;
      explicit CWorkstationInformation( _In_ WKSTA_INFO_102 const * source ) noexcept;
      explicit CWorkstationInformation( _In_ CWorkstationInformation const& source ) noexcept;

      /*
      ** Patterned after WKSTA_INFO_102
      */

      DWORD PlatformID{ 0 };
      std::wstring ComputerName;
      std::wstring LANGroup;
      DWORD MajorVersion{ 0 };
      DWORD MinorVersion{ 0 };
      std::wstring LANRoot;
      DWORD NumberOfLoggedOnUsers{ 0 };

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      void Copy( _In_ WKSTA_INFO_100 const * source ) noexcept;
      void Copy( _In_ WKSTA_INFO_101 const * source ) noexcept;
      void Copy( _In_ WKSTA_INFO_102 const * source ) noexcept;
      void Copy( _In_ CWorkstationInformation const& source ) noexcept;
      void Empty( void ) noexcept;
      CWorkstationInformation const& operator = ( _In_ CWorkstationInformation const& source ) noexcept;
};

class CWorkstationTransport
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CWorkstationTransport() noexcept;

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      explicit CWorkstationTransport( _In_ WKSTA_TRANSPORT_INFO_0 const * source ) noexcept;
      explicit CWorkstationTransport( _In_ CWorkstationTransport const& source ) noexcept;

      /*
      ** Patterned after WKSTA_USER_INFO_1
      */

      DWORD QualityOfService{ 0 };
      DWORD NumberOfVirtualCircuits{ 0 };
      std::wstring Name;
      std::wstring Address;
      bool WANish{ false };

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      void Copy( _In_ WKSTA_TRANSPORT_INFO_0 const * source ) noexcept;
      void Copy( _In_ CWorkstationTransport const& source ) noexcept;
      void Empty( void )  noexcept;
      CWorkstationTransport const& operator = ( _In_ CWorkstationTransport const& source ) noexcept;
};

class CNetWorkstation : public CNetwork
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      /*
      ** Workstation information variables
      */

       WKSTA_INFO_100 * m_InformationBuffer100{ nullptr };
       WKSTA_INFO_101 * m_InformationBuffer101{ nullptr };
       WKSTA_INFO_102 * m_InformationBuffer102{ nullptr };

      /*
      ** Transport enumeration variables
      */

       WKSTA_TRANSPORT_INFO_0 * m_TransportBuffer{ nullptr };
       DWORD m_TransportResumeHandle{ 0 };
       DWORD m_TransportCurrentEntryNumber{ 0 };
       DWORD m_TransportNumberOfEntriesRead{ 0 };
       DWORD m_TransportTotalNumberOfEntries{ 0 };

      /*
      ** User enumeration variables
      */

       WKSTA_USER_INFO_1 * m_UserBuffer{ nullptr };
       DWORD m_UserResumeHandle{ 0 };
       DWORD m_UserCurrentEntryNumber{ 0 };
       DWORD m_UserNumberOfEntriesRead{ 0 };
       DWORD m_UserTotalNumberOfEntries{ 0 };

   public:

       CNetWorkstation(_In_ CNetWorkstation const&) = delete;
       CNetWorkstation& operator=(_In_ CNetWorkstation const&) = delete;
      CNetWorkstation(_In_ std::wstring_view machine_name = { nullptr, 0 }) noexcept;
      virtual ~CNetWorkstation();

      virtual void Close( void ) noexcept;
      virtual _Check_return_ bool EnumerateInformation( void ) noexcept;
      virtual _Check_return_ bool EnumerateTransports( void ) noexcept;
      virtual _Check_return_ bool EnumerateUsers( void ) noexcept;
      virtual _Check_return_ bool GetCurrentUser( __inout CWorkstationUser& information ) noexcept;
      virtual _Check_return_ bool GetNext( __inout CWorkstationInformation& information ) noexcept;
      virtual _Check_return_ bool GetNext( __inout CWorkstationTransport& information ) noexcept;
      virtual _Check_return_ bool GetNext( __inout CWorkstationUser& information ) noexcept;
};

#endif // NET_WORKSTATION_CLASS_HEADER
