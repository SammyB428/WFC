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
** $Workfile: wfc_get_directory_usage_percentage.cpp $
** $Revision: 12 $
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

_Check_return_ uint32_t Win32FoundationClasses::wfc_get_directory_usage_percentage( _In_ std::wstring const& directory_path ) noexcept
{
   double number_of_bytes_free_on_drive     = 0.0;
   double number_of_bytes_used_in_directory = 0.0;
   double high_value                        = static_cast< double >( 0xFFFFFFFF );

   high_value++;

   std::wstring mask( directory_path );

   if ( mask.at( mask.length() - 1 ) != '\\' and
        mask.at( mask.length() - 1 ) != '/' )
   {
      mask.push_back( '/' );
   }

   mask.append(WSTRING_VIEW(L"*.*"));

   WIN32_FIND_DATA find_data;

   ZeroMemory( &find_data, sizeof( find_data ) );

   auto find_file_handle = FindFirstFileW( mask.c_str(), &find_data );

   if ( find_file_handle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      number_of_bytes_used_in_directory += find_data.nFileSizeLow;

      if ( find_data.nFileSizeHigh != 0 )
      {
         number_of_bytes_used_in_directory += static_cast< double >( static_cast< double >( find_data.nFileSizeHigh ) * high_value );
      }

      while( FindNextFileW( find_file_handle, &find_data ) != FALSE )
      {
         number_of_bytes_used_in_directory += find_data.nFileSizeLow;

         if ( find_data.nFileSizeHigh != 0 )
         {
            number_of_bytes_used_in_directory += static_cast< double >( static_cast< double >( find_data.nFileSizeHigh ) * high_value );
         }
      }

      FindClose( find_file_handle );
   }
   else
   {
      return( 0 );
   }

   // Strip off the "/*.*" from the directory name

   mask.erase(mask.length() - 4);

   ULARGE_INTEGER number_of_free_bytes_on_disk{ 0, 0 };
   ULARGE_INTEGER total_number_of_bytes_on_disk{ 0, 0 };

   if ( GetDiskFreeSpaceExW( mask.c_str(),
                           &number_of_free_bytes_on_disk,
                           &total_number_of_bytes_on_disk,
                            nullptr ) == FALSE )
   {
      //WFCTRACE( TEXT( "Failure!" ) );
      //WFCTRACEERROR( GetLastError() );
      return( 0 );
   }

   number_of_bytes_free_on_drive  = number_of_free_bytes_on_disk.LowPart;
   number_of_bytes_free_on_drive += static_cast< double >( static_cast< double >( number_of_free_bytes_on_disk.HighPart ) * high_value );

   double percentage = 0.0;

   percentage = number_of_bytes_used_in_directory / number_of_bytes_free_on_drive;
   percentage *= 100.0;

   return( static_cast< uint32_t >( percentage ) );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_get_directory_usage_percentage</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that computes how much room on a disk a directory is using (non-recursive).">
</HEAD>

<BODY>

<H1>wfc_get_directory_usage_percentage</H1>
$Revision: 12 $<HR>

<H2>Declaration</H2>
<PRE><CODE>DWORD wfc_get_directory_usage_percentage( const std::wstring&amp; directory_name )</CODE></PRE>

<H2>Description</H2>
This function tells you how much space is used by
the specified directory. It takes into account quotas and
distributed file systems (DFS). If the function fails, it will
return zero.

<H2>Example</H2>

<PRE><CODE>void check_home_directory_usage( const std::wstring&amp; home_directory )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;check_home_directory_usage()&quot; ) );

   double percent_used = <B>wfc_get_directory_usage_percentage</B>( home_directory );

   if ( percent_used &gt;= 85.0 )
   {
      _tprintf( &quot;%s is %lf full\n&quot;, (LPCTSTR) home_directory, percent_used );
   }
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_get_directory_usage_percentage</B>() uses the following API's:
<UL>
<LI>FindClose
<LI>FindFirstFile
<LI>FindNextFile
<LI>GetDiskFreeSpaceEx
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_get_directory_usage_percentage.cpp $<BR>
$Modtime: 6/26/01 10:58a $
</BODY>

</HTML>
*/
