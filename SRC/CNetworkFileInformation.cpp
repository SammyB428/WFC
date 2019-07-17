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
** $Workfile: CNetworkFileInformation.cpp $
** $Revision: 11 $
** $Modtime: 6/26/01 10:49a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

USING_WFC_NAMESPACE

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

CNetworkFileInformation::CNetworkFileInformation()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CNetworkFileInformation::CNetworkFileInformation( __in FILE_INFO_3 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CNetworkFileInformation::CNetworkFileInformation( __in CNetworkFileInformation const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CNetworkFileInformation::~CNetworkFileInformation()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void CNetworkFileInformation::Copy( __in FILE_INFO_3 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      m_Initialize();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      PathName.assign( source->fi3_pathname );
      UserName.assign( source->fi3_username );

      ID            = source->fi3_id;
      Permissions   = source->fi3_permissions;
      NumberOfLocks = source->fi3_num_locks;
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
      return;
   }
   WFC_END_CATCH_ALL
}

void CNetworkFileInformation::Copy( __in CNetworkFileInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this != &source );

   /*
   ** Make sure we ain't copying ourselves
   */

   if ( this == &source )
   {
      return;
   }

   ID            = source.ID;
   Permissions   = source.Permissions;
   NumberOfLocks = source.NumberOfLocks;
   PathName.assign(source.PathName);
   UserName.assign(source.UserName);
}

void CNetworkFileInformation::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void CNetworkFileInformation::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   UserName.clear();
   PathName.clear();
   ID            = 0;
   Permissions   = 0;
   NumberOfLocks = 0;
}

CNetworkFileInformation const& CNetworkFileInformation::operator = ( __in CNetworkFileInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this != &source );

   if ( this != &source )
   {
      Copy( source );
   }

   return( *this );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CNetworkFileInformation</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles network files information.">
</HEAD>

<BODY>
<H1>CNetworkFileInformation : CObject</H1>
$Revision: 11 $<BR>
<HR>

<H2>Description</H2>
This class allows you to NetFilexxx API in Win32.

<H2>Data Members</H2>
None.

<H2>Methods</H2>

<DL COMPACT>

<DT><DFN><B><A NAME="Close">Close</A></B></DFN><DD>Closes the connection to the machine. If you pass
a CNetworkFileInformation object to Close, it will close that opened file.

<DT><DFN><B><A NAME="Enumerate">Enumerate</A></B></DFN><DD>Initializes the object for enumeration.

<DT><DFN><B><A NAME="GetNext">GetNext</A></B></DFN><DD>Retrieves the next CNetworkFileInformation.
When you reach the end of the list, GetNext() will return FALSE.

<DT><DFN><B><A NAME="Serialize">Serialize</A></B></DFN><DD>After all we are a CObject...

</DL>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CNetworkFileInformation.cpp $<BR>
$Modtime: 6/26/01 10:49a $
</BODY>

</HTML>
*/
