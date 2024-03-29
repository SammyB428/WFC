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
** $Workfile: CCryptographicKey.cpp $
** $Revision: 34 $
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
static auto const THIS_FILE{ __FILE__ };
#endif // _DEBUG

static inline _Check_return_ bool wfc_trim_decrypted_data( __inout std::vector<uint8_t>& decrypted_data ) noexcept
{
   // Now that we've decrypted the data, let's strip off any padding

   if ( decrypted_data.size() < 4 )
   {
      //WFCTRACE( TEXT( "Not enough decrypted data." ) );
      return( false );
   }

   uint8_t const byte_1{ decrypted_data.at(0) };
   uint8_t const byte_2{ decrypted_data.at(1) };
   uint8_t const byte_3{ decrypted_data.at(2) };
   uint8_t const byte_4{ decrypted_data.at(3) };

   std::size_t size_of_plaintext{ Win32FoundationClasses::MAKE_DATA_LENGTH(byte_1, byte_2, byte_3, byte_4) };

   decrypted_data.erase(std::begin(decrypted_data), std::begin(decrypted_data) + 4);

   ASSERT( size_of_plaintext <= decrypted_data.size() );

   if ( size_of_plaintext > decrypted_data.size() )
   {
      //WFCTRACE( TEXT( "UH OH! Something bad happened" ) );
      return( false );
   }

   // Now strip off that nasty padding....

   auto const number_of_bytes_to_strip{ decrypted_data.size() - size_of_plaintext };

   Win32FoundationClasses::remove_at( decrypted_data, decrypted_data.size() - number_of_bytes_to_strip, number_of_bytes_to_strip );

   // Let's make one final check

   ASSERT( size_of_plaintext == decrypted_data.size() );

   return( true );
}

// Construction

Win32FoundationClasses::CCryptographicKey::CCryptographicKey() noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_Key                  = static_cast< HCRYPTKEY >( NULL );
   m_AutomaticallyDestroy = false;
}

Win32FoundationClasses::CCryptographicKey::CCryptographicKey( _In_ HCRYPTKEY source_handle, _In_ bool automatically_destroy ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_Key                  = static_cast< HCRYPTKEY >( NULL );
   m_AutomaticallyDestroy = false;

   std::ignore = FromHandle( source_handle, automatically_destroy );
}

Win32FoundationClasses::CCryptographicKey::~CCryptographicKey()
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AutomaticallyDestroy == true)
   {
       std::ignore = Destroy();
   }
}

