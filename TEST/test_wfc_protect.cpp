/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2012, Samuel R. Blackburn
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

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#define new DEBUG_NEW
#endif // _DEBUG

__checkReturn bool test_wfc_protect( __out std::string& class_name, __out int& test_number_that_failed ) noexcept
{
   class_name.assign(STRING_VIEW("wfc_protect_and_unprotect_data" ));

   char plaintext[ 25 ];

   strcpy_s( plaintext, sizeof( plaintext ), "WFC is actually useful." );

   std::size_t size_of_ciphertext_buffer{ 0 };

   if (Win32FoundationClasses::wfc_protect_data( reinterpret_cast<uint8_t const *>(plaintext), strlen( plaintext ) + 1, nullptr, &size_of_ciphertext_buffer ) == false )
   {
      test_number_that_failed = 1;
      return( failure() );
   }

   auto ciphertext{ std::make_unique<uint8_t[]>(size_of_ciphertext_buffer) };

   if (Win32FoundationClasses::wfc_protect_data( reinterpret_cast<uint8_t const *>(plaintext), strlen( plaintext ) + 1, ciphertext.get(), &size_of_ciphertext_buffer ) == false )
   {
      test_number_that_failed = 2;
      return( failure() );
   }

   std::size_t size_of_decrypted{ 0 };

   if (Win32FoundationClasses::wfc_unprotect_data( ciphertext.get(), size_of_ciphertext_buffer, nullptr, &size_of_decrypted ) == FALSE )
   {
      test_number_that_failed = 3;
      return( failure() );
   }

   auto decrypted{ std::make_unique<uint8_t[]>(size_of_decrypted) };

   if (Win32FoundationClasses::wfc_unprotect_data( ciphertext.get(), size_of_ciphertext_buffer, decrypted.get(), &size_of_decrypted ) == FALSE )
   {
      test_number_that_failed = 4;
      return( failure() );
   }

   if ( strcmp( plaintext, reinterpret_cast<char const *>(decrypted.get()) ) not_eq 0 )
   {
      test_number_that_failed = 5;
      return( failure() );
   }

   TCHAR filename[ MAX_PATH + 64 ];

   ZeroMemory( filename, sizeof( filename ) );

   std::ignore = GetWindowsDirectory( filename, MAX_PATH );

   _tcscat_s( filename, std::size( filename ), TEXT( "\\Notepad.exe" ) );

   if (Win32FoundationClasses::wfc_check_digital_signature( filename ) == false )
   {
       class_name.assign(STRING_VIEW("wfc_check_digital_signature()"));
       test_number_that_failed = 6;
       //return( failure() );
   }

   // Now test lanman hashing

   uint8_t lanman_hash[16];

   memset(lanman_hash, 0xAA, sizeof(lanman_hash));

   Win32FoundationClasses::wfc_calculate_lanman_hash(STRING_VIEW(""), lanman_hash);

   // Empty hash AAD3B435B51404EEAAD3B435B51404EE

   if (lanman_hash[0] not_eq 0xaa or
       lanman_hash[1] not_eq 0xd3 or
       lanman_hash[2] not_eq 0xb4 or
       lanman_hash[3] not_eq 0x35 or
       lanman_hash[4] not_eq 0xb5 or
       lanman_hash[5] not_eq 0x14 or
       lanman_hash[6] not_eq 0x04 or
       lanman_hash[7] not_eq 0xee or
       lanman_hash[8] not_eq 0xaa or
       lanman_hash[9] not_eq 0xd3 or
       lanman_hash[10] not_eq 0xb4 or
       lanman_hash[11] not_eq 0x35 or
       lanman_hash[12] not_eq 0xb5 or
       lanman_hash[13] not_eq 0x14 or
       lanman_hash[14] not_eq 0x04 or
       lanman_hash[15] not_eq 0xee )
   {
       class_name.assign(STRING_VIEW("wfc_calculate_lanman_hash()"));
       test_number_that_failed = 7;
       return(failure());
   }

   ZeroMemory(lanman_hash, sizeof(lanman_hash));

   Win32FoundationClasses::wfc_calculate_lanman_hash(STRING_VIEW("hashcat"), lanman_hash);

   // hashcat - NTLM:b4b9b02e6f09a9bd760f388b67351e2b LM: 29 9B D1 28 C1 10 1F D6 AA D3 B4 35 B5 14 04 EE

   if (lanman_hash[0] not_eq 0x29 or
       lanman_hash[1] not_eq 0x9b or
       lanman_hash[2] not_eq 0xd1 or
       lanman_hash[3] not_eq 0x28 or
       lanman_hash[4] not_eq 0xc1 or
       lanman_hash[5] not_eq 0x10 or
       lanman_hash[6] not_eq 0x1f or
       lanman_hash[7] not_eq 0xd6 or
       lanman_hash[8] not_eq 0xaa or
       lanman_hash[9] not_eq 0xd3 or
       lanman_hash[10] not_eq 0xb4 or
       lanman_hash[11] not_eq 0x35 or
       lanman_hash[12] not_eq 0xb5 or
       lanman_hash[13] not_eq 0x14 or
       lanman_hash[14] not_eq 0x04 or
       lanman_hash[15] not_eq 0xee)
   {
       class_name.assign(STRING_VIEW("wfc_calculate_lanman_hash()"));
       test_number_that_failed = 8;
       return(failure());
   }

   ZeroMemory(lanman_hash, sizeof(lanman_hash));

   Win32FoundationClasses::wfc_calculate_nt_hash(WSTRING_VIEW(L"hashcat"), lanman_hash);

   // b4 b9 b0 2e 6f 09 a9 bd 76 0f 38 8b 67 35 1e 2b
   if (lanman_hash[0] not_eq 0xb4 or
       lanman_hash[1] not_eq 0xb9 or
       lanman_hash[2] not_eq 0xb0 or
       lanman_hash[3] not_eq 0x2e or
       lanman_hash[4] not_eq 0x6f or
       lanman_hash[5] not_eq 0x09 or
       lanman_hash[6] not_eq 0xa9 or
       lanman_hash[7] not_eq 0xbd or
       lanman_hash[8] not_eq 0x76 or
       lanman_hash[9] not_eq 0x0f or
       lanman_hash[10] not_eq 0x38 or
       lanman_hash[11] not_eq 0x8b or
       lanman_hash[12] not_eq 0x67 or
       lanman_hash[13] not_eq 0x35 or
       lanman_hash[14] not_eq 0x1e or
       lanman_hash[15] not_eq 0x2b )
   {
       class_name.assign(STRING_VIEW("wfc_calculate_nt_hash()"));
       test_number_that_failed = 9;
       return(failure());
   }

   test_number_that_failed = 9;
   return( true );
}
