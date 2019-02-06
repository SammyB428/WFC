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

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#pragma hdrstop

void main( void )
{
   CoInitialize( NULL );

   CLSID class_id;

   class_id.Data1      = 0x514;
   class_id.Data2      = 0;
   class_id.Data3      = 0x10;
   class_id.Data4[ 0 ] = 0x80;
   class_id.Data4[ 1 ] = 0x00;
   class_id.Data4[ 2 ] = 0x00;
   class_id.Data4[ 3 ] = 0xAA;
   class_id.Data4[ 4 ] = 0x00;
   class_id.Data4[ 5 ] = 0x6D;
   class_id.Data4[ 6 ] = 0x2E;
   class_id.Data4[ 7 ] = 0xA4;

   IUnknown * unknown_p = (IUnknown *) NULL;

   HRESULT result = CoCreateInstance( class_id,
                                      NULL,
                                      0x17,
                                      __uuidof( IUnknown ),
                                      reinterpret_cast< void ** >( &unknown_p ) );

   if ( result == 0 )
   {
      _tprintf( TEXT( "COM Locked.\n" ) );

      MSG message;

      while( 1 )
      {
         while( GetMessage( &message, NULL, 0, 0xFFFFFFFF ) )
         {
            DispatchMessage( &message );
         }
      }
   }
   else
   {
      _tprintf( TEXT( "COM NOT Locked.\n" ) );
   }
}
