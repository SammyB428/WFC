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
** $Workfile: cping.cpp $
** $Revision: 33 $
** $Modtime: 6/26/01 10:50a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CPing::CPing()
{
    WFC_VALIDATE_POINTER(this);

    m_Text = nullptr;
    m_NumberOfTextBytes = 0;
    m_ICMP_DLL_Instance = ::LoadLibrary(TEXT("ICMP.DLL"));

    if (m_ICMP_DLL_Instance not_eq static_cast<HMODULE>(NULL))
    {
#pragma warning(disable:4191)
        m_Open = (ICMP_OPEN_FUNCTION)      ::GetProcAddress(m_ICMP_DLL_Instance, "IcmpCreateFile"); // Cannot be UNICODE string
        m_Close = (ICMP_CLOSE_FUNCTION)     ::GetProcAddress(m_ICMP_DLL_Instance, "IcmpCloseHandle"); // Cannot be UNICODE string
        m_SendEcho = (ICMP_SEND_ECHO_FUNCTION) ::GetProcAddress(m_ICMP_DLL_Instance, "IcmpSendEcho"); // Cannot be UNICODE string
#pragma warning(default:4191)
    }
    else
    {
        //WFCTRACEERROR( ::GetLastError() );
        m_Open = nullptr;
        m_Close = nullptr;
        m_SendEcho = nullptr;
    }
}

Win32FoundationClasses::CPing::~CPing()
{
    if (m_ICMP_DLL_Instance not_eq static_cast<HMODULE>(NULL))
    {
        ::FreeLibrary(m_ICMP_DLL_Instance);
    }

    m_Open = nullptr;
    m_Close = nullptr;
    m_SendEcho = nullptr;

    if (m_Text not_eq nullptr)
    {
        free(m_Text);
    }

    m_Text = nullptr;
    m_NumberOfTextBytes = 0;
}

void Win32FoundationClasses::CPing::ConvertErrorToString(_In_ DWORD const error_code, _Out_ std::wstring& meaning) const noexcept
{
    WFC_VALIDATE_POINTER(this);

    switch (error_code)
    {
    case IP_SUCCESS:

        meaning.assign(WSTRING_VIEW(L"IP_SUCCESS: Success"));
        break;

    case IP_BUF_TOO_SMALL:

        meaning.assign(WSTRING_VIEW(L"IP_BUFF_TOO_SMALL: Buffer too small"));
        break;

    case IP_DEST_NET_UNREACHABLE:

        meaning.assign(WSTRING_VIEW(L"IP_DEST_NET_UNREACHABLE: Network unreachable"));
        break;

    case IP_DEST_HOST_UNREACHABLE:

        meaning.assign(WSTRING_VIEW(L"IP_DEST_HOST_UNREACHABLE: Destination Host Unreachable"));
        break;

    case IP_DEST_PROT_UNREACHABLE:

        meaning.assign(WSTRING_VIEW(L"IP_DEST_PROT_UNREACHABLE"));
        break;

    case IP_DEST_PORT_UNREACHABLE:

        meaning.assign(WSTRING_VIEW(L"IP_DEST_PORT_UNREACHABLE"));
        break;

    case IP_NO_RESOURCES:

        meaning.assign(WSTRING_VIEW(L"IP_NO_RESOURCES"));
        break;

    case IP_BAD_OPTION:

        meaning.assign(WSTRING_VIEW(L"IP_BAD_OPTION"));
        break;

    case IP_HW_ERROR:

        meaning.assign(WSTRING_VIEW(L"IP_HW_ERROR: Hardware Error"));
        break;

    case IP_PACKET_TOO_BIG:

        meaning.assign(WSTRING_VIEW(L"IP_PACKET_TOO_BIG: Packet too big"));
        break;

    case IP_REQ_TIMED_OUT:

        meaning.assign(WSTRING_VIEW(L"IP_REQ_TIMED_OUT: Request timed out"));
        break;

    case IP_BAD_REQ:

        meaning.assign(WSTRING_VIEW(L"IP_BAD_REQ"));
        break;

    case IP_BAD_ROUTE:

        meaning.assign(WSTRING_VIEW(L"IP_BAD_ROUTE"));
        break;

    case IP_TTL_EXPIRED_TRANSIT:

        meaning.assign(WSTRING_VIEW(L"IP_TTL_EXPIRED_TRANSIT"));
        break;

    case IP_TTL_EXPIRED_REASSEM:

        meaning.assign(WSTRING_VIEW(L"IP_TTL_EXPIRED_REASSEM"));
        break;

    case IP_PARAM_PROBLEM:

        meaning.assign(WSTRING_VIEW(L"IP_PARAM_PROBLEM"));
        break;

    case IP_SOURCE_QUENCH:

        meaning.assign(WSTRING_VIEW(L"IP_SOURCE_QUENCH"));
        break;

    case IP_OPTION_TOO_BIG:

        meaning.assign(WSTRING_VIEW(L"IP_OPTION_TOO_BIG"));
        break;

    case IP_BAD_DESTINATION:

        meaning.assign(WSTRING_VIEW(L"IP_BAD_DESTINATION"));
        break;

    case IP_ADDR_DELETED:

        meaning.assign(WSTRING_VIEW(L"IP_ADDR_DELETED"));
        break;

    case IP_SPEC_MTU_CHANGE:

        meaning.assign(WSTRING_VIEW(L"IP_SPEC_MTU_CHANGE"));
        break;

    case IP_MTU_CHANGE:

        meaning.assign(WSTRING_VIEW(L"IP_MTU_CHANGE"));
        break;

    case IP_GENERAL_FAILURE:

        meaning.assign(WSTRING_VIEW(L"IP_GENERAL_FAILURE"));
        break;

    case IP_PENDING:

        meaning.assign(WSTRING_VIEW(L"IP_PENDING"));
        break;

    default:

        meaning.assign(WSTRING_VIEW(L"Unknown error "));
        meaning.append(std::to_wstring(error_code));
        break;
    }
}

