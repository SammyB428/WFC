/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2015, Samuel R. Blackburn
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
** $Workfile: curl.cpp $
** $Revision: 16 $
** $Modtime: 6/26/01 10:52a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

USING_WFC_NAMESPACE

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

CUniformResourceLocator::CUniformResourceLocator()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

CUniformResourceLocator::CUniformResourceLocator( __in const CUniformResourceLocator& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CUniformResourceLocator::CUniformResourceLocator( __in const CUniversalNamingConvention& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CUniformResourceLocator::CUniformResourceLocator( __in_z LPCTSTR source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CUniformResourceLocator::~CUniformResourceLocator()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

__checkReturn int CUniformResourceLocator::Compare( _In_ const CUniformResourceLocator& source ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( compare_no_case( URL, source.URL ) );
}

void CUniformResourceLocator::Copy(_In_ const CUniformResourceLocator& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ProtocolName.assign( source.ProtocolName );
   MachineName.assign( source.MachineName );
   PathName.assign( source.PathName );
   PortName.assign( source.PortName );
   URL.assign( source.URL );
}

void CUniformResourceLocator::Copy( __in CUniversalNamingConvention const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Empty();

   ProtocolName.assign( L"file" );
   MachineName.assign( source.ServerName );
   PathName.assign( source.ShareName );
   PathName.push_back( '/' );
   PathName.append( source.PathName );

   /*
   ** Now go through PathName to make sure it has /'s instead of \'s
   */

   for ( auto const location_of_slash : Range(PathName.length()) )
   {
      if ( PathName.at( location_of_slash ) == '\\' )
      {
         PathName.at( location_of_slash ) = '/';
      }
   }

   Make();
}

void CUniformResourceLocator::Copy( __in_z LPCTSTR source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   Empty();

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      std::wstring temp_string = source;

      std::size_t location = temp_string.find( ':' );

      if ( location == std::wstring::npos )
      {
         return;
      }

      ProtocolName.assign( temp_string.substr( 0, location ) );

      temp_string.assign( right( temp_string, ( temp_string.length() - location ) - 1 ) );

      while( temp_string.at( 0 ) == '/' )
      {
         temp_string.erase(0, 1);
      }

      location = temp_string.find( '/' );

      if ( location == std::wstring::npos )
      {
         MachineName.assign( temp_string );
      }
      else
      {
         MachineName.assign( temp_string.substr( 0, location ) );
         PathName.assign( right( temp_string, ( temp_string.length() - location ) - 1 ) );
      }

      /*
      ** Now see if MachineName has a PortName in it
      */

      location = MachineName.find( ':' );

      if ( location != std::wstring::npos )
      {
         PortName.assign( right( MachineName, ( MachineName.length() - location ) - 1 ) );
         MachineName.assign( MachineName.substr( 0, location ) );
      }

      Make();
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void CUniformResourceLocator::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ProtocolName.clear();
   MachineName.clear();
   PathName.clear();
   PortName.clear();
   URL.clear();
}

void CUniformResourceLocator::Make( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   URL.assign( ProtocolName );
   URL.append( L"://" );
   URL.append( MachineName );

   if ( PortName.empty() == false )
   {
      URL.push_back( ':' );
      URL.append( PortName );
   }

   URL.push_back( '/' );
   URL.append( PathName );
}

__checkReturn CUniformResourceLocator& CUniformResourceLocator::operator = ( __in const CUniformResourceLocator& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

__checkReturn CUniformResourceLocator& CUniformResourceLocator::operator = ( __in const CUniversalNamingConvention& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

__checkReturn CUniformResourceLocator& CUniformResourceLocator::operator = ( __in_z LPCTSTR source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
   return( *this );
}

__checkReturn bool CUniformResourceLocator::operator == ( __in const CUniformResourceLocator& right_url ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( Compare( right_url ) == I_AM_EQUAL_TO_THAT)
   {
      return( true );
   }
   else
   {
      return( false );
   }
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CUniformResourceLocator::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );

   dump_context << TEXT( "ProtocolName = \"" ) << ProtocolName << TEXT( "\"\n" );
   dump_context << TEXT( "MachineName  = \"" ) << MachineName  << TEXT( "\"\n" );
   dump_context << TEXT( "PathName     = \"" ) << PathName     << TEXT( "\"\n" );
   dump_context << TEXT( "PortName     = \"" ) << PortName     << TEXT( "\"\n" );
   dump_context << TEXT( "URL          = \"" ) << URL          << TEXT( "\"\n" );
}

#endif // _DEBUG

// End of source
