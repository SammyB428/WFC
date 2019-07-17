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
** $Workfile: RASPPPCR.cpp $
** $Revision: 15 $
** $Modtime: 6/26/01 10:55a $
** $Reuse Tracing Code: 1 $
*/

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

CPointToPointProtocolProjectionResult::CPointToPointProtocolProjectionResult()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

CPointToPointProtocolProjectionResult::CPointToPointProtocolProjectionResult( CPointToPointProtocolProjectionResult const& source )
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

CPointToPointProtocolProjectionResult::CPointToPointProtocolProjectionResult( RAS_PPP_PROJECTION_RESULT const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Empty();
   Copy( source );
}

CPointToPointProtocolProjectionResult::~CPointToPointProtocolProjectionResult()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

void CPointToPointProtocolProjectionResult::Copy( CPointToPointProtocolProjectionResult const& source )
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      WFCTRACE( TEXT( "Attempt to make a copy of ourself (such silliness)." ) );
      return;
   }

   AppleTalk.Copy( source.AppleTalk );
   IP.Copy( source.IP );
   IPX.Copy( source.IPX );
   NetBEUI.Copy( source.NetBEUI );
}

void CPointToPointProtocolProjectionResult::Copy( __in RAS_PPP_PROJECTION_RESULT const * source )
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
      AppleTalk.Copy( &source->at );
      IP.Copy( &source->ip );
      IPX.Copy( &source->ipx );
      NetBEUI.Copy( &source->nbf );
   }
   WFC_CATCH_ALL
   {
      Empty();
      return;
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CPointToPointProtocolProjectionResult::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CPointToPointProtocolProjectionResult at " ) << (VOID *) this << TEXT( "\n{\n" );

   dump_context << TEXT( "   AppleTalk is " );
   AppleTalk.Dump( dump_context );

   dump_context << TEXT( "   IP is " );
   IP.Dump( dump_context );

   dump_context << TEXT( "   IPX is " );
   IPX.Dump( dump_context );

   dump_context << TEXT( "   NetBEUI is " );
   NetBEUI.Dump( dump_context );

   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void CPointToPointProtocolProjectionResult::Empty( void )
{
   WFC_VALIDATE_POINTER( this );
   AppleTalk.Empty();
   IP.Empty();
   IPX.Empty();
   NetBEUI.Empty();
}

CPointToPointProtocolProjectionResult& CPointToPointProtocolProjectionResult::operator = ( CPointToPointProtocolProjectionResult const& source )
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
<TITLE>WFC - CPointToPointProtocolProjectionResult</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, RAS, Remote Access Service, Dialup networking">
</HEAD>

<BODY>

<H1>CPointToPointProtocolProjectionResult : CObject</H1>
$Revision: 15 $
<HR>

<H2>Description</H2>
This class is based on the RAS_PPP_PROJECTION_RESULT structure.

<H2>Data Members</H2>

<B>None.</B>

<H2>Methods</H2>

<DL COMPACT>

<DT><B><A NAME="Copy">Copy</A></B><DD>Copies another CPointToPointProtocolProjectionResult or a RAS_PPP_PROJECTION_RESULT structure.
<DT><B><A NAME="Empty">Empty</A></B><DD>Clears all data members.
<DT><B><A NAME="Serialize">Serialize</A></B><DD>After all, we are a CObject...

</DL>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: RASPPPCR.cpp $<BR>
$Modtime: 6/26/01 10:55a $
</BODY>

</HTML>
*/
