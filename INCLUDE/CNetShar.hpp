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
** $Workfile: cnetshar.hpp $
** $Revision: 15 $
** $Modtime: 6/26/01 11:02a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( NETWORK_SHARE_CLASS_HEADER )

#define NETWORK_SHARE_CLASS_HEADER

class CNetworkShareInformation
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CNetworkShareInformation() noexcept;
      explicit CNetworkShareInformation(_In_ SHARE_INFO_1 const * information_p ) noexcept;
      explicit CNetworkShareInformation(_In_ SHARE_INFO_2 const * information_p ) noexcept;
      explicit CNetworkShareInformation(_In_ CNetworkShareInformation const& source ) noexcept;

      std::wstring NetworkName;
      DWORD   Type{ 0 };
      std::wstring Remark;
      DWORD   Permissions{ 0 };
      DWORD   MaximumNumberOfUses{ 0 };
      DWORD   CurrentNumberOfUses{ 0 };
      std::wstring PathName;
      std::wstring Password;

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      void Copy( _In_ SHARE_INFO_1 const * source ) noexcept;
      void Copy(_In_ SHARE_INFO_2 const * source ) noexcept;
      void Copy(_In_ CNetworkShareInformation const& source ) noexcept;
      void Empty( void ) noexcept;
      CNetworkShareInformation const& operator = (_In_ CNetworkShareInformation const& source ) noexcept;
};

class CNetworkShares : public CNetwork
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      /*
      ** Connection information variables
      */

       SHARE_INFO_1* m_1InformationBuffer{ nullptr };
       SHARE_INFO_2* m_2InformationBuffer{ nullptr };

      /*
      ** File Information enumeration variables
      */

       DWORD m_1Index{ 0 };
       DWORD m_1ResumeHandle{ 0 };
       DWORD m_1CurrentEntryNumber{ 0 };
       DWORD m_1NumberOfEntriesRead{ 0 };
       DWORD m_1TotalNumberOfEntries{ 0 };

       DWORD m_2Index{ 0 };
       DWORD m_2ResumeHandle{ 0 };
       DWORD m_2CurrentEntryNumber{ 0 };
       DWORD m_2NumberOfEntriesRead{ 0 };
       DWORD m_2TotalNumberOfEntries{ 0 };

      _Check_return_ bool m_GetChunk( void ) noexcept;

   public:

       CNetworkShares(_In_ CNetworkShares const&) = delete;
       _Check_return_ CNetworkShares& operator=(_In_ CNetworkShares const&) = delete;
      CNetworkShares(_In_ std::wstring_view machine_name = { nullptr, 0 });
      virtual ~CNetworkShares();

      virtual _Check_return_ bool Add( _Inout_ CNetworkShareInformation& share_to_add ) noexcept;
      virtual _Check_return_ DWORD Check( _In_ std::wstring_view name_of_device ) noexcept;
      virtual void Close( void ) noexcept override;
      virtual _Check_return_ bool Delete(_Inout_ CNetworkShareInformation& share_to_delete ) noexcept;
      virtual _Check_return_ bool Enumerate( void ) noexcept;
      virtual _Check_return_ bool GetNext(_Inout_ CNetworkShareInformation& information ) noexcept;
      virtual void GetAll(std::vector<CNetworkShareInformation>& shares) noexcept;
};

#endif // NETWORK_SHARE_CLASS_HEADER
