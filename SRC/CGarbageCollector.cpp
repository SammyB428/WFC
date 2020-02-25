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
** $Workfile: CGarbageCollector.cpp $
** $Revision: 20 $
** $Modtime: 6/26/01 10:47a $
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

Win32FoundationClasses::CGarbageCollector::CGarbageCollector() noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_ReferenceCount = 0;
   m_SelfDestruct   = FALSE;
}

Win32FoundationClasses::CGarbageCollector::~CGarbageCollector() noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_ReferenceCount = 0;
   m_SelfDestruct   = FALSE;
}

void Win32FoundationClasses::CGarbageCollector::AddReference( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   _InterlockedIncrement( (long *) &m_ReferenceCount );

   //WFCTRACEVAL( TEXT( "Reference count is now " ), m_ReferenceCount );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CGarbageCollector::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CGarbageCollector at " ) << (VOID *) this    << TEXT( "\n{\n" );
   dump_context << TEXT( "   m_ReferenceCount is " ) << m_ReferenceCount << TEXT( "\n" );
   dump_context << TEXT( "   m_SelfDestruct is   " );

   if ( m_SelfDestruct == FALSE )
   {
      dump_context << TEXT( "False\n" );
   }
   else
   {
      dump_context << TEXT( "True\n" );
   }

   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ long Win32FoundationClasses::CGarbageCollector::GetReferenceCount( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_ReferenceCount );
}

_Check_return_ bool Win32FoundationClasses::CGarbageCollector::GetSelfDestruct( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_SelfDestruct );
}

void Win32FoundationClasses::CGarbageCollector::Release(__out_opt bool * object_was_deleted ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( object_was_deleted );

   if ( m_ReferenceCount <= 0 )
   {
      ASSERT( FALSE );
   }

   WFC_TRY
   {
      if ( _InterlockedDecrement( &m_ReferenceCount ) == 0 )
      {
         if ( m_SelfDestruct == true)
         {
            if ( object_was_deleted != nullptr )
            {
               *object_was_deleted = true;
            }

            delete this;
         }
         else
         {
            if ( object_was_deleted != nullptr )
            {
               *object_was_deleted = false;
            }
         }
      }
      else
      {
         if ( object_was_deleted != nullptr )
         {
            *object_was_deleted = false;
         }
      }
   }
   WFC_CATCH_ALL
   {
      // Do Nothing
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CGarbageCollector::SetSelfDestruct( _In_ bool const self_destruct ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( self_destruct == true )
   {
      m_SelfDestruct = true;
   }
   else
   {
      m_SelfDestruct = false;
   }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CGarbageCollector</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that makes it easy to implement copy-on-write (reference counted) objects.">
</HEAD>

<BODY>

<H1>CGarbageCollector</H1>
$Revision: 20 $<HR>

<H2>Description</H2>
This class implements a simple garbage collector. You inherit from
<B>CGarbageCollector</B> and your class will self destruct when the
reference count reaches zero. <B>CGarbageCollector</B> should only
be set to self destruct when it has been created via <CODE>new</CODE>.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="AddReference">AddReference</A></B>( void )</PRE><DD>
This increments the reference count and prevents the
object from being deleted.

<DT><PRE>void <B><A NAME="Dump">Dump</A></B>( CDumpContext&amp; dump_context ) const</PRE><DD>
Present only in debug builds. Dumps interesting information about
<B>CGarbageCollector</B> to the <CODE>dump_context</CODE> provided.

<DT><PRE>BOOL <B><A NAME="GetSelfDestruct">GetSelfDestruct</A></B>( void ) const</PRE><DD>
Returns whether this object will self destruct when the
reference count reaches zero.

<DT><PRE>long <B><A NAME="GetReferenceCount">GetReferenceCount</A></B>( void ) const</PRE><DD>
Returns the reference count.

<DT><PRE>void <B><A NAME="Release">Release</A></B>( BOOL * deleted = nullptr )</PRE><DD>
Decrements the reference count and checks to see if it is zero.
If the reference count is zero, the object will self destruct
(i.e. call <CODE>delete this</CODE>). If <CODE>deleted</CODE> is
not NULL, it will be filled with TRUE if the object was deleted
or FALSE if there are still references to it somewhere.

<DT><PRE>void <B><A NAME="SetSelfDestruct">SetSelfDestruct</A></B>( BOOL self_destruct = TRUE )</PRE><DD>
This should be called after <B>CGarbageCollector</B> has been
created via <CODE>new</CODE>.

</DL>

<H2>Example</H2><PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

class CThreadData : public <B>CGarbageCollector</B>
{
   protected:

      DWORD m_Data{0};

   public:

      void SetData( DWORD data ) { m_Data = data; };
      DWORD GetData( void ) const { return( m_Data ); };
};

void worker_thread( void * parameter )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;worker_thread()&quot; ) );

   CThreadData * thread_data = (CThreadData *) parameter;

   // Make sure the thread data is around until we call Release
   thread_data-&gt;AddReference();

   // Go do something that takes a really long time

   std::this_thread::sleep_for(std::chrono::milliseconds(10000));

   thread_data-&gt;Release();
}

void start_thread( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;start_thread()&quot; ) );

   CThreadData * data_p = nullptr;
   
   WFC_TRY
   {
      data_p = new CThreadData;
   }
   WFC_CATCH_ALL
   {
      return;
   }
   WFC_END_CATCH_ALL

   data_p-&gt;AddReference();
   data_p-&gt;SetSelfDestruct( TRUE );

   _beginthread( worker_thread, DEFAULT_THREAD_STACK_SIZE, data_p );

   std::this_thread::sleep_for(std::chrono::milliseconds(1));

   data_p-&gt;Release();
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CGarbageCollector.cpp $<BR>
$Modtime: 6/26/01 10:47a $
</BODY>

</HTML>
*/
