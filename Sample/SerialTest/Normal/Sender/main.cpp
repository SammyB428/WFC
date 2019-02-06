/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2003, Samuel R. Blackburn
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
*/

#include "sender.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void main( void )
{
   WFCTRACEINIT( TEXT( "main()" ) );

   DWORD number_of_rounds = 10;
   DWORD test_buffer_size = ( 16 * 1024 ) + 1;
   DWORD checksum         = 0;
   DWORD loop_index       = 0;
   DWORD number_of_bytes_to_send = test_buffer_size * sizeof( DWORD );

   DWORD * test_buffer = NULL;

   CSerialFile serial;

   CString port( TEXT( "COM1:38400,n,8,1" ) );

   if ( serial.Open( port ) == FALSE )
   {
      WFCTRACEVAL( TEXT( "Can't open " ), port );
      _tprintf( TEXT( "Can't open %s\n" ), (LPCTSTR) port );
      return;
   }

   CRandomNumberGenerator2 random;

   while( loop_index < number_of_rounds )
   {
      prepare_buffer( random, test_buffer, test_buffer_size, checksum );
      serial.Write( &number_of_bytes_to_send, sizeof( number_of_bytes_to_send ) );
      serial.Write( test_buffer, number_of_bytes_to_send );
      serial.Write( &checksum, sizeof( checksum ) );

      loop_index++;
   }

   number_of_bytes_to_send = 0xFFFFFFFF;

   serial.Write( &number_of_bytes_to_send, sizeof( number_of_bytes_to_send ) );
   serial.Close();
}
