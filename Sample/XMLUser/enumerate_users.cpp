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
** $Workfile: enumerate_users.cpp $
** $Revision: 2 $
** $Modtime: 5/16/98 4:24p $
*/

#include "XMLUser.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void enumerate_users( CExtensibleMarkupLanguageDocument& document, LPCTSTR machine_name )
{
   WFCTRACEINIT( TEXT( "enumerate_users()" ) );

   CExtensibleMarkupLanguageElement * root_element_p = document.GetRootElement();

   CNetworkUsers users( machine_name );

   CNetworkUserInformation user_information;

   CString friendly_machine_name;

   if ( users.Enumerate() != FALSE )
   {
      CExtensibleMarkupLanguageElement * element_p = NULL;

      element_p = CExtensibleMarkupLanguageElement::NewElement( root_element_p );

      element_p->SetTag( TEXT( "MACHINE" ) );

      add_machine_name( users, element_p );

      add_level( users.GetLevel(), element_p );

      add_current_time( element_p );

      while( users.GetNext( user_information ) != FALSE )
      {
         add_to_xml( user_information, element_p );
      }
   }
   else
   {
      DWORD error_code = users.GetErrorCode();

      CString error_message;

      Convert_NERR_Code_to_String( error_code, error_message );

      _tprintf( TEXT( "CNetworkUsers.Enumerate( user_information ), ErrorCode == %d \"%s\"\n" ), error_code, (LPCTSTR) error_message );
   }
}
