/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2019, Samuel R. Blackburn
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
      CMixerControlDetailsData( __in CMixerControlDetailsData const&     source );
      CMixerControlDetailsData( __in MIXERCONTROLDETAILS_LISTTEXT const& source );
      CMixerControlDetailsData( __in MIXERCONTROLDETAILS_BOOLEAN const&  source );
      CMixerControlDetailsData( __in MIXERCONTROLDETAILS_SIGNED const&   source );
      CMixerControlDetailsData( __in MIXERCONTROLDETAILS_UNSIGNED const& source );
      virtual ~CMixerControlDetailsData();

      // Properties

      DWORD   Parameter1{ 0 };
      DWORD   Parameter2{ 0 };
      std::wstring Name;

      // Methods

      virtual void Copy( __in CMixerControlDetailsData const&     source ) noexcept;
      virtual void Copy( __in MIXERCONTROLDETAILS_LISTTEXT const& source ) noexcept;
      virtual void Copy( __in MIXERCONTROLDETAILS_BOOLEAN const&  source ) noexcept;
      virtual void Copy( __in MIXERCONTROLDETAILS_SIGNED const&   source ) noexcept;
      virtual void Copy( __in MIXERCONTROLDETAILS_UNSIGNED const& source ) noexcept;
      virtual void Empty( void ) noexcept;

      // Operators

      virtual CMixerControlDetailsData& operator = ( __in CMixerControlDetailsData const&     source ) noexcept;
      virtual CMixerControlDetailsData& operator = ( __in MIXERCONTROLDETAILS_LISTTEXT const& source ) noexcept;
      virtual CMixerControlDetailsData& operator = ( __in MIXERCONTROLDETAILS_BOOLEAN const&  source ) noexcept;
      virtual CMixerControlDetailsData& operator = ( __in MIXERCONTROLDETAILS_SIGNED const&   source ) noexcept;
      virtual CMixerControlDetailsData& operator = ( __in MIXERCONTROLDETAILS_UNSIGNED const& source ) noexcept;

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
      CMixerControlDetails( __in CMixerControlDetails const& source ) noexcept;
      CMixerControlDetails( __in MIXERCONTROLDETAILS const&  source );
      virtual ~CMixerControlDetails();

      // Properties

      DWORD ID{ 0 };
      DWORD NumberOfChannels{ 0 };
      HWND  WindowHandleOfOwner{ nullptr };
      DWORD NumberOfMultipleItemsPerChannel{ 0 };

      // Methods

      virtual void Copy( __in CMixerControlDetails const& source ) noexcept;
      virtual void Copy( __in MIXERCONTROLDETAILS const&  source ) noexcept;
      virtual void Empty( void ) noexcept;

      // Operators

      virtual CMixerControlDetails& operator = ( __in CMixerControlDetails const& source ) noexcept;
      virtual CMixerControlDetails& operator = ( __in MIXERCONTROLDETAILS const&  source ) noexcept;

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
      CMixerControl( __in CMixerControl const& source );
      CMixerControl( __in MIXERCONTROL const&  source );
      virtual ~CMixerControl();

      // Properties

      enum class Types : uint32_t
      {
         Custom = MIXERCONTROL_CT_CLASS_CUSTOM,
         Fader  = MIXERCONTROL_CT_CLASS_FADER,
         List   = MIXERCONTROL_CT_CLASS_LIST,
         Meter  = MIXERCONTROL_CT_CLASS_METER,
         Number = MIXERCONTROL_CT_CLASS_NUMBER,
         Slider = MIXERCONTROL_CT_CLASS_SLIDER,
         Switch = MIXERCONTROL_CT_CLASS_SWITCH,
         Time   = MIXERCONTROL_CT_CLASS_TIME
      };

      enum class ControlType : uint32_t
      {
         Unknown        = 0,
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

      enum class StatusFlags : uint32_t
      {
         Disabled = MIXERCONTROL_CONTROLF_DISABLED,
         Multiple = MIXERCONTROL_CONTROLF_MULTIPLE,
         Uniform  = MIXERCONTROL_CONTROLF_UNIFORM
      };

      enum class Units : uint32_t
      {
         Custom   = MIXERCONTROL_CT_UNITS_CUSTOM,
         Boolean  = MIXERCONTROL_CT_UNITS_BOOLEAN,
         Signed   = MIXERCONTROL_CT_UNITS_SIGNED,
         Unsigned = MIXERCONTROL_CT_UNITS_UNSIGNED,
         Decibels = MIXERCONTROL_CT_UNITS_DECIBELS,
         Percent  = MIXERCONTROL_CT_UNITS_PERCENT
      };

      DWORD       ID{ 0 };
      ControlType Type{ ControlType::Unknown };
      DWORD       StatusFlags{ 0 };
      DWORD       NumberOfItemsPerChannel{ 0 };
      std::wstring ShortName;
      std::wstring Name;
      DWORD       Minimum{ 0 };
      DWORD       Maximum{ 0 };
      DWORD       NumberOfSteps{ 0 };
      DWORD       NumberOfCustomDataBytes{ 0 };
      std::vector<uint32_t> Data;

      // Methods

      virtual void  Copy( __in CMixerControl const& source ) noexcept;
      virtual void  Copy( __in MIXERCONTROL const& source  ) noexcept;
      virtual void  Empty( void ) noexcept;
      virtual _Check_return_ CMixerControl::Types GetType( void ) const noexcept;
      virtual void  GetTypeName( __out std::wstring& name ) const noexcept;
      virtual _Check_return_ Units GetUnits( void ) const noexcept;
      virtual _Check_return_ bool  IsBooleanUnits(  void ) const noexcept;
      virtual _Check_return_ bool  IsCustom(        void ) const noexcept;
      virtual _Check_return_ bool  IsCustomUnits(   void ) const noexcept;
      virtual _Check_return_ bool  IsDecibelsUnits( void ) const noexcept;
      virtual _Check_return_ bool  IsDisabled(      void ) const noexcept;
      virtual _Check_return_ bool  IsFader(         void ) const noexcept;
      virtual _Check_return_ bool  IsList(          void ) const noexcept;
      virtual _Check_return_ bool  IsMeter(         void ) const noexcept;
      virtual _Check_return_ bool  IsMultiple(      void ) const noexcept;
      virtual _Check_return_ bool  IsNumber(        void ) const noexcept;
      virtual _Check_return_ bool  IsPercentUnits(  void ) const noexcept;
      virtual _Check_return_ bool  IsSignedUnits(   void ) const noexcept;
      virtual _Check_return_ bool  IsSlider(        void ) const noexcept;
      virtual _Check_return_ bool  IsSwitch(        void ) const noexcept;
      virtual _Check_return_ bool  IsTime(          void ) const noexcept;
      virtual _Check_return_ bool  IsUniform(       void ) const noexcept;
      virtual _Check_return_ bool  IsUnsignedUnits( void ) const noexcept;

      // Operators

      virtual CMixerControl& operator = ( __in CMixerControl const& source ) noexcept;
      virtual CMixerControl& operator = ( __in MIXERCONTROL const&  source ) noexcept;

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
      CMixerLine( __in CMixerLine const& source );
      CMixerLine( __in MIXERLINE const&  source );
      virtual ~CMixerLine();

      // Properties

      enum class ComponentType : uint32_t
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

      enum class TargetType : uint32_t
      {
         Undefined = MIXERLINE_TARGETTYPE_UNDEFINED,
         WaveOut   = MIXERLINE_TARGETTYPE_WAVEOUT,
         WaveIn    = MIXERLINE_TARGETTYPE_WAVEIN,
         MidiOut   = MIXERLINE_TARGETTYPE_MIDIOUT,
         MidiIn    = MIXERLINE_TARGETTYPE_MIDIIN,
         Auxillary = MIXERLINE_TARGETTYPE_AUX
      };

      enum class Status : uint32_t
      {
         Active       = MIXERLINE_LINEF_ACTIVE,
         Disconnected = MIXERLINE_LINEF_DISCONNECTED,
         Source       = MIXERLINE_LINEF_SOURCE
      };

      DWORD    DestinationNumber{ 0 };
      DWORD    Source{ 0 };
      DWORD    ID{ 0 };
      DWORD    Status{ 0 };
      DWORD_PTR Reserved{ 0 };
      CMixerLine::ComponentType Component{ CMixerLine::ComponentType::destinationUndefined };
      DWORD    NumberOfChannels{ 0 };
      DWORD    NumberOfConnections{ 0 };
      DWORD    NumberOfControls{ 0 };
      std::wstring ShortName;
      std::wstring Name;
      TargetType    Target{ TargetType::Undefined };
      DWORD    TargetDeviceID{ 0 };
      DWORD    TargetManufacturer{ 0 };
      DWORD    TargetProduct{ 0 };
      DWORD    TargetDriverVersion{ 0 };
      std::wstring TargetName;

      // Methods

      virtual void Copy( __in CMixerLine const& source ) noexcept;
      virtual void Copy( __in MIXERLINE const&  source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual void GetComponent( __out std::wstring& component_string ) const noexcept;
      virtual void GetStatus( __out std::wstring& status_string ) const noexcept;
      virtual void GetTarget( __out std::wstring& target_string ) const noexcept;
      virtual _Check_return_ bool IsActive( void ) const noexcept;
      virtual _Check_return_ bool IsDisconnected( void ) const noexcept;
      virtual _Check_return_ bool IsSource( void ) const noexcept;

      // Operators

      virtual _Check_return_ CMixerLine& operator = ( __in CMixerLine const& source ) noexcept;
      virtual _Check_return_ CMixerLine& operator = ( __in MIXERLINE const&  source ) noexcept;

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
      CMixerCapabilities( __in CMixerCapabilities const& source ) noexcept;
      CMixerCapabilities( __in MIXERCAPS const&  source );
      virtual ~CMixerCapabilities();

      // Properties

      DWORD   Manufacturer{ 0 };
      DWORD   Product{ 0 };
      DWORD   Version{ 0 };
      std::wstring ProductName;
      DWORD   Support{ 0 };
      DWORD   NumberOfDestinations{ 0 };

      // Methods

      virtual void Copy( __in CMixerCapabilities const& source ) noexcept;
      virtual void Copy( __in MIXERCAPS const& source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ bool Get( __in UINT const device_number ) noexcept;

      // Operators

      virtual CMixerCapabilities& operator = ( __in CMixerCapabilities const& source ) noexcept;
      virtual CMixerCapabilities& operator = ( __in MIXERCAPS const&  source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CMixer
{
   protected:

       DWORD m_ErrorCode{ 0 };

       HMIXER m_Handle{ nullptr };

      UINT_PTR m_DeviceID{ 0 };

   public:

      // Construction

       CMixer(__in CMixer const&) = delete;
       CMixer& operator=(__in CMixer const&) = delete;
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
      virtual _Check_return_ bool  Get( __out CMixerCapabilities& capabilities ) noexcept;
      virtual _Check_return_ bool  GetAllControls( __in CMixerLine const& line, __out std::vector<CMixerControl>& array_of_CMixerControl_pointers ) noexcept;
      virtual _Check_return_ bool  GetByComponent( __in CMixerLine::ComponentType const component, __out CMixerLine& line ) noexcept;
      virtual _Check_return_ bool  GetByConnection( __in DWORD const destination_number, __in DWORD const source, __out CMixerLine& line ) noexcept;
      virtual _Check_return_ bool  GetByDestination( __in DWORD const destination_number, __out CMixerLine& line ) noexcept;
      virtual _Check_return_ bool  GetByID( __in DWORD const id, __out CMixerLine& line ) noexcept;
      virtual _Check_return_ bool  GetControlDetails(  __in CMixerLine const& line, __in CMixerControl const& control, __out std::vector<CMixerControlDetailsData>& array) noexcept;
      virtual _Check_return_ bool  GetControlListText( __in CMixerLine const& line, __in CMixerControl const& control, __out std::vector<CMixerControlDetailsData>& array ) noexcept;
      inline constexpr _Check_return_ UINT_PTR GetDeviceID(void) const noexcept { return(m_DeviceID); }
      inline constexpr _Check_return_ DWORD GetErrorCode(void) const noexcept { return(m_ErrorCode); };
      virtual void GetErrorString( __out std::wstring& error_string ) const noexcept;
      inline constexpr _Check_return_ HMIXEROBJ GetHandle(void) const noexcept { return((HMIXEROBJ)m_Handle); }
      virtual _Check_return_ std::size_t GetNumberOfDevices( void ) const noexcept;
      virtual _Check_return_ bool  Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0) noexcept;
      virtual _Check_return_ bool  SetControlDetails( __in CMixerLine const& line, __in CMixerControl const& control, __in std::vector<CMixerControlDetailsData> const& array) noexcept;

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

      DWORD m_WhatToNotify{ 0 };
      DWORD_PTR m_WhoToNotify{ 0 };
      DWORD_PTR m_NotifyData{ 0 };

      CMixer        m_Mixer;
      CMixerLine    m_MixerLine;
      CMixerControl m_MixerControl;

   public:

      // Construction

       inline CMixerControlInstance() noexcept
       {
           m_WhatToNotify = 0;
           m_WhoToNotify = 0;
           m_NotifyData = 0;
       }

       inline CMixerControlInstance(__in CMixerControlInstance const& source) noexcept
       {
           Copy(source);
       }

       inline virtual ~CMixerControlInstance() noexcept
       {
           m_WhatToNotify = 0;
           m_WhoToNotify = 0;
           m_NotifyData = 0;
       }

      // Methods

      virtual void Close( void ) noexcept;
      virtual void Copy( __in CMixerControlInstance const& source ) noexcept;
      virtual _Check_return_ bool Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0 ) noexcept;
      virtual void SetLine( __in CMixerLine const& line ) noexcept;
      virtual void SetControl( __in CMixerControl const& control ) noexcept;

      // Operators

      virtual CMixerControlInstance& operator = ( __in CMixerControlInstance const& source ) noexcept;

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

      virtual _Check_return_ bool m_GetSettings( void ) noexcept;
      virtual _Check_return_ bool m_SetSettings( void ) noexcept;

   public:

      // Construction

       inline CMixerSourceSelector(__in CMixerSourceSelector const& source) noexcept { Copy(source); }
       inline CMixerSourceSelector(__in CMixerLine const& destination, __in CMixerControl const& mixer_control) noexcept
       {
           if (m_MixerControl.Type != CMixerControl::ControlType::Mixer)
           {
               m_MixerLine.Empty();
           }

           SetLine(destination);
           SetControl(mixer_control);
       }

       inline virtual ~CMixerSourceSelector() noexcept { Close(); }

      // Methods

      virtual void  Copy( __in CMixerSourceSelector const& source ) noexcept;
      virtual _Check_return_ CMixerLine::ComponentType GetSource( void ) const noexcept;
      virtual _Check_return_ bool  IsSelected( __in DWORD const source ) noexcept;
      _Check_return_ bool  Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0 ) noexcept override;
      virtual _Check_return_ bool  Select( __in DWORD const source, __in bool selected = true ) noexcept;
      virtual _Check_return_ bool  Unselect( __in DWORD const source ) noexcept;

      // Operators

      virtual CMixerSourceSelector& operator = ( __in CMixerSourceSelector const& source ) noexcept;

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

      virtual _Check_return_ bool m_GetAll( void ) noexcept;

   public:

      // Construction

      CMixerVolumeControl() noexcept;
      CMixerVolumeControl( __in CMixerVolumeControl const& source ) noexcept;
      virtual ~CMixerVolumeControl();

      // Methods

      virtual void  Copy( __in CMixerVolumeControl const& source ) noexcept;
      virtual _Check_return_ DWORD GetLeftChannelVolume( void ) noexcept;
      virtual _Check_return_ DWORD GetMinimum( void ) const noexcept;
      virtual _Check_return_ DWORD GetMaximum( void ) const noexcept;
      virtual _Check_return_ DWORD GetRightChannelVolume( void ) noexcept;
      virtual _Check_return_ DWORD GetVolume( void ) noexcept;
      virtual _Check_return_ bool  SetLeftChannelVolume( __in DWORD const new_volume ) noexcept;
      virtual _Check_return_ bool  SetRightChannelVolume( __in DWORD const new_volume ) noexcept;
      virtual _Check_return_ bool  SetVolume( __in DWORD const new_volume ) noexcept;

      // Operators

      virtual _Check_return_ CMixerVolumeControl& operator = ( __in CMixerVolumeControl const& source ) noexcept;

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

      virtual _Check_return_ bool m_GetAll( void ) noexcept;

   public:

      // Construction

      CMixerSwitchControl();
      CMixerSwitchControl( __in CMixerSwitchControl const& source );
      virtual ~CMixerSwitchControl();

      // Methods

      virtual void Copy( __in CMixerSwitchControl const& source ) noexcept;
      virtual _Check_return_ bool GetState( void ) noexcept;
      virtual _Check_return_ bool SetState( __in bool const turn_it_on = true ) noexcept;
      virtual _Check_return_ bool TurnOff( void ) noexcept;
      virtual _Check_return_ bool TurnOn( void ) noexcept;

      // Operators

      virtual _Check_return_ CMixerSwitchControl& operator = ( __in CMixerSwitchControl const& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CMixerSource : public CMixerControlInstance
{
   public:

      // Construction

       inline CMixerSource() noexcept {}
       inline CMixerSource(__in CMixerSource const& source) noexcept { Copy(source); }
       inline virtual ~CMixerSource() {};

      // Methods

      virtual void Copy( __in CMixerSource const& source ) noexcept;
      virtual _Check_return_ bool GetControl( __out CMixerVolumeControl& control ) noexcept;
      _Check_return_ bool Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0 ) noexcept override;

      // Operators

      virtual CMixerSource& operator = ( __in CMixerSource const& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CMixerDestination : public CMixerControlInstance
{
   protected:

      // Properties

       std::unique_ptr<CMixerSourceSelector> m_SourceSelector;

   public:

      // Construction

       inline CMixerDestination() noexcept {};
       inline CMixerDestination(__in CMixerDestination const& source) noexcept { Copy(source); }
       inline virtual ~CMixerDestination() {}

      // Methods

      virtual void Copy( __in CMixerDestination const& source ) noexcept;
      virtual _Check_return_ bool GetSource( __in Win32FoundationClasses::CMixerLine::ComponentType const component_type, __out CMixerSource& source ) noexcept;
      virtual _Check_return_ bool IsSourceSelected( __in DWORD const CMixerLineSource ) noexcept;
      _Check_return_ bool Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0 ) noexcept override;
      virtual _Check_return_ bool SelectSource( __in DWORD const CMixerLineSource, __in bool new_selection = true ) noexcept;
      virtual _Check_return_ bool UnselectSource( __in DWORD const CMixerLineSource ) noexcept;

      // Operators

      virtual _Check_return_ CMixerDestination& operator = ( __in CMixerDestination const& source ) noexcept;

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
      CMixerWaveIn( __in CMixerWaveIn const& source );
      virtual ~CMixerWaveIn();

      // Methods

      virtual void  Copy( __in CMixerWaveIn const& source ) noexcept;
      virtual _Check_return_ DWORD GetLeftChannelRecordingGain( void ) noexcept;
      virtual _Check_return_ DWORD GetMaximumGain( void ) const noexcept;
      virtual _Check_return_ DWORD GetMinimumGain( void ) const noexcept;
      virtual _Check_return_ DWORD GetRecordingGain( void ) noexcept;
      virtual _Check_return_ DWORD GetRightChannelRecordingGain( void ) noexcept;
      _Check_return_ bool  Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0 ) noexcept override;
      virtual _Check_return_ bool  SetLeftChannelRecordingGain( __in DWORD const new_level ) noexcept;
      virtual _Check_return_ bool  SetRecordingGain( __in DWORD const new_level ) noexcept;
      virtual _Check_return_ bool  SetRightChannelRecordingGain( __in DWORD const new_level ) noexcept;

      // Operators

      virtual _Check_return_ CMixerWaveIn& operator = ( __in CMixerWaveIn const& source ) noexcept;

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

       inline CMixerSpeakers() noexcept {};
       inline CMixerSpeakers(__in CMixerSpeakers const& source) noexcept { Copy(source); }
       inline virtual ~CMixerSpeakers() {};

      // Methods

      virtual void Copy( __in CMixerSpeakers const& source ) noexcept;
      virtual _Check_return_ DWORD GetLeftChannelVolume( void ) noexcept;
      virtual _Check_return_ DWORD GetMaximumVolume( void ) noexcept;
      virtual _Check_return_ DWORD GetMinimumVolume( void ) noexcept;
      virtual _Check_return_ DWORD GetRightChannelVolume( void ) noexcept;
      virtual _Check_return_ DWORD GetVolume( void ) noexcept;
      virtual _Check_return_ bool  IsMuted( void ) noexcept;
      virtual _Check_return_ bool  Mute( __in bool const muting = true ) noexcept;
      _Check_return_ bool  Open( __in UINT_PTR device_id = 0, __in DWORD what_to_notify = 0, __in DWORD_PTR who_to_notify = 0, __in DWORD_PTR notify_data = 0 ) noexcept override;
      virtual _Check_return_ bool  SetLeftChannelVolume( __in DWORD const new_level ) noexcept;
      virtual _Check_return_ bool  SetRightChannelVolume( __in DWORD const new_level ) noexcept;
      virtual _Check_return_ bool  SetVolume( __in DWORD const new_level ) noexcept;
      virtual _Check_return_ bool  UnMute( void ) noexcept;

      // Operators

      virtual _Check_return_ CMixerSpeakers& operator = ( __in CMixerSpeakers const& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // MIXER_API_CLASS_HEADER
