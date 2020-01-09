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
** $Workfile: CFileTime.hpp $
** $Revision: 14 $
** $Modtime: 6/26/01 11:01a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined ( FILE_TIME_CLASS_HEADER )

#define FILE_TIME_CLASS_HEADER

#if ! defined( __oledb_h__ )

struct DBTIMESTAMP
    {
    SHORT year{ 0 };
    USHORT month{ 0 };
    USHORT day{ 0 };
    USHORT hour{ 0 };
    USHORT minute{ 0 };
    USHORT second{ 0 };
    ULONG fraction{ 0 };
    };

#endif // __oledb_h__

class CFileTime : public _FILETIME
{
    public:

       static constexpr uint64_t const DecThirty1899 = 94353120000000000UI64;

       // The number of FILETIME ticks since Jan 1, 2001
       static constexpr uint64_t const JanFirst2001                                    = 126227808000000000UI64;

       // The number of FILETIME ticks since Jan 1, 1970
       static constexpr uint64_t const JanFirst1970                                    = 116444736000000000UI64;

       // The number of FILETIME ticks between Jan 1, 1 and Jan 1, 1601
       static constexpr uint64_t const TicksBetween0001And1601                         = 504911232000000000I64;

       // The number of FILETIME ticks in one microsecond
       static constexpr uint64_t const OneMicrosecond                                  = 10;

       // The number of FILETIME ticks in one millisecond
       static constexpr uint64_t const NumberOfTicksPerMillisecond                     = 10000;

       // The number of milliseconds between Windows (1601) and Unix (1970) epochs
       static constexpr uint64_t const NumberOfMillisecondsBetweenWindowsAndUnixEpochs = 11644473600000I64;

       // The number of milliseconds in one minute
       static constexpr uint64_t const NumberOfMillisecondsInOneMinute                 = 60000I64;

       // The number of milliseconds in one hour
       static constexpr uint64_t const NumberOfMillisecondsInOneHour                   = 3600000I64;

       // The number of milliseconds in one day
       static constexpr uint64_t const NumberOfMillisecondsInOneDay                    = 86400000I64;

       // The number of milliseconds in one week
       static constexpr uint64_t const NumberOfMillisecondsInOneWeek                   = 604800000I64;

       // The number of nanoseconds in one microsecond
       static constexpr uint64_t const NumberOfNanosecondsInOneMicrosecond             = 1000I64;

       // The number of nanoseconds in one FILETIME tick
       static constexpr uint64_t const NumberOfNanosecondsInOneFiletimeTick            = 100I64;

       // The number of microseconds in one millisecond
       static constexpr uint64_t const NumberOfMicrosecondsInOneMillisecond            = 1000;
       static constexpr uint64_t const NumberOfMicrosecondsInOneSecond                 = 1000000;

       // The number of FILETIME ticks in one microsecond
       static constexpr uint64_t const NumberOfFiletimeTicksInOneMicrosecond           = 10;

       // The number of FILETIME ticks in one millisecond
       static constexpr uint64_t const NumberOfFiletimeTicksInOneMillisecond           = 10000;

       // The number of FILETIME ticks in one second
       static constexpr uint64_t const NumberOfFiletimeTicksInOneSecond                = 10000000;

       // The number of FILETIME ticks in one minute
       static constexpr uint64_t const NumberOfFiletimeTicksInOneMinute                = 600000000;

       // The number of FILETIME ticks in one hour
       static constexpr uint64_t const NumberOfFiletimeTicksInOneHour                  = 36000000000I64;

       // The number of FILETIME ticks in one day
       static constexpr uint64_t const NumberOfFiletimeTicksInOneDay                   = 864000000000I64;

       // The number of FILETIME ticks in one year
       static constexpr uint64_t const NumberOfFiletimeTicksInOneYear                  = 315360000000000I64;

       static inline constexpr _Check_return_ int64_t ConvertHoursMinutesToSeconds( _In_ int const hours, _In_ int const minutes ) noexcept
       {
          int64_t const return_value = ( static_cast<int64_t>(hours) * 3600 ) + ( static_cast<int64_t>(minutes) * 60 );

          return( return_value );
       }

       static inline constexpr _Check_return_ int64_t ConvertHoursMinutesToTicks(_In_ int const hours, _In_ int const minutes ) noexcept
       {
          int64_t return_value = ConvertHoursMinutesToSeconds( hours, minutes );

          return_value *= CFileTime::NumberOfFiletimeTicksInOneSecond;

          return( return_value );
       }

