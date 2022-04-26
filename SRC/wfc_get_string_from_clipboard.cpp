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
** $Workfile: wfc_get_string_from_clipboard.cpp $
** $Revision: 4 $
** $Modtime: 6/26/01 10:58a $
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

_Check_return_ bool Win32FoundationClasses::wfc_get_string_from_clipboard( _Out_ std::wstring& string_to_return ) noexcept
{
   if ( OpenClipboard( static_cast< HWND >( NULL ) ) == FALSE )
   {
      //WFCTRACEERROR( GetLastError() );
      string_to_return.clear();
      return( false );
   }

   auto data_handle{ ::GetClipboardData(CF_TEXT) };

   if ( data_handle not_eq static_cast< HANDLE >( NULL ) )
   {
      copy( string_to_return, reinterpret_cast<LPCSTR>(data_handle)  );

      CloseClipboard();

      return( true );
   }
   else
   {
      //WFCTRACEERROR( GetLastError() );
      string_to_return.clear();
      return( false );
   }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_get_string_from_clipboard</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that starts the screen saver.">
</HEAD>

<BODY>

<H1>wfc_get_string_from_clipboard</H1>
$Revision: 4 $<HR>

<H2>Declaration</H2>
<PRE>bool <B>wfc_get_string_from_clipboard</B>( std::wstring&amp; string_to_return )</PRE>

<H2>Description</H2>
This function retrieves a string from the clipboard.

<H2>Example</H2>

<PRE><CODE>&#35;if ! defined( WINVER )
&#35;define WINVER 0x401
&#35;endif // WINVER

&#35;define WFC_STL

&#35;include &lt;wfc.h&gt;
&#35;include &lt;imagehlp.h&gt;
&#35;pragma hdrstop

&#35;pragma comment( lib, &quot;imagehlp.lib&quot; )

int _tmain( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   TCHAR undecorated_name[ 1025 ];

   ZeroMemory( undecorated_name, sizeof( undecorated_name ) );

   std::wstring clipboard_string;

   if ( <B>wfc_get_string_from_clipboard</B>( clipboard_string ) == true )
   {
      if ( UnDecorateSymbolName( clipboard_string,
                                 undecorated_name,
                                 sizeof( undecorated_name ),
                                 UNDNAME_COMPLETE ) > 0 )
      {
         _tprintf( TEXT( &quot;%s\n&quot; ), undecorated_name );
      }
      else
      {
         _tprintf( TEXT( &quot;%s doesn't demangle\n&quot; ), (LPCTSTR) clipboard_string );
      }
   }

   return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_get_string_from_clipboard</B>() uses the following API's:
<UL>
<LI>CloseClipboard
<LI>GetClipboardData
<LI>OpenClipboard
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_get_string_from_clipboard.cpp $<BR>
$Modtime: 6/26/01 10:58a $
</BODY>
</HTML>
*/
