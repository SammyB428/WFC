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
** $Workfile: CMixerDestination.cpp $
** $Revision: 17 $
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

// Methods

void Win32FoundationClasses::CMixerDestination::Copy( _In_ Win32FoundationClasses::CMixerDestination const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   m_SourceSelector = nullptr;

   Win32FoundationClasses::CMixerControlInstance::Copy( source );
}

// Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CMixerDestination::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   dump_context << TEXT( "a CMixerDestination at " ) << (VOID *) this << TEXT( "\n{\n" );

   if ( m_SourceSelector_p == nullptr )
   {
      dump_context << TEXT( "   m_SourceSelector_p is NULL\n" );
   }
   else
   {
      dump_context << TEXT( "   m_SourceSelector_p is " );
      m_SourceSelector_p->Dump( dump_context );
   }

   CMixerControlInstance::Dump( dump_context );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ bool Win32FoundationClasses::CMixerDestination::GetSource( _In_ Win32FoundationClasses::CMixerLine::ComponentType const type_of_source, _Out_ Win32FoundationClasses::CMixerSource& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Win32FoundationClasses::CMixerLine line;

   for ( auto const source_number : Range(m_MixerLine.NumberOfConnections) )
   {
      if ( m_Mixer.GetByConnection( m_MixerLine.DestinationNumber, static_cast<DWORD>(source_number), line ) == true )
      {
         if ( line.Component == type_of_source )
         {
            source.SetLine( line );
            std::ignore = source.Open( m_Mixer.GetDeviceID(), m_WhatToNotify, m_WhoToNotify, m_NotifyData );
            return( true );
         }
      }
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CMixerDestination::IsSourceSelected( _In_ DWORD const source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   bool return_value{ false };

   if ( m_SourceSelector.get() not_eq nullptr )
   {
      return_value = m_SourceSelector->IsSelected( source );
   }
   else
   {
      //WFCTRACE( TEXT( "m_SourceSelector_p is NULL" ) );
   }

   return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CMixerDestination::Open( _In_ UINT_PTR device_number, _In_ DWORD what_to_notify, _In_ DWORD_PTR who_to_notify, _In_ DWORD_PTR notify_data ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_WhatToNotify = what_to_notify;
   m_WhoToNotify  = who_to_notify;
   m_NotifyData   = notify_data;

   auto const return_value{ CMixerControlInstance::Open(device_number, what_to_notify, who_to_notify, notify_data) };

   if ( return_value == false )
   {
      return( false );
   }

   return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CMixerDestination::SelectSource(_In_ DWORD const source, _In_ bool selection) noexcept
{
   WFC_VALIDATE_POINTER( this );

   bool return_value{ false };

   if ( m_SourceSelector.get() not_eq nullptr )
   {
      return_value = m_SourceSelector->Select( source, selection );
   }

   return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CMixerDestination::UnselectSource( _In_ DWORD const source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   bool return_value{ false };

   if ( m_SourceSelector.get() not_eq nullptr )
   {
      return_value = m_SourceSelector->Unselect( source );
   }

   return( return_value );
}

_Check_return_ Win32FoundationClasses::CMixerDestination& Win32FoundationClasses::CMixerDestination::operator = ( _In_ Win32FoundationClasses::CMixerDestination const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
