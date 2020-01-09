/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2018, Samuel R. Blackburn
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
** $Workfile: CExtensibleMarkupLanguageAttribute.cpp $
** $Revision: 15 $
** $Modtime: 6/26/01 10:45a $
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

USING_WFC_NAMESPACE

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CExtensibleMarkupLanguageAttribute</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, push technology, source code">
<META name="description" content="The C++ class that handles XML attributes.">
</HEAD>

<BODY>

<H1>CExtensibleMarkupLanguageAttribute</H1>

$Revision: 15 $

<HR>

<H2>Description</H2>

This class holds the name and value of an XML attribute. It is a very
trivial class.

<H2>Data Members</H2>

<DL COMPACT>

<DT><PRE>std::wstring <B>Name</B></PRE><DD>Contains the attribute name.
<DT><PRE>std::wstring <B>Value</B></PRE><DD>Contains the value of the attribute.

</DL>

<H2>Construction</H2>

<DL COMPACT>

<DT><PRE><B>CExtensibleMarkupLanguageAttribute</B>()
<B>CExtensibleMarkupLanguageAttribute</B>( const CExtensibleMarkupLanguageAttribute&amp; source )</PRE><DD>
Constructs the object.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Copy">Copy</A></B>( const CExtensibleMarkupLanguageAttribute&amp; source )</PRE><DD>
Copies another <B>CExtensibleMarkupLanguageAttribute</B>.

<DT><PRE>void <B><A NAME="Empty">Empty</A></B>( void )</PRE><DD>
Clears all data members.

</DL>

<H2>Operators</H2>

<DL COMPACT>

<DT><PRE>CExtensibleMarkupLanguageAttribute&amp; operator <B>=</B> ( const CExtensibleMarkupLanguageAttribute&amp; source )</PRE><DD>
Calls <B>Copy</B>().

</DL>

<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CExtensibleMarkupLanguageAttribute.cpp $<BR>
$Modtime: 6/26/01 10:45a $
</BODY>

</HTML>
*/
