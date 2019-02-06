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
** $Workfile: wfc_move_file_at_reboot.cpp $
** $Revision: 8 $
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

__checkReturn bool PASCAL Win32FoundationClasses::wfc_move_file_at_reboot( _In_ const std::wstring& existing_file, __in const std::wstring& file_to_move ) noexcept
{
   // First, we must delete the destination file

   if ( MoveFileExW( file_to_move.c_str(), nullptr, MOVEFILE_DELAY_UNTIL_REBOOT ) == FALSE )
   {
      //WFCTRACEERROR( GetLastError() );
      ///WFCTRACEVAL( TEXT( "Can't schedule a deletion for " ), file_to_move );
   }

   if ( MoveFileExW( existing_file.c_str(), file_to_move.c_str(), MOVEFILE_DELAY_UNTIL_REBOOT ) == FALSE )
   {
      //WFCTRACEERROR( GetLastError() );
      //WFCTRACEVAL( TEXT( "Can't schedule a move for " ), file_to_move );
      return( false );
   }

   // The children at Microsoft that wrote Windows 95 forgot about MoveFileEx
   // so they stuck with something they could understand, INI files!!!!

   // WritePrivateProfileString( TEXT( "Rename" ), file_to_move, existing_file, TEXT( "wininit.ini" ) );

   return( true );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_move_file_at_reboot</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="Simple C function that moves a file at boot time.">
</HEAD>

<BODY>

<H1>wfc_move_file_at_reboot</H1>
$Revision: 8 $<HR>

<H2>Declaration</H2>

<PRE>void <B>wfc_move_file_at_reboot</B>( const std::wstring&amp; existing_file, const std::wstring&amp; file_to_move )</PRE>

<H2>Description</H2>

This function allows you to replace a file at reboot. You normally
need to do this when the file is constantly in use (like a DLL). The
only time to replace the file is when it is not being used by the
program that is always running. The only time to do this is when
NT is rebooting. This is how operating system files get updated.

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   if ( number_of_command_line_arguments < 3 )
   {
      _tprintf( TEXT( &quot;Usage: moveatreboot source destination\n&quot; ) );
      return( EXIT_FAILURE );
   }

   if ( <B>wfc_move_file_at_reboot</B>( command_line_arguments[ 1 ], command_line_arguments[ 2 ] ) != FALSE )
   {
      _tprintf( &quot;Move OK\n&quot; );
   }
   else
   {
      _tprintf( &quot;Move not ok\n&quot; );
   }

   return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_move_file_at_reboot</B>() uses the following API's:
<UL>
<LI>MoveFileEx
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_move_file_at_reboot.cpp $<BR>
$Modtime: 6/26/01 10:59a $
</BODY>

</HTML>
*/