       static inline _Check_return_ uint64_t TicksNow( void ) noexcept
       {
           FILETIME file_time{ 0, 0 };

           GetSystemTimePreciseAsFileTime( &file_time );

           ULARGE_INTEGER li;

           li.LowPart = file_time.dwLowDateTime;
           li.HighPart = file_time.dwHighDateTime;

           return( li.QuadPart );
       }

       inline constexpr CFileTime() noexcept : _FILETIME{ 0, 0 } {}

       inline CFileTime(_In_ CFileTime const& source ) noexcept
       {
          Copy( source );
       }

       inline CFileTime(_In_ CSystemTime const& source ) noexcept
       {
          Copy( source );
       }

       inline CFileTime(_In_ WORD ms_dos_date, _In_ WORD ms_dos_time ) noexcept
       {
          Copy( ms_dos_date, ms_dos_time );
       }

       inline CFileTime(_In_ FILETIME const * source ) noexcept
       {
          Copy( source );
       }

       inline constexpr CFileTime( _In_ FILETIME const& source ) noexcept
       {
           dwLowDateTime = source.dwLowDateTime;
           dwHighDateTime = source.dwHighDateTime;
       }

       inline CFileTime(_In_ SYSTEMTIME const * source ) noexcept
       {
          Copy( source );
       }

       inline CFileTime(_In_ SYSTEMTIME const& source ) noexcept
       {
          Copy( source );
       }

       inline CFileTime(_In_ TIMESTAMP_STRUCT const * timestamp ) noexcept
       {
          Copy( timestamp );
       }

       inline CFileTime(_In_ DBTIMESTAMP const * timestamp ) noexcept
       {
          Copy( timestamp );
       }

       inline CFileTime(_In_ TIMESTAMP_STRUCT const& timestamp ) noexcept
       {
          Copy( timestamp );
       }

       inline CFileTime(_In_ DBTIMESTAMP const& timestamp ) noexcept
       {
          Copy( timestamp );
       }

       // 2000-07-20
       // As suggested by Josh Parris (Josh.Parris@auspost.com.au)
       inline constexpr CFileTime(_In_ LARGE_INTEGER const& source ) noexcept
       {
          Copy( source );
       }

       inline constexpr CFileTime(_In_ LARGE_INTEGER const * source ) noexcept
       {
          Copy( source );
       }

       inline constexpr CFileTime(_In_ ULARGE_INTEGER const& source ) noexcept
       {
          Copy( source );
       }

       inline constexpr CFileTime(_In_ ULARGE_INTEGER const * source ) noexcept
       {
          Copy( source );
       }

       inline CFileTime(_In_ int const year, _In_ int const month, _In_ int const day, _In_ int const hour, _In_ int const minute, _In_ int const second, _In_ int const milliseconds = 0 ) noexcept
       {
          Set( year, month, day, hour, minute, second, milliseconds );
       }

       inline CFileTime(_In_ int const year, _In_ int const month, _In_ int const day, _In_ int const hour, _In_ int const minute, _In_ double const seconds ) noexcept
       {
          Set( year, month, day, hour, minute, seconds );
       }

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CFileTime()
      {
         Empty();
      }

      inline void ToLocalTime( void ) noexcept
      {
         FILETIME local_time = { 0, 0 };

         if ( ::FileTimeToLocalFileTime( this, &local_time ) != FALSE )
         {
            dwLowDateTime = local_time.dwLowDateTime;
            dwHighDateTime = local_time.dwHighDateTime;
         }
      }

      inline constexpr void AddSeconds( _In_ double const number_of_seconds ) noexcept
      {
         int64_t const number_of_ticks = static_cast<int64_t>( static_cast<double>(CFileTime::NumberOfFiletimeTicksInOneSecond) * number_of_seconds );

         int64_t ll = static_cast<int64_t>( static_cast<int64_t>(dwHighDateTime) << 32 );

         ll += dwLowDateTime;
         ll += number_of_ticks;

         dwLowDateTime = static_cast<DWORD>(ll);
         dwHighDateTime = ll >> 32;
      }

      inline constexpr _Check_return_ bool IsAfter(_In_ FILETIME const& that ) const noexcept
      {
         if ( dwHighDateTime > that.dwHighDateTime )
         {
            return( true );
         }

         if ( dwHighDateTime == that.dwHighDateTime )
         {
            if ( dwLowDateTime > that.dwLowDateTime )
            {
               return( true );
            }
         }

         return( false );
      }

