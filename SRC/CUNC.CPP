/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2020, Samuel R. Blackburn
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
** $Workfile: cunc.cpp $
** $Revision: 18 $
** $Modtime: 6/26/01 10:52a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CUniversalNamingConvention::CUniversalNamingConvention() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CUniversalNamingConvention::CUniversalNamingConvention( _In_ Win32FoundationClasses::CUniversalNamingConvention const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CUniversalNamingConvention::CUniversalNamingConvention( _In_ Win32FoundationClasses::CUniformResourceLocator const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CUniversalNamingConvention::CUniversalNamingConvention( __in_z LPCTSTR source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

_Check_return_ int Win32FoundationClasses::CUniversalNamingConvention::Compare( _In_ Win32FoundationClasses::CUniversalNamingConvention const& source ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( this == &source )
   {
      return( I_AM_EQUAL_TO_THAT );
   }

   return( compare_no_case( UNC, source.UNC ) );
}

void Win32FoundationClasses::CUniversalNamingConvention::Copy( _In_ Win32FoundationClasses::CUniversalNamingConvention const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this not_eq &source );

   /*
   ** Make sure we ain't copying ourselves
   */

   if ( this == &source )
   {
      return;
   }

   ServerName.assign( source.ServerName );
   ShareName.assign( source.ShareName );
   PathName.assign( source.PathName );

   /*
   ** Its safer if we construct the UNC
   */

   Make();
}

void Win32FoundationClasses::CUniversalNamingConvention::Copy( _In_ Win32FoundationClasses::CUniformResourceLocator const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Empty();

   ServerName.assign( source.MachineName );
   PathName.assign( source.PathName );

   std::size_t location_of_slash = PathName.find( '/' );

   if ( location_of_slash not_eq std::wstring::npos )
   {
      ShareName.assign( PathName.substr( 0, location_of_slash ) );
      PathName.assign( right( PathName, ( PathName.length() - location_of_slash ) - 1 ) );
   }

   /*
   ** Now go through the Path and convert /'s to \'s
   */

   for ( auto const path_index : Range(PathName.length()) )
   {
      if ( PathName.at(path_index) == '/' )
      {
         PathName.at(path_index) = '\\';
      }
   }

   Make();
}

void Win32FoundationClasses::CUniversalNamingConvention::Copy( __in_z LPCTSTR source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      /*
      ** Input should be "\\server_name\share_name\directory\filename.ext"
      */

      /*
      ** Make sure we start out with a virgin object
      */

      Empty();

      if ( source == nullptr )
      {
         return;
      }

      std::wstring temp_string( source );

      if ( temp_string.length() < 2 or temp_string.substr( 0, 2 ).compare(WSTRING_VIEW(L"\\\\")) not_eq I_AM_EQUAL_TO_THAT )
      {
         PathName.assign( temp_string );
         Make();
         return;
      }

      /*
      ** See if the first two characters are back slashes, if so, rip them off
      */

      while( temp_string.at( 0 ) == '\\' )
      {
         temp_string.erase(0, 1);
      }

      std::size_t location_of_back_slash = temp_string.find( '\\' );

      /*
      ** First field is ServerName
      */

      if ( location_of_back_slash == std::wstring::npos )
      {
         /*
         ** The user sent us something like "\\dodah.txt"
         */

         PathName.assign( temp_string );
         Make();
         return;
      }

      /*
      ** First field is server name
      */

      ServerName.assign( temp_string.substr( 0, location_of_back_slash ) );

      temp_string.assign( right( temp_string, ( temp_string.length() - location_of_back_slash ) - 1 ) );

      /*
      ** Second field is ShareName
      */

      location_of_back_slash = temp_string.find( '\\' );

      if ( location_of_back_slash == std::wstring::npos )
      {
         /*
         ** User sent us something like "\\server_name\"
         */

         Make();
         return;
      }

      ShareName.assign( temp_string.substr( 0, location_of_back_slash ) );
      PathName.assign( right( temp_string, ( temp_string.length() - location_of_back_slash ) - 1 ) );

      Make();
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CUniversalNamingConvention::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ServerName.clear();
   ShareName.clear();
   PathName.clear();
   UNC.clear();
}

void Win32FoundationClasses::CUniversalNamingConvention::Make( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

#if ! defined( WFC_STL )
   TRACE( TEXT( "CUniversalNamingConvention::Make() : Adding \"\\\\\" + \"%s\" + \"\\\" + \"%s\" + \"\\\" + \"%s\"\n" ),
          (LPCTSTR) ServerName,
          (LPCTSTR) ShareName,
          (LPCTSTR) PathName );
#endif // WFC_STL

   UNC.assign(WSTRING_VIEW(L"\\\\"));
   UNC.append( ServerName );
   UNC.push_back( '\\' );
   UNC.append( ShareName );
   UNC.push_back( '\\' );
   UNC.append( PathName );
}

_Check_return_ Win32FoundationClasses::CUniversalNamingConvention& Win32FoundationClasses::CUniversalNamingConvention::operator = ( _In_ Win32FoundationClasses::CUniversalNamingConvention const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

_Check_return_ Win32FoundationClasses::CUniversalNamingConvention& Win32FoundationClasses::CUniversalNamingConvention::operator = ( _In_ Win32FoundationClasses::CUniformResourceLocator const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

_Check_return_ Win32FoundationClasses::CUniversalNamingConvention& Win32FoundationClasses::CUniversalNamingConvention::operator = ( __in_z LPCTSTR source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
   return( *this );
}

_Check_return_ bool Win32FoundationClasses::CUniversalNamingConvention::operator == ( _In_ Win32FoundationClasses::CUniversalNamingConvention const& right_unc ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( Compare( right_unc ) == I_AM_EQUAL_TO_THAT)
   {
      return( true );
   }
   else
   {
      return( false );
   }
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CUniversalNamingConvention::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );

   dump_context << TEXT( "ServerName = \"" ) << ServerName << TEXT( "\"\n" );
   dump_context << TEXT( "ShareName  = \"" ) << ShareName  << TEXT( "\"\n" );
   dump_context << TEXT( "PathName   = \"" ) << PathName   << TEXT( "\"\n" );
   dump_context << TEXT( "UNC        = \"" ) << UNC        << TEXT( "\"\n" );
}

#endif // _DEBUG

// End of source
