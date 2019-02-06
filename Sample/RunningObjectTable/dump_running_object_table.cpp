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

void dump_running_object_table( void )
{
   WFCTRACEINIT( TEXT( "dump_running_object_table()" ) );

   WFC_TRY
   {
      WFCTRACEVAL( TEXT( "IRunningObjectTable is " ), __uuidof( IRunningObjectTable ) );
      CComPtr< IRunningObjectTable > running_object_table_p = NULL;

      HRESULT ole_result = GetRunningObjectTable( 0, &running_object_table_p );

      if ( FAILED( ole_result ) )
      {
         WFCTRACEVAL( TEXT( "OLE SUCKS! Error " ), ole_result );
         return;
      }

      WFCTRACEVAL( TEXT( "IEnumMoniker is " ), __uuidof( IEnumMoniker ) );
      CComPtr< IEnumMoniker > moniker_enumerator_p = NULL;

      ole_result = running_object_table_p->EnumRunning( &moniker_enumerator_p );

      if ( FAILED( ole_result ) )
      {
         WFCTRACEVAL( TEXT( "OLE SUCKS! Error " ), ole_result );
         return;
      }

      // in ATL, they want you to use CComPtr for pointers to COM objects. However,
      // they don't work in all cases. Yet another example of Microsoft technology
      // that is *almost* useful. Specifically, when you take the address of a pointer
      // if that point is already set, the CComPtr class will not call Release on the
      // wrapped pointer.

      WFCTRACEVAL( TEXT( "IMoniker is " ), __uuidof( IMoniker ) );
      IMoniker * moniker_pointer = NULL;

      WFCTRACEVAL( TEXT( "IBindCtx is " ), __uuidof( IBindCtx ) );
      CComPtr< IBindCtx > bind_context_object_p = NULL;

      ole_result = CreateBindCtx( 0, &bind_context_object_p );

      if ( FAILED( ole_result ) )
      {
         WFCTRACEVAL( TEXT( "OLE SUCKS! Error " ), ole_result );
         return;
      }

      WFCTRACEVAL( TEXT( "IMalloc is " ), __uuidof( IMalloc ) );
      CComPtr< IMalloc > malloc_p = NULL;

      ole_result = CoGetMalloc( 1, &malloc_p );

      if ( FAILED( ole_result ) )
      {
         WFCTRACEVAL( TEXT( "OLE SUCKS! Error " ), ole_result );
         return;
      }

      LPOLESTR ole_string = NULL;

      ULONG number_of_monikers_fetched = 0;

      while( moniker_enumerator_p->Next( 1, &moniker_pointer, &number_of_monikers_fetched ) == S_OK )
      {
         ole_result = moniker_pointer->GetDisplayName( bind_context_object_p, NULL, &ole_string );

         if ( FAILED( ole_result ) )
         {
            WFCTRACEVAL( TEXT( "OLE SUCKS! Error " ), ole_result );
         }
         else
         {
            CString id_string( ole_string );

            WFCTRACEVAL( TEXT( "id_string is " ), id_string );

            _tprintf( TEXT( "%s " ), (LPCTSTR) id_string );

            id_string.TrimLeft( TEXT( '!' ) ); // Things are deliniated by !'s in COM Monikers
            id_string.TrimLeft();
            id_string.TrimRight();

            CString class_name;

            get_class_name( id_string, class_name );

            class_name.TrimLeft();
            class_name.TrimRight();

            _tprintf( TEXT( "(%s)\n" ), (LPCTSTR) class_name );

            WFCTRACEVAL( TEXT( "class name is " ), class_name );
         }

         malloc_p->Free( ole_string );
         ole_string = NULL;

         moniker_pointer->Release();
         moniker_pointer = NULL;
      }
   }
   WFC_CATCH_ALL
   {
   }
   WFC_END_CATCH_ALL
}
