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
** $Workfile: CMixerWaveIn.cpp $
** $Revision: 17 $
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

CMixerWaveIn::CMixerWaveIn()
{
   WFC_VALIDATE_POINTER( this );
}

CMixerWaveIn::CMixerWaveIn( __in const CMixerWaveIn& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CMixerWaveIn::~CMixerWaveIn()
{
   WFC_VALIDATE_POINTER( this );
   Close();
}

// Methods

void CMixerWaveIn::Copy( __in const CMixerWaveIn& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   m_RecordingGain.Copy( source.m_RecordingGain );
   (void) Open( source.m_Mixer.GetDeviceID(), source.m_WhatToNotify, source.m_WhoToNotify, source.m_NotifyData );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CMixerWaveIn::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CMixerWaveIn at " ) << (VOID *) this << TEXT( "\n{\n" );
   CMixerControlInstance::Dump( dump_context );
   dump_context << TEXT( "   m_RecordingGain is " );
   m_RecordingGain.Dump( dump_context );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

__checkReturn DWORD CMixerWaveIn::GetMaximumGain( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_RecordingGain.GetMaximum() ) ;
}

__checkReturn DWORD CMixerWaveIn::GetMinimumGain( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_RecordingGain.GetMinimum() );
}

__checkReturn DWORD CMixerWaveIn::GetLeftChannelRecordingGain( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   const DWORD gain = m_RecordingGain.GetLeftChannelVolume();

   return( gain );
}

__checkReturn DWORD CMixerWaveIn::GetRecordingGain( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   const DWORD gain = m_RecordingGain.GetVolume();

   return( gain );
}

__checkReturn DWORD CMixerWaveIn::GetRightChannelRecordingGain( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   const DWORD gain = m_RecordingGain.GetRightChannelVolume();

   return( gain );
}

__checkReturn bool CMixerWaveIn::Open( __in UINT_PTR device_number, __in DWORD what_to_notify, __in DWORD_PTR who_to_notify, __in DWORD_PTR notify_data ) noexcept
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

   return_value = m_Mixer.GetByComponent( CMixerLine::destinationWaveIn, m_MixerLine );

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

   bool control_was_found = false;

   for ( const auto& entry : array )
   {
      if ( entry.Type == CMixerControl::Mixer )
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

      if ( entry.Type == CMixerControl::Volume )
      {
         if ( control_was_found == false )
         {
            m_RecordingGain.SetLine( m_MixerLine );
            m_RecordingGain.SetControl( entry );
            (void) m_RecordingGain.Open( device_number, what_to_notify, who_to_notify, notify_data );
            control_was_found = true;
         }
      }
   }

   if ( control_was_found == false )
   {
      Close();
      return( false );
   }

   return( return_value );
}

__checkReturn bool CMixerWaveIn::SetLeftChannelRecordingGain( __in const DWORD desired_level ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   const bool return_value = m_RecordingGain.SetLeftChannelVolume( desired_level );

   return( return_value );
}

__checkReturn bool CMixerWaveIn::SetRecordingGain( __in const DWORD desired_level ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   const bool return_value = m_RecordingGain.SetVolume( desired_level );

   return( return_value );
}

__checkReturn bool CMixerWaveIn::SetRightChannelRecordingGain( __in const DWORD desired_level ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   const bool return_value = m_RecordingGain.SetRightChannelVolume( desired_level );

   return( return_value );
}

__checkReturn CMixerWaveIn& CMixerWaveIn::operator=( __in const CMixerWaveIn& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
