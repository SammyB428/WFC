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

#if ! defined( TIME_EX_CLASS_HEADER_FILE )

#define TIME_EX_CLASS_HEADER_FILE

#if defined( WFC_STL )

class CTimeSpan
{
   protected:

       time_t m_NumberOfSeconds{ 0 };

   public:

      inline constexpr CTimeSpan() noexcept { m_NumberOfSeconds = 0; }
      inline constexpr explicit CTimeSpan( _In_ unsigned long const number_of_seconds ) noexcept { m_NumberOfSeconds = number_of_seconds; }
      inline explicit  CTimeSpan( _In_ unsigned long const days, _In_ unsigned long const hours, _In_ unsigned long const minutes, _In_ unsigned long const number_of_seconds ) noexcept
      {
         m_NumberOfSeconds  = number_of_seconds;
         m_NumberOfSeconds += ( minutes * 60L );
         m_NumberOfSeconds += ( 60L * 60L * hours );
         m_NumberOfSeconds += ( 24L * 60L * 60L * days );
      }

      inline constexpr explicit CTimeSpan( _In_ CTimeSpan const& source ) noexcept { m_NumberOfSeconds = source.m_NumberOfSeconds; }

      inline constexpr void Empty( void ) noexcept { m_NumberOfSeconds = 0; }

      inline constexpr [[nodiscard]] _Check_return_ long    GetTotalHours( void ) const noexcept { return( (long)( m_NumberOfSeconds / 3600L ) ); }
      inline constexpr [[nodiscard]] _Check_return_ long    GetTotalMinutes( void ) const noexcept { return( (long) (m_NumberOfSeconds / 60L ) ); }
      inline constexpr [[nodiscard]] _Check_return_ time_t  GetTotalSeconds( void ) const noexcept { return( m_NumberOfSeconds ); }
      inline constexpr [[nodiscard]] _Check_return_ int64_t GetTotalTicks( void ) const noexcept { return( (int64_t) ( (int64_t) m_NumberOfSeconds * static_cast<int64_t>(10000000)) ); }
      inline constexpr [[nodiscard]] _Check_return_ long    GetDays( void ) const noexcept { return( (long) ( m_NumberOfSeconds / 86400L ) ); }
      inline constexpr [[nodiscard]] _Check_return_ long    GetHours( void ) const noexcept { return( GetTotalHours() - ( GetDays() * 24L ) ); }
      inline constexpr [[nodiscard]] _Check_return_ long    GetMinutes( void ) const noexcept { return( GetTotalMinutes() - ( GetTotalHours() * 60L ) ); }
      inline constexpr [[nodiscard]] _Check_return_ long    GetSeconds( void ) const noexcept { return( (long) ( GetTotalSeconds() - ( GetTotalMinutes() * 60L ) ) ); }
      inline constexpr               void    SetTicks( _In_ uint64_t const ticks ) noexcept { m_NumberOfSeconds = (time_t) ( ticks / 10000000); }
      inline _Check_return_ CTimeSpan operator-( _In_ CTimeSpan const& source ) const noexcept { return( CTimeSpan( (unsigned long) ( m_NumberOfSeconds - source.m_NumberOfSeconds ) ) ); }
      inline _Check_return_ CTimeSpan operator+( _In_ CTimeSpan const& source ) const noexcept { return( CTimeSpan( (unsigned long) ( m_NumberOfSeconds + source.m_NumberOfSeconds ) ) ); }
      inline constexpr [[nodiscard]] _Check_return_ CTimeSpan const& operator+=( _In_ CTimeSpan const& source ) noexcept { m_NumberOfSeconds += source.m_NumberOfSeconds; return( *this ); }
      inline constexpr [[nodiscard]] _Check_return_ CTimeSpan const& operator-=( _In_ CTimeSpan const& source ) noexcept { m_NumberOfSeconds -= source.m_NumberOfSeconds; return( *this ); }
      inline constexpr [[nodiscard]] _Check_return_ bool operator == ( _In_ CTimeSpan const& source ) const noexcept { return( m_NumberOfSeconds == source.m_NumberOfSeconds ); }
      inline constexpr [[nodiscard]] _Check_return_ bool operator != ( _In_ CTimeSpan const& source ) const noexcept { return( m_NumberOfSeconds != source.m_NumberOfSeconds ); }
      inline constexpr [[nodiscard]] _Check_return_ bool operator < ( _In_ CTimeSpan const& source ) const noexcept  { return( m_NumberOfSeconds <  source.m_NumberOfSeconds ); }
      inline constexpr [[nodiscard]] _Check_return_ bool operator > ( _In_ CTimeSpan const& source ) const noexcept  { return( m_NumberOfSeconds >  source.m_NumberOfSeconds ); }
      inline constexpr [[nodiscard]] _Check_return_ bool operator <= ( _In_ CTimeSpan const& source ) const noexcept { return( m_NumberOfSeconds <= source.m_NumberOfSeconds ); }
      inline constexpr [[nodiscard]] _Check_return_ bool operator >= ( _In_ CTimeSpan const& source ) const noexcept { return( m_NumberOfSeconds >= source.m_NumberOfSeconds ); }

