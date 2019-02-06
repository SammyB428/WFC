/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2003, Samuel R. Blackburn
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
** $Workfile: wfc.h $
** $Revision: 64 $
** $Modtime: 5/04/00 6:57p $
*/

#define WINVER 0x401
#define WFC_STL

#include <wfc.h>
#pragma hdrstop

USING_WFC_NAMESPACE

bool is_script_safe( const CByteArray& script_contents )
{
   if ( script_contents.GetSize() == 0 )
   {
      return( true );
   }

   CString search;

   SIZE_T loop_index = 0;

   while( loop_index < script_contents.GetSize() )
   {
      search += (TCHAR) script_contents.GetAt( loop_index );
      loop_index++;
   }

   // For easy searching, convert everything to lower case

   search.MakeLower();

   // Now look for "dangerous" VBS methods

   CStringArray bad_words;

   bad_words.Add( TEXT( "filesystemobject" ) );
   bad_words.Add( TEXT( "opentextfile" ) );
   bad_words.Add( TEXT( "getspecialfolder" ) );
   bad_words.Add( TEXT( "getfile" ) );
   bad_words.Add( TEXT( "currentversion" ) ); // as in HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
   bad_words.Add( TEXT( "runservices" ) );
   bad_words.Add( TEXT( "start page" ) );
   bad_words.Add( TEXT( "deletefile" ) ); // Deleting a file
   bad_words.Add( TEXT( "createtextfile" ) );
   bad_words.Add( TEXT( "regwrite" ) ); // Writing to the registry
   bad_words.Add( TEXT( "regread" ) ); // Reading my registry
   bad_words.Add( TEXT( "fileexists" ) ); // testing for the existence of a file
   bad_words.Add( TEXT( "addresslists" ) ); // MAPI address book (e-mail)
   bad_words.Add( TEXT( "recipients" ) ); // Sending mail
   bad_words.Add( TEXT( "hkey_local_machine" ) ); // Registry hive
   bad_words.Add( TEXT( "hkey_current_user" ) );

   bool i_think_it_is_bad = false;

   loop_index = 0;

   while( loop_index < bad_words.GetSize() )
   {
      if ( search.Find( bad_words.GetAt( loop_index ) ) != (-1) )
      {
         // The script contains a dangerous word
         i_think_it_is_bad = true;
      }

      loop_index++;
   }

   LPCSTR title_text = "Script may be safe, run it?";

   if ( i_think_it_is_bad == true )
   {
      title_text = "SCRIPT MAY BE HARMFUL!!!!!!! Still Run it?";
   }

   if ( MessageBoxA( NULL,
      (const char *) script_contents.GetData(),
                     title_text,
                     MB_YESNO ) == IDYES )
   {
      return( true );
   }

   return( false );
}

int _tmain( int argc, LPCTSTR argv[] )
{
   int loop_index = 1;

   CFile script_file;

   CByteArray script_contents;

   while( loop_index < argc )
   {
      if ( script_file.Open( argv[ loop_index ], CFile::modeRead ) != FALSE )
      {
         script_contents.SetSize( script_file.GetLength() );
         script_file.Read( script_contents.GetData(), script_contents.GetSize() );
         script_file.Close();
         script_contents.Add( 0 ); // NULL terminate the string
         loop_index = argc;
      }

      loop_index++;
   }

   if ( is_script_safe( script_contents ) == true )
   {
      // The user has determined that the script is safe, let's allow it to run

      // We need to do both CScript.exe and WScript.exe
      // CScript.exe sends text output to the command console
      // WScript.exe sends text output to a GUI window

      CString new_command_line( TEXT( "original_CScript.exe " ) );
      //CString new_command_line( TEXT( "original_WScript.exe " ) );

      loop_index = 1;

      while( loop_index < argc )
      {
         new_command_line += argv[ loop_index ];
         new_command_line += TEXT( " " );

         loop_index++;
      }

      STARTUPINFO startup_information;

      ZeroMemory( &startup_information, sizeof( startup_information ) );
      startup_information.cb = sizeof( startup_information );

      PROCESS_INFORMATION process_information;

      ZeroMemory( &process_information, sizeof( process_information ) );

      TCHAR command_line_string[ 4096 ];

      _tcsncpy( command_line_string, new_command_line, DIMENSION_OF( command_line_string ) );

      if ( CreateProcess( NULL,
                          command_line_string,
                          0,
                          0,
                          1,
                          NORMAL_PRIORITY_CLASS,
                          0,
                          0,
                         &startup_information,
                         &process_information ) != FALSE )
      {
         WaitForSingleObject( process_information.hProcess, INFINITE );
         CloseHandle( process_information.hProcess );
      }
   }

   return( EXIT_SUCCESS );
}
