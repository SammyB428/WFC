/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2016, Samuel R. Blackburn
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
** $Workfile: CTapeGetMediaParameters.cpp $
** $Revision: 14 $
** $Modtime: 6/26/01 10:51a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

CTapeGetMediaParameters::CTapeGetMediaParameters()
{
   Empty();
}

CTapeGetMediaParameters::CTapeGetMediaParameters( __in CTapeGetMediaParameters const& source )
{
   Copy( source );
}

CTapeGetMediaParameters::CTapeGetMediaParameters( __in TAPE_GET_MEDIA_PARAMETERS const * source )
{
   Copy( source );
}

CTapeGetMediaParameters::~CTapeGetMediaParameters()
{
   Empty();
}

void CTapeGetMediaParameters::Copy( __in CTapeGetMediaParameters const& source ) noexcept
{
   Copy( reinterpret_cast<TAPE_GET_MEDIA_PARAMETERS const *>(&source) );
}

void CTapeGetMediaParameters::Copy( __in TAPE_GET_MEDIA_PARAMETERS const * source ) noexcept
{
   ASSERT( source != nullptr );

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      Capacity.LowPart   = source->Capacity.LowPart;
      Capacity.HighPart  = source->Capacity.HighPart;
      Remaining.LowPart  = source->Remaining.LowPart;
      Remaining.HighPart = source->Remaining.HighPart;
      BlockSize          = source->BlockSize;
      PartitionCount     = source->PartitionCount;
      WriteProtected     = source->WriteProtected;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CTapeGetMediaParameters::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CTapeGetMediaParameters at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   Capacity.LowPart = "   ) << Capacity.LowPart   << TEXT( "\n" );
   dump_context << TEXT( "   Capacity.HighPart = "  ) << Capacity.HighPart  << TEXT( "\n" );
   dump_context << TEXT( "   Remaining.LowPart = "  ) << Remaining.LowPart  << TEXT( "\n" );
   dump_context << TEXT( "   Remaining.HighPart = " ) << Remaining.HighPart << TEXT( "\n" );
   dump_context << TEXT( "   BlockSize = "          ) << BlockSize          << TEXT( "\n" );
   dump_context << TEXT( "   PartitionCount = "     ) << PartitionCount     << TEXT( "\n" );
   dump_context << TEXT( "   WriteProtected = "     ) << WriteProtected     << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void CTapeGetMediaParameters::Empty( void ) noexcept
{
   Capacity.LowPart   = 0;
   Capacity.HighPart  = 0;
   Remaining.LowPart  = 0;
   Remaining.HighPart = 0;
   BlockSize          = 0;
   PartitionCount     = 0;
   WriteProtected     = 0;
}

CTapeGetMediaParameters& CTapeGetMediaParameters::operator=( __in CTapeGetMediaParameters const& source ) noexcept
{
   Copy( source );
   return( *this );
}

CTapeGetMediaParameters& CTapeGetMediaParameters::operator=( __in TAPE_GET_MEDIA_PARAMETERS const& source ) noexcept
{
   Copy( &source );
   return( *this );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CTapeGetMediaParameters</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that encapsulates TAPE_GET_MEDIA_PARAMETERS.">
</HEAD>

<BODY>

<H1>CTapeGetMediaParameters : _TAPE_GET_MEDIA_PARAMETERS</H1>
$Revision: 14 $<BR>
<HR>

<H2>Description</H2>

This class wraps the TAPE_GET_MEDIA_PARAMETERS data structure.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CTapeGetMediaParameters</B>()
<B>CTapeGetMediaParameters</B>( const CTapeGetMediaParameters&amp; source )
<B>CTapeGetMediaParameters</B>( const TAPE_GET_MEDIA_PARAMETERS * source )</PRE><DD>
Constructs the object.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Copy">Copy</A></B>( const CTapeGetMediaParameters&amp; source )
void <B>Copy</B>( const TAPE_GET_MEDIA_PARAMETERS * source )</PRE><DD>
Copies the contents of <CODE>source</CODE> to this object.

<DT><PRE>void <B><A NAME="Dump">Dump</A></B>( CDumpContext&amp; dump_context )</PRE><DD>
Present in debug builds only. Dumps the contents of the object
to the <CODE>dump_context</CODE>.

<DT><PRE>void <B><A NAME="Empty">Empty</A></B>( void )</PRE><DD>
Zeroizes the member variables.

</DL>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CTapeGetMediaParameters.cpp $<BR>
$Modtime: 6/26/01 10:51a $
</BODY>

</HTML>
The following line should go in AUTOEXP.DAT so the debugging tooltips will format properly
ToolTipFormatLine=CTape=hFile=<m_hFile> name=<m_strFileName.m_pchData,s>
*/
