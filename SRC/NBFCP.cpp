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
** $Workfile: NBFCP.cpp $
** $Revision: 16 $
** $Modtime: 6/26/01 10:54a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

#if ! defined( WFC_NO_RAS )

USING_WFC_NAMESPACE

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

CNetBEUIFramerProjectionResult::CNetBEUIFramerProjectionResult()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

CNetBEUIFramerProjectionResult::CNetBEUIFramerProjectionResult( CNetBEUIFramerProjectionResult const& source )
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

CNetBEUIFramerProjectionResult::CNetBEUIFramerProjectionResult( RAS_PPP_NBFCP_RESULT const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Empty();
   Copy( source );
}

CNetBEUIFramerProjectionResult::CNetBEUIFramerProjectionResult( const CRemoteAccessServiceConnectionProjection& source )
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

CNetBEUIFramerProjectionResult::~CNetBEUIFramerProjectionResult()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

void CNetBEUIFramerProjectionResult::Copy( CNetBEUIFramerProjectionResult const& source )
{
   WFC_VALIDATE_POINTER( this );
   ErrorCode       = source.ErrorCode;
   NetBiosError    = source.NetBiosError;
   Name            = source.Name;
   WorkstationName = source.WorkstationName;
}

void CNetBEUIFramerProjectionResult::Copy( CRemoteAccessServiceConnectionProjection const& source )
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   CRemoteAccessServiceConnectionProjection::Copy( source );
}

void CNetBEUIFramerProjectionResult::Copy( RAS_PPP_NBFCP_RESULT const * source )
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
      ErrorCode    = source->dwError;
      NetBiosError = source->dwNetBiosError;
      Name         = source->szName;
      ::wfc_convert_lpcwstr_to_cstring( source->wszWksta, WorkstationName );
   }
   WFC_CATCH_ALL
   {
      Empty();
      return;
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CNetBEUIFramerProjectionResult::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CNetBEUIFramerProjectionResult at " ) << (VOID *) this << TEXT( "\n{\n" );
   CRemoteAccessServiceConnectionProjection::Dump( dump_context );
   dump_context << TEXT( "   NetBiosError is    " ) << NetBiosError    << TEXT( "\n" );
   dump_context << TEXT( "   Name is            " ) << Name            << TEXT( "\n" );
   dump_context << TEXT( "   WorkstationName is " ) << WorkstationName << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void CNetBEUIFramerProjectionResult::Empty( void )
{
   WFC_VALIDATE_POINTER( this );
   CRemoteAccessServiceConnectionProjection::Empty();
   NetBiosError = 0;
   Name.Empty();
   WorkstationName.Empty();
}

CNetBEUIFramerProjectionResult& CNetBEUIFramerProjectionResult::operator = ( CNetBEUIFramerProjectionResult const& source )
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
<TITLE>WFC - CNetBEUIFramerProjectionResult</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, RAS, Remote Access Service, Dialup networking, source code">
</HEAD>

<BODY>

<H1>CNetBEUIFramerProjectionResult : CRemoteAccessServiceConnectionProjection</H1>
$Revision: 16 $
<HR>

<H2>Description</H2>

This class is based on the RAS_PPP_NBFCP_RESULT structure.

<H2>Data Members</H2>

<B>None.</B>

<H2>Methods</H2>

<DL COMPACT>

<DT><DFN><B><A NAME="Copy">Copy</A></B></DFN><DD>Copies another CNetBEUIFramerProjectionResult or a RAS_PPP_NBFCP_RESULT structure.

<DT><DFN><B><A NAME="Empty">Empty</A></B></DFN><DD>Clears all data members.

<DT><DFN><B><A NAME="Serialize">Serialize</A></B></DFN><DD>After all, we are a CObject...

</DL>

<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: NBFCP.cpp $<BR>
$Modtime: 6/26/01 10:54a $
</BODY>
</HTML>
*/
