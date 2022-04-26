/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2022, Samuel R. Blackburn
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
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

_Check_return_ bool Win32FoundationClasses::wfc_protect_data( __in_bcount( number_of_bytes_in_plaintext ) uint8_t const * plaintext_buffer, _In_ std::size_t const number_of_bytes_in_plaintext, __inout_opt uint8_t * ciphertext_buffer, __inout std::size_t * address_of_number_of_cipher_bytes ) noexcept
{
   WFC_VALIDATE_POINTER( address_of_number_of_cipher_bytes );
   WFC_VALIDATE_POINTER_NULL_OK( plaintext_buffer );
   WFC_VALIDATE_POINTER_NULL_OK( ciphertext_buffer );

   if ( number_of_bytes_in_plaintext == 0 )
   {
      *address_of_number_of_cipher_bytes = 0;
      return( true );
   }

   DATA_BLOB plaintext  { (DWORD) number_of_bytes_in_plaintext, (BYTE *) plaintext_buffer };
   DATA_BLOB ciphertext { 0, nullptr };

   auto const return_value{ ::CryptProtectData(&plaintext,
                                                nullptr, // data description
                                                nullptr, // salt
                                                nullptr, // reserved
                                                nullptr, // prompt
                                                CRYPTPROTECT_UI_FORBIDDEN,
                                               &ciphertext) };

   if ( return_value == CRYPT_FAILED )
   {
      return( false );
   }

   if ( ciphertext_buffer not_eq nullptr )
   {
      if ( *address_of_number_of_cipher_bytes >= ciphertext.cbData )
      {
         CopyMemory( ciphertext_buffer, ciphertext.pbData, ciphertext.cbData );
      }
      else
      {
         *address_of_number_of_cipher_bytes = ciphertext.cbData;
          std::ignore = ::LocalFree( ciphertext.pbData );
          return( false );
      }
   }

   *address_of_number_of_cipher_bytes = ciphertext.cbData;
   std::ignore = ::LocalFree( ciphertext.pbData );
   return( true );
}

_Check_return_ bool Win32FoundationClasses::wfc_unprotect_data( __in_bcount( number_of_bytes_in_ciphertext ) uint8_t const * ciphertext_buffer, _In_ std::size_t const number_of_bytes_in_ciphertext, __inout_opt uint8_t* plaintext_buffer, __inout std::size_t * address_of_number_of_plain_bytes ) noexcept
{
   WFC_VALIDATE_POINTER( address_of_number_of_plain_bytes );
   WFC_VALIDATE_POINTER_NULL_OK( ciphertext_buffer );
   WFC_VALIDATE_POINTER_NULL_OK( plaintext_buffer );

   if ( number_of_bytes_in_ciphertext == 0 )
   {
      *address_of_number_of_plain_bytes = 0;
      return( true );
   }

   DATA_BLOB ciphertext { (DWORD) number_of_bytes_in_ciphertext, (BYTE *) ciphertext_buffer };
   DATA_BLOB plaintext  { 0, nullptr };

   auto const return_value{ ::CryptUnprotectData(&ciphertext,
                                                  nullptr, // data description
                                                  nullptr, // salt
                                                  nullptr, // reserved
                                                  nullptr, // prompt
                                                  CRYPTPROTECT_UI_FORBIDDEN,
                                                 &plaintext) };

   if ( return_value == CRYPT_FAILED )
   {
      return( false );
   }

   if ( plaintext_buffer not_eq nullptr )
   {
      if ( *address_of_number_of_plain_bytes >= plaintext.cbData )
      {
         CopyMemory( plaintext_buffer, plaintext.pbData, plaintext.cbData );
      }
      else
      {
         *address_of_number_of_plain_bytes = plaintext.cbData;
          std::ignore = ::LocalFree( plaintext.pbData );
          return( false );
      }
   }

   *address_of_number_of_plain_bytes = plaintext.cbData;
   std::ignore = ::LocalFree( plaintext.pbData );

   return( true );
}

// End of source
