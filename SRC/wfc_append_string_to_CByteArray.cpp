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
** $Workfile: wfc_append_string_to_.cpp $
** $Revision: 4 $
** $Modtime: 6/26/01 10:57a $
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

void Win32FoundationClasses::wfc_append_string_to_byte_array( _In_ std::wstring const& string_to_append, _Out_ std::vector<uint8_t>& bytes ) noexcept
{
   wfc_append_unicode_string_to_byte_array( string_to_append.c_str(), bytes );
}

void Win32FoundationClasses::wfc_append_ascii_string_to_byte_array( __in_z char const * string_to_append, __inout std::vector<uint8_t>& bytes ) noexcept
{
   std::size_t const string_length = strlen( string_to_append );
   std::size_t const original_array_size = bytes.size();

   bytes.resize( string_length + original_array_size );

   auto buffer = bytes.data();

   CopyMemory( &buffer[ original_array_size ], string_to_append, string_length );
}

void Win32FoundationClasses::wfc_append_unicode_string_to_byte_array( __in_z wchar_t const * string_to_append, __inout std::vector<uint8_t>& bytes ) noexcept
{
   std::size_t const string_length = wcslen( string_to_append ) * sizeof( wchar_t );
   std::size_t const original_array_size = bytes.size();

   bytes.resize( string_length + original_array_size );

   uint8_t * buffer = bytes.data();

   CopyMemory( &buffer[ original_array_size ], string_to_append, string_length );
}

// End of source
