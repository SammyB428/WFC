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
** $Workfile: CSystemTime.hpp $
** $Revision: 17 $
** $Modtime: 6/26/01 11:04a $
*/

#if ! defined( TIME_EX_CLASS_HEADER_FILE )

#define TIME_EX_CLASS_HEADER_FILE

#if defined( WFC_STL )

class CTimeSpan
{
   protected:

       time_t m_NumberOfSeconds{ 0 };

   public:

      inline constexpr CTimeSpan() { m_NumberOfSeconds = 0; }
      inline  CTimeSpan( __in const unsigned long number_of_seconds ) { m_NumberOfSeconds = number_of_seconds; }
      inline  CTimeSpan( __in const unsigned long days, __in const unsigned long hours, __in const unsigned long minutes, __in const unsigned long number_of_seconds )
      {
         m_NumberOfSeconds  = number_of_seconds;
         m_NumberOfSeconds += ( minutes * 60L );
         m_NumberOfSeconds += ( 60L * 60L * hours );
         m_NumberOfSeconds += ( 24L * 60L * 60L * days );
      }

      inline CTimeSpan( __in const CTimeSpan& source ) { m_NumberOfSeconds = source.m_NumberOfSeconds; }
      inline ~CTimeSpan() { m_NumberOfSeconds = 0; }

      inline constexpr void Empty( void ) { m_NumberOfSeconds = 0; }

      inline constexpr __checkReturn long    GetTotalHours( void ) const noexcept { return( (long)( m_NumberOfSeconds / 3600L ) ); }
      inline constexpr __checkReturn long    GetTotalMinutes( void ) const noexcept { return( (long) (m_NumberOfSeconds / 60L ) ); }
      inline constexpr __checkReturn time_t  GetTotalSeconds( void ) const noexcept { return( m_NumberOfSeconds ); }
      inline constexpr __checkReturn int64_t GetTotalTicks( void ) const noexcept { return( (int64_t) ( (int64_t) m_NumberOfSeconds * static_cast<int64_t>(10000000)) ); }
      inline constexpr __checkReturn long    GetDays( void ) const noexcept { return( (long) ( m_NumberOfSeconds / 86400L ) ); }
      inline constexpr __checkReturn long    GetHours( void ) const noexcept { return( GetTotalHours() - ( GetDays() * 24L ) ); }
      inline constexpr __checkReturn long    GetMinutes( void ) const noexcept { return( GetTotalMinutes() - ( GetTotalHours() * 60L ) ); }
      inline constexpr __checkReturn long    GetSeconds( void ) const noexcept { return( (long) ( GetTotalSeconds() - ( GetTotalMinutes() * 60L ) ) ); }
      inline constexpr               void    SetTicks( __in uint64_t const ticks ) noexcept { m_NumberOfSeconds = (time_t) ( ticks / 10000000); }
      inline __checkReturn CTimeSpan operator-( __in CTimeSpan const& source ) const noexcept { return( CTimeSpan( (unsigned long) ( m_NumberOfSeconds - source.m_NumberOfSeconds ) ) ); }
      inline __checkReturn CTimeSpan operator+( __in CTimeSpan const& source ) const noexcept { return( CTimeSpan( (unsigned long) ( m_NumberOfSeconds + source.m_NumberOfSeconds ) ) ); }
      inline constexpr __checkReturn const CTimeSpan& operator+=( __in CTimeSpan const& source ) noexcept { m_NumberOfSeconds += source.m_NumberOfSeconds; return( *this ); }
      inline constexpr __checkReturn const CTimeSpan& operator-=( __in CTimeSpan const& source ) noexcept { m_NumberOfSeconds -= source.m_NumberOfSeconds; return( *this ); }
      inline constexpr __checkReturn bool operator == ( __in CTimeSpan const& source ) const noexcept { return( m_NumberOfSeconds == source.m_NumberOfSeconds ); }
      inline constexpr __checkReturn bool operator != ( __in CTimeSpan const& source ) const noexcept { return( m_NumberOfSeconds != source.m_NumberOfSeconds ); }
      inline constexpr __checkReturn bool operator < ( __in CTimeSpan const& source ) const noexcept  { return( m_NumberOfSeconds <  source.m_NumberOfSeconds ); }
      inline constexpr __checkReturn bool operator > ( __in CTimeSpan const& source ) const noexcept  { return( m_NumberOfSeconds >  source.m_NumberOfSeconds ); }
      inline constexpr __checkReturn bool operator <= ( __in CTimeSpan const& source ) const noexcept { return( m_NumberOfSeconds <= source.m_NumberOfSeconds ); }
      inline constexpr __checkReturn bool operator >= ( __in CTimeSpan const& source ) const noexcept { return( m_NumberOfSeconds >= source.m_NumberOfSeconds ); }

