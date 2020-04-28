/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2020, Samuel R. Blackburn
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
** $Workfile: CNetworkConnectionInformation.cpp $
** $Revision: 12 $
** $Modtime: 6/26/01 10:49a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CNetworkConnectionInformation::CNetworkConnectionInformation()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CNetworkConnectionInformation::CNetworkConnectionInformation( _In_ CONNECTION_INFO_1 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkConnectionInformation::CNetworkConnectionInformation( _In_ Win32FoundationClasses::CNetworkConnectionInformation const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CNetworkConnectionInformation::~CNetworkConnectionInformation()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void Win32FoundationClasses::CNetworkConnectionInformation::Copy( _In_ CONNECTION_INFO_1 const * source ) noexcept
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
      UserName.assign( source->coni1_username );
      NetName.assign(  source->coni1_netname );

      ID            = source->coni1_id;
      Type          = source->coni1_type;
      NumberOfOpens = source->coni1_num_opens;
      NumberOfUsers = source->coni1_num_users;
      Time          = source->coni1_time;
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkConnectionInformation::Copy( _In_ Win32FoundationClasses::CNetworkConnectionInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this not_eq &source );

   /*
   ** Check to make sure we ain't copying ourselves
   */

   if ( this == &source )
   {
      return;
   }

   ID            = source.ID;
   Type          = source.Type;
   NumberOfOpens = source.NumberOfOpens;
   NumberOfUsers = source.NumberOfUsers;
   Time          = source.Time;
   UserName.assign( source.UserName );
   NetName.assign( source.NetName );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CNetworkConnectionInformation::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );

   dump_context << TEXT( "ID = "            ) << ID            << TEXT( "\n"   );
   dump_context << TEXT( "Type = "          ) << Type          << TEXT( "\n"   );
   dump_context << TEXT( "NumberOfOpens = " ) << NumberOfOpens << TEXT( "\n"   );
   dump_context << TEXT( "NumberOfUsers = " ) << NumberOfUsers << TEXT( "\n"   );
   dump_context << TEXT( "Time = "          ) << Time          << TEXT( "\n"   );
   dump_context << TEXT( "UserName = \""    ) << UserName      << TEXT( "\"\n" );
   dump_context << TEXT( "NetName = \""     ) << NetName       << TEXT( "\"\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CNetworkConnectionInformation::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void Win32FoundationClasses::CNetworkConnectionInformation::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   UserName.clear();
   NetName.clear();
   ID            = 0;
   Type          = 0;
   NumberOfUsers = 0;
   NumberOfOpens = 0;
   Time          = 0;
}

Win32FoundationClasses::CNetworkConnectionInformation const& Win32FoundationClasses::CNetworkConnectionInformation::operator=( _In_ Win32FoundationClasses::CNetworkConnectionInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this not_eq &source );

   if ( this not_eq &source )
   {
      Copy( source );
   }

   return( *this );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CNetworkConnectionInformation</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles network connections information.">
</HEAD>

<BODY>

<H1>CNetworkConnectionInformation : CObject</H1>
$Revision: 12 $<BR>
<HR>

<H2>Description</H2>
This class allows you to enumerate the network connections.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CNetworkConnectionInformation</B>()
<B>CNetworkConnectionInformation</B>( LPCTSTR machine_name )</PRE><DD>
Constructs the object.

</DL>

<H2>Data Members</H2>
None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the connection to the machine.

<DT><PRE>BOOL <B><A NAME="Enumerate">Enumerate</A></B>( LPCTSTR share_or_computer_name )</PRE><DD>
Initializes the object for enumeration.

<DT><PRE>BOOL <B><A NAME="GetNext">GetNext</A></B>( CNetworkConnectionInformation&amp; information )</PRE><DD>
Retrieves the next set of connection information.

<DT><PRE>void <B><A NAME="Serialize">Serialize</A></B>( CArchive&amp; archive )</PRE><DD>
After all we are a CObject...

</DL>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<H2>API's Used</H2>

<B>CNetworkConnections</B> uses the following API's:

<UL>
<LI>NetApiBufferFree
<LI>NetConnectionEnum
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CNetworkConnectionInformation.cpp $<BR>
$Modtime: 6/26/01 10:49a $
</BODY>

</HTML>
*/
