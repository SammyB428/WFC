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
** $Workfile: RASPSTAT.CPP $
** $Revision: 16 $
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

CRemoteAccessServicePortStatistics::CRemoteAccessServicePortStatistics()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

CRemoteAccessServicePortStatistics::CRemoteAccessServicePortStatistics( CRemoteAccessServicePortStatistics const& source )
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

CRemoteAccessServicePortStatistics::CRemoteAccessServicePortStatistics( RAS_PORT_STATISTICS const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Empty();
   Copy( source );
}

CRemoteAccessServicePortStatistics::~CRemoteAccessServicePortStatistics()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

_Check_return_ int CRemoteAccessServicePortStatistics::Compare( __in CRemoteAccessServicePortStatistics const& right_port ) const
{
   WFC_VALIDATE_POINTER( this );

   if ( NumberOfBytesTransmitted < right_port.NumberOfBytesTransmitted )
   {
      return( I_AM_LESS_THAN_THAT );
   }
   else if ( NumberOfBytesTransmitted > right_port.NumberOfBytesTransmitted )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   if ( NumberOfBytesReceived < right_port.NumberOfBytesReceived )
   {
      return( I_AM_LESS_THAN_THAT );
   }
   else if ( NumberOfBytesReceived > right_port.NumberOfBytesReceived )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   if ( NumberOfFramesTransmitted < right_port.NumberOfFramesTransmitted )
   {
      return( I_AM_LESS_THAN_THAT );
   }
   else if ( NumberOfFramesTransmitted > right_port.NumberOfFramesTransmitted )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   if ( NumberOfFramesReceived < right_port.NumberOfFramesReceived )
   {
      return( I_AM_LESS_THAN_THAT );
   }
   else if ( NumberOfFramesReceived > right_port.NumberOfFramesReceived )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   if ( NumberOfCRCErrors < right_port.NumberOfCRCErrors )
   {
      return( I_AM_LESS_THAN_THAT );
   }
   else if ( NumberOfCRCErrors > right_port.NumberOfCRCErrors )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   if ( NumberOfTimeoutErrors < right_port.NumberOfTimeoutErrors )
   {
      return( I_AM_LESS_THAN_THAT );
   }
   else if ( NumberOfTimeoutErrors > right_port.NumberOfTimeoutErrors )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   if ( NumberOfAlignmentErrors < right_port.NumberOfAlignmentErrors )
   {
      return( I_AM_LESS_THAN_THAT );
   }
   else if ( NumberOfAlignmentErrors > right_port.NumberOfAlignmentErrors )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   if ( NumberOfHardwareOverrunErrors < right_port.NumberOfHardwareOverrunErrors )
   {
      return( I_AM_LESS_THAN_THAT );
   }
   else if ( NumberOfHardwareOverrunErrors > right_port.NumberOfHardwareOverrunErrors )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   if ( NumberOfFramingErrors < right_port.NumberOfFramingErrors )
   {
      return( I_AM_LESS_THAN_THAT );
   }
   else if ( NumberOfFramingErrors > right_port.NumberOfFramingErrors )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   if ( NumberOfBufferOverrunErrors < right_port.NumberOfBufferOverrunErrors )
   {
      return( I_AM_LESS_THAN_THAT );
   }
   else if ( NumberOfBufferOverrunErrors > right_port.NumberOfBufferOverrunErrors )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   if ( NumberOfBytesTransmittedUncompressed < right_port.NumberOfBytesTransmittedUncompressed )
   {
      return(I_AM_LESS_THAN_THAT);
   }
   else if ( NumberOfBytesTransmittedUncompressed > right_port.NumberOfBytesTransmittedUncompressed )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   if ( NumberOfBytesReceivedUncompressed < right_port.NumberOfBytesReceivedUncompressed )
   {
      return(I_AM_LESS_THAN_THAT);
   }
   else if ( NumberOfBytesReceivedUncompressed > right_port.NumberOfBytesReceivedUncompressed )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   if ( NumberOfBytesTransmittedCompressed < right_port.NumberOfBytesTransmittedCompressed )
   {
      return(I_AM_LESS_THAN_THAT);
   }
   else if ( NumberOfBytesTransmittedCompressed > right_port.NumberOfBytesTransmittedCompressed )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   if ( NumberOfBytesReceivedCompressed < right_port.NumberOfBytesReceivedCompressed )
   {
      return(I_AM_LESS_THAN_THAT);
   }
   else if ( NumberOfBytesReceivedCompressed > right_port.NumberOfBytesReceivedCompressed )
   {
      return( I_AM_GREATER_THAN_THAT );
   }

   return( I_AM_EQUAL_TO_THAT );
}

