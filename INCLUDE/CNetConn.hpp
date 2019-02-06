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
** $Workfile: cnetconn.hpp $
** $Revision: 13 $
** $Modtime: 6/26/01 11:02a $
*/

#if ! defined( NETWORK_CONNECTION_CLASS_HEADER )

#define NETWORK_CONNECTION_CLASS_HEADER

class CNetworkConnectionInformation
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CNetworkConnectionInformation();

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      CNetworkConnectionInformation( __in const CONNECTION_INFO_1 *information_p );
      CNetworkConnectionInformation( __in const CNetworkConnectionInformation& source );
      virtual ~CNetworkConnectionInformation();

      DWORD   ID;
      DWORD   Type;
      DWORD   NumberOfUsers;
      DWORD   NumberOfOpens;
      DWORD   Time;
      std::wstring UserName;
      std::wstring NetName;

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      virtual void Copy( __in const CONNECTION_INFO_1 *source ) noexcept;
      virtual void Copy( __in const CNetworkConnectionInformation& source ) noexcept;
      virtual void Empty( void ) noexcept;

      virtual const CNetworkConnectionInformation& operator=( __in const CNetworkConnectionInformation& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

class CNetworkConnections : public CNetwork
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      /*
      ** Connection information variables
      */

      CONNECTION_INFO_1 * m_1InformationBuffer;

      /*
      ** File Information enumeration variables
      */

      DWORD m_1Index;
      DWORD m_1ResumeHandle;
      DWORD m_1CurrentEntryNumber;
      DWORD m_1NumberOfEntriesRead;
      DWORD m_1TotalNumberOfEntries;

      __checkReturn bool m_GetChunk( void ) noexcept;

      WCHAR m_WideShareOrComputerName[ MAX_PATH ];

public:

    CNetworkConnections(__in const CNetworkConnections&) = delete;
    CNetworkConnections& operator=(__in const CNetworkConnections&) = delete;
    CNetworkConnections();
      CNetworkConnections( __in_z_opt LPCTSTR machine_name );
      virtual ~CNetworkConnections();

      virtual void Close( void ) noexcept;
      virtual __checkReturn bool Enumerate( __in_z_opt LPCTSTR share_or_computer_name ) noexcept;
      virtual __checkReturn bool GetNext( __out CNetworkConnectionInformation& information ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

#endif // NETWORK_CONNECTION_CLASS_HEADER
