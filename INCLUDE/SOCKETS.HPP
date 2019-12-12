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
** $Workfile: sockets.hpp $
** $Revision: 24 $
** $Modtime: 7/12/01 8:00p $
*/

#if ! defined( SOCKET_CLASS_HEADER )

#define SOCKET_CLASS_HEADER

// 2001-07-12
// In the Beta 2 for Whistler Platform SDK, the children at Microsoft moved
// the definition for LPTRANSMIT_FILE_BUFFERS around (a problem when you want
// to use Winsock2.h). We will define it here to make sure it will compile.

#if ! defined( TF_DISCONNECT )

struct TRANSMIT_FILE_BUFFERS
{
    LPVOID Head{ nullptr };
    DWORD  HeadLength{ 0 };
    LPVOID Tail{ nullptr };
    DWORD  TailLength{ 0 };
}
TRANSMIT_FILE_BUFFERS;

#define TF_DISCONNECT   (0x01)
#define TF_REUSE_SOCKET (0x02)
#define TF_WRITE_BEHIND (0x04)

#endif // TF_DISCONNECT

#if ! defined( TF_USE_DEFAULT_WORKER )
#define TF_USE_DEFAULT_WORKER (0x00)
#endif

#if ! defined( TF_USE_SYSTEM_THREAD )
#define TF_USE_SYSTEM_THREAD (0x10)
#endif

#if ! defined( TF_USE_KERNEL_APC )
#define TF_USE_KERNEL_APC (0x20)
#endif

