/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 1998-2003, 2019, Samuel R. Blackburn
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

/* SPDX-License-Identifier: BSD-2-Clause */

#include "XMLCheck.h"
#pragma hdrstop

#if defined( _DEBUG )
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   // WFCTRACELEVELON( 31 );
   WFCTRACEINIT( TEXT( "_tmain()" ) );

   if ( number_of_command_line_arguments < 2 )
   {
      _tprintf( TEXT( "Usage: xml_file [xml_file [xml_file [...]]]\n" ) );
      return( EXIT_SUCCESS );
   }

   WIN32_FIND_DATAW find_data;

   std::error_code filesystem_error_code;

   for( auto const index : Range(number_of_command_line_arguments, StartingRangePosition(1)) )
   {
       if (std::filesystem::exists(command_line_arguments[index], filesystem_error_code) == true)
       {
           if (std::filesystem::is_directory(command_line_arguments[index], filesystem_error_code) == false)
           {
               check_xml_file(command_line_arguments[index]);
           }
           else
           {
               std::vector<std::filesystem::path> filenames;

               for (auto const& entry : std::filesystem::recursive_directory_iterator(command_line_arguments[index], filesystem_error_code))
               {
                   if (std::filesystem::is_directory(entry, filesystem_error_code) == false and
                       std::filesystem::file_size(entry, filesystem_error_code) > 0) // Skip empty files, pffexport likes to write these...
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

           auto const find_file_handle{ ::FindFirstFileW(command_line_arguments[index], &find_data) };

           if (find_file_handle not_eq INVALID_HANDLE_VALUE)
           {
               std::filesystem::path full_path(command_line_arguments[index]);

               full_path.append(find_data.cFileName);

               check_xml_file(full_path.c_str());

               while (::FindNextFile(find_file_handle, &find_data) != FALSE)
               {
                   check_xml_file(find_data.cFileName);
               }

               ::FindClose(find_file_handle);
           }
       }
   }

   return( EXIT_SUCCESS );
}
