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

      CWorkstationUser( __in const WKSTA_USER_INFO_1 * source );
      CWorkstationUser( __in const CWorkstationUser& source );
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

      virtual void Copy( __in const WKSTA_USER_INFO_1 * source ) noexcept;
      virtual void Copy( __in const CWorkstationUser& source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual const CWorkstationUser& operator = ( __in const CWorkstationUser& source ) noexcept;
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

      CWorkstationInformation( __in const WKSTA_INFO_100 * source );
      CWorkstationInformation( __in const WKSTA_INFO_101 * source );
      CWorkstationInformation( __in const WKSTA_INFO_102 * source );
      CWorkstationInformation( __in const CWorkstationInformation& source );
      virtual ~CWorkstationInformation();

      /*
      ** Patterned after WKSTA_INFO_102
      */

      DWORD   PlatformID;
      std::wstring ComputerName;
      std::wstring LANGroup;
      DWORD   MajorVersion;
      DWORD   MinorVersion;
      std::wstring LANRoot;
      DWORD   NumberOfLoggedOnUsers;

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      virtual void Copy( __in const WKSTA_INFO_100 * source ) noexcept;
      virtual void Copy( __in const WKSTA_INFO_101 * source ) noexcept;
      virtual void Copy( __in const WKSTA_INFO_102 * source ) noexcept;
      virtual void Copy( __in const CWorkstationInformation& source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual const CWorkstationInformation& operator = ( __in const CWorkstationInformation& source ) noexcept;
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

      CWorkstationTransport( __in const WKSTA_TRANSPORT_INFO_0 * source );
      CWorkstationTransport( __in const CWorkstationTransport& source );
      virtual ~CWorkstationTransport();

      /*
      ** Patterned after WKSTA_USER_INFO_1
      */

      DWORD   QualityOfService;
      DWORD   NumberOfVirtualCircuits;
      std::wstring Name;
      std::wstring Address;
      BOOL    WANish;

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      virtual void Copy( __in const WKSTA_TRANSPORT_INFO_0 * source ) noexcept;
      virtual void Copy( __in const CWorkstationTransport& source ) noexcept;
      virtual void Empty( void )  noexcept;
      virtual const CWorkstationTransport& operator = ( __in const CWorkstationTransport& source ) noexcept;
};

class CNetWorkstation : public CNetwork
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      /*
      ** Workstation information variables
      */

      WKSTA_INFO_100 * m_InformationBuffer100;
      WKSTA_INFO_101 * m_InformationBuffer101;
      WKSTA_INFO_102 * m_InformationBuffer102;

      /*
      ** Transport enumeration variables
      */

      WKSTA_TRANSPORT_INFO_0 * m_TransportBuffer;
      DWORD m_TransportResumeHandle;
      DWORD m_TransportCurrentEntryNumber;
      DWORD m_TransportNumberOfEntriesRead;
      DWORD m_TransportTotalNumberOfEntries;

      /*
      ** User enumeration variables
      */

      WKSTA_USER_INFO_1 * m_UserBuffer;
      DWORD m_UserResumeHandle;
      DWORD m_UserCurrentEntryNumber;
      DWORD m_UserNumberOfEntriesRead;
      DWORD m_UserTotalNumberOfEntries;

   public:

       CNetWorkstation(__in const CNetWorkstation&) = delete;
       CNetWorkstation& operator=(__in const CNetWorkstation&) = delete;
       CNetWorkstation();
      CNetWorkstation( __in_z_opt LPCTSTR machine_name );
      virtual ~CNetWorkstation();

      virtual void Close( void ) noexcept;
      virtual __checkReturn BOOL EnumerateInformation( void ) noexcept;
      virtual __checkReturn BOOL EnumerateTransports( void ) noexcept;
      virtual __checkReturn BOOL EnumerateUsers( void ) noexcept;
      virtual __checkReturn BOOL GetCurrentUser( __inout CWorkstationUser& information ) noexcept;
      virtual __checkReturn BOOL GetNext( __inout CWorkstationInformation& information ) noexcept;
      virtual __checkReturn BOOL GetNext( __inout CWorkstationTransport& information ) noexcept;
      virtual __checkReturn BOOL GetNext( __inout CWorkstationUser& information ) noexcept;
};

#endif // NET_WORKSTATION_CLASS_HEADER
