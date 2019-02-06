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
** $Workfile: print_provider_error.cpp $
** $Revision: 1 $
** $Modtime: 3/07/00 4:53a $
** $Reuse Tracing Code: 1 $
*/

#if ! defined( WFC_STL )

#if defined( _DEBUG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#endif // WFC_STL

void print_provider_error( _ConnectionPtr connection_p )
{
   WFCTRACEINIT( TEXT( "print_provider_error()" ) );

   ErrorPtr error_p = NULL;

   if ( connection_p->Errors->Count > 0 )
   {
      long number_of_errors = connection_p->Errors->Count;
      long loop_index       = 0;

      WFCTRACEVAL( TEXT( "There are this many errors " ), number_of_errors );

      _tprintf( TEXT( "There are %ld provider errors.\n" ), number_of_errors );

      while( loop_index < number_of_errors )
      {
         error_p = connection_p->Errors->GetItem( loop_index );

         WFCTRACEVAL( TEXT( "Error " ), error_p->Number );
         WFCTRACEVAL( TEXT( "Description " ), CString( (const wchar_t *) error_p->Description.GetAddress() ) );
         WFCTRACEVAL( TEXT( "Source " ), CString( (const wchar_t *) error_p->Source.GetAddress() ) );

         _tprintf( TEXT( "%08lX - %s\n" ), error_p->Number, (LPCTSTR) error_p->Description.GetAddress() );
         loop_index++;
      }
   }
}