void CRemoteAccessServicePortStatistics::Copy( const RAS_PORT_STATISTICS * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   // Choose to live

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      NumberOfBytesTransmitted             = source->dwBytesXmited;
      NumberOfBytesReceived                = source->dwBytesRcved;
      NumberOfFramesTransmitted            = source->dwFramesXmited;
      NumberOfFramesReceived               = source->dwFramesRcved;
      NumberOfCRCErrors                    = source->dwCrcErr;
      NumberOfTimeoutErrors                = source->dwTimeoutErr;
      NumberOfAlignmentErrors              = source->dwAlignmentErr;
      NumberOfHardwareOverrunErrors        = source->dwHardwareOverrunErr;
      NumberOfFramingErrors                = source->dwFramingErr;
      NumberOfBufferOverrunErrors          = source->dwBufferOverrunErr;
      NumberOfBytesTransmittedUncompressed = source->dwBytesXmitedUncompressed;
      NumberOfBytesReceivedUncompressed    = source->dwBytesRcvedUncompressed;
      NumberOfBytesTransmittedCompressed   = source->dwBytesXmitedCompressed;
      NumberOfBytesReceivedCompressed      = source->dwBytesRcvedCompressed;
   }
   WFC_CATCH_ALL
   {
      Empty();
      return;
   }
   WFC_END_CATCH_ALL
}

void CRemoteAccessServicePortStatistics::Copy( CRemoteAccessServicePortStatistics const& source )
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   NumberOfBytesTransmitted             = source.NumberOfBytesTransmitted;
   NumberOfBytesReceived                = source.NumberOfBytesReceived;
   NumberOfFramesTransmitted            = source.NumberOfFramesTransmitted;
   NumberOfFramesReceived               = source.NumberOfFramesReceived;
   NumberOfCRCErrors                    = source.NumberOfCRCErrors;
   NumberOfTimeoutErrors                = source.NumberOfTimeoutErrors;
   NumberOfAlignmentErrors              = source.NumberOfAlignmentErrors;
   NumberOfHardwareOverrunErrors        = source.NumberOfHardwareOverrunErrors;
   NumberOfFramingErrors                = source.NumberOfFramingErrors;
   NumberOfBufferOverrunErrors          = source.NumberOfBufferOverrunErrors;
   NumberOfBytesTransmittedUncompressed = source.NumberOfBytesTransmittedUncompressed;
   NumberOfBytesReceivedUncompressed    = source.NumberOfBytesReceivedUncompressed;
   NumberOfBytesTransmittedCompressed   = source.NumberOfBytesTransmittedCompressed;
   NumberOfBytesReceivedCompressed      = source.NumberOfBytesReceivedCompressed;
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CRemoteAccessServicePortStatistics::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CRemoteAccessServicePortStatistics at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   NumberOfBytesTransmitted is             " ) << NumberOfBytesTransmitted             << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfBytesReceived is                " ) << NumberOfBytesReceived                << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfFramesTransmitted is            " ) << NumberOfFramesTransmitted            << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfFramesReceived is               " ) << NumberOfFramesReceived               << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfCRCErrors is                    " ) << NumberOfCRCErrors                    << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfTimeoutErrors is                " ) << NumberOfTimeoutErrors                << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfAlignmentErrors is              " ) << NumberOfAlignmentErrors              << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfHardwareOverrunErrors is        " ) << NumberOfHardwareOverrunErrors        << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfFramingErrors is                " ) << NumberOfFramingErrors                << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfBufferOverrunErrors is          " ) << NumberOfBufferOverrunErrors          << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfBytesTransmittedUncompressed is " ) << NumberOfBytesTransmittedUncompressed << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfBytesReceivedUncompressed is    " ) << NumberOfBytesReceivedUncompressed    << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfBytesTransmittedCompressed is   " ) << NumberOfBytesTransmittedCompressed   << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfBytesReceivedCompressed is      " ) << NumberOfBytesReceivedCompressed      << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void CRemoteAccessServicePortStatistics::Empty( void )
{
   WFC_VALIDATE_POINTER( this );

   NumberOfBytesTransmitted             = 0;
   NumberOfBytesReceived                = 0;
   NumberOfFramesTransmitted            = 0;
   NumberOfFramesReceived               = 0;
   NumberOfCRCErrors                    = 0;
   NumberOfTimeoutErrors                = 0;
   NumberOfAlignmentErrors              = 0;
   NumberOfHardwareOverrunErrors        = 0;
   NumberOfFramingErrors                = 0;
   NumberOfBufferOverrunErrors          = 0;
   NumberOfBytesTransmittedUncompressed = 0;
   NumberOfBytesReceivedUncompressed    = 0;
   NumberOfBytesTransmittedCompressed   = 0;
   NumberOfBytesReceivedCompressed      = 0;
}

