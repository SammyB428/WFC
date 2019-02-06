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
** $Workfile: open_connection_to_database.cpp $
** $Revision: 2 $
** $Modtime: 3/21/00 4:39a $
** $Reuse Tracing Code: 1 $
*/

#if ! defined( WFC_STL )

#if defined( _DEBUG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#endif // WFC_STL

bool open_connection_to_database( const CDatabaseParameters& database_parameters, _ConnectionPtr& connection_p )
{
   WFCTRACEINIT( TEXT( "open_connection_to_database()" ) );

   CString connection_string;

   database_parameters.GetConnectionString( connection_string );

   WFCTRACEVAL( TEXT( "Connection string is " ), connection_string );

   HRESULT result = S_OK;

   try
   {
      _bstr_t connection_string_as_bstring( (LPCTSTR) connection_string );

      try
      {
         result = connection_p.CreateInstance( __uuidof( Connection ) );
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

      if ( FAILED( result ) )
      {
         WFCTRACEVAL( TEXT( "Failing at line " ), __LINE__ );

         // Throw an exception to figure out what went wrong

         _com_issue_error( result );
         return( false );
      }

      connection_p->CommandTimeout = 300; // Five minutes

      DWORD options = 0;

      _bstr_t user_name_bstring( "" );
      _bstr_t user_password_bstring( "" );

      try
      {
         result = connection_p->Open( connection_string_as_bstring,
                                      user_name_bstring,
                                      user_password_bstring,
                                      options );
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

      if ( FAILED( result ) )
      {
         WFCTRACEVAL( TEXT( "Failing at line " ), __LINE__ );

         // Throw an exception to figure out what went wrong

         connection_p->Close();
         _com_issue_error( result );
         return( false );
      }

      return( true );
   }
   catch( _com_error &exception )
   {
      print_provider_error( connection_p );
      print_COM_error( exception );
      return( false );
   }
   catch( ... )
   {
      return( false );
   }
}
