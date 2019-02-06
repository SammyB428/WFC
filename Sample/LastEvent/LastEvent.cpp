/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 1998-2003, Samuel R. Blackburn
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
** $Workfile: LastEvent.cpp $
** $Revision: 2 $
** $Modtime: 4/02/98 6:24a $
*/

#define WINVER 0x0401
#define _WIN32_WINNT 0x0401
#define WFC_STL
#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

#if defined( UNICODE )
extern "C"
#endif // UNICODE

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   WFCTRACEINIT( TEXT( "main()" ) );

   DWORD number_of_events = 10;

   CEventLog event_log;

   if ( event_log.Open( TEXT( "System" ) ) == FALSE )
   {
      WFCTRACEVAL( TEXT( "event_log.Open() failed with error code " ), event_log.GetErrorCode() );
      _tprintf( TEXT( "Can't open event log because %lu\n" ), event_log.GetErrorCode() );
      return( EXIT_SUCCESS );
   }

   if ( number_of_command_line_arguments > 2 )
   {
      number_of_events = _ttoi( command_line_arguments[ 1 ] );

      if ( number_of_events == 0 )
      {
         number_of_events = 1;
      }
   }

   DWORD number_of_records = 0;

   if ( event_log.GetNumberOfRecords( number_of_records ) == FALSE )
   {
      _tprintf( TEXT( "Can't get number of records because %lu\n" ), event_log.GetErrorCode() );
      return( EXIT_SUCCESS );
   }

   WFCTRACEVAL( TEXT( "Number of records is " ), number_of_records );
   _tprintf( TEXT( "There are %lu number of records.\n" ), number_of_records );

   DWORD last_record_number = 0;

   last_record_number = event_log.GetOldestRecordNumber();

   WFCTRACEVAL( TEXT( "Oldest record number is " ), last_record_number );
   _tprintf( TEXT( "Oldest record number is %lu\n" ), last_record_number );

   CEventLogRecord record;

   DWORD index = 0;

   while( index < number_of_events )
   {
      if ( event_log.Read( index + 1, record, EVENTLOG_FORWARDS_READ | EVENTLOG_SEQUENTIAL_READ ) == FALSE )
      {
         WFCTRACEVAL( TEXT( "Cant read because " ), event_log.GetErrorCode() );
         _tprintf( TEXT( "Can't read record #%lu because %lu\n" ), index, event_log.GetErrorCode() );
      }
      else
      {
         if ( record.Strings.GetSize() > 0 )
         {
            _tprintf( TEXT( "%s, %lu %s\n" ),
                      (LPCTSTR) record.Source,
                      (DWORD) LOWORD( record.EventID ),
                      (LPCTSTR) record.Strings.GetAt( 0 ) );
         }
         else
         {
            _tprintf( TEXT( "%s, %lu\n" ),
                      (LPCTSTR) record.Source,
                      (DWORD) LOWORD( record.EventID ) );
         }
      }

      index++;
   }

   return( EXIT_SUCCESS );
}
