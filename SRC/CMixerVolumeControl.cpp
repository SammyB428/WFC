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
** $Workfile: CMixerVolumeControl.cpp $
** $Revision: 16 $
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

USING_WFC_NAMESPACE

// Construction

CMixerVolumeControl::CMixerVolumeControl() noexcept
{
   WFC_VALIDATE_POINTER( this );
   (void) m_Channels.push_back( m_LeftChannel );
   (void) m_Channels.push_back( m_RightChannel );
}

CMixerVolumeControl::CMixerVolumeControl( __in CMixerVolumeControl const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Channels.push_back( m_LeftChannel );
   m_Channels.push_back( m_RightChannel );
   Copy( source );
}

CMixerVolumeControl::~CMixerVolumeControl() noexcept
{
   WFC_VALIDATE_POINTER( this );
}

// Methods

void CMixerVolumeControl::Copy( __in CMixerVolumeControl const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   (void) Open( source.m_Mixer.GetDeviceID(), source.m_WhatToNotify, source.m_WhoToNotify, source.m_NotifyData );

   m_Channels.clear();
   m_Channels.push_back(m_LeftChannel);
   m_Channels.push_back(m_RightChannel);
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CMixerVolumeControl::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CMixerVolumeControl at " ) << (VOID *) this << TEXT( "\n{\n" );
   CMixerControlInstance::Dump( dump_context );
   dump_context << TEXT( "   m_LeftChannel is " );
   m_LeftChannel.Dump( dump_context );
   dump_context << TEXT( "   m_RightChannel is " );
   m_RightChannel.Dump( dump_context );
   dump_context << TEXT( "   m_BothChannels is " );
   m_BothChannels.Dump( dump_context );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ DWORD CMixerVolumeControl::GetMaximum( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_MixerControl.Maximum );
}

_Check_return_ DWORD CMixerVolumeControl::GetMinimum( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_MixerControl.Minimum );
}

_Check_return_ DWORD CMixerVolumeControl::GetLeftChannelVolume( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD gain = 0;

   if ( m_GetAll() == true )
   {
      gain = m_LeftChannel.Parameter1;
   }

   return( gain );
}

_Check_return_ DWORD CMixerVolumeControl::GetVolume( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD gain = 0;

   if ( m_GetAll() == true )
   {
      gain = m_BothChannels.Parameter1;
   }

   return( gain );
}

_Check_return_ DWORD CMixerVolumeControl::GetRightChannelVolume( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD gain = 0;

   if ( m_GetAll() == true )
   {
      gain = m_RightChannel.Parameter1;
   }

   return( gain );
}

_Check_return_ bool CMixerVolumeControl::m_GetAll( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   std::vector<CMixerControlDetailsData> array;

   bool const return_value = m_Mixer.GetControlDetails( m_MixerLine, m_MixerControl, array );

   if ( return_value == false )
   {
      Close();
      return( false );
   }

   if (array.empty() == false)
   {
       m_LeftChannel.Copy(array.at(0));
   }

   if (array.size() > 1)
   {
       m_RightChannel.Copy(array.at(1));
   }

   if ( m_LeftChannel.Parameter1 > m_RightChannel.Parameter1 )
   {
      m_BothChannels.Parameter1 = m_LeftChannel.Parameter1;
   }
   else
   {
      m_BothChannels.Parameter1 = m_RightChannel.Parameter1;
   }

   return( return_value );
}

_Check_return_ bool CMixerVolumeControl::SetLeftChannelVolume( __in DWORD const desired_level ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD new_level = desired_level;

   if ( new_level < m_MixerControl.Minimum )
   {
      new_level = m_MixerControl.Minimum;
   }

   if ( new_level > m_MixerControl.Maximum )
   {
      new_level = m_MixerControl.Maximum;
   }

   (void) m_GetAll();

   m_LeftChannel.Parameter1 = new_level;

   bool const return_value = m_Mixer.SetControlDetails( m_MixerLine, m_MixerControl, m_Channels );

   if ( return_value != FALSE )
   {
      (void) m_GetAll();
   }

   return( return_value );
}

_Check_return_ bool CMixerVolumeControl::SetVolume( __in DWORD const desired_level ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD new_level = desired_level;

   if ( new_level < m_MixerControl.Minimum )
   {
      new_level = m_MixerControl.Minimum;
   }

   if ( new_level > m_MixerControl.Maximum )
   {
      new_level = m_MixerControl.Maximum;
   }

   m_LeftChannel.Parameter1  = new_level;
   m_RightChannel.Parameter1 = new_level;

   bool const return_value = m_Mixer.SetControlDetails( m_MixerLine, m_MixerControl, m_Channels );

   if ( return_value == true )
   {
      (void) m_GetAll();
   }

   return( return_value );
}

_Check_return_ bool CMixerVolumeControl::SetRightChannelVolume( __in DWORD const desired_level ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD new_level = desired_level;

   if ( new_level < m_MixerControl.Minimum )
   {
      new_level = m_MixerControl.Minimum;
   }

   if ( new_level > m_MixerControl.Maximum )
   {
      new_level = m_MixerControl.Maximum;
   }

   (void) m_GetAll();

   m_RightChannel.Parameter1 = new_level;

   bool const return_value = m_Mixer.SetControlDetails( m_MixerLine, m_MixerControl, m_Channels );

   if ( return_value == true)
   {
      (void) m_GetAll();
   }

   return( return_value );
}

_Check_return_ CMixerVolumeControl& CMixerVolumeControl::operator=( __in CMixerVolumeControl const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
