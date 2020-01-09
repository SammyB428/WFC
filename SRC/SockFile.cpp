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
** $Workfile: sockfile.cpp $
** $Revision: 15 $
** $Modtime: 6/26/01 10:56a $
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

USING_WFC_NAMESPACE

CSimpleSocketFile::CSimpleSocketFile( __in SOCKET const client_id, __in_z LPCTSTR name, __in_z LPCTSTR dotted_ip_address )
{
    WFC_VALIDATE_POINTER( this );

    m_SocketID = client_id;

#if defined( WFC_STL )
    m_FileHandle = (HANDLE) client_id;
#else
    m_hFile    = (CFILE_HFILE) client_id;
#endif

    if ( name == nullptr )
    {
        Name.clear();
    }
    else
    {
        Name.assign( name );
    }

    if ( dotted_ip_address == nullptr )
    {
        Address.clear();
    }
    else
    {
        Address.assign( dotted_ip_address );
    }

    // The filename should be the host name followed by IP address then port number.
    // Example: "Sammy (195.99.72.1)"

#if ! defined( WFC_STL )
    m_strFileName.Format( TEXT( "%s (%s)" ), (LPCTSTR) Name, (LPCTSTR) Address );
#else // WFC_STL

    std::wstring temp_name;
    format(temp_name, L"%s (%s)", Name.c_str(), Address.c_str() );
    m_FileName.assign(temp_name);
#endif // WFC_STL
}

CSimpleSocketFile::~CSimpleSocketFile()
{
    WFC_VALIDATE_POINTER( this );
    Close();
}

_Check_return_ bool CSimpleSocketFile::Open( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( true );
}

#pragma warning( disable : 4100 )

_Check_return_ bool CSimpleSocketFile::Open(_In_ std::filesystem::path const& name, _In_ UINT const port ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    return( true );
}

#pragma warning( default : 4100 )

// End of source