// Methods

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::Decrypt(_In_ std::vector<uint8_t> const&          data_to_decrypt,
    _Out_ std::vector<uint8_t>&          decrypted_data,
    _In_ bool const                 this_is_the_last_chunk_of_data_to_be_decrypted,
    __in_opt Win32FoundationClasses::CCryptographicHash * hash_p,
    _In_ DWORD const               flags,
    _In_ bool const                use_wfc_trim) noexcept
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER_NULL_OK(hash_p);

    // This is really stupid. The CryptDecrypt function takes an in/out
    // for the buffer parameter. Besides not being const correct, it negates
    // the possibility of using memory mapped files. Normally, Microsoft
    // would notice something like this. This must be some pinhead's idea.

    auto hash_handle{ static_cast<HCRYPTHASH>(NULL) };

    if (hash_p == nullptr)
    {
        //WFCTRACE( TEXT( "Defaulting to no hash" ) );
        hash_handle = static_cast<HCRYPTHASH>(NULL);
    }

    // We were passed a pointer, don't trust it...

    WFC_TRY
    {
       if (hash_p not_eq nullptr)
       {
          hash_handle = hash_p->GetHandle();
       }

    // Copy the data to decrypt over to the output buffer...

    decrypted_data.assign(data_to_decrypt.cbegin(), data_to_decrypt.cend());

    _ASSERTE(decrypted_data.size() <= 0xFFFFFFFF); // To detect 32-bit overruns...

    auto data_length{ (DWORD)decrypted_data.size() };

    auto return_value{ ::CryptDecrypt(m_Key,
                                   hash_handle,
                                   this_is_the_last_chunk_of_data_to_be_decrypted,
                                   flags,
                                   decrypted_data.data(),
                                  &data_length) };

    if (return_value == FALSE)
    {
       m_ErrorCode = ::GetLastError();

#if defined( _DEBUG )

       // Spit out a nice debugging message, these are all of the valid
       // error codes as documented in VC5

       std::wstring debug_message;

       switch (m_ErrorCode)
       {
          case ERROR_INVALID_HANDLE:

             debug_message.assign(WSTRING_VIEW(L"ERROR_INVALID_HANDLE - One of the parameters specifies an invalid handle."));
             break;

          case ERROR_INVALID_PARAMETER:

             debug_message.assign(WSTRING_VIEW(L"ERROR_INVALID_PARAMETER - One of the parameters contains an invalid value. This is most often an illegal pointer."));
             break;

          case NTE_BAD_ALGID:

             debug_message.assign(WSTRING_VIEW(L"NTE_BAD_ALGID - The hKey session key specifies an algorithm that this CSP does not support."));
             break;

          case NTE_BAD_DATA:

             debug_message.assign(WSTRING_VIEW(L"NTE_BAD_DATA - The data to be decrypted is invalid. For example, when a block cipher is used and the Final flag FALSE, the value specified by pdwDataLen must be a multiple of the block size. This error can also be returned when the padding is found to be invalid."));
             break;

          case NTE_BAD_FLAGS:

             debug_message.assign(WSTRING_VIEW(L"NTE_BAD_FLAGS - The dwFlags parameter is nonzero."));
             break;

          case NTE_BAD_HASH:

             debug_message.assign(WSTRING_VIEW(L"NTE_BAD_HASH - The hHash parameter contains an invalid handle."));
             break;

          case NTE_BAD_KEY:

             debug_message.assign(WSTRING_VIEW(L"NTE_BAD_KEY - The hKey parameter does not contain a valid handle to a key."));
             break;

          case NTE_BAD_LEN:

             debug_message.assign(WSTRING_VIEW(L"NTE_BAD_LEN - The size of the output buffer is too small to hold the generated plaintext."));
             break;

          case NTE_BAD_UID:

             debug_message.assign(WSTRING_VIEW(L"NTE_BAD_UID - The CSP context that was specified when the key was created cannot be found."));
             break;

          case NTE_DOUBLE_ENCRYPT:

             debug_message.assign(WSTRING_VIEW(L"NTE_DOUBLE_ENCRYPT - The application attempted to decrypt the same data twice."));
             break;

          case NTE_FAIL:

             debug_message.assign(WSTRING_VIEW(L"NTE_FAIL - The function failed in some unexpected way."));
             break;

          default:

             debug_message.assign(WSTRING_VIEW(L"Undocumented error "));
             debug_message.append(std::to_wstring(m_ErrorCode));
             break;
       }

       wprintf(L"%s\n", debug_message.c_str());

#endif // _DEBUG

       // OK, it failed but this may not be an unrecoverable error

       if (m_ErrorCode == NTE_BAD_LEN)
       {
           //WFCTRACEVAL( TEXT( "This size wasn't enough " ), (DWORD) data_to_decrypt.GetSize() );
           //WFCTRACEVAL( TEXT( "I had to expand to " ), data_length );

           // Ah! We need to expand our buffer to make room...

           decrypted_data.resize(data_length);

           //WFCTRACE( TEXT( "Trying again" ) );

           return_value = ::CryptDecrypt(m_Key,
                                          hash_handle,
                                          this_is_the_last_chunk_of_data_to_be_decrypted,
                                          flags,
                                          decrypted_data.data(),
                                         &data_length);

           if (return_value == FALSE)
           {
              m_ErrorCode = ::GetLastError();

              //WFCTRACE( TEXT( "Second attempt failed." ) );
              //WFCTRACEERROR( m_ErrorCode );

              return(false);
           }
           else
           {
               // Now that we've decrypted the data, let's strip off any padding

               if (use_wfc_trim == true)
               {
                   std::ignore = wfc_trim_decrypted_data(decrypted_data);
               }
            }

            return(true);
         }

         return(false);
      }
      else
      {
        // Now that we've decrypted the data, let's strip off any padding

        if (use_wfc_trim == true)
        {
            std::ignore = wfc_trim_decrypted_data(decrypted_data);
        }
     }
    }
        WFC_CATCH_ALL
    {
        // Something bad happened. Don't let any data out

        m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
        decrypted_data.clear();
        return(false);
    }
        WFC_END_CATCH_ALL

        return(true);
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::Destroy( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Key == static_cast<HCRYPTKEY>( NULL ) )
   {
      m_AutomaticallyDestroy = false;
      return( true );
   }

   auto return_value{ ::CryptDestroyKey(m_Key) };

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();
      //WFCTRACEERROR( m_ErrorCode );
   }
   else
   {
      return_value = TRUE;
   }

   m_Key                  = static_cast<HCRYPTKEY>( NULL );
   m_AutomaticallyDestroy = false;

   return( return_value == FALSE ? false : true );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::Encrypt( std::vector<uint8_t> const&          data_to_encrypt,
    std::vector<uint8_t>&          encrypted_data,
                                       bool                 this_is_the_last_chunk_of_data_to_be_encrypted,
    Win32FoundationClasses::CCryptographicHash * hash_p,
                                       DWORD                flags,
                                       bool                 encode_size ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( hash_p );

   // This is really stupid. The CryptEncrypt function takes an in/out
   // for the buffer parameter. Besides not being const correct, it negates
   // the possibility of using memory mapped files. Normally, Microsoft
   // would notice something like this. This must be some pinhead's idea.

   auto hash_handle{ static_cast<HCRYPTHASH>(NULL) };

   if ( hash_p == nullptr )
   {
      //WFCTRACE( TEXT( "Defaulting to no hash" ) );
      hash_handle = static_cast< HCRYPTHASH >( NULL );
   }

   // We were passed a pointer, don't trust it...

   WFC_TRY
   {
      if ( hash_p not_eq nullptr )
      {
         hash_handle = hash_p->GetHandle();
      }

      // We must correct for a design flaw in the CryptoAPI. What you
      // send to be encrypted is not what you get back from the
      // decrypt. Most of the time your data will be padded with
      // bytes to fill out a "block size". THIS IS STUPID! When we
      // read data from a file we do not get an entire cluster of
      // data back. Why should we put up with this from the CryptoAPI?

      // Copy the data to encrypt over to the output buffer...

      encrypted_data = data_to_encrypt;

      _ASSERTE( encrypted_data.size() <= 0xFFFFFFFF );

      auto number_of_bytes_to_encrypt{ static_cast<DWORD>(encrypted_data.size()) };

      if ( encode_size == true )
      {
         // Now, to correct for the design flaw, add the number of bytes
         // Write the length

         uint8_t const byte_1{ static_cast<uint8_t>(number_of_bytes_to_encrypt bitand 0xFF) };
         number_of_bytes_to_encrypt >>= 8;

         uint8_t const byte_2{ static_cast<uint8_t>(number_of_bytes_to_encrypt bitand 0xFF) };
         number_of_bytes_to_encrypt >>= 8;

         uint8_t const byte_3{ static_cast<uint8_t>(number_of_bytes_to_encrypt bitand 0xFF) };
         number_of_bytes_to_encrypt >>= 8;

         uint8_t const byte_4{ static_cast<uint8_t>(number_of_bytes_to_encrypt bitand 0xFF) };

         // Make room for the length

         encrypted_data.insert( std::begin(encrypted_data) + 0, 4, 0 );

         encrypted_data.at( 0 ) = byte_1;
         encrypted_data.at( 1 ) = byte_2;
         encrypted_data.at( 2 ) = byte_3;
         encrypted_data.at( 3 ) = byte_4;

         number_of_bytes_to_encrypt = (DWORD) encrypted_data.size();
      }
      
      DWORD required_buffer_size{ number_of_bytes_to_encrypt };

      auto return_value{ ::CryptEncrypt(m_Key,
                                     hash_handle,
                                     this_is_the_last_chunk_of_data_to_be_encrypted,
                                     flags,
                                     encrypted_data.data(),
                                    &required_buffer_size,
                                     number_of_bytes_to_encrypt) };

      if ( return_value == FALSE )
      {
         m_ErrorCode = ::GetLastError();

#if defined( _DEBUG )

         // Spit out a nice debugging message, these are all of the valid
         // error codes as documented in VC5

         std::wstring debug_message;

         switch( m_ErrorCode )
         {
            case ERROR_INVALID_HANDLE:

               debug_message.assign(WSTRING_VIEW(L"ERROR_INVALID_HANDLE - One of the parameters specifies an invalid handle." ) );
               break;

            case ERROR_INVALID_PARAMETER:

               debug_message.assign(WSTRING_VIEW(L"ERROR_INVALID_PARAMETER - One of the parameters contains an invalid value. This is most often an illegal pointer." ) );
               break;

            case NTE_BAD_ALGID:

               debug_message.assign(WSTRING_VIEW(L"NTE_BAD_ALGID - The hKey session key specifies an algorithm that this CSP does not support." ) );
               break;

            case NTE_BAD_DATA:

               debug_message.assign(WSTRING_VIEW(L"NTE_BAD_DATA - The data to be encrypted is invalid. For example, when a block cipher is used and the Final flag is FALSE, the value specified by pdwDataLen must be a multiple of the block size." ) );
               break;

            case NTE_BAD_FLAGS:

               debug_message.assign(WSTRING_VIEW(L"NTE_BAD_FLAGS - The dwFlags parameter is nonzero." ) );
               break;

            case NTE_BAD_HASH:

               debug_message.assign(WSTRING_VIEW(L"NTE_BAD_HASH - The hHash parameter contains an invalid handle." ) );
               break;

            case NTE_BAD_KEY:

               debug_message.assign(WSTRING_VIEW(L"NTE_BAD_KEY - The hKey parameter does not contain a valid handle to a key." ) );
               break;

            case NTE_BAD_LEN:

               debug_message.assign(WSTRING_VIEW(L"NTE_BAD_LEN - The size of the output buffer is too small to hold the generated ciphertext." ) );
               break;

            case NTE_BAD_UID:

               debug_message.assign(WSTRING_VIEW(L"NTE_BAD_UID - The CSP context that was specified when the key was created cannot be found." ) );
               break;

            case NTE_DOUBLE_ENCRYPT:

               debug_message.assign(WSTRING_VIEW(L"NTE_DOUBLE_ENCRYPT - The application attempted to encrypt the same data twice." ) );
               break;

            case NTE_FAIL:

               debug_message.assign(WSTRING_VIEW(L"NTE_FAIL - The function failed in some unexpected way." ) );
               break;

            case NTE_NO_MEMORY:

               debug_message.assign(WSTRING_VIEW(L"NTE_NO_MEMORY - The CSP ran out of memory during the operation." ) );
               break;

            default:

               debug_message.assign(WSTRING_VIEW(L"Undocumented error "));
               debug_message.append(std::to_wstring(m_ErrorCode));
               break;
         }

         wprintf(L"%s\n", debug_message.c_str());

#endif // _DEBUG

         //WFCTRACE( TEXT( "First attempt to encrypt failed." ) );

         // OK, it failed but this may not be an unrecoverable error

         // ERROR_MORE_DATA is an undocumented return code.

         if ( m_ErrorCode == ERROR_MORE_DATA )
         {
            //WFCTRACEVAL( TEXT( "This size wasn't enough " ), number_of_bytes_to_encrypt );
            //WFCTRACEVAL( TEXT( "I had to expand to " ), required_buffer_size );

            // Ah! We need to expand our buffer to make room...

            encrypted_data.resize( required_buffer_size );

            //WFCTRACE( TEXT( "Trying again" ) );

            return_value = ::CryptEncrypt( m_Key,
                                           hash_handle,
                                           this_is_the_last_chunk_of_data_to_be_encrypted,
                                           flags,
                                           encrypted_data.data(),
                                          &number_of_bytes_to_encrypt,
                                           required_buffer_size );

            if ( return_value == FALSE )
            {
               m_ErrorCode = ::GetLastError();

               //WFCTRACEVAL( TEXT( "Second attempt failed with " ), m_ErrorCode );
               //WFCTRACEERROR( m_ErrorCode );

               return( false );
            }

            return( true );
         }

         return( false );
      }
   }
   WFC_CATCH_ALL
   {
      // Something bad happened. Don't let any data out

      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      encrypted_data.clear();
      return( false );
   }
   WFC_END_CATCH_ALL

   return( true );
}
 
