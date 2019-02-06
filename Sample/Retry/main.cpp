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
**
** $Workfile: wfc.h $
** $Revision: 67 $
** $Modtime: 5/26/00 10:01a $
*/

/*
** I wrote thie program while trying to install the April 2000
** edition of the Platform SDK from Microsoft's website using a 56Kb
** modem. The installer kept timing out and I'd have to hit the
** Retry button every 20 minutes or so. So, I wrote this toy program
** to hit the button for me.
*/

#define WINVER 0x401
#define WFC_STL
#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

#if defined( _DEBUG )
#define SLEEP_INTERVAL (5*1000)
#else
#define SLEEP_INTERVAL (67*1000)
#endif

void main( void )
{
   WFCTRACEINIT( TEXT( "main()" ) );

   CByteArray web_page_contents;

   CUniformResourceLocator url( TEXT( "http://www.google.com" ) );

   while( true )
   {
      wfc_get_web_page( url, web_page_contents );

      _tprintf( TEXT( "Read %lu bytes.\n" ), (unsigned long) web_page_contents.GetSize() );
      web_page_contents.RemoveAll();
      Sleep( SLEEP_INTERVAL );
   }

#if 0

   HWND retry_window_handle = NULL;

   DWORD number_of_retries = 0;

   while( true )
   {
      retry_window_handle = FindWindow( TEXT( "#32770" ), TEXT( "Microsoft Platform SDK" ) );

      if ( retry_window_handle != NULL )
      {
         // The window is there, let's tell it to retry

         if ( PostMessage( retry_window_handle, WM_COMMAND, 0xBB9, 0 ) == FALSE )
         {
            _tprintf( TEXT( "Failed to PostMessage(), Error Code %lu\n" ), GetLastError() );
         }
         else
         {
            number_of_retries++;
            _tprintf( TEXT( "Number of retries is %lu.\n" ), number_of_retries );
            WFCTRACEVAL( TEXT( "Number of retries is " ), number_of_retries );
         }
      }

      Sleep( 30 * 1000 );
   }
#endif
}