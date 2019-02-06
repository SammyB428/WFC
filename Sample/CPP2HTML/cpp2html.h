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

#if ! defined( CPP_TO_HTML_HEADER_FILE_INCLUDED )

#define CPP_TO_HTML_HEADER_FILE_INCLUDED

#define WFC_STL

#if ! defined( WINVER )
#define WINVER 0x401
#endif

#include <wfc.h>

USING_WFC_NAMESPACE

inline void htmlize( const char * input_string, char * output_string )
{
   long input_string_index = 0;
   long output_string_index = 0;

   output_string[ 0 ] = 0;
   output_string[ 1 ] = 0;

   while( input_string[ input_string_index ] != 0 )
   {
      output_string[ output_string_index ] = 0;

      if ( input_string[ input_string_index ] == '<' )
      {
         // less-than translates to &lt;
         output_string[ output_string_index++ ] = '&';
         output_string[ output_string_index++ ] = 'l';
         output_string[ output_string_index++ ] = 't';
         output_string[ output_string_index++ ] = ';';
      }
      else if ( input_string[ input_string_index ] == '>' )
      {
         // greater-than translates to &gt;
         output_string[ output_string_index++ ] = '&';
         output_string[ output_string_index++ ] = 'g';
         output_string[ output_string_index++ ] = 't';
         output_string[ output_string_index++ ] = ';';
      }
      else if ( input_string[ input_string_index ] == '&' )
      {
         // ampersand translates to &amp;
         output_string[ output_string_index++ ] = '&';
         output_string[ output_string_index++ ] = 'a';
         output_string[ output_string_index++ ] = 'm';
         output_string[ output_string_index++ ] = 'p';
         output_string[ output_string_index++ ] = ';';
      }
      else if ( input_string[ input_string_index ] == '\"' )
      {
         // quote translates to &quot;
         output_string[ output_string_index++ ] = '&';
         output_string[ output_string_index++ ] = 'q';
         output_string[ output_string_index++ ] = 'u';
         output_string[ output_string_index++ ] = 'o';
         output_string[ output_string_index++ ] = 't';
         output_string[ output_string_index++ ] = ';';
      }
      else
      {
         output_string[ output_string_index++ ] = input_string[ input_string_index ];
      }

      output_string[ output_string_index ] = 0;
      input_string_index++;
   }
}

#endif // CPP_TO_HTML_HEADER_FILE_INCLUDED
