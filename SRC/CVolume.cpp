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
** $Workfile: CVolume.cpp $
** $Revision: 10 $
** $Modtime: 6/26/01 10:53a $
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

USING_WFC_NAMESPACE

// This is a C++ class based on the Microsoft Knowledge Base article Q165721

CVolume::CVolume()
{
   WFC_VALIDATE_POINTER( this );
   m_Handle      = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
   m_DriveLetter = 0;
   m_DriveType   = DRIVE_UNKNOWN;
}

CVolume::~CVolume()
{
   WFC_VALIDATE_POINTER( this );
   (void) Close();
}

_Check_return_ bool CVolume::AutomaticallyEject( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      //WFCTRACE( TEXT( "You must first call Open()." ) );
      return( false );
   }

   DWORD number_of_bytes_returned = 0;

   const BOOL return_value = ::DeviceIoControl( m_Handle,
                                     IOCTL_STORAGE_EJECT_MEDIA,
                                     nullptr,
                                     0,
                                     nullptr,
                                     0,
                                    &number_of_bytes_returned,
                                     nullptr );

   return( return_value == FALSE ? false : true );
}

_Check_return_ bool CVolume::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_Name.clear();
   m_DriveLetter = 0;
   m_DriveType   = DRIVE_UNKNOWN;

   if ( m_Handle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      (void) Win32FoundationClasses::wfc_close_handle( m_Handle );
   }

   m_Handle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );

   return( true );
}

_Check_return_ bool CVolume::Dismount( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      return( false );
   }

   DWORD number_of_bytes_returned = 0;

   const BOOL return_value = ::DeviceIoControl( m_Handle,
                                     FSCTL_DISMOUNT_VOLUME,
                                     nullptr,
                                     0,
                                     nullptr,
                                     0,
                                    &number_of_bytes_returned,
                                     nullptr );

   return( return_value == FALSE ? false : true );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CVolume::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CVolume at " ) << (VOID *) this << TEXT( "\n{\n" );

   dump_context << TEXT( "   m_Handle is " );

   if ( m_Handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      dump_context << TEXT( "INVALID_HANDLE_VALUE\n" );
   }
   else
   {
      dump_context << m_Handle << TEXT( "\n" );
   }

   dump_context << TEXT( "   m_DriveType is " );

   switch( m_DriveType )
   {
      case DRIVE_UNKNOWN:

         dump_context << TEXT( "DRIVE_UNKNOWN\n" );
         break;

      case DRIVE_NO_ROOT_DIR:

         dump_context << TEXT( "DRIVE_NO_ROOT_DIR\n" );
         break;

      case DRIVE_REMOVABLE:

         dump_context << TEXT( "DRIVE_REMOVABLE\n" );
         break;

      case DRIVE_FIXED:

         dump_context << TEXT( "DRIVE_FIXED\n" );
         break;

      case DRIVE_REMOTE:

         dump_context << TEXT( "DRIVE_REMOTE\n" );
         break;

      case DRIVE_CDROM:

         dump_context << TEXT( "DRIVE_CDROM\n" );
         break;

      case DRIVE_RAMDISK:

         dump_context << TEXT( "DRIVE_RAMDISK\n" );
         break;

      default:

         dump_context << TEXT( "unknown( " ) << m_DriveType << TEXT( ")\n" );
   }

   dump_context << TEXT( "   m_Name is " ) << m_Name << TEXT( "\n" );
   dump_context << TEXT( "   m_DriveLetter is " ) << m_DriveLetter << TEXT( "\n" );

   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ bool CVolume::Eject( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      return( false );
   }

   if ( Lock() == false )
   {
      //WFCTRACEERROR( ::GetLastError() );
      return( false );
   }

   if ( Dismount() == false )
   {
      //WFCTRACEERROR( ::GetLastError() );
      return( false );
   }

   if ( PreventRemoval( false ) == false )
   {
      //WFCTRACEERROR( ::GetLastError() );
      return( false );
   }

   if ( AutomaticallyEject() == false )
   {
      //WFCTRACEERROR( ::GetLastError() );
      return( false );
   }

   return( true );
}

_Check_return_ HANDLE CVolume::GetHandle( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_Handle );
}

_Check_return_ bool CVolume::GetType( __out UINT& drive_type ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      drive_type = DRIVE_UNKNOWN;
      return( false );
   }

   drive_type = m_DriveType;

   return( true );
}

_Check_return_ bool CVolume::Load( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      //WFCTRACE( TEXT( "You must first call Open()." ) );
      return( false );
   }

   DWORD number_of_bytes_returned = 0;

   const BOOL return_value = ::DeviceIoControl( m_Handle,
                                     IOCTL_STORAGE_LOAD_MEDIA,
                                     nullptr,
                                     0,
                                     nullptr,
                                     0,
                                    &number_of_bytes_returned,
                                     nullptr );

   return( return_value == FALSE ? false : true );
}

_Check_return_ bool CVolume::Lock( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      //WFCTRACE( TEXT( "You must first call Open()." ) );
      return( false );
   }

   DWORD number_of_bytes_returned = 0;
   DWORD sleep_time               = 0;

   sleep_time = 10000 / 20; // Try 20 times in 10 seconds

   for ( auto const number_of_tries : Range(20) )
   {
      if ( ::DeviceIoControl( m_Handle,
                              FSCTL_LOCK_VOLUME,
                              nullptr,
                              0,
                              nullptr,
                              0,
                             &number_of_bytes_returned,
                              nullptr ) != FALSE )
      {
         return( true );
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
   }

   return( false );
}