      _Check_return_ std::wstring Format( _In_z_ wchar_t const * format_string ) const noexcept;
};

#endif // WFC_STL

class CTimeEx
{
   protected:

       time_t m_Time{ 0 };

      _Check_return_ time_t m_Make_time_t( _In_ struct tm const * time_p ) noexcept;

   public:

      // Construction

       inline constexpr CTimeEx() noexcept { m_Time = 0; };
       inline constexpr explicit CTimeEx(_In_ CTimeEx const& source) noexcept { Copy(source); }
       inline constexpr explicit CTimeEx(_In_ CTimeEx const* source) noexcept { Copy(source); }
       inline constexpr explicit CTimeEx(_In_ time_t const source) noexcept { m_Time = source; }
       explicit CTimeEx( _In_ struct tm const * source ) noexcept;
       explicit CTimeEx( _In_ struct tm const&  source ) noexcept;
       explicit CTimeEx( _In_ int const year, _In_ int const month, _In_ int const day, _In_ int const hour, _In_ int const minute, _In_ int const second, _In_ int const daylight_savings_time = -1 ) noexcept;
      inline constexpr explicit CTimeEx(_In_ FILETIME const& source) noexcept { Copy(source); }
      inline constexpr explicit CTimeEx(_In_ FILETIME const* source) noexcept { Copy(source); }

      // Methods

      static void GetCurrentTheTime( _Out_ CTimeEx& source ) noexcept;
      static void GreenwichMeanTime( _In_ time_t const * address_of_a_time_t, _Out_ struct tm * address_of_a_tm_structure ) noexcept;

      inline constexpr [[nodiscard]] _Check_return_ int Compare(_In_ CTimeEx const& source) const noexcept
      {
          if (m_Time > source.m_Time)
          {
              // We are greater than source

              return(I_AM_GREATER_THAN_THAT);
          }

          if (m_Time < source.m_Time)
          {
              // We are less than source

              return(I_AM_LESS_THAN_THAT);
          }

          // We could be equal to source, need to go to the nanosecond level

          if (m_Time > source.m_Time)
          {
              // We are greater than source

              return(I_AM_GREATER_THAN_THAT);
          }

          if (m_Time < source.m_Time)
          {
              return(I_AM_LESS_THAN_THAT);
          }

          // Welp, it looks like we're equal

          return(I_AM_EQUAL_TO_THAT);
      }

      inline constexpr void Copy(_In_ CTimeEx const& source) noexcept
      {
          m_Time = source.m_Time;
      }

      inline constexpr void Copy(_In_ CTimeEx const* source) noexcept
      {
          if (source not_eq nullptr)
          {
              m_Time = source->m_Time;
          }
          else
          {
              m_Time = 0;
          }
      }

      inline constexpr void Copy(_In_ time_t const source) noexcept
      {
          m_Time = source;
      }

      void Copy(_In_ struct tm const * source ) noexcept;
      inline void Copy(_In_ struct tm const& source) noexcept { Copy(&source); }
      
      inline constexpr void Copy(_In_ FILETIME const& source) noexcept
      {
          int64_t ll{ source.dwHighDateTime };

          ll <<= 32;
          ll += source.dwLowDateTime;
          ll -= 116444736000000000ULL; // The number of FILETIME ticks since Jan 1, 1970

          m_Time = static_cast<time_t>(ll / 10000000ULL); // The number of FILETIME ticks in one second
      }

      inline constexpr void Copy(_In_ FILETIME const * source) noexcept
      {
          if (source not_eq nullptr)
          {
              Copy(*source);
          }
          else
          {
              m_Time = 0;
          }
      }

