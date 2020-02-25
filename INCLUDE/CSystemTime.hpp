/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2020, Samuel R. Blackburn
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

/* SPDX-License-Identifier: BSD-2-Clause */

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

      inline constexpr CSystemTime() noexcept : _SYSTEMTIME{0, 0, 0, 0, 0, 0, 0, 0} {}

      inline CSystemTime( _In_ CSystemTime const& source ) noexcept
      {
         Copy( source );
      }

      inline CSystemTime( _In_ CFileTime const& source ) noexcept
      {
         Copy( source );
      }

#if ! defined(WE_ARE_BUILDING_WFC_ON_UNIX)
      inline CSystemTime( _In_ CTime const& source ) noexcept
      {
         Copy( source );
      }
#endif

#if ! defined( WFC_STL )
      CSystemTime( _In_ COleDateTime const& source ) noexcept;
#endif // WFC_STL

      inline CSystemTime( _In_ SYSTEMTIME const * source ) noexcept
      {
         Copy( source );
      }

      inline CSystemTime( _In_ SYSTEMTIME const& source ) noexcept
      {
         Copy( source );
      }

      inline CSystemTime( _In_ FILETIME const * source ) noexcept
      {
         Copy( source );
      }

      inline CSystemTime( _In_ FILETIME const& source ) noexcept
      {
         Copy( source );
      }

      inline CSystemTime( _In_ TIME_OF_DAY_INFO const * source ) noexcept
      {
         Copy( source );
      }

      inline CSystemTime( _In_ TIME_OF_DAY_INFO const& source ) noexcept
      {
         Copy( source );
      }

      inline CSystemTime( _In_ TIMESTAMP_STRUCT const * source ) noexcept // from sqltypes.h
      {
         Copy( source );
      }

      inline CSystemTime( _In_ TIMESTAMP_STRUCT const& source ) noexcept // from sqltypes.h
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

      inline static _Check_return_ CSystemTime GetCurrentUTCTime( void ) noexcept
      {
         SYSTEMTIME system_time;

         ::GetSystemTime( &system_time );

         return( CSystemTime( &system_time ) );
      }

      inline static _Check_return_ CSystemTime GetCurrentLocalTime( void ) noexcept
      {
         SYSTEMTIME system_time;

         ::GetLocalTime( &system_time );

         return( CSystemTime( &system_time ) );
      }

      inline static _Check_return_ uint32_t GetMinutesSinceMondayUTC( void ) noexcept
      {
         SYSTEMTIME system_time;

         ::GetSystemTime( &system_time );

         CTimeEx const now( system_time.wYear, system_time.wMonth, system_time.wDay, system_time.wHour, system_time.wMinute, system_time.wSecond );

         int const day_of_week = now.GetDayOfWeek(); // 1 == Sunday, 2  == Monday

         int days_from_monday = day_of_week - 2;

         if ( days_from_monday < 0 )
         {
            days_from_monday += 7;
         }

         uint32_t const return_value = (days_from_monday * NUMBER_OF_MINUTES_IN_ONE_DAY) + (system_time.wHour * 60) + system_time.wMinute;

         return( return_value );
      }

      inline static _Check_return_ uint32_t GetMinutesSinceMondayLocal( void ) noexcept
      {
         SYSTEMTIME system_time;

         ::GetLocalTime( &system_time );

         CTimeEx const now( system_time.wYear, system_time.wMonth, system_time.wDay, system_time.wHour, system_time.wMinute, system_time.wSecond );

         int const day_of_week = now.GetDayOfWeek(); // 1 == Sunday, 2  == Monday

         int days_from_monday = day_of_week - 2;

         if ( days_from_monday < 0 )
         {
            days_from_monday += 7;
         }

         uint32_t const return_value = (days_from_monday * NUMBER_OF_MINUTES_IN_ONE_DAY) + (system_time.wHour * 60) + system_time.wMinute;

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
          if (wYear != 0 or
              wMonth != 0 or
              wDay != 0 or
              wHour != 0 or
              wMinute != 0 or
              wSecond != 0 or
              wMilliseconds != 0)
          {
              return(false);
          }

          return(true);
      }

      inline constexpr _Check_return_ int Compare(_In_ CSystemTime const& source) const noexcept
      {
          if (wYear < source.wYear)
          {
              return(I_AM_LESS_THAN_THAT);
          }

          if (wYear > source.wYear)
          {
              return(I_AM_GREATER_THAN_THAT);
          }

          if (wMonth < source.wMonth)
          {
              return(I_AM_LESS_THAN_THAT);
          }

          if (wMonth > source.wMonth)
          {
              return(I_AM_GREATER_THAN_THAT);
          }

          if (wDay < source.wDay)
          {
              return(I_AM_LESS_THAN_THAT);
          }

          if (wDay > source.wDay)
          {
              return(I_AM_GREATER_THAN_THAT);
          }

          if (wHour < source.wHour)
          {
              return(I_AM_LESS_THAN_THAT);
          }

          if (wHour > source.wHour)
          {
              return(I_AM_GREATER_THAN_THAT);
          }

          if (wMinute < source.wMinute)
          {
              return(I_AM_LESS_THAN_THAT);
          }

          if (wMinute > source.wMinute)
          {
              return(I_AM_GREATER_THAN_THAT);
          }

          if (wSecond < source.wSecond)
          {
              return(I_AM_LESS_THAN_THAT);
          }

          if (wSecond > source.wSecond)
          {
              return(I_AM_GREATER_THAN_THAT);
          }

          if (wMilliseconds < source.wMilliseconds)
          {
              return(I_AM_LESS_THAN_THAT);
          }

          if (wMilliseconds > source.wMilliseconds)
          {
              return(I_AM_GREATER_THAN_THAT);
          }

          return(I_AM_EQUAL_TO_THAT);
      }

      inline constexpr void Copy( _In_ CSystemTime const& source ) noexcept
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

      void Copy( _In_ CFileTime const& source ) noexcept;

