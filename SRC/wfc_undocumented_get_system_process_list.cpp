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
** $Workfile: wfc_undocumented_get_system_process_list.cpp $
** $Revision: 6 $
** $Modtime: 6/26/01 10:59a $
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

void Win32FoundationClasses::wfc_debug_error_code( _In_ DWORD const error_code ) noexcept
{
    std::wstring error_string;

    Win32FoundationClasses::wfc_get_error_string( error_code, error_string );

    std::wstring error_message;

    format( error_message, L"ERROR %lu - ", error_code );
    error_message.append( error_string.c_str() );
    error_message.append( L"\n" );

    OutputDebugStringW( error_message.c_str() );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_undocumented_get_system_process_list( __out_bcount( size_of_buffer ) BYTE * buffer, __in DWORD const size_of_buffer ) noexcept
{
    HMODULE const ntdll_module_handle = GetModuleHandle(TEXT("ntdll.dll"));

    if (ntdll_module_handle == NULL)
    {
        return(false);
    }

    DWORD (__stdcall *NtQuerySystemInformation)( DWORD, VOID *, DWORD, DWORD * );

#pragma warning(disable:4191)

    NtQuerySystemInformation = ( DWORD (__stdcall *)( DWORD, VOID *, DWORD, DWORD *) )
        GetProcAddress(ntdll_module_handle, "NtQuerySystemInformation" );
#pragma warning(default:4191)

    if ( NtQuerySystemInformation == nullptr )
    {
        return( 0 );
    }

    if ( NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS::SystemProcessInformation, buffer, size_of_buffer, 0 ) != 0 )
    {
        return( 0 );
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_get_process_command_line(_In_ HANDLE const process_id, _Inout_ std::wstring& command_line) noexcept
{
    // This function has no associated import library. You must use the LoadLibrary and GetProcAddress functions to dynamically link to Ntdll.dll.

    command_line.clear();

    auto process_handle = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD) process_id);

    if (is_bad_handle(process_handle) == true)
    {
        DWORD const error_code = GetLastError();
        wfc_debug_error_code(error_code);
        return(false);
    }

    PROCESS_BASIC_INFORMATION pbi;

    ZeroMemory(&pbi, sizeof(pbi));

    ULONG number_of_bytes_written = 0;

    HMODULE const ntdll_module_handle = GetModuleHandle(TEXT("ntdll.dll"));

    if (ntdll_module_handle == NULL)
    {
        return(false);
    }

    DWORD(__stdcall *Nt_QueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

#pragma warning(disable:4191)
    Nt_QueryInformationProcess = (DWORD(__stdcall *)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG))
        GetProcAddress(ntdll_module_handle, "NtQueryInformationProcess");
#pragma warning(default:4191)

    NTSTATUS const return_status = Nt_QueryInformationProcess(process_handle, ProcessBasicInformation, &pbi, sizeof(pbi), &number_of_bytes_written);

    _ASSERTE(return_status == ERROR_SUCCESS);

    if (return_status != ERROR_SUCCESS)
    {
        DWORD const error_code = GetLastError();
        wfc_debug_error_code(error_code);
        return(false);
    }

    // 1. find the Process Environment Block 
    PEB process_environment_block;

    ZeroMemory(&process_environment_block, sizeof(process_environment_block));

    SIZE_T number_of_bytes_read = 0;

    if (::ReadProcessMemory(process_handle, pbi.PebBaseAddress, &process_environment_block, sizeof(process_environment_block), &number_of_bytes_read) == FALSE)
    {
        DWORD const error_code = GetLastError();
        wfc_debug_error_code(error_code);
        (void)wfc_close_handle(process_handle);
        return(false);
    }

    RTL_USER_PROCESS_PARAMETERS process_parameters;

    ZeroMemory(&process_parameters, sizeof(process_parameters));

    if (::ReadProcessMemory(process_handle, (LPVOID)process_environment_block.ProcessParameters, &process_parameters, sizeof(process_parameters), &number_of_bytes_read) == FALSE)
    {
        DWORD const error_code = GetLastError();
        wfc_debug_error_code(error_code);
        (void)wfc_close_handle(process_handle);
        return(false);
    }

    DWORD number_of_characters = 32769;
    DWORD number_of_bytes_in_command_line_string = number_of_characters * sizeof(wchar_t);

    auto wide_command_line_string = (wchar_t *)_aligned_malloc(number_of_bytes_in_command_line_string, 4096); // Need aligned memory

    WFC_VALIDATE_POINTER(wide_command_line_string);

    if (wide_command_line_string == nullptr)
    {
        (void)wfc_close_handle(process_handle);
        return(false);
    }

    ZeroMemory(wide_command_line_string, number_of_bytes_in_command_line_string);

    if (::ReadProcessMemory(process_handle, (LPVOID)process_parameters.CommandLine.Buffer,
        wide_command_line_string,
        std::min((DWORD)process_parameters.CommandLine.Length, (number_of_bytes_in_command_line_string - 2)),
        &number_of_bytes_read) == FALSE)
    {
        DWORD const error_code = GetLastError();
        wfc_debug_error_code(error_code);
        (void)wfc_close_handle(process_handle);
        _aligned_free(wide_command_line_string);
        return(false);
    }

    (void)wfc_close_handle(process_handle);

    command_line.assign(wide_command_line_string);

    _aligned_free(wide_command_line_string);
    return(true);
}

