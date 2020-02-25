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
** $Workfile: CEventLogRecord.CPP $
** $Revision: 23 $
** $Modtime: 6/26/01 10:45a $
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

Win32FoundationClasses::CEventLogRecord::CEventLogRecord() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CEventLogRecord::CEventLogRecord( _In_ Win32FoundationClasses::CEventLogRecord const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

Win32FoundationClasses::CEventLogRecord::CEventLogRecord( _In_ EVENTLOGRECORD const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Empty();
   Copy( source );
}

Win32FoundationClasses::CEventLogRecord::~CEventLogRecord() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

_Check_return_ int Win32FoundationClasses::CEventLogRecord::Compare( _In_ Win32FoundationClasses::CEventLogRecord const& source ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   int return_value = I_AM_EQUAL_TO_THAT;

   if ( RecordNumber < source.RecordNumber )
   {
      return_value = I_AM_LESS_THAN_THAT;
   }
   else if ( RecordNumber > source.RecordNumber )
   {
      return_value = I_AM_GREATER_THAN_THAT;
   }

   return( return_value );
}

void Win32FoundationClasses::CEventLogRecord::Copy( _In_ Win32FoundationClasses::CEventLogRecord const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   Source.assign( source.Source );
   ComputerName.assign( source.ComputerName );
   RecordNumber        = source.RecordNumber;
   Reserved            = source.Reserved;
   TimeGenerated       = source.TimeGenerated;
   TimeWritten         = source.TimeWritten;
   EventID             = source.EventID;
   EventType           = source.EventType;
   EventCategory       = source.EventCategory;
   PairedEventFlags    = source.PairedEventFlags;
   ClosingRecordNumber = source.ClosingRecordNumber;
   UserSID = source.UserSID;
   Data = source.Data;

   Strings.assign( source.Strings.cbegin(), source.Strings.cend() );
}

void Win32FoundationClasses::CEventLogRecord::Copy( _In_ EVENTLOGRECORD  const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   Empty();

   // Choose to live

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      // The EVENTLOGRECORD structure looks like it was designed by one
      // of the grown-ups at Microsoft (i.e. no Hungarian notation crap)

      RecordNumber        = source->RecordNumber;
      Reserved            = source->Reserved;
      TimeGenerated       = CTime( source->TimeGenerated );
      TimeWritten         = CTime( source->TimeWritten   );
      EventID             = source->EventID;
      EventType           = source->EventType;
      EventCategory       = source->EventCategory;
      PairedEventFlags    = source->ReservedFlags;
      ClosingRecordNumber = source->ClosingRecordNumber;

      // Here's where things start to get a little tricky...

      BYTE * byte_p = (BYTE *) source;
      BYTE byte_to_add = 0;

      DWORD here  = source->UserSidOffset;

      for ( auto const index : Range(source->UserSidLength) )
      {
         byte_to_add = byte_p[ here ];
         (void) UserSID.push_back( byte_to_add );
         here++;
      }

      here = source->DataOffset;

      for ( auto const index : Range(source->DataLength) )
      {
         (void) Data.push_back( byte_p[ here ] );
         here++;
      }

      here = sizeof( EVENTLOGRECORD );

      // TCHAR character = 0;

      TCHAR * character_pointer = (TCHAR *) &byte_p[ here ];

      // Next comes the SourceName

      // 1999-10-11, Thanks go to Louis Beauchamp (louisbea@nortelnetworks.com)
      // For finding a UNICODE problem here

      while( *character_pointer != 0 )
      {
         Source.push_back( *character_pointer );
         character_pointer++;
         here += sizeof( TCHAR );
      }

      while( *character_pointer == 0 )
      {
         character_pointer++;
         here += sizeof( TCHAR );
      }

      // ComputerName

      while( *character_pointer != 0 )
      {
         ComputerName.push_back( *character_pointer );
         character_pointer++;
         here += sizeof( TCHAR );
      }

      // Strings

      // Thanks go to Bejan Aminifard (bejan@ix.netcom.com) for
      // finding the bug where I wasn't copying all of the strings
      // in the record. OOPS!

      std::wstring data_string;

      here = source->StringOffset;

      character_pointer = reinterpret_cast<wchar_t *>(&byte_p[ here ]);

      for (auto const number_of_strings : Range(source->NumStrings) )
      {
         data_string.clear();

         while( *character_pointer != 0 )
         {
            data_string.push_back( *character_pointer );
            character_pointer++;
            here += sizeof(wchar_t);
         }

         (void) Strings.push_back( data_string );

         // Advance to the next string

         if ( *character_pointer == 0 and ( ( number_of_strings + 1 ) < source->NumStrings ) )
         {
            while( *character_pointer == 0 )
            {
               character_pointer++;
               here += sizeof(wchar_t);
            }
         }
      }
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CEventLogRecord::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   CObject::Dump( dump_context );

   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   Source              = " ) << Source              << TEXT( "\n" );
   dump_context << TEXT( "   ComputerName        = " ) << ComputerName        << TEXT( "\n" );
   dump_context << TEXT( "   RecordNumber        = " ) << RecordNumber        << TEXT( "\n" );
   dump_context << TEXT( "   Reserved            = " ) << Reserved            << TEXT( "\n" );
   dump_context << TEXT( "   TimeGenerated       = " ) << TimeGenerated       << TEXT( "\n" );
   dump_context << TEXT( "   TimeWritten         = " ) << TimeWritten         << TEXT( "\n" );
   dump_context << TEXT( "   EventID             = " ) << EventID             << TEXT( "\n" );
   dump_context << TEXT( "   EventType           = " ) << EventType           << TEXT( "\n" );
   dump_context << TEXT( "   EventCategory       = " ) << EventCategory       << TEXT( "\n" );
   dump_context << TEXT( "   PairedEventFlags    = " ) << PairedEventFlags    << TEXT( "\n" );
   dump_context << TEXT( "   ClosingRecordNumber = " ) << ClosingRecordNumber << TEXT( "\n" );
   dump_context << TEXT( "   UserSID             = " ) << UserSID             << TEXT( "\n" );
   dump_context << TEXT( "   Data                = " ) << Data                << TEXT( "\n" );
   dump_context << TEXT( "   Strings             = " ) << Strings             << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CEventLogRecord::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Source.clear();
   ComputerName.clear();
   RecordNumber        = 0;
   Reserved            = 0;
   TimeGenerated       = CTime( 0 );
   TimeWritten         = CTime( 0 );
   EventID             = 0;
   EventType           = 0;
   EventCategory       = 0;
   PairedEventFlags    = 0;
   ClosingRecordNumber = 0;
   UserSID.clear();
   Data.clear();
   Strings.clear();
}

_Check_return_ Win32FoundationClasses::CEventLogRecord& Win32FoundationClasses::CEventLogRecord::operator = ( _In_ Win32FoundationClasses::CEventLogRecord const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

_Check_return_ bool Win32FoundationClasses::CEventLogRecord::operator == ( _In_ Win32FoundationClasses::CEventLogRecord const& right_record ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( Compare( right_record ) == I_AM_EQUAL_TO_THAT)
   {
      return( true );
   }
   else
   {
      return( false );
   }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CEventLogRecord</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, event logging, source code">
<META name="description" content="The C++ class that handles event log records.">
</HEAD>

<BODY>

<H1>CEventLogRecord : CObject</H1>
$Revision: 23 $
<HR>

<H2>Description</H2>

This class makes it easy to deal with Event Log records. It is patterened after
the EVENTLOGRECORD data structure.

<H2>Data Members</H2>

<DL COMPACT>

<DT><B>ComputerName</B><DD>Name of the computer where the event occured.
<DT><B>Source</B><DD>Name of the application that generated the event.
<DT><B>RecordNumber</B><DD>Number of the record.
<DT><B>Reserved</B><DD>A special field that Microsoft doesn't know
what to do with.
<DT><B>TimeGenerated</B><DD>Time when the event occured.
<DT><B>TimeWritten</B><DD>Time when the event was written.
<DT><B>EventID</B><DD>ID of the event.
<DT><B>EventType</B><DD>Type of event.
<DT><B>EventCategory</B><DD>The category of event.
<DT><B>PairedEventFlags</B><DD>Flags.
<DT><B>ClosingRecordNumber</B><DD>Used in paired events.
<DT><B>UserSID</B><DD>The user's SID.
<DT><B>Data</B><DD>Any data that was written with the event.
<DT><B>Strings</B><DD>String data associated with the event.
</DL>

<H2>Methods</H2>

<DL COMPACT>
<DT><B><A NAME="Compare">Compare</A></B><DD>Compares this object with another one.
<DT><B><A NAME="Copy">Copy</A></B><DD>Copies another CEventLogRecord or a EVENTLOGRECORD.
<DT><B><A NAME="Empty">Empty</A></B><DD>Sets all data members to an initial state.
<DT><B><A NAME="Serialize">Serialize</A></B><DD>After all, we are a CObject...
</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

void test_CEventLog( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;test_CEventLog()&quot; ) );

   <A HREF="CEVNTLOG.htm">CEventLog</A> log;

   if ( log.Open( TEXT( &quot;WFC&quot; ) ) == FALSE )
   {
      WFCTRACE( TEXT( &quot;Can't Open WFC event log because&quot; ) );
      WFCTRACEERROR( log.GetErrorCode() );
   }

   if ( log.RegisterSource( TEXT( &quot;WFC&quot; ) ) == FALSE )
   {
      WFCTRACE( TEXT( &quot;Can't Open WFC event log because&quot; ) );
      WFCTRACEERROR( log.GetErrorCode() );
   }

   <B>CEventLogRecord</B> record;

   DWORD loop_index = 0;

   while( log.Read( loop_index, record ) == true )
   {
#if defined( _DEBUG )
      record.Dump( afxDump );
#endif // _DEBUG
      loop_index++;
   }
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CEventLogRecord.CPP $<BR>
$Modtime: 6/26/01 10:45a $
</BODY>

</HTML>
*/
