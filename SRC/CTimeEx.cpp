/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2019, Samuel R. Blackburn
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
** $Workfile: CTimeEx.cpp $
** $Revision: 8 $
** $Modtime: 6/26/01 10:51a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // WIN32

USING_WFC_NAMESPACE

/*
** This module uses exception handling to trap
** for errant pointers. This is useless on Unix
** systems because they are too stupid to detect
** such conditions. On Windows NT machines the
** errant pointers are trapped and the catch()
** block of the handler is executed.
*/

#define NUMBER_OF_NANOSECONDS_IN_ONE_SECOND (1000000000)

#if defined( WFC_STL )

_Check_return_ std::wstring CTimeSpan::Format( _In_z_ wchar_t const * format_string ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( format_string );

   std::wstring return_value;

   WFC_TRY
   {
      std::wstring number_string;

      int format_string_index = 0;

      wchar_t character = 0;

      character = format_string[ format_string_index ];

      while( character != 0 )
      {
         if ( character == '%' )
         {
            format_string_index++;
            character = format_string[ format_string_index ];

            switch( character )
            {
               case '%':

                  return_value.push_back( '%' );
                  break;

               case 'D':

                  format( number_string, L"%ld", GetDays() );
                  return_value.append( number_string );
                  break;

               case 'H':

                  format( number_string, L"%02ld", GetHours() );
                  return_value.append( number_string );
                  break;

               case 'M':

                  format( number_string, L"%02ld", GetMinutes() );
                  return_value.append( number_string );
                  break;

               case 'S':

                  format( number_string, L"%02ld", GetSeconds() );
                  return_value.append( number_string );
                  break;
            }
         }
         else
         {
            return_value.push_back( format_string[ format_string_index ] );
         }

         format_string_index++;
         character = format_string[ format_string_index ];
      }
   }
   WFC_CATCH_ALL
   {
   }
   WFC_END_CATCH_ALL

   return( return_value );
}

#endif // WFC_STL

CTimeEx::CTimeEx( __in struct tm const * time_p )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( time_p );
   Copy( time_p );
}

CTimeEx::CTimeEx( __in struct tm const& time_structure )
{
   WFC_VALIDATE_POINTER( this );
   Copy( time_structure );
}

CTimeEx::CTimeEx( __in int const year, __in int const month, __in int const day, __in int const hour, __in int const minute, __in int const second, __in int const daylight_savings_time )
{
   WFC_VALIDATE_POINTER( this );
   Set( year, month, day, hour, minute, second, daylight_savings_time );
}

void CTimeEx::Copy( __in struct tm const * time_p ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( time_p );

   m_Time = m_Make_time_t( time_p );

   if ( m_Time == (-1) )
   {
      m_Time = 0;
   }
}

void CTimeEx::Copy( __in struct tm const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( &source );
}

void CTimeEx::CopyModifiedJulianDate( __in double const number_of_days_since_17_november_1858 ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   double whole_days = 0.0;

   double fractional_days = std::modf( number_of_days_since_17_november_1858, &whole_days );

   whole_days -= 40587.0;

   // There are 40,587 days between 17 Nov 1858 and 01 Jan 1970
   // There are 86,400 seconds in one day

   double number_of_seconds_into_the_day = fractional_days * 86400.0;

   m_Time = (long) (whole_days * 86400.0);

   double whole_seconds = 0.0;
   double fractional_seconds = std::modf( number_of_seconds_into_the_day, &whole_seconds );

   m_Time += static_cast<long>(whole_seconds);
}

void CTimeEx::CopyTo(_Out_ std::wstring& iso_8601_format_string ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   iso_8601_format_string.assign( Format( L"%Y-%m-%dT%H:%M:%S" ) );
   iso_8601_format_string.push_back( 'Z' );
}

void CTimeEx::FileNameFormat( _Out_ std::wstring& iso_8601_format_string ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   iso_8601_format_string.assign(Format( L"%Y%m%d_%H%M%S" ));
}

void CTimeEx::CopyTo(_Out_ struct tm& destination ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   GreenwichMeanTime( &m_Time, &destination );
}

_Check_return_ std::wstring CTimeEx::Format( _In_z_ wchar_t const * format_string ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( format_string );

   wchar_t buffer[ 4096 ];

   if ( format_string == nullptr )
   {
      buffer[ 0 ] = 0x00;
      buffer[ 0 ] = 0x00;

      return( buffer );
   }

   struct tm time_structure;

   // We were passed a pointer, don't trust it

   ZeroMemory( buffer, sizeof( buffer ) );
   ZeroMemory( &time_structure, sizeof( time_structure ) );

   GreenwichMeanTime( &m_Time, &time_structure );

   if ( time_structure.tm_year < 0 )
   {
       time_structure.tm_year = 10;
       time_structure.tm_mon  = 1;
       time_structure.tm_mday = 1;
       time_structure.tm_hour = 0;
       time_structure.tm_min  = 0;
       time_structure.tm_sec  = 0;
   }

   wcsftime( buffer, std::size( buffer ), format_string, &time_structure );

   return( buffer );
}

