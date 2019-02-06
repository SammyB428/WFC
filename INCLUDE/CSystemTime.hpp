/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2016, Samuel R. Blackburn
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
** $Workfile: CSystemTime.hpp $
** $Revision: 17 $
** $Modtime: 6/26/01 11:04a $
*/

#if ! defined ( SYSTEM_TIME_CLASS_HEADER )

#define SYSTEM_TIME_CLASS_HEADER

// Thanks go to Dean Grimm (dean@cortron.com) for suggesting
// the SQL date/time types in CSystemTime

class CFileTime;

#define NUMBER_OF_MINUTES_IN_ONE_DAY (1440)
#define NUMBER_OF_MINUTES_IN_ONE_HOUR (60)

class CSystemTime : public _SYSTEMTIME
{
   public:

      inline CSystemTime()
      {
         Empty();
      }

      inline CSystemTime( __in const CSystemTime& source )
      {
         Copy( source );
      }

      inline CSystemTime( __in const CFileTime& source )
      {
         Copy( source );
      }

      inline CSystemTime( __in const CTime& source )
      {
         Copy( source );
      }

#if ! defined( WFC_STL )
      CSystemTime( const COleDateTime& source );
#endif // WFC_STL

      inline CSystemTime( __in const SYSTEMTIME * source )
      {
         Copy( source );
      }

      inline CSystemTime( __in const SYSTEMTIME& source )
      {
         Copy( source );
      }

      inline CSystemTime( __in const FILETIME * source )
      {
         Copy( source );
      }

      inline CSystemTime( __in const FILETIME& source )
      {
         Copy( source );
      }

      inline CSystemTime( __in const TIME_OF_DAY_INFO * source )
      {
         Copy( source );
      }

      inline CSystemTime( __in const TIME_OF_DAY_INFO& source )
      {
         Copy( source );
      }

      inline CSystemTime( __in const TIMESTAMP_STRUCT * source ) // from sqltypes.h
      {
         Copy( source );
      }

      inline CSystemTime( __in const TIMESTAMP_STRUCT& source ) // from sqltypes.h
      {
         Copy( source );
      }

      inline virtual ~CSystemTime()
      {
         Empty();
      }

      /*
      ** Unfortunately, I can't call this GetCurrentTime (like in CTime)
      ** because Microsoft #define's it in winbase.h and it royally screws me.
      */

      inline static __checkReturn CSystemTime GetCurrentUTCTime( void ) noexcept
      {
         SYSTEMTIME system_time;

         ::GetSystemTime( &system_time );

         return( CSystemTime( &system_time ) );
      }

      inline static __checkReturn CSystemTime GetCurrentLocalTime( void ) noexcept
      {
         SYSTEMTIME system_time;

         ::GetLocalTime( &system_time );

         return( CSystemTime( &system_time ) );
      }

      inline static __checkReturn uint32_t GetMinutesSinceMondayUTC( void ) noexcept
      {
         SYSTEMTIME system_time;

         ::GetSystemTime( &system_time );

         const CTimeEx now( system_time.wYear, system_time.wMonth, system_time.wDay, system_time.wHour, system_time.wMinute, system_time.wSecond );

         int day_of_week = now.GetDayOfWeek(); // 1 == Sunday, 2  == Monday

         int days_from_monday = day_of_week - 2;

         if ( days_from_monday < 0 )
         {
            days_from_monday += 7;
         }

         const uint32_t return_value = (days_from_monday * NUMBER_OF_MINUTES_IN_ONE_DAY) + (system_time.wHour * 60) + system_time.wMinute;

         return( return_value );
      }

      inline static __checkReturn uint32_t GetMinutesSinceMondayLocal( void ) noexcept
      {
         SYSTEMTIME system_time;

         ::GetLocalTime( &system_time );

         const CTimeEx now( system_time.wYear, system_time.wMonth, system_time.wDay, system_time.wHour, system_time.wMinute, system_time.wSecond );

         int day_of_week = now.GetDayOfWeek(); // 1 == Sunday, 2  == Monday

         int days_from_monday = day_of_week - 2;

         if ( days_from_monday < 0 )
         {
            days_from_monday += 7;
         }

         const uint32_t return_value = (days_from_monday * NUMBER_OF_MINUTES_IN_ONE_DAY) + (system_time.wHour * 60) + system_time.wMinute;

         return( return_value );
      }

      inline constexpr void Empty( void ) noexcept
      {
         WFC_VALIDATE_POINTER( this );

         wYear         = 0;
         wMonth        = 0;
         wDay          = 0;
         wDayOfWeek    = 0;
         wHour         = 0;
         wMinute       = 0;
         wSecond       = 0;
         wMilliseconds = 0;
      }

      inline constexpr _Check_return_ bool IsEmpty(void) const noexcept
      {
          if (wYear != 0 ||
              wMonth != 0 ||
              wDay != 0 ||
              wHour != 0 ||
              wMinute != 0 ||
              wSecond != 0 ||
              wMilliseconds != 0)
          {
              return(false);
          }

          return(true);
      }

