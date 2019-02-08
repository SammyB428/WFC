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
** $Workfile: CMixer.hpp $
** $Revision: 14 $
** $Modtime: 6/26/01 11:02a $
*/

#if ! defined( MIXER_API_CLASS_HEADER )

#define MIXER_API_CLASS_HEADER

class CMixerControlDetailsData
{
   public:

      // Construction

      CMixerControlDetailsData();
      CMixerControlDetailsData( __in const CMixerControlDetailsData&     source );
      CMixerControlDetailsData( __in const MIXERCONTROLDETAILS_LISTTEXT& source );
      CMixerControlDetailsData( __in const MIXERCONTROLDETAILS_BOOLEAN&  source );
      CMixerControlDetailsData( __in const MIXERCONTROLDETAILS_SIGNED&   source );
      CMixerControlDetailsData( __in const MIXERCONTROLDETAILS_UNSIGNED& source );
      virtual ~CMixerControlDetailsData();

      // Properties

      DWORD   Parameter1;
      DWORD   Parameter2;
      std::wstring Name;

      // Methods

      virtual void Copy( __in const CMixerControlDetailsData&     source ) noexcept;
      virtual void Copy( __in const MIXERCONTROLDETAILS_LISTTEXT& source ) noexcept;
      virtual void Copy( __in const MIXERCONTROLDETAILS_BOOLEAN&  source ) noexcept;
      virtual void Copy( __in const MIXERCONTROLDETAILS_SIGNED&   source ) noexcept;
      virtual void Copy( __in const MIXERCONTROLDETAILS_UNSIGNED& source ) noexcept;
      virtual void Empty( void ) noexcept;

      // Operators

