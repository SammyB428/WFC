/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2015, Samuel R. Blackburn
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

#if ! defined( NET_WORKSTATION_CLASS_HEADER )

#define NET_WORKSTATION_CLASS_HEADER

class CWorkstationUser
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CWorkstationUser();

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      CWorkstationUser( __in WKSTA_USER_INFO_1 const * source );
      CWorkstationUser( __in CWorkstationUser const& source );
      virtual ~CWorkstationUser();

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

      virtual void Copy( __in WKSTA_USER_INFO_1 const * source ) noexcept;
      virtual void Copy( __in CWorkstationUser const& source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual CWorkstationUser const& operator = ( __in CWorkstationUser const& source ) noexcept;
};

class CWorkstationInformation
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CWorkstationInformation();

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      CWorkstationInformation( __in WKSTA_INFO_100 const * source );
      CWorkstationInformation( __in WKSTA_INFO_101 const * source );
      CWorkstationInformation( __in WKSTA_INFO_102 const * source );
      CWorkstationInformation( __in CWorkstationInformation const& source );
      virtual ~CWorkstationInformation();

      /*
      ** Patterned after WKSTA_INFO_102
      */

      DWORD   PlatformID{ 0 };
      std::wstring ComputerName;
      std::wstring LANGroup;
      DWORD   MajorVersion{ 0 };
      DWORD   MinorVersion{ 0 };
      std::wstring LANRoot;
      DWORD   NumberOfLoggedOnUsers{ 0 };

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      virtual void Copy( __in WKSTA_INFO_100 const * source ) noexcept;
      virtual void Copy( __in WKSTA_INFO_101 const * source ) noexcept;
      virtual void Copy( __in WKSTA_INFO_102 const * source ) noexcept;
      virtual void Copy( __in CWorkstationInformation const& source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual CWorkstationInformation const& operator = ( __in CWorkstationInformation const& source ) noexcept;
};

class CWorkstationTransport
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CWorkstationTransport();

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      CWorkstationTransport( __in WKSTA_TRANSPORT_INFO_0 const * source );
      CWorkstationTransport( __in CWorkstationTransport const& source );
      virtual ~CWorkstationTransport();

      /*
      ** Patterned after WKSTA_USER_INFO_1
      */

      DWORD   QualityOfService{ 0 };
      DWORD   NumberOfVirtualCircuits{ 0 };
      std::wstring Name;
      std::wstring Address;
      BOOL    WANish{ FALSE };

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      virtual void Copy( __in WKSTA_TRANSPORT_INFO_0 const * source ) noexcept;
      virtual void Copy( __in CWorkstationTransport const& source ) noexcept;
      virtual void Empty( void )  noexcept;
      virtual CWorkstationTransport const& operator = ( __in CWorkstationTransport const& source ) noexcept;
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

       CNetWorkstation(__in CNetWorkstation const&) = delete;
       CNetWorkstation& operator=(__in CNetWorkstation const&) = delete;
       CNetWorkstation();
      CNetWorkstation( __in_z_opt LPCTSTR machine_name );
      virtual ~CNetWorkstation();

      virtual void Close( void ) noexcept;
      virtual _Check_return_ BOOL EnumerateInformation( void ) noexcept;
      virtual _Check_return_ BOOL EnumerateTransports( void ) noexcept;
      virtual _Check_return_ BOOL EnumerateUsers( void ) noexcept;
      virtual _Check_return_ BOOL GetCurrentUser( __inout CWorkstationUser& information ) noexcept;
      virtual _Check_return_ BOOL GetNext( __inout CWorkstationInformation& information ) noexcept;
      virtual _Check_return_ BOOL GetNext( __inout CWorkstationTransport& information ) noexcept;
      virtual _Check_return_ BOOL GetNext( __inout CWorkstationUser& information ) noexcept;
};

#endif // NET_WORKSTATION_CLASS_HEADER
