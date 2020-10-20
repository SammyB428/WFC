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
** $Workfile: CExtensibleMarkupLanguage.hpp $
** $Revision: 36 $
** $Modtime: 8/30/01 7:01p $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( XML_CLASSES_HEADER )

#define XML_CLASSES_HEADER

    // Helper functions

    _Check_return_ bool is_xml_BaseChar(_In_ uint32_t const character_to_test) noexcept;
    _Check_return_ bool is_xml_Char(_In_ uint32_t const character_to_test) noexcept;
    _Check_return_ bool is_xml_CombiningChar(_In_ uint32_t const character_to_test) noexcept;
    _Check_return_ bool is_xml_Digit(_In_ uint32_t const character_to_test) noexcept;
    _Check_return_ bool is_xml_Extender(_In_ uint32_t const character_to_test) noexcept;
    _Check_return_ bool is_xml_Ideographic(_In_ uint32_t const character_to_test) noexcept;
    _Check_return_ bool is_xml_Letter(_In_ uint32_t const character_to_test) noexcept;
    _Check_return_ bool is_xml_NameChar(_In_ uint32_t const character_to_test) noexcept;
    _Check_return_ bool is_xml_PubidChar(_In_ uint32_t const character_to_test) noexcept;

    inline constexpr _Check_return_ bool is_xml_white_space(_In_ uint32_t const character_to_test) noexcept
    {
        // Test according to Rule 3

        if (character_to_test == 0x0020 or
            character_to_test == 0x000D or
            character_to_test == 0x000A or
            character_to_test == 0x0009)
        {
            return(true);
        }

        return(false);
    }

    // An Entity (as described in the XML specification section 4.2) is "An
    // entity reference refers to the content of a named entity." Doesn't that
    // make sense?? OK, if you're a layman (like me) it means "that which lies
    // between & and ;". What it does is allow you to define your own
    // search-and-replace strings. If you know HTML, then you know what an
    // "entity" is. Here's some examples from HTML:
    // &gt; &lt; &quot;
    // XML let's you roll your own:
    // &myentity; &yourentity;
    // In the DTD document (which I'll get around to parsing later) contains
    // a list of these entities in <!ENTITY lines for example:
    // <!ENTITY myentity "I hate entities." >

    class CExtensibleMarkupLanguageEntities
    {
    protected:

        std::vector<std::wstring> m_Entities;
        std::vector<std::wstring> m_Values;

    public:

        // Construction

        CExtensibleMarkupLanguageEntities() noexcept;
        explicit CExtensibleMarkupLanguageEntities(_In_ CExtensibleMarkupLanguageEntities const& source) noexcept;

        // Methods

        _Check_return_ bool Add(_In_ std::wstring_view tag, _In_ std::wstring_view text) noexcept;

        inline void Copy(_In_ CExtensibleMarkupLanguageEntities const& source) noexcept
        {
            m_Entities = source.m_Entities;
            m_Values = source.m_Values;
        }

        // Returns the object to an initial state
        inline void Empty(void) noexcept
        {
            m_Entities.clear();
            m_Values.clear();
        }

        _Check_return_ bool Enumerate(_Inout_ std::size_t& enumerator) const noexcept;
        _Check_return_ bool GetNext(_Inout_ std::size_t& enumerator, _Out_ std::wstring& entity, _Out_ std::wstring& value) const noexcept;

        inline _Check_return_ std::size_t GetSize(void) const noexcept
        {
            ASSERT(m_Entities.size() == m_Values.size());
            return(m_Entities.size());
        }

        _Check_return_ bool IsEntity(_In_ std::wstring_view tag, _Out_ uint32_t& rule_that_was_broken) const noexcept;
        _Check_return_ bool Resolve(_In_ std::wstring_view tag, _Out_ std::wstring& text) const noexcept;

        // Operators

        _Check_return_ CExtensibleMarkupLanguageEntities& operator=(_In_ CExtensibleMarkupLanguageEntities const& source) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

        virtual void Dump(CDumpContext& dump_context) const;

#endif // _DEBUG
    };

    class CExtensibleMarkupLanguageAttribute
    {
    public:

        // Construction

        CExtensibleMarkupLanguageAttribute() noexcept = default;
        inline explicit CExtensibleMarkupLanguageAttribute(_In_ CExtensibleMarkupLanguageAttribute const& source) noexcept
        {
            Name = source.Name;
            Value = source.Value;
        }

        // Properties

        std::wstring Name;
        std::wstring Value;

        // The byte index where the first character of the name is.
        uint64_t NameOffset{ 0 };

        // The byte indec where the first character of the value is (after the double quote).
        uint64_t ValueOffset{ 0 };

        // Methods

        inline void Copy(_In_ CExtensibleMarkupLanguageAttribute const& source) noexcept
        {
            Name = source.Name;
            Value = source.Value;
            NameOffset = source.NameOffset;
            ValueOffset = source.ValueOffset;
        }

        // Returns the object to an initial state.
        inline void Empty(void) noexcept
        {
            Name.clear();
            Value.clear();
            NameOffset = 0;
            ValueOffset = 0;
        }

        _Check_return_ CExtensibleMarkupLanguageAttribute& operator = (_In_ CExtensibleMarkupLanguageAttribute const& source) noexcept
        {
            Name = source.Name;
            Value = source.Value;
            NameOffset = source.NameOffset;
            ValueOffset = source.ValueOffset;
            return(*this);
        }

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

        virtual void Dump(CDumpContext& dump_context) const;

#endif // _DEBUG
    };

    class CExtensibleMarkupLanguageDocument; // Avoid chicken-or-the-egg problem
    class CExtensibleMarkupLanguageElement;  // Avoid chicken-or-the-egg problem

    using XML_ELEMENT_CALLBACK = void (*)(_Inout_ void* parameter, _Inout_ CExtensibleMarkupLanguageElement * element_p);

    class CExtensibleMarkupLanguageElement
    {
    public:

        enum class ElementType : uint32_t
        {
            Unknown = 0,
            ProcessingInstruction, // Start Tag is "<?" End Tag is "?>
            Comment, // Start Tag is "<!--" End Tag is "-->"
            CharacterData, // Start Tag is "<![CDATA[" End Tag is "]]>"
            Element, // A user's element
            TextSegment, // that which lies between sub-elements
            MetaData // Anything in a "<!" field that ain't typeComment or typeCharacterData
        };

    private:

        // Don't allow stack objects...

        CExtensibleMarkupLanguageElement() noexcept;
        explicit CExtensibleMarkupLanguageElement(_In_ CExtensibleMarkupLanguageElement const& source) noexcept;

    protected:

        CExtensibleMarkupLanguageDocument* m_Document{ nullptr };

        CExtensibleMarkupLanguageElement* m_Parent{ nullptr };

        std::vector<CExtensibleMarkupLanguageElement*> m_Children;   // Array of CExtensibleMarkupLanguageElement pointers
        std::vector<CExtensibleMarkupLanguageAttribute*> m_Attributes; // Array of CExtensibleMarkupLanguageAttribute pointers

        std::wstring m_Contents;
        std::wstring m_Tag;

        ElementType m_Type{ ElementType::Unknown };

        CParsePoint m_Beginning;
        CParsePoint m_Ending;

        bool m_AbortParsing{ false }; // Used only while parsing a document
        bool m_IsTagTerminated{ false };
        bool m_IsAllWhiteSpace{ false }; // true if we are a typeTextSegment and m_Text contains nothing but space-like characters
        bool m_ShorthandTerminatorDetected{ false };

        void m_AddCharacterToOutput(_In_ uint32_t const character, _In_ uint32_t const write_options, _Inout_ std::vector<uint8_t>& output) const noexcept;
        void m_AddIndentation(_Inout_ std::vector<uint8_t>& bytes) const noexcept;
        void m_AppendAttributes(_Inout_ std::vector<uint8_t>& data) const noexcept;
        void m_AppendChildren(_Inout_ std::vector<uint8_t>& data) const noexcept;
        void m_DecrementIndentation(void) const noexcept;
        void m_GetTag(_In_ std::wstring_view xml_data, _Inout_ std::wstring& tag) noexcept;
        void m_IncrementIndentation(void) const noexcept;
        inline constexpr void m_Initialize(void) noexcept { m_Parent = nullptr; };
        _Check_return_ bool m_ParseCDATASection(_In_ std::wstring_view tag, _In_ CDataParser const& parser) noexcept;
        _Check_return_ bool m_ParseDOCTYPESection(_In_ std::wstring_view tag, _In_ CDataParser const& parser) noexcept;
        _Check_return_ bool m_ParseElementName(_In_ std::wstring_view name, _Inout_ std::wstring& parent_name, _Inout_ uint32_t& desired_instance_number, _Out_ std::wstring& child_name) const noexcept;
        _Check_return_ bool m_ParseProcessingInstruction(void) noexcept;
        _Check_return_ bool m_ParseTag(_In_ std::wstring_view tag, _Inout_ bool& did_tag_contain_terminator, _In_ CDataParser const& parser) noexcept;
        _Check_return_ bool m_ParseXMLDeclaration(_In_ std::wstring_view tag) noexcept;
        void m_ReportParsingError(_In_ std::wstring_view error_message) noexcept;
        void m_ResolveEntities(_Inout_ std::wstring& encoded_entity_string) const noexcept;
        _Check_return_ bool m_TrimQuotesAndSpaces(_Inout_ std::wstring& value, _In_ wchar_t const quote_to_trim = '\"') const noexcept;

    public:

#if ! defined(WE_ARE_BUILDING_WFC_ON_UNIX)
        static _Check_return_ [[nodiscard]] CExtensibleMarkupLanguageElement* NewElement(__inout_opt CExtensibleMarkupLanguageElement* parent_element = nullptr, _In_ ElementType const type = ElementType::Element, __inout_opt CExtensibleMarkupLanguageDocument* document = nullptr) noexcept;
#else
        // Unix (g++-9) complains about [[nodiscard]]
        static _Check_return_ CExtensibleMarkupLanguageElement* NewElement(__inout_opt CExtensibleMarkupLanguageElement* parent_element = nullptr, _In_ ElementType const type = ElementType::Element, __inout_opt CExtensibleMarkupLanguageDocument* document = nullptr) noexcept;
#endif
        static void DeleteElement(_Inout_ CExtensibleMarkupLanguageElement* element_p) noexcept;

        virtual ~CExtensibleMarkupLanguageElement();

        /*
        ** <ITEM HREF=" http://www.samplecdf.com/page1.htm">
        **    <LOGO HREF=" http://www.samplecdf.com/red.ico" STYLE="ICON"/>
        **    <Log VALUE="document:view"/>
        **    <TITLE>The Red Page</TITLE>
        **    <ABSTRACT>This is the abstract description for the red page.</ABSTRACT>
        **    These are the times that try mens souls.
        ** </ITEM>
        */

        /*
        ** Element Start Tag     Attribute Name
        **  |                    |
        **  |    +---------------+
        **  v    v
        ** <ITEM HREF="http://www.samplecdf.com/page1.htm">
        ** These are the times that try mens souls.</ITEM>
        ** ^                                       ^
        ** |                                       |
        ** |                                       Element End Tag
        ** Content
        */

        // Element manipulation
        void                 AddChild(_Inout_ CExtensibleMarkupLanguageElement* element_p, _In_ bool const check_for_uniqueness = true, _In_ uint32_t const insert_at = (0xFFFFFFFF)) noexcept;
        void                 DestroyChildren(void) noexcept;
        _Check_return_ bool   EnumerateChildren(_Inout_ std::size_t& enumerator) const noexcept;
        _Check_return_ bool   GetNextChild(_Inout_ std::size_t& enumerator, _Out_ CExtensibleMarkupLanguageElement*& element_p) const noexcept;
        inline _Check_return_ std::size_t GetNumberOfChildren(void) const noexcept { return(m_Children.size()); };
        _Check_return_ SSIZE_T GetIndexOfChild(_In_ CExtensibleMarkupLanguageElement const* element_p) const noexcept;
        _Check_return_ [[nodiscard]] CExtensibleMarkupLanguageElement* NewChild(_In_ std::wstring_view tag_name = { L"", 0 }, _In_ ElementType const type = ElementType::Element) noexcept;
        _Check_return_ [[nodiscard]] CExtensibleMarkupLanguageElement* NewComment(_In_ std::wstring_view text) noexcept;

        void                 RemoveChild(_Inout_ CExtensibleMarkupLanguageElement* element_p) noexcept;

        // Attributes
        _Check_return_ bool  AddAttribute(_In_ std::wstring_view name, _In_ std::wstring_view value, _In_ uint64_t const name_offset = 0, _In_ uint64_t const value_offset = 0) noexcept;
        void  DestroyAttributeByName(_In_ std::wstring_view name) noexcept;
        void  DestroyAttributeByValue(_In_ std::wstring_view name) noexcept;
        void  DestroyAttributes(void) noexcept;
        _Check_return_ bool  EnumerateAttributes(_Inout_ std::size_t& enumerator) const noexcept;
        _Check_return_ bool  GetAttributeByName(_Out_ CExtensibleMarkupLanguageAttribute& attribute) const noexcept;
        _Check_return_ bool  GetAttributeByName(_In_ std::wstring_view name, _Out_ std::wstring& value) const noexcept;
        _Check_return_ CExtensibleMarkupLanguageAttribute* GetAttributeByName(_In_ std::wstring_view name) const noexcept; // Added by Mats Johnsson 1998-09-02
        _Check_return_ bool   GetAttributeByValue(_Out_ CExtensibleMarkupLanguageAttribute& attribute) const noexcept;
        _Check_return_ bool   GetNextAttribute(_Inout_ std::size_t& enumerator, _Out_ CExtensibleMarkupLanguageAttribute*& attribute) const noexcept;
        inline _Check_return_ std::size_t GetNumberOfAttributes(void) const noexcept { return(m_Attributes.size()); };

        // General being a good C++ citizen kind of things go here

        _Check_return_ bool AddText(_In_ std::wstring_view text_segment) noexcept;
        void  Copy(_In_ CExtensibleMarkupLanguageElement const& source) noexcept;
        _Check_return_ std::size_t CountChildren(_In_ std::wstring_view name) const noexcept;
        void Empty(void) noexcept;
        _Check_return_ CExtensibleMarkupLanguageElement* GetAnyChild(_In_ std::wstring_view name) const noexcept;
        _Check_return_ bool ForAny(_In_ std::wstring_view name, __callback XML_ELEMENT_CALLBACK callback, _Inout_ void* callback_context) const noexcept;
        _Check_return_ bool ForEach(_In_ std::wstring_view name, __callback XML_ELEMENT_CALLBACK callback, _Inout_ void* callback_context) const noexcept;
        inline constexpr _Check_return_ bool GetAbortParsing(void) const noexcept { return(m_AbortParsing); };
        inline constexpr void GetBeginning(_Inout_ CParsePoint& parse_point) const noexcept { parse_point.Copy(m_Beginning); };
        _Check_return_ CExtensibleMarkupLanguageElement* GetChild(_In_ std::wstring_view tag_name) const noexcept;
        _Check_return_ CExtensibleMarkupLanguageElement* GetChild(_In_ std::wstring_view tag_name, _In_ std::size_t const instance) const noexcept;
        _Check_return_ CExtensibleMarkupLanguageElement* GetChild(_In_ std::size_t const index) const noexcept;
        _Check_return_ bool GetChildText(_In_ std::wstring_view tag_name, _Inout_ std::wstring& child_text, _Inout_ CParsePoint& beginning) const noexcept;
        inline constexpr _Check_return_ CExtensibleMarkupLanguageDocument* GetDocument(void) const noexcept { return(m_Document); };
        inline constexpr void GetEnding(_Inout_ CParsePoint& parse_point) const noexcept { parse_point.Copy(m_Ending); };
        void  GetFullyQualifiedName(_Inout_ std::wstring& name) const noexcept;
        inline void  GetContents(_Inout_ std::wstring& contents) const noexcept { contents = m_Contents; };
        _Check_return_ CExtensibleMarkupLanguageElement* GetElementAfterMe(void) const noexcept;
        _Check_return_ int GetInstance(void) const noexcept;
        void  GetNameAndInstance(_Out_ std::wstring& name) const noexcept;
        inline constexpr _Check_return_ CExtensibleMarkupLanguageElement* GetParent(void) const noexcept { return(m_Parent); };
        _Check_return_ CExtensibleMarkupLanguageElement* GetParent(_In_ std::wstring_view name) const noexcept;
        inline void GetTag(_Inout_ std::wstring& tag) const noexcept { tag.assign(m_Tag); };
        inline std::wstring const& Tag(void) const noexcept { return(m_Tag); }
        void  GetText(_Out_ std::wstring& text) const noexcept;
        _Check_return_ uint32_t GetTotalNumberOfChildren(void) const noexcept;
        inline constexpr _Check_return_ ElementType GetType(void) const noexcept { return(m_Type); };
        inline constexpr _Check_return_ bool IsAllWhiteSpace(void) const noexcept { return(m_IsAllWhiteSpace); };
        inline constexpr _Check_return_ bool IsRoot(void) const noexcept { return((m_Parent == nullptr) ? true : false); }
        _Check_return_ bool  Parse(_In_ CParsePoint const& beginning, _In_ CDataParser const& parser) noexcept;
        inline constexpr void SetAbortParsing(_In_ bool const abort_parsing = true) noexcept { m_AbortParsing = abort_parsing; };
        inline void SetContents(_In_ std::wstring_view contents) noexcept { m_Contents.assign(contents); };
        inline void SetDocument(_In_ CExtensibleMarkupLanguageDocument* document_p) noexcept
        {
            m_Document = document_p;

            for (auto const child_p : m_Children)
            {
                child_p->SetDocument(document_p);
            }
        }
        inline void SetTag(_In_ std::wstring_view tag_name) noexcept { m_Tag.assign(tag_name); };
        inline constexpr void SetType(_In_ ElementType const element_type) noexcept
        {
            switch (element_type)
            {
            case ElementType::Unknown:
            case ElementType::ProcessingInstruction:
            case ElementType::Comment:
            case ElementType::CharacterData:
            case ElementType::Element:
            case ElementType::TextSegment:
            case ElementType::MetaData:

                m_Type = element_type;
                break;

            default:

                m_Type = ElementType::Unknown;
            }
        }

        void WriteTo(_Inout_ std::vector<uint8_t>& destination) const noexcept;

        void Trim(void) noexcept;

        _Check_return_ [[nodiscard]] CExtensibleMarkupLanguageElement* NewChildValue(_In_ std::wstring_view tag_name, _In_ uint32_t const value) noexcept;
        _Check_return_ [[nodiscard]] CExtensibleMarkupLanguageElement* NewChildValue(_In_ std::wstring_view tag_name, _In_  int32_t const value) noexcept;
        _Check_return_ [[nodiscard]] CExtensibleMarkupLanguageElement* NewChildValue(_In_ std::wstring_view tag_name, _In_ uint64_t const value) noexcept;
        _Check_return_ [[nodiscard]] CExtensibleMarkupLanguageElement* NewChildValue(_In_ std::wstring_view tag_name, _In_  int64_t const value) noexcept;
        _Check_return_ [[nodiscard]] CExtensibleMarkupLanguageElement* NewChildValue(_In_ std::wstring_view tag_name, _In_ std::wstring_view value) noexcept;
        _Check_return_ [[nodiscard]] CExtensibleMarkupLanguageElement* NewChildValue(_In_ std::wstring_view tag_name, _In_ double const value) noexcept;
        _Check_return_ [[nodiscard]] CExtensibleMarkupLanguageElement* NewChildValue(_In_ std::wstring_view tag_name, _In_ std::vector<uint8_t> const& value) noexcept;
        _Check_return_ [[nodiscard]] CExtensibleMarkupLanguageElement* NewChildValue(_In_ std::wstring_view tag_name, __in_bcount(number_of_bytes) uint8_t const* buffer, _In_ std::size_t const number_of_bytes) noexcept;

        _Check_return_ bool GetChildValue(_In_ std::wstring_view tag_name, _Out_ bool& value) const noexcept;
        _Check_return_ bool GetChildValue(_In_ std::wstring_view tag_name, _Out_ uint32_t& value) const noexcept;
        _Check_return_ bool GetChildValue(_In_ std::wstring_view tag_name, _Out_ uint64_t& value) const noexcept;
        _Check_return_ bool GetChildValue(_In_ std::wstring_view tag_name, _Out_ double& value) const noexcept;
        _Check_return_ bool GetChildValue(_In_ std::wstring_view tag_name, _Out_ std::wstring& value) const noexcept;
        _Check_return_ bool GetChildValue(_In_ std::wstring_view tag_name, _Out_ std::vector<uint8_t>& value) const noexcept;

        inline _Check_return_ CExtensibleMarkupLanguageElement* GetTheFirstChild(void) const noexcept // We can't call this GetFirstChild because that is a Windows macro defined in windowsx.h and will screw up the compile
        {
            if (m_Children.empty() == false)
            {
                return(m_Children[0]);
            }

            return(nullptr);
        }

        _Check_return_ CExtensibleMarkupLanguageElement& operator=(_In_ CExtensibleMarkupLanguageElement const& source) noexcept;
    };

    // Parsing options

