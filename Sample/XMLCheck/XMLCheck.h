/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 1998-2003, 2013, Samuel R. Blackburn
** All rights reserved.
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

#if ! defined( XML_CHECK_HEADER_FILE )

#define XML_CHECK_HEADER_FILE

#pragma warning( disable: 4206 )
#pragma warning( disable: 4514 )  /* unreferenced inline function has been removed */
#pragma warning( disable: 4710 )  /* not inlined */
#pragma warning( disable: 4917 )  /* a GUID can only be associated with a class, I got this warning when I started compiling for Windows 7 */

//#define WFC_NOFORCE_LIBS
#define WFC_SILENT
#define WFC_STL
#include <wfc.h>
#include <filesystem>

namespace filesystem = std::experimental::filesystem::v1;

USING_WFC_NAMESPACE

inline constexpr _Check_return_ UINT read_open_mode(void) noexcept
{
    return(static_cast<UINT>(static_cast<UINT>(CFile64::OpenFlags::modeRead) | static_cast<UINT>(CFile64::OpenFlags::shareDenyNone) | static_cast<UINT>(CFile64::OpenFlags::shareAllowDelete)));
}

void check_xml_file( wchar_t const * filename ) noexcept;

#endif // XML_CHECK_HEADER_FILE