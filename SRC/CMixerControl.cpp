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
** $Workfile: CMixerControl.cpp $
** $Revision: 21 $
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

CMixerControl::CMixerControl()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

CMixerControl::CMixerControl( __in const CMixerControl& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CMixerControl::CMixerControl( __in const MIXERCONTROL& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CMixerControl::~CMixerControl()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

void CMixerControl::Copy( __in const CMixerControl& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   Empty();
   ID                      = source.ID;
   Type                    = source.Type;
   StatusFlags             = source.StatusFlags;
   NumberOfItemsPerChannel = source.NumberOfItemsPerChannel;
   ShortName.assign( source.ShortName );
   Name.assign( source.Name );
   Minimum                 = source.Minimum;
   Maximum                 = source.Maximum;
   NumberOfSteps           = source.NumberOfSteps;
   NumberOfCustomDataBytes = source.NumberOfCustomDataBytes;
   Data = source.Data;
}

void CMixerControl::Copy( __in const MIXERCONTROL& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Empty();
   ID                      = source.dwControlID;
   Type                    = static_cast<CMixerControl::ControlType>(source.dwControlType);
   StatusFlags             = source.fdwControl;
   NumberOfItemsPerChannel = source.cMultipleItems;
   ShortName.assign(source.szShortName);
   Name.assign(source.szName);
   Minimum                 = source.Bounds.dwMinimum;
   Maximum                 = source.Bounds.dwMaximum;
   NumberOfSteps           = source.Metrics.cSteps;
   NumberOfCustomDataBytes = source.Metrics.cbCustomData;
   Data.push_back( source.Metrics.dwReserved[ 0 ] );
   Data.push_back( source.Metrics.dwReserved[ 1 ] );
   Data.push_back( source.Metrics.dwReserved[ 2 ] );
   Data.push_back( source.Metrics.dwReserved[ 3 ] );
   Data.push_back( source.Metrics.dwReserved[ 4 ] );
   Data.push_back( source.Metrics.dwReserved[ 5 ] );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CMixerControl::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   std::wstring temp_string;

   dump_context << TEXT( "a CMixerControl at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   ID is " ) << ID << TEXT( "\n" );

   DWORD thing = Type & MIXERCONTROL_CT_CLASS_MASK;

   switch( thing )
   {
      case typeFader:

         temp_string = TEXT( "typeFader |" );
         break;

      case typeList:

         temp_string = TEXT( "typeList |" );
         break;

      case typeMeter:

         temp_string = TEXT( "typeMeter |" );
         break;

      case typeNumber:

         temp_string = TEXT( "typeNumber |" );
         break;

      case typeSlider:

         temp_string = TEXT( "typeSlider |" );
         break;

      case typeSwitch:

         temp_string = TEXT( "typeSwitch |" );
         break;

      case typeTime:

         temp_string = TEXT( "typeTime |" );
         break;

      default:

         temp_string = TEXT( "unknown |" );
         break;
   }

   thing = Type & MIXERCONTROL_CT_UNITS_MASK;

   switch( thing )
   {
      case unitsBoolean:

         temp_string += TEXT( "unitsBoolean" );
         break;

      case unitsSigned:

         temp_string += TEXT( "unitsSigned" );
         break;

      case unitsUnsigned:

         temp_string += TEXT( "unitsUnsigned" );
         break;

      case unitsDecibels:

         temp_string += TEXT( "unitsDecibels" );
         break;

      case unitsPercent:

         temp_string += TEXT( "unitsPercent" );
         break;

      default:

         temp_string += TEXT( "unitsCustom" );
         break;
   }

   switch( Type )
   {
      case Custom:

         temp_string += TEXT( " [Custom]" );
         break;

      case Bass:

         temp_string += TEXT( " [Bass]" );
         break;

      case Equalizer:

         temp_string += TEXT( " [Equalizer]" );
         break;

      case Fader:

         temp_string += TEXT( " [Fader]" );
         break;

      case Treble:

         temp_string += TEXT( " [Treble]" );
         break;

      case Volume:

         temp_string += TEXT( " [Volume]" );
         break;

      case Mixer:

         temp_string += TEXT( " [Mixer]" );
         break;

      case MultipleSelect:

         temp_string += TEXT( " [MultipleSelect]" );
         break;

      case Mux:

         temp_string += TEXT( " [Mux]" );
         break;

      case SingleSelect:

         temp_string += TEXT( " [SingleSelect]" );
         break;

      case BooleanMeter:

         temp_string += TEXT( " [BooleanMeter]" );
         break;

      case PeakMeter:

         temp_string += TEXT( " [PeakMeter]" );
         break;

      case SignedMeter:

         temp_string += TEXT( " [SignedMeter]" );
         break;

      case UnsignedMeter:

         temp_string += TEXT( " [UnsignedMeter]" );
         break;

      case Decibels:

         temp_string += TEXT( " [Decibels]" );
         break;

      case Percent:

         temp_string += TEXT( " [Percent]" );
         break;

      case Signed:

         temp_string += TEXT( " [Signed]" );
         break;

      case Unsigned:

         temp_string += TEXT( " [Unsigned]" );
         break;

      case Pan:

         temp_string += TEXT( " [Pan]" );
         break;

      case QSoundPan:

         temp_string += TEXT( " [QSoundPan]" );
         break;

      case Slider:

         temp_string += TEXT( " [Slider]" );
         break;

      case Boolean:

         temp_string += TEXT( " [Boolean]" );
         break;

      case Button:

         temp_string += TEXT( " [Button]" );
         break;

      case Loudness:

         temp_string += TEXT( " [Loudness]" );
         break;

      case Mono:

         temp_string += TEXT( " [Mono]" );
         break;

      case Mute:

         temp_string += TEXT( " [Mute]" );
         break;

      case OnOff:

         temp_string += TEXT( " [OnOff]" );
         break;

      case StereoEnhance:

         temp_string += TEXT( " [StereoEnhance]" );
         break;

      case MicroTime:

         temp_string += TEXT( " [MicroTime]" );
         break;

      case MilliTime:

         temp_string += TEXT( " [MilliTime]" );
         break;

      default:

         temp_string += TEXT( " [Unknown]" );
         break;
   }

   dump_context << TEXT( "   Type is                    " ) << Type                    << TEXT( " ( " ) << temp_string << TEXT( " )\n" );
   dump_context << TEXT( "   StatusFlags is             " ) << StatusFlags             << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfItemsPerChannel is " ) << NumberOfItemsPerChannel << TEXT( "\n" );
   dump_context << TEXT( "   Minimum is                 " ) << Minimum                 << TEXT( "\n" );
   dump_context << TEXT( "   Maximum is                 " ) << Maximum                 << TEXT( "\n" );
   dump_context << TEXT( "   ShortName is               " ) << ShortName               << TEXT( "\n" );
   dump_context << TEXT( "   Name is                    " ) << Name                    << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfSteps is           " ) << NumberOfSteps           << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfCustomDataBytes is " ) << NumberOfCustomDataBytes << TEXT( "\n" );
   
   if ( Data.GetSize() > 5 )
   {
      dump_context << TEXT( "   Data[ 0 ] is               " ) << Data.GetAt( 0 ) << TEXT( "\n" );
      dump_context << TEXT( "   Data[ 1 ] is               " ) << Data.GetAt( 1 ) << TEXT( "\n" );
      dump_context << TEXT( "   Data[ 2 ] is               " ) << Data.GetAt( 2 ) << TEXT( "\n" );
      dump_context << TEXT( "   Data[ 3 ] is               " ) << Data.GetAt( 3 ) << TEXT( "\n" );
      dump_context << TEXT( "   Data[ 4 ] is               " ) << Data.GetAt( 4 ) << TEXT( "\n" );
      dump_context << TEXT( "   Data[ 5 ] is               " ) << Data.GetAt( 5 ) << TEXT( "\n" );
   }
   else
   {
      dump_context << TEXT( "   Data is corrupted.\n" );
   }

   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void CMixerControl::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ID                      = 0;
   NumberOfCustomDataBytes = 0;
   NumberOfItemsPerChannel = 0;
   NumberOfSteps           = 0;
   Maximum                 = 0;
   Minimum                 = 0;
   StatusFlags             = 0;
   Type                    = ControlType::Unknown;
   ShortName.clear();
   Name.clear();
   Data.clear();
}

__checkReturn CMixerControl::Types CMixerControl::GetType( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   CMixerControl::Types const type = static_cast<CMixerControl::Types>(static_cast<uint32_t>(Type) & MIXERCONTROL_CT_CLASS_MASK);

   switch( type )
   {
   case CMixerControl::Types::Fader:
      case CMixerControl::Types::List:
      case CMixerControl::Types::Meter:
      case CMixerControl::Types::Number:
      case CMixerControl::Types::Slider:
      case CMixerControl::Types::Switch:
      case CMixerControl::Types::Time:

         return( type );

      default:

         return(CMixerControl::Types::Custom);
   }
}

void CMixerControl::GetTypeName( _Out_ std::wstring& name ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   switch( Type )
   {
       case CMixerControl::ControlType::Custom:

         name.assign(L"Custom");
         break;

      case CMixerControl::ControlType::Bass:

         name.assign(L"Bass Fader");
         break;

      case CMixerControl::ControlType::Equalizer:

         name.assign(L"Equalizer Fader" );
         break;

      case CMixerControl::ControlType::Fader:

         name.assign( L"Fader" );
         break;

      case CMixerControl::ControlType::Treble:

         name.assign(L"Treble Fader" );
         break;

      case CMixerControl::ControlType::Volume:

         name.assign(L"Volume Fader" );
         break;

      case CMixerControl::ControlType::Mixer:

         name.assign(L"Mixer List" );
         break;

      case CMixerControl::ControlType::MultipleSelect:

         name.assign(L"Multiple-Select List" );
         break;

      case CMixerControl::ControlType::Mux:

         name.assign(L"Mux List" );
         break;

      case CMixerControl::ControlType::SingleSelect:

         name.assign(L"Single-Select List" );
         break;

      case CMixerControl::ControlType::BooleanMeter:

         name.assign(L"Boolean Meter" );
         break;

      case CMixerControl::ControlType::PeakMeter:

         name.assign(L"Peak Meter" );
         break;

      case CMixerControl::ControlType::SignedMeter:

         name.assign(L"Signed Meter" );
         break;

      case CMixerControl::ControlType::UnsignedMeter:

          name.assign(L"Unsigned Meter" );
         break;

      case CMixerControl::ControlType::Decibels:

          name.assign(L"Decibels Number" );
         break;

      case CMixerControl::ControlType::Percent:

          name.assign(L"Percent Number" );
         break;

      case CMixerControl::ControlType::Signed:

          name.assign( L"Signed Number" );
         break;

      case CMixerControl::ControlType::Unsigned:

          name.assign(L"Unsigned Number" );
         break;

      case CMixerControl::ControlType::Pan:

          name.assign(L"Pan Slider" );
         break;

      case CMixerControl::ControlType::QSoundPan:

          name.assign(L"Q-Sound Pan Slider" );
         break;

      case CMixerControl::ControlType::Slider:

          name.assign(L"Slider" );
         break;

      case CMixerControl::ControlType::Boolean:

          name.assign( L"Boolean" );
         break;

      case CMixerControl::ControlType::Button:

         name.assign( L"Button" );
         break;

      case CMixerControl::ControlType::Loudness:

          name.assign( L"Loudness Switch" );
         break;

      case CMixerControl::ControlType::Mono:

          name.assign(L"Mono Switch" );
         break;

      case CMixerControl::ControlType::Mute:

          name.assign(L"Mute Switch" );
         break;

      case CMixerControl::ControlType::OnOff:

          name.assign(L"On/Off Switch" );
         break;

      case CMixerControl::ControlType::StereoEnhance:

          name.assign(L"Stereo Enhance Switch" );
         break;

      case CMixerControl::ControlType::MicroTime:

          name.assign(L"Microsecond Time" );
         break;

      case CMixerControl::ControlType::MilliTime:

          name.assign( L"Millisecond Time" );
         break;

      default:

          name.assign(L"Unknown");
          break;
   }
}

__checkReturn CMixerControl::Units CMixerControl::GetUnits( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   uint32_t const units = static_cast<uint32_t>( static_cast<uint32_t>(Type) & MIXERCONTROL_CT_UNITS_MASK);

   switch( units )
   {
      case static_cast<uint32_t>(CMixerControl::Units::Boolean):
      case static_cast<uint32_t>(CMixerControl::Units::Signed):
      case static_cast<uint32_t>(CMixerControl::Units::Unsigned):
      case static_cast<uint32_t>(CMixerControl::Units::Decibels):
      case static_cast<uint32_t>(CMixerControl::Units::Percent):

         return( static_cast<CMixerControl::Units>(units) );

      default:

         return(CMixerControl::Units::Custom);
   }
}

__checkReturn bool CMixerControl::IsBooleanUnits( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetUnits() == CMixerControl::Units::Boolean )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsCustom( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetType() == CMixerControl::Types::Custom )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsCustomUnits( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetUnits() == CMixerControl::Units::Custom )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsDecibelsUnits( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetUnits() == CMixerControl::Units::Decibels )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsDisabled( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD const status = ( StatusFlags & static_cast<uint32_t>(CMixerControl::StatusFlags::Disabled) );

   if ( status != 0 )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsFader( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetType() == CMixerControl::Types::Fader )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsList( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetType() == CMixerControl::Types::List )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsMeter( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetType() == CMixerControl::Types::Meter )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsMultiple( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD const multiple = ( StatusFlags & static_cast<uint32_t>(CMixerControl::StatusFlags::Multiple) );

   if ( multiple != 0 )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsNumber( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetType() == CMixerControl::Types::Number )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsPercentUnits( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetUnits() == CMixerControl::Units::Percent )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsSignedUnits( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetUnits() == CMixerControl::Units::Signed )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsSlider( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetType() == CMixerControl::Types::Slider )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsSwitch( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetType() == CMixerControl::Types::Switch )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsTime( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetType() == CMixerControl::Types::Time )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsUniform( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD const uniform = ( StatusFlags & static_cast<uint32_t>(CMixerControl::StatusFlags::Uniform) );

   if ( uniform != 0 )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CMixerControl::IsUnsignedUnits( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GetUnits() == CMixerControl::Units::Unsigned )
   {
      return( true );
   }

   return( false );
}

// Operators

CMixerControl& CMixerControl::operator=( __in CMixerControl const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

CMixerControl& CMixerControl::operator=( __in MIXERCONTROL const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CMixerControl</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles Mixer controls.">
</HEAD>

<BODY>

<H1>CMixerControl : CObject</H1>
$Revision: 21 $<BR><HR>

<H2>Description</H2>

This class encapsulates the MIXERCONTROL data structure. What is a MIXERCONTROL?
The way the Mixer API works is the idea of sources and controls. A control is
something you can set or read.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CMixerControl</B>()
<B>CMixerControl</B>( const CMixerControl&amp; source )
<B>CMixerControl</B>( const MIXERCONTROL&amp; source )</PRE><DD>
Constructs the object.

</DL>

<H2>Data Members</H2>

<DL COMPACT>

<DT><PRE><B>ID</B></PRE><DD>A unique control ID.

<DT><PRE><B>Type</B></PRE><DD>Holds the type of control. It will be one of the following:

<UL>
<LI>Custom<LI>Bass<LI>Equalizer<LI>Fader<LI>Treble<LI>Volume<LI>Mixer<LI>MultipleSelect
<LI>Mux<LI>SingleSelect<LI>BooleanMeter<LI>PeakMeter<LI>SignedMeter<LI>UnsignedMeter
<LI>Decibels<LI>Percent<LI>Signed<LI>Unsigned<LI>Pan<LI>QSoundPan<LI>Slider<LI>Boolean
<LI>Button<LI>Loudness<LI>Mono<LI>Mute<LI>OnOff<LI>StereoEnhance<LI>MicroTime<LI>MilliTime
</UL>

<DT><PRE><B>StatusFlags</B></PRE><DD>Holds several flags. Here they are:

<UL>
<LI><CODE>statusDisabled</CODE>
<LI><CODE>statusMultiple</CODE>
<LI><CODE>statusUniform</CODE>
</UL>

<DT><B>NumberOfItemsPerChannel</B><DD>Holds the number of items per channel.

<DT><PRE><B>ShortName</B></PRE><DD>Contains the short name of the control.
Example: &quot;Vol&quot;

<DT><PRE><B>Name</B></PRE><DD>Contains the long name of the control.
Example: &quot;Volume&quot;

<DT><PRE><B>Minimum</B></PRE><DD>Holds the minimum possible value.

<DT><PRE><B>Maximum</B></PRE><DD>Holds the maximum possible value.

<DT><PRE><B>NumberOfSteps</B></PRE><DD>Holds the number of steps between <B>Minimum</B>
and <B>Maximum</B>.

<DT><PRE><B>NumberOfCustomDataBytes</B></PRE><DD>Holds the number of custom data bytes.

<DT><PRE><B>Data</B></PRE><DD>Holds the custom data bytes.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Copy">Copy</A></B>( const CMixerControl&amp; source )
void <B>Copy</B>( const MIXERCONTROL&amp; source )</PRE><DD>
Copies the contents of another CMixerControl or a
MIXERCONTROL data structure.

<DT><PRE>void <B><A NAME="Empty">Empty</A></B>( void )</PRE><DD>Zeroizes the data members.

<DT><PRE>DWORD <B><A NAME="GetType">GetType</A></B>( void ) const</PRE><DD>
Returns the type (or class) of the control. It
will be one of the following:

<UL>
<LI><CODE>typeCustom</CODE>
<LI><CODE>typeFader</CODE>
<LI><CODE>typeList</CODE>
<LI><CODE>typeMeter</CODE>
<LI><CODE>typeNumber</CODE>
<LI><CODE>typeSlider</CODE>
<LI><CODE>typeSwitch</CODE>
<LI><CODE>typeTime</CODE>
</UL>

<DT><PRE>void <B><A NAME="GetTypeName">GetTypeName</A></B>( std::wstring&amp; name ) const</PRE><DD>
Returns a string containing a human readable
form for the <B>Type</B>.

<DT><PRE>DWORD <B><A NAME="GetUnits">GetUnits</A></B>( void ) const</PRE><DD>
Returns the units the control is measured in. It
will return one of the following:

<UL>
<LI><CODE>unitsCustom</CODE>
<LI><CODE>unitsBoolean</CODE>
<LI><CODE>unitsSigned</CODE>
<LI><CODE>unitsUnsigned</CODE>
<LI><CODE>unitsDecibels</CODE>
<LI><CODE>unitsPercent</CODE>
</UL>

<DT><PRE>BOOL <B><A NAME="IsBooleanUnits">IsBooleanUnits</A></B>( void ) const</PRE><DD>
Returns TRUE if the control uses Boolean Units.
False if it doesn't.

<DT><PRE>BOOL <B><A NAME="IsCustom">IsCustom</A></B>( void ) const</PRE><DD>
Returns TRUE if the control is a Custom.
False if it isn't.

<DT><PRE>BOOL <B><A NAME="IsCustomUnits">IsCustomUnits</A></B>( void ) const</PRE><DD>
Returns TRUE if the control uses Custom Units.
False if it doesn't.

<DT><PRE>BOOL <B><A NAME="IsDecibelsUnits">IsDecibelsUnits</A></B>( void ) const</PRE><DD>
Returns TRUE if the control uses Decibel Units.
False if it doesn't.

<DT><PRE>BOOL <B><A NAME="IsDisabled">IsDisabled</A></B>( void ) const</PRE><DD>
Returns TRUE if the control is Disabled.
False if it isn't.

<DT><PRE>BOOL <B><A NAME="IsFader">IsFader</A></B>( void ) const</PRE><DD>
Returns TRUE if the control is a Fader.
False if it isn't.

<DT><PRE>BOOL <B><A NAME="IsList">IsList</A></B>( void ) const</PRE><DD>
Returns TRUE if the control is a list of controls.
False if it isn't.

<DT><PRE>BOOL <B><A NAME="IsMeter">IsMeter</A></B>( void ) const</PRE><DD>
Returns TRUE if the control is a Meter.
False if it isn't.

<DT><PRE>BOOL <B><A NAME="IsMultiple">IsMultiple</A></B>( void ) const</PRE><DD>
Returns TRUE if the control is multiple.
False if it isn't.

<DT><PRE>BOOL <B><A NAME="IsNumber">IsNumber</A></B>( void ) const</PRE><DD>
Returns TRUE if the control is a number.
False if it isn't.

<DT><PRE>BOOL <B><A NAME="IsPercentUnits">IsPercentUnits</A></B>( void ) const</PRE><DD>
Returns TRUE if the control uses percent units.
False if it doesn't.

<DT><PRE>BOOL <B><A NAME="IsSignedUnits">IsSignedUnits</A></B>( void ) const</PRE><DD>
Returns TRUE if the control uses signed number units.
False if it doesn't.

<DT><PRE>BOOL <B><A NAME="IsSlider">IsSlider</A></B>( void ) const</PRE><DD>
Returns TRUE if the control is a slider.
False if it isn't.

<DT><PRE>BOOL <B><A NAME="IsSwitch">IsSwitch</A></B>( void ) const</PRE><DD>
Returns TRUE if the control is a switch.
False if it isn't.

<DT><PRE>BOOL <B><A NAME="IsTime">IsTime</A></B>( void ) const</PRE><DD>
Returns TRUE if the control is a time.
False if it isn't.

<DT><PRE>BOOL <B><A NAME="IsUniform">IsUniform</A></B>( void ) const</PRE><DD>
Returns TRUE if the control is uniform.
False if it isn't.

<DT><PRE>BOOL <B><A NAME="IsUnsignedUnits">IsUnsignedUnits</A></B>( void ) const</PRE><DD>
Returns TRUE if the control uses unsigned number units.
False if it doesn't.

<DT><PRE>void <B><A NAME="Serialize">Serialize</A></B>( CArchive&amp; archive )</PRE><DD>
After all, we are a CObject...

</DL>

<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CMixerControl.cpp $<BR>
$Modtime: 6/26/01 10:48a $
</BODY>

</HTML>
*/
