/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2022, Samuel R. Blackburn
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
** $Revision: 24 $
** $Modtime: 7/17/01 5:23a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if ! defined( WFC_STL )

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#endif // WFC_STL

#if ! defined( DWORD_PTR )
#define DWORD_PTR DWORD
#endif

static inline void append_buffer( __in_bcount( number_of_bytes ) uint8_t const * buffer_parameter, _In_ std::size_t const number_of_bytes, __inout std::vector<uint8_t>& bytes ) noexcept
{
   WFC_VALIDATE_POINTER( buffer_parameter );
   auto const current_length{ bytes.size() };

   bytes.resize( current_length + number_of_bytes );

   auto buffer{ bytes.data() };

   CopyMemory( &buffer[ current_length ], buffer_parameter, number_of_bytes );
}

static inline void append_string( __in_z char const * string_p, __inout std::vector<uint8_t>& bytes ) noexcept
{
   WFC_VALIDATE_POINTER( string_p );
   std::size_t const string_length{ strlen(string_p) };

   append_buffer( reinterpret_cast<uint8_t const *>(string_p), string_length, bytes );
}

static inline void append_item( _In_ Win32FoundationClasses::WFC_WEB_POST_DATA * item, __in_z char const * boundary, __inout std::vector<uint8_t>& bytes )
{
   WFC_VALIDATE_POINTER( item );
   WFC_VALIDATE_POINTER( item->variable_name );
   WFC_VALIDATE_POINTER( boundary );

   append_string( boundary, bytes );
   append_string( "\r\nContent-Disposition: form-data; name=\"", bytes );
   append_string( item->variable_name, bytes );
   append_string( "\"", bytes );

   switch( item->data_type )
   {
      case WFC_WEB_POST_STRING:

         WFC_VALIDATE_POINTER( item->ascii_string );
         append_string( "\r\n\r\n", bytes );
         append_string( item->ascii_string, bytes );
         break;

      case WFC_WEB_POST_BUFFER:

         WFC_VALIDATE_POINTER( item->byte_buffer.buffer );
         append_string( "\r\n\r\n", bytes );
         append_buffer( item->byte_buffer.buffer, item->byte_buffer.buffer_length, bytes );
         break;

      case WFC_WEB_POST_CBYTEARRAY:

         append_string( "\r\n\r\n", bytes );
         append_buffer( item->bytes->data(), item->bytes->size(), bytes );
         break;

      case WFC_WEB_POST_FILE_HANDLE:

         WFC_VALIDATE_POINTER( item->file.ascii_filename );
         append_string( "; filename=\"", bytes );
         append_string( item->file.ascii_filename, bytes );
         append_string( "\"\r\nContent-Type: application/octet-stream\r\n\r\n", bytes );

         Win32FoundationClasses::CFile64 file( item->file.file_handle );

         // Unlike the children at Microsoft that wrote ISAPI, WFC uses 64-bit file offsets
         std::ignore = file.Seek( (int64_t) item->file.file_offset.QuadPart, Win32FoundationClasses::CFile64::SeekPosition::begin );

         std::vector<uint8_t> bytes_read;

         _ASSERTE( item->file.number_of_bytes.QuadPart <= 0xFFFFFFFF );
         std::ignore = file.Read( bytes_read, item->file.number_of_bytes.LowPart );

         bytes.insert(std::end(bytes), std::cbegin(bytes_read), std::cend(bytes_read));
         bytes_read.clear();

         break;
   }

   append_string( "\r\n", bytes );
}

