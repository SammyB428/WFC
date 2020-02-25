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
** $Workfile: wfc_enable_all_privileges.cpp $
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

_Check_return_ bool Win32FoundationClasses::wfc_enable_all_privileges( void ) noexcept
{
   static constexpr LPCTSTR static_PrivilegeNames[] =
   {
      SE_INTERACTIVE_LOGON_NAME,
      SE_NETWORK_LOGON_NAME,
      SE_BATCH_LOGON_NAME,
      SE_SERVICE_LOGON_NAME,
      SE_CREATE_TOKEN_NAME,
      SE_ASSIGNPRIMARYTOKEN_NAME,
      SE_LOCK_MEMORY_NAME,
      SE_INCREASE_QUOTA_NAME,
      SE_UNSOLICITED_INPUT_NAME,
      SE_MACHINE_ACCOUNT_NAME,
      SE_TCB_NAME,
      SE_SECURITY_NAME,
      SE_TAKE_OWNERSHIP_NAME,
      SE_LOAD_DRIVER_NAME,
      SE_SYSTEM_PROFILE_NAME,
      SE_SYSTEMTIME_NAME,
      SE_PROF_SINGLE_PROCESS_NAME,
      SE_INC_BASE_PRIORITY_NAME,
      SE_CREATE_PAGEFILE_NAME,
      SE_CREATE_PERMANENT_NAME,
      SE_BACKUP_NAME,
      SE_RESTORE_NAME,
      SE_SHUTDOWN_NAME,
      SE_DEBUG_NAME,
      SE_AUDIT_NAME,
      SE_SYSTEM_ENVIRONMENT_NAME,
      SE_CHANGE_NOTIFY_NAME,
      SE_REMOTE_SHUTDOWN_NAME,
      SE_DENY_INTERACTIVE_LOGON_NAME, // New NT5 privileges and rights
      SE_DENY_NETWORK_LOGON_NAME,
      SE_DENY_BATCH_LOGON_NAME,
      SE_DENY_SERVICE_LOGON_NAME,
      SE_UNDOCK_NAME,
      SE_SYNC_AGENT_NAME,
      SE_ENABLE_DELEGATION_NAME,
      SE_MANAGE_VOLUME_NAME,
      SE_REMOTE_INTERACTIVE_LOGON_NAME, // Longhorn privileges
      SE_DENY_REMOTE_INTERACTIVE_LOGON_NAME,
      SE_IMPERSONATE_NAME,
      SE_CREATE_GLOBAL_NAME,
      SE_TRUSTED_CREDMAN_ACCESS_NAME,
      SE_RELABEL_NAME,
      SE_INC_WORKING_SET_NAME,
      SE_TIME_ZONE_NAME,
      SE_CREATE_SYMBOLIC_LINK_NAME,
      SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME, // Windows 10?
      SE_ACTIVATE_AS_USER_CAPABILITY,
      SE_CONSTRAINED_IMPERSONATION_CAPABILITY,
      SE_SESSION_IMPERSONATION_CAPABILITY,
      SE_MUMA_CAPABILITY,
      SE_DEVELOPMENT_MODE_NETWORK_CAPABILITY,
      SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME,
   };

   bool return_value = true;

   for ( auto const loop_index : Range( std::size( static_PrivilegeNames ) ) )
   {
      if ( Win32FoundationClasses::wfc_enable_privilege( static_PrivilegeNames[ loop_index ] ) == false )
      {
         return_value = false;
      }
   }

   return( return_value );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_enable_all_privileges</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that encapsulates CloseHandle(). It will spit out a debugging message if CloseHandle() fails.">
</HEAD>

<BODY>

<H1>wfc_enable_all_privileges</H1>
$Revision: 5 $<HR>

<H2>Declaration</H2>
<PRE>BOOL <B>wfc_enable_all_privileges</B>( void )</PRE>

<H2>Description</H2>
This function returns TRUE if all privileges on a system
have been enabled. FALSE will be returned if at least one
privelege could not be enabled.
It will attempt to enable the following privileges:

<UL>
<LI><CODE>SE_INTERACTIVE_LOGON_NAME</CODE>
<LI><CODE>SE_NETWORK_LOGON_NAME</CODE>
<LI><CODE>SE_BATCH_LOGON_NAME</CODE>
<LI><CODE>SE_SERVICE_LOGON_NAME</CODE>
<LI><CODE>SE_CREATE_TOKEN_NAME</CODE>
<LI><CODE>SE_ASSIGNPRIMARYTOKEN_NAME</CODE>
<LI><CODE>SE_LOCK_MEMORY_NAME</CODE>
<LI><CODE>SE_INCREASE_QUOTA_NAME</CODE>
<LI><CODE>SE_UNSOLICITED_INPUT_NAME</CODE>
<LI><CODE>SE_MACHINE_ACCOUNT_NAME</CODE>
<LI><CODE>SE_TCB_NAME</CODE>
<LI><CODE>SE_SECURITY_NAME</CODE>
<LI><CODE>SE_TAKE_OWNERSHIP_NAME</CODE>
<LI><CODE>SE_LOAD_DRIVER_NAME</CODE>
<LI><CODE>SE_SYSTEM_PROFILE_NAME</CODE>
<LI><CODE>SE_SYSTEMTIME_NAME</CODE>
<LI><CODE>SE_PROF_SINGLE_PROCESS_NAME</CODE>
<LI><CODE>SE_INC_BASE_PRIORITY_NAME</CODE>
<LI><CODE>SE_CREATE_PAGEFILE_NAME</CODE>
<LI><CODE>SE_CREATE_PERMANENT_NAME</CODE>
<LI><CODE>SE_BACKUP_NAME</CODE>
<LI><CODE>SE_RESTORE_NAME</CODE>
<LI><CODE>SE_SHUTDOWN_NAME</CODE>
<LI><CODE>SE_DEBUG_NAME</CODE>
<LI><CODE>SE_AUDIT_NAME</CODE>
<LI><CODE>SE_SYSTEM_ENVIRONMENT_NAME</CODE>
<LI><CODE>SE_CHANGE_NOTIFY_NAME</CODE>
<LI><CODE>SE_REMOTE_SHUTDOWN_NAME</CODE>
<LI><CODE>SE_DENY_INTERACTIVE_LOGON_NAME</CODE>
<LI><CODE>SE_DENY_NETWORK_LOGON_NAME</CODE>
<LI><CODE>SE_DENY_BATCH_LOGON_NAME</CODE>
<LI><CODE>SE_DENY_SERVICE_LOGON_NAME</CODE>
<LI><CODE>SE_UNDOCK_NAME</CODE>
<LI><CODE>SE_SYNC_AGENT_NAME</CODE>
<LI><CODE>SE_ENABLE_DELEGATION_NAME</CODE>
<LI><CODE>SE_MANAGE_VOLUME_NAME</CODE>
<LI><CODE>SE_REMOTE_INTERACTIVE_LOGON_NAME</CODE>
<LI><CODE>SE_DENY_REMOTE_INTERACTIVE_LOGON_NAME</CODE>
<LI><CODE>SE_IMPERSONATE_NAME</CODE>
<LI><CODE>SE_CREATE_GLOBAL_NAME</CODE>
</UL>

<H2>Example</H2>

<PRE><CODE>int _tmain( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   if ( <B>wfc_enable_all_privileges</B>() == FALSE )
   {
      _tprintf( TEXT( &quot;Could not enable all privileges.\n&quot; ) );
   }

   return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_enable_all_privileges</B>() uses the following API's:
<UL>
<LI>None.
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_enable_all_privileges.cpp $<BR>
$Modtime: 6/26/01 10:58a $
</BODY>

</HTML>
*/
