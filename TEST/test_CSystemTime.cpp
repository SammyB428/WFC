/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2017, Samuel R. Blackburn
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
** $Workfile: test_CSystemTime.cpp $
** $Revision: 2 $
** $Modtime: 1/31/00 7:47p $
*/

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

_Check_return_ bool test_CSystemTime( __out std::string& class_name, __out int& test_number_that_failed ) noexcept
{
   WFCTRACEINIT( TEXT( "test_CSystemTime()" ) );

   class_name.assign(STRING_VIEW("CSystemTime"));

   TIMESTAMP_STRUCT timestamp_struct;

   timestamp_struct.year     = 1752;
   timestamp_struct.month    = 9;
   timestamp_struct.day      = 2;
   timestamp_struct.hour     = 7;
   timestamp_struct.minute   = 32;
   timestamp_struct.second   = 22;
   timestamp_struct.fraction = 14;

   CSystemTime system_time( &timestamp_struct );

#if 0 
   if ( system_time.wDayOfWeek != 6 )
   {
      test_number_that_failed = 1;
      return( failure() );
   }
#endif

   if ( system_time.wYear != 1752 )
   {
      test_number_that_failed = 1;
      return( failure() );
   }

   if ( system_time.wMonth != 9 )
   {
      test_number_that_failed = 2;
      return( failure() );
   }

   if ( system_time.wDay != 2 )
   {
      test_number_that_failed = 3;
      return( failure() );
   }

   if ( system_time.wHour != 7 )
   {
      test_number_that_failed = 4;
      return( failure() );
   }

   if ( system_time.wMinute != 32 )
   {
      test_number_that_failed = 5;
      return( failure() );
   }

   if ( system_time.wSecond != 22 )
   {
      test_number_that_failed = 6;
      return( failure() );
   }

   if ( system_time.wMilliseconds != 140 )
   {
      test_number_that_failed = 7;
      return( failure() );
   }

   CSystemTime time_2( system_time );

   if ( ! ( time_2 == system_time ) )
   {
      test_number_that_failed = 8;
      return( failure() );
   }

   time_2.wMilliseconds++;

   if ( time_2 == system_time )
   {
      test_number_that_failed = 9;
      return( failure() );
   }

   time_2.Empty();

   time_2.wYear  = 2014;
   time_2.wMonth = 3;
   time_2.wDay   = 24;

   uint32_t minutes_since_monday = time_2.NumberOfMinutesSinceMonday();

   if ( minutes_since_monday != 0 )
   {
      test_number_that_failed = 10;
      return( failure() );
   }

   time_2.wDay = 25;

   minutes_since_monday = time_2.NumberOfMinutesSinceMonday();

   if ( minutes_since_monday != NUMBER_OF_MINUTES_IN_ONE_DAY )
   {
      test_number_that_failed = 11;
      return( failure() );
   }

   time_2.wDay = 30;

   minutes_since_monday = time_2.NumberOfMinutesSinceMonday();

   if ( minutes_since_monday != (NUMBER_OF_MINUTES_IN_ONE_DAY * 6) )
   {
      test_number_that_failed = 12;
      return( failure() );
   }

   uint64_t const ticks = 129748979200327745I64;

   CFileTime ft;
 
   ft.CopyTicks( ticks );

   std::wstring postgresql_timestamp = ft.PostgreSQLTimestamp();

   if ( postgresql_timestamp.compare(WSTRING_VIEW(L"2012-02-28 10:18:40.032774")) != I_AM_EQUAL_TO_THAT )
   {
       test_number_that_failed = 13;
       return( failure() );
   }

   struct tm c_time; // "02 Feb 2015 09:50"

   ZeroMemory( &c_time, sizeof( c_time ) );

   c_time.tm_year = 2015 - 1900;
   c_time.tm_mon = 2 - 1; // 0 == Jan, 1 == Feb
   c_time.tm_mday = 2;
   c_time.tm_hour = 9;
   c_time.tm_min = 50;

   static constexpr time_t const correct_answer = 1422870600; // from http://www.onlineconversion.com/unix_time.htm 

   time_t const time_value = _mkgmtime( &c_time );

   _ASSERTE( time_value == correct_answer );

   ft.Set( c_time.tm_year + 1900, c_time.tm_mon + 1, c_time.tm_mday, c_time.tm_hour, c_time.tm_min, c_time.tm_sec );

   if (ft.IsEmpty() == true)
   {
       test_number_that_failed = 14;
       return(failure());
   }

   time_t const second_time = ft.AsUnixTime();

   _ASSERT_EXPR( second_time == correct_answer, _CRT_WIDE("CFileTime.Set() failed to convert to 1422870600 correctly.") );

   if ( time_value != second_time )
   {
       test_number_that_failed = 15;
       return( failure() );
   }

   test_number_that_failed = 15;
   return( true );
}
