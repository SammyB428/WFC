/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2019, Samuel R. Blackburn
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
** $Workfile: CPhysicalDiskFile.cpp $
** $Revision: 15 $
** $Modtime: 6/26/01 10:50a $
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

Win32FoundationClasses::CPhysicalDiskFile::CPhysicalDiskFile() noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_Buffer       = nullptr;
   m_BufferSize   = 0;
   m_BufferOffset = 0;

   ::ZeroMemory( &m_DiskGeometry, sizeof( m_DiskGeometry ) );
}

Win32FoundationClasses::CPhysicalDiskFile::~CPhysicalDiskFile() noexcept
{
   WFC_VALIDATE_POINTER( this );

   Close();

   WFC_VALIDATE_POINTER_NULL_OK( m_Buffer );

   if ( m_Buffer not_eq nullptr )
   {
       _aligned_free( m_Buffer );
   }

   m_Buffer       = nullptr;
   m_BufferSize   = 0;
   m_BufferOffset = 0;

   ::ZeroMemory( &m_DiskGeometry, sizeof( m_DiskGeometry ) );
}

void Win32FoundationClasses::CPhysicalDiskFile::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_FileHandle not_eq reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) )
   {
       Win32FoundationClasses::CFile64::Close();
   }

   if ( m_Buffer not_eq nullptr )
   {
      _aligned_free( m_Buffer );
   }

   m_Buffer       = nullptr;
   m_BufferSize   = 0;
   m_BufferOffset = 0;

   ::ZeroMemory( &m_DiskGeometry, sizeof( m_DiskGeometry ) );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CPhysicalDiskFile::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   dump_context << TEXT( "a CPhysicalDiskFile at " ) << (VOID *) this << TEXT( "\n{\n" );

   CFile::Dump( dump_context );

   dump_context << "   m_DiskGeometry.MediaType is ";

   // These are taken from WINIOCTL.H

   switch( m_DiskGeometry.MediaType )
   {
      case Unknown:

         dump_context << TEXT( "Unknown (Format is Unknown)\n" );
         break;

      case F5_1Pt2_512:

         dump_context << TEXT( "F5_1Pt2_512 (5.25\", 1.2MB,  512 bytes per sector)\n" );
         break;

      case F3_1Pt44_512:

         dump_context << TEXT( "F3_1Pt44_512 (3.5\",  1.44MB, 512 bytes per sector)\n" );
         break;

      case F3_2Pt88_512:

         dump_context << TEXT( "F3_2Pt88_512 (3.5\",  2.88MB, 512 bytes per sector )\n" );
         break;

      case F3_20Pt8_512:

         dump_context << TEXT( "F3_20Pt8_512 (3.5\",  20.8MB, 512 bytes per sector)\n" );
         break;

      case F3_720_512:

         dump_context << TEXT( "F3_720_512 (3.5\",  720KB,  512 bytes per sector)\n" );
         break;

      case F5_360_512:

         dump_context << TEXT( "F5_360_512 (5.25\", 360KB,  512 bytes per sector)\n" );
         break;

      case F5_320_512:

         dump_context << TEXT( "F5_320_512 (5.25\", 320KB,  512 bytes/sector)\n" );
         break;

      case F5_320_1024:

         dump_context << TEXT( "F5_320_1024 (5.25\", 320KB,  1024 bytes/sector)\n" );
         break;

      case F5_180_512:

         dump_context << TEXT( "F5_180_512 (5.25\", 180KB,  512 bytes per sector)\n" );
         break;

      case F5_160_512:

         dump_context << TEXT( "F5_160_512 (5.25\", 160KB,  512 bytes/sector)\n" );
         break;

      case RemovableMedia:

         dump_context << TEXT( "RemovableMedia (Removable media other than floppy)\n" );
         break;

      case FixedMedia:

         dump_context << TEXT( "FixedMedia (Fixed hard disk media)\n" );
         break;

      case F3_120M_512:

         dump_context << TEXT( "F3_120M_512 (3.5\", 120M Floppy)\n" );
         break;

      default:

         dump_context << TEXT( "Undocumented format has a value of " ) << m_DiskGeometry.MediaType << TEXT( "\n" );
         break;
   }
}

#endif // _DEBUG

_Check_return_ uint64_t Win32FoundationClasses::CPhysicalDiskFile::GetLength( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   ULARGE_INTEGER total_number_of_bytes;

   GetLength( total_number_of_bytes );

   return( total_number_of_bytes.QuadPart );
}

