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
** $Workfile: CCOMSucks.cpp $
** $Revision: 5 $
** $Modtime: 6/26/01 10:44a $
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

static DWORD WINAPI com_sucks_message_pump_thread(VOID * parameter) noexcept
{
    // Use Mutlithreaded apartment model, not that it matters much...
    (void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    VARIANT string_variant;
    VARIANT integer_variant;

    VariantInit(&string_variant);
    VariantInit(&integer_variant);

    BSTR string_value = SysAllocString(L"COM Sucks!");

    // This conversion will lockup COM in NT4 and below
    (void)VariantChangeType(&integer_variant, &string_variant, 0, VT_I2);

    SetEvent(reinterpret_cast<HANDLE>(parameter));

    VariantClear(&integer_variant);
    VariantClear(&string_variant);

    MSG window_message;

    // Start the pump

    while (GetMessage(&window_message, static_cast<HWND>(NULL), 0, 0))
    {
        TranslateMessage(&window_message);
        DispatchMessage(&window_message);
    }

    CoUninitialize();
    return(0);
}

Win32FoundationClasses::CCOMSucks::CCOMSucks() noexcept
{
    DWORD thread_id = 0;

    auto const event_handle = MANUAL_RESET_EVENT();

    if (event_handle != NULL)
    {
        auto const thread_handle = ::CreateThread(nullptr,
            0,
            com_sucks_message_pump_thread,
            reinterpret_cast<LPVOID>(event_handle),
            0,
            &thread_id);

        if (thread_handle != NULL)
        {
            ::WaitForSingleObject(event_handle, 5000);
            (void)Win32FoundationClasses::wfc_close_handle(thread_handle);
        }
    }

    (void)Win32FoundationClasses::wfc_close_handle(event_handle);
}

Win32FoundationClasses::CCOMSucks::~CCOMSucks() noexcept
{
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CCOMSucks</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that allows COM to be used in a non-GUI application.">
</HEAD>

<BODY>

<H1>CCOMSucks</H1>
$Revision: 5 $<BR>
<HR>

<H2>Description</H2>

This class allows you to use COM in a non-GUI application. It solves the
problem described in Knowledge Base article
<A HREF="http://support.microsoft.com/support/kb/articles/Q189/4/27.ASP">Q189427</A>.

<H2>Data Members</H2>
None.

<H2>Methods</H2>
None.

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

void main( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;read_next_chunk()&quot; ) );

   <B>CCOMSucks</B> com_requires_a_gui;

   do_non_gui_server_stuff();
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CCOMSucks.cpp $<BR>
$Modtime: 6/26/01 10:44a $
</BODY>

</HTML>
*/
