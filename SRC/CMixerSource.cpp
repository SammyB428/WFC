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
** $Workfile: CMixerSource.cpp $
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

CMixerSource::CMixerSource()
{
   WFC_VALIDATE_POINTER( this );
}

CMixerSource::CMixerSource( __in const CMixerSource& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CMixerSource::~CMixerSource()
{
   WFC_VALIDATE_POINTER( this );
}

// Methods

void CMixerSource::Copy( __in const CMixerSource& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   CMixerControlInstance::Copy( source );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CMixerSource::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   dump_context << TEXT( "a CMixerSource at " ) << (VOID *) this << TEXT( "\n{\n" );
   CMixerControlInstance::Dump( dump_context );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

__checkReturn bool CMixerSource::GetControl( __out CMixerVolumeControl& control ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   bool return_value = false;

   std::vector<CMixerControl> controls_array;

   if ( m_Mixer.GetAllControls( m_MixerLine, controls_array ) != FALSE )
   {
      for ( const auto& entry : controls_array )
      {
         if (entry.IsFader() )
         {
            return_value = true;
            control.SetLine( m_MixerLine );
            control.SetControl(entry);
            (void) control.Open( m_Mixer.GetDeviceID(), m_WhatToNotify, m_WhoToNotify, m_NotifyData );
         }
      }
   }
#if defined( _DEBUG )
   else
   {
      std::wstring error_string;
      m_Mixer.GetErrorString( error_string );
      //WFCTRACEVAL( TEXT( "GetAllControls() failed with " ), error_string );
   }
#endif // _DEBUG

   return( return_value );
}

__checkReturn bool CMixerSource::Open( __in UINT_PTR device_number, __in DWORD what_to_notify, __in DWORD_PTR who_to_notify, __in DWORD_PTR notify_data ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_WhatToNotify = what_to_notify;
   m_WhoToNotify  = who_to_notify;
   m_NotifyData   = notify_data;

   const bool return_value = CMixerControlInstance::Open( device_number, what_to_notify, who_to_notify, notify_data );

   if ( return_value == false )
   {
      return( false );
   }

   return( true );
}

CMixerSource& CMixerSource::operator=( __in const CMixerSource& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
