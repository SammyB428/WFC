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
** $Workfile: CDataMemory.cpp $
** $Revision: 20 $
** $Modtime: 6/26/01 10:45a $
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

CDataMemory::CDataMemory()
{
   WFC_VALIDATE_POINTER( this );
   m_Position = 0;
}

CDataMemory::CDataMemory( __in CDataMemory const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CDataMemory::CDataMemory( __in_bcount( number_of_bytes ) uint8_t const * buffer_p, __in std::size_t const number_of_bytes )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( buffer_p );
   Copy( buffer_p, number_of_bytes );
}

CDataMemory::CDataMemory( __in std::vector<uint8_t> const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CDataMemory::~CDataMemory()
{
   WFC_VALIDATE_POINTER( this );
   m_Position = 0;
}

void CDataMemory::Append( __in std::vector<uint8_t> const& data ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Data.insert( std::end(m_Data), std::cbegin(data), std::cend(data) );
   m_Position = m_Data.size();
}

void CDataMemory::Append( __in CDataMemory const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Append( source.m_Data );
}

void CDataMemory::AppendTo( __out std::vector<uint8_t>& destination ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   destination.insert( std::end(destination), std::cbegin(m_Data), std::cend(m_Data) );
}

void CDataMemory::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Position = 0;
   m_Data.clear();
}

void CDataMemory::Copy( __in_bcount( number_of_bytes ) uint8_t const * buffer_p, __in std::size_t const number_of_bytes ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( buffer_p );

   m_Position = 0;

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      m_Data.resize( number_of_bytes );

      if ( number_of_bytes > 0 )
      {
         ::memcpy( m_Data.data(), buffer_p, number_of_bytes );
      }
   }
   WFC_CATCH_ALL
   {
      // Do Nothing
   }
   WFC_END_CATCH_ALL
}

void CDataMemory::Copy( __in std::vector<uint8_t> const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_Position = 0;
   m_Data = source;
}

void CDataMemory::Copy( __in CDataMemory const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_Data = source.m_Data;
   m_Position = source.m_Position;
}

void CDataMemory::CopyTo( __out std::vector<uint8_t>& data ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   data = m_Data;
}

void CDataMemory::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Position = 0;
   m_Data.clear();
}

void CDataMemory::Flush( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
}

__checkReturn BYTE const * CDataMemory::GetData( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_Data.data() );
}

__checkReturn uint64_t CDataMemory::GetLength( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_Data.size() );
}

__checkReturn uint64_t CDataMemory::GetPosition( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_Position );
}

__checkReturn bool CDataMemory::Open( __in_z LPCTSTR filename, __in UINT const mode ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( filename );

   m_Position = 0;

   // We were passed a pointer, don't trust it
   WFC_TRY
   {
#if ! defined( WFC_STL )
      m_strFileName = filename;
#else // WFC_STL
      m_FileName.assign( filename );
#endif // WFC_STL
      return( true );
   }
   WFC_CATCH_ALL
   {
      return( false );
   }
   WFC_END_CATCH_ALL
}

