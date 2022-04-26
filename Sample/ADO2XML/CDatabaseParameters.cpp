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
** $Workfile: CDatabaseParameters.cpp $
** $Revision: 2 $
** $Modtime: 3/21/00 4:22a $
** $Reuse Tracing Code: 1 $
*/

#if ! defined( WFC_STL )

#if defined( _DEBUG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#endif // WFC_STL

CDatabaseParameters::CDatabaseParameters()
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::CDatabaseParameters()" ) );
}

CDatabaseParameters::CDatabaseParameters( const CDatabaseParameters& source )
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::CDatabaseParameters( CDatabaseParameters& )" ) );

   Copy( source );
}

CDatabaseParameters::~CDatabaseParameters()
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::~CDatabaseParameters()" ) );
   Empty();
}

void CDatabaseParameters::Copy( const CDatabaseParameters& source )
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::Copy()" ) );

   if ( &source == this )
   {
      return;
   }

   m_Provider       = source.m_Provider;
   m_UserID         = source.m_UserID;
   m_Password       = source.m_Password;
   m_DataSource     = source.m_DataSource;
   m_InitialCatalog = source.m_InitialCatalog;
}

void CDatabaseParameters::Empty( void )
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::Empty()" ) );

   m_Provider.Empty();
   m_UserID.Empty();
   m_Password.Empty();
   m_DataSource.Empty();
   m_InitialCatalog.Empty();
}

CDatabaseParameters& CDatabaseParameters::operator=( const CDatabaseParameters& source )
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::operator=()" ) );
   Copy( source );
   return( *this );
}

void CDatabaseParameters::GetDataSource( CString& return_value ) const
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::GetDataSource()" ) );
   return_value = m_DataSource;
}

void CDatabaseParameters::GetInitialCatalog( CString& return_value ) const
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::GetInitialCatalog()" ) );
   return_value = m_InitialCatalog;
}

void CDatabaseParameters::GetPassword( CString& return_value ) const
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::GetPassword()" ) );
   return_value = m_Password;
}

void CDatabaseParameters::GetProvider( CString& return_value ) const
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::GetProvider()" ) );
   return_value = m_Provider;
}

void CDatabaseParameters::GetUserID( CString& return_value ) const
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::GetUserID()" ) );
   return_value = m_UserID;
}

void CDatabaseParameters::SetDataSource( LPCTSTR set_string )
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::SetDataSource()" ) );
   m_DataSource = set_string;
}

void CDatabaseParameters::SetInitialCatalog( LPCTSTR set_string )
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::SetInitialCatalog()" ) );
   m_InitialCatalog = set_string;
}

void CDatabaseParameters::SetPassword( LPCTSTR set_string )
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::SetPassword()" ) );
   m_Password = set_string;
}

void CDatabaseParameters::SetProvider( LPCTSTR set_string )
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::SetProvier()" ) );
   m_Provider = set_string;
}

void CDatabaseParameters::SetUserID( LPCTSTR set_string )
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::SetUserID()" ) );
   m_UserID = set_string;
}

void CDatabaseParameters::SetConnectionString( LPCTSTR set_string )
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::SetConnectionString()" ) );
   m_ConnectionString = set_string;
}

void CDatabaseParameters::GetConnectionString( CString& return_value ) const
{
   WFCTRACEINIT( TEXT( "CDatabaseParameters::GetConnectionString()" ) );

   if ( m_ConnectionString.GetLength() > 0 )
   {
      // The user already specified a connection string, return
      // that one

      return_value = m_ConnectionString;
      return;
   }

   CString temporary_string;

   return_value.Empty();

   GetProvider( temporary_string );

   if ( temporary_string.GetLength() > 0 )
   {
      return_value += TEXT( "Provider=" );
      return_value += temporary_string;
      return_value += TEXT( ";" );
   }

   GetDataSource( temporary_string );

   if ( temporary_string.GetLength() > 0 )
   {
      return_value += TEXT( "Data Source=" );
      return_value += temporary_string;
      return_value += TEXT( ";" );
   }

   GetInitialCatalog( temporary_string );

   if ( temporary_string.GetLength() > 0 )
   {
      return_value += TEXT( "Initial Catalog=" );
      return_value += temporary_string;
      return_value += TEXT( ";" );
   }

   GetUserID( temporary_string );

   if ( temporary_string.GetLength() > 0 )
   {
      return_value += TEXT( "User Id=" );
      return_value += temporary_string;
      return_value += TEXT( ";" );
   }

   DWORD user_id_length{ temporary_string.GetLength() };

   GetPassword( temporary_string );

   if ( temporary_string.GetLength() > 0 ||
        user_id_length > 0 )
   {
      return_value += TEXT( "Password=" );
      return_value += temporary_string;
      return_value += TEXT( ";" );
   }
}