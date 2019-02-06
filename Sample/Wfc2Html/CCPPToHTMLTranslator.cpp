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
*/

#include "wfc2html.h"
#pragma hdrstop

CCPPToHTMLTranslator::CCPPToHTMLTranslator()
{
   WFCTRACEINIT( TEXT( "CCPPToHTMLTranslator::CCPPToHTMLTranslator()" ) );
}

CCPPToHTMLTranslator::~CCPPToHTMLTranslator()
{
   WFCTRACEINIT( TEXT( "CCPPToHTMLTranslator::~CCPPToHTMLTranslator()" ) );
}

void CCPPToHTMLTranslator::AddClass( const CString& class_name, const CString& filename )
{
   WFCTRACEINIT( TEXT( "CCPPToHTMLTranslator::AddClass()" ) );
}

void CCPPToHTMLTranslator::m_StripDocumentation( CStringArray& html ) const
{
   WFCTRACEINIT( TEXT( "CCPPToHTMLTranslator::m_StripDocumentation()" ) );

   int index            = 0;
   int index_of_if_zero = 0;
   int number_of_lines  = html.GetSize();

   BOOL if_zero_found = FALSE;

   CString if_zero_line( TEXT( "#if 0" ) );
   CString start_tag( TEXT( "<WFC_DOCUMENTATION>" ) );
   CString line;

   while( index < number_of_lines )
   {
      line = html.GetAt( index );

      line.TrimRight();

      if ( line.GetLength() >= if_zero_line.GetLength() )
      {
         if ( if_zero_line.CompareNoCase( line.Left( if_zero_line.GetLength() ) ) == 0 )
         {
            if_zero_found    = TRUE;
            index_of_if_zero = index;
         }
         else if ( line.GetLength() >= start_tag.GetLength() )
         {
            if ( start_tag.CompareNoCase( line.Left( start_tag.GetLength() ) ) == 0 )
            {
               if ( if_zero_found != FALSE )
               {
                  html.RemoveAt( index_of_if_zero, html.GetSize() - index_of_if_zero );
                  return;
               }
            }
         }
         else
         {
            if_zero_found    = FALSE;
            index_of_if_zero = html.GetUpperBound();
         }
      }

      index++;
   }
}

void CCPPToHTMLTranslator::m_TranslateCharacters( CStringArray& html ) const
{
   WFCTRACEINIT( TEXT( "CCPPToHTMLTranslator::m_TranslateCharacters()" ) );

   CString line;
   CString temp_string;

   int index           = 0;
   int number_of_lines = html.GetSize();
   int location        = 0;

   BOOL exit_loop       = FALSE;
   BOOL change_was_made = FALSE;

   while( index < number_of_lines )
   {
      line = html.GetAt( index );

      exit_loop       = FALSE;
      change_was_made = FALSE;

      exit_loop = FALSE;

      while( exit_loop == FALSE )
      {
         location = line.Find( TEXT( '&' ) );

         if ( location != (-1) )
         {
            temp_string  = line.Left( location );
            temp_string += TEXT( "`#038;" );
            temp_string += line.Right( line.GetLength() - ( location + 1 ) );
            line = temp_string;
            change_was_made = TRUE;
         }
         else
         {
            exit_loop = TRUE;
         }
      }

      while( exit_loop == FALSE )
      {
         location = line.Find( TEXT( '>' ) );

         if ( location != (-1) )
         {
            temp_string  = line.Left( location );
            temp_string += TEXT( "&gt;" );
            temp_string += line.Right( line.GetLength() - ( location + 1 ) );
            line = temp_string;
            change_was_made = TRUE;
         }
         else
         {
            exit_loop = TRUE;
         }
      }

      exit_loop = FALSE;

      while( exit_loop == FALSE )
      {
         location = line.Find( TEXT( '<' ) );

         if ( location != (-1) )
         {
            temp_string  = line.Left( location );
            temp_string += TEXT( "&lt;" );
            temp_string += line.Right( line.GetLength() - ( location + 1 ) );
            line = temp_string;
            change_was_made = TRUE;
         }
         else
         {
            exit_loop = TRUE;
         }
      }

      exit_loop = FALSE;

      while( exit_loop == FALSE )
      {
         location = line.Find( TEXT( '\"' ) );

         if ( location != (-1) )
         {
            temp_string  = line.Left( location );
            temp_string += TEXT( "&quot;" );
            temp_string += line.Right( line.GetLength() - ( location + 1 ) );
            line = temp_string;
            change_was_made = TRUE;
         }
         else
         {
            exit_loop = TRUE;
         }
      }

      if ( change_was_made != FALSE )
      {
         // See if we replaced &'s

         exit_loop = FALSE;

         while( exit_loop == FALSE )
         {
            location = line.Find( TEXT( "`#038;" ) );

            if ( location != (-1) )
            {
               temp_string  = line.Left( location );
               temp_string += TEXT( '&' );
               temp_string += line.Right( line.GetLength() - ( location + 1 ) );
               line = temp_string;
            }
            else
            {
               exit_loop = TRUE;
            }
         }

         html.SetAt( index, line );
      }

      index++;
   }
}

