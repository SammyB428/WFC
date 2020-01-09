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
** $Workfile: CMemoryFile.cpp $
** $Revision: 17 $
** $Modtime: 6/26/01 10:47a $
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

CMemoryFile::CMemoryFile() noexcept
{
    WFC_VALIDATE_POINTER( this );

    m_FileLength    = 0;
    m_Length        = 0;
    m_MappedLength  = 0;
    m_MappedPointer = nullptr;
    m_Pointer       = nullptr;
    Size            = 0;
    m_Attributes    = FILE_ATTRIBUTE_NORMAL;
    m_TemplateFile  = static_cast< HANDLE >( NULL );
    m_MapOffsetParameter = 0;
    m_MapLengthParameter = 0;
    m_Access = 0;
    m_Protections = 0;
    m_DesiredAddress = 0;
    m_LastError = ERROR_SUCCESS;

    SYSTEM_INFO system_information;
    ::GetSystemInfo( &system_information );

    m_AllocationGranularity = system_information.dwAllocationGranularity;

    /*
    ** 1999-04-18
    ** Thanks go to Perry Rapp, (PRapp@itb.nawcsti.navy.mil) for 
    ** finding that I wasn't initializing all members here. DOH!
    */

    m_SecurityAttributes_p = static_cast<SECURITY_ATTRIBUTES *>(nullptr);
    m_SecurityDescriptor_p = static_cast<SECURITY_DESCRIPTOR *>(nullptr);
    m_MapHandle            = static_cast< HANDLE >( INVALID_HANDLE_VALUE );

    ZeroMemory( &m_FileInformation, sizeof( m_FileInformation ) );
}

CMemoryFile::CMemoryFile( __in std::size_t const allocation_granularity,
                         __in_opt SECURITY_ATTRIBUTES * security_attributes,
                         __in_opt SECURITY_DESCRIPTOR * security_descriptor ) noexcept
{
    WFC_VALIDATE_POINTER( this );

#if defined( _DEBUG )
    SYSTEM_INFO _si;
    ::GetSystemInfo( &_si );

    _ASSERTE( allocation_granularity == _si.dwAllocationGranularity );
#endif // _DEBUG

    m_FileLength    = 0;
    m_Length        = 0;
    m_MappedLength  = 0;
    m_MappedPointer = nullptr;
    m_Pointer       = nullptr;
    Size            = 0;
    m_Attributes    = FILE_ATTRIBUTE_NORMAL;
    m_TemplateFile  = static_cast< HANDLE >( NULL );
    m_MapOffsetParameter = 0;
    m_MapLengthParameter = 0;
    m_Access = 0;
    m_Protections = 0;
    m_DesiredAddress = 0;
    m_LastError = ERROR_SUCCESS;

    m_AllocationGranularity = allocation_granularity;
    m_SecurityAttributes_p  = security_attributes;
    m_SecurityDescriptor_p  = security_descriptor;
    m_MapHandle             = static_cast< HANDLE >( INVALID_HANDLE_VALUE );

    ZeroMemory( &m_FileInformation, sizeof( m_FileInformation ) );
}

CMemoryFile::~CMemoryFile() noexcept
{
    WFC_VALIDATE_POINTER( this );

    Close();

    m_Uninitialize();

    m_AllocationGranularity = 0;
    m_FileLength            = 0;
    m_Length                = 0;
    m_MapHandle             = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
    m_MappedLength          = 0;
    m_MappedPointer         = nullptr;
    m_Pointer               = nullptr;
    m_SecurityAttributes_p  = nullptr;
    m_SecurityDescriptor_p  = nullptr;
    Size                    = 0;
    m_MapOffsetParameter = 0;
    m_MapLengthParameter = 0;
    m_Access = 0;
    m_Protections = 0;
    m_DesiredAddress = 0;
    m_LastError = ERROR_SUCCESS;

    ZeroMemory( &m_FileInformation, sizeof( m_FileInformation ) );
}

void CMemoryFile::Close( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_MappedPointer != nullptr )
    {
        if ( ::UnmapViewOfFile( m_MappedPointer ) == FALSE )
        {
            //WFCTRACEERROR( ::GetLastError() );
            //WFCTRACE( TEXT( "UnmapViewOfFile() failed with the above error." ) );
        }

        m_MappedPointer = nullptr;
        m_Pointer       = nullptr;
    }

    if ( m_MapHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        if ( ::CloseHandle( m_MapHandle ) == FALSE )
        {
            //WFCTRACEERROR( ::GetLastError() );
            //WFCTRACE( TEXT( "CloseHandle() failed with the above error." ) );
        }

        m_MapHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
    }

    m_Filename.clear();
    m_FileLength         = 0;
    m_Length             = 0;
    m_MappedLength       = 0;
    Size                 = 0;
    m_MapOffsetParameter = 0;
    m_MapLengthParameter = 0;
    m_Access             = 0;
    m_Protections        = 0;
    m_DesiredAddress     = nullptr;

    ZeroMemory( &m_FileInformation, sizeof( m_FileInformation ) );
}

_Check_return_ bool CMemoryFile::Flush( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( ::FlushViewOfFile( m_MappedPointer, m_MappedLength ) == 0 )
    {
        //WFCTRACEERROR( ::GetLastError() );
        //WFCTRACE( TEXT( "FlushViewOfFile() failed for the above reason." ) );
        return( false );
    }

    return( true );
}

_Check_return_ bool CMemoryFile::FromHandle( _In_ HANDLE const file_handle, _In_ UINT const open_flags, _In_ uint64_t const beginning_at_offset, _In_ std::size_t const number_of_bytes_to_map, _In_opt_ void const * desired_address ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    bool const return_value = m_MapTheFile( file_handle, open_flags, beginning_at_offset, number_of_bytes_to_map, desired_address );

    return( return_value );
}

void CMemoryFile::GetFilename(_Out_ std::wstring& filename) const noexcept
{
    WFC_VALIDATE_POINTER(this);

    filename.assign(m_Filename);
}

_Check_return_ bool CMemoryFile::GetInformation( _Inout_ BY_HANDLE_FILE_INFORMATION& information ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    (void) memcpy_s( &information, sizeof(information), &m_FileInformation, sizeof(m_FileInformation) );

    return( true );
}

void CMemoryFile::m_Initialize( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    ASSERT( m_SecurityAttributes_p == nullptr );
    ASSERT( m_SecurityDescriptor_p == nullptr );
    ASSERT( m_MapHandle            == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );

    m_Attributes    = FILE_ATTRIBUTE_NORMAL;
    m_TemplateFile  = static_cast<HANDLE>( NULL );

    WFC_TRY
    {
        m_SecurityAttributes_p = new SECURITY_ATTRIBUTES;
    }
    WFC_CATCH_ALL
    {
        m_SecurityAttributes_p = nullptr;
    }
    WFC_END_CATCH_ALL

        if ( m_SecurityAttributes_p == nullptr )
        {
            return;
        }

        m_SecurityDescriptor_p = wfc_create_null_dacl();

        if ( m_SecurityDescriptor_p == nullptr )
        {
            delete m_SecurityAttributes_p;
            m_SecurityAttributes_p = nullptr;

            return;
        }

        m_SecurityAttributes_p->nLength              = sizeof( SECURITY_ATTRIBUTES );
        m_SecurityAttributes_p->lpSecurityDescriptor = m_SecurityDescriptor_p;
        m_SecurityAttributes_p->bInheritHandle       = TRUE;
}

