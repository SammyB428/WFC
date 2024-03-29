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
** $Workfile: CMixerControlDetailsData.cpp $
** $Revision: 18 $
** $Modtime: 6/26/01 10:48a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#endif // _DEBUG

// Construction

Win32FoundationClasses::CMixerControlDetailsData::CMixerControlDetailsData()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CMixerControlDetailsData::CMixerControlDetailsData( _In_ Win32FoundationClasses::CMixerControlDetailsData const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CMixerControlDetailsData::CMixerControlDetailsData( _In_ MIXERCONTROLDETAILS_LISTTEXT const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CMixerControlDetailsData::CMixerControlDetailsData( _In_ MIXERCONTROLDETAILS_BOOLEAN const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CMixerControlDetailsData::CMixerControlDetailsData( _In_ MIXERCONTROLDETAILS_SIGNED const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CMixerControlDetailsData::CMixerControlDetailsData( _In_ MIXERCONTROLDETAILS_UNSIGNED const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

void Win32FoundationClasses::CMixerControlDetailsData::Copy( _In_ Win32FoundationClasses::CMixerControlDetailsData const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   Parameter1 = source.Parameter1;
   Parameter2 = source.Parameter2;
   Name.assign(source.Name);
}

void Win32FoundationClasses::CMixerControlDetailsData::Copy( _In_ MIXERCONTROLDETAILS_LISTTEXT const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Parameter1 = source.dwParam1;
   Parameter2 = source.dwParam2;
   Name.assign(source.szName);
}

void Win32FoundationClasses::CMixerControlDetailsData::Copy( _In_ MIXERCONTROLDETAILS_BOOLEAN const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Parameter1 = source.fValue;
   Parameter2 = 0;
   Name.clear();
}

void Win32FoundationClasses::CMixerControlDetailsData::Copy( _In_ MIXERCONTROLDETAILS_SIGNED const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Parameter1 = source.lValue;
   Parameter2 = 0;
   Name.clear();
}

void Win32FoundationClasses::CMixerControlDetailsData::Copy( _In_ MIXERCONTROLDETAILS_UNSIGNED const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Parameter1 = source.dwValue;
   Parameter2 = 0;
   Name.clear();
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CMixerControlDetailsData::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );

   std::wstring temp_string;

   dump_context << TEXT( "a CMixerControlDetailsData at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   Parameter1 is " ) << Parameter1 << TEXT( "\n" );
   dump_context << TEXT( "   Parameter2 is " ) << Parameter2;

   if ( Name.IsEmpty() == false )
   {
      if ( Parameter2 == CMixerLine::sourceUndefined )
      {
         temp_string = TEXT( "sourceUndefined" );
      }
      else if ( Parameter2 == CMixerLine::sourceDigital )
      {
         temp_string = TEXT( "sourceDigital" );
      }
      else if ( Parameter2 == CMixerLine::sourceLine )
      {
         temp_string = TEXT( "sourceLine" );
      }
      else if ( Parameter2 == CMixerLine::sourceMicrophone )
      {
         temp_string = TEXT( "sourceMicrophone" );
      }
      else if ( Parameter2 == CMixerLine::sourceSynthesizer )
      {
         temp_string = TEXT( "sourceSynthesizer" );
      }
      else if ( Parameter2 == CMixerLine::sourceCompactDisc )
      {
         temp_string = TEXT( "sourceCompactDisc" );
      }
      else if ( Parameter2 == CMixerLine::sourceTelephone )
      {
         temp_string = TEXT( "sourceTelephone" );
      }
      else if ( Parameter2 == CMixerLine::sourcePCSpeaker )
      {
         temp_string = TEXT( "sourcePCSpeaker" );
      }
      else if ( Parameter2 == CMixerLine::sourceWaveOut )
      {
         temp_string = TEXT( "sourceWaveOut" );
      }
      else if ( Parameter2 == CMixerLine::sourceAuxillary )
      {
         temp_string = TEXT( "sourceAuxillary" );
      }
      else if ( Parameter2 == CMixerLine::sourceAnalog )
      {
         temp_string = TEXT( "sourceAnalog" );
      }
      else
      {
         temp_string = TEXT( "is invalid" );
      }

      dump_context << TEXT( " ( " ) << temp_string << TEXT( " )" );
   }

   dump_context << TEXT( "\n   Name is " ) << Name << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CMixerControlDetailsData::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Parameter1 = 0;
   Parameter2 = 0;
   Name.clear();
}

Win32FoundationClasses::CMixerControlDetailsData& Win32FoundationClasses::CMixerControlDetailsData::operator = ( _In_ Win32FoundationClasses::CMixerControlDetailsData const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CMixerControlDetailsData& Win32FoundationClasses::CMixerControlDetailsData::operator = ( _In_ MIXERCONTROLDETAILS_LISTTEXT const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CMixerControlDetailsData& Win32FoundationClasses::CMixerControlDetailsData::operator = ( _In_ MIXERCONTROLDETAILS_BOOLEAN const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CMixerControlDetailsData& Win32FoundationClasses::CMixerControlDetailsData::operator = ( _In_ MIXERCONTROLDETAILS_SIGNED const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CMixerControlDetailsData& Win32FoundationClasses::CMixerControlDetailsData::operator = ( _In_ MIXERCONTROLDETAILS_UNSIGNED const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
