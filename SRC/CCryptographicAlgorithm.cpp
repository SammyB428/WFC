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
** $Workfile: CCryptographicAlgorithm.cpp $
** $Revision: 25 $
** $Modtime: 6/26/01 10:44a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

// 2000-10-28
// Thanks go to Danny Smith (danny_r_smith_2001@yahoo.co.nz) for adding
// the capability to use a non-Microsoft compiler (namely gcc)

#include <wfc.h>
#pragma hdrstop

#if ( _MSC_VER >= 1020 ) || defined( WFC_HAVE_WINCRYPT )

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

// Construction

Win32FoundationClasses::CCryptographicAlgorithm::CCryptographicAlgorithm() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CCryptographicAlgorithm::CCryptographicAlgorithm( _In_ Win32FoundationClasses::CCryptographicAlgorithm const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CCryptographicAlgorithm::CCryptographicAlgorithm( _In_ PROV_ENUMALGS const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CCryptographicAlgorithm::~CCryptographicAlgorithm() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

// Methods

void Win32FoundationClasses::CCryptographicAlgorithm::Copy( _In_ Win32FoundationClasses::CCryptographicAlgorithm const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      //WFCTRACE( TEXT( "Attempt to make a copy of ourself (such silliness)." ) );
      return;
   }

   Identifier = source.Identifier;
   BitLength  = source.BitLength;
   Name.assign( source.Name );
}

void Win32FoundationClasses::CCryptographicAlgorithm::Copy( _In_ PROV_ENUMALGS const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Identifier = source.aiAlgid;
   BitLength  = source.dwBitLen;
   copy( Name, source.szName );
}

void Win32FoundationClasses::CCryptographicAlgorithm::Copy( _In_ PROV_ENUMALGS const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   WFC_TRY
   {
      Copy( *source );
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CCryptographicAlgorithm::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CCryptographicAlgorithm at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   Identifier is "              ) << Identifier    << TEXT( "\n"    );
   dump_context << TEXT( "   BitLength is  "              ) << BitLength     << TEXT( "\n"    );
   dump_context << TEXT( "   Name is       "              ) << Name          << TEXT( "\n"    );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CCryptographicAlgorithm::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Identifier = 0;
   BitLength  = 0;
   Name.clear();
}

_Check_return_ bool Win32FoundationClasses::CCryptographicAlgorithm::IsDataEncrypt( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GET_ALG_CLASS( Identifier ) == classDataEncrypt )
   {
      //WFCTRACE( TEXT( "Returning TRUE" ) );
      return(true);
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicAlgorithm::IsHash( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GET_ALG_CLASS( Identifier ) == classHash )
   {
      return( true );
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicAlgorithm::IsKeyExchange( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GET_ALG_CLASS( Identifier ) == classKeyExchange )
   {
      return( true );
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicAlgorithm::IsMessageEncrypt( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GET_ALG_CLASS( Identifier ) == classMessageEncrypt )
   {
      return( true );
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicAlgorithm::IsSignature( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( GET_ALG_CLASS( Identifier ) == classSignature )
   {
      return( true );
   }

   return( false );
}

// Operators

_Check_return_ Win32FoundationClasses::CCryptographicAlgorithm& Win32FoundationClasses::CCryptographicAlgorithm::operator=( _In_ Win32FoundationClasses::CCryptographicAlgorithm const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

_Check_return_ Win32FoundationClasses::CCryptographicAlgorithm& Win32FoundationClasses::CCryptographicAlgorithm::operator=( _In_ PROV_ENUMALGS const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

#endif // _MSC_VER

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CCryptographicAlgorithm</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles CryptoAPI algorithms.">
</HEAD>

<BODY>

<H1>CCryptographicAlgorithm : CObject</H1>
$Revision: 25 $<BR><HR>

<H2>Description</H2>

This is the class handles Cryptographic Algorithms. It
encapsulates the PROV_ENUMALGS data structure and gives you
a few little helpers.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CCryptographicAlgorithm</B>()
<B>CCryptographicAlgorithm</B>( const CCryptographicAlgorithm&amp; source )
<B>CCryptographicAlgorithm</B>( const PROV_ENUMALGS&amp; source )</PRE><DD>
Constructs the object.

</DL>

<H2>Data Members</H2>

<DL COMPACT>

<DT><PRE>DWORD <B>Identifier</B></PRE><DD>
The defined constant for the algorithm.

<DT><PRE>DWORD <B>BitLength</B></PRE><DD>
The number of bits in the key. This will usually be choked by the
United States Commerce Department to 40 bits (which has been proven
to be easily breakable).

<DT><PRE>std::wstring <B>Name</B></PRE><DD>
Hold the name of the algorithm.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Copy">Copy</A></B>( const CCryptographicAlgorithm&amp; source )
void <B>Copy</B>( const PROV_ENUMALGS&amp;  source )
void <B>Copy</B>( const PROV_ENUMALGS * source )</PRE><DD>
Copies the data from another object.

<DT><PRE>void <B><A NAME="Empty">Empty</A></B>( void )</PRE><DD>
Clears the data members.

<DT><PRE>BOOL <B><A NAME="IsDataEncrypt">IsDataEncrypt</A></B>( void )</PRE><DD>
Returns TRUE if this algorithm can be used to encrypt data.

<DT><PRE>BOOL <B><A NAME="IsHash">IsHash</A></B>( void )</PRE><DD>
Returns TRUE if this algorithm is a secure hash algorithm.

<DT><PRE>BOOL <B><A NAME="IsKeyExchange">IsKeyExchange</A></B>( void )</PRE><DD>
Returns TRUE if this algorithm is used to exchange keys with another
person.

<DT><PRE>BOOL <B><A NAME="IsMessageEncrypt">IsMessageEncrypt</A></B>( void )</PRE><DD>
Returns TRUE if this algorithm is used to encrypt messages.

<DT><PRE>BOOL <B><A NAME="IsSignature">IsSignature</A></B>( void )</PRE><DD>
Returns TRUE if this algorithm is used to digitally sign messages.

</DL>

<H2>Operators</H2>

<DL COMPACT>

<DT><PRE>operator <B>=</B> ( const CCryptographicAlgorithm&amp; source )
operator <B>=</B> ( const PROV_ENUMALGS&amp; source )</PRE><DD>
These call the <B>Copy</B>() methods.

</DL>

<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>

<H2>API's Used</H2>
<B>CCryptographicAlgorithm</B> uses the following API's:
<UL>
<LI>GET_ALG_CLASS
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CCryptographicAlgorithm.cpp $<BR>
$Modtime: 6/26/01 10:44a $
</BODY>

</HTML>
*/