      void   CopyModifiedJulianDate( _In_ double const number_of_days_since_17_november_1858) noexcept;
      inline constexpr void CopyTo(_Out_ time_t& destination) const noexcept { destination = m_Time; }
      void   CopyTo( _Out_ struct tm& destination ) const noexcept;
      void   CopyTo(_Out_ std::wstring& iso_8601_date) const noexcept;
      inline constexpr void Empty(void) noexcept { m_Time = 0; };
      void   FileNameFormat(_Out_ std::wstring& out_string) const noexcept;
      _Check_return_ std::wstring Format( _In_z_ wchar_t const * format_string ) const noexcept; // same as strftime
      _Check_return_ int    GetDay( void ) const noexcept;
      _Check_return_ int    GetDayOfWeek( void ) const noexcept;
      _Check_return_ int    GetDayOfYear( void ) const noexcept;
      _Check_return_ int    GetHour( void ) const noexcept;
      _Check_return_ int    GetMinute( void ) const noexcept;
      _Check_return_ int    GetMinuteOfDay( void ) const noexcept;
      _Check_return_ int    GetMonth( void ) const noexcept;
      _Check_return_ int    GetSecond( void ) const noexcept;
      void   GetTime( _Out_ struct tm& time_structure ) const noexcept;
      _Check_return_ time_t GetTotalSeconds( void ) const noexcept;
      _Check_return_ int    GetYear( void ) const noexcept;

      inline constexpr void AddSeconds( _In_ time_t const seconds ) noexcept
      {
          m_Time += seconds;
      }

      inline constexpr void SubtractSeconds( _In_ time_t const seconds ) noexcept
      {
          m_Time -= seconds;
      }

      inline _Check_return_ bool IsAfter( _In_ CTimeEx const& when ) const noexcept
      {
          if ( Compare( when ) > 0 )
          {
              return( true );
          }

          return( false );
      }

      inline _Check_return_ bool IsBefore( _In_ CTimeEx const& when ) const noexcept
      {
          if ( Compare( when ) < 0 )
          {
              return( true );
          }

          return( false );
      }

      void Now( void ) noexcept;
      void Set(_In_ std::wstring const& iso_8601_date ) noexcept;
      void Set(_In_ int year, _In_ int month, _In_ int day, _In_ int hour, _In_ int minute, _In_ int second, _In_ int daylight_savings_time = -1 ) noexcept;

      // Operators

      inline constexpr _Check_return_ operator time_t      ( void           )      const noexcept { return( m_Time ); };
      inline constexpr _Check_return_ CTimeEx&  operator =      ( _In_ CTimeEx const&    source )       noexcept { Copy( source ); return( *this ); };
      inline constexpr _Check_return_ CTimeEx&  operator =          ( _In_ time_t const      source )        noexcept { Copy( source ); return( *this ); };
      inline _Check_return_       CTimeEx&  operator =          ( _In_ struct tm const&  source )       noexcept { Copy( source ); return( *this ); };
      inline constexpr _Check_return_ bool      operator ==         ( _In_ CTimeEx const&    source ) const noexcept { return( ( Compare( source ) == I_AM_EQUAL_TO_THAT ) ? true : false ); };
      inline constexpr _Check_return_ bool      operator !=         ( _In_ CTimeEx const&    source ) const noexcept { return( ( Compare( source ) == I_AM_EQUAL_TO_THAT ) ? false : true); };
      inline constexpr _Check_return_ bool      operator >=         ( _In_ CTimeEx const&    source ) const noexcept { return( ( Compare( source ) > (-1) ) ? true : false); };
      inline constexpr _Check_return_ bool      operator <=         ( _In_ CTimeEx const&    source ) const noexcept { return( ( Compare( source ) < 1    ) ? true : false); };
      inline constexpr _Check_return_ bool      operator >          ( _In_ CTimeEx const&    source ) const noexcept { return( ( Compare( source ) > I_AM_EQUAL_TO_THAT) ? true : false); };
      inline constexpr _Check_return_ bool      operator <          ( _In_ CTimeEx const&    source ) const noexcept { return( ( Compare( source ) < I_AM_EQUAL_TO_THAT) ? true : false); };
      inline _Check_return_       CTimeSpan operator -          ( _In_ CTimeEx const&    source ) const noexcept { return( CTimeSpan( (unsigned long) ( m_Time - source.m_Time ) ) ); };
      inline _Check_return_       CTimeEx   operator -          ( _In_ CTimeSpan const&  source ) const noexcept { return( CTimeEx( m_Time - source.GetTotalSeconds() ) ); };
      inline _Check_return_       CTimeEx   operator +          ( _In_ CTimeSpan const&  source ) const noexcept { return( CTimeEx( m_Time + source.GetTotalSeconds() ) ); };
      inline _Check_return_ CTimeEx const&  operator +=         ( _In_ CTimeSpan const&  source )       noexcept { m_Time += source.GetTotalSeconds(); return( *this ); };
      inline _Check_return_ CTimeEx const&  operator -=         ( _In_ CTimeSpan const&  source )       noexcept { m_Time -= source.GetTotalSeconds(); return( *this ); };
};

#if defined( WFC_STL )

