/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2003, Samuel R. Blackburn
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
** $Workfile: CCOMSucks.cpp $
** $Revision: 5 $
** $Modtime: 6/26/01 10:44a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#if defined( WFC_WORK_IN_PROGRESS )

USING_WFC_NAMESPACE

CVeryLargeMemoryArray::CVeryLargeMemoryArray( unsigned __int64 number_of_bytes_per_element, unsigned __int64 number_of_bytes_per_page ) :
   CVeryLargeArray( number_of_bytes_per_element, number_of_bytes_per_page )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::CVeryLargeMemoryArray()" ) );
   WFC_VALIDATE_POINTER( this );

   SYSTEM_INFO system_information;

   ZeroMemory( &system_information, sizeof( system_information ) );

   GetSystemInfo( &system_information );

   m_OperatingSystemPageSize = system_information.dwPageSize;

   _ASSERTE( m_OperatingSystemPageSize > 0 );

   m_HeapHandle = HeapCreate( 0, 0, 0 );

   _ASSERTE( m_HeapHandle != NULL );
}

CVeryLargeMemoryArray::~CVeryLargeMemoryArray()
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::~CVeryLargeMemoryArray()" ) );
   WFC_VALIDATE_POINTER( this );
}

bool CVeryLargeMemoryArray::Add( const void * element )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::Add()" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( element );
   _ASSERTE( m_HeapHandle != NULL );

   return( false );
}

bool CVeryLargeMemoryArray::Coalesce( void )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::Coalesce()" ) );
   WFC_VALIDATE_POINTER( this );
   _ASSERTE( m_HeapHandle != NULL );

   return( false );
}

bool CVeryLargeMemoryArray::Copy( const CVeryLargeArray& source )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::Copy()" ) );
   WFC_VALIDATE_POINTER( this );
   _ASSERTE( m_HeapHandle != NULL );

   return( false );
}

bool CVeryLargeMemoryArray::ForEach( WFC_VLA_ELEMENT_CALLBACK callback, void * callback_context )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::ForEach()" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( callback );
   WFC_VALIDATE_POINTER_NULL_OK( callback_context );
   _ASSERTE( m_HeapHandle != NULL );

   return( false );
}

void CVeryLargeMemoryArray::FreeExtra( void )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::FreeExtra()" ) );
   WFC_VALIDATE_POINTER( this );
   _ASSERTE( m_HeapHandle != NULL );
}

bool CVeryLargeMemoryArray::GetAt( unsigned __int64 array_index, void * destination ) const
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::GetAt()" ) );
   WFC_VALIDATE_POINTER( this );
   _ASSERTE( m_HeapHandle != NULL );

   return( false );
}

bool CVeryLargeMemoryArray::InsertAt( unsigned __int64 array_index, void * new_element, unsigned __int64 number_of_times_to_insert_it )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::InsertAt( new_element )" ) );
   WFC_VALIDATE_POINTER( this );
   _ASSERTE( m_HeapHandle != NULL );

   return( false );
}

bool CVeryLargeMemoryArray::InsertAt( unsigned __int64 array_index, CVeryLargeArray& source )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::InsertAt( CVeryLargeArray )" ) );
   WFC_VALIDATE_POINTER( this );
   _ASSERTE( m_HeapHandle != NULL );

   return( false );
}

void CVeryLargeMemoryArray::RemoveAll( void )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::RemoveAll()" ) );
   WFC_VALIDATE_POINTER( this );
   _ASSERTE( m_HeapHandle != NULL );

   if ( m_HeapHandle != NULL )
   {
      ::HeapDestroy( m_HeapHandle );
   }

   m_HeapHandle = ::HeapCreate( 0, 0, 0 );

   _ASSERTE( m_HeapHandle != NULL );
}

bool CVeryLargeMemoryArray::RemoveAt( unsigned __int64 array_index, unsigned __int64 number_of_elements_to_remove )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::RemoveAt()" ) );
   WFC_VALIDATE_POINTER( this );
   _ASSERTE( m_HeapHandle != NULL );

   return( false );
}

bool CVeryLargeMemoryArray::SetSize( unsigned __int64 number_of_elements )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::SetSize()" ) );
   WFC_VALIDATE_POINTER( this );
   _ASSERTE( m_HeapHandle != NULL );

   return( false );
}

bool CVeryLargeMemoryArray::SetAt( unsigned __int64 array_index, void * new_element )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeMemoryArray::SetAt()" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( new_element );
   _ASSERTE( m_HeapHandle != NULL );

   return( false );
}

#endif // WFC_WORK_IN_PROGRESS

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CVeryLargeMemoryArray</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ array base class that is designed to handle billions of entries in memory.">
</HEAD>

<BODY>

<H1>CVeryLargeMemoryArray</H1>
$Revision: 5 $<BR>
<HR>

<H2>Description</H2>


<H2>Data Members</H2>
None.

<H2>Methods</H2>
None.

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

void main( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;read_next_chunk()&quot; ) );

   <B>CVeryLargeMemoryArray</B> com_requires_a_gui;

   do_non_gui_server_stuff();
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CCOMSucks.cpp $<BR>
$Modtime: 6/26/01 10:44a $
</BODY>

</HTML>
*/
