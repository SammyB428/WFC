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
** $Workfile: main.cpp $
** $Revision: 1 $
** $Modtime: 3/18/00 7:29a $
** $Reuse Tracing Code: 1 $
*/

#if ! defined( WFC_STL )

#if defined( _DEBUG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#endif // WFC_STL

int _tmain( int argc, LPCTSTR argv[] )
{
   WFCTRACEINIT( TEXT( "main()" ) );

   // Start COM
   CoInitialize( NULL );

 //  VARIANT v;

   CDatabaseParameters database_parameters;

   // Get these parameters from our config file (XML of course)
   database_parameters.SetProvider( TEXT( "Microsoft.Jet.OLEDB.4.0" ) );

   // List of MDB's on my system

   //database_parameters.SetDataSource( TEXT( "c:\\program files\\microsoft visual studio\\common\\tools\\vcm\\vcm.mdb" ) );
   //database_parameters.SetDataSource( TEXT( "c:\\program files\\common files\\microsoft shared\\pushpins\\autopins\\am7ppins.mdb" ) );
   //database_parameters.SetDataSource( TEXT( "c:\\winnt\\msapps\\repostry\\repostry.mdb" ) );
   database_parameters.SetDataSource( TEXT( "d:\\msoffice\\access\\samples\\northwind.mdb" ) );
   //database_parameters.SetDataSource( TEXT( "f:\\cd\nt4sp6a\\i386\\empty_pb.mdb" ) );

   CExtensibleMarkupLanguageDocument settings;

   convert_desired_tables( settings.GetRootElement(), database_parameters );

   // Stop COM
   CoUninitialize();
   return( EXIT_SUCCESS );
}
