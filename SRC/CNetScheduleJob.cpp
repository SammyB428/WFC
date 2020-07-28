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
** $Workfile: CNetScheduleJob.cpp $
** $Revision: 12 $
** $Modtime: 6/26/01 10:49a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

/*
** CNetworkScheduleJob stuff
*/

Win32FoundationClasses::CNetworkScheduleJob::CNetworkScheduleJob()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CNetworkScheduleJob::CNetworkScheduleJob( _In_ AT_ENUM const * source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CNetworkScheduleJob::CNetworkScheduleJob( _In_ AT_INFO const * source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CNetworkScheduleJob::CNetworkScheduleJob( _In_ Win32FoundationClasses::CNetworkScheduleJob const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CNetworkScheduleJob::~CNetworkScheduleJob()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void Win32FoundationClasses::CNetworkScheduleJob::Copy( _In_ AT_ENUM const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      m_Initialize();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      ID                               = source->JobId;
      NumberOfMillisecondsFromMidnight = (DWORD) source->JobTime;
      DaysOfTheMonth                   = source->DaysOfMonth;
      DaysOfTheWeek                    = source->DaysOfWeek;
      Flags                            = source->Flags;

#if defined( UNICODE )
      Command.assign(source->Command);
#else

      // These strings are UNICODE.
      WCHAR unicode_string[ 4096 ];

      ZeroMemory( unicode_string, sizeof( unicode_string ) );

      wcsncpy_s( unicode_string, std::size( unicode_string ), source->Command, std::size( unicode_string ) - 1 );

      char ascii_string[ 4096 ];

      ZeroMemory( ascii_string, sizeof( ascii_string ) );

      ::UNICODE_to_ASCII( unicode_string, ascii_string );

      Command = ascii_string;
#endif // UNICODE
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
      return;
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkScheduleJob::Copy( _In_ AT_INFO const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      m_Initialize();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      ID                               = 0;
      NumberOfMillisecondsFromMidnight = (DWORD) source->JobTime;
      DaysOfTheMonth                   = source->DaysOfMonth;
      DaysOfTheWeek                    = source->DaysOfWeek;
      Flags                            = source->Flags;

#if defined( UNICODE )
      Command.assign(source->Command);
#else

      // These strings are UNICODE.
      WCHAR unicode_string[ 4096 ];

      ZeroMemory( unicode_string, sizeof( unicode_string ) );

      wcsncpy_s( unicode_string, std::size( unicode_string ), source->Command, std::size( unicode_string ) - 1 );

      char ascii_string[ 4096 ];

      ZeroMemory( ascii_string, sizeof( ascii_string ) );

      ::UNICODE_to_ASCII( unicode_string, ascii_string );

      Command = ascii_string;
#endif // UNICODE
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
      return;
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkScheduleJob::Copy( _In_ Win32FoundationClasses::CNetworkScheduleJob const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this not_eq &source );

   /*
   ** Make sure we ain't copying ourselves
   */

   if ( this == &source )
   {
      return;
   }

   ID                               = source.ID;
   NumberOfMillisecondsFromMidnight = source.NumberOfMillisecondsFromMidnight;
   DaysOfTheMonth                   = source.DaysOfTheMonth;
   DaysOfTheWeek                    = source.DaysOfTheWeek;
   Flags                            = source.Flags;
   Command.assign(source.Command);
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CNetworkScheduleJob::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );

   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   ID                               = " ) << ID                               << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfMillisecondsFromMidnight = " ) << NumberOfMillisecondsFromMidnight << TEXT( "\n" );
   dump_context << TEXT( "   DaysOfTheMonth                   = " ) << DaysOfTheMonth                   << TEXT( " ( " );

   std::wstring temp_string;
   std::wstring number_string;

   for( auto const index : Range(32) )
   {
      if ( bit_test( DaysOfTheMonth, index ) == 1 )
      {
         number_string.Format( TEXT( "%zu, " ), index + 1 );
         temp_string += number_string;
      }
   }

   dump_context << temp_string << TEXT( ")\n" );

   dump_context << TEXT( "   DaysOfTheWeek                    = " ) << DaysOfTheWeek                    << TEXT( " (" );

   temp_string.Empty();

   if ( bit_test( DaysOfTheWeek, 0 ) == 1 )
   {
      temp_string += TEXT( "Monday" );
   }

   if ( bit_test( DaysOfTheWeek, 1 ) == 1 )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( ", " );
      }

      temp_string += TEXT( "Tuesday" );
   }

   if ( bit_test( DaysOfTheWeek, 2 ) == 1 )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( ", " );
      }

      temp_string += TEXT( "Wednesday" );
   }

   if ( bit_test( DaysOfTheWeek, 3 ) == 1 )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( ", " );
      }

      temp_string += TEXT( "Thursday" );
   }

   if ( bit_test( DaysOfTheWeek, 4 ) == 1 )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( ", " );
      }

      temp_string += TEXT( "Friday" );
   }

   if ( bit_test( DaysOfTheWeek, 5 ) == 1 )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( ", " );
      }

      temp_string += TEXT( "Saturday" );
   }

   if ( bit_test( DaysOfTheWeek, 6 ) == 1 )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( ", " );
      }

      temp_string += TEXT( "Sunday" );
   }

   dump_context << temp_string << TEXT( " )\n" );

   dump_context << TEXT( "   Flags                            = " ) << Flags                            << TEXT( " ( " );

   if ( Flags bitand JOB_RUN_PERIODICALLY )
   {
      temp_string = TEXT( "JOB_RUN_PERIODICALLY" );
   }

   if ( Flags bitand JOB_EXEC_ERROR )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "JOB_EXEC_ERROR" );
   }

   if ( Flags bitand JOB_RUNS_TODAY )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "JOB_RUNS_TODAY" );
   }

   if ( Flags bitand JOB_ADD_CURRENT_DATE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "JOB_ADD_CURRENT_DATE" );
   }

   if ( Flags bitand JOB_NONINTERACTIVE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "JOB_NONINTERACTIVE" );
   }

   dump_context << temp_string << TEXT( " )" );

   temp_string.Empty();

   if ( Flags == JOB_INPUT_FLAGS )
   {
      dump_context << TEXT( " also known as JOB_INPUT_FLAGS" );
   }

   if ( Flags == JOB_OUTPUT_FLAGS )
   {
      dump_context << TEXT( " also known as JOB_OUTPUT_FLAGS" );
   }

   dump_context << TEXT( "\n" );

   dump_context << TEXT( "   Command                          = " ) << Command                          << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CNetworkScheduleJob::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void Win32FoundationClasses::CNetworkScheduleJob::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ID                               = 0;
   NumberOfMillisecondsFromMidnight = 0;
   DaysOfTheMonth                   = 0;
   DaysOfTheWeek                    = 0;
   Flags                            = 0;
   Command.clear();
}