_Check_return_ bool CVolume::Open( _In_ TCHAR const drive_letter ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Handle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      (void) Close();
   }

   std::wstring root_directory;

   format( root_directory, L"%c:\\", drive_letter );

   m_DriveType = ::GetDriveTypeW( root_directory.c_str() );

   DWORD access_flags = 0;

   switch( m_DriveType )
   {
      default:
      case DRIVE_REMOVABLE:

         access_flags = GENERIC_READ bitor GENERIC_WRITE;
         break;

      case DRIVE_CDROM:

         access_flags = GENERIC_READ;
         break;
   }

   format( m_Name, L"\\\\.\\%c:", drive_letter );
   m_DriveLetter = drive_letter;

   m_Handle = ::CreateFileW( m_Name.c_str(),
                            access_flags,
                            FILE_SHARE_READ bitor FILE_SHARE_WRITE,
                            nullptr,
                            OPEN_EXISTING,
                            0,
                            nullptr );

   if ( m_Handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      //WFCTRACEVAL( TEXT( "Can't open " ), m_Name );
      //WFCTRACEERROR( ::GetLastError() );

      m_DriveType   = DRIVE_UNKNOWN;
      m_DriveLetter = 0;
      m_Name.clear();

      return( false );
   }

   return( true );
}

_Check_return_ bool CVolume::PreventRemoval( __in bool const prevent_removal ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      return( false );
   }

   DWORD number_of_bytes_returned = 0;

   PREVENT_MEDIA_REMOVAL buffer;

   ::ZeroMemory( &buffer, sizeof( buffer ) );

   buffer.PreventMediaRemoval = (BOOLEAN) prevent_removal;

   const BOOL return_value = ::DeviceIoControl( m_Handle,
                                     IOCTL_STORAGE_MEDIA_REMOVAL,
                                    &buffer,
                                     sizeof( buffer ),
                                     nullptr,
                                     0,
                                    &number_of_bytes_returned,
                                     nullptr );

   return(return_value == FALSE ? false : true );
}

_Check_return_ bool CVolume::Unlock( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      return( false );
   }

   DWORD number_of_bytes_returned = 0;

   const BOOL return_value = ::DeviceIoControl( m_Handle,
                                     FSCTL_UNLOCK_VOLUME,
                                     nullptr,
                                     0,
                                     nullptr,
                                     0,
                                    &number_of_bytes_returned,
                                     nullptr );

   return( return_value == FALSE ? false : true );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CVolume</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles ejecting removable media, locking and dismounting volumes, etc.">
</HEAD>

<BODY>

<H1>CVolume</H1>
$Revision: 10 $<BR>
<HR>

<H2>Description</H2>
This class makes it easy to handle volumes.

<H2>Data Members</H2>
None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="AutomaticallyEject">AutomaticallyEject</A></B>( void )</PRE><DD>
Ejects the volume if it is not in use.

<DT><PRE>BOOL <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the handle to the volume.

<DT><PRE>BOOL <B><A NAME="Dismount">Dismount</A></B>( void )</PRE><DD>
Dismounts the volume.

<DT><PRE>BOOL <B><A NAME="Eject">Eject</A></B>( void )</PRE><DD>
OK, here's something useful. This method will call the
other methods in the correct order and have the media
spit out of the computer.

<DT><PRE>HANDLE <B><A NAME="GetHandle">GetHandle</A></B>( void ) const</PRE><DD>
Gives you the handle to the volume so you can call the
Win32 API yourself.

<DT><PRE>BOOL <B><A NAME="GetType">GetType</A></B>( UINT&amp; drive_type )</PRE><DD>
Tells you what type of volume you&#39;ve opened. It will return
one of the following (found in <CODE>winioctl.h</CODE>):
<UL>
<LI><CODE>DRIVE_UNKNOWN</CODE> - The drive type cannot be determined.
<LI><CODE>DRIVE_NO_ROOT_DIR</CODE> - The root directory does not exist.
<LI><CODE>DRIVE_REMOVABLE</CODE> - The disk can be removed from the drive.
<LI><CODE>DRIVE_FIXED</CODE> - The disk cannot be removed from the drive.
<LI><CODE>DRIVE_REMOTE</CODE> - The disk is a remote (networked) drive.
<LI><CODE>DRIVE_CDROM</CODE> - The drive is a CD-ROM.
<LI><CODE>DRIVE_RAMDISK</CODE> - The drive is a RAM disk.
</UL>

<DT><PRE>BOOL <B><A NAME="Load">Load</A></B>( void )</PRE><DD>
Loads the volume.

<DT><PRE>BOOL <B><A NAME="Lock">Lock</A></B>( void )</PRE><DD>
Locks the volume.

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( TCHAR drive_letter )</PRE><DD>
Opens a handle to the drive containing the volume.

<DT><PRE>BOOL <B><A NAME="PreventRemoval">PreventRemoval</A></B>( BOOL prevent_removal = TRUE )</PRE><DD>
Tells NT whether or not you want to prevent the removal
of the volume.

<DT><PRE>BOOL <B><A NAME="Unlock">Unlock</A></B>( void )</PRE><DD>
Unlocks the volume.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

void eject_zip_cartridge( TCHAR zip_drive_letter )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;eject_zip_cartridge()&quot; ) );

   <B>CVolume</B> zip_volume;

   if ( zip_volume.Open( zip_drive_letter ) != FALSE )
   {
      zip_volume.Eject();
   }
}</CODE></PRE>

<H2>API's Used</H2>
<UL>
<LI>CreateFile
<LI>DeviceIoControl
<LI>GetDriveType
<LI>GetLastError
<LI>Sleep
<LI>ZeroMemory
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CVolume.cpp $<BR>
$Modtime: 6/26/01 10:53a $
</BODY>

</HTML>
*/
