#include "jc2html.h"
#pragma hdrstop

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
** $Revision: 68 $
** $Modtime: 8/08/00 6:19p $
*/

// Based on Braem Franky's work in his ClassBrowser WTL sample app
// Author : Braem Franky
// e-mail : Franky.Braem17@yucom.be
// web    : www.braem17.yucom.be

void dump_class_file( LPCTSTR filename )
{
   WFCTRACEINIT( TEXT( "dump_class_file()" ) );
   WFCTRACEVAL( TEXT( "Working on " ), CString( filename ) );

   CClassFile java_class_file;

   java_class_file.setFileName( std::wstring( filename ) );
   java_class_file.read();

   std::wstring item;

   if ( java_class_file.isInterface() )
   {
      item += TEXT( "interface " );
   }

   item += java_class_file.getName();

   if ( java_class_file.isSuper() )
   {
      item += L" extends " + java_class_file.getSuper();
   }

   WFCTRACEVAL( TEXT( "" ), CString( item.c_str() ) );
   _tprintf( TEXT( "%s\n" ), (LPCTSTR) item.c_str() );

   // Interfaces

   WFCTRACE( TEXT( "Interfaces:" ) );
   _tprintf( TEXT( "Interfaces:\n" ) );

   StringVector &interfaces = java_class_file.getInterfaces();

   StringVector::iterator itString;

   for( itString = std::begin(interfaces); itString != std::end(interfaces); itString++ )
   {
      item = *itString;
      WFCTRACEVAL( TEXT( "" ), CString( item.c_str() ) );
      _tprintf( TEXT( "%s\n" ), (LPCTSTR) item.c_str() );
   }

   // Innerclasses

   WFCTRACE( TEXT( "Inner Classes:" ) );
   _tprintf( TEXT( "Inner Classes:\n" ) );

   StringVector &innerClasses = java_class_file.getInnerClasses();

   for( itString = std::begin(innerClasses); itString != std::end(innerClasses); itString++ )
   {
      item = *itString;
      WFCTRACEVAL( TEXT( "" ), CString( item.c_str() ) );
      _tprintf( TEXT( "%s\n" ), (LPCTSTR) item.c_str() );
   }

   // Methods
   String2MethodMap &methods = java_class_file.getMethods();

   for( String2MethodMap::iterator it2 = std::begin(methods); it2 != std::end(methods); it2++ )
   {
      if ( it2->second->getName().at( 0 ) == _T('<') )
      {
         continue;
      }

      item = TEXT( "" );

      switch( it2->second->GetAccessFlag() )
      {
         case PUBLIC:

            item += TEXT( "public " );
            break;

         case PROTECTED:

            item += TEXT( "protected " );
            break;

         case PRIVATE:

            item += TEXT( "private " );
            break;
      }

      if ( it2->second->isStatic() )
      {
         item += TEXT( "static " );
      }

      if ( it2->second->isFinal() )
      {
         item += L"final ";
      }

      item += it2->second->getType() + L' ' + it2->second->getName() + L'('; 

      StringVector &parameters = it2->second->getParameters();

      for( StringVector::iterator it3 = std::begin(parameters); it3 != std::end(parameters); it3++ )
      {
         item += *it3;

         if ( it3 != std::end(parameters) - 1 )
         {
            item += L", ";
         }
      }

      item += L')';

      StringVector &exceptions = it2->second->getExceptions();

      if ( exceptions.size() > 0 )
      {
         item += TEXT( " throws " );
      }

      for( it3 = std::begin(exceptions); it3 != std::end(exceptions); it3++ )
      {
         item += *it3;

         if ( it3 != std::end(exceptions) - 1 )
         {
            item += TEXT( ", " );
         }
      }

      WFCTRACEVAL( TEXT( "" ), CString( item.c_str() ) );
      _tprintf( TEXT( "%s\n" ), (LPCTSTR) item.c_str() );
   }

   // Fields

   String2FieldMap &fields = java_class_file.getFields();

   for( String2FieldMap::iterator it = std::begin(fields); it != std::end(fields); it++ )
   {
      item = L"";

      switch( it->second->GetAccessFlag() )
      {
         case PUBLIC:

            item += L"public ";
            break;

         case PROTECTED:

            item += L"protected ";
            break;

         case PRIVATE:

            item += L"private ";
            break;
      }

      if ( it->second->isStatic() )
      {
         item += L"static ";
      }

      if ( it->second->isFinal() )
      {
         item += L"final ";
      }

      item += it->second->getType() + L' ' + it->second->getName();

      CConstantValue *pconstant = it->second->GetConstantValue();

      if ( pconstant != NULL )
      {
         item += L" = " + pconstant->toString();
      }

      WFCTRACEVAL( TEXT( "" ), CString( item.c_str() ) );
      _tprintf( TEXT( "%s\n" ), (LPCTSTR) item.c_str() );
   }
}

int _tmain( int argc,  LPCTSTR argv[] )
{
   WFCTRACEINIT( TEXT( "main()" ) );

   CStringArray filenames;

   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\logo\\alogo.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\logo\\CLogoDrawer.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\ALower.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CCommodityActive.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CCurrencyMajor.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CEconomicsEmployment.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CEconomicsInflation.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CEquityActive.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CEquityMarket.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CEquityMovers.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CEquityUpdate.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CEquityWidely.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CInterestMortgage.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CInterestTreasuries.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CLowerDrawer.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CLowerFolder.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CLowerShelf.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CLowerSubShelf.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\lower\\CLowerTab.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\managers\\ColorEntry.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\managers\\ColorManager.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\managers\\DataTreeManager.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\managers\\FileManager.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\managers\\FirstTime.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\managers\\FlagManager.class" ) );
   filenames.Add( TEXT( "C:\\WINNT\\Java\\Lib\\ibit\\managers\\LogManager.class" ) );

   SIZE_T loop_index = 0;

   while( loop_index < filenames.GetSize() )
   {
      dump_class_file( filenames.GetAt( loop_index ) );
      loop_index++;
   }

   return( EXIT_SUCCESS );
}
