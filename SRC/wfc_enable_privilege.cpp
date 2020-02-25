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
** $Workfile: wfc_enable_privilege.cpp $
** $Revision: 5 $
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

_Check_return_ bool Win32FoundationClasses::wfc_enable_privilege( __in_z LPCTSTR privilege_name ) noexcept
{
   WFC_TRY
   {
      auto token_handle = static_cast< HANDLE >( NULL );

      if ( OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES bitor TOKEN_QUERY, &token_handle ) == FALSE )
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACE( TEXT( "Can't OpenProcessToken()" ) );
         return( false );
      }

      TOKEN_PRIVILEGES token_privileges;

      ZeroMemory( &token_privileges, sizeof( token_privileges ) );

      if ( LookupPrivilegeValue( nullptr, privilege_name, &token_privileges.Privileges[ 0 ].Luid ) == FALSE )
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACEVAL( TEXT( "Can't LookupPrivilegeValue for " ), privilege_name );
         (void) Win32FoundationClasses::wfc_close_handle( token_handle );
         return( false );
      }

      token_privileges.PrivilegeCount             = 1;
      token_privileges.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED;

      if ( AdjustTokenPrivileges( token_handle, FALSE, &token_privileges, 0, nullptr, 0 ) == FALSE )
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACE( TEXT( "Can't AdjustTokenPrivileges" ) );
         (void) Win32FoundationClasses::wfc_close_handle( token_handle );
         return( false );
      }

      (void) Win32FoundationClasses::wfc_close_handle( token_handle );

      return( true );
   }
   WFC_CATCH_ALL
   {
      return( false );
   }
   WFC_END_CATCH_ALL
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_enable_privilege</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that makes it easy to enable a single privilege.">
</HEAD>

<BODY>

<H1>wfc_enable_privilege</H1>
$Revision: 5 $<HR>

<H2>Declaration</H2>
<PRE>BOOL <B>wfc_enable_privilege</B>( LPCTSTR privilege_name )</PRE>

<H2>Description</H2>
This function returns TRUE if the <CODE>privilege_name</CODE>
was acquired. It will return FALSE if that privilege could
not be enabled.

<H2>Example</H2>

<PRE><CODE>int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   if ( number_of_command_line_arguments &lt; 2 )
   {
      return( EXIT_SUCCESS );
   }

   int loop_index = 1;

   while( loop_index &lt; number_of_command_line_arguments )
   {
      if ( <B>wfc_enable_privilege</B>( command_line_arguments[ loop_index ] ) == FALSE )
      {
         _tprintf( TEXT( &quot;Could not enable the \&quot;%s\&quot; privilege.\n&quot; ),
                   command_line_arguments[ loop_index ] );
      }

      loop_index++;
   }

   return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_enable_privilege</B>() uses the following API's:
<UL>
<LI>AdjustTokenPrivileges
<LI>GetCurrentProcess
<LI>GetLastError
<LI>LookupPrivilegeValue
<LI>OpenProcessToken
<LI>ZeroMemory
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_enable_privilege.cpp $<BR>
$Modtime: 6/26/01 10:58a $
</BODY>

</HTML>
*/
