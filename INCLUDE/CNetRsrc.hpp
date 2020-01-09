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
** $Workfile: cnetrsrc.hpp $
** $Revision: 13 $
** $Modtime: 6/26/01 11:02a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( NET_RESOURCE_CLASS_HEADER )

#define NET_RESOURCE_CLASS_HEADER

class CNetworkResourceInformation
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CNetworkResourceInformation();
      CNetworkResourceInformation( __in NETRESOURCE const * source );
      CNetworkResourceInformation( __in CNetworkResourceInformation const& source );

      virtual ~CNetworkResourceInformation();

      /*
      ** Patterned after NETRESOURCE
      */

      std::wstring   LocalName;
      std::wstring   RemoteName;
      std::wstring   Comment;
      std::wstring   Provider;
      DWORD Scope{ 0 };
      DWORD Type{ 0 };
      DWORD DisplayType{ 0 };
      DWORD Usage{ 0 };

      virtual void Copy( __in NETRESOURCE const * source ) noexcept;
      virtual void Copy( __in CNetworkResourceInformation const& source ) noexcept;
      virtual void Empty( void ) noexcept;
};

class CNetworkResources : public CNetwork
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

       HANDLE m_ResumeHandle{ nullptr };

       NETRESOURCE m_NetResource{ 0 };

   public:

       enum class Scope
       {
           Connected = RESOURCE_CONNECTED,
           All = RESOURCE_GLOBALNET,
           Persistent = RESOURCE_REMEMBERED,
           Recent = RESOURCE_RECENT,
           Context = RESOURCE_CONTEXT
       };

      enum Type  {
                    typeAny      = RESOURCETYPE_ANY,
                    typeDisk     = RESOURCETYPE_DISK,
                    typePrint    = RESOURCETYPE_PRINT,
                    typeReserved = RESOURCETYPE_RESERVED,
                    typeUnknown  = RESOURCETYPE_UNKNOWN
                 };

      enum Usage {
                    usageAll           = RESOURCEUSAGE_ALL,
                    usageConnectable   = RESOURCEUSAGE_CONNECTABLE,
                    usageContainer     = RESOURCEUSAGE_CONTAINER,
                    usageNoLocalDevice = RESOURCEUSAGE_NOLOCALDEVICE,
                    usageSibling       = RESOURCEUSAGE_SIBLING,
                    usageAttached      = RESOURCEUSAGE_ATTACHED,
                    usageReserved      = RESOURCEUSAGE_RESERVED
                 };

      CNetworkResources(__in CNetworkResources const&) = delete;
      CNetworkResources& operator=(__in CNetworkResources const&) = delete;
      CNetworkResources(_In_ std::wstring_view machine_name = { nullptr, 0 });
      virtual ~CNetworkResources();

      virtual _Check_return_ bool Enumerate( __inout CNetworkResourceInformation& information ) noexcept;
      virtual _Check_return_ bool GetNext( __inout CNetworkResourceInformation& information ) noexcept;
};

#endif // NET_RESOURCE_CLASS_HEADER