_Check_return_ bool Win32FoundationClasses::wfc_web_post( _In_ std::wstring_view url, _In_ WFC_WEB_POST_DATA ** data, __inout_opt std::wstring * response ) noexcept
{
   WFC_VALIDATE_POINTER( data );

   if ( url.empty() == true or
        data == nullptr )
   {
      return( false );
   }

   char const * boundary_string{ "----4225101SRBLLBWFCISFREEFORYOUANDME" };

   std::vector<uint8_t> bytes_to_send;

   int number_of_parameters{ 0 };

   WFC_TRY
   {
      while( data[ number_of_parameters ] not_eq nullptr )
      {
         auto data_p{ data[number_of_parameters] };

         append_item( data_p, boundary_string, bytes_to_send );

         number_of_parameters++;
      }

      append_string( boundary_string, bytes_to_send );
      append_string( "--\r\n", bytes_to_send );

      DWORD const internet_open_access_type{ INTERNET_OPEN_TYPE_PRECONFIG };
      LPCTSTR internet_open_proxy{ nullptr };
      LPCTSTR internet_open_proxy_bypass{ nullptr };
      DWORD const internet_open_flags{ 0 };

      auto internet_handle{ ::InternetOpenW(L"Mozilla/4.0 (compatible; Win32 Foundation Classes; http://www.samblackburn.com)",
                                      internet_open_access_type,
                                      internet_open_proxy,
                                      internet_open_proxy_bypass,
                                      internet_open_flags) };

      if ( internet_handle == static_cast< HINTERNET >( NULL ) )
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACE( TEXT( "InternetOpen() failed." ) );
         return( false );
      }

      INTERNET_PORT const internet_connect_port{ INTERNET_DEFAULT_HTTP_PORT };
      LPCTSTR       internet_connect_user_name{ nullptr };
      LPCTSTR       internet_connect_password{ nullptr };
      DWORD const internet_connect_service{ INTERNET_SERVICE_HTTP };
      DWORD const internet_connect_flags{ INTERNET_FLAG_RELOAD bitor INTERNET_FLAG_NO_CACHE_WRITE };
      DWORD const internet_connect_context{ 0 };

      CUniformResourceLocator uniform_resource_locator( url );

      auto internet_connection_handle{ ::InternetConnectW(internet_handle,
                                                    uniform_resource_locator.MachineName.c_str(),
                                                    internet_connect_port,
                                                    internet_connect_user_name,
                                                    internet_connect_password,
                                                    internet_connect_service,
                                                    internet_connect_flags,
                                                    internet_connect_context) };

      if ( internet_connection_handle == static_cast< HINTERNET >( NULL ) )
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACE( TEXT( "InternetOpenUrl() failed." ) );

         InternetCloseHandle( internet_handle );
         internet_handle = static_cast< HINTERNET >( NULL );

         return( false );
      }

      LPCTSTR   http_open_request_verb{ TEXT("POST") };
      LPCTSTR   http_open_request_object_name{ uniform_resource_locator.PathName.c_str() };
      LPCTSTR   http_open_request_version{ nullptr };
      LPCTSTR   http_open_request_referer{ nullptr };
      LPCTSTR* http_open_request_accept_types{ nullptr };
      DWORD const http_open_request_flags{ INTERNET_FLAG_NO_CACHE_WRITE };
      DWORD_PTR const http_open_request_context{ 0 };

      auto http_request_handle{ HttpOpenRequest(internet_connection_handle,
                                             http_open_request_verb,
                                             http_open_request_object_name,
                                             http_open_request_version,
                                             http_open_request_referer,
                                             http_open_request_accept_types,
                                             http_open_request_flags,
                                             http_open_request_context) };

      if ( http_request_handle == static_cast< HINTERNET >( NULL ) )
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACE( TEXT( "HttpOpenRequest() failed." ) );

         InternetCloseHandle( internet_connection_handle );
         internet_connection_handle = static_cast< HINTERNET >( NULL );

         InternetCloseHandle( internet_handle );
         internet_handle = static_cast< HINTERNET >( NULL );

         return( false );
      }

      // Now put in the parameters...

      char temp_string[ 256 ];
      
      strcpy_s( temp_string, std::size( temp_string ), "Content-Type: multipart/form-data; boundary=" );
      strcat_s( temp_string, std::size( temp_string ), boundary_string );
      strcat_s( temp_string, std::size( temp_string ), "\r\n" );

      DWORD const http_add_request_headers_modifiers{ HTTP_ADDREQ_FLAG_REPLACE bitor HTTP_ADDREQ_FLAG_ADD };

      if ( HttpAddRequestHeadersA( http_request_handle,
                                   temp_string,
                           (DWORD) strlen( temp_string ),
                                   http_add_request_headers_modifiers ) == FALSE )
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACE( TEXT( "HttpAddRequestHeaders() failed." ) );

         InternetCloseHandle( http_request_handle );
         http_request_handle = static_cast< HINTERNET >( NULL );

         InternetCloseHandle( internet_connection_handle );
         internet_connection_handle = static_cast< HINTERNET >( NULL );

         InternetCloseHandle( internet_handle );
         internet_handle = static_cast< HINTERNET >( NULL );

         return( false );
      }

      _ASSERTE( bytes_to_send.size() <= 0xFFFFFFFF ); // Max size of a 32-bit DWORD

      INTERNET_BUFFERS http_send_request_ex_input_buffers;

      http_send_request_ex_input_buffers.dwStructSize    = sizeof( http_send_request_ex_input_buffers );
      http_send_request_ex_input_buffers.Next            = nullptr;
      http_send_request_ex_input_buffers.lpcszHeader     = nullptr;
      http_send_request_ex_input_buffers.dwHeadersLength = 0;
      http_send_request_ex_input_buffers.dwHeadersTotal  = 0;
      http_send_request_ex_input_buffers.lpvBuffer       = nullptr;
      http_send_request_ex_input_buffers.dwBufferLength  = 0;
      http_send_request_ex_input_buffers.dwBufferTotal   = (DWORD) bytes_to_send.size();
      http_send_request_ex_input_buffers.dwOffsetLow     = 0;
      http_send_request_ex_input_buffers.dwOffsetHigh    = 0;

      INTERNET_BUFFERS* http_send_request_ex_output_buffers{ nullptr };
      DWORD const        http_send_request_ex_flags{ 0 };
      DWORD const        http_send_request_ex_context{ 0 };

      if ( HttpSendRequestEx( http_request_handle,
                             &http_send_request_ex_input_buffers,
                              http_send_request_ex_output_buffers,
                              http_send_request_ex_flags,
                              http_send_request_ex_context ) == FALSE )
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACE( TEXT( "HttpSendRequestEx() failed." ) );

         InternetCloseHandle( http_request_handle );
         http_request_handle = static_cast< HINTERNET >( NULL );

         InternetCloseHandle( internet_connection_handle );
         internet_connection_handle = static_cast< HINTERNET >( NULL );

         InternetCloseHandle( internet_handle );
         internet_handle = static_cast< HINTERNET >( NULL );

         return( false );
      }

      // Now send the guts of the data

      DWORD number_of_bytes_sent{ 0 };

      InternetWriteFile( http_request_handle,
                         bytes_to_send.data(),
                 (DWORD) bytes_to_send.size(),
                        &number_of_bytes_sent );

      ASSERT( number_of_bytes_sent == bytes_to_send.size() );

      INTERNET_BUFFERS* http_end_request_output_buffers{ nullptr };
      DWORD const        http_end_request_flags{ 0 };
      DWORD const        http_end_request_context{ 0 };

      if ( HttpEndRequest( http_request_handle,
                           http_end_request_output_buffers,
                           http_end_request_flags,
                           http_end_request_context ) == FALSE )
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACE( TEXT( "HttpEndRequest() failed." ) );

         InternetCloseHandle( http_request_handle );
         http_request_handle = static_cast< HINTERNET >( NULL );

         InternetCloseHandle( internet_connection_handle );
         internet_connection_handle = static_cast< HINTERNET >( NULL );

         InternetCloseHandle( internet_handle );
         internet_handle = static_cast< HINTERNET >( NULL );

         return( false );
      }

      // Now read back the status

      number_of_bytes_sent = 0;

      constexpr std::size_t const response_buffer_size{ (64 * 1024) + 1 };

      auto response_buffer{ std::make_unique<uint8_t[]>(response_buffer_size) };

      if ( response_buffer.get() not_eq nullptr )
      {
         ZeroMemory( response_buffer.get(), response_buffer_size );

         InternetReadFile( http_request_handle,
                           response_buffer.get(),
                           response_buffer_size - 1, // Always leave room for the null terminator
                          &number_of_bytes_sent );
      }

      InternetCloseHandle( http_request_handle );
      http_request_handle = static_cast< HINTERNET >( NULL );

      InternetCloseHandle( internet_connection_handle );
      internet_connection_handle = static_cast< HINTERNET >( NULL );

      InternetCloseHandle( internet_handle );
      internet_handle = static_cast< HINTERNET >( NULL );

      if ( response not_eq nullptr and response_buffer not_eq nullptr )
      {
         copy( *response, reinterpret_cast<char const *>(response_buffer.get()) );
      }

      return( true );
   }
   WFC_CATCH_ALL
   {
      return( false );
   }
   WFC_END_CATCH_ALL
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_web_post</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that retrieves a web page and stores in an array of strings.">
</HEAD>

