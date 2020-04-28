/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
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
** $Workfile: dumyfile.cpp $
** $Revision: 28 $
** $Modtime: 6/26/01 10:53a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CDummyFile::CDummyFile()
{
   WFC_VALIDATE_POINTER( this );

   m_LastError = ERROR_SUCCESS;
   m_TemplateHandle = reinterpret_cast< HANDLE >( NULL );

#if defined( WFC_STL )
   m_FileHandle = static_cast< HANDLE >( NULL );
#endif // WFC_STL
}

Win32FoundationClasses::CDummyFile::~CDummyFile()
{
   WFC_VALIDATE_POINTER( this );

   Close();

   m_LastError = ERROR_SUCCESS;
   m_TemplateHandle = reinterpret_cast< HANDLE >( NULL );
#if defined( WFC_STL )
   m_FileHandle = static_cast< HANDLE >( NULL );
#endif // WFC_STL
}

void Win32FoundationClasses::CDummyFile::Abort( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Close();
}

void Win32FoundationClasses::CDummyFile::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   /*
   ** This is here to trap calls that attempt to close an already closed file.
   ** I don't consider this an error but MFC does...
   */

   if ( GetHandle() not_eq static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      CFile64::Close();
   }
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CDummyFile::Dump( CDumpContext& dump_context ) const
{
   CFile::Dump( dump_context );

   dump_context << TEXT( "m_LastError      = " ) << m_LastError      << TEXT( "\n" );
   dump_context << TEXT( "m_TemplateHandle = " ) << m_TemplateHandle << TEXT( "\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CDummyFile::Flush( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
}

_Check_return_ uint64_t Win32FoundationClasses::CDummyFile::GetLength( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( 0 );
}

_Check_return_ uint64_t Win32FoundationClasses::CDummyFile::GetPosition( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( FALSE ); // Unsupported function
   return( 0 );
}

_Check_return_ bool Win32FoundationClasses::CDummyFile::LockRange( _In_ uint64_t const /* position */, _In_ uint64_t const /* number_of_bytes_to_lock */ ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( FALSE ); // Unsupported function
   return( false );
}

_Check_return_ DWORD Win32FoundationClasses::CDummyFile::m_CreateCreationDistribution( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( OPEN_EXISTING );
}

_Check_return_ DWORD Win32FoundationClasses::CDummyFile::m_CreateDesiredAccess( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( GENERIC_READ bitor GENERIC_WRITE );
}

_Check_return_ DWORD Win32FoundationClasses::CDummyFile::m_CreateFlagsAndAttributes( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( 0 );
}

_Check_return_ LPSECURITY_ATTRIBUTES Win32FoundationClasses::CDummyFile::m_CreateSecurityAttributes( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( nullptr );
}

_Check_return_ DWORD Win32FoundationClasses::CDummyFile::m_CreateShareMode( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( 0 );
}

_Check_return_ HANDLE Win32FoundationClasses::CDummyFile::m_CreateTemplateHandle( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_TemplateHandle );
}

#pragma warning( disable : 4100 )

_Check_return_ uint64_t Win32FoundationClasses::CDummyFile::Seek( _In_ int64_t const /* Offset */, _In_ SeekPosition const /* From */ ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( FALSE ); // Unsupported function
   return( 0L );
}

#pragma warning( default : 4100 )

#pragma warning( disable : 4100 )

void Win32FoundationClasses::CDummyFile::SetLength( _In_ uint64_t const /* length */ ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( FALSE ); // Unsupported function
}

#if ! defined( WFC_STL )

void Win32FoundationClasses::CDummyFile::SetStatus( __in_z LPCTSTR /* name */, _In_ CFileStatus const& /* status */ )
{
   WFC_VALIDATE_POINTER( this );
}

#endif // WFC_STL

void Win32FoundationClasses::CDummyFile::UnlockRange(_In_ uint64_t const /* position */, _In_ uint64_t const /* number_of_bytes_to_unlock */ ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( FALSE ); // Unsupported function
}

