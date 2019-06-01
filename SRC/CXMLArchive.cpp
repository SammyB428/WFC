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
** $Workfile: CXMLArchive.cpp $
** $Revision: 22 $
** $Modtime: 6/26/01 10:53a $
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

// 1998-04-25 - Thanks go to Alexander Sokolsky (sokolsky@who.net)
// for pointing out my flawed assumption that all times are
// UTC time.

static inline void _append_time_zone_offset( __inout std::wstring& time_string )
{
   TIME_ZONE_INFORMATION time_zone_information;

   ZeroMemory( &time_zone_information, sizeof( time_zone_information ) );

   DWORD zone_identifier = GetTimeZoneInformation( &time_zone_information );

   if ( zone_identifier == 0xFFFFFFFF )
   {
      return;
   }

   int offset_hours   = time_zone_information.Bias / 60;
   int offset_minutes = time_zone_information.Bias % 60;

   offset_hours   *= (-1);
   offset_minutes *= (-1);

   TCHAR offset_character = TEXT( '+' );

   if ( offset_hours < 0 || offset_minutes < 0 )
   {
      if ( offset_hours < 0 )
      {
         offset_hours *= (-1);
      }

      if ( offset_minutes < 0 )
      {
         offset_minutes *= (-1);
      }

      offset_character = TEXT( '-' );
   }

   if ( offset_hours == 0 && offset_minutes == 0 )
   {
      time_string.push_back('Z');
   }
   else
   {
      std::wstring zone_string;

      format( zone_string, L"X%02d:%02d", offset_hours, offset_minutes );
      zone_string.at( 0 ) = offset_character;

      time_string.append( zone_string );
   }
}

CXMLArchive::CXMLArchive()
{
   WFC_VALIDATE_POINTER( this );

   m_AddNewLineAfterEachElement = false;
   m_AmIWriting = false;
   m_Element_p  = nullptr;
   _append_time_zone_offset( m_TimeZone );
}

CXMLArchive::~CXMLArchive()
{
   WFC_VALIDATE_POINTER( this );

   m_AddNewLineAfterEachElement = false;
   m_AmIWriting = false;
   m_Element_p  = nullptr;
}

__checkReturn bool CXMLArchive::GetAddNewLineAfterEachElement( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_AddNewLineAfterEachElement );
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::GetElement( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_Element_p );
}

__checkReturn bool CXMLArchive::IsLoading( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false )
   {
      return( true );
   }

   return( false );
}

