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
** $Workfile: wfc_kill_most_recently_used_document_list.cpp $
** $Revision: 1 $
** $Modtime: 6/26/01 10:58a $
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

__checkReturn bool PASCAL Win32FoundationClasses::wfc_kill_most_recently_used_document_list( void ) noexcept
{
   CRegistry registry;

   if ( registry.Connect( HKEY_CURRENT_USER ) == FALSE )
   {
      //WFCTRACEERROR( registry.GetErrorCode() );
      return( false );
   }

   if ( registry.Open( L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RecentDocs",
      (CRegistry::CreatePermissions)( CRegistry::permissionRead | CRegistry::permissionSetValue | CRegistry::permissionWrite ) ) == FALSE )
   {
      //WFCTRACEERROR( registry.GetErrorCode() );
      return( false );
   }

   std::wstring list;

   if ( registry.GetStringValue( L"MRUList", list ) == false )
   {
      //WFCTRACEERROR( registry.GetErrorCode() );
      return( false );
   }

   std::wstring name_of_value_to_delete;

   std::vector<uint8_t> unicode_string;

   for ( auto const loop_index : Range(list.length()) )
   {
      //name_of_value_to_delete  = TEXT( "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RecentDocs\\" );
      name_of_value_to_delete.clear();
      name_of_value_to_delete.push_back( list.at( loop_index ) );
      //WFCTRACEVAL( TEXT( "Would delete " ), name_of_value_to_delete );

      (void) registry.GetBinaryValue( name_of_value_to_delete.c_str(), unicode_string );

      if ( registry.DeleteValue( name_of_value_to_delete.c_str() ) == false )
      {
         //WFCTRACEERROR( registry.GetErrorCode() );
         //printf( TEXT( "Can't delete reference to %s\n" ), ascii_string );
      }
   }

   list.clear();

   if ( registry.SetStringValue( L"MRUList", list ) == false )
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
<TITLE>WFC - wfc_kill_most_recently_used_document_list</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that starts the screen saver.">
</HEAD>

<BODY>

<H1>wfc_get_activex_servers</H1>
$Revision: 1 $<HR>

<H2>Declaration</H2>
<PRE>bool <B>wfc_kill_most_recently_used_document_list</B>( void )</PRE>

<H2>Description</H2>
This retrieves a list of websites where ActiveX controls will be
downloaded from.

<H2>Example</H2>

<PRE><CODE>void main( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   if ( <B>wfc_kill_most_recently_used_document_list</B>() == false )
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
