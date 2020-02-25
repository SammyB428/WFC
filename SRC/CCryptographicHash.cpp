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
** $Workfile: CCryptographicHash.cpp $
** $Revision: 28 $
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

Win32FoundationClasses::CCryptographicHash::CCryptographicHash() noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_Hash = static_cast< HCRYPTHASH >( NULL );
   m_AutomaticallyDestroy = FALSE;
}

Win32FoundationClasses::CCryptographicHash::CCryptographicHash( _In_ HCRYPTHASH source_handle, _In_ bool automatically_destroy ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_Hash = static_cast< HCRYPTHASH >( NULL );
   m_AutomaticallyDestroy = automatically_destroy;

   (void) FromHandle( source_handle, automatically_destroy );
}

Win32FoundationClasses::CCryptographicHash::~CCryptographicHash() noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AutomaticallyDestroy == true)
   {
      (void) Destroy();
   }
}

// Methods

_Check_return_ bool Win32FoundationClasses::CCryptographicHash::Destroy( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   BOOL return_value = ::CryptDestroyHash( m_Hash );

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
   }
   else
   {
      return_value = TRUE;
   }

   m_Hash                 = static_cast< HCRYPTHASH >( NULL );
   m_AutomaticallyDestroy = false;

   return( return_value == FALSE ? false : true );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicHash::FromHandle( _In_ HCRYPTHASH source_handle, _In_ bool automatically_destroy ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // See if we are already handling a key

   if ( m_Hash != static_cast< HCRYPTHASH >( NULL ) )
   {
      if ( m_AutomaticallyDestroy == true)
      {
         (void) Destroy();
      }
   }

   m_Hash = source_handle;
   m_AutomaticallyDestroy = automatically_destroy;

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicHash::GetAlgorithmIdentifier( _Out_ DWORD& identifier ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   uint32_t buffer_length = sizeof( identifier );

   return( GetParameter( parameterAlgorithmIdentifier, (BYTE *) &identifier, buffer_length ) );
}

_Check_return_ HCRYPTHASH Win32FoundationClasses::CCryptographicHash::GetHandle( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_Hash );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicHash::GetLength( _Out_ uint32_t& length ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   uint32_t buffer_length = sizeof( length );

   return( GetParameter( parameterLength, (BYTE *) &length, buffer_length ) );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicHash::GetValue(_Out_ std::vector<uint8_t>& value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    value.resize(1024); // A Massive value

    uint32_t buffer_length = static_cast<uint32_t>(value.size());

    bool const return_value = GetParameter(parameterValue, value.data(), buffer_length);

    if (return_value == true)
    {
        value.resize(buffer_length);
    }

    return(return_value);
}

_Check_return_ bool Win32FoundationClasses::CCryptographicHash::GetParameter( _In_ uint32_t const parameter_to_get, __inout_bcount( buffer_length ) uint8_t *buffer, __inout uint32_t& buffer_length, _In_ uint32_t const flags ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( buffer );

   DWORD buffer_length_parameter = buffer_length;

   BOOL return_value = ::CryptGetHashParam( m_Hash, parameter_to_get, buffer, &buffer_length_parameter, flags );

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
   }
   else
   {
      return_value = TRUE;
   }

   // Frank O'Rourke <forourke@trintech.com> found a bug here. For some unknown
   // reason, I was returning m_ErrorCode. I guess I had a case of the stupids that day.

   return( return_value == FALSE ? false : true );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicHash::Hash( __in_bcount( buffer_size ) uint8_t const * buffer, _In_ DWORD const buffer_size, _In_ DWORD const flags ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if defined( _DEBUG )
   if ( buffer_size > 0 )
   {
      WFC_VALIDATE_POINTER( buffer );
   }
#endif

   // They screwed up the declaration. The buffer parameter should be
   // BYTE const * but it isn't. That means we cannot make our
   // data_to_compute_hash_on const correct.

   BOOL return_value = ::CryptHashData( m_Hash, buffer, buffer_size, flags );

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value == FALSE ? false : true );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicHash::Hash(_In_ std::vector<uint8_t> const& data_to_compute_hash_on, _In_ DWORD const flags) noexcept
{
    WFC_VALIDATE_POINTER(this);

    _ASSERTE(data_to_compute_hash_on.size() <= 0xFFFFFFFF); // To detect a 32-bit overrrun

    return(Hash(data_to_compute_hash_on.data(), (DWORD)data_to_compute_hash_on.size(), flags));
}

_Check_return_ bool Win32FoundationClasses::CCryptographicHash::Hash( _In_ Win32FoundationClasses::CCryptographicKey const& key_to_hash, _In_ DWORD const flags ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   BOOL return_value = ::CryptHashSessionKey( m_Hash, key_to_hash.GetHandle(), flags );

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value == FALSE ? false : true );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicHash::SetParameter( _In_ DWORD const parameter_to_set, __inout BYTE * buffer, _In_ DWORD const flags ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( buffer );

   // The buffer parameter to CryptSetHashParam is not const correct. It is
   // described in the documentation as an [in] parameter. This means the
   // CryptoAPI promises not to modify it. However, the parameter is not
   // typed as const. This means they screwed up the prototype.

   BOOL return_value = ::CryptSetHashParam( m_Hash, parameter_to_set, buffer, flags );

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value == FALSE ? false : true );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicHash::SetValue( __inout std::vector<uint8_t>& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   return( SetParameter( parameterValue, value.data() ) );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicHash::Sign( _In_ DWORD const which_key_to_sign_with, _In_ std::wstring const& password, __inout std::vector<uint8_t>& signed_hash, _In_ DWORD const flags ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   signed_hash.clear();

   DWORD number_of_bytes_in_signature = 0;

   // Find out how many bytes we need for the signed hash

   BOOL return_value = ::CryptSignHashW( m_Hash,
                                   which_key_to_sign_with,
                                   password.c_str(),
                                   flags,
                                   nullptr,
                                  &number_of_bytes_in_signature );

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACE( TEXT( "Can't get size of signed hash" ) );
      //WFCTRACEERROR( m_ErrorCode );
      return( false );
   }

   signed_hash.resize( number_of_bytes_in_signature );

   return_value = ::CryptSignHashW( m_Hash,
                                   which_key_to_sign_with,
                                   password.c_str(),
                                   flags,
                                   signed_hash.data(),
                                  &number_of_bytes_in_signature );

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACE( TEXT( "Can't get signed hash" ) );
      //WFCTRACEERROR( m_ErrorCode );
      return( false );
   }

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicHash::VerifySignature( __inout std::vector<uint8_t>& signature_to_be_verified, __inout Win32FoundationClasses::CCryptographicKey& public_key_to_verify_with, __inout std::wstring& password, _In_ DWORD const flags ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Once this function has been called, Destroy() must be called.

   _ASSERTE( signature_to_be_verified.size() <= 0xFFFFFFFF ); // To detect 32-bit overrun...

   BOOL return_value = ::CryptVerifySignatureW( m_Hash,
                                          signature_to_be_verified.data(),
                                  (DWORD) signature_to_be_verified.size(),
                                          public_key_to_verify_with.GetHandle(),
                                          password.c_str(),
                                          flags );

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
      return( false );
   }

   // Call succeeded, we must now destroy our hash

   (void) Destroy();

   return( true );
}

#endif // _MSC_VER

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CCryptographicHash</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles CryptoAPI hashing.">
</HEAD>

<BODY>

<H1>CCryptographicHash : <A HREF="CCryptography.htm">CCryptography</A></H1>
$Revision: 28 $<BR><HR>

<H2>Description</H2>

This class encapsulates cryptographic hashes. A &quot;cryptographic hash&quot; is
basically a checksum.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CCryptographicHash</B>()
<B>CCryptographicHash</B>( HCRYPTHASH source_handle, BOOL automatically_destroy = TRUE )</PRE><DD>
Constructs the object.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="Destroy">Destroy</A></B>( void )</PRE><DD>
Destroys the hash.

<DT><PRE>BOOL <B><A NAME="FromHandle">FromHandle</A></B>( HCRYPTHASH source_handle, BOOL automatically_destroy = TRUE )</PRE><DD>
Tells the class to start playing with the HCRYPTHASH provided.

<DT><PRE>BOOL <B><A NAME="GetAlgorithmIdentifier">GetAlgorithmIdentifier</A></B>( DWORD&amp; identifier )</PRE><DD>
Retrieves the identifier for the algorithm.

<DT><PRE>HCRYPTHASH <B><A NAME="GetHandle">GetHandle</A></B>( void ) const</PRE><DD>
Returns the encapsulated HCRYPTHASH so you can play with the API directly.

<DT><PRE>BOOL <B><A NAME="GetLength">GetLength</A></B>( DWORD&amp; block_length )</PRE><DD>
Retrieves the block length.

<DT><PRE>BOOL <B><A NAME="GetValue">GetValue</A></B>( std::vector&lt;uint8_t&gt;&amp; hash_result )</PRE><DD>
Retrieves the hash itself.

<DT><PRE>BOOL <B><A NAME="GetParameter">GetParameter</A></B>( DWORD const parameter_to_get, BYTE *buffer, DWORD&amp; buffer_length, DWORD const flags = 0 )</PRE><DD>
Retrieves one of the following bits of information:
<UL>
<LI>parameterAlgorithmIdentifier
<LI>parameterLength
<LI>parameterValue
</UL>
It is probably easier to call the helper functions, <B>GetAlgorithmIdentifier</B>(),
<B>GetLength</B>() or <B>GetValue</B>() than <B>GetParameter</B>().

<DT><PRE>BOOL <B><A NAME="Hash">Hash</A></B>( std::vector&lt;uint8_t&gt;&amp; data_to_compute_a_hash_on, DWORD flags = CRYPT_USERDATA )
BOOL <B>Hash</B>( const CCryptographicKey&amp; key_to_hash, DWORD flags = 0 )</PRE><DD>
Computes the hash value from the data block (<CODE>data_to_compute_hash_on</CODE>)or
the CCryptographicKey (<CODE>key_to_hash</CODE>) given. You can retrieve the hash via <B>GetValue</B>.

<DT><PRE>BOOL <B><A NAME="SetParameter">SetParameter</A></B>( DWORD const parameter_to_set, BYTE *buffer, DWORD const flags = 0 )</PRE><DD>
Sets a parameter of the key. You can set one of these parameters:
<UL>
<LI>parameterValue
</UL>

<DT><PRE>BOOL <B><A NAME="SetValue">SetValue</A></B>( std::vector&lt;uint8_t&gt;&amp; value )</PRE><DD>
Basically calls <B>SetParameter</B>() with parameterValue.

<DT><PRE>BOOL <B><A NAME="Sign">Sign</A></B>( DWORD which_key_to_sign_with, std::wstring&amp; password, std::vector&lt;uint8_t&gt;&amp; signature, DWORD flags = 0 )</PRE><DD>
After you've created the hash you must sign it. You
must supply a plain text password to be used in the signature.

<DT><PRE>BOOL <B><A NAME="VerifySignature">VerifySignature</A></B>( std::vector&lt;uint8_t&gt;&amp; signature, CCryptographicKey&amp; public_key_to_verify_with, std::wstring&amp; password, DWORD flags = 0 )</PRE><DD>
To verify the signature, you must supply
the exact same plain text password as the person who signed (using <B>Sign</B>()) the hash.

</DL>

<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>

<H2>API's Used</H2>
<UL>
<LI>CryptDestroyHash
<LI>CryptGetHashParam
<LI>CryptHashData
<LI>CryptHashSessionKey
<LI>CryptSetHashParam
<LI>CryptSignHash
<LI>CryptVerifySignature
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CCryptographicHash.cpp $<BR>
$Modtime: 6/26/01 10:44a $
</BODY>
</HTML>
The following line should go in AUTOEXP.DAT so the debugging tooltips will format properly
ToolTipFormatLine=CCryptographicHash=m_Hash=<m_Hash>
*/
