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
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

BOOL directory_read_callback_function( void * , const CString& filename )
{
   //check_xml_file( filename );
   return( TRUE );
}

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   // WFCTRACELEVELON( 31 );
   WFCTRACEINIT( TEXT( "_tmain()" ) );

   if ( number_of_command_line_arguments < 2 )
   {
      _tprintf( TEXT( "Usage: xml_file [xml_file [xml_file [...]]]\n" ) );
      return( EXIT_SUCCESS );
   }

#if 0

   CFileDirectory directory;

   int index = 1;

   while( index < number_of_command_line_arguments )
   {
      if ( directory.Open( command_line_arguments[ index ] ) != FALSE )
      {
         directory.ReadRecursively( directory_read_callback_function, NULL );
      }
      else
      {
         _tprintf( TEXT( "Can't open directory \"%s\"\n" ), command_line_arguments[ index ] );
      }

      index++;
   }

#else

   HANDLE find_file_handle = (HANDLE) NULL;

   WIN32_FIND_DATAW find_data;

   std::error_code filesystem_error_code;

   for( auto const index : Range1(number_of_command_line_arguments) )
   {
       if (filesystem::exists(command_line_arguments[index], filesystem_error_code) == true)
       {
           if (filesystem::is_directory(command_line_arguments[index], filesystem_error_code) == false)
           {
               check_xml_file(command_line_arguments[index]);
           }
           else
           {
               std::vector<filesystem::path> filenames;

               for (auto const& entry : filesystem::recursive_directory_iterator(command_line_arguments[index], filesystem_error_code))
               {
                   if (filesystem::is_directory(entry, filesystem_error_code) == false &&
                       filesystem::file_size(entry, filesystem_error_code) > 0) // Skip empty files, pffexport likes to write these...
                   {
                       filenames.push_back(entry.path());
                   }
               }

               // Now check the files
               for (auto const& filename : filenames)
               {
                   check_xml_file(filename.c_str());
               }
           }
       }
       else
       {
           ZeroMemory(&find_data, sizeof(find_data));

           find_file_handle = FindFirstFileW(command_line_arguments[index], &find_data);

           if (find_file_handle != INVALID_HANDLE_VALUE)
           {
               filesystem::path full_path(command_line_arguments[index]);

               full_path.append(find_data.cFileName);

               check_xml_file(full_path.c_str());

               while (FindNextFile(find_file_handle, &find_data) != FALSE)
               {
                   check_xml_file(find_data.cFileName);
               }

               FindClose(find_file_handle);
           }
       }
   }

#endif

   return( EXIT_SUCCESS );
}