_Check_return_ bool Win32FoundationClasses::CCryptographicKey::Export(Win32FoundationClasses::CCryptographicKey& key_for_whoever_we_are_exporting_to,
    std::vector<uint8_t>&        key_in_exported_form,
    KeyFormat format_of_exported_key,
    DWORD              flags ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD number_of_bytes_in_exported_key{ 0 };

   auto their_key{ key_for_whoever_we_are_exporting_to.m_Key };

   if ( format_of_exported_key == KeyFormat::PublicKey )
   {
      // This parameter is unused for exporting your public key (which does not require protection)
      their_key = static_cast<HCRYPTKEY>( NULL );
   }

   // Figure out how big an exported key is

   auto return_value{ ::CryptExportKey(m_Key,
                                    their_key,
                                    static_cast<DWORD>(format_of_exported_key),
                                    flags,
                                    nullptr,
                                   &number_of_bytes_in_exported_key) };

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();

      //WFCTRACE( TEXT( "Can't determine length of exported key." ) );
      //WFCTRACEERROR( m_ErrorCode );

      return( false );
   }

   // Now that we know how big an exported key is, set our size...

   key_in_exported_form.resize( number_of_bytes_in_exported_key );

   return_value = ::CryptExportKey( m_Key,
                                    their_key,
                                    static_cast<DWORD>(format_of_exported_key),
                                    flags,
                                    key_in_exported_form.data(),
                                   &number_of_bytes_in_exported_key );

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();

