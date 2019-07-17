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
** $Workfile: CRASADM.HPP $
** $Revision: 10 $
** $Modtime: 6/26/01 11:03a $
*/

#if ! defined( REMOTE_ACCESS_SERVICE_ADMINISTRATION_CLASS_HEADER )

#define REMOTE_ACCESS_SERVICE_ADMINISTRATION_CLASS_HEADER

#if ! defined( WFC_NO_RAS )

// 2000-10-30
// Thanks go to Danny Smith (danny_r_smith_2001@yahoo.co.nz) for
// making this compilable in GCC (added WFC_HAVE_RASSAPI).
// It was pretty stupid of me to assume that everyone uses Visual C++.

#if ( _MSC_VER >= 1020 ) || defined( WFC_HAVE_RASSAPI )

class CRemoteAccessServiceAdministration
{
   protected:

       RAS_PORT_0* m_PortArray{ nullptr };

       WORD m_TotalNumberOfEntries{ 0 };
       WORD m_CurrentEntryNumber{ 0 };

       UINT m_ErrorCode{ 0 };

      virtual void m_Initialize( void ) noexcept;

   public:

       CRemoteAccessServiceAdministration(CRemoteAccessServiceAdministration const&) = delete;
       CRemoteAccessServiceAdministration& operator=(CRemoteAccessServiceAdministration const&) = delete;
       
       CRemoteAccessServiceAdministration();

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRemoteAccessServiceAdministration();

      virtual void  GetErrorString( __in UINT const error_code, __out std::wstring& error_string ) const noexcept;
      virtual DWORD GetErrorCode( void ) const noexcept;
      virtual BOOL  GetUserAccountServer( __out std::wstring& user_account_server_name, __in_z LPCTSTR ras_server_name, __in_z_opt LPCTSTR ras_server_domain_name = nullptr ) noexcept;
      virtual BOOL  ClearPortStatistics( __in std::wstring const& port_name, __in_z_opt LPCTSTR server_name = nullptr ) noexcept; // Default to this machine
      virtual BOOL  DisconnectPort( __in std::wstring const& port_name, __in_z_opt LPCTSTR server_name = nullptr ) noexcept; // RasAdminPortDisconnect
      virtual _Check_return_ BOOL EnumeratePorts( __in_z_opt LPCTSTR server_name = nullptr ) noexcept; // RasAdminPortEnum
      virtual _Check_return_ BOOL GetNext( __out CRemoteAccessServicePort& port ) noexcept;
      //virtual BOOL  GetInformation( CRemoteAccessServicePort& port ); // RasAdminPortGetInfo
      //virtual BOOL  GetInformation(); // RasAdminServerGetInfo
      //virtual BOOL  GetInformation(); // RasAdminUserGetInfo
      //virtual BOOL  SetInformation(); // RasAdminUserSetInfo

      //virtual BOOL  AcceptNewConnection(); // RasAdminAcceptNewConnection
      //virtual BOOL  ConnectionHangupNotification(); // RasAdminConnectionHandgupNotification
      //virtual BOOL  GetIPAddressForUser(); // RasAdminGetIpAddressForUser
      //virtual BOOL  ReleaseIPAddress(); // RasAdminReleaseIpAddress

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // _MSC_VER
#endif // WFC_NO_RAS

#endif // REMOTE_ACCESS_SERVICE_ADMINISTRATION_CLASS_HEADER
