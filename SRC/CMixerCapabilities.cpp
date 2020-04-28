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
** $Workfile: CMixerCapabilities.cpp $
** $Revision: 20 $
** $Modtime: 6/26/01 10:48a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

Win32FoundationClasses::CMixerCapabilities::CMixerCapabilities()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CMixerCapabilities::CMixerCapabilities( _In_ Win32FoundationClasses::CMixerCapabilities const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CMixerCapabilities::CMixerCapabilities( _In_ MIXERCAPS const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CMixerCapabilities::~CMixerCapabilities()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

void Win32FoundationClasses::CMixerCapabilities::Copy( _In_ Win32FoundationClasses::CMixerCapabilities const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      //WFCTRACE( TEXT( "Attempt to make a copy of ourself (such silliness)." ) );
      return;
   }

   Manufacturer         = source.Manufacturer;
   Product              = source.Product;
   Version              = source.Version;
   ProductName.assign( source.ProductName );
   Support              = source.Support;
   NumberOfDestinations = source.NumberOfDestinations;
}

void Win32FoundationClasses::CMixerCapabilities::Copy( _In_ MIXERCAPS const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Manufacturer         = source.wMid;
   Product              = source.wPid;
   Version              = source.vDriverVersion;
   ProductName.assign(source.szPname);
   Support              = source.fdwSupport;
   NumberOfDestinations = source.cDestinations;
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CMixerCapabilities::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   dump_context << TEXT( "a CMixerCapabilities at "    ) << (VOID *) this        << TEXT( "\n{\n" );
   dump_context << TEXT( "   Manufacturer is         " ) << Manufacturer         << TEXT( "\n" );
   dump_context << TEXT( "   Product is              " ) << Product              << TEXT( "\n" );
   dump_context << TEXT( "   Version is              " ) << Version              << TEXT( "\n" );
   dump_context << TEXT( "   ProductName is          " ) << ProductName          << TEXT( "\n" );
   dump_context << TEXT( "   Support is              " ) << Support              << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfDestinations is " ) << NumberOfDestinations << TEXT( "\n}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CMixerCapabilities::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Manufacturer         = 0;
   Product              = 0;
   Version              = 0;
   ProductName.clear();
   Support              = 0;
   NumberOfDestinations = 0;
}

_Check_return_ bool Win32FoundationClasses::CMixerCapabilities::Get( _In_ UINT const device_number ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   MIXERCAPS mixer_capabilities;

   ::ZeroMemory( &mixer_capabilities, sizeof( mixer_capabilities ) );

   const MMRESULT result = ::mixerGetDevCaps( device_number, &mixer_capabilities, sizeof( mixer_capabilities ) );

   if ( result not_eq MMSYSERR_NOERROR )
   {
      // Leave us with that empty feeling
      Empty();
      return( false );
   }

   Copy( mixer_capabilities );

   return( true );
}

Win32FoundationClasses::CMixerCapabilities& Win32FoundationClasses::CMixerCapabilities::operator = ( _In_ Win32FoundationClasses::CMixerCapabilities const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CMixerCapabilities& Win32FoundationClasses::CMixerCapabilities::operator = ( _In_ MIXERCAPS const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CMixerCapabilities</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles Mixer capabilities.">
</HEAD>

<BODY>

<H1>CMixerCapabilities : CObject</H1>
$Revision: 20 $<BR><HR>

<H2>Description</H2>
This class encapsulates the MIXERCAPS data structure.

<H2>Data Members</H2>

<DL>

<DT><B>Manufacturer</B><DD>Holds the manufacturer's identifier.
<DT><B>Product</B><DD>Holds the product identifier.
<DT><B>Version</B><DD>Holds the version of the driver.
<DT><B>ProductName</B><DD>Holds the name of the mixer device.
<DT><B>Support</B><DD>A 32-bit bitmap mask of supported functions. The only
problem is there are no definitions for the bits so this member is basically
worthless.
<DT><B>NumberOfDestinations</B><DD>Holds the number of places you can
route audio output to. There will always be at least one destination (i.e. speakers).

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><B><A NAME="Copy">Copy</A></B><DD>Copies the contents of another CMixerCapabilities or a
MIXERCAPS data structure.

<DT><B><A NAME="Empty">Empty</A></B><DD>Zeroizes the data members.

<DT><B><A NAME="Get">Get</A></B><DD>Retrieves the mixer's capabilities via CMixerCapabilities.

<DT><B><A NAME="Serialize">Serialize</A></B><DD>After all, we are a CObject...

</DL>

<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CMixerCapabilities.cpp $<BR>
$Modtime: 6/26/01 10:48a $
</BODY>

</HTML>
*/