      __checkReturn LONG Compare( __in const CSystemTime& source ) const noexcept;

      inline constexpr void Copy( __in const CSystemTime& source ) noexcept
      {
         wYear         = source.wYear;
         wMonth        = source.wMonth;
         wDay          = source.wDay;
         wDayOfWeek    = source.wDayOfWeek;
         wHour         = source.wHour;
         wMinute       = source.wMinute;
         wSecond       = source.wSecond;
         wMilliseconds = source.wMilliseconds;
      }

      void Copy( __in const Win32FoundationClasses::CFileTime& source ) noexcept;

      void Copy( __in const CTime& source ) noexcept;

#if ! defined( WFC_STL )
      void Copy( const COleDateTime&      source ) noexcept;
#endif // WFC_STL

      inline constexpr void Copy( __in const SYSTEMTIME * source ) noexcept
      {
         wYear         = source->wYear;
         wMonth        = source->wMonth;
         wDay          = source->wDay;
         wDayOfWeek    = source->wDayOfWeek;
         wHour         = source->wHour;
         wMinute       = source->wMinute;
         wSecond       = source->wSecond;
         wMilliseconds = source->wMilliseconds;
      }

      inline constexpr void Copy( __in const SYSTEMTIME& source ) noexcept
      {
         wYear         = source.wYear;
         wMonth        = source.wMonth;
         wDay          = source.wDay;
         wDayOfWeek    = source.wDayOfWeek;
         wHour         = source.wHour;
         wMinute       = source.wMinute;
         wSecond       = source.wSecond;
         wMilliseconds = source.wMilliseconds;
      }

      void Copy( __in const FILETIME * source ) noexcept;

      inline void Copy( __in const FILETIME& source ) noexcept
      {
         WFC_VALIDATE_POINTER( this );

         SYSTEMTIME system_time;

         if ( ::FileTimeToSystemTime( &source, &system_time ) != FALSE )
         {
            Copy( system_time );
         }
         else
         {
            Empty();
         }
      }

      void Copy( __in const TIME_OF_DAY_INFO * source ) noexcept;
      void Copy( __in const TIME_OF_DAY_INFO&  source ) noexcept;
      void Copy( __in const TIMESTAMP_STRUCT * source ) noexcept; // from sqltypes.h
      void Copy( __in const TIMESTAMP_STRUCT&  source ) noexcept; // from sqltypes.h

#if ! defined( WFC_STL )
      virtual void CopyTo( COleDateTime&      destination ) const noexcept;
#endif // WFC_STL

      void CopyTo( __out CTime&             destination ) const noexcept;
      void CopyTo( __out TIMESTAMP_STRUCT * destination ) const noexcept;
      void CopyTo( __out TIMESTAMP_STRUCT&  destination ) const noexcept;
      void CopyTo( __out double&            destination ) const noexcept;
      void CopyTo( __out SYSTEMTIME&        destination ) const noexcept;

      inline void GetUTC( void ) noexcept
      {
         WFC_VALIDATE_POINTER( this );
         Copy( GetCurrentUTCTime() );
      }

      inline void GetLocal( void ) noexcept
      {
         WFC_VALIDATE_POINTER( this );
         Copy( GetCurrentLocalTime() );
      }

      __checkReturn uint32_t NumberOfMinutesSinceMonday( void ) const noexcept;
      __checkReturn BOOL Set( void ) const noexcept;

      inline __checkReturn int64_t Ticks( void ) const noexcept
      {
          FILETIME ft = { 0, 0 };

          if (::SystemTimeToFileTime(this, &ft) == FALSE)
          {
              return(0);
          }

         LARGE_INTEGER lt;

         lt.HighPart = (LONG) ft.dwHighDateTime;
         lt.LowPart = ft.dwLowDateTime;

         return( lt.QuadPart );
      }

      /*
      ** Operators
      */

      inline constexpr __checkReturn CSystemTime& operator = ( __in const CSystemTime&  source ) noexcept
      {
         Copy( source );
         return( *this );
      }

      __checkReturn CSystemTime& operator = ( __in const CFileTime&    source ) noexcept;
      __checkReturn CSystemTime& operator = ( __in const CTime&        source ) noexcept;

#if ! defined( WFC_STL )
      __checkReturn CSystemTime& operator = ( __in const COleDateTime& source ) noexcept;
#endif // WFC_STL

      __checkReturn bool operator == ( __in const CSystemTime& source ) const noexcept;
      __checkReturn bool operator != ( __in const CSystemTime& source ) const noexcept;
      __checkReturn bool operator >  ( __in const CSystemTime& source ) const noexcept;
      __checkReturn bool operator <  ( __in const CSystemTime& source ) const noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG

      __checkReturn FILETIME AsFiletime() const noexcept;
};

#endif // SYSTEM_TIME_CLASS_HEADER