_Check_return_ DWORD Win32FoundationClasses::CPing::Ping( _In_ std::wstring const& name_or_address, __out_opt Win32FoundationClasses::CPingResults * results_p, _In_ short desired_time_to_live) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (results_p not_eq nullptr)
    {
        results_p->Empty();
    }

    if (m_Open == nullptr)
    {
        m_LastError = ERROR_INVALID_ADDRESS;
        return(0);
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto icmp_handle { m_Open() };

       if (icmp_handle == static_cast<HANDLE>(INVALID_HANDLE_VALUE))
       {
          m_LastError = ERROR_INVALID_HANDLE;
          return(0);
       }

       DWORD status{ 0 };

       if (m_Text == nullptr)
       {
          BYTE buffer[60];

          ::ZeroMemory(buffer, sizeof(buffer));

          // OK, I'll get a little vain, Yes, this is ASCII strings and not UNICODE dependent

          ::strcpy_s((char *)buffer, std::size(buffer), "Samuel R. Blackburn Samuel R. Blackburn Samuel R. Blackburn");

          SetText(buffer, 59);
       }

       auto echo_reply_p{ static_cast<Win32FoundationClasses::IP_ECHO_REPLY*>(GlobalAllocPtr(GHND, sizeof(IP_ECHO_REPLY) + m_NumberOfTextBytes)) };

       if (echo_reply_p not_eq nullptr)
       {
          SetAddress(name_or_address);

          echo_reply_p->Options.Ttl = (unsigned char)desired_time_to_live;
          echo_reply_p->Data = (VOID *)Name.c_str();
          echo_reply_p->DataSize = (unsigned short)m_NumberOfTextBytes;

          if (m_SendEcho == nullptr)
          {
             m_LastError = ERROR_INVALID_ADDRESS;
             GlobalFreePtr(echo_reply_p);
             return(0);
          }

          // Thanks go to Curits Yanko (cyanko@ziplink.net) for finding
          // a boo boo here. I was linking inet_addr() and not thunking it.

          ULONG internet_address{ 0 };

          // DANGER! DANGER! DANGER! inet_addr() is not UNICODE correct!!!

 #if defined( _UNICODE )

          {
             char address[20];

             ZeroMemory(address, sizeof(address));

             copy_to( Address, address, sizeof(address));

             internet_address = m_Thunk_p->inet_addr(reinterpret_cast<char const *>(address));
          }

 #else

          internet_address = m_Thunk_p->inet_addr(reinterpret_cast<char const *>(Address));

 #endif // _UNICODE

          if (internet_address == INADDR_NONE)
          {
              //WFCTRACEERROR( GetLastError() );
           }

          // Now that we're done with the thunker...

          status = m_SendEcho(icmp_handle,
                               internet_address,
                               m_Text,
                        (WORD)m_NumberOfTextBytes,
                               nullptr,
                               echo_reply_p,
                               sizeof(IP_ECHO_REPLY) + (WORD)m_NumberOfTextBytes,
                               5000);

          if (status not_eq 0)
          {
             if (results_p not_eq nullptr)
             {
                results_p->Copy(echo_reply_p);
             }
          }
          else
          {
             m_LastError = ::GetLastError();
             //WFCTRACEERROR( m_LastError );
          }

          if (echo_reply_p not_eq nullptr)
          {
             GlobalFreePtr(echo_reply_p);
             echo_reply_p = nullptr;
          }
       }

       m_Close(icmp_handle);

       return(status);
    }
        WFC_CATCH_ALL
    {
       m_LastError = ERROR_EXCEPTION_IN_SERVICE;
       return(0);
    }
        WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CPing::Open(void) noexcept
{
    WFC_VALIDATE_POINTER(this);
    return(true);
}