class CTime
{
   protected:

      CTimeEx m_Time;

   public:

      inline CTime() noexcept = default;
      inline explicit CTime( _In_ time_t const the_time ) noexcept : m_Time( the_time ) {};
      inline CTime( _In_ int const year, _In_ int const month, _In_ int const day, _In_ int const hour, _In_ int const minute, _In_ int const second, _In_ int const daylight_savings_time = -1 ) noexcept
      {
         m_Time.Copy( CTimeEx( year, month, day, hour, minute, second, daylight_savings_time ) );
      }

      // 2000-10-30
      // Thanks go to Danny Smith (danny_r_smith_2001@yahoo.co.nz) for
      // making this compilable in GCC.
      // GCC wants explicit conversion from CTimeEx
      // for CTime operator - (CTimeSpan const&) and CTime operator + (CTimeSpan const&)	
      inline constexpr explicit CTime( CTimeEx const& source ) noexcept : m_Time( source ) {};

      inline constexpr void Empty(void) noexcept
      {
          m_Time.Empty();
      }

      static CTime GetCurrentTime() noexcept { CTimeEx the_time; CTimeEx::GetCurrentTheTime( the_time ); return( CTime( (time_t) the_time ) ); }

      // Snap all of the calls to CTimeEx

      inline std::wstring Format( _In_z_ wchar_t const * format_string ) const noexcept { return( m_Time.Format( format_string ) ); }
      inline _Check_return_ int     GetDay(       void ) const noexcept { return( m_Time.GetDay()       ); }
      inline _Check_return_ int     GetDayOfWeek( void ) const noexcept { return( m_Time.GetDayOfWeek() ); }
      inline _Check_return_ int     GetHour(      void ) const noexcept { return( m_Time.GetHour()      ); }
      inline _Check_return_ int     GetMinute(    void ) const noexcept { return( m_Time.GetMinute()    ); }
      inline _Check_return_ int     GetMonth(     void ) const noexcept { return( m_Time.GetMonth()     ); }
      inline _Check_return_ int     GetSecond(    void ) const noexcept { return( m_Time.GetSecond()    ); }
      inline _Check_return_ int     GetYear(      void ) const noexcept { return( m_Time.GetYear()      ); }
      inline _Check_return_ time_t  GetTime(      void ) const noexcept { return( m_Time.operator time_t() ); }

      inline                 operator time_t     ( void                     ) const noexcept { return( m_Time.operator time_t() ); }
      inline       CTime&    operator =          ( _In_ CTime const&      source )       noexcept { m_Time.Copy( source.m_Time ); return( *this ); }
      inline       CTime&    operator =          ( _In_ time_t const     source )       noexcept { m_Time.Copy( source        ); return( *this ); }
      inline       CTime&    operator =          ( _In_ struct tm const&  source )       noexcept { m_Time.Copy( source        ); return( *this ); }
      inline       bool      operator ==         ( _In_ CTime const&      source ) const noexcept { return( m_Time.operator==( source.m_Time ) ); }
      inline       bool      operator !=         ( _In_ CTime const&      source ) const noexcept { return( m_Time.operator!=( source.m_Time ) ); }
      inline       bool      operator >=         ( _In_ CTime const&      source ) const noexcept { return( m_Time.operator>=( source.m_Time ) ); }
      inline       bool      operator <=         ( _In_ CTime const&      source ) const noexcept { return( m_Time.operator<=( source.m_Time ) ); }
      inline       bool      operator >          ( _In_ CTime const&      source ) const noexcept { return( m_Time.operator> ( source.m_Time ) ); }
      inline       bool      operator <          ( _In_ CTime const&      source ) const noexcept { return( m_Time.operator< ( source.m_Time ) ); }
      inline       CTimeSpan operator -          ( _In_ CTime const&      source ) const noexcept { return( m_Time.operator- ( source.m_Time ) ); }
      inline       CTime     operator -          ( _In_ CTimeSpan const&  source ) const noexcept { return( CTime(m_Time.operator- ( source ) ) ); }
      inline       CTime     operator +          ( _In_ CTimeSpan const&  source ) const noexcept { return( CTime(m_Time.operator+ ( source ) ) ); }
      inline CTime const&    operator +=         ( _In_ CTimeSpan const&  source )       noexcept { m_Time.AddSeconds( source.GetTotalSeconds() ); return( *this ); }
      inline CTime const&    operator -=         ( _In_ CTimeSpan const&  source )       noexcept { m_Time.SubtractSeconds( source.GetTotalSeconds() ); return( *this ); }
};

#endif // WFC_STL

#endif // TIME_EX_CLASS_HEADER_FILE
