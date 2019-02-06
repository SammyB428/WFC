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
** $Workfile: add_to_xml.cpp $
** $Revision: 2 $
** $Modtime: 5/16/98 4:26p $
*/

#include "XMLUser.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

BOOL serialize_network_user_information( CXMLArchive * archive_p, const CString& name, void * parameter )
{
   WFCTRACEINIT( TEXT( "serialize_network_user_information()" ) );

   if ( parameter == NULL )
   {
      WFCTRACE( TEXT( "parameter is nULL!" ) );
      return( FALSE );
   }

   CNetworkUserInformation * user_p = NULL;

   user_p = reinterpret_cast< CNetworkUserInformation * >( parameter );

   // We were passed a pointer, don't trust it

   try
   {
      if ( archive_p->IsStoring() != FALSE )
      {
         // We are storing
         archive_p->Write( TEXT( "NAME" ),               user_p->Name              );
         archive_p->Write( TEXT( "FULL_NAME" ),          user_p->FullName          );
         archive_p->Write( TEXT( "LOGON_HOURS" ),        user_p->LogonHours        );
         archive_p->Write( TEXT( "ENCRYPTED_PASSWORD" ), user_p->EncryptedPassword );
         archive_p->Write( TEXT( "PASSWORD_AGE" ),       user_p->PasswordAge       );
         archive_p->Write( TEXT( "LAST_LOGON" ),         user_p->LastLogon, FALSE  );
         archive_p->Write( TEXT( "LAST_LOGOFF" ),        user_p->LastLogoff, FALSE );
      }
      else
      {
         archive_p->Read( TEXT( "NAME" ),               user_p->Name              );
         archive_p->Read( TEXT( "FULL_NAME" ),          user_p->FullName          );
         archive_p->Read( TEXT( "LOGON_HOURS" ),        user_p->LogonHours        );
         archive_p->Read( TEXT( "ENCRYPTED_PASSWORD" ), user_p->EncryptedPassword );
         archive_p->Read( TEXT( "PASSWORD_AGE" ),       user_p->PasswordAge       );
         archive_p->Read( TEXT( "LAST_LOGON" ),         user_p->LastLogon         );
         archive_p->Read( TEXT( "LAST_LOGOFF" ),        user_p->LastLogoff        );
      }
   }
   catch( ... )
   {
      return( FALSE );
   }

   return( TRUE );
}

void add_to_xml( CNetworkUserInformation& user, CExtensibleMarkupLanguageElement * parent_element_p )
{
   WFCTRACEINIT( TEXT( "add_to_xml()" ) );

   if ( parent_element_p == NULL )
   {
      WFCTRACE( TEXT( "Parent element is NULL!" ) );
      return;
   }

//   parent_element_p->AddText( TEXT( "\n\n" ) ); // Make resulting file more humanly readable

   CExtensibleMarkupLanguageElement * element_p = NULL;

   element_p = CExtensibleMarkupLanguageElement::NewElement( parent_element_p );

   if ( element_p == NULL )
   {
      return;
   }

   element_p->SetTag( TEXT( "USER" ) );

   CXMLArchive archive;

   archive.WriteTo( element_p );

   archive.SerializeObject( TEXT( "USER" ),
                            reinterpret_cast< void * >( &user ),
                            serialize_network_user_information );
}