void CTimeEx::GetCurrentTheTime( __out CTimeEx& source ) noexcept
{
   struct tm time_structure;

   ZeroMemory( &time_structure, sizeof( time_structure ) );

   time_t current_time = static_cast< time_t >( NULL );

   current_time = ::time( nullptr );

   GreenwichMeanTime( &current_time, &time_structure );

   source.Copy( &time_structure );
}

void CTimeEx::Now( void ) noexcept
{
   struct tm time_structure;

   ZeroMemory( &time_structure, sizeof( time_structure ) );

   time_t current_time = static_cast< time_t >( NULL );

   current_time = ::time( nullptr );

   GreenwichMeanTime( &current_time, &time_structure );

   Copy( &time_structure );
}

_Check_return_ int CTimeEx::GetDay( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   struct tm time_structure;

   GetTime( time_structure );

   return( time_structure.tm_mday );
}

_Check_return_ int CTimeEx::GetDayOfWeek( void ) const noexcept // 1=Sunday
{
   WFC_VALIDATE_POINTER( this );

   struct tm time_structure;

   GetTime( time_structure );

   return( time_structure.tm_wday + 1 );
}

_Check_return_ int CTimeEx::GetDayOfYear( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   struct tm time_structure;

   GetTime( time_structure );

   return( time_structure.tm_yday + 1 );
}

_Check_return_ int CTimeEx::GetMinuteOfDay( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   struct tm time_structure;

   GetTime( time_structure );

   int const minutes = ( time_structure.tm_hour * 60 ) + time_structure.tm_min;

   return( minutes );
}

_Check_return_ int CTimeEx::GetHour( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   struct tm time_structure;

   GetTime( time_structure );

   return( time_structure.tm_hour );
}

_Check_return_ int CTimeEx::GetMinute( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   struct tm time_structure;

   GetTime( time_structure );

   return( time_structure.tm_min );
}

_Check_return_ int CTimeEx::GetMonth( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   struct tm time_structure;

   GetTime( time_structure );

   return( time_structure.tm_mon + 1 );
}

_Check_return_ int CTimeEx::GetSecond( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   struct tm time_structure;

   GetTime( time_structure );

   return( time_structure.tm_sec );
}

_Check_return_ time_t CTimeEx::GetTotalSeconds( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_Time );
}

void CTimeEx::GetTime( __out struct tm& time_structure ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   GreenwichMeanTime( &m_Time, &time_structure );
}

_Check_return_ int CTimeEx::GetYear( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   struct tm time_structure;

   GetTime( time_structure );

   return( time_structure.tm_year + 1900 );
}

#define NUMBER_OF_SECONDS_IN_A_DAY           (24L * 60L * 60L)    /* secs   in a day */
#define NUMBER_OF_SECONDS_IN_A_YEAR          (365L * NUMBER_OF_SECONDS_IN_A_DAY)    /* secs in a year */
#define NUMBER_OF_SECONDS_IN_FOUR_YEARS     (1461L * NUMBER_OF_SECONDS_IN_A_DAY)   /* secs in a 4 year interval */
#define BASE_DAY_OF_THE_WEEK          4                    /* 01-01-70 was a Thursday */

