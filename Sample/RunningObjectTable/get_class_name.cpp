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
*/

#include "RunningObjectTable.h"
#pragma hdrstop

void get_class_name( const CString& class_id_string, CString& class_name )
{
   WFCTRACEINIT( TEXT( "get_class_name( CString )" ) );

   CRegistry registry;

   if ( registry.Connect( (HKEY) CRegistry::keyClassesRoot ) == FALSE )
   {
      WFCTRACE( TEXT( "Can't connect to registry " ) );
      WFCTRACEERROR( registry.GetErrorCode() );
      return;
   }

   CString key_string( TEXT( "CLSID\\" ) );

   key_string += class_id_string;

   if ( registry.Open( key_string, (CRegistry::CreatePermissions) CRegistry::permissionRead ) != FALSE )
   {
      // The NULL parameter means "read the default value"
      registry.GetStringValue( NULL, class_name );
   }

   WFCTRACEVAL( TEXT( "Returning " ), class_name );
}

void get_class_name( const CLSID& class_id, CString& class_name )
{
   WFCTRACEINIT( TEXT( "get_class_name( CLSID )" ) );

   class_name.Empty();

   CString class_id_string;

   class_id_to_string( class_id, class_id_string );

   get_class_name( class_id_string, class_name );

   WFCTRACEVAL( TEXT( "Returning " ), class_name );
}
