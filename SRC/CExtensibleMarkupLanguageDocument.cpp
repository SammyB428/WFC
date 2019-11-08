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
** $Workfile: CExtensibleMarkupLanguageDocument.cpp $
** $Revision: 50 $
** $Modtime: 8/30/01 7:21p $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

#include <VersionHelpers.h>

USING_WFC_NAMESPACE

CExtensibleMarkupLanguageDocument::CExtensibleMarkupLanguageDocument() noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Initialize in the same order as in the header file. This makes it
   // easier to debug.

   m_XML = nullptr;

   // m_Callbacks is an object and doesn't need initializing
   // m_Entities is an object and doesn't need initializing
   m_IgnoreWhiteSpace     = false;
   m_AutomaticIndentation = false;
   m_IndentationLevel     = 0;
   m_IndentBy             = 0;
   m_ParseOptions         = WFC_XML_IGNORE_MISSING_XML_DECLARATION;
   m_WriteOptions         = WFC_XML_WRITE_AS_ASCII;
   m_IsStandalone         = true;
   m_UseNamespace         = false;
   m_ParentChildSeparatorCharacter = '.';

   // 1998-08-27 Thanks go to Christopher Remington (chrisr@silversoft.net)
   // for finding where I wasn't intializing m_ParseErrorEncountered.

   m_ParseErrorEncountered = false;

   // m_ErrorElementBeganAt is an object and doesn't need initializing
   // m_ErrorOccuredAt is an object and doesn't need initializing
   // m_ErrorTagName is an object and doesn't need initializing
   // m_ErrorMessage is an object and doesn't need initializing

   m_ConversionCodePage = CP_UTF8;

   if (IsWindowsXPOrGreater() == false)
   {
       // Piece of crap Windows95 garbage
       m_ConversionCodePage = CP_ACP;
   }

   NumberOfBytesPerCharacter = 1;

   m_InitializeEntities();
   m_InitializeRootElement();
}

CExtensibleMarkupLanguageDocument::CExtensibleMarkupLanguageDocument( _In_ CExtensibleMarkupLanguageDocument const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Initialize in the same order as in the header file. This makes it
   // easier to debug.

   m_XML = nullptr;

   // m_Callbacks is an object and doesn't need initializing
   // m_Entities is an object and doesn't need initializing
   m_IgnoreWhiteSpace     = false;
   m_AutomaticIndentation = false;
   m_IndentationLevel     = 0;
   m_IndentBy             = 0;
   m_ParseOptions         = WFC_XML_IGNORE_MISSING_XML_DECLARATION;
   m_WriteOptions         = WFC_XML_WRITE_AS_ASCII;
   m_IsStandalone         = true;
   m_UseNamespace         = false;
   m_ParentChildSeparatorCharacter = TEXT( '.' );

   // 1998-08-27 Thanks go to Christopher Remington (chrisr@silversoft.net)
   // for finding where I wasn't intializing m_ParseErrorEncountered.

   m_ParseErrorEncountered = false;

   // m_ErrorElementBeganAt is an object and doesn't need initializing
   // m_ErrorOccuredAt is an object and doesn't need initializing
   // m_ErrorTagName is an object and doesn't need initializing
   // m_ErrorMessage is an object and doesn't need initializing

   m_ConversionCodePage = CP_UTF8;

   if (IsWindowsXPOrGreater() == false)
   {
       // Piece of crap Windows95 garbage
       m_ConversionCodePage = CP_ACP;
   }

   m_InitializeEntities();
   m_InitializeRootElement();

   // Now that we have a valid object, let's copy someone.

   Copy( source );
}

CExtensibleMarkupLanguageDocument::~CExtensibleMarkupLanguageDocument()
{
   WFC_VALIDATE_POINTER( this );

   Empty();

   m_IgnoreWhiteSpace      = false;
   m_AutomaticIndentation  = false;
   m_IndentationLevel      = 0;
   m_IndentBy              = 0;
   m_ParseErrorEncountered = false;
   m_ParseOptions          = 0;
   m_WriteOptions          = WFC_XML_WRITE_AS_ASCII;
   m_IsStandalone          = false;
   m_UseNamespace          = false;
   m_ConversionCodePage    = 0;

   if ( m_XML != nullptr )
   {
      CExtensibleMarkupLanguageElement::DeleteElement( m_XML );
      m_XML = nullptr;
   }
}

_Check_return_ bool CExtensibleMarkupLanguageDocument::AddCallback( _In_ std::wstring_view element_name, __callback XML_ELEMENT_CALLBACK callback, __inout void * callback_parameter ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( callback );

   if ( element_name.empty() == true )
   {
      //WFCTRACE( TEXT( "Nameless element callbacks not supported" ) );
      return( false );
   }

   if ( callback == nullptr )
   {
      //WFCTRACE( TEXT( "NULL Callback not added" ) );
      return( false );
   }

   // Now go find this entry (to make sure we don't call it twice)

   for( auto& entry : m_Callbacks )
   {
      if ( entry.callback  == callback and
           entry.parameter == callback_parameter and
           element_name.compare( entry.name ) == I_AM_EQUAL_TO_THAT)
      {
         return( false );
      }
   }

   // If we get here, it means we have a new entry for the list

   XML_ELEMENT_CALLBACK_ENTRY new_entry;

   new_entry.callback  = callback;
   new_entry.parameter = callback_parameter;
   new_entry.name.assign(element_name);

   m_Callbacks.push_back(new_entry);

   return( true );
}

_Check_return_ bool CExtensibleMarkupLanguageDocument::AddEntity( _In_ std::wstring_view entity, _In_ std::wstring_view value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // First, we must check to see if we allow default entities to be replaced...
   // The default entities are:
   // &amp;
   // &apos;
   // &gt;
   // &lt;
   // &quot;

   if (is_flagged( m_ParseOptions, WFC_XML_ALLOW_REPLACEMENT_OF_DEFAULT_ENTITIES ) == true )
   {
      // We don't care if default entities are replaced 
      if ( m_Entities.Add( entity, value ) == false )
      {
         return( false );
      }

      return( true );
   }
   else
   {
      // We must check to make sure the default entities aren't replaced

      if ( entity.compare(WSTRING_VIEW(L"&amp;")) == I_AM_EQUAL_TO_THAT or
           entity.compare(WSTRING_VIEW(L"&apos;")) == I_AM_EQUAL_TO_THAT or
           entity.compare(WSTRING_VIEW(L"&gt;")) == I_AM_EQUAL_TO_THAT or
           entity.compare(WSTRING_VIEW(L"&lt;")) == I_AM_EQUAL_TO_THAT or
           entity.compare(WSTRING_VIEW(L"&quot;")) == I_AM_EQUAL_TO_THAT)
      {
         // The user tried to replace a default entity
         return( false );
      }
      else
      {
         if ( m_Entities.Add( entity, value ) == false )
         {
            return( false );
         }

         return( true );
      }
   }
}

void CExtensibleMarkupLanguageDocument::Append( _In_ CExtensibleMarkupLanguageDocument const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_XML == nullptr or source.m_XML == nullptr )
   {
      //WFCTRACE( TEXT( "an m_XML is NULL!" ) );
      return;
   }

   std::size_t enumerator = 0;

   if ( source.m_XML->EnumerateChildren( enumerator ) == true )
   {
       CExtensibleMarkupLanguageElement * element_to_copy_p = nullptr;

       while( source.m_XML->GetNextChild( enumerator, element_to_copy_p ) == true )
       {
         if ( element_to_copy_p != nullptr )
         {
            auto element_to_add_p = CExtensibleMarkupLanguageElement::NewElement( m_XML );

            if ( element_to_add_p == nullptr )
            {
               //WFCTRACE( TEXT( "Can't create element!" ) );
               return;
            }

            element_to_add_p->Copy( *element_to_copy_p );

            m_XML->AddChild( element_to_add_p, false );
         }
      }
   }
}

void CExtensibleMarkupLanguageDocument::Copy( _In_ CExtensibleMarkupLanguageDocument const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Empty();

   if ( m_XML == nullptr )
   {
      m_XML = CExtensibleMarkupLanguageElement::NewElement( nullptr, CExtensibleMarkupLanguageElement::ElementType::ProcessingInstruction, this );

      if ( m_XML == nullptr )
      {
         //WFCTRACE( TEXT( "Can't allocate memory for the document node" ) );
         return;
      }
   }

   if ( source.m_XML != nullptr )
   {
      m_XML->Copy( *source.m_XML );
   }
}

void CExtensibleMarkupLanguageDocument::CopyCallbacks( _In_ CExtensibleMarkupLanguageDocument const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_RemoveAllCallbacks();

   for( auto const& entry : m_Callbacks )
   {
       // 2000-10-18
       // Thanks go to Larry Bredehoeft (L.Bredehoeft@imandi.com) for finding a bug here
      (void )AddCallback( entry.name, entry.callback, entry.parameter );
   }
}

struct CONTAINS_ELEMENT_NAME
{
    bool found{ false };
    std::wstring_view desired_name;
};

