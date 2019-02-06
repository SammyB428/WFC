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
** $Workfile: CXMLArchive.hpp $
** $Revision: 11 $
** $Modtime: 6/26/01 11:05a $
*/

#if ! defined( XML_ARCHIVE_CLASS_HEADER )

#define XML_ARCHIVE_CLASS_HEADER

class CXMLArchive; // Catch-22 here

using XML_ARCHIVE_SERIALIZE_OBJECT = bool (*)( __inout CXMLArchive * archive_p, __in const wchar_t * name, __inout void * parameter );

class CXMLArchive
{
   protected:

       bool m_AmIWriting{ false };
       bool m_AddNewLineAfterEachElement{ false };

      std::wstring m_TimeZone;

      CExtensibleMarkupLanguageElement * m_Element_p{ nullptr };

   public:

       CXMLArchive(_In_ const CXMLArchive&) = delete;
       _Check_return_ CXMLArchive& operator=(_In_ const CXMLArchive&) = delete;
 
       CXMLArchive();
      virtual ~CXMLArchive();

      virtual void SetAddNewLineAfterEachElement( __in const bool add_new_line = true ) noexcept;
      virtual __checkReturn bool GetAddNewLineAfterEachElement( void ) const noexcept;
      virtual __checkReturn bool IsLoading( void ) const noexcept;
      virtual __checkReturn bool IsStoring( void ) const noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * GetElement( void ) const noexcept;

      virtual __checkReturn CExtensibleMarkupLanguageElement * Read( _In_z_ const wchar_t * tag, __out bool&             value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out std::vector<uint8_t>& value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out std::vector<uint32_t>& value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out CFileTime&        value ) noexcept;

#if ! defined( WFC_STL )
      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out COleDateTime&     value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out COleDateTimeSpan& value ) noexcept;
#endif // WFC_STL

      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out std::wstring&          value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out std::vector<std::wstring>& value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out CSystemTime&      value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out CTime&            value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out CTimeSpan&        value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out double&           value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out uint32_t&         value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out int64_t&          value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Read(_In_z_ const wchar_t * tag, __out uint64_t&         value ) noexcept;

      virtual void ReadFrom( __inout CExtensibleMarkupLanguageElement * element_p ) noexcept;

      virtual __checkReturn bool SerializeObject( _In_z_ const wchar_t * tag, __inout void * object_p, __callback XML_ARCHIVE_SERIALIZE_OBJECT write_function ) noexcept;

      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in const bool              value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in const std::vector<uint8_t>& value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in const std::vector<uint32_t>& value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in const CFileTime&        value ) noexcept;

#if ! defined( WFC_STL )      
      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in const COleDateTime&     value, BOOL value_is_UTC_time = FALSE ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in const COleDateTimeSpan& value ) noexcept;
#endif // WFC_STL

      virtual __checkReturn CExtensibleMarkupLanguageElement * Write( _In_z_ const wchar_t * tag, __in const std::wstring&          value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in const std::vector<std::wstring>& value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in const CSystemTime&      value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in const CTime&            value, __in const bool value_is_UTC_time = false ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in const CTimeSpan&        value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in double                  value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in const uint32_t          value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in const int64_t           value ) noexcept;
      virtual __checkReturn CExtensibleMarkupLanguageElement * Write(_In_z_ const wchar_t * tag, __in const uint64_t          value ) noexcept;

      virtual void WriteTo( __inout CExtensibleMarkupLanguageElement * element_p ) noexcept;
};

#endif // XML_ARCHIVE_CLASS_HEADER