__checkReturn UINT CDataMemory::Read(__out_bcount(read_size) void * buffer, __in const UINT read_size ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( buffer );

   if ( read_size == 0 )
   {
      return( 0 );
   }

   if ( buffer == nullptr )
   {
      return( 0 );
   }

   UINT number_of_bytes_to_read = read_size;

   // The user passed us a pointer, don't trust it

   WFC_TRY
   {
      std::size_t number_of_bytes_read = 0;

      if ( ( m_Position + number_of_bytes_to_read ) > (DWORD) m_Data.size() )
      {
         number_of_bytes_to_read = (UINT) ( (LONGLONG) m_Data.size() - m_Position );
      }

      auto byte_buffer = reinterpret_cast<BYTE *>( buffer );

      if ( number_of_bytes_to_read > 0 )
      {
         for ( UINT byte_buffer_index = 0; byte_buffer_index < number_of_bytes_to_read; byte_buffer_index++ )
         {
            byte_buffer[ byte_buffer_index ] = m_Data.at( (size_t) m_Position );
            m_Position++;
         }

         number_of_bytes_read = number_of_bytes_to_read;
      }
      else
      {
         number_of_bytes_read = 0;
      }

      return( (UINT) number_of_bytes_read );
   }
   WFC_CATCH_ALL
   {
      return( 0 );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ uint64_t CDataMemory::Seek(_In_ int64_t const offset, _In_ CFile64::SeekPosition const from ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   switch( from )
   {
   case SeekPosition::begin:

         m_Position = offset;
         break;

   case SeekPosition::current:

         m_Position += offset;
         break;

   case SeekPosition::end:

         m_Position = m_Data.size() - offset;
         break;
   }

   if ( m_Position >= (uint64_t) m_Data.size() )
   {
      m_Position = m_Data.size() - 1;
   }

   if ( m_Position < 0 )
   {
      m_Position = 0;
   }

   return( m_Position );
}

void CDataMemory::Write( __in_bcount( number_of_bytes_to_write ) void const * buffer, __in UINT const number_of_bytes_to_write ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( buffer );

   if ( number_of_bytes_to_write == 0 )
   {
      return;
   }

   if ( buffer == nullptr )
   {
      return;
   }

   // The user sent us a pointer, don't trust it

   WFC_TRY
   {
      auto byte_buffer = reinterpret_cast<BYTE const *>( buffer );

      for( auto const index : Range(number_of_bytes_to_write) )
      {
         // Thanks go to Darin Greaham (greaham@cyberramp.net) for finding
         // where I was not incrementing index. That caused an endless loop
         // to occur. DOH!
         // 1998-08-09

         (void) m_Data.push_back( byte_buffer[index] );
         // m_Data.Append( (const BYTE *) buffer, number_of_bytes_to_write );
      }
   }
   WFC_CATCH_ALL
   {
      return;
   }
   WFC_END_CATCH_ALL
}

__checkReturn CDataMemory const& CDataMemory::operator=( __in CDataMemory const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

__checkReturn CDataMemory const& CDataMemory::operator=( __in std::vector<uint8_t> const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

__checkReturn CDataMemory const& CDataMemory::operator+=( __in std::vector<uint8_t> const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Append( source );
   return( *this );
}

__checkReturn CDataMemory const& CDataMemory::operator+=( __in CDataMemory const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Append( source );
   return( *this );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CDataMemory</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles chunks of RIFF data in memory.">
</HEAD>

<BODY>

<H1>CDataMemory : <A HREF="CDataFile.htm">CDataFile</A></H1>
$Revision: 20 $<BR>
<HR>

<H2>Description</H2>

This is the interesting <A HREF="CDataFile.htm">CDataFile</A>
derivative. It allows you to treat a chunk of memory like
it was a file. This makes it easy to embed <A HREF="CDataChunk.htm">CDataChunk</A>'s
within other chunks (i.e. a chunk of chunks).

<H2>Data Members</H2>
None.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CDataMemory</B>()
<B>CDataMemory</B>( const CDataMemory&amp; source )
<B>CDataMemory</B>( const BYTE * buffer_p, DWORD number_of_bytes )
<B>CDataMemory</B>( const std::vector&lt;uint8_t&gt;&amp; source )</PRE><DD>
Constructs the object.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Append">Append</A></B>( const std::vector&lt;uint8_t&gt;&amp; source )
void <B>Append</B>( const CDataMemory&amp; source )</PRE><DD>
Appends the contents of <CODE>source</CODE> to this object.

<DT><PRE>void <B><A NAME="AppendTo">AppendTo</A></B>( std::vector&lt;uint8_t&gt;&amp; data )</PRE><DD>
Appends the contents of this object to <CODE>data</CODE>.

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the &quot;file&quot;.

<DT><PRE>void <B><A NAME="Copy">Copy</A></B>( const BYTE * buffer, DWORD number_of_bytes_to_copy )
void <B>Copy</B>( const std::vector&lt;uint8_t&gt;&amp; buffer )
void <B>Copy</B>( const CDataMemory&amp; buffer )</PRE><DD>
Copies the contents of <CODE>buffer</CODE> to this object. It
replaces the current contents of this object with whatever is
in <CODE>buffer</CODE>.

<DT><PRE>void <B><A NAME="CopyTo">CopyTo</A></B>( std::vector&lt;uint8_t&gt;&amp; data )</PRE><DD>
Replaces the contents of <CODE>data</CODE> with the contents
of this object.

<DT><PRE>void <B><A NAME="Empty">Empty</A></B>( void )</PRE><DD>
Empties this objects.

<DT><PRE>void <B><A NAME="Flush">Flush</A></B>( void )</PRE><DD>
Does nothing, just intercept the call.
Since we aren't really a file, it would just cause
the base class to blow up.

<DT><PRE>const BYTE * <B><A NAME="GetData">GetData</A></B>( void ) const</PRE><DD>
Returns a pointer to the raw data.

<DT><PRE>DWORD <B><A NAME="GetLength">GetLength</A></B>( void ) const</PRE><DD>
Returns the number of bytes currently in the buffer.

<DT><PRE>DWORD <B><A NAME="GetPosition">GetPosition</A></B>( void ) const</PRE><DD>
Returns the current file pointer.

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( LPCTSTR filename, UINT mode )</PRE><DD>
Doesn't do anything.

<DT><PRE>UINT <B><A NAME="Read">Read</A></B>( void * buffer, UINT number_of_bytes_to_read )</PRE><DD>
Reads data from the buffer.

<DT><PRE>long <B><A NAME="Seek">Seek</A></B>( long offset, UINT from )</PRE><DD>
Positions the file pointer.

<DT><PRE>void <B><A NAME="Write">Write</A></B>( const void * buffer, UINT number_of_bytes_to_write )</PRE><DD>
Appends data to the buffer.

</DL>

<H2>Operators</H2>

<DL COMPACT>

<DT><PRE>const CDataMemory&amp; operator <B>=</B> ( const CDataMemory&amp; source )
const CDataMemory&amp; operator <B>=</B> ( const std::vector&lt;uint8_t&gt;&amp; source )</PRE><DD>
Calls the <B>Copy</B>() method.

<DT><PRE>const CDataMemory&amp; operator <B>+=</B> ( const CDataMemory&amp; source )
const CDataMemory&amp; operator <B>+=</B> ( const std::vector&lt;uint8_t&gt;&amp; source )</PRE><DD>
Calls the <B>Append</B>() method.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

void read_next_chunk( <B>CDataMemory</B>&amp; data_in_memory, <A HREF="CDataChunk.htm">CDataChunk</A>&amp; data )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;read_next_chunk()&quot; ) );

   data_in_memory.GetData( data );

   _tprintf( TEXT( &quot;Read %lu bytes\n&quot; ), data.Data.GetSize() );
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CDataMemory.cpp $<BR>
$Modtime: 6/26/01 10:45a $
</BODY>
</HTML>
*/
