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
** $Workfile: get_blob.cpp $
** $Revision: 2 $
** $Modtime: 3/21/00 4:34a $
** $Reuse Tracing Code: 1 $
*/

#if ! defined( WFC_STL )

#if defined( _DEBUG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#endif // WFC_STL

void get_blob( FieldPtr& field_p, CByteArray& blob )
{
   WFCTRACEINIT( TEXT( "get_blob()" ) );

   blob.RemoveAll();

   try
   {
      long number_of_bytes_in_blob = field_p->GetActualSize();

      if ( number_of_bytes_in_blob < 1 )
      {
         WFCTRACE( TEXT( "This is an empty blob." ) );
         return;
      }

      WFCTRACEVAL( TEXT( "number_of_bytes_in_blob is " ), number_of_bytes_in_blob );

      DWORD data_type = 0;
      
      try
      {
         data_type = field_p->GetType();
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

      CString data_string;

      convert_DataTypeEnum_to_string( data_type, data_string );

      WFCTRACEVAL( TEXT( "Data type is " ), data_string );

      _variant_t blob_contents;

      try
      {
         blob_contents = field_p->GetChunk( number_of_bytes_in_blob );
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

      // See if we need to convert it

      WFCTRACEVARIANT( TEXT( "Blob contents " ), blob_contents );

      if ( V_VT( &blob_contents ) == VT_BSTR )
      {
         _bstr_t value_as_a_bstring( blob_contents );

         long string_length = value_as_a_bstring.length();
         long loop_index    = 0;

         const wchar_t * characters = (const wchar_t *) value_as_a_bstring;

         while( loop_index < string_length )
         {
            blob.Add( LOBYTE( characters[ loop_index ] ) );
            blob.Add( HIBYTE( characters[ loop_index ] ) );
            loop_index++;
         }

         return;
      }

      if ( V_VT( &blob_contents ) != ( (VT_ARRAY) | (VT_UI1) ) )
      {
         blob_contents.ChangeType( ( (VT_ARRAY) | (VT_UI1) ) );
      }

      // Now convert from a variant to a CByteArray

      blob.SetSize( V_ARRAY( &blob_contents )->rgsabound[ 0 ].cElements );

      if ( blob.GetSize() == 0 )
      {
         WFCTRACE( TEXT( "Another empty blob." ) );
         return;
      }

      void * buffer_pointer = NULL;

      SafeArrayAccessData( V_ARRAY( &blob_contents ), &buffer_pointer );
      (void) memcpy_s( blob.GetData(), blob.GetSize(), buffer_pointer, blob.GetSize() );
      SafeArrayUnaccessData( V_ARRAY( &blob_contents ) );
   }
   catch( _com_error &exception )
   {
      WFCTRACE( TEXT( "COM execption caught getting blob." ) );
      print_COM_error( exception );
   }
   catch( ... )
   {
      WFCTRACE( TEXT( "execption caught." ) );
   }
}
