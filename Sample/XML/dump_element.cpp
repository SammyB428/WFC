/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 1998-2003, Samuel R. Blackburn
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
** $Workfile: dump_element.cpp $
** $Revision: 3 $
** $Modtime: 4/16/98 6:29a $
*/

#include "my_xml.h"
#pragma hdrstop

typedef struct _element_variables
{
   CString name;
   CString type_string;
   DWORD   number_of_children;
   DWORD   indentation_level;
   DWORD   index;
   DWORD   enumerator;
   CExtensibleMarkupLanguageElement * sub_element_p;
}
ELEMENT_VARIABLES, *ELEMENT_VARIABLES_P;

void get_type_string( DWORD type, CString& type_string )
{
   switch( type )
   {
      case CExtensibleMarkupLanguageElement::typeUnknown:

         type_string = TEXT( "typeUnknown" );
         break;

      case CExtensibleMarkupLanguageElement::typeProcessingInstruction:

         type_string = TEXT( "typeProcessingInstruction" );
         break;

      case CExtensibleMarkupLanguageElement::typeComment:

         type_string = TEXT( "typeComment" );
         break;

      case CExtensibleMarkupLanguageElement::typeCharacterData:

         type_string = TEXT( "typeCharacterData" );
         break;

      case CExtensibleMarkupLanguageElement::typeElement:

         type_string = TEXT( "typeElement" );
         break;

      case CExtensibleMarkupLanguageElement::typeTextSegment:

         type_string = TEXT( "typeTextSegment" );
         break;

      case CExtensibleMarkupLanguageElement::typeMetaData:

         type_string = TEXT( "typeMetaData" );
         break;

      default:

         type_string.Format( TEXT( "Invalid value of %lu" ), type );
         break;
   }
}

void make_printable( CString& string_to_print )
{
   int location = 0;

   CString temp_string;

   // Get rid of carriabe returns

   location = string_to_print.Find( CARRIAGE_RETURN );

   while( location != (-1) )
   {
      temp_string = string_to_print.Left( location );
      temp_string += TEXT( "\\n" );
      temp_string += string_to_print.Right( string_to_print.GetLength() - ( location + 1 ) );

      string_to_print = temp_string;

      location = string_to_print.Find( CARRIAGE_RETURN );
   }

   // Now get rid of line feeds

   location = string_to_print.Find( LINE_FEED );

   while( location != (-1) )
   {
      temp_string  = string_to_print.Left( location );
      temp_string += string_to_print.Right( string_to_print.GetLength() - ( location + 1 ) );

      string_to_print = temp_string;

      location = string_to_print.Find( LINE_FEED );
   }

   // Now get rid of TABs (tabs SUCK!)

   location = string_to_print.Find( 0x9 );

   while( location != (-1) )
   {
      temp_string  = string_to_print.Left( location );
      temp_string += TEXT( "\\t" );
      temp_string += string_to_print.Right( string_to_print.GetLength() - ( location + 1 ) );

      string_to_print = temp_string;

      location = string_to_print.Find( 0x9 );
   }
}

void dump_element( CExtensibleMarkupLanguageElement * element_p, DWORD indentation_level )
{
   if ( element_p == NULL )
   {
      return;
   }

   // We are a recursive function! Let's do what we can to conserve stack space

   ELEMENT_VARIABLES_P variables_p = new ELEMENT_VARIABLES;

   if ( variables_p == NULL )
   {
      return;
   }

   variables_p->indentation_level  = indentation_level + 1;
   variables_p->number_of_children = element_p->GetNumberOfChildren();
   variables_p->sub_element_p      = NULL;
   variables_p->enumerator         = 0;

   element_p->GetTag( variables_p->name );

   variables_p->index = 0;

   while( variables_p->index < variables_p->indentation_level )
   {
      _tprintf( TEXT( "   " ) );
      variables_p->index++;
   }

   get_type_string( element_p->GetType(), variables_p->type_string );

   if ( variables_p->number_of_children == 0 )
   {
      _tprintf( TEXT( "\"%s\" (%s)\n" ), (LPCTSTR) variables_p->name, (LPCTSTR) variables_p->type_string );
   }
   else if ( variables_p->number_of_children == 1 )
   {
      _tprintf( TEXT( "\"%s\" (%s) has 1 child\n" ), (LPCTSTR) variables_p->name, (LPCTSTR) variables_p->type_string );
   }
   else
   {
      _tprintf( TEXT( "\"%s\" (%s) has %lu children\n" ), (LPCTSTR) variables_p->name, (LPCTSTR) variables_p->type_string, variables_p->number_of_children );
   }

   element_p->GetText( variables_p->name );

   if ( variables_p->name.GetLength() > 0 )
   {
      variables_p->index = 0;

      while( variables_p->index < variables_p->indentation_level )
      {
         _tprintf( TEXT( "   " ) );
         variables_p->index++;
      }

      make_printable( variables_p->name );

      _tprintf( TEXT( "Text is \"%s\"\n" ), (LPCTSTR) variables_p->name );
   }

   if ( element_p->EnumerateChildren( variables_p->enumerator ) != FALSE )
   {
      while( element_p->GetNextChild( variables_p->enumerator, variables_p->sub_element_p ) != FALSE )
      {
         dump_element( variables_p->sub_element_p, variables_p->indentation_level );
      }
   }

   delete variables_p;
   variables_p = NULL;
}