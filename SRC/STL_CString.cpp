/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2014, Samuel R. Blackburn
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
** $Workfile: STL_CString.cpp $
** $Revision: 16 $
** $Modtime: 1/05/02 7:21a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( WFC_STL )

// Get rid of a ton of warnings

#pragma warning( disable : 4710 4702 )

// Now start picking up the STL stuff

#include <string>
#include <vector>
#include <algorithm>

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void Win32FoundationClasses::CString::m_Format( __in_z __format_string LPCTSTR format_string, va_list argument_list ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( format_string );

   WFC_TRY
   {
      va_list saved_argument_list;

      long buffer_size = (long) ( _tcslen( format_string ) * 2 );

      Win32FoundationClasses::CString format_object( format_string );

      SSIZE_T string_index = 0;

      while( ( string_index = format_object.Find( TEXT( '%' ), string_index ) ) != CHARACTER_NOT_FOUND )
      {
         string_index++;
         // Add 128 bytes for every % in the string
         buffer_size += 128;
      }

      TCHAR * buffer_to_write_to = nullptr;
      
      WFC_TRY
      {
         buffer_to_write_to = (TCHAR *) malloc( ( buffer_size + 1 ) * sizeof( TCHAR ) );

          if ( buffer_to_write_to == nullptr )
          {
            va_end( saved_argument_list );
            return;
          }

         bool exit_loop = false;

         while( exit_loop == false )
         {
            saved_argument_list = argument_list;

            if ( _vsntprintf_s( buffer_to_write_to, buffer_size, _TRUNCATE, format_string, saved_argument_list ) == (-1) )
            {
               buffer_size *= 2;

               free( buffer_to_write_to );
               buffer_to_write_to = nullptr;

               buffer_to_write_to = (TCHAR *) malloc( ( buffer_size + 1 ) * sizeof( TCHAR ) );

               if ( buffer_to_write_to == nullptr )
               {
                  va_end( saved_argument_list );
                  return;
               }
            } 
            else
            {
               exit_loop = true;
            }
         }

         va_end( saved_argument_list );

         // Now copy the string into our buffer

         m_String.assign( buffer_to_write_to );
         free( buffer_to_write_to );
         buffer_to_write_to = nullptr;
      }
      WFC_CATCH_ALL
      {
         WFC_TRY
         {
            if ( buffer_to_write_to != nullptr )
            {
               free( buffer_to_write_to );
               buffer_to_write_to = nullptr;
            }
         }
         WFC_CATCH_ALL
         {
         }
         WFC_END_CATCH_ALL
      }
      WFC_END_CATCH_ALL
   }
   WFC_CATCH_ALL
   {
   }
   WFC_END_CATCH_ALL
}

// Methods

void __cdecl Win32FoundationClasses::CString::Format( __in_z __format_string LPCTSTR printf_format_string, ... ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( printf_format_string );

   WFC_TRY
   {
      va_list argument_list;

      va_start( argument_list, printf_format_string );
      m_Format( printf_format_string, argument_list );
      va_end( argument_list );
   }
   WFC_CATCH_ALL
   {
   }
   WFC_END_CATCH_ALL
}

//void TrimRight( LPCTSTR string1 )

#endif // WFC_STL
