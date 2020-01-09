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
** $Workfile: CMixerSourceSelector.cpp $
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
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

USING_WFC_NAMESPACE

// Methods

void CMixerSourceSelector::Copy( __in CMixerSourceSelector const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      //WFCTRACE( TEXT( "Attempt to make a copy of ourself (such silliness)." ) );
      return;
   }

   m_MixerLine.Copy( source.m_MixerLine );
   m_MixerControl.Copy( source.m_MixerControl );

   if ( m_MixerControl.Type != CMixerControl::ControlType::Mixer )
   {
      m_MixerControl.Empty();
   }

   (void) Open( source.m_Mixer.GetDeviceID(), source.m_WhatToNotify, source.m_WhoToNotify, source.m_NotifyData );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CMixerSourceSelector::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CMixerSourceSelector at " ) << (VOID *) this << TEXT( "\n{\n" );
   CMixerControlInstance::Dump( dump_context );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ CMixerLine::ComponentType CMixerSourceSelector::GetSource( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_MixerLine.Component );
}

_Check_return_ bool CMixerSourceSelector::IsSelected( __in DWORD const source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   bool return_value = m_GetSettings();

   for ( auto const& entry : m_Settings )
   {
      if ( entry.Parameter2 == source )
      {
         return_value = ( ( entry.Parameter1 == FALSE ) ? false : true );
      }
   }

   return( return_value );
}

_Check_return_ bool CMixerSourceSelector::m_GetSettings( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   std::vector<CMixerControlDetailsData> array;

   bool return_value = m_Mixer.GetControlDetails( m_MixerLine, m_MixerControl, array );

   if ( return_value == true)
   {
      for ( auto const index : Range(array.size()) )
      {
         if (index < m_Settings.size() )
         {
             m_Settings.at(index).Parameter1 = array.at(index).Parameter1;
         }
      }
   }

   return( return_value );
}

_Check_return_ bool CMixerSourceSelector::m_SetSettings( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   bool const return_value = m_Mixer.SetControlDetails( m_MixerLine, m_MixerControl, m_Settings );

   return( return_value );
}

_Check_return_ bool CMixerSourceSelector::Open( __in UINT_PTR device_number, __in DWORD what_to_notify, __in DWORD_PTR who_to_notify, __in DWORD_PTR notify_data ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_WhatToNotify = what_to_notify;
   m_WhoToNotify  = who_to_notify;
   m_NotifyData   = notify_data;

   bool return_value = CMixerControlInstance::Open( device_number, what_to_notify, who_to_notify, notify_data );

   if ( return_value == false )
   {
      //WFCTRACE( TEXT( "Can't open device" ) );
      return( false );
   }

   return_value = m_Mixer.GetControlListText( m_MixerLine, m_MixerControl, m_Settings );

   if ( return_value == false )
   {
      Close();
      return( false );
   }

   return_value = m_GetSettings();

   return( return_value );
}

_Check_return_ bool CMixerSourceSelector::Select( __in DWORD const source, __in bool selected ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   bool return_value = m_GetSettings();

   bool source_was_found = false;

   for ( auto& entry : m_Settings )
   {
      if ( entry.Parameter2 == source )
      {
         source_was_found = true;
         entry.Parameter1 = ( selected == false ) ? FALSE : TRUE;
      }
   }

   if ( source_was_found == true )
   {
      return_value = m_SetSettings();
   }
   else
   {
      //WFCTRACE( TEXT( "Source was not found" ) );
   }

   return( return_value );
}

_Check_return_ bool CMixerSourceSelector::Unselect( __in DWORD const source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   bool return_value = Select( source, false );

   return( return_value );
}

CMixerSourceSelector& CMixerSourceSelector::operator=( __in CMixerSourceSelector const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
