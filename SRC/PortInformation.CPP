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
** $Workfile: PortInformation.CPP $
** $Revision: 16 $
** $Modtime: 6/26/01 10:55a $
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

Win32FoundationClasses::CPortInformation::CPortInformation()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CPortInformation::CPortInformation( _In_ PORT_INFO_1 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CPortInformation::CPortInformation( _In_ PORT_INFO_2 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CPortInformation::CPortInformation( _In_ Win32FoundationClasses::CPortInformation const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CPortInformation::~CPortInformation()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

void Win32FoundationClasses::CPortInformation::Copy( _In_ PORT_INFO_1 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   Empty();

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( source->pName != nullptr )
      {
         Name.assign(source->pName);
      }
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CPortInformation::Copy( _In_ PORT_INFO_2 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   Empty();

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( source->pPortName != nullptr )
      {
         Name.assign(source->pPortName);
      }

      if ( source->pMonitorName != nullptr )
      {
         Monitor.assign(source->pMonitorName);
      }

      if ( source->pDescription != nullptr )
      {
         Description.assign(source->pDescription);
      }

      Type = source->fPortType;

      TypeToString( Type, TypeName );
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CPortInformation::Copy( _In_ Win32FoundationClasses::CPortInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Name.assign( source.Name );
   Monitor.assign( source.Monitor );
   Type = source.Type;
   TypeToString( Type, TypeName );
}

void Win32FoundationClasses::CPortInformation::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Name.clear();
   Monitor.clear();
   Description.clear();
   TypeName.clear();
   Type = 0;
}

void Win32FoundationClasses::CPortInformation::TypeToString( _In_ DWORD type, _Out_ std::wstring& string ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   string.clear(); // Always start out with an empty string

   if ( is_flagged( type, PORT_TYPE_WRITE ) == true )
   {
      string.assign(WSTRING_VIEW(L"Write"));
   }

   if (is_flagged(type, PORT_TYPE_READ ) == true )
   {
      if ( string.empty() == false )
      {
         string.append(WSTRING_VIEW(L", "));
      }

      string.append(WSTRING_VIEW(L"Read"));
   }

   if (is_flagged(type, PORT_TYPE_REDIRECTED ) == true )
   {
       if (string.empty() == false)
       {
           string.append(WSTRING_VIEW(L", "));
       }

      string.append(WSTRING_VIEW(L"Redirected"));
   }

   if (is_flagged(type, PORT_TYPE_NET_ATTACHED ) == true )
   {
       if (string.empty() == false)
       {
           string.append(WSTRING_VIEW(L", "));
       }

      string.append(WSTRING_VIEW(L"Net Attached"));
   }
}

Win32FoundationClasses::CPortInformation& Win32FoundationClasses::CPortInformation::operator = ( _In_ Win32FoundationClasses::CPortInformation const& source ) noexcept
{
   Copy( source );
   WFC_VALIDATE_POINTER( this );
   return( *this );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CPortInformation::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CPortInformation at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   Name        = \"" ) << Name        << TEXT( "\"\n" );
   dump_context << TEXT( "   Monitor     = \"" ) << Monitor     << TEXT( "\"\n" );
   dump_context << TEXT( "   Description = \"" ) << Description << TEXT( "\"\n" );
   dump_context << TEXT( "   Type        =  "  ) << Type        << TEXT( "\n"   );
   dump_context << TEXT( "   TypeName    = \"" ) << TypeName    << TEXT( "\"\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CPortInformation</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, RAS, Remote Access Service, Dialup networking">
</HEAD>

<BODY>

<H1>CPortInformation : CObject</H1>
$Revision: 16 $<HR><H2>Description</H2>
This class provides that same basic infotmation as the PORT_INFO_2 data structure.

<H2>Data Members</H2>

<DL COMPACT>

<DT><DFN><B>Name</B></DFN><DD>Name of the port like &quot;COM1:&quot; or &quot;LPT1:&quot;.
<DT><DFN><B>Monitor</B></DFN><DD>I ain't quite figured this one out yet.
<DT><DFN><B>Description</B></DFN><DD>Presumably the description the user typed
in when they shared this port.
<DT><DFN><B>Type</B></DFN><DD>What type the port is.
<DT><DFN><B>Type</B></DFN><DD>The type in human readable form.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><DFN><B><A NAME="Copy">Copy</A></B></DFN><DD>Copies another CPortInformation or a PORT_INFO_1
structure or a PORT_INFO_2 structure.
<DT><DFN><B><A NAME="Empty">Empty</A></B></DFN><DD>Clears all data members.
<DT><DFN><B><A NAME="Serialize">Serialize</A></B></DFN><DD>Saves/restores the object.
<DT><DFN><B><A NAME="TypeToString">TypeToString</A></B></DFN><DD>Converts the Type member variable to a human
readable string.
</DL>

<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: PortInformation.CPP $<BR>
$Modtime: 6/26/01 10:55a $
</BODY>

</HTML>
*/
