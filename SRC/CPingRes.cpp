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
** $Workfile: CPingRes.cpp $
** $Revision: 26 $
** $Modtime: 6/26/01 10:50a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CPingResults::CPingResults() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CPingResults::CPingResults( _In_ Win32FoundationClasses::CPingResults const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CPingResults::CPingResults( _In_ Win32FoundationClasses::CPingResults const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CPingResults::CPingResults( _In_ IP_ECHO_REPLY const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CPingResults::CPingResults( _In_ IP_ECHO_REPLY const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

void Win32FoundationClasses::CPingResults::Copy( _In_ Win32FoundationClasses::CPingResults const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   Address.assign( source.Address );
   RoundTripTimeInMilliseconds = source.RoundTripTimeInMilliseconds;
   TimeToLive                  = source.TimeToLive;
}

void Win32FoundationClasses::CPingResults::Copy( _In_ Win32FoundationClasses::CPingResults const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( source not_eq nullptr )
      {
         Copy( *source );
      }
      else
      {
         Empty();
      }
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CPingResults::Copy( _In_ IP_ECHO_REPLY const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Win32FoundationClasses::format( Address, "%d.%d.%d.%d",
             (int) LOBYTE( LOWORD( source.Address ) ),
             (int) HIBYTE( LOWORD( source.Address ) ),
             (int) LOBYTE( HIWORD( source.Address ) ),
             (int) HIBYTE( HIWORD( source.Address ) ) );
   RoundTripTimeInMilliseconds = source.RoundTripTime;
   TimeToLive                  = source.Options.Ttl;
}

void Win32FoundationClasses::CPingResults::Copy( _In_ IP_ECHO_REPLY const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( source not_eq nullptr )
      {
         Copy( *source );
      }
      else
      {
         Empty();
      }
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CPingResults::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Address.clear();
   RoundTripTimeInMilliseconds = 0;
   TimeToLive = 0;
}

Win32FoundationClasses::CPingResults& Win32FoundationClasses::CPingResults::operator = ( _In_ Win32FoundationClasses::CPingResults const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CPingResults::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );

   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   Address is                   \"" ) << Address << TEXT( "\"\n" );
   dump_context << TEXT( "   RoundTripTimeInMilliseconds is " ) << RoundTripTimeInMilliseconds << TEXT( "\n" );
   dump_context << TEXT( "   TimeToLive                  is " ) << TimeToLive << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CPingResults</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that holds the results of a ping.">
</HEAD>

<BODY>
<H1>CPingResults</H1>
$Revision: 26 $
<HR>

<H2>Description</H2>
This class is filled in by
<A HREF="CPing.htm">CPing::Ping()</A>. It contains the data gathered in the ping.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CPingResults</B>()
<B>CPingResults</B>( const CPingResults&amp;   source )
<B>CPingResults</B>( const CPingResults *  source )
<B>CPingResults</B>( const IP_ECHO_REPLY&amp;  source )
<B>CPingResults</B>( const IP_ECHO_REPLY * source )</PRE><DD>
Constructs the object.

</DL>

<H2>Data Members</H2>

<DL COMPACT>

<DT><PRE>std::wstring <B>Address</B></PRE><DD>The dotted IP address of the pinged host.

<DT><PRE>DWORD <B>RoundTripTimeInMilliseconds</B></PRE><DD>How many milliseconds it took for the packet to go and come back.

<DT><PRE>BYTE <B>TimeToLive</B></PRE><DD>Number of &quothops&quot

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Copy">Copy</A></B>( const CPingResults&amp;   source )
void <B>Copy</B>( const CPingResults *  source )
void <B>Copy</B>( const IP_ECHO_REPLY&amp;  source )
void <B>Copy</B>( const IP_ECHO_REPLY * source )</PRE><DD>
Copies other <B>CPingResults</B> classes or IP_ECHO_REPLY structures.

<DT><PRE>void <B><A NAME="Dump">Dump</A></B>( CDumpContext&amp; dump_context ) const</PRE><DD>
Present only in debug builds. Dumps the contents of this object to the
<CODE>dump_context</CODE>.

<DT><PRE>void <B><A NAME="Empty">Empty</A></B>( void )</PRE><DD>
Resets all data members to an emtpy state.

</DL>

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   <A HREF="SOCKETS.htm">CSimpleSocket</A>::StartWindowsSockets();

   <A HREF="CPing.htm">CPing</A> ping;

   DWORD return_value = 0;

   <B>CPingResults</B> results;

   return_value = ping.Ping( TEXT( &quot;20.2.1.3&quot; ), &amp;results );

   if ( return_value == 0 )
   {
      DWORD error_code = ping.GetErrorCode();

      std::wstring error_message;

      ping.ConvertErrorToString( error_code, error_message );

      _tprintf( TEXT( &quot;Ping Error: %s\n&quot; ), error_message.c_str() );
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

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CPingRes.cpp $<BR>
$Modtime: 6/26/01 10:50a $
</BODY>

</HTML>
*/
