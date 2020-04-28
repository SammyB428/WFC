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
** $Workfile: CSystemTime.cpp $
** $Revision: 32 $
** $Modtime: 6/26/01 10:51a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if ! defined( WFC_STL )
#include <afxdisp.h> // for COleDateTime
#endif // WFC_STL

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

// I found this algorithm on the net and converted it to a static inline
// Aaron Crane (aaron.crane@pobox.com) posted it originally, I fixed
// the 1752 problem myself. It seems no one knows about the days that 
// were dropped from September 1752. What are they teaching in schools
// these days?? This method uses Zeller's Conguruence.

static inline constexpr _Check_return_ uint16_t __get_day_of_week( _In_ int year, _In_ int month, _In_ int const day ) noexcept
{
   if ( month <= 2 )
   {
      year++;
      month += 12;
   }

   uint32_t day_of_week = 0;

   double term_1 = 0.0;

   term_1 = static_cast< double >( month + 1 ) * 3.0;
   term_1 /= 5.0;

   day_of_week  = ( day + ( month * 2 ) + static_cast< int >( term_1 ) + year + ( year / 4 ) );
   day_of_week -= ( year / 100 );
   day_of_week += ( year / 400 );
   day_of_week++;

   // Let's fix things for the Sept 3-13th that don't exist in 1752
   if ( year == 1752 )
   {
      if ( month < 9 )
      {
         day_of_week -= 10;
      }
      else if ( month == 9 )
      {
         if ( day <= 2 )
         {
            day_of_week -= 10;
         }
      }
   }
   else if ( year < 1752 )
   {
      day_of_week -= 10;
   }

   return( static_cast< uint16_t >( day_of_week % 7 ) );
}

#if ! defined( WFC_STL )