void PASCAL Win32FoundationClasses::wfc_calculate_lanman_hash(__in_z char const * ascii_string, __out_bcount(16) uint8_t * hash_value) noexcept
{
    // This function has no associated import library. You must use the LoadLibrary and GetProcAddress functions to dynamically link to Ntdll.dll.
    static HMODULE advapi32_module_handle = NULL;

    ZeroMemory(hash_value, 16);

    if (advapi32_module_handle == NULL)
    {
        advapi32_module_handle = GetModuleHandleW(L"advapi32.dll");
    }

    if (advapi32_module_handle == NULL)
    {
        // Need to load library
        advapi32_module_handle = LoadLibraryW(L"advapi32.dll");
    }

    if (advapi32_module_handle == NULL)
    {
        return;
    }

    static DWORD(__stdcall *SystemFunction006)(LPCSTR, LPBYTE) = nullptr;

#pragma warning(disable:4191)
    if (SystemFunction006 == nullptr)
    {
        SystemFunction006 = (DWORD(__stdcall *)(LPCSTR, LPBYTE)) GetProcAddress(advapi32_module_handle, "SystemFunction006");

        if (SystemFunction006 == nullptr)
        {
            return;
        }
    }
#pragma warning(default:4191)

    if (ascii_string == nullptr)
    {
        return;
    }

    if (ascii_string[0] == 0x00)
    {
        // Empty hash
        NTSTATUS const return_status = SystemFunction006(ascii_string, hash_value);
        _ASSERTE(return_status == ERROR_SUCCESS);
    }

    // To duplicate the behavior LanMan hashing, we need to convert the string to upper case

    std::size_t const upper_case_string_size = strlen(ascii_string) + 1;

    auto upper_case_string = std::make_unique<char []>(upper_case_string_size);

    if (upper_case_string.get() == nullptr)
    {
        return;
    }

    strcpy_s(upper_case_string.get(), upper_case_string_size, ascii_string);
    _strupr_s(upper_case_string.get(), upper_case_string_size);

    NTSTATUS const return_status = SystemFunction006(upper_case_string.get(), hash_value);
    _ASSERTE(return_status == ERROR_SUCCESS);
}

