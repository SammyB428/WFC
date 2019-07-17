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
** $Workfile: CMixerDestination.cpp $
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

CMixerDestination::CMixerDestination()
{
   WFC_VALIDATE_POINTER( this );
   m_SourceSelector_p = nullptr;
}

CMixerDestination::CMixerDestination( __in CMixerDestination const& source )
{
   WFC_VALIDATE_POINTER( this );
   m_SourceSelector_p = nullptr;
   Copy( source );
}

CMixerDestination::~CMixerDestination()
{
   WFC_VALIDATE_POINTER( this );

   if ( m_SourceSelector_p != nullptr )
   {
      WFC_TRY
      {
         delete m_SourceSelector_p;
      }
      WFC_CATCH_ALL
      {
         ;
      }
      WFC_END_CATCH_ALL
   }

   m_SourceSelector_p = nullptr;
}

// Methods

void CMixerDestination::Copy( __in CMixerDestination const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( m_SourceSelector_p );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   if ( m_SourceSelector_p != nullptr )
   {
      WFC_TRY
      {
         delete m_SourceSelector_p;
      }
      WFC_CATCH_ALL
      {
      }
      WFC_END_CATCH_ALL

      m_SourceSelector_p = nullptr;
   }

   CMixerControlInstance::Copy( source );
}

// Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CMixerDestination::Dump( CDumpContext& dump_context ) const
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

_Check_return_ bool CMixerDestination::GetSource( __in CMixerLine::ComponentType const type_of_source, __out CMixerSource& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   CMixerLine line;

   for ( auto const source_number : Range(m_MixerLine.NumberOfConnections) )
   {
      if ( m_Mixer.GetByConnection( m_MixerLine.DestinationNumber, static_cast<DWORD>(source_number), line ) != false )
      {
         if ( line.Component == type_of_source )
         {
            source.SetLine( line );
            (void) source.Open( m_Mixer.GetDeviceID(), m_WhatToNotify, m_WhoToNotify, m_NotifyData );
            return( true );
         }
      }
   }

   return( false );
}

_Check_return_ bool CMixerDestination::IsSourceSelected( __in DWORD const source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_SourceSelector_p );

   bool return_value = false;

   if ( m_SourceSelector_p != nullptr )
   {
      return_value = m_SourceSelector_p->IsSelected( source );
   }
   else
   {
      //WFCTRACE( TEXT( "m_SourceSelector_p is NULL" ) );
   }

   return( return_value );
}

_Check_return_ bool CMixerDestination::Open( __in UINT_PTR device_number, __in DWORD what_to_notify, __in DWORD_PTR who_to_notify, __in DWORD_PTR notify_data ) noexcept
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

   return( return_value );
}

_Check_return_ bool CMixerDestination::SelectSource(__in DWORD const source, __in bool selection) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_SourceSelector_p );

   bool return_value = false;

   if ( m_SourceSelector_p != nullptr )
   {
      return_value = m_SourceSelector_p->Select( source, selection );
   }

   return( return_value );
}

_Check_return_ bool CMixerDestination::UnselectSource( __in DWORD const source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_SourceSelector_p );

   bool return_value = false;

   if ( m_SourceSelector_p != nullptr )
   {
      return_value = m_SourceSelector_p->Unselect( source );
   }

   return( return_value );
}

_Check_return_ CMixerDestination& CMixerDestination::operator = ( __in CMixerDestination const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
