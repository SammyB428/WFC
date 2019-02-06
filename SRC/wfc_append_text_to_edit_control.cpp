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
** $Workfile: wfc_append_text_to_edit_control.cpp $
** $Revision: 7 $
** $Modtime: 6/26/01 10:57a $
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

void PASCAL Win32FoundationClasses::wfc_append_text_to_edit_control( __in HWND edit_control_handle, __in_z LPCTSTR text_string ) noexcept
{
   if ( text_string == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it...

   WFC_TRY
   {
      int index = GetWindowTextLength( edit_control_handle );

      SetFocus( edit_control_handle ); // Do I need this?
      SendMessage( edit_control_handle, EM_SETSEL, (WPARAM) index, (LPARAM) index );
      SendMessage( edit_control_handle, EM_REPLACESEL, 0, (LPARAM) text_string );
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
<TITLE>WFC - wfc_append_text_to_edit_control</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that makes it easy to add text to an edit control.">
</HEAD>

<BODY>

<H1>wfc_append_text_to_edit_control</H1>
$Revision: 7 $<HR>

<H2>Declaration</H2>

<PRE>void <B>wfc_append_text_to_edit_control</B>( void )</PRE>

<H2>Description</H2>

This function appends a string to an edit control.

<H2>Example</H2>

<PRE><CODE>void add_time( HWND edit_control_handle )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;add_time()&quot; ) );

   CTime the_time = CTime::GetCurrentTime();

   std::wstring time_string;

   time_string = the_time.Format( TEXT( &quot;%Y-%m-%dT%H:%M:%S&quot; ) );
   
   <B>wfc_append_text_to_edit_control</B>( edit_control_handle, time_string );
}</CODE></PRE>

<H2>API's Used</H2>

<B>wfc_append_text_to_edit_control</B>() uses the following API's:
<UL>
<LI>GetWindowTextLength
<LI>SendMessage
<LI>SetFocus
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_append_text_to_edit_control.cpp $<BR>
$Modtime: 6/26/01 10:57a $
</BODY>

</HTML>
*/
