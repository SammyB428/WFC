#include "ado2xml.h"
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
** $Workfile: convert_desired_tables.cpp $
** $Revision: 2 $
** $Modtime: 3/21/00 4:25a $
** $Reuse Tracing Code: 1 $
*/

#if ! defined( WFC_STL )

#if defined( _DEBUG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#endif // WFC_STL

void convert_desired_tables( const CExtensibleMarkupLanguageElement * settings, const CDatabaseParameters& database_parameters )
{
   WFCTRACEINIT( TEXT( "convert_desired_tables()" ) );

   _ConnectionPtr connection_p = NULL;

   if ( open_connection_to_database( database_parameters, connection_p ) == false )
   {
      WFCTRACE( TEXT( "Can't open connection to database." ) );
      _tprintf( TEXT( "Can't open connection to database.\n" ) );
      return;
   }

   // Get the names of the tables in that database

   CStringArray table_names;

	if ( get_table_names( connection_p, table_names ) == false )
	{
      WFCTRACE( TEXT( "Can't get table names." ) );
      _tprintf( TEXT( "Can't get table names.\n" ) );
	   return;
	}

   WFCTRACEVAL( TEXT( "Number of tables " ), table_names.GetSize() );

   //CString desired_table_name( TEXT( "Categories" ) );
   CString desired_table_name;

   if ( table_names.GetSize() > 0 )
   {
      SIZE_T loop_index = 0;

      CString table_name;
      CString where_clause; // Get this from the XML configuration data

      while( loop_index < table_names.GetSize() )
      {
         table_name = table_names.GetAt( loop_index );

         WFCTRACEVAL( TEXT( "Table " ), table_name );

         if (  desired_table_name.GetLength() == 0 ||
               desired_table_name.Compare( table_name ) == 0 )
         {
            convert_table( connection_p, table_name, where_clause );
         }

         loop_index++;
      }
   }

   try
   {
      connection_p->Close();
   }
   catch( _com_error &exception )
   {
      WFCTRACEVAL( TEXT( "COM exception caught at line " ), __LINE__ );
      print_COM_error( exception );
   }
   catch( ... )
   {
      WFCTRACEVAL( TEXT( "Exception caught at line " ), __LINE__ );
   }
}
