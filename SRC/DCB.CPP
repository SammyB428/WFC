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
** $Workfile: dcb.cpp $
** $Revision: 18 $
** $Modtime: 6/26/01 10:53a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CDeviceControlBlock::CDeviceControlBlock() noexcept
{
   WFC_VALIDATE_POINTER( this );
   ::ZeroMemory( (DCB *) this, sizeof( DCB ) );
   DCBlength = sizeof( DCB );
   fBinary = TRUE; // Always TRUE for NT
}

Win32FoundationClasses::CDeviceControlBlock::CDeviceControlBlock( _In_ DCB const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   DCBlength = sizeof( DCB );

   Copy( source );
}

Win32FoundationClasses::CDeviceControlBlock::CDeviceControlBlock( _In_ DCB const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   DCBlength = sizeof( DCB );

   Copy( source );
}

Win32FoundationClasses::CDeviceControlBlock::CDeviceControlBlock( _In_ Win32FoundationClasses::CDeviceControlBlock const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   DCBlength = sizeof( DCB );

   Copy( source );
}

void Win32FoundationClasses::CDeviceControlBlock::Copy( _In_ Win32FoundationClasses::CDeviceControlBlock const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( (DCB *) &source );
}

void Win32FoundationClasses::CDeviceControlBlock::Copy( _In_ DCB const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( &source );
}