      virtual CMixerControlDetailsData& operator = ( __in const CMixerControlDetailsData&     source ) noexcept;
      virtual CMixerControlDetailsData& operator = ( __in const MIXERCONTROLDETAILS_LISTTEXT& source ) noexcept;
      virtual CMixerControlDetailsData& operator = ( __in const MIXERCONTROLDETAILS_BOOLEAN&  source ) noexcept;
      virtual CMixerControlDetailsData& operator = ( __in const MIXERCONTROLDETAILS_SIGNED&   source ) noexcept;
      virtual CMixerControlDetailsData& operator = ( __in const MIXERCONTROLDETAILS_UNSIGNED& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CMixerControlDetails
{
   public:

      // Construction

      CMixerControlDetails();
      CMixerControlDetails( __in const CMixerControlDetails& source );
      CMixerControlDetails( __in const MIXERCONTROLDETAILS&  source );
      virtual ~CMixerControlDetails();

      // Properties

      DWORD ID;
      DWORD NumberOfChannels;
      HWND  WindowHandleOfOwner;
      DWORD NumberOfMultipleItemsPerChannel;

      // Methods

      virtual void Copy( __in const CMixerControlDetails& source ) noexcept;
      virtual void Copy( __in const MIXERCONTROLDETAILS&  source ) noexcept;
      virtual void Empty( void ) noexcept;

      // Operators

      virtual CMixerControlDetails& operator = ( __in const CMixerControlDetails& source ) noexcept;
      virtual CMixerControlDetails& operator = ( __in const MIXERCONTROLDETAILS&  source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CMixerControl
{
   public:

      // Construction

      CMixerControl();
      CMixerControl( __in const CMixerControl& source );
      CMixerControl( __in const MIXERCONTROL&  source );
      virtual ~CMixerControl();

      // Properties

      enum _TypesOfThingys
      {
         typeCustom = MIXERCONTROL_CT_CLASS_CUSTOM,
         typeFader  = MIXERCONTROL_CT_CLASS_FADER,
         typeList   = MIXERCONTROL_CT_CLASS_LIST,
         typeMeter  = MIXERCONTROL_CT_CLASS_METER,
         typeNumber = MIXERCONTROL_CT_CLASS_NUMBER,
         typeSlider = MIXERCONTROL_CT_CLASS_SLIDER,
         typeSwitch = MIXERCONTROL_CT_CLASS_SWITCH,
         typeTime   = MIXERCONTROL_CT_CLASS_TIME
      };

      enum _Thingys
      {
         Custom         = MIXERCONTROL_CONTROLTYPE_CUSTOM,
         Bass           = MIXERCONTROL_CONTROLTYPE_BASS,
         Equalizer      = MIXERCONTROL_CONTROLTYPE_EQUALIZER,
         Fader          = MIXERCONTROL_CONTROLTYPE_FADER,
         Treble         = MIXERCONTROL_CONTROLTYPE_TREBLE,
         Volume         = MIXERCONTROL_CONTROLTYPE_VOLUME,
         Mixer          = MIXERCONTROL_CONTROLTYPE_MIXER,
         MultipleSelect = MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT,
         Mux            = MIXERCONTROL_CONTROLTYPE_MUX,
         SingleSelect   = MIXERCONTROL_CONTROLTYPE_SINGLESELECT,
         BooleanMeter   = MIXERCONTROL_CONTROLTYPE_BOOLEANMETER,
         PeakMeter      = MIXERCONTROL_CONTROLTYPE_PEAKMETER,
         SignedMeter    = MIXERCONTROL_CONTROLTYPE_SIGNEDMETER,
         UnsignedMeter  = MIXERCONTROL_CONTROLTYPE_UNSIGNEDMETER,
         Decibels       = MIXERCONTROL_CONTROLTYPE_DECIBELS,
         Percent        = MIXERCONTROL_CONTROLTYPE_PERCENT,
         Signed         = MIXERCONTROL_CONTROLTYPE_SIGNED,
         Unsigned       = MIXERCONTROL_CONTROLTYPE_UNSIGNED,
         Pan            = MIXERCONTROL_CONTROLTYPE_PAN,
         QSoundPan      = MIXERCONTROL_CONTROLTYPE_QSOUNDPAN,
         Slider         = MIXERCONTROL_CONTROLTYPE_SLIDER,
         Boolean        = MIXERCONTROL_CONTROLTYPE_BOOLEAN,
         Button         = MIXERCONTROL_CONTROLTYPE_BUTTON,
         Loudness       = MIXERCONTROL_CONTROLTYPE_LOUDNESS,
         Mono           = MIXERCONTROL_CONTROLTYPE_MONO,
         Mute           = MIXERCONTROL_CONTROLTYPE_MUTE,
         OnOff          = MIXERCONTROL_CONTROLTYPE_ONOFF,
         StereoEnhance  = MIXERCONTROL_CONTROLTYPE_STEREOENH,
         MicroTime      = MIXERCONTROL_CONTROLTYPE_MICROTIME,
         MilliTime      = MIXERCONTROL_CONTROLTYPE_MILLITIME
      };

      enum _StatusFlags
      {
         statusDisabled = MIXERCONTROL_CONTROLF_DISABLED,
         statusMultiple = MIXERCONTROL_CONTROLF_MULTIPLE,
         statusUniform  = MIXERCONTROL_CONTROLF_UNIFORM
      };

      enum _Units
      {
         unitsCustom   = MIXERCONTROL_CT_UNITS_CUSTOM,
         unitsBoolean  = MIXERCONTROL_CT_UNITS_BOOLEAN,
         unitsSigned   = MIXERCONTROL_CT_UNITS_SIGNED,
         unitsUnsigned = MIXERCONTROL_CT_UNITS_UNSIGNED,
         unitsDecibels = MIXERCONTROL_CT_UNITS_DECIBELS,
         unitsPercent  = MIXERCONTROL_CT_UNITS_PERCENT
      };

      DWORD       ID;
      DWORD       Type;
      DWORD       StatusFlags;
      DWORD       NumberOfItemsPerChannel;
      std::wstring ShortName;
      std::wstring Name;
      DWORD       Minimum;
      DWORD       Maximum;
      DWORD       NumberOfSteps;
      DWORD       NumberOfCustomDataBytes;
      std::vector<uint32_t> Data;

      // Methods

      virtual void  Copy( __in const CMixerControl& source ) noexcept;
      virtual void  Copy( __in const MIXERCONTROL& source  ) noexcept;
      virtual void  Empty( void ) noexcept;
      virtual __checkReturn uint32_t GetType( void ) const noexcept;
      virtual void  GetTypeName( __out std::wstring& name ) const noexcept;
      virtual __checkReturn uint32_t GetUnits( void ) const noexcept;
      virtual __checkReturn bool  IsBooleanUnits(  void ) const noexcept;
      virtual __checkReturn bool  IsCustom(        void ) const noexcept;
      virtual __checkReturn bool  IsCustomUnits(   void ) const noexcept;
      virtual __checkReturn bool  IsDecibelsUnits( void ) const noexcept;
      virtual __checkReturn bool  IsDisabled(      void ) const noexcept;
      virtual __checkReturn bool  IsFader(         void ) const noexcept;
      virtual __checkReturn bool  IsList(          void ) const noexcept;
      virtual __checkReturn bool  IsMeter(         void ) const noexcept;
      virtual __checkReturn bool  IsMultiple(      void ) const noexcept;
      virtual __checkReturn bool  IsNumber(        void ) const noexcept;
      virtual __checkReturn bool  IsPercentUnits(  void ) const noexcept;
      virtual __checkReturn bool  IsSignedUnits(   void ) const noexcept;
      virtual __checkReturn bool  IsSlider(        void ) const noexcept;
      virtual __checkReturn bool  IsSwitch(        void ) const noexcept;
      virtual __checkReturn bool  IsTime(          void ) const noexcept;
      virtual __checkReturn bool  IsUniform(       void ) const noexcept;
      virtual __checkReturn bool  IsUnsignedUnits( void ) const noexcept;

      // Operators

      virtual CMixerControl& operator = ( __in const CMixerControl& source ) noexcept;
      virtual CMixerControl& operator = ( __in const MIXERCONTROL&  source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CMixerLine
{
   public:

      // Construction

      CMixerLine();
      CMixerLine( __in const CMixerLine& source );
      CMixerLine( __in const MIXERLINE&  source );
      virtual ~CMixerLine();

      // Properties

      enum _Components
      {
         destinationUndefined  = MIXERLINE_COMPONENTTYPE_DST_UNDEFINED,
         destinationDigital    = MIXERLINE_COMPONENTTYPE_DST_DIGITAL,
         destinationLine       = MIXERLINE_COMPONENTTYPE_DST_LINE,
         destinationMonitor    = MIXERLINE_COMPONENTTYPE_DST_MONITOR,
         destinationSpeakers   = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS,
         destinationHeadphones = MIXERLINE_COMPONENTTYPE_DST_HEADPHONES,
         destinationTelephone  = MIXERLINE_COMPONENTTYPE_DST_TELEPHONE,
         destinationWaveIn     = MIXERLINE_COMPONENTTYPE_DST_WAVEIN,
         destinationVoiceIn    = MIXERLINE_COMPONENTTYPE_DST_VOICEIN,
         sourceUndefined       = MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED,
         sourceDigital         = MIXERLINE_COMPONENTTYPE_SRC_DIGITAL, 
         sourceLine            = MIXERLINE_COMPONENTTYPE_SRC_LINE,
         sourceMicrophone      = MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE,
         sourceSynthesizer     = MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER,
         sourceCompactDisc     = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC,
         sourceTelephone       = MIXERLINE_COMPONENTTYPE_SRC_TELEPHONE,
         sourcePCSpeaker       = MIXERLINE_COMPONENTTYPE_SRC_PCSPEAKER,
         sourceWaveOut         = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT,
         sourceAuxillary       = MIXERLINE_COMPONENTTYPE_SRC_AUXILIARY,
         sourceAnalog          = MIXERLINE_COMPONENTTYPE_SRC_ANALOG
      };

      enum _TargetTypes
      {
         targetUndefined = MIXERLINE_TARGETTYPE_UNDEFINED,
         targetWaveOut   = MIXERLINE_TARGETTYPE_WAVEOUT,
         targetWaveIn    = MIXERLINE_TARGETTYPE_WAVEIN,
         targetMidiOut   = MIXERLINE_TARGETTYPE_MIDIOUT,
         targetMidiIn    = MIXERLINE_TARGETTYPE_MIDIIN,
         targetAuxillary = MIXERLINE_TARGETTYPE_AUX
      };

      enum _Statusses_or_is_it_Stati
      {
         statusActive       = MIXERLINE_LINEF_ACTIVE,
         statusDisconnected = MIXERLINE_LINEF_DISCONNECTED,
         statusSource       = MIXERLINE_LINEF_SOURCE
      };

      DWORD    DestinationNumber;
      DWORD    Source;
      DWORD    ID;
      DWORD    Status;
      DWORD_PTR Reserved;
      DWORD    Component;
      DWORD    NumberOfChannels;
      DWORD    NumberOfConnections;
      DWORD    NumberOfControls;
      std::wstring ShortName;
      std::wstring Name;
      DWORD    Target;
      DWORD    TargetDeviceID;
      DWORD    TargetManufacturer;
      DWORD    TargetProduct;
      DWORD    TargetDriverVersion;
      std::wstring TargetName;

      // Methods

      virtual void Copy( __in const CMixerLine& source ) noexcept;
      virtual void Copy( __in const MIXERLINE&  source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual void GetComponent( __out std::wstring& component_string ) const noexcept;
      virtual void GetStatus( __out std::wstring& status_string ) const noexcept;
      virtual void GetTarget( __out std::wstring& target_string ) const noexcept;
      virtual __checkReturn bool IsActive( void ) const noexcept;
      virtual __checkReturn bool IsDisconnected( void ) const noexcept;
      virtual __checkReturn bool IsSource( void ) const noexcept;

      // Operators

      virtual __checkReturn CMixerLine& operator = ( __in const CMixerLine& source ) noexcept;
      virtual __checkReturn CMixerLine& operator = ( __in const MIXERLINE&  source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CMixerCapabilities
{
   public:

      // Construction

      CMixerCapabilities();
      CMixerCapabilities( __in const CMixerCapabilities& source );
      CMixerCapabilities( __in const MIXERCAPS&  source );
      virtual ~CMixerCapabilities();

      // Properties

      DWORD   Manufacturer;
      DWORD   Product;
      DWORD   Version;
      std::wstring ProductName;
      DWORD   Support;
      DWORD   NumberOfDestinations;

      // Methods

      virtual void Copy( __in const CMixerCapabilities& source ) noexcept;
      virtual void Copy( __in const MIXERCAPS& source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual __checkReturn bool Get( __in const UINT device_number ) noexcept;

      // Operators

      virtual CMixerCapabilities& operator = ( __in const CMixerCapabilities& source ) noexcept;
      virtual CMixerCapabilities& operator = ( __in const MIXERCAPS&  source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CMixer
{
   protected:

       DWORD m_ErrorCode{ 0 };

       HMIXER m_Handle{ 0 };

      UINT_PTR m_DeviceID{ 0 };

   public:

      // Construction

       CMixer(__in const CMixer&) = delete;
       CMixer& operator=(__in const CMixer&) = delete;
       CMixer();
      virtual ~CMixer();

      // Properties

      enum class Callbacks : uint32_t
      {
         callbackEvent    = CALLBACK_EVENT,
         callbackFunction = CALLBACK_FUNCTION,
         callbackNone     = CALLBACK_NULL,
         callbackTask     = CALLBACK_TASK,
         callbackThread   = CALLBACK_THREAD,
         callbackWindow   = CALLBACK_WINDOW
      };

      enum class Notifiers : uint32_t
      {
         notifyWindowHandle           = CALLBACK_WINDOW,
         notifyAuxillaryDeviceNumber  = MIXER_OBJECTF_AUX,
         notifyMIDIInputDeviceHandle  = MIXER_OBJECTF_HMIDIIN,
         notifyMIDIOutputDeviceHandle = MIXER_OBJECTF_HMIDIOUT,
         notifyMixerDeviceHandle      = MIXER_OBJECTF_HMIXER,
         notifyWaveInDeviceHandle     = MIXER_OBJECTF_HWAVEIN,
         notifyWaveOutDeviceHandle    = MIXER_OBJECTF_HWAVEOUT,
         notifyMIDIInputNumber        = MIXER_OBJECTF_MIDIIN,
         notifyMIDIOutputNumber       = MIXER_OBJECTF_MIDIOUT,
         notifyMixerNumber            = MIXER_OBJECTF_MIXER,
         notifyWaveInDeviceNumber     = MIXER_OBJECTF_WAVEIN,
         notifyWaveOutDeviceNumber    = MIXER_OBJECTF_WAVEOUT
      };

      // Methods

      virtual void  Close( void ) noexcept;
      virtual __checkReturn bool  Get( __out CMixerCapabilities& capabilities ) noexcept;
      virtual __checkReturn bool  GetAllControls( __in const CMixerLine& line, __out std::vector<CMixerControl>& array_of_CMixerControl_pointers ) noexcept;
      virtual __checkReturn bool  GetByComponent( __in const DWORD component, __out CMixerLine& line ) noexcept;
      virtual __checkReturn bool  GetByConnection( __in const DWORD destination_number, __in const DWORD source, __out CMixerLine& line ) noexcept;
      virtual __checkReturn bool  GetByDestination( __in const DWORD destination_number, __out CMixerLine& line ) noexcept;
      virtual __checkReturn bool  GetByID( __in const DWORD id, __out CMixerLine& line ) noexcept;
      virtual __checkReturn bool  GetControlDetails(  __in const CMixerLine& line, __in const CMixerControl& control, __out std::vector<CMixerControlDetailsData>& array) noexcept;
      virtual __checkReturn bool  GetControlListText( __in const CMixerLine& line, __in const CMixerControl& control, __out std::vector<CMixerControlDetailsData>& array ) noexcept;
      virtual __checkReturn UINT_PTR GetDeviceID( void ) const noexcept;
      virtual __checkReturn DWORD GetErrorCode( void ) const noexcept;
      virtual void GetErrorString( __out std::wstring& error_string ) const noexcept;
      virtual __checkReturn HMIXEROBJ GetHandle( void ) const noexcept;
      virtual __checkReturn size_t  GetNumberOfDevices( void ) const noexcept;
      virtual __checkReturn bool  Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0) noexcept;
      virtual __checkReturn bool  SetControlDetails( __in const CMixerLine& line, __in const CMixerControl& control, __in const std::vector<CMixerControlDetailsData>& array) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

// Thus endeth the Mixer API encapsulation objects, now let's put these together to make
// something coherent and easier to use

// Thanks go to Sean Dynan (sdynan@cccgroup.co.uk) for finding a flaw
// in the design of the following class' Open() methods. They weren't
// the same as CMixer's.

class CMixerControlInstance
{
   protected:

      // Properties

      DWORD m_WhatToNotify;
      DWORD_PTR m_WhoToNotify;
      DWORD_PTR m_NotifyData;

      CMixer        m_Mixer;
      CMixerLine    m_MixerLine;
      CMixerControl m_MixerControl;

   public:

      // Construction

      CMixerControlInstance();
      CMixerControlInstance( __in const CMixerControlInstance& source );
      virtual ~CMixerControlInstance();

      // Methods

      virtual void  Close( void ) noexcept;
      virtual void  Copy( __in const CMixerControlInstance& source ) noexcept;
      virtual __checkReturn bool Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0 ) noexcept;
      virtual void  SetLine( __in const CMixerLine& line ) noexcept;
      virtual void  SetControl( __in const CMixerControl& control ) noexcept;

      // Operators

      virtual CMixerControlInstance& operator = ( __in const CMixerControlInstance& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CMixerSourceSelector : public CMixerControlInstance
{
   protected:

      // Properties

      std::vector<CMixerControlDetailsData> m_Settings;

      virtual __checkReturn bool m_GetSettings( void ) noexcept;
      virtual __checkReturn bool m_SetSettings( void ) noexcept;

   public:

      // Construction

      CMixerSourceSelector( __in const CMixerSourceSelector& source );
      CMixerSourceSelector( __in const CMixerLine& destination, __in const CMixerControl& mixer_control );
      virtual ~CMixerSourceSelector();

      // Methods

      virtual void  Copy( __in const CMixerSourceSelector& source ) noexcept;
      virtual __checkReturn DWORD GetSource( void ) const noexcept;
      virtual __checkReturn bool  IsSelected( __in const DWORD source ) noexcept;
      __checkReturn bool  Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0 ) noexcept override;
      virtual __checkReturn bool  Select( __in const DWORD source, __in bool selected = true ) noexcept;
      virtual __checkReturn bool  Unselect( __in const DWORD source ) noexcept;

      // Operators

      virtual CMixerSourceSelector& operator = ( __in const CMixerSourceSelector& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CMixerVolumeControl : public CMixerControlInstance
{
   protected:

      // Properties

      CMixerControlDetailsData m_LeftChannel;
      CMixerControlDetailsData m_RightChannel;
      CMixerControlDetailsData m_BothChannels;

      std::vector<CMixerControlDetailsData> m_Channels;

      virtual __checkReturn bool m_GetAll( void ) noexcept;

   public:

      // Construction

      CMixerVolumeControl();
      CMixerVolumeControl( __in const CMixerVolumeControl& source );
      virtual ~CMixerVolumeControl();

      // Methods

      virtual void  Copy( __in const CMixerVolumeControl& source ) noexcept;
      virtual __checkReturn DWORD GetLeftChannelVolume( void ) noexcept;
      virtual __checkReturn DWORD GetMinimum( void ) const noexcept;
      virtual __checkReturn DWORD GetMaximum( void ) const noexcept;
      virtual __checkReturn DWORD GetRightChannelVolume( void ) noexcept;
      virtual __checkReturn DWORD GetVolume( void ) noexcept;
      virtual __checkReturn bool  SetLeftChannelVolume( __in const DWORD new_volume ) noexcept;
      virtual __checkReturn bool  SetRightChannelVolume( __in const DWORD new_volume ) noexcept;
      virtual __checkReturn bool  SetVolume( __in const DWORD new_volume ) noexcept;

      // Operators

      virtual __checkReturn CMixerVolumeControl& operator = ( __in const CMixerVolumeControl& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CMixerSwitchControl : public CMixerControlInstance
{
   protected:

      // Properties

      CMixerControlDetailsData m_Switch;

      std::vector<CMixerControlDetailsData> m_Array;

      virtual __checkReturn bool m_GetAll( void ) noexcept;

   public:

      // Construction

      CMixerSwitchControl();
      CMixerSwitchControl( __in const CMixerSwitchControl& source );
      virtual ~CMixerSwitchControl();

      // Methods

      virtual void Copy( __in const CMixerSwitchControl& source ) noexcept;
      virtual __checkReturn bool GetState( void ) noexcept;
      virtual __checkReturn bool SetState( __in const BOOL turn_it_on = TRUE ) noexcept;
      virtual __checkReturn bool TurnOff( void ) noexcept;
      virtual __checkReturn bool TurnOn( void ) noexcept;

      // Operators

      virtual __checkReturn CMixerSwitchControl& operator = ( __in const CMixerSwitchControl& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CMixerSource : public CMixerControlInstance
{
   public:

      // Construction

      CMixerSource();
      CMixerSource( __in const CMixerSource& source );
      virtual ~CMixerSource();

      // Methods

      virtual void Copy( __in const CMixerSource& source ) noexcept;
      virtual __checkReturn bool GetControl( __out CMixerVolumeControl& control ) noexcept;
      __checkReturn bool Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0 ) noexcept override;

      // Operators

      virtual CMixerSource& operator = ( __in const CMixerSource& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CMixerDestination : public CMixerControlInstance
{
   protected:

      // Properties

      CMixerSourceSelector * m_SourceSelector_p;

   public:

      // Construction

      CMixerDestination();
      CMixerDestination( __in const CMixerDestination& source );
      virtual ~CMixerDestination();

      // Methods

      virtual void Copy( __in const CMixerDestination& source ) noexcept;
      virtual __checkReturn bool GetSource( __in const DWORD CMixerLineSource, __out CMixerSource& source ) noexcept;
      virtual __checkReturn bool IsSourceSelected( __in const DWORD CMixerLineSource ) noexcept;
      __checkReturn bool Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0 ) noexcept override;
      virtual __checkReturn bool SelectSource( __in const DWORD CMixerLineSource, __in bool new_selection = true ) noexcept;
      virtual __checkReturn bool UnselectSource( __in const DWORD CMixerLineSource ) noexcept;

      // Operators

      virtual __checkReturn CMixerDestination& operator = ( __in const CMixerDestination& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CMixerWaveIn : public CMixerDestination
{
   protected:

      // Properties

      CMixerVolumeControl m_RecordingGain;

   public:

      // Construction

      CMixerWaveIn();
      CMixerWaveIn( __in const CMixerWaveIn& source );
      virtual ~CMixerWaveIn();

      // Methods

      virtual void  Copy( __in const CMixerWaveIn& source ) noexcept;
      virtual __checkReturn DWORD GetLeftChannelRecordingGain( void ) noexcept;
      virtual __checkReturn DWORD GetMaximumGain( void ) const noexcept;
      virtual __checkReturn DWORD GetMinimumGain( void ) const noexcept;
      virtual __checkReturn DWORD GetRecordingGain( void ) noexcept;
      virtual __checkReturn DWORD GetRightChannelRecordingGain( void ) noexcept;
      __checkReturn bool  Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0 ) noexcept override;
      virtual __checkReturn bool  SetLeftChannelRecordingGain( __in const DWORD new_level ) noexcept;
      virtual __checkReturn bool  SetRecordingGain( __in const DWORD new_level ) noexcept;
      virtual __checkReturn bool  SetRightChannelRecordingGain( __in const DWORD new_level ) noexcept;

      // Operators

      virtual __checkReturn CMixerWaveIn& operator = ( __in const CMixerWaveIn& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CMixerSpeakers : public CMixerDestination
{
   protected:

      // Properties

      CMixerVolumeControl m_Volume;
      CMixerSwitchControl m_Mute;

   public:

      // Construction

      CMixerSpeakers();
      CMixerSpeakers( __in const CMixerSpeakers& source );
      virtual ~CMixerSpeakers();

      // Methods

      virtual void Copy( __in const CMixerSpeakers& source ) noexcept;
      virtual __checkReturn DWORD GetLeftChannelVolume( void ) noexcept;
      virtual __checkReturn DWORD GetMaximumVolume( void ) noexcept;
      virtual __checkReturn DWORD GetMinimumVolume( void ) noexcept;
      virtual __checkReturn DWORD GetRightChannelVolume( void ) noexcept;
      virtual __checkReturn DWORD GetVolume( void ) noexcept;
      virtual __checkReturn bool  IsMuted( void ) noexcept;
      virtual __checkReturn bool  Mute( __in const bool muting = true ) noexcept;
      __checkReturn bool  Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0 ) noexcept override;
      virtual __checkReturn bool  SetLeftChannelVolume( __in const DWORD new_level ) noexcept;
      virtual __checkReturn bool  SetRightChannelVolume( __in const DWORD new_level ) noexcept;
      virtual __checkReturn bool  SetVolume( __in const DWORD new_level ) noexcept;
      virtual __checkReturn bool  UnMute( void ) noexcept;

      // Operators

      virtual __checkReturn CMixerSpeakers& operator = ( __in const CMixerSpeakers& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // MIXER_API_CLASS_HEADER