void CTimeEx::GreenwichMeanTime( __in time_t const * time_t_pointer, __out struct tm * tm_structure_p ) noexcept
{
   // This method is here because there is not a reliable gmtime() method for
   // all operating systems. Some aren't thread safe. The One of the standard
   // Unix thread safe versions is called gmtime_r() but this isn't present
   // on all of the Unixes.

   tm_structure_p->tm_hour = 0;

   long calendar_time_to_convert = (long) *time_t_pointer;

   int is_current_year_a_leap_year = 0;

   int temporary_time = 0;

   int days_in_a_year_by_month[ 13 ];

   if ( calendar_time_to_convert < 0L )
   {
      return;
   }

   /*
    * Determine years since 1970. First, identify the four-year interval
    * since this makes handling leap-years easy (note that 2000 IS a
    * leap year and 2100 is out-of-range).
    */

   temporary_time = (int) ( calendar_time_to_convert / NUMBER_OF_SECONDS_IN_FOUR_YEARS );

   calendar_time_to_convert -= ( (long) temporary_time * NUMBER_OF_SECONDS_IN_FOUR_YEARS );

   /*
    * Determine which year of the interval
    */

   temporary_time = ( temporary_time * 4 ) + 70;

   if ( calendar_time_to_convert >= NUMBER_OF_SECONDS_IN_A_YEAR )
   {
      temporary_time++;
      calendar_time_to_convert -= NUMBER_OF_SECONDS_IN_A_YEAR;

      if ( calendar_time_to_convert >= NUMBER_OF_SECONDS_IN_A_YEAR )
      {
         temporary_time++;
         calendar_time_to_convert -= NUMBER_OF_SECONDS_IN_A_YEAR;

         /*
          * Note, it takes 366 days-worth of seconds to get past a leap
          * year.
          */

         if ( calendar_time_to_convert >= ( NUMBER_OF_SECONDS_IN_A_YEAR + NUMBER_OF_SECONDS_IN_A_DAY ) )
         {
            temporary_time++;
            calendar_time_to_convert -= ( NUMBER_OF_SECONDS_IN_A_YEAR + NUMBER_OF_SECONDS_IN_A_DAY );
         }
         else
         {
            /*
             * In a leap year after all, set the flag.
             */

            is_current_year_a_leap_year++;
         }
      }
   }

   /*
    * temporary_time now holds the value for tm_year. calendar_time_to_convert now holds the
    * number of elapsed seconds since the beginning of that year.
    */

   tm_structure_p->tm_year = temporary_time;

   /*
    * Determine days since January 1 (0 - 365). This is the tm_yday value.
    * Leave calendar_time_to_convert with number of elapsed seconds in that day.
    */

   tm_structure_p->tm_yday = (int) ( calendar_time_to_convert / NUMBER_OF_SECONDS_IN_A_DAY );
   calendar_time_to_convert -= (long) ( tm_structure_p->tm_yday ) * NUMBER_OF_SECONDS_IN_A_DAY;

   /*
    * Determine months since January (0 - 11) and day of month (1 - 31)
    */

   if ( is_current_year_a_leap_year )
   {
      days_in_a_year_by_month[  0 ] = -1;
      days_in_a_year_by_month[  1 ] = 30;
      days_in_a_year_by_month[  2 ] = 59;
      days_in_a_year_by_month[  3 ] = 90;
      days_in_a_year_by_month[  4 ] = 120;
      days_in_a_year_by_month[  5 ] = 151;
      days_in_a_year_by_month[  6 ] = 181;
      days_in_a_year_by_month[  7 ] = 212;
      days_in_a_year_by_month[  8 ] = 243;
      days_in_a_year_by_month[  9 ] = 273;
      days_in_a_year_by_month[ 10 ] = 304;
      days_in_a_year_by_month[ 11 ] = 334;
      days_in_a_year_by_month[ 12 ] = 365;
   }
   else
   {
      days_in_a_year_by_month[  0 ] = -1;
      days_in_a_year_by_month[  1 ] = 30;
      days_in_a_year_by_month[  2 ] = 58;
      days_in_a_year_by_month[  3 ] = 89;
      days_in_a_year_by_month[  4 ] = 119;
      days_in_a_year_by_month[  5 ] = 150;
      days_in_a_year_by_month[  6 ] = 180;
      days_in_a_year_by_month[  7 ] = 211;
      days_in_a_year_by_month[  8 ] = 242;
      days_in_a_year_by_month[  9 ] = 272;
      days_in_a_year_by_month[ 10 ] = 303;
      days_in_a_year_by_month[ 11 ] = 333;
      days_in_a_year_by_month[ 12 ] = 364;
   }

   for ( temporary_time = 1 ; days_in_a_year_by_month[ temporary_time ] < tm_structure_p->tm_yday ; temporary_time++ )
   {
      ;
   }

   tm_structure_p->tm_mon = --temporary_time;

   tm_structure_p->tm_mday = tm_structure_p->tm_yday - days_in_a_year_by_month[ temporary_time ];

   /*
    * Determine days since Sunday (0 - 6)
    */

   tm_structure_p->tm_wday = ((int)( *time_t_pointer / NUMBER_OF_SECONDS_IN_A_DAY ) + BASE_DAY_OF_THE_WEEK ) % 7;

   /*
    *  Determine hours since midnight (0 - 23), minutes after the hour
    *  (0 - 59), and seconds after the minute (0 - 59).
    */

   tm_structure_p->tm_hour = (int) ( calendar_time_to_convert / 3600 );
   calendar_time_to_convert -= (long) tm_structure_p->tm_hour * 3600L;

   tm_structure_p->tm_min = (int) ( calendar_time_to_convert / 60 );
   tm_structure_p->tm_sec = (int) ( calendar_time_to_convert - (tm_structure_p->tm_min) * 60 );

   tm_structure_p->tm_isdst = 0;
}

_Check_return_ time_t CTimeEx::m_Make_time_t( __in struct tm const * time_parameter ) noexcept
{
   WFC_VALIDATE_POINTER( this );

/*
 * ChkAdd evaluates to TRUE if dest = src1 + src2 has overflowed
 */
#define ChkAdd(dest, src1, src2)   ( ((src1 >= 0L) and (src2 >= 0L) \
    and (dest < 0L)) or ((src1 < 0L) and (src2 < 0L) and (dest >= 0L)) )

/*
 * ChkMul evaluates to TRUE if dest = src1 * src2 has overflowed
 */
