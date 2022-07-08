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
** $Workfile: wfc_convert_double_null_terminated_string.cpp $
** $Revision: 6 $
** $Modtime: 11/28/00 6:37p $
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

void Win32FoundationClasses::wfc_free_unicode_conversion_context( _Inout_ VOID * unicode_conversion_context ) noexcept
{
   WFC_VALIDATE_POINTER( unicode_conversion_context );

   auto IMultilanguage2_interface_p{ reinterpret_cast<IMultiLanguage2*>(unicode_conversion_context) };

   if ( IMultilanguage2_interface_p not_eq nullptr )
   {
      IMultilanguage2_interface_p->Release();
   }
}

_Check_return_ VOID * Win32FoundationClasses::wfc_get_unicode_conversion_context( _In_ bool const is_com_already_started_in_this_thread ) noexcept
{
   if ( is_com_already_started_in_this_thread == false )
   {
       WFC_INITIALIZE_SINGLE_THREADED_COM_NO_DDE();
   }

   GUID const mlang_class_id               { 0x275C23E2, 0x3747, 0x11D0, 0x9F, 0xEA, 0x00, 0xAA, 0x00, 0x3F, 0x86, 0x46 };
   GUID const imultilanguage2_interface_id { 0xDCCFC164, 0x2B38, 0x11D2, 0xB7, 0xEC, 0x00, 0xC0, 0x4F, 0x8F, 0x5D, 0x9A };

   //WFCTRACEVAL( TEXT( "mlang_class_id is " ), mlang_class_id );
   //WFCTRACEVAL( TEXT( "imultilanguage2_interface_id is " ), imultilanguage2_interface_id );

   VOID * return_value{ nullptr };

   auto const result_code{ ::CoCreateInstance(mlang_class_id,
                                              nullptr,
                                              CLSCTX_INPROC_SERVER bitor CLSCTX_LOCAL_SERVER,
                                              imultilanguage2_interface_id,
                                             &return_value) };

   return( return_value );
}

#define MINIMUM_CODE_PAGE_DETECTION_BUFFER_SIZE (512)

