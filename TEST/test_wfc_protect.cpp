/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2016, Samuel R. Blackburn
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
** $Workfile: test_CJulianDate.cpp $
** $Revision: 1 $
** $Modtime: 9/15/98 8:58a $
*/

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

__checkReturn bool test_wfc_protect( __out std::string& class_name, __out int& test_number_that_failed ) noexcept
{
   class_name.assign( "wfc_protect_and_unprotect_data" );

   char plaintext[ 25 ];

   strcpy_s( plaintext, sizeof( plaintext ), "WFC is actually useful." );

   std::size_t size_of_ciphertext_buffer = 0;

   if ( wfc_protect_data( reinterpret_cast<uint8_t const *>(plaintext), strlen( plaintext ) + 1, nullptr, &size_of_ciphertext_buffer ) == false )
   {
      test_number_that_failed = 1;
      return( failure() );
   }

   auto ciphertext = std::make_unique<uint8_t[]>(size_of_ciphertext_buffer );

   if ( wfc_protect_data( reinterpret_cast<uint8_t const *>(plaintext), strlen( plaintext ) + 1, ciphertext.get(), &size_of_ciphertext_buffer ) == false )
   {
      test_number_that_failed = 2;
      return( failure() );
   }

   std::size_t size_of_decrypted = 0;

   if ( wfc_unprotect_data( ciphertext.get(), size_of_ciphertext_buffer, nullptr, &size_of_decrypted ) == FALSE )
   {
      test_number_that_failed = 3;
      return( failure() );
   }

   auto decrypted = std::make_unique<uint8_t[]>( size_of_decrypted );

   if ( wfc_unprotect_data( ciphertext.get(), size_of_ciphertext_buffer, decrypted.get(), &size_of_decrypted ) == FALSE )
   {
      test_number_that_failed = 4;
      return( failure() );
   }

   if ( strcmp( plaintext, reinterpret_cast<char const *>(decrypted.get()) ) != 0 )
   {
      test_number_that_failed = 5;
      return( failure() );
   }

   TCHAR filename[ MAX_PATH + 64 ];

   ZeroMemory( filename, sizeof( filename ) );

   (void) GetWindowsDirectory( filename, MAX_PATH );

   _tcscat_s( filename, std::size( filename ), TEXT( "\\Notepad.exe" ) );

   if ( wfc_check_digital_signature( filename ) == false )
   {
       class_name.assign( "wfc_check_digital_signature()" );
       test_number_that_failed = 6;
       //return( failure() );
   }

   // Now test lanman hashing

   uint8_t lanman_hash[16];

   memset(lanman_hash, 0x55, sizeof(lanman_hash));

   wfc_calculate_lanman_hash(nullptr, lanman_hash);

   if (lanman_hash[0] != 0x00)
   {
       class_name.assign("wfc_calculate_lanman_hash()");
       test_number_that_failed = 7;
       return(failure());
   }

   memset(lanman_hash, 0xAA, sizeof(lanman_hash));

   wfc_calculate_lanman_hash("", lanman_hash);

   // Empty hash AAD3B435B51404EEAAD3B435B51404EE

   if (lanman_hash[0] != 0xaa ||
       lanman_hash[1] != 0xd3 ||
       lanman_hash[2] != 0xb4 ||
       lanman_hash[3] != 0x35 ||
       lanman_hash[4] != 0xb5 ||
       lanman_hash[5] != 0x14 ||
       lanman_hash[6] != 0x04 ||
       lanman_hash[7] != 0xee ||
       lanman_hash[8] != 0xaa ||
       lanman_hash[9] != 0xd3 ||
       lanman_hash[10] != 0xb4 ||
       lanman_hash[11] != 0x35 ||
       lanman_hash[12] != 0xb5 ||
       lanman_hash[13] != 0x14 ||
       lanman_hash[14] != 0x04 ||
       lanman_hash[15] != 0xee )
   {
       class_name.assign("wfc_calculate_lanman_hash()");
       test_number_that_failed = 8;
       return(failure());
   }

   ZeroMemory(lanman_hash, sizeof(lanman_hash));

   wfc_calculate_lanman_hash("hashcat", lanman_hash);

   // hashcat - NTLM:b4b9b02e6f09a9bd760f388b67351e2b LM: 29 9B D1 28 C1 10 1F D6 AA D3 B4 35 B5 14 04 EE

   if (lanman_hash[0] != 0x29 ||
       lanman_hash[1] != 0x9b ||
       lanman_hash[2] != 0xd1 ||
       lanman_hash[3] != 0x28 ||
       lanman_hash[4] != 0xc1 ||
       lanman_hash[5] != 0x10 ||
       lanman_hash[6] != 0x1f ||
       lanman_hash[7] != 0xd6 ||
       lanman_hash[8] != 0xaa ||
       lanman_hash[9] != 0xd3 ||
       lanman_hash[10] != 0xb4 ||
       lanman_hash[11] != 0x35 ||
       lanman_hash[12] != 0xb5 ||
       lanman_hash[13] != 0x14 ||
       lanman_hash[14] != 0x04 ||
       lanman_hash[15] != 0xee)
   {
       class_name.assign("wfc_calculate_lanman_hash()");
       test_number_that_failed = 9;
       return(failure());
   }

   ZeroMemory(lanman_hash, sizeof(lanman_hash));

   wfc_calculate_nt_hash(L"hashcat", lanman_hash);

   // b4 b9 b0 2e 6f 09 a9 bd 76 0f 38 8b 67 35 1e 2b
   if (lanman_hash[0] != 0xb4 ||
       lanman_hash[1] != 0xb9 ||
       lanman_hash[2] != 0xb0 ||
       lanman_hash[3] != 0x2e ||
       lanman_hash[4] != 0x6f ||
       lanman_hash[5] != 0x09 ||
       lanman_hash[6] != 0xa9 ||
       lanman_hash[7] != 0xbd ||
       lanman_hash[8] != 0x76 ||
       lanman_hash[9] != 0x0f ||
       lanman_hash[10] != 0x38 ||
       lanman_hash[11] != 0x8b ||
       lanman_hash[12] != 0x67 ||
       lanman_hash[13] != 0x35 ||
       lanman_hash[14] != 0x1e ||
       lanman_hash[15] != 0x2b )
   {
       class_name.assign("wfc_calculate_nt_hash()");
       test_number_that_failed = 10;
       return(failure());
   }

   test_number_that_failed = 10;
   return( true );
}