void Win32FoundationClasses::CDummyFile::Write( _In_ std::string_view string_to_write ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   CFile64::Write( static_cast<void const *>(string_to_write.data()), static_cast<UINT>(string_to_write.length()));
}

void Win32FoundationClasses::CDummyFile::Write(_In_ std::vector<uint8_t> const& data_to_write ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   UINT number_of_bytes_to_write = (UINT) data_to_write.size();

   // Well whaddya know, there's an undocumented yet "public" function
   // that let's us have quick access to the raw bytes...

   auto buffer = data_to_write.data();

   Win32FoundationClasses::CFile64::Write( static_cast<void const *>(buffer), number_of_bytes_to_write );
}

#pragma warning( default : 4100 )

// End of source

/*
<HTML>
<HEAD>
<TITLE>WFC - CDummyFile</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that intercepts calls to CFile that would blow up when your derived class isn't playing with data on a disk.">
</HEAD>

<BODY>

<H1>CDummyFile : CFile</H1>
$Revision: 28 $<BR>
<HR>

<H2>Description</H2>

This class intercepts all calls to functions in CFile that would blow up
if what your file HANDLE doesn't point to a file on a disk drive (like
a TCP/IP socket or the serial port). It also adds a couple of useful 
functions that should have been in CFile (IMHO).

<H2>Data Members</H2>

None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Abort">Abort</A></B>( void )</PRE><DD>Closes the file.

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>Closes the file.

<DT><PRE>void <B><A NAME="Flush">Flush</A></B>( void )</PRE><DD>Flushes file buffers.

<DT><PRE>DWORD <B><A NAME="GetErrorCode">GetErrorCode</A></B>( void ) const</PRE><DD>
Retrieves the error code if a function fails or returns FALSE.

<DT><PRE>DWORD <B><A NAME="GetLength">GetLength</A></B>( void ) const</PRE><DD>
Makes no sense if you don't have a disk file. It always returns zero.

<DT><PRE>DWORD <B><A NAME="GetPosition">GetPosition</A></B>( void ) const</PRE><DD>
Makes no sense. Always returns zero.

<DT><PRE>BOOL <B><A NAME="GetStatus">GetStatus</A></B>( CFileStatus&amp; status )
BOOL <B>GetStatus</B>( LPCTSTR filename, CFileStatus&amp; status )</PRE><DD>Makes no sense.

<DT><PRE>void <B><A NAME="LockRange">LockRange</A></B>( DWORD position, DWORD count )</PRE><DD>Makes no sense.

<DT><PRE>void <B><A NAME="Remove">Remove</A></B>( LPCTSTR filename )</PRE><DD>
Makes no sense. Always returns FALSE.

<DT><PRE>void <B><A NAME="Rename">Rename</A></B>( LPCTSTR old_filename, LPCTSTR new_filename )</PRE><DD>Makes no sense.

<DT><PRE>LONG <B><A NAME="Seek">Seek</A></B>( LONG offset, UINT from )</PRE><DD>
Makes no sense. Always returns zero.

<DT><PRE>void <B><A NAME="Serialize">Serialize</A></B>( CArchive&amp; archive )</PRE><DD>Saves/restores the object.

<DT><PRE>void <B><A NAME="SetLength">SetLength</A></B>( DWORD length )</PRE><DD>Makes no sense.

<DT><PRE>void <B><A NAME="SetStatus">SetStatus</A></B>( LPCTSTR name, CFileStatus&amp; status )</PRE><DD>Makes no sense.

<DT><PRE>void <B><A NAME="UnlockRange">UnlockRange</A></B>( DWORD position, DWORD count )</PRE><DD>Makes no sense.

<DT><PRE>void <B><A NAME="Write">Write</A></B>( const std::wstring&amp; string_to_write )
void <B>Write</B>( const std::vector&lt;uint8_t&gt;&amp; data_to_write )</PRE><DD>Adds capability to write a a std::vector&lt;uint8_t&gt;.

</DL>

<H2>Example</H2>
<PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: dumyfile.cpp $<BR>
$Modtime: 6/26/01 10:53a $
</BODY>

</HTML>
*/
