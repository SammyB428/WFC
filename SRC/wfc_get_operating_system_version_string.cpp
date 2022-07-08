/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2018, Samuel R. Blackburn
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
** $Workfile: wfc_get_operating_system_version_string.cpp $
** $Revision: 14 $
** $Modtime: 6/26/01 10:58a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#define new DEBUG_NEW
#endif // _DEBUG

void Win32FoundationClasses::wfc_get_operating_system_version_string( _Out_ std::wstring& version_string ) noexcept
{
    version_string.clear();

    COperatingSystemVersionInformation version_information;

    if ( version_information.Fill() == true )
    {
        std::wstring operating_system_identity;

        switch( version_information.dwPlatformId )
        {
        case VER_PLATFORM_WIN32s:

            operating_system_identity.assign(WSTRING_VIEW(L"Windows 3.1 running Win32s" ) );
            break;

        case VER_PLATFORM_WIN32_WINDOWS:

            operating_system_identity.assign(WSTRING_VIEW(L"Windows 95" ) );
            break;

        case VER_PLATFORM_WIN32_NT:

            operating_system_identity.assign(WSTRING_VIEW(L"Windows NT" ) );
            break;

        default:

            format( operating_system_identity, L"Unknown Operating System with an ID of %lu", version_information.dwPlatformId );
            break;
        }

        format( version_string, L"%s Version %lu.%lu Build %lu %s",
            operating_system_identity,
            version_information.dwMajorVersion,
            version_information.dwMinorVersion,
            version_information.dwBuildNumber,
            version_information.szCSDVersion );
    }
}

#pragma pack(1)
struct RAW_SYSTEM_MANAGEMENT_BIOS_DATA
{
    uint8_t Used20CallingMethod{ 0 };
    uint8_t	SMBIOSMajorVersion{ 0 };
    uint8_t	SMBIOSMinorVersion{ 0 };
    uint8_t	DmiRevision{ 0 };
    uint32_t Length{ 0 };
#pragma warning( push )
#pragma warning( disable: 4200 )
    uint8_t	SMBIOSTableData[];
#pragma warning (pop)
};

struct DEVICE_MANAGEMENT_INFORMATION_HEADER
{
    uint8_t  type{ 0 };
    uint8_t  length{ 0 };
    uint16_t handle{ 0 };
    uint8_t  data[1]{ 0 };
};

struct SM_SYSTEM_INFORMATION
{
    uint8_t  type_is_1{ 0 };
    uint8_t  length{ 0 };
    uint16_t handle{ 0 };
    uint8_t  Manufacturer{ 0 };
    uint8_t  Product{ 0 };
    uint8_t  Version{ 0 };
    uint8_t  SerialNumber{ 0 };
    uint8_t  uuid[16]{ 0 };
    uint8_t  WakeUpID{ 0 };
    uint8_t  SKUNumber{ 0 };
    uint8_t  Family{ 0 };
    uint8_t  data[1]{ 0 };
};

struct BASE_BOARD_INFORMATION
{
    uint8_t  type{ 0 };
    uint8_t  length{ 0 };
    uint16_t handle{ 0 };
    uint8_t  Manufacturer{ 0 };
    uint8_t  Product{ 0 };
    uint8_t  Version{ 0 };
    uint8_t  SerialNumber{ 0 };
    uint8_t  AssetTag{ 0 };
    uint8_t  Features{ 0 };
    uint8_t  LocationInChassis{ 0 };
    uint16_t ChassisHandle{ 0 };
    uint8_t  TypeOfBoard{ 0 };
    uint8_t  NumberOfHandles{ 0 };
    uint8_t  data[1]{ 0 };
};
#pragma pack()

// What we want is in the base board data
#define SYSTEM_MANAGEMENT_SYSTEM_INFORMATION (1)
#define SYSTEM_MANAGEMENT_BIOS_BASE_BOARD_INFORMATION (2)

inline static _Check_return_ DEVICE_MANAGEMENT_INFORMATION_HEADER const * __get_smbios_info( _In_ uint8_t const desired_type, _In_reads_bytes_( buffer_size ) uint8_t const * buffer, _In_ std::size_t const buffer_size ) noexcept
{
    int buffer_index{ 0 };

    auto header{ reinterpret_cast<DEVICE_MANAGEMENT_INFORMATION_HEADER const*>(buffer) };

    while( buffer_index < buffer_size ) // Cannot be converted to a Range loop
    {
        header = reinterpret_cast<DEVICE_MANAGEMENT_INFORMATION_HEADER const *>(&buffer[ buffer_index ]);

        if ( header->type == desired_type )
        {
            return( header );
        }

        buffer_index += header->length;

        while( buffer_index < buffer_size and not ( buffer[ buffer_index ] == 0 and buffer[ buffer_index + 1 ] == 0 ) )
        {
            buffer_index++;
        }

        buffer_index += 2; // Skip the zeroes
    }

    return( nullptr );
}

