/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2022, Samuel R. Blackburn
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
** $Workfile: wfc_close_handle.cpp $
** $Revision: 14 $
** $Modtime: 6/26/01 10:57a $
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

_Check_return_ bool Win32FoundationClasses::wfc_close_handle( __in_opt HANDLE handle ) noexcept
{
    if (handle == 0 or handle == INVALID_HANDLE_VALUE)
    {
        return(true);
    }

   try
   {
      // CloseHandle will throw an exception.... DOH!
       return(CloseHandle(handle) == FALSE ? false : true);
   }
   catch( ... )
   {
   }

   return( false );
}

_Check_return_ SECURITY_DESCRIPTOR * Win32FoundationClasses::wfc_create_null_dacl( void ) noexcept
{
   // We need to create a file that *ANY* process on the system can
   // open. To do this we, we need to create the file with a NULL DACL.
   // Please read Knowledge Base article Q106387.

   auto security_descriptor_p{ static_cast<SECURITY_DESCRIPTOR*>(::LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH)) };

   if ( security_descriptor_p == nullptr )
   {
      // Oh well, hope for the best...
      //WFCTRACEERROR( GetLastError() );
      //WFCTRACE( TEXT( "Can't allocate memory for security descriptor." ) );
      return( nullptr );
   }

   if ( ::InitializeSecurityDescriptor( security_descriptor_p, SECURITY_DESCRIPTOR_REVISION ) == FALSE )
   {
      // Oh well, hope for the best...
      //WFCTRACEERROR( GetLastError() );
      //WFCTRACE( TEXT( "Can't initialize security descriptor." ) );
      ::LocalFree( (HLOCAL) security_descriptor_p );
      return( nullptr );
   }

   ASSERT( ::IsValidSecurityDescriptor( security_descriptor_p ) not_eq FALSE );

   if ( ::IsValidSecurityDescriptor( security_descriptor_p ) == FALSE )
   {
      // Oh well, hope for the best...
      //WFCTRACEERROR( GetLastError() );
      //WFCTRACE( TEXT( "Invalid security descriptor just after creating it." ) );
      LocalFree( (HLOCAL) security_descriptor_p );
      return( nullptr );
   }

   if ( ::SetSecurityDescriptorDacl( security_descriptor_p,
                                     TRUE,
                             (ACL *) nullptr, // This is the "NULL DACL"
                                     FALSE ) == FALSE )
   {
      // Oh well, hope for the best...
      //WFCTRACEERROR( GetLastError() );
      //WFCTRACE( TEXT( "Can't set security descriptor DACL." ) );
      ::LocalFree( (HLOCAL) security_descriptor_p );
      return( nullptr );
   }

   ASSERT( ::IsValidSecurityDescriptor( security_descriptor_p ) not_eq FALSE );

   if ( ::IsValidSecurityDescriptor( security_descriptor_p ) == FALSE )
   {
      // Oh well, hope for the best...
      //WFCTRACEERROR( GetLastError() );
      //WFCTRACE( TEXT( "Invalid security descriptor just after setting NULL DACL." ) );
      ::LocalFree( (HLOCAL) security_descriptor_p );
      return( nullptr );
   }

   return ( security_descriptor_p );
}

void Win32FoundationClasses::wfc_destroy_null_dacl( __in_opt void * null_dacl ) noexcept
{
   if ( null_dacl not_eq nullptr )
   {
      ::LocalFree( (HLOCAL) null_dacl );
   }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_close_handle</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that encapsulates CloseHandle(). It will spit out a debugging message if CloseHandle() fails.">
</HEAD>

<BODY>

<H1>wfc_close_handle</H1>
$Revision: 14 $<HR>

<H2>Declaration</H2>
<PRE>BOOL <B>wfc_close_handle</B>( HANDLE handle )</PRE>

<H2>Description</H2>
This function returns TRUE if the handle was successfully closed
using the <B>CloseHandle</B>() Win32 API.

<H2>Example</H2>

<PRE><CODE>int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   if ( number_of_command_line_arguments &lt; 2 )
   {
      return( EXIT_SUCCESS );
   }

   TCHAR physical_disk[ MAX_PATH ];

   ZeroMemory( physical_disk, sizeof( physical_disk ) );

   _stprintf( physical_disk, TEXT( &quot;\\\\.\\PHYSICALDRIVE%u&quot; ), _ttoi( command_line_arguments[ 1 ] ) );

   auto disk_handle = static_cast< HANDLE >( NULL );

   disk_handle = CreateFile( physical_disk,
                             GENERIC_READ bitor GENERIC_WRITE,
                             0,
                             nullptr,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,
                             0 );

   if ( disk_handle not_eq static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      DISK_GEOMETRY disk_geometry;

      ZeroMemory( &amp;disk_geometry, sizeof( disk_geometry ) );

      DWORD number_of_bytes_read = 0;

      if ( DeviceIoControl( disk_handle,
                            IOCTL_DISK_GET_DRIVE_GEOMETRY,
                            nullptr,
                            0,
                           &amp;disk_geometry,
                            sizeof( disk_geometry ),
                           &amp;number_of_bytes_read,
                            0 ) not_eq FALSE )
      {
         _tprintf( TEXT( &quot;Number of Cylinders (low)     %lu\n&quot; ), disk_geometry.Cylinders.LowPart  );
         _tprintf( TEXT( &quot;Number of Cylinders (high)    %lu\n&quot; ), disk_geometry.Cylinders.HighPart );
         _tprintf( TEXT( &quot;Number of Tracks per Cylinder %lu\n&quot; ), disk_geometry.TracksPerCylinder  );
         _tprintf( TEXT( &quot;Number of Sectors per Track   %lu\n&quot; ), disk_geometry.SectorsPerTrack    );
         _tprintf( TEXT( &quot;Number of Bytes per Sector    %lu\n&quot; ), disl_geometry.BytesPerSector     );
      }

      <B>wfc_close_handle</B>( disk_handle );
   }

   return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_close_handle</B>() uses the following API's:
<UL>
<LI>CloseHandle
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_close_handle.cpp $<BR>
$Modtime: 6/26/01 10:57a $
</BODY>

</HTML>
*/
