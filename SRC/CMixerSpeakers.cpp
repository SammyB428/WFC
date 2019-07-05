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
** $Workfile: CMixerSpeakers.cpp $
** $Revision: 18 $
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

CMixerSpeakers::CMixerSpeakers()
{
   WFC_VALIDATE_POINTER( this );
}

CMixerSpeakers::CMixerSpeakers( __in const CMixerSpeakers& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CMixerSpeakers::~CMixerSpeakers()
{
   WFC_VALIDATE_POINTER( this );
}

// Methods

void CMixerSpeakers::Copy( __in const CMixerSpeakers& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   m_Volume.Copy( source.m_Volume );
   m_Mute.Copy( source.m_Mute );
   (void) Open( source.m_Mixer.GetDeviceID(), source.m_WhatToNotify, source.m_WhoToNotify, source.m_NotifyData );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CMixerSpeakers::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   dump_context << TEXT( "a CMixerSpeakers at " ) << (VOID *) this << TEXT( "\n{\n" );
   CMixerDestination::Dump( dump_context );

   dump_context << TEXT( "   m_Volume is " );
   m_Volume.Dump( dump_context );

   dump_context << TEXT( "   m_Mute is " );
   m_Mute.Dump( dump_context );

   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ DWORD CMixerSpeakers::GetMaximumVolume( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_Volume.GetMaximum() );
}

_Check_return_ DWORD CMixerSpeakers::GetMinimumVolume( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_Volume.GetMinimum() );
}

_Check_return_ DWORD CMixerSpeakers::GetLeftChannelVolume( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto const volume = m_Volume.GetLeftChannelVolume();

   return( volume );
}

_Check_return_ DWORD CMixerSpeakers::GetVolume( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto const volume = m_Volume.GetVolume();

   return( volume );
}

_Check_return_ DWORD CMixerSpeakers::GetRightChannelVolume( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto const volume = m_Volume.GetRightChannelVolume();

   return( volume );
}

_Check_return_ bool CMixerSpeakers::IsMuted( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Mute.GetState() != false )
   {
      return( true );
   }

   return( false );
}

_Check_return_ bool CMixerSpeakers::Mute( __in const bool mute_on ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   const bool return_value = m_Mute.SetState( mute_on );

   return( return_value );
}

_Check_return_ bool CMixerSpeakers::Open( __in UINT_PTR device_number, __in DWORD what_to_notify, __in DWORD_PTR who_to_notify, __in DWORD_PTR notify_data ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_WhatToNotify = what_to_notify;
   m_WhoToNotify  = who_to_notify;
   m_NotifyData   = notify_data;

   bool return_value = CMixerDestination::Open( device_number, what_to_notify, who_to_notify, notify_data );

   if ( return_value == false )
   {
      return( false );
   }

   return_value = m_Mixer.GetByComponent( CMixerLine::ComponentType::destinationSpeakers, m_MixerLine );

   if ( return_value == false )
   {
      Close();
      return( false );
   }

   std::vector<CMixerControl> array;

   return_value = m_Mixer.GetAllControls( m_MixerLine, array );

   if ( return_value == false )
   {
      Close();
      return( false );
   }

   bool volume_control_was_found = false;
   bool mute_control_was_found   = false;
 
   for ( auto const& entry : array )
   {
      if ( entry.Type == CMixerControl::ControlType::Mixer )
      {
         WFC_TRY
         {
            m_SourceSelector_p = new CMixerSourceSelector( m_MixerLine, entry );
         }
         WFC_CATCH_ALL
         {
            m_SourceSelector_p = nullptr;
         }
         WFC_END_CATCH_ALL

         WFC_VALIDATE_POINTER_NULL_OK( m_SourceSelector_p );
         ASSERT( m_SourceSelector_p != nullptr );

         if ( m_SourceSelector_p != nullptr )
         {
            (void) m_SourceSelector_p->Open( device_number, what_to_notify, who_to_notify, notify_data );
         }
      }

      if ( entry.Type == CMixerControl::ControlType::Volume )
      {
         if ( volume_control_was_found == false )
         {
            m_Volume.SetLine( m_MixerLine );
            m_Volume.SetControl( entry );
            (void) m_Volume.Open( device_number, what_to_notify, who_to_notify, notify_data );
            volume_control_was_found = true;
         }
         else
         {
            //WFCTRACE( TEXT( "More than one volume control was found" ) );
         }
      }

      if ( entry.Type == CMixerControl::ControlType::Mute )
      {
         if ( mute_control_was_found == false )
         {
            m_Mute.SetLine( m_MixerLine );
            m_Mute.SetControl( entry );
            (void) m_Mute.Open( device_number, what_to_notify, who_to_notify, notify_data );
            mute_control_was_found = true;
         }
         else
         {
            //WFCTRACE( TEXT( "More than one volume control was found" ) );
         }
      }
   }

   if ( volume_control_was_found == false )
   {
      Close();
      return( false );
   }

   if ( mute_control_was_found == false )
   {
      Close();
      return( false );
   }

   return( return_value );
}

_Check_return_ bool CMixerSpeakers::SetLeftChannelVolume( __in DWORD const desired_level ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   const bool return_value = m_Volume.SetLeftChannelVolume( desired_level );

   return( return_value );
}

_Check_return_ bool CMixerSpeakers::SetVolume( __in DWORD const desired_level ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   const bool return_value = m_Volume.SetVolume( desired_level );

   return( return_value );
}

_Check_return_ bool CMixerSpeakers::SetRightChannelVolume( __in DWORD const desired_level ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   const bool return_value = m_Volume.SetRightChannelVolume( desired_level );

   return( return_value );
}

_Check_return_ bool CMixerSpeakers::UnMute( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   const bool return_value = m_Mute.TurnOff();

   return( return_value );
}

_Check_return_ CMixerSpeakers& CMixerSpeakers::operator=( __in CMixerSpeakers const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
