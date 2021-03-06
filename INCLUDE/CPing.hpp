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
** $Workfile: cping.hpp $
** $Revision: 11 $
** $Modtime: 6/26/01 11:03a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( PING_CLASS_HEADER )

#define PING_CLASS_HEADER

#if ! defined( IP_EXPORT_INCLUDED )

#define IP_EXPORT_INCLUDED

/*NOINC*/

//
// IP types
//
using IPAddr = unsigned long;     // An IP address.
using IPMask = unsigned long;     // An IP subnet mask.
using IP_STATUS = unsigned long;  // Status code returned from IP APIs.

/*INC*/
#if 0
struct IP_OPTION_INFORMATION {
    unsigned char       Ttl{ 0 };             // Time To Live
    unsigned char       Tos{ 0 };             // Type Of Service
    unsigned char       Flags{ 0 };           // IP header flags
    unsigned char       OptionsSize{ 0 };     // Size in bytes of options data
    unsigned char FAR * OptionsData{ nullptr };     // Pointer to options data
}; /* ip_option_information */

struct ICMP_ECHO_REPLY {
    IPAddr                  Address{ 0 };         // Replying address
    unsigned long           Status{ 0 };          // Reply status
    unsigned long           RoundTripTime{ 0 };   // RTT in milliseconds
    unsigned short          DataSize{ 0 };        // Echo data size
    unsigned short          Reserved{ 0 };        // Reserved for system use
    void FAR              * Data{ nullptr };            // Pointer to the echo data
    IP_OPTION_INFORMATION   Options;         // Reply options
}; /* icmp_echo_reply */

/*NOINC*/

#endif // 0
/*INC*/

//
// IP status codes returned to transports and user IOCTLs.
//

#define IP_STATUS_BASE              11000

#define IP_SUCCESS                  0
#define IP_BUF_TOO_SMALL            (IP_STATUS_BASE + 1)
#define IP_DEST_NET_UNREACHABLE     (IP_STATUS_BASE + 2)
#define IP_DEST_HOST_UNREACHABLE    (IP_STATUS_BASE + 3)
#define IP_DEST_PROT_UNREACHABLE    (IP_STATUS_BASE + 4)
#define IP_DEST_PORT_UNREACHABLE    (IP_STATUS_BASE + 5)
#define IP_NO_RESOURCES             (IP_STATUS_BASE + 6)
#define IP_BAD_OPTION               (IP_STATUS_BASE + 7)
#define IP_HW_ERROR                 (IP_STATUS_BASE + 8)
#define IP_PACKET_TOO_BIG           (IP_STATUS_BASE + 9)
#define IP_REQ_TIMED_OUT            (IP_STATUS_BASE + 10)
#define IP_BAD_REQ                  (IP_STATUS_BASE + 11)
#define IP_BAD_ROUTE                (IP_STATUS_BASE + 12)
#define IP_TTL_EXPIRED_TRANSIT      (IP_STATUS_BASE + 13)
#define IP_TTL_EXPIRED_REASSEM      (IP_STATUS_BASE + 14)
#define IP_PARAM_PROBLEM            (IP_STATUS_BASE + 15)
#define IP_SOURCE_QUENCH            (IP_STATUS_BASE + 16)
#define IP_OPTION_TOO_BIG           (IP_STATUS_BASE + 17)
#define IP_BAD_DESTINATION          (IP_STATUS_BASE + 18)

//
// The next group are status codes passed up on status indications.
//

#define IP_ADDR_DELETED             (IP_STATUS_BASE + 19)
#define IP_SPEC_MTU_CHANGE          (IP_STATUS_BASE + 20)
#define IP_MTU_CHANGE               (IP_STATUS_BASE + 21)

#define IP_GENERAL_FAILURE          (IP_STATUS_BASE + 50)

#define MAX_IP_STATUS               IP_GENERAL_FAILURE

#define IP_PENDING                  (IP_STATUS_BASE + 255)

//
// IP header flags
//
#define IP_FLAG_DF      0x2         // Don't fragment this packet.

//
// IP Option Types
//
#define IP_OPT_EOL      0                       // End of list option
#define IP_OPT_NOP      1                       // No operation
#define IP_OPT_SECURITY 0x82                    // Security option.
#define IP_OPT_LSRR     0x83                    // Loose source route.
#define IP_OPT_SSRR     0x89                    // Strict source route.
#define IP_OPT_RR       0x7                     // Record route.
#define IP_OPT_TS       0x44                    // Timestamp.
#define IP_OPT_SID      0x88                    // Stream ID (obsolete)
#define MAX_OPT_SIZE    40

#endif // IP_EXPORT_INCLUDED

// === Structures Required by the ICMP.DLL ====================================

struct IP_OPTION_INFORMATION {
    unsigned char Ttl{ 0 };                                           // Time To Live
   unsigned char Tos{ 0 };                                        // Type Of Service
   unsigned char Flags{ 0 };                                      // IP header flags
   unsigned char OptionsSize{ 0 };                  // Size in bytes of options data
   unsigned char *OptionsData{ nullptr };                       // Pointer to options data
};

struct IP_ECHO_REPLY {
    DWORD Address{ 0 };                                           // Replying address
    unsigned long  Status{ 0 };                                       // Reply status
    unsigned long  RoundTripTime{ 0 };                         // RTT in milliseconds
    unsigned short DataSize{ 0 };                                   // Echo data size
    unsigned short Reserved{ 0 };                          // Reserved for system use
    void* Data{ nullptr };                                      // Pointer to the echo data
    IP_OPTION_INFORMATION Options;                              // Reply options
};