_Check_return_ void * CMemoryFile::Map( _In_ uint64_t const offset, _In_ std::size_t const length ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_MapOffsetParameter == offset and m_MapLengthParameter == length )
    {
        // The caller has asked for this before, nothing to do
        return( m_Pointer );
    }

    if ( m_MappedPointer != nullptr )
    {
        if ( ::UnmapViewOfFile( m_MappedPointer ) == FALSE )
        {
            //WFCTRACEERROR( ::GetLastError() );
            //WFCTRACE( TEXT( "UnmapViewOfFile() failed with the above error." ) );
        }

        m_MappedPointer = nullptr;
        m_Pointer       = nullptr;
    }

    m_MapOffsetParameter = offset;
    m_MapLengthParameter = length;

    ULARGE_INTEGER large_file_offset = { 0, 0 };

    large_file_offset.QuadPart = offset;

    if ( ( large_file_offset.QuadPart % m_AllocationGranularity ) != 0 )
    {
        // The user wants to start mapping at a point in the file that doesn't fall
        // on a memory allocation boundary (usually a 64KB boundary). That means we
        // must do some creative programming to give the user what they asked for.

        large_file_offset.QuadPart -= ( large_file_offset.QuadPart % m_AllocationGranularity );
    }

    m_MappedLength = length + static_cast< std::size_t >( offset - large_file_offset.QuadPart );

    _ASSERTE( m_MappedPointer == nullptr );

    m_MappedPointer = ::MapViewOfFileEx( m_MapHandle,
        m_Access,
        large_file_offset.HighPart,
        large_file_offset.LowPart,
        m_MappedLength,
        (LPVOID) m_DesiredAddress );

    if ( m_MappedPointer == nullptr )
    {
        //WFCTRACEERROR( ::GetLastError() );
        //WFCTRACE( TEXT( "MapViewOfFileEx() failed with the above error." ) );

        if ( ::CloseHandle( m_MapHandle ) == FALSE )
        {
            //WFCTRACEERROR( ::GetLastError() );
            //WFCTRACE( TEXT( "CloseHandle() failed with the aboce error." ) );
        }

        m_MapHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );

        Close();

        return( nullptr );
    }

    m_Filename.clear();
    m_Length   = m_MapLengthParameter;
    Size       = m_MapLengthParameter;

    char * temp_pointer = (char *) m_MappedPointer;

    m_Pointer = &temp_pointer[ m_MappedLength - m_MapLengthParameter ];

    return( m_Pointer );
}

_Check_return_ bool CMemoryFile::m_MapTheFile( __in HANDLE const file_handle, __in UINT const open_flags, __in uint64_t const beginning_at_offset, __in std::size_t const number_of_bytes_to_map_parameter, __in_opt void const * desired_address ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_Pointer != nullptr )
    {
        Close();
    }

    if ( is_bad_handle( file_handle ) == true )
    {
#if defined( _DEBUG )
        if ( file_handle == static_cast< HANDLE >( NULL ) )
        {
            //WFCTRACE( TEXT( "file_handle is NULL!" ) );
        }
        else if ( file_handle == INVALID_HANDLE_VALUE )
        {
            //WFCTRACE( TEXT( "file_handle is INVALID_HANDLE_VALUE!" ) );
        }
#endif

        m_LastError = ERROR_INVALID_PARAMETER;
        return( false );
    }

    m_DesiredAddress = (void *) desired_address;

    ZeroMemory( &m_FileInformation, sizeof( m_FileInformation ) );

    if ( ::GetFileInformationByHandle( file_handle, &m_FileInformation ) == 0 )
    {
        m_LastError = GetLastError();
        //WFCTRACEERROR( m_LastError );
        return( false );
    }

    // Start with the least amount of permissions needed
    m_Protections = PAGE_READONLY;
    m_Access = FILE_MAP_READ;

    UINT const just_flags = open_flags bitand 0x0F;

    if ( just_flags == (UINT) CFile64::OpenFlags::modeRead )
    {
        //WFCTRACE( TEXT( "User wants read-only access." ) );
        m_Protections = PAGE_READONLY;
        m_Access = FILE_MAP_READ;
    }

    if ( ( just_flags bitand (UINT)CFile64::OpenFlags::modeWrite ) or ( just_flags bitand (UINT) CFile64::OpenFlags::modeReadWrite ) )
    {
        //WFCTRACE( TEXT( "User wants Read/Write access." ) );
        m_Protections = PAGE_READWRITE;
        m_Access = FILE_MAP_WRITE;
    }

    // Let's adjust number_of_bytes_to_map if we're using an offset

    LARGE_INTEGER file_length = { 0, 0 };

    if ( ::GetFileSizeEx( file_handle, &file_length ) == FALSE )
    {
        m_LastError = GetLastError();
        //WFCTRACEERROR( m_LastError );
        //WFCTRACE( TEXT( "Can't get file size because of above error." ) );
        return( false );
    }

    m_FileLength = file_length.QuadPart;

    std::size_t number_of_bytes_to_map = number_of_bytes_to_map_parameter;

    if ( beginning_at_offset > 0 )
    {
        if ( beginning_at_offset > m_FileLength )
        {
            //WFCTRACE( TEXT( "You want to start mapping a file after the end of the file." ) );
            m_LastError = ERROR_INVALID_PARAMETER;
            return( false );
        }

        if ( number_of_bytes_to_map == 0 )
        {
            // user has specified default length, since the user also
            // specified an offset, we need to compute how many bytes
            // are really gonna be mapped.

            number_of_bytes_to_map = static_cast< std::size_t >( m_FileLength - beginning_at_offset );
        }
    }
    else
    {
        if ( number_of_bytes_to_map == 0 )
        {
            _ASSERTE( m_FileLength > 0 );
            number_of_bytes_to_map = static_cast< std::size_t >( m_FileLength );
        }
    }

    m_MapHandle = ::CreateFileMapping( file_handle,
        m_SecurityAttributes_p,
        m_Protections,
        0, // Microsoft isn't very clear about these parameters, always use zero
        0, // Microsoft isn't very clear about these parameters, always use zero
        nullptr );

    if ( m_MapHandle == static_cast< HANDLE >( NULL ) )
    {
        m_LastError = ::GetLastError();
        //WFCTRACEERROR( m_LastError );
        //WFCTRACE( TEXT( "CreateFileMapping() failed with the above error." ) );

        m_MapHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
        return( false );
    }

    (void) Map( beginning_at_offset, number_of_bytes_to_map );

    return( true );
}