void Win32FoundationClasses::CPhysicalDiskFile::GetLength( _Out_ ULARGE_INTEGER& total_number_of_bytes ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   total_number_of_bytes.QuadPart = 0;

   total_number_of_bytes.QuadPart  = (int64_t) ( (int64_t) m_DiskGeometry.BytesPerSector * (int64_t) m_DiskGeometry.SectorsPerTrack );
   total_number_of_bytes.QuadPart *= (int64_t) m_DiskGeometry.TracksPerCylinder;
   total_number_of_bytes.QuadPart *= (int64_t) m_DiskGeometry.Cylinders.QuadPart;
}

_Check_return_ DWORD Win32FoundationClasses::CPhysicalDiskFile::GetMediaType( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_DiskGeometry.MediaType );
}

_Check_return_ bool Win32FoundationClasses::CPhysicalDiskFile::GetSectorSize( _Out_ std::size_t& number_of_bytes_per_sector ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_FileHandle == reinterpret_cast< CFILE_HFILE >( INVALID_HANDLE_VALUE ) )
   {
      number_of_bytes_per_sector = 0;
      return( false );
   }
   else
   {
      number_of_bytes_per_sector = m_DiskGeometry.BytesPerSector;
      return( true );
   }
}

_Check_return_ bool Win32FoundationClasses::CPhysicalDiskFile::m_SetSectorSize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ASSERT( m_FileHandle not_eq reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) );

   if ( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      return( false );
   }

   ::ZeroMemory( &m_DiskGeometry, sizeof( m_DiskGeometry ) );

   DWORD number_of_bytes_read = 0;

   if ( ::DeviceIoControl( m_FileHandle,
                           IOCTL_DISK_GET_DRIVE_GEOMETRY,
                           nullptr,
                           0,
                          &m_DiskGeometry,
                           sizeof( m_DiskGeometry ),
                          &number_of_bytes_read,
                           nullptr ) == FALSE )
   {
       m_LastError = ::GetLastError();
      //WFCTRACEERROR( ::GetLastError() );

      Close();

      return( false );
   }

   // We must read and write on sector boundaries

   ASSERT( m_DiskGeometry.BytesPerSector not_eq 0 );

   if ( m_DiskGeometry.BytesPerSector == 0 )
   {
      Close();
      return( false );
   }

   m_BufferSize  = ( 2 * 1024 * 1024 ); // Two Megabytes
   m_BufferSize += m_DiskGeometry.BytesPerSector; // This makes sure we have a two meg buffer

   ASSERT( m_Buffer == nullptr );

   m_Buffer = static_cast< uint8_t *>(_aligned_malloc(m_BufferSize, 65536));

   if ( m_Buffer == nullptr )
   {
      //WFCTRACEERROR( ::GetLastError() );

      Close();

      return( false );
   }

   // Now we need to see if the address of the buffer sits on a sector boundary

   m_BufferOffset = 0;

   // This will probably break when we go to a 64-bit memory address space

   while( (DWORD)( (uint64_t) (&m_Buffer[ m_BufferOffset ]) % m_DiskGeometry.BytesPerSector ) not_eq 0 )
   {
      m_BufferOffset++;
   }

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CPhysicalDiskFile::Open(_In_ std::filesystem::path const& drive_letter, _In_ UINT const) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ASSERT( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) );

   if ( m_FileHandle not_eq reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      Close();
   }

   if (drive_letter.empty() == true)
   {
       return(false);
   }

   std::wstring filename;

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      format( filename, L"\\\\.\\%c:", drive_letter.c_str()[ 0 ] );

      m_FileHandle = ::CreateFileW( filename.c_str(),
                                   GENERIC_READ bitor GENERIC_WRITE,
                                   FILE_SHARE_READ bitor FILE_SHARE_WRITE,
                                   nullptr,
                                   OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL bitor FILE_FLAG_WRITE_THROUGH bitor FILE_FLAG_NO_BUFFERING,
                                   reinterpret_cast< HANDLE >( NULL ) );

      if ( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
      {
          m_LastError = ::GetLastError();
         //WFCTRACEERROR( ::GetLastError() );

         // Clean up any memory allocations and return to a known state

         Close();

         return( false );
      }

      if (m_SetSectorSize() == false )
      {
         return( false );
      }

      return( true );
   }
   WFC_CATCH_ALL
   {
   }
   WFC_END_CATCH_ALL

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CPhysicalDiskFile::Open( _In_ int const physical_disk_number, _In_ UINT const) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ASSERT( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) );

   if ( m_FileHandle not_eq reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      Close();
   }

   std::wstring filename;

   format( filename, L"\\\\.\\PhysicalDrive%d", physical_disk_number );

   m_FileHandle = ::CreateFileW( filename.c_str(),
                                GENERIC_READ bitor GENERIC_WRITE,
                                0,
                                nullptr,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL bitor FILE_FLAG_WRITE_THROUGH bitor FILE_FLAG_NO_BUFFERING,
                                reinterpret_cast< HANDLE >( NULL ) );

   if ( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
       m_LastError = ::GetLastError();
      //WFCTRACEERROR( ::GetLastError() );

      // Clean up any memory allocations and return to a known state

      Close();

      return( false );
   }

   if ( m_SetSectorSize() == false )
   {
      return( false );
   }

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CPhysicalDiskFile::OpenRead(_In_z_ wchar_t const * physical_drive_name) noexcept
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER(physical_drive_name);

    ASSERT(m_FileHandle == reinterpret_cast< HANDLE >(INVALID_HANDLE_VALUE));

    if (m_FileHandle not_eq reinterpret_cast< HANDLE >(INVALID_HANDLE_VALUE))
    {
        Close();
    }

    m_FileHandle = ::CreateFileW(physical_drive_name,
        GENERIC_READ,
        FILE_SHARE_READ bitor FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL bitor FILE_FLAG_NO_BUFFERING,
        reinterpret_cast< HANDLE >(NULL));

    if (m_FileHandle == reinterpret_cast< HANDLE >(INVALID_HANDLE_VALUE))
    {
        m_LastError = ::GetLastError();
        //WFCTRACEERROR( ::GetLastError() );

        // Clean up any memory allocations and return to a known state

        Close();

        return(false);
    }

    if (m_SetSectorSize() == false)
    {
        return(false);
    }

    return(true);
}

