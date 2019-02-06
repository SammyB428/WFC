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
** $Workfile: print_COM_error.cpp $
** $Revision: 1 $
** $Modtime: 3/14/00 4:46a $
** $Reuse Tracing Code: 1 $
*/

#if ! defined( WFC_STL )

#if defined( _DEBUG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#endif // WFC_STL

void print_COM_error( _com_error &exception )
{
   WFCTRACEINIT( TEXT( "print_COM_error()" ) );

   _bstr_t source( exception.Source() );
   _bstr_t description( exception.Description() );

#if defined( _DEBUG )

   {
      CString error_string;

      error_string.Format( TEXT( "%08lX" ), (unsigned long) exception.Error() );
      WFCTRACEVAL( TEXT( "HRESULT " ), error_string );
   }

#endif // _DEBUG

   WFCTRACEVAL( TEXT( "HRESULT " ), (DWORD) exception.Error() );
   WFCTRACEVAL( TEXT( "Source " ), CString( (const wchar_t *) source.GetAddress() ) );
   WFCTRACEVAL( TEXT( "Description " ), CString( (const wchar_t *) description.GetAddress() ) );
   WFCTRACEVAL( TEXT( "Error Message " ), CString( exception.ErrorMessage() ) );

   _tprintf( TEXT( "COM Error:\n" ) );
   _tprintf( TEXT( "Code: %08lX\n" ), exception.Error() );
   _tprintf( TEXT( "Source: %s\n" ), (LPCTSTR) source );
   _tprintf( TEXT( "Description: %s\n" ), (LPCTSTR) description );
   _tprintf( TEXT( "Message: %s\n" ), exception.ErrorMessage() );
}