      __checkReturn std::wstring Format( _In_z_ wchar_t const * format_string ) const noexcept;
};

#endif // WFC_STL

class CTimeEx
{
   protected:

       time_t m_Time{ 0 };

      __checkReturn time_t m_Make_time_t( __in struct tm const * time_p ) noexcept;

   public:

      // Construction

      CTimeEx();
      CTimeEx( __in CTimeEx const&    source );
      CTimeEx( __in CTimeEx const *   source );
      CTimeEx( __in time_t const     source );
      CTimeEx( __in struct tm const * source );
      CTimeEx( __in struct tm const&  source );
      CTimeEx( __in int const year, __in int const month, __in int const day, __in int const hour, __in int const minute, __in int const second, __in int const daylight_savings_time = -1 );
      CTimeEx( __in FILETIME const& source );
      CTimeEx( __in FILETIME const * source );
     ~CTimeEx();

      // Methods

      static void GetCurrentTheTime( __out CTimeEx& source ) noexcept;
      static void GreenwichMeanTime( __in time_t const * address_of_a_time_t, __out struct tm * address_of_a_tm_structure ) noexcept;

      __checkReturn int Compare( __in CTimeEx const& source ) const noexcept;
      void   Copy( __in CTimeEx const&    source ) noexcept;
      void   Copy( __in CTimeEx const *   source ) noexcept;
      void   Copy( __in time_t const     source ) noexcept;
      void   Copy( __in struct tm const * source ) noexcept;
      void   Copy( __in struct tm const&  source ) noexcept;
      void   Copy( __in FILETIME const&   source ) noexcept;
      void   Copy( __in FILETIME const *  source ) noexcept;
      void   CopyModifiedJulianDate( __in double const source ) noexcept;
      void   CopyTo( __out time_t&    destination ) const noexcept;
      void   CopyTo( __out struct tm& destination ) const noexcept;
      void   CopyTo(__out std::wstring& iso_8601_date) const noexcept;
      void   Empty( void ) noexcept;
      void   FileNameFormat(_Out_ std::wstring& out_string) const noexcept;
      __checkReturn std::wstring Format( _In_z_ wchar_t const * format_string ) const noexcept; // same as strftime
      __checkReturn int    GetDay( void ) const noexcept;
      __checkReturn int    GetDayOfWeek( void ) const noexcept;
      __checkReturn int    GetDayOfYear( void ) const noexcept;
      __checkReturn int    GetHour( void ) const noexcept;
      __checkReturn int    GetMinute( void ) const noexcept;
      __checkReturn int    GetMinuteOfDay( void ) const noexcept;
      __checkReturn int    GetMonth( void ) const noexcept;
      __checkReturn int    GetSecond( void ) const noexcept;
      void   GetTime( __out struct tm& time_structure ) const noexcept;
      __checkReturn time_t GetTotalSeconds( void ) const noexcept;
      __checkReturn int    GetYear( void ) const noexcept;

      inline constexpr void AddSeconds( __in time_t const seconds ) noexcept
      {
          m_Time += seconds;
      }

