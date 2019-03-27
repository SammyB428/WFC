/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2014, Samuel R. Blackburn
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
** $Workfile: wfc_get_web_page.cpp $
** $Revision: 25 $
** $Modtime: 9/06/01 2:49p $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if ! defined( WFC_STL )

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

    // MFC Version

    void PASCAL Win32FoundationClasses::wfc_get_web_page( __in const CUniformResourceLocator& url, __out std::vector<std::wstring>& page_contents, __in const DWORD options )
{
    CInternetSession * internet_session_p = nullptr;

    CHttpFile * source_file_p = nullptr;

    try
    {
        page_contents.RemoveAll();

        std::wstring temp_string;

        temp_string.Format( TEXT( "%08lX:%08lX wfc_get_web_page( \"%s\" )" ),
            (unsigned long) ::GetCurrentThreadId(),
            ( unsigned long) ::GetTickCount(),
            (LPCTSTR) url );

        WFCTRACEVAL( TEXT( "Unique ID is " ), temp_string );

        internet_session_p = nullptr;

        WFC_TRY
        {
            WFC_TRY
            {
                internet_session_p = new CInternetSession( temp_string );
            }
            WFC_CATCH_ALL
            {
                if ( internet_session_p == nullptr )
                {
                    return;
                }
            }
            WFC_END_CATCH_ALL

                // We use the INTERNET_FLAG_RELOAD because this function is probably NOT
                // going to be the basis for a browser. It will usually be used to retrieve
                // things like stock quotes that change frequently. In this case, cacheing
                // screws you because you don't get the latest data from the Web.

                source_file_p = (CHttpFile *) internet_session_p->OpenURL( url, 1, options );
        }
#if defined( WFC_USE_MFC_EXCEPTIONS )
        WFC_CATCH( CInternetException *, exception_p )
        {
#if defined( _DEBUG )

            TCHAR error_message[ 513 ];

            ZeroMemory( error_message, sizeof( error_message ) );

            exception_p->GetErrorMessage( error_message, DIMENSION_OF( error_message ) );

            WFCTRACEVAL( TEXT( "Error is " ), error_message );

#endif // _DEBUG

            exception_p->Delete();
        }
#endif // WFC_USE_MFC_EXCEPTIONS
        WFC_CATCH_ALL
        {
        }
        WFC_END_CATCH_ALL

            if ( source_file_p == nullptr )
            {
                return;
            }

            source_file_p->SetReadBufferSize( 65535 * 4 );

            int index = 0;

            while( source_file_p->ReadString( temp_string ) != FALSE )
            {
                index++;
                page_contents.Add( temp_string );
            }

            source_file_p->Close();
            delete source_file_p;
            internet_session_p->Close();
            delete internet_session_p;
    }
    WFC_CATCH_ALL
    {
        WFC_TRY
        {
            source_file_p->Close();
        }
        WFC_CATCH_ALL
        {
            ;
        }
        WFC_END_CATCH_ALL

            WFC_TRY
        {
            delete source_file_p;
        }
        WFC_CATCH_ALL
        {
            ;
        }
        WFC_END_CATCH_ALL

            WFC_TRY
        {
            internet_session_p->Close();
        }
        WFC_CATCH_ALL
        {
            ;
        }
        WFC_END_CATCH_ALL

            WFC_TRY
        {
            delete internet_session_p;
        }
        WFC_CATCH_ALL
        {
            ;
        }
        WFC_END_CATCH_ALL

            return;
    }
    WFC_END_CATCH_ALL
}

void PASCAL Win32FoundationClasses::wfc_get_web_page( __in const CUniformResourceLocator& url, __out std::vector<uint8_t>& page_contents, __in const DWORD options )
{
    page_contents.clear();

    std::vector<std::wstring> web_page;

    Win32FoundationClasses::wfc_get_web_page( url, web_page, options );

    int loop_index = 0;

    while( loop_index < web_page.GetSize() )
    {
        Win32FoundationClasses::wfc_append_string_to_byte_array( web_page.at( loop_index ), page_contents );
        loop_index++;
    }
}

#else

// STL Version

USING_WFC_NAMESPACE

void PASCAL Win32FoundationClasses::wfc_get_web_page( __in const CUniformResourceLocator& url, __out std::vector<uint8_t>& page_contents, __in const DWORD options ) noexcept
{
    page_contents.clear();

    std::wstring temp_string;

    format( temp_string, L"%08lX:%08lX wfc_get_web_page( \"%s\" )",
        (unsigned long) ::GetCurrentThreadId(),
        (unsigned long) ::GetTickCount64(),
        (LPCTSTR) url );

    //WFCTRACEVAL( TEXT( "Unique ID is " ), temp_string );

    auto internet_handle = InternetOpen( TEXT( "Mozilla/4.0 (compatible; Win32 Foundation Classes; http://www.samblackburn.com)" ),
        INTERNET_OPEN_TYPE_PRECONFIG,
        nullptr,
        nullptr,
        0 );

    if ( internet_handle == static_cast< HINTERNET >( NULL ) )
    {
        //WFCTRACEERROR( GetLastError() );
        //WFCTRACE( TEXT( "InternetOpen() failed." ) );
        return;
    }

    auto url_handle = InternetOpenUrl( internet_handle,
        url,
        nullptr,
        0,
        options,
        1 );

    if ( url_handle == static_cast< HINTERNET >( NULL ) )
    {
        //WFCTRACEERROR( GetLastError() );
        //WFCTRACE( TEXT( "InternetOpenUrl() failed." ) );

        InternetCloseHandle( internet_handle );
        internet_handle = static_cast< HINTERNET >( NULL );

        return;
    }

    std::vector<uint8_t> bytes_read_in_this_chunk;

    bytes_read_in_this_chunk.resize( 256 * 1024 );

    DWORD number_of_bytes_read = 0;

    BOOL return_value = InternetReadFile( url_handle,
        bytes_read_in_this_chunk.data(),
        (DWORD) bytes_read_in_this_chunk.size(),
        &number_of_bytes_read );

    while( return_value != FALSE && number_of_bytes_read > 0 )
    {
        bytes_read_in_this_chunk.resize( number_of_bytes_read );
        page_contents.insert( std::end(page_contents), std::cbegin(bytes_read_in_this_chunk), std::cend(bytes_read_in_this_chunk));

        bytes_read_in_this_chunk.resize( 256 * 1024 );
        return_value = InternetReadFile( url_handle,
            bytes_read_in_this_chunk.data(),
            (DWORD) bytes_read_in_this_chunk.size(),
            &number_of_bytes_read );
    }

    InternetCloseHandle( internet_handle );
}