#if ! defined(WE_ARE_BUILDING_WFC_ON_UNIX)
      void Copy( _In_ CTime const& source ) noexcept;
#endif // WE_ARE_BUILDING_WFC_ON_UNIX

#if ! defined( WFC_STL )
      void Copy( COleDateTime const&      source ) noexcept;
#endif // WFC_STL

      inline constexpr void Copy( _In_ SYSTEMTIME const * source ) noexcept
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

      inline constexpr void Copy( _In_ SYSTEMTIME const& source ) noexcept
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

      void Copy( _In_ FILETIME const * source ) noexcept;

      inline void Copy( _In_ FILETIME const& source ) noexcept
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

      void Copy( _In_ TIME_OF_DAY_INFO const * source ) noexcept;
      void Copy( _In_ TIME_OF_DAY_INFO const&  source ) noexcept;
      void Copy( _In_ TIMESTAMP_STRUCT const * source ) noexcept; // from sqltypes.h
      void Copy( _In_ TIMESTAMP_STRUCT const&  source ) noexcept; // from sqltypes.h

#if ! defined( WFC_STL )
      virtual void CopyTo( COleDateTime&      destination ) const noexcept;
#endif // WFC_STL

#if ! defined(WE_ARE_BUILDING_WFC_ON_UNIX)
      void CopyTo( _Out_ CTime&             destination ) const noexcept;
#endif
      void CopyTo( _Out_ TIMESTAMP_STRUCT * destination ) const noexcept;
      void CopyTo( _Out_ TIMESTAMP_STRUCT&  destination ) const noexcept;
#if ! defined( WE_ARE_BUILDING_WFC_ON_UNIX )
      void CopyTo( _Out_ double&            destination ) const noexcept;
#endif // WE_ARE_BUILDING_WFC_ON_UNIX
      void CopyTo( _Out_ SYSTEMTIME&        destination ) const noexcept;

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

      _Check_return_ uint32_t NumberOfMinutesSinceMonday( void ) const noexcept;
#if ! defined(WE_ARE_BUILDING_WFC_ON_UNIX)
      _Check_return_ bool Set( void ) const noexcept;
#endif
      inline _Check_return_ int64_t Ticks( void ) const noexcept
      {
          FILETIME ft = { 0, 0 };

          if (::SystemTimeToFileTime(this, &ft) == FALSE)
          {
              return(0);
          }

         LARGE_INTEGER lt;

         lt.u.HighPart = (long) ft.dwHighDateTime;
         lt.u.LowPart = ft.dwLowDateTime;

         return( lt.QuadPart );
      }

      /*
      ** Operators
      */

      inline constexpr _Check_return_ CSystemTime& operator = ( _In_ CSystemTime const& source ) noexcept
      {
         Copy( source );
         return( *this );
      }

      _Check_return_ CSystemTime& operator = ( _In_ CFileTime const&    source ) noexcept;
#if ! defined(WE_ARE_BUILDING_WFC_ON_UNIX)
      _Check_return_ CSystemTime& operator = ( _In_ CTime const&        source ) noexcept;
#endif
#if ! defined( WFC_STL )
      _Check_return_ CSystemTime& operator = ( _In_ COleDateTime const& source ) noexcept;
#endif // WFC_STL

      _Check_return_ bool operator == ( _In_ CSystemTime const& source ) const noexcept;
      _Check_return_ bool operator != ( _In_ CSystemTime const& source ) const noexcept;
      _Check_return_ bool operator >  ( _In_ CSystemTime const& source ) const noexcept;
      _Check_return_ bool operator <  ( _In_ CSystemTime const& source ) const noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG

      _Check_return_ FILETIME AsFiletime() const noexcept;
};

#endif // SYSTEM_TIME_CLASS_HEADER
