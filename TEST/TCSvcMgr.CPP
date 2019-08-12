/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2016, Samuel R. Blackburn
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
** $Workfile: TCSvcMgr.CPP $
** $Revision: 3 $
** $Modtime: 1/31/00 7:43p $
*/

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

__checkReturn bool test_CServiceControlManager( __out std::string& class_name, __out int& test_number_that_failed ) noexcept
{
   class_name.assign( "CServiceControlManager" );
   test_number_that_failed = 1;

   CServiceControlManager manager;

   std::wstring machine_name;

   if ( manager.Open( GENERIC_READ, NULL, machine_name.c_str() ) == false )
   {
      _tprintf( TEXT( "Can't Open ServiceControlManager\n" ) );
      return( failure() );
   }

   std::size_t number_of_services = 0;

   if ( manager.EnumerateStatus( SERVICE_STATE_ALL, SERVICE_TYPE_ALL ) == true )
   {
     // _tprintf( TEXT( "Services on %s:\n" ), (LPCTSTR) machine_name );

      CServiceNameAndStatus status;

      while( manager.GetNext( status ) == true )
      {
#if defined( _DEBUG ) && ! defined( WFC_STL )
  //       status.Dump( afxDump );
#endif // _DEBUG
    //     _tprintf( TEXT( "%s - \"%s\"\n" ), status.lpServiceName, status.lpDisplayName );

         number_of_services++;
      }
   }
   else
   {
      test_number_that_failed = 2;
      return( failure() );
   }

   if ( number_of_services == 0 )
   {
      // Oh come on, do you really expect me to believe that NT has no services?!?!?
      test_number_that_failed = 3;
      return( failure() );
   }

   test_number_that_failed = 3;

   return( true );
}