      inline constexpr _Check_return_ bool IsBefore(_In_ FILETIME const& that ) const noexcept
      {
         if ( dwHighDateTime < that.dwHighDateTime )
         {
            return( true );
         }

         if ( dwHighDateTime == that.dwHighDateTime )
         {
            if ( dwLowDateTime < that.dwLowDateTime )
            {
               return( true );
            }
         }

         return( false );
      }

      inline constexpr _Check_return_ LONG Compare(_In_ CFileTime const& that ) const noexcept
      {
         // David LeBlanc (whisper@accessone.com) had problems with VC4.2
         // not compiling dynamic_cast's correctly. That is why the old-style
         // casts are used here.
         // 1998-08-19

          ULARGE_INTEGER const this_value{ dwLowDateTime, dwHighDateTime };
          ULARGE_INTEGER const that_value{ that.dwLowDateTime, that.dwHighDateTime };

          if (this_value.QuadPart > that_value.QuadPart)
          {
              return(I_AM_GREATER_THAN_THAT);
          }
          else if (this_value.QuadPart < that_value.QuadPart)
          {
              return(I_AM_LESS_THAN_THAT);
          }

          return(I_AM_EQUAL_TO_THAT);
      }

      inline constexpr void Copy(_In_ CFileTime const& source ) noexcept
      {
         dwLowDateTime  = source.dwLowDateTime;
         dwHighDateTime = source.dwHighDateTime;
      }

      inline void Copy(_In_ CSystemTime const& source ) noexcept
      {
         // David LeBlanc (whisper@accessone.com) had problems with VC4.2
         // not compiling dynamic_cast's correctly. That is why the old-style
         // casts are used here.
         // 1998-08-19
         Copy( static_cast<SYSTEMTIME const *>(&source) );
      }

      inline void Copy(_In_ FILETIME const * source ) noexcept
      {
         // Copying ourself is a silly thing to do

         if ( source == this )
         {
            return;
         }

         if ( source == nullptr )
         {
            Empty();
            return;
         }

         // We were passed a pointer, don't trust it

         WFC_TRY
         {
            dwLowDateTime  = source->dwLowDateTime;
            dwHighDateTime = source->dwHighDateTime;
         }
         WFC_CATCH_ALL
         {
            Empty();
            return;
         }
         WFC_END_CATCH_ALL
      }

      inline constexpr void Copy( _In_ FILETIME const& source ) noexcept
      {
         dwLowDateTime  = source.dwLowDateTime;
         dwHighDateTime = source.dwHighDateTime;
      }

      inline void Copy(_In_ SYSTEMTIME const * source ) noexcept
      {
         if ( source == nullptr )
         {
            Empty();
            return;
         }

         FILETIME file_time{ 0, 0 };

         // We were passed a pointer, don't trust it

         WFC_TRY
         {
            if ( ::SystemTimeToFileTime( source, &file_time ) != FALSE )
            {
               dwLowDateTime  = file_time.dwLowDateTime;
               dwHighDateTime = file_time.dwHighDateTime;
            }
            else
            {
               Empty();
            }
         }
         WFC_CATCH_ALL
         {
            Empty();
         }
         WFC_END_CATCH_ALL
      }
      
      inline void Copy(_In_ SYSTEMTIME const& source ) noexcept
      {
          FILETIME file_time{ 0, 0 };

         if ( ::SystemTimeToFileTime( &source, &file_time ) != FALSE )
         {
            dwLowDateTime  = file_time.dwLowDateTime;
            dwHighDateTime = file_time.dwHighDateTime;
         }
         else
         {
            Empty();
         }
      }

      inline void Copy(_In_ WORD ms_dos_date, _In_ WORD ms_dos_time ) noexcept
      {
          FILETIME file_time{ 0, 0 };

         if ( ::DosDateTimeToFileTime( ms_dos_date, ms_dos_time, &file_time ) != FALSE )
         {
            dwLowDateTime  = file_time.dwLowDateTime;
            dwHighDateTime = file_time.dwHighDateTime;
         }
         else
         {
            Empty();
         }
      }

      inline constexpr void Copy(_In_ LARGE_INTEGER const& source ) noexcept
      {
         dwLowDateTime  = source.LowPart;
         dwHighDateTime = source.HighPart;
      }

