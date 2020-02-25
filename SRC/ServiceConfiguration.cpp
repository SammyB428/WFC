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
** $Workfile: ServiceConfiguration.cpp $
** $Revision: 19 $
** $Modtime: 6/26/01 10:56a $
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

// Here's the ASCII Version

Win32FoundationClasses::CServiceConfigurationA::CServiceConfigurationA()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CServiceConfigurationA::CServiceConfigurationA( _In_ Win32FoundationClasses::CServiceConfigurationA const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CServiceConfigurationA::CServiceConfigurationA( _In_ Win32FoundationClasses::CServiceConfigurationA const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CServiceConfigurationA::CServiceConfigurationA( _In_ _QUERY_SERVICE_CONFIGA const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CServiceConfigurationA::CServiceConfigurationA( _In_ _QUERY_SERVICE_CONFIGA const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CServiceConfigurationA::~CServiceConfigurationA()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

void Win32FoundationClasses::CServiceConfigurationA::Copy( _In_ Win32FoundationClasses::CServiceConfigurationA const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();

   m_TypeOfService        = source.m_TypeOfService;
   m_WhenToStart          = source.m_WhenToStart;
   m_ErrorControl         = source.m_ErrorControl;
   m_Tag                  = source.m_Tag;
   m_NameOfExecutableFile.assign( source.m_NameOfExecutableFile );
   m_LoadOrderGroup.assign( source.m_LoadOrderGroup );
   m_StartName.assign( source.m_StartName );
   m_DisplayName.assign( source.m_DisplayName );

   m_Dependencies.assign(source.m_Dependencies.cbegin(), source.m_Dependencies.cend());
}

void Win32FoundationClasses::CServiceConfigurationA::Copy( _In_ Win32FoundationClasses::CServiceConfigurationA const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   ASSERT( source != this );

   if ( source == this )
   {
      return;
   }

   // The user passed us a pointer, don't trust it

   WFC_TRY
   {
      if ( source != nullptr )
      {
         Copy( *source );
      }
      else
      {
         Empty();
      }
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CServiceConfigurationA::Copy( _In_ _QUERY_SERVICE_CONFIGA const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // The user didn't pass us a pointer but they did pass us
   // something that contains pointers so we cannot trust it

   WFC_TRY
   {
      Empty();

      // Copy the data structure stuff first...

      m_TypeOfService      = source.dwServiceType;
      m_WhenToStart        = source.dwStartType;
      m_ErrorControl       = source.dwErrorControl;
      m_Tag                = source.dwTagId;

      if ( source.lpBinaryPathName != nullptr )
      {
         m_NameOfExecutableFile.assign( source.lpBinaryPathName );
      }
      else
      {
         m_NameOfExecutableFile.clear();
      }

      if ( source.lpLoadOrderGroup != nullptr )
      {
         m_LoadOrderGroup.assign( source.lpLoadOrderGroup );
      }
      else
      {
         m_LoadOrderGroup.clear();
      }

      if ( source.lpServiceStartName != nullptr )
      {
         m_StartName.assign( source.lpServiceStartName );
      }
      else
      {
         m_StartName.clear();
      }

      if ( source.lpDisplayName != nullptr )
      {
         m_DisplayName.assign( source.lpDisplayName );
      }
      else
      {
         m_DisplayName.clear();
      }

      // Now for the fun part, double-NULL terminated string....

      std::string string_to_add;

      if ( source.lpDependencies != nullptr )
      {
         BYTE *temp_pointer = (BYTE *) source.lpDependencies;

         while ( temp_pointer[ 0 ] != 0x00 )
         {
            string_to_add.assign(reinterpret_cast<const char *>(temp_pointer));

            m_Dependencies.push_back( string_to_add );

            // Thanks go to Ullrich Pollaehne [Ullrich_Pollaehne@ka2.maus.de]
            // for noticing my stupidity here (i.e. finding a bug);
            temp_pointer += ::strlen( (LPCSTR) temp_pointer ) + 1; // YES, this should be strlen() and not _tcslen()
         }
      }
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CServiceConfigurationA::Copy( _In_ _QUERY_SERVICE_CONFIGA const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( source != nullptr )
      {
         Copy( *source );
      }
      else
      {
         Empty();
      }
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CServiceConfigurationA::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // We put a try block here because it is called from
   // catch blocks (like in the Copy() method). In other
   // words, we absolutely cannot fail.

   WFC_TRY
   {
      m_TypeOfService = 0;
      m_WhenToStart   = 0;
      m_ErrorControl  = 0;
      m_Tag           = 0;
      m_NameOfExecutableFile.clear();
      m_LoadOrderGroup.clear();
      m_StartName.clear();
      m_DisplayName.clear();
      m_Dependencies.clear();
   }
   WFC_CATCH_ALL
   {
      return;
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CServiceConfigurationA::GetDependencies( _Out_ std::vector<std::string>& dependencies ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   dependencies.assign(m_Dependencies.cbegin(), m_Dependencies.cend());
}

void Win32FoundationClasses::CServiceConfigurationA::GetDisplayName( _Out_ std::string& display_name ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   display_name.assign( m_DisplayName );
}

_Check_return_ DWORD Win32FoundationClasses::CServiceConfigurationA::GetErrorControl( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_ErrorControl );
}

void Win32FoundationClasses::CServiceConfigurationA::GetLoadOrderGroup( _Out_ std::string& load_order_group ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   load_order_group.assign( m_LoadOrderGroup );
}

void Win32FoundationClasses::CServiceConfigurationA::GetNameOfExecutableFile( _Out_ std::string& name_of_executable ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   name_of_executable.assign( m_NameOfExecutableFile );
}

void Win32FoundationClasses::CServiceConfigurationA::GetStartName( _Out_ std::string& start_name ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   start_name.assign( m_StartName );
}

_Check_return_ DWORD Win32FoundationClasses::CServiceConfigurationA::GetTag( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_Tag );
}

_Check_return_ DWORD Win32FoundationClasses::CServiceConfigurationA::GetTypeOfService( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_TypeOfService );
}

_Check_return_ DWORD Win32FoundationClasses::CServiceConfigurationA::GetWhenToStart( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_WhenToStart );
}

// Thanks go to Ullrich Pollaehne [Ullrich_Pollaehne@ka2.maus.de] for
// pointing out I had no implementation for these guys.

_Check_return_ Win32FoundationClasses::CServiceConfigurationA& Win32FoundationClasses::CServiceConfigurationA::operator=( _In_ Win32FoundationClasses::CServiceConfigurationA const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

_Check_return_ Win32FoundationClasses::CServiceConfigurationA& Win32FoundationClasses::CServiceConfigurationA::operator=( _In_ _QUERY_SERVICE_CONFIGA const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CServiceConfigurationA::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );

   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   m_TypeOfService is        "   ) << m_TypeOfService        << TEXT( "\n" );
   dump_context << TEXT( "   m_WhenToStart is          "   ) << m_WhenToStart           << TEXT( "\n" );
   dump_context << TEXT( "   m_ErrorControl is         "   ) << m_ErrorControl         << TEXT( "\n" );
   dump_context << TEXT( "   m_Tag is                  "   ) << m_Tag                  << TEXT( "\n" );
   dump_context << TEXT( "   m_NameOfExectableFile is \"" ) << m_NameOfExecutableFile << TEXT( "\"\n" );
   dump_context << TEXT( "   m_LoadOrderGroup is       \"" ) << m_LoadOrderGroup       << TEXT( "\"\n" );
   dump_context << TEXT( "   m_StartName is            \"" ) << m_StartName            << TEXT( "\"\n" );
   dump_context << TEXT( "   m_DisplayName is          \"" ) << m_DisplayName          << TEXT( "\"\n" );

   for ( auto const index : Range(m_Dependencies.GetSize()) )
   {
      dump_context << TEXT( "   m_Dependencies[ " ) << index << TEXT( " ] is \"" ) << m_Dependencies.GetAt( index ) << TEXT( "\"\n" );
   }

   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

// Here's the UNICODE version

Win32FoundationClasses::CServiceConfigurationW::CServiceConfigurationW()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CServiceConfigurationW::CServiceConfigurationW( _In_ Win32FoundationClasses::CServiceConfigurationW const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CServiceConfigurationW::CServiceConfigurationW( _In_ Win32FoundationClasses::CServiceConfigurationW const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CServiceConfigurationW::CServiceConfigurationW( _In_ QUERY_SERVICE_CONFIGW const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CServiceConfigurationW::CServiceConfigurationW( _In_ QUERY_SERVICE_CONFIGW const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CServiceConfigurationW::~CServiceConfigurationW()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

void Win32FoundationClasses::CServiceConfigurationW::Copy( _In_ Win32FoundationClasses::CServiceConfigurationW const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Empty();

   m_TypeOfService        = source.m_TypeOfService;
   m_WhenToStart          = source.m_WhenToStart;
   m_ErrorControl         = source.m_ErrorControl;
   m_Tag                  = source.m_Tag;
   m_NameOfExecutableFile.assign( source.m_NameOfExecutableFile );
   m_LoadOrderGroup.assign( source.m_LoadOrderGroup );
   m_StartName.assign( source.m_StartName );
   m_DisplayName .assign( source.m_DisplayName );

   m_Dependencies.assign(source.m_Dependencies.cbegin(), source.m_Dependencies.cend());
}

void Win32FoundationClasses::CServiceConfigurationW::Copy( _In_ Win32FoundationClasses::CServiceConfigurationW const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   ASSERT( source != this );

   if ( source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( source != nullptr )
      {
         Copy( *source );
      }
      else
      {
         Empty();
      }
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CServiceConfigurationW::Copy( _In_ QUERY_SERVICE_CONFIGW const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // The user didn't pass us a pointer but they did pass us
   // something that contains pointers so we cannot trust it

   WFC_TRY
   {
      Empty();

      // Copy the data structure stuff first...

      m_TypeOfService      = source.dwServiceType;
      m_WhenToStart        = source.dwStartType;
      m_ErrorControl       = source.dwErrorControl;
      m_Tag                = source.dwTagId;

      if ( source.lpBinaryPathName != nullptr )
      {
#if defined( UNICODE )
         m_NameOfExecutableFile.assign( source.lpBinaryPathName );
#else // UNICODE
         wfc_convert_lpcwstr_to_cstring( source.lpBinaryPathName, m_NameOfExecutableFile );
#endif // UNICODE
      }
      else
      {
         m_NameOfExecutableFile.clear();
      }

      if ( source.lpLoadOrderGroup != nullptr )
      {
#if defined( UNICODE )
         m_LoadOrderGroup.assign( source.lpLoadOrderGroup );
#else // UNICODE
         wfc_convert_lpcwstr_to_cstring( source.lpLoadOrderGroup, m_LoadOrderGroup );
#endif // UNICODE
      }
      else
      {
         m_LoadOrderGroup.clear();
      }

      if ( source.lpServiceStartName != nullptr )
      {
#if defined( UNICODE )
         m_StartName.assign( source.lpServiceStartName );
#else // UNICODE
         wfc_convert_lpcwstr_to_cstring( source.lpServiceStartName, m_StartName );
#endif // UNICODE
      }
      else
      {
         m_StartName.clear();
      }

      if ( source.lpDisplayName != nullptr )
      {
#if defined( UNICODE )
         m_DisplayName.assign( source.lpDisplayName );
#else // UNICODE
         wfc_convert_lpcwstr_to_cstring( source.lpDisplayName, m_DisplayName );
#endif // UNICODE
      }
      else
      {
         m_DisplayName.clear();
      }

      // Now for the fun part, double-NULL terminated string....

      if ( source.lpDependencies != nullptr )
      {
         wchar_t *temp_pointer = (wchar_t *) source.lpDependencies;

         while ( temp_pointer[ 0 ] != 0x00 )
         {
            (void) m_Dependencies.push_back( temp_pointer );
            temp_pointer += wcslen( (LPCWSTR) temp_pointer ) + 1;
         }
      }
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CServiceConfigurationW::Copy( _In_ QUERY_SERVICE_CONFIGW const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   // The user passed us a pointer, don't trust it

   WFC_TRY
   {
      if ( source != nullptr )
      {
         Copy( *source );
      }
      else
      {
         Empty();
      }
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CServiceConfigurationW::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // We put a try block here because it is called from
   // catch blocks. In other words, we absolutely cannot
   // fail.

   WFC_TRY
   {
      m_TypeOfService = 0;
      m_WhenToStart   = 0;
      m_ErrorControl  = 0;
      m_Tag           = 0;
      m_NameOfExecutableFile.clear();
      m_LoadOrderGroup.clear();
      m_StartName.clear();
      m_DisplayName.clear();
      m_Dependencies.clear();
   }
   WFC_CATCH_ALL
   {
      return;
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CServiceConfigurationW::GetDependencies( _Out_ std::vector<std::wstring>& dependencies ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   dependencies.assign(m_Dependencies.cbegin(), m_Dependencies.cend());
}

void Win32FoundationClasses::CServiceConfigurationW::GetDisplayName( _Out_ std::wstring& display_name ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   display_name.assign( m_DisplayName );
}

void Win32FoundationClasses::CServiceConfigurationW::GetLoadOrderGroup( _Out_ std::wstring& load_order_group ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   load_order_group.assign( m_LoadOrderGroup );
}

void Win32FoundationClasses::CServiceConfigurationW::GetNameOfExecutableFile( _Out_ std::wstring& name_of_executable ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   name_of_executable.assign( m_NameOfExecutableFile );
}

void Win32FoundationClasses::CServiceConfigurationW::GetStartName( _Out_ std::wstring& start_name ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   start_name.assign( m_StartName );
}

// Thanks go to Ullrich Pollaehne [Ullrich_Pollaehne@ka2.maus.de] for
// pointing out I had no implementation for these guys.

_Check_return_ Win32FoundationClasses::CServiceConfigurationW& Win32FoundationClasses::CServiceConfigurationW::operator=( _In_ Win32FoundationClasses::CServiceConfigurationW const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

_Check_return_ Win32FoundationClasses::CServiceConfigurationW& Win32FoundationClasses::CServiceConfigurationW::operator=( _In_ _QUERY_SERVICE_CONFIGW const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CServiceConfigurationW::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );

   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   m_TypeOfService is        "   ) << m_TypeOfService        << TEXT( "\n" );
   dump_context << TEXT( "   m_WhenToStart is          "   ) << m_WhenToStart          << TEXT( "\n" );
   dump_context << TEXT( "   m_ErrorControl is         "   ) << m_ErrorControl         << TEXT( "\n" );
   dump_context << TEXT( "   m_Tag is                  "   ) << m_Tag                  << TEXT( "\n" );
   dump_context << TEXT( "   m_NameOfExecutableFile is \"" ) << m_NameOfExecutableFile << TEXT( "\"\n" );
   dump_context << TEXT( "   m_LoadOrderGroup is       \"" ) << m_LoadOrderGroup       << TEXT( "\"\n" );
   dump_context << TEXT( "   m_StartName is            \"" ) << m_StartName            << TEXT( "\"\n" );
   dump_context << TEXT( "   m_DisplayName is          \"" ) << m_DisplayName          << TEXT( "\"\n" );

   for ( auto const index : Range(m_Dependencies.GetSize()) )
   {
      dump_context << TEXT( "   m_Dependencies[ " ) << index << TEXT( " ] is \"" ) << m_Dependencies.GetAt( index ) << TEXT( "\"\n" );
   }

   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

// End of source
