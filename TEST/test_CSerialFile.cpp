/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2015, Samuel R. Blackburn
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
** $Workfile: test_CSerialFile.cpp $
** $Revision: 3 $
** $Modtime: 1/31/00 7:48p $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void test_CSerialFile( void ) noexcept
{
   WFCTRACEINIT( TEXT( "test_CSerialFile()" ) );

   CSerialFile serial;

   if ( serial.Open(WSTRING_VIEW(L"COM2:9600,n,8,1")) == false )
   {
      WFCTRACE( TEXT( "Can't open port" ) );
      return;
   }

   CCommunicationsConfiguration configuration;

   if ( serial.ConfigurationDialog( configuration ) == FALSE )
   {
#if defined( _DEBUG ) && ! defined( WFC_STL )
      configuration.Dump( afxDump );
#endif // _DEBUG
      WFCTRACEVAL( TEXT( "ConfigurationDialog() failed with " ), serial.LastError() );
      return;
   }
   else
   {
      if ( serial.GetConfiguration( configuration ) != FALSE )
      {
#if defined( _DEBUG ) && ! defined( WFC_STL )
         serial.Dump( afxDump );
#endif // _DEBUG
      }
   }
}
