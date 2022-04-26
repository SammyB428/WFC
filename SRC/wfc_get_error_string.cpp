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
** $Workfile: wfc_get_error_string.cpp $
** $Revision: 14 $
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

void Win32FoundationClasses::wfc_get_error_string( _In_ DWORD const error_code, _Out_ std::wstring& error_string ) noexcept
{
   error_string.clear();

   LPVOID message_buffer{ nullptr };

   ::FormatMessageW( FORMAT_MESSAGE_ALLOCATE_BUFFER bitor
                    FORMAT_MESSAGE_FROM_SYSTEM bitor
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    nullptr,
                    error_code,
                    MAKELANGID( LANG_NEUTRAL, SUBLANG_SYS_DEFAULT ),
          (LPTSTR) &message_buffer,
                    0,
                    nullptr );

   if (message_buffer not_eq nullptr)
   {

       error_string.assign(static_cast<wchar_t const*>(message_buffer));

       ::LocalFree(message_buffer);
       message_buffer = nullptr;
   }

   trim_right( error_string );

   if ( error_string.empty() == true )
   {
       format(error_string, L"Unknown System Error %lu", (unsigned long) error_code );
   }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_get_error_string</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that converts error codes to something humanly readable.">
</HEAD>

<BODY>

<H1>wfc_get_error_string</H1>
$Revision: 14 $<HR>

<H2>Declaration</H2>
<PRE>void <B>wfc_get_error_string</B>( DWORD error_code, std::wstring&amp; error_string )</PRE>

<H2>Description</H2>
This function takes a value returned from GetLastError() and formats 
it into something humanly readable.

<H2>Example</H2>
<PRE><CODE>void print_error( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;print_error()&quot; ) );

   DWORD error_code = GetLastError();

   std::wstring something_a_human_can_understand;

   <B>wfc_get_error_string</B>( error_code, something_a_human_can_understand );

   _tprintf( TEXT( &quot;%s\n&quot; ), (LPCTSTR) something_a_human_can_understand );
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_get_error_string</B>() uses the following API's:
<UL>
<LI>FormatMessage
<LI>LocalFree
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_get_error_string.cpp $<BR>
$Modtime: 6/26/01 10:58a $
</BODY>

</HTML>
*/
