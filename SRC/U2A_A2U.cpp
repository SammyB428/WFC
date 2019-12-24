/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2019, Samuel R. Blackburn
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
** $Workfile: u2a_a2u.cpp $
** $Revision: 18 $
** $Modtime: 6/26/01 10:57a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

void PASCAL Win32FoundationClasses::ASCII_to_UNICODE(_In_ std::string_view ansi_string, __out_z LPWSTR unicode_string) noexcept
{
    WFC_VALIDATE_POINTER_NULL_OK(unicode_string);

    if (ansi_string.empty() == true or unicode_string == nullptr)
    {
        return;
    }

    if (ansi_string.data() == (LPCSTR)unicode_string)
    {
        ASSERT(FALSE);
        return;
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       SIZE_T loop_index = 0;

       while (loop_index < ansi_string.length() and ansi_string[loop_index] != 0x00)
       {
          unicode_string[loop_index] = ansi_string[loop_index];
          loop_index++;
       }

       unicode_string[loop_index] = 0; // NULL terminate
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void PASCAL Win32FoundationClasses::UNICODE_to_ASCII(_In_ std::wstring_view unicode_string, __out_z LPSTR ansi_string, __in SSIZE_T number_of_unicode_characters_to_convert, __in const UINT code_page) noexcept
{
    WFC_VALIDATE_POINTER_NULL_OK(ansi_string);

    if (unicode_string.length() == 0 or ansi_string == nullptr)
    {
        return;
    }

    // 2000-04-30
    // Thanks go to Josh Parris (Josh.Parris@auspost.com.au) for finding
    // a bug here. unicode_string cannot equal ansi_string because that will
    // cause WideCharToMultiByte() to fail.

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       if (number_of_unicode_characters_to_convert == (-1))
       {
          number_of_unicode_characters_to_convert = (long)(unicode_string.length() + 1);
       }

       if (number_of_unicode_characters_to_convert > (long)(unicode_string.length() + 1))
       {
          number_of_unicode_characters_to_convert = (long)(unicode_string.length() + 1);
       }

       if (WideCharToMultiByte(code_page, 0, unicode_string.data(), (int)number_of_unicode_characters_to_convert, ansi_string, (int)number_of_unicode_characters_to_convert, nullptr, nullptr) == 0)
       {
          ansi_string[0] = 0x00;
          //WFCTRACEERROR( GetLastError() );
          //WFCTRACE( TEXT( "Conversion from UNICODE to ASCII failed for the above resaon." ) );
       }
    }
        WFC_CATCH_ALL
    {
       WFC_TRY
       {
          ansi_string[0] = 0x00;
       }
       WFC_CATCH_ALL
       {
       }
       WFC_END_CATCH_ALL

       return;
    }
        WFC_END_CATCH_ALL
}

// End of source
