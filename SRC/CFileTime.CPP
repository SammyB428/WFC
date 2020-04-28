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
** $Workfile: CFileTime.CPP $
** $Revision: 34 $
** $Modtime: 6/26/01 10:46a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#if ! defined( WFC_STL )

void Win32FoundationClasses::CFileTime::CopyTo( COleDateTime& destination ) const
{
   WFC_VALIDATE_POINTER( this );
   CSystemTime system_time( this );

   system_time.CopyTo( destination );
}

#endif // WFC_STL

_Check_return_ std::wstring Win32FoundationClasses::CFileTime::SQLDate(void) const noexcept
{
    Win32FoundationClasses::CSystemTime stime( this );

    std::wstring return_value;
    Win32FoundationClasses::format( return_value, L"%04d-%02d-%02d", (int) stime.wYear, (int) stime.wMonth, (int) stime.wDay );

    return( return_value );
}

_Check_return_ std::wstring Win32FoundationClasses::CFileTime::SQLTime(void) const noexcept
{
    Win32FoundationClasses::CSystemTime stime( this );

    std::wstring return_value;
    Win32FoundationClasses::format( return_value, L"%02d:%02d:%02d", (int) stime.wHour, (int) stime.wMinute, (int) stime.wSecond );

    return( return_value );
}

_Check_return_ std::wstring Win32FoundationClasses::CFileTime::SQLTimestamp(void) const noexcept
{
    Win32FoundationClasses::CSystemTime stime( this );

    std::wstring return_value;
    Win32FoundationClasses::format( return_value, L"%04d-%02d-%02d %02d:%02d:%02d", (int) stime.wYear, (int) stime.wMonth, (int) stime.wDay, (int) stime.wHour, (int) stime.wMinute, (int) stime.wSecond );

    return( return_value );
}

void Win32FoundationClasses::CFileTime::ISO8601(_Out_ std::wstring& return_value) const noexcept
{
    uint64_t const this_time = AsTicks();
    uint64_t const microsecond_remainder = (this_time % CFileTime::NumberOfFiletimeTicksInOneSecond) / CFileTime::NumberOfFiletimeTicksInOneMicrosecond;

    Win32FoundationClasses::CSystemTime stime(this);

    if (microsecond_remainder > 0)
    {
        double fractional_part = (double)((double)microsecond_remainder / (double)CFileTime::NumberOfMicrosecondsInOneSecond); // One million microseconds in one second

        fractional_part += (double)stime.wSecond;

        Win32FoundationClasses::format(return_value, L"%04d-%02d-%02dT%02d:%02d:", (int)stime.wYear, (int)stime.wMonth, (int)stime.wDay, (int)stime.wHour, (int)stime.wMinute);

        std::wstring seconds_part;

        Win32FoundationClasses::format(seconds_part, L"%2.6lf", fractional_part);

        if (seconds_part.at(1) == '.')
        {
            // The value is less than 10, we need to insert a leading zero...
            return_value.push_back('0');
        }

        return_value.append(seconds_part);

        Win32FoundationClasses::trim_right(return_value, '0');
        return_value.push_back('Z');
    }
    else
    {
        Win32FoundationClasses::format( return_value, L"%04d-%02d-%02dT%02d:%02d:%02dZ", (int)stime.wYear, (int)stime.wMonth, (int)stime.wDay, (int)stime.wHour, (int)stime.wMinute, (int)stime.wSecond);
    }
}