<BODY>

<H1>wfc_web_post</H1>
$Revision: 24 $<HR>

<H2>Declaration</H2>
<PRE><CODE>bool <B>wfc_web_post</B>( CUniformResourceLocator&amp; url, std::vectgor&lt;std::wstring&gt;&amp; page_contents )
void <B>wfc_web_post</B>( CUniformResourceLocator&amp; url, std::vector&lt;uint8_t&gt;&amp; page_contents )</CODE></PRE>

<H2>Description</H2>
This function takes a url and retrieves it from Internet. What you get is an
array containing the source for that page. This function is meant
to provide the capability to pull data from Internet and chew on it inside
a program (as opposed to display it to a user).

<H2>Example</H2>

<PRE><CODE>void print_maryland_weather_report( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;print_maryland_weather_report()&quot; ) );

   CUniformResourceLocator url( TEXT( &quot;http://iwin.nws.noaa.gov/iwin/md/hourly.html&quot; ) );

   std::vector&lt;std::wstring&gt; weather_report;

   <B>wfc_get_web_page</B>( url, weather_report );

   int index{ 0 };
   int number_of_lines_in_report{ weather_report.GetSize() };

   while( index &lt; number_of_lines_in_report )
   {
      _tprintf( TEXT( &quot;%s\n&quot; ), (LPCTSTR) weather_report.GetAt( index ) );
      index++;
   }
}</CODE></PRE>

<HR><I>Copyright, 1995-2001, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_get_web_page.cpp $<BR>
$Modtime: 7/17/01 5:23a $
</BODY>

</HTML>
*/
