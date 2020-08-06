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
** $Workfile: cnetfile.hpp $
** $Revision: 14 $
** $Modtime: 6/26/01 11:02a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( NETWORK_FILE_CLASS_HEADER )

#define NETWORK_FILE_CLASS_HEADER

class CNetworkFileInformation
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CNetworkFileInformation() noexcept;

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      explicit CNetworkFileInformation( _In_ FILE_INFO_3 const * information_p ) noexcept;
      explicit CNetworkFileInformation( _In_ CNetworkFileInformation const& source ) noexcept;

      DWORD   ID{ 0 };
      DWORD   Permissions{ 0 };
      DWORD   NumberOfLocks{ 0 };
      std::wstring PathName;
      std::wstring UserName;

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      void Copy( _In_ FILE_INFO_3 const * source ) noexcept;
      void Copy( _In_ CNetworkFileInformation const& source ) noexcept;
      void Empty( void ) noexcept;
      CNetworkFileInformation const& operator = ( _In_ CNetworkFileInformation const& source ) noexcept;
};

class CNetworkFiles : public CNetwork
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      /*
      ** Workstation information variables
      */

       FILE_INFO_3 * m_103InformationBuffer{ nullptr };

      /*
      ** File Information enumeration variables
      */

       DWORD m_103Index{ 0 };
       DWORD m_103ResumeHandle{ 0 };
       DWORD m_103CurrentEntryNumber{ 0 };
       DWORD m_103NumberOfEntriesRead{ 0 };
       DWORD m_103TotalNumberOfEntries{ 0 };

      bool m_GetChunk( void ) noexcept;

   public:

       CNetworkFiles(CNetworkFiles const&) = delete;
       CNetworkFiles& operator=(CNetworkFiles const&) = delete;
      CNetworkFiles(_In_ std::wstring_view machine_name = { nullptr, 0 });
      virtual ~CNetworkFiles();

      virtual void  Close( void ) noexcept;
      virtual bool  Close( __inout CNetworkFileInformation& file_to_close ) noexcept;
      virtual _Check_return_ bool  Enumerate( void ) noexcept;
      virtual _Check_return_ bool  GetNext( __inout CNetworkFileInformation& information ) noexcept;
};

#endif // NETWORK_FILE_CLASS_HEADER