_Check_return_ std::wstring Win32FoundationClasses::CFileTime::PostgreSQLTimestamp(void) const noexcept
{
    std::wstring return_value;

    uint64_t const this_time = AsTicks();
    uint64_t const microsecond_remainder = ( this_time % CFileTime::NumberOfFiletimeTicksInOneSecond) / CFileTime::NumberOfFiletimeTicksInOneMicrosecond;

    Win32FoundationClasses::CSystemTime stime;

    stime.Copy( *this );

    if ( microsecond_remainder > 0 )
    {
        double fractional_part = (double)( (double) microsecond_remainder / (double)CFileTime::NumberOfMicrosecondsInOneSecond); // One million microseconds in one second

        fractional_part += (double) stime.wSecond;

        Win32FoundationClasses::format(return_value, L"%04d-%02d-%02d %02d:%02d:", (int)stime.wYear, (int)stime.wMonth, (int)stime.wDay, (int)stime.wHour, (int)stime.wMinute);

        std::wstring seconds_part;

        Win32FoundationClasses::format(seconds_part, L"%02.6lf", fractional_part);

        if (seconds_part.at(1) == '.')
        {
            // The value is less than 10, we need to insert a leading zero...
            return_value.push_back('0');
        }

        return_value.append(seconds_part);

        Win32FoundationClasses::trim_right( return_value, '0' );
    }
    else
    {
        Win32FoundationClasses::format( return_value, L"%04d-%02d-%02d %02d:%02d:%02d", (int) stime.wYear, (int) stime.wMonth, (int) stime.wDay, (int) stime.wHour, (int) stime.wMinute, (int) stime.wSecond );
    }

    return( return_value );
}

#if ! defined( WFC_STL )

Win32FoundationClasses::CFileTime::operator COleDateTime() const
{
   WFC_VALIDATE_POINTER( this );
   COleDateTime return_value;

   CopyTo( return_value );

   return( return_value );
}

#endif // WFC_STL

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CFileTime::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   dump_context << TEXT( "a CFileTime at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   dwLowDateTime is  " ) << dwLowDateTime  << TEXT( "\n" );
   dump_context << TEXT( "   dwHighDateTime is " ) << dwHighDateTime << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CFileTime</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="The C++ class that handles Win32 file timestamps.">
</HEAD>

<BODY>

<H1>CFileTime : FILETIME</H1>

$Revision: 34 $<BR>

<HR>

<H2>Description</H2>

This class encapsulates the FILETIME data structure in Win32. The benefit
from doing this is adding capability to convert between tons of other
time types.

<H2>Data Members</H2>
Whatever is in FILETIME.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B><A NAME="CFileTime">CFileTime</A></B>()
<B>CFileTime</B>( const CFileTime&amp; source )
<B>CFileTime</B>( const <A HREF="CSystemTime.htm">CSystemTime</A>&amp; source )
<B>CFileTime</B>( WORD ms_dos_date, WORD ms_dos_time )
<B>CFileTime</B>( const FILETIME * source )
<B>CFileTime</B>( const FILETIME&amp; source )
<B>CFileTime</B>( const SYSTEMTIME * source )
<B>CFileTime</B>( const SYSTEMTIME&amp; source )
<B>CFileTime</B>( const TIMESTAMP_STRUCT * source )
<B>CFileTime</B>( const TIMESTAMP_STRUCT&amp; source )
<B>CFileTime</B>( const DBTIMESTAMP * source )
<B>CFileTime</B>( const DBTIMESTAMP&amp; source )
<B>CFileTime</B>( const LARGE_INTEGER * source )
<B>CFileTime</B>( const LARGE_INTEGER&amp; source )
<B>CFileTime</B>( const ULARGE_INTEGER * source )
<B>CFileTime</B>( const ULARGE_INTEGER&amp; source )</PRE><DD>
These should just about do the job of constructing a new CFileTime.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>LONG <B><A NAME="Compare">Compare</A></B>( const CFileTime&amp; source )</PRE><DD>
Returns the same values as std::wstring::compare().