      inline constexpr void SubtractSeconds( __in time_t const seconds ) noexcept
      {
          m_Time -= seconds;
      }

      inline __checkReturn bool IsAfter( __in CTimeEx const& when ) const noexcept
      {
          if ( Compare( when ) > 0 )
          {
              return( true );
          }

          return( false );
      }

      inline __checkReturn bool IsBefore( __in CTimeEx const& when ) const noexcept
      {
          if ( Compare( when ) < 0 )
          {
              return( true );
          }

          return( false );
      }

      void Now( void ) noexcept;
      void Set( __in const std::wstring& iso_8601_date ) noexcept;
      void Set( __in int year, __in int month, __in int day, __in int hour, __in int minute, __in int second, __in int daylight_savings_time = -1 ) noexcept;

      // Operators

      inline constexpr __checkReturn                 operator time_t     ( void                     )      const noexcept { return( m_Time ); };
      inline __checkReturn       CTimeEx&  operator =          ( __in CTimeEx const&    source )       noexcept { Copy( source ); return( *this ); };
      inline __checkReturn       CTimeEx&  operator =          ( __in time_t const     source )       noexcept { Copy( source ); return( *this ); };
      inline __checkReturn       CTimeEx&  operator =          ( __in struct tm const&  source )       noexcept { Copy( source ); return( *this ); };
      inline __checkReturn       bool      operator ==         ( __in CTimeEx const&    source ) const noexcept { return( ( Compare( source ) == 0   ) ? TRUE  : FALSE ); };
      inline __checkReturn       bool      operator !=         ( __in CTimeEx const&    source ) const noexcept { return( ( Compare( source ) == 0   ) ? FALSE : TRUE  ); };
      inline __checkReturn       bool      operator >=         ( __in CTimeEx const&    source ) const noexcept { return( ( Compare( source ) > (-1) ) ? TRUE  : FALSE ); };
      inline __checkReturn       bool      operator <=         ( __in CTimeEx const&    source ) const noexcept { return( ( Compare( source ) < 1    ) ? TRUE  : FALSE ); };
      inline __checkReturn       bool      operator >          ( __in CTimeEx const&    source ) const noexcept { return( ( Compare( source ) > 0    ) ? TRUE  : FALSE ); };
      inline __checkReturn       bool      operator <          ( __in CTimeEx const&    source ) const noexcept { return( ( Compare( source ) < 0    ) ? TRUE  : FALSE ); };
      inline __checkReturn       CTimeSpan operator -          ( __in CTimeEx const&    source ) const noexcept { return( CTimeSpan( (unsigned long) ( m_Time - source.m_Time ) ) ); };
      inline __checkReturn       CTimeEx   operator -          ( __in CTimeSpan const&  source ) const noexcept { return( CTimeEx( m_Time - source.GetTotalSeconds() ) ); };
      inline __checkReturn       CTimeEx   operator +          ( __in CTimeSpan const&  source ) const noexcept { return( CTimeEx( m_Time + source.GetTotalSeconds() ) ); };
      inline __checkReturn CTimeEx const&  operator +=         ( __in CTimeSpan const&  source )       noexcept { m_Time += source.GetTotalSeconds(); return( *this ); };
      inline __checkReturn CTimeEx const&  operator -=         ( __in CTimeSpan const&  source )       noexcept { m_Time -= source.GetTotalSeconds(); return( *this ); };
};

#if defined( WFC_STL )

class CTime
{
   protected:

      CTimeEx m_Time;

   public:

      inline CTime(){};
      inline CTime( __in const time_t the_time ) : m_Time( the_time ) {};
      inline CTime( __in const int year, __in const int month, __in const int day, __in const int hour, __in const int minute, __in const int second, __in const int daylight_savings_time = -1 )
      {
         m_Time.Copy( CTimeEx( year, month, day, hour, minute, second, daylight_savings_time ) );
      }

