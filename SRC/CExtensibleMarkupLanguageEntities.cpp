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
** $Workfile: CExtensibleMarkupLanguageEntities.cpp $
** $Revision: 24 $
** $Modtime: 6/26/01 10:46a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CExtensibleMarkupLanguageEntities::CExtensibleMarkupLanguageEntities() noexcept
{
   WFC_VALIDATE_POINTER( this );
}

Win32FoundationClasses::CExtensibleMarkupLanguageEntities::CExtensibleMarkupLanguageEntities( _In_ Win32FoundationClasses::CExtensibleMarkupLanguageEntities const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CExtensibleMarkupLanguageEntities::~CExtensibleMarkupLanguageEntities()
{
   WFC_VALIDATE_POINTER( this );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageEntities::Add( _In_ std::wstring_view entity, _In_ std::wstring_view text_parameter ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // entity is in &xxx; form

   // Validate the entity

   if (entity.length() < 3 )
   {
      return( false );
   }

   if ( entity[ 0 ] not_eq '&' )
   {
      return( false );
   }

   if ( entity[entity.length() - 1 ] not_eq ';' )
   {
      return( false );
   }

   // Now validate the name characters according to Rule 68->5

   if ( Win32FoundationClasses::is_xml_Letter( entity[ 1 ] ) == false and
        entity[ 1 ] not_eq '_' and
        entity[ 1 ] not_eq ':' )
   {
      //WFCTRACE( TEXT( "Bad first character of entity name." ) );
      return( false );
   }

   for ( auto const loop_index : Range(entity.length() - 1, StartingRangePosition(2)) )
   {
      if ( Win32FoundationClasses::is_xml_NameChar( entity[ loop_index ] ) == false )
      {
         //WFCTRACEVAL( TEXT( "This entity is bad " ), entity );
         //WFCTRACEVAL( TEXT( "This character index " ), (uint32_t) loop_index );
         return( false );
      }
   }

   // Now validate the characters according to Rule 9 (REC-xml.htm#NT-EntityValue) 

   std::wstring const text(text_parameter);

   if ( text.find( '%' ) not_eq std::wstring::npos )
   {
      return( false );
   }

   // First, normalize the text as described in Appendix D

   std::wstring resolved_string;

   if ( text.find( WSTRING_VIEW(L"&#") ) == std::wstring::npos )
   {
      // Well, that was easy
      resolved_string = text;
   }
   else
   {
      std::wstring temp_string( text );
      std::wstring number_string;

      std::size_t location_of_character_reference = temp_string.find(WSTRING_VIEW(L"&#"));

      while( location_of_character_reference not_eq std::wstring::npos )
      {
         resolved_string.append( temp_string.substr( 0, location_of_character_reference ) );
         temp_string.erase(0, location_of_character_reference + 2);
         number_string.clear();

         // See if we are decimal or hexadecimal . . .

         bool is_hexadecimal_character_reference = false;

         if ( temp_string.empty() == false and temp_string.at( 0 ) == 'x' )
         {
            // We are hexadecimal
            is_hexadecimal_character_reference = true;

            // Skip the x (yes, it must be lower case)

            temp_string.erase(0, 1);

            while( temp_string.empty() == false and
                   ( ( temp_string.at( 0 ) >= '0' and temp_string.at( 0 ) <= '9' ) or
                     ( temp_string.at( 0 ) >= 'A' and temp_string.at( 0 ) <= 'Z' ) or
                     ( temp_string.at( 0 ) >= 'a' and temp_string.at( 0 ) <= 'z' ) ) )
            {
               number_string.push_back( temp_string.at( 0 ) );
               temp_string.erase(0, 1);
            }
         }
         else
         {
            // We are decimal

            while( temp_string.empty() == false and
                   temp_string.at( 0 ) >= '0' and temp_string.at( 0 ) <= '9' )
            {
               number_string.push_back( temp_string.at( 0 ) );
               temp_string.erase(0, 1);
            }
         }

         if ( temp_string.empty() == true or temp_string.at( 0 ) not_eq ';' )
         {
            //WFCTRACE( TEXT( "Ill-formed character reference" ) );
            return( false );
         }

         temp_string.erase(0, 1);

         uint32_t character_to_add_to_string = 0;

         if ( is_hexadecimal_character_reference == true)
         {
            character_to_add_to_string = static_cast<uint32_t>(as_hexadecimal_integer(number_string));
         }
         else
         {
            character_to_add_to_string = static_cast<uint32_t>(as_integer(number_string));
         }

         if ( Win32FoundationClasses::is_xml_Char( character_to_add_to_string ) == false )
         {
            //WFCTRACE( TEXT( "Illegal XML character resolved" ) );
            return( false );
         }

         resolved_string.push_back( static_cast< wchar_t >( as_integer(number_string) ) );
         location_of_character_reference = temp_string.find(WSTRING_VIEW(L"&#"));
      }

      resolved_string.append( temp_string );
   }

   // The list is sorted for quick searching using a binary search

   if (m_Entities.empty() == true)
   {
       (void)m_Entities.push_back(std::wstring(entity));
       (void)m_Values.push_back(resolved_string);
       return(true);
   }

   auto const index = add_to_unique_sorted_vector(entity, m_Entities);

   if (index >= (SSIZE_T) m_Values.size())
   {
       // Appended
       m_Values.push_back(resolved_string);
   }
   else
   {
       if (m_Values.size() == m_Entities.size())
       {
           // Replace an existing value
           m_Values[index] = resolved_string;
       }
       else
       {
           // Insert new value at this index
           m_Values.insert(m_Values.begin() + index, resolved_string);
       }
   }

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageEntities::Enumerate( _Inout_ std::size_t& enumerator ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   ASSERT( m_Entities.size() == m_Values.size() );

   enumerator = 0;

   if ( m_Entities.size() > 0 )
   {
      return( true );
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageEntities::GetNext( _Inout_ std::size_t& enumerator, _Out_ std::wstring& entity, _Out_ std::wstring& value ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   entity.clear();
   value.clear();

   ASSERT( m_Entities.size() == m_Values.size() );

   if ( enumerator < m_Entities.size() )
   {
      entity = m_Entities[ enumerator ];
      value = m_Values[ enumerator ];

      enumerator++;

      return( true );
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageEntities::IsEntity( _In_ std::wstring_view entity, _Out_ uint32_t& rule_that_was_broken ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   // First, check to see if this is a mathematical entity.

   rule_that_was_broken = 0;

   if ( entity.length() < 3 )
   {
      // We must have a leading & and trailing ; and at least one character in the middle

      rule_that_was_broken = 68;
      return( false );
   }

   if ( entity[ 0 ] not_eq '&' )
   {
      rule_that_was_broken = 68;
      return( false );
   }

   if ( entity[entity.length() - 1 ] not_eq ';' )
   {
      rule_that_was_broken = 68;
      return( false );
   }

   if ( entity[ 1 ] == '#' )
   {
      // Check to see if mathematical entity is possible

      if ( entity.length() == 3 )
      {
         rule_that_was_broken = 66;
         return( false );
      }

      // Let's see if it is hexadecimal or decimal

      if ( entity[ 2 ] == 'X' )
      {
         rule_that_was_broken = 66;
         return( false );
      }

      if ( entity[ 2 ] == 'x' )
      {
         // Yup, we be hexadecimal

         // loop through the digits but skip the trailing ;

         // We use entity_length - 4 because we need to skip the &#x at the beginning and ; at the end

         for ( auto const loop_index : Range( entity.length() - 4 ) )
         {
            if ( WFC_IS_HEXADECIMAL( entity[ loop_index + 3 ] ) == true ) // + 3 skips &#x
            {
               rule_that_was_broken = 66;
               return( false );
            }
         }

         return( true );
      }
      else
      {
         // Nope, we must be decimal

         // We use entity_length - 3 because we need to skip the &# at the beginning and ; at the end
         for ( auto const loop_index : Range( entity.length() - 3 ) )
         {
            if (fast_is_digit( entity[ loop_index + 2 ] ) == false ) // + 2 skips &#
            {
               rule_that_was_broken = 66;
               return( false );
            }
         }

         return( true );
      }
   }

   // Welp, it weren't no mathematical entity

   // Now validate the name characters according to Rule 68->5

   if ( Win32FoundationClasses::is_xml_Letter( entity[ 1 ] ) == false and
        entity[ 1 ] not_eq '_' and
        entity[ 1 ] not_eq ':' )
   {
      //WFCTRACE( TEXT( "Bad first character of entity name." ) );
      rule_that_was_broken = 5;
      return( false );
   }

   for ( auto const loop_index : Range(entity.length() - 1, StartingRangePosition(2)) )
   {
      if ( Win32FoundationClasses::is_xml_NameChar( entity[ loop_index ] ) == false )
      {
         //WFCTRACEVAL( TEXT( "This entity is bad " ), entity );
         //WFCTRACEVAL( TEXT( "This character index " ), (uint32_t) loop_index );
         rule_that_was_broken = 5;
         return( false );
      }
   }

   if (m_Entities.empty() == true)
   {
       rule_that_was_broken = 0;
       return(false);
   }

   if (std::binary_search(std::begin(m_Entities), std::end(m_Entities), entity) == false)
   {
       // Returning false with rule set to zero means it is well-formed
       // but it is not in our database.

       return(false);
   }

   return(true);
}

_Check_return_ bool Win32FoundationClasses::CExtensibleMarkupLanguageEntities::Resolve( _In_ std::wstring_view entity, _Out_ std::wstring& text ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   ASSERT( m_Entities.size() == m_Values.size() );

   text.clear();

   // First, check to see if this is a mathematical entity.

   if (entity.length() < 3 )
   {
      // We must have a leading & and trailing ; and at least one character in the middle

      text.clear();
      return( false );
   }

   if ( entity[ 0 ] not_eq '&' )
   {
      return( false );
   }

   if ( entity[entity.length() - 1 ] not_eq ';' )
   {
      return( false );
   }

   if ( entity[ 1 ] == '#' )
   {
      // Check to see if mathematical entity is possible

      if (entity.length() == 3 )
      {
         //WFCTRACE( TEXT( "No body for the mathematical entity." ) );
         text.clear();
         return( false );
      }

      // Let's see if it is hexadecimal or decimal

      if ( entity[ 2 ] == 'X' )
      {
         //WFCTRACE( TEXT( "Ill-formed entity, it uses a capital X instead of x." ) );
         text.clear();
         return( false );
      }

      wchar_t number_string[ 12 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // insanely large buffer

      std::size_t number_index = 0;

      long translated_character = 0;

      if ( entity[ 2 ] == 'x' )
      {
         // Yup, we be hexadecimal

         // loop through the digits but skip the trailing ;

         // We use entity_length - 4 because we need to skip the &#x at the beginning and ; at the end

         for ( auto const loop_index : Range(entity.length() - 4 ) )
         {
            if (iswxdigit( entity[ loop_index + 3 ] ) == 0 ) // + 3 skips &#x
            {
               //WFCTRACEVAL( TEXT( "Ill-formed hexadecimal entity. Character is not a hexadecimal digit at index " ), (uint32_t) ( loop_index + 3 ) );
               text.clear();
               return( false );
            }

            if ( number_index > 0 )
            {
               if ( number_index < 11 )
               {
                  number_string[ number_index ] = entity[ loop_index + 3 ];
                  number_index++;
                  number_string[ number_index ] = 0; // zero terminate the string
               }
            }
            else
            {
               // Skip any leading zeroes
               if ( entity[ loop_index + 3 ] not_eq '0' )
               {
                  number_string[ number_index ] = entity[ loop_index + 3 ];
                  number_index++;
                  number_string[ number_index ] = 0; // zero terminate the string
               }
            }
         }

         // number_string now contains the hex number of the character

         wchar_t * stopped_at_address = nullptr;

         translated_character = wcstol( number_string, &stopped_at_address, 16 );

         text.push_back( (wchar_t) translated_character );

         return( true );
      }
      else
      {
         // Nope, we must be decimal

         // We use entity_length - 3 because we need to skip the &# at the beginning and ; at the end
         for ( auto const loop_index : Range(entity.length() - 3 ) )
         {
            if (fast_is_digit( entity[ loop_index + 2 ] ) == false ) // + 2 skips &#
            {
               //WFCTRACEVAL( TEXT( "Ill-formed decimal entity. Character is not a digit at index " ), (uint32_t) ( loop_index + 2 ) );
               text.clear();
               return( false );
            }

            if ( number_index > 0 )
            {
               if ( number_index < 11 )
               {
                  number_string[ number_index ] = entity[ loop_index + 2 ];
                  number_index++;
                  number_string[ number_index ] = 0; // zero terminate the string
               }
            }
            else
            {
               // Skip any leading zeroes
               if ( entity[loop_index + 2 ] not_eq '0' )
               {
                  number_string[ number_index ] = entity[ loop_index + 2 ];
                  number_index++;
                  number_string[ number_index ] = 0; // zero terminate the string
               }
            }
         }

         // number_string now contains the decimal number of the character

         translated_character = static_cast<wchar_t>(as_integer( number_string ));

         text.push_back( static_cast<wchar_t>(translated_character) );

         return( true );
      }
   }

   // Welp, it weren't no mathematical entity

   if (m_Entities.empty() == true)
   {
       text.clear();
       return(false);
   }

   auto lower = std::lower_bound(std::cbegin(m_Entities), std::cend(m_Entities), entity);
   bool const found = lower not_eq std::cend(m_Entities) and not (entity < *lower);

   if (found == true)
   {
       std::size_t const array_index = std::distance(std::cbegin(m_Entities), lower);
       text = m_Values[ array_index ];
       return( true );
   }

   // Entity was not found

   //WFCTRACEVAL( TEXT( "Cannot resolve " ), entity );

   text.clear();
   return( false );
}

_Check_return_ Win32FoundationClasses::CExtensibleMarkupLanguageEntities& Win32FoundationClasses::CExtensibleMarkupLanguageEntities::operator=( _In_ Win32FoundationClasses::CExtensibleMarkupLanguageEntities const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CExtensibleMarkupLanguageEntities</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, push technology, source code">
<META name="description" content="The C++ class that handles XML entities.">
</HEAD>

<BODY>

<H1>CExtensibleMarkupLanguageEntities</H1>

$Revision: 24 $

<HR>

<H2>Description</H2>

This class holds the name and value of an XML entity. It is a very
trivial class.

<H2>Methods</H2>

<DL COMPACT>

<DT><B><A NAME="Copy">Copy</A></B><DD>Copies another <B>CExtensibleMarkupLanguageEntities</B>.

<DT><B><A NAME="Empty">Empty</A></B><DD>Clears all data members.

</DL>

<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CExtensibleMarkupLanguageEntities.cpp $<BR>
$Modtime: 6/26/01 10:46a $
</BODY>

</HTML>
*/