static inline _Check_return_ bool __detect_code_page(_In_ IMultiLanguage2 * interface_pointer, _In_reads_bytes_( number_of_bytes_in_buffer ) uint8_t const * bytes, _In_ std::size_t number_of_bytes_in_buffer, _In_ DWORD encoding_hints, _Out_ DWORD& detected_code_page ) noexcept
{
   WFC_VALIDATE_POINTER( interface_pointer );
   WFC_VALIDATE_POINTER( bytes );

   WFC_TRY
   {
      detected_code_page = WFC_DEFAULT_CODEPAGE;

      if (bytes == nullptr or number_of_bytes_in_buffer < 1)
      {
          return(false);
      }

      DWORD const number_of_encodings{ 100 };

      auto encodings{ std::make_unique<DetectEncodingInfo[]>(number_of_encodings) };

      if ( encodings.get() == nullptr )
      {
         return( false );
      }

      auto buffer_to_scan{ reinterpret_cast<char*>(const_cast<BYTE*>(bytes)) };

      std::unique_ptr<uint8_t[]> new_buffer;

      auto number_of_bytes{ static_cast<INT>(number_of_bytes_in_buffer) };

      if (number_of_bytes < MINIMUM_CODE_PAGE_DETECTION_BUFFER_SIZE)
      {
          int const number_of_copies{ (MINIMUM_CODE_PAGE_DETECTION_BUFFER_SIZE / number_of_bytes) + 1 };

          auto const new_buffer_size{ static_cast<int>((number_of_copies * static_cast<int>(number_of_bytes_in_buffer)) + number_of_copies) };

          new_buffer = std::make_unique<uint8_t []>(new_buffer_size);

          WFC_VALIDATE_POINTER(new_buffer.get());

          if (new_buffer.get() == nullptr)
          {
              return(false);
          }

          int buffer_write_index{ 0 };

          while (buffer_write_index < new_buffer_size)
          {
              CopyMemory(&new_buffer[buffer_write_index], bytes, number_of_bytes_in_buffer);
              buffer_write_index += static_cast<int>(number_of_bytes_in_buffer);
              new_buffer[buffer_write_index] = ' ';
              buffer_write_index++;
          }

          // Now switch to the new buffer

          buffer_to_scan = reinterpret_cast<char *>(new_buffer.get());
          number_of_bytes = new_buffer_size;
      }

      bool return_value{ false };

      INT array_size{ number_of_encodings };

      ZeroMemory( encodings.get(), sizeof( DetectEncodingInfo ) * number_of_encodings );

      auto const ole_result{ interface_pointer->DetectInputCodepage(encoding_hints,
                                                                    RETURN_ALL_POSSIBLE_ENCODINGS,
                                                                    buffer_to_scan,
                                                                   &number_of_bytes,
                                                                    encodings.get(),
                                                                   &array_size) };

      auto const error_code{ ::GetLastError() };

      if ( ole_result not_eq S_OK )
      {
         if ( ole_result not_eq S_FALSE )
         {
            //WFCTRACEVAL( TEXT( "Failed with error code " ), ole_result );
            //WFCTRACEVAL( TEXT( "While working on this many bytes " ), (UINT) number_of_bytes_in_buffer );
            return( false );
         }
      }

      // Time to decide on the best code page

      if ( array_size > 0 )
      {
         INT last_confidence{ encodings[0].nConfidence };

         detected_code_page = encodings[ 0 ].nCodePage;

         if ( static_cast<long>(encodings[ 0 ].nDocPercent) < 0 )
         {
            detected_code_page = WFC_DEFAULT_CODEPAGE;
         }
         else
         {
            return_value = true;
         }

         for ( auto const encodings_index : Range(array_size) )
         {
            if ( static_cast<long>(encodings[ encodings_index ].nDocPercent ) > 0 )
            {
               // Some part of the document looks like this code page

               if ( encodings[ encodings_index ].nConfidence > last_confidence )
               {
                  detected_code_page = encodings[ encodings_index ].nCodePage;
                  return_value = true;
               }
            }
         }
      }
      else
      {
         //WFCTRACE( TEXT( "No array elements returned" ) );
      }

      return( return_value );
   }
   WFC_CATCH_ALL
   {
   }
   WFC_END_CATCH_ALL

   return( false );
}

