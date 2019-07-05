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
** $Workfile: CPasswordChangeFilter.cpp $
** $Revision: 16 $
** $Modtime: 6/26/01 10:50a $
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

CPasswordChangeFilter::CPasswordChangeFilter()
{
   WFC_VALIDATE_POINTER( this );
}

CPasswordChangeFilter::~CPasswordChangeFilter()
{
   WFC_VALIDATE_POINTER( this );
}

bool CPasswordChangeFilter::AddFilter( _In_ std::wstring const& dll_name ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   CRegistry registry;

   if ( registry.Connect( (HKEY) CRegistry::keyLocalMachine ) == false )
   {
      return( false );
   }

   if ( registry.Open( L"SYSTEM\\CurrentControlSet\\Control\\Lsa", (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue ) ) == FALSE )
   {
      return( false );
   }

   std::vector<std::wstring> current_filters;

   GetCurrentFilters( current_filters );

   for ( auto const& entry : current_filters )
   {
      if ( compare_no_case( entry, dll_name ) == I_AM_EQUAL_TO_THAT )
      {
         // The filter is already added. That was silly.
         return( true );
      }
   }

   (void) current_filters.push_back( dll_name );

   if ( registry.SetStringArrayValue( L"Notification Packages", current_filters ) == false )
   {
      return( false );
   }

   return( true );
}

bool CPasswordChangeFilter::GetCurrentFilters( _Out_ std::vector<std::wstring>& current_filters ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   current_filters.clear();

   CRegistry registry;

   if ( registry.Connect( (HKEY) CRegistry::keyLocalMachine ) == FALSE )
   {
      return( false );
   }

   if ( registry.Open( L"SYSTEM\\CurrentControlSet\\Control\\Lsa", (CRegistry::CreatePermissions)(CRegistry::permissionRead) ) == FALSE )
   {
      return( false );
   }

   if ( registry.GetStringArrayValue( L"Notification Packages", current_filters ) == false )
   {
      current_filters.clear();
      return( false );
   }

   return( true );
}

bool CPasswordChangeFilter::OnChanged( __in PASSWORD_FILTER_STRING * /* user_name */, __in ULONG /* relative_id */, __in PASSWORD_FILTER_STRING * /* new_password */ ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( true );
}

bool CPasswordChangeFilter::OnFilter( __in PASSWORD_FILTER_STRING * /* user_name */, __in PASSWORD_FILTER_STRING * /* full_user_name */, __in PASSWORD_FILTER_STRING * /* new_password */, __in bool /* was_set */ ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( true );
}

bool CPasswordChangeFilter::OnInitialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( true );
}

__checkReturn bool CPasswordChangeFilter::RemoveFilter( __in std::wstring const& dll_name ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   CRegistry registry;

   if ( registry.Connect( (HKEY) CRegistry::keyLocalMachine ) == FALSE )
   {
      return( false );
   }

   if ( registry.Open( L"SYSTEM\\CurrentControlSet\\Control\\Lsa", (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue ) ) == FALSE )
   {
      return( false );
   }

   std::vector<std::wstring> current_filters;

   GetCurrentFilters( current_filters );

   std::size_t index = 0;
   std::size_t number_of_filters = current_filters.size();

   std::wstring filter_name;

   while( index < number_of_filters )  // Can't be converted to range loop
   {
      filter_name.assign( current_filters.at( index ) );

      if ( compare_no_case( filter_name, dll_name ) == I_AM_EQUAL_TO_THAT )
      {
         // We found one.
         current_filters.erase(current_filters.cbegin() + index );
         number_of_filters--;
      }
      else
      {
         index++;
      }
   }

   (void) current_filters.push_back( dll_name );

   if ( registry.SetStringArrayValue( L"Notification Packages", current_filters ) == false )
   {
      return( false );
   }

   return( true );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CPasswordChangeFilter</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that intercepts password changes on a domain.">
</HEAD>

<BODY>
<H1>CPasswordChangeFilter</H1>
$Revision: 16 $<BR><HR>

<H2>Description</H2>
This class makes it easy to filter password changes in NT.

<H2>Data Members</H2>None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="AddFilter">AddFilter</A></B>( const std::wstring&amp; dll_name )</PRE><DD>
This method adds the <CODE>dll_name</CODE> to the list of DLL's
that will be called when a password changes.

<DT><PRE>BOOL <B><A NAME="GetCurrentFilters">GetCurrentFilters</A></B>( std:::vector&lt;std::wstring&gt;&amp; current_filters )</PRE><DD>
Retrieves a list of filters currently in use. These are the names of the DLL's
that are called when a user's password changes.

<DT><PRE>BOOL <B><A NAME="OnChanged">OnChanged</A></B>( PASSWORD_FILTER_STRING * user_name, ULONG relative_id, PASSWORD_FILTER_STRING * new_password )</PRE><DD>
This is called when the user changed their password. This is where you would
do smurphy things like change passwords on other applications (or operating
operating systems). This is the key to &quot;single user logon&quot; setups.
Whenever the user changes their password, <B>OnChanged</B>() will get called
allowing you to take the appropriate actions to change their password any
where else it needs changing.

<DT><PRE>BOOL <B><A NAME="OnFilter">OnFilter</A></B>( PASSWORD_FILTER_STRING * user_name, PASSWORD_FILTER_STRING * full_user_name, PASSWORD_FILTER_STRING * new_password, BOOL was_set )</PRE><DD>
Called when a user is changing their password. This is where you would
put your code to &quot;score&quot; a user's typed in password. If it
does not meet your randomness criteria, return FALSE. If the password
is OK, then return TRUE. The default is to return TRUE.

<DT><PRE>BOOL <B><A NAME="OnInitialize">OnInitialize</A></B>( void )</PRE><DD>
Returns TRUE. Override it to do what you want.

<DT><PRE>BOOL <B><A NAME="RemoveFilter">RemoveFilter</A></B>( const std::wstring&amp; dll_name )</PRE><DD>
This method removes the <CODE>dll_name</CODE> from the list of DLL's
that will be called when a password changes.

</DL>

<H2>Notes</H2>

Notifications and filtering only takes place on the machine that holds
the passwords. This could be the Primary Domain Controller (PDC) if you are
using domains. If you are using domains, don't forget to install the
password filter on the Backup Domain Controllers (BDC) so the filters
will continue if an election was forced (i.e. role reversal).

<H2>Example</H2><PRE><CODE>void _tmain()
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   <B>CPasswordChangeFilter</B> filter;

   filter.AddFilter( TEXT( &quot;e:\\wfc\\sample\\passwordfilter\\dll\\password.dll&quot; ) );
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CPasswordChangeFilter.cpp $<BR>
$Modtime: 6/26/01 10:50a $
</BODY>

</HTML>
*/
