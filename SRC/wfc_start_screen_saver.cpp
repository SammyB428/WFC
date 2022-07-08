/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2022, Samuel R. Blackburn
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
** $Workfile: wfc_start_screen_saver.cpp $
** $Revision: 12 $
** $Modtime: 6/26/01 10:59a $
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

void Win32FoundationClasses::wfc_start_screen_saver( void ) noexcept
{
   ::SendMessage( GetDesktopWindow(), WM_SYSCOMMAND, SC_SCREENSAVE, 0L );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_start_screen_saver</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that starts the screen saver.">
</HEAD>

<BODY>

<H1>wfc_start_screen_saver</H1>
$Revision: 12 $<HR>

<H2>Declaration</H2>
<PRE>void <B>wfc_start_screen_saver</B>( void )</PRE>

<H2>Description</H2>
This function starts the current screen saver.

<H2>Example</H2>

<PRE><CODE>int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   <B>wfc_start_screen_saver</B>();

   return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_start_screen_saver</B>() uses the following API's:
<UL>
<LI>GetDesktopWindow
<LI>SendMessage
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_start_screen_saver.cpp $<BR>
$Modtime: 6/26/01 10:59a $
</BODY>
</HTML>
*/
