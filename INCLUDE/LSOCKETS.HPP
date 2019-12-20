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
** $Workfile: lsockets.hpp $
** $Revision: 16 $
** $Modtime: 6/26/01 11:05a $
*/

#if ! defined( LISTENING_SOCKET_CLASS_HEADER )

#define LISTENING_SOCKET_CLASS_HEADER

class CListeningSocket : public CSimpleSocket
{
   private:

       int m_NumberOfSimultaneousSocketsToAllow{ 0 };

      unsigned long m_BindToAddress{ 0 };

      void m_Initialize( void ) noexcept;

   public:

       CListeningSocket(CListeningSocket const&) = delete;
       CListeningSocket& operator=(CListeningSocket const&) = delete;

       CListeningSocket() noexcept;
      //CListeningSocket( __in SOCKET client_id ) noexcept;

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CListeningSocket();

      virtual void OnNewConnection( _In_ SOCKET socket_id, __in_z LPCTSTR host_name, __in_z LPCTSTR dotted_ip_address_string ) = 0; // Called by WaitForConnection
      _Check_return_ bool Open( void ) noexcept override;
      _Check_return_ bool Open(_In_ std::filesystem::path const& channel_name, __in UINT const port_number = 23 ) noexcept override;

      virtual _Check_return_ bool WaitForConnection( __in_z LPCTSTR p_name ) noexcept;
      virtual _Check_return_ bool WaitForConnection( __in int const p_number ) noexcept;
      virtual _Check_return_ bool WaitForConnection( void ) noexcept;

      // 2000-02-13
      // Good addition suggested by Scott Kolek (scott@skm-software.com)

      virtual void SetBindToAddress( __in unsigned long const address = INADDR_ANY ) noexcept;
      virtual _Check_return_ unsigned long GetBindToAddress( void ) const noexcept;

      virtual void SetConnectionBacklog( __in int const backlog ) noexcept;
      virtual _Check_return_ int GetConnectionBacklog( void ) const noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // LISTENING_SOCKET_CLASS_HEADER
