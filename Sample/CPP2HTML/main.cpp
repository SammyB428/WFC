/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1997-2003, Samuel R. Blackburn
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
*/

#include "cpp2html.h"
#pragma hdrstop

void main( void )
{
   WFCTRACEINIT( TEXT( "main()" ) );

   CFile64 input_file;

   TCHAR * output_string = new TCHAR[ 1024 * 1024 ];

   if ( input_file.Open( TEXT( "\\wfc\\src\\ccomsucks.cpp" ), CFile::modeRead ) != FALSE )
   {
      _tprintf( TEXT( "<HTML><HEAD><TITLE>CComSucks.cpp</TITLE></HEAD><BODY>" ) );
      _tprintf( TEXT( "<H1>CComSucks.cpp</H1><HR><PRE>" ) );

      long line_number = 1;

      CString input_string;

      while( input_file.Read( input_string ) != FALSE )
      {
         _tprintf( TEXT( "<A NAME=\"%03ld\">%3ld</A>: " ), line_number, line_number );
         htmlize( input_string, output_string );
         _tprintf( TEXT( "%s\n" ), output_string );
         line_number++;
      }

      /*
      // For every line, there must be a symbol processing function
      // all symbols are universal
      // for each line, parse symbols, for each symbol, generate a link to the
      // show usage summary... kick out static html page
      // all output is static pages, no database required
      // must have a global symbol table that contains links to all usage points (line numbers)
      // must have a file object, give summary such as numnber of numeric constants, number of symbols...
      // two pass system, that way i can eliminate the single-use symbols.
      */

      _tprintf( TEXT( "</PRE><HR></BODY></HTML>" ) );
   }
   else
   {
      WFCTRACE( TEXT( "Can't open input file." ) );
   }

   delete [] output_string;
   output_string = NULL;
}
