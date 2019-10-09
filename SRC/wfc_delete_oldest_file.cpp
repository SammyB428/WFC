/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2017, Samuel R. Blackburn
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
** $Workfile: wfc_delete_oldest_file.cpp $
** $Revision: 7 $
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

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_delete_oldest_file( _In_z_ const wchar_t * directory_path ) noexcept
{
    if (directory_path == nullptr || directory_path[0] == 0x00)
    {
        return(false);
    }

    std::wstring filename;

    BOOL return_value = FALSE;

    double oldest_file_time = 0.0;
    double this_file_time   = 0.0;
    double high_value       = static_cast< double >( 0xFFFFFFFF );

    high_value++;

    std::wstring mask( directory_path );

    if ( mask.at( mask.length() - 1 ) != '\\' &&
        mask.at( mask.length() - 1 ) != '/' )
    {
        mask.push_back( '/' );
    }

    mask.append(WSTRING_VIEW(L"*.*"));

    WIN32_FIND_DATAW find_data;

    ZeroMemory( &find_data, sizeof( find_data ) );

    HANDLE find_file_handle = FindFirstFileW( mask.c_str(), &find_data );

    if ( find_file_handle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        oldest_file_time  = find_data.ftLastWriteTime.dwLowDateTime;
        oldest_file_time += static_cast< double >( static_cast< double >( find_data.ftLastWriteTime.dwHighDateTime ) * high_value );

        filename.assign( find_data.cFileName );

        while( FindNextFileW( find_file_handle, &find_data ) != FALSE )
        {
            this_file_time  = find_data.ftLastWriteTime.dwLowDateTime;
            this_file_time += static_cast< double >( static_cast< double >( find_data.ftLastWriteTime.dwHighDateTime ) * high_value );

            if ( this_file_time < oldest_file_time )
            {
                oldest_file_time = this_file_time;
                filename.assign( find_data.cFileName );
            }
        }

        FindClose( find_file_handle );
    }
    else
    {
        return( false );
    }

    mask.erase(mask.length() - 3);
    mask.append( filename );

    return_value = DeleteFile( mask.c_str() );

    if ( return_value == FALSE )
    {
        return( false );
    }
    else
    {
        return( true );
    }
}

_Check_return_ uint32_t PASCAL Win32FoundationClasses::wfc_number_of_files_in_directory( _In_z_ wchar_t const * directory_path ) noexcept
{
    uint32_t number_of_files = 0;

    std::wstring filename;

    BOOL return_value = FALSE;

    std::wstring mask( directory_path );

    if ( mask.at( mask.length() - 1 ) != '\\' &&
         mask.at( mask.length() - 1 ) != '/' )
    {
        mask.push_back( '/' );
    }

    mask.append(WSTRING_VIEW(L"*.*"));

    WIN32_FIND_DATAW find_data;

    ZeroMemory( &find_data, sizeof( find_data ) );

    HANDLE find_file_handle = FindFirstFileW( mask.c_str(), &find_data );

    if ( find_file_handle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        filename.assign( find_data.cFileName );

        if ( filename.compare(WSTRING_VIEW(L".")) != I_AM_EQUAL_TO_THAT &&  filename.compare(WSTRING_VIEW(L"..")) != I_AM_EQUAL_TO_THAT)
        {
            number_of_files++;
        }

        while( FindNextFileW( find_file_handle, &find_data ) != FALSE )
        {
            filename.assign( find_data.cFileName );

            if ( filename.compare(WSTRING_VIEW(L".")) != I_AM_EQUAL_TO_THAT &&  filename.compare(WSTRING_VIEW(L"..")) != I_AM_EQUAL_TO_THAT)
            {
                number_of_files++;
            }
        }

        (void) ::FindClose( find_file_handle );
    }

    return( number_of_files );
}

_Check_return_ uint64_t PASCAL Win32FoundationClasses::wfc_number_of_bytes_in_directory( _In_z_ wchar_t const * directory_path ) noexcept
{
    uint64_t number_of_bytes = 0;

    std::wstring filename;

    BOOL return_value = FALSE;

    std::wstring mask( directory_path );

    if ( mask.at( mask.length() - 1 ) != '\\' &&
         mask.at( mask.length() - 1 ) != '/' )
    {
        mask.push_back( '/' );
    }

    mask.append(WSTRING_VIEW(L"*.*"));

    WIN32_FIND_DATAW find_data;

    ZeroMemory( &find_data, sizeof( find_data ) );

    HANDLE find_file_handle = FindFirstFileW( mask.c_str(), &find_data );

    LARGE_INTEGER large_integer;

    if ( find_file_handle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
    {
        filename.assign( find_data.cFileName );

        if ( filename.compare(WSTRING_VIEW(L".")) != I_AM_EQUAL_TO_THAT &&  filename.compare(WSTRING_VIEW(L"..")) != I_AM_EQUAL_TO_THAT)
        {
            large_integer.HighPart = find_data.nFileSizeHigh;
            large_integer.LowPart  = find_data.nFileSizeLow;

            number_of_bytes += large_integer.QuadPart;
        }

        while( FindNextFileW( find_file_handle, &find_data ) != FALSE )
        {
            filename.assign( find_data.cFileName );

            if ( filename.compare(WSTRING_VIEW(L".")) != I_AM_EQUAL_TO_THAT &&  filename.compare(WSTRING_VIEW(L"..")) != I_AM_EQUAL_TO_THAT)
            {
                large_integer.HighPart = find_data.nFileSizeHigh;
                large_integer.LowPart  = find_data.nFileSizeLow;

                number_of_bytes += large_integer.QuadPart;
            }
        }

        (void) ::FindClose( find_file_handle );
    }

    return( number_of_bytes );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_delete_oldest_file</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that deletes the oldest file in a directory.">
</HEAD>

<BODY>

<H1>wfc_delete_oldest_file</H1>
$Revision: 7 $<HR>

<H2>Declaration</H2>
<PRE><CODE>BOOL wfc_delete_oldest_file( const std::wstring&amp; directory_name )</CODE></PRE>

<H2>Description</H2>
This exciting revolutionary concept in computing . . .
OK, this is a boring function.
All it does is search a directory for the oldest file and
deletes it. I use it to clear out log file directories
when I need room. A file's age is computed using the
time the file was last written to. This function will
return TRUE if a file was deleted or FALSE if no file 
was deleted.

<H2>Example</H2>
<PRE><CODE>void make_some_room( const std::wstring&amp; log_directory )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;make_some_room()&quot; ) );

double percent_used = 0.0;

percent_used = wfc_get_directory_usage_percentage( log_directory );

while( percent_used &gt;= 85.0 )
{
<B>wfc_delete_oldest_file</B>( log_directory );
percent_used = wfc_get_directory_usage_percentage( log_directory );
}
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_delete_oldest_file</B>() uses the following API's:
<UL>
<LI>DeleteFile
<LI>FindClose
<LI>FindFirstFile
<LI>FindNextFile
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_delete_oldest_file.cpp $<BR>
$Modtime: 6/26/01 10:58a $
</BODY>

</HTML>
*/