Win32FoundationClasses::CSystemTime::CSystemTime( _In_ COleDateTime const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

#endif // WFC_STL

void Win32FoundationClasses::CSystemTime::Copy( _In_ FILETIME const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   SYSTEMTIME system_time;

   if ( ::FileTimeToSystemTime( source, &system_time ) not_eq FALSE )
   {
      Copy( system_time );
   }
   else
   {
      Empty();
   }
}

void Win32FoundationClasses::CSystemTime::Copy( _In_ Win32FoundationClasses::CFileTime const& source ) noexcept
{
   FILETIME ft;

   ft.dwLowDateTime = source.dwLowDateTime;
   ft.dwHighDateTime = source.dwHighDateTime;

   SYSTEMTIME system_time;

   if ( ::FileTimeToSystemTime( &ft, &system_time ) not_eq FALSE )
   {
      Copy( system_time );
      return;
   }
   else
   {
      Empty();
   }
}

#if ! defined(WE_ARE_BUILDING_WFC_ON_UNIX)
void Win32FoundationClasses::CSystemTime::Copy( _In_ CTime const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   wYear         = static_cast< WORD >( source.GetYear()  );
   wMonth        = static_cast< WORD >( source.GetMonth() );
   wDay          = static_cast< WORD >( source.GetDay()   );

   // CTime day of week says Sunday is day 1, SYSTEMTIME says Sunday is zero
   wDayOfWeek    = static_cast< WORD >( source.GetDayOfWeek() - 1 );
   wHour         = static_cast< WORD >( source.GetHour()          );
   wMinute       = static_cast< WORD >( source.GetMinute()        );
   wSecond       = static_cast< WORD >( source.GetSecond()        );
   wMilliseconds = 0;
}
#endif // WE_ARE_BUILDING_WFC_ON_UNIX

#if ! defined( WFC_STL )

void Win32FoundationClasses::CSystemTime::Copy( _In_ COleDateTime const& source )
{
   WFC_VALIDATE_POINTER( this );

   if ( source.GetStatus() not_eq COleDateTime::valid )
   {
      Empty();
      return;
   }

   wYear         = static_cast< WORD >( source.GetYear() );
   wMonth        = static_cast< WORD >( source.GetMonth() );
   wDay          = static_cast< WORD >( source.GetDay() );

   // COleDateTime day of week says Sunday is day 1, SYSTEMTIME says Sunday is zero
   wDayOfWeek    = static_cast< WORD >( source.GetDayOfWeek() - 1 );
   wHour         = static_cast< WORD >( source.GetHour() );
   wMinute       = static_cast< WORD >( source.GetMinute() );
   wSecond       = static_cast< WORD >( source.GetSecond() );
   wMilliseconds = 0;
}

#endif // WFC_STL

void Win32FoundationClasses::CSystemTime::Copy( _In_ TIME_OF_DAY_INFO const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   wHour         = static_cast<uint16_t>( source->tod_hours );
   wMinute       = static_cast<uint16_t>( source->tod_mins  );
   wSecond       = static_cast<uint16_t>( source->tod_secs  );
   wDay          = static_cast<uint16_t>( source->tod_day   );
   wMonth        = static_cast<uint16_t>( source->tod_month );
   wYear         = static_cast<uint16_t>( source->tod_year  );
   wMilliseconds = static_cast<uint16_t>( source->tod_hunds * 10 );

   wDayOfWeek = __get_day_of_week( wYear, wMonth, wDay );
}

void Win32FoundationClasses::CSystemTime::Copy( _In_ TIME_OF_DAY_INFO const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   wHour         = static_cast<uint16_t>( source.tod_hours );
   wMinute       = static_cast<uint16_t>( source.tod_mins  );
   wSecond       = static_cast<uint16_t>( source.tod_secs  );
   wDay          = static_cast<uint16_t>( source.tod_day   );
   wMonth        = static_cast<uint16_t>( source.tod_month );
   wYear         = static_cast<uint16_t>( source.tod_year  );
   wMilliseconds = static_cast<uint16_t>( source.tod_hunds * 10 );

   wDayOfWeek = __get_day_of_week( wYear, wMonth, wDay );
}

void Win32FoundationClasses::CSystemTime::Copy( _In_ TIMESTAMP_STRUCT const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   wYear         = static_cast<uint16_t>( source->year   );
   wMonth        = static_cast<uint16_t>( source->month  );
   wDay          = static_cast<uint16_t>( source->day    );
   wHour         = static_cast<uint16_t>( source->hour   );
   wMinute       = static_cast<uint16_t>( source->minute );
   wSecond       = static_cast<uint16_t>( source->second );
   wMilliseconds = static_cast<uint16_t>( source->fraction * 10 );

   wDayOfWeek = __get_day_of_week( wYear, wMonth, wDay );
}

void Win32FoundationClasses::CSystemTime::Copy( _In_ TIMESTAMP_STRUCT const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   wYear         = static_cast<uint16_t>( source.year   );
   wMonth        = static_cast<uint16_t>( source.month  );
   wDay          = static_cast<uint16_t>( source.day    );
   wHour         = static_cast<uint16_t>( source.hour   );
   wMinute       = static_cast<uint16_t>( source.minute );
   wSecond       = static_cast<uint16_t>( source.second );
   wMilliseconds = static_cast<uint16_t>( source.fraction * 10 );

   wDayOfWeek = __get_day_of_week( wYear, wMonth, wDay );
}

#if ! defined( WFC_STL )

void CSystemTime::CopyTo( _Out_ COleDateTime& destination ) const
{
   WFC_VALIDATE_POINTER( this );

   if ( wYear > 1899 )
   {
      destination = COleDateTime( wYear, wMonth, wDay, wHour, wMinute, wSecond );
   }
   else
   {
      destination.m_dt     = 0.0;
      destination.m_status = COleDateTime::invalid;
   }
}

#endif // WFC_STL

#if ! defined(WE_ARE_BUILDING_WFC_ON_UNIX)

void Win32FoundationClasses::CSystemTime::CopyTo( _Out_ CTime& destination ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( wYear > 1969 and wYear < 2038 )
   {
      destination = CTime( wYear, wMonth, wDay, wHour, wMinute, wSecond );
   }
   else
   {
      destination = CTime( (time_t) 0 );
   }
}
#endif

void Win32FoundationClasses::CSystemTime::CopyTo( _Out_ TIMESTAMP_STRUCT * destination ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( destination );

   if ( destination == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   destination->year     = wYear;
   destination->month    = wMonth;
   destination->day      = wDay;
   destination->hour     = wHour;
   destination->minute   = wMinute;
   destination->second   = wSecond;
   destination->fraction = ( wMilliseconds / 10 );
}

void Win32FoundationClasses::CSystemTime::CopyTo( _Out_ TIMESTAMP_STRUCT& destination ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   destination.year     = wYear;
   destination.month    = wMonth;
   destination.day      = wDay;
   destination.hour     = wHour;
   destination.minute   = wMinute;
   destination.second   = wSecond;
   destination.fraction = ( wMilliseconds / 10 );
}

#if ! defined( WE_ARE_BUILDING_WFC_ON_UNIX )
void Win32FoundationClasses::CSystemTime::CopyTo( _Out_ double& destination ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   // SystemTimeToVariantTime() is not const correct. To prevent
   // the children at Microsoft from harming us, we will copy
   // our FILETIME to an expendable one.

   SYSTEMTIME system_time;

   system_time.wDay          = wDay;
   system_time.wDayOfWeek    = wDayOfWeek;
   system_time.wHour         = wHour;
   system_time.wMilliseconds = wMilliseconds;
   system_time.wMinute       = wMinute;
   system_time.wMonth        = wMonth;
   system_time.wSecond       = wSecond;
   system_time.wYear         = wYear;

   if ( ::SystemTimeToVariantTime( &system_time, &destination ) == 0 )
   {
      destination = 0.0;
   }
}
#endif // WE_ARE_BUILDING_WFC_ON_UNIX

void Win32FoundationClasses::CSystemTime::CopyTo( _Out_ SYSTEMTIME& destination ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   destination.wYear         = wYear;
   destination.wMonth        = wMonth;
   destination.wDay          = wDay;
   destination.wDayOfWeek    = wDayOfWeek;
   destination.wHour         = wHour;
   destination.wMinute       = wMinute;
   destination.wSecond       = wSecond;
   destination.wMilliseconds = wMilliseconds;
}

_Check_return_ uint32_t Win32FoundationClasses::CSystemTime::NumberOfMinutesSinceMonday( void ) const noexcept
{
    Win32FoundationClasses::CTimeEx now( wYear, wMonth, wDay, wHour, wMinute, wSecond );

   int day_of_week = now.GetDayOfWeek(); // 1 == Sunday, 2  == Monday

   int days_from_monday = day_of_week - 2;

   if ( days_from_monday < 0 )
   {
      days_from_monday += 7;
   }

   uint32_t const return_value = (days_from_monday * NUMBER_OF_MINUTES_IN_ONE_DAY) + (wHour * 60) + wMinute;

   return( return_value );
}

#if ! defined(WE_ARE_BUILDING_WFC_ON_UNIX)
_Check_return_ bool Win32FoundationClasses::CSystemTime::Set( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( ::SetSystemTime( this ) == FALSE )
   {
      // Maybe we need to get permission...
      HANDLE token_handle = static_cast< HANDLE >( NULL );

      DWORD error_code = 0;

      if ( ::OpenProcessToken( ::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES bitor TOKEN_QUERY, &token_handle ) == FALSE )
      {
         error_code = ::GetLastError();
         //WFCTRACEERROR( error_code );
         return( false );
      }

      TOKEN_PRIVILEGES token_privileges;

      ::ZeroMemory( &token_privileges, sizeof( token_privileges ) );

      if ( ::LookupPrivilegeValue( nullptr, SE_SYSTEMTIME_NAME, &token_privileges.Privileges[ 0 ].Luid ) == FALSE )
      {
         error_code = ::GetLastError();
         //WFCTRACEERROR( error_code );
         (void) Win32FoundationClasses::wfc_close_handle( token_handle );
         token_handle = static_cast< HANDLE >( NULL );
         return( false );
      }

      token_privileges.PrivilegeCount             = 1;
      token_privileges.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED;

      if ( ::AdjustTokenPrivileges( token_handle, FALSE, &token_privileges, 0, (PTOKEN_PRIVILEGES) nullptr, 0 ) == FALSE )
      {
         error_code = ::GetLastError();
         //WFCTRACEERROR( error_code );
         (void) Win32FoundationClasses::wfc_close_handle( token_handle );
         token_handle = static_cast< HANDLE >( NULL );
         return( false );
      }

      // Finally, let's get around to setting the time

      if ( ::SetSystemTime( this ) == FALSE )
      {
         error_code = ::GetLastError();
         //WFCTRACEERROR( error_code );
         (void) Win32FoundationClasses::wfc_close_handle( token_handle );
         token_handle = static_cast< HANDLE >( NULL );
         return( false );
      }

      (void) Win32FoundationClasses::wfc_close_handle( token_handle );
      token_handle = static_cast< HANDLE >( NULL );
      return( true );
   }

   return( false );
}
#endif

/*
** Operators
*/

_Check_return_ Win32FoundationClasses::CSystemTime& Win32FoundationClasses::CSystemTime::operator = ( _In_ Win32FoundationClasses::CFileTime const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

#if ! defined(WE_ARE_BUILDING_WFC_ON_UNIX)
_Check_return_ Win32FoundationClasses::CSystemTime& Win32FoundationClasses::CSystemTime::operator = ( _In_ Win32FoundationClasses::CTime const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}
#endif

#if ! defined( WFC_STL )

_Check_return_ CSystemTime& CSystemTime::operator = ( _In_ COleDateTime const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

#endif // WFC_STL

_Check_return_ bool Win32FoundationClasses::CSystemTime::operator == ( _In_ Win32FoundationClasses::CSystemTime const& source ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( Compare( source ) == I_AM_EQUAL_TO_THAT)
   {
      return( TRUE );
   }

   return( FALSE );
}

_Check_return_ bool Win32FoundationClasses::CSystemTime::operator != ( _In_ Win32FoundationClasses::CSystemTime const& source ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( Compare( source ) == I_AM_EQUAL_TO_THAT)
   {
      return( FALSE );
   }

   return( FALSE );
}

_Check_return_ bool Win32FoundationClasses::CSystemTime::operator > ( _In_ Win32FoundationClasses::CSystemTime const& source ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( Compare( source ) > 0 )
   {
      return( TRUE );
   }
   else
   {
      return( FALSE );
   }
}

_Check_return_ bool Win32FoundationClasses::CSystemTime::operator < ( _In_ Win32FoundationClasses::CSystemTime const& source ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( Compare( source ) < 0 )
   {
      return( TRUE );
   }
   else
   {
      return( FALSE );
   }
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CSystemTime::Dump( CDumpContext& dump_context ) const
{
   LPCTSTR months[ 12 ] = { TEXT( "(January)"   ),
                            TEXT( "(February)"  ),
                            TEXT( "(March)"     ),
                            TEXT( "(April)"     ),
                            TEXT( "(May)"       ),
                            TEXT( "(June)"      ),
                            TEXT( "(July)"      ),
                            TEXT( "(August)"    ),
                            TEXT( "(September)" ),
                            TEXT( "(October)"   ),
                            TEXT( "(November)"  ),
                            TEXT( "(December)"  )
                          };

   LPCTSTR days[ 7 ] = { TEXT( "(Sunday)"    ),
                         TEXT( "(Monday)"    ),
                         TEXT( "(Tuesday)"   ),
                         TEXT( "(Wednesday)" ),
                         TEXT( "(Thursday)"  ),
                         TEXT( "(Friday)"    ),
                         TEXT( "(Saturday)"  )
                       };

   dump_context << TEXT( "a CSystemTime at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   wYear is  " ) << wYear << TEXT( "\n" );
   dump_context << TEXT( "   wMonth is " ) << wMonth;

   if ( wMonth > 0 and wMonth < 13 )
   {
      dump_context << TEXT( " " ) << months[ wMonth - 1 ] << TEXT( "\n" );
   }
   else
   {
      dump_context << TEXT( " (Invalid)\n" );
   }

   dump_context << TEXT( "   wDayOfWeek is " ) << wDayOfWeek;

   if ( wDayOfWeek < 7 )
   {
      dump_context << TEXT( " " ) << days[ wDayOfWeek ] << TEXT( "\n" );
   }
   else
   {
      dump_context << TEXT( " (Invalid)\n" );
   }

   dump_context << TEXT( "   wDay          is " ) << wDay          << TEXT( "\n" );
   dump_context << TEXT( "   wHour         is " ) << wHour         << TEXT( "\n" );
   dump_context << TEXT( "   wMinute       is " ) << wMinute       << TEXT( "\n" );
   dump_context << TEXT( "   wSecond       is " ) << wSecond       << TEXT( "\n" );
   dump_context << TEXT( "   wMilliseconds is " ) << wMilliseconds << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ FILETIME Win32FoundationClasses::CSystemTime::AsFiletime() const noexcept
{
    FILETIME return_value = { 0, 0 };

    Win32FoundationClasses::CFileTime as_a_filetime( this );

    as_a_filetime.CopyTo( return_value );

    return( return_value );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CSystemTime</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that lets you set the clock in NT.">
</HEAD>

<BODY>

<H1>CSystemTime : SYSTEMTIME</H1>
$Revision: 32 $<BR><HR>

<H2>Description</H2>
This class encapsulates the SYSTEMTIME data structure in Win32. The benefit
from doing this is adding capability to convert between a bunch of time 
representations.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CSystemTime</B>()
<B>CSystemTime</B>( const COleDateTime&amp; source )
<B>CSystemTime</B>( const CSystemTime&amp; source )
<B>CSystemTime</B>( const <A HREF="CFileTime.htm">CFileTime</A>&amp; source )
<B>CSystemTime</B>( const CTime&amp; source )
<B>CSystemTime</B>( const SYSTEMTIME * source )
<B>CSystemTime</B>( const FILETIME * source )
<B>CSystemTime</B>( const TIME_OF_DAY_INFO * source )
<B>CSystemTime</B>( const TIMESTAMP_STRUCT * source )</PRE><DD>
Constructs the object.
The constructor that takes a COleDateTime parameter is not present
in STL builds.

</DL>

<H2>Data Members</H2>

Whatever is in SYSTEMTIME.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>LONG <B><A NAME="Compare">Compare</A></B>( const CSystemTime&amp; source )</PRE><DD>
Returns the same values as std::wstring::compare().

<DT><PRE>void <B><A NAME="Copy">Copy</A></B>( const COleDateTime&amp; source )
void <B>Copy</B>( const CSystemTime&amp; source )
void <B>Copy</B>( const <A HREF="CFileTime.htm">CFileTime</A>&amp; source )
void <B>Copy</B>( const CTime&amp; source )
void <B>Copy</B>( const SYSTEMTIME * source )
void <B>Copy</B>( const FILETIME * source )
void <B>Copy</B>( const TIME_OF_DAY_INFO * source )
void <B>Copy</B>( const TIMESTAMP_STRUCT * source )</PRE><DD>
Sets the time to what you specify, you can copy
another
<A HREF="CFileTime.htm">CFileTime</A>, <B>CSystemTime</B>, FILETIME,
SYSTEMTIME or TIME_OF_DAY_INFO.

<DT><PRE>void <B><A NAME="CopyTo">CopyTo</A></B>( double&amp; destination ) const
void <B>CopyTo</B>( CTime&amp; destination ) const
void <B>CopyTo</B>( SYSTEMTIME&amp; destination ) const
void <B>CopyTo</B>( TIMESTAMP_STRUCT&amp; destination )
void <B>CopyTo</B>( TIMESTAMP_STRUCT * destination )</PRE>
Copies the date/time out to the various types.

<DT><PRE>void <B><A NAME="Empty">Empty</A></B>( void )</PRE><DD>Clears the data members.

<DT><PRE>void <B><A NAME="Get">Get</A></B>( void )</PRE><DD>
Retrieves the current time and stores in in the object.

<DT><PRE>static CSystemTime <B><A NAME="GetTheCurrentTime">GetTheCurrentTime</A></B>( void )</PRE><DD>
I would have liked to call this function <B>GetCurrentTime</B>() but Microsoft
screwed me in the <CODE>winbase.h</CODE> file with a &#35;define. Oh well, that's life.
This method retrieves the current time and returns it as a <B>CSystemTime</B>
object.

<DT><PRE>BOOL <B><A NAME="Set">Set</A></B>( void ) const</PRE><DD>
Sets the computer's clock equal to the time stored in this object.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   <B>CSystemTime</B> the_time;

   the_time.Get(); // Get the current time

   the_time.wHour++;

   if ( the_time.Set() not_eq FALSE )
   {
      _tprintf( TEXT( &quot;Time advanced by one hour\n&quot; ) );
   }
   else
   {
      _tprintf( TEXT( &quot;Can't adjust time\n&quot; ) );
   }
}</CODE></PRE>

<H2>API's Used</H2>

<UL>
<LI>AdjustTokenPrivileges
<LI>FileTimeToSystemTime
<LI>GetSystemTime
<LI>LookupPrivilegeValue
<LI>OpenProcessToken
<LI>SetSystemTime
<LI>SystemTimeToVariantTime
<LI>ZeroMemory
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CSystemTime.cpp $<BR>
$Modtime: 6/26/01 10:51a $
</BODY>

</HTML>
*/