CRemoteAccessServicePortStatistics& CRemoteAccessServicePortStatistics::operator = ( CRemoteAccessServicePortStatistics const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

_Check_return_ bool CRemoteAccessServicePortStatistics::operator==( __in CRemoteAccessServicePortStatistics const& right_side ) const
{
   WFC_VALIDATE_POINTER( this );

   return( Compare( right_side ) == I_AM_EQUAL_TO_THAT ? true : false );
}

_Check_return_ bool CRemoteAccessServicePortStatistics::operator<( CRemoteAccessServicePortStatistics const& right_side ) const
{
   WFC_VALIDATE_POINTER( this );

   return(Compare(right_side) == I_AM_LESS_THAN_THAT ? true : false);
   }

_Check_return_ bool CRemoteAccessServicePortStatistics::operator>( CRemoteAccessServicePortStatistics const& right_side ) const
{
   WFC_VALIDATE_POINTER( this );

   return (Compare(right_side) == I_AM_GREATER_THAN_THAT ? true : false);
}

_Check_return_ bool CRemoteAccessServicePortStatistics::operator<=( CRemoteAccessServicePortStatistics const& right_side ) const
{
   WFC_VALIDATE_POINTER( this );

   return(Compare(right_side) < I_AM_GREATER_THAN_THAT ? true : false);
}

_Check_return_ bool CRemoteAccessServicePortStatistics::operator>=( CRemoteAccessServicePortStatistics const& right_side ) const
{
   WFC_VALIDATE_POINTER( this );

   return (Compare(right_side) > I_AM_LESS_THAN_THAT) ? true : false );
}

#endif // WFC_NO_RAS

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CRemoteAccessServicePortStatistics</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, RAS, Remote Access Service, Dialup networking">
</HEAD>

<BODY>

<H1>CRemoteAccessServicePortStatistics : CObject</H1>
$Revision: 16 $
<HR>

<H2>Description</H2>
This class is based on the RAS_PORT_STATISTICS structure.

<H2>Data Members</H2>
NumberOfBytesTransmitted<BR>
NumberOfBytesReceived<BR>
NumberOfFramesTransmitted<BR>
NumberOfFramesReceived<BR>
NumberOfCRCErrors<BR>
NumberOfTimeoutErrors<BR>
NumberOfAlignmentErrors<BR>
NumberOfHardwareOverrunErrors<BR>
NumberOfFramingErrors<BR>
NumberOfBufferOverrunErrors<BR>
NumberOfBytesTransmittedUncompressed<BR>
NumberOfBytesReceivedUncompressed<BR>
NumberOfBytesTransmittedCompressed<BR>
NumberOfBytesReceivedCompressed<BR>

<H2>Methods</H2>

<DL COMPACT>

<DT><B><A NAME="Compare">Compare</A></B><DD>Same as std::wstring::compare().
<DT><B><A NAME="Copy">Copy</A></B><DD>Copies another CRemoteAccessServicePortStatistics or a RAS_PORT_STATISTICS structure.
<DT><B><A NAME="Empty">Empty</A></B><DD>Clears all data members.
<DT><B><A NAME="Serialize">Serialize</A></B><DD>After all, we are a CObject...

</DL>

<H2>Operators</H2>

<B>=<BR>==<BR>&lt;<BR>&gt;<BR>&lt;=<BR>&gt;=</B>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: RASPSTAT.CPP $<BR>
$Modtime: 6/26/01 10:55a $
</BODY>

</HTML>
*/