__checkReturn bool CXMLArchive::IsStoring( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting != false )
   {
      return( true );
   }

   return( false );
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read(_In_z_ const wchar_t * tag, __out bool& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   value = false;

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = nullptr;

      uint32_t boolean_value = 0;

      // We save them as a DWORD

      sub_element_p = Read( tag, boolean_value );

      if ( sub_element_p != nullptr )
      {
         if ( boolean_value == 0 )
         {
            value = FALSE;
         }
         else
         {
            value = TRUE;
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read(_In_z_ const wchar_t * tag, __out std::vector<uint8_t>& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
      value.clear();
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = nullptr;

      // Always start from a known state
      value.clear();

      sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         CBase64Coding coder;

         std::wstring contents;
         
         sub_element_p->GetText( contents );

         (void) coder.Decode( contents.c_str(), value );
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read(_In_z_ const wchar_t * tag, __out std::vector<uint32_t>& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   value.clear();

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      std::wstring data;

      auto sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         // Cool! We have the top level element. Now we
         // need to cycle through the sub-elements.

         DWORD index = 0;

         bool exit_loop = false;

         while( exit_loop == false )
         {
             auto entry_p = sub_element_p->GetChild( L"ENTRY", index );

            if ( entry_p == nullptr )
            {
               //WFCTRACE( TEXT( "Exitting" ) );
               // We've reached the last entry
               exit_loop = true;
            }
            else
            {
               entry_p->GetText( data );

               value.push_back( _wtol( data.c_str() ) );

               index++;
            }
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read(_In_z_ const wchar_t * tag, _Out_ std::wstring& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   value.clear();

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = nullptr;

      sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         sub_element_p->GetText( value );
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ CExtensibleMarkupLanguageElement * CXMLArchive::Read( _In_z_ const wchar_t * tag, __out std::vector<std::wstring>& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   value.clear();

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      std::wstring data;

      auto sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         // Cool! We have the top level element. Now we
         // need to cycle through the sub-elements.

         DWORD index = 0;

         bool exit_loop = false;

         CExtensibleMarkupLanguageElement * entry_p = nullptr;

         while( exit_loop == false )
         {
            entry_p = sub_element_p->GetChild( L"ENTRY", index );

            if ( entry_p == nullptr )
            {
               // We've reached the last entry
               exit_loop = true;
            }
            else
            {
               entry_p->GetText( data );

               (void) value.push_back( data );

               index++;
            }
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read( _In_z_ const wchar_t * tag, __out double& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
       value = 0.0;
      return( nullptr );
   }

   // Always start from a known state
   value = 0.0;

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = nullptr;

      sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         std::wstring string_value;

         sub_element_p->GetText( string_value );

         value = _ttof( string_value.c_str() );
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read( _In_z_ const wchar_t * tag, __out uint32_t& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   value = 0;

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = nullptr;

      sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         std::wstring string_value;

         sub_element_p->GetText( string_value );

         value = ::_ttol( string_value.c_str() );
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read( _In_z_ const wchar_t * tag, __out int64_t& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   value = 0;

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = nullptr;

      sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         std::wstring string_value;

         sub_element_p->GetText( string_value );

         value = ::_ttoi64( string_value.c_str() );
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read( _In_z_ const wchar_t * tag, __out uint64_t& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   value = 0;

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = nullptr;

      sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         std::wstring string_value;

         sub_element_p->GetText( string_value );

         value = (ULONGLONG) ::_ttoi64( string_value.c_str() );
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read( _In_z_ const wchar_t * tag, __out Win32FoundationClasses::CFileTime& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   value.Empty();

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = nullptr;

      sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         std::wstring string_value;

         sub_element_p->GetText( string_value );

         (void) ::wfc_parse_iso_8601_string( string_value.c_str(), value );
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

#if ! defined( WFC_STL )

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read( _In_z_ const wchar_t * tag, __out COleDateTime& value )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = nullptr;

      sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         std::wstring string_value;

         sub_element_p->GetText( string_value );

         ::wfc_parse_iso_8601_string( string_value, value );
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read( _In_z_ const wchar_t * tag, __out COleDateTimeSpan& value )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = nullptr;

      sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         std::wstring string_value;

         sub_element_p->GetText( string_value );

         value = COleDateTimeSpan( 0, 0, 0, ::_ttol( string_value ) );
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

#endif // WFC_STL

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read( _In_z_ const wchar_t * tag, __out CSystemTime& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   value.Empty();

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = nullptr;

      sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         std::wstring string_value;

         sub_element_p->GetText( string_value );

         (void) wfc_parse_iso_8601_string( string_value.c_str(), value );
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read( _In_ const wchar_t * tag, __out CTime& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER(tag);

   value.Empty();

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = nullptr;

      sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         std::wstring string_value;

         sub_element_p->GetText( string_value );

         (void) ::wfc_parse_iso_8601_string( string_value.c_str(), value );
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Read( _In_z_ const wchar_t * tag, __out CTimeSpan& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   value.Empty();

   if ( m_AmIWriting != false || m_Element_p == nullptr )
   {
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = nullptr;

      sub_element_p = m_Element_p->GetChild( tag );

      if ( sub_element_p != nullptr )
      {
         std::wstring string_value;

         sub_element_p->GetText( string_value );

         value = CTimeSpan( 0, 0, 0, ::_ttol( string_value.c_str() ) );
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

void CXMLArchive::ReadFrom( __inout CExtensibleMarkupLanguageElement * element_p ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_AmIWriting = false;
   m_Element_p  = element_p;
}

__checkReturn bool CXMLArchive::SerializeObject( _In_z_ const wchar_t * tag, __inout void * object_p, __callback XML_ARCHIVE_SERIALIZE_OBJECT serialize_function ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( object_p == nullptr )
   {
      //WFCTRACE( TEXT( "Object parameter is nullptr!" ) );
      return( false );
   }

   if ( serialize_function == nullptr )
   {
      //WFCTRACE( TEXT( "Callback is nullptr!" ) );
      return( false );
   }

   if ( wcslen( tag ) == 0 )
   {
      //WFCTRACE( TEXT( "Tag is empty!" ) );
      return( false );
   }

   bool return_value = false;

   // We were passed pointers, don't trust them

   WFC_TRY
   {
      return_value = serialize_function( this, tag, object_p );
   }
   WFC_CATCH_ALL
   {
      return_value = false;
   }
   WFC_END_CATCH_ALL

   return( return_value );
}

void CXMLArchive::SetAddNewLineAfterEachElement( __in const bool add_new_line ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_AddNewLineAfterEachElement = add_new_line;
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const bool value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         // Since we're XML, we must convert everything to a string

         sub_element_p->SetTag( tag );

         if ( value == FALSE )
         {
            (void) sub_element_p->AddText( L"0" );
         }
         else
         {
            (void) sub_element_p->AddText( L"1" );
         }

         if ( m_AddNewLineAfterEachElement != false )
         {
            (void) m_Element_p->AddText( TEXT( "\n" ) );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const std::vector<uint8_t>& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         std::wstring contents;

         CBase64Coding encoder;

         (void) encoder.Encode( value, contents );

         sub_element_p->SetTag( tag );
         (void) sub_element_p->AddText( contents.c_str() );

         if ( m_AddNewLineAfterEachElement != false )
         {
            (void) m_Element_p->AddText( TEXT( "\n" ) );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const std::vector<uint32_t>& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
      return( nullptr );
   }

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         sub_element_p->SetTag( tag );

         if ( m_AddNewLineAfterEachElement != false )
         {
            (void) m_Element_p->AddText( TEXT( "\n" ) );
         }

         CXMLArchive entry_archiver;

         entry_archiver.SetAddNewLineAfterEachElement( m_AddNewLineAfterEachElement );

         entry_archiver.WriteTo( sub_element_p );

         for ( auto const array_index : Range(value.size()))
         {
            (void) entry_archiver.Write( L"ENTRY", value.at( array_index ) );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const Win32FoundationClasses::CFileTime& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // In a perfect object world, we would simply convert the value parameter
      // to a string and call the Write() method that takes two strings and let
      // that method create the new sub element. However, that would make memory
      // leak detection harder since everything allocated would be from that
      // one location. This way, we get a better chance of tracking down the
      // offending leaker.

      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         // Since we're XML, we must convert everything to a string

         CTime the_time;

         value.CopyTo( the_time );

         std::wstring contents(the_time.Format( L"%Y-%m-%dT%H:%M:%S"));

         // 1998-04-25 - Thanks go to Alexander Sokolsky (sokolsky@who.net)
         // for finding a bug here. I wasn't including the time zone
         // offset information.

         contents.append( m_TimeZone );

         sub_element_p->SetTag( tag );
         (void) sub_element_p->AddText( contents.c_str() );

         if ( m_AddNewLineAfterEachElement != false )
         {
            (void) m_Element_p->AddText( TEXT( "\n" ) );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

#if ! defined( WFC_STL )

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const COleDateTime& value, BOOL value_is_UTC_time )
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // In a perfect object world, we would simply convert the value parameter
      // to a string and call the Write() method that takes two strings and let
      // that method create the new sub element. However, that would make memory
      // leak detection harder since everything allocated would be from that
      // one location. This way, we get a better chance of tracking down the
      // offending leaker.

      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         // Since we're XML, we must convert everything to a string

         std::wstring contents;

         format( contents, L"%Y-%m-%dT%H:%M:%S" );

         // 1998-04-25 - Thanks go to Alexander Sokolsky (sokolsky@who.net)
         // for finding a bug here. I wasn't including the time zone
         // offset information.

         if ( value_is_UTC_time == FALSE )
         {
            // Convert to UTC
            contents += m_TimeZone;
         }
         else
         {
            // Time value is UTC
            contents += TEXT( "Z" );
         }

         sub_element_p->SetTag( tag );
         sub_element_p->AddText( contents );

         if ( m_AddNewLineAfterEachElement != false )
         {
            m_Element_p->AddText( TEXT( "\n" ) );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const COleDateTimeSpan& value )
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // In a perfect object world, we would simply convert the value parameter
      // to a string and call the Write() method that takes two strings and let
      // that method create the new sub element. However, that would make memory
      // leak detection harder since everything allocated would be from that
      // one location. This way, we get a better chance of tracking down the
      // offending leaker.

      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         // Since we're XML, we must convert everything to a string

         std::wstring contents;

         format( contents, L"%lu", value.GetTotalSeconds() );

         sub_element_p->SetTag( tag );
         sub_element_p->AddText( contents );

         if ( m_AddNewLineAfterEachElement != false )
         {
            m_Element_p->AddText( TEXT( "\n" ) );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

#endif // WFC_STL

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const std::wstring& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
      return( nullptr );
   }

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         sub_element_p->SetTag( tag );
         (void) sub_element_p->AddText( value.c_str() );

         if ( m_AddNewLineAfterEachElement != false )
         {
            (void) m_Element_p->AddText( L"\n" );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const std::vector<std::wstring>& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
      return( nullptr );
   }

   WFC_TRY
   {
      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         sub_element_p->SetTag( tag );

         if ( m_AddNewLineAfterEachElement != false )
         {
            (void) m_Element_p->AddText( TEXT( "\n" ) );
         }

         CXMLArchive entry_archiver;

         entry_archiver.SetAddNewLineAfterEachElement( m_AddNewLineAfterEachElement );

         entry_archiver.WriteTo( sub_element_p );

         std::size_t array_index       = 0;
         std::size_t number_of_entries = value.size();

         while( array_index < number_of_entries )
         {
            (void) entry_archiver.Write( L"ENTRY", value.at( array_index ) );
            array_index++;
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const CSystemTime& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // In a perfect object world, we would simply convert the value parameter
      // to a string and call the Write() method that takes two strings and let
      // that method create the new sub element. However, that would make memory
      // leak detection harder since everything allocated would be from that
      // one location. This way, we get a better chance of tracking down the
      // offending leaker.

      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         // Since we're XML, we must convert everything to a string

         CTime the_time;

         value.CopyTo( the_time );

         // System Time is always UTC

         std::wstring contents( the_time.Format( L"%Y-%m-%dT%H:%M:%SZ") );

         sub_element_p->SetTag( tag );
         (void) sub_element_p->AddText( contents.c_str() );

         if ( m_AddNewLineAfterEachElement != false )
         {
            (void) m_Element_p->AddText( L"\n" );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const CTime& value, __in const bool value_is_UTC_time ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // In a perfect object world, we would simply convert the value parameter
      // to a string and call the Write() method that takes two strings and let
      // that method create the new sub element. However, that would make memory
      // leak detection harder since everything allocated would be from that
      // one location. This way, we get a better chance of tracking down the
      // offending leaker.

      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         // Since we're XML, we must convert everything to a string

         std::wstring contents( value.Format( L"%Y-%m-%dT%H:%M:%S" ) );

         // 1998-04-25 - Thanks go to Alexander Sokolsky (sokolsky@who.net)
         // for finding a bug here. I wasn't including the time zone
         // offset information.

         if ( value_is_UTC_time == false )
         {
            // Time value needs to be converted to UTC
            contents.append( m_TimeZone );
         }
         else
         {
            // Time value is already UTC
            contents.push_back('Z');
         }

         sub_element_p->SetTag( tag );
         (void) sub_element_p->AddText( contents.c_str() );

         if ( m_AddNewLineAfterEachElement != false )
         {
            (void) m_Element_p->AddText( L"\n" );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const CTimeSpan& value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // In a perfect object world, we would simply convert the value parameter
      // to a string and call the Write() method that takes two strings and let
      // that method create the new sub element. However, that would make memory
      // leak detection harder since everything allocated would be from that
      // one location. This way, we get a better chance of tracking down the
      // offending leaker.

      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         // Since we're XML, we must convert everything to a string

         std::wstring contents;

         format( contents, L"%I64u", static_cast<uint64_t>(value.GetTotalSeconds()) );

         sub_element_p->SetTag( tag );
         (void) sub_element_p->AddText( contents.c_str() );

         if ( m_AddNewLineAfterEachElement != false )
         {
            (void) m_Element_p->AddText( TEXT( "\n" ) );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in double value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // In a perfect object world, we would simply convert the value parameter
      // to a string and call the Write() method that takes two strings and let
      // that method create the new sub element. However, that would make memory
      // leak detection harder since everything allocated would be from that
      // one location. This way, we get a better chance of tracking down the
      // offending leaker.

      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         // Since we're XML, we must convert everything to a string

         std::wstring contents;

         format( contents, L"%.15lf", value );

         // We'll be friendly here and strip off trailing zeroes

         bool exit_loop = false;

         while( exit_loop == false )
         {
            if ( contents.empty() == false )
            {
               if ( contents.at( contents.length() - 1 ) == '0' )
               {
                  contents.assign(contents.substr( 0, contents.length() - 1 ));
               }
               else
               {
                  exit_loop = true;
               }
            }
            else
            {
               exit_loop = true;
            }
         }

         if ( contents.empty() == true )
         {
            contents.assign(L"0.0");
         }

         sub_element_p->SetTag( tag );
         (void) sub_element_p->AddText( contents.c_str() );

         if ( m_AddNewLineAfterEachElement != false )
         {
            (void) m_Element_p->AddText( TEXT( "\n" ) );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const uint32_t value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // In a perfect object world, we would simply convert the value parameter
      // to a string and call the Write() method that takes two strings and let
      // that method create the new sub element. However, that would make memory
      // leak detection harder since everything allocated would be from that
      // one location. This way, we get a better chance of tracking down the
      // offending leaker.

      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         // Since we're XML, we must convert everything to a string

         std::wstring contents;

         format( contents, L"%lu", value );

         sub_element_p->SetTag( tag );
         (void) sub_element_p->AddText( contents.c_str() );

         if ( m_AddNewLineAfterEachElement != false )
         {
            (void) m_Element_p->AddText( TEXT( "\n" ) );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const int64_t value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // In a perfect object world, we would simply convert the value parameter
      // to a string and call the Write() method that takes two strings and let
      // that method create the new sub element. However, that would make memory
      // leak detection harder since everything allocated would be from that
      // one location. This way, we get a better chance of tracking down the
      // offending leaker.

      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         // Since we're XML, we must convert everything to a string

         std::wstring contents;

         format( contents, L"%I64d", value );

         sub_element_p->SetTag( tag );
         (void) sub_element_p->AddText( contents.c_str() );

         if ( m_AddNewLineAfterEachElement != false )
         {
            (void) m_Element_p->AddText( TEXT( "\n" ) );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

__checkReturn CExtensibleMarkupLanguageElement * CXMLArchive::Write( _In_z_ const wchar_t * tag, __in const uint64_t value ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AmIWriting == false || m_Element_p == nullptr )
   {
      //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
      return( nullptr );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // In a perfect object world, we would simply convert the value parameter
      // to a string and call the Write() method that takes two strings and let
      // that method create the new sub element. However, that would make memory
      // leak detection harder since everything allocated would be from that
      // one location. This way, we get a better chance of tracking down the
      // offending leaker.

      CExtensibleMarkupLanguageElement * sub_element_p = CExtensibleMarkupLanguageElement::NewElement( m_Element_p );

      if ( sub_element_p != nullptr )
      {
         // Since we're XML, we must convert everything to a string

         std::wstring contents;

         format( contents, L"%I64u", value );

         sub_element_p->SetTag( tag );
         (void) sub_element_p->AddText( contents.c_str() );

         if ( m_AddNewLineAfterEachElement != false )
         {
            (void) m_Element_p->AddText( TEXT( "\n" ) );
         }
      }

      return( sub_element_p );
   }
   WFC_CATCH_ALL
   {
      return( nullptr );
   }
   WFC_END_CATCH_ALL
}

void CXMLArchive::WriteTo(__inout CExtensibleMarkupLanguageElement * element_p ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_AmIWriting = true;
   m_Element_p  = element_p;
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CXMLArchive</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, XML, source code">
<META name="description" content="The C++ class that handles serializing (writing and reading) objects in XML documents.">
</HEAD>

<BODY>

<H1>CXMLArchive</H1>
$Revision: 22 $<BR><HR>

<H2>Description</H2>
This class handles serializing data (aka object persistence) to and
from XML documents. It is patterned after the MFC CArchive class.

<H2>Data Members</H2>None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="GetAddNewLineAfterEachElement">GetAddNewLineAfterEachElement</A></B>( void ) const</PRE><DD>
Returns TRUE if the class will add a new line after writing an
element to the document. This makes the resulting document easier to
read by humans.

<DT><PRE>BOOL <B><A NAME="IsLoading">IsLoading</A></B>( void ) const</PRE><DD>
Returns TRUE if this object is being used to load
data from an XML document. This is set by calling <B>ReadFrom</B>().

<DT><PRE>BOOL <B><A NAME="IsStoring">IsStoring</A></B>( void ) const</PRE><DD>
Returns TRUE if this object is being used to write
data to an XML document. This is set by calling <B>WriteTo</B>().

<DT><PRE><A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B><A NAME="Read">Read</A></B>( const std::wstring&amp; tag, BOOL&amp;             value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, std::wstring&amp;          value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, std::vector&lt;std::wstring&gt;&amp;     value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, std::vector&lt;uint8_t&gt;&amp;       value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, std::vector&lt;uint32_t&gt;&amp;      value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, <A HREF="CFileTime.htm">CFileTime</A>&amp;        value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, COleDateTime&amp;     value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, COleDateTimeSpan&amp; value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, <A HREF="CSystemTime.htm">CSystemTime</A>&amp;      value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, CTime&amp;            value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, CTimeSpan&amp;        value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, double&amp;           value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, DWORD&amp;            value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, LONGLONG&amp;         value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Read</B>( const std::wstring&amp; tag, ULONGLONG&amp;        value )</PRE><DD>
This method reads <CODE>value</CODE> from the XML document with the tag of <CODE>tag</CODE>.
The data is read from children of the element passed in <B>ReadFrom</B>().
std::vector&lt;uint8_t&gt; values are automatically encoded using Base64.
<A HREF="CFileTime.htm">CFileTime</A>, <A HREF="CSystemTime.htm">CSystemTime</A>,
and CTime values
are written in ISO8601 format. CTimeSpan values are written in seconds.
<B>Read</B>() will return a pointer to the
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A>
object upon success or NULL on failure. If <B>Read</B>() fails,
<CODE>value</CODE> will be set to an initial state.

<DT><PRE>void <B><A NAME="ReadFrom">ReadFrom</A></B>( <A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * element_p )</PRE><DD>
Tells the object where to get its data from. Subsequent calls to <B>Read</B>()
will cause data to be read from the children of <CODE>element_p</CODE>.

<DT><PRE>void <B><A NAME="SetAddNewLineAfterEachElement">SetAddNewLineAfterEachElement</A></B>( BOOL add_new_line = TRUE )</PRE><DD>
When you set this to TRUE, the object will add a line break after
writing an element. This results in an XML document that is slightly
larger but much easier to read by humans.

<DT><PRE><A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B><A NAME="Write">Write</A></B>( const std::wstring&amp; tag, BOOL                    value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, const std::vector&lt;uint8_t&gt;&amp;       value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, const std::vector&lt;uint32_t&gt;&amp;      value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, const <A HREF="CFileTime.htm">CFileTime</A>&amp;        value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, const COleDateTime&amp;     value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, const COleDateTimeSpan&amp; value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, const std::wstring&amp;          value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, const std::vector&lt;std::wstring&gt;&amp;     value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, const <A HREF="CSystemTime.htm">CSystemTime</A>&amp;      value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, const CTime&amp;            value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, const CTimeSpan&amp;        value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, double                  value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, DWORD                   value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, LONGLONG                value )
<A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * <B>Write</B>( const std::wstring&amp; tag, ULONGLONG               value )</PRE><DD>
This method writes <CODE>value</CODE> to the XML document.
A new element is created with a tag name of <CODE>tag</CODE>. The data
is written to this new element and the element is then made a child of
the element passed to <B><A HREF="#WriteTo">WriteTo()</A></B>.
std::vector&lt;uint8_t&gt; will be Base64 encoded before being written to the document.
<A HREF="CFileTime.htm">CFileTime</A>,
<A HREF="CSystemTime.htm">CSystemTime</A>,
COleDateTime, and CTime will be written in ISO8601 format. Only
<A HREF="CSystemTime.htm">CSystemTime</A> will be written with a time
zone of Z. The other representations of time will have the time zone
offset information written.
CTimeSpan and COleDateTimeSpan will be written as total number of seconds.

<DT><PRE>void <B><A NAME="WriteTo">WriteTo</A></B>( <A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * element_p )</PRE><DD>
This tells the object where to write data. When data is written to <CODE>element_p</CODE>,
new elements are created and made children of <CODE>element_p</CODE>.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

class CSimpleClass
{
   protected:

      CTime        m_Birthday;
      std::wstring m_Name;
      double       m_PriceLimit;

   public:

      void Serialize( <B>CXMLArchive</B>&amp; archive );
};

void CSimpleClass::Serialize( <B>CXMLArchive</B>&amp; archive )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;CSimpleClass::Serialize()&quot; ) );

   <A HREF="CExtensibleMarkupLanguageElement.htm">CExtensibleMarkupLanguageElement</A> * element_for_this_class = nullptr;

   <B>CXMLArchive</B> sub_archive;

   if ( archive.<A HREF="#IsStoring">IsStoring</A>() )
   {
      element_for_this_class = archive.<A HREF="#Write">Write</A>( TEXT( &quot;CSIMPLECLASS&quot; ), TEXT( &quot;&quot; ) );

      sub_archive.<A HREF="#WriteTo">WriteTo</A>( element_for_this_class );

      sub_archive.Write( TEXT( &quot;BIRTHDAY&quot;    ), m_Birthday   );
      sub_archive.Write( TEXT( &quot;NAME&quot;        ), m_Name       );
      sub_archive.Write( TEXT( &quot;PRICE_LIMIT&quot; ), m_PriceLimit );
   }
   else
   {
      element_for_this_class = archive.<A HREF="#Read">Read</A>( TEXT( &quot;CSIMPLECLASS&quot; ), TEXT( &quot;&quot; ) );

      sub_archive.<A HREF="#ReadFrom">ReadFrom</A>( element_for_this_class );

      sub_archive.Read( TEXT( &quot;BIRTHDAY&quot;    ), m_Birthday   );
      sub_archive.Read( TEXT( &quot;NAME&quot;        ), m_Name       );
      sub_archive.Read( TEXT( &quot;PRICE_LIMIT&quot; ), m_PriceLimit );
   }
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CXMLArchive.cpp $<BR>
$Modtime: 6/26/01 10:53a $
</BODY>

</HTML>
*/