void Win32FoundationClasses::CDeviceControlBlock::Copy( _In_ DCB const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == this )
   {
      return;
   }

   if ( source == nullptr )
   {
      ::ZeroMemory( (DCB *) this, sizeof( DCB ) );
      DCBlength = sizeof( DCB );
      fBinary = TRUE; // Always TRUE for NT
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      BaudRate          = source->BaudRate;
      fBinary           = source->fBinary;
      fParity           = source->fParity;
      fOutxCtsFlow      = source->fOutxCtsFlow;
      fOutxDsrFlow      = source->fOutxDsrFlow;
      fDtrControl       = source->fDtrControl;
      fDsrSensitivity   = source->fDsrSensitivity;
      fTXContinueOnXoff = source->fTXContinueOnXoff;
      fOutX             = source->fOutX;
      fInX              = source->fInX;
      fErrorChar        = source->fErrorChar;
      fNull             = source->fNull;
      fRtsControl       = source->fRtsControl;
      fDummy2           = source->fDummy2;
      wReserved         = source->wReserved;
      XonLim            = source->XonLim;
      XoffLim           = source->XoffLim;
      ByteSize          = source->ByteSize;
      Parity            = source->Parity;
      StopBits          = source->StopBits;
      XonChar           = source->XonChar;
      XoffChar          = source->XoffChar;
      ErrorChar         = source->ErrorChar;
      EofChar           = source->EofChar;
      EvtChar           = source->EvtChar;
   }
   WFC_CATCH_ALL
   {
      ::ZeroMemory( (DCB *) this, sizeof( DCB ) );
      DCBlength = sizeof( DCB );
      fBinary = TRUE; // Always TRUE for NT
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CDeviceControlBlock::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CDeviceControlBlock at " ) << (void *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   DCBlength =         " ) << DCBlength         << TEXT( "\n" );
   dump_context << TEXT( "   BaudRate =          " ) << BaudRate          << TEXT( " (" );

   switch( BaudRate )
   {
      case CBR_110:

         dump_context << TEXT( "CBR_110" );
         break;

      case CBR_300:

         dump_context << TEXT( "CBR_300" );
         break;

      case CBR_600:

         dump_context << TEXT( "CBR_600" );
         break;

      case CBR_1200:

         dump_context << TEXT( "CBR_1200" );
         break;

      case CBR_2400:

         dump_context << TEXT( "CBR_2400" );
         break;

      case CBR_4800:

         dump_context << TEXT( "CBR_4800" );
         break;

      case CBR_9600:

         dump_context << TEXT( "CBR_9600" );
         break;

      case CBR_14400:

         dump_context << TEXT( "CBR_14400" );
         break;

      case CBR_19200:

         dump_context << TEXT( "CBR_19200" );
         break;

      case CBR_38400:

         dump_context << TEXT( "CBR_38400" );
         break;

      case CBR_56000:

         dump_context << TEXT( "CBR_56000" );
         break;

      case CBR_57600:

         dump_context << TEXT( "CBR_57600" );
         break;

      case CBR_115200:

         dump_context << TEXT( "CBR_115200" );
         break;

      case CBR_128000:

         dump_context << TEXT( "CBR_128000" );
         break;

      case CBR_256000:

         dump_context << TEXT( "CBR_256000" );
         break;
   }

   dump_context << TEXT( ")\n" );

   dump_context << TEXT( "   fBinary =           " ) << fBinary           << TEXT( "\n" );
   dump_context << TEXT( "   fParity =           " ) << fParity           << TEXT( "\n" );
   dump_context << TEXT( "   fOutxCtsFlow =      " ) << fOutxCtsFlow      << TEXT( "\n" );
   dump_context << TEXT( "   fOutxDsrFlow =      " ) << fOutxDsrFlow      << TEXT( "\n" );
   dump_context << TEXT( "   fDtrControl =       " ) << fDtrControl       << TEXT( " (" );

   switch( fDtrControl )
   {
      case DTR_CONTROL_DISABLE:

         dump_context << TEXT( "DTR_CONTROL_DISABLE" );
         break;

      case DTR_CONTROL_ENABLE:

         dump_context << TEXT( "DTR_CONTROL_ENABLE" );
         break;

      case DTR_CONTROL_HANDSHAKE:

         dump_context << TEXT( "DTR_CONTROL_HANDSHAKE" );
         break;
   }

   dump_context << TEXT( ")\n" );

   dump_context << TEXT( "   fDsrSensitivity =   " ) << fDsrSensitivity   << TEXT( "\n" );
   dump_context << TEXT( "   fTXContinueOnXoff = " ) << fTXContinueOnXoff << TEXT( "\n" );
   dump_context << TEXT( "   fOutX =             " ) << fOutX             << TEXT( "\n" );
   dump_context << TEXT( "   fInX =              " ) << fInX              << TEXT( "\n" );
   dump_context << TEXT( "   fErrorChar =        " ) << fErrorChar        << TEXT( "\n" );
   dump_context << TEXT( "   fNull =             " ) << fNull             << TEXT( "\n" );
   dump_context << TEXT( "   fRtsControl =       " ) << fRtsControl       << TEXT( " (" );

   switch( fRtsControl )
   {
      case RTS_CONTROL_DISABLE:

         dump_context << TEXT( "RTS_CONTROL_DISABLE" );
         break;

      case RTS_CONTROL_ENABLE:

         dump_context << TEXT( "RTS_CONTROL_ENABLE" );
         break;

      case RTS_CONTROL_HANDSHAKE:

         dump_context << TEXT( "RTS_CONTROL_HANDSHAKE" );
         break;

      case RTS_CONTROL_TOGGLE:

         dump_context << TEXT( "RTS_CONTROL_TOGGLE" );
         break;
   }

   dump_context << TEXT( ")\n" );

   dump_context << TEXT( "   fDummy2 =           " ) << fDummy2           << TEXT( "\n" );
   dump_context << TEXT( "   wReserved =         " ) << wReserved         << TEXT( "\n" );
   dump_context << TEXT( "   XonLim =            " ) << XonLim            << TEXT( "\n" );
   dump_context << TEXT( "   XoffLim =           " ) << XoffLim           << TEXT( "\n" );
   dump_context << TEXT( "   ByteSize =          " ) << ByteSize          << TEXT( "\n" );
   dump_context << TEXT( "   Parity =            " ) << Parity            << TEXT( " (" );

   switch( Parity )
   {
      case EVENPARITY:

         dump_context << TEXT( "EVENPARITY" );
         break;

      case MARKPARITY:

         dump_context << TEXT( "MARKPARITY" );
         break;

      case NOPARITY:

         dump_context << TEXT( "NOPARITY" );
         break;

      case ODDPARITY:

         dump_context << TEXT( "ODDPARITY" );
         break;

      case SPACEPARITY:

         dump_context << TEXT( "SPACEPARITY" );
         break;
   }

   dump_context << TEXT( ")\n" );

   dump_context << TEXT( "   StopBits =          " ) << StopBits          << TEXT( " (" );

   switch( StopBits )
   {
      case ONESTOPBIT:

         dump_context << TEXT( "ONESTOPBIT" );
         break;

      case ONE5STOPBITS:

         dump_context << TEXT( "ONE5STOPBITS" );
         break;

      case TWOSTOPBITS:

         dump_context << TEXT( "TWOSTOPBITS" );
         break;
   }

   dump_context << TEXT( ")\n" );

   dump_context << TEXT( "   XonChar =           " ) << XonChar           << TEXT( "\n" );
   dump_context << TEXT( "   XoffChar =          " ) << XoffChar          << TEXT( "\n" );
   dump_context << TEXT( "   ErrorChar =         " ) << ErrorChar         << TEXT( "\n" );
   dump_context << TEXT( "   EofChar =           " ) << EofChar           << TEXT( "\n" );
   dump_context << TEXT( "   EvtChar =           " ) << EvtChar           << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

Win32FoundationClasses::CDeviceControlBlock& Win32FoundationClasses::CDeviceControlBlock::operator=( _In_ Win32FoundationClasses::CDeviceControlBlock const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CDeviceControlBlock& Win32FoundationClasses::CDeviceControlBlock::operator=( _In_ DCB const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
