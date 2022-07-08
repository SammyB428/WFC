/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2022, Samuel R. Blackburn
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
** $Workfile: CMixerSwitchControl.cpp $
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
static auto const THIS_FILE{ __FILE__ };
#endif // _DEBUG

// Construction

Win32FoundationClasses::CMixerSwitchControl::CMixerSwitchControl()
{
   WFC_VALIDATE_POINTER( this );
   m_Array.push_back( m_Switch );
}

Win32FoundationClasses::CMixerSwitchControl::CMixerSwitchControl( _In_ Win32FoundationClasses::CMixerSwitchControl const& source )
{
   WFC_VALIDATE_POINTER( this );
   m_Array.push_back( m_Switch );
   Copy( source );
}

Win32FoundationClasses::CMixerSwitchControl::~CMixerSwitchControl()
{
   WFC_VALIDATE_POINTER( this );
}

// Methods

void Win32FoundationClasses::CMixerSwitchControl::Copy( _In_ Win32FoundationClasses::CMixerSwitchControl const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   std::ignore = Open( source.m_Mixer.GetDeviceID(), source.m_WhatToNotify, source.m_WhoToNotify, source.m_NotifyData );
   m_Array.clear();
   m_Array.push_back(m_Switch);
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CMixerSwitchControl::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CMixerSwitchControl at " ) << (VOID *) this << TEXT( "\n{\n" );
   CMixerControlInstance::Dump( dump_context );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ bool Win32FoundationClasses::CMixerSwitchControl::GetState( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto const return_value{ m_GetAll() };

   if ( return_value == true )
   {
      return( ( ( m_Switch.Parameter1 == 0 ) ? false : true ) );
   }

   return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CMixerSwitchControl::m_GetAll( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   std::vector<CMixerControlDetailsData> array;

   auto const return_value{ m_Mixer.GetControlDetails(m_MixerLine, m_MixerControl, array) };

   if ( return_value == false )
   {
      Close();
      return( false );
   }

   if (array.empty() == false)
   {
       // Left channel
       m_Switch.Copy(array.front());
   }

   return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CMixerSwitchControl::SetState( _In_ bool const turn_on ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Set left channel level

   m_Switch.Parameter1 = turn_on;

   auto return_value{ m_Mixer.SetControlDetails(m_MixerLine, m_MixerControl, m_Array) };

   if ( return_value == true )
   {
       std::ignore = m_GetAll();
   }

   return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CMixerSwitchControl::TurnOff( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto const return_value{ SetState(false) };

   return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CMixerSwitchControl::TurnOn( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto const return_value{ SetState(true) };

   return( return_value );
}

_Check_return_ Win32FoundationClasses::CMixerSwitchControl& Win32FoundationClasses::CMixerSwitchControl::operator=( _In_ Win32FoundationClasses::CMixerSwitchControl const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