_Check_return_ bool Win32FoundationClasses::wfc_detect_code_page( _Inout_ VOID * unicode_conversion_context, _In_reads_bytes_( number_of_bytes_in_buffer ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes_in_buffer, _In_ uint32_t const encoding_hints, _Out_ uint32_t& code_page ) noexcept
{
   code_page = 0;

   if (unicode_conversion_context == nullptr)
   {
       return(false);
   }

   auto IMultilanguage2InterfacePointer{ reinterpret_cast<IMultiLanguage2*>(unicode_conversion_context) };

   DWORD other_code_page{ 0 };

   WFC_TRY
   {
       if (__detect_code_page(IMultilanguage2InterfacePointer,
                              buffer,
                              number_of_bytes_in_buffer,
                              encoding_hints,
                              other_code_page) == false)
       {
           return(false);
       }
   }
   WFC_CATCH_ALL
   {
       return(false);
   }
   WFC_END_CATCH_ALL

   code_page = other_code_page;

   return( true );
}

_Check_return_ bool Win32FoundationClasses::wfc_convert_to_unicode(
                                   _Inout_ VOID *            unicode_conversion_context, 
                                   _In_  std::vector<uint8_t> const& bytes,
                                   __out_ecount_z( number_of_wide_characters ) wchar_t * unicode_string,
                                   _In_ std::size_t const            number_of_wide_characters,
                                   _In_ DWORD const            suggested_code_page,
                                   _In_ DWORD const            encoding_hints,
                                   __out_opt DWORD *           real_code_page_p ) noexcept
{
   WFC_VALIDATE_POINTER( unicode_conversion_context );

   return( Win32FoundationClasses::wfc_convert_to_unicode( unicode_conversion_context, bytes.data(), bytes.size(), unicode_string, number_of_wide_characters, suggested_code_page, encoding_hints, real_code_page_p ) );
}

_Check_return_ bool Win32FoundationClasses::wfc_convert_to_unicode( _Inout_ VOID *       unicode_conversion_context,
                                    _In_reads_bytes_( number_of_bytes_in_buffer ) BYTE const * bytes,
                                    _In_ std::size_t const       number_of_bytes_in_buffer,
                                    __out_ecount_z( number_of_wide_characters ) wchar_t *    unicode_string,
                                    _In_ std::size_t const       number_of_wide_characters,
                                    _In_ DWORD const       suggested_code_page,
                                    _In_ DWORD const       encoding_hints,
                                    __out_opt DWORD *      real_code_page_p ) noexcept
{
   WFC_VALIDATE_POINTER( unicode_conversion_context );

   if (real_code_page_p not_eq nullptr)
   {
       *real_code_page_p = 0;
   }

   auto IMultilanguage2InterfacePointer{ reinterpret_cast<IMultiLanguage2*>(unicode_conversion_context) };

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      ZeroMemory( unicode_string, number_of_wide_characters * sizeof( wchar_t ) );

      DWORD mode{ 0 };

      UINT source_size{ static_cast<UINT>(number_of_bytes_in_buffer) };
      UINT destination_number_of_characters{ 0 };

      if ( IMultilanguage2InterfacePointer == nullptr )
      {
         // We must assume COM has been inialized...

         IMultilanguage2InterfacePointer = reinterpret_cast<IMultiLanguage2*>(Win32FoundationClasses::wfc_get_unicode_conversion_context(false));

         if ( IMultilanguage2InterfacePointer == nullptr )
         {
            //WFCTRACE( TEXT( "Can't get MLANG interface pointer." ) );
            return( false );
         }
      }

      auto const result_code{ IMultilanguage2InterfacePointer->ConvertStringToUnicodeEx(
                   &mode,
                    suggested_code_page,
           reinterpret_cast<CHAR*>(const_cast<BYTE*>(bytes)),
                   &source_size,
                    unicode_string,
                   &destination_number_of_characters,
                    0,
                    nullptr) };

      //WFCTRACEVAL( TEXT( "result_code is " ), result_code );

      if ( result_code == S_OK )
      {
         // We're done!
         return( true );
      }

      DWORD other_code_page{ 0 };

      if ( __detect_code_page( IMultilanguage2InterfacePointer,
                               bytes,
                               number_of_bytes_in_buffer,
                               encoding_hints,
                               other_code_page ) == false )
      {
         return( false );
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
<TITLE>WFC - wfc_convert_to_unicode</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that bytes to Unicode.">
</HEAD>

<BODY>

<H1>wfc_convert_to_unicode</H1>
$Revision: 6 $<HR>

<H2>Declaration</H2>
<PRE><CODE>void wfc_convert_to_unicode( const BYTE * unicode_string, buffer, std::vectgor&lt;std::wstring&gt;&amp; string_array ) )</CODE></PRE>

<H2>Description</H2>
This function converts a double-NULL terminated string to a std::vectgor&lt;std::wstring&gt; object.

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   TCHAR dos_device_names[ 4096 ];

   ZeroMemory( dos_device_names, sizeof( dos_device_names ) );

   DWORD return_value { 0 };

   return_value = QueryDosDevice( nullptr, dos_device_names, std::size( dos_device_names ) );

   std::vectgor&lt;std::wstring&gt; names;

   <B>wfc_convert_double_null_terminated_string</B>( reinterpret_cast&lt; const BYTE * &gt;( dos_device_names ), names );

   int loop_index         { 0};
   int number_of_elements { names.GetSize()};

   while( loop_index < number_of_elements )
   {
      _tprintf( TEXT( &quot;%s\n&quot; ), (LPCTSTR) names.GetAt( loop_index ) );
      WFCTRACEVAL( TEXT( &quot;Name: &quot; ), names.GetAt( loop_index ) );
      loop_index++;
   }

   return( EXIT_SUCCESS );
}</CODE></PRE>

<HR><I>Copyright, 1995-2001, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_convert_double_null_terminated_string.cpp $<BR>
$Modtime: 11/28/00 6:37p $
</BODY>

</HTML>
*/