_Check_return_ bool Win32FoundationClasses::CPhysicalDiskFile::OpenRead(_In_ int const physical_disk_number ) noexcept
{
    WFC_VALIDATE_POINTER(this);

    std::wstring filename;

    format(filename, L"\\\\.\\PhysicalDrive%d", physical_disk_number);

    return(OpenRead(filename.c_str()));
}

_Check_return_ UINT Win32FoundationClasses::CPhysicalDiskFile::Read( __out_bcount( count ) void * buffer, _In_ UINT const count ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( buffer );

   ASSERT( m_FileHandle not_eq reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   ASSERT( m_Buffer not_eq nullptr );

   if ( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      return( 0 );
   }

   if ( count == 0 )
   {
      return( 0 );
   }

   WFC_VALIDATE_POINTER( buffer );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      UINT number_of_bytes_to_read = count;

      if ( count > ( m_BufferSize - m_BufferOffset ) )
      {
         number_of_bytes_to_read = m_BufferSize - m_BufferOffset;
      }

      const UINT number_of_bytes_actually_read = CFile64::Read( &m_Buffer[ m_BufferOffset ], number_of_bytes_to_read );

      ASSERT( number_of_bytes_actually_read == number_of_bytes_to_read );

      ::CopyMemory( buffer, &m_Buffer[ m_BufferOffset ], number_of_bytes_actually_read );

      return( number_of_bytes_actually_read );
   }
   WFC_CATCH_ALL
   {
      return( 0 );
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CPhysicalDiskFile::SetLength( _In_ uint64_t const) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( FALSE ); // Unsupported function
}

void Win32FoundationClasses::CPhysicalDiskFile::Write( __in_bcount( count ) void const * buffer, _In_ UINT const count ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( buffer );
   WFC_VALIDATE_POINTER( m_Buffer );

   ASSERT( m_FileHandle not_eq reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) );

   if ( m_FileHandle == reinterpret_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      return;
   }

   if ( count == 0 )
   {
      return;
   }

   WFC_VALIDATE_POINTER( buffer );

   // We were passed a pointer, don't trust it

   UINT number_of_bytes_to_write = 0;
   UINT number_of_bytes_written  = 0;

   WFC_TRY
   {
      while( number_of_bytes_written < count ) // Cannot be converted to a Range loop
      {
         number_of_bytes_to_write = count - number_of_bytes_written;

         if ( number_of_bytes_to_write > ( m_BufferSize - m_BufferOffset ) )
         {
            number_of_bytes_to_write = m_BufferSize - m_BufferOffset;
         }

         ::CopyMemory( &m_Buffer[ m_BufferOffset ], buffer, number_of_bytes_to_write );
         CFile64::Write( &m_Buffer[ m_BufferOffset ], number_of_bytes_to_write );

         number_of_bytes_written += number_of_bytes_to_write;
      }
   }
   WFC_CATCH_ALL
   {
      // Do Nothing
   }
   WFC_END_CATCH_ALL
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CPhysicalDiskFile</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that makes it easy to read raw disk sectors in Windows NT.">
</HEAD>

<BODY>

<H1>CPhysicalDiskFile : CFile</H1>

$Revision: 15 $<BR>

<HR>

<H2>Description</H2>
This class makes it easy to read physical sectors on a disk.

<H2>Data Members</H2>

None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>Closes the disk.

<DT><PRE>DWORD <B><A NAME="GetLength">GetLength</A></B>( void ) const
void <B>GetLength</B>( ULARGE_INTEGER&amp; size ) const</PRE><DD>
Returns the total number of bytes in the disk. If (and this is
quite likely) your disk contains more that 4GB, <B>GetLength</B>()
will return 4GB. 4GB is the maximum value of a DWORD. For safety, you
should use the ULARGE_INTEGER version of this function.

<DT><PRE>DWORD <B><A NAME="GetMediaType">GetMediaType</A></B>( void ) const</PRE><DD>
Returns the type of media that has been opened.

<DT><PRE>BOOL <B><A NAME="GetSectorSize">GetSectorSize</A></B>( DWORD&amp; number_of_bytes_per_sector ) const</PRE><DD>
Return TRUE if the disk has been <B>Open</B>()'d or FALSE if <B>Open</B>()
has not yet been called. It will fill <CODE>number_of_bytes_per_sector</CODE>
with the number of bytes per sector on the disk on success.

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( LPCTSTR drive_letter, UINT open_flags, CFileException * error = nullptr )
BOOL <B>Open</B>( int physical_disk_number, UINT open_flags, CFileException * error = nullptr )</PRE><DD>
Opens the disk. The <CODE>open_flags</CODE> and <CODE>error</CODE>
parameters are ignored.

<DT><PRE>UINT <B><A NAME="Read">Read</A></B>( void * buffer, UINT number_of_bytes_to_read )</PRE><DD>
Reads data from the disk. The class takes care of all that memory
boundary on a page size stuff.

<DT><PRE>void <B><A NAME="SetLength">SetLength</A></B>( DWORD length )</PRE><DD>
This function is intercepted. In other words, it does nothing.

<DT><PRE>void <B><A NAME="Write">Write</A></B>( void * buffer, UINT number_of_bytes_to_write )</PRE><DD>
Writes data to the disk.

</DL>

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

BOOL copy_diskette_to_file( CFile&amp; file )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;copy_diskette_to_file()&quot; ) );

   <B>CPhysicalDiskFile</B> disk;

   if ( disk.Open( &quot;A:&quot;, 0 ) == false )
   {
      return( FALSE );
   }

   std::vector&lt;uint8_t&gt; disk_contents;

   DWORD length = disk.GetLength();

   disk_contents.SetSize( length );

   disk.Read( disk_contents.GetBuffer(), disk_contents.GetSize() );

   // disk_contents now contains all sectors on the diskette

   file.Write( disk_contents.GetBuffer(), disk_contents.GetSize() );

   return( TRUE );
}

