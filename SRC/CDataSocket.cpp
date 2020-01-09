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
** $Workfile: CDataSocket.cpp $
** $Revision: 19 $
** $Modtime: 6/26/01 10:45a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

CDataSocket::CDataSocket() noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Socket_p = nullptr;
}

CDataSocket::CDataSocket( __inout CSimpleSocket * socket_p ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( socket_p );
   m_Socket_p = nullptr;
   Attach( socket_p );
}

CDataSocket::~CDataSocket() noexcept
{
   WFC_VALIDATE_POINTER( this );
}

void CDataSocket::Attach(__inout CSimpleSocket * socket_p ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( socket_p );
   m_Socket_p = socket_p;
}

_Check_return_ uint32_t CDataSocket::Read(__out_bcount(size_of_buffer) void * buffer, __in uint32_t const size_of_buffer ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Socket_p );

   if ( m_Socket_p == nullptr )
   {
      return( 0 );
   }

   if ( size_of_buffer == 0 )
   {
      return( 0 );
   }

   WFC_VALIDATE_POINTER( buffer );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      BYTE * temporary_buffer = (BYTE *) buffer;

      bool exit_loop = false;

      UINT return_value = 0;

      int number_of_bytes_read    = 0;
      int number_of_bytes_to_read = size_of_buffer;

      while( exit_loop == false )
      {
         number_of_bytes_read = m_Socket_p->Read( temporary_buffer, number_of_bytes_to_read );

         if ( number_of_bytes_read == SOCKET_ERROR or number_of_bytes_read == 0 )
         {
            //WFCTRACE( TEXT( "ERROR! Can't read socket\n" ) );
            exit_loop = true;
         }
         else
         {
            return_value            += number_of_bytes_read;
            temporary_buffer        += number_of_bytes_read;
            number_of_bytes_to_read -= number_of_bytes_read;

            if ( return_value >= size_of_buffer )
            {
               exit_loop = true;
            }
         }
      }

      //WFCTRACEVAL( TEXT( "Number of bytes read is " ), return_value );

      return( return_value );
   }
   WFC_CATCH_ALL
   {
      return( 0 );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ uint64_t CDataSocket::Seek( _In_ int64_t const, _In_ CFile64::SeekPosition const) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( 0 );
}

void CDataSocket::Write(__in_bcount(number_of_bytes_to_write) void const * buffer, __in uint32_t const number_of_bytes_to_write) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( number_of_bytes_to_write == 0 )
   {
      return;
   }

   if ( m_Socket_p == nullptr )
   {
      //WFCTRACE( TEXT( "You have to socket attached" ) );
      return;
   }

   WFC_VALIDATE_POINTER( m_Socket_p );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      m_Socket_p->Write( buffer, number_of_bytes_to_write );
   }
   WFC_CATCH_ALL
   {
      return;
   }
   WFC_END_CATCH_ALL
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CDataSocket</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles sending/receiving chunks of RIFF data through a TCP/IP socket. This is the basis for a generic protocol.">
</HEAD>

<BODY>

<H1>CDataSocket : <A HREF="CDataFile.htm">CDataFile</A></H1>
$Revision: 19 $<BR>
<HR>

<H2>Description</H2>

This class allows you handle the input and output of
<A HREF="CDataChunk.htm">CDataChunk</A> objects 
through a TCP/IP socket easily.

<H2>Data Members</H2>
None.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CDataSocket</B>()
<B>CDataSocket</B>( <A HREF="SOCKETS.htm">CSimpleSocket</A> * socket_p )</PRE><DD>
Constructs the object.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Attach">Attach</A></B>( <A HREF="SOCKETS.htm">CSimpleSocket</A> * socket_p )</PRE><DD>
Tells <B>CDataSocket</B> which socket to use.

<DT><PRE>UINT <B>Read</B>( void * buffer, UINT number_of_bytes_to_read )</PRE><DD>
Reads data from the socket.

<DT><PRE>long <B><A NAME="Seek">Seek</A></B>( long offset, UINT from )</PRE><DD>
Intercepts this call. It doesn't make a whole lot of sense to <B>Seek</B>()
around a socket.

<DT><PRE>void <B><A NAME="Write">Write</A></B>( const void * buffer, UINT number_of_bytes_to_read )</PRE><DD>
Reads data from the socket.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

void read_next_chunk( <B>CDataSocket</B>&amp; socket, <A HREF="CDataChunk.htm">CDataChunk</A>&amp; data )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;read_next_chunk()&quot; ) );

   socket.GetData( data );

   _tprintf( TEXT( &quot;Read %lu bytes\n&quot; ), data.Data.GetSize() );
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CDataSocket.cpp $<BR>
$Modtime: 6/26/01 10:45a $
</BODY>

</HTML>
*/