      inline constexpr void Copy( _In_ LARGE_INTEGER const * source ) noexcept
      {
         dwLowDateTime  = source->LowPart;
         dwHighDateTime = source->HighPart;
      }

      inline constexpr void Copy(_In_ ULARGE_INTEGER const& source ) noexcept
      {
         dwLowDateTime  = source.LowPart;
         dwHighDateTime = source.HighPart;
      }

      inline constexpr void CopyTicks( _In_ uint64_t const ticks ) noexcept
      {
          ULARGE_INTEGER li{ { 0, 0 } };

          li.QuadPart = ticks;

          dwLowDateTime  = li.LowPart;
          dwHighDateTime = li.HighPart;
      }

      inline constexpr void CopyTicks(_In_ int64_t const ticks ) noexcept
      {
          ULARGE_INTEGER li{ { 0, 0 } };

         li.QuadPart = static_cast<uint64_t>(ticks);
         dwLowDateTime  = li.LowPart;
         dwHighDateTime = li.HighPart;
      }

      inline constexpr void Copy(_In_ ULARGE_INTEGER const * source ) noexcept
      {
         dwLowDateTime  = source->LowPart;
         dwHighDateTime = source->HighPart;
      }

      inline void Copy(_In_ TIMESTAMP_STRUCT const * source ) noexcept
      {
          static_assert(offsetof(TIMESTAMP_STRUCT, year) == offsetof(DBTIMESTAMP, year), "The offset of the year member is different between the TIMESTAMP_STRUCT and DBTIMESTAMP structures.");
          static_assert(offsetof(TIMESTAMP_STRUCT, month) == offsetof(DBTIMESTAMP, month), "The offset of the month member is different between the TIMESTAMP_STRUCT and DBTIMESTAMP structures.");
          static_assert(offsetof(TIMESTAMP_STRUCT, day) == offsetof(DBTIMESTAMP, day), "The offset of the day member is different between the TIMESTAMP_STRUCT and DBTIMESTAMP structures.");
          static_assert(offsetof(TIMESTAMP_STRUCT, hour) == offsetof(DBTIMESTAMP, hour), "The offset of the hour member is different between the TIMESTAMP_STRUCT and DBTIMESTAMP structures.");
          static_assert(offsetof(TIMESTAMP_STRUCT, minute) == offsetof(DBTIMESTAMP, minute), "The offset of the minute member is different between the TIMESTAMP_STRUCT and DBTIMESTAMP structures.");
          static_assert(offsetof(TIMESTAMP_STRUCT, second) == offsetof(DBTIMESTAMP, second), "The offset of the second member is different between the TIMESTAMP_STRUCT and DBTIMESTAMP structures.");
          static_assert(offsetof(TIMESTAMP_STRUCT, fraction) == offsetof(DBTIMESTAMP, fraction), "The offset of the fraction member is different between the TIMESTAMP_STRUCT and DBTIMESTAMP structures.");
          Copy( reinterpret_cast<DBTIMESTAMP const *>(source) );
      }

      inline void Copy(_In_ TIMESTAMP_STRUCT const& source ) noexcept
      {
         Copy( reinterpret_cast<DBTIMESTAMP const *>(&source) );
      }

      inline void Copy(_In_ DBTIMESTAMP const * source ) noexcept
      {
         if ( source == nullptr )
         {
            Empty();
            return;
         }

         WFC_TRY
         {
            Copy( *source );
         }
         WFC_CATCH_ALL
         {
            Empty();
         }
         WFC_END_CATCH_ALL
      }

      inline void Copy( _In_ DBTIMESTAMP const& source ) noexcept
      {
          SYSTEMTIME system_time{0, 0, 0, 0, 0, 0, 0, 0};

         system_time.wYear   = source.year;
         system_time.wMonth  = source.month;
         system_time.wDay    = source.day;
         system_time.wHour   = source.hour;
         system_time.wMinute = source.minute;
         system_time.wSecond = source.second;
         system_time.wMilliseconds = 0;

         FILETIME file_time{ 0, 0 };

         if ( ::SystemTimeToFileTime( &system_time, &file_time ) == FALSE )
         {
            Empty();
            return;
         }

         // Now add the billionths of a second

         // Filetime is in 100-nanosecond increments so we have to convert

         DWORD const hundred_nanosecond_increments = source.fraction / 100;

         ULARGE_INTEGER large_integer{ file_time.dwLowDateTime, file_time.dwHighDateTime };

         large_integer.QuadPart += hundred_nanosecond_increments;

         dwLowDateTime  = large_integer.LowPart;
         dwHighDateTime = large_integer.HighPart;
      }

