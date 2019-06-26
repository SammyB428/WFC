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
** $Workfile: wfc_start_screen_saver.cpp $
** $Revision: 12 $
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

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_read_xml_from_file( _In_z_ const wchar_t * filename, _Inout_ CExtensibleMarkupLanguageDocument& document ) noexcept
{
    WFC_VALIDATE_POINTER( filename );

    CMemoryFile memory_mapped_file;

    if ( memory_mapped_file.Open( filename, static_cast<UINT>(CFile64::OpenFlags::modeRead) ) != false )
    {
        CDataParser parser;

        if ( parser.Initialize(static_cast<uint8_t const *>(memory_mapped_file.GetPointer()), memory_mapped_file.Size ) == false )
        {
            return( false );
        }

        return( ( document.Parse( parser ) != FALSE ) ? true : false );
    }
    else
    {
        // Can't map the file for some reason....

        CFile64 input_file;

        if ( input_file.Open( filename, static_cast<UINT>(CFile64::OpenFlags::modeRead) ) != FALSE )
        {
            return( input_file.Read( document ) );
        }
    }

    return( true );
}

void PASCAL Win32FoundationClasses::wfc_get_program_data_directory(__out std::wstring& application_data_directory) noexcept
{
    application_data_directory.clear();

    PWSTR directory_name = nullptr;

    if (SUCCEEDED(::SHGetKnownFolderPath(FOLDERID_ProgramData, 0, nullptr, &directory_name)))
    {
        application_data_directory.assign(directory_name);

        ::CoTaskMemFree(directory_name);

        directory_name = nullptr;

        if (application_data_directory.empty() != false)
        {
            // RATS! The directory is empty
            return;
        }

        if (ends_with(application_data_directory, '\\' ) == false)
        {
            application_data_directory.push_back(L'\\');
        }
    }
    else
    {
        application_data_directory.clear();
    }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_read_xml_from_file</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that reads XML from a file.">
</HEAD>

<BODY>

<H1>wfc_read_xml_from_file</H1>
$Revision: 12 $<HR>

<H2>Declaration</H2>
<PRE>bool <B>wfc_read_xml_from_file</B>( LPCTSTR filename, CExtensibleMarkupLanguageDocument&amp; xml )</PRE>

<H2>Description</H2>
This function reads the XML from the specified file.

<H2>Example</H2>

<PRE><CODE>int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

<B>wfc_read_xml_from_file</B>( argv[ 1 ] );

return( EXIT_SUCCESS );
}</CODE></PRE>

<HR><I>Copyright, 203, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_start_screen_saver.cpp $<BR>
$Modtime: 6/26/01 10:59a $
</BODY>
</HTML>
*/
