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
** $Workfile: CMixerControlDetails.cpp $
** $Revision: 15 $
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

CMixerControlDetails::CMixerControlDetails()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

CMixerControlDetails::CMixerControlDetails( __in CMixerControlDetails const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CMixerControlDetails::CMixerControlDetails( __in MIXERCONTROLDETAILS const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CMixerControlDetails::~CMixerControlDetails()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

// Methods

void CMixerControlDetails::Copy( __in CMixerControlDetails const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   ID                              = source.ID;
   NumberOfChannels                = source.NumberOfChannels;
   WindowHandleOfOwner             = source.WindowHandleOfOwner;
   NumberOfMultipleItemsPerChannel = source.NumberOfMultipleItemsPerChannel;
}

void CMixerControlDetails::Copy( __in MIXERCONTROLDETAILS const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ID                              = source.dwControlID;
   NumberOfChannels                = source.cChannels;
   WindowHandleOfOwner             = source.hwndOwner;
   NumberOfMultipleItemsPerChannel = source.cMultipleItems;
}

// Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CMixerControlDetails::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   dump_context << TEXT( "a CMixerControlDetails at "             ) << (VOID *) this                   << TEXT( "\n{\n" );
   dump_context << TEXT( "   ID is                              " ) << ID                              << TEXT( "\n"    );
   dump_context << TEXT( "   NumberOfChannels is                " ) << NumberOfChannels                << TEXT( "\n"    );
   dump_context << TEXT( "   WindowHandleOfOwner is             " ) << WindowHandleOfOwner             << TEXT( "\n"    );
   dump_context << TEXT( "   NumberOfMultipleItemsPerChannel is " ) << NumberOfMultipleItemsPerChannel << TEXT( "\n"    );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void CMixerControlDetails::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ID                              = 0;
   NumberOfChannels                = 0;
   WindowHandleOfOwner             = static_cast< HWND >( NULL );
   NumberOfMultipleItemsPerChannel = 0;
}

// Operators

CMixerControlDetails& CMixerControlDetails::operator=( __in CMixerControlDetails const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

CMixerControlDetails& CMixerControlDetails::operator=( __in MIXERCONTROLDETAILS const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
