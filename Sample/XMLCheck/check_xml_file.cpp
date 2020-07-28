/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 1998-2003, 2013, Samuel R. Blackburn
** All rights reserved.
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
** $Workfile: save_document.cpp $
** $Revision: 1 $
** $Modtime: 3/23/98 7:09p $
*/

#include "XMLCheck.h"
#pragma hdrstop

#if defined( _DEBUG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void check_xml_file( wchar_t const * filename ) noexcept
{
   WFCTRACEINIT( TEXT( "check_xml_file()" ) );
   //WFCTRACEVAL( TEXT( "Checking " ), filename );
   // d:\WFC\test\xml\xmltest\not-wf\sa

   Win32FoundationClasses::CExtensibleMarkupLanguageDocument document;

   auto parsing_options = document.GetParseOptions();

   parsing_options |= WFC_XML_ALLOW_REPLACEMENT_OF_DEFAULT_ENTITIES;

   document.SetParseOptions( parsing_options );

   Win32FoundationClasses::CFile64 file;

   if ( file.Open( filename, read_open_mode()) == FALSE )
   {
      WFCTRACEVAL( TEXT( "Can't open " ), filename );
      WFCTRACEERROR( GetLastError() );
      _tprintf( TEXT( "Can't open %s\n" ), (LPCTSTR) filename );
      return;
   }

   std::vector<uint8_t> bytes;

   bytes.resize( file.GetLength() );

   file.Read( bytes.data(), (uint32_t) bytes.size() );
   file.Close();

   Win32FoundationClasses::CDataParser parser;

   parser.Initialize( &bytes, false );

   Win32FoundationClasses::CSystemTime begin_parsing;
   Win32FoundationClasses::CSystemTime end_parsing;

   uint64_t start_tick_count = 0;
   uint64_t end_tick_count   = 0;

   BOOL should_parse_fail = FALSE;

   begin_parsing.GetUTC();
   start_tick_count = GetTickCount64();

   WFCTRACELEVELON( 31 );

   if ( document.Parse( parser ) != FALSE )
   {
      // We parsed OK

      end_tick_count = GetTickCount64();
      end_parsing.GetUTC();

#if defined( _DEBUG )

      if ( should_parse_fail != FALSE )
      {
         // We should not have been able to parse this XML.
         ASSERT( FALSE );
      }

#endif // _DEBUG

      //WFCTRACEVAL( TEXT( "XML parsed OK " ), filename );
      _tprintf( TEXT( "\"%s\" parsed OK\n" ), (LPCTSTR) filename );

      Win32FoundationClasses::CTime start_time;
      Win32FoundationClasses::CTime end_time;

      begin_parsing.CopyTo( start_time );
      end_parsing.CopyTo( end_time );

      Win32FoundationClasses::CTimeSpan time_span;
      
      time_span = end_time - start_time;

      if ( time_span.GetTotalSeconds() == 0 )
      {
         // Less than a second!

         uint64_t const number_of_milliseconds = end_tick_count - start_tick_count;

         //WFCTRACEVAL( TEXT( "Parse time in Milliseconds is " ), number_of_milliseconds );
         _tprintf( TEXT( "It took %" PRIu64 " milliseconds to parse.\n\n" ), number_of_milliseconds );
      }
      else
      {
          uint64_t const number_of_milliseconds = end_tick_count - start_tick_count;
         //WFCTRACEVAL( TEXT( "Parse time in seconds is " ), time_span.GetTotalSeconds() );
         _tprintf( TEXT( "It took %" PRIu32 " seconds (%" PRIu64 " milliseconds) to parse %zu elements (%zu bytes).\n\n" ),
                  (uint32_t) time_span.GetTotalSeconds(),
                  number_of_milliseconds,
                  document.GetNumberOfElements(),
                  bytes.size() );
      }

#if 0

      // Make sure we write out what we read in

      CFile output_file;

      if ( output_file.Open( TEXT( "d:\\temp\\check.out" ), CFile::modeCreate | CFile::modeWrite ) == FALSE )
      {
         WFCTRACE( TEXT( "Can't open check.out" ) );
      }
      else
      {
         CByteArray xml_output;

         document.WriteTo( xml_output );

         output_file.Write( xml_output.GetData(), xml_output.GetSize() );

         WFCTRACEVAL( TEXT( "Wrote to " ), output_file.GetFilePath() );

         output_file.Close();
      }

#if 0

      CExtensibleMarkupLanguageElement * element_p = document.GetElement( TEXT( "ovvoba.vergabenummern.gesamtvergabelosnummer" ) );

      if ( element_p != NULL )
      {
         CString directory;

         element_p->GetText( directory );

         WFCTRACEVAL( TEXT( "Directory is " ), directory );
      }

#endif // 0

#endif // _DEBUG

      WFCTRACELEVELOFF( 31 );
   }
   else
   {
      WFCTRACEVAL( TEXT( "FAILED! " ), filename );
      _tprintf( TEXT( "FAIL! \"%s\"\n" ), (LPCTSTR) filename );

      std::wstring tag_name;
      std::wstring error_message;

      Win32FoundationClasses::CParsePoint began_at;
      Win32FoundationClasses::CParsePoint failed_at;

      document.GetParsingErrorInformation( tag_name, began_at, failed_at, &error_message );

      _tprintf( TEXT( "Tag is %s\n" ), tag_name.c_str() );
      _tprintf( TEXT( "Reason is %s\n" ), error_message.c_str() );
      _tprintf( TEXT( "Element began at line %" PRIu64 ", column %" PRIu64 " (byte index %" PRIu64 ")\n" ),
                began_at.GetLineNumber(),
                began_at.GetLineIndex(),
                began_at.GetIndex() );

      _tprintf( TEXT( "Error occurred at line %" PRIu64 ", column %" PRIu64 " (byte index %" PRIu64 ")\n\n" ),
          failed_at.GetLineNumber(),
          failed_at.GetLineIndex(),
          failed_at.GetIndex() );

      WFCTRACEVAL( TEXT( "Tag Name is " ), tag_name.c_str() );
      WFCTRACEVAL( TEXT( "Reason is " ), error_message.c_str() );
      WFCTRACEVAL( TEXT( "began at line number " ), began_at.GetLineNumber() );
      WFCTRACEVAL( TEXT( "began at column " ), began_at.GetLineIndex() );
      WFCTRACEVAL( TEXT( "began at byte index " ), began_at.GetIndex() );
      WFCTRACEVAL( TEXT( "err'd at line number " ), failed_at.GetLineNumber() );
      WFCTRACEVAL( TEXT( "err'd at column " ), failed_at.GetLineIndex() );
      WFCTRACEVAL( TEXT( "err'd at byte index " ), failed_at.GetIndex() );
   }
}

/*
C:\Program Files\Microsoft Visual Studio\VC98\MFC\INCLUDE\afx.inl(27) : fatal error C1001: INTERNAL COMPILER ERROR
  (compiler file 'E:\8168\vc98\p2\src\P2\main.c', line 494)
    Please choose the Technical Support command on the Visual C++
    Help menu, or open the Technical Support help file for more information
Error executing cl.exe.
*/
