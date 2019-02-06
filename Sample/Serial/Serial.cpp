/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1996-2003, Samuel R. Blackburn
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
** $Workfile: Serial.cpp $
** $Revision: 4 $
** $Modtime: 3/08/98 3:19p $
*/

#include "Serial.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

int _tmain( int argc, LPCTSTR argv[] )
{
   WFCTRACEINIT( TEXT( "main()" ) );

   CSerialFile serial_port;

   if ( serial_port.Open( TEXT( "COM2:4800,n,8,1" ) ) == FALSE )
   {
      WFCTRACE( TEXT( "Can't open serial port" ) );
      return( EXIT_SUCCESS );
   }

   CCommunicationProperties properties;

   if ( serial_port.GetProperties( properties ) == FALSE )
   {
      WFCTRACEVAL( TEXT( "Can't get properties. Reason " ), serial_port.GetErrorCode() );
   }

   CString nmea_string( TEXT( "$GPGLL,4916.45,N,12311.12,W*71\r\n" ) );

   while( 1 )
   {
      serial_port.Write( nmea_string );
      _tprintf( nmea_string );
      Sleep( 2000 );
   }

   return( EXIT_SUCCESS );
}
