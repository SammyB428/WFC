/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2016, Samuel R. Blackburn
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
** $Workfile: TCNSrc.cpp $
** $Revision: 2 $
** $Modtime: 3/08/98 3:30p $
*/

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void print_resource( CNetworkResourceInformation& information ) noexcept
{
   _tprintf( TEXT( "\"%s\" - \"%s\" - \"%s\" - \"%s\"\n" ),
           information.LocalName.c_str(),
           information.RemoteName.c_str(),
           information.Comment.c_str(),
           information.Provider.c_str());
}

_Check_return_ bool test_CNetResource( _Out_ std::string& class_name, _Out_ int& test_number_that_failed ) noexcept
{
   WFCTRACEINIT( TEXT( "test_CNetResource()" ) );

   class_name.assign( "CNetworkResources" );
   test_number_that_failed = 1;

   CNetworkResources net_resource;

   CNetworkResourceInformation information;

   information.Scope = static_cast<DWORD>(CNetworkResources::Scope::All);
   information.Usage = CNetworkResources::usageConnectable | CNetworkResources::usageContainer;
   information.Type  = CNetworkResources::typeDisk;

   if ( net_resource.Enumerate( information ) != FALSE )
   {
      print_resource( information );

      while( net_resource.GetNext( information ) != FALSE )
      {
         print_resource( information );
      }

      DWORD error_code = net_resource.GetErrorCode();

      std::wstring error_message;

      Convert_NERR_Code_to_String( error_code, error_message );

      _tprintf( TEXT( "CNetSession.ErrorCode == %d \"%s\"\n" ), error_code, error_message.c_str());
   }
   else
   {
      DWORD error_code = net_resource.GetErrorCode();

      std::wstring error_message;

      Convert_NERR_Code_to_String( error_code, error_message );

      _tprintf( TEXT( "CNetSession.ErrorCode == %d \"%s\"\n" ), error_code, error_message.c_str());
      return( failure() );
   }

   return( true );
}