Win32FoundationClasses::CNetworkScheduleJob const& Win32FoundationClasses::CNetworkScheduleJob::operator = ( _In_ Win32FoundationClasses::CNetworkScheduleJob const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this not_eq &source );

   if ( this not_eq &source )
   {
      Copy( source );
   }

   return( *this );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CNetworkScheduleJob</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, at, winat, source code">
<META name="description" content="The C++ class that lets you schedule programs to run at certain times.">
</HEAD>

<BODY>
<H1>CNetworkScheduleJob : CObject</H1>
$Revision: 12 $<BR>
<HR>

<H2>Description</H2>
This handles the same information as AT_ENUM and AT_INFO.
It is used to schedule jobs that run unattended.

<H2>Construction</H2>

<DL COMPACT>

<DT><PRE><B>CNetworkScheduleJob</B>()
<B>CNetworkScheduleJob</B>( const AT_ENUM * source )
<B>CNetworkScheduleJob</B>( const AT_INFO * source )
<B>CNetworkScheduleJob</B>( const CNetworkScheduleJob&amp; source )</PRE><DD>
Constructs the object.

</DL>

<H2>Data Members</H2>

<DL COMPACT>

<DT><PRE>DWORD <B>ID</B></PRE><DD>
Contains the job identifier.

<DT><PRE>DWORD <B>NumberOfMillisecondsFromMidnight</B></PRE><DD>
Contains the number of milliseconds from midnight that the job
will execute.

<DT><PRE>DWORD <B>DaysOfTheMonth</B></PRE><DD>
A bitmapped field of the days of the month when the job
will execute. Bit zero corresponds to the first day of the month.

<DT><PRE>DWORD <B>DaysOfTheWeek</B></PRE><DD>
A bitmapped field of the days of the week when the job
will execute. Bit zero corresponds to Monday.

<DT><PRE>DWORD <B>Flags</B></PRE><DD>
A bitmapped field describing job properties.

<DT><PRE>std::wstring <B>Command</B></PRE><DD>
The actual command line that will be executed.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Copy">Copy</A></B>( const AT_ENUM * source )
void <B>Copy</B>( const AT_INFO * source )
void <B>Copy</B>( const CNetworkScheduleJob&amp; source )</PRE><DD>
Copies the contents of source.

<DT><PRE>void <B><A NAME="Dump">Dump</A></B>( CDumpContext&amp; dump_context ) const</PRE><DD>
Present only in debug builds. Dumps useful information to the 
debugging window.

<DT><PRE>void <B><A NAME="Empty">Empty</A></B>( void )</PRE><DD>
Zeroizes the object.

<DT><PRE>void <B>Serialize</B>( CArchive&amp; archive )</PRE><DD>
After all we are a CObject...

</DL>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<H2>API's Used</H2>

<B>CNetworkScheduleJob</B> encapsulates the following data structures:

<UL>
<LI>AT_ENUM
<LI>AT_INFO
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CNetScheduleJob.cpp $<BR>
$Modtime: 6/26/01 10:49a $
</BODY>

</HTML>
*/