struct WFC_SOCKET_THUNK
{
    HINSTANCE dll_instance_handle{ nullptr };
    TCHAR     dll_name[MAX_PATH]{ 0 };
    SOCKET(PASCAL FAR* accept)(SOCKET, struct sockaddr FAR*, int FAR*) { nullptr };
   int                    (PASCAL FAR * bind)(SOCKET , const struct sockaddr FAR *, int ) { nullptr };
   int                    (PASCAL FAR * closesocket)(SOCKET ) { nullptr };
   int                    (PASCAL FAR * connect)(SOCKET , const struct sockaddr FAR *, int ) { nullptr };
   int                    (PASCAL FAR * ioctlsocket)( SOCKET, long , u_long FAR * ) { nullptr };
   int                    (PASCAL FAR * getpeername)( SOCKET, struct sockaddr FAR *, int FAR * ) { nullptr };
   int                    (PASCAL FAR * getsockname)( SOCKET, struct sockaddr FAR *, int FAR * ) { nullptr };
   int                    (PASCAL FAR * getsockopt)( SOCKET, int, int, char FAR *, int FAR * ) { nullptr };
   u_long                 (PASCAL FAR * htonl)( u_long ) { nullptr };
   u_short                (PASCAL FAR * htons)( u_short ) { nullptr };
   unsigned long          (PASCAL FAR * inet_addr)( const char FAR * ) { nullptr };
   char FAR *             (PASCAL FAR * inet_ntoa)( struct in_addr ) { nullptr };
   int                    (PASCAL FAR * listen)( SOCKET, int ) { nullptr };
   u_long                 (PASCAL FAR * ntohl)( u_long ) { nullptr };
   u_short                (PASCAL FAR * ntohs)( u_short ) { nullptr };
   int                    (PASCAL FAR * recv)( SOCKET, char FAR *, int, int ) { nullptr };
   int                    (PASCAL FAR * recvfrom)( SOCKET, char FAR *, int, int, struct sockaddr FAR *, int FAR * ) { nullptr };
   int                    (PASCAL FAR * select)( int , fd_set FAR *, fd_set FAR *, struct fd_set FAR *, const struct timeval FAR * ) { nullptr };
   int                    (PASCAL FAR * send)( SOCKET, const char FAR *, int, int ) { nullptr };
   int                    (PASCAL FAR * sendto)( SOCKET, const char FAR *, int, int, const struct sockaddr FAR *, int ) { nullptr };
   int                    (PASCAL FAR * setsockopt)( SOCKET, int, int, const char FAR *, int ) { nullptr };
   int                    (PASCAL FAR * shutdown)( SOCKET, int ) { nullptr };
   SOCKET                 (PASCAL FAR * socket)( int, int, int ) { nullptr };
   struct hostent FAR *   (PASCAL FAR * gethostbyaddr)( const char FAR *, int, int ) { nullptr };
   struct hostent FAR *   (PASCAL FAR * gethostbyname)( const char FAR * ) { nullptr };
   int                    (PASCAL FAR * gethostname)( char FAR *, int ) { nullptr };
   struct servent FAR *   (PASCAL FAR * getservbyport)( int port, const char FAR * ) { nullptr };
   struct servent FAR *   (PASCAL FAR * getservbyname)( const char FAR *, const char FAR * ) { nullptr };
   struct protoent FAR *  (PASCAL FAR * getprotobynumber)( int ) { nullptr };
   struct protoent FAR *  (PASCAL FAR * getprotobyname)( const char FAR * ) { nullptr };
   int                    (PASCAL FAR * WSAStartup)( WORD, LPWSADATA ) { nullptr };
   int                    (PASCAL FAR * WSACleanup)( void ) { nullptr };
   void                   (PASCAL FAR * WSASetLastError)( int ) { nullptr };
   int                    (PASCAL FAR * WSAGetLastError)( void ) { nullptr };
   BOOL                   (PASCAL FAR * WSAIsBlocking)( void ) { nullptr };
   int                    (PASCAL FAR * WSAUnhookBlockingHook)( void ) { nullptr };
   FARPROC                (PASCAL FAR * WSASetBlockingHook)( FARPROC ) { nullptr };
   int                    (PASCAL FAR * WSACancelBlockingCall)( void ) { nullptr };
   HANDLE                 (PASCAL FAR * WSAAsyncGetServByName)( HWND, u_int , const char FAR *, const char FAR *, char FAR *, int ) { nullptr };
   HANDLE                 (PASCAL FAR * WSAAsyncGetServByPort)( HWND, u_int , int port, const char FAR *, char FAR *, int ) { nullptr };
   HANDLE                 (PASCAL FAR * WSAAsyncGetProtoByName)( HWND, u_int, const char FAR *, char FAR *, int ) { nullptr };
   HANDLE                 (PASCAL FAR * WSAAsyncGetProtoByNumber)( HWND, u_int, int number, char FAR *, int ) { nullptr };
   HANDLE                 (PASCAL FAR * WSAAsyncGetHostByName)( HWND, u_int, const char FAR *, char FAR *, int ) { nullptr };
   HANDLE                 (PASCAL FAR * WSAAsyncGetHostByAddr)( HWND, u_int, const char FAR *, int, int, char FAR *, int ) { nullptr };
   int                    (PASCAL FAR * WSACancelAsyncRequest)( HANDLE ) { nullptr };
   int                    (PASCAL FAR * WSAAsyncSelect)(SOCKET, HWND, u_int, long ) { nullptr };
   int                    (PASCAL FAR * WSARecvEx)(SOCKET, char FAR *, int, int FAR * ) { nullptr };
   BOOL                   (PASCAL FAR * TransmitFile)( SOCKET, HANDLE, DWORD,  DWORD, LPOVERLAPPED, LPTRANSMIT_FILE_BUFFERS, DWORD ) { nullptr };
   BOOL                   (PASCAL FAR * AcceptEx)( SOCKET, SOCKET, PVOID, DWORD, DWORD, DWORD, LPDWORD, LPOVERLAPPED ) { nullptr };
   VOID                   (PASCAL FAR * GetAcceptExSockaddrs)( PVOID, DWORD, DWORD, DWORD, struct sockaddr **, LPINT, struct sockaddr **, LPINT ) { nullptr };
   int                    (PASCAL FAR * __WSAFDIsSet)(SOCKET, fd_set FAR * ) { nullptr };
};

class CSimpleSocket : public CDummyFile
{
   private:

      void m_Initialize( void ) noexcept;
      static void m_InitializeThunk( void ) noexcept;

   protected:

      std::wstring m_PortName;

      short m_PortNumberInNetworkByteOrder{ 0 };

      SOCKET m_SocketID{ 0 };

      TCHAR m_StringReadTerminatorCharacter{ 0 };

      DWORD m_Options{ 0 };

      static long m_ReferenceCount;

      static WFC_SOCKET_THUNK * m_Thunk_p;

   public:

      enum _Options
      {
         optionsDefault               = 0x00000000,
         optionsDontResolveHostName   = 0x00000001,
         optionsDontResolvePortNumber = 0x00000002
      };

      /*
      ** Construction
      */

      CSimpleSocket(CSimpleSocket const&) = delete;
      CSimpleSocket& operator=(CSimpleSocket const&) = delete;

      CSimpleSocket() noexcept;
      CSimpleSocket( __in SOCKET const client_id ) noexcept;

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CSimpleSocket() noexcept;

      std::wstring      Address;
      std::wstring      Name;
      std::vector<std::wstring> AliasList;

      static void __stdcall GetMyAddress( __out std::wstring& dotted_ip_address ) noexcept;
      static void __stdcall GetMyAddress( __out_bcount(4) uint8_t * as_bytes  ) noexcept;
      static void __stdcall GetMyName( __out std::wstring& machine_name ) noexcept;
      static void __stdcall StartWindowsSockets( void ) noexcept;
      static void __stdcall StopWindowsSockets( void ) noexcept;
      static void __stdcall TranslateErrorCode( __in DWORD const error_code, __out_z LPTSTR destination_string, __in DWORD const size_of_destination_string ) noexcept;

      /*
      ** Getting data from the object
      */

      virtual void   GetAddress( __out std::wstring& a ) const noexcept;
      virtual _Check_return_ SOCKET GetID( void ) const noexcept;
      virtual void   GetName( __out std::wstring& a ) const noexcept;
      virtual void   GetPort( __out short& p ) const noexcept;
      virtual void   GetPort( __out std::wstring& p ) const noexcept;
      virtual _Check_return_ TCHAR GetStringReadTerminatorCharacter( void ) const noexcept;

      /*
      ** Putting data into the object
      */

      virtual void SetAddress( _In_ std::wstring_view a ) noexcept; // Fills in Address, Name and AliasList
      virtual void SetID( __in SOCKET const id ) noexcept;
      virtual void SetName( __in_z LPCTSTR a ) noexcept;
      virtual void SetPort( __in_z LPCTSTR p ) noexcept;
      virtual void SetPort( __in short const p ) noexcept;
      virtual void SetStringReadTerminatorCharacter( __in TCHAR const terminator ) noexcept;

      /*
      ** Misc Functions
      */

      virtual _Check_return_ bool Open( void ) = 0;
      _Check_return_ bool Open(_In_ std::wstring_view channel_name, _In_ UINT const port_number = 23 ) noexcept override = 0;
      virtual void Close( void ) noexcept;
      virtual _Check_return_ DWORD GetOptions( void ) const noexcept;
      virtual _Check_return_ bool IsBroken( void ) const noexcept;
      virtual _Check_return_ bool IsDataWaiting( void ) noexcept;
      virtual void SetOptions( __in DWORD const new_options ) noexcept;
      virtual _Check_return_ bool SetSocketOption( __in int const level, __in int const option_name, __in_z char const * option_value, __in int const option_length ) noexcept;
      virtual _Check_return_ bool SetReadTimeout( __in DWORD const number_of_milliseconds ) noexcept;
      virtual _Check_return_ bool SetWriteTimeout(__in DWORD const number_of_milliseconds ) noexcept;

      // Not all versions of Visual C++ have TF_DISCONNECT defined

#if defined( TF_DISCONNECT )

      virtual _Check_return_ bool Transmit( __inout CFile64& file_to_send, __in DWORD const flags = TF_DISCONNECT, __in_opt std::vector<uint8_t> * send_before = nullptr, __in_opt std::vector<uint8_t> * send_after = nullptr ) noexcept;

#endif // TF_DISCONNECT

      void Write( __in std::string const& data_to_write ) noexcept override;
      void Write( __in_bcount( number_of_bytes_to_write ) VOID const *buffer, __in UINT const number_of_bytes_to_write ) noexcept override;

      virtual _Check_return_ UINT Read( __out_bcount(number_of_bytes_to_read) VOID * buffer, __in UINT const number_of_bytes_to_read ) noexcept;
      virtual _Check_return_ bool Read( __out std::string& line_to_read ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dc ) const;

#endif // _DEBUG
};

#endif // SOCKET_CLASS_HEADER
