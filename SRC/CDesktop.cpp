/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2018, Samuel R. Blackburn
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
** $Workfile: CDesktop.cpp $
** $Revision: 22 $
** $Modtime: 6/26/01 10:45a $
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

Win32FoundationClasses::CDesktop::CDesktop() noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_AutomaticallyClose = false;
   m_DesktopHandle      = static_cast< HDESK >( NULL );
   m_ErrorCode          = 0;
}

Win32FoundationClasses::CDesktop::CDesktop( _In_ HDESK desktop_handle ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_AutomaticallyClose = false;
   m_DesktopHandle      = static_cast< HDESK >( NULL );
   m_ErrorCode          = 0;

   Attach( desktop_handle );
}

Win32FoundationClasses::CDesktop::~CDesktop()
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AutomaticallyClose == true)
   {
       std::ignore = Close();
   }

   m_AutomaticallyClose = false;
   m_DesktopHandle      = static_cast< HDESK >( NULL );
   m_ErrorCode          = 0;
}

void Win32FoundationClasses::CDesktop::Attach( _In_ HDESK desktop_handle ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_DesktopHandle not_eq static_cast< HDESK >( NULL ) and m_AutomaticallyClose == true)
   {
       std::ignore = Close();
   }

   m_DesktopHandle      = desktop_handle;
   m_AutomaticallyClose = false; // We didn't open it so don't close it
}

_Check_return_ bool Win32FoundationClasses::CDesktop::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   BOOL return_value{ TRUE };

   if ( m_DesktopHandle not_eq static_cast< HDESK >( NULL ) )
   {
      return_value = ::CloseDesktop( m_DesktopHandle );

      if ( return_value == FALSE )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACE( TEXT( "CloseDesktop() failed" ) );
         //WFCTRACEERROR( m_ErrorCode );
      }
      else
      {
         m_DesktopHandle = static_cast< HDESK >( NULL );
         return_value    = TRUE;
      }
   }
   else
   {
      //WFCTRACE( TEXT( "Desktop already closed." ) );
   }

   return( return_value == FALSE ? false : true );
}

_Check_return_ bool Win32FoundationClasses::CDesktop::Create( _In_ std::wstring_view name_of_desktop, _In_ DWORD desired_access, __in_opt LPSECURITY_ATTRIBUTES security_attributes_p, _In_ DWORD flags ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( security_attributes_p );

   bool return_value{ false };

   if ( m_DesktopHandle not_eq static_cast< HDESK >( NULL ) and m_AutomaticallyClose == true)
   {
       std::ignore = Close();
   }

   m_AutomaticallyClose = false;

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // The first parameter is mis-typed, it should be const but it ain't.
      // This means we must code around it.

      wchar_t desktop_name[ 513 ];

      ::ZeroMemory( desktop_name, sizeof( desktop_name ) );
      
      std::ignore = wcsncpy_s( desktop_name, std::size( desktop_name ), name_of_desktop.data(), name_of_desktop.length() );

      // CreateDesktop is not const correct

      m_DesktopHandle = ::CreateDesktopW( desktop_name,
                                         nullptr, // because the docs say so
                                         nullptr, // because the docs say so
                                         flags,
                                         desired_access,
                                         security_attributes_p );

      if ( m_DesktopHandle == static_cast< HDESK >( NULL ) )
      {
         m_ErrorCode = ::GetLastError();
         //WFCTRACEERROR( m_ErrorCode );
         return_value = false;
      }
      else
      {
         m_AutomaticallyClose = true; // We created it, we'll kill it
         return_value = true;
      }

      return( return_value );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( false );
   }
   WFC_END_CATCH_ALL
}

static BOOL CALLBACK CDesktop__WindowEnumerator( HWND window_handle, LPARAM lParam ) noexcept
{
   WFC_VALIDATE_POINTER_NULL_OK( lParam );

   auto window_array = reinterpret_cast<std::vector<HWND> *>( lParam );

   // We were passed a pointer by the caller, don't trust it

   __try
   {
      if ( window_array not_eq nullptr )
      {
         window_array->push_back( window_handle );
      }
   }
   __except( EXCEPTION_EXECUTE_HANDLER )
   {
      return( FALSE );
   }

   return( TRUE );
}

_Check_return_ bool Win32FoundationClasses::CDesktop::GetWindows( _Out_ std::vector<HWND>& window_handles ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Always start out with a known state of the array

   window_handles.clear();

   bool return_value{ false };

   if ( m_DesktopHandle == static_cast<HDESK>( NULL ) )
   {
      //WFCTRACE( TEXT( "Desktop is not yet opened" ) );
      m_ErrorCode = ERROR_INVALID_HANDLE;
      return( false );
   }

   return_value = ::EnumDesktopWindows( m_DesktopHandle, CDesktop__WindowEnumerator, (LPARAM) &window_handles );

   if ( return_value == false )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
   }
   else
   {
      return_value = true;
   }

   return( return_value );
}

void Win32FoundationClasses::CDesktop::GetThread( _In_ DWORD thread_id ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( thread_id not_eq 0 )
   {
      m_DesktopHandle = ::GetThreadDesktop( thread_id );
   }
   else
   {
      m_DesktopHandle = ::GetThreadDesktop( ::GetCurrentThreadId() );
   }
}

