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
** $Workfile: CMixerControlInstance.cpp $
** $Revision: 19 $
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

CMixerControlInstance::CMixerControlInstance()
{
   WFC_VALIDATE_POINTER( this );

   m_WhatToNotify = 0;
   m_WhoToNotify  = 0;
   m_NotifyData   = 0;
}

CMixerControlInstance::CMixerControlInstance( __in CMixerControlInstance const& source )
{
   WFC_VALIDATE_POINTER( this );

   m_WhatToNotify = 0;
   m_WhoToNotify  = 0;
   m_NotifyData   = 0;

   Copy( source );
}

CMixerControlInstance::~CMixerControlInstance()
{
   WFC_VALIDATE_POINTER( this );

   m_WhatToNotify = 0;
   m_WhoToNotify  = 0;
   m_NotifyData   = 0;
}

void CMixerControlInstance::Copy( __in CMixerControlInstance const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   (void) m_Mixer.Open( source.m_Mixer.GetDeviceID(), source.m_WhatToNotify, source.m_WhoToNotify, source.m_NotifyData );
   m_MixerLine.Copy( source.m_MixerLine );
   m_MixerControl.Copy( source.m_MixerControl );
}

void CMixerControlInstance::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Mixer.Close();
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CMixerControlInstance::Dump( CDumpContext& dump_context ) const
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

_Check_return_ bool CMixerControlInstance::Open( __in UINT_PTR device_number, __in DWORD what_to_notify, __in DWORD_PTR who_to_notify, __in DWORD_PTR notify_data ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_WhatToNotify = what_to_notify;
   m_WhoToNotify  = who_to_notify;
   m_NotifyData   = notify_data;

   const bool return_value = m_Mixer.Open( device_number, what_to_notify, who_to_notify, notify_data );

   if ( return_value == false )
   {
      return( false );
   }

   return( true );
}

void CMixerControlInstance::SetLine( __in CMixerLine const& line ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_MixerLine.Copy( line );
}

void CMixerControlInstance::SetControl( __in CMixerControl const& control ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_MixerControl.Copy( control );
}

CMixerControlInstance& CMixerControlInstance::operator=( __in CMixerControlInstance const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
