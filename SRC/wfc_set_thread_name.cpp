/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2014, Samuel R. Blackburn
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
** $Workfile: wfc_set_thread_name.cpp $
** $Revision: 7 $
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

#if ( _MSC_VER >= 1300 ) // Visual Studio 7.0

static void __stdcall __set_visual_studio_thread_name( __in_z LPCSTR thread_name ) noexcept
{
#pragma pack( push, 8 )
   struct THREAD_NAME_INFORMATION
   {
       DWORD  Type{ 0 };
       LPCSTR AsciiThreadName{ nullptr };
       DWORD  ThreadID{ 0 };
       DWORD  Flags{ 0 };
   };
#pragma pack( pop )

   THREAD_NAME_INFORMATION thread_name_information;

   thread_name_information.Type            = 0x1000;
   thread_name_information.AsciiThreadName = thread_name;
   thread_name_information.ThreadID        = -1; // GetCurrentThreadId();
   thread_name_information.Flags           = 0;

   __try
   {
      RaiseException( 0x406D1388, // Looks suspiciously like an EXE function address...
                      0,
                      sizeof( thread_name_information ) / sizeof( ULONG_PTR ),
       (ULONG_PTR *) &thread_name_information );
   }
   __except( EXCEPTION_CONTINUE_EXECUTION )
   {
   }
}

#endif // _MSC_VER >= 1300

static inline _Check_return_ Win32FoundationClasses::WFC_THREAD_INFORMATION_BLOCK * _get_thread_information_block( void ) noexcept
{
    Win32FoundationClasses::WFC_THREAD_INFORMATION_BLOCK * return_value{ nullptr };

#if 0
#if defined( _MSC_VER )

   __asm
   {
      mov eax, fs:[18h]
      mov return_value, eax
   }

#elif defined( __GNUC__ )

	__asm__ __volatile__("
       movl	%%fs:(0x18), %%eax\n
       movl	%%eax, %0"
    : "=g" (return_value)
    :
    :"ax", "memory"
	);

#endif
#endif

   return( return_value );
}

/*
** This is basically cleaned up code from the Jan 2000 issue of MSJ
*/

void Win32FoundationClasses::wfc_set_thread_name( __in_z LPCSTR thread_name ) noexcept
{
#if ( _MSC_VER >= 1300 ) // Visual Studio 7.0
   __set_visual_studio_thread_name( thread_name );
#endif

   WFC_THREAD_INFORMATION_BLOCK * thread_information_block_p = _get_thread_information_block();

   thread_information_block_p->Arbitrary = (VOID *) thread_name;
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_set_thread_name</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that allows you to give a string name to a thread.">
</HEAD>

<BODY>

<H1>wfc_set_thread_name</H1>
$Revision: 7 $<HR>

<H2>Declaration</H2>
<PRE>void <B>wfc_set_thread_name</B>( LPCTSTR thread_name )</PRE>

<H2>Description</H2>
This function gives a thread a name supplied by the caller.
This makes debugging easier. Instead of having thread 0x3C, you
can have thread &quot;server_worker_thread&quot;.
<P>By putting <CODE>(char*)((DW(@TIB)+0x14))</CODE> in the debugger&#39;s
watch window, you can see the thread name appear. When you select a new
thread to debug, the thread&#39;s name will automatically appear
in the watch window.

<H2>Example</H2>

<PRE><CODE>int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   <B>wfc_set_thread_name</B>( &quot;_tmain()&quot; );

   std::wstring thread_name;

   <A HREF="wfc_get_thread_name.htm">wfc_get_thread_name</A>( thread_name );

   if ( thread_name.Compare( &quot;_tmain()&quot; ) == 0 )
   {
      _tprintf( TEXT( &quot;Pass.\n&quot; ) );
   }
   else
   {
      _tprintf( TEXT( &quot;FAIL!\n&quot; ) );
   }

   return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>
None! It does use a little Intel specific assembly code.

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_set_thread_name.cpp $<BR>
$Modtime: 6/26/01 10:59a $
</BODY>

</HTML>
*/
