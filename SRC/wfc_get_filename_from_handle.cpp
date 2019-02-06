/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2016, Samuel R. Blackburn
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
** $Workfile: wfc_get_filename_from_handle.cpp $
** $Revision: 4 $
** $Modtime: 6/26/01 10:58a $
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

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_get_filename_from_handle( _In_ const HANDLE file_handle, _Out_ std::wstring& filename ) noexcept
{
    typedef struct _io_status_block
    {
        DWORD Status;
        DWORD Information;
    }
    MY_IO_STATUS_BLOCK;

    typedef struct _file_name_information
    {
        DWORD FileNameLength;
        wchar_t FileName[ 4096 ];
    }
    MY_FILE_NAME_INFORMATION;

    filename.clear();

    const HMODULE ntdll_module_handle = GetModuleHandleW(L"ntdll.dll");

    if (ntdll_module_handle == NULL)
    {
        return(false);
    }

    DWORD (__stdcall *NtQueryInformationFile)( HANDLE, MY_IO_STATUS_BLOCK *, VOID *, DWORD, DWORD );

#pragma warning(disable:4191)
    NtQueryInformationFile = ( DWORD (__stdcall *)( HANDLE, MY_IO_STATUS_BLOCK *, VOID *, DWORD, DWORD) )
        GetProcAddress(ntdll_module_handle, "NtQueryInformationFile" );
#pragma warning(default:4191)

    if ( NtQueryInformationFile == nullptr )
    {
        return( false );
    }

    MY_FILE_NAME_INFORMATION filename_structure;

    ZeroMemory( &filename_structure, sizeof( filename_structure ) );

    MY_IO_STATUS_BLOCK status_block;

    status_block.Status      = 0;
    status_block.Information = 0;

    DWORD status = NtQueryInformationFile( file_handle,
        &status_block,
        &filename_structure,
        sizeof( filename_structure ),
        9 );

    if ( status == 0 )
    {
        filename_structure.FileName[ filename_structure.FileNameLength / sizeof( wchar_t ) ] = 0;
    }
    else
    {
        return( false );
    }

    filename.assign(filename_structure.FileName);

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_get_command_line( _Out_ std::wstring& full_command_line ) noexcept
{
    wchar_t path[ 4096 ];

    ZeroMemory( path, sizeof( path ) );

    if ( GetModuleFileNameW( static_cast< HMODULE >( NULL ), path, static_cast<DWORD>(std::size( path )) ) != 0 )
    {
        full_command_line.assign( path );
        full_command_line.push_back( ' ' );

        int argc = 0;

        LPWSTR * argv = CommandLineToArgvW( GetCommandLineW(), &argc );

        int argv_index = 1;

        while( argv_index < argc )
        {
            full_command_line.append( argv[ argv_index ] );
            full_command_line.push_back( ' ' );
            argv_index++;
        }

        trim_right(full_command_line);

        return( true );
    }

    full_command_line.clear();
    return( false );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_get_filename_from_handle</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that returns a file's name given only its handle.">
</HEAD>

<BODY>

<H1>wfc_get_filename_from_handle</H1>
$Revision: 4 $<HR>

<H2>Declaration</H2>
<PRE>BOOL <B>wfc_get_filename_from_handle</B>( HANDLE file_handle, std::wstring&amp; file_name )</PRE>

<H2>Description</H2>
This function retrieves the name of a file given only its handle.
It does this by using a partially documented function named
<CODE>NtQueryInformationFile</CODE> in NTDLL.DLL.

<H2>Example</H2>
<PRE><CODE>void print_error( DWORD error_code, HANDLE file_handle )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;print_error()&quot; ) );

std::wstring something_a_human_can_understand;

<A HREF="wfc_get_error_string.htm">wfc_get_error_string</A>( error_code, something_a_human_can_understand );

std::wstring filename;

if ( <B>wfc_get_filename_from_handle</B>( file_handle, filename ) != FALSE )
{
_tprintf( TEXT( &quot;%s - %s\n&quot; ),
(LPCTSTR) filename,
(LPCTSTR) something_a_human_can_understand );
}
else
{
_tprintf( TEXT( &quot;%s\n&quot; ), (LPCTSTR) something_a_human_can_understand );
}
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_get_error_string</B>() uses the following API's:
<UL>
<LI>GetModuleHandle
<LI>GetProcAddress
<LI>NtQueryInformationFile
<LI>ZeroMemory
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_get_filename_from_handle.cpp $<BR>
$Modtime: 6/26/01 10:58a $
</BODY>

</HTML>
*/