void CCPPToHTMLTranslator::Translate( const CStringArray& cpp_source, CStringArray& html ) const
{
   WFCTRACEINIT( TEXT( "CCPPToHTMLTranslator::Translate()" ) );

   html.Copy( cpp_source );

   m_StripDocumentation( html );
   m_TranslateCharacters( html );

   html.InsertAt( 0, TEXT( "<PRE><CODE>" ) );
   html.InsertAt( 0, TEXT( "</HEAD><BODY>" ) );
   html.InsertAt( 0, TEXT( "<META NAME=\"GENERATOR\" CONTENT=\"WFC2HTML\">" ) );
   html.InsertAt( 0, TEXT( "<HEAD>" ) );
   html.InsertAt( 0, TEXT( "<HTML>" ) );
   html.Add( TEXT( "</CODE></PRE>" ) );
   html.Add( TEXT( "</BODY></HTML>" ) );
}

#if 0
<WFC_DOCUMENTATION>
<HTML>
<HEAD><TITLE>WFC - CMixer</TITLE></HEAD>
<BODY>
<H1>CMixer : CObject</H1>
$Revision: 12 $<BR><HR>
<H2>Description</H2>
This is the class handles general mixer stuff. It retrieves things
like mixer capabilities, opening and closing.
<H2>Data Members</H2>None.
<H2>Methods</H2>
<DL COMPACT>
<DT><B>Close</B><DD>Closes the connection to the mixer device.
<DT><B>Get</B><DD>Retrieves the mixer's capabilities via CMixerCapabilities.
<DT><B>GetAllControls</B><DD>
<DT><B>GetByComponent</B><DD>
<DT><B>GetByConnection</B><DD>
<DT><B>GetByDestination</B><DD>
<DT><B>GetByID</B><DD>
<DT><B>GetControlDetails</B><DD>
<DT><B>GetControlListText</B><DD>
<DT><B>GetDeviceID</B><DD>
<DT><B>GetErrorCode</B><DD>
<DT><B>GetErrorString</B><DD>
<DT><B>GetHandle</B><DD>
<DT><B>GetNumberOfDevices</B><DD>
<DT><B>Open</B><DD>
<DT><B>Serialize</B><DD>
<DT><B>SetControlDetails</B><DD>
</DL>
<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>
<H2>API's Used</H2>
<UL>
<LI>mixerClose
<LI>mixerGetControlDetails
<LI>mixerGetDevCaps
<LI>mixerGetLineControls
<LI>mixerGetLineInfo
<LI>mixerGetNumDevs
<LI>mixerOpen
<LI>mixerSetControlDetails
</UL>
<I>Copyright, 1997, Samuel R. Blackburn</I><BR>
$Workfile: CMixer.cpp $<BR>
$Modtime: 8/03/97 10:13p $
</BODY>
</HTML>
</WFC_DOCUMENTATION>
#endif
