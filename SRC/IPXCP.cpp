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
** $Workfile: IPXCP.cpp $
** $Revision: 16 $
** $Modtime: 6/26/01 10:53a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#endif // _DEBUG

#if ! defined( WFC_NO_RAS )

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

CInternetworkPacketExchangeProjectionResult::CInternetworkPacketExchangeProjectionResult()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

CInternetworkPacketExchangeProjectionResult::CInternetworkPacketExchangeProjectionResult( CInternetworkPacketExchangeProjectionResult const& source )
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

CInternetworkPacketExchangeProjectionResult::CInternetworkPacketExchangeProjectionResult( RAS_PPP_IPXCP_RESULT const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Empty();
   Copy( source );
}

CInternetworkPacketExchangeProjectionResult::CInternetworkPacketExchangeProjectionResult( CRemoteAccessServiceConnectionProjection const& source )
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

CInternetworkPacketExchangeProjectionResult::~CInternetworkPacketExchangeProjectionResult()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

void CInternetworkPacketExchangeProjectionResult::Copy( _In_ CInternetworkPacketExchangeProjectionResult const& source )
{
   WFC_VALIDATE_POINTER( this );
   ErrorCode = source.ErrorCode;
   Address   = source.Address;
}

void CInternetworkPacketExchangeProjectionResult::Copy( CRemoteAccessServiceConnectionProjection const& source )
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   CRemoteAccessServiceConnectionProjection::Copy( source );
}

void CInternetworkPacketExchangeProjectionResult::Copy( const RAS_PPP_IPXCP_RESULT * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   // Choose to live

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      ErrorCode = source->dwError;
      ::wfc_convert_lpcwstr_to_cstring( source->wszAddress, Address );
   }
   WFC_CATCH_ALL
   {
      Empty();
      return;
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CInternetworkPacketExchangeProjectionResult::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CInternetworkPacketExchangeProjectionResult at " ) << (VOID *) this << TEXT( "\n{\n" );
   CRemoteAccessServiceConnectionProjection::Dump( dump_context );
   dump_context << TEXT( "   Address is " ) << Address << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif

void CInternetworkPacketExchangeProjectionResult::Empty( void )
{
   WFC_VALIDATE_POINTER( this );
   CRemoteAccessServiceConnectionProjection::Empty();
   Address.Empty();
}

CInternetworkPacketExchangeProjectionResult& CInternetworkPacketExchangeProjectionResult::operator = ( CInternetworkPacketExchangeProjectionResult const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

#endif // WFC_NO_RAS

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CInternetworkPacketExchangeProjectionResult</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, RAS, Remote Access Service, Dialup networking, source code">
</HEAD>

<BODY>

<H1>CInternetworkPacketExchangeProjectionResult : CRemoteAccessServiceConnectionProjection</H1>
$Revision: 16 $
<HR>

<H2>Description</H2>

This class is based on the RAS_PPP_IPXCP_RESULT structure.

<H2>Data Members</H2>

<B>Address</B> - Contains the address.

<H2>Methods</H2>

<DL COMPACT>

<DT><DFN><B><A NAME="Copy">Copy</A></B></DFN><DD>Copies another CInternetworkPacketExchangeProjectionResult or a RAS_PPP_IPXCP_RESULT structure.

<DT><DFN><B><A NAME="Empty">Empty</A></B></DFN><DD>Clears all data members.

<DT><DFN><B><A NAME="Serialize">Serialize</A></B></DFN><DD>After all, we are a CObject...

</DL>

<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: IPXCP.cpp $<BR>
$Modtime: 6/26/01 10:53a $
</BODY>
</HTML>
*/
