#define WINVER 0x0401
#define _WIN32_WINNT 0x0401
#define WFC_STL
#include <wfc.h>
#pragma hdrstop

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
**
** $Workfile: WeatherChannelWeather.cpp $
** $Revision: 1 $
** $Modtime: 1/02/99 7:20a $
*/

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

USING_WFC_NAMESPACE

void convert_to_bytes( const CStringArray& strings, CByteArray& bytes )
{
   int loop_index        = 0;
   int number_of_strings = strings.GetSize();
   int string_index      = 0;
   int string_length     = 0;

   CString this_string;

   bytes.RemoveAll();

   while( loop_index < number_of_strings )
   {
      this_string = strings.GetAt( loop_index );

      string_index = 0;
      string_length = this_string.GetLength();

      while( string_index < string_length )
      {
         bytes.Add( (BYTE) this_string.GetAt( string_index ) );
         string_index++;
      }

      loop_index++;
   }
}

void parse_WeatherChannel_weather_page( const CStringArray& web_page, CWeather& weather )
{
   WFCTRACEINIT( TEXT( "parse_WeatherChannel_weather_page()" ) );

   CString text( TEXT( "" ) );

   // Start with an empty weather object

   weather.Empty();

   CByteArray bytes;

   convert_to_bytes( web_page, bytes );

   // CDataParser parser( bytes );

   CParsePoint point;
}