void CMemoryFile::m_Uninitialize( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_SecurityAttributes_p != nullptr )
    {
        delete m_SecurityAttributes_p;
        m_SecurityAttributes_p = nullptr;
    }

    if ( m_SecurityDescriptor_p != nullptr )
    {
        wfc_destroy_null_dacl( m_SecurityDescriptor_p );
        m_SecurityDescriptor_p = nullptr;
    }
}

_Check_return_ bool CMemoryFile::Open(_In_ std::wstring_view filename, __in UINT const open_flags, __in uint64_t const beginning_at_offset, __in std::size_t const number_of_bytes_to_map, __in_opt void const * desired_address ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_Pointer != nullptr )
    {
        Close();
    }

    if ( filename.empty() == true )
    {
        return( false );
    }

    CFile64 file;

    file.SetAttributes( GetAttributes() );
    file.SetTemplateFile( GetTemplateFile() );

    // The user passed us a pointer, don't trust it

    WFC_TRY
    {
        if ( file.Open( filename, open_flags ) == false )
        {
            m_LastError = file.LastError();
            //WFCTRACE( TEXT( "Can't open file" ) );
            return( false );
        }

        if ( m_MapTheFile( file.GetHandle(), open_flags, beginning_at_offset, number_of_bytes_to_map, desired_address ) == false )
        {
            //WFCTRACE( TEXT( "Can't map the file." ) );
            return( false );
        }

        m_Filename.assign( filename );

        return( true );
    }
    WFC_CATCH_ALL
    {
    }
    WFC_END_CATCH_ALL

        return( false );
}

_Check_return_ bool CMemoryFile::Open( _In_ std::string_view filename, _In_ UINT const open_flags, _In_ uint64_t const beginning_at_offset, _In_ std::size_t const number_of_bytes_to_map, _In_opt_ void const * desired_address ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_Pointer != nullptr )
    {
        Close();
    }

    if ( filename.empty() == true )
    {
        return( false );
    }

    CFile64 file;

    file.SetAttributes( GetAttributes() );
    file.SetTemplateFile( GetTemplateFile() );

    // The user passed us a pointer, don't trust it

    WFC_TRY
    {
        std::wstring wide_filename;

        copy(wide_filename, filename);

        if ( file.Open(wide_filename, open_flags ) == false )
        {
            //WFCTRACE( TEXT( "Can't open file" ) );
            return( false );
        }

        if ( m_MapTheFile( file.GetHandle(), open_flags, beginning_at_offset, number_of_bytes_to_map, desired_address ) == false )
        {
            //WFCTRACE( TEXT( "Can't map the file." ) );
            return( false );
        }

        m_Filename.assign(wide_filename);

        return( true );
    }
    WFC_CATCH_ALL
    {
    }
    WFC_END_CATCH_ALL

    return( false );
}

CSharedMemory::CSharedMemory() noexcept
{
    WFC_VALIDATE_POINTER( this );

    m_Buffer = nullptr;
    m_Size = 0;
    m_Handle = INVALID_HANDLE_VALUE;
    m_LastError = 0;
}