<DT><PRE>void <B><A NAME="Copy">Copy</A></B>( const CFileTime&amp; source )
void <B>Copy</B>( const <A HREF="CSystemTime.htm">CSystemTime</A>&amp; source )
void <B>Copy</B>( WORD ms_dos_date, WORD ms_dos_time )
void <B>Copy</B>( const FILETIME * source )
void <B>Copy</B>( const FILETIME&amp; source )
void <B>Copy</B>( const SYSTEMTIME * source )
void <B>Copy</B>( const SYSTEMTIME&amp; source )
void <B>Copy</B>( const LARGE_INTEGER * source )
void <B>Copy</B>( const LARGE_INTEGER&amp; source )
void <B>Copy</B>( const ULARGE_INTEGER * source )
void <B>Copy</B>( const ULARGE_INTEGER&amp; source )
void <B>Copy</B>( const TIMESTAMP_STRUCT * source )
void <B>Copy</B>( const TIMESTAMP_STRUCT&amp; source )
void <B>Copy</B>( const DBTIMESTAMP * source )
void <B>Copy</B>( const DBTIMESTAMP&amp; source )</PRE><DD>
Sets the time to what you specify, you can copy
another CFileTime,
<A HREF="CSystemTime.htm">CSystemTime</A>,

<DT><PRE>void <B><A NAME="CopyTo">CopyTo</A></B>( const COleDateTime&amp; source )
void <B>CopyTo</B>( WORD&amp; ms_dos_date, WORD&amp; ms_dos_time )
void <B>CopyTo</B>( const CTime&amp; source )
void <B>CopyTo</B>( const DBTIMESTAMP&amp; source )
void <B>CopyTo</B>( const TIMESTAMP_STRUCT&amp; source )</PRE><DD>
Copies the contents of this object to another objects.

<DT><PRE>void <B><A NAME="Empty">Empty</A></B>( void )</PRE><DD>Clears the data members.

<DT><PRE>void <B><A NAME="FillWithCurrentTime">FillWithCurrentTime</A></B>( void )</PRE><DD>
Gets the current time on the machine.

</DL>

<H2>Operators</H2>

<DL COMPACT>

<DT><PRE>BOOL operator <B>==</B> ( const CFileTime&amp; source ) const</PRE></DD>
Compares this file time with another.

<DT><PRE>BOOL operator <B>&lt;</B> ( const CFileTime&amp; source ) const</PRE></DD>
Compares this file time with another.

<DT><PRE>BOOL operator <B>&gt;</B> ( const CFileTime&amp; source ) const</PRE></DD>
Compares this file time with another.

<DT><PRE>CFileTime&amp; operator <B>=</B> ( const CFileTime&amp; source )
CFileTime&amp; operator <B>=</B> ( const CSystemTime&amp; source )
CFileTime&amp; operator <B>=</B> ( const LARGE_INTEGER&amp; source )
CFileTime&amp; operator <B>=</B> ( const ULARGE_INTEGER&amp; source )
CFileTime&amp; operator <B>=</B> ( const DBTIMESTAMP&amp; source )
CFileTime&amp; operator <B>=</B> ( const TIMESTAMP_STRUCT&amp; source )</PRE></DD>
Basically calls <B><A HREF="#Copy">Copy()</A></B>.

<DT><PRE>operator <B>LARGE_INTEGER</B> ( void ) const</PRE></DD>
Returns the filetime as a LARGE_INTEGER.

<DT><PRE>operator <B>ULARGE_INTEGER</B> ( void ) const</PRE></DD>
Returns the filetime as a ULARGE_INTEGER.

<DT><PRE>operator <B>COleDateTime</B> ( void ) const</PRE></DD>
Returns the filetime as a COleDateTime.

</DL>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<H2>API's Used</H2>

<B>CFileTime</B> uses the following API's:

<UL>
<LI>CoFileTimeNow
<LI>CoFileTimeToDosDateTime
<LI>CompareFileTime
<LI>DosDateTimeToFileTime
<LI>FileTimeToSystemTime
<LI>SystemTimeToFileTime
</UL>

<HR><I>Copyright, 2000, Samuel R. Blackburn</I><BR>
$Workfile: CFileTime.CPP $<BR>
$Modtime: 6/26/01 10:46a $
</BODY>

</HTML>
*/