_Check_return_ bool Win32FoundationClasses::CDesktop::Open( _In_ std::wstring_view name_of_desktop, _In_ DWORD desired_access, _In_ DWORD flags, _In_ bool inheritable ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   bool return_value{ false };

   if ( m_DesktopHandle not_eq static_cast< HDESK >( NULL ) and m_AutomaticallyClose == true)
   {
       std::ignore = Close();
   }

   m_AutomaticallyClose = false;

   // The first parameter is mis-typed, it should be const but it ain't.
   // This means we must code around it

   wchar_t desktop_name[ 513 ];

   ::ZeroMemory( desktop_name, sizeof( desktop_name ) );
      
   std::ignore = wcsncpy_s( desktop_name, std::size( desktop_name ), name_of_desktop.data(), name_of_desktop.length() );

   // OpenDesktop() is not const correct

   m_DesktopHandle = ::OpenDesktopW( desktop_name, flags, inheritable, desired_access );

   if ( m_DesktopHandle == static_cast< HDESK >( NULL ) )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
      return_value = false;
   }
   else
   {
      return_value = true;
   }

   return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CDesktop::OpenInput( _In_ DWORD desired_access, _In_ DWORD flags, _In_ bool inheritable ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   bool return_value{ false };

   if ( m_DesktopHandle not_eq static_cast< HDESK >( NULL ) and m_AutomaticallyClose == true)
   {
       std::ignore = Close();
   }

   m_AutomaticallyClose = false;

   m_DesktopHandle = ::OpenInputDesktop( flags, inheritable, desired_access );

   if ( m_DesktopHandle == static_cast< HDESK >( NULL ) )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
      return_value = false;
   }
   else
   {
      return_value = true;
   }

   return( return_value );
}

void Win32FoundationClasses::CDesktop::SetAutomaticallyClose( _In_ bool const automatically_close ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   if ( automatically_close == false )
   {
      m_AutomaticallyClose = false;
   }
   else
   {
      m_AutomaticallyClose = true;
   }
}

_Check_return_ bool Win32FoundationClasses::CDesktop::SetThread( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto return_value{ ::SetThreadDesktop(m_DesktopHandle) };

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value == FALSE ? false : true );
}

_Check_return_ bool Win32FoundationClasses::CDesktop::SwitchTo( _In_ Win32FoundationClasses::CDesktop const& desktop_to_switch_to ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto return_value{ ::SwitchDesktop(desktop_to_switch_to.m_DesktopHandle) };

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value == FALSE ? false : true );
}

Win32FoundationClasses::CDesktop::operator HDESK () const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_DesktopHandle );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CDesktop</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles the Win32 Desktop (not to be confused with the GUI desktop).">
</HEAD>

<BODY>

<H1>CDesktop</H1>
$Revision: 22 $<BR>
<HR>

<H2>Description</H2>
This class allows you to play with desktops.

<H2>Data Members</H2>
None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Attach">Attach</A></B>( HDESK desktop_handle )</PRE><DD>
Attaches an existing desktop handle to this object.

<DT><PRE>BOOL <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the desktop.

<DT><PRE>BOOL <B><A NAME="Create">Create</A></B>( const std::wstring&amp; name_of_desktop,
                   DWORD    desired_access = DESKTOP_CREATEWINDOW,
      LPSECURITY_ATTRIBUTES security_attributes_p = nullptr,
                   LPCTSTR  display_device_name = nullptr,
                   DWORD    flags = 0 )</PRE><DD>
Creates a new desktop.

<DT><PRE>BOOL <B><A NAME="GetAutomaticallyClose">GetAutomaticallyClose</A></B>( void ) const</PRE><DD>
Returns whether the encapsulated desktop will be closed in the
destructor.

<DT><PRE>DWORD <B><A NAME="GetErrorCode">GetErrorCode</A></B>( void ) const</PRE><DD>
Returns the last error encountered.

<DT><PRE>HDESK <B><A NAME="GetHandle">GetHandle</A></B>( void ) const</PRE><DD>
Returns the encapsulated HDESK handle.

<DT><PRE>void <B><A NAME="GetThread">GetThread</A></B>( DWORD thread_id = 0 )</PRE><DD>
Attaches this <B>CDesktop</B> to the dekstop of the specified
<CODE>thread_id</CODE>.

<DT><PRE>BOOL <B><A NAME="GetWindows">GetWindows</A></B>( std::vector&lt;HWND&gt;&amp; window_handles )</PRE><DD>
Gathers a list of all window handles for the desktop.

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( const std::wstring&amp; desktop_name,  DWORD desired_access = DESKTOP_CREATEWINDOW, DWORD flags = 0, BOOL inherit = FALSE )</PRE><DD>
Opens the desktop.

<DT><PRE>BOOL <B><A NAME="OpenInput">OpenInput</A></B>( DWORD desired_access = accessCreateWindow, DWORD flags = 0, BOOL inherit = FALSE )</PRE><DD>
Opens the desktop for input.

<DT><PRE>void <B><A NAME="SetAutomaticallyClose">SetAutomaticallyClose</A></B>( BOOL automatically_close = TRUE )</PRE><DD>
Tells <B>CDesktop</B> to call <B>Close</B>() in the destructor.

<DT><PRE>BOOL <B><A NAME="SetThread">SetThread</A></B>( void )</PRE><DD>
Sets the current thread's desktop to this <B>CDesktop</B>.

<DT><PRE>BOOL <B><A NAME="SwitchTo">SwitchTo</A></B>( const CDesktop&amp; new_desktop )</PRE><DD>
Makes the <CODE>new_desktop</CODE> visible and activates it.

</DL>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<H2>API's Used</H2>
<UL>
<LI>CloseDesktop
<LI>EnumDesktopWindows
<LI>GetCurrentThreadId
<LI>GetLastError
<LI>GetThreadDesktop
<LI>SetThreadDesktop
<LI>SwitchDesktop
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CDesktop.cpp $<BR>
$Modtime: 6/26/01 10:45a $
</BODY>

</HTML>
*/
