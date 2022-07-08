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
** $Workfile: wfc_am_i_administrator.cpp $
** $Revision: 16 $
** $Modtime: 6/26/01 10:57a $
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

_Check_return_ bool Win32FoundationClasses::wfc_am_i_administrator( void ) noexcept
{
   // Plagerized from Microsoft Knowledge Base article Q118626

   auto access_token_handle{ static_cast<HANDLE>(NULL) };

   BYTE buffer[ 1024 ];

   auto token_groups_p{ reinterpret_cast<PTOKEN_GROUPS>(buffer) };

   DWORD buffer_size{ 0 };

   if ( OpenThreadToken( GetCurrentThread(),
                         TOKEN_QUERY,
                         TRUE,
                        &access_token_handle ) == FALSE )
   {
      if ( GetLastError() not_eq ERROR_NO_TOKEN )
      {
         return( false );
      }

      // retry against process token if no thread token exists

      if ( OpenProcessToken( GetCurrentProcess(),
                             TOKEN_QUERY,
                            &access_token_handle ) == FALSE )
      {
         return( false );
      }
   }

   BOOL success{ ::GetTokenInformation(access_token_handle,
                                  TokenGroups,
                                  buffer,
                                  1024,
                                 &buffer_size) };

   // use wfc_close_handle instead of CloseHandle when you don't want exceptions thrown
   std::ignore = Win32FoundationClasses::wfc_close_handle( access_token_handle );
   access_token_handle = static_cast< HANDLE >( NULL );

   if ( success == FALSE )
   {
      return( false );
   }

   SID_IDENTIFIER_AUTHORITY nt_authority_sid{ SECURITY_NT_AUTHORITY };

   PSID administrators_sid_p{ nullptr };

   if ( AllocateAndInitializeSid( &nt_authority_sid,
                                   2,
                                   SECURITY_BUILTIN_DOMAIN_RID,
                                   DOMAIN_ALIAS_RID_ADMINS,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                  &administrators_sid_p ) == FALSE )
   {
      return( false );
   }

   // assume that we don't find the admin SID.
   success = FALSE;

   for ( auto const x : Range(token_groups_p->GroupCount) )
   {
      if ( EqualSid( administrators_sid_p, token_groups_p->Groups[ x ].Sid ) not_eq FALSE )
      {
         success = TRUE;
         break;
      }
   }

   FreeSid( administrators_sid_p );

   return( success == FALSE ? false : true );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_am_i_administrator</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="Simple C function that tells you if you are running with Administrator priveleges.">
</HEAD>

<BODY>

<H1>wfc_am_i_administrator</H1>
$Revision: 16 $<HR>

<H2>Declaration</H2>

<PRE>BOOL <B>wfc_am_i_administrator</B>( void )</PRE>

<H2>Description</H2>

This function returns TRUE if your program is running
with Administrator priveleges, FALSE if you don't have
Administrator proveleges. It uses the method described in
the Microsoft Knowledge Base article
<A HREF="http://www.microsoft.com/kb/articles/q118/6/26.htm" TARGET="_parent">Q118626</A>.

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   if ( <B>wfc_am_i_administrator</B>() not_eq FALSE )
   {
      _tprintf( TEXT( &quot;You are Administrator or in the Administrators group\n&quot; ) );
   }
   else
   {
      _tprintf( TEXT( &quot;You are nothing, a nobody!\n&quot; ) );
   }

   return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>

<B>wfc_am_i_administrator</B>() uses the following API's:
<UL>
<LI>AllocateAndInitializeSid
<LI>EqualSid
<LI>FreeSid
<LI>GetCurrentProcess
<LI>GetCurrentThread
<LI>GetLastError
<LI>GetTokenInformation
<LI>OpenProcessToken
<LI>OpenThreadToken
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_am_i_administrator.cpp $<BR>
$Modtime: 6/26/01 10:57a $
</BODY>

</HTML>
*/
