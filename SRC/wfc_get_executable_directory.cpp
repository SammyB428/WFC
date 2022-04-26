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
** $Workfile: wfc_get_executable_directory.cpp $
** $Revision: 7 $
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

void Win32FoundationClasses::wfc_get_executable_directory(_Out_ std::wstring& directory_name_ending_in_a_slash) noexcept
{
    wchar_t path[4096];

    ZeroMemory(path, sizeof(path));

    directory_name_ending_in_a_slash.clear();

    if (GetModuleFileNameW(static_cast< HMODULE >(NULL), path, static_cast<DWORD>(std::size(path))) not_eq 0)
    {
        *(wcsrchr(path, L'\\') + 1) = 0x00;

        directory_name_ending_in_a_slash.assign(path);
    }
    else
    {
        directory_name_ending_in_a_slash.assign(WSTRING_VIEW(L"\\"));
    }
}

void Win32FoundationClasses::wfc_get_dll_directory(_Out_ std::wstring& directory_name_ending_in_a_slash) noexcept
{
    wchar_t path[4096];

    ZeroMemory(path, sizeof(path));

    HMODULE module_handle{ NULL };

    if (not ::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS bitor
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCWSTR)&Win32FoundationClasses::wfc_am_i_administrator,
        &module_handle))
    {
        directory_name_ending_in_a_slash.clear();
        return;
    }

    if (::GetModuleFileNameW(module_handle, path, static_cast<DWORD>(std::size(path))) not_eq 0)
    {
        *(wcsrchr(path, L'\\') + 1) = 0x00;

        directory_name_ending_in_a_slash.assign(path);
    }
    else
    {
        directory_name_ending_in_a_slash.assign(WSTRING_VIEW(L"\\"));
    }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_get_executable_directory</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that gives you the full pathname to the exectuable.">
</HEAD>

<BODY>

<H1>wfc_get_executable_directory</H1>
$Revision: 7 $<HR>

<H2>Declaration</H2>
<PRE>void <B>wfc_get_executable_directory</B>( std::wstring&amp; directory_name_ending_in_a_slash )</PRE>

<H2>Description</H2>
This function returns the complete path name to the executable. The
string will end in a slash for easy creation of filenames.
Interestingly enough, it is impossible to port this function to Unix.
It is not possible in Unix for a running process to determine which file
it came from. If you ever need an example of something NT can do that
Unix cannot, this is it.

<H2>Example</H2>

<PRE><CODE>int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   std::wstring my_executable;

   <B>wfc_get_executable_directory</B>( my_executable );

   _tprintf( TEXT( &quot;Executing %s\n&quot;, (LPCTSTR) my_executable );

   return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>

<B>wfc_get_executable_directory</B>() uses the following API's:
<UL>
<LI>GetModuleFileName
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_get_executable_directory.cpp $<BR>
$Modtime: 6/26/01 10:58a $
</BODY>

</HTML>
*/