#define WFC_XML_IGNORE_CASE_IN_XML_DECLARATION             (0x1)
#define WFC_XML_ALLOW_REPLACEMENT_OF_DEFAULT_ENTITIES      (0x2)
#define WFC_XML_FAIL_ON_ILL_FORMED_ENTITIES                (0x4)
#define WFC_XML_IGNORE_ALL_WHITE_SPACE_ELEMENTS            (0x8)
#define WFC_XML_IGNORE_MISSING_XML_DECLARATION             (0x10)
#define WFC_XML_DISALLOW_MULTIPLE_ELEMENTS                 (0x20)
#define WFC_XML_LOOSE_COMMENT_PARSING                      (0x40)
#define WFC_XML_ALLOW_AMPERSANDS_IN_ELEMENTS               (0x80)
#define WFC_XML_FORCE_AT_LEAST_ONE_ELEMENT_MUST_BE_PRESENT (0x100)
#define WFC_XML_CHECK_ENTITIES_FOR_ILLEGAL_CHARACTERS      (0x200)

// Writing Options
#define WFC_XML_INCLUDE_TYPE_INFORMATION              (0x1)
#define WFC_XML_DONT_OUTPUT_XML_DECLARATION           (0x2)
#define WFC_XML_WRITE_AS_UNICODE                      (0x4)
#define WFC_XML_WRITE_AS_BIG_ENDIAN                   (0x8)
#define WFC_XML_WRITE_AS_UCS4                         (0x10)
#define WFC_XML_WRITE_AS_UCS4_UNUSUAL_2143            (0x20)
#define WFC_XML_WRITE_AS_UCS4_UNUSUAL_3412            (0x40)
#define WFC_XML_WRITE_AS_ASCII                        (0x80)
#define WFC_XML_WRITE_AS_UTF8                         (0x100)
#define WFC_XML_WRITE_AS_UTF7                         (0x200)

    struct XML_ELEMENT_CALLBACK_ENTRY
    {
        std::wstring         name;
        XML_ELEMENT_CALLBACK callback{ nullptr };
        void* parameter{ nullptr };
    };

    class CExtensibleMarkupLanguageDocument
    {
    protected:

        CExtensibleMarkupLanguageElement* m_XML{ nullptr }; // The <?xml ... ?> tag

        std::vector<XML_ELEMENT_CALLBACK_ENTRY> m_Callbacks;

        CExtensibleMarkupLanguageEntities m_Entities;

        bool m_IgnoreWhiteSpace{ false };
        bool m_AutomaticIndentation{ false };

        uint32_t m_IndentationLevel{ 0 };
        uint32_t m_IndentBy{ 0 };
        uint32_t m_ParseOptions{ 0 };
        uint32_t m_WriteOptions{ 0 };

        // Parsing Error Information

        CParsePoint m_ErrorElementBeganAt;
        CParsePoint m_ErrorOccuredAt;
        std::wstring m_ErrorTagName;
        std::wstring m_ErrorMessage;

        // Data from XMLDecl
        std::wstring m_Encoding;
        std::wstring m_Version;
        std::wstring m_Namespace;

        wchar_t m_ParentChildSeparatorCharacter{ '.' };

        uint32_t m_ConversionCodePage{ 0 };
        bool     m_ParseErrorEncountered{ false };
        bool     m_IsStandalone{ true };
        bool     m_UseNamespace{ false };

        void m_InitializeRootElement(void) noexcept;
        void m_InitializeEntities(void) noexcept;
        void m_RemoveAllCallbacks(void) noexcept;

    public:

        std::size_t NumberOfBytesPerCharacter{ 1 };

        CExtensibleMarkupLanguageDocument() noexcept;
        explicit CExtensibleMarkupLanguageDocument(_In_ CExtensibleMarkupLanguageDocument const& source) noexcept;
        virtual ~CExtensibleMarkupLanguageDocument();

        _Check_return_ bool AddEntity(_In_ std::wstring_view entity, _In_ std::wstring_view value) noexcept;
        void Append(_In_ CExtensibleMarkupLanguageDocument const& source) noexcept;
        _Check_return_ bool ContainsElementName(_In_ std::wstring_view name) const noexcept;
        void  Copy(_In_ CExtensibleMarkupLanguageDocument const& source) noexcept;
        void  CopyCallbacks(_In_ CExtensibleMarkupLanguageDocument const& source) noexcept;
        _Check_return_ std::size_t CountElements(_In_ std::wstring_view name) const noexcept;
        void  Empty(void) noexcept;
        _Check_return_ CExtensibleMarkupLanguageElement* GetElement(_In_ std::wstring_view name) const noexcept;
        void  GetAutomaticIndentation(_Out_ bool& automatically_indent, _Out_ uint32_t& level, _Out_ uint32_t& indent_by) const noexcept;
        inline  constexpr _Check_return_ uint32_t GetConversionCodePage(void) const noexcept { return(m_ConversionCodePage); }
        void  GetEncoding(_Out_ std::wstring& encoding) const noexcept;
        inline constexpr _Check_return_ CExtensibleMarkupLanguageEntities const& GetEntities(void) const noexcept { return(m_Entities); }
        inline constexpr _Check_return_ bool GetIgnoreWhiteSpace(void) const noexcept { return(m_IgnoreWhiteSpace); }
        void  GetNamespace(_Out_ std::wstring& name_space) const noexcept;
        _Check_return_ std::size_t GetNumberOfElements(void) const noexcept;
        inline constexpr _Check_return_ wchar_t GetParentChildSeparatorCharacter(void) const noexcept { return(m_ParentChildSeparatorCharacter); }
        inline constexpr _Check_return_ uint32_t GetParseOptions(void) const noexcept { return(m_ParseOptions); }
        void  GetParsingErrorInformation(_Out_ std::wstring& tag_name, _Out_ CParsePoint& beginning, _Out_ CParsePoint& error_location, _Out_opt_ std::wstring* error_message = static_cast<std::wstring*>(nullptr)) const noexcept;
        void  GetParsingErrorInformation(_Out_ std::wstring& message) const noexcept;
        inline constexpr _Check_return_ CExtensibleMarkupLanguageElement* GetRootElement(void) const noexcept { return(m_XML); }
        inline void GetVersion(_Out_ std::wstring& version) const noexcept { version.assign(m_Version); };
        inline constexpr _Check_return_ uint32_t GetWriteOptions(void) const noexcept { return(m_WriteOptions); }
        inline constexpr _Check_return_ bool IsStandalone(void) const noexcept { return(m_IsStandalone); }
        _Check_return_ bool Parse(_Inout_ CDataParser& source) noexcept;
        _Check_return_ bool ResolveEntity(_In_ std::wstring_view entity, _Out_ std::wstring& resolved_to) const noexcept;
        inline constexpr void SetAutomaticIndentation(_In_ bool const automatically_indent = true, _In_ uint32_t const indentation_level = 0, _In_ uint32_t const indent_by = 2) noexcept
        {
            m_AutomaticIndentation = automatically_indent;
            m_IndentationLevel = indentation_level;
            m_IndentBy = indent_by;
        }
        void  SetConversionCodePage(_In_ uint32_t const new_page) noexcept;
        void  SetEncoding(_In_ std::wstring_view encoding) noexcept;
        inline constexpr _Check_return_ bool SetIgnoreWhiteSpace(_In_ bool const ignore_whitespace) noexcept
        {
            bool const return_value = m_IgnoreWhiteSpace;
            m_IgnoreWhiteSpace = ignore_whitespace;
            return(return_value);
        }
        void SetNamespace(_In_ std::wstring_view name_space) noexcept;
        inline constexpr _Check_return_ bool SetParentChildSeparatorCharacter(_In_ wchar_t const separator) noexcept
        {
            if (separator == 0x00)
            {
                return(false);
            }

            m_ParentChildSeparatorCharacter = separator;
        }

        inline constexpr _Check_return_ uint32_t SetParseOptions(_In_ uint32_t const new_options) noexcept
        {
            uint32_t const return_value = m_ParseOptions;

            m_ParseOptions = new_options;

            return(return_value);
        }

        void SetParsingErrorInformation(_In_ std::wstring_view tag_name, _In_ CParsePoint const& beginning, _In_ CParsePoint const& error_location, _In_ std::wstring_view error_message) noexcept;
        inline constexpr void SetStandalone(_In_ bool const standalone) noexcept { m_IsStandalone = standalone; }

        inline constexpr void SetStrictParsing(void) noexcept
        {
            uint32_t options = WFC_XML_FAIL_ON_ILL_FORMED_ENTITIES;

            options or_eq WFC_XML_DISALLOW_MULTIPLE_ELEMENTS;
            options or_eq WFC_XML_FORCE_AT_LEAST_ONE_ELEMENT_MUST_BE_PRESENT;
            options or_eq WFC_XML_CHECK_ENTITIES_FOR_ILLEGAL_CHARACTERS;

            (void)SetParseOptions(options);
        }

        inline void SetVersion(_In_ std::wstring_view version_string) noexcept { m_Version.assign(version_string); }
        _Check_return_ uint32_t SetWriteOptions(_In_ uint32_t const new_options) noexcept;

        inline void Trim(void) noexcept
        {
            if (m_XML not_eq nullptr)
            {
                m_XML->Trim();
            }
        }

        _Check_return_ inline bool UseNamespace(void) const noexcept { return(m_UseNamespace); };
        void  WriteTo(_Out_ std::vector<uint8_t>& destination) const noexcept;

        // Add callback methods here...

        _Check_return_ bool AddCallback(_In_ std::wstring_view element_name, __callback XML_ELEMENT_CALLBACK callback, _Inout_ void* callback_parameter) noexcept;
        _Check_return_ bool EnumerateCallbacks(_Out_ std::size_t& enumerator) const noexcept;
        void               ExecuteCallbacks(_Inout_ CExtensibleMarkupLanguageElement* element_p) const noexcept;
        _Check_return_ bool GetNextCallback(_Inout_ uint32_t& enumerator, _Out_ std::wstring& element_name, _Out_ XML_ELEMENT_CALLBACK& callback, _Out_ void*& parameter) const noexcept;
        _Check_return_ bool RemoveCallback(_In_ std::wstring_view element_name, __callback XML_ELEMENT_CALLBACK callback, _Inout_ void* callback_parameter) noexcept;
        _Check_return_ bool ParseErrorOccurred(void) const noexcept;

        inline _Check_return_ CExtensibleMarkupLanguageDocument& operator=(_In_ CExtensibleMarkupLanguageDocument const& source) noexcept
        {
            Copy(source);
            return(*this);
        }

        inline _Check_return_ CExtensibleMarkupLanguageDocument& operator+=(_In_ CExtensibleMarkupLanguageDocument const& source) noexcept
        {
            Append(source);
            return(*this);
        }
    };
#endif // XML_CLASSES_HEADER
