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
** $Workfile: TEVNTLOG.CPP $
** $Revision: 6 $
** $Modtime: 4/01/00 1:01p $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void test_CEventLog( void ) noexcept
{
   WFCTRACEINIT( TEXT( "test_CEventLog()" ) );

#if 0
   CEventLog log( TEXT( "Dodah" ) );

   LPCTSTR string_array[ 1 ];

   string_array[ 0 ] = TEXT( "Hello World" );

   log.Report( CEventLog::eventInformation, 0, 0, 1, string_array );
   log.ReportInformation( TEXT( "ReportInformation" ) );
   log.ReportError( TEXT( "ReportError" ) );
#endif

   CEventLog log;

   DWORD enumerator = 0;

   if ( log.EnumerateLogs( enumerator ) == false )
   {
      WFCTRACE( TEXT( "Can't enumerate logs.\n" ) );
   }

   std::wstring log_name;

   while( log.GetNextLog( enumerator, log_name ) == true )
   {
      WFCTRACEVAL( TEXT( "Log: " ), log_name.c_str() );
   }

   if ( log.CreateCustomLog(WSTRING_VIEW(L"WFC")) == false )
   {
      WFCTRACE( TEXT( "Can't create WFC event log because" ) );
      WFCTRACEERROR( log.GetErrorCode() );
   }

   if ( log.Open( TEXT( "WFC" ) ) == false )
   {
      WFCTRACE( TEXT( "Can't Open WFC event log because" ) );
      WFCTRACEERROR( log.GetErrorCode() );
   }

   if ( log.RegisterSource(WSTRING_VIEW(L"WFC")) == false )
   {
      WFCTRACE( TEXT( "Can't Open WFC event log because" ) );
      WFCTRACEERROR( log.GetErrorCode() );
   }

   log.ReportInformation( TEXT( "ReportInformation" ) );
   log.ReportError( TEXT( "ReportError" ) );

   CEventLogRecord record;

   DWORD loop_index = 0;

   while( log.Read( loop_index, record ) == true )
   {
#if defined( _DEBUG ) && ! defined( WFC_STL )
      record.Dump( afxDump );
#endif // _DEBUG

      for( auto const& entry : record.Strings )
      {
         WFCTRACEVAL( TEXT( "String is " ), entry.c_str() );
      }

      loop_index++;
   }

   ASSERT( loop_index != 0 );

#if 1
   if ( log.Delete(WSTRING_VIEW(L"WFC")) == false )
   {
      WFCTRACE( TEXT( "Can't delete WFC event log because" ) );
      WFCTRACEERROR( log.GetErrorCode() );
   }
#endif
}