CSharedMemory::~CSharedMemory() noexcept
{
    WFC_VALIDATE_POINTER( this );

    Close();
}

void CSharedMemory::Close(void) noexcept
{
    WFC_VALIDATE_POINTER( this );

    Name.clear();

    if ( m_Buffer != nullptr )
    {
        ::UnmapViewOfFile( m_Buffer );
        m_Buffer = nullptr;
    }

    if ( is_bad_handle( m_Handle ) == false )
    {
        (void) wfc_close_handle( m_Handle );
        m_Handle = INVALID_HANDLE_VALUE;
    }

    m_Size = 0;
}

static _Check_return_ bool CreateDACL(_Inout_ SECURITY_ATTRIBUTES *sa) noexcept
{
    wchar_t const *sdd = L"D:"
        L"(D;OICI;GA;;;BG)" //Deny guests
        L"(D;OICI;GA;;;AN)" //Deny anonymous
        L"(A;OICI;GRGWGX;;;AU)" //Allow read, write and execute for Users
        L"(A;OICI;GA;;;BA)"; //Allow all for Administrators

    return ConvertStringSecurityDescriptorToSecurityDescriptor(sdd, SDDL_REVISION_1, &sa->lpSecurityDescriptor, nullptr) == TRUE;
}

static _Check_return_ bool CreateLowIntegritySACL( _Inout_ SECURITY_ATTRIBUTES *sa ) noexcept
{
    wchar_t const * sdd = L"S:(ML;;NW;;;LW)";

    return ConvertStringSecurityDescriptorToSecurityDescriptor(sdd, SDDL_REVISION_1, &sa->lpSecurityDescriptor, nullptr) == TRUE;
}

_Check_return_ bool CSharedMemory::Create(_In_ std::wstring_view name, _In_ std::size_t const number_of_bytes ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    _ASSERTE( name.empty() == false );

    SECURITY_ATTRIBUTES security_attributes;

    security_attributes.nLength              = sizeof( security_attributes );
    security_attributes.lpSecurityDescriptor = static_cast<void *>(wfc_create_null_dacl());
    security_attributes.lpSecurityDescriptor = nullptr;
    security_attributes.bInheritHandle       = TRUE;

    bool security_access_created = false;

    //security_access_created = CreateDACL( &security_attributes );
    //security_access_created = CreateLowIntegritySACL( &security_attributes );

    bool const return_value = Create( name, number_of_bytes, &security_attributes );

    wfc_destroy_null_dacl( security_attributes.lpSecurityDescriptor );

    return( return_value );
}

_Check_return_ bool CSharedMemory::Create(_In_ std::wstring_view name, _In_ std::size_t const number_of_bytes, _In_ SECURITY_ATTRIBUTES * security_attributes ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    _ASSERTE( name.empty() == false );

    Close();

    Name.assign( name );

    ULARGE_INTEGER buffer_size;

    buffer_size.QuadPart = number_of_bytes;

    m_Handle = ::CreateFileMappingW( INVALID_HANDLE_VALUE,
                                    security_attributes,
                                    PAGE_READWRITE, // | SEC_COMMIT,
                                    buffer_size.HighPart,
                                    buffer_size.LowPart,
                                    Name.c_str() );

    m_LastError = ::GetLastError();

    if ( m_Handle == static_cast< HANDLE >( NULL ) )
    {
        //WFCTRACEERROR( m_LastError );
        //WFCTRACEVAL( TEXT( "Can't map to " ), Name );
        Name.clear();
        return( false );
    }

    m_Buffer = static_cast<uint8_t *>(::MapViewOfFile( m_Handle, FILE_MAP_ALL_ACCESS, 0, 0, number_of_bytes ));

    if ( m_Buffer == nullptr )
    {
        m_LastError = ::GetLastError();
        //WFCTRACEERROR( m_LastError );
        //WFCTRACEVAL( TEXT( "Can't MapViewOfFile to " ), Name );
        (void) wfc_close_handle( m_Handle );
        m_Handle = INVALID_HANDLE_VALUE;
        Name.clear();
        return( false );
    }

    m_Size = number_of_bytes;

    return( true );
}

