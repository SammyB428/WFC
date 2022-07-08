/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2022, Samuel R. Blackburn
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
** $Workfile: CJulianDate.cpp $
** $Revision: 8 $
** $Modtime: 6/26/01 10:47a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#define new DEBUG_NEW
#endif // _DEBUG

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CJulianDate::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CJulianDate at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   m_JulianDays is " ) << m_JulianDays << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ bool Win32FoundationClasses::CJulianDate::Set( _In_ int const year, _In_ int const month, _In_ int const day_parameter, _In_ int const hours, _In_ int const minutes, _In_ int const seconds ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   double x{ 0.0 };

   auto day{ day_parameter };

   if ( year == 1582 )
   {
      if ( month == 10 )
      {
         if ( day > 4 and day < 15 )
         {
            day = 15;
         }
      }
   }

   x = (double) ( ( 12 * ( year + 4800 ) ) + month - 3 );
   m_JulianDays = ( 2 * ( x - ( ::floor( x / 12.0 ) * 12 ) ) + 7 + ( 365 * x ) ) / 12;
   m_JulianDays = ::floor( m_JulianDays ) + day + ::floor( x / 48.0 ) - 32083;

   if ( m_JulianDays > 2299170L )
   {
      m_JulianDays = m_JulianDays + ::floor( x / 4800.0 ) - ::floor( x / 1200.0 ) + 38;
   }

   auto double_hours{ static_cast<double>(hours) };
   auto double_minutes{ static_cast<double>(minutes) };
   auto double_seconds{ static_cast<double>(seconds) };

   double_hours   /= (double) 24.0;
   double_minutes /= (double) 1440.0;
   double_seconds /= (double) 86400.0;

   m_JulianDays += ( ( double_hours + double_minutes + double_seconds ) - 0.5 );

   return( true );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CJulianDate</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="The C++ class that handles dates and times.">
</HEAD>

<BODY>

<H1>CJulianDate</H1>
$Revision: 8 $<BR>
<HR>

<H2>Description</H2>

This class allows you to play with dates.
<STRONG>It is still under development so don&#39;t use it.</STRONG>

<H2>Data Members</H2>
None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="Set">Set</A></B>( int year, int month, int day, int hours, int minutes, int seconds )</PRE><DD>
Sets the date and time of the object.

</DL>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, Samuel R. Blackburn</I><BR>
$Workfile: CJulianDate.cpp $<BR>
$Modtime: 6/26/01 10:47a $

</BODY>
</HTML>
*/
