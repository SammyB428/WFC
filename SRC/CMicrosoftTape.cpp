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
** $Workfile: CMicrosoftTape.cpp $
** $Revision: 24 $
** $Modtime: 6/26/01 10:47a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#define new DEBUG_NEW
#endif // _DEBUG

/*
** CMicrosoftTape
** As documented in ftp://ftp.microsoft.com/developr/drg/Tape/MTF10.ZIP
*/

Win32FoundationClasses::CMicrosoftTape::CMicrosoftTape() noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
   m_AutomaticallyClose = false;
}

Win32FoundationClasses::CMicrosoftTape::~CMicrosoftTape()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void Win32FoundationClasses::CMicrosoftTape::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_TapeHeader.Empty();
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CMicrosoftTape::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   dump_context << TEXT( "a CMicrosoftTape at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   m_TapeHeader " );
   m_TapeHeader.Dump( dump_context );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CMicrosoftTape::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
}

_Check_return_ bool Win32FoundationClasses::CMicrosoftTape::EnumerateSets( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( true );
}

_Check_return_ bool Win32FoundationClasses::CMicrosoftTape::Open( _In_ UINT const tape_drive_number_starting_at_zero ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( CTape::Open( tape_drive_number_starting_at_zero ) == true )
   {
       std::ignore = ReadHeader();
      return( true );
   }
   else
   {
      return( false );
   }
}

_Check_return_ bool Win32FoundationClasses::CMicrosoftTape::ReadHeader( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( WFC_STL )
   ASSERT( m_FileHandle not_eq static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
#else
   ASSERT( m_hFile not_eq (CFILE_HFILE) hFileNull );
#endif

   if ( m_FileHandle == 
#if defined( WFC_STL )
       (HANDLE)
#else
       (CFILE_HFILE)
#endif
       INVALID_HANDLE_VALUE )
   {
      m_LastError = ERROR_SUCCESS;
      return( false );
   }

   //Rewind();

   DWORD number_of_bytes_read{ 0 };

   if ( ::ReadFile( m_FileHandle, (LPVOID) (MTF_TAPE *) &m_TapeHeader, sizeof( MTF_TAPE ), &number_of_bytes_read, nullptr ) == FALSE )
   {
      return( false );
   }

   return( true );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CMicrosoftTape</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code, MTF">
<META name="description" content="The C++ class that handles the Microsoft Tape Format (MTF).">
</HEAD>

<BODY>

<H1>CMicrosoftTape : <A HREF="CTape.htm">CTape</A></H1>

$Revision: 24 $<BR>
<HR>

<H2>Description</H2>

This class is a work in progress. If you find it useful let me know and I'll
extend it further. This is basically a toy.
The Microsoft Tape format is documented in a Postscript file called
<CODE><A HREF="mtf10.zip">MTF10.ZIP</A></CODE> which can be found at
<CODE>ftp://ftp.microsoft.com/developr/drg/Tape/MTF10.ZIP</CODE>

<H2>Data Members</H2>None.

<H2>Methods</H2>

<DL COMPACT>

<DT><B><A NAME="Empty">Empty</A></B><DD>Clears all internal data members.

<DT><B><A NAME="EnumerateSets">EnumerateSets</A></B><DD>This doesn't work. But if it did it would
enumerate all the sets on the tape.

<DT><B><A NAME="Open">Open</A></B><DD>Opens the tape drive for reading and reads the header.

<DT><B><A NAME="ReadHeader">ReadHeader</A></B><DD>Retrieves the header information about the
tape backup.

</DL>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CMicrosoftTape.cpp $<BR>
$Modtime: 6/26/01 10:47a $
</BODY>

</HTML>
*/
