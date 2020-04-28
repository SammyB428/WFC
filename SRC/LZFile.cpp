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
** $Workfile: lzfile.cpp $
** $Revision: 31 $
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

Win32FoundationClasses::CLZFile::CLZFile() noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CLZFile::~CLZFile()
{
   WFC_VALIDATE_POINTER( this );
   Close();
}

void Win32FoundationClasses::CLZFile::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_LZFileHandle >= 0 )
   {
      ::LZClose( m_LZFileHandle );
      m_LZFileHandle = LZERROR_BADINHANDLE;
#if defined( WFC_STL )
      m_FileHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
#else
      m_hFile = (CFILE_HFILE) CFile::hFileNull;
#endif
   }

   m_Initialize();
}

_Check_return_ bool Win32FoundationClasses::CLZFile::Copy( _In_ Win32FoundationClasses::CLZFile const * source_p ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source_p );
   return( Copy( *source_p ) );
}

_Check_return_ bool Win32FoundationClasses::CLZFile::Copy( _In_ Win32FoundationClasses::CLZFile const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   LONG size_of_destination_file = 0;

   size_of_destination_file = ::LZCopy( source.m_LZFileHandle, m_LZFileHandle );

   if ( size_of_destination_file < 0 )
   {
      m_LastError = size_of_destination_file;
      return( false );
   }

   return( true );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CLZFile::Dump( CDumpContext& dump_context ) const
{
   CDummyFile::Dump( dump_context );

   dump_context << TEXT( "m_LZFileHandle       = " ) << m_LZFileHandle << TEXT( "\n" );
   dump_context << TEXT( "m_OpenFileStructure is " );
   m_OpenFileStructure.Dump( dump_context );
}

#endif // _DEBUG

_Check_return_ bool Win32FoundationClasses::CLZFile::GetExpandedName( _In_ std::wstring_view name_of_compressed_file, _Out_ std::wstring& original_file_name ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   wchar_t file_name[ _MAX_PATH ];

   ::ZeroMemory( file_name, sizeof( file_name ) );

   std::wstring name(name_of_compressed_file);

   // We were passed a pointer, don't trust it

   try
   {
      if ( ::GetExpandedNameW(const_cast<LPWSTR>(name.data()), file_name ) == 1 )
      {
         original_file_name.assign( file_name );
         return( true );
      }
      else
      {
         original_file_name.clear();
         return( false );
      }
   }
   WFC_CATCH_ALL
   {
       m_LastError = ERROR_EXCEPTION_IN_SERVICE;
      original_file_name.clear();
      return( false );
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CLZFile::m_Initialize( void ) noexcept
{
   m_LZFileHandle = 0;
   m_OpenFileStructure.Empty();
   WFC_VALIDATE_POINTER( this );
}

_Check_return_ bool Win32FoundationClasses::CLZFile::Open(_In_ std::filesystem::path const& file_name, _In_ UINT const style ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( file_name.empty() == true )
   {
      return( false );
   }

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      // First parameter to LZOpenFile is mis-typed. It is not const-correct

      std::wstring non_const_string( file_name );

      m_LZFileHandle = ::LZOpenFileW( const_cast<LPWSTR>(non_const_string.data()), &m_OpenFileStructure, (WORD) style );

      if ( m_LZFileHandle < 0 )
      {
         m_LastError = m_LZFileHandle;
         return( false );
      }

      m_FileName.assign( file_name );

      return( true );
   }
   WFC_CATCH_ALL
   {
      m_LastError = ERROR_EXCEPTION_IN_SERVICE;
      return( false );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ UINT Win32FoundationClasses::CLZFile::Read( __out_bcount( size_of_buffer ) void * buffer, _In_ UINT const size_of_buffer ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( buffer );

   if ( size_of_buffer == 0 )
   {
      return( 0 );
   }

   WFC_VALIDATE_POINTER( buffer );

   INT number_of_bytes_read = 0;

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      number_of_bytes_read = ::LZRead( m_LZFileHandle, static_cast<char *>(buffer), size_of_buffer );

      if ( number_of_bytes_read < 0 )
      {
         m_LastError = number_of_bytes_read;
         return( 0 );
      }

      return( (UINT) number_of_bytes_read );
   }
   WFC_CATCH_ALL
   {
      m_LastError = ERROR_EXCEPTION_IN_SERVICE;
      return( 0 );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ uint64_t Win32FoundationClasses::CLZFile::Seek(_In_ int64_t const offset, _In_ Win32FoundationClasses::CFile64::SeekPosition const from ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle not_eq static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
#else
   ASSERT( m_hFile not_eq (CFILE_HFILE) CFile::hFileNull );
#endif
  ASSERT( from == Win32FoundationClasses::CFile64::SeekPosition::begin or from == Win32FoundationClasses::CFile64::SeekPosition::end or from == Win32FoundationClasses::CFile64::SeekPosition::current );
  ASSERT((int)Win32FoundationClasses::CFile64::SeekPosition::begin == FILE_BEGIN and (int)Win32FoundationClasses::CFile64::SeekPosition::end == FILE_END and (int)Win32FoundationClasses::CFile64::SeekPosition::current == FILE_CURRENT );

   LONG const offset_from_beginning_of_file = ::LZSeek( m_LZFileHandle, (LONG) offset, (INT) from );

   if ( offset_from_beginning_of_file < 0 )
   {
      m_LastError = offset_from_beginning_of_file;
   }

   return( offset_from_beginning_of_file );
}

void Win32FoundationClasses::CLZFile::TranslateErrorCode( _In_ int const error_code, _Out_ std::wstring& error_message ) noexcept
{
   switch( error_code )
   {
      case LZERROR_BADINHANDLE:

         error_message.assign(WSTRING_VIEW( L"Invalid input handle" ) );
         return;

      case LZERROR_BADOUTHANDLE:

         error_message.assign(WSTRING_VIEW( L"Invalid output handle" ) );
         return;

      case LZERROR_READ:

         error_message.assign(WSTRING_VIEW( L"Corrupt compressed file format" ) );
         return;

      case LZERROR_WRITE:

         error_message.assign(WSTRING_VIEW( L"Out of space for output file" ) );
         return;

      case LZERROR_GLOBALLOC:

         error_message.assign(WSTRING_VIEW( L"Insufficient memory for LZFile struct" ) );
         return;

      case LZERROR_GLOBLOCK:

         error_message.assign( WSTRING_VIEW( L"Bad global handle" ) );
         return;

      case LZERROR_BADVALUE:

         error_message.assign(WSTRING_VIEW( L"Input parameter out of acceptable range" ) );
         return;

      case LZERROR_UNKNOWNALG:

         error_message.assign(WSTRING_VIEW( L"Compression algorithm not recognized" ) );
         return;

      default:

         format( error_message, L"Unknown error number %d", error_code );
         return;
   }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CLZFile</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles LZ compressed files.">
</HEAD>

<BODY>

<H1>CLZFile : <A HREF="DumyFile.htm">CDummyFile</A></H1>
$Revision: 31 $<BR><HR>

<H2>Description</H2>

This class makes it easy to play with LZ compressed files. It encapsulates the LZ API.

<H2>Data Members</H2>

None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>Closes the LZ File.

<DT><PRE>BOOL <B><A NAME="Copy">Copy</A></B>( const CLZFile&amp; source )
BOOL <B>Copy</B>( const CLZFile * source )</PRE><DD>
Copies one CLZFile to this one.

<DT><PRE>BOOL <B><A NAME="GetExpandedName">GetExpandedName</A></B>( LPTSTR name_of_compressed_file, std::wstring&amp; original_file_name )</PRE><DD>
Retrieves what the name of the expanded file will be.

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( const char *channel_name, UINT style = OF_READ, CFileException* pError = nullptr )</PRE><DD>
Opens an LZ compressed file for expanding.

<DT><PRE>UINT <B><A NAME="Read">Read</A></B>( void * buffer, UINT size_of_buffer )</PRE><DD>
Expands data stored in the compressed file.

<DT><PRE>LONG <B><A NAME="Seek">Seek</A></B>( LONG offset, UINT from )</PRE><DD>
Moves the file pointer.

<DT><PRE>static void <B><A NAME="TranslateErrorCode">TranslateErrorCode</A></B>( int error_code, std::wstring&amp; error_message )</PRE><DD>
Translates an error code ( LZERROR_xxx) into a human readable string.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

void test_CLZFile( LPTSTR lz_file_name )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;test_CLZFile()&quot; ) );

   <B>CLZFile</B> lz_file;

   if ( lz_file_name == nullptr )
   {
      return;
   }

   std::wstring expanded_name;

   lz_file.GetExpandedName( lz_file_name, expanded_name );

   _tprintf( TEXT( &quot;LZ File \&quot;%s\&quot; expanded name is \&quot;%s\&quot;\n&quot; ), lz_file_name, (LPCTSTR) expanded_name );

   if ( lz_file.Open( lz_file_name ) not_eq FALSE )
   {
      <B>CLZFile</B> output_file;

      if ( output_file.Open( expanded_name, OF_CREATE ) not_eq FALSE )
      {
         if ( output_file.Copy( lz_file ) not_eq FALSE )
         {
            _tprintf( TEXT( &quot;Successfully copied\n&quot; ) );
         }
         else
         {
            std::wstring error_message_string;
            CLZFile::TranslateErrorCode( (int) lz_file.GetErrorCode(), error_message_string );
            _tprintf( TEXT( &quot;Can't Copy because %s\n&quot; ), (LPCTSTR) error_message_string );
         }
      }
      else
      {
         std::wstring error_message_string;
         CLZFile::TranslateErrorCode( (int) output_file.GetErrorCode(), error_message_string );
         _tprintf( TEXT( &quot;Can't open LZ output file because %s\n&quot; ), (LPCTSTR) error_message_string );
      }
   }
   else
   {
      std::wstring error_message_string;
      CLZFile::TranslateErrorCode( (int) lz_file.GetErrorCode(), error_message_string );
      _tprintf( TEXT( &quot;Can't open LZ file \&quot;%s\&quot; because \n&quot; ), lz_file_name, (LPCTSTR) error_message_string );
   }
}</CODE></PRE>

<H2>API's Used</H2>

<B>CLZFile</B> uses the following API's:

<UL>
<LI>GetExpandedName
<LI>LZClose
<LI>LZCopy
<LI>LZOpenFile
<LI>LZRead
<LI>LZSeek
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: lzfile.cpp $<BR>
$Modtime: 6/26/01 10:53a $
</BODY>
</HTML>
*/