#if ! defined( _ICMP_INCLUDED_ )

#define _ICMP_INCLUDED_

//
// Exported Routines.
//

//++
//
// Routine Description:
//
//     Opens a handle on which ICMP Echo Requests can be issued.
//
// Arguments:
//
//     None.
//
// Return Value:
//
//     An open file handle or INVALID_HANDLE_VALUE. Extended error information
//     is available by calling GetLastError().
//
//--

HANDLE
WINAPI
IcmpCreateFile(
    VOID
    );


//++
//
// Routine Description:
//
//     Closes a handle opened by ICMPOpenFile.
//
// Arguments:
//
//     IcmpHandle  - The handle to close.
//
// Return Value:
//
//     TRUE if the handle was closed successfully, otherwise FALSE. Extended
//     error information is available by calling GetLastError().
//
//--

BOOL
WINAPI
IcmpCloseHandle(
    HANDLE  IcmpHandle
    );

//++
//
// Routine Description:
//
//     Sends an ICMP Echo request and returns one or more replies. The
//     call returns when the timeout has expired or the reply buffer
//     is filled.
//
// Arguments:
//
//     IcmpHandle           - An open handle returned by ICMPCreateFile.
//
//     DestinationAddress   - The destination of the echo request.
//
//     RequestData          - A buffer containing the data to send in the
//                            request.
//
//     RequestSize          - The number of bytes in the request data buffer.
//
//     RequestOptions       - Pointer to the IP header options for the request.
//                            May be NULL.
//
//     ReplyBuffer          - A buffer to hold any replies to the request.
//                            On return, the buffer will contain an array of
//                            ICMP_ECHO_REPLY structures followed by options
//                            and data. The buffer should be large enough to
//                            hold at least one ICMP_ECHO_REPLY structure
//                            and 8 bytes of data - this is the size of
//                            an ICMP error message.
//
//     ReplySize            - The size in bytes of the reply buffer.
//
//     Timeout              - The time in milliseconds to wait for replies.
//
// Return Value:
//
//     Returns the number of replies received and stored in ReplyBuffer. If
//     the return value is zero, extended error information is available
//     via GetLastError().
//
//--

DWORD
WINAPI
IcmpSendEcho(
    HANDLE                   IcmpHandle,
    IPAddr                   DestinationAddress,
    LPVOID                   RequestData,
    WORD                     RequestSize,
    IP_OPTION_INFORMATION *  RequestOptions,
    LPVOID                   ReplyBuffer,
    DWORD                    ReplySize,
    DWORD                    Timeout
    );


#endif // _ICMP_INCLUDED_

using ICMP_OPEN_FUNCTION = HANDLE (WINAPI*)(VOID);
using ICMP_CLOSE_FUNCTION = VOID (WINAPI *)( HANDLE );
using ICMP_SEND_ECHO_FUNCTION = DWORD (WINAPI *)( HANDLE, IPAddr, LPVOID, WORD, IP_OPTION_INFORMATION *, LPVOID, DWORD, DWORD );

class CPingResults
{
   public:

      CPingResults() noexcept;
      explicit CPingResults( _In_ CPingResults const & source ) noexcept;
      explicit CPingResults( _In_ CPingResults const * source ) noexcept;
      explicit CPingResults( _In_ IP_ECHO_REPLY const & source ) noexcept;
      explicit CPingResults( _In_ IP_ECHO_REPLY const * source ) noexcept;

      std::string Address;
      DWORD       RoundTripTimeInMilliseconds{ 0 };
      uint8_t     TimeToLive{ 0 };

      void Copy( _In_ CPingResults const&   source ) noexcept;
      void Copy( _In_ CPingResults const *  source ) noexcept;
      void Copy( _In_ IP_ECHO_REPLY const&  source ) noexcept;
      void Copy( _In_ IP_ECHO_REPLY const * source ) noexcept;
      void Empty( void ) noexcept;

      CPingResults& operator = ( _In_ CPingResults const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

class CPing : public CSimpleSocket
{
   protected:

       HMODULE m_ICMP_DLL_Instance{ nullptr };

       uint8_t* m_Text{ nullptr };
       std::size_t m_NumberOfTextBytes{ 0 };

      HANDLE (WINAPI *m_Open)    ( VOID );
      VOID   (WINAPI *m_Close)   ( HANDLE );
      DWORD  (WINAPI *m_SendEcho)( HANDLE, IPAddr, LPVOID, WORD, IP_OPTION_INFORMATION *, LPVOID, DWORD, DWORD );

   public:

       CPing(CPing const&) = delete;
       CPing& operator=(CPing const&) = delete;
       
       CPing();
      virtual ~CPing();

      virtual void ConvertErrorToString( _In_ DWORD const error_code, _Out_ std::wstring& meaning ) const noexcept;
      _Check_return_ bool Open( void ) noexcept override;
      _Check_return_ bool Open(_In_ std::filesystem::path const& channel_name, _In_ UINT const port_number = 23 ) noexcept override;

      virtual _Check_return_ DWORD Ping( _In_ std::wstring const& name_or_address, __out_opt CPingResults * results_p = nullptr, _In_ short desired_time_to_live = 255 ) noexcept;
      virtual void SetText( __in_bcount(number_of_bytes ) uint8_t const * bytes, _In_ std::size_t const number_of_bytes ) noexcept;
};

#endif // PING_CLASS_HEADER
