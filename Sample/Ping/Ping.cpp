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
** $Workfile: Ping.cpp $
** $Revision: 4 $
** $Modtime: 6/06/98 6:25a $
*/

#define WFC_STL
#define WINVER 0x0401
#define _WIN32_WINNT 0x0401
#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   WFCTRACELEVELON( 31 );
   WFCTRACEINIT( TEXT( "_tmain()" ) );

   CPing ping;

   ping.SetText( (const BYTE *) "is alive", 8 );

   DWORD return_value = 0;

   CPingResults results;

   return_value = ping.Ping( TEXT( "192.168.0.3" ), &results );

   if ( return_value == 0 )
   {
      DWORD error_code = ping.GetErrorCode();

      CString error_message;

      ping.ConvertErrorToString( error_code, error_message );

      WFCTRACEVAL( TEXT( "Ping error " ), error_message );
      _tprintf( TEXT( "Ping Error: %s\n" ), (LPCTSTR) error_message );
   }
   else
   {
      WFCTRACEVAL( TEXT( "Ping OK to " ), results.Address );
      WFCTRACEVAL( TEXT( "Time in ms " ), results.RoundTripTimeInMilliseconds );

      _tprintf( TEXT( "Ping: %s Round Trip %lums TTL: %d\n" ),
                (LPCTSTR) results.Address,
                results.RoundTripTimeInMilliseconds,
                (int) results.TimeToLive );
   }

   return( EXIT_SUCCESS );
}