#define ChkMul(dest, src1, src2)   ( src1 ? (dest/src1 != src2) : 0 )

   long time_1 = 0;
   long time_2 = 0;
   long time_3 = 0;

   struct tm tm_time;

   ZeroMemory( &tm_time, sizeof( tm_time ) );

   tm_time.tm_year  = time_parameter->tm_year;
   tm_time.tm_mon   = time_parameter->tm_mon;
   tm_time.tm_mday  = time_parameter->tm_mday;
   tm_time.tm_hour  = time_parameter->tm_hour;
   tm_time.tm_min   = time_parameter->tm_min;
   tm_time.tm_sec   = time_parameter->tm_sec;
   tm_time.tm_isdst = time_parameter->tm_isdst;

   /*
    * First, make sure tm_year is reasonably close to being in range.
    */

   if ( ((time_1 = tm_time.tm_year) < 69L) or (time_1 > 139L) )
   {
      return( (time_t) -1 );
   }

   /*
    * Adjust month value so it is in the range 0 - 11. This is because
    * we don't know how many days are in months 12, 13, 14, etc.
    */

   if ( (tm_time.tm_mon < 0) or (tm_time.tm_mon > 11) )
   {
      /*
       * no danger of overflow because the range check above.
       */

      time_1 += (tm_time.tm_mon / 12);

      if ( (tm_time.tm_mon %= 12) < 0 )
      {
         tm_time.tm_mon += 12;
         time_1--;
      }

      /*
       * Make sure year count is still in range.
       */
      if ( (time_1 < 69) or (time_1 > 139) )
      {
         return( (time_t) -1 );
      }
   }

   /***** HERE: time_1 holds number of elapsed years *****/

   /*
    * Calculate days elapsed minus one, in the given year, to the given
    * month. Check for leap year and adjust if necessary.
    */

   constexpr int const days[ 13 ] = { -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364 };

   time_2 = days[ tm_time.tm_mon ];

   if ( ! ( time_1 bitand 3 ) and ( tm_time.tm_mon > 1 ) )
   {
      time_2++;
   }

   /*
    * Calculate elapsed days since base date (midnight, 1/1/70, UTC)
    *
    *
    * 365 days for each elapsed year since 1970, plus one more day for
    * each elapsed leap year. no danger of overflow because of the range
    * check (above) on time_1.
    */

   time_3 = (time_1 - 70L) * 365L + ((time_1 - 1L) >> 2) - 17L;

   /*
    * elapsed days to current month (still no possible overflow)
    */

   time_3 += time_2;

   /*
    * elapsed days to current date. overflow is now possible.
    */

   time_1 = time_3 + (time_2 = (long)(tm_time.tm_mday));

   if ( ChkAdd( time_1, time_3, time_2 ) )
   {
      return( (time_1) -1 );
   }

   /***** HERE: time_1 holds number of elapsed days *****/

   /*
    * Calculate elapsed hours since base date
    */

   time_2 = time_1 * 24L;

   if ( ChkMul( time_2, time_1, 24L ) )
   {
      return( (time_1) -1 );
   }

   time_1 = time_2 + (time_3 = (long)tm_time.tm_hour);

   if ( ChkAdd( time_1, time_2, time_3 ) )
   {
      return( (time_1) -1 );
   }

   /***** HERE: time_1 holds number of elapsed hours *****/

   /*
    * Calculate elapsed minutes since base date
    */

   time_2 = time_1 * 60L;

   if ( ChkMul( time_2, time_1, 60L ) )
   {
      return( (time_1) -1 );
   }

   time_1 = time_2 + (time_3 = (long)tm_time.tm_min);

   if ( ChkAdd( time_1, time_2, time_3 ) )
   {
      return( (time_1) -1 );
   }

   /***** HERE: time_1 holds number of elapsed minutes *****/

   /*
    * Calculate elapsed seconds since base date
    */

   time_2 = time_1 * 60L;

   if ( ChkMul( time_2, time_1, 60L ) )
   {
      return( (time_t) -1 );
   }

   time_1 = time_2 + (time_3 = (long)tm_time.tm_sec);

   if ( ChkAdd( time_1, time_2, time_3 ) )
   {
      return( (time_t) -1 );
   }

   /***** HERE: time_1 holds number of elapsed seconds *****/

   return( (time_t) time_1 );
}

void CTimeEx::Set( __in int const year, __in int const month, __in int const day, __in int const hour, __in int const minute, __in int const second, __in int const daylight_savings_time ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ASSERT( day   >= 1 and day   <= 31 );
   ASSERT( month >= 1 and month <= 12 );
   ASSERT( year  >= 1900 );

   struct tm tm_structure;

   ZeroMemory( &tm_structure, sizeof( tm_structure ) );

   tm_structure.tm_year  = year  - 1900;
   tm_structure.tm_mon   = month - 1;
   tm_structure.tm_mday  = day;
   tm_structure.tm_hour  = hour;
   tm_structure.tm_min   = minute;
   tm_structure.tm_sec   = second;
   tm_structure.tm_isdst = daylight_savings_time;

   m_Time = m_Make_time_t( &tm_structure );

   if ( m_Time == (-1) )
   {
      m_Time  = 0;
   }
}