      inline constexpr void CopyUnixTime( _In_ time_t const unix_time ) noexcept
      {
         int64_t const ll = Int32x32To64(unix_time, CFileTime::NumberOfFiletimeTicksInOneSecond) + CFileTime::JanFirst1970;
         dwLowDateTime = static_cast<DWORD>(ll);
         dwHighDateTime = ll >> 32;
      }

      inline constexpr void CopyCOMDate(_In_ double const stupid_COM_date ) noexcept
      {
          ULARGE_INTEGER li{ 0, 0 };

          li.QuadPart = static_cast<uint64_t>(static_cast<double>(NumberOfFiletimeTicksInOneDay) * stupid_COM_date);
          li.QuadPart += DecThirty1899;

          dwHighDateTime = li.HighPart;
          dwLowDateTime = li.LowPart;
      }

      inline void Set(_In_ int const year, _In_ int const month, _In_ int const day, _In_ int const hour, _In_ int const minute, _In_ int const second, _In_ int const milliseconds = 0 ) noexcept
      {
         CSystemTime system_time;

         system_time.wYear = static_cast<WORD>(year);
         system_time.wMonth = static_cast<WORD>(month);
         system_time.wDay = static_cast<WORD>(day);
         system_time.wHour = static_cast<WORD>(hour);
         system_time.wMinute = static_cast<WORD>(minute);
         system_time.wSecond = static_cast<WORD>(second);
         system_time.wMilliseconds = static_cast<WORD>(milliseconds);

         Copy( system_time );
      }

      void Set(_In_ int const year, _In_ int const month, _In_ int const day, _In_ int const hour, _In_ int const minute, _In_ double const seconds ) noexcept
      {
         CTimeEx the_time( year, month, day, hour, minute, 0 );

         int64_t ll = Int32x32To64( static_cast<time_t>(the_time), CFileTime::NumberOfFiletimeTicksInOneSecond) + CFileTime::JanFirst1970;

         int64_t const number_of_ticks = static_cast<int64_t>( static_cast<double>(CFileTime::NumberOfFiletimeTicksInOneSecond) * seconds );

         ll += number_of_ticks;

         dwLowDateTime = static_cast<DWORD>(ll);
         dwHighDateTime = ll >> 32;
      }

      inline constexpr void CopyAppleNSDate( _In_ double const nsdate ) noexcept
      {
          ULARGE_INTEGER unsigned_large_integer{ { 0, 0 } };

          // 2018-04-06 - SRB - I hate Apple. They changed the generally accepted time unit of NSDate from one second to one nanosecond.

          if (nsdate > static_cast<double>(1144344598.0)) // Is this date after April 6, 2037?
          {
              // The unit of measurement is one nanosecond
              unsigned_large_integer.QuadPart = static_cast<uint64_t>(nsdate / static_cast<double>(CFileTime::NumberOfNanosecondsInOneFiletimeTick)) + JanFirst2001;
          }
          else
          {
              // The unit of measurement is one second
              unsigned_large_integer.QuadPart = static_cast<uint64_t>(nsdate * static_cast<double>(CFileTime::NumberOfFiletimeTicksInOneSecond)) + JanFirst2001;
          }

          dwHighDateTime = unsigned_large_integer.HighPart;
          dwLowDateTime = unsigned_large_integer.LowPart;
      }

      inline constexpr void AddTicks(_In_ int64_t const number_of_ticks ) noexcept
      {
          ULARGE_INTEGER unsigned_large_integer{ { dwLowDateTime, dwHighDateTime } };

          unsigned_large_integer.QuadPart += number_of_ticks;

          dwHighDateTime = unsigned_large_integer.HighPart;
          dwLowDateTime = unsigned_large_integer.LowPart;
      }

      inline constexpr void AddNanoseconds(_In_ int64_t const number_of_nanoseconds) noexcept
      {
          AddTicks(number_of_nanoseconds / NumberOfNanosecondsInOneFiletimeTick);
      }

