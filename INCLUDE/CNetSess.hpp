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
** $Workfile: cnetsess.hpp $
** $Revision: 14 $
** $Modtime: 6/26/01 11:02a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( NET_SESSION_CLASS_HEADER )

#define NET_SESSION_CLASS_HEADER

class CNetworkSessionInformation
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CNetworkSessionInformation() noexcept;
      explicit CNetworkSessionInformation( _In_ SESSION_INFO_502 const * source ) noexcept;
      explicit CNetworkSessionInformation( _In_ CNetworkSessionInformation const& source ) noexcept;

      /*
      ** Patterned after SESSION_INFO_502
      */

      std::wstring   ClientName;
      std::wstring   UserName;
      DWORD     NumberOfOpens{ 0 };
      CTimeSpan Time;
      CTimeSpan IdleTime;
      DWORD     UserFlags{ 0 };
      std::wstring   ClientTypeName;
      std::wstring   Transport;

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      void Copy( _In_ SESSION_INFO_502 const * source ) noexcept;
      void Copy( _In_ CNetworkSessionInformation const& source ) noexcept;
      void Empty( void ) noexcept;

      CNetworkSessionInformation& operator=( _In_ CNetworkSessionInformation const& ) noexcept;
};

class CNetworkSessions : public CNetwork
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      std::wstring m_ServerName;
      std::wstring m_ClientName;

      bool m_Retrieved0{ false };
      bool m_Retrieved1{ false };
      bool m_Retrieved2{ false };
      bool m_Retrieved10{ false };
      bool m_Retrieved502{ false };

      DWORD m_ResumeHandle{ 0 };

      virtual void m_Get_0_Data( void ) noexcept;
      virtual void m_Get_1_Data( void ) noexcept;
      virtual void m_Get_2_Data( void ) noexcept;
      virtual void m_Get_10_Data( void ) noexcept;
      virtual void m_Get_502_Data( void ) noexcept;

   public:

       CNetworkSessions(CNetworkSessions const&) = delete;
       CNetworkSessions& operator=(CNetworkSessions const&) = delete;
      CNetworkSessions(_In_ std::wstring_view machine_name = { nullptr, 0 }) noexcept;
      virtual ~CNetworkSessions();

      virtual void  Close( void ) noexcept;
      virtual _Check_return_ bool Delete( __inout CNetworkSessionInformation& session_to_delete ) noexcept;
      virtual _Check_return_ bool Enumerate( __inout CNetworkSessionInformation& information ) noexcept;
      virtual _Check_return_ bool GetNext( __inout CNetworkSessionInformation& information ) noexcept;
};

#endif // NET_SESSION_CLASS_HEADER