_Check_return_ bool CSharedMemory::Open(_In_ std::wstring_view name, _In_ std::size_t const number_of_bytes, _In_ bool const read_only ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    _ASSERTE( name.empty() == false );

    Close();

    Name.assign( name );

    ULARGE_INTEGER buffer_size;

    buffer_size.QuadPart = number_of_bytes;

    m_Handle = ::OpenFileMappingW( ( read_only == false ) ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ, FALSE, Name.c_str() );

    if ( m_Handle == static_cast< HANDLE >( NULL ) )
    {
        m_LastError = ::GetLastError();
        //WFCTRACEERROR( m_LastError );
        //WFCTRACEVAL( TEXT( "Can't map to " ), Name );
        Name.clear();
        return( false );
    }

    m_Buffer = static_cast<uint8_t *>(::MapViewOfFile( m_Handle, ( read_only == false ) ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ, 0, 0, number_of_bytes ));

    if ( m_Buffer == nullptr )
    {
        m_LastError = ::GetLastError();
        //WFCTRACEERROR( m_LastError );
        //WFCTRACEVAL( TEXT( "Can't MapViewOfFile to " ), Name );
        (void) wfc_close_handle( m_Handle );
        m_Handle = INVALID_HANDLE_VALUE;
        Name.clear();
        return( false );
    }

    m_Size = number_of_bytes;

    return( true );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CMemoryFile</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, memory mapped files, mmap, source code">
<META name="description" content="A simple C++ class that handles memory mapped files.">
</HEAD>

<BODY>

<H1>CMemoryFile</H1>

$Revision: 17 $
<HR>

<H2>Description</H2>

This class makes memory mapped files easy to use.
Memory mapped files are great when you have to access the
contents of a file directly. Instead of declaring a big
buffer (at least the size of the file) then reading the
file into that buffer, mapping the file contents to a pointer
is much faster.

<BR>

Mapping a file to a pointer lets you dereference the pointer
as needed and let the operating system handle the retrieval
of the bytes from the file. Very cool.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CMemoryFile</B>()
<B>CMemoryFile</B>( DWORD                 allocation_granularity,
SECURITY_ATTRIBUTES * security_attributes = nullptr,
SECURITY_DESCRIPTOR * security_descriptor = nullptr )</PRE><DD>
Constructs the object. By specifying <CODE>allocation_granularity</CODE> you can
avoid the overhead of calling <CODE>GetSystemInfo()</CODE> which the default
constructor does.
This is useful when you have to map thousands of files. Since the allocation
granularity never changes, when keep calling <CODE>GetSystemInfo()</CODE>?

</DL>

<H2>Data Members</H2>

<DL COMPACT>

<DT><PRE>DWORD <B><A NAME="Size">Size</A></B></PRE><DD>
Holds the same value as <B>GetLength</B>(). This member
is exposed for speed only. It is not used internally by
the class.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the file.

<DT><PRE>BOOL <B><A NAME="Flush">Flush</A></B>( void )</PRE><DD>
Synchronizes the contents of memory with the contents
of the physical disk file.

<DT><PRE>BOOL <B><A NAME="FromHandle">FromHandle</A></B>( HANDLE file_handle,
UINT open_flags = CFile64::modeRead,
ULONGLONG begining_at = 0,
DWORD number_of_bytes_to_map,
void * desired_address = nullptr )</PRE><DD>
Pretty much the same as <A HREF="#Open">Open()</A> except you pass
in the handle to the file to use in mapping. This makes it much
faster when you have to remap different sections of the same file.
If you pass the handle to the file, you can skip the overhead that
<A HREF="#Open">Open()</A> must incur by going to the filesystem to
actually open the file you're going to map. <B>FromHandle()</B> doesn't
have that problem.

<DT><PRE>DWORD <B><A NAME="GetAttributes">GetAttributes</A></B>( void ) const</PRE><DD>
Retrieves the attributes used in the wrapped CreateFile() API. This value is used in
the sixth parameter to CreateFile(). This method is called by the
<A HREF="#Open">Open</A> method. The default value is FILE_ATTRIBUTE_NORMAL.

<DT><PRE>ULONGLONG <B><A NAME="GetFileLength">GetFileLength</A></B>( void ) const</PRE><DD>
Returns the length of the <B>Open()</B>'d file.
Yes, this returns a 64-bit length.

<DT><PRE>void <B><A NAME="GetFilename">GetFilename</A></B>( std::wstring&amp; filename ) const</PRE><DD>
Retrieves the name of the file that is mapped. If <CODE>filename</CODE>
is empty, no file has been opened.

<DT><PRE>DWORD <B><A NAME="GetLength">GetLength</A></B>( void ) const</PRE><DD>
Returns the number of bytes that were mapped.

<DT><PRE>void * <B><A NAME="GetPointer">GetPointer</A></B>( void ) const</PRE><DD>
Returns the pointer to the mapped bytes. If this method
returns NULL, it means there is no file mapped.

<DT><PRE>HANDLE <B><A NAME="GetTemplateFile">GetTemplateFile</A></B>( void ) const</PRE><DD>
Retrieves the handle to the file that will be used as a template in the
<A HREF="#Open">Open</A> method. This handle is used as the seventh parameter
to the wrapped CreateFile() API.

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( LPCTSTR filename,
UINT open_flags = CFile64::modeRead,
ULONGLONG begining_at = 0,
DWORD number_of_bytes_to_map,
void * desired_address = nullptr )</PRE><DD>
This method is patterend after the <B>Open</B>() method in the
<A HREF="http://www.stingsoft.com/mfc_faq/">MFC</A>
CFile64 class. In it's simplest form, you pass a name of a file
and it will open that file in read only mode. This is great
for parsing data files.

You don't have to worry about the <CODE>beginning_at</CODE> value
be a special value (like in the call to the <CODE>MapViewOfFile()</CODE>
API).

The other parameters have these meanings:

<UL>

<LI><CODE><B>open_flags</B></CODE> - May be one of the following values:
<UL>
<LI><CODE><B>CFile64::modeRead</B></CODE> - Open the file for read only
<LI><CODE><B>CFile64::modeWrite</B></CODE> - Open the file for read/write
<LI><CODE><B>CFile64::modeReadWrite</B></CODE> - Open the file for read/write
</UL>

<LI><CODE><B>beginning_at</B></CODE> - The offset into the file of
where to start mapping.

<LI><CODE><B>number_of_bytes_to_map</B></CODE> - The number of bytes
to map. If this parameter is zero, the default is to compute
the number of bytes that were mapped. You can call <B>GetLength</B>()
to find out how many bytes were mapped.

<LI><CODE><B>desired_address</B></CODE> - This is where you ask
nicely that the address returned by <B>GetPointer</B>() be this
value. Win32 may or may not be able to map the bytes in the file
to <CODE>desired_address</CODE>. Never count on <CODE>desired_address</CODE>
actually being used.

</UL>

<DT><PRE>void <B><A NAME="SetAttributes">SetAttributes</A></B>( DWORD attributes )</PRE><DD>
Sets the attributes used in the wrapped CreateFile() API. This value is used in
the sixth parameter to CreateFile().

<DT><PRE>void <B><A NAME="SetTemplateFile">SetTemplateFile</A></B>( HANDLE template_file_handle )</PRE><DD>
Sets the handle to the file that will be used as a template in the
<A HREF="#Open">Open</A> method. This handle is used as the seventh parameter
to the wrapped CreateFile() API.

</DL>

<H2>Example</H2><PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

void test_CMemoryFile( void )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;test_CMemoryFile()&quot; ) );

<B>CMemoryFile</B> memory_mapped_file;

// Set things up so the file that is mapped is automatically deleted

memory_mapped_file.SetAttributes( FILE_FLAG_DELETE_ON_CLOSE );

BYTE * pointer = nullptr;

if ( memory_mapped_file.Open( TEXT( &quot;datafile.dat&quot; ) ) == false )
{
return;
}

pointer = (BYTE *) memory_mapped_file.GetPointer();

_tprintf( TEXT( &quot;The last byte of the file is %X\n&quot; ),
pointer[ memory_mapped_file.GetLength() - 1 ] );
}</CODE></PRE>

<H2>API&#39;s Used</H2>

<UL>
<LI>CloseHandle
<LI>CreateFileMapping
<LI>FlushViewOfFile
<LI>GetFileSize
<LI>GetLastError
<LI>GetSystemInfo
<LI>InitializeSecurityDescriptor
<LI>MapViewOfFileEx
<LI>SetSecurityDescriptorDacl
<LI>UnmapViewOfFile
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CMemoryFile.cpp $<BR>
$Modtime: 6/26/01 10:47a $

</BODY>

</HTML>
*/