      inline constexpr void CopyChrome(_In_ int64_t const number_of_microseconds_since_01_Jan_1601 ) noexcept
      {
          ULARGE_INTEGER unsigned_large_integer{ { 0, 0 } };

          unsigned_large_integer.QuadPart = number_of_microseconds_since_01_Jan_1601 * NumberOfFiletimeTicksInOneMicrosecond;

          dwHighDateTime = unsigned_large_integer.HighPart;
          dwLowDateTime = unsigned_large_integer.LowPart;
      }

      inline constexpr void CopyPRTime(_In_ uint64_t const pr_time ) noexcept
      {
          uint64_t integer = pr_time * NumberOfFiletimeTicksInOneMicrosecond;

          integer += JanFirst1970;

          dwLowDateTime = static_cast<DWORD>(integer);
          dwHighDateTime = integer >> 32;
      }

      inline constexpr void CopyUnixMillisecondTime(_In_ uint64_t const unix_milliseconds ) noexcept
      {
          ULARGE_INTEGER unsigned_large_integer{ { 0, 0 } };

          unsigned_large_integer.QuadPart = unix_milliseconds;
          unsigned_large_integer.QuadPart *= NumberOfTicksPerMillisecond;
          unsigned_large_integer.QuadPart += JanFirst1970;

          dwHighDateTime = unsigned_large_integer.HighPart;
          dwLowDateTime = unsigned_large_integer.LowPart;
      }

#if ! defined( WFC_STL )
      void CopyTo( COleDateTime& destination ) const noexcept;
#endif // WFC_STL

      inline void CopyTo( _Out_ CTime& destination ) const noexcept
      {
         CSystemTime system_time( this );
         system_time.CopyTo( destination );
      }

      inline void CopyTo( _Out_ DBTIMESTAMP&  destination ) const noexcept
      {
          SYSTEMTIME system_time{ 0, 0, 0, 0, 0, 0, 0, 0 };

         if ( ::FileTimeToSystemTime( this, &system_time ) == FALSE )
         {
            destination.day      = 0;
            destination.fraction = 0;
            destination.hour     = 0;
            destination.minute   = 0;
            destination.month    = 0;
            destination.second   = 0;
            destination.year     = 0;
            return;
         }

         destination.year   = system_time.wYear;
         destination.month  = system_time.wMonth;
         destination.day    = system_time.wDay;
         destination.hour   = system_time.wHour;
         destination.minute = system_time.wMinute;
         destination.second = system_time.wSecond;

         ULARGE_INTEGER const large_integer{ dwLowDateTime, dwHighDateTime };

         destination.fraction = ( static_cast<DWORD>( large_integer.QuadPart % 10000000 ) ) * 100;
      }

      inline void CopyTo( _Out_ TIMESTAMP_STRUCT& destination ) const noexcept
      {
         SYSTEMTIME system_time{ 0, 0, 0, 0, 0, 0, 0, 0 };

         if ( ::FileTimeToSystemTime( this, &system_time ) == FALSE )
         {
            destination.day      = 0;
            destination.fraction = 0;
            destination.hour     = 0;
            destination.minute   = 0;
            destination.month    = 0;
            destination.second   = 0;
            destination.year     = 0;
            return;
         }

         destination.year   = system_time.wYear;
         destination.month  = system_time.wMonth;
         destination.day    = system_time.wDay;
         destination.hour   = system_time.wHour;
         destination.minute = system_time.wMinute;
         destination.second = system_time.wSecond;

         ULARGE_INTEGER large_integer;

         large_integer.LowPart  = dwLowDateTime;
         large_integer.HighPart = dwHighDateTime;

         destination.fraction = ( static_cast<DWORD>( large_integer.QuadPart % 10000000 ) ) * 100;
      }

      inline void CopyTo( _Out_ WORD& ms_dos_date, _Out_ WORD& ms_dos_time ) const noexcept
      {
         // CoFileTimeToDosDateTime() is not const correct. To prevent
         // the children at Microsoft from harming us, we will copy
         // our FILETIME to an expendable one.

          FILETIME file_time{ 0, 0 };

         file_time.dwHighDateTime = dwHighDateTime;
         file_time.dwLowDateTime  = dwLowDateTime;

         if ( ::CoFileTimeToDosDateTime( &file_time, &ms_dos_date, &ms_dos_time ) == FALSE )
         {
            // The API failed.
            ms_dos_date = 0;
            ms_dos_time = 0;
         }
      }

