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
** $Workfile: CMixerLine.cpp $
** $Revision: 16 $
** $Modtime: 6/26/01 10:48a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

USING_WFC_NAMESPACE

// Construction

CMixerLine::CMixerLine()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

CMixerLine::CMixerLine( __in const CMixerLine& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CMixerLine::CMixerLine( __in const MIXERLINE& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CMixerLine::~CMixerLine()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

// Methods

void CMixerLine::Copy( __in const CMixerLine& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   DestinationNumber   = source.DestinationNumber;
   Source              = source.Source;
   ID                  = source.ID;
   Status              = source.Status;
   Reserved            = source.Reserved;
   Component           = source.Component;
   NumberOfChannels    = source.NumberOfChannels;
   NumberOfConnections = source.NumberOfConnections;
   NumberOfControls    = source.NumberOfControls;
   Target              = source.Target;
   TargetDeviceID      = source.TargetDeviceID;
   TargetManufacturer  = source.TargetManufacturer;
   TargetProduct       = source.TargetProduct;
   TargetDriverVersion = source.TargetDriverVersion;
   ShortName.assign(source.ShortName);
   Name.assign(source.Name);
   TargetName.assign(source.TargetName);
}

void CMixerLine::Copy( __in const MIXERLINE& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DestinationNumber   = source.dwDestination;
   Source              = source.dwSource;
   ID                  = source.dwLineID;
   Status              = source.fdwLine;
   Reserved            = source.dwUser;
   Component           = source.dwComponentType;
   NumberOfChannels    = source.cChannels;
   NumberOfConnections = source.cConnections;
   NumberOfControls    = source.cControls;
   Target              = source.Target.dwType;
   TargetDeviceID      = source.Target.dwDeviceID;
   TargetManufacturer  = source.Target.wMid;
   TargetProduct       = source.Target.wPid;
   TargetDriverVersion = source.Target.vDriverVersion;
   ShortName.assign(source.szShortName);
   Name.assign(source.szName);
   TargetName.assign(source.Target.szPname);
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CMixerLine::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   std::wstring temp_string;

   dump_context << TEXT( "a CMixerLine at "           ) << (VOID *) this     << TEXT( "\n{\n" );
   dump_context << TEXT( "   ShortName is           " ) << ShortName         << TEXT( "\n" );
   dump_context << TEXT( "   Name is                " ) << Name              << TEXT( "\n" );
   dump_context << TEXT( "   DestinationNumber is   " ) << DestinationNumber << TEXT( "\n" );
   dump_context << TEXT( "   Source is              " ) << Source            << TEXT( "\n" );
   dump_context << TEXT( "   ID is                  " ) << ID                << TEXT( "\n" );

   GetStatus( temp_string );
   dump_context << TEXT( "   Status is              " ) << temp_string       << TEXT( "\n" );
   dump_context << TEXT( "   Reserved is            " ) << Reserved          << TEXT( "\n" );
   
   GetComponent( temp_string );
   dump_context << TEXT( "   Component is           " ) << temp_string         << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfChannels is    " ) << NumberOfChannels    << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfConnections is " ) << NumberOfConnections << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfControls is    " ) << NumberOfControls    << TEXT( "\n" );

   GetTarget( temp_string );
   dump_context << TEXT( "   Target is              " ) << temp_string         << TEXT( "\n" );
   dump_context << TEXT( "   TargetName is          " ) << TargetName          << TEXT( "\n" );
   dump_context << TEXT( "   TargetDeviceID is      " ) << TargetDeviceID      << TEXT( "\n" );
   dump_context << TEXT( "   TargetManufacturer is  " ) << TargetManufacturer  << TEXT( "\n" );
   dump_context << TEXT( "   TargetProduct is       " ) << TargetProduct       << TEXT( "\n" );
   dump_context << TEXT( "   TargetDriverVersion is " ) << TargetDriverVersion << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void CMixerLine::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DestinationNumber   = 0;
   Source              = 0;
   ID                  = 0;
   Status              = 0;
   Reserved            = 0;
   Component           = 0;
   NumberOfChannels    = 0;
   NumberOfConnections = 0;
   NumberOfControls    = 0;
   Target              = 0;
   TargetDeviceID      = 0;
   TargetManufacturer  = 0;
   TargetProduct       = 0;
   TargetDriverVersion = 0;
   ShortName.clear();
   Name.clear();
   TargetName.clear();
}

void CMixerLine::GetComponent( _Out_ std::wstring& component_string ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   switch( Component )
   {
      case destinationUndefined:

         component_string.assign(L"Undefined Destination" );
         break;

      case destinationDigital:

         component_string.assign(L"Digital Destination" );
         break;

      case destinationLine:

         component_string.assign(L"Line Destination" );
         break;

      case destinationMonitor:

         component_string.assign(L"Monitor Destination" );
         break;

      case destinationSpeakers:

         component_string.assign(L"Speakers Destination" );
         break;

      case destinationHeadphones:

         component_string.assign(L"Headphones Destination" );
         break;

      case destinationTelephone:

         component_string.assign(L"Telephone Destination" );
         break;

      case destinationWaveIn:

         component_string.assign(L"Wave In Destination" );
         break;

      case destinationVoiceIn:

         component_string.assign(L"Voice In Destination" );
         break;

      case sourceUndefined:

         component_string.assign(L"Undefined Source" );
         break;

      case sourceDigital:

         component_string.assign(L"Digital Source" );
         break;

      case sourceLine:

         component_string.assign(L"Line Source" );
         break;

      case sourceMicrophone:

         component_string.assign(L"Microphone Source" );
         break;

      case sourceSynthesizer:

         component_string.assign(L"Synthesizer Source" );
         break;

      case sourceCompactDisc:

         component_string.assign(L"Compact Disc Source" );
         break;

      case sourceTelephone:

         component_string.assign(L"Telephone Source" );
         break;

      case sourcePCSpeaker:

         component_string.assign(L"PC Speaker Source" );
         break;

      case sourceWaveOut:

         component_string.assign(L"Wave Out Source" );
         break;

      case sourceAuxillary:

         component_string.assign(L"Auxillary Source" );
         break;

      case sourceAnalog:

         component_string.assign(L"Analog Source" );
         break;

      default:

         format( component_string, L"Unknown (%lu)", Component );
         break;
   }
}

void CMixerLine::GetStatus( _Out_ std::wstring& status_string ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   status_string.clear();

   if ( Status & statusActive )
   {
      status_string.assign(L"Active" );
   }

   if ( Status & statusDisconnected )
   {
      if ( status_string.empty() == false )
      {
         status_string.append( L", " );
      }

      status_string.append( L"Disconnected" );
   }

   if ( Status & statusSource )
   {
      if ( status_string.empty() == false )
      {
         status_string.append( L", " );
      }

      status_string.append(L"Source" );
   }
}

void CMixerLine::GetTarget( _Out_ std::wstring& target_string ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   switch( Target )
   {
      case targetUndefined:

         target_string.assign(L"Undefined" );
         break;

      case targetWaveOut:

         target_string.assign(L"Wave Out" );
         break;

      case targetWaveIn:

         target_string.assign(L"Wave In" );
         break;

      case targetMidiOut:

         target_string.assign(L"MIDI Out" );
         break;

      case targetMidiIn:

         target_string.assign(L"MIDI In" );
         break;

      case targetAuxillary:

         target_string.assign(L"Aux" );
         break;

      default:

         format( target_string, L"Unknown (%lu)", Target );
         break;
   }
}

__checkReturn bool CMixerLine::IsActive( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   const DWORD active = ( Status & statusActive );

   if ( active != 0 )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerLine::IsDisconnected( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   const DWORD disconnected = ( Status & statusDisconnected );

   if ( disconnected != 0 )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerLine::IsSource( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   const DWORD source = ( Status & statusSource );

   if ( source != 0 )
   {
      return( true );
   }

   return( false );
}

__checkReturn CMixerLine& CMixerLine::operator=( __in const CMixerLine& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

__checkReturn CMixerLine& CMixerLine::operator=( __in const MIXERLINE& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
