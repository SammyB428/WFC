/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2015, Samuel R. Blackburn
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
** $Workfile: RemoteAccessServiceUser.cpp $
** $Revision: 19 $
** $Modtime: 6/26/01 10:56a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#if ! defined( WFC_NO_RAS )

USING_WFC_NAMESPACE

// 1999-07-18
// Thanks go to Vagif Abilov (vagif@online.no) for finding a bug here.
// I was not IMPLEMENT_SERIAL()'ing

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

CRemoteAccessServiceUser::CRemoteAccessServiceUser()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

CRemoteAccessServiceUser::CRemoteAccessServiceUser( CRemoteAccessServiceUser const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CRemoteAccessServiceUser::CRemoteAccessServiceUser( RAS_USER_0 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CRemoteAccessServiceUser::~CRemoteAccessServiceUser()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

void CRemoteAccessServiceUser::Copy( RAS_USER_0 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   // Choose to live
   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      Privileges = source->bfPrivilege;
      ::wfc_convert_lpcwstr_to_cstring( source->szPhoneNumber, PhoneNumber );
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void CRemoteAccessServiceUser::Copy( CRemoteAccessServiceUser const& source )
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   Privileges  = source.Privileges;
   PhoneNumber = source.PhoneNumber;
}

void CRemoteAccessServiceUser::Empty( void )
{
   WFC_VALIDATE_POINTER( this );
   Privileges = 0;
   PhoneNumber.Empty();
}

// 1999-07-18
// Thanks go to Vagif Abilov (vagif@online.no) for finding a bug here.
// I had forgotten to put in the assignment operator

CRemoteAccessServiceUser& CRemoteAccessServiceUser::operator = ( CRemoteAccessServiceUser const& source )
{
   WFC_VALIDATE_POINTER( this );

   if ( this != &source )
   {
      Copy( source );
   }

   return( *this );
}

// 1999-07-18
// Thanks go to Vagif Abilov (vagif@online.no) for finding a bug here.
// I had forgotten to put in the Dump() method

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
 
void CRemoteAccessServiceUser::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );

   dump_context << TEXT( "a CRemoteAccessServiceUser at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   Privileges is " )  << Privileges  << TEXT( "\n" );
   dump_context << TEXT( "   PhoneNumber is " ) << PhoneNumber << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

#endif // WFC_NO_RAS

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CRemoteAccessServiceUser</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, RAS, Remote Access Service, Dialup networking">
</HEAD>

<BODY>

<H1>CRemoteAccessServiceUser : CObject</H1>
$Revision: 19 $
<HR>

<H2>Description</H2>
This class is based on the RAS_USER_0 structure.

<H2>Data Members</H2>

<B>None.</B>

<H2>Methods</H2>

<DL COMPACT>
<DT><B><A NAME="Copy">Copy</A></B><DD>Copies another CRemoteAccessServiceUser or a RAS_USER_0 structure.
</DL>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: RemoteAccessServiceUser.cpp $<BR>
$Modtime: 6/26/01 10:56a $
</BODY>

</HTML>
*/