void PASCAL Win32FoundationClasses::wfc_get_web_page( __in const CUniformResourceLocator& url, __out std::vector<std::wstring>& page_contents, __in const DWORD options ) noexcept
{
    page_contents.clear();

    std::vector<uint8_t> bytes;

    Win32FoundationClasses::wfc_get_web_page( url, bytes, options );

    (void) bytes.push_back( 0 ); // zero Terminate the string

    std::wstring web_page;
    
    copy( web_page, reinterpret_cast<const char *>(bytes.data()));

    bytes.clear();

    std::size_t location_of_end_of_line_character = 0;
    std::size_t start_searching_at_index          = 0;

    location_of_end_of_line_character = web_page.find( '\n' );

    while( location_of_end_of_line_character != std::wstring::npos )
    {
        (void) page_contents.push_back( web_page.substr( start_searching_at_index, location_of_end_of_line_character - start_searching_at_index ) );
        start_searching_at_index += ( ( location_of_end_of_line_character - start_searching_at_index ) + 1 );
        location_of_end_of_line_character = web_page.find( '\n', start_searching_at_index );
    }

    (void) page_contents.push_back( web_page.substr( start_searching_at_index, web_page.length() - start_searching_at_index ) );
}

#endif // WFC_STL

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_get_web_page</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that retrieves a web page and stores in an array of strings.">
</HEAD>

<BODY>

<H1>wfc_get_web_page</H1>
$Revision: 25 $<HR>

<H2>Declaration</H2>
<PRE><CODE>void <B>wfc_get_web_page</B>( CUniformResourceLocator&amp; url, std::vector&lt;std::wstring&gt;&amp; page_contents, DWORD options )
void <B>wfc_get_web_page</B>( CUniformResourceLocator&amp; url, std::vector&lt;uint8_t&gt;&amp; page_contents, DWORD options )</CODE></PRE>

<H2>Description</H2>
This function takes a url and retrieves it from Internet. What you get is an
array containing the source for that page. This function is meant
to provide the capability to pull data from Internet and chew on it inside
a program (as opposed to display it to a user).

<P>

The default <CODE>options</CODE> are:

<TABLE BORDER="0" CELLPADDING="5">
<TR>
<TD><CODE>INTERNET_FLAG_RELOAD</CODE></TD>
<TD>Microsoft (Internet Explorer) tries to &quot;help&quot; you by caching web pages
that you have visited. This flag tell Microsoft to stop helping and just
go to the website and retrieve the page.</TD>
</TR>
<TR>
<TD><CODE>INTERNET_FLAG_TRANSFER_ASCII</CODE></TD>
<TD>Can't remember why I have this flag or if it is even needed.</TD>
</TR>
<TR>
<TD><CODE>INTERNET_FLAG_NO_CACHE_WRITE</CODE></TD>
<TD>Tells Microsoft (Internet Explorer) to stop &quot;helping&quot; me
by writing the retrieved web page to a cache. This flags prevents the web
page from being written to the browser cache.</TD>
</TR>
<TR>
<TD><CODE>INTERNET_FLAG_IGNORE_CERT_CN_INVALID</CODE></TD>
<TD>Ignores invalid certificates when retrieving a secure web page.</TD>
</TR>
<TR>
<TD><CODE>INTERNET_FLAG_IGNORE_CERT_DATE_INVALID</CODE></TD>
<TD>Ignores invalid certificates when retrieving a secure web page.</TD>
</TR>
</TABLE>

<H2>Example</H2>

<PRE><CODE>void print_maryland_weather_report( void )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;print_maryland_weather_report()&quot; ) );

CUniformResourceLocator url( TEXT( &quot;http://iwin.nws.noaa.gov/iwin/md/hourly.html&quot; ) );

std::vector&lt;std::wstring&gt; weather_report;

<B>wfc_get_web_page</B>( url, weather_report );

int index = 0;
int number_of_lines_in_report = weather_report.GetSize();

while( index &lt; number_of_lines_in_report )
{
_tprintf( TEXT( &quot;%s\n&quot; ), (LPCTSTR) weather_report.GetAt( index ) );
index++;
}
}</CODE></PRE>

<HR><I>Copyright, 1995-2002, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_get_web_page.cpp $<BR>
$Modtime: 9/06/01 2:49p $
</BODY>

</HTML>
*/
