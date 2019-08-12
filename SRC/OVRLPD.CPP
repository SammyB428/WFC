/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2015, Samuel R. Blackburn
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
** $Workfile: ovrlpd.cpp $
** $Revision: 18 $
** $Modtime: 6/26/01 10:54a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

// Construction

COverlapped::COverlapped()
{
   WFC_VALIDATE_POINTER( this );

   Internal     = 0;
   InternalHigh = 0;
   Offset       = 0;
   OffsetHigh   = 0;
   hEvent       = MANUAL_RESET_EVENT();
   m_AutomaticallyDelete = true;
}

COverlapped::COverlapped( __in OVERLAPPED const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   Internal     = 0;
   InternalHigh = 0;
   Offset       = 0;
   OffsetHigh   = 0;
   hEvent       = static_cast< HANDLE >( NULL );
   m_AutomaticallyDelete = false;

   Copy( source );
}

COverlapped::COverlapped( __in OVERLAPPED const& source )
{
   WFC_VALIDATE_POINTER( this );

   Internal     = 0;
   InternalHigh = 0;
   Offset       = 0;
   OffsetHigh   = 0;
   hEvent       = static_cast< HANDLE >( NULL );
   m_AutomaticallyDelete = false;

   Copy( source );
}

COverlapped::COverlapped( __in COverlapped const& source )
{
   WFC_VALIDATE_POINTER( this );

   Internal     = 0;
   InternalHigh = 0;
   Offset       = 0;
   OffsetHigh   = 0;
   hEvent       = static_cast< HANDLE >( NULL );
   m_AutomaticallyDelete = false;

   Copy( source );
}

COverlapped::~COverlapped()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

// Methods

void COverlapped::Copy( __in COverlapped const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Empty();

   Internal     = source.Internal;
   InternalHigh = source.InternalHigh;
   Offset       = source.Offset;
   OffsetHigh   = source.OffsetHigh;
   hEvent       = source.hEvent;
   SetAutomaticallyDelete( false );
}

void COverlapped::Copy( __in OVERLAPPED const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   Empty();

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      Internal     = source->Internal;
      InternalHigh = source->InternalHigh;
      Offset       = source->Offset;
      OffsetHigh   = source->OffsetHigh;
      hEvent       = source->hEvent;
      SetAutomaticallyDelete( false );
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void COverlapped::Copy( __in OVERLAPPED const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Empty();

   Copy( &source );
}

void COverlapped::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Internal     = 0;
   InternalHigh = 0;
   Offset       = 0;
   OffsetHigh   = 0;

   if ( m_AutomaticallyDelete == true && hEvent != static_cast< HANDLE >( NULL ) )
   {
      (void) Win32FoundationClasses::wfc_close_handle( hEvent );
   }

   hEvent = static_cast< HANDLE >( NULL );
}

void COverlapped::SetAutomaticallyDelete( __in bool const automatically_delete ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_AutomaticallyDelete = automatically_delete;
}

void COverlapped::SetEvent( __in HANDLE new_event_handle ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AutomaticallyDelete == true && hEvent != static_cast< HANDLE >( NULL ) )
   {
      (void) Win32FoundationClasses::wfc_close_handle( hEvent );
      hEvent = static_cast< HANDLE >( NULL );
   }

   hEvent = new_event_handle;

   // We didn't create this event so don't automatically delete it
   SetAutomaticallyDelete( false );
}

// Operators

_Check_return_ COverlapped& COverlapped::operator=( __in COverlapped const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

_Check_return_ COverlapped& COverlapped::operator=( __in OVERLAPPED const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
