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
*/

#include "receiver.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void main( void )
{
   WFCTRACEINIT( TEXT( "main()" ) );

   CSerialFile serial;

   CString port( TEXT( "COM1:38400,n,8,1" ) );

   if ( serial.Open( port ) == FALSE )
   {
      WFCTRACEERROR( GetLastError() );
      WFCTRACEVAL( TEXT( "Can't open " ), port );
      _tprintf( TEXT( "Can't open %s\n" ), (LPCTSTR) port );
      return;
   }

   // Format on the wire is in DWORDS (Intel format because my two
   // test machines, 1 Pentium, 1 486 laptop are both Intel).
   // The first DWORD is the number of bytes being sent, then
   // those bytes, then a DWORD that is a checksum of the block.

   DWORD number_of_bytes_to_read = 0;
   DWORD computed_checksum       = 0;
   DWORD reported_checksum       = 0;
   DWORD number_of_test_blocks   = 0;
   DWORD number_of_errors        = 0;

   if ( serial.Read( &number_of_bytes_to_read, sizeof( number_of_bytes_to_read ) ) == FALSE )
   {
      WFCTRACEERROR( GetLastError() );
      _tprintf( TEXT( "Can't read first block.\n" ) );
      return;
   }

   BYTE * buffer = NULL;

   while( number_of_bytes_to_read != 0xFFFFFFFF )
   {
      buffer = new BYTE[ number_of_bytes_to_read ];

      if ( buffer != NULL )
      {
         if ( serial.Read( buffer, number_of_bytes_to_read ) == FALSE )
         {
            WFCTRACEERROR( GetLastError() );
            WFCTRACEVAL( TEXT( "Can't read this many bytes " ), number_of_bytes_to_read );
            _tprintf( TEXT( "Failed to read %lu bytes.\n" ), number_of_bytes_to_read );
            number_of_errors++;
         }
         else
         {
            if ( ( number_of_bytes_to_read % sizeof( DWORD ) ) != 0 )
            {
               WFCTRACE( TEXT( "Bad sender" ) );
               _tprintf( TEXT( "Sender is not sending DWORDs. Aborting test.\n" ) );
               delete [] buffer;
               buffer = NULL;
               return;
            }

            computed_checksum = compute_checksum( (const DWORD *) buffer, number_of_bytes_to_read / sizeof( DWORD ) );

            if ( serial.Read( &reported_checksum, sizeof( reported_checksum ) ) == FALSE )
            {
               WFCTRACEERROR( GetLastError() );
               WFCTRACE( TEXT( "Can't read reported checksum." ) );
               _tprintf( TEXT( "Can't read reported checksum.\n" ) );
               number_of_errors++;
            }
            else
            {
               if ( computed_checksum != reported_checksum )
               {
                  WFCTRACE( TEXT( "Failed checksum." ) );
                  _tprintf( TEXT( "Failed checksum.\n" ) );
                  number_of_errors++;
               }
            }
         }

         delete [] buffer;
         buffer = NULL;
      }

      number_of_test_blocks++;

      if ( serial.Read( &number_of_bytes_to_read, sizeof( number_of_bytes_to_read ) ) == FALSE )
      {
         WFCTRACEERROR( GetLastError() );
         WFCTRACE( TEXT( "Giving up." ) );
         number_of_bytes_to_read = 0xFFFFFFFF;
      }
   }

   if ( buffer != NULL )
   {
      delete [] buffer;
      buffer = NULL;
   }

   WFCTRACEVAL( TEXT( "Number of tested blocks " ), number_of_test_blocks );
   WFCTRACEVAL( TEXT( "Number that failed " ), number_of_errors );

   _tprintf( TEXT( "%lu errors in %lu blocks\n" ), number_of_errors, number_of_test_blocks );

   serial.Close();
}
