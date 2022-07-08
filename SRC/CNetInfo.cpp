/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2020, Samuel R. Blackburn
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
** $Workfile: cnetinfo.cpp $
** $Revision: 20 $
** $Modtime: 6/26/01 10:49a $
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

/*
** CNetInformation stuff
*/

Win32FoundationClasses::CNetworkInformation::CNetworkInformation() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CNetworkInformation::CNetworkInformation( _In_ NETINFOSTRUCT const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkInformation::CNetworkInformation( _In_ Win32FoundationClasses::CNetworkInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

void Win32FoundationClasses::CNetworkInformation::Copy(_In_ NETINFOSTRUCT const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      cbStructure       = source->cbStructure;
      dwProviderVersion = source->dwProviderVersion;
      dwStatus          = source->dwStatus;
      dwCharacteristics = source->dwCharacteristics;
      dwHandle          = source->dwHandle;
      wNetType          = source->wNetType;
      dwPrinters        = source->dwPrinters;
      dwDrives          = source->dwDrives;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkInformation::Copy(_In_ CNetworkInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   Copy( static_cast<NETINFOSTRUCT const *>(&source) );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CNetworkInformation::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CNetworkInformation at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   cbStructure       = " ) << cbStructure       << TEXT( "\n" );
   dump_context << TEXT( "   dwProviderVersion = " ) << dwProviderVersion << TEXT( "\n" );
   dump_context << TEXT( "   dwStatus          = " ) << dwStatus          << TEXT( "\n" );
   dump_context << TEXT( "   dwCharacteristics = " ) << dwCharacteristics << TEXT( "\n" );
   dump_context << TEXT( "   dwHandle          = " ) << dwHandle          << TEXT( "\n" );
   dump_context << TEXT( "   wNetType          = " ) << wNetType          << TEXT( "\n" );
   dump_context << TEXT( "   dwPrinters        = " ) << dwPrinters        << TEXT( "\n" );
   dump_context << TEXT( "   dwDrives          = " ) << dwDrives          << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CNetworkInformation::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   cbStructure       = sizeof( NETINFOSTRUCT );
   dwProviderVersion = 0;
   dwStatus          = 0;
   dwCharacteristics = 0;
   dwHandle          = 0;
   wNetType          = 0;
   dwPrinters        = 0;
   dwDrives          = 0;
}

Win32FoundationClasses::CNetworkInformation const& Win32FoundationClasses::CNetworkInformation::operator = ( _In_ Win32FoundationClasses::CNetworkInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ASSERT( this not_eq &source );

   if ( this not_eq &source )
   {
      Copy( source );
   }

   return( *this );
}

// End of source