      inline _Check_return_ time_t AsUnixTime( void ) const noexcept
      {
         CSystemTime system_time( this );

         CTime unix_time;

         system_time.CopyTo( unix_time );

         return( unix_time.GetTime() );
      }

      inline constexpr void CopyTo( _Out_ FILETIME& destination ) const noexcept
      {
         destination.dwLowDateTime  = dwLowDateTime;
         destination.dwHighDateTime = dwHighDateTime;
      }

      inline constexpr void Empty( void ) noexcept
      {
         dwLowDateTime  = 0;
         dwHighDateTime = 0;
      }

      inline constexpr _Check_return_ bool IsEmpty( void ) const noexcept
      {
         return( ( dwHighDateTime == 0 and dwLowDateTime == 0 ) ? true : false );
      }

      inline void FillWithCurrentTime( void ) noexcept
      {
         ::GetSystemTimePreciseAsFileTime( this );
      }

      _Check_return_ std::wstring SQLDate( void ) const noexcept;
      _Check_return_ std::wstring SQLTime( void ) const noexcept;
      _Check_return_ std::wstring SQLTimestamp( void ) const noexcept;
      _Check_return_ std::wstring PostgreSQLTimestamp( void ) const noexcept;

      inline _Check_return_ std::wstring ISO8601( void ) const noexcept
      {
          std::wstring return_value;

          ISO8601( return_value );

          return( return_value );
      }

      void ISO8601(_Out_ std::wstring& destination) const noexcept;

      // This gives you the FILETIME ticks as a sixty-four bit integer.
      inline constexpr _Check_return_ int64_t Ticks( void ) const noexcept
      {
         LARGE_INTEGER const return_value{ dwLowDateTime, static_cast<LONG>(dwHighDateTime) };

         return( return_value.QuadPart );
      }

      inline constexpr _Check_return_ uint64_t AsTicks(void) const noexcept
      {
          ULARGE_INTEGER const ul{ dwLowDateTime, dwHighDateTime };

          return(ul.QuadPart);
      }

      /*
      ** Operators
      */

      inline constexpr _Check_return_ bool operator == (_In_ CFileTime const& source ) const noexcept
      {
        return( Compare( source ) == I_AM_EQUAL_TO_THAT ? true : false);
      }

      inline constexpr _Check_return_ bool operator != (_In_ CFileTime const& source ) const noexcept
      {
        return( Compare( source ) == I_AM_EQUAL_TO_THAT ? false : true);
      }

      inline constexpr _Check_return_ bool operator < (_In_ CFileTime const& source ) const noexcept
      {
         return( Compare( source ) < I_AM_EQUAL_TO_THAT ? true : false);
      }

      inline constexpr _Check_return_ bool operator > (_In_ CFileTime const& source ) const noexcept
      {
         return( Compare( source ) > I_AM_EQUAL_TO_THAT ? true : false );
      }

      inline constexpr _Check_return_ CFileTime& operator = (_In_ CFileTime const& source ) noexcept
      {
         Copy( source );
         return( *this );
      }

      inline _Check_return_ CFileTime& operator = (_In_ CSystemTime const& source ) noexcept
      {
         Copy( source );
         return( *this );
      }

      inline _Check_return_ CFileTime& operator = (_In_ LARGE_INTEGER const& source ) noexcept
      {
         Copy( source );
         return( *this );
      }

      inline constexpr _Check_return_ CFileTime& operator = (_In_ ULARGE_INTEGER const& source ) noexcept
      {
         Copy( source );
         return( *this );
      }

      inline _Check_return_ CFileTime& operator = (_In_ DBTIMESTAMP const& source ) noexcept
      {
         Copy( source );
         return( *this );
      }

      inline _Check_return_ CFileTime& operator = ( _In_ TIMESTAMP_STRUCT const& source ) noexcept
      {
         Copy( source );
         return( *this );
      }

      inline constexpr operator LARGE_INTEGER() const noexcept
      {
          LARGE_INTEGER return_value{ dwLowDateTime, static_cast<LONG>(dwHighDateTime) };

         return( return_value );
      }

      inline constexpr operator ULARGE_INTEGER() const noexcept
      {
          ULARGE_INTEGER return_value{ dwLowDateTime, dwHighDateTime };

         return( return_value );
      }

#if ! defined( WFC_STL )
      operator COleDateTime() const noexcept;
#endif // WFC_STL

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};   

#endif // FILE_TIME_CLASS_HEADER