      // 2000-10-30
      // Thanks go to Danny Smith (danny_r_smith_2001@yahoo.co.nz) for
      // making this compilable in GCC.
      // GCC wants explicit conversion from CTimeEx
      // for CTime operator - (const CTimeSpan&) and CTime operator + (const CTimeSpan&)	
      inline CTime( const CTimeEx& source ) : m_Time( source ) {};

      inline ~CTime() {};

      inline void Empty(void)
      {
          m_Time.Empty();
      }

      static CTime PASCAL GetCurrentTime() noexcept { CTimeEx the_time; CTimeEx::GetCurrentTheTime( the_time ); return( CTime( (time_t) the_time ) ); }

      // Snap all of the calls to CTimeEx

      inline std::wstring Format( _In_z_ const wchar_t * format_string ) const noexcept { return( m_Time.Format( format_string ) ); }
      inline __checkReturn int     GetDay(       void ) const noexcept { return( m_Time.GetDay()       ); }
      inline __checkReturn int     GetDayOfWeek( void ) const noexcept { return( m_Time.GetDayOfWeek() ); }
      inline __checkReturn int     GetHour(      void ) const noexcept { return( m_Time.GetHour()      ); }
      inline __checkReturn int     GetMinute(    void ) const noexcept { return( m_Time.GetMinute()    ); }
      inline __checkReturn int     GetMonth(     void ) const noexcept { return( m_Time.GetMonth()     ); }
      inline __checkReturn int     GetSecond(    void ) const noexcept { return( m_Time.GetSecond()    ); }
      inline __checkReturn int     GetYear(      void ) const noexcept { return( m_Time.GetYear()      ); }
      inline __checkReturn time_t  GetTime(      void ) const noexcept { return( m_Time.operator time_t() ); }

      inline                 operator time_t     ( void                     ) const noexcept { return( m_Time.operator time_t() ); }
      inline       CTime&    operator =          ( __in const CTime&      source )       noexcept { m_Time.Copy( source.m_Time ); return( *this ); }
      inline       CTime&    operator =          ( __in const time_t      source )       noexcept { m_Time.Copy( source        ); return( *this ); }
      inline       CTime&    operator =          ( __in const struct tm&  source )       noexcept { m_Time.Copy( source        ); return( *this ); }
      inline       bool      operator ==         ( __in const CTime&      source ) const noexcept { return( m_Time.operator==( source.m_Time ) ); }
      inline       bool      operator !=         ( __in const CTime&      source ) const noexcept { return( m_Time.operator!=( source.m_Time ) ); }
      inline       bool      operator >=         ( __in const CTime&      source ) const noexcept { return( m_Time.operator>=( source.m_Time ) ); }
      inline       bool      operator <=         ( __in const CTime&      source ) const noexcept { return( m_Time.operator<=( source.m_Time ) ); }
      inline       bool      operator >          ( __in const CTime&      source ) const noexcept { return( m_Time.operator> ( source.m_Time ) ); }
      inline       bool      operator <          ( __in const CTime&      source ) const noexcept { return( m_Time.operator< ( source.m_Time ) ); }
      inline       CTimeSpan operator -          ( __in const CTime&      source ) const noexcept { return( m_Time.operator- ( source.m_Time ) ); }
      inline       CTime     operator -          ( __in const CTimeSpan&  source ) const noexcept { return( m_Time.operator- ( source        ) ); }
      inline       CTime     operator +          ( __in const CTimeSpan&  source ) const noexcept { return( m_Time.operator+ ( source        ) ); }
      inline const CTime&    operator +=         ( __in const CTimeSpan&  source )       noexcept { m_Time.AddSeconds( source.GetTotalSeconds() ); return( *this ); }
      inline const CTime&    operator -=         ( __in const CTimeSpan&  source )       noexcept { m_Time.SubtractSeconds( source.GetTotalSeconds() ); return( *this ); }
};

#endif // WFC_STL

#endif // TIME_EX_CLASS_HEADER_FILE