_Check_return_ bool Win32FoundationClasses::CPing::Open(_In_ std::filesystem::path const&, _In_ UINT const) noexcept
{
    WFC_VALIDATE_POINTER(this);
    return(true);
}

void Win32FoundationClasses::CPing::SetText(__in_bcount(number_of_bytes) uint8_t const * bytes, _In_ std::size_t const number_of_bytes) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_Text not_eq nullptr)
    {
        WFC_VALIDATE_POINTER(m_Text);
        free(m_Text);
        m_Text = nullptr;
        m_NumberOfTextBytes = 0;
    }

    if (bytes not_eq nullptr and number_of_bytes > 0)
    {
        m_Text = static_cast<uint8_t *>(malloc(number_of_bytes));

        if (m_Text not_eq nullptr)
        {
            std::ignore = memcpy_s(m_Text, number_of_bytes, bytes, number_of_bytes);
            m_NumberOfTextBytes = number_of_bytes;
        }
    }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CPing</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that lets you ping TCP/IP hosts.">
</HEAD>

<BODY>

<H1>CPing : <A HREF="SOCKETS.htm">CSimpleSocket</A></H1>
$Revision: 33 $
<HR>

<H2>Description</H2>
This class makes it easy for you to ping another TCP/IP host.

<H2>Data Members</H2><B>None.</B><P>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="ConvertErrorToString">ConvertErrorToString</A></B>( DWORD error_code, std::wstring&amp; meaning ) const</PRE><DD>
If <B>Ping</B>() should fail, this will translate the error code into
something humanly readable.

<DT><PRE>DWORD <B><A NAME="Ping">Ping</A></B>( const std::wstring&amp; name_or_address, <A HREF="CPingRes.htm">CPingResults</A> * results_p, short desired_time_to_live )</PRE><DD>
Sends the ping request and received the results. Results are put into a
<A HREF="CPingRes.htm">CPingResults</A> object. It returns the number of
replies received.

</DL>

<H2>Example</H2><PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

int _tmain( int number_of_command_line_arguments, TCHAR *command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   <A HREF="SOCKETS.htm">CSimpleSocket</A>::StartWindowsSockets();

   <B>CPing</B> ping;

   DWORD return_value { 0 };

   <A HREF="CPingRes.htm">CPingResults</A> results;

   return_value = ping.Ping( TEXT( &quot;20.2.1.3&quot; ), &results );

   if ( return_value == 0 )
   {
      DWORD error_code { ping.GetErrorCode() };

      std::wstring error_message;

      ping.ConvertErrorToString( error_code, error_message );

      _tprintf( TEXT( &quot;Ping Error: %s\n&quot; ) ), error_message.c_str() );
   }
   else
   {
      _tprintf( TEXT( &quot;Ping: %s Round Trip %lums TTL: %d\n&quot; ),
              (LPCTSTR) results.Address,
              results.RoundTripTimeInMilliseconds,
              (int) results.TimeToLive );
   }

   <A HREF="SOCKETS.htm">CSimpleSocket</A>::StopWindowsSockets();

   return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>

<B>CPing</B> uses the following API's:

<UL>
<LI>FreeLibrary
<LI>GlobalAllocPtr
<LI>GlobalFreePtr
<LI>IcmpCloseHandle
<LI>IcmpCreateFile
<LI>IcmpSendEcho
<LI>inet_addr
<LI>LoadLibrary
<LI>ZeroMemory
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: cping.cpp $<BR>
$Modtime: 6/26/01 10:50a $
</BODY>

</HTML>
*/
