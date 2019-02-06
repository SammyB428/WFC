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
** $Workfile: TURL.CPP $
** $Revision: 3 $
** $Modtime: 3/08/98 3:32p $
*/

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

__checkReturn bool test_CUniformResourceLocator( __out std::string& class_name, __out int& test_number_that_failed ) noexcept
{
   WFCTRACEINIT( TEXT( "test_CUniformResourceLocator()" ) );

   class_name.assign( "CUniformResourceLocator" );

   test_number_that_failed = 1;

   CUniformResourceLocator url( TEXT( "Serial://COM2:9600,n,8,1/Settings.ini" ) );

   if ( url.ProtocolName == TEXT( "Serial" )     &&  
        url.MachineName  == TEXT( "COM2" )       &&
        url.PortName     == TEXT( "9600,n,8,1" ) &&
        url.PathName     == TEXT( "Settings.ini" ) )
   {
      return( true );
   }

   _tprintf( TEXT( "CUniformResourceLocator FAILED!\n" ) );
   _tprintf( TEXT( "ProtocolName == \"%s\" when it should be \"Serial\"\n" ),     url.ProtocolName.c_str());
   _tprintf( TEXT( "MachineName  == \"%s\" when it should be \"COM2\"\n" ),       url.MachineName.c_str());
   _tprintf( TEXT( "PortName     == \"%s\" when it should be \"9600,n,8,1\n" ),   url.PortName.c_str());
   _tprintf( TEXT( "Pathname     == \"%s\" when it should be \"Settings.ini\n" ), url.PathName.c_str());

   return( failure() );
}