#if defined( _DEBUG )

      // Spit out a nice debugging message, these are all of the valid
      // error codes as documented in VC5

      std::wstring debug_message;

      switch( m_ErrorCode )
      {
         case ERROR_INVALID_HANDLE:

            debug_message.assign(WSTRING_VIEW(L"ERROR_INVALID_HANDLE - One of the parameters specifies an invalid handle." ) );
            break;

         case ERROR_INVALID_PARAMETER:

            debug_message.assign(WSTRING_VIEW(L"ERROR_INVALID_PARAMETER - One of the parameters contains an invalid value. This is most often an illegal pointer." ) );
            break;

         case NTE_BAD_FLAGS:

            debug_message.assign(WSTRING_VIEW(L"NTE_BAD_FLAGS - The dwFlags parameter is nonzero." ) );
            break;

         case NTE_BAD_KEY:

            debug_message.assign(WSTRING_VIEW(L"NTE_BAD_KEY - One or both of the keys specified by hKey and hExpKey are invalid." ) );
            break;

         case NTE_BAD_KEY_STATE:

            debug_message.assign(WSTRING_VIEW(L"NTE_BAD_KEY_STATE - You do not have permission to export the key. That is, when the hKey key was created, the CRYPT_EXPORTABLE flag was not specified" ) );
            break;

         case NTE_BAD_PUBLIC_KEY:

            debug_message.assign(WSTRING_VIEW(L"NTE_BAD_PUBLIC_KEY - The key blob type specified by dwBlobType is PUBLICKEYBLOB, but hExpKey does not contain a public key handle." ) );
            break;

         case NTE_BAD_TYPE:

            debug_message.assign(WSTRING_VIEW(L"NTE_BAD_TYPE - The dwBlobType parameter specifies an unknown blob type." ) );
            break;

         case NTE_BAD_UID:

            debug_message.assign(WSTRING_VIEW(L"NTE_BAD_UID - The CSP context that was specified when the hKey key was created cannot be found." ) );
            break;

         case NTE_NO_KEY:

            debug_message.assign(WSTRING_VIEW(L"NTE_NO_KEY - A session key is being exported and the hExpKey parameter does not specify a public key." ) );
            break;

         default:

            debug_message.assign(WSTRING_VIEW(L"Undocumented error "));
            debug_message.append(std::to_wstring(m_ErrorCode));
            break;
      }

      wprintf(L"%s\n", debug_message.c_str());

