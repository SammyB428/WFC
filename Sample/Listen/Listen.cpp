/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 1996-2003, Samuel R. Blackburn
** All rights reserved.
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
** $Workfile: CMixerControl.cpp $
** $Revision: 3 $
** $Modtime: 11/17/96 5:51p $
*/

#define WINVER 0x0401
#define _WIN32_WINNT 0x0401
#define WFC_STL
#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG )
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

USING_WFC_NAMESPACE

class CMyListeningSocket : public CListeningSocket
{
   public:

      virtual void OnNewConnection( SOCKET socket_id, const char *host_name, const char *dotted_ip_address_string );
};

void CMyListeningSocket::OnNewConnection( SOCKET socket_id, const char *host_name, const char *dotted_ip_address_string )
{
   WFCTRACEINIT( TEXT( "CMyListeningSocket::OnNewConnection()" ) );

   CString string_to_send;

   string_to_send.Format( TEXT( "Your name is \"%s\", your IP address is \"%s\"\n" ),
                          host_name,
                          dotted_ip_address_string );

   CSimpleSocketFile socket_file( socket_id, host_name, dotted_ip_address_string );

   socket_file.Write( string_to_send );

   socket_file.Close();
}

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   WFCTRACEINIT( TEXT( "main()" ) );

   CMyListeningSocket incoming_connection;

   // Wait for a connection on port number 1111 (23 == telnet)

   while( incoming_connection.WaitForConnection( 1111 ) != FALSE )
   {
      WFCTRACE( TEXT( "Connection was made" ) );
   }

   return( EXIT_SUCCESS );
}
