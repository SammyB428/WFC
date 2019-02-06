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

#define WINVER 0x0401
#define _WIN32_WINNT 0x0401
#define WFC_STL
#include <wfc.h>
USING_WFC_NAMESPACE

#include "../CSimpleClass.hpp"
#pragma hdrstop

class CChunkReceivingSocket : public CListeningSocket
{
   public:

      virtual void OnNewConnection( SOCKET socket_id, const char *host_name, const char *dotted_ip_address_string ); // Called by WaitForConnection
};

void CChunkReceivingSocket::OnNewConnection( SOCKET socket_id, const char *host_name, const char *dotted_ip_address_string )
{
   CSimpleSocketFile socket( socket_id, host_name, dotted_ip_address_string );

   CDataChunk chunk;

   CDataSocket data_socket( &socket );

   // Read a chunk

   data_socket.GetData( chunk );

   // Now find out what to do with it

   switch( chunk.Identifier )
   {
      case MAKE_DATA_ID( 'S', 'I', 'M', 'P' ):
      {
         // We've got a CSimpleClass chunk

         CSimpleClass simple_class;

         CDataArchive archive;

         archive.ReadFrom( &chunk );

         simple_class.Serialize( archive );

         _tprintf( TEXT( "Received " ) );

         simple_class.Dump();
      }

      break;
   }

   socket.Close();
}

void _tmain( void )
{
   WFCTRACEINIT( TEXT( "_tmain()" ) );

   CChunkReceivingSocket socket;

   if ( socket.WaitForConnection( 6060 ) != FALSE )
   {
      _tprintf( TEXT( "Got a connection\n" ) );
   }
   else
   {
      TCHAR error_message[ 513 ];

      ZeroMemory( error_message, sizeof( error_message ) );

      CSimpleSocket::TranslateErrorCode( socket.GetErrorCode(), error_message, DIMENSION_OF( error_message ) );

      _tprintf( TEXT( "Couldn't open because %s\n" ), error_message );
   }
}
