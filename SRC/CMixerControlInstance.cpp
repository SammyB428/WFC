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
** $Workfile: CMixerControlInstance.cpp $
** $Revision: 19 $
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

void Win32FoundationClasses::CMixerControlInstance::Copy( _In_ Win32FoundationClasses::CMixerControlInstance const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   std::ignore = m_Mixer.Open( source.m_Mixer.GetDeviceID(), source.m_WhatToNotify, source.m_WhoToNotify, source.m_NotifyData );
   m_MixerLine.Copy( source.m_MixerLine );
   m_MixerControl.Copy( source.m_MixerControl );
}

void Win32FoundationClasses::CMixerControlInstance::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Mixer.Close();
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CMixerControlInstance::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   std::wstring temp_string;

   dump_context << TEXT( "a CMixerControlInstance at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   m_Mixer is " );
   m_Mixer.Dump( dump_context );
   dump_context << TEXT( "   m_MixerLine is " );
   m_MixerLine.Dump( dump_context );
   dump_context << TEXT( "   m_MixerControl is " );
   m_MixerControl.Dump( dump_context );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ bool Win32FoundationClasses::CMixerControlInstance::Open( _In_ UINT_PTR device_number, _In_ DWORD what_to_notify, _In_ DWORD_PTR who_to_notify, _In_ DWORD_PTR notify_data ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_WhatToNotify = what_to_notify;
   m_WhoToNotify  = who_to_notify;
   m_NotifyData   = notify_data;

   auto const return_value{ m_Mixer.Open(device_number, what_to_notify, who_to_notify, notify_data) };

   if ( return_value == false )
   {
      return( false );
   }

   return( true );
}

void Win32FoundationClasses::CMixerControlInstance::SetLine( _In_ Win32FoundationClasses::CMixerLine const& line ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_MixerLine.Copy( line );
}

void Win32FoundationClasses::CMixerControlInstance::SetControl( _In_ Win32FoundationClasses::CMixerControl const& control ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_MixerControl.Copy( control );
}

Win32FoundationClasses::CMixerControlInstance& Win32FoundationClasses::CMixerControlInstance::operator=( _In_ Win32FoundationClasses::CMixerControlInstance const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