static inline void __read_string( _In_ uint8_t const * buffer, _In_ int const desired_string_number, _Out_ std::wstring& output ) noexcept
{
    output.clear();

    if ( desired_string_number < 1 )
    {
        return;
    }

    int current_string_number{ 1 };
    int string_index{ 0 };
    int buffer_index{ 0 };

    while( current_string_number <= desired_string_number )
    {
        if ( current_string_number == desired_string_number )
        {
            // Copy this string.

            std::string temp_string;
            temp_string.assign( reinterpret_cast<const char *>(&buffer[ buffer_index ]) );
            output.assign(std::cbegin(temp_string), std::cend(temp_string));
            return;
        }

        buffer_index += static_cast<int>(strlen( reinterpret_cast<const char *>(&buffer[ buffer_index ]) ) + 1); // + 1 to skip the zero

        current_string_number++;
    }
}

void Win32FoundationClasses::wfc_get_bios_uuid( _Out_ GUID& uuid ) noexcept
{
    // Can be verified with
    // wmic path Win32_ComputerSystemProduct get UUID

    ZeroMemory( &uuid, sizeof( uuid ) );

    uint32_t const buffer_size{ ::GetSystemFirmwareTable('RSMB', 0, nullptr, 0) };

    if ( buffer_size == 0 )
    {
        return;
    }

    auto buffer{ _aligned_malloc(buffer_size, 4096) };

    if ( buffer == nullptr )
    {
        return;
    }

    std::ignore = ::GetSystemFirmwareTable( 'RSMB', 0, buffer, buffer_size );

    auto data{ reinterpret_cast<RAW_SYSTEM_MANAGEMENT_BIOS_DATA*>(buffer) };

    auto header{ reinterpret_cast<SM_SYSTEM_INFORMATION const*>(__get_smbios_info(SYSTEM_MANAGEMENT_SYSTEM_INFORMATION, &data->SMBIOSTableData[0], data->Length)) };

    if ( header not_eq nullptr )
    {
        // Woo hoo! We found our structure, now read the string from that buffer.
        std::ignore = memcpy_s( &uuid, sizeof(uuid), &header->uuid, sizeof( uuid ) );
    }

    _aligned_free( buffer );
    buffer = nullptr;
}

void Win32FoundationClasses::wfc_get_bios_serial_number( _Out_ std::wstring& serial_number ) noexcept
{
    serial_number.clear();

    uint32_t const buffer_size{ ::GetSystemFirmwareTable('RSMB', 0, nullptr, 0) };

    if ( buffer_size == 0 )
    {
        return;
    }

    auto buffer{ _aligned_malloc(buffer_size, 4096) };

    if ( buffer == nullptr )
    {
        return;
    }

    std::ignore = ::GetSystemFirmwareTable( 'RSMB', 0, buffer, buffer_size );

    auto data{ reinterpret_cast<RAW_SYSTEM_MANAGEMENT_BIOS_DATA*>(buffer) };

    auto header{ reinterpret_cast<SM_SYSTEM_INFORMATION const*>(__get_smbios_info(SYSTEM_MANAGEMENT_SYSTEM_INFORMATION, &data->SMBIOSTableData[0], data->Length)) };

    if ( header not_eq nullptr )
    {
        // Woo hoo! We found our structure, now read the string from that buffer.
        __read_string( &header->data[ 0 ], header->SerialNumber, serial_number );
    }

    _aligned_free( buffer );
    buffer = nullptr;
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_get_operating_system_version_string</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that retrieves the operating system version string like the one that is displayed when NT is booting.">
</HEAD>

<BODY>

<H1>wfc_get_operating_system_version_string</H1>
$Revision: 14 $<HR>

<H2>Declaration</H2>
<PRE>void <B>wfc_get_operating_system_version_string</B>( std::wstring&amp; string )</PRE>

<H2>Description</H2>
This function retrieves the operating system version string like the
one that is displayed when NT is booting.

<H2>Example</H2>

<PRE><CODE>void print_version( void )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;print_version()&quot; ) );

std::wstring version_string;

<B>wfc_get_operating_system_version_string</B>( version_string );

_tprintf( TEXT( &quot;You are running %s\n&quot; ), (LPCTSTR) version_string );
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_get_operating_system_version_string</B>() uses the following API's:
<UL>
<LI>GetVersionEx
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_get_operating_system_version_string.cpp $<BR>
$Modtime: 6/26/01 10:58a $
</BODY>

</HTML>
*/
