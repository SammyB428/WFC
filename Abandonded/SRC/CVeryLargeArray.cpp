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

CVeryLargeArray::CVeryLargeArray( unsigned __int64 number_of_bytes_per_element, unsigned __int64 number_of_bytes_per_page )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeArray::CVeryLargeArray()" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( number_of_bytes_per_element );

   m_NumberOfBytesPerElement = number_of_bytes_per_element;
   m_NumberOfBytesPerPage    = number_of_bytes_per_page;

   // Now idiot proof a few things... Well, try at least. It seems they
   // always come up with a better idiot every day.

   _ASSERTE( m_NumberOfBytesPerElement > 0 );
   _ASSERTE( m_NumberOfBytesPerPage > m_NumberOfBytesPerElement );

   if ( m_NumberOfBytesPerElement == 0 )
   {
      m_NumberOfBytesPerElement = 1;
   }

   if ( m_NumberOfBytesPerPage < m_NumberOfBytesPerElement )
   {
      WFCTRACE( TEXT( "Idiot detected..." ) );

      while( m_NumberOfBytesPerPage < m_NumberOfBytesPerElement )
      {
         m_NumberOfBytesPerPage += WFC_DEFAULT_VLA_PAGE_SIZE;
      }
   }

   m_NumberOfElementsInArray = 0;
   m_NumberOfPagesInArray    = 0;
}

void CVeryLargeArray::Append( const void * element, unsigned __int64 number_of_elements_to_append )
{
   WFCLTRACEINIT( TEXT( "CVeryLargeArray::Append( const void * )" ) );
   WFC_VALIDATE_POINTER( this );

   unsigned long loop_count = 0;

   while( loop_count < number_of_elements_to_append )
   {
      Add( element );
      loop_count++;
   }
}

static int __wfc_vla_appender_callback( void * context, void * element )
{
   CVeryLargeArray * array_to_append_to = (CVeryLargeArray *) context;

   array_to_append_to->Add( element );

   return( WFC_VLA_CONTINUE_PROCESSING );
}

void CVeryLargeArray::Append( CVeryLargeArray& source ) WFC_DOESNT_THROW_EXCEPTIONS
{
   WFCLTRACEINIT( TEXT( "CVeryLargeArray::Append( CVeryLargeArray )" ) );
   WFC_VALIDATE_POINTER( this );

  _ASSERTE( GetElementSize() == source.GetElementSize() );

  if ( GetElementSize() != source.GetElementSize() )
  {
     return;
  }

  source.ForEach( __wfc_vla_appender_callback, this );
}

CVeryLargeArray::~CVeryLargeArray()
{
   WFCLTRACEINIT( TEXT( "CVeryLargeArray::~CVeryLargeArray()" ) );
   WFC_VALIDATE_POINTER( this );
}

#endif // WFC_WORK_IN_PROGRESS

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CVeryLargeArray</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ array base class that is designed to handle billions of entries.">
</HEAD>

<BODY>

<H1>CVeryLargeArray</H1>
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

   <B>CVeryLargeArray</B> com_requires_a_gui;

   do_non_gui_server_stuff();
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CCOMSucks.cpp $<BR>
$Modtime: 6/26/01 10:44a $
</BODY>

</HTML>
*/
