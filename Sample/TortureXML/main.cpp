/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 1996-2003, Samuel R. Blackburn
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
**/

#include "TortureXML.h"
#pragma hdrstop

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   CRandomNumberGenerator2 random;

   DWORD number_of_characters = 100;

   if ( number_of_command_line_arguments > 1 )
   {
      number_of_characters = _ttoi( command_line_arguments[ 1 ] );
   }

   if ( number_of_characters < 1 )
   {
      number_of_characters = 10;
   }

   create_huge_element(                TEXT( "torture001.xml" ), number_of_characters, random );
   create_huge_encoding(               TEXT( "torture002.xml" ), number_of_characters, random );
   create_huge_version(                TEXT( "torture003.xml" ), number_of_characters, random );
   create_huge_element_and_attribute(  TEXT( "torture004.xml" ), number_of_characters, random );
   create_huge_element_name(           TEXT( "torture005.xml" ), number_of_characters, random );
   create_huge_character_reference(    TEXT( "torture006.xml" ), number_of_characters, random );

   return( EXIT_SUCCESS );
}
