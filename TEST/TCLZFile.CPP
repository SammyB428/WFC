/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2019, Samuel R. Blackburn
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
** $Workfile: TCLZFile.CPP $
** $Revision: 2 $
** $Modtime: 3/08/98 3:31p $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void test_CLZFile( _In_ std::wstring_view lz_file_name ) noexcept
{
    CLZFile lz_file;

    if ( lz_file_name.empty() == true )
    {
        return;
    }

    std::wstring expanded_name;

    (void) lz_file.GetExpandedName( lz_file_name, expanded_name );

    wprintf( L"LZ File \"%.*s\" expanded name is \"%s\"\n", static_cast<int>(lz_file_name.length()), lz_file_name.data(), expanded_name.c_str() );

    if ( lz_file.Open( lz_file_name ) != FALSE )
    {
        CLZFile output_file;

        if ( output_file.Open(WSTRING_VIEW(L"d:\\temp\\winhlp32.exe"), OF_CREATE ) != FALSE )
        {
            if ( output_file.Copy( lz_file ) != FALSE )
            {
                _tprintf( TEXT( "Successfully copied\n" ) );
            }
            else
            {
                std::wstring error_message_string;
                CLZFile::TranslateErrorCode( (int) lz_file.LastError(), error_message_string );
                _tprintf( TEXT( "Can't Copy because %s\n" ), error_message_string.c_str() );
            }
        }
        else
        {
            std::wstring error_message_string;
            CLZFile::TranslateErrorCode( (int) output_file.LastError(), error_message_string );
            _tprintf( TEXT( "Can't open LZ output file because %s\n" ), error_message_string.c_str() );
        }
    }
    else
    {
        std::wstring error_message_string;
        CLZFile::TranslateErrorCode( (int) lz_file.LastError(), error_message_string );
        wprintf( L"Can't open LZ file \"%.*s\" because %s\n", static_cast<int>(lz_file_name.length()), lz_file_name.data(), error_message_string.c_str() );
    }
}