void CTimeEx::Set( __in std::wstring const& iso_8601_string ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // This routine is written for simplicity. This
   // means it is very long and slow. Yes, I could
   // write it to be faster but it wouldn't be as
   // easy to follow.

   Empty();

   std::wstring temp_string( iso_8601_string );

   std::size_t const string_length = temp_string.length();

   if ( string_length < 4 )
   {
      return;
   }

   std::wstring value( temp_string.substr( 0, 4 ) );

   int const year = static_cast<int>(as_integer( value ));

   temp_string.erase(0, 4);

   // Let's see if we should bail

   if ( temp_string.empty() == true )
   {
      // Yup, let's bail

      Set( year, 1, 1, 0, 0, 0 );
      return;
   }

   // The next character should be a -
   //     |
   //     v
   // 1963-05-02T21:30:01.45Z

   if ( temp_string.at( 0 ) != '-' )
   {
      return;
   }

   temp_string.erase(0, 1);

   //      |
   //      v
   // 1963-05-02T21:30:01.45Z

   if ( temp_string.empty() == true )
   {
      // OK, sloppy date but we'll take it

      Set( year, 1, 1, 0, 0, 0 );
      return;
   }

   if ( temp_string.length() < 2 )
   {
      return;
   }

   value.assign( temp_string.substr( 0, 2 ) );

   temp_string.erase(0, 2);

   int const month = static_cast<int>(as_integer(value));

   // Now let's idiot proof the month

   if ( month < 1 or month > 12 )
   {
      return;
   }

   // Let's see if we should bail

   if ( temp_string.empty() == true )
   {
      // Yup, let's bail

      Set( year, month, 1, 0, 0, 0 );
      return;
   }

   // The next character should be a -
   //        |
   //        v
   // 1963-05-02T21:30:01.45Z

   if ( temp_string.at( 0 ) != '-' )
   {
      return;
   }

   temp_string.erase(0, 1);

   if ( temp_string.empty() == true )
   {
      // OK, sloppy date but we'll take it

      Set( year, month, 1, 0, 0, 0 );
      return;
   }

   // We should now be sitting on the day
   //         |
   //         v
   // 1963-05-02T21:30:01.45Z

   if ( temp_string.length() < 2 )
   {
      return;
   }

   value.assign( temp_string.substr( 0, 2 ) );

   temp_string.erase(0, 2);

   int const day = static_cast<int>(as_integer(value));

   // Now let's idiot proof the day

   if ( day < 1 or day > 31 )
   {
      return;
   }

   // Let's see if we should bail

   if ( temp_string.empty() == true )
   {
      // Yup, let's bail

      Set( year, month, day, 0, 0, 0 );
      return;
   }

   // The next character should be a T
   //           |
   //           v
   // 1963-05-02T21:30:01.45Z

   if ( temp_string.at( 0 ) != 'T' )
   {
      return;
   }

   temp_string.erase(0, 1);

   if ( temp_string.empty() == true )
   {
      // OK, sloppy date but we'll take it

      Set( year, month, day, 0, 0, 0 );
      return;
   }

   // We should now be sitting on the hours
   //            |
   //            v
   // 1963-05-02T21:30:01.45Z

   if ( temp_string.length() < 2 )
   {
      return;
   }

   value.assign( temp_string.substr( 0, 2 ) );

   int const hours = static_cast<int>(as_integer(value));

   if ( hours > 24 )
   {
      return;
   }

   temp_string.erase(0, 2);

   // Let's see if we're finished

   if ( temp_string.empty() == true )
   {
      Set( year, month, day, hours, 0, 0 );
      return;
   }

   // We should be at a colon
   //              |
   //              v
   // 1963-05-02T21:30:01.45Z

   if ( temp_string.at( 0 ) != ':' )
   {
      return;
   }

   temp_string.erase(0, 1);

   if ( temp_string.empty() == true )
   {
      // Oddly formed but not illegal
      Set( year, month, day, hours, 0, 0 );
      return;
   }

   if ( temp_string.length() < 2 )
   {
      return;
   }

   value.assign( temp_string.substr( 0, 2 ) );

   int minutes = static_cast<int>(as_integer(value));

   if ( minutes > 59 )
   {
      return;
   }

   temp_string.erase(0, 2);

   // Let's see if we are finished

   if ( temp_string.empty() == true )
   {
      Set( year, month, day, hours, minutes, 0 );
      return;
   }

   // We should be at another colon
   //                 |
   //                 v
   // 1963-05-02T21:30:01.45Z

   if ( temp_string.at(0) != ':' )
   {
      return;
   }

   temp_string.erase(0, 1);

   if ( temp_string.empty() == true )
   {
      // Oddly formed but legal
      Set( year, month, day, hours, minutes, 0 );
      return;
   }

   // Now let's get them there seconds
   //                  |
   //                  v
   // 1963-05-02T21:30:01.45Z

   if ( temp_string.length() < 2 )
   {
      // That ain't right
      return;
   }

   value.assign( temp_string.substr( 0, 2 ) );

   int seconds = static_cast<int>(as_integer(value));

   temp_string.erase(0, 2);

   // Let's see if we're finished

   if ( temp_string.empty() == true )
   {
      Set( year, month, day, hours, minutes, seconds );
      return;
   }

   char const character = static_cast<char>(temp_string.at( 0 ));

   // Here's where it gets interesting, this can be the
   // end of the string or a fractional second

   if ( character == 'Z' )
   {
      // We be done
      Set( year, month, day, hours, minutes, seconds );
      return;
   }

   unsigned long nanoseconds = 0;

   if ( character == '.' )
   {
      std::size_t const character_index = temp_string.find_first_of(WSTRING_VIEW(L"Z+-"));

      if ( character_index == std::wstring::npos )
      {
         //WFCTRACE( TEXT( "Ill formed fractional seconds" ) );
         return;
      }

      value.assign(WSTRING_VIEW(L"0."));
      value.append( temp_string.substr( 0, character_index ) );

      double fractional_second = fractional_second = _wtof( value.c_str() );

      fractional_second *= static_cast<double>(NUMBER_OF_NANOSECONDS_IN_ONE_SECOND);

      nanoseconds = static_cast<unsigned long>(fractional_second);

      Set( year, month, day, hours, minutes, seconds, nanoseconds );

      temp_string.erase(0, character_index);
   }

   Set( year, month, day, hours, minutes, seconds, nanoseconds );

   if ( temp_string.empty() == true )
   {
      return;
   }

   //                       |
   //                       v
   // 1963-05-02T21:30:01.45Z

   if ( temp_string.at( 0 ) == 'Z' )
   {
      return;
   }

   // OK, what's left must be a hour and minute offset

   //                       |
   //                       v
   // 1963-05-02T21:30:01.45-05:00

   if ( temp_string.length() < 6 )
   {
      return;
   }

   //                          |
   //                          v
   // 1963-05-02T21:30:01.45-05:00

   if ( temp_string.at( 3 ) != ':' )
   {
      return;
   }

   int const offset_hours   = static_cast<int>(as_integer(temp_string.substr( 1, 2 )));
   int const offset_minutes = static_cast<int>(as_integer(temp_string.substr( 4, 2 )));

   CTimeSpan const time_span( 0, offset_hours, offset_minutes, 0 );

   if ( temp_string.at( 0 ) == '-' )
   {
      m_Time += time_span.GetTotalSeconds();
   }
   else
   {
      m_Time -= time_span.GetTotalSeconds();
   }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CTimeEx</TITLE>
</HEAD>

<BODY>

<H1>CTimeEx : timespec </H1>

$Revision: 8 $

<BR><HR>

<H2>Description</H2>

This class extends the timespec data structure. It provides an easy way to deal
with time in C++. <B>CTimeEx</B> deals only in GMT time. If you want local time,
you will have to drop down to C.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CTimeEx</B>()
<B>CTimeEx</B>( const CTimeEx&amp;    source )
<B>CTimeEx</B>( const CTimeEx *   source )
<B>CTimeEx</B>( const timespec&amp;   source )
<B>CTimeEx</B>( const timespec *  source )
<B>CTimeEx</B>( const time_t      source )
<B>CTimeEx</B>( const struct tm * source )
<B>CTimeEx</B>( const struct tm&amp;  source )
<B>CTimeEx</B>( int year,
       int month,
       int day,
       int hour,
       int minute,
       int second,
       unsigned long nanoseconds = 0,
       int daylight_savings_time = -1 )</PRE><DD>
Constructs the object in a whole bunch of different ways.
</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>int <B><A NAME="Compare">Compare</A></B>( const CTimeEx&amp; source ) const</PRE><DD>
This method compares the current <B>CTimeEx</B> with a
<B>CTimeEx</B> passed. If the two are equal, the return value is zero.
If the current
<B>CTimeEx</B> is less than the <B>CTimeEx</B> passed as an argument,
the return value is 1. If the
current <B>CTimeEx</B> is less than the <B>CTimeEx</B>
passed as an argument, the return value is -1.

<DT><PRE>void <B><A NAME="Copy">Copy</A></B>( const CTimeEx&amp;  source )
void <B>Copy</B>( const CTimeEx * source )</PRE><DD>
This method copies the contents of another <B>CTimeEx</B> object.

<DT><PRE>void <B>Copy</B>( const timespec&amp;  source )
void <B>Copy</B>( const timespec * source )</PRE><DD>
This method copies the contents of a timespec object.

<DT><PRE>void <B>Copy</B>( const time_t source )</PRE><DD>
This method copies the contents of a time_t object.

<DT><PRE>void <B>Copy</B>( const struct tm&amp;  source )
void <B>Copy</B>( const struct tm * source )</PRE><DD>
This method copies the contents of a tm structure.

<DT><PRE>void <B>CopyModifiedJulianDate</B>( double source )</PRE><DD>
Loads the data members based upon the Modified Julian Date (MJD).
MJD is the number of days since November 17, 1858. Why this date?
Why <B>NOT</B> this date?

<DT><PRE>void <B><A NAME="CopyTo">CopyTo</A></B>( struct tm&amp; destination ) const
void <B>CopyTo</B>( time_t&amp; destination ) const
void <B>CopyTo</B>( timespec&amp; destination ) const
void <B>CopyTo</B>( std::wstring&amp; iso_8601_string ) const</PRE><DD>
Copies the time from the <B>CTimeEx</B> to the destination specified.
If you copy to a std::wstring, it will
be written in ISO8601 format.

<DT><PRE>void <B><A NAME="Empty">Empty</A></B>( void )</PRE><DD>
This method zeroizes the data members.

<DT><PRE>std::wstring <B><A NAME="Format">Format</A></B>( const char *format_string ) const</PRE><DD>
This method lets you format the time into a string.
It uses the same arguments as <CODE>strftime()</CODE>.
The allowed percent codes are as follows:

<UL>
<LI><CODE>%%</CODE> - same as %
<LI><CODE>%a</CODE> - abbreviated weekday name
<LI><CODE>%A</CODE> - full weekday name
<LI><CODE>%b</CODE> - abbreviated month name
<LI><CODE>%B</CODE> - full month name
<LI><CODE>%c</CODE> - date and time
<LI><CODE>%C</CODE> - century number
<LI><CODE>%d</CODE> - day of month (01-31)
<LI><CODE>%D</CODE> - date as %m/%d/%y
<LI><CODE>%e</CODE> - day of month (single digits are preceeded by a space)
<LI><CODE>%h</CODE> - abbreviated month name
<LI><CODE>%H</CODE> - hour (00-23)
<LI><CODE>%I</CODE> - hour (01-12)
<LI><CODE>%j</CODE> - day number of year (001-366)
<LI><CODE>%KC</CODE> - appropriate date and time representation
<LI><CODE>%m</CODE> - month number (01-12)
<LI><CODE>%M</CODE> - minute (00-59)
<LI><CODE>%n</CODE> - same as new-line
<LI><CODE>%p</CODE> - AM or PM
<LI><CODE>%r</CODE> - time as %I:%M:%S [AM|PM]
<LI><CODE>%R</CODE> - time as %H:%M
<LI><CODE>%S</CODE> - seconds (00-61, allows for leap seconds)
<LI><CODE>%t</CODE> - same as tab
<LI><CODE>%T</CODE> - time as %H:%M:%S
<LI><CODE>%U</CODE> - week number of year (00-53) Sunday is first day of week 1
<LI><CODE>%w</CODE> - weekday number (0-6) Sunday = 0
<LI><CODE>%W</CODE> - week number of year (00-53), Monday is first day of week 1
<LI><CODE>%x</CODE> - date
<LI><CODE>%X</CODE> - time
<LI><CODE>%y</CODE> - year within century
<LI><CODE>%Y</CODE> - four digit year
<LI><CODE>%Z</CODE> - time zone name
</UL>

<DT><PRE>static void <B><A NAME="GetCurrentTime">GetCurrentTime</A></B>( CTimeEx&amp; time )</PRE><DD>
Retrieves the current time from the operating system.

<DT><PRE>int <B><A NAME="GetDay">GetDay</A></B>( void ) const</PRE><DD>Returns the day of the month (1-31).

<DT><PRE>int <B><A NAME="GetDayOfWeek">GetDayOfWeek</A></B>( void ) const</PRE><DD>Returns the day of the week (1=Sunday).

<DT><PRE>int <B><A NAME="GetDayOfYear">GetDayOfYear</A></B>( void ) const</PRE><DD>Returns the day of the year (1-366).

<DT><PRE>int <B><A NAME="GetHour">GetHour</A></B>( void ) const</PRE><DD>Returns the hour of the day.

<DT><PRE>int <B><A NAME="GetMinute">GetMinute</A></B>( void ) const</PRE><DD>Returns the minute of the hour.

<DT><PRE>int <B><A NAME="GetMonth">GetMonth</A></B>( void ) const</PRE><DD>Returns the month of the year (1=January).

<DT><PRE>long <B><A NAME="GetNanoseconds">GetNanoseconds</A></B>( void ) const</PRE><DD>Returns the nanosecond part of the time.

<DT><PRE>int <B><A NAME="GetSecond">GetSecond</A></B>( void ) const</PRE><DD>Returns the seconds in the minute.

<DT><PRE>struct tm * <B><A NAME="GetTime">GetTime</A></B>( struct tm * ) const</PRE><DD>
Returns a struct tm pointer that points to the time in GMT.

<DT><PRE>time_t <B><A NAME="GetTotalSeconds">GetTotalSeconds</A></B>( void ) const</PRE><DD>Returns the total number of seconds in the time.

<DT><PRE>int <B><A NAME="GetYear">GetYear</A></B>( void ) const</PRE><DD>Returns the four digit year.

<DT><PRE>static void <B><A NAME="GreenwichMeanTime">GreenwichMeanTime</A></B>( const time_t * address_of_a_time_t, struct tm * address_of_a_tm_structure )</PRE><DD>
This a thread-safe and portable implementation of <CODE>gmtime()</CODE>
that will compile on all of the
standard Unixes. It converts a <CODE>time_t</CODE> to a <CODE>struct tm</CODE>.

<DT><PRE>void <B><A NAME="Set">Set</A></B>( int year, int month, int day, int hour, int minute, int second, unsigned long nanosecond = 0, int daylight_savings_time = -1 )
void <B>Set</B>( const std::wstring&amp; iso_8601_date_string )</PRE><DD>
Allows you to set the individual year, month, day, hour,
minute, seconds and nanoseconds of the time. It will
also accept a string with a date in ISO8601 format.

</DL>

<H2>Operators</H2>

<DL COMPACT>

<DT><PRE>operator <B>time_t</B> ( void ) const</PRE></DD>
This allows you to use a <B>CTimeEx</B> anywhere you would need a
<B>time_t</B>.

<DT><PRE>operator <B>timespec *</B> ( void )</PRE><DD>
This allows you to use a <B>CTimeEx</B> anywhere you would need a
<B>timespec</B> pointer.

<DT><PRE>CTimeEx&amp; operator <B>=</B> ( const CTimeEx&amp; source )
CTimeEx&amp; operator <B>=</B> ( const timespec&amp; source )
CTimeEx&amp; operator <B>=</B> ( const time_t source )
CTimeEx&amp; operator <B>=</B> ( const struct tm&amp; source )</PRE><DD>
This allows you to copy various representations of time and put
them into a CTimeEx.

<DT><PRE>BOOL operator <B>==</B> ( const CTimeEx&amp; source ) const</PRE><DD>
This will return TRUE if this CTimeEx is equal to <CODE>source</CODE>.

<DT><PRE>BOOL operator <B>!=</B> ( const CTimeEx&amp; source ) const</PRE><DD>
This will return TRUE if this CTimeEx is
not equal to <CODE>source</CODE>.

<DT><PRE>BOOL operator <B>&gt;=</B> ( const CTimeEx&amp; source ) const</PRE><DD>
This will return TRUE if this CTimeEx is greater than or equal to
<CODE>source</CODE>.

<DT><PRE>BOOL operator <B>&lt;=</B> ( const CTimeEx&amp; source ) const</PRE><DD>
This will return TRUE if this <B>CTimeEx</B> is less than or equal to
<CODE>source</CODE>.

<DT><PRE>BOOL operator <B>&gt;</B> ( const CTimeEx&amp; source ) const</PRE><DD>
This will return TRUE if this CTimeEx is greater than <CODE>source</CODE>.

<DT><PRE>BOOL operator <B>&lt;</B> ( const CTimeEx&amp; source ) const</PRE><DD>
This will return TRUE if this CTimeEx is less than <CODE>source</CODE>.

<DT><PRE>CTimeEx operator <B>-</B> ( const <A HREF="CTimeSpan.html">CTimeSpan</A>&amp; value ) const
CTimeSpan operator <B>-</B> ( const CTimeEx&amp; source ) const</PRE><DD>
Allows you to subtract a time span from a CTimeEx.

<DT><PRE>CTimeEx operator <B>+</B> ( const <A HREF="CTimeSpan.html">CTimeSpan</A>&amp; value ) const</PRE><DD>
Allows you to add a time span to a CTimeEx.

<DT><PRE>const CTimeEx&amp; <B>+=</B> ( const <A HREF="CTimeSpan.html">CTimeSpan</A>&amp; value ) const</PRE><DD>
Allows you to increment this CTimeEx object by the amount of time in
<CODE>value</CODE>.

<DT><PRE>const CTimeEx&amp; <B>-=</B> ( const <A HREF="CTimeSpan.html">CTimeSpan</A>&amp; value ) cosnt</PRE><DD>
Allows you to decrement this CTimeEx object by the amount of time in <CODE>value</CODE>.

</DL>

<H2>Example</H2>

<PRE><CODE>void main()
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;main()&quot; ) );

   <A HREF="CTimeSpan.html">CTimeSpan</A> how_long_i_have_been_alive;

   <B>CTimeEx</B> current_time;

   <B>CTimeEx</B>::GetCurrentTime( current_time );

   <B>CTimeEx</B> my_birthday( 1963, 5, 2, 21, 22, 29, 1000000000 ); // 9:22.30pm, May 2, 1963

   how_long_i_have_been_alive = current_time - my_birthday;

   std::wstring time_string;

   time_string = how_long_i_have_been_alive.Format( &quot;%D days, %H hours, %M minutes, %S seconds&quot; );

   printf( &quot;%s\n&quot;, (char const *) time_string );

   <A HREF="CTimeSpan.htm">CTimeSpan</A> one_thousand_hours( 0, 1000, 0, 0 );

   current_time += one_thousand_hours;

   time_string = current_time.Format( &quot;%x %X&quot; );

   printf( &quot;A thousand hours from now is %s\n&quot;, (char const *) time_string );

   std::wstring iso_date;

   current_time.CopyTo( iso_date );

   printf( &quot;Standard Date: %s\n&quot;, (char const *) iso_date );
}</CODE></PRE>

<H2>API&#39;s Used</H2>

<B>CTimeEx</B> uses the following API&#39;s:

<UL>
<LI>_tcsftime
<LI>time
</UL>

<HR>

Copyright, 2000, <A HREF="mailto:sam_blackburn@pobox.com">Samuel R. Blackburn</A>
</BODY>

</HTML>
*/
