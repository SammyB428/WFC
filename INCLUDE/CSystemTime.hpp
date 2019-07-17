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

      inline CSystemTime( __in CSystemTime const& source )
      {
         Copy( source );
      }

      inline CSystemTime( __in CFileTime const& source )
      {
         Copy( source );
      }

      inline CSystemTime( __in CTime const& source )
      {
         Copy( source );
      }

#if ! defined( WFC_STL )
      CSystemTime( _In_ COleDateTime const& source );
#endif // WFC_STL

      inline CSystemTime( __in SYSTEMTIME const * source )
      {
         Copy( source );
      }

      inline CSystemTime( __in SYSTEMTIME const& source )
      {
         Copy( source );
      }

      inline CSystemTime( __in FILETIME const * source )
      {
         Copy( source );
      }

      inline CSystemTime( __in FILETIME const& source )
      {
         Copy( source );
      }

      inline CSystemTime( __in TIME_OF_DAY_INFO const * source )
      {
         Copy( source );
      }

      inline CSystemTime( __in TIME_OF_DAY_INFO const& source )
      {
         Copy( source );
      }

      inline CSystemTime( __in TIMESTAMP_STRUCT const * source ) // from sqltypes.h
      {
         Copy( source );
      }

      inline CSystemTime( __in TIMESTAMP_STRUCT const& source ) // from sqltypes.h
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

         int day_of_week = now.GetDayOfWeek(); // 1 == Sunday, 2  == Monday

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

      _Check_return_ LONG Compare( __in CSystemTime const& source ) const noexcept;

      inline constexpr void Copy( __in CSystemTime const& source ) noexcept
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

      void Copy( __in Win32FoundationClasses::CFileTime const& source ) noexcept;

      void Copy( __in CTime const& source ) noexcept;

#if ! defined( WFC_STL )
      void Copy( COleDateTime const&      source ) noexcept;
#endif // WFC_STL

      inline constexpr void Copy( __in SYSTEMTIME const * source ) noexcept
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

      inline constexpr void Copy( __in SYSTEMTIME const& source ) noexcept
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

      void Copy( __in FILETIME const * source ) noexcept;

      inline void Copy( __in FILETIME const& source ) noexcept
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

      void Copy( __in TIME_OF_DAY_INFO const * source ) noexcept;
      void Copy( __in TIME_OF_DAY_INFO const&  source ) noexcept;
      void Copy( __in TIMESTAMP_STRUCT const * source ) noexcept; // from sqltypes.h
      void Copy( __in TIMESTAMP_STRUCT const&  source ) noexcept; // from sqltypes.h

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

      _Check_return_ uint32_t NumberOfMinutesSinceMonday( void ) const noexcept;
      _Check_return_ BOOL Set( void ) const noexcept;

      inline _Check_return_ int64_t Ticks( void ) const noexcept
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

      inline constexpr _Check_return_ CSystemTime& operator = ( __in CSystemTime const& source ) noexcept
      {
         Copy( source );
         return( *this );
      }

      _Check_return_ CSystemTime& operator = ( __in CFileTime const&    source ) noexcept;
      _Check_return_ CSystemTime& operator = ( __in CTime const&        source ) noexcept;

#if ! defined( WFC_STL )
      _Check_return_ CSystemTime& operator = ( __in COleDateTime const& source ) noexcept;
#endif // WFC_STL

      _Check_return_ bool operator == ( __in CSystemTime const& source ) const noexcept;
      _Check_return_ bool operator != ( __in CSystemTime const& source ) const noexcept;
      _Check_return_ bool operator >  ( __in CSystemTime const& source ) const noexcept;
      _Check_return_ bool operator <  ( __in CSystemTime const& source ) const noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG

      _Check_return_ FILETIME AsFiletime() const noexcept;
};

#endif // SYSTEM_TIME_CLASS_HEADER
