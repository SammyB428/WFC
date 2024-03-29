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
** $Workfile: CExtensibleMarkupLanguageElement.cpp $
** $Revision: 84 $
** $Modtime: 1/05/02 11:19a $
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

    /*
    ** Thanks go to Mats Johnsson (mats.johnsson@comfact.com) for adding
    ** humanly readable error messages to the works.
    ** 1998-08-17
    */

// All comparison must be case-sensitive. This is in accordance
// with the term "match" in section 1.2 (Terminology) in the spec.
// Yes, this is vague.

Win32FoundationClasses::CExtensibleMarkupLanguageElement::CExtensibleMarkupLanguageElement() noexcept
{
    WFC_VALIDATE_POINTER( this );

    m_AbortParsing                = false;
    m_Document                    = nullptr;
    m_Parent                      = nullptr;
    m_Type                        = ElementType::Unknown;
    m_IsTagTerminated             = false;
    m_IsAllWhiteSpace             = false;
    m_ShorthandTerminatorDetected = false;
    m_Beginning.Empty();
    m_Ending.Empty();
}

Win32FoundationClasses::CExtensibleMarkupLanguageElement::CExtensibleMarkupLanguageElement( _In_ Win32FoundationClasses::CExtensibleMarkupLanguageElement const& source ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    m_AbortParsing                = false;
    m_Document                    = nullptr;
    m_Parent                      = nullptr;
    m_Type                        = ElementType::Unknown;
    m_IsTagTerminated             = false;
    m_IsAllWhiteSpace             = false;
    m_ShorthandTerminatorDetected = false;
    m_Beginning.Empty();
    m_Ending.Empty();
    Copy( source );
}

