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
** $Workfile: CCryptography.cpp $
** $Revision: 19 $
** $Modtime: 6/26/01 10:44a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

// 2000-10-28
// Thanks go to Danny Smith (danny_r_smith_2001@yahoo.co.nz) for adding
// the capability to use a non-Microsoft compiler (namely gcc)

#if ( _MSC_VER >= 1020 ) || defined( WFC_HAVE_WINCRYPT )

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

USING_WFC_NAMESPACE

// Construction

CCryptography::CCryptography()
{
   WFC_VALIDATE_POINTER( this );

   m_ErrorCode = 0;
}

CCryptography::~CCryptography()
{
   WFC_VALIDATE_POINTER( this );

   m_ErrorCode = 0;
}

// Methods

__checkReturn DWORD CCryptography::GetErrorCode( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_ErrorCode );
}

#endif // _MSC_VER

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CCryptography</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ base class for all CryptoAPI related classes.">
</HEAD>

<BODY>

<H1>CCryptography</H1>
$Revision: 19 $<BR><HR>

<H2>Description</H2>
This is the base class for all cryptographic classes that provide functionality.

<H2>Data Members</H2>None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>DWORD <B><A NAME="GetErrorCode">GetErrorCode</A></B>( void ) const</PRE><DD>
Returns the error code of the last error.

</DL>

<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CCryptography.cpp $<BR>
$Modtime: 6/26/01 10:44a $
</BODY>

</HTML>
*/
