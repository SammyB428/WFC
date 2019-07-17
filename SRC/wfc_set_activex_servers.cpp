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
** $Workfile: wfc_set_activex_servers.cpp $
** $Revision: 3 $
** $Modtime: 6/26/01 10:59a $
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

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_set_activex_servers( __in std::vector<std::wstring> const& server_list ) noexcept
{
   std::wstring string_to_put_in_registry;
   std::wstring temp_string;

   for ( auto const loop_index : Range(server_list.size()) )
   {
      temp_string.assign( server_list.at( loop_index ) );

      trim(temp_string);

      if ( temp_string.empty() == false )
      {
         if ( compare_no_case( temp_string, L"CODEBASE" ) == I_AM_EQUAL_TO_THAT )
         {
            // This is the evil CODEBASE setting. It means your computer is
            // wide open to downloading malicious ActiveX controls
            string_to_put_in_registry.append( temp_string );
         }
         else
         {
            string_to_put_in_registry.push_back( '<' );
            string_to_put_in_registry.append( temp_string );
            string_to_put_in_registry.push_back( '>' );
         }

         string_to_put_in_registry.push_back( ';' );
      }
   }

   trim_right( string_to_put_in_registry, ';' );

   CRegistry registry;

   if ( registry.Connect( HKEY_LOCAL_MACHINE ) == false )
   {
      //WFCTRACEERROR( registry.GetErrorCode() );
      return( false );
   }

   if ( registry.Open( L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings" ) == false )
   {
      //WFCTRACEERROR( registry.GetErrorCode() );
      return( false );
   }

   if ( registry.SetValue( L"CodeBaseSearchPath", string_to_put_in_registry ) == false )
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
<TITLE>WFC - wfc_set_activex_servers</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that controls which servers ActiveX controls can be downloaded from.">
</HEAD>

<BODY>

<H1>wfc_set_activex_servers</H1>
$Revision: 3 $<HR>

<H2>Declaration</H2>
<PRE>bool <B>wfc_set_activex_servers</B>( const std::vector&lt;std::wstring&gt;&amp; server_list )</PRE>

<H2>Description</H2>
This sets the list of websites where ActiveX controls will be
downloaded from.

<H2>Example</H2>

<PRE><CODE>bool prevent_download_of_malicious_activex_controls( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   std::vector<std::wstring> trusted_server_list;

   if ( <A HREF="wfc_get_activex_servers.htm">wfc_get_activex_servers</A>( trusted_server_list ) == false )
   {
      return( false );
   }

   int loop_index = 0;

   std::wstring server_name;

   while( loop_index &lt; trusted_server_list.GetSize() )
   {
      server_name = trusted_server_list.GetAt( loop_index );

      if ( server_name.CompareNoCase( &quot;CODEBASE&quot; ) == 0 )
      {
         trusted_server_list.RemoveAt( loop_index );
      }
      else
      {
         loop_index++;
      }
   }

   if ( <B>wfc_set_activex_servers</B>( trusted_server_list ) == false )
   {
      return( false );
   }

   return( true );
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_set_activex_servers.cpp $<BR>
$Modtime: 6/26/01 10:59a $
</BODY>
</HTML>
*/
