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
** $Workfile: wfc_create_hard_link.cpp $
** $Revision: 8 $
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

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_create_hard_link( __in_z LPCTSTR new_filename, __in_z LPCTSTR existing_filename, __in_opt LPSECURITY_ATTRIBUTES sa ) noexcept
{
   WFC_TRY
   {
      ASSERT( new_filename != nullptr );
      ASSERT( existing_filename != nullptr );
      ASSERT( new_filename != existing_filename );
      ASSERT( _tcscmp( new_filename, existing_filename ) != 0 );

      if ( new_filename      == nullptr or
           existing_filename == nullptr or
           new_filename      == existing_filename )
      {
         return( false );
      }

      if ( _tcscmp( new_filename, existing_filename ) == 0 )
      {
         return( false );
      }

      // Enable some privileges, this improves our chances of succeeding

      if ( wfc_enable_privilege( SE_BACKUP_NAME ) == false )
      {
         //WFCTRACE( TEXT( "Can't enable the backup privilege." ) );
      }

      if ( wfc_enable_privilege( SE_RESTORE_NAME ) == false )
      {
         //WFCTRACE( TEXT( "Can't enable the restore privilege." ) );
      }

      HANDLE existing_file_handle = CreateFile( existing_filename,
                                         FILE_WRITE_ATTRIBUTES,
                                         FILE_SHARE_READ bitor FILE_SHARE_WRITE bitor FILE_SHARE_DELETE,
                                         sa,
                                         OPEN_EXISTING,
                                         0,
                                         static_cast< HANDLE >( NULL ) );

      if ( existing_file_handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
      {
         //WFCTRACEERROR( GetLastError() );
         return( false );
      }

      // Now we descend into the UNICODE world...

      WCHAR link_file_path[ MAX_PATH + 1 ];
      WCHAR unicode_new_filename[ MAX_PATH + 1 ];

      ZeroMemory( link_file_path, sizeof( link_file_path ) );
      ZeroMemory( unicode_new_filename, sizeof( unicode_new_filename ) );

#if ! defined( UNICODE )
      ASCII_to_UNICODE( new_filename, unicode_new_filename );
#else
      _tcscpy_s( unicode_new_filename, std::size( unicode_new_filename ), new_filename );
#endif

      DWORD link_file_path_length = 0;

      LPWSTR unicode_filename_portion = nullptr;

      link_file_path_length = GetFullPathNameW( unicode_new_filename, MAX_PATH, link_file_path, &unicode_filename_portion );

      if ( link_file_path_length == 0 )
      {
         //WFCTRACEERROR( GetLastError() );

         (void) Win32FoundationClasses::wfc_close_handle( existing_file_handle );
         return( false );
      }

      DWORD number_of_bytes_in_path = ( link_file_path_length + 1 ) * sizeof( WCHAR );

      LPVOID context = nullptr;

      WIN32_STREAM_ID stream;

      ZeroMemory( &stream, sizeof( stream ) );

      stream.dwStreamId         = BACKUP_LINK;
      stream.dwStreamAttributes = 0;
      stream.dwStreamNameSize   = 0;
      stream.Size.QuadPart      = number_of_bytes_in_path;

      DWORD stream_header_size      = (DWORD) ( (LPBYTE) &stream.cStreamName - (LPBYTE) &stream + stream.dwStreamNameSize );
      DWORD number_of_bytes_written = 0;

      if ( BackupWrite( existing_file_handle,
              (LPBYTE) &stream,
                        stream_header_size,
                       &number_of_bytes_written,
                        FALSE,
                        FALSE,
                       &context ) == FALSE )
      {
         //WFCTRACEERROR( GetLastError() );

         (void) Win32FoundationClasses::wfc_close_handle( existing_file_handle );
         return( false );
      }

      ASSERT( number_of_bytes_written == stream_header_size );
      number_of_bytes_written = 0;

      if ( BackupWrite( existing_file_handle,
               (LPBYTE) link_file_path,
                        number_of_bytes_in_path,
                       &number_of_bytes_written,
                        FALSE,
                        FALSE,
                       &context ) == FALSE )
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACEVAL( TEXT( "At line number " ), __LINE__ );

         (void) Win32FoundationClasses::wfc_close_handle( existing_file_handle );
         return( false );
      }

      ASSERT( number_of_bytes_written == number_of_bytes_in_path );
      number_of_bytes_written = 0;

      if ( BackupWrite( existing_file_handle,
                        nullptr,
                        0,
                       &number_of_bytes_written,
                        TRUE,
                        FALSE,
                       &context ) == FALSE )
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACEVAL( TEXT( "At line number " ), __LINE__ );

         (void) Win32FoundationClasses::wfc_close_handle( existing_file_handle );
         return( false );
      }

      (void) Win32FoundationClasses::wfc_close_handle( existing_file_handle );
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
<TITLE>WFC - wfc_create_hard_link</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that encapsulates CloseHandle(). It will spit out a debugging message if CloseHandle() fails.">
</HEAD>

<BODY>

<H1>wfc_create_hard_link</H1>
$Revision: 8 $<HR>

<H2>Declaration</H2>
<PRE>BOOL <B>wfc_create_hard_link</B>( LPCTSTR new_filename, LPCTSTR existing_filename, LPSECURITY_ATTRIBUTES sa )</PRE>

<H2>Description</H2>
This function creates two directory entries for one file.
NT 5.0 will have an API called <CODE>CreateHardLink()</CODE>
that does the same thing.

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   if ( number_of_command_line_arguments &lt; 2 )
   {
      _tprintf( TEXT( &quot;Usage: ln new_filename existing_filename\n&quot; ) );
      return( EXIT_SUCCESS );
   }

   if ( <B>wfc_create_hard_link</B>( command_line_arguments[ 1 ],
                                 command_line_arguments[ 2 ],
                                 nullptr ) == FALSE )
   {
      _tprintf( TEXT( &quot;Failed.\n&quot; ) );
      return( EXIT_FAILURE );
   }

   return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_create_hard_link</B>() uses the following API's:
<UL>
<LI>BackupWrite
<LI>CreateFile
<LI>GetFullPathNameW
<LI>ZeroMemory
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_create_hard_link.cpp $<BR>
$Modtime: 6/26/01 10:58a $
</BODY>

</HTML>
*/
