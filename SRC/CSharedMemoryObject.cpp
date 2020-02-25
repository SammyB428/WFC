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
** $Workfile: CSharedMemoryObject.cpp $
** $Revision: 8 $
** $Modtime: 6/26/01 10:51a $
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

Win32FoundationClasses::CSharedMemoryObject::CSharedMemoryObject() noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_MapHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
   m_Pointer   = nullptr;
   m_Size      = 0;
}

Win32FoundationClasses::CSharedMemoryObject::~CSharedMemoryObject() noexcept
{
   WFC_VALIDATE_POINTER( this );

   Close();

   m_MapHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
   m_Pointer   = nullptr;
   m_Size      = 0;
}

void Win32FoundationClasses::CSharedMemoryObject::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_MapHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      return;
   }

   if ( m_Pointer != nullptr )
   {
      if ( ::UnmapViewOfFile( m_Pointer ) == FALSE )
      {
         //WFCTRACEERROR( ::GetLastError() );
      }

      m_Pointer = nullptr;
   }

   if ( ::CloseHandle( m_MapHandle ) == FALSE )
   {
      //WFCTRACEERROR( ::GetLastError() );
   }

   m_MapHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
   m_Size = 0;
   m_Name.clear();
}

_Check_return_ bool Win32FoundationClasses::CSharedMemoryObject::Create( _In_ std::wstring_view object_name, _In_ std::size_t const size_in_bytes ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Close();

   m_Name.assign(object_name);

   m_MapHandle = ::CreateFileMappingW( INVALID_HANDLE_VALUE,
                                      nullptr,
                                      PAGE_READWRITE,
                                      0,
                              (DWORD) size_in_bytes,
                                      m_Name.c_str() );

   if ( m_MapHandle == static_cast< HANDLE >( NULL ) )
   {
      //WFCTRACEERROR( ::GetLastError() );
      m_MapHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
      m_Name.clear();
      return( false );
   }

   m_Size = size_in_bytes;

   m_Pointer = ::MapViewOfFile( m_MapHandle, FILE_MAP_ALL_ACCESS, 0, 0, m_Size );

   if ( m_Pointer == nullptr )
   {
      //WFCTRACEERROR( ::GetLastError() );
      Close();
      return( false );
   }

   return( true );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CSharedMemoryObject</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that make handling shared memory easy.">
</HEAD>

<BODY>

<H1>CSharedMemoryObject</H1>
$Revision: 8 $<BR>
<HR>

<H2>Description</H2>

This class allows you to create shared memory objects easily.
A shared memory segment is a chunk of memory that is shared
between multiple processes in a system. Shared memory is
great for things like holding status information. For example,
you could have a process that reads position information from
a GPS unit and puts it into shared memory. You could have many
other applications connect to that shared memory and read the
position data.

<H2>Data Members</H2>
None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the object. You can no longer use the pointer.

<DT><PRE>BOOL <B><A NAME="Create">Create</A></B>( const std::wstring&amp; object_name, DWORD size_in_bytes )</PRE><DD>
Creates a shared memory object. If the object specified by
<CODE>object_name</CODE> already exists, you will attach to it. In other
words, when <B>Create</B>() returns TRUE, you will have a shared
memory object whether it was newly created or attached to an existing one.

<DT><PRE>void <B><A NAME="GetName">GetName</A></B>( std::wstring&amp; object_name ) const</PRE><DD>
Returns the name of the shared memory object.

<DT><PRE>void * <B><A NAME="GetPointer">GetPointer</A></B>( void ) const</PRE><DD>
Returns the pointer to the shared memory segment. It will return
NULL if <B><A HREF="#Create">Create</A></B>() has not yet been called.

<DT><PRE>DWORD <B><A NAME="GetSize">GetSize</A></B>( void ) const</PRE><DD>
Returns the size in bytes fo the shared memory object. It
will return zero if <B>Create</B>() has not yet been called.

</DL>

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

void update_time( const std::wstring&amp queue_name )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;update_time()&quot; ) );

   <B>CSharedMemoryObject</B> queue;

   if ( queue.<B>Create</B>( queue_name, sizeof( APPLICATION_QUEUE ) != FALSE )
   {
      APPLICATION_QUEUE * queue_p = nullptr;

      queue_p = (APPLICATION_QUEUE *) queue.<B>GetPointer</B>();

      if ( queue_p != nullptr )
      {
         <A HREF="CSystemTime.htm">CSystemTime</A> system_time;

         system_time.Get();

         queue_p-&gt;last_updated = system_time;
      }
   }
}</CODE></PRE>

<H2>API's Used</H2>

<UL>
<LI>CloseHandle
<LI>CreateFileMapping
<LI>GetLastError
<LI>MapViewOfFile
<LI>UnmapViewOfFile
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CSharedMemoryObject.cpp $<BR>
$Modtime: 6/26/01 10:51a $
</BODY>

</HTML>
*/