void find_element_name( void * parameter, CExtensibleMarkupLanguageElement * element_p ) noexcept
{
    auto context = static_cast<CONTAINS_ELEMENT_NAME *>( parameter );

    if ( context->found == false )
    {
        if ( compare_no_case( context->desired_name, element_p->Tag() ) == 0 )
        {
            context->found = true;
        }
    }
}

_Check_return_ bool CExtensibleMarkupLanguageDocument::ContainsElementName( _In_ std::wstring_view name ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   CONTAINS_ELEMENT_NAME context;

   context.desired_name = name;
   context.found = false;

   if ( m_XML != nullptr )
   {
      (void) m_XML->ForAny( name, find_element_name, &context );

      return( context.found );
   }

   return( false );
}

_Check_return_ std::size_t CExtensibleMarkupLanguageDocument::CountElements( _In_ std::wstring_view name ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_XML != nullptr )
   {
      return( m_XML->CountChildren( name ) );
   }

   return( 0 );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CExtensibleMarkupLanguageDocument::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );
   dump_context << TEXT( " a CExtensibleMarkupLanguageDocument at " ) << (VOID *) this << TEXT( "\n{\n" );

   dump_context << TEXT( "      m_IgnoreWhiteSpace is " );

   if ( m_UseNamespace == false )
   {
      dump_context << TEXT( "false\n" );
   }
   else
   {
      dump_context << TEXT( "true\n" );
   }

   dump_context << TEXT( "      m_Namespace is " );
   dump_context << m_Namespace;

   dump_context << TEXT( "      m_IgnoreWhiteSpace is " );

   if ( m_IgnoreWhiteSpace == false )
   {
      dump_context << TEXT( "false\n" );
   }
   else
   {
      dump_context << TEXT( "true\n" );
   }

   dump_context << TEXT( "      m_AutomaticIndentation is " );

   if ( m_AutomaticIndentation == false )
   {
      dump_context << TEXT( "false\n" );
   }
   else
   {
      dump_context << TEXT( "true\n" );
   }

   dump_context << TEXT( "   m_IndentationLevel is " ) << m_IndentationLevel << TEXT( "\n" );
   dump_context << TEXT( "   m_IndentBy         is " ) << m_IndentBy         << TEXT( "\n" );

   dump_context << TEXT( "   m_XML" );

   if ( m_XML == nullptr )
   {
      dump_context << TEXT( " is NULL\n" );
   }
   else
   {
      m_XML->Dump( dump_context );
   }

   dump_context << TEXT( "   m_ParseErrorEncountered is " );

   if ( m_ParseErrorEncountered == false )
   {
      dump_context << TEXT( "false\n" );
   }
   else
   {
      dump_context << TEXT( "true\n" );
   }

   dump_context << TEXT( "   m_ParseOptions is " ) << m_ParseOptions << TEXT( "\n" );

   if ( m_ParseOptions > 0 )
   {
      // Some parsing options have been set. Let's break out the bit fields

      dump_context << TEXT( "   {\n" );

      if ( m_ParseOptions bitand WFC_XML_IGNORE_CASE_IN_XML_DECLARATION )
      {
         dump_context << TEXT( "      WFC_XML_IGNORE_CASE_IN_XML_DECLARATION\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_ALLOW_REPLACEMENT_OF_DEFAULT_ENTITIES )
      {
         dump_context << TEXT( "      WFC_XML_ALLOW_REPLACEMENT_OF_DEFAULT_ENTITIES\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_FAIL_ON_ILL_FORMED_ENTITIES )
      {
         dump_context << TEXT( "      WFC_XML_FAIL_ON_ILL_FORMED_ENTITIES\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_IGNORE_ALL_WHITE_SPACE_ELEMENTS )
      {
         dump_context << TEXT( "      WFC_XML_IGNORE_ALL_WHITE_SPACE_ELEMENTS\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_IGNORE_MISSING_XML_DECLARATION )
      {
         dump_context << TEXT( "      WFC_XML_IGNORE_MISSING_XML_DECLARATION\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_DISALLOW_MULTIPLE_ELEMENTS )
      {
         dump_context << TEXT( "      WFC_XML_DISALLOW_MULTIPLE_ELEMENTS\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_LOOSE_COMMENT_PARSING )
      {
         dump_context << TEXT( "      WFC_XML_LOOSE_COMMENT_PARSING\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_ALLOW_AMPERSANDS_IN_ELEMENTS )
      {
         dump_context << TEXT( "      WFC_XML_ALLOW_AMPERSANDS_IN_ELEMENTS\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_FORCE_AT_LEAST_ONE_ELEMENT_MUST_BE_PRESENT )
      {
         dump_context << TEXT( "      WFC_XML_FORCE_AT_LEAST_ONE_ELEMENT_MUST_BE_PRESENT\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_CHECK_ENTITIES_FOR_ILLEGAL_CHARACTERS )
      {
         dump_context << TEXT( "      WFC_XML_CHECK_ENTITIES_FOR_ILLEGAL_CHARACTERS\n" );
      }

      dump_context << TEXT( "   }\n" );
   }

   dump_context << TEXT( "   m_WriteOptions is " ) << m_WriteOptions << TEXT( "\n" );

   if ( m_WriteOptions > 0 )
   {
      // Some parsing options have been set. Let's break out the bit fields

      dump_context << TEXT( "   {\n" );

      if ( m_ParseOptions bitand WFC_XML_INCLUDE_TYPE_INFORMATION )
      {
         dump_context << TEXT( "      WFC_XML_INCLUDE_TYPE_INFORMATION\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_DONT_OUTPUT_XML_DECLARATION )
      {
         dump_context << TEXT( "      WFC_XML_DONT_OUTPUT_XML_DECLARATION\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_WRITE_AS_UNICODE )
      {
         dump_context << TEXT( "      WFC_XML_WRITE_AS_UNICODE\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_WRITE_AS_ASCII )
      {
         dump_context << TEXT( "      WFC_XML_WRITE_AS_ASCII\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_WRITE_AS_UCS4 )
      {
         dump_context << TEXT( "      WFC_XML_WRITE_AS_UCS4\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_2143 )
      {
         dump_context << TEXT( "      WFC_XML_WRITE_AS_UCS4_UNUSUAL_2143\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_3412 )
      {
         dump_context << TEXT( "      WFC_XML_WRITE_AS_UCS4_UNUSUAL_3412\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_WRITE_AS_UTF8 )
      {
         dump_context << TEXT( "      WFC_XML_WRITE_AS_UTF8\n" );
      }

      if ( m_ParseOptions bitand WFC_XML_WRITE_AS_UTF7 )
      {
         dump_context << TEXT( "      WFC_XML_WRITE_AS_UTF7\n" );
      }

      dump_context << TEXT( "   }\n" );
   }

   dump_context << TEXT( "   m_Version      is " ) << m_Version  << TEXT( "\n" );
   dump_context << TEXT( "   m_Encoding     is " ) << m_Encoding << TEXT( "\n" );
   dump_context << TEXT( "   m_IsStandalone is " );

   if ( m_IsStandalone == false )
   {
      dump_context << TEXT( "false\n" );
   }
   else
   {
      dump_context << TEXT( "true\n" );
   }

   dump_context << TEXT( "   m_ParentChildSeparatorCharacter is " ) << m_ParentChildSeparatorCharacter << TEXT( "\n" );
   dump_context << TEXT( "   m_ErrorTagName is " ) << m_ErrorTagName << TEXT( "\n" );

   dump_context << TEXT( "   m_ErrorElementBeganAt is " );
   //m_ErrorElementBeganAt.Dump( dump_context );

   dump_context << TEXT( "   m_ErrorOccuredAt is " );
   //m_ErrorOccuredAt.Dump( dump_context );

   dump_context << TEXT( "   m_ErrorMessage is " ) << m_ErrorMessage << TEXT( "\n" );

   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void CExtensibleMarkupLanguageDocument::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_RemoveAllCallbacks();
   m_Entities.Empty();

   m_ParseErrorEncountered = false;
   m_ErrorElementBeganAt.Empty();
   m_ErrorOccuredAt.Empty();
   m_ErrorTagName.clear();
   m_ErrorMessage.clear();

   if ( m_XML != nullptr )
   {
      CExtensibleMarkupLanguageElement::DeleteElement( m_XML );
      m_XML = nullptr;
   }

   m_IgnoreWhiteSpace = false;
   m_ParseOptions     = WFC_XML_IGNORE_MISSING_XML_DECLARATION;
   m_WriteOptions     = WFC_XML_WRITE_AS_ASCII;
   m_UseNamespace     = false;

   // Things from XMLDecl
   m_Encoding.clear();
   m_IsStandalone = true;
   m_Version.clear();
   m_Namespace.clear();

   m_InitializeEntities();
   m_InitializeRootElement();
}

_Check_return_ bool CExtensibleMarkupLanguageDocument::EnumerateCallbacks( _Out_ std::size_t& enumerator ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   enumerator = 0;

   if ( m_Callbacks.empty() == true )
   {
      // There aren't any callbacks to enumerate
      return( false );
   }

   return( true );
}

void CExtensibleMarkupLanguageDocument::ExecuteCallbacks( _Inout_ CExtensibleMarkupLanguageElement * element_p ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( element_p );

   if ( element_p == nullptr )
   {
      //WFCTRACE( TEXT( "Can't pass NULL!" ) );
      return;
   }

   for( auto const& entry : m_Callbacks )
   {
      WFC_TRY
      {
          if (element_p->Tag().compare( entry.name ) == I_AM_EQUAL_TO_THAT)
          {
             // We found a callback
             entry.callback( entry.parameter, element_p );
          }
      }
      WFC_CATCH_ALL
      {
         // Do Nothing
      }
      WFC_END_CATCH_ALL
   }
}

void CExtensibleMarkupLanguageDocument::GetAutomaticIndentation( _Out_ bool& automatically_indent, _Out_ uint32_t& indentation_level, _Out_ uint32_t& indent_by ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   automatically_indent = m_AutomaticIndentation;
   indentation_level    = m_IndentationLevel;
   indent_by            = m_IndentBy;
}

_Check_return_ CExtensibleMarkupLanguageElement * CExtensibleMarkupLanguageDocument::GetElement( _In_ std::wstring_view element_name ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( m_XML );

   if ( m_XML == nullptr )
   {
      return( nullptr );
   }

   // The element_name is in the form of "Parent(0).Child(1)"

   return( m_XML->GetChild( element_name ) );
}

void CExtensibleMarkupLanguageDocument::GetEncoding( _Out_ std::wstring& encoding ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if (is_flagged( m_WriteOptions, WFC_XML_WRITE_AS_UNICODE) == true )
   {
      encoding.assign(WSTRING_VIEW(L"UTF-16"));
   }
   else
   {
      encoding.assign( m_Encoding );
   }
}

void CExtensibleMarkupLanguageDocument::GetNamespace( _Out_ std::wstring& name_space ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   name_space.assign( m_Namespace );
}

_Check_return_ bool CExtensibleMarkupLanguageDocument::GetNextCallback( _Inout_ uint32_t& enumerator, _Out_ std::wstring& element_name, _Out_ XML_ELEMENT_CALLBACK& callback, _Out_ void *& parameter ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( enumerator >= static_cast< uint32_t >( m_Callbacks.size() ) )
   {
      element_name.clear();
      callback = static_cast< XML_ELEMENT_CALLBACK >( nullptr );
      parameter = nullptr;

      return( false );
   }

   element_name.assign(m_Callbacks[enumerator].name );
   callback  = m_Callbacks[enumerator].callback;
   parameter = m_Callbacks[enumerator].parameter;

   enumerator++;

   return( true );
}

_Check_return_ std::size_t CExtensibleMarkupLanguageDocument::GetNumberOfElements( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( m_XML );

   if ( m_XML == nullptr )
   {
      return( 0 );
   }

   return( m_XML->GetTotalNumberOfChildren() );
}

void CExtensibleMarkupLanguageDocument::GetParsingErrorInformation(_Out_ std::wstring& message ) const noexcept
{
    message.clear();

    std::wstring name;
    CParsePoint starting_at;
    CParsePoint error_location;
    std::wstring error_message;

    GetParsingErrorInformation(name, starting_at, error_location, &error_message);

    format(message, L"%s, %s began at line %d column %d, error location line %d column %d",
        error_message,
        name,
        (int)starting_at.GetLineNumber(),
        (int)starting_at.GetLineIndex(),
        (int)error_location.GetLineNumber(),
        (int)error_location.GetLineIndex());
}

void CExtensibleMarkupLanguageDocument::GetParsingErrorInformation(_Out_ std::wstring& tag_name, _Out_ CParsePoint& beginning, _Out_ CParsePoint& error_location, __out_opt std::wstring * error_message_p ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( error_message_p );

   tag_name.assign( m_ErrorTagName );
   beginning.Copy( m_ErrorElementBeganAt );
   error_location.Copy( m_ErrorOccuredAt );

   if ( error_message_p != nullptr )
   {
      // We were passed a pointer by the user, don't trust it

      WFC_TRY
      {
         error_message_p->assign( m_ErrorMessage );
      }
      WFC_CATCH_ALL
      {
         // Do Nothing
      }
      WFC_END_CATCH_ALL
   }
}

void CExtensibleMarkupLanguageDocument::m_InitializeEntities( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Entities.GetSize() > 0 )
   {
      m_Entities.Empty();
   }

   // Add the internal entities listed in section 4.6 of REC-xml-19980210
   (void) m_Entities.Add(WSTRING_VIEW(L"&amp;"), WSTRING_VIEW(L"&") );
   (void) m_Entities.Add(WSTRING_VIEW(L"&apos;"), WSTRING_VIEW(L"'") );
   (void) m_Entities.Add(WSTRING_VIEW(L"&gt;"), WSTRING_VIEW(L">") );
   (void) m_Entities.Add(WSTRING_VIEW(L"&lt;"), WSTRING_VIEW(L"<") );
   (void) m_Entities.Add(WSTRING_VIEW(L"&quot;"), WSTRING_VIEW(L"\"") );
}

void CExtensibleMarkupLanguageDocument::m_InitializeRootElement( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( m_XML );

   if ( m_XML != nullptr )
   {
      CExtensibleMarkupLanguageElement::DeleteElement( m_XML );
      m_XML = nullptr;
   }

   m_XML = CExtensibleMarkupLanguageElement::NewElement( nullptr, CExtensibleMarkupLanguageElement::ElementType::ProcessingInstruction, this );
}

void CExtensibleMarkupLanguageDocument::m_RemoveAllCallbacks( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Callbacks.clear();
}

static inline _Check_return_ bool __is_leading_spaces( _Inout_ CDataParser& source ) noexcept
{
   uint8_t const byte_1 = source.GetAt( 0 );
   uint8_t const byte_2 = source.GetAt( 1 );
   uint8_t const byte_3 = source.GetAt( 2 );
   uint8_t const byte_4 = source.GetAt( 3 );

   if ( is_xml_white_space( byte_1 ) == true)
   {
      // The first character was a space, the possibilities are ASCII, little endian
      // Unicode or UCS-4 little endian

      if ( byte_2 != 0 )
      {
         // ASCII
         (void) source.SetTextToASCII( true );
         return( true );
      }

      // Second byte is zero

      if ( byte_3 == 0 )
      {
         // Third character is zero, last chance for UCS-4 little endian

         if ( byte_4 == 0 )
         {
            (void) source.SetTextToUCS4( true );
            (void) source.SetUCS4Order( 4321 );
            return( true );
         }
      }
      else if ( byte_4 == 0 )
      {
         (void) source.SetTextToASCII( false );
         (void) source.SetTextToBigEndian( true );
         return( true );
      }
   }
   
   // First character was not a space

   if ( byte_1 != 0 )
   {
      return( false );
   }

   if ( byte_2 != 0 )
   {
      if ( is_xml_white_space( byte_2 ) == false)
      {
         return( false );
      }

      if ( byte_3 == 0 )
      {
         if ( byte_4 == 0 )
         {
            (void) source.SetTextToUCS4( true );
            (void) source.SetUCS4Order( 3412 );
            return( true );
         }
         else
         {
            return( false );
         }
      }
      else
      {
         if ( byte_4 == 0 )
         {
            (void) source.SetTextToASCII( false );
            (void) source.SetTextToBigEndian( true );
            return( true );
         }
         else
         {
            return( false );
         }
      }
   }

   // First two bytes are NULL...

   if ( byte_3 == 0 )
   {
      if ( is_xml_white_space( byte_4 ) == true)
      {
         (void) source.SetTextToUCS4( true );
         (void) source.SetUCS4Order( 1234 );
         return( true );
      }

      return( false );
   }
   else
   {
      if ( is_xml_white_space( byte_3 ) == true)
      {
         if ( byte_4 == 0 )
         {
            (void) source.SetTextToUCS4( true );
            (void) source.SetUCS4Order( 2143 );
            return( true );
         }
      }

      return( false );
   }

   return( false );
}

_Check_return_ bool CExtensibleMarkupLanguageDocument::ParseErrorOccurred(void) const noexcept
{
    return(true);
}

_Check_return_ bool CExtensibleMarkupLanguageDocument::Parse( _Inout_ CDataParser& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( m_XML );

   m_ParseErrorEncountered = false;
   m_ErrorElementBeganAt.Empty();
   m_ErrorOccuredAt.Empty();
   m_ErrorTagName.clear();
   m_ErrorMessage.clear();

   if ( m_XML == nullptr )
   {
      m_XML = CExtensibleMarkupLanguageElement::NewElement( nullptr, CExtensibleMarkupLanguageElement::ElementType::ProcessingInstruction, this );

      if ( m_XML == nullptr )
      {
         //WFCTRACE( TEXT( "Can't allocate memory for the document node" ) );
         m_ParseErrorEncountered = ParseErrorOccurred();
         m_ErrorMessage.assign(WSTRING_VIEW(L"Can't allocate memory."));
         return( false );
      }
   }

   if ( source.GetSize() < 4 )
   {
      // The minimum size of an XML document is 4 bytes (<a/>)
      m_ParseErrorEncountered = ParseErrorOccurred();
      m_ErrorMessage.assign(WSTRING_VIEW(L"Too few bytes to parse."));
      return( false );
   }

   // Here's where we look for the <?xml tag

   CParsePoint parse_point;

   // Check for leading spaces in accordance with Rule 27.

   if ( __is_leading_spaces( source ) == false )
   {
      // See if the FEFF or FFFE tags are present

      if ( source.GetAt( 0 ) == 0xFE )
      {
         if ( source.GetAt( 1 ) == 0xFF )
         {
            // We found the 0xFEFF tag.
            // That means it is UTF-16 Big Endian

            (void) source.SetTextToASCII( false );
            (void) source.SetTextToBigEndian( true );
         }
         else
         {
            m_ParseErrorEncountered = ParseErrorOccurred();
            m_ErrorMessage.assign(WSTRING_VIEW(L"Second character of Byte Order Mark is invalid for UTF-16 Big Endian (Appendix F)."));
            return( false );
         }

         // Skip the first two characters
         parse_point.SetIndex( 2 );
      }
      else if ( source.GetAt( 0 ) == 0xFF )
      {
         if ( source.GetAt( 1 ) == 0xFE )
         {
            // We found the 0xFFFE tag.
            // That means it is UTF-16 Little Endian

            (void) source.SetTextToASCII( false );
            (void) source.SetTextToBigEndian( false );
         }
         else
         {
            m_ParseErrorEncountered = ParseErrorOccurred();
            m_ErrorMessage.assign(WSTRING_VIEW(L"Second character of Byte Order Mark is invalid for UTF-16 Little Endian (Appendix F)."));
            return( false );
         }

         // Skip the first two characters
         parse_point.SetIndex( 2 );
      }
      else if ( source.GetAt( 0 ) == 0xEF and
                source.GetAt( 1 ) == 0xBB and
                source.GetAt( 2 ) == 0xBF )
      {
         // UTF-8 encoding
         (void) source.SetTextToASCII( true );
         // Skip the first three characters
         parse_point.SetIndex( 3 );
      }
      else
      {
         // Now it gets a little more interesting

         uint8_t byte_1 = source.GetAt( 0 );
         uint8_t byte_2 = source.GetAt( 1 );
         uint8_t byte_3 = source.GetAt( 2 );
         uint8_t byte_4 = source.GetAt( 3 );

         if ( byte_1 == 0x3C and byte_2 == 0x3F )
         {
            // Plain old ASCII
            (void) source.SetTextToASCII( true );
         }
         else if ( byte_1 == 0x00 and wfc_is_ascii( byte_2 ) == true and
                   byte_3 == 0x00 and wfc_is_ascii( byte_4 ) == true )
         {
            // UTF-16, Big Endian
            (void) source.SetTextToASCII( false );
            (void) source.SetTextToBigEndian( true );

            // Don't autoincrement parse_point
         }
         else if ( wfc_is_ascii( byte_1 ) == true and byte_2 == 0x00 and
                   wfc_is_ascii( byte_3 ) == true and byte_4 == 0x00 )
         {
            // UTF-16, Little Endian
            (void) source.SetTextToASCII( false );
            (void) source.SetTextToBigEndian( false );

            // Don't autoincrement parse_point
         }
         else if ( byte_1 == 0x00 and
                   byte_2 == 0x00 and
                   byte_3 == 0x00 and
                   wfc_is_ascii( byte_4 ) == true )
         {
            // UCS-4 Big Endian
            (void) source.SetTextToUCS4( true );
            (void) source.SetUCS4Order( 1234 );
         }
         else if ( wfc_is_ascii( byte_1 ) == true and
                   byte_2 == 0x00 and
                   byte_3 == 0x00 and
                   byte_4 == 0x00 )
         {
            // UCS-4 Little Endian
            (void) source.SetTextToUCS4( true );
            (void) source.SetUCS4Order( 4321 );
         }
         else if ( byte_1 == 0x00 and
                   byte_2 == 0x00 and
                   wfc_is_ascii( byte_3 ) == true and
                   byte_4 == 0x00 )
         {
            // UCS-4 Unusual
            (void) source.SetTextToUCS4( true );
            (void) source.SetUCS4Order( 2143 );
         }
         else if ( byte_1 == 0x00 and
                   wfc_is_ascii( byte_2 ) == true and
                   byte_3 == 0x00 and
                   byte_4 == 0x00 )
         {
            // UCS-4 Unusual
            (void) source.SetTextToUCS4( true );
            (void) source.SetUCS4Order( 3412 );
         }
         else if ( wfc_is_ascii( byte_1 ) == true )
         {
            // We'll just default to ASCII
            (void) source.SetTextToASCII( true );
         }
         else
         {
            m_ParseErrorEncountered = ParseErrorOccurred();
            m_ErrorMessage.assign(WSTRING_VIEW(L"Can't determine Character Encoding (Appendix F)."));
            return( false );
         }
      }
   }

   NumberOfBytesPerCharacter = source.NumberOfBytesPerCharacter();

   CParsePoint beginning_of_tag;
   CParsePoint end_of_tag;

   // Now skip leading spaces until we get to a less-than sign.
   // This according to Rule 1->27->3

   uint32_t character_to_test = source.PeekCharacter( parse_point, 0 );

   while( Win32FoundationClasses::is_xml_white_space( character_to_test ) == true )
   {
      // 2006-08-11 Fixed a bug where Unicode or UCS-4 files with leading spaces
      // weren't being parsed.
      source.AdvanceByOneCharacter( parse_point, character_to_test );

      if ( source.PeekAtCharacter( parse_point, character_to_test, 0 ) == false )
      {
         m_ParseErrorEncountered = ParseErrorOccurred();
         m_ErrorMessage.assign(WSTRING_VIEW(L"Document is all white space (Rule 1)."));
         return( false );
      }
   }

   // See if the first 5 chracters are <?xml

   bool xml_declaration_found         = false;
   bool xml_declaration_is_mixed_case = false;

   if (   source.PeekCharacter( parse_point, 0 ) == '<' and
          source.PeekCharacter( parse_point, 1 ) == '?' and
        ( source.PeekCharacter( parse_point, 2 ) == 'x'   or
          source.PeekCharacter( parse_point, 2 ) == 'X' ) and
        ( source.PeekCharacter( parse_point, 3 ) == 'm'   or
          source.PeekCharacter( parse_point, 3 ) == 'M' ) and
        ( source.PeekCharacter( parse_point, 4 ) == 'l'   or
          source.PeekCharacter( parse_point, 4 ) == 'L' ) )
   {
      beginning_of_tag.Copy( parse_point );
      xml_declaration_found = true;

      if ( source.PeekCharacter( parse_point, 2 ) != 'x' or
           source.PeekCharacter( parse_point, 3 ) != 'm' or
           source.PeekCharacter( parse_point, 4 ) != 'l' )
      {
         xml_declaration_is_mixed_case = true;
      }
   }
   else
   {
      if ( source.PeekCharacter( parse_point, 0 ) != '<' )
      {
         m_ParseErrorEncountered = ParseErrorOccurred();
         m_ErrorMessage.assign(WSTRING_VIEW(L"First non-space character is not '<' (Rule 1)."));
         return( false );
      }
   }

   // According to REC-xml-19980210 Rule 23, the beginning of an XML document
   // is at the "<?xml" tag. Notice this is lower case!

   if ( xml_declaration_found == true)
   {
      // See if it is lower case

      if ( xml_declaration_is_mixed_case == true and
         not ( is_flagged( m_ParseOptions, WFC_XML_IGNORE_CASE_IN_XML_DECLARATION ) == true ) )
      {
         m_ParseErrorEncountered = ParseErrorOccurred();
         m_ErrorMessage.assign(WSTRING_VIEW(L"XML Declaration is not lower case (Rule 23)."));
         return( false );
      }

      // If we get here, it means we've got an XML declaration
      // Just break out of the if...
   }
   else
   {
      // If we get here, it means we don't have an XML declaration but we
      // do have something that starts with '<'

      if (is_flagged( m_ParseOptions, WFC_XML_IGNORE_MISSING_XML_DECLARATION) == true)
      {
         //WFCTRACE( TEXT( "Skipping the missing XML declaration" ) );

         if ( m_XML == nullptr )
         {
            m_InitializeRootElement();
         }

         if ( m_XML != nullptr )
         {
            m_XML->SetContents(WSTRING_VIEW(L"xml version=\"1.0\" standalone=\"yes\""));
            (void) m_XML->AddAttribute(WSTRING_VIEW(L"version"), WSTRING_VIEW(L"1.0") );
            (void) m_XML->AddAttribute(WSTRING_VIEW(L"standalone"), WSTRING_VIEW(L"yes") );
            SetVersion(WSTRING_VIEW(L"1.0"));
            SetStandalone( true );
         }

         // OK, the XML declaration is missing, this is bad but allowable under
         // the specification. Let's just use the default set in the constructors.

         beginning_of_tag.Copy( parse_point );

         while( beginning_of_tag.GetIndex() < source.GetSize() )
         {
            uint32_t const character = source.PeekCharacter( beginning_of_tag, 0 );

            auto child_element_p = (character != '<' ) ? CExtensibleMarkupLanguageElement::NewElement(m_XML, CExtensibleMarkupLanguageElement::ElementType::TextSegment, this)
                                                       : CExtensibleMarkupLanguageElement::NewElement(m_XML, CExtensibleMarkupLanguageElement::ElementType::Element, this);

            if ( child_element_p == nullptr )
            {
               //WFCTRACE( TEXT( "Can't allocate memory for another element" ) );

               if ( m_XML != nullptr )
               {
                  m_XML->DestroyChildren();
               }

               return( false );
            }

            if ( child_element_p->Parse( beginning_of_tag, source ) == false )
            {
               //WFCTRACE( TEXT( "Parse failed." ) );

               if ( m_XML != nullptr )
               {
                  m_XML->DestroyChildren();
               }

               return( false );
            }

            child_element_p->GetEnding( beginning_of_tag );
         }

         return( true );
      }
      else
      {
         // We still failed

         m_ParseErrorEncountered = ParseErrorOccurred();
         m_ErrorMessage.assign(WSTRING_VIEW(L"Can't find beginning-of-XML tag (<?xml) regardless of case."));
         return( false );
      }
   }

   // If we get here, it means we are parsing starting at an XML declaration

   //WFCTRACEVAL( TEXT( "Found beginning at byte " ), (uint32_t) beginning_of_tag.GetIndex() );
   //WFCTRACEVAL( TEXT( "Line " ), (uint32_t) beginning_of_tag.GetLineNumber() );
   //WFCTRACEVAL( TEXT( "Column " ), (uint32_t) beginning_of_tag.GetLineIndex() );

   if ( source.Find( beginning_of_tag, L"?>", end_of_tag ) == false )
   {
      //WFCTRACE( TEXT( "Can't find ?>" ) );
      m_ParseErrorEncountered = ParseErrorOccurred();
      m_ErrorMessage.assign(WSTRING_VIEW(L"Can't find the end of the end of the beginning-of-XML tag (?>)."));
      return( false );
   }

   if ( m_XML == nullptr )
   {
      //WFCTRACE( TEXT( "m_XML is NULL!" ) );
      m_ParseErrorEncountered = ParseErrorOccurred();
      m_ErrorMessage.assign(WSTRING_VIEW(L"Internal error : m_XML is NULL."));
      return( false );
   }

   if ( m_XML->Parse( beginning_of_tag, source ) == false )
   {
      //WFCTRACE( TEXT( "m_XML - Can't parse" ) );

      // m_ParseErrorEncountered and m_ErrorMessag are set by the m_XML->Parse() call
      // so we don't need to fill them in here.

      m_XML->DestroyAttributes();
      m_XML->SetContents(WSTRING_VIEW(L"xml version=\"1.0\" standalone=\"yes\""));
      (void) m_XML->AddAttribute(WSTRING_VIEW(L"version"), WSTRING_VIEW(L"1.0"));
      (void) m_XML->AddAttribute(WSTRING_VIEW(L"standalone"), WSTRING_VIEW(L"yes"));

      return( false );
   }

   // Now let's see if the user wants to make the parser more useless

   if (is_flagged( m_ParseOptions, WFC_XML_DISALLOW_MULTIPLE_ELEMENTS ) == true )
   {
      std::size_t element_enumerator = 0;

      if ( m_XML->EnumerateChildren( element_enumerator ) == true )
      {
         CExtensibleMarkupLanguageElement * child_p = nullptr;

         uint32_t element_count = 0;

         while( m_XML->GetNextChild( element_enumerator, child_p ) == true )
         {
            if ( child_p->GetType() == CExtensibleMarkupLanguageElement::ElementType::Element )
            {
               // We have more than one element, that is. strictly speaking, a bad thing
               // but having more than one element is so much more useful. Never mind that.
               // The spec says to error out. So we will...

               element_count++;

               if ( element_count > 1 )
               {
                  m_ParseErrorEncountered = ParseErrorOccurred();
                  m_ErrorMessage.assign(WSTRING_VIEW(L"There can be only one element (Rule 1)."));
                  child_p->GetBeginning( m_ErrorElementBeganAt );
                  m_ErrorOccuredAt.Copy( m_ErrorElementBeganAt );
                  child_p->GetTag( m_ErrorTagName );

                  return( false );
               }
            }
         }
      }
   }

   if (is_flagged( m_ParseOptions, WFC_XML_FORCE_AT_LEAST_ONE_ELEMENT_MUST_BE_PRESENT) == true)
   {
      std::size_t element_enumerator = 0;

      if ( m_XML->EnumerateChildren( element_enumerator ) == true)
      {
         CExtensibleMarkupLanguageElement * child_p = nullptr;

         bool there_aint_one = true;

         while( m_XML->GetNextChild( element_enumerator, child_p ) == true and there_aint_one == true )
         {
            if ( child_p->GetType() == CExtensibleMarkupLanguageElement::ElementType::Element )
            {
               // We have more than one element, that is. strictly speaking, a bad thing
               // but having more than one element is so much more useful. Never mind that.
               // The spec says to error out. So we will...

               there_aint_one = false;
            }
         }

         if ( there_aint_one == true )
         {
            m_ParseErrorEncountered = ParseErrorOccurred();
            m_ErrorMessage.assign(WSTRING_VIEW(L"There must be only one element (Rule 1)."));
            m_ErrorElementBeganAt.Empty();
            m_ErrorTagName.clear();

            return( false );
         }
      }
   }

   //WFCTRACEVAL( TEXT( "?xml began at line " ), (uint32_t) beginning_of_tag.GetLineNumber() );
   //WFCTRACEVAL( TEXT( "Column " ),(uint32_t)  beginning_of_tag.GetLineIndex() );
   //WFCTRACEVAL( TEXT( "And ended at line " ), (uint32_t) end_of_tag.GetLineNumber() );
   //WFCTRACEVAL( TEXT( "Column " ), (uint32_t) end_of_tag.GetLineIndex() );

   return( true );
}

_Check_return_ bool CExtensibleMarkupLanguageDocument::RemoveCallback( _In_ std::wstring_view element_name, __callback XML_ELEMENT_CALLBACK callback, __inout void * callback_parameter ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( callback );

   // Now go find this entry (to make sure we don't call it twice)

   std::size_t loop_index        = 0;
   std::size_t number_of_entries = m_Callbacks.size();

   while( loop_index < number_of_entries ) // Cannot be converted to a Range loop
   {
      if (m_Callbacks[loop_index].callback  == callback and
          m_Callbacks[loop_index].parameter == callback_parameter and
          element_name.compare( m_Callbacks[loop_index].name ) == I_AM_EQUAL_TO_THAT)
      {
         m_Callbacks.erase( std::begin(m_Callbacks) + loop_index );
         number_of_entries--;
         loop_index--;
      }

      loop_index++;
   }

   return( true );
}

_Check_return_ bool CExtensibleMarkupLanguageDocument::ResolveEntity( _In_ std::wstring_view entity, _Out_ std::wstring& resolved_to ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Entities.Resolve( entity, resolved_to ) == false )
   {
      return( false );
   }

   return( true );
}

void CExtensibleMarkupLanguageDocument::SetConversionCodePage(_In_ uint32_t const new_page ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_ConversionCodePage = new_page;

   if ( m_ConversionCodePage != CP_ACP )
   {
       if (IsWindowsXPOrGreater() == false)
       {
           // Piece of crap Windows95 garbage
           m_ConversionCodePage = CP_ACP;
       }
   }
}

void CExtensibleMarkupLanguageDocument::SetEncoding( _In_ std::wstring_view encoding ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Encoding.assign( encoding );
}

void CExtensibleMarkupLanguageDocument::SetNamespace( _In_ std::wstring_view name_space ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( name_space.empty() == true )
   {
      m_UseNamespace = false;
      m_Namespace.clear();
   }
   else
   {
      m_UseNamespace = true;
      m_Namespace.assign( name_space );
   }
}

void CExtensibleMarkupLanguageDocument::SetParsingErrorInformation( _In_ std::wstring_view tag_name, _In_ CParsePoint const& beginning, _In_ CParsePoint const& error_location, _In_ std::wstring_view error_message ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_ParseErrorEncountered == false )
   {
      m_ParseErrorEncountered = ParseErrorOccurred();
      m_ErrorTagName.assign( tag_name );
      m_ErrorElementBeganAt.Copy( beginning );
      m_ErrorOccuredAt.Copy( error_location );

      if ( error_message.empty() == true )
      {
         m_ErrorMessage.clear();
      }
      else
      {
         m_ErrorMessage.assign( error_message );
      }
   }
}

_Check_return_ uint32_t CExtensibleMarkupLanguageDocument::SetWriteOptions( _In_ uint32_t const new_options ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   uint32_t return_value = m_WriteOptions;

   m_WriteOptions = new_options;

#if defined( _DEBUG )
   // Assert the crap out of these options

   if ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF8 )
   {
      // UTF-8 was specified, UTF-7 & ASCII are invalid
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF7 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_ASCII ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UNICODE ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_2143 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_3412 ) );
   }
   else if ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF7 )
   {
      // UTF-7 was specified, UTF-8 & ASCII are invalid
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF8 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_ASCII ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UNICODE ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_2143 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_3412 ) );
   }
   else if ( m_WriteOptions bitand WFC_XML_WRITE_AS_ASCII )
   {
      // ASCII was specified, UTF is invalid
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF7 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF8 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UNICODE ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_2143 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_3412 ) );
   }
   else if ( m_WriteOptions bitand WFC_XML_WRITE_AS_UNICODE )
   {
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_ASCII ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF7 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF8 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_2143 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_3412 ) );
   }
   else if ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4 )
   {
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_ASCII ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF7 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF8 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UNICODE ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_2143 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_3412 ) );
   }
   else if ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_2143 )
   {
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_ASCII ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF7 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF8 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UNICODE ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_3412 ) );
   }
   else if ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_3412 )
   {
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_ASCII ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF7 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UTF8 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UNICODE ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4 ) );
      ASSERT(not ( m_WriteOptions bitand WFC_XML_WRITE_AS_UCS4_UNUSUAL_2143 ) );
   }
#endif // _DEBUG

   if (is_flagged(m_WriteOptions, WFC_XML_WRITE_AS_ASCII) == true)
   {
      SetConversionCodePage( CP_ACP );
   }
   else if (is_flagged(m_WriteOptions, WFC_XML_WRITE_AS_UTF8) == true)
   {
      SetConversionCodePage( CP_UTF8 );
   }
   else if (is_flagged(m_WriteOptions, WFC_XML_WRITE_AS_UTF7) == true)
   {
      SetConversionCodePage( CP_UTF7 );
   }
   else
   {
      SetConversionCodePage( CP_ACP );
   }

   return( return_value );
}

void CExtensibleMarkupLanguageDocument::WriteTo( _Out_ std::vector<uint8_t>& destination ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_XML );

   destination.clear();

   if ( m_XML == nullptr )
   {
      //WFCTRACE( TEXT( "Root element is NULL." ) );
      return;
   }

   if (is_flagged(m_WriteOptions, WFC_XML_DONT_OUTPUT_XML_DECLARATION) == true)
   {
      // The user doesn't want to write the XML declaration. Just write
      // the children.

      std::size_t enumerator = 0;

      if ( m_XML->EnumerateChildren( enumerator ) == true )
      {
         CExtensibleMarkupLanguageElement * child_p = nullptr;

         while( m_XML->GetNextChild( enumerator, child_p ) == true )
         {
            if ( child_p != nullptr )
            {
               child_p->WriteTo( destination );
            }
         }
      }
      else
      {
         //WFCTRACE( TEXT( "No children to enumerate and thusly write." ) );
      }
   }
   else
   {
      // See if the user wants to write as UNICODE or not

      if (is_flagged(m_WriteOptions, WFC_XML_WRITE_AS_UNICODE) == true)
      {
         if (is_flagged(m_WriteOptions, WFC_XML_WRITE_AS_BIG_ENDIAN) == true)
         {
            // Write UTF-16 Big Endian as specified in Appendix F
            (void) destination.push_back( 0xFE );
            (void) destination.push_back( 0xFF );
         }
         else
         {
            (void) destination.push_back( 0xFF );
            (void) destination.push_back( 0xFE );
         }
      }

      m_XML->WriteTo( destination );
   }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CExtensibleMarkupLanguageDocument</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, push technology, source code">
<META name="description" content="The C++ class that parses and constructs XML documents.">
</HEAD>

<BODY>

<H1>CExtensibleMarkupLanguageDocument</H1>
$Revision: 50 $
<HR>

<H2>Description</H2>

This class is the mother of all
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A>
classes. It holds the
things like the element tree and settings that apply to
the entire document. It is designed to help application developers
handle XML-like data. It will parse (and construct) well formed,
standalone XML documents. It will also allow you to loosen the 
parsing rules when dealing with XML from sources you can't control.

<H2>Construction</H2>

<DL COMPACT>

<DT><PRE><B>CExtensibleMarkupLanguageDocument</B>()
<B>CExtensibleMarkupLanguageDocument</B>( const CExtensibleMarkupLanguageDocument&amp; source )</PRE><DD>
Creates another <B>CExtensibleMarkupLanguageDocument</B>.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>bool <B><A NAME="AddCallback">AddCallback</A></B>( const char * element_name, XML_ELEMENT_CALLBACK callback, void * callback_parameter )</PRE><DD>
Allows you to specify a function (and a parameter for that function) that
will be called when an element with a tag matching <CODE>element_name</CODE>
has been successfully parsed. The <CODE>element_name</CODE> comparison
is not case sensitive.

<DT><PRE>void <B><A NAME="Append">Append</A></B>( const CExtensibleMarkupLanguageDocument&amp; source )</PRE><DD>
Appends the elements of <CODE>source</CODE> to this document.

<DT><PRE>void <B><A NAME="Copy">Copy</A></B>( const CExtensibleMarkupLanguageDocument&amp; source )</PRE><DD>
Copies the contents of <CODE>source</CODE> to this object. It will not
copy the callback functions as this may cause unintentional results.

<DT><PRE>void <B><A NAME="CopyCallbacks">CopyCallbacks</A></B>( const CExtensibleMarkupLanguageDocument&amp; source )</PRE><DD>
Copies the callback functions from <CODE>source</CODE> to this object.
If you are a careful programmer, this is perfectly safe to do. Generally
speaking, you shouldn&#39;t have to copy the callbacks of <CODE>source</CODE>
because parsing should have already taken place.

<DT><PRE>uint32_t <B><A NAME="CountElements">CountElements</A></B>( wchar_t const * element_name ) const</PRE><DD>
Counts the number of elements. <CODE>element_name</CODE> takes much the
same form as used in the <B><A HREF="#GetElement">GetElement()</A></B> method.
Consider the following
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
If you wanted to know how many &quot;Boy&quot; elements there
are in the first set of characters, you would use an element name
of <CODE>&quot;SouthPark.Characters&quot;</CODE> If you wanted to
know how many &quot;Girl&quot; elements there are in the second
set of characters, you would use this for <CODE>element_name</CODE>:
&quot;Southpark.Characters(1).Girl&quot;

<DT><PRE>void <B><A NAME="Empty">Empty</A></B>( void )</PRE><DD>
Empties the contents of the document. The object is reset to an
intial state. All elements are deleted. All callbacks are deleted.

<DT><PRE>bool <B><A NAME="EnumerateCallbacks">EnumerateCallbacks</A></B>( uint32_t&amp; enumerator ) const</PRE><DD>
Initializes the <CODE>enumerator</CODE> in preparation for calling
<B><A HREF="#GetNextCallback">GetNextCallback()</A></B>. If there are
no callbacks (i.e. <B><A HREF="#AddCallback">AddCallback()</A></B> has
not been called), false will be returned. If there are callbacks, true
will be returned.

<DT><PRE>void <B><A NAME="ExecuteCallbacks">ExecuteCallbacks</A></B>( <A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * element_p )</PRE><DD>
This is generally called during the parsing of a document by the
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A>
that just parsed itself. However, you can pull an element out of the
document and call <B>ExecuteCallbacks</B>() yourself.

<DT><PRE>void <B><A NAME="GetAutomaticIndentation">GetAutomaticIndentation</A></B>( bool&amp; automatically_indent, uint32_t&amp; indentation_level, uint32_t&amp; indent_by ) const</PRE><DD>
Retrieves the automatic indentation parameters. Automatic indentation does
nothing but make the XML output look pretty. It makes it easier for humans
to read. If your application is sensitive to white space, don't use automatic
indentation.

<DT><PRE>uint32_t <B><A NAME="GetConversionCodePage">GetConversionCodePage</A></B>( void ) const</PRE><DD>
Returns the code page that will be used for conversion from UNICODE.

<DT><PRE><A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B><A NAME="GetElement">GetElement</A></B>( wchar_t const * element_name ) const</PRE><DD>
Searches and finds the specified element in the document. The
<CODE>element_name</CODE> is in the form of &quot;Parent(0).Child(0)&quot;
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
To retrieve the element for Cartman, <CODE>element_name</CODE> should
be &quot;Southpark.Characters.Boy&quot; If you want Ms. Ellen (even
though she doesn't play for the home team) you would use
&quot;Southpark.Characters(1).Girl(1)&quot;

<DT><PRE>void <B><A NAME="GetEncoding">GetEncoding</A></B>( std::wstring&amp; encoding ) const</PRE><DD>
Returns the encoding of the document.

<DT><PRE>const <A HREF="CExtensibleMarkupLanguageEntities.htm">CExtensibleMarkupLanguageEntities</A>&amp; <B><A NAME="GetEntities">GetEntities</A></B>( void ) const</PRE><DD>
Returns a const reference to the entities for this document.
Basically all you can do with it is enumerate the entries.

<DT><PRE>bool <B><A NAME="GetIgnoreWhiteSpace">GetIgnoreWhiteSpace</A></B>( void ) const</PRE><DD>
Returns whether or not the document will suppress the output
of elements that contain only space characters. This output
occurs when you call <B><A HREF="#WriteTo">WriteTo()</A></B>.

<DT><PRE>void <B><A NAME="GetNamespace">GetNamespace</A></B>( std::wstring&amp; name_space ) const</PRE><DD>
Retrieves the namespace name if one is being used.

<DT><PRE>bool <B><A NAME="GetNextCallback">GetNextCallback</A></B>( uint32_t&amp; enumerator, std::wstring&amp; element_name, XML_ELEMENT_CALLBACK&amp; callback, void *&amp; callback_parameter )</PRE><DD>
Retrieves the next callback. It will return true if the callback has been
retrieved or false if you are at the end of the list. If false is returned,
all parameters are set to NULL. Callbacks are added via the
<B><A HREF="#AddCallback">AddCallback()</A></B> method.

<DT><PRE>uint32_t <B><A NAME="GetNumberOfElements">GetNumberOfElements</A></B>( void ) const</PRE><DD>
Returns the number of elements in this document.

<DT><PRE>wchar_t <B><A NAME="GetParentChildSeparatorCharacter">GetParentChildSeparatorCharacter</A></B>( void ) const</PRE><DD>
Returns the character that will be used to separate parent element names
from child element names in the <B>GetElement</B>() method.

<DT><PRE>uint32_t <B><A NAME="GetParseOptions">GetParseOptions</A></B>( void ) const</PRE><DD>
Returns the parse options. This is a bit field (32 wide) that
controls the sloppiness of the parser.

<DT><PRE>void <B><A NAME="GetParsingErrorInformation">GetParsingErrorInformation</A></B>( std::wstring&amp; tag_name, <A HREF="CParsePoint.htm">CParsePoint</A>&amp; beginning, <A HREF="CParsePoint.htm">CParsePoint</A>&amp; error_location, std::wstring * error_message = nullptr ) const</PRE><DD>
If
<B><A HREF="#Parse">Parse()</A></B>
returns false, you can call this method to find out
interesting information as to where the parse failed. This will help you
correct the
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A>.
If <CODE>error_message</CODE> is not NULL, it will be filled
with a human readable error message.
The <CODE>beginning</CODE> parameter is filled with the location in
the document where the element began.
The <CODE>error_location</CODE> parameter is filled with the location
where the parser encountered the fatal problem.

<DT><PRE><A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>GetRootElement</B>( void ) const</PRE><DD>
Returns the pointer to the ultimate parent element. This will be the element
that contains the data from the <CODE>&lt;?xml ... ?&gt;</CODE> line.

<DT><PRE>void <B><A NAME="GetVersion">GetVersion</A></B>( std::wstring&amp; version ) const</PRE><DD>
Returns the version of the document.

<DT><PRE>uint32_t <B><A NAME="GetWriteOptions">GetWriteOptions</A></B>( void ) const</PRE><DD>
Returns the writing options. This is a bit field (32 wide) that
controls how the
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A>
documents are written.

<DT><PRE>bool <B><A NAME="IsStandalone">IsStandalone</A></B>( void ) const</PRE><DD>
Returns true if this is a standalone document.

<DT><PRE>bool <B><A NAME="Parse">Parse</A></B>( const <A HREF="CDataParser.htm">CDataParser</A>&amp; source )</PRE><DD>
Parses the data from <CODE>source</CODE>. This will construct
the document tree.

<DT><PRE>bool <B><A NAME="RemoveCallback">RemoveCallback</A></B>( const char * element_name, XML_ELEMENT_CALLBACK callback, void * callback_parameter )</PRE><DD>
This will remove the specified callback from the list. All parameters
must match for the callback to be removed.

<DT><PRE>bool <B><A NAME="ResolveEntity">ResolveEntity</A></B>( const std::wstring&amp; entity, std::wstring&amp; resolved_to ) const</PRE><DD>
This method will resolve the <CODE>entity</CODE> and put the result into <CODE>resolved_to</CODE>.
If the entity cannot be resolved, it will return false.

<DT><PRE>void <B><A NAME="SetAutomaticIndentation">SetAutomaticIndentation</A></B>( bool automatically_indent = true, uint32_t starting_column = 0, uint32_t indent_by = 2 )</PRE><DD>
This will turn automatic indentation on or off.

<DT><PRE>bool <B><A NAME="SetConversionCodePage">SetConversionCodePage</A></B>( uint32_t new_page )</PRE><DD>
When you must convert from UNICODE to something else, this is the
code page that will be used. See the <CODE><B>WideCharToMultiByte()</B></CODE> Win32 API
for more information. If the code is run on a real operating system (NT), the
default code page is <CODE>CP_UTF8</CODE>. If you are running on a piece of crap
(Windows 95) the default code page is <CODE>CP_ACP</CODE>.

<DT><PRE>void <B><A NAME="SetEncoding">SetEncoding</A></B>( LPCTSTR encoding )</PRE><DD>
Sets the encoding of the document. You will usuall do this when you are about
to write the document.

<DT><PRE>bool <B><A NAME="SetIgnoreWhiteSpace">SetIgnoreWhiteSpace</A></B>( bool ignore_whitespace )</PRE><DD>
Tells the document whether or not to ignore text segments that contain
only space characters. It returns what the previous setting was.

<DT><PRE>void <B><A NAME="SetNamespace">SetNamespace</A></B>( LPCTSTR name_space )</PRE><DD>
Sets the namespace name. This name will be prepended to all element names when the XML is written
(via the <A HREF="#WriteTo">WriteTo</A> method). If <CODE>name_space</CODE> is non-NULL and not
empty, the name will be used and the <A HREF="#UseNamespace">UseNamespace</A> will return true.
If <CODE>name_space</CODE> is NULL or empty, a name will not be used (the default) and
<A HREF="#UseNamespace">UseNamespace</A> will return false.

<DT><PRE>bool <B><A NAME="SetParentChildSeparatorCharacter">SetParentChildSeparatorCharacter</A></B>( TCHAR separator )</PRE><DD>
Allows you to specify the character that will separate parent and child
names in the <B>GetElement()</B> call.

<DT><PRE>uint32_t <B><A NAME="SetParseOptions">SetParseOptions</A></B>( uint32_t new_options )</PRE><DD>
Sets the parsing options. This allows you to customize the parser to
be as loose or as strict as you want. The default is to be as strict
as possible when parsing. <B>SetParseOptions</B>() returns the previous
options. Here are the current parse options that can be set:

<UL>

<LI><B><CODE>WFC_XML_IGNORE_CASE_IN_XML_DECLARATION</CODE></B> - When set, this option
will allow uppercase letters in the XML declaration. For example:
<CODE>&lt;?XmL ?&gt;</CODE> will be allowed even though it does not
conform to the
<A HREF="http://www.w3.org/TR/" TARGET="_parent">specification</A>.

<LI><B><CODE>WFC_XML_ALLOW_REPLACEMENT_OF_DEFAULT_ENTITIES</CODE></B> - Though the
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML specification</A>
doesn&#39;t talk about it, what should a parser do if default entities
are replaced? If you set this option, the parser will allow replacement
of the default entities. Here is a list of the default entities:

<UL>
<LI><B><CODE>&#38;amp;</CODE></B>
<LI><B><CODE>&#38;apos;</CODE></B>
<LI><B><CODE>&#38;gt;</CODE></B>
<LI><B><CODE>&#38;lt;</CODE></B>
<LI><B><CODE>&#38;quot;</CODE></B>
</UL>

<LI><B><CODE>WFC_XML_FAIL_ON_ILL_FORMED_ENTITIES</CODE></B> - <I><B>Not yet implemented.</B></I>
It will allow the parser to ignore ill formed entities such as <BR>
<CODE>&lt;!ENTITY amp &quot;&#38;#38;&quot;&gt;</CODE>

<LI><B><CODE>WFC_XML_IGNORE_ALL_WHITE_SPACE_ELEMENTS</CODE></B> - Tells the parser
to ignore elements (of type <B><CODE>typeTextSegment</CODE></B>) that contain
nothing but white space characters. <B>WARNING!</B> If you use this option, it will
not be possible to reproduce that input file exactly. Elements that contain
nothing but white spaces will be deleted from the document.

<LI><B><CODE>WFC_XML_IGNORE_MISSING_XML_DECLARATION</CODE></B> - Tells the parser
to ignore the fact that the <CODE>&lt;?xml ?&gt;</CODE> element is missing.
If it was not specified in the data stream, one will be automatically
added to the document. This is the default behavior.

<LI><B><CODE>WFC_XML_DISALLOW_MULTIPLE_ELEMENTS</CODE></B> - Tells the parser
to allow multiple elements to be present in the document. The first rule (Rule 1)
of the 
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML specification</A>
says (like Connor MacLeod of the clan MacLeod) <I>There can be only one</I>
element in an XML document. That element can have a billion child elements
but there can be only one root element. If this option is set
(it is not set by default), the parser will strictly enforce this rule. This rule
really gets in the way of using XML for things like log files (where you
want to open the file, append a record to it and close the file).

<LI><B><CODE>WFC_XML_LOOSE_COMMENT_PARSING</CODE></B> - Tells the parser
to allow double dashes (--) to appear in comment tags.

<LI><B><CODE>WFC_XML_ALLOW_AMPERSANDS_IN_ELEMENTS</CODE></B> - Tells the parser
to allow &amp;'s to appear in the contents of an element without being
a reference of some kind.

<LI><B><CODE>WFC_XML_FORCE_AT_LEAST_ONE_ELEMENT_MUST_BE_PRESENT</CODE></B> - Tells
the parser that one element must be present in the XML. You cannot have valid
XML that consists only of processing instructions and comments (for example).

</UL>

<DT><PRE>void <B><A NAME="SetParsingErrorInformation">SetParsingErrorInformation</A></B>( const std::wstring&amp; tag_name, const <A HREF="CParsePoint.htm">CParsePoint</A>&amp; beginning, const <A HREF="CParsePoint.htm">CParsePoint</A>&amp; error_location, LPCTSTR error_message = NULL )</PRE><DD>
This method is usually called by the element that cannot parse
itself. There is logic that prevents the information from being
overwritten by subsequent calls to <B>SetParsingErrorInformation</B>().
This means you can call <B>SetParsingErrorInformation</B>() as
many times as you want but only information from the first call
will be recorded (and reported via
<B><A HREF="#GetParsingErrorInformation">GetParsingErrorInformation()</A></B>)
for each call to
<B><A HREF="#Parse">Parse()</A></B>.

<DT><PRE>void <B><A NAME="SetStandalone">SetStandalone</A></B>( bool standalone )</PRE><DD>
Sets the standalone attribute of the document.
This is usually done just before you start writing the document.

<DT><PRE>void <B><A NAME="SetVersion">SetVersion</A></B>( LPCTSTR version )</PRE><DD>
Sets the version of the document.
This is usually done just before you start writing the document.

<DT><PRE>uint32_t <B><A NAME="SetWriteOptions">SetWriteOptions</A></B>( uint32_t new_options )</PRE><DD>
Sets the writing options. This allows you to customize how the
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A> is written.
The default is to be as strict as possible when writing.
<B>SetWriteOptions</B>() returns the previous
options. Here are the current options that can be set:

<UL>

<LI><B><CODE>WFC_XML_INCLUDE_TYPE_INFORMATION</CODE></B> - <I><B>Not Yet Implemented.</B></I>
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A>
is woefully inept at handling data. They use things called DTD&#39;s but
they have a &quot;the world is flat&quot; outlook on life.
DTD&#39;s lack the ability to scope.
It would be like programming where all variables have to have unique names
no matter what function they exist in.
DTD&#39;s are used to give HTML browsers the ability to correctly
display
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A>.
They also give you the ability to do some lame data validation.
In the future, I will include the
ability to write type information in a programmer friendly fashion.
This type information is intended to be a programmer-to-programmer
communication medium.

<LI><B><CODE>WFC_XML_DONT_OUTPUT_XML_DECLARATION</CODE></B> - This allows
you to skip writing the
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A>
declaration when you output. For example, this
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A> document:
<PRE><CODE>&lt;?xml version=&quot;1.0&quot; ?&gt;
&lt;TRUTH&gt;Sam loves Laura.&lt;/TRUTH&gt;</CODE></PRE>
Would look like this when <CODE>WFC_XML_DONT_OUTPUT_XML_DECLARATION</CODE>
is set:
<PRE><CODE>&lt;TRUTH&gt;Sam loves Laura.&lt;/TRUTH&gt;</CODE></PRE>

<LI><B><CODE>WFC_XML_WRITE_AS_UNICODE</CODE></B> - This
tells the document to write output as UNICODE (two bytes per
character). It will default to writing in little endian format.

<LI><B><CODE>WFC_XML_WRITE_AS_BIG_ENDIAN</CODE></B> - This
tells the document to write UNICODE or UCS4 characters in
big endian format.

<LI><B><CODE>WFC_XML_WRITE_AS_UCS4</CODE></B> - This will write
the data in UCS4 (four bytes per character). The default is to
write in little endian format. For example, the &lt; character
would come out as these bytes <CODE>3C&nbsp;00&nbsp;00&nbsp;00</CODE>

<LI><B><CODE>WFC_XML_WRITE_AS_UCS4_UNUSUAL_2143</CODE></B> - This
will format the output in an unusual 4 bytes per character format.
For example, the &lt; character
would come out as these bytes <CODE>00&nbsp;00&nbsp;3C&nbsp;00</CODE>

<LI><B><CODE>WFC_XML_WRITE_AS_UCS4_UNUSUAL_3412</CODE></B> - This
will format the output in another unusual 4 bytes per character
format.
For example, the &lt; character
would come out as these bytes <CODE>00&nbsp;3C&nbsp;00&nbsp;00</CODE>

<LI><B><CODE>WFC_XML_WRITE_AS_ASCII</CODE></B> - This 
will format the output in ASCII format. This is the default.

<LI><B><CODE>WFC_XML_WRITE_AS_UTF8</CODE></B> - This will
write the data out in UTF-8 format.

</UL>

<DT><PRE>bool <B><A NAME="UseNamespace">UseNamespace</A></B>( void ) const</PRE><DD>
Returns true if a namespace is in use, false otherwise.

<DT><PRE>void <B><A NAME="WriteTo">WriteTo</A></B>( std::vector&lt;uint8_t&gt;&amp; destination )</PRE><DD>
Write the data to <CODE>destination</CODE> in
<A HREF="http://www.w3.org/TR/" TARGET="_parent">XML</A> form.

</DL>

<H2>Operators</H2>

<DL COMPACT>

<DT><PRE>CExtensibleMarkupLanguageDocument&amp; operator <B>=</B> ( const CExtensibleMarkupLanguageDocument&amp; source )</PRE><DD>
Calls <B><A HREF="#Copy">Copy()</A></B>.

<DT><PRE>CExtensibleMarkupLanguageDocument&amp; operator <B>+=</B> ( const CExtensibleMarkupLanguageDocument&amp; source )</PRE><DD>
Calls <B><A HREF="#Append">Append()</A></B>.

</DL>

<H2>Example</H2><PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

bool get_bytes( const std::wstring&amp; filename, std::vector&lt;uint8_t&gt;&amp; bytes )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;get_bytes()&quot; ) );

   bytes.RemoveAll();

   <A HREF="CFile64.htm">CFile64</A> file;

   if ( file.Open( filename, <A HREF="CFile64.htm">CFile64</A>::modeRead ) == false )
   {
      return( false );
   }

   bytes.SetSize( file.GetLength() );

   file.Read( bytes.GetData(), bytes.GetSize() );

   file.Close();

   return( true );
}

bool parse_document( const std::wstring&amp; filename, CExtensibleMarkupLanguageDocument&amp; document )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;parse_document()&quot; ) );

   std::vector&lt;uint8_t&gt; bytes;

   if ( get_bytes( filename, bytes ) == false )
   {
      return( false );
   }

   <A HREF="CDataParser.htm">CDataParser</A> parser;

   parser.<A HREF="CDataParser.htm#Initialize">Initialize</A>( &amp;bytes, false );

   if ( document.<A HREF="#Parse">Parse</A>( parser ) == true )
   {
      _tprintf( TEXT( &quot;Parsed OK\n&quot; ) );
   }
   else
   {
      _tprintf( TEXT( &quot;Can't parse document\n&quot; ) );
   }

   return( true );
}

void stanza_callback( void * parameter, <A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * element_p )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;stanza_callback()&quot; ) );

   _tprintf( TEXT( &quot;Got a stanza with %lu children\n&quot; ), (uint32_t) element_p->GetNumberOfChildren() );
}

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   <B>CExtensibleMarkupLanguageDocument</B> document;

   document.<A HREF="#AddCallback">AddCallback</A>( TEXT( &quot;stanza&quot; ), stanza_callback, NULL );

   if ( parse_document( TEXT( &quot;poem.xml&quot; ), document ) == true )
   {
      std::vector&lt;uint8_t&gt; bytes;

      document.<A HREF="#SetWriteOptions">SetWriteOptions</A>( WFC_XML_DONT_OUTPUT_XML_DECLARATION );

      document.<A HREF="#WriteTo">WriteTo</A>( bytes );

      _tprintf( TEXT( &quot;Wrote %d bytes\n&quot; ), bytes.GetSize() );

      <A HREF="CFile64.htm">CFile64</A> file;

      if ( file.Open( TEXT( &quot;xml.out&quot; ), <A HREF="CFile64.htm">CFile64</A>::modeCreate | <A HREF="CFile64.htm">CFile64</A>::modeWrite ) == true )
      {
         file.Write( bytes.GetData(), bytes.GetSize() );
         file.Close();
      }
   }

   return( EXIT_SUCCESS );
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CExtensibleMarkupLanguageDocument.cpp $<BR>
$Modtime: 8/30/01 7:21p $
</BODY>

</HTML>
The following line should go in AUTOEXP.DAT so the debugging tooltips will format properly
ToolTipFormatLine=
*/