#endif // _DEBUG

      //WFCTRACEVAL( TEXT( "Can't export key after determining the length is " ), number_of_bytes_in_exported_key );

      return( false );
   }

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::FromHandle( _In_ HCRYPTKEY source_handle, _In_ bool automatically_destroy ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // See if we are already handling a key

   if ( m_Key not_eq static_cast< HCRYPTKEY >( NULL ) )
   {
      if ( m_AutomaticallyDestroy == true)
      {
          std::ignore = Destroy();
      }
   }

   m_Key                  = source_handle;
   m_AutomaticallyDestroy = automatically_destroy;

   return( true );
}

_Check_return_ HCRYPTKEY Win32FoundationClasses::CCryptographicKey::GetHandle( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_Key );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::GetParameter( _In_ Parameter const parameter_to_get, __inout_bcount( buffer_length ) uint8_t * buffer, __inout DWORD& buffer_length, _In_ DWORD const flags ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( buffer );

   auto return_value{ ::CryptGetKeyParam(m_Key, static_cast<DWORD>(parameter_to_get), buffer, &buffer_length, flags) };

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();

#if defined( _DEBUG )

      // Spit out a nice debugging message, these are all of the valid
      // error codes as documented in VC5

      std::wstring debug_message;

      switch( m_ErrorCode )
      {
         case ERROR_INVALID_HANDLE:

            debug_message.assign(WSTRING_VIEW(L"ERROR_INVALID_HANDLE - One of the parameters specifies an invalid handle." ) );
            break;

         case ERROR_INVALID_PARAMETER:

            debug_message.assign(WSTRING_VIEW(L"ERROR_INVALID_PARAMETER - One of the parameters contains an invalid value. This is most often an illegal pointer." ) );
            break;

         case NTE_BAD_FLAGS:

            debug_message.assign(WSTRING_VIEW(L"NTE_BAD_FLAGS - The dwFlags parameter is nonzero." ) );
            break;

         case NTE_BAD_KEY:

            debug_message.assign(WSTRING_VIEW(L"NTE_BAD_KEY - The key specified by the hKey parameter is invalid." ) );
            break;

         case NTE_NO_KEY:

            debug_message.assign(WSTRING_VIEW(L"NTE_NO_KEY - The key specified by the hKey parameter does not exist." ) );
            break;

         case NTE_BAD_TYPE:

            debug_message.assign(WSTRING_VIEW(L"NTE_BAD_TYPE - The dwParam parameter specifies an unknown parameter number." ) );
            break;

         case NTE_BAD_UID:

            debug_message.assign(WSTRING_VIEW(L"NTE_BAD_UID - The CSP context that was specified when the hKey key was created cannot be found." ) );
            break;

         default:

            debug_message.assign(WSTRING_VIEW(L"Undocumented error "));
            debug_message.append(std::to_wstring(m_ErrorCode));
            break;
      }

      wprintf(L"%s\n", debug_message.c_str());

#endif // _DEBUG
   }

   return( return_value == FALSE ? false : true );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::GetAlgorithmIdentifier( _Out_ DWORD& identifier ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto buffer_length{ static_cast<DWORD>(sizeof(identifier)) };

   return( GetParameter( Parameter::AlgorithmIdentifier, (BYTE *) &identifier, buffer_length ) );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::GetBlockLength(_Out_ DWORD& block_length ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD buffer_length{ sizeof(block_length) };

   return( GetParameter( Parameter::BlockLengthInBytes, (BYTE *) &block_length, buffer_length ) );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::GetCipherMode(_Out_ DWORD& cipher_mode ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD buffer_length{ sizeof(cipher_mode) };

   return( GetParameter( Parameter::CipherMode, (BYTE *) &cipher_mode, buffer_length ) );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::GetInitializationVector(_Out_ std::vector<uint8_t>& initialization_vector ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   initialization_vector.resize( 65536 ); // A Massive initialization vector

   auto buffer_length{ static_cast<DWORD>(initialization_vector.size()) };
   auto const return_value{ GetParameter(Parameter::InitializationVector, initialization_vector.data(), buffer_length) };

   if ( return_value == true)
   {
      initialization_vector.resize( buffer_length );
   }

   return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::GetNumberOfBitsProcessedPerCycle(_Out_ DWORD& number_of_bits ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD buffer_length{ sizeof(number_of_bits) };

   return( GetParameter( Parameter::NumberOfBitsProcessedPerCycle, (BYTE *) &number_of_bits, buffer_length ) );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::GetPaddingMode(_Out_ DWORD& padding_mode ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD buffer_length{ sizeof(padding_mode) };

   return( GetParameter( Parameter::PaddingMode, (BYTE *) &padding_mode, buffer_length ) );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::GetPermissions(_Out_ DWORD& permissions ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD buffer_length{ sizeof(permissions) };

   return( GetParameter( Parameter::Permissions, (BYTE *) &permissions, buffer_length ) );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::GetSalt( _Out_ std::vector<uint8_t>& salt ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   salt.resize( 65536 ); // A Massive grain of salt

   DWORD buffer_length{ (DWORD)salt.size() };

   bool const return_value{ GetParameter(Parameter::Salt, salt.data(), buffer_length) };

   if ( return_value == true)
   {
      salt.resize( buffer_length );
   }

   return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::SetCipherMode( _In_ CipherMode const mode ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   return( SetParameter( Parameter::CipherMode, (BYTE *) &mode ) );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::SetInitializationVector( _In_ BYTE const * buffer ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( this );

   return( SetParameter( Parameter::InitializationVector, buffer ) );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::SetNumberOfBitsProcessedPerCycle( _In_ DWORD const number_of_bits ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   return( SetParameter( Parameter::NumberOfBitsProcessedPerCycle, (BYTE *) &number_of_bits ) );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::SetPaddingMode( _In_ DWORD const mode ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   return( SetParameter( Parameter::PaddingMode, (BYTE const *) &mode ) );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::SetParameter( _In_ Parameter const parameter_to_set, _In_ uint8_t const * buffer, _In_ DWORD const flags ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( buffer );

   // The buffer parameter to CryptSetKeyParam is not const correct. It is
   // described in the documentation as an [in] parameter. This means the
   // CryptoAPI promises not to modify it. However, the parameter is not
   // typed as const. This means they screwed up the prototype.

   auto return_value{ ::CryptSetKeyParam(m_Key, static_cast<DWORD>(parameter_to_set), buffer, flags) };

   if ( return_value == FALSE )
   {
      m_ErrorCode = ::GetLastError();

#if defined( _DEBUG )

      // Spit out a nice debugging message, these are all of the valid
      // error codes as documented in VC5

      std::wstring debug_message;

      switch( m_ErrorCode )
      {
         case ERROR_INVALID_HANDLE:

            debug_message.assign(WSTRING_VIEW(L"ERROR_INVALID_HANDLE - One of the parameters specifies an invalid handle." ) );
            break;

         case ERROR_BUSY:

            debug_message.assign(WSTRING_VIEW(L"ERROR_BUSY - The CSP context is currently being used by another process (i.e. you've got a crappy crypto provider)." ) );
            break;

         case ERROR_INVALID_PARAMETER:

            debug_message.assign(WSTRING_VIEW(L"ERROR_INVALID_PARAMETER - One of the parameters contains an invalid value. This is most often an illegal pointer." ) );
            break;

         case NTE_BAD_FLAGS:

            debug_message.assign(WSTRING_VIEW(L"NTE_BAD_FLAGS - The dwFlags parameter is nonzero or the pbData buffer contains an invalid value." ) );
            break;

         case NTE_BAD_TYPE:

            debug_message.assign(WSTRING_VIEW(L"NTE_BAD_TYPE - The dwParam parameter specifies an unknown parameter number." ) );
            break;

         case NTE_BAD_UID:

            debug_message.assign(WSTRING_VIEW(L"NTE_BAD_UID - The CSP context that was specified when the hKey key was created cannot be found." ) );
            break;

         case NTE_FAIL:

            debug_message.assign(WSTRING_VIEW(L"NTE_FAIL - The function failed in some unexpected way." ) );
            break;

         default:

            debug_message.assign(WSTRING_VIEW(L"Undocumented error "));
            debug_message.append(std::to_wstring(m_ErrorCode));
            break;
      }

      wprintf(L"%s\n", debug_message.c_str());

#endif // _DEBUG
   }
   else
   {
      return_value = TRUE;
   }

   return( return_value == FALSE ? false : true );
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::SetPermissions( _In_ DWORD permissions ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   return( SetParameter( Parameter::Permissions, static_cast<uint8_t *>(IntToPtr(permissions) )));
}

_Check_return_ bool Win32FoundationClasses::CCryptographicKey::SetSalt( _In_ uint8_t const * buffer ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   return( SetParameter( Parameter::Salt, buffer ) );
}

#endif // _MSC_VER

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CCryptographicKey</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles CryptoAPI keys.">
</HEAD>

<BODY>

<H1>CCryptographicKey : <A HREF="CCryptography.htm">CCryptography</A></H1>
$Revision: 34 $<BR><HR>

<H2>Description</H2>

This class encapsulates cryptographic keys.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CCryptographicKey</B>()
<B>CCryptographicKey</B>( HCRYPTKEY source_handle, BOOL automatically_destroy = TRUE )</PRE><DD>
Constructs a new object or allows you to treat an existing HCRYPTKEY as an object.

</DL>

<H2>Data Members</H2>None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="Decrypt">Decrypt</A></B>( const std::vector&lt;uint8_t&gt;&amp;          data_to_decrypt,
                    std::vector&lt;uint8_t&gt;&amp;          decrypted_data, 
                    BOOL                 this_is_the_last_chunk_of_data_to_be_decrypted = TRUE,
                    <A HREF="CCryptographicHash.htm">CCryptographicHash</A> * hash_p = nullptr,
                    DWORD                flags = 0 )</PRE><DD>
This method decrypts data.

<DT><PRE>BOOL <B><A NAME="Destroy">Destroy</A></B>( void )</PRE><DD>
Destroys the key.

<DT><PRE>BOOL <B><A NAME="Encrypt">Encrypt</A></B>( const std::vector&lt;uint8_t&gt;&amp;          data_to_encrypt,
                    std::vector&lt;uint8_t&gt;&amp;          encrypted_data,
                    BOOL                 this_is_the_last_chunk_to_be_encrypted = TRUE,
                    <A HREF="CCryptographicHash.htm">CCryptographicHash</A> * hash_p = nullptr,
                    DWORD                flags  = 0 )</PRE><DD>
This method encrypts data.

<DT><PRE>BOOL <B><A NAME="Export">Export</A></B>( CCryptographicKey&amp; key_for_whoever_we_are_exporting_to,
                    std::vector&lt;uint8_t&gt;&amp; key_in_exported_form,
                    DWORD       export_format = formatSimple,
                    DWORD       flags         = 0 )</PRE><DD>
Exports the key so you can send it to others.

<DT><PRE>BOOL <B><A NAME="FromHandle">FromHandle</A></B>( HCRYPTKEY source_handle, BOOL automatically_destroy = TRUE )</PRE><DD>
Tells the class to start playing with the HCRYPTKEY provided. If
<CODE>automatically_destroy</CODE> is set to TRUE, <B>Destroy</B>() will be
called by the destructor.

<DT><PRE>BOOL <B><A NAME="GetAlgorithmIdentifier">GetAlgorithmIdentifier</A></B>( DWORD&amp; identifier )</PRE><DD>
Retrieves the identifier for the algorithm. It basically calls <B>GetParameter</B>()
with <CODE>parameterAlgorithmIdentifier</CODE>.

<DT><PRE>BOOL <B><A NAME="GetBlockLength">GetBlockLength</A></B>( DWORD&amp; block_length )</PRE><DD>
Retrieves the block length. It basically calls <B>GetParameter</B>()
with <CODE>parameterBlockLengthInBytes</CODE>.

<DT><PRE>BOOL <B><A NAME="GetCipherMode">GetCipherMode</A></B>( DWORD&amp; cipher_mode )</PRE><DD>
Retrieves the cipher mode. It basically calls <B>GetParameter</B>()
with <CODE>parameterCipherMode</CODE>.
The <CODE>cipher_mode</CODE> will be set to one of the following:
<UL>
<LI>modeCipherBlockChaining
<LI>modeCipherFeedback
<LI>modeElectronicCodebook
<LI>modeOutputFeedback
</UL>

<DT><PRE>HCRYPTKEY <B><A NAME="GetHandle">GetHandle</A></B>( void ) const</PRE><DD>
Returns the underlying HCRYPTKEY so you can call the API directly.

<DT><PRE>BOOL <B><A NAME="GetInitializationVector">GetInitializationVector</A></B>( std::vector&lt;uint8_t&gt;&amp; initialization_vector )</PRE><DD>
Retrieves the initialization vector. It calls <B>GetParameter</B>() with
<CODE>parameterInitializationVector</CODE>.

<DT><PRE>BOOL <B><A NAME="GetNumberOfBitsProcessedPerCycle">GetNumberOfBitsProcessedPerCycle</A></B>( DWORD&amp; number_of_bits )</PRE><DD>
Retrieves the
number of bits processed per cycle if the cipher mode is modeCipherFeedback or
modeOutputFeedback.

<DT><PRE>BOOL <B><A NAME="GetPaddingMode">GetPaddingMode</A></B>( DWORD&amp; padding_mode )</PRE><DD>
Retrieves the padding mode. Currently the only padding mode is PKCS5_PADDING.

<DT><PRE>BOOL <B><A NAME="GetParameter">GetParameter</A></B>( DWORD const parameter_to_get, BYTE *buffer, DWORD&amp; buffer_length, DWORD const flags = 0 )</PRE><DD>
Retrieves one of the following bits of information (i.e. 
<CODE>parameter_to_get</CODE> can be one of):
<UL>
<LI>parameterAlgorithmIdentifier
<LI>parameterBlockLengthInBytes
<LI>parameterCipherMode
<LI>parameterInitializationVector
<LI>parameterNumberOfBitsProcessedPerCycle
<LI>parameterPaddingMode
<LI>parameterPermissions
<LI>parameterSalt
</UL>

<DT><PRE>BOOL <B><A NAME="GetPermissions">GetPermissions</A></B>( DWORD&amp; permissions )</PRE><DD>
Retrieves the permissions.

<DT><PRE>BOOL <B><A NAME="GetSalt">GetSalt</A></B>( std::vector&lt;uint8_t&gt;&amp; salt )</PRE><DD>
Retrieves the Salt.

<DT><PRE>BOOL <B><A NAME="SetCipherMode">SetCipherMode</A></B>( DWORD mode )</PRE><DD>
Sets the cipher mode to be used. The <CODE>mode</CODE> parameter can
be one of the following:
<UL>
<LI>modeCipherBlockChaining
<LI>modeCipherFeedback
<LI>modeElectronicCodebook
<LI>modeOutputFeedback
</UL>

<DT><PRE>BOOL <B><A NAME="SetInitializationVector">SetInitializationVector</A></B>( BYTE * buffer )</PRE><DD>
Sets the initialization vector.

<DT><PRE>BOOL <B><A NAME="SetNumberOfBitsProcessedPerCycle">SetNumberOfBitsProcessedPerCycle</A></B>( DWORD number_of_bits )</PRE><DD>
Sets the number of bits
processed per cycle. This is valid only if the cipher mode is modeCipherFeedback
or modeOutputFeedback.

<DT><PRE>BOOL <B><A NAME="SetPaddingMode">SetPaddingMode</A></B>( DWORD mode )</PRE><DD>
Sets the padding mode to be used by the cipher.

<DT><PRE>BOOL <B><A NAME="SetParameter">SetParameter</A></B>( DWORD const parameter_to_set, BYTE * buffer, DWORD const flags = 0 )</PRE><DD>
Sets a parameter of the key. You can set one of these parameters
(i.e. <CODE>parameter_to_set</CODE> can be one of these values):
<UL>
<LI>parameterCipherMode
<LI>parameterInitializationVector
<LI>parameterNumberOfBitsProcessedPerCycle
<LI>parameterPaddingMode
<LI>parameterPermissions
<LI>parameterSalt
</UL>

<DT><PRE>BOOL <B><A NAME="SetPermissions">SetPermissions</A></B>( DWORD permissions )</PRE><DD>
Sets permissions on the key. The <CODE>permissions</CODE> can be:
<UL>
<LI>permissionDecrypt
<LI>permissionEncrypt
<LI>permissionExport
<LI>permissionMAC
<LI>permissionRead
<LI>permissionWrite
</UL>

<DT><PRE>BOOL <B><A NAME="SetSalt">SetSalt</A></B>( BYTE * buffer )</PRE><DD>
Sets the salt value.

</DL>

<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>

<H2>API's Used</H2>
<UL>
<LI>CryptDecrypt
<LI>CryptDestroyKey
<LI>CryptEncrypt
<LI>CryptExportKey
<LI>CryptGetKeyParam
<LI>CryptSetKeyParam
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CCryptographicKey.cpp $<BR>
$Modtime: 6/26/01 10:44a $
</BODY>

</HTML>
*/
