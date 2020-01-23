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
** $Workfile: test_CNetwork.CPP $
** $Revision: 2 $
** $Modtime: 3/08/98 3:31p $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void test_CNetwork( LPCTSTR machine_name ) noexcept
{
   WFCTRACEINIT( TEXT( "test_CNetwork()" ) );

   CNetwork machine( machine_name );

   if ( machine.EnumeratePorts() )
   {
      if ( machine_name != NULL )
      {
         _tprintf( TEXT( "Ports on %s\n" ), machine_name );
      }
      else
      {
         _tprintf( TEXT( "Ports on local machine\n" ) );
      }

      CPortInformation port;

      while( machine.GetNext( port ) == true )
      {
         _tprintf( TEXT( "Name:        %s\n" ), port.Name.c_str());
         _tprintf( TEXT( "Monitor:     %s\n" ), port.Monitor.c_str());
         _tprintf( TEXT( "Description: %s\n" ), port.Description.c_str());
         _tprintf( TEXT( "Type:        %04lX\n" ), port.Type );
         _tprintf( TEXT( "TypeName:    %s\n\n" ), port.TypeName.c_str());
      }
   }
   else
   {
      DWORD error_code = machine.GetErrorCode();
      ReportError( error_code );
      _tprintf( TEXT( "Can't EnumeratePorts, reason code %lu\n" ), error_code );
   }

   if ( machine.IsRebootable() )
   {
      if ( machine_name != NULL )
      {
         _tprintf( TEXT( "Yes, I can reboot %s\n" ), machine_name );
      }
      else
      {
         _tprintf( TEXT( "Yes, I can reboot this machine\n" ) );
      }
   }
}