Win32FoundationClasses::CExtensibleMarkupLanguageElement::~CExtensibleMarkupLanguageElement()
{
    WFC_VALIDATE_POINTER( this );

    Empty();
    m_Document                    = nullptr;
    m_Parent                      = nullptr;
    m_Type                        = ElementType::Unknown;
    m_IsTagTerminated             = false;
    m_IsAllWhiteSpace             = false;
    m_ShorthandTerminatorDetected = false;
    m_Beginning.Empty();
    m_Ending.Empty();
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::AddAttribute(_In_ std::wstring_view name, _In_ std::wstring_view value, _In_ uint64_t const name_offset, _In_ uint64_t const value_offset ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    auto attribute_p{ new Win32FoundationClasses::CExtensibleMarkupLanguageAttribute };

    //WFCTRACEVAL( TEXT( "Adding attribute named " ), name );
    //WFCTRACEVAL( TEXT( "Value is " ), value );

    attribute_p->Name.assign( name );
    attribute_p->Value.assign( value );
    attribute_p->NameOffset = name_offset;
    attribute_p->ValueOffset = value_offset;

    // 1998-09-04
    // Thanks go to Mats Johnsson (mats.johnsson@comfact.com)
    // for pointing out that I wasn't normalizing the attribute
    // values according to section 3.3.3 of XML 1.0 Recommendatation
    // dated 1998-02-10

    // First, we need replace and CARRIAGE_RETURN/LINE_FEED pairs
    // with a single space

    std::wstring temporary_string;

    auto location_of_character{ attribute_p->Value.find(CARRIAGE_RETURN) };

    while( location_of_character not_eq attribute_p->Value.npos)
    {
        // We found a carriage return, let's normalize it.

        attribute_p->Value[ location_of_character ] = ' ';

        if ( location_of_character < attribute_p->Value.length() - 1 )
        {
            // There's at least one more character after the carriage return

            if ( attribute_p->Value.at( location_of_character + 1 ) == LINE_FEED )
            {
                // Yup. It was one of those danged line feeds. Let's delete it

                temporary_string = attribute_p->Value.substr( 0, location_of_character );
                temporary_string.append( right( attribute_p->Value, attribute_p->Value.length() - ( location_of_character + 1 ) ) );

                attribute_p->Value = temporary_string;
            }
        }

        location_of_character = attribute_p->Value.find( CARRIAGE_RETURN );
    }

    // Now that we've gotten rid of all carriage returns (and carriage
    // return/line feed pairs), we need to get rid of all line feeds.

    location_of_character = attribute_p->Value.find( LINE_FEED );

    while( location_of_character not_eq attribute_p->Value.npos)
    {
        // We found a carriage return, let's normalize it.

        attribute_p->Value[ location_of_character ] = ' ';
        location_of_character = attribute_p->Value.find( LINE_FEED );
    }

    // And last but not least, we need to get rid of all TAB characters

    location_of_character = attribute_p->Value.find(TAB_CHARACTER);

    while( location_of_character not_eq attribute_p->Value.npos)
    {
        // We found a carriage return, let's normalize it.

        attribute_p->Value[ location_of_character ] = ' ';
        location_of_character = attribute_p->Value.find(TAB_CHARACTER);
    }

    // 1999-06-21
    // Fixed a couple of (unreported, I found them myself) bugs dealing with
    // well formedness constraints in regards to attributes

    if ( GetAttributeByName( attribute_p->Name ) not_eq nullptr )
    {
        std::wstring parsing_error_message(WSTRING_VIEW(L"Attributes must be unique (Rule 40). An attribute named \""));
 
        parsing_error_message.append(attribute_p->Name);
        parsing_error_message.append(WSTRING_VIEW(L"\" was repeated."));

        m_ReportParsingError( parsing_error_message );

        delete attribute_p;
        attribute_p = nullptr;

        return( false );
    }

    if ( attribute_p->Value.find( '<' ) not_eq attribute_p->Value.npos)
    {
        std::wstring parsing_error_message(WSTRING_VIEW(L"Attribute values cannot contain a '<' (Rule 41). Offending attribute is named named \""));

        parsing_error_message.append(attribute_p->Name);
        parsing_error_message.push_back('\"');
        m_ReportParsingError( parsing_error_message );

        delete attribute_p;
        attribute_p = nullptr;

        return( false );
    }

    // 2003-01-24, Thanks go to Anne Jobmann for finding a bug here.
    // I was not allowing entity references in attributes. Apparently, I mis-read
    // the XML spec.

    auto const index_of_ampersand{ attribute_p->Value.find('&') };

    if ( index_of_ampersand not_eq attribute_p->Value.npos)
    {
        auto const index_of_semicolon{ attribute_p->Value.find(';', index_of_ampersand + 1) };

        if ( index_of_semicolon == attribute_p->Value.npos)
        {
            std::wstring parsing_error_message(WSTRING_VIEW(L"Attribute values cannot contain a '&' (Rule 41). Offending attribute is named \""));
            parsing_error_message.append(attribute_p->Name);
            parsing_error_message.push_back('\"');

            m_ReportParsingError( parsing_error_message );

            delete attribute_p;
            attribute_p = nullptr;

            return( false );
        }

        // A semicolon follows the ampersand. We will assume it to be an entity reference.
    }

    m_Attributes.push_back( attribute_p );

    return( true );
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::AddChild( _Inout_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * item_p, _In_ bool const check_for_uniqueness, _In_ uint32_t const insert_at ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( item_p );
    ASSERT( item_p not_eq this );

    if ( item_p == this )
    {
        // We can't be our own child
        return;
    }

    if ( item_p not_eq nullptr )
    {
        // First, let's make sure we aren't already in there

        auto const number_of_entries{ m_Children.size() };

        if (check_for_uniqueness == true)
        {
           for ( auto const loop_index : Range(number_of_entries))
           {
               if (m_Children[loop_index] == item_p)
               {
                   //WFCTRACE( TEXT( "Element is already a child." ) );
                   // the item is already in the list
                   return;
               }
           }
        }

        // If we get here, it means the item_p ain't already in the list

        item_p->SetDocument( m_Document );
        item_p->m_Parent = this;

        // 2000-03-12
        // New functionality as suggested by Perry Rapp, we now have
        // the ability to add a child at a specific index.

        if ( insert_at >= number_of_entries )
        {
            m_Children.push_back( item_p );
        }
        else
        {
            m_Children.insert( std::begin(m_Children) + insert_at, item_p );
        }
    }
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::AddText(_In_ std::wstring_view text_segment_parameter ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    // Create a child text segment thingy

    auto child_element_p{ NewElement(this, ElementType::TextSegment, m_Document) };

    if ( child_element_p == nullptr )
    {
        return( false );
    }

    // Here's where we encode the default entities

    if (text_segment_parameter.find_first_of(WSTRING_VIEW(L"&\"<>'")) == text_segment_parameter.npos )
    {
        // Nothing to translate
        child_element_p->SetContents( text_segment_parameter );
    }
    else
    {
        std::wstring encoded_entity_string(text_segment_parameter);

        Win32FoundationClasses::replace( encoded_entity_string, WSTRING_VIEW(L"&"), WSTRING_VIEW(L"&amp;"));
        Win32FoundationClasses::replace( encoded_entity_string, WSTRING_VIEW(L">"), WSTRING_VIEW(L"&gt;"));
        Win32FoundationClasses::replace( encoded_entity_string, WSTRING_VIEW(L"<"), WSTRING_VIEW(L"&lt;"));
        Win32FoundationClasses::replace( encoded_entity_string, WSTRING_VIEW(L"\""), WSTRING_VIEW(L"&quot;"));
        Win32FoundationClasses::replace( encoded_entity_string, WSTRING_VIEW(L"'"), WSTRING_VIEW(L"&apos;"));

        child_element_p->SetContents( encoded_entity_string );
    }

    return( true );
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::Copy( _In_ Win32FoundationClasses::CExtensibleMarkupLanguageElement const& source ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    Empty();

    m_Tag                         = source.m_Tag;
    m_Contents                    = source.m_Contents;
    m_ShorthandTerminatorDetected = source.m_ShorthandTerminatorDetected;
    m_IsAllWhiteSpace             = source.m_IsAllWhiteSpace;
    m_IsTagTerminated             = source.m_IsTagTerminated;
    m_Type                        = source.m_Type;

    // Let's copy the attributes

    for( auto const that_attribute_p : source.m_Attributes )
    {
        if ( that_attribute_p not_eq nullptr )
        {
            auto this_attribute_p{ new Win32FoundationClasses::CExtensibleMarkupLanguageAttribute };

            if ( this_attribute_p == nullptr )
            {
                //WFCTRACE( TEXT( "Can't allocate memory for a new attribute" ) );
                Empty();
                return;
            }

            this_attribute_p->Copy( *that_attribute_p );

            m_Attributes.push_back( this_attribute_p );
            this_attribute_p = nullptr;
        }
    }

    // Now let's copy the children

    for( auto const that_item_p : source.m_Children )
    {
        if ( that_item_p not_eq nullptr )
        {
            auto this_item_p{ NewElement(this) };

            if ( this_item_p == nullptr )
            {
                //WFCTRACE( TEXT( "Can't allocate memory for a new child item" ) );
                Empty();
                return;
            }

            this_item_p->Copy( *that_item_p );

            AddChild( this_item_p );
        }
    }
}

_Check_return_ std::size_t Win32FoundationClasses::CExtensibleMarkupLanguageElement::CountChildren(_In_ std::wstring_view name_parameter ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    if (name_parameter.empty() == true)
    {
        return( 0 );
    }

    wchar_t parent_child_separator_character{ '.' };

    if ( m_Document not_eq nullptr )
    {
        parent_child_separator_character = m_Document->GetParentChildSeparatorCharacter();
    }

    if ( name_parameter.find( parent_child_separator_character ) == name_parameter.npos )
    {
        // Count the elements at this level

        // Many thanks go to Johann Plank (johann.plank@atabb.mail.abb.com)
        // for finding a hole in my code. I forgot to write this section!!!

        std::size_t enumerator{ 0 };

        if ( EnumerateChildren( enumerator ) == true )
        {
            std::size_t number_of_children{ 0 };

            Win32FoundationClasses::CExtensibleMarkupLanguageElement * child_p{ nullptr };

            while( GetNextChild( enumerator, child_p ) == true )
            {
                if ( child_p->m_Tag.compare(name_parameter) == I_AM_EQUAL_TO_THAT)
                {
                    number_of_children++;
                }
            }

            return( number_of_children );
        }

        return( 0 );
    }
    else
    {
        // Find the last . in the name, we know there's one in there somewhere

        std::size_t index_of_character{ 0 };
        SSIZE_T loop_index{ static_cast<SSIZE_T>(name_parameter.length()) - 1 };

        while( loop_index >= 0 )
        {
            if (name_parameter[ loop_index ] == parent_child_separator_character )
            {
                index_of_character = loop_index;
                loop_index = 0; // exit_loop
            }

            loop_index--;
        }

        std::wstring parent_name( name_parameter.substr( 0, index_of_character ) );
        std::wstring child_name( right( name_parameter, name_parameter.length() - ( index_of_character + 1 ) ) );

        //WFCTRACEVAL( TEXT( "parent_name is " ), parent_name );
        //WFCTRACEVAL( TEXT( "child_name is " ), child_name );

        // Now that we have the parent and child names, we need
        // to massage the child_name so it doesn't have any [ or ] in it

        index_of_character = child_name.find( '(' );

        if ( index_of_character not_eq child_name.npos )
        {
            // Yup, we've got us a [

            //child_name.assign( child_name.substr( 0, index_of_character ) );
            child_name.erase(std::cbegin(child_name) + index_of_character, std::cend(child_name));
        }

        // Now let's find any )'s (which may have preceeded (

        index_of_character = child_name.find( ')' );

        if ( index_of_character not_eq child_name.npos)
        {
            // Yup, we've got us a (

            //child_name.assign( child_name.substr(0, index_of_character) );
            child_name.erase(std::cbegin(child_name) + index_of_character, std::cend(child_name));
        }

        // We may have chopped off the entire string

        if ( child_name.empty() == true )
        {
            //WFCTRACE( TEXT( "Trimmed everything off" ) );
            return( 0 );
        }

        // Whew! OK, now we need to get that parent

        auto element_p{ GetChild(parent_name) };

        if ( element_p == nullptr )
        {
            //WFCTRACE( TEXT( "Can't find parent" ) );
            return( 0 );
        }

        std::size_t enumerator{ 0 };

        if ( element_p->EnumerateChildren( enumerator ) == true )
        {
            std::size_t number_of_children{ 0 };

            Win32FoundationClasses::CExtensibleMarkupLanguageElement * child_p{ nullptr };

            while( element_p->GetNextChild( enumerator, child_p ) == true )
            {
                if ( child_name.compare( child_p->m_Tag ) == I_AM_EQUAL_TO_THAT)
                {
                    number_of_children++;
                }
            }

            return( number_of_children );
        }
        else
        {
            //WFCTRACE( TEXT( "No children to enumerate" ) );
            return( 0 );
        }
    }

    return( 0 );
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::DeleteElement( _Inout_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * element_p ) noexcept
{
    WFC_VALIDATE_POINTER_NULL_OK( element_p );

    // This destroys the element in whatever manor is appropriate for how NewElement()
    // created it.

    if ( element_p not_eq nullptr )
    {
        delete element_p;
    }
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::DestroyAttributeByName(_In_ std::wstring_view name ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    std::size_t loop_index{ 0 };
    auto number_of_attributes{ m_Attributes.size() };

    while( loop_index < number_of_attributes ) // Cannot be converted to a Range loop
    {
        auto attribute_p{ m_Attributes[loop_index] };

        if ( attribute_p not_eq nullptr )
        {
            if ( attribute_p->Name.compare( name ) == I_AM_EQUAL_TO_THAT)
            {
                // We found one
                m_Attributes.erase( std::begin(m_Attributes) + loop_index );

                delete attribute_p;
                loop_index--;
                number_of_attributes--;
            }
        }

        loop_index++;
    }
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::DestroyAttributeByValue(_In_ std::wstring_view value ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    std::size_t loop_index{ 0 };
    auto number_of_attributes{ m_Attributes.size() };

    while( loop_index < number_of_attributes ) // Cannot be converted to a Range loop
    {
        auto attribute_p{ m_Attributes[loop_index] };

        if ( attribute_p not_eq nullptr )
        {
            if ( attribute_p->Value.compare( value ) == I_AM_EQUAL_TO_THAT)
            {
                // We found one
                m_Attributes.erase( std::begin(m_Attributes) + loop_index );

                delete attribute_p;
                loop_index--;
                number_of_attributes--;
            }
        }

        loop_index++;
    }
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::DestroyAttributes( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    for (auto const attribute_p : m_Attributes)
    {
        if (attribute_p not_eq nullptr)
        {
            delete attribute_p;
        }
    }

    m_Attributes.clear();
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::DestroyChildren( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    std::for_each( std::cbegin(m_Children), std::cend(m_Children), DeleteElement );
    m_Children.clear();
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::Empty( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    m_IsTagTerminated = false;
    DestroyChildren();
    DestroyAttributes();
    m_Tag.clear();
    m_Contents.clear();
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::EnumerateAttributes( _Inout_ std::size_t& enumerator ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    enumerator = 0;

    if ( m_Attributes.empty() == true )
    {
        // There ain't no attributes to enumerate
        return( false );
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::EnumerateChildren( _Inout_ std::size_t& enumerator ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    enumerator = 0;

    if ( m_Children.empty() == true )
    {
        // There ain't no children to enumerate
        return( false );
    }

    return( true );
}

struct FIND_ANY_CHILD_CONTEXT
{
    Win32FoundationClasses::CExtensibleMarkupLanguageElement * child{ nullptr };
};

static void __find_any_child_callback( void * parameter, _Inout_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * element_p ) noexcept
{
    auto context_p{ static_cast<FIND_ANY_CHILD_CONTEXT*>(parameter) };

    if ( context_p not_eq nullptr )
    {
        context_p->child = element_p;
    }
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetAnyChild(_In_ std::wstring_view name ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    FIND_ANY_CHILD_CONTEXT context;

    context.child = nullptr;

    std::ignore = ForAny( name, __find_any_child_callback, &context );

    return( context.child );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::ForAny(_In_ std::wstring_view name, __callback Win32FoundationClasses::XML_ELEMENT_CALLBACK callback, _Inout_ void * callback_context ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( callback_context );

    if ( name.empty() == true )
    {
        // Nothing to do
        return( true );
    }

    std::size_t enumerator{ 0 };

    if (EnumerateChildren(enumerator) == true)
    {
        Win32FoundationClasses::CExtensibleMarkupLanguageElement * child_p{ nullptr };

        bool exit_loop{ false };

        while (exit_loop == false and
            GetNextChild(enumerator, child_p) == true)
        {
            WFC_VALIDATE_POINTER(child_p);

            if (child_p->GetType() == ElementType::Element)
            {
                if (child_p->m_Tag.compare(name) == I_AM_EQUAL_TO_THAT)
                {
                    callback(callback_context, child_p);

                    if (child_p->GetAbortParsing() == true)
                    {
                        child_p->SetAbortParsing(true);
                        exit_loop = true;
                    }
                }
            }

            std::ignore = child_p->ForAny(name, callback, callback_context);
        }
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::ForEach(_In_ std::wstring_view name, __callback Win32FoundationClasses::XML_ELEMENT_CALLBACK callback, _Inout_ void * callback_context ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( callback_context );

    if ( name.empty() == true )
    {
        // Nothing to do
        return( true );
    }

    std::size_t enumerator{ 0 };

    if ( EnumerateChildren( enumerator ) == true)
    {
        Win32FoundationClasses::CExtensibleMarkupLanguageElement * child_p{ nullptr };

        bool exit_loop{ false };

        while( exit_loop == false and
               GetNextChild( enumerator, child_p ) == true)
        {
            WFC_VALIDATE_POINTER( child_p );

            if ( child_p->GetType() == ElementType::Element )
            {
                if ( child_p->m_Tag.compare( name ) == I_AM_EQUAL_TO_THAT)
                {
                    callback( callback_context, child_p );

                    if ( child_p->GetAbortParsing() == true)
                    {
                        child_p->SetAbortParsing( true );
                        exit_loop = true;
                    }
                }
            }
        }
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetAttributeByName( _Out_ Win32FoundationClasses::CExtensibleMarkupLanguageAttribute& attribute ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    attribute.Empty();

    std::size_t enumerator{ 0 };

    if ( EnumerateAttributes( enumerator ) == true)
    {
        Win32FoundationClasses::CExtensibleMarkupLanguageAttribute * this_attribute_p{ nullptr };

        while( GetNextAttribute( enumerator, this_attribute_p ) == true )
        {
            if ( this_attribute_p not_eq nullptr )
            {
                if ( attribute.Name.compare( this_attribute_p->Name ) == I_AM_EQUAL_TO_THAT)
                {
                    attribute.Copy( *this_attribute_p );
                    return( true );
                }
            }
        }
    }

    attribute.Empty();

    return( false );
}

// Suggested by Mats Johnsson (mats.johnsson@comfact.com)

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageAttribute * Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetAttributeByName( _In_ std::wstring_view name ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    std::size_t enumerator{ 0 };

    if ( EnumerateAttributes( enumerator ) == true)
    {
        Win32FoundationClasses::CExtensibleMarkupLanguageAttribute * this_attribute_p{ nullptr };

        while( GetNextAttribute( enumerator, this_attribute_p ) == true )
        {
            if ( this_attribute_p not_eq nullptr )
            {
                if ( this_attribute_p->Name.compare(name) == I_AM_EQUAL_TO_THAT)
                {
                    return( this_attribute_p );
                }
            }
        }
    }

    return( nullptr );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetAttributeByName(_In_ std::wstring_view name, _Out_ std::wstring& value ) const noexcept
{
    WFC_VALIDATE_POINTER(this);

    // Always start from a known state
    value.clear();

    std::size_t enumerator{ 0 };

    if ( EnumerateAttributes( enumerator ) == true)
    {
        Win32FoundationClasses::CExtensibleMarkupLanguageAttribute * this_attribute_p{ nullptr };

        while( GetNextAttribute( enumerator, this_attribute_p ) == true )
        {
            if ( this_attribute_p not_eq nullptr )
            {
                if ( this_attribute_p->Name.compare(name) == I_AM_EQUAL_TO_THAT)
                {
                    value = this_attribute_p->Value;

                    // Now that we have the attribute value, we need to resolve any
                    // entities in it. This according to section 3.3.3 of
                    // the XML 1.0 recommendation (REC-xml-19980210)

                    m_ResolveEntities( value );

                    return( true );
                }
            }
        }
    }

    return( false );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetAttributeByValue( _Out_ Win32FoundationClasses::CExtensibleMarkupLanguageAttribute& attribute ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    attribute.Empty();

    std::size_t enumerator{ 0 };

    if ( EnumerateAttributes( enumerator ) == true)
    {
        Win32FoundationClasses::CExtensibleMarkupLanguageAttribute * this_attribute_p{ nullptr };

        while( GetNextAttribute( enumerator, this_attribute_p ) == true )
        {
            if ( this_attribute_p not_eq nullptr )
            {
                if ( attribute.Value.compare( this_attribute_p->Value ) == I_AM_EQUAL_TO_THAT)
                {
                    attribute.Copy( *this_attribute_p );
                    return( true );
                }
            }
        }
    }

    return( false );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetChildText( _In_ std::wstring_view name, _Inout_ std::wstring& text, _Inout_ CParsePoint& beginning ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    auto child{ GetChild(name) };

    if ( child == nullptr )
    {
        beginning.Empty();
        text.clear();
        return( false );
    }

    child->GetText( text );
    child->GetBeginning( beginning );

    auto const first_child{ child->GetTheFirstChild() };

    if ( first_child not_eq nullptr )
    {
        first_child->GetBeginning( beginning );
    }

    return( true );
}

_Check_return_ SSIZE_T Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetIndexOfChild(_In_ Win32FoundationClasses::CExtensibleMarkupLanguageElement const * child) const noexcept
{
    WFC_VALIDATE_POINTER(this);

    for( auto const child_index : Range(m_Children.size()))
    {
        if (reinterpret_cast<uint64_t>(m_Children[child_index]) == reinterpret_cast<uint64_t>(child))
        {
            return(child_index);
        }
    }

    return(-1);
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetChild(_In_ std::size_t const index) const noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (index >= m_Children.size())
    {
        return(nullptr);
    }

    return(m_Children[index]);
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetChild( _In_ std::wstring_view name ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    std::wstring parent_name;
    std::wstring child_name;

    uint32_t desired_instance_number{ 0 };

    if ( m_ParseElementName( name, parent_name, desired_instance_number, child_name ) == false )
    {
        //WFCTRACEVAL( TEXT( "Can't parse element name " ), name );
        return( nullptr );
    }

    //WFCTRACEVAL( TEXT( "desired parent_name is " ), parent_name );
    //WFCTRACEVAL( TEXT( "desired_instance_number is " ), desired_instance_number );

    uint32_t this_instance_number{ 0 };

    for ( auto const loop_index : Range(m_Children.size()) )
    {
        auto element_p{ m_Children[loop_index] };

        if ( element_p not_eq nullptr )
        {
            if ( parent_name.compare( element_p->m_Tag ) == I_AM_EQUAL_TO_THAT)
            {
                if ( this_instance_number == desired_instance_number )
                {
                    // YIPPEE!! We found our instance! Let's see if this is the
                    // ultimate element we're searching for or if we are a parent
                    // searching for a child

                    if ( child_name.empty() == true )
                    {
                        //WFCTRACEVAL( TEXT( "We are the one! " ), parent_name );
                        // We are the one!
                        return( element_p );
                    }
                    else
                    {
                        //WFCTRACEVAL( TEXT( "Contiuing the search " ), parent_name );
                        // Nope, we are not the Key Master, continue the search
                        // further down the tree

                        return( element_p->GetChild( child_name ) );
                    }
                }
                else
                {
                    //WFCTRACE( TEXT( "Wrong instance" ) );
                }

                this_instance_number++;
            }
        }
        else
        {
            //WFCTRACE( TEXT( "Element is zero!" ) );
        }
    }

    return( nullptr );
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetChild(_In_ std::wstring_view name, _In_ std::size_t const desired_instance_number) const noexcept
{
    WFC_VALIDATE_POINTER(this);

    uint32_t this_instance_number{ 0 };

    for (auto const loop_index : Range(m_Children.size()))
    {
        auto element_p{ m_Children[loop_index] };

        if (element_p not_eq nullptr)
        {
            if (element_p->m_Tag.compare(name) == I_AM_EQUAL_TO_THAT)
            {
                if (this_instance_number == desired_instance_number)
                {
                   return(element_p);
                }

                this_instance_number++;
            }
        }
    }

    return(nullptr);
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetFullyQualifiedName( _Inout_ std::wstring& name ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    name.clear();

    if ( IsRoot() == true or m_Tag.empty() == true )
    {
        // The root is nameless
        return;
    }

    GetNameAndInstance( name );

    std::wstring parent_name;

    wchar_t parent_child_separator_character{ '.' };

    if ( m_Document not_eq nullptr )
    {
        parent_child_separator_character = m_Document->GetParentChildSeparatorCharacter();
    }

    auto parent_element_p{ GetParent() };

    while( parent_element_p not_eq nullptr )
    {
        parent_element_p->GetNameAndInstance( parent_name );

        if ( parent_element_p->IsRoot() == true or parent_name.empty() == true )
        {
            return;
        }

        parent_name.push_back( parent_child_separator_character );
        parent_name.append( name );
        name = parent_name;

        parent_element_p = parent_element_p->GetParent();
    }
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetElementAfterMe(void) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    auto parent_element_p{ GetParent() };

    std::size_t enumerator{ 0 };

    if ( parent_element_p->EnumerateChildren( enumerator ) == true)
    {
        Win32FoundationClasses::CExtensibleMarkupLanguageElement * child_element_p{ nullptr };

        while( parent_element_p->GetNextChild( enumerator, child_element_p ) == true )
        {
            if ( child_element_p == this )
            {
                // We've found ourselves!

                // Now continue the enumeration until we reach a typeElement element
                while( parent_element_p->GetNextChild( enumerator, child_element_p ) == true )
                {
                    if ( child_element_p not_eq nullptr )
                    {
                        if ( child_element_p->GetType() == ElementType::Element )
                        {
                            return( child_element_p );
                        }
                    }
                }
            }
        }
    }

    return( nullptr );
}

_Check_return_ int Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetInstance( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    auto parent_element_p{ GetParent() };

    std::size_t enumerator{ 0 };

    if ( parent_element_p->EnumerateChildren( enumerator ) == true)
    {
        std::size_t instance_number{ 0 };

        Win32FoundationClasses::CExtensibleMarkupLanguageElement * child_element_p{ nullptr };

        while( parent_element_p->GetNextChild( enumerator, child_element_p ) == true )
        {
            if ( child_element_p == this )
            {
                // We've found ourselves

                return( static_cast<int>(instance_number) );
            }

            // Let's see if this is an instance of us

            if ( m_Tag.compare(child_element_p->Tag()) == I_AM_EQUAL_TO_THAT)
            {
                // We have the same name, let's increment our count
                instance_number++;
            }
        }
    }

    return( 0 );
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetNameAndInstance( _Out_ std::wstring& name ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    name.clear();

    if ( IsRoot() == true or m_Tag.empty() == true )
    {
        // The root is nameless
        return;
    }

    GetTag( name );

    // Now let's find which instance we are

    auto const instance_number{ GetInstance() };

    if ( instance_number > 0 )
    {
        name.push_back('(');
        name.append(std::to_wstring(instance_number));
        name.push_back(')');
    }
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetNextAttribute( _Inout_ std::size_t& enumerator, _Out_ Win32FoundationClasses::CExtensibleMarkupLanguageAttribute*& attribute_p ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_Attributes.empty() == true )
    {
        attribute_p = nullptr;
        return( false );
    }

    if ( enumerator > static_cast< uint32_t >( m_Attributes.size() - 1 ) )
    {
        attribute_p = nullptr;
        return( false );
    }

    attribute_p = m_Attributes[ enumerator ];

    enumerator++;

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetNextChild( _Inout_ std::size_t& enumerator, _Out_ Win32FoundationClasses::CExtensibleMarkupLanguageElement*& item_p ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    // 1999-06-07
    // Thanks go to Ryan Swan (rswan@nj.navisys.com) for finding a bug here.
    // If m_Children is empty, I thought there was 4 billion children. DOH!

    if ( m_Children.empty() == true )
    {
        item_p = nullptr;
        return( false );
    }

    if ( enumerator > static_cast< uint32_t >( m_Children.size() - 1 ) )
    {
        item_p = nullptr;
        return( false );
    }

    item_p = m_Children[ enumerator ];

    enumerator++;

    return( true );
}

// Suggested by Mats Johnsson (mats.johnsson@comfact.com)

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetParent( _In_ std::wstring_view name ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( m_Parent );

    if ( m_Parent not_eq nullptr )
    {
        if ( name.compare( m_Parent->m_Tag ) == I_AM_EQUAL_TO_THAT)
        {
            return( m_Parent );
        }
        else
        {
            //WFCTRACEVAL( TEXT( "Continuing the search " ), name );
            return( m_Parent->GetParent( name ) );
        }
    }

    return( nullptr );
}

static inline void __convert_from_utf8(_Inout_ std::wstring& s) noexcept
{
    // This method scans the incoming wstring for utf8-chars. If any are found, it
    // will thin then convert them.

    // The conversion will only take place if there are characters above 127 and below 256
    // If any characters are above 256, it means Unicode is already present.

    bool conversion_needed{ false };

    for (auto const character : s)
    {
        if (character > 255)
        {
            // Unicode is present, conversion not possible
            return;
        }

        if (character > 127)
        {
            conversion_needed = true;
            break;
        }
    }

    if (conversion_needed == false)
    {
        return;
    }

    // Now thin the string

    std::string thin_string;

    for (auto const character : s)
    {
        thin_string.push_back(static_cast<char>(character));
    }

    Win32FoundationClasses::copy_utf8(s, thin_string.c_str());
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetText( _Out_ std::wstring& text ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( m_Document );

    // Always start with an empty string

    text.clear();

    // Retrieves the contents and text segments

    if ( m_Document->GetIgnoreWhiteSpace() == true)
    {
        // We should ignore white space

        if ( m_IsAllWhiteSpace == false and
             m_Type not_eq ElementType::Comment and
             m_Type not_eq ElementType::ProcessingInstruction and
             m_Type not_eq ElementType::Unknown and
             m_Type not_eq ElementType::MetaData)
        {
            text = m_Contents;
        }
    }
    else
    {
        _ASSERTE(m_Type not_eq ElementType::Unknown);

        // We shouldn't ignore white space
        if (m_Type not_eq ElementType::Comment and
            m_Type not_eq ElementType::ProcessingInstruction and
            m_Type not_eq ElementType::Unknown and
            m_Type not_eq ElementType::MetaData)
        {
            text = m_Contents;
        }
    }

    // Here's where it begins to get fun. If we are
    // a textSegment, then we need to translate the entities.

    if ( m_Type == ElementType::TextSegment )
    {
        __convert_from_utf8(text);
        m_ResolveEntities( text );
    }

    // Now get all of the text of our children

    for (auto const child : m_Children)
    {
        if (child not_eq nullptr)
        {
            _ASSERTE(child->GetType() not_eq ElementType::Unknown);

            if (child->GetType() not_eq ElementType::Comment and
                child->GetType() not_eq ElementType::ProcessingInstruction and
                child->GetType() not_eq ElementType::Unknown and
                child->GetType() not_eq ElementType::MetaData)
            {
                std::wstring text_segment;

                child->GetText(text_segment);
                text.append(text_segment);
            }
        }
    }
}

_Check_return_ uint32_t Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetTotalNumberOfChildren( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    uint32_t return_value{ 0 };

    for ( auto const loop_index : Range(m_Children.size()) )
    {
        auto const element_p{ m_Children[loop_index] };

        if ( element_p not_eq nullptr )
        {
            return_value += element_p->GetTotalNumberOfChildren();
        }

        return_value++; // Don't forget to count this child
    }

    return( return_value );
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_AddCharacterToOutput( _In_ uint32_t const character, _In_ uint32_t const options, _Inout_ std::vector<uint8_t>& output ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( options );

    if ( is_flagged(options, WFC_XML_WRITE_AS_ASCII) == true)
    {
#if defined( _DEBUG )
        if ( character > 255 )
        {
            ASSERT( false );
        }
#endif // _DEBUG
        output.push_back( LOBYTE( LOWORD( character ) ) );
    }
    else if (is_flagged(options, WFC_XML_WRITE_AS_UNICODE) == true)
    {
        if (is_flagged(options, WFC_XML_WRITE_AS_BIG_ENDIAN) == true)
        {
            output.push_back( HIBYTE( LOWORD( character ) ) );
            output.push_back( LOBYTE( LOWORD( character ) ) );
        }
        else
        {
            output.push_back( LOBYTE( LOWORD( character ) ) );
            output.push_back( HIBYTE( LOWORD( character ) ) );
        }
    }
    else if (is_flagged(options, WFC_XML_WRITE_AS_UCS4) == true)
    {
        if (is_flagged(options, WFC_XML_WRITE_AS_BIG_ENDIAN) == true)
        {
            // 1234
            output.push_back( HIBYTE( HIWORD( character ) ) );
            output.push_back( LOBYTE( HIWORD( character ) ) );
            output.push_back( HIBYTE( LOWORD( character ) ) );
            output.push_back( LOBYTE( LOWORD( character ) ) );
        }
        else
        {
            // 4321
            output.push_back( LOBYTE( LOWORD( character ) ) );
            output.push_back( HIBYTE( LOWORD( character ) ) );
            output.push_back( LOBYTE( HIWORD( character ) ) );
            output.push_back( HIBYTE( HIWORD( character ) ) );
        }
    }
    else if ( is_flagged(options, WFC_XML_WRITE_AS_UCS4_UNUSUAL_2143) == true)
    {
        output.push_back( LOBYTE( HIWORD( character ) ) );
        output.push_back( HIBYTE( HIWORD( character ) ) );
        output.push_back( LOBYTE( LOWORD( character ) ) );
        output.push_back( HIBYTE( LOWORD( character ) ) );
    }
    else if ( is_flagged( options, WFC_XML_WRITE_AS_UCS4_UNUSUAL_3412) == true)
    {
        output.push_back( HIBYTE( LOWORD( character ) ) );
        output.push_back( LOBYTE( LOWORD( character ) ) );
        output.push_back( HIBYTE( HIWORD( character ) ) );
        output.push_back( LOBYTE( HIWORD( character ) ) );
    }
    else if (is_flagged(options, WFC_XML_WRITE_AS_UTF8 ) == true or
             is_flagged(options, WFC_XML_WRITE_AS_UTF7 ) == true )
    {
        // This is by far the slowest method

        ASSERT( character <= 0xFFFF ); // Make sure it will fit

        wchar_t string[ 2 ];

        string[ 0 ] = static_cast<wchar_t>(character);
        string[ 1 ] = 0x00;

#if ! defined(WE_ARE_BUILDING_WFC_ON_UNIX)
        uint8_t output_buffer[ 32 ];

        int number_of_bytes_written{ 0 };

        if ( m_Document not_eq nullptr )
        {
            number_of_bytes_written = ::WideCharToMultiByte( m_Document->GetConversionCodePage(), 0, string, 1, reinterpret_cast<char *>(output_buffer), 31, nullptr, nullptr );
        }
        else
        {
            // Hack for Windows 95 which doesn't support CP_UTF8
            number_of_bytes_written = ::WideCharToMultiByte( CP_ACP, 0, string, 1, reinterpret_cast<char *>(output_buffer), 31, nullptr, nullptr );
        }

        if ( number_of_bytes_written > 0 )
        {
            for ( auto const loop_index : Range(number_of_bytes_written) )
            {
                output.push_back( output_buffer[ loop_index ] );
            }
        }
        else
        {
            ASSERT( false );
        }
#else // WE_ARE_BUILDING_WFC_ON_UNIX
        // Unix verstion

        auto converted_text{ to_utf8(string) };

        for (auto const converted_character : converted_text)
        {
            output.push_back(static_cast<uint8_t>(converted_character));
        }

#endif //WE_ARE_BUILDING_WFC_ON_UNIX
    }
    else
    {
        // An invalid WriteOptions value has been used.
        ASSERT( false );
    }
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_AddIndentation( _Inout_ std::vector<uint8_t>& bytes ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    uint32_t write_options{ WFC_XML_WRITE_AS_ASCII };

    if ( m_Document not_eq nullptr )
    {
        write_options = m_Document->GetWriteOptions();
    }
    else
    {
        return;
    }

    bool automatically_indent{ false };

    uint32_t indentation_level{ 0 };
    uint32_t indent_by{ 0 };

    m_Document->GetAutomaticIndentation( automatically_indent, indentation_level, indent_by );

    if ( automatically_indent == true)
    {
        m_AddCharacterToOutput( CARRIAGE_RETURN, write_options, bytes );
        m_AddCharacterToOutput( LINE_FEED,       write_options, bytes );

        indent_by = 0;

        while( indent_by < indentation_level ) // Cannot be converted to a Range loop
        {
            m_AddCharacterToOutput( ' ', write_options, bytes );
            indent_by++;
        }
    }
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_AppendAttributes( _Inout_ std::vector<uint8_t>& data ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    uint32_t write_options{ WFC_XML_WRITE_AS_ASCII };

    if ( m_Document not_eq nullptr )
    {
        write_options = m_Document->GetWriteOptions();
    }

    for ( auto const attribute_index : Range(m_Attributes.size()) )
    {
        auto attribute_p{ m_Attributes[attribute_index] };

        if ( attribute_p not_eq nullptr )
        {
            auto const string_length{ attribute_p->Name.length() };

            if ( string_length > 0 )
            {
                m_AddCharacterToOutput( ' ', write_options, data );
            }

            for ( auto const string_index : Range(string_length) )
            {
#if ! defined( UNICODE )
                m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( attribute_p->Name.GetAt( string_index ) ) ), write_options, data );
#else
                m_AddCharacterToOutput( static_cast< uint32_t >( attribute_p->Name[ string_index ] ), write_options, data );
#endif
            }

            m_AddCharacterToOutput( '=', write_options, data );

            bool we_need_single_quotes{ false };

            if ( attribute_p->Value.find( '\"' ) == attribute_p->Value.npos )
            {
                m_AddCharacterToOutput( '\"', write_options, data );
            }
            else
            {
                we_need_single_quotes = true;
                m_AddCharacterToOutput( '\'', write_options, data );
            }

            for ( auto const string_index : Range(attribute_p->Value.length()) )
            {
#if ! defined( UNICODE )
                m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( attribute_p->Value.GetAt( string_index ) ) ), write_options, data );
#else
                m_AddCharacterToOutput( static_cast< uint32_t >( attribute_p->Value.at( string_index ) ), write_options, data );
#endif
            }

            if ( we_need_single_quotes == false )
            {
                m_AddCharacterToOutput( '\"', write_options, data );
            }
            else
            {
                m_AddCharacterToOutput( '\'', write_options, data );
            }
        }
    }
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_AppendChildren( _Inout_ std::vector<uint8_t>& data ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    for( auto const element_p : m_Children )
    {
        if ( element_p not_eq nullptr )
        {
            element_p->WriteTo( data );
        }
    }
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_DecrementIndentation( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( m_Document );

    bool automatically_indent{ false };

    uint32_t indentation_level{ 0 };
    uint32_t indent_by{ 0 };

    m_Document->GetAutomaticIndentation( automatically_indent, indentation_level, indent_by );

    if ( automatically_indent == true)
    {
        indentation_level -= indent_by;

        m_Document->SetAutomaticIndentation( automatically_indent, indentation_level, indent_by );
    }
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_GetTag( _In_ std::wstring_view xml_data, _Inout_ std::wstring& tag ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    tag.clear();

    std::size_t loop_index{ 0 };

    while( loop_index < xml_data.length() ) // Cannot be converted to a Range loop
    {
        if ( xml_data.at( loop_index ) == '<' )
        {
            loop_index++;

            while( loop_index < xml_data.length() ) // Cannot be converted to a Range loop
            {
                if ( xml_data.at( loop_index ) == '>' )
                {
                    return;
                }

                tag.push_back( xml_data.at( loop_index ) );

                loop_index++;
            }
        }

        loop_index++;
    }

    // if we get here, it means something is wrong (i.e. no closing > character )

    tag.clear();
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_IncrementIndentation( void ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( m_Document );

    bool automatically_indent{ false };

    uint32_t indentation_level{ 0 };
    uint32_t indent_by{ 0 };

    m_Document->GetAutomaticIndentation( automatically_indent, indentation_level, indent_by );

    if ( automatically_indent == true)
    {
        indentation_level += indent_by;

        m_Document->SetAutomaticIndentation( automatically_indent, indentation_level, indent_by );
    }
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_ParseCDATASection( _In_ std::wstring_view tag, _In_ CDataParser const& parser ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    // When we get here, we should have "<![CDATA[" some characters and a ">" in the tag parameter.
    // the trailing ">" may or may not be part of the terminator for "<![CDATA["

    // CDATA sections are a hack. What rookie garbage. They are delimited by ]]>
    // we must assume that ]]> will escape ]]> occuring in the data (although
    // the XML specification doesn't clearly state this if it states it all).

    // We need the CDataParser because triggering on > (which caused m_ParseTag to
    // be called) may have been seen in the CDATA section

    // We should be sitting on a CDATA section now.

    if ( tag.length() < 10 )
    {
        return( false );
    }

    // We have to do this checking all over again because we are a virtual function.
    // We cannot trust that someone who inherited from CExtensibleMarkupLanguageElement
    // knows what they are doing.

    uint32_t character{ 0x00 };

    if ( starts_with(tag, WSTRING_VIEW(L"<![CDATA[") ) == false)
    {
        //WFCTRACEVAL( TEXT( "tag does not begin with \"<![CDATA[\" it begins with " ), temp_string );
        return( false );
    }

    m_Type = ElementType::CharacterData;

    std::wstring temp_string(tag.substr(0, 9));
    m_Tag.assign( right( temp_string, 7 ) ); // Record the tag
    m_Contents.clear();

    // Now we need to see if the end of this string contains the ]]> tag
    // which may or may not signify the end of the cdata section

    temp_string.assign( right( tag, 3 ) );

    if ( temp_string.compare(WSTRING_VIEW(L"]]>")) == I_AM_EQUAL_TO_THAT)
    {
        // This may be the end...

        if ( parser.PeekCharacter( m_Ending, 0 ) == ']' and
            parser.PeekCharacter( m_Ending, 1 ) == ']' and
            parser.PeekCharacter( m_Ending, 2 ) == '>' )
        {
            //WFCTRACE( TEXT( "Escape sequence detected!" ) );

            m_Contents.append(WSTRING_VIEW( L"]]>" )); // Add the escaped characters

            bool exit_loop{ false };

            while( exit_loop == false )
            {
                if ( m_Ending.GetIndex() >= parser.GetSize() )
                {
                    //WFCTRACE( TEXT( "We ran out of room!" ) );
                    return( false );
                }

                character = parser.PeekCharacter( m_Ending, 0 );

                if ( character == ']' )
                {
                    // DANG! It might be the terminator

                    if ( parser.PeekCharacter( m_Ending, 1 ) == ']' and
                        parser.PeekCharacter( m_Ending, 2 ) == '>' )
                    {
                        // OK, we've got a terminator, let's see if it is an escape sequence

                        if ( parser.PeekCharacter( m_Ending, 3 ) == ']' and
                             parser.PeekCharacter( m_Ending, 4 ) == ']' and
                             parser.PeekCharacter( m_Ending, 5 ) == '>' )
                        {
                            // Yup, it is another escape sequence
                            // Let's skip the next 5 characters

                            m_Contents.append(WSTRING_VIEW(L"]]"));

                            parser.AdvanceByOneCharacter( m_Ending, character );
                            parser.AdvanceByOneCharacter( m_Ending, ']' );
                            parser.AdvanceByOneCharacter( m_Ending, '>' );
                            parser.AdvanceByOneCharacter( m_Ending, ']' );
                            parser.AdvanceByOneCharacter( m_Ending, ']' );
                            character = '>';
                        }
                        else
                        {
                            // Well whaddya know, we found The Terminitor!
                            parser.AdvanceByOneCharacter( m_Ending, ']' );
                            parser.AdvanceByOneCharacter( m_Ending, '>' );
                            return( true );
                        }

                        m_Contents.push_back( static_cast< wchar_t >( character ) );
                        parser.AdvanceByOneCharacter( m_Ending, character );
                    }
                }
                else
                {
                    // This is a safe character to add
                    m_Contents.push_back( static_cast< wchar_t >( character ) );
                }
            }
        }
        else
        {
            // Normally terminated

            // Thanks to Stephane Drouin (stephane.drouin@inference.com)
            // for finding an off-by-one error here. I was subtracting
            // 11 when it should have been 12.
            // 1998-07-15

            m_Contents.assign( tag.substr(9, tag.length() - 12 ) );
            return( true );
        }
    }
    else
    {
        // Nope, no such luck, add this text to the m_Contents and search for the true ending
        m_Contents.assign( right( tag, tag.length() - 9 ) ); // Everything except <![CDATA
        bool exit_loop{ false };

        while( exit_loop == false )
        {
            if ( m_Ending.GetIndex() >= parser.GetSize() )
            {
                //WFCTRACE( TEXT( "We ran out of room!" ) );
                return( false );
            }

            character = parser.PeekCharacter( m_Ending, 0 );

            if ( character == ']' )
            {
                // DANG! It might be the terminator

                if ( parser.PeekCharacter( m_Ending, 1 ) == ']' and
                    parser.PeekCharacter( m_Ending, 2 ) == '>' )
                {
                    // OK, we've got a terminator, let's see if it is an escape sequence

                    if ( parser.PeekCharacter( m_Ending, 3 ) == ']' and
                         parser.PeekCharacter( m_Ending, 4 ) == ']' and
                         parser.PeekCharacter( m_Ending, 5 ) == '>' )
                    {
                        // Yup, it is another escape sequence
                        // Let's skip the next 5 characters

                        m_Contents.append(WSTRING_VIEW(L"]]"));

                        parser.AdvanceByOneCharacter( m_Ending, character );
                        parser.AdvanceByOneCharacter( m_Ending, ']' );
                        parser.AdvanceByOneCharacter( m_Ending, '>' );
                        parser.AdvanceByOneCharacter( m_Ending, ']' );
                        character = '>';
                    }
                    else
                    {
                        // Well whaddya know, we found The Terminitor!
                        parser.AdvanceByOneCharacter( m_Ending, ']' );
                        parser.AdvanceByOneCharacter( m_Ending, '>' );
                        return( true );
                    }

                    m_Contents.push_back( static_cast< wchar_t >( character ) );

                    parser.AdvanceByOneCharacter( m_Ending, character );
                }
                else
                {
                    // 1998-12-02, bug fix for release 38
                    // I found that trailing ]]]]'s weren't properly added, DOH!
                    // At least, for once, I found a bug in WFC before it being
                    // reported to me.

                    m_Contents.push_back( static_cast< wchar_t >( character ) );
                }
            }
            else
            {
                // This is a safe character to add
                m_Contents.push_back( static_cast< wchar_t >( character ) );
            }

            parser.AdvanceByOneCharacter( m_Ending, character );
        }
    }

    character = parser.PeekCharacter( m_Ending, 0 );

    if ( character not_eq '<' )
    {
        return( false );
    }

    parser.AdvanceByOneCharacter( m_Ending, character );

    character = parser.PeekCharacter( m_Ending, 0 );

    if ( character not_eq '!' )
    {
        return( false );
    }

    parser.AdvanceByOneCharacter( m_Ending, character );

    character = parser.PeekCharacter( m_Ending, 0 );

    if ( character not_eq '[' )
    {
        return( false );
    }

    parser.AdvanceByOneCharacter( m_Ending, character );

    character = parser.PeekCharacter( m_Ending, 0 );

    if ( character not_eq 'C' )
    {
        return( false );
    }

    parser.AdvanceByOneCharacter( m_Ending, character );

    character = parser.PeekCharacter( m_Ending, 0 );

    if ( character not_eq 'D' )
    {
        return( false );
    }

    parser.AdvanceByOneCharacter( m_Ending, character );

    character = parser.PeekCharacter( m_Ending, 0 );

    if ( character not_eq 'A' )
    {
        return( false );
    }

    parser.AdvanceByOneCharacter( m_Ending, character );

    character = parser.PeekCharacter( m_Ending, 0 );

    if ( character not_eq 'T' )
    {
        return( false );
    }

    parser.AdvanceByOneCharacter( m_Ending, character );

    character = parser.PeekCharacter( m_Ending, 0 );

    if ( character not_eq 'A' )
    {
        return( false );
    }

    parser.AdvanceByOneCharacter( m_Ending, character );

    character = parser.PeekCharacter( m_Ending, 0 );

    if ( character not_eq '[' )
    {
        return( false );
    }

    parser.AdvanceByOneCharacter( m_Ending, character );

    return( false );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_ParseDOCTYPESection( _In_ std::wstring_view tag, _In_ CDataParser const& parser ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    // IDEA: Convert all parsing to CParsePoint's!!!!

    // The crappy thing about DOCTYPE sections is they don't follow XML syntax.
    // They are leftovers from SGML (i.e. the obfuscation layer of XML) days.

    // The tag parameter contains a terminating > character, however, we don't
    // know if there are any child elements yet.

    ASSERT( IsRoot() == false );

    // First, strip off the DOCTYPE characters

    std::wstring string_to_parse{ right(tag, tag.length() - 9) };

    trim_left( string_to_parse);

    //WFCTRACEVAL( TEXT( "Now parsing " ), string_to_parse );

    // We should now be sitting at the Name part of the doctypedecl

    if ( string_to_parse.length() < 1 )
    {
        ASSERT( false );
        m_ReportParsingError(WSTRING_VIEW(L"DOCTYPE Section, not enough room to parse Name (Rule 28)."));
        return( false );
    }

    uint32_t character{ 0 };

#if ! defined( UNICODE )
    // ASCII Build

    character = static_cast< uint32_t >( static_cast< BYTE >( string_to_parse.GetAt( 0 ) ) );

#else

    // UNICODE build

    character = string_to_parse.at( 0 );

#endif // UNICODE

    // Test the first character of the name for legality (according to Rule 5)

    if ( Win32FoundationClasses::is_xml_Letter( character ) == false )
    {
        // the first character can also be a _ or a :

        if ( character not_eq '_' and character not_eq ':' )
        {
            m_ReportParsingError(WSTRING_VIEW(L"DOCTYPE Section, illegal first character of Name (Rule 5)."));
            return( false );
        }
    }

    // Name sections can be terminated by a space or a [

    auto location_of_character{ string_to_parse.find_first_of(WSTRING_VIEW(L" \t\r\n[")) };

    if ( location_of_character == string_to_parse.npos )
    {
        // 1999-09-01
        // Thanks go to Kim Winther Jorgensen <kwj@dia.dk> for finding
        // a bug here. I was not properly parsing simple DOCTYPE declarations

        // Let's see if we have a simplistic DOCTYPE

        location_of_character = string_to_parse.find( '>' );

        if ( location_of_character == string_to_parse.npos )
        {
            m_ReportParsingError(WSTRING_VIEW(L"Illegal DOCTYPE name terminator (Rule 28)."));
            return( false );
        }
    }

    std::wstring name( string_to_parse.substr( 0, location_of_character ) );

    trim_right(name);
    //WFCTRACEVAL( TEXT( "Name is " ), name );

    // Now let's validate name as being valid characters

    for ( auto const loop_index : Range(name.length(), StartingRangePosition(1)) )
    {
#if ! defined( UNICODE )
        // ASCII Build

        character = static_cast< uint32_t >( static_cast< BYTE >( name.at( loop_index ) ) );

#else

        // UNICODE build

        character = name.at( loop_index );

#endif // UNICODE

        if ( Win32FoundationClasses::is_xml_NameChar( character ) == false )
        {
            m_ReportParsingError(WSTRING_VIEW(L"Illegal DOCTYPE name character (Rule 28)."));
            return( false );
        }
    }

    // Is the last character of the name a '['

    // Keep contents for later writing

    m_Contents.assign(WSTRING_VIEW(L"DOCTYPE "));
    m_Contents.append( name );

    string_to_parse.erase(0, name.length());
    trim_left(string_to_parse);

    //WFCTRACEVAL( TEXT( "Now parsing " ), string_to_parse );

    if ( string_to_parse.compare(WSTRING_VIEW(L">")) == I_AM_EQUAL_TO_THAT)
    {
        // We're done
        return( true );
    }

    // The next character can be one of three possibilites, [, S or P

    if ( string_to_parse.at( 0 ) not_eq '[' and
         string_to_parse.at( 0 ) not_eq 'S' and
         string_to_parse.at( 0 ) not_eq 'P' )
    {
        m_ReportParsingError(WSTRING_VIEW(L"Impossible character in DOCTYPE declaration (Rule 28)."));
        return( false );
    }

    // If we get here, we know the next thing is either SYSTEM, PUBLIC or [

    if ( string_to_parse.at( 0 ) not_eq '[' )
    {
        if ( string_to_parse.length() < 6 )
        {
            m_ReportParsingError(WSTRING_VIEW(L"No more room to parse SYSTEM or PUBLIC (Rule 75)."));
            return( false );
        }

        if ( starts_with( string_to_parse, WSTRING_VIEW(L"PUBLIC") ) == false and
             starts_with( string_to_parse, WSTRING_VIEW(L"SYSTEM") ) == false)
        {
            m_ReportParsingError(WSTRING_VIEW(L"DOCTYPE ExternalID is not SYSTEM or PUBLIC (Rule 75)."));
            return( false );
        }

        bool is_public{ false };

        if ( string_to_parse.at( 0 ) == 'P' )
        {
            is_public = true;
        }

        if ( is_public == true)
        {
            // If it is PUBLIC, there should be two quoted strings following it (Rule 75)

            //WFCTRACE( TEXT( "Parsing PUBLIC" ) );

            m_Contents.append(WSTRING_VIEW(L" PUBLIC "));

            string_to_parse.erase(0, 6);
            trim_left(string_to_parse);

            //WFCTRACEVAL( TEXT( "Would now parse " ), string_to_parse );

            // We should now be sitting on a quoted string

            if ( string_to_parse.length() < 2 )
            {
                m_ReportParsingError(WSTRING_VIEW(L"Not enough room to quote a string (Rule 11)."));
                return( false );
            }

            if ( string_to_parse.at( 0 ) not_eq '\'' and
                 string_to_parse.at( 0 ) not_eq '\"' )
            {
                m_ReportParsingError(WSTRING_VIEW(L"String not properly quoted (Rule 11)."));
                return( false );
            }

            bool we_started_with_a_double_quote{ false };

            if ( string_to_parse.at( 0 ) == '\"' )
            {
                we_started_with_a_double_quote = true;
            }

            // If we get here, it means we need to strip off the leading quote

            string_to_parse.erase(0, 1);

            if ( we_started_with_a_double_quote == true)
            {
                location_of_character = string_to_parse.find( '\"' );
            }
            else
            {
                location_of_character = string_to_parse.find( '\'' );
            }

            if ( location_of_character == string_to_parse.npos )
            {
                m_ReportParsingError(WSTRING_VIEW(L"Mismatched quotes (Rule 11)."));
                return( false );
            }

            name.assign( string_to_parse.substr( 0, location_of_character ) );

            //WFCTRACEVAL( TEXT( "Name is " ), name );

            // Now check to make sure the characters in name are legal Pubidchar's

            for ( auto const name_loop_index : Range(name.length()) ) // Range1 ???????
            {
                if ( Win32FoundationClasses::is_xml_PubidChar( name.at( name_loop_index ) ) == false )
                {
                    m_ReportParsingError(WSTRING_VIEW(L"Illegal PubidChar (Rule 13)."));
                    return( false );
                }
            }

            string_to_parse.erase(0, location_of_character + 1);
            trim_left(string_to_parse);

            //WFCTRACEVAL( TEXT( "Would now parse " ), string_to_parse );

            if ( we_started_with_a_double_quote == true)
            {
                m_Contents.push_back( '\"' );
                m_Contents.append( name );
                m_Contents.push_back( '\"' );
            }
            else
            {
                m_Contents.push_back( '\'' );
                m_Contents.append( name );
                m_Contents.push_back( '\'' );
            }

            // We should now be sitting on another quote character (SystemLiteral)

            if ( string_to_parse.length() < 2 )
            {
                // There's not enough room for an opening and closing quote
                m_ReportParsingError(WSTRING_VIEW(L"No more room to quote a string (Rule 11)."));
                return( false );
            }

            if ( string_to_parse.at( 0 ) not_eq '\'' and
                 string_to_parse.at( 0 ) not_eq '\"' )
            {
                m_ReportParsingError(WSTRING_VIEW(L"PUBLIC SystemLiteral not properly quoted (Rule 11)."));
                return( false );
            }

            we_started_with_a_double_quote = false;

            if ( string_to_parse.at( 0 ) == '\"' )
            {
                we_started_with_a_double_quote = true;
            }

            // If we get here, it means we need to strip off the leading quote

            string_to_parse.erase(0, 1);

            if ( we_started_with_a_double_quote == true)
            {
                location_of_character = string_to_parse.find( '\"' );
            }
            else
            {
                location_of_character = string_to_parse.find( '\'' );
            }

            if ( location_of_character == string_to_parse.npos )
            {
                m_ReportParsingError(WSTRING_VIEW(L"Mismatched quotes (Rule 11)."));
                return( false );
            }

            name.assign( string_to_parse.substr( 0, location_of_character ) );
            //WFCTRACEVAL( TEXT( "Name is " ), name );

            string_to_parse.erase(0, location_of_character + 1);
            trim_left(string_to_parse);

            //WFCTRACEVAL( TEXT( "Would  now parse " ), string_to_parse );

            if ( we_started_with_a_double_quote == true)
            {
                m_Contents.push_back( '\"' );
                m_Contents.append( name );
                m_Contents.push_back( '\"' );
            }
            else
            {
                m_Contents.push_back( '\'' );
                m_Contents.append( name );
                m_Contents.push_back( '\'' );
            }

            //WFCTRACEVAL( TEXT( "m_Contents isss now " ), m_Contents );

            if ( string_to_parse.empty() == true )
            {
                m_ReportParsingError(WSTRING_VIEW(L"No more room (Rule 28)."));
                return( false );
            }
        }
        else
        {
            // If it is SYSTEM, there should be one quoted string following it (Rule 75)

            //WFCTRACE( TEXT( "Parsing SYSTEM" ) );

            m_Contents.append(WSTRING_VIEW(L" SYSTEM "));

            string_to_parse.erase(0, 6);
            trim_left(string_to_parse);

            //WFCTRACEVAL( TEXT( "Would now parse " ), string_to_parse );

            // We should now be sitting on a quote character (SystemLiteral)

            if ( string_to_parse.length() < 2 )
            {
                // There's not enough room for an opening an and closing quotes
                m_ReportParsingError(WSTRING_VIEW(L"No more room to parse a quoted string (Rule 11)."));
                return( false );
            }

            if ( string_to_parse.at( 0 ) not_eq '\'' and
                 string_to_parse.at( 0 ) not_eq '\"' )
            {
                m_ReportParsingError(WSTRING_VIEW(L"PUBLIC SystemLiteral not properly quoted (Rule 11)."));
                return( false );
            }

            bool we_started_with_a_double_quote{ false };

            if ( string_to_parse.at( 0 ) == '\"' )
            {
                we_started_with_a_double_quote = true;
            }

            // If we get here, it means we need to strip off the leading quote

            string_to_parse.erase(0, 1);

            if ( we_started_with_a_double_quote == true)
            {
                location_of_character = string_to_parse.find( '\"' );
            }
            else
            {
                location_of_character = string_to_parse.find( '\'' );
            }

            if ( location_of_character == string_to_parse.npos )
            {
                m_ReportParsingError(WSTRING_VIEW(L"Mismatched quotes (Rule 11)."));
                return( false );
            }

            name.assign( string_to_parse.substr( 0, location_of_character ) );

            //WFCTRACEVAL( TEXT( "Name is " ), name );

            string_to_parse.erase(0, location_of_character + 1);
            trim_left(string_to_parse);

            //WFCTRACEVAL( TEXT( "Would now parse " ), string_to_parse );

            if ( we_started_with_a_double_quote == true)
            {
                m_Contents.push_back( '\"' );
                m_Contents.append( name );
                m_Contents.push_back( '\"' );
            }
            else
            {
                m_Contents.push_back( '\'' );
                m_Contents.append( name );
                m_Contents.push_back( '\'' );
            }

            //WFCTRACEVAL( TEXT( "m_Contents iss now " ), m_Contents );

            if ( string_to_parse.empty() == true )
            {
                m_ReportParsingError(WSTRING_VIEW(L"No more room (Rule 28)."));
                return( false );
            }
        }
    }

    // Yet another inconsistency in XML, child elements of DOCTYPE are delimited
    // by square brackets. No, it doesn't make any sense. It is one of the problems
    // of maintaining backward compatibility with SGML.

    if ( string_to_parse.at( 0 ) == '>' )
    {
        //WFCTRACE( TEXT( "Finished parsing." ) );
        return( true );
    }

    if ( string_to_parse.at( 0 ) not_eq '[' )
    {
        m_ReportParsingError(WSTRING_VIEW(L"Illegal character at contiuation point (Rule 28)."));
        return( false );
    }

    m_Contents.append(WSTRING_VIEW(L" ["));

    // If it is [, then we have a whole lot of SGML pain to go through

    string_to_parse.erase(0,1);

    // The next valid characters are spaces (Rule 28), a closing bracket (Rule 28) or an opening < (Rule 29)
    // Let's go for the easy stuff first

    while( string_to_parse.empty() == false and Win32FoundationClasses::is_xml_white_space( string_to_parse.at( 0 ) ) )
    {
        m_Contents.push_back( string_to_parse.at( 0 ) );
        string_to_parse.erase(0, 1);
    }

    //WFCTRACEVAL( TEXT( "Now would parse " ), string_to_parse );

    // We have now narrowed things down to a ] or a <

    if ( string_to_parse.empty() == true )
    {
        m_ReportParsingError(WSTRING_VIEW(L"No room after continuation point (Rule 28)."));
        return( false );
    }

    if ( string_to_parse.at( 0 ) == ']' )
    {
        m_Contents.push_back( ']' );

        string_to_parse.erase(0, 1);
        trim_left( string_to_parse);
        //WFCTRACEVAL( TEXT( "Now would parse " ), string_to_parse );

        if ( string_to_parse.empty() == true )
        {
            m_ReportParsingError(WSTRING_VIEW(L"No room after closing ] (Rule 28)."));
            return( false );
        }

        // This character must be the closing >

        if ( string_to_parse.at( 0 ) not_eq '>' )
        {
            m_ReportParsingError(WSTRING_VIEW(L"Missing closing > (Rule 28)."));
            return( false );
        }

        return( true );
    }

    // It wasn't a ']' so it must be a <

    if ( string_to_parse.at( 0 ) not_eq '<' )
    {
        m_ReportParsingError(WSTRING_VIEW(L"Ill-formed DOCTYPE child, missing < (Rule 29)."));
        return( false );
    }

    // We must recurse

    CParsePoint beginning_of_child( m_Beginning );

    // The following while loop is to preserve the line and column counts

    int number_of_bytes_per_character{ 1 };

    if ( parser.IsTextASCII() == false )
    {
        if ( parser.IsTextUCS4() == true)
        {
            number_of_bytes_per_character = 4;
        }
        else
        {
            number_of_bytes_per_character = 2;
        }
    }

    while( beginning_of_child.GetIndex() < ( m_Ending.GetIndex() - ( string_to_parse.length() * number_of_bytes_per_character ) ) )
    {
        parser.AdvanceByOneCharacter( beginning_of_child );
    }

    // Now parse until we find the end of DOCTYPE

    bool exit_loop{ false };

    while( exit_loop == false )
    {
        Win32FoundationClasses::CExtensibleMarkupLanguageElement * sub_element_p{ nullptr };

        if ( parser.PeekCharacter( beginning_of_child, 0 ) == '<' )
        {
            sub_element_p = NewElement( this, ElementType::Element, m_Document );

            if ( sub_element_p->Parse( beginning_of_child, parser ) == false )
            {
                //WFCTRACE( TEXT( "Failed to parse sub element" ) );
                return( false );
            }
        }
        else if ( parser.PeekCharacter( beginning_of_child, 0 ) == '%' )
        {
            // We've got a reference to an entity

            std::wstring entity_string;

            if ( parser.GetUntilAndIncluding( beginning_of_child, L";", entity_string ) == false )
            {
                m_ReportParsingError(WSTRING_VIEW(L"DOCTYPE PEReference is missing terminating semi-colon (Rule 69)."));
                return( false );
            }

            //WFCTRACEVAL( TEXT( "Would now parse " ), entity_string );

            std::wstring entity_to_resolve( entity_string );

            if ( entity_to_resolve.at( 0 ) == '%' )
            {
                entity_to_resolve[ 0 ] = '&';
            }

            std::wstring entity_resolved_to;

            if ( m_Document == nullptr )
            {
                m_ReportParsingError(WSTRING_VIEW(L"No entity database."));
                return( false );
            }

            if ( m_Document->ResolveEntity( entity_to_resolve, entity_resolved_to ) == false )
            {
                m_ReportParsingError(WSTRING_VIEW(L"Can't resolve DOCTYPE PEReference (Rule 69)."));
                return( false );
            }

            // OK, now that we have resolved the entity, we need to parse it (it is kind of recursive)

            bool exit_parse_loop{ false };

            while( exit_parse_loop == false )
            {
                if ( entity_resolved_to.at( 0 ) == '%' )
                {
                    // I hates dragon rabbits. We have another entity to resolve.

                    entity_to_resolve.assign( entity_resolved_to );
                    entity_to_resolve[ 0 ] = '&';

                    if ( m_Document->ResolveEntity( entity_to_resolve, entity_resolved_to ) == false )
                    {
                        m_ReportParsingError(WSTRING_VIEW(L"Can't resolve embedded DOCTYPE PEReference (Rule 69)."));
                        return( false );
                    }
                }
                else if ( entity_resolved_to.at( 0 ) == '<' )
                {
                    // This code implements an insert type function. We need
                    // to parse bytes that aren't present in the data stream
                    // (wrapped by CDataParser). We need to insert those bytes
                    // at this time and parse them. We create a new CDataParser
                    // to solve this problem.

                    std::vector<uint8_t> new_bytes;

                    append_to_bytes( entity_resolved_to, new_bytes );

                    CDataParser entity_parser;

                    std::ignore = entity_parser.Initialize( new_bytes.data(), new_bytes.size() );

                    CParsePoint entity_beginning;

                    sub_element_p = NewElement( this, ElementType::Element, m_Document );

                    if ( sub_element_p == nullptr )
                    {
                        m_ReportParsingError(WSTRING_VIEW(L"Ran out of memory allocating for a tricky element."));
                        return( false );
                    }

                    if ( sub_element_p->Parse( entity_beginning, entity_parser ) == false )
                    {
                        m_ReportParsingError(WSTRING_VIEW(L"Can't parse tricky element (Rule 69)."));
                        return( false );
                    }
                    else
                    {
                        exit_parse_loop = true;
                        sub_element_p->m_Ending.Copy( beginning_of_child );
                    }
                }
                else
                {
                    // If we get here, it means there's most likely been an external
                    // reference. Since we are a standalone only parser, we must
                    // error out.

                    std::wstring error_message;

                    error_message.assign(WSTRING_VIEW(L"Unsupported (non-standalone) External Reference to \""));
                    error_message.append( entity_resolved_to );
                    error_message.push_back( '\"' );

                    m_ReportParsingError( error_message );

                    return( false );
                }
            }
        }
        else
        {
            sub_element_p = NewElement( this, ElementType::TextSegment, m_Document );

            // The only thing we cal allow in a text segment that is a child of DOCTYPE is
            // on that contains all white spaces, Rule 28

            sub_element_p->m_Beginning.Copy( beginning_of_child );

            while( Win32FoundationClasses::is_xml_white_space( parser.PeekCharacter( beginning_of_child, 0 ) ) == true )
            {
                sub_element_p->m_Contents.push_back( static_cast< wchar_t >( parser.PeekCharacter( beginning_of_child, 0 ) ) );
                parser.AdvanceByOneCharacter( beginning_of_child );
            }

            sub_element_p->m_Ending.Copy( beginning_of_child );
        }

        // Now skip characters until we get to ] or <

        sub_element_p->GetEnding( beginning_of_child );

        if ( beginning_of_child.GetIndex() >= parser.GetSize() )
        {
            // We're done
            return( true ); // this should probably be false
        }

        auto const character_to_test{ parser.PeekCharacter(beginning_of_child, 0) };

        if ( character_to_test == ']' )
        {
            // Hey! we're at the end, now skip spaces

            parser.AdvanceByOneCharacter( beginning_of_child, ']' );

            while( Win32FoundationClasses::is_xml_white_space( parser.PeekCharacter( beginning_of_child, 0 ) ) == true )
            {
                parser.AdvanceByOneCharacter( beginning_of_child );

                if ( beginning_of_child.GetIndex() >= parser.GetSize() )
                {
                    m_ReportParsingError(WSTRING_VIEW(L"Error 2490 (Rule 28)."));
                    return( false ); // this should probably be false
                }
            }

            // We had better be on a closing >

            if ( parser.PeekCharacter( beginning_of_child, 0 ) not_eq '>' )
            {
                m_ReportParsingError(WSTRING_VIEW(L"Ill-formed DOCTYPE child, missing terminating > (Rule 28)."));
                return( false ); // this should probably be false
            }

            parser.AdvanceByOneCharacter( beginning_of_child );
            m_Ending.Copy( beginning_of_child );

            return( true );
        }

        sub_element_p = nullptr;
    }

    // Need to validate the sub-elements to make sure they ain't XML (i.e. regular elements)
    // They must all be processing instructions

    m_ReportParsingError(WSTRING_VIEW(L"DOCTYPE Section did not parse."));
    return( false );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_ParseElementName( _In_ std::wstring_view name, _Inout_ std::wstring& parent_name, _Inout_ uint32_t& desired_instance_number, _Out_ std::wstring& child_name ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( m_Document );

    parent_name.clear();
    child_name.clear();

    if ( name.empty() == true )
    {
        return( false );
    }

    wchar_t parent_child_separator_character{ '.' };

    if ( m_Document not_eq nullptr )
    {
        parent_child_separator_character = m_Document->GetParentChildSeparatorCharacter();
    }

    parent_name.assign( name );

    // Let's pull off the first field

    auto location_of_separator{ parent_name.find(parent_child_separator_character) };

    if ( location_of_separator not_eq parent_name.npos )
    {
        child_name.assign( right( parent_name, parent_name.length() - ( location_of_separator + 1 ) ) );
        //parent_name.assign( parent_name.substr( 0, location_of_separator ) );
        parent_name.erase(std::cbegin(parent_name) + location_of_separator, std::cend(parent_name));
    }

    //WFCTRACEVAL( TEXT( "Parent name is " ), parent_name );
    //WFCTRACEVAL( TEXT( "Child name is  " ), child_name );

    // What we're looking for is the parent_name portion of what was passed to us.
    // We know it's name but we need to know which instance of that named object we want.

    desired_instance_number = 0;

    auto location_of_beginning{ parent_name.find('(') };

    if (location_of_beginning not_eq parent_name.npos )
    {
        // Looks like we've got an instance number in here

        auto location_of_ending{ parent_name.find(')') };

        // Check to make sure they didn't pass us something like "Southpark)0("

        if ( location_of_ending < location_of_beginning or location_of_ending == parent_name.npos )
        {
            //WFCTRACE( TEXT( "[] is ][ (they're backwards)" ) );
            return( false );
        }

        location_of_beginning++; // skip the (

        if ( location_of_beginning >= location_of_ending )
        {
            //WFCTRACE( TEXT( "location_of_beginning >= location_of_ending" ) );
            // we saw "()" so we default to an instance number of zero
            desired_instance_number = 0;
        }
        else
        {
            // There something between ( and )

            std::wstring instance_number_string;

            instance_number_string.assign( parent_name.substr( location_of_beginning, (location_of_ending - location_of_beginning ) ) );

            trim(instance_number_string);

            // WFCTRACEVAL( TEXT( "instance_number_string is " ), instance_number_string );

            desired_instance_number = static_cast<uint32_t>(as_integer( instance_number_string ));

            // Now trim off the ( xxx )

            //parent_name.assign( parent_name.substr( 0, location_of_beginning - 1 ) );
            parent_name.erase(std::cbegin(parent_name) + (location_of_beginning - 1), std::cend(parent_name));
        }
    }
    else
    {
        //WFCTRACE( TEXT( "There ain't no ()" ) );
        desired_instance_number = 0;
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_ParseProcessingInstruction( void ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_Contents.empty() == true )
    {
        m_ReportParsingError(WSTRING_VIEW(L"The processing instruction cannot be empty (Rule 16)."));
        return( false );
    }

    // m_Contents has been filled with the data between the <? and the ?>

    if ( m_Contents.length() > 2 )
    {
        if ( starts_with_no_case( m_Contents, WSTRING_VIEW(L"xml") ) == true)
        {
            return( m_ParseXMLDeclaration( m_Contents ) );
        }
    }

    // Check to see if the first character of the name is legal

    std::wstring_view const legal_characters( WSTRING_VIEW(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_:") );

    auto const value_index{ legal_characters.find(m_Contents[0]) };

    if ( value_index == legal_characters.npos )
    {
        m_ReportParsingError(WSTRING_VIEW(L"Illegal first character in processing instruction (Rule 16->5)."));
        return( false );
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_ParseTag( _In_ std::wstring_view tag, _Inout_ bool& did_tag_contain_terminator, _In_ CDataParser const& parser ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    // At this point, tag should contain both a '<' and a '>'

    did_tag_contain_terminator = false;

    std::wstring tag_data( tag );
    std::wstring tag_name;

    // The first thing we must do is check for a <![CDATA[ section.

    if ( tag.length() > 9 )
    {
        if (starts_with(tag, WSTRING_VIEW(L"<![CDATA[") ) == true)
        {
            // Yup, this here is a CDATA section

            if ( m_ParseCDATASection( tag, parser ) == true)
            {
                did_tag_contain_terminator = true; // CDATA sections are self-terminating
                return( true );
            }
            else
            {
                m_ReportParsingError(WSTRING_VIEW(L"Ill-formed CDATA section."));
                return( false );
            }
        }
    }

    auto location_of_character{ tag_data.find_first_of(WSTRING_VIEW(L" \t\r\n")) }; // This list of space characters is taken from section 2.3 of the XML 1.0 recommendation (REC-xml-19980210)

    if ( location_of_character == tag_data.npos )
    {
        //WFCTRACE( TEXT( "There ain't no attributes" ) );

        // There ain't no attributes (and no spaces)

        tag_name.assign( tag );
        tag_data.clear();

        if ( tag_name.empty() == false )
        {
            if ( tag_name.at( 0 ) == '<' )
            {
                tag_name.erase(0, 1);
            }
        }

        if ( tag_name.empty() == false )
        {
            if ( tag_name[ tag_name.length() - 1 ] == '>' )
            {
                tag_name.erase(tag_name.length() - 1, 1);
            }
        }

        trim( tag_name );

        // Let's see if we are a terminator

        if ( tag_name.empty() == false )
        {
            if ( tag_name[ 0 ] == '/' )
            {
                did_tag_contain_terminator = true;
            }
            else
            {
                // Thanks go to Jeff Winkler (winkler1@sprynet.com)
                // For adding this section. I was not properly
                // handling self terminating tags (like <job/>)

                if ( tag_name.at( tag_name.length() - 1 ) ==  '/' )
                {
                    did_tag_contain_terminator = true;

                    // Now trim off that terminator
                    //tag_name.assign( tag_name.substr( 0, tag_name.length() - 1 ) );
                    tag_name.erase(tag_name.length() - 1, 1);
                }
            }
        }

        m_Tag.assign( tag_name );
    }
    else
    {
        // There are some attributes

        tag_name.assign( tag_data.substr( 0, location_of_character ) );

        if ( tag_name.empty() == false )
        {
            if ( tag_name.at( 0 ) == '<' )
            {
                // OOPS! We need to trim off the beginning
                tag_name.erase(0, 1);
                trim(tag_name);
            }
        }
    }

    //WFCTRACEVAL( TEXT( "Setting m_Tag to " ), tag_name );

    // Checkout the first character of the tag name, it may be
    // a special one

    if ( tag_name.empty() == true )
    {
        m_ReportParsingError(WSTRING_VIEW(L"Tag is empty."));
        return( false );
    }

    switch( tag_name.at( 0 ) )
    {
    case '?':

        // We got us one of them there processing instructions

        m_Type = ElementType::ProcessingInstruction;

        m_Tag.clear();

        if ( tag.find(WSTRING_VIEW(L"?>")) == tag.npos )
        {
            // If we get here, it means we hit something tricky like "<?pi some data ? > <??>"
            //WFCTRACE( TEXT( "Can't find processing instruction terminator \"?>\"" ) );

            m_Contents.assign( right( tag, tag.length() - 2 ) );
            //WFCTRACEVAL( TEXT( "Contents is now " ), m_Contents );

            std::wstring tricky_end;

            if ( parser.GetUntilAndIncluding( m_Ending, L"?>", tricky_end ) == false )
            {
                m_ReportParsingError(WSTRING_VIEW(L"Processing instruction is not terminated with ?> (Rule 16)"));
                return( false );
            }

            m_Contents.append( tricky_end.substr( 0, tricky_end.length() - 2 ) );

            if ( IsRoot() == false )
            {
                did_tag_contain_terminator = true;
            }

            return( m_ParseProcessingInstruction() );
        }

        m_Contents.assign( tag.substr( 2, tag.length() - 4 ) );

        if ( IsRoot() == false )
        {
            did_tag_contain_terminator = true;
        }

        //WFCTRACEVAL( "Contents is ", m_Contents );

        return( m_ParseProcessingInstruction() );
        break;

    case '!':

        // We got us one of them there META tags, but hold on! It could
        // be any one of a number of things (I thought this was designed
        // to be easily parsed).

        //WFCTRACE( TEXT( "It is a META tag thingy" ) );

        if ( tag_name.length() < 3 )
        {
            m_ReportParsingError(WSTRING_VIEW(L"META tag is too short."));
            return( false );
        }

        // See if it is a comment <!--
        if ( tag_name.at( 1 ) == '-' and
             tag_name.at( 2 ) == '-' )
        {
            // Yup, it is a comment

            // WFCTRACE( TEXT( "It is a comment" ) );

            m_Type = ElementType::Comment;

            m_Contents.assign( right( tag, tag.length() - 4 ) );

            if ( m_Document not_eq nullptr and ( is_flagged( m_Document->GetParseOptions(), WFC_XML_LOOSE_COMMENT_PARSING) == true ) )
            {
                location_of_character = m_Contents.find(WSTRING_VIEW(L"-->"));

                if ( location_of_character == m_Contents.npos )
                {
                    // If we get here, it means the comment text contains a >

                    std::wstring temp_string;

                    if ( parser.GetUntilAndIncluding( m_Ending, L"-->", temp_string ) == false )
                    {
                        //WFCTRACE( TEXT( "Can't find comment terminator \"-->\"" ) );
                        m_ReportParsingError(WSTRING_VIEW(L"Comment is missing the --> terminator (Rule 15)."));
                        return( false );
                    }

                    m_Contents.append( temp_string );
                    location_of_character = m_Contents.find(WSTRING_VIEW(L"-->"));
                }
            }
            else
            {
                // Strict comment parsing

                location_of_character = m_Contents.find(WSTRING_VIEW(L"--"));

                if ( location_of_character == STRING_NOT_FOUND )
                {
                    //WFCTRACE( TEXT( "Can't find comment terminator \"--\"" ) );
                    m_ReportParsingError(WSTRING_VIEW(L"Comment is missing the -- terminator (Rule 15)."));
                    return( false );
                }

                if ( m_Contents.at( location_of_character + 2 ) not_eq '>' )
                {
                    //WFCTRACE( TEXT( "Comment contains illegal double dashes (Rule 15)." ) );
                    m_ReportParsingError(WSTRING_VIEW(L"Comment contains illegal double dashes (Rule 15)."));
                    return( false );
                }
            }

            m_Tag.clear();
            //m_Contents.assign( m_Contents.substr( 0, location_of_character ) );
            m_Contents.erase(location_of_character);
            did_tag_contain_terminator = true;

            return( true );
        }
        else if ( tag_name.at( 1 ) == '[' and
                ( tag_name.at( 2 ) == 'C' or tag_name.at( 2 ) == 'c' ) )
        {
            // We may have a CDATA section here

            if ( tag_name.length() < 9 )
            {
                //WFCTRACE( TEXT( "Giving up on funky CData section" ) );
                m_ReportParsingError(WSTRING_VIEW(L"Ill-formed CDATA section (Rule 19)."));
                return( false );
            }

            std::wstring cdata( tag_name.substr( 1, 8 ) );

            if ( cdata.compare(WSTRING_VIEW(L"![CDATA[")) == I_AM_EQUAL_TO_THAT)
            {
                //WFCTRACE( TEXT( "It is a CDATA section" ) );

                // Well, whaddya know... It is a CDATA section
                m_Type = ElementType::CharacterData;
                tag_name.assign(WSTRING_VIEW(L"[CDATA["));

                if ( m_ParseCDATASection( tag, parser ) == false )
                {
                    //WFCTRACE( TEXT( "Can't parse CDATA section" ) );
                    m_ReportParsingError(WSTRING_VIEW(L"Bad CDATA section (Rule 20)."));
                    return( false );
                }
            }
            else
            {
                m_Type                     = ElementType::MetaData;
                did_tag_contain_terminator = true;
            }
        }
        else if ( tag_name.length() > 6 and
                  tag_name.at( 1 ) == 'E' and
                  tag_name.at( 2 ) == 'N' and
                  tag_name.at( 3 ) == 'T' and
                  tag_name.at( 4 ) == 'I' and
                  tag_name.at( 5 ) == 'T' and
                  tag_name.at( 6 ) == 'Y' )
        {
            //WFCTRACE( TEXT( "ENTITY tag found" ) );

            // First, parse it like a generic processing instruction. This
            // allows us to output it just like we input it.

            m_Tag.assign( right( tag_name, tag_name.length() - 1 ) );

            // The rest of the line goes into m_Contents

            m_Contents.assign( right( tag_data, tag_data.length() - ( location_of_character + 1 ) ) );

            // Strip off the trailing '>'

            if ( m_Contents.empty() == false )
            {
                if ( m_Contents.at( m_Contents.length() - 1 ) == '>' )
                {
                    //m_Contents.assign( m_Contents.substr( 0, m_Contents.length() - 1 ) );
                    m_Contents.erase(m_Contents.length() - 1, 1);
                }
            }

            // Now add the entity to the document

            auto location_of_entity{ tag_data.find(WSTRING_VIEW(L"ENTITY")) };

            if ( location_of_entity == tag_data.npos )
            {
                m_ReportParsingError(WSTRING_VIEW(L"Ill-formed ENTITY section (Rule 71)."));
                return( false );
            }

            tag_data.erase(0, tag_data.length() - (location_of_entity + 6));

            trim(tag_data);

            if ( tag_data.empty() == false )
            {
                // There's something in there, Rule 72 says there can be a
                // percent sign as the first character

                if ( tag_data.at( 0 ) == '%' )
                {
                    tag_data.erase(0, 1);

                    // Trim leading spaces
                    trim_left( tag_data );
                }

                // Hopefully we are now left with a string like this:
                // WFC_HOME "c:\wfc"

                if ( tag_data.length() > 1 )
                {
                    // We should now be on the entity name

                    auto const location_of_space{ tag_data.find_first_of(WSTRING_VIEW(L" \t\r\n")) };

                    if ( location_of_space == tag_data.npos )
                    {
                        m_ReportParsingError(WSTRING_VIEW(L"Ill-formed ENTITY section[2] (Rule 71)."));
                        return( false );
                    }

                    std::wstring entity_name( tag_data.substr( 0, location_of_space ) );

                    tag_data.erase(0, location_of_space);

                    trim_left(tag_data);

                    // We should now be on a quote (single or double)

                    if ( tag_data.at( 0 ) not_eq '"' and tag_data.at( 0 ) not_eq '\'' )
                    {
                        // 1999-03-26
                        // Thanks go to Jurgen Lumera (jlumera@vftis.com) for finding
                        // a bug where I wasn't correctly parsing ENTITY lines. DOH DOH DOH!
                        // We must be an external entity

                        //WFCTRACE( TEXT( "First character of entity value is not a quote or single quote (see Rule 9). Let's see if it is an external reference." ) );

                        // We must attempt to parse according to rules 75 and 76

                        if ( tag_data.at( 0 ) not_eq 'S' and tag_data.at( 0 ) not_eq 'P' )
                        {
                            //WFCTRACE( TEXT( "Ill-formed external entity value. Character is not S or P. Refer to rule 75." ) );
                            m_ReportParsingError(WSTRING_VIEW(L"Ill-formed ENTITY section[4] (Rule 71)."));
                            return( false );
                        }

                        if ( tag_data.at( 0 ) == 'S' )
                        {
                            // Entity must be SYSTEM

                            if ( tag_data.length() < 6 )
                            {
                                // Not enough room to succeede
                                //WFCTRACE( TEXT( "Ran out of room parsing SYSTEM. Refer to rule 75." ) );
                                m_ReportParsingError(WSTRING_VIEW(L"Ill-formed ENTITY section (Rule 75)."));
                                return( false );
                            }

                            // See if the next few characters make up YSTEM

                            if ( tag_data.at( 1 ) not_eq 'Y' or
                                 tag_data.at( 2 ) not_eq 'S' or
                                 tag_data.at( 3 ) not_eq 'T' or
                                 tag_data.at( 4 ) not_eq 'E' or
                                 tag_data.at( 5 ) not_eq 'M' )
                            {
                                //WFCTRACE( TEXT( "External entity is not SYSTEM. Refer to rule 75." ) );
                                m_ReportParsingError(WSTRING_VIEW(L"Ill-formed ENTITY section[2] (Rule 75)."));
                                return( false );
                            }

                            // <!ENTITY smelc80562AA1 SYSTEM "smelc80562AA1.tif" NDATA saetiff>

                            tag_data.erase(0, 6);
                            trim_left(tag_data);
                        }
                        else
                        {
                            // Entity must be PUBLIC
                            if ( tag_data.length() < 6 )
                            {
                                // Not enough room to succeede
                                //WFCTRACE( TEXT( "Ran out of room parsing PUBLIC. Refer to rule 75." ) );
                                m_ReportParsingError(WSTRING_VIEW(L"Ill-formed ENTITY (PUBLIC) section (Rule 75)."));
                                return( false );
                            }

                            // See if the next few characters make up UBLIC

                            if ( tag_data.at( 1 ) not_eq 'U' or
                                 tag_data.at( 2 ) not_eq 'B' or
                                 tag_data.at( 3 ) not_eq 'L' or
                                 tag_data.at( 4 ) not_eq 'I' or
                                 tag_data.at( 5 ) not_eq 'C' )
                            {
                                //WFCTRACE( TEXT( "External entity is not PUBLIC. Refer to rule 75." ) );
                                m_ReportParsingError(WSTRING_VIEW(L"Ill-formed ENTITY (PUBLIC) section[2] (Rule 75)."));
                                return( false );
                            }

                            m_Contents.assign( tag_data );
                            did_tag_contain_terminator = true;

                            return( true );
                        }
                    }

                    if ( tag_data.at( 0 ) == '"' )
                    {
                        // Double quote terminated

                        //WFCTRACEVAL( TEXT( "Double quoted " ), tag_data );

                        tag_data.erase(0, 1);

                        // New for 70
                        // 2002-07-01
                        // Need to detect multiple quotes in the body (ibm09n03.xml)

                        location_of_entity = tag_data.find( '"' );

                        while( location_of_entity == tag_data.npos)
                        {
                            // This could be the result of a > existing in the entity value

                            // WFCTRACE( TEXT( "Can't find closing double quote." ) );

                            std::wstring temp_string;

                            if ( parser.GetUntilAndIncluding( m_Ending, L">", temp_string ) == false )
                            {
                                m_ReportParsingError(WSTRING_VIEW(L"ENTITY value missing closing double quote (Rule 9)."));
                                return( false );
                            }

                            tag_data.append( temp_string );

                            location_of_entity = tag_data.find( '"' );
                        }

                        // Check for more than one double-quote...

                        if ( tag_data.find( '"', location_of_entity + 1 ) not_eq tag_data.npos )
                        {
                            m_ReportParsingError(WSTRING_VIEW(L"ENTITY value has extra double quote (Rule 9)."));
                            return( false );
                        }

                        //tag_data.assign( tag_data.substr( 0, location_of_entity ) );
                        tag_data.erase(location_of_entity);
                    }
                    else
                    {
                        // Single quote terminated

                        //WFCTRACEVAL( TEXT( "Single quoted " ), tag_data );

                        tag_data.erase(0, 1);

                        location_of_entity = tag_data.find( '\'' );

                        while( location_of_entity == tag_data.npos )
                        {
                            //WFCTRACE( TEXT( "Can't find closing single quote." ) );

                            std::wstring temp_string;

                            if ( parser.GetUntilAndIncluding( m_Ending, L">", temp_string ) == false )
                            {
                                m_ReportParsingError(WSTRING_VIEW(L"ENTITY value missing closing single quote (Rule 9)."));
                                return( false );
                            }

                            tag_data.append( temp_string );

                            location_of_entity = tag_data.find( '\'' );
                        }

                        // Check for more than one double-quote...

                        if ( tag_data.find( '\'', location_of_entity + 1 ) not_eq tag_data.npos )
                        {
                            m_ReportParsingError(WSTRING_VIEW(L"ENTITY value has extra single quote (Rule 9)."));
                            return( false );
                        }

                        //tag_data.assign( tag_data.substr( 0, location_of_entity ) );
                        tag_data.erase(location_of_entity);
                    }

                    // 2002-06-29
                    // New for 70
                    // Check for illegal characters in entity values according to Rule 9
                    // This is a tricky rule. You can have entity or character references
                    // (which include & and %) but you can't have & or % stading alone.

                    auto location_of_semicolon{ tag_data.find(';') };
                    auto location_of_illegal_character{ tag_data.find('&') };

                    if ( location_of_illegal_character not_eq tag_data.npos )
                    {
                        if ( location_of_illegal_character > location_of_semicolon )
                        {
                            m_ReportParsingError(WSTRING_VIEW(L"ENTITY value contains & but is not used in an entity reference (Rule 9)."));
                            return( false );
                        }
                    }

                    location_of_illegal_character = tag_data.find( '%' );

                    if ( location_of_illegal_character not_eq tag_data.npos )
                    {
                        if ( location_of_illegal_character > location_of_semicolon )
                        {
                            m_ReportParsingError(WSTRING_VIEW(L"ENTITY value contains % but is not used in a character reference (Rule 9)."));
                            return( false );
                        }
                    }

                    //WFCTRACEVAL( TEXT( "Entity value is " ), tag_data );

                    if ( m_Document not_eq nullptr )
                    {
                        std::wstring entity_to_add(WSTRING_VIEW(L"&"));

                        entity_to_add.append( entity_name );
                        entity_to_add.push_back( ';' );

                        if ( m_Document->AddEntity( entity_to_add, tag_data ) == false )
                        {
                            //WFCTRACE( TEXT( "Can't add entity to document." ) );
                            m_ReportParsingError(WSTRING_VIEW(L"Internal error adding ENTITY"));
                            return( false );
                        }
                    }
                    else
                    {
                        //WFCTRACE( TEXT( "Not adding entity because we are an orphaned element." ) );
                    }

                    m_Type                     = ElementType::MetaData;
                    did_tag_contain_terminator = true;

                    return( true );
                }
            }
        }
        else if ( tag_name.length() > 7 and
                  tag_name.at( 1 ) == 'D' and
                  tag_name.at( 2 ) == 'O' and
                  tag_name.at( 3 ) == 'C' and
                  tag_name.at( 4 ) == 'T' and
                  tag_name.at( 5 ) == 'Y' and
                  tag_name.at( 6 ) == 'P' and
                  tag_name.at( 7 ) == 'E' )
        {
            //WFCTRACE( TEXT( "DOCTYPE tag found" ) );

            m_Type = ElementType::MetaData;
            did_tag_contain_terminator = true;

            if ( m_ParseDOCTYPESection( tag, parser ) == false )
            {
                // m_ParseDOCTYPESection() calls m_ReportParsingError()
                return( false );
            }
            else
            {
                return( true );
            }
        }
        else
        {
            // This is some generic processing instruction

            //WFCTRACE( TEXT( "Generic processing instruction" ) );

            // Strip off the leading !
            tag_name.erase(0,1);

            m_Type                     = ElementType::MetaData;
            did_tag_contain_terminator = true;

            m_Tag.assign( tag_name );

            // The rest of the line goes into m_Contents

            m_Contents.assign( right( tag_data, tag_data.length() - ( location_of_character + 1 ) ) );

            // Strip off the trailing '>'

            if ( m_Contents.empty() == false )
            {
                if ( m_Contents.at( m_Contents.length() - 1 ) == '>' )
                {
                    //m_Contents.assign( m_Contents.substr( 0, m_Contents.length() - 1 ) );
                    m_Contents.erase(m_Contents.length() - 1, 1);
                }
            }

            return( true );
        }

        break;

    default:

        m_Type = ElementType::Element;
        break;
    }

    m_Tag.assign( tag_name );

    if ( m_Type == ElementType::ProcessingInstruction )
    {
        return( true );
    }
    else if ( m_Type == ElementType::Element )
    {
        // We must now validate the element name as being legal

        if ( m_Tag.empty() == true )
        {
            m_ReportParsingError(WSTRING_VIEW(L"EmptyTag."));
            return( false );
        }

        if ( Win32FoundationClasses::is_xml_Letter( m_Tag[ 0 ] ) == false and
            m_Tag[ 0 ] not_eq '/' and
            m_Tag[ 0 ] not_eq '_' and
            m_Tag[ 0 ] not_eq ':' )
        {
            m_ReportParsingError(WSTRING_VIEW(L"First character of element name is not legal (Rule 5)."));
            return( false );
        }

        // 1999-09-10
        // Thanks go to Jean-Loup Clement (Jean-Loup.Clement@bull.net)
        // for finding out that VC5 doesn't support UINT32.

        for ( auto const name_loop_index : Range(m_Tag.length(), StartingRangePosition(1)) )
        {
            if ( Win32FoundationClasses::is_xml_NameChar( m_Tag[ name_loop_index ] ) == false )
            {
                std::wstring error_message(WSTRING_VIEW(L"Character "));
                error_message.append(std::to_wstring(name_loop_index));
                error_message.append(WSTRING_VIEW(L" of element named \""));
                error_message.append(m_Tag);
                error_message.append(WSTRING_VIEW(L"\" is invalid (Rule 999192)."));

                m_ReportParsingError( error_message );
                return( false );
            }
        }
    }

    tag_data.erase(0, location_of_character + 1);
    trim_left(tag_data);

    // Let's see if we are terminated via shorthand

    if ( tag_data.length() > 1 )
    {
        // The next-to-last character should be a /
        if ( tag_data.at( tag_data.length() - 2 ) == '/' )
        {
            did_tag_contain_terminator = true;
        }

        if ( tag_data.at( 0 ) == '/' )
        {
            return( true );
        }
    }

    bool we_parsed_at_least_one_attribute{ false };

    while( tag_data.length() > 1 )
    {
        location_of_character = tag_data.find( '=' );

        if ( location_of_character == tag_data.npos )
        {
            if ( we_parsed_at_least_one_attribute == false )
            {
                m_ReportParsingError(WSTRING_VIEW(L"Attribute must have '=' between the name and the value (Rule 41)."));
                return( false );
            }

            //WFCTRACEVAL( TEXT( "No More attributes in " ), tag_data );
            return( true );
        }

        we_parsed_at_least_one_attribute = true;

        std::wstring attribute_name( tag_data.substr( 0, location_of_character ) );
        trim( attribute_name);

        tag_data.erase(0, location_of_character + 1);
        trim_left(tag_data);

        // Here's where Jeff Winkler found another bug. I was not
        // correctly parsing attributes with spaces in them. DOH!

        if ( tag_data.empty() == true )
        {
            //WFCTRACE( TEXT( "Ran out of room looking for attribute value" ) );
            m_ReportParsingError(WSTRING_VIEW(L"Not yet documented error 2726."));
            return( false );
        }

        // When we get here tag_data contains "Two Words">

        // Search for the beginning quote (single or double)
        // The XML specification (Rule [10]) says attribute
        // values can be deliminated by single or double quotes

        bool we_started_with_a_double_quote{ false };

        if ( tag_data.at( 0 ) not_eq '\"' )
        {
            //WFCTRACE( TEXT( "First character of an attribute value was not a quote!" ) );

            if ( tag_data.at( 0 ) not_eq '\'' )
            {
                //WFCTRACE( TEXT( "And it ain't a single quote either!" ) );
                m_ReportParsingError(WSTRING_VIEW(L"Attribute values must begin with a \" or a ' (Rule 10)."));
                return( false );
            }
            else
            {
                we_started_with_a_double_quote = false;
            }
        }
        else
        {
            we_started_with_a_double_quote = true;
        }

        // If we get here, we need to strip off the leading quote

        tag_data.erase(0, 1);

#if 0
        if ( we_started_with_a_double_quote == true)
        {
            location_of_character = tag_data.Find( '\"' );

            if ( location_of_character not_eq CHARACTER_NOT_FOUND and location_of_character not_eq tag_data.GetLength() - 1 )
            {
                // Looks like we got one of them there doubley terminated strings such as <x att="This"That">
                m_ReportParsingError(WSTRING_VIEW(L"Attribute value cannot contain another double quote (Rule 10)." ) );
            }
        }
        else
        {
            location_of_character = tag_data.Find( TEXT( '\'' ) );

            if ( location_of_character not_eq CHARACTER_NOT_FOUND and location_of_character not_eq tag_data.GetLength() - 1 )
            {
                // Looks like we got one of them there doubley terminated strings such as <x att="This"That">
                m_ReportParsingError(WSTRING_VIEW(L"Attribute value cannot contain another single quote (Rule 10)." ) );
            }
        }
#else
        // 2003-11-05 I haven't solved that problem yet (the double thingy)
        if ( we_started_with_a_double_quote == true)
        {
            location_of_character = tag_data.find( '\"' );
        }
        else
        {
            location_of_character = tag_data.find( '\'' );
        }

#endif

        if ( location_of_character == tag_data.npos )
        {
            //WFCTRACEVAL( TEXT( "Couldn't find anything at line " ), __LINE__ );

            // At the end of the line?

            if (tag_data.empty() == true )
            {
                return( false );
            }

            std::wstring attribute_value(tag_data);

            // Since we are the last, we need to check the terminating characters
            // The last character should always be a '>'

            if ( attribute_value.at( attribute_value.length() - 1 ) not_eq '>' )
            {
                m_ReportParsingError(WSTRING_VIEW(L"Not yet documented error 2785."));
                return( false );
            }

            // Trim off that character

           // attribute_value.assign( attribute_value.substr( 0, attribute_value.length() - 1 ) );
            attribute_value.erase(attribute_value.length() - 1, 1);

            // Now, here comes the fun part. The next-to-last
            // character will be different depending on the type
            // of element this fella is.

            if ( m_Type == ElementType::ProcessingInstruction )
            {
                // The next-to-last character should be a ?

                if ( attribute_value.at( attribute_value.length() - 1 ) not_eq '?' )
                {
                    m_ReportParsingError(WSTRING_VIEW(L"Not yet documented error 2804."));
                    return( false );
                }

                //attribute_value.assign( attribute_value.substr( 0, attribute_value.length() - 1 ) );
                attribute_value.erase(attribute_value.length() - 1, 1);
            }
            else if ( m_Type == ElementType::MetaData or m_Type == ElementType::Element )
            {
                if ( attribute_value.empty() == false )
                {
                    if ( attribute_value.at( attribute_value.length() - 1 ) == '/' )
                    {
                        // Shorthand terminator
                        did_tag_contain_terminator = true;

                        //attribute_value.assign( attribute_value.substr( 0, attribute_value.length() - 1 ) );
                        attribute_value.erase(attribute_value.length() - 1, 1);
                    }
                }
            }

            // Add this attribute...

            if ( attribute_value.empty() == false )
            {
                if ( we_started_with_a_double_quote == true)
                {
                    if ( m_TrimQuotesAndSpaces( attribute_value, '\"' ) == false )
                    {
                        m_ReportParsingError(WSTRING_VIEW(L"Attribute missing matching \" (Rule 10)."));
                        return( false );
                    }
                }
                else
                {
                    if ( m_TrimQuotesAndSpaces( attribute_value, '\'' ) == false )
                    {
                        m_ReportParsingError(WSTRING_VIEW(L"Attribute missing matching \' (Rule 10)."));
                        return( false );
                    }
                }

                if ( AddAttribute( attribute_name, attribute_value ) == false )
                {
                    // Adding attribute failed. It called m_ReportParsingError() for us.
                    return( false );
                }

                return( true );
            }
            else
            {
                // Rule 10
                m_ReportParsingError(WSTRING_VIEW(L"Empty attribute not allowed (Rule 10)."));
                return( false );
            }
        }
        else
        {
            std::wstring attribute_value( tag_data.substr( 0, location_of_character ) );

            if ( we_started_with_a_double_quote == true)
            {
                std::ignore = m_TrimQuotesAndSpaces( attribute_value, '\"' );
            }
            else
            {
                std::ignore = m_TrimQuotesAndSpaces( attribute_value, '\'' );
            }

            tag_data.erase(0, location_of_character + 1);
            trim_left(tag_data);

            // Now calculate the offsets of the new attribute values

            std::wstring temp_string(attribute_name);
            temp_string.push_back('=');
            temp_string.push_back('\"');

            std::size_t offset_of_attribute_name{ 0 };
            auto tag_index_of_attribute_name{ tag.find(temp_string, m_Tag.length()) };

            if (tag_index_of_attribute_name == tag.npos)
            {
                offset_of_attribute_name = 0;
            }
            else
            {
                offset_of_attribute_name = m_Beginning.GetIndex() + tag_index_of_attribute_name;
            }

            temp_string.clear();
            temp_string.push_back('\"');
            temp_string.append(attribute_value);

            auto offset_of_attribute_value{ tag.find(temp_string, tag_index_of_attribute_name + 1) }; // + 1 to skip the equals sign

            if (offset_of_attribute_value == tag.npos)
            {
                offset_of_attribute_value = 0;
            }
            else
            {
                offset_of_attribute_value += m_Beginning.GetIndex() + 1; // + 1 to account for the beginning double-quote of our search pattern
            }

            if ( AddAttribute( attribute_name, attribute_value, offset_of_attribute_name, offset_of_attribute_value) == false )
            {
                // Adding attribute failed. It called m_ReportParsingError() for us.
                return( false );
            }
        }

        attribute_name.clear();
    }

    //WFCTRACEVAL( TEXT( "Successfully parsed tag " ), m_Tag );

    if ( m_Tag.length() > 1 )
    {
        if ( m_Tag.at( 0 ) == '/' )
        {
            // If we get here, it means that our end tag had trailing spaces.
            // Or, it could have had attributes which is a no no.

            if ( m_Attributes.empty() == false )
            {
                m_ReportParsingError(WSTRING_VIEW(L"Attributes not allowed in end tag (Rule 42)."));
                return( false );
            }

            did_tag_contain_terminator = true;
        }
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_ParseXMLDeclaration( _In_ std::wstring_view tag ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( m_Document );

    std::wstring const temp_string(tag.substr( 0, 3 ));

    uint32_t parsing_options{ 0 };

    if ( m_Document not_eq nullptr )
    {
        parsing_options = m_Document->GetParseOptions();
    }

    if (is_flagged( parsing_options, WFC_XML_IGNORE_CASE_IN_XML_DECLARATION ) == false )
    {
        // We must begin with lower case xml

        if ( temp_string.compare(WSTRING_VIEW(L"xml")) not_eq I_AM_EQUAL_TO_THAT)
        {
            m_ReportParsingError(WSTRING_VIEW(L"The xml declaration must be lower case (Rule 23)."));
            return( false );
        }
    }

    // We've found the XML declaration, let's see if we are the root element

    if ( IsRoot() == false )
    {
        // We ain't root. That means we've come across another XML declaration in the data.
        // That ain't legal.

        m_ReportParsingError(WSTRING_VIEW(L"There can be only one prolog (Rule 1)."));
        return( false );
    }

    std::wstring string_to_parse( right( tag, tag.length() - 3 ) );
    trim_left(string_to_parse);

    if ( string_to_parse.length() < 7 )
    {
        m_ReportParsingError(WSTRING_VIEW(L"Missing VersionInfo data (Rule 23)."));
        return( false );
    }

    auto attribute_name{ string_to_parse.substr(0, 7) };

    if ( attribute_name.compare(WSTRING_VIEW(L"version")) not_eq I_AM_EQUAL_TO_THAT)
    {
        m_ReportParsingError(WSTRING_VIEW(L"The VersionInfo must be lower case (Rule 23)."));
        return( false );
    }

    string_to_parse.erase(0, 7);
    trim_left(string_to_parse);

    // We should be at the = character

    if ( string_to_parse.empty() == true )
    {
        m_ReportParsingError(WSTRING_VIEW(L"Truncated VersionInfo (Rule 24)."));
        return( false );
    }

    if ( string_to_parse.at( 0 ) not_eq '=' )
    {
        m_ReportParsingError(WSTRING_VIEW(L"Ill-formed VersionInfo missing = character (Rule 24)."));
        return( false );
    }

    string_to_parse.erase(0, 1);
    trim_left(string_to_parse);

    if ( string_to_parse.empty() == true )
    {
        m_ReportParsingError(WSTRING_VIEW(L"Ill-formed VersionInfo, missing VersionNum (Rule 24)."));
        return( false );
    }

    bool we_started_with_a_double_quote{ false };

    if ( string_to_parse.at( 0 ) not_eq '\"' and
         string_to_parse.at( 0 ) not_eq '\'' )
    {
        m_ReportParsingError(WSTRING_VIEW(L"Illegal VersionNum delimiter, must be a single or double quote (Rule 24)."));
        return( false );
    }

    if ( string_to_parse.at( 0 ) == '\"' )
    {
        we_started_with_a_double_quote = true;
    }

    string_to_parse.erase(0, 1);

    std::size_t location_of_delimiter{ 0 };

    if ( we_started_with_a_double_quote == true)
    {
        location_of_delimiter = string_to_parse.find( '\"' );
    }
    else
    {
        location_of_delimiter = string_to_parse.find( '\'' );
    }

    if ( location_of_delimiter == string_to_parse.npos )
    {
        m_ReportParsingError(WSTRING_VIEW(L"VersionNum missing matching delimiter (Rule 24)."));
        return( false );
    }

    std::wstring value( string_to_parse.substr( 0, location_of_delimiter ) );

    if ( value.empty() == true )
    {
        m_ReportParsingError(WSTRING_VIEW(L"Empty VersionNum not allowed (Rule 26)."));
        return( false );
    }

    string_to_parse.erase(0, location_of_delimiter + 1);
    trim_left(string_to_parse);

    std::wstring_view legal_characters(WSTRING_VIEW(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.:"));

    for ( auto const value_index : Range(value.length()) )
    {
        if ( legal_characters.find( value.at( value_index ) ) == legal_characters.npos )
        {
            std::wstring error_message(WSTRING_VIEW(L"Character "));
            error_message.append(std::to_wstring(value_index));
            error_message.append(WSTRING_VIEW(L" (decimal "));
            error_message.append(std::to_wstring(static_cast<int>(value.at(value_index))));
            error_message.append(WSTRING_VIEW(L") of the \"version\" attribute is illegal (Rule 26). "));

            //WFCTRACE( error_message );

            m_ReportParsingError( error_message );

            return( false );
        }
    }

    if ( m_Document not_eq nullptr )
    {
        m_Document->SetVersion( value );
    }

    if ( string_to_parse.empty() == true )
    {
        // EncodingDecl and SDDecl are missing but that's OK
        return( true );
    }

    // If we get here, we can expect to see EncodingDecl or SDDecl

    //WFCTRACEVAL( TEXT( "Now parsing" ), string_to_parse );

    // Let's see if we are sitting on "encoding"

    if ( string_to_parse.length() < 8 )
    {
        // There's not enough room for either "encoding" or "standalone"
        m_ReportParsingError(WSTRING_VIEW(L"Not enough room left to parse (Rule 23)."));
        return( false );
    }

    value.assign( string_to_parse.substr( 0, 8 ) );

    if ( value.compare(WSTRING_VIEW(L"encoding")) == I_AM_EQUAL_TO_THAT)
    {
        // We are encoding, let's parse it

        string_to_parse.erase(0, 8);
        trim_left(string_to_parse);

        // We should be at the = character

        if ( string_to_parse.empty() == true )
        {
            m_ReportParsingError(WSTRING_VIEW(L"Truncated EncodingDecl (Rule 80)."));
            return( false );
        }

        if ( string_to_parse.at( 0 ) not_eq '=' )
        {
            m_ReportParsingError(WSTRING_VIEW(L"Ill-formed EncodingDecl missing = character (Rule 80)."));
            return( false );
        }

        string_to_parse.erase(0,1);
        trim_left(string_to_parse);

        if ( string_to_parse.empty() == true )
        {
            m_ReportParsingError(WSTRING_VIEW(L"Ill-formed EncodingDecl, missing EncName (Rule 80)."));
            return( false );
        }

        we_started_with_a_double_quote = false;

        if ( string_to_parse.at( 0 ) not_eq '\"' and
             string_to_parse.at( 0 ) not_eq '\'' )
        {
            m_ReportParsingError(WSTRING_VIEW(L"Illegal EncName delimiter, must be a single or double quote (Rule 80)."));
            return( false );
        }

        if ( string_to_parse.at( 0 ) == '\"' )
        {
            we_started_with_a_double_quote = true;
        }

        string_to_parse.erase(0, 1);

        location_of_delimiter = 0;

        if ( we_started_with_a_double_quote == true)
        {
            location_of_delimiter = string_to_parse.find( '\"' );
        }
        else
        {
            location_of_delimiter = string_to_parse.find( '\'');
        }

        if ( location_of_delimiter == string_to_parse.npos )
        {
            m_ReportParsingError(WSTRING_VIEW(L"EncName missing matching delimiter (Rule 80)."));
            return( false );
        }

        value.assign( string_to_parse.substr( 0, location_of_delimiter ) );

        if ( value.empty() == true )
        {
            m_ReportParsingError(WSTRING_VIEW(L"Empty EncName not allowed (Rule 81)."));
            return( false );
        }

        string_to_parse.erase(0, location_of_delimiter + 1);
        trim_left(string_to_parse);

        // Now validate the contents of EncName

        auto const character{ value.at(0) };

        if (  ( character >= 'a' and character <= 'z' ) or
              ( character >= 'A' and character <= 'Z' ) )
        {
            // Do Nothing
        }
        else
        {
            m_ReportParsingError(WSTRING_VIEW(L"Illegal first character in EncName (Rule 81)."));
            return( false );
        }

        if ( value.length() > 1 )
        {
            legal_characters = WSTRING_VIEW(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789._-");

            for ( auto const loop_index : Range(value.length(), StartingRangePosition(1)) )
            {
                if ( legal_characters.find( value.at( loop_index ) ) == legal_characters.npos )
                {
                    std::wstring error_message(WSTRING_VIEW(L"Character "));
                    error_message.append(std::to_wstring(loop_index + 1));
                    error_message.append(WSTRING_VIEW(L" is illegal in EncName of \""));
                    error_message.append(value);
                    error_message.append(WSTRING_VIEW(L"\" (Rule 81)."));

                    m_ReportParsingError(error_message);
                    return( false );
                }
            }

            if ( m_Document not_eq nullptr )
            {
                m_Document->SetEncoding( value );
            }
        }
    }

    if ( string_to_parse.empty() == true )
    {
        // Missing SDDecl but that's OK
        return( true );
    }

    // If we get here, we can expect to see SDDecl

    if ( string_to_parse.length() < 10 )
    {
        // There's not enough room for either "encoding" or "standalone"
        m_ReportParsingError(WSTRING_VIEW(L"Not enough room left to parse SDDecl (Rule 32)."));
        return( false );
    }

    value.assign( string_to_parse.substr( 0, 10 ) );

    if ( value.compare(WSTRING_VIEW(L"standalone")) == I_AM_EQUAL_TO_THAT)
    {
        // We are standalone, let's parse it
        string_to_parse.erase(0, 10);
        trim_left(string_to_parse);

        // We should be at the = character

        if ( string_to_parse.empty() == true )
        {
            m_ReportParsingError(WSTRING_VIEW(L"Truncated SDDecl (Rule 32)."));
            return( false );
        }

        if ( string_to_parse.at( 0 ) not_eq '=' )
        {
            m_ReportParsingError(WSTRING_VIEW(L"Ill-formed SDDecl missing = character (Rule 32)."));
            return( false );
        }

        string_to_parse.erase(0, 1);
        trim_left(string_to_parse);

        if ( string_to_parse.empty() == true )
        {
            m_ReportParsingError(WSTRING_VIEW(L"Ill-formed SDDecl, missing data (Rule 32)."));
            return( false );
        }

        we_started_with_a_double_quote = false;

        if ( string_to_parse.at( 0 ) not_eq '\"' and
             string_to_parse.at( 0 ) not_eq '\'' )
        {
            m_ReportParsingError(WSTRING_VIEW(L"Illegal SDDecl delimiter, must be a single or double quote (Rule 32)."));
            return( false );
        }

        if ( string_to_parse.at( 0 ) == '\"' )
        {
            we_started_with_a_double_quote = true;
        }

        string_to_parse.erase(0, 1);

        location_of_delimiter = 0;

        if ( we_started_with_a_double_quote == true)
        {
            location_of_delimiter = string_to_parse.find( '\"' );
        }
        else
        {
            location_of_delimiter = string_to_parse.find( '\'' );
        }

        if ( location_of_delimiter == string_to_parse.npos )
        {
            m_ReportParsingError(WSTRING_VIEW(L"SDDecl missing matching delimiter (Rule 32)."));
            return( false );
        }

        value.assign( string_to_parse.substr( 0, location_of_delimiter ) );

        if ( value.empty() == true )
        {
            m_ReportParsingError(WSTRING_VIEW(L"Empty SDDecl not allowed (Rule 32)."));
            return( false );
        }

        string_to_parse.erase(0, location_of_delimiter + 1);
        trim_left(string_to_parse);

        // Now validate the contents of SDDecl

        if ( value.compare(WSTRING_VIEW(L"yes")) not_eq I_AM_EQUAL_TO_THAT and value.compare(WSTRING_VIEW(L"no")) not_eq I_AM_EQUAL_TO_THAT)
        {
            m_ReportParsingError(WSTRING_VIEW(L"Illegal value for SDDecl, it must me lower case \"yes\" or \"no\" (Rule 32)."));
            return( false );
        }

        if ( m_Document not_eq nullptr )
        {
            if ( value.compare(WSTRING_VIEW(L"yes")) == I_AM_EQUAL_TO_THAT)
            {
                m_Document->SetStandalone( true );
            }
            else
            {
                m_Document->SetStandalone( false );
            }
        }
    }

    if ( string_to_parse.empty() == false )
    {
        m_ReportParsingError(WSTRING_VIEW(L"Extra characters after SDDecl (Rule 23)."));
        return( false );
    }

    return( true );
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_ReportParsingError(_In_ std::wstring_view error_message ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( m_Document );

    if ( m_Document not_eq nullptr )
    {
        m_Document->SetParsingErrorInformation( m_Tag, m_Beginning, m_Ending, error_message );
    }
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_ResolveEntities( _Inout_ std::wstring& encoded_entity_string ) const noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( m_Document );

    if ( m_Document == nullptr )
    {
        // There ain't no stinking document! Only the document knows what the entities are
        return;
    }

    if ( encoded_entity_string.find( '&' ) == encoded_entity_string.npos )
    {
        // YIPPEE!! There aren't any entities to decode
        return;
    }

    // Yup, there be entities in that there string

    std::wstring translated_string;
    std::wstring temp_string( encoded_entity_string );
    std::wstring entity;
    std::wstring text;

    auto location{ temp_string.find('&') };

    while( location not_eq temp_string.npos )
    {
        // We found the beginning of an entity (hopefully)
        translated_string.append( temp_string.substr( 0, location ) );

        temp_string.erase(0, location);

        location = temp_string.find( ';' );

        if ( location == temp_string.npos )
        {
            //WFCTRACEVAL( TEXT( "Found the beginning of an entity but not the end in: " ), encoded_entity_string );
            return;
        }

        entity.assign( temp_string.substr( 0, location + 1 ) );

        if ( m_Document->ResolveEntity( entity, text ) == false )
        {
            //WFCTRACEVAL( TEXT( "Unresloved entity " ), entity );
            return;
        }

        translated_string.append( text );

        temp_string.erase(0, location + 1);

        // Let's try to find another entity...
        location = temp_string.find( '&' );
    }

    translated_string.append( temp_string );

    encoded_entity_string.assign( translated_string );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::m_TrimQuotesAndSpaces( _Inout_ std::wstring& value, _In_ wchar_t const quote_to_trim ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    trim(value);

    bool return_value{ false };

    if ( value.empty() == false )
    {
        if ( value.at( 0 ) == quote_to_trim )
        {
            value.erase(0, 1);
        }

        if ( value.empty() == false )
        {
            if ( value.at( value.length() - 1 ) == quote_to_trim )
            {
                return_value = true;
                value.erase(0, value.length() - 1 );
            }
        }

        trim(value);
    }

    return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetChildValue(_In_ std::wstring_view tag_name, _Out_ uint32_t& value ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    value = 0;

    auto child{ GetChild(tag_name) };

    if ( child == nullptr )
    {
        return( false );
    }

    std::wstring value_text;

    child->GetText( value_text );

    trim(value_text);

    value = static_cast<uint32_t>(as_integer(value_text));

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetChildValue(_In_ std::wstring_view tag_name, _Out_ uint64_t& value ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    value = 0;

    auto child{ GetChild(tag_name) };

    if ( child == nullptr )
    {
        return( false );
    }

    std::wstring value_text;

    child->GetText( value_text );

    trim(value_text);
    make_lower(value_text);

    value = static_cast<uint64_t>(as_unsigned_integer(value_text));

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetChildValue(_In_ std::wstring_view tag_name, _Out_ double& value ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    value = 0;

    auto child{ GetChild(tag_name) };

    if ( child == nullptr )
    {
        return( false );
    }

    std::wstring value_text;

    child->GetText( value_text );

    Win32FoundationClasses::trim(value_text);
    Win32FoundationClasses::make_lower(value_text);

    value = Win32FoundationClasses::as_double( value_text );

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetChildValue(_In_ std::wstring_view tag_name, _Out_ bool& value ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    value = false;

    auto child{ GetChild(tag_name) };

    if ( child == nullptr )
    {
        return( false );
    }

    std::wstring value_text;

    child->GetText( value_text );

    trim(value_text);
    make_lower(value_text);

    if ( value_text.empty() == true )
    {
        return( false );
    }

    if ( starts_with( value_text, 't' ) == true or
         starts_with( value_text, 'y' ) == true )
    {
        value = true;
        return( true );
    }

    int const integer_value{ static_cast<int>(Win32FoundationClasses::as_integer(value_text)) };

    if ( integer_value not_eq 0 )
    {
        value = true;
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetChildValue(_In_ std::wstring_view tag_name, _Out_ std::wstring& value ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    value.clear();

    auto child{ GetChild(tag_name) };

    if ( child == nullptr )
    {
        return( false );
    }

    child->GetText( value );

    return( true );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::GetChildValue(_In_ std::wstring_view tag_name, _Out_ std::vector<uint8_t>& value ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    value.clear();

    auto child{ GetChild(tag_name) };

    if ( child == nullptr )
    {
        return( false );
    }

    std::wstring encoded_bytes;

    child->GetText( encoded_bytes );

    Win32FoundationClasses::CBase64Coding coder;

    return( coder.Decode( encoded_bytes, value ) == false ? false : true );
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::NewChildValue(_In_ std::wstring_view tag_name, _In_ uint32_t const value ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    auto return_value{ NewChild(tag_name) };

    if ( return_value not_eq nullptr )
    {
        std::ignore = return_value->AddText( std::to_wstring(value) );
    }

    return( return_value );
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::NewChildValue(_In_ std::wstring_view tag_name, _In_ int32_t const value ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    auto return_value{ NewChild(tag_name) };

    if ( return_value not_eq nullptr )
    {
        std::ignore = return_value->AddText( std::to_wstring(value) );
    }

    return( return_value );
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::NewChildValue(_In_ std::wstring_view tag_name, _In_ uint64_t const value ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    auto return_value{ NewChild(tag_name) };

    if ( return_value not_eq nullptr )
    {
        std::ignore = return_value->AddText( std::to_wstring(value) );
    }

    return( return_value );
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::NewChildValue(_In_ std::wstring_view tag_name, _In_ int64_t const value ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    auto return_value{ NewChild(tag_name) };

    if ( return_value not_eq nullptr )
    {
        std::ignore = return_value->AddText( std::to_wstring(value) );
    }

    return( return_value );
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::NewChildValue(_In_ std::wstring_view tag_name, _In_ double const value ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    auto return_value{ NewChild(tag_name) };

    if ( return_value not_eq nullptr )
    {
        std::wstring formatted_value;

        format( formatted_value, L"%lf", value );

        std::ignore = return_value->AddText( formatted_value );
    }

    return( return_value );
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::NewChildValue(_In_ std::wstring_view tag_name, _In_ std::vector<uint8_t> const& value ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    std::wstring as_string;

    CBase64Coding coder;

    std::ignore = coder.Encode( value, as_string );

    return( NewChildValue( tag_name, as_string ) );
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::NewChildValue(_In_ std::wstring_view tag_name, __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    std::wstring as_string;

    CBase64Coding coder;

    std::ignore = coder.Encode( buffer, number_of_bytes, as_string );

    return( NewChildValue( tag_name, as_string ) );
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::NewChildValue(_In_ std::wstring_view tag_name, _In_ std::wstring_view value ) noexcept
{
    WFC_VALIDATE_POINTER( this );

    auto return_value{ NewChild(tag_name) };

    if ( return_value not_eq nullptr and value.empty() == false )
    {
        std::ignore = return_value->AddText( value );
    }

    return( return_value );
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::NewChild( _In_ std::wstring_view tag_name, _In_ ElementType const element_type ) noexcept
{
    auto child_p{ NewElement(this, element_type) };

    if ( child_p not_eq nullptr and tag_name.empty() == false )
    {
        child_p->SetTag( tag_name );
    }

    return( child_p );
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::NewComment(_In_ std::wstring_view text) noexcept
{
    auto return_value{ NewElement(this, ElementType::Comment) };

    if (return_value not_eq nullptr and text.empty() == false)
    {
        return_value->m_Contents.assign(text);
    }

    return(return_value);
}


_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * Win32FoundationClasses::CExtensibleMarkupLanguageElement::NewElement( _Inout_opt_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * parent_item, _In_ ElementType const type, __inout_opt Win32FoundationClasses::CExtensibleMarkupLanguageDocument * document ) noexcept
{
    auto element_p{ new Win32FoundationClasses::CExtensibleMarkupLanguageElement() };

    if ( element_p not_eq nullptr )
    {
        element_p->SetType( type );
        element_p->m_Parent   = parent_item;
        element_p->m_Document = document;

        // Let's see if we should inherit the document

        if ( element_p->m_Parent not_eq nullptr )
        {
            if ( element_p->m_Document == nullptr )
            {
                element_p->m_Document = element_p->m_Parent->m_Document;
            }

            // Since we have a parent, that parent must have a child
            element_p->m_Parent->AddChild( element_p, false );
        }
    }

    return( element_p );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageElement::Parse( _In_ Win32FoundationClasses::CParsePoint const& beginning, _In_ CDataParser const& parser ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER_NULL_OK( m_Document );

    m_AbortParsing = false;

    // Start out with an empty object

    Empty();

    uint32_t parsing_options{ 0 };

    if ( m_Document not_eq nullptr )
    {
        parsing_options = m_Document->GetParseOptions();
    }

    if ( m_Ending.GetIndex() >= parser.GetSize() )
    {
        //WFCTRACE( TEXT( "Ran out of room before we started" ) );
        m_ReportParsingError(WSTRING_VIEW(L"There are no more bytes to parse. We ran out of bytes before we ran out of XML."));
        return( false );
    }

    if ( m_Type == ElementType::Unknown )
    {
        //WFCTRACE( TEXT( "We don't know what type of element we are therefore we don't know how to parse the data" ) );
        m_ReportParsingError(WSTRING_VIEW(L"Type of element is unknown. Could not parse element data."));
        return( false );
    }

    m_Beginning.Copy( beginning );
    m_Ending.Copy( beginning );

    auto end_of_data{ static_cast<uint32_t>(parser.GetSize()) };
    uint32_t character{ 0x00 };

    // OK we are at a location that must contain something interesting
    // to parse. How we parse depends on what type of element we are.

    if ( m_Type == ElementType::TextSegment )
    {
        //WFCTRACE( TEXT( "We are a text segment" ) );

        m_IsAllWhiteSpace = true;

        // 1999-08-20
        // Thanks go to Scott Dowson (scott.dowson@pnl.gov) for the tremendous
        // speed improvement

        std::vector<uint8_t> text_segment_array;

        // We are a text segment, we start copying until we reach
        // a < or until we run out of room.

        while( m_Ending.GetIndex() < end_of_data )
        {
            character = parser.PeekCharacter( m_Ending, 0 );

            if ( character == '<' )
            {
                // We've reached the end of our segment

                text_segment_array.push_back( 0 );

#if defined( UNICODE )
                text_segment_array.push_back( 0 );
#endif // UNICODE

                m_Contents.assign( reinterpret_cast<wchar_t const *>(text_segment_array.data()) );

                return( true );
            }

            // Now let's see if we are still in the running to be all white space

            if ( m_IsAllWhiteSpace == true)
            {
                if ( character not_eq 0x20 and
                     character not_eq TAB_CHARACTER and
                     character not_eq CARRIAGE_RETURN and
                     character not_eq LINE_FEED )
                {
                    // We didn't find this character in the space string so it must not be a space-like character

                    m_IsAllWhiteSpace = false;
                }
            }

            parser.AdvanceByOneCharacter( m_Ending, character );

#if defined( UNICODE )

            text_segment_array.push_back( LOBYTE( character ) );
            text_segment_array.push_back( HIBYTE( character ) );

#else // UNICODE

            text_segment_array.push_back( static_cast< uint8_t >( character ) );

#endif // UNICODE
        }
    }
    else
    {
        //WFCTRACE( TEXT( "We are an element" ) );

        // We are some type of element, we should therefore be sitting on
        // a '<' (to start the tag).

        character = parser.PeekCharacter( m_Ending, 0 );

        if ( character not_eq '<' )
        {
            ASSERT( false );
            return( false );
        }

        // OK, we are sitting on a '<', let's parse the tag
        // We need to put the data into a string so m_ParseTag can deal with it

        std::wstring tag_string;

        // 1998-11-22, new for Release 38
        // Thanks go to Jeff Barczewski (jb@snowflight.com) for finding a bug where
        // comments weren't begin parsed correctly.

        // There's a special case we must consider here. If what we're parsing
        // is a comment (i.e. it begins with "<!--"), we must ignore all special
        // characters until we reach "-->".

        bool terminator_found{ false };

        if ( parser.PeekCharacter( m_Ending, 1 ) == '!' and
             parser.PeekCharacter( m_Ending, 2 ) == '-' and
             parser.PeekCharacter( m_Ending, 3 ) == '-' )
        {
            // Yup, we are in the special case here. We are parsing a comment tag
            terminator_found = parser.GetUntilAndIncluding( m_Ending, L"-->", tag_string );
        }
        else if ( parser.PeekCharacter( m_Ending, 1 ) == '!' and
                  parser.PeekCharacter( m_Ending, 2 ) == '[' and
                  parser.PeekCharacter( m_Ending, 3 ) == 'C' and
                  parser.PeekCharacter( m_Ending, 4 ) == 'D' and
                  parser.PeekCharacter( m_Ending, 5 ) == 'A' and
                  parser.PeekCharacter( m_Ending, 6 ) == 'T' and
                  parser.PeekCharacter( m_Ending, 7 ) == 'A' and
                  parser.PeekCharacter( m_Ending, 8 ) == '[' )
        {
            // Gawl dang CDATA sections
            terminator_found = parser.GetUntilAndIncluding( m_Ending, L"]]>", tag_string );
        }
        else
        {
            terminator_found = parser.GetUntilAndIncluding( m_Ending, L">", tag_string );
        }

        if ( terminator_found == true)
        {
            //WFCTRACEVAL( TEXT( "tag_string is " ), tag_string );

            if ( m_ParseTag( tag_string, m_ShorthandTerminatorDetected, parser ) == false )
            {
                //WFCTRACE( TEXT( "Can't m_ParseTag()" ) );
                // m_ParseTag calls m_ReportParsingError
                //m_ReportParsingError( WSTRING_VIEW(L"Internal error: Could not parse tag." ) );
                Empty();
                return( false );
            }

            if ( m_ShorthandTerminatorDetected == true)
            {
                //WFCTRACE( TEXT( "m_ShorthandTerminatorDetected is true" ) );
                m_IsTagTerminated = true;
            }

            // We've parsed our opening tag, let's see what the future holds

            Win32FoundationClasses::CExtensibleMarkupLanguageElement * sub_element_p{ nullptr };

            // while ( m_IsTagTerminated == false or m_Type == typeProcessingInstruction )
            while ( m_IsTagTerminated == false )
            {
                // We've got sub_elements!
                //WFCTRACE( TEXT( "We've got sub-elements!" ) );

                // Let's see if the sub-element is a text segment or not

                if ( m_Ending.GetIndex() < end_of_data )
                {
                    character = parser.PeekCharacter( m_Ending, 0 );

                    if ( character not_eq '<' )
                    {
#if 0
                        if ( m_Type == typeProcessingInstruction and m_Parent not_eq nullptr )
                        {
                            // If we are a processing instruction then the child will be the
                            // child of our parent and not us. The child will be a sibling

                            //WFCTRACEVAL( TEXT( "New sub element will be SIBLING a text segment, character was " ), character );
                            //WFCTRACEVAL( TEXT( "It will be a child of " ), m_Parent->m_Tag );
                            sub_element_p = NewElement( m_Parent, typeTextSegment, m_Document ); // Makes the new element a child of this element
                        }
                        else
                        {
                            //WFCTRACEVAL( TEXT( "New sub element will be a text segment, character was " ), character );
                            sub_element_p = NewElement( this, typeTextSegment, m_Document ); // Makes the new element a child of this element
                        }
#endif
                        //WFCTRACEVAL( TEXT( "New sub element will be a text segment, character was " ), character );
                        sub_element_p = NewElement( this, ElementType::TextSegment, m_Document ); // Makes the new element a child of this element
                    }
                    else
                    {
                        //WFCTRACE( TEXT( "New sub element will be some sort of element" ) );
                        sub_element_p = NewElement( this, ElementType::Element, m_Document ); // Makes the new element a child of this element
                    }

                    if ( sub_element_p == nullptr )
                    {
                        m_ReportParsingError(WSTRING_VIEW(L"Internal error: Could not create new element."));
                        Empty();
                        return( false );
                    }
                }
                else
                {
                    //WFCTRACE( TEXT( "We are at the end of the document" ) );

                    // We are at the end of the document

                    if ( IsRoot() == false )
                    {
                        m_ReportParsingError(WSTRING_VIEW(L"Premature end of data."));
                        Empty();

                        return( false );
                    }

                    if ( m_Document not_eq nullptr )
                    {
                        m_Document->ExecuteCallbacks( this );

                        if ( GetAbortParsing() == true)
                        {
                            m_ReportParsingError(WSTRING_VIEW(L"Parsing was aborted by Callback function."));
                            return( false );
                        }
                    }

                    return( true );
                }

                if ( sub_element_p not_eq nullptr )
                {
                    //WFCTRACE( TEXT( "Sub element was created" ) );

                    if ( sub_element_p->Parse( m_Ending, parser ) == true)
                    {
                        //WFCTRACE( TEXT( "Adding sub element as child" ) );
                        sub_element_p->GetEnding( m_Ending ); // Here's where we check to see if the sub-element just added is our terminator

                        sub_element_p->GetTag( tag_string );

                        if ( tag_string.empty() == false )
                        {
                            if ( tag_string.at( 0 ) == '/' )
                            {
                                // We've got a terminator, let's make sure it is ours!

                                tag_string.erase(0, 1);

                                trim(tag_string);

                                if ( tag_string.compare( m_Tag ) not_eq I_AM_EQUAL_TO_THAT)
                                {
                                    //WFCTRACEVAL( TEXT( "received a terminator for " ), tag_string );
                                    //WFCTRACEVAL( TEXT( "Unfortunately we are a " ), m_Tag );
                                    //WFCTRACE( TEXT( "That means we have overlapping tags!" ) );

                                    // Error message added by Mats, 1998-08-17
                                    std::wstring error_text(WSTRING_VIEW(L"Overlapping tags detected. Found \""));
                                    error_text.append(tag_string);
                                    error_text.append(WSTRING_VIEW(L"\" but needed to find \""));
                                    error_text.append(m_Tag);
                                    error_text.append(WSTRING_VIEW(L"\"."));

                                    m_ReportParsingError( error_text );

                                    RemoveChild( sub_element_p ); // Because NewElement made it our child
                                    DeleteElement( sub_element_p );
                                    sub_element_p = nullptr;
                                    Empty();

                                    return( false );
                                }
                                else
                                {
                                    //WFCTRACE( TEXT( "We found our terminator!" ) );

                                    // Yup, we are the correct terminator
                                    m_IsTagTerminated = true;

                                    RemoveChild( sub_element_p ); // Because NewElement made it our child
                                    DeleteElement( sub_element_p );
                                    sub_element_p = nullptr;

                                    if ( m_Document not_eq nullptr )
                                    {
                                        m_Document->ExecuteCallbacks( this );
                                    }

                                    // Now that the callbacks have had their shot, let's
                                    // see if we should abort parsing

                                    if ( GetAbortParsing() == true)
                                    {
                                        m_ReportParsingError(WSTRING_VIEW(L"Parsing was aborted by Callback function."));
                                        return( false );
                                    }

                                    return( true );
                                }
                            }
                        }
                        else
                        {
                            // 1998-12-02
                            // Larry Brendehoeft (Larry.Bredehoeft@sierra.com) suggested this feature be added

                            // 1999-08-19
                            // Added & checking

                            // 2000-10-24
                            // Juro Gottweis (juro@asc.sk) Found a bug here. I was trimming all-white-space
                            // elements when just WFC_XML_ALLOW_AMPERSANDS_IN_ELEMENTS was specified

                            if ( is_flagged( parsing_options, WFC_XML_IGNORE_ALL_WHITE_SPACE_ELEMENTS) == true or
                                 is_flagged( parsing_options, WFC_XML_ALLOW_AMPERSANDS_IN_ELEMENTS ) == true )
                            {
                                if ( sub_element_p->GetType() == ElementType::TextSegment )
                                {
                                    WFC_VALIDATE_bool( sub_element_p->m_IsAllWhiteSpace );

                                    if ( sub_element_p->m_IsAllWhiteSpace == true)
                                    {
                                        if ( is_flagged( parsing_options, WFC_XML_IGNORE_ALL_WHITE_SPACE_ELEMENTS ) == true )
                                        {
                                            //WFCTRACE( TEXT( "Removing all white-space element." ) );
                                            RemoveChild( sub_element_p );
                                            DeleteElement( sub_element_p );
                                            sub_element_p = nullptr;
                                        }
                                    }
                                    else
                                    {
                                        //WFCTRACE( TEXT( "Checking for ampersands..." ) );

                                        if ( sub_element_p->m_IsAllWhiteSpace == false )
                                        {
                                            // If we get here, it means we need to check the text segment for ampersands

                                            auto location_of_ampersand{ sub_element_p->m_Contents.find('&') };

                                            while( location_of_ampersand not_eq sub_element_p->m_Contents.npos)
                                            {
                                                // There's an ampersand in there, now let's validate it

                                                auto const location_of_semicolon{ sub_element_p->m_Contents.find(';', location_of_ampersand + 1) };

                                                if ( location_of_semicolon == sub_element_p->m_Contents.npos )
                                                {
                                                    m_ReportParsingError(WSTRING_VIEW(L"Ill-formed entity reference in text section (Rule 68)."));
                                                    return( false );
                                                }

                                                uint32_t rule_that_was_broken{ 0 };

                                                if ( m_Document->GetEntities().IsEntity( sub_element_p->m_Contents.substr( location_of_ampersand, ( location_of_semicolon + 1 ) - location_of_ampersand ), rule_that_was_broken ) == false )
                                                {
                                                    std::wstring error_message(WSTRING_VIEW(L"Bad entity reference in text section (Rule "));

                                                    error_message.append(std::to_wstring(rule_that_was_broken));
                                                    error_message.append(WSTRING_VIEW(L")."));

                                                    m_ReportParsingError( error_message );
                                                    return( false );
                                                }

                                                location_of_ampersand = sub_element_p->m_Contents.find( '&', location_of_ampersand + 1 );
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        /*
                        // This error reporting is unnecessary because sub_element_p will have
                        // reported the information. We would only be overwriting what it reported.

                        // Error message added by Mats, 1998-08-17
                        std::wstring error_text;

                        error_text.Format( TEXT( "Internal error: Parsing of sub-element failed. Current element is \"%s\". ),
                        (LPCTSTR) m_Tag );

                        m_ReportParsingError( error_text );
                        */

                        RemoveChild( sub_element_p );
                        DeleteElement( sub_element_p );
                        sub_element_p = nullptr;

                        return( false );
                    }
                }
            }
        }
        else
        {
            // Terminator could not be found
            // 1998-12-08 what happens when the terminator ain't found
            m_ReportParsingError(WSTRING_VIEW(L"Could not find terminator. Premature end of data encountered."));
            return( false );
        }
    }

    if ( m_Document not_eq nullptr )
    {
        m_Document->ExecuteCallbacks( this );
    }

    // Now that the callbacks have had their shot, let's
    // see if we should abort parsing

    if ( GetAbortParsing() == true)
    {
        m_ReportParsingError(WSTRING_VIEW(L"Parsing aborted by Callback function."));
        return( false );
    }

    return( true );
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::RemoveChild( _Inout_ Win32FoundationClasses::CExtensibleMarkupLanguageElement * element_p ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    WFC_VALIDATE_POINTER( element_p );

    for ( auto const loop_index : Range(m_Children.size()) )
    {
        if ( m_Children[ loop_index ] == element_p or
             m_Children[ loop_index ] == nullptr )
        {
            m_Children.erase( std::begin(m_Children) + loop_index );
            return;
        }
    }
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::WriteTo( _Inout_ std::vector<uint8_t>& xml ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    if ( m_Type == ElementType::Unknown )
    {
        //WFCTRACE( TEXT( "Don't know how to write a typeUnknown element" ) );
        return;
    }

    uint32_t write_options{ WFC_XML_WRITE_AS_ASCII };

    if ( m_Document not_eq nullptr )
    {
        write_options = m_Document->GetWriteOptions();
    }

    if ( m_Type == ElementType::TextSegment )
    {
        for ( auto const loop_index : Range(m_Contents.length()) )
        {
#if ! defined( UNICODE )
            m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( m_Contents.GetAt( loop_index ) ) ), write_options,  xml );
#else
            m_AddCharacterToOutput( static_cast< uint32_t >( m_Contents.at( loop_index ) ), write_options,  xml );
#endif
        }
    }
    else if ( m_Type == ElementType::MetaData )
    {
        m_AddCharacterToOutput( '<', write_options, xml );
        m_AddCharacterToOutput( '!', write_options, xml );

        for ( auto const loop_index : Range(m_Tag.length()) )
        {
#if ! defined( UNICODE )
            m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( m_Tag.GetAt( loop_index ) ) ), write_options, xml );
#else
            m_AddCharacterToOutput( static_cast< uint32_t >( m_Tag.at( loop_index ) ), write_options, xml );
#endif
        }

        // Even though we started with a !, we don't end with one

        // MetaData has no attributes

        // m_AddCharacterToOutput( TEXT( ' ' ), write_options, xml ); // No space between ! and name

        for ( auto const loop_index : Range(m_Contents.length()) )
        {
#if ! defined( UNICODE )
            m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( m_Contents.GetAt( loop_index ) ) ), write_options,  xml );
#else
            m_AddCharacterToOutput( static_cast< uint32_t >( m_Contents.at( loop_index ) ), write_options,  xml );
#endif
        }

        // Now we have to see if we are a DOCTYPE element. If so, we need to
        // reverse the order in which children are added as well as how we are
        // terminated. Yes, this is stupid. Yes, it doesn't fit with the rest
        // of XML syntax. Yes, we have SGML to thank for this.

        if ( m_Contents.length() > 6 and starts_with( m_Contents, WSTRING_VIEW(L"DOCTYPE") ) == true)
        {
            // Yup, we are a DOCTYPE element

            m_AppendChildren( xml );

            if ( m_Children.empty() == false )
            {
                m_AddCharacterToOutput( ']', write_options, xml );
            }

            m_AddCharacterToOutput( '>', write_options, xml );
        }
        else
        {
            m_AddCharacterToOutput( '>', write_options, xml );
            m_AppendChildren( xml );
        }
    }
    else if ( m_Type == ElementType::Comment )
    {
        m_AddIndentation(xml);
        m_AddCharacterToOutput( '<', write_options, xml );
        m_AddCharacterToOutput( '!', write_options, xml );
        m_AddCharacterToOutput( '-', write_options, xml );
        m_AddCharacterToOutput( '-', write_options, xml );

        for ( auto const loop_index : Range(m_Contents.length()) )
        {
#if ! defined( UNICODE )
            m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( m_Contents.GetAt( loop_index ) ) ), write_options, xml );
#else
            m_AddCharacterToOutput( static_cast< uint32_t >( m_Contents.at( loop_index ) ), write_options, xml );
#endif
        }

        m_AddCharacterToOutput( '-', write_options, xml );
        m_AddCharacterToOutput( '-', write_options, xml );
        m_AddCharacterToOutput( '>', write_options, xml );
    }
    else if ( m_Type == ElementType::CharacterData )
    {
        m_AddCharacterToOutput( '<', write_options, xml );
        m_AddCharacterToOutput( '!', write_options, xml );
        m_AddCharacterToOutput( '[', write_options, xml );
        m_AddCharacterToOutput( 'C', write_options, xml );
        m_AddCharacterToOutput( 'D', write_options, xml );
        m_AddCharacterToOutput( 'A', write_options, xml );
        m_AddCharacterToOutput( 'T', write_options, xml );
        m_AddCharacterToOutput( 'A', write_options, xml );
        m_AddCharacterToOutput( '[', write_options, xml );

        std::wstring temp_string( m_Contents);

        auto location_of_string{ temp_string.find(WSTRING_VIEW(L"]]>")) };

        while( location_of_string not_eq temp_string.npos )
        {
            // There's sections that need escaping

            for( auto const cdata_index : Range( location_of_string + 3 ) )
            {
#if ! defined( UNICODE )
                m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( temp_string.GetAt( cdata_index ) ) ), write_options, xml );
#else
                m_AddCharacterToOutput( static_cast< uint32_t >( temp_string.at( cdata_index ) ), write_options, xml );
#endif

                // 1998-09-30 Thanks go to Pei-Chiang Ma (north@uni-scape.com) for
                // finding a bug here. I was incrementing index instead of cdata_index.
            }

            // Now add the escape sequence

            m_AddCharacterToOutput( ']', write_options, xml );
            m_AddCharacterToOutput( ']', write_options, xml );
            m_AddCharacterToOutput( '>', write_options, xml );

            temp_string.erase(0, location_of_string + 3);

            location_of_string = temp_string.find(WSTRING_VIEW(L"]]>"));
        }

        for ( auto const cdata_index : Range(temp_string.length()) )
        {
#if ! defined( UNICODE )
            m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( temp_string.GetAt( cdata_index ) ) ), write_options, xml );
#else
            m_AddCharacterToOutput( static_cast< uint32_t >( temp_string.at( cdata_index ) ), write_options, xml );
#endif
        }

        m_AddCharacterToOutput( ']', write_options, xml );
        m_AddCharacterToOutput( ']', write_options, xml );
        m_AddCharacterToOutput( '>', write_options, xml ); // 1998-12-02 bug fixed by Larry Brendehoeft (Larry.Bredehoeft@sierra.com)
    }
    else if ( m_Type == ElementType::ProcessingInstruction )
    {
        // We may be the root of the problem...

        m_AddCharacterToOutput( '<', write_options, xml );
        m_AddCharacterToOutput( '?', write_options, xml );

        if ( IsRoot() == true)
        {
            m_AddCharacterToOutput( 'x', write_options, xml );
            m_AddCharacterToOutput( 'm', write_options, xml );
            m_AddCharacterToOutput( 'l', write_options, xml );
            m_AddCharacterToOutput( ' ', write_options, xml );
            m_AddCharacterToOutput( 'v', write_options, xml );
            m_AddCharacterToOutput( 'e', write_options, xml );
            m_AddCharacterToOutput( 'r', write_options, xml );
            m_AddCharacterToOutput( 's', write_options, xml );
            m_AddCharacterToOutput( 'i', write_options, xml );
            m_AddCharacterToOutput( 'o', write_options, xml );
            m_AddCharacterToOutput( 'n', write_options, xml );
            m_AddCharacterToOutput( '=', write_options, xml );

            if ( m_Document == nullptr )
            {
                //WFCTRACE( TEXT( "m_Document is NULL!" ) );
                xml.clear();
                return;
            }

            std::wstring temp_string;

            m_Document->GetVersion( temp_string );

            if ( temp_string.empty() == true )
            {
                m_AddCharacterToOutput( '\"', write_options, xml );
                m_AddCharacterToOutput( '1', write_options, xml );
                m_AddCharacterToOutput( '.', write_options, xml );
                m_AddCharacterToOutput( '0', write_options, xml );
                m_AddCharacterToOutput( '\"', write_options, xml );
            }
            else
            {
                m_AddCharacterToOutput( '\"', write_options, xml );

                // We will trust the document to have been set correctly

                for ( auto const loop_index : Range(temp_string.length()) )
                {
#if ! defined( UNICODE )
                    m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( temp_string.GetAt( loop_index ) ) ), write_options, xml );
#else
                    m_AddCharacterToOutput( static_cast< uint32_t >( temp_string.at( loop_index ) ), write_options, xml );
#endif
                }

                m_AddCharacterToOutput( '\"', write_options, xml );
            }

            // Now let's see if it has encoding

            m_Document->GetEncoding( temp_string );

            if ( temp_string.empty() == false )
            {
                m_AddCharacterToOutput( ' ', write_options, xml );
                m_AddCharacterToOutput( 'e', write_options, xml );
                m_AddCharacterToOutput( 'n', write_options, xml );
                m_AddCharacterToOutput( 'c', write_options, xml );
                m_AddCharacterToOutput( 'o', write_options, xml );
                m_AddCharacterToOutput( 'd', write_options, xml );
                m_AddCharacterToOutput( 'i', write_options, xml );
                m_AddCharacterToOutput( 'n', write_options, xml );
                m_AddCharacterToOutput( 'g', write_options, xml );
                m_AddCharacterToOutput( '=', write_options, xml );
                m_AddCharacterToOutput( '\"', write_options, xml );

                // We will trust the document to have been set correctly

                for ( auto const loop_index : Range(temp_string.length()) )
                {
#if ! defined( UNICODE )
                    m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( temp_string.GetAt( loop_index ) ) ), write_options, xml );
#else
                    m_AddCharacterToOutput( static_cast< uint32_t >( temp_string.at( loop_index ) ), write_options, xml );
#endif
                }

                m_AddCharacterToOutput( '\"', write_options, xml );
            }

            // Now add the standalone declaration

            m_AddCharacterToOutput( ' ', write_options, xml );
            m_AddCharacterToOutput( 's', write_options, xml );
            m_AddCharacterToOutput( 't', write_options, xml );
            m_AddCharacterToOutput( 'a', write_options, xml );
            m_AddCharacterToOutput( 'n', write_options, xml );
            m_AddCharacterToOutput( 'd', write_options, xml );
            m_AddCharacterToOutput( 'a', write_options, xml );
            m_AddCharacterToOutput( 'l', write_options, xml );
            m_AddCharacterToOutput( 'o', write_options, xml );
            m_AddCharacterToOutput( 'n', write_options, xml );
            m_AddCharacterToOutput( 'e', write_options, xml );
            m_AddCharacterToOutput( '=', write_options, xml );
            m_AddCharacterToOutput( '\"', write_options, xml );

            if ( m_Document->IsStandalone() == true)
            {
                m_AddCharacterToOutput( 'y', write_options, xml );
                m_AddCharacterToOutput( 'e', write_options, xml );
                m_AddCharacterToOutput( 's', write_options, xml );
            }
            else
            {
                m_AddCharacterToOutput( 'n', write_options, xml );
                m_AddCharacterToOutput( 'o', write_options, xml );
            }

            m_AddCharacterToOutput( '\"', write_options, xml );
        }
        else
        {
            for ( auto const loop_index : Range(m_Contents.length()) )
            {
#if ! defined( UNICODE )
                m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( m_Contents.GetAt( loop_index ) ) ), write_options, xml );
#else
                m_AddCharacterToOutput( static_cast< uint32_t >( m_Contents.at( loop_index ) ), write_options, xml );
#endif
            }
        }

        m_AddCharacterToOutput( '?', write_options, xml );
        m_AddCharacterToOutput( '>', write_options, xml );

        m_AppendChildren( xml );
    }
    else if ( m_Type == ElementType::Element )
    {
        m_IncrementIndentation();

        // To avoid writing whitespace, we must check to see if we are the
        // first element being written. This can happen if the root element
        // is NOT written (i.e. the WFC_XML_DONT_OUTPUT_XML_DECLARATION write
        // option was specified).

        if ( xml.empty() == true )
        {
            m_DecrementIndentation();
        }
        else
        {
            m_AddIndentation( xml );
        }

        m_AddCharacterToOutput( '<', write_options, xml );

        // Add Tag

        // First, see if we are in a namespace...

        if ( m_Document not_eq nullptr and m_Document->UseNamespace() == true)
        {
            std::wstring name_space;

            m_Document->GetNamespace( name_space );

            for ( auto const loop_index : Range(name_space.length()) )
            {
#if ! defined( UNICODE )
                m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( name_space.GetAt( loop_index ) ) ), write_options, xml );
#else
                m_AddCharacterToOutput( static_cast< uint32_t >( name_space.at( loop_index ) ), write_options, xml );
#endif
            }

            if (name_space.length() > 0 )
            {
                // We wrote something so output a colon...
                m_AddCharacterToOutput( ':', write_options, xml );
            }
        }

        for ( auto const loop_index : Range(m_Tag.length()) )
        {
#if ! defined( UNICODE )
            m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( m_Tag.GetAt( loop_index ) ) ), write_options, xml );
#else
            m_AddCharacterToOutput( static_cast< uint32_t >( m_Tag.at( loop_index ) ), write_options, xml );
#endif
        }

        m_AppendAttributes( xml );

        if ( m_ShorthandTerminatorDetected == true and m_Children.size() == 0 )
        {
            // Terminated elements have no text fields
            m_AddCharacterToOutput( '/', write_options, xml );
            m_AddCharacterToOutput( '>', write_options, xml );
        }
        else
        {
            m_AddCharacterToOutput( '>', write_options, xml );

            for ( auto const loop_index : Range(m_Contents.length()) )
            {
#if ! defined( UNICODE )
                m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( m_Contents.GetAt( loop_index ) ) ), write_options, xml );
#else
                m_AddCharacterToOutput( static_cast< uint32_t >( m_Contents.at( loop_index ) ), write_options, xml );
#endif
            }

            m_AppendChildren( xml );

            if ( m_Children.empty() == false )
            {
                auto first_child_p{ m_Children[0] };

                if ( first_child_p->m_Type not_eq ElementType::TextSegment or m_Children.size() > 1 )
                {
                    m_AddIndentation( xml );
                }
            }

            // Now add our closing tag

            m_AddCharacterToOutput( '<', write_options, xml );
            m_AddCharacterToOutput( '/', write_options, xml );

            for ( auto const loop_index : Range(m_Tag.length()) )
            {
#if ! defined( UNICODE )
                m_AddCharacterToOutput( static_cast< uint32_t >( static_cast< BYTE >( m_Tag.GetAt( loop_index ) ) ), write_options, xml );
#else
                m_AddCharacterToOutput( static_cast< uint32_t >( m_Tag.at( loop_index ) ), write_options, xml );
#endif
            }

            m_AddCharacterToOutput( '>', write_options, xml );
        }

        m_DecrementIndentation();
    }
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageElement& Win32FoundationClasses::CExtensibleMarkupLanguageElement::operator=( _In_ Win32FoundationClasses::CExtensibleMarkupLanguageElement const& source ) noexcept
{
    WFC_VALIDATE_POINTER( this );
    Copy( source );
    return( *this );
}

void Win32FoundationClasses::CExtensibleMarkupLanguageElement::Trim(void) noexcept
{
    // Look for children that are all spaces and delete them
    if (m_Children.empty() == true)
    {
        return;
    }

    std::vector<Win32FoundationClasses::CExtensibleMarkupLanguageElement *> space_children;

    for (auto child : m_Children)
    {
        if (child->GetType() == ElementType::TextSegment)
        {
            if (child->IsAllWhiteSpace() == true)
            {
                space_children.push_back(child);
            }
        }
        else
        {
            child->Trim();
        }
    }

    for (auto child : space_children)
    {
        RemoveChild(child);
    }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CExtensibleMarkupLanguageElement</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, push technology, Extensible Markup Language, source code">
<META name="description" content="The C++ class that parses and constructs XML elements.">
</HEAD>

<BODY>

<H1>CExtensibleMarkupLanguageElement</H1>
$Revision: 84 $
<HR>

<H2>Description</H2>

This class is the work horse of the
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A>
classes. It does all of the parsing
and outputting the elements that make up an
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A>
document in a very 
simplistic manner. It does not assume that you are connected to Internet
or that networking even exists (or files for that matter). To parse an
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A>
document, you must retrieve the document. How this document is retrieved
is left to the caller of this class. This class only parses
what it is given. It will not make any attempts to retrieve data
on its own.

<H2>Construction</H2>

<DL COMPACT>
<DT><PRE>static CExtensibleMarkupLanguageElement *<B><A NAME="NewElement">NewElement</A></B>( CExtensibleMarkupLanguageElement * parent_element = nullptr, uint32_t type = typeElement, <A HREF="CExtensibleMarkupLanguageDocument.htm">CExtensibleMarkupLanguageDocument</A> * document = nullptr )</PRE><DD>
Creates another <B>CExtensibleMarkupLanguageElement</B>.
There is no constructor for this class. If you want
to create a <B>CExtensibleMarkupLanguageElement</B> object you must use <B>NewElement</B>().
The <CODE>type</CODE> parameter
can be one of the following values:<P>
<UL>
<LI><CODE><B>typeUnknown</B></CODE> - We don't know what it is.
<LI><CODE><B>typeProcessingInstruction</B></CODE> - Start Tag is &quot;&lt;?&quot; End Tag is &quot;?&gt;&quot;
<LI><CODE><B>typeComment</B></CODE> - Start Tag is &quot;&lt;!--&quot; End Tag is &quot;--&gt;&quot;
<LI><CODE><B>typeCharacterData</B></CODE> - Start Tag is &quot;&lt;![CDATA[&quot; End Tag is &quot;]]&gt;&quot;
<LI><CODE><B>typeElement</B></CODE> - A user's element
<LI><CODE><B>typeTextSegment</B></CODE> - That which lies between sub-elements
<LI><CODE><B>typeMetaData</B></CODE> -  Anything in a &quot;&lt;!&quot; field that ain't <CODE><B>typeComment</B></CODE> or <CODE><B>typeCharacterData</B></CODE>
</UL>
If the <CODE>document</CODE> parameter is NULL, the newly created
element will inherit the document of the <CODE>parent_element</CODE>.
The newly created element will also have been made a child
of <CODE>parent_element</CODE>.
</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>bool <B><A NAME="AddAttribute">AddAttribute</A></B>( const std::wstring&amp; name, const std::wstring&amp; value )</PRE><DD>
Attributes are those parts of an element that ain&#39;t supposed to be data. It
contains meta data (data about data). Sometimes though, people put data into
the attributes. Consider the following sample:
<PRE>&lt;DATE data=&quot;19630502&quot;/&gt;</PRE>
The part <CODE>data=&quot;19630502&quot;</CODE> is considered to be the attribute.
The attribute name is &quot;data&quot; and its value is &quot;19630502&quot;.
It returns true if the attribute was successfully added.

<DT><PRE>void <B><A NAME="AddChild">AddChild</A></B>( CExtensibleMarkupLanguageElement * element_p, uint32_t insert_at = (0xFFFFFFFF) )</PRE><DD>
Makes <CODE>element_p</CODE> a child of this element. A copy of
<CODE>element_p</CODE> is <STRONG>not</STRONG> made so don&#39;t send
<B>AddChild</B>() the address of a local variable. Also, the <CODE>element_p</CODE>
will have it&#39;s document member set to the document of this element.
If you set <CODE>insert_at</CODE> to be less than the number of children,
the new child will be added at that location. This allows you to insert
children at a specific location in the list (useful for pretty-text documents
but frivolous for data-centric tasks.)

<DT><PRE>bool <B><A NAME="AddText">AddText</A></B>( const std::wstring&amp; text_segment )</PRE><DD>
This method will create a child element of type <CODE><B>typeTextSegment</B></CODE>
and set its contents to <CODE>text_segment</CODE>.

<DT><PRE>void <B><A NAME="Copy">Copy</A></B>( const CExtensibleMarkupLanguageElement&amp; source )</PRE><DD>
Copies the contents of <CODE>source</CODE>. It will copy
all attributes and children of <CODE>source</CODE>.

<DT><PRE>uint32_t <B><A NAME="CountChildren">CountChildren</A></B>( const std::wstring&amp; name ) const</PRE><DD>
Returns the number of children by that name.
Consider the following
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A> snippet:
<PRE><CODE>&lt;Southpark&gt;
&lt;Characters&gt;
&lt;Boy&gt;Cartman&lt;/Boy&gt;
&lt;Boy&gt;Kenny&lt;/Boy&gt;
&lt;Boy&gt;Kyle&lt;/Boy&gt;
&lt;Boy&gt;Stan&lt;/Boy&gt;
&lt;/Characters&gt;
&lt;Characters&gt;
&lt;Girl&gt;Wendy&lt;/Girl&gt;
&lt;Boy&gt;Chef&lt;/Boy&gt;
&lt;Girl&gt;Ms. Ellen&lt;/Girl&gt;
&lt;/Characters&gt;
&lt;/Southpark&gt;</CODE></PRE>
If you wanted to know how many &quot;Girl&quot; children there
are in the second set of characters, you would use a <CODE>name</CODE>
of <CODE>&quot;SouthPark.Characters(1).Girl&quot;</CODE>
If you set the parent/child separator character (via the
<A HREF="CExtensibleMarkupLanguageDocument.htm#SetParentChildSeparatorCharacter">SetParentChildSeparatorCharacter()</A>
method) to a forward slash, you could us a <CODE>name</CODE> of
of <CODE>&quot;SouthPark/Characters(1)/Girl&quot;</CODE>

<DT><PRE>void <B><A NAME="DestroyAttributeByName">DestroyAttributeByName</A></B>( const std::wstring&amp; name )</PRE><DD>
Let's you get rid of a particular attribute.

<DT><PRE>void <B><A NAME="DestroyAttributeByValue">DestroyAttributeByValue</A></B>( const std::wstring&amp; value )</PRE><DD>
Let's you get rid of all attributes with a particular value.

<DT><PRE>void <B><A NAME="DestroyAttributes">DestroyAttributes</A></B>( void )</PRE><DD>
Removes all of the attributes associated with this element.

<DT><PRE>void <B><A NAME="DestroyChildren">DestroyChildren</A></B>( void )</PRE><DD>
Removes all children (this will destroy all grand children too).

<DT><PRE>void <B><A NAME="Dump">Dump</A></B>( CDumpContext&amp; dump_context ) const</PRE><DD>
Present only in debug builds of WFC. It helps in debugging. It will
print the contents of this object in human readable form to <CODE>dump_context</CODE>.

<DT><PRE>void <B><A NAME="Empty">Empty</A></B>( void )</PRE><DD>
Destroys all children and attributes. Empties the tag name and text fields.
It does <STRONG>not</STRONG> reset the parent document
(<B><A HREF="#SetDocument">SetDocument</A></B>())
pointer nor the parent element pointer.

<DT><PRE>bool <B><A NAME="EnumerateAttributes">EnumerateAttributes</A></B>( uint32_t&amp; enumerator ) const</PRE><DD>
Allows you to enumerate through the attributes. It will initialize
<CODE>enumerator</CODE> and return true if there are any attributes
to enumerate through. It will return false when the number of attributes
is zero (i.e. there ain't no attributes to enumerate).

<DT><PRE>bool <B><A NAME="EnumerateChildren">EnumerateChildren</A></B>( uint32_t&amp; enumerator ) const</PRE><DD>
Allows you to enumerate through the children. It will initialize
<CODE>enumerator</CODE> and return true if there are any children
to enumerate through. It will return false when the number of children
is zero (i.e. there ain't no children to enumerate).

<DT><PRE>bool <B><A NAME="ForEach">ForEach</A></B>( LPCTSTR name, XML_ELEMENT_CALLBACK callback, void * callback_context ) const</PRE><DD>
I got sick and tired of constantly writing code to enumerate child elements
of a particular name and process them. To make life easier, I added <CODE>ForEach</CODE> which
wraps that logic. It will call <CODE>callback</CODE> for every child element with a tag
that matches <CODE>name</CODE>. It uses the standard callback function mechanism, if you want
to stop the enumeration, simply call <A HREF="#SetAbortParsing"><B>SetAbortParsing</B></A>.

<DT><PRE>bool <B><A NAME="GetAbortParsing">GetAbortParsing</A></B>( void ) const</PRE><DD>
Returns whether you should abort parsing or not. This method is here
so you can abort the parsing of an
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A>
stream whenever a callback function deems it necessary.

<DT><PRE>bool <B><A NAME="GetAttributeByName">GetAttributeByName</A></B>( <A HREF="CExtensibleMarkupLanguageAttribute.htm">CExtensibleMarkupLanguageAttribute</A>&amp; attribute ) const
bool <B>GetAttributeByName</B>( const std::wstring&amp; name, std::wstring&amp; value ) const
<A HREF="CExtensibleMarkupLanguageAttribute.htm">CExtensibleMarkupLanguageAttribute</A> * <B>GetAttributeByName</B>( const std::wstring&amp; name ) const</PRE><DD>
Retrieves the value of an attribute based on its name. If you retrieve the
<CODE>value</CODE>, you will get the attribute value after all entities
in the string have been resolved. If you retrieve the entire attribute
(in a CExtensibleMarkupLanguageAttribute), the value will not pass through
the entity resolution filter. The reason for this is there is no link between a
<A HREF="CExtensibleMarkupLanguageAttribute.htm">CExtensibleMarkupLanguageAttribute</A> and a
<A HREF="CExtensibleMarkupLanguageDocument.htm">CExtensibleMarkupLanguageDocument</A>.
Only the <A HREF="CExtensibleMarkupLanguageDocument.htm">CExtensibleMarkupLanguageDocument</A>
knows about entities.

<DT><PRE>bool <B><A NAME="GetAttributeByValue">GetAttributeByValue</A></B>( <A HREF="CExtensibleMarkupLanguageAttribute.htm">CExtensibleMarkupLanguageAttribute</A>&amp; attribute ) const</PRE><DD>
Retrieves a copy of an attribute by its value.

<DT><PRE>void <B><A NAME="GetBeginning">GetBeginning</A></B>( <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point ) const</PRE><DD>
Fills <CODE>parse_point</CODE> with the point at which this element
began in the data stream.

<DT><PRE>CExtensibleMarkupLanguageElement * <B><A NAME="GetChild">GetChild</A></B>( const std::wstring&amp; child_name ) const</PRE><DD>
Returns the child of the given <CODE>child_name</CODE>.
Consider the following
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A> snippet:
<PRE><CODE>&lt;Southpark&gt;
&lt;Characters&gt;
&lt;Boy&gt;Cartman&lt;/Boy&gt;
&lt;Boy&gt;Kenny&lt;/Boy&gt;
&lt;Boy&gt;Kyle&lt;/Boy&gt;
&lt;Boy&gt;Stan&lt;/Boy&gt;
&lt;/Characters&gt;
&lt;Characters&gt;
&lt;Girl&gt;Wendy&lt;/Girl&gt;
&lt;Boy&gt;Chef&lt;/Boy&gt;
&lt;Girl&gt;Ms. Ellen&lt;/Girl&gt;
&lt;/Characters&gt;
&lt;/Southpark&gt;</CODE></PRE>
To retrieve the element for Cartman, <CODE>child_name</CODE> should
be &quot;Southpark.Characters.Boy&quot; If you want Ms. Ellen (even
though she doesn&#39;t play for the home team) you would use
&quot;Southpark.Characters(1).Girl(1)&quot;
If you set the parent/child separator character (via the
<A HREF="CExtensibleMarkupLanguageDocument.htm#SetParentChildSeparatorCharacter">SetParentChildSeparatorCharacter()</A>
method) to a forward slash, you could us a <CODE>child_name</CODE> of
of &quot;Southpark/Characters(1)/Girl(1)&quot;

<DT><PRE>void <B><A NAME="GetCompleteName">GetCompleteName</A></B>( std::wstring&amp; name ) const</PRE><DD>
This is the inverse to <B><A HREF="#GetChild">GetChild</A></B>().
It will give you the complete path name for this element.

<DT><PRE>void <B><A NAME="GetContents">GetContents</A></B>( std::wstring&amp; contents ) const</PRE><DD>
Retrieves the contents of this element only. It will not resolve
any entities nor will it retrieve the contents of any children elements.
Returns the data (or contents) portion of the element. For example, if the
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A> element was:
<PRE>&lt;LINE n=&quot;1&quot;&gt;Darmok and Jilad at Tenagara!&lt;/LINE&gt;</PRE>
<B>GetContents</B>() would fill <CODE>contents</CODE> with 
<STRONG>NOTHING!</STRONG> If you want to retrieve 
&quot;Darmok and Jilad at Tenagara!&quot; you will have to call
<B><A HREF="#GetText">GetText</A></B>(). The reason for this is the parser has to support
embedded elements in the text portion of this element. Consider the following:
<PRE>&lt;LINE n=&quot;1&quot;&gt;Darmok and &lt;ITALICS&gt;Jilad&lt;/ITALICS&gt; at Tenagara!&lt;/LINE&gt;</PRE>
Notice there is an embedded element named <CODE>ITALICS</CODE> right in
the middle of perfectly good text.

<DT><PRE><A HREF="CExtensibleMarkupLanguageDocument.htm">CExtensibleMarkupLanguageDocument</A> * <B><A NAME="GetDocument">GetDocument</A></B>( void ) const</PRE><DD>
Returns the pointer to the parent document.

<DT><PRE>void <B><A NAME="GetEnding">GetEnding</A></B>( <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point ) const</PRE><DD>
Fills <CODE>parse_point</CODE> with the point at which this element
ended in the data stream.

<DT><PRE>void <B><A NAME="GetName">GetName</A></B>( std::wstring&amp; name ) const</PRE><DD>
Gives you the name (and possibly instance number) of this element.

<DT><PRE>bool <B><A NAME="GetNextAttribute">GetNextAttribute</A></B>( uint32_t&amp; enumerator, <A HREF="CExtensibleMarkupLanguageAttribute.htm">CExtensibleMarkupLanguageAttribute</A>*&amp; attribute_p ) const</PRE><DD>
Returns true if <CODE>attribute_p</CODE> was filled with an attribute&#39;s pointer.
Returns false (and sets <CODE>attribute_p</CODE> to NULL) when no
attribute has been retrieved.

<DT><PRE>bool <B><A NAME="GetNextChild">GetNextChild</A></B>( uint32_t&amp; enumerator, CExtensibleMarkupLanguageElement *&amp; element_p ) const</PRE><DD>
Returns true if <CODE>element_p</CODE> was filled with a child's pointer.
Returns false (and sets <CODE>element_p</CODE> to NULL) when no
child has been retrieved.

<DT><PRE>uint32_t <B><A NAME="GetNumberOfChildren">GetNumberOfChildren</A></B>( void ) const</PRE><DD>
Returns the number of children this element has. Consider the following
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A>
snippet:
<PRE><CODE>&lt;Southpark&gt;
&lt;Characters&gt;
&lt;Boy&gt;Cartman&lt;/Boy&gt;
&lt;Boy&gt;Kenny&lt;/Boy&gt;
&lt;Boy&gt;Kyle&lt;/Boy&gt;
&lt;Boy&gt;Stan&lt;/Boy&gt;
&lt;/Characters&gt;
&lt;Characters&gt;
&lt;Girl&gt;Wendy&lt;/Girl&gt;
&lt;Boy&gt;Chef&lt;/Boy&gt;
&lt;Girl&gt;Ms. Ellen&lt;/Girl&gt;
&lt;/Characters&gt;
&lt;/Southpark&gt;</CODE></PRE>
If you have the element for Southpark and call <B>GetNumberOfChildren</B>(), it
will return 

<DT><PRE>uint32_t <B><A NAME="GetNumberOfAttributes">GetNumberOfAttributes</A></B>( void ) const</PRE><DD>
Returns the number of attributes this element has.

<DT><PRE>CExtensibleMarkupLanguageElement * <B><A NAME="GetParent">GetParent</A></B>( void ) const</PRE><DD>
Returns the pointer to the immediate parent of this element.

<DT><PRE>CExtensibleMarkupLanguageElement * <B>GetParent</B>( const std::wstring&amp; name ) const</PRE><DD>
Returns the parent element that matches <CODE>name</CODE>. The element
returned my be the parent, grand parent or great grand parent.

<DT><PRE>void <B><A NAME="GetTag">GetTag</A></B>( std::wstring&amp; tag ) const</PRE><DD>
Returns the tag value. For example, if the
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A> element was:
<PRE>&lt;DATE data=&quot;1963-05-02&quot;/&gt;</PRE>
<B>GetTag</B>() will return &quot;DATE&quot;

<DT><PRE>void <B><A NAME="GetText">GetText</A></B>( std::wstring&amp; text_string ) const</PRE><DD>
Retrieves all text segments and resolves all entities in the text.
This parser is very very particular about
whether white-space is holy or not.
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A>
is for data so we will treat every character in the
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A>
document as though it is data. Consider the following:
<PRE>&lt;WFC&gt;
&lt;AUTHOR&gt;Samuel R. Blackburn
wfc@pobox.com&lt;/AUTHOR&gt;
&lt;/WFC&gt;</PRE>
The WFC element has <I>three</I> children. The first child
is of type <CODE><B>typeTextSegment</B></CODE>
and contains the data between the end of the <CODE>&lt;WFC&gt;</CODE>
tag and the beginning of the <CODE>&lt;AUTHOR&gt;</CODE> tag (i.e.
a new-line character).
The next child is of type <CODE><B>typeElement</B></CODE>
which contains the AUTHOR element. This last child
is of type <CODE><B>typeTextSegment</B></CODE>
which contains the data between the end of the
<CODE>&lt;/AUTHOR&gt;</CODE> tag and the beginning of the
<CODE>&lt;/WFC&gt;</CODE> tag (another new-line character).

<BR>

<STRONG>NOTE:</STRONG> There is one thing you must keep in mind
when calling <B>GetText</B>(). Remember that we have a pointer to our parent
<A HREF="CExtensibleMarkupLanguageDocument.htm">CExtensibleMarkupLanguageDocument</A>.
That parent has a property called <B>IgnoreWhiteSpace</B> and is boolean.
If the user sets that property to true, <B>GetText</B>() will ignore
text segments (<CODE><B>typeTextSegment</B></CODE> child elements) that
contain nothing but space characters.

<DT><PRE>uint32_t <B><A NAME="GetTotalNumberOfChildren">GetTotalNumberOfChildren</A></B>( void ) const</PRE><DD>
This tells you how many children this element has plus the number
of children they have and so on and so forth.

<DT><PRE>uint32_t <B><A NAME="GetType">GetType</A></B>( void ) const</PRE><DD>
Tells you what type of element this object is. It will return
one of the following values:
<UL>
<LI><CODE><B>typeUnknown</B></CODE> - We don't know what it is.
<LI><CODE><B>typeProcessingInstruction</B></CODE> - Start Tag is &quot;&lt;?&quot; End Tag is &quot;?&gt;&quot;
<LI><CODE><B>typeComment</B></CODE> - Start Tag is &quot;&lt;!--&quot; End Tag is &quot;--&gt;&quot;
<LI><CODE><B>typeCharacterData</B></CODE> - Start Tag is &quot;&lt;![CDATA[&quot; End Tag is &quot;]]&gt;&quot;
<LI><CODE><B>typeElement</B></CODE> - A user's element
<LI><CODE><B>typeTextSegment</B></CODE> - That which lies between sub-elements
<LI><CODE><B>typeMetaData</B></CODE> -  Anything in a &quot;&lt;!&quot; field that ain't <CODE><B>typeComment</B></CODE> or <CODE><B>typeCharacterData</B></CODE>
</UL>

<DT><PRE>bool <B><A NAME="IsAllWhiteSpace">IsAllWhiteSpace</A></B>( void ) const</PRE><DD>
Returns true if this is a <CODE><B>typeTextSegment</B></CODE> and
it contains nothing but space-like characters.

<DT><PRE>bool <B><A NAME="IsRoot">IsRoot</A></B>( void ) const</PRE><DD>
Returns true if this element is the root element. The root element
holds the data from the
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A> identifier.
The <A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A> identifier line
looks something like this:
<PRE>&lt;?xml version=&quot;1.0&quot;?&gt;</PRE>

<DT><PRE>static CExtensibleMarkupLanguageElement *<B>NewChild</B>( LPCTSTR tag_name = nullptr, uint32_t type = typeElement )</PRE><DD>
Creates a new child for this element and sets the tag name.
The <CODE>type</CODE> parameter can be one of the following values:
<P>

<UL>
<LI><CODE><B>typeUnknown</B></CODE> - We don't know what it is.
<LI><CODE><B>typeProcessingInstruction</B></CODE> - Start Tag is &quot;&lt;?&quot; End Tag is &quot;?&gt;&quot;
<LI><CODE><B>typeComment</B></CODE> - Start Tag is &quot;&lt;!--&quot; End Tag is &quot;--&gt;&quot;
<LI><CODE><B>typeCharacterData</B></CODE> - Start Tag is &quot;&lt;![CDATA[&quot; End Tag is &quot;]]&gt;&quot;
<LI><CODE><B>typeElement</B></CODE> - A user's element
<LI><CODE><B>typeTextSegment</B></CODE> - That which lies between sub-elements
<LI><CODE><B>typeMetaData</B></CODE> -  Anything in a &quot;&lt;!&quot; field that ain't <CODE><B>typeComment</B></CODE> or <CODE><B>typeCharacterData</B></CODE>
</UL>

<DT><PRE>static CExtensibleMarkupLanguageElement *<B>NewElement</B>( CExtensibleMarkupLanguageElement * parent_element = nullptr, uint32_t type = typeElement, <A HREF="CExtensibleMarkupLanguageDocument.htm">CExtensibleMarkupLanguageDocument</A> * document = NULL )</PRE><DD>
Creates another <B>CExtensibleMarkupLanguageElement</B>.
There is no constructor for CExtensibleMarkupLanguageElement. If you want
to create a CExtensibleMarkupLanguageElement you must use <B>NewElement</B>().
The <CODE>type</CODE> parameter can be one of the following values:

<P>

<UL>
<LI><CODE><B>typeUnknown</B></CODE> - We don't know what it is.
<LI><CODE><B>typeProcessingInstruction</B></CODE> - Start Tag is &quot;&lt;?&quot; End Tag is &quot;?&gt;&quot;
<LI><CODE><B>typeComment</B></CODE> - Start Tag is &quot;&lt;!--&quot; End Tag is &quot;--&gt;&quot;
<LI><CODE><B>typeCharacterData</B></CODE> - Start Tag is &quot;&lt;![CDATA[&quot; End Tag is &quot;]]&gt;&quot;
<LI><CODE><B>typeElement</B></CODE> - A user's element
<LI><CODE><B>typeTextSegment</B></CODE> - That which lies between sub-elements
<LI><CODE><B>typeMetaData</B></CODE> -  Anything in a &quot;&lt;!&quot; field that ain't <CODE><B>typeComment</B></CODE> or <CODE><B>typeCharacterData</B></CODE>
</UL>
If the <CODE>document</CODE> parameter is NULL, the newly created element
will inherit the document from the <CODE>parent_element</CODE>.

<DT><PRE>bool <B><A NAME="Parse">Parse</A></B>( const <A HREF="CParsePoint.htm">CParsePoint</A>&amp; beginning, const <A HREF="CDataParser.htm">CDataParser</A>&amp; parser )</PRE><DD>
Tells the element to start read itself from <CODE>parser</CODE>
starting at the location in <CODE>beginning</CODE>. It returns true if it successfully
parsed (and all of its children successfully parsed).

<DT><PRE>void <B><A NAME="RemoveChild">RemoveChild</A></B>( CExtensibleMarkupLanguageElement * element_p )</PRE><DD>
Removes <CODE>element_p</CODE> from the list of children. It will not
destroy the child. This is the method you would use to steal a child
from one element and give it to another.

<DT><PRE>void <B><A NAME="SetAbortParsing">SetAbortParsing</A></B>( bool abort_parsing = true )</PRE><DD>
This sets a flag in the class to abort parsing. This is usually
called from callback functions. When an element is finished parsing,
it is sent to the document's list of callback functions. One of those
callbacks may choose to abort parsing of the document. One example of
why you would want to do this is searching. You would parse the document
until you found what you're looking for then stop parsing the document
(because the document may be very large).

<DT><PRE>void <B><A NAME="SetDocument">SetDocument</A></B>( <A HREF="CExtensibleMarkupLanguageDocument.htm">CExtensibleMarkupLanguageDocument</A> * document_p )</PRE><DD>
Tells the element which
<A HREF="CExtensibleMarkupLanguageDocument.htm">CExtensibleMarkupLanguageDocument</A>
this element belongs to.

<DT><PRE>void <B><A NAME="SetTag">SetTag</A></B>( const std::wstring&amp; tag_name )</PRE><DD>
Sets the tag name.

<DT><PRE>void <B><A NAME="SetType">SetType</A></B>( uint32_t element_type )</PRE><DD>
<CODE>element_type</CODE> can be one of the following values:

<P>

<UL>
<LI><CODE><B>typeUnknown</B></CODE> - We don't know what it is.
<LI><CODE><B>typeProcessingInstruction</B></CODE> - Start Tag is &quot;&lt;?&quot; End Tag is &quot;?&gt;&quot;
<LI><CODE><B>typeComment</B></CODE> - Start Tag is &quot;&lt;!--&quot; End Tag is &quot;--&gt;&quot;
<LI><CODE><B>typeCharacterData</B></CODE> - Start Tag is &quot;&lt;![CDATA[&quot; End Tag is &quot;]]&gt;&quot;
<LI><CODE><B>typeElement</B></CODE> - A user's element
<LI><CODE><B>typeTextSegment</B></CODE> - That which lies between sub-elements
<LI><CODE><B>typeMetaData</B></CODE> -  Anything in a &quot;&lt;!&quot; field that ain't <CODE><B>typeComment</B></CODE> or <CODE><B>typeCharacterData</B></CODE>
</UL>

<P>

If <CODE>element_type</CODE> is not one of the above values, the type
will be set to <CODE><B>typeUnknown</B></CODE>.

<DT><PRE>void <B><A NAME="WriteTo">WriteTo</A></B>( std::vector&lt;uint8_t&gt;&amp; destination )</PRE><DD>
Writes the element (and sub-elements) to the byte array in
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A> form.

</DL>

<H2>Operators</H2>

<DT><PRE>CExtensibleMarkupLanguageElement&amp; operator <B>=</B> ( const CExtensibleMarkupLanguageElement&amp; source )</PRE><DD>
Basically calls <B><A HREF="#Copy">Copy</A></B>().

<H2>Example</H2><PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

bool contains_id_attribute( <B>CExtensibleMarkupLanguageElement</B> * element_p )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;contains_id_attribute()&quot; ) );

<A HREF="CExtensibleMarkupLanguageAttribute.htm">CExtensibleMarkupLanguageAttribute</A> * attribute_p = nullptr;

attribute_p = element_p-&gt;<A HREF="#GetAttributeByName">GetAttributeByName</A>( &quot;ID&quot; );

if ( attribute_p == nullptr )
{
return( false );
}

return( true );
}

struct SUM_TOTAL
{
   int64_t total{0};
};

void <A TITLE="This is an XML_ELEMENT_CALLBACK function.">entry_callback</A>( void * context, <B>CExtensibleMarkupLanguageElement</B> * element_p )
{
auto sum_p { (SUM_TOTAL_P) context};

std::wstring text;

element_p-&gt;GetText( text );

sum_p-&gt;total += atoi( text );
}

int64_t calculate_total( <B>CExtensibleMarkupLanguageElement</B> * data_p )
{
// Time to total up the expenditures (in elements named EXPEND)

SUM_TOTAL sum;

sum.total = 0;

data_p-&gt;ForEach( TEXT( &quot;EXPEND&quot; ), entry_callback, &amp;sum );

return( sum.total );
}


</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CExtensibleMarkupLanguageElement.cpp $<BR>
$Modtime: 1/05/02 11:19a $

</BODY>
</HTML>
The following line should go in AUTOEXP.DAT so the debugging tooltips will format properly
ToolTipFormatLine=CExtensibleMarkupLanguageElement =<m_Tag,s>
*/
