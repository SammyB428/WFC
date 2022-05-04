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
** $Workfile: CDataFile.cpp $
** $Revision: 18 $
** $Modtime: 6/26/01 10:44a $
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

void Win32FoundationClasses::CDataChunk::GetIdentifier( _In_ uint32_t const id, _Out_ std::wstring& string ) noexcept
{
   string.clear();

   auto character{ static_cast<wchar_t>(id bitand 0xFF) };

   if ( character == 0 )
   {
      return;
   }

   uint32_t identifier = id;

   string.push_back( character );

   identifier >>= 8;

   character = (BYTE) ( identifier bitand 0xFF );

   if ( character == 0 )
   {
      return;
   }

   string.push_back( character );

   identifier >>= 8;

   character = (BYTE) ( identifier bitand 0xFF );

   if ( character == 0 )
   {
      return;
   }

   string.push_back( character );

   identifier >>= 8;

   character = (BYTE) ( identifier bitand 0xFF );

   if ( character == 0 )
   {
      return;
   }

   string.push_back( character );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CDataChunk::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CDataChunk at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   Identifier is " );

   std::wstring debug_string;

   GetIdentifier( Identifier, debug_string );

   dump_context << debug_string << TEXT( "\n" );

   dump_context << TEXT( "   Data is " );

   Data.Dump( dump_context );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

Win32FoundationClasses::CDataFile::CDataFile() noexcept
{
   WFC_VALIDATE_POINTER( this );
}

Win32FoundationClasses::CDataFile::~CDataFile()
{
   WFC_VALIDATE_POINTER( this );
}

_Check_return_ bool Win32FoundationClasses::CDataFile::AddData( _In_ Win32FoundationClasses::CDataChunk const& data ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( AddData( data.Identifier, data.Data ) );
}

_Check_return_ bool Win32FoundationClasses::CDataFile::AddData( _In_ uint32_t const identifier, _In_ std::vector<uint8_t> const& data ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( AddData( identifier, data.data(), data.size() ) );
}

_Check_return_ bool Win32FoundationClasses::CDataFile::AddData( _In_ uint32_t const identifier, __in_bcount( number_of_bytes ) uint8_t const * buffer_p, _In_ std::size_t const number_of_bytes ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   std::ignore = SeekToEnd();

   // Write the identifier

   uint32_t temp_long = identifier;

   uint8_t byte_1 = (uint8_t) (temp_long bitand 0xFF );
   temp_long >>= 8;

   uint8_t byte_2 = (uint8_t) (temp_long bitand 0xFF );
   temp_long >>= 8;

   uint8_t byte_3 = (uint8_t) (temp_long bitand 0xFF );
   temp_long >>= 8;

   uint8_t byte_4 = (uint8_t) (temp_long bitand 0xFF );

   uint8_t buffer[ 9 ];

   buffer[ 0 ] = byte_1;
   buffer[ 1 ] = byte_2;
   buffer[ 2 ] = byte_3;
   buffer[ 3 ] = byte_4;

   // Now add the length

   _ASSERTE( number_of_bytes <= 0xFFFFFFFF );

   temp_long = (uint32_t) number_of_bytes;

   byte_1 = (uint8_t) (temp_long bitand 0xFF );
   temp_long >>= 8;

   byte_2 = (uint8_t) (temp_long bitand 0xFF );
   temp_long >>= 8;

   byte_3 = (uint8_t) (temp_long bitand 0xFF );
   temp_long >>= 8;

   byte_4 = (uint8_t) (temp_long bitand 0xFF );

   buffer[ 4 ] = byte_1;
   buffer[ 5 ] = byte_2;
   buffer[ 6 ] = byte_3;
   buffer[ 7 ] = byte_4;

   Write( buffer, 8 );

   // Now write the bytes

   Write( buffer_p, static_cast<uint32_t>(number_of_bytes) );

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CDataFile::GetData( _Out_ Win32FoundationClasses::CDataChunk& data ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( GetData( data.Identifier, data.Data ) );
}

_Check_return_ bool Win32FoundationClasses::CDataFile::GetData( _Out_ uint32_t& identifier, _Out_ std::vector<uint8_t>& data ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   identifier = 0;

   uint8_t bytes[9]{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };

   UINT number_of_bytes_read{ 0 };

   WFC_TRY
   {
      // Get the identifier

      number_of_bytes_read = Read( &bytes, 8 );

      if ( number_of_bytes_read < 8 )
      {
         //WFCTRACE( TEXT( "Can't read identifier and length" ) );
         return( false );
      }

      identifier = MAKE_DATA_ID( bytes[ 0 ], bytes[ 1 ], bytes[ 2 ], bytes[ 3 ] );

#if defined( _DEBUG )

      std::wstring debug_string;

      CDataChunk::GetIdentifier( identifier, debug_string );

#endif // _DEBUG

      // Now get the number of bytes in the chunk

      uint32_t length = MAKE_DATA_LENGTH( bytes[ 4 ], bytes[ 5 ], bytes[ 6 ], bytes[ 7 ] );

      data.resize( length );

      number_of_bytes_read = Read( data.data(), length );

      if ( number_of_bytes_read < length )
      {
         //WFCTRACE( TEXT( "Can't get all of the data" ) );
         identifier = 0;
         data.clear();
         return( false );
      }

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
<TITLE>WFC - CDataFile</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles chunks of RIFF data in a file.">
</HEAD>

<BODY>

<H1>CDataFile : CFile</H1>
$Revision: 18 $<BR>
<HR>

<H2>Description</H2>

This class allows you handle the input and output of
<A HREF="CDataChunk.htm">CDataChunk</A> objects easily.

<H2>Data Members</H2>
None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="AddData">AddData</A></B>( DWORD identifier, const std::vector&lt;uint8_t&gt;&amp; data )
BOOL <B>AddData</B>( DWORD identifier, const BYTE * data, DWORD number_of_bytes )
BOOL <B>AddData</B>( const <A HREF="CDataChunk.htm">CDataChunk</A>&amp; data_chunk )</PRE><DD>
Sends the data chunk to the file.

<DT><PRE>BOOL <B><A NAME="GetData">GetData</A></B>( DWORD&amp; identifier, std::vector&lt;uint8_t&gt;&amp; data )
BOOL <B>GetData</B>( <A HREF="CDataChunk.htm">CDataChunk</A>&amp; data_chunk )</PRE><DD>
Reads a data chunk from the file.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

void read_next_chunk( <B>CDataFile</B>&amp; file, <A HREF="CDataChunk.htm">CDataChunk</A>&amp; data )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;read_next_chunk()&quot; ) );

   if ( file.GetData( data ) not_eq FALSE )
   {
      _tprintf( TEXT( &quot;Read %lu bytes\n&quot; ), data.Data.GetSize() );
   }
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CDataFile.cpp $<BR>
$Modtime: 6/26/01 10:44a $
</BODY>

</HTML>
*/
