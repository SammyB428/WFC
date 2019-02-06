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

CVeryLargeDiskArray::CVeryLargeDiskArray( unsigned __int64 number_of_bytes_per_element, unsigned __int64 number_of_bytes_per_page ) :
   CVeryLargeArray( number_of_bytes_per_element, number_of_bytes_per_page )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::CVeryLargeDiskArray()" ) );
   WFC_VALIDATE_POINTER( this );
}

CVeryLargeDiskArray::~CVeryLargeDiskArray()
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::~CVeryLargeDiskArray()" ) );
   WFC_VALIDATE_POINTER( this );
}

bool CVeryLargeDiskArray::Add( const void * element )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::Add()" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( element );

   return( false );
}

bool CVeryLargeDiskArray::Coalesce( void )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::Coalesce()" ) );
   WFC_VALIDATE_POINTER( this );

   return( false );
}

bool CVeryLargeDiskArray::Copy( const CVeryLargeArray& source )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::Copy()" ) );
   WFC_VALIDATE_POINTER( this );

   return( false );
}

bool CVeryLargeDiskArray::ForEach( WFC_VLA_ELEMENT_CALLBACK callback, void * callback_context )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::ForEach()" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( callback );
   WFC_VALIDATE_POINTER_NULL_OK( callback_context );

   return( false );
}

void CVeryLargeDiskArray::FreeExtra( void )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::FreeExtra()" ) );
   WFC_VALIDATE_POINTER( this );
}

bool CVeryLargeDiskArray::GetAt( unsigned __int64 array_index, void * destination ) const
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::GetAt()" ) );
   WFC_VALIDATE_POINTER( this );

   return( false );
}

bool CVeryLargeDiskArray::InsertAt( unsigned __int64 array_index, void * new_element, unsigned __int64 number_of_times_to_insert_it )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::InsertAt( new_element )" ) );
   WFC_VALIDATE_POINTER( this );

   return( false );
}

bool CVeryLargeDiskArray::InsertAt( unsigned __int64 array_index, CVeryLargeArray& source )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::InsertAt( CVeryLargeArray )" ) );
   WFC_VALIDATE_POINTER( this );

   return( false );
}

void CVeryLargeDiskArray::RemoveAll( void )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::RemoveAll()" ) );
   WFC_VALIDATE_POINTER( this );
}

bool CVeryLargeDiskArray::RemoveAt( unsigned __int64 array_index, unsigned __int64 number_of_elements_to_remove )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::RemoveAt()" ) );
   WFC_VALIDATE_POINTER( this );

   return( false );
}

bool CVeryLargeDiskArray::SetSize( unsigned __int64 number_of_elements )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::SetSize()" ) );
   WFC_VALIDATE_POINTER( this );

   return( false );
}

bool CVeryLargeDiskArray::SetAt( unsigned __int64 array_index, void * new_element )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeDiskArray::SetAt()" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( new_element );

   return( false );
}

#endif // WFC_WORK_IN_PROGRESS

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CVeryLargeDiskArray</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ array base class that is designed to handle billions of entries in memory.">
</HEAD>

<BODY>

<H1>CVeryLargeDiskArray</H1>
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

   <B>CVeryLargeDiskArray</B> com_requires_a_gui;

   do_non_gui_server_stuff();
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CCOMSucks.cpp $<BR>
$Modtime: 6/26/01 10:44a $
</BODY>

</HTML>
*/