void get_sector( TCHAR drive_letter, DWORD sector_number, std::vector&lt;uint8_t&gt;&amp; sector_contents )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;get_sector()&quot; ) );

   sector_contents.Empty();

   TCHAR filename[ 3 ];

   filename[ 0 ] = drive_letter;
   filename[ 1 ] = TEXT( ':' );
   filename[ 2 ] = 0x00;

   <B>CPhysicalDiskFile</B> disk;

   if ( disk.Open( filename, 0 ) == false )
   {
      return;
   }

   DWORD sector_size = 0;

   disk.GetSectorSize( sector_size );

   DWORD location_to_seek_to = sector_size * sector_number;

   if ( ( location_to_seek_to + sector_size ) &gt; disk.GetLength() )
   {
      return;
   }

   disk.Seek( location_to_seek_to, <A HREF="CFile64.htm">CFile64</A>::begin );

   sector_contents.SetSize( sector_size );

   disk.Read( sector_contents.GetData(), sector_size );
}</CODE></PRE>

<H2>API's Used</H2>

<B>CNetworkUsers</B> uses the following API's:

<UL>
<LI>CopyMemory
<LI>CreateFile
<LI>DeviceIoControl
<LI>ZeroMemory
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CPhysicalDiskFile.cpp $<BR>
$Modtime: 6/26/01 10:50a $
</BODY>

</HTML>
*/
