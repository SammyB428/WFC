/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 2003-2014, Samuel R. Blackburn
** All rights reserved.
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
** $Workfile: wfc_kill_most_recently_used_document_list.cpp $
** $Revision: 1 $
** $Modtime: 6/26/01 10:58a $
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

_Check_return_ bool Win32FoundationClasses::wfc_make_ntfs_go_fast( void ) noexcept
{
   CRegistry registry;

   if ( registry.Connect( HKEY_LOCAL_MACHINE ) == FALSE )
   {
      //WFCTRACEERROR( registry.GetErrorCode() );
      return( false );
   }

   if ( registry.Open( TEXT( "SYSTEM\\CurrentControlSet\\Control\\FileSystem" ),
      (CRegistry::CreatePermissions)( CRegistry::permissionRead bitor CRegistry::permissionSetValue bitor CRegistry::permissionWrite ) ) == FALSE )
   {
      //WFCTRACEERROR( registry.GetErrorCode() );
      return( false );
   }

   if ( registry.SetDoubleWordValue( TEXT( "NtfsDisable8dot3NameCreation" ), 1 ) == FALSE )
   {
      //WFCTRACEERROR( registry.GetErrorCode() );
      return( false );
   }

   if ( registry.SetDoubleWordValue( TEXT( "NtfsDisableLastAccessUpdate" ), 1 ) == FALSE )
   {
      //WFCTRACEERROR( registry.GetErrorCode() );
      return( false );
   }
 
   return( true );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_make_ntfs_go_fast</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that optimizes NTFS.">
</HEAD>

<BODY>

<H1>wfc_make_ntfs_go_fast</H1>
$Revision: 1 $<HR>

<H2>Declaration</H2>
<PRE>bol <B>wfc_make_ntfs_go_fast</B>( void )</PRE>

<H2>Description</H2>
This optimizes NTFS by making the following registry changes:
<UL>
<LI>HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\FileSystem\NtfsDisable8dot3NameCreation
<LI>HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\FileSystem\NtfsDisableLastAccessUpdate
</UL>

<H2>Example</H2>

<PRE><CODE>void main( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_main()&quot; ) );

   if ( <B>wfc_make_ntfs_go_fast</B>( trusted_server_list ) == false )
   {
      return;
   }

   printf( &quot;Done.\n&quot; );
}</CODE></PRE>

<HR><I>Copyright, 2003, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_kill_most_recently_used_document_list.cpp $<BR>
$Modtime: 6/26/01 10:58a $
</BODY>
</HTML>
*/