void PASCAL Win32FoundationClasses::wfc_calculate_nt_hash(__in_z wchar_t const * unicode_string, __out_bcount(16) uint8_t * hash_value) noexcept
{
    // This function has no associated import library. You must use the LoadLibrary and GetProcAddress functions to dynamically link to Ntdll.dll.
    static HMODULE advapi32_module_handle = NULL;

    ZeroMemory(hash_value, 16);

    if (advapi32_module_handle == NULL)
    {
        advapi32_module_handle = GetModuleHandleW(L"advapi32.dll");
    }

    if (advapi32_module_handle == NULL)
    {
        // Need to load library
        advapi32_module_handle = LoadLibraryW(L"advapi32.dll");
    }

    if (advapi32_module_handle == NULL)
    {
        return;
    }

    static DWORD(__stdcall *SystemFunction007)(PCUNICODE_STRING, LPBYTE) = nullptr;

#pragma warning(disable:4191)
    if (SystemFunction007 == nullptr)
    {
        SystemFunction007 = (DWORD(__stdcall *)(PCUNICODE_STRING, LPBYTE)) GetProcAddress(advapi32_module_handle, "SystemFunction007");

        if (SystemFunction007 == nullptr)
        {
            return;
        }
    }
#pragma warning(default:4191)

    if (unicode_string == nullptr)
    {
        return;
    }

    UNICODE_STRING u;

    u.Length = (USHORT) wcslen(unicode_string) * sizeof(wchar_t);
    u.MaximumLength = u.Length;
    u.Buffer = (PWSTR) unicode_string;

    NTSTATUS const return_status = SystemFunction007(&u, hash_value);
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_undocumented_get_system_process_list</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="Simple C that uses an undocumented NT function (NtQuerySystemInformation) to get a list of active processes on an NT system.">
</HEAD>

<BODY>

<H1>wfc_undocumented_get_system_process_list</H1>
$Revision: 6 $<HR>

<H2>Declaration</H2>

<PRE>DWORD <B>wfc_undocumented_get_system_process_list</B>( BYTE * buffer, DWORD size_of_buffer )</PRE>

<H2>Description</H2>

This function allows you to get a whole bunch of information about the
currently running processes. The <CODE>CreateTime</CODE>
<CODE>UserTime</CODE> and <CODE>KernelTime</CODE> are FILTIME
structures.

<P>

<B>WARNING</B> It uses an undocumented call and may no longer be supported.

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

void print_system_record( WFC_SYSTEM_PROCESS_INFORMATION * process_p )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;print_system_record()&quot; ) );

_tprintf( TEXT( &quot;Thread Count     = %lu\n&quot;,  process_p-&gt;ThreadCount );
_tprintf( TEXT( &quot;CreateTime       = %I64u\n&quot;, process_p-&gt;CreateTime );
_tprintf( TEXT( &quot;UserTime         = %I64u\n&quot;, process_p-&gt;UsetTime );
_tprintf( TEXT( &quot;KernelTime       = %I64u\n&quot;, process_p-&gt;KernelTime );
_tprintf( TEXT( &quot;Name             = %S\n&quot;, process_p-&gt;Name.Buffer );
_tprintf( TEXT( &quot;BasePriority     = %lu\n&quot;, process_p-&gt;BasePriority );
_tprintf( TEXT( &quot;UniqueProcessID  = %lu\n&quot;, process_p-&gt;UniqueProcessID );
_tprintf( TEXT( &quot;InheritedFrom    = %lu\n&quot;, process_p-&gt;InheritedFromUniqueProcessID );
_tprintf( TEXT( &quot;HandleCount      = %lu\n&quot;, process_p-&gt;HandleCount );

_tprintf( TEXT( &quot;VmCounters =\n{\n&quot );
print_vm_counters( &amp;process_p-&gt;VmCounters );
_tprintf( TEXT) &quot;}\n&quot; );

_tprintf( TEXT( &quot;CommitCharge     = %lu\n&quot;, process_p-&gt;CommitCharge );

DWORD loop_index = 0;

while( loop_index &lt; process_p-&gt;ThreadCount )
{
_tprintf( TEXT( &quot;Thread %lu\n{\n&quot;, loop_index );
print_system_thread( &amp;process_p-&gt;Threads[ loop_index ] );
_tprintf( TEXT( &quot;}\n&quot; );

loop_index++;
}
}

int _tmain( int, LPCTSTR[] )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

DWORD size_of_buffer = 128 * 1024;

BYTE * buffer = (BYTE *) malloc( size_of_buffer );

if ( <B>wfc_undocumented_get_system_process_list</B>( buffer, size_of_buffer ) == FALSE )
{
_tprintf( TEXT( &quot;Can't get process information.\n&quot; );
free( buffer );
return( EXIT_FAILURE );
}

WFC_SYSTEM_PROCESS_INFORMATION * info_p = (WFC_SYSTEM_PROCESS_INFORMATION *) buffer );

while( info_p != nullptr &amp;&amp; info_p-&gt;Next != 0 )
{
print_system_record( info_p );
info_p = GetNext( info_p );
}

free( buffer );

return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_undocumented_get_system_process_list</B>() uses the following
<B>undocumented</B> API's:
<UL>
<LI>NtQuerySystemInformation
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_undocumented_get_system_process_list.cpp $<BR>
$Modtime: 6/26/01 10:59a $
</BODY>

</HTML>
*/
