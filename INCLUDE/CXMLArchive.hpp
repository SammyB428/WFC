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
** $Workfile: CXMLArchive.hpp $
** $Revision: 11 $
** $Modtime: 6/26/01 11:05a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( XML_ARCHIVE_CLASS_HEADER )

#define XML_ARCHIVE_CLASS_HEADER

class CXMLArchive; // Catch-22 here

using XML_ARCHIVE_SERIALIZE_OBJECT = bool (*)( __inout CXMLArchive * archive_p, _In_ std::wstring_view name, __inout void * parameter );

class CXMLArchive
{
   protected:

       bool m_AmIWriting{ false };
       bool m_AddNewLineAfterEachElement{ false };

      std::wstring m_TimeZone;

      CExtensibleMarkupLanguageElement * m_Element_p{ nullptr };

   public:

       CXMLArchive(_In_ CXMLArchive const&) = delete;
       _Check_return_ CXMLArchive& operator=(_In_ CXMLArchive const&) = delete;
 
       CXMLArchive();
      virtual ~CXMLArchive();

      virtual void SetAddNewLineAfterEachElement( _In_ bool const add_new_line = true ) noexcept;
      virtual _Check_return_ bool GetAddNewLineAfterEachElement( void ) const noexcept;
      virtual _Check_return_ bool IsLoading( void ) const noexcept;
      virtual _Check_return_ bool IsStoring( void ) const noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * GetElement( void ) const noexcept;

      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ bool&             value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ std::vector<uint8_t>& value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ std::vector<uint32_t>& value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ CFileTime&        value ) noexcept;

#if ! defined( WFC_STL )
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ COleDateTime&     value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ COleDateTimeSpan& value ) noexcept;
#endif // WFC_STL

      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ std::wstring&          value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ std::vector<std::wstring>& value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ CSystemTime&      value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ CTime&            value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ CTimeSpan&        value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ double&           value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ uint32_t&         value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ int64_t&          value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Read(_In_ std::wstring_view tag, _Out_ uint64_t&         value ) noexcept;

      virtual void ReadFrom( __inout CExtensibleMarkupLanguageElement * element_p ) noexcept;

      virtual _Check_return_ bool SerializeObject(_In_ std::wstring_view tag, __inout void * object_p, __callback XML_ARCHIVE_SERIALIZE_OBJECT write_function ) noexcept;

      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ bool const             value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ std::vector<uint8_t> const& value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ std::vector<uint32_t> const& value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ CFileTime const&        value ) noexcept;

#if ! defined( WFC_STL )      
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ COleDateTime const&     value, BOOL value_is_UTC_time = FALSE ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ COleDateTimeSpan const& value ) noexcept;
#endif // WFC_STL

      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ std::wstring_view          value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ std::vector<std::wstring> const& value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ CSystemTime const&      value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ CTime const&            value, _In_ bool const value_is_UTC_time = false ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ CTimeSpan const&        value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ double const            value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ uint32_t const         value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ int64_t const          value ) noexcept;
      virtual _Check_return_ CExtensibleMarkupLanguageElement * Write(_In_ std::wstring_view tag, _In_ uint64_t const         value ) noexcept;

      virtual void WriteTo( __inout CExtensibleMarkupLanguageElement * element_p ) noexcept;
};

#endif // XML_ARCHIVE_CLASS_HEADER
