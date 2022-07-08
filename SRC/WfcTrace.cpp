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
** $Workfile: WfcTrace.cpp $
** $Revision: 32 $
** $Modtime: 10/10/01 3:16a $
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

int  Win32FoundationClasses::CWfcTrace::Indent{ 0 };
bool Win32FoundationClasses::CWfcTrace::Tracing{ true };
LONG Win32FoundationClasses::CWfcTrace::Levels{ 1 };

Win32FoundationClasses::CWfcTrace::CWfcTrace( __in_z LPCTSTR function_name, _In_ std::size_t const tracing_level ) noexcept
{
   m_TracingLevel = tracing_level;

   if ( is_flagged( Levels, m_TracingLevel ) == true )
   {
      if ( Tracing == true )
      {
         m_FunctionName = function_name;

         m_Indent();

         std::wstring string_to_output(WSTRING_VIEW(L"Entering "));

         string_to_output.append( m_FunctionName );
         string_to_output.append(WSTRING_VIEW(L"\n"));

         OutputDebugStringW( string_to_output.c_str() );
      }

      Indent += 2;
   }
}

Win32FoundationClasses::CWfcTrace::~CWfcTrace()
{
   if ( is_flagged( Levels, m_TracingLevel ) == true )
   {
      Indent -= 2;

      if ( Tracing == true )
      {
         m_Indent();

         std::wstring string_to_output(WSTRING_VIEW(L"Leaving  "));

         string_to_output.append( m_FunctionName );
         string_to_output.append(WSTRING_VIEW(L"\n"));

         OutputDebugStringW( string_to_output.c_str() );
         m_FunctionName = nullptr;
      }
   }
}

void Win32FoundationClasses::CWfcTrace::m_Indent( void ) const noexcept
{
   if ( is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      if ( Indent > 0 )
      {
         std::wstring spaces;

         for ( auto const index : Range(Indent) )
         {
            spaces.push_back( ' ' );
         }

         OutputDebugStringW( spaces.c_str() );
      }
   }
}

void Win32FoundationClasses::CWfcTrace::Output( _In_z_ LPCTSTR message ) const noexcept
{
   if ( is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      m_Indent();

      std::wstring output_string( m_FunctionName );

      output_string.append(WSTRING_VIEW(L" : "));
      output_string.append( message );

      if ( output_string.length() > 510 )
      {
         output_string.erase(510);
      }

      output_string.append(WSTRING_VIEW(L"\n"));

      OutputDebugStringW( output_string.c_str() );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ int const integer ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
       TCHAR temp_string[11]{ 0 };

      ::ZeroMemory( temp_string, sizeof( temp_string ) );
      _itot_s( integer, temp_string, std::size( temp_string ), 10 );

      std::wstring m( message );

      m.append( temp_string );

      Output( m.c_str() );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ UINT const integer ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      Output( message, (ULONG) integer );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ long const a_long ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      TCHAR temp_string[ 11 ];

      ::ZeroMemory( temp_string, sizeof( temp_string ) );
      _ltot_s( a_long, temp_string, std::size( temp_string ), 10 );

      std::wstring m( message );

      m.append( temp_string );

      Output( m.c_str() );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ ULONG const a_long ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      TCHAR temp_string[ 11 ];

      ::ZeroMemory( temp_string, sizeof( temp_string ) );
      _ultot_s( a_long, temp_string, std::size( temp_string ), 10 );

      std::wstring m( message );

      m.append( temp_string );

      Output( m.c_str() );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ LONGLONG const a_long ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      TCHAR temp_string[ 31 ];

      ::ZeroMemory( temp_string, sizeof( temp_string ) );
      _i64tot_s( a_long, temp_string, std::size( temp_string ), 10 );

      std::wstring m( message );

      m.append( temp_string );

      Output( m.c_str() );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ ULONGLONG const a_long ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      TCHAR temp_string[ 31 ];

      ::ZeroMemory( temp_string, sizeof( temp_string ) );
      _ui64tot_s( a_long, temp_string, std::size( temp_string ), 10 );

      std::wstring m( message );

      m.append( temp_string );

      Output( m.c_str() );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ double const a_double ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      int decimal{ 0 };
      int sign{ 0 };

      char ascii_string[ 128 ];

	  ZeroMemory( ascii_string, sizeof( ascii_string ) );

      LPCTSTR string_p{ nullptr };

      _fcvt_s( ascii_string, std::size( ascii_string ), a_double, 7, &decimal, &sign );

   #if defined( UNICODE )

      // We need to convert

      WCHAR unicode_string[ 1024 ];

      unicode_string[ 0 ] = 0x00;

      ASCII_to_UNICODE( ascii_string, unicode_string );

      string_p = unicode_string;

   #else

      // No conversion needed

      string_p = &ascii_string[ 0 ];

   #endif

      std::wstring string;

      if ( sign not_eq 0 )
      {
         string.push_back( '-' );
      }

      if ( decimal < 1 )
      {
         string.push_back( '.' );

         int index{ 0 };

         while( index > decimal ) // Cannot be converted to a Range loop
         {
            string.push_back( '0' );
            index--;
         }

         string.append( string_p );
      }
      else if ( decimal >= (int) _tcslen( string_p ) )
      {
         string.append( string_p );
         string.push_back( '.' );
      }
      else
      {
         int index{ 0 };

         while( index < decimal )
         {
            string.push_back( string_p[ index ] );
            index++;
         }

         string.push_back( '.' );

         index = decimal;

         while( index < (int) _tcslen( string_p ) )
         {
            string.push_back( string_p[ index ] );
            index++;
         }
      }

      Output( message, string );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ std::wstring const& string ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      std::wstring message2(message);
      message2.append(string);
      Output(message2.c_str());
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ VOID const * pointer ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
       std::wstring pointer_string;

      format( pointer_string, L"%p", pointer );

      Output( message, pointer_string );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ LARGE_INTEGER const& value ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
       std::wstring pointer_string;

       Win32FoundationClasses::format( pointer_string, L"{ QuadPart=%" PRId64 " { LowPart=0x%" PRIX32 " HighPart=0x%" PRIX32 " } }",
                             value.QuadPart, value.LowPart, value.HighPart );

      Output( message, pointer_string );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ ULARGE_INTEGER const& value ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
       std::wstring pointer_string;

       Win32FoundationClasses::format( pointer_string, L"{ QuadPart=%" PRIu64 " { LowPart=0x%" PRIX32 " HighPart=0x%" PRIX32 " } }",
                             value.QuadPart, value.LowPart, value.HighPart );

      Output( message, pointer_string );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ RECT const& value ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
       std::wstring pointer_string;

      format( pointer_string, L"{ left = %ld top = %ld right = %ld bottom = %ld }",
                             value.left, value.top, value.right, value.bottom );

      Output( message, pointer_string );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ POINT const& value ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
       std::wstring pointer_string(WSTRING_VIEW(L"{ x = "));
       pointer_string.append(std::to_wstring(value.x));
       pointer_string.append(WSTRING_VIEW(L" y = "));
       pointer_string.append(std::to_wstring(value.y));
       pointer_string.push_back(' ');
       pointer_string.push_back('}');

       Output( message, pointer_string );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ SIZE const& value ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      std::wstring pointer_string(WSTRING_VIEW(L"{ cx = "));
      pointer_string.append(std::to_wstring(value.cx));
      pointer_string.append(WSTRING_VIEW(L" cy = "));
      pointer_string.append(std::to_wstring(value.cy));
      pointer_string.push_back(' ');
      pointer_string.push_back('}');

      Output( message, pointer_string );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ GUID const& value ) const noexcept // New for Release 52
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      std::wstring class_name;

      format( class_name, L"GUID{%08lX-%04lX-%04lX-%02X%02X-%02X%02X%02X%02X%02X%02X}",
                    (unsigned long) value.Data1,
                    (unsigned int)  value.Data2,
                    (unsigned int)  value.Data3,
                    (unsigned int)  value.Data4[ 0 ],
                    (unsigned int)  value.Data4[ 1 ],
                    (unsigned int)  value.Data4[ 2 ],
                    (unsigned int)  value.Data4[ 3 ],
                    (unsigned int)  value.Data4[ 4 ],
                    (unsigned int)  value.Data4[ 5 ],
                    (unsigned int)  value.Data4[ 6 ],
                    (unsigned int)  value.Data4[ 7 ] );

      Output( message, class_name );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ FILETIME const& value ) const noexcept // New for Release 52
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      SYSTEMTIME system_time;

      ZeroMemory( &system_time, sizeof( system_time ) );

      std::wstring class_name;

      if ( FileTimeToSystemTime( &value, &system_time ) == FALSE )
      {
          Win32FoundationClasses::format( class_name, L"{dwLowDateTime=%08lX dwHighDateTime=%08lX}",
                           (unsigned long) value.dwLowDateTime,
                           (unsigned long) value.dwHighDateTime );
      }
      else
      {
          Win32FoundationClasses::format( class_name, L"{dwLowDateTime=%08lX dwHighDateTime=%08lX (%04d-%02d-%02d %02d:%02d:%02d.%04d)}",
                           (unsigned long) value.dwLowDateTime,
                           (unsigned long) value.dwHighDateTime,
                           (int) system_time.wYear,
                           (int) system_time.wMonth,
                           (int) system_time.wDay,
                           (int) system_time.wHour,
                           (int) system_time.wMinute,
                           (int) system_time.wSecond,
                           (int) system_time.wMilliseconds
                          );
      }

      Output( message, class_name );
   }
}

void Win32FoundationClasses::CWfcTrace::Output( __in_z LPCTSTR message, _In_ SYSTEMTIME const& value ) const noexcept // New for Release 52
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      std::wstring class_name;

      Win32FoundationClasses::format( class_name, L"{%04d-%02d-%02d %02d:%02d:%02d.%04d}",
                        (int) value.wYear,
                        (int) value.wMonth,
                        (int) value.wDay,
                        (int) value.wHour,
                        (int) value.wMinute,
                        (int) value.wSecond,
                        (int) value.wMilliseconds
                       );

      Output( message, class_name );
   }
}

void Win32FoundationClasses::CWfcTrace::OutputBinary( __in_z LPCTSTR message, _In_ ULONG const a_long ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      int index{ 31 };

      std::wstring bit_string;

      bit_string.reserve(36);

      while( index >= 0 )
      {
         if ( _bittest( reinterpret_cast<LONG const *>(&a_long), index ) == 1 )
         {
            bit_string.push_back( '1' );
         }
         else
         {
            bit_string.push_back( '0' );
         }

         if ( index == 24 or
              index == 16 or
              index ==  8 )
         {
            bit_string.push_back( ' ' );
         }

         index--;
      }

      Output( message, bit_string );
   }
}

void Win32FoundationClasses::CWfcTrace::OutputVariant( __in_z LPCTSTR message, _In_ VARIANT const& value ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      OutputVariant( message, &value );
   }
}

void Win32FoundationClasses::CWfcTrace::OutputVariant( __in_z LPCTSTR message, _In_ VARIANT const * value ) const noexcept
{
   if (is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      std::wstring data_type;

      DWORD vt{ V_VT(value) };

      // Types taken from WTYPES.H -- VARENUM data type

      if (is_flagged(vt, VT_VECTOR) == true)
      {
         data_type.assign(WSTRING_VIEW(L"VT_VECTOR"));
      }

      if (is_flagged(vt, VT_ARRAY) == true)
      {
         if ( data_type.empty() == false )
         {
            data_type.append(WSTRING_VIEW(L" | " ) );
         }

         data_type.append(WSTRING_VIEW(L"VT_ARRAY" ) );
      }

      if (is_flagged(vt, VT_BYREF) == true)
      {
          if (data_type.empty() == false)
          {
              data_type.append(WSTRING_VIEW(L" | "));
          }

         data_type.append(WSTRING_VIEW(L"VT_BYREF" ) );
      }

      if (is_flagged(vt, VT_RESERVED) == true)
      {
          if (data_type.empty() == false)
          {
              data_type.append(WSTRING_VIEW(L" | "));
          }

         data_type.append(WSTRING_VIEW(L"VT_RESERVED" ) );
      }

      DWORD basic_type{ vt bitand VT_TYPEMASK };

      switch( basic_type )
      {
         case VT_EMPTY:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_EMPTY" ) );
            break;

         case VT_NULL:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_NULL" ) );
            break;

         case VT_I2:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_I2" ) );
            break;

         case VT_I4:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_I4" ) );
            break;

         case VT_R4:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_R4" ) );
            break;

         case VT_R8:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_R8" ) );
            break;

         case VT_CY:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_CY" ) );
            break;

         case VT_DATE:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_DATE" ) );
            break;

         case VT_BSTR:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_BSTR" ) );
            break;

         case VT_DISPATCH:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_DISPATCH" ) );
            break;

         case VT_ERROR:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_ERROR" ) );
            break;

         case VT_BOOL:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_BOOL" ) );
            break;

         case VT_VARIANT:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_VARIANT" ) );
            break;

         case VT_UNKNOWN:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_UNKNOWN" ) );
            break;

         case VT_DECIMAL:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }
             
             data_type.append(WSTRING_VIEW(L"VT_DECIMAL" ) );
            break;

         case VT_I1:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_I1" ) );
            break;

         case VT_UI1:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_UI1" ) );
            break;

         case VT_UI2:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_UI2" ) );
            break;

         case VT_UI4:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_UI4" ) );
            break;

         case VT_I8:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }
             
             data_type.append(WSTRING_VIEW(L"VT_I8" ) );
            break;

         case VT_UI8:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_UI8" ) );
            break;

         case VT_INT:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_INT" ) );
            break;

         case VT_UINT:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

             data_type.append(WSTRING_VIEW(L"VT_UINT" ) );
            break;

         case VT_VOID:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_VOID" ) );
            break;

         case VT_HRESULT:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_HRESULT" ) );
            break;

         case VT_PTR:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_PTR" ) );
            break;

         case VT_SAFEARRAY:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_SAFEARRAY" ) );
            break;

         case VT_CARRAY:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }
             
             data_type.append(WSTRING_VIEW(L"VT_CARRAY" ) );
            break;

         case VT_USERDEFINED:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_USERDEFINED" ) );
            break;

         case VT_LPSTR:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_LPSTR" ) );
            break;

         case VT_LPWSTR:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_LPWSTR" ) );
            break;

         case VT_RECORD:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_RECORD" ) );
            break;

         case VT_FILETIME:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_FILETIME" ) );
            break;

         case VT_BLOB:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_BLOB" ) );
            break;

         case VT_STREAM:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_STREAM" ) );
            break;

         case VT_STORAGE:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_STORAGE" ) );
            break;

         case VT_STREAMED_OBJECT:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_STREAMED_OBJECT" ) );
            break;

         case VT_STORED_OBJECT:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_STORED_OBJECT" ) );
            break;

         case VT_BLOB_OBJECT:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }
             
             data_type.append(WSTRING_VIEW(L"VT_BLOB_OBJECT" ) );
            break;

         case VT_CF:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_CF" ) );
            break;

         case VT_CLSID:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_CLSID" ) );
            break;

         case VT_BSTR_BLOB:

             if (data_type.empty() == false)
             {
                 data_type.append(WSTRING_VIEW(L" | "));
             }

            data_type.append(WSTRING_VIEW(L"VT_BSTR_BLOB" ) );
            break;

         default:

            {
               std::wstring data_type_string(WSTRING_VIEW(L"Unknown("));

               data_type_string.append(std::to_wstring(basic_type));
               data_type_string.push_back(')');

               if (data_type.empty() == false)
               {
                   data_type.append(WSTRING_VIEW(L" | "));
               }

               data_type.append( data_type_string );
            }

            break;
      }

      std::wstring variant_message( message );
   
      variant_message.append(WSTRING_VIEW(L"Variant type is "));
      variant_message.append( data_type );

      Output( variant_message.c_str() );
   }
}

void Win32FoundationClasses::CWfcTrace::ReportError( _In_ DWORD const error_code ) noexcept
{
   if ( is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      std::wstring error_string;

      Win32FoundationClasses::wfc_get_error_string( error_code, error_string );

      std::wstring error_message(WSTRING_VIEW(L"ERROR "));

      error_message.append(std::to_wstring(error_code));
      error_message.append(WSTRING_VIEW(L" - "));
      error_message.append( error_string );

      Output( error_message.c_str() );
   }
}

void Win32FoundationClasses::CWfcTrace::ReportSocketError( _In_ DWORD const error_code ) noexcept
{
   if ( is_flagged( Levels, m_TracingLevel ) == true and Tracing == true )
   {
      TCHAR error_string[ MAX_PATH * 2 ];

      ::ZeroMemory( error_string, sizeof( error_string ) );

      CSimpleSocket::TranslateErrorCode( error_code, error_string, static_cast<DWORD>(std::size( error_string )) );

      std::wstring error_message(WSTRING_VIEW(L"SOCKET ERROR "));
      error_message.append(std::to_wstring(error_code));
      error_message.append(WSTRING_VIEW(L" - "));
      error_message.append( error_string );

      Output( error_message.c_str() );
   }
}

void Win32FoundationClasses::CWfcTrace::TraceOff( void ) noexcept
{
   Tracing = false;
}

void Win32FoundationClasses::CWfcTrace::TraceOn( void ) noexcept
{
   Tracing = true;
}

void Win32FoundationClasses::CWfcTrace::TraceAllOff( void ) noexcept
{
   Tracing = false;
   Indent  = 0;
   Levels  = 0;
}

void Win32FoundationClasses::CWfcTrace::TraceAllOn( void ) noexcept
{
   Tracing = true;
   Levels  = 0xFFFFFFFF;
}

void Win32FoundationClasses::CWfcTrace::TraceLevelOn( _In_ std::size_t const level ) noexcept
{
   if ( level > 31 )
   {
      _bittestandset( reinterpret_cast<LONG *>(&Levels), 31 );
      return;
   }

   static_assert(sizeof(LONG) == sizeof(Levels), "The size of LONG is not the same as the size of Levels");

   _bittestandset( reinterpret_cast<LONG *>(&Levels), level );
}

void Win32FoundationClasses::CWfcTrace::TraceLevelOff( _In_ std::size_t const level ) noexcept
{
   if ( level > 31 )
   {
      _bittestandreset(reinterpret_cast<LONG*>(&Levels), 31 );
   }

   _bittestandreset(reinterpret_cast<LONG*>(&Levels), level );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CWfcTrace</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that makes tracing much easier.">
</HEAD>

<BODY>

<H1>CWfcTrace</H1>
$Revision: 32 $
<HR>

<H2>Description</H2>

This class helps to debug your programs.
It prints out messages when entering and leaving functions. It automatically
indents so you can see which functions called who.
Normally, you will never use this class directly, you will use the
<A HREF="#Macros">macros</A>.
All messages generated by this class are sent to <CODE>OutputDebugString()</CODE>.

<P>

The way I write and debug programs is to have lots and lots of debugging
output. I prefer to have too many debug messages than too few. That way,
I am sure to not miss anything. By having lots of debug output, you can
see how a program really works as opposed to how you <I>think</I> a
program works. I like having twenty pages of debugging output to sift
through so I can see my bugs and the context they present themselves.

<H2>Data Members</H2>

<DL COMPACT>

<DT><PRE>static int <B>Indent</B></PRE><DD>
This value is the number of spaces that will be printed before
each line of output.

<DT><PRE>static DWORD <B>Levels</B></PRE><DD>
This is a bit mask containing which of the 32 levels of debugging are
turned on.

<DT><PRE>static BOOL <B>Tracing</B></PRE><DD>
This boolean determines if any output will be generated.
When TRUE, output (for levels that are on) will be generated.
If FALSE, nothing is outputted.

</DL>

<H2>Constructors</H2>
<DL COMPACT>
<DT><PRE><B>CWfcTrace</B>( LPCTSTR function_name, DWORD tracing_level )</PRE><DD>
You must provide the name and which level this object belongs.

</DL>

<H2>Static Methods</H2>

<DL COMPACT>
<DT><PRE>static void <B><A NAME="TraceAllOn">TraceAllOn</A></B>( void )</PRE><DD>
Turns on all 32 levels of tracing.

<DT><PRE>static void <B><A NAME="TraceAllOff">TraceAllOff</A></B>( void )</PRE><DD>
Turns off all 32 levels of tracing.

<DT><PRE>static void <B><A NAME="TraceLevelOn">TraceLevelOn</A></B>( DWORD level )</PRE><DD>
Turns one of the 32 levels of tracing on.

<DT><PRE>static void <B><A NAME="TraceLevelOff">TraceLevelOff</A></B>( DWORD level )</PRE><DD>
Turns one of the 32 levels of tracing off.

<DT><PRE>static void <B><A NAME="TraceOn">TraceOn</A></B>( void )</PRE><DD>
Allows messages to be printed.

<DT><PRE>static void <B><A NAME="TraceOff">TraceOff</A></B>( void )</PRE><DD>
Disallows messages to be printed.

</DL>

<H2>Methods</H2>

<DL COMPACT>
<DT><PRE>void <A NAME="Output">Output</A>( LPCTSTR message ) const
void Output( LPCTSTR message, const int integer ) const
void Output( LPCTSTR message, const UINT integer ) const
void Output( LPCTSTR message, const long a_long ) const
void Output( LPCTSTR message, const ULONG a_long ) const
void Output( LPCTSTR message, const LONGLONG a_long ) const
void Output( LPCTSTR message, const ULONGLONG a_long ) const
void Output( LPCTSTR message, const double a_double ) const
void Output( LPCTSTR message, LPCTSTR string ) const
void Output( LPCTSTR message, CObject const * object ) const
void Output( LPCTSTR message, const VOID * pointer ) const
void Output( LPCTSTR message, const CObject&amp; object ) const
void Output( LPCTSTR message, const LARGE_INTEGER&amp; value ) const
void Output( LPCTSTR message, const ULARGE_INTEGER&amp; value ) const
void Output( LPCTSTR message, const RECT&amp; value ) const
void Output( LPCTSTR message, const POINT&amp; value ) const
void Output( LPCTSTR message, const SIZE&amp; value ) const
void Output( LPCTSTR message, const GUID&amp; value ) const
void Output( LPCTSTR message, const FILETIME&amp; value ) const
void Output( LPCTSTR message, const SYSTEMTIME&amp; value ) const
</PRE><DD>

Would you just look at all the stuff you can output!
I toyed with the idea of putting more data structures (like <CODE>WNDCLASS</CODE>)
but once you start down that road, you get very little return on investment.
So, I stuck with the basic data types.

<DT><PRE>void <A NAME="OutputBinary">OutputBinary</A>( LPCTSTR message, const ULONG a_long ) const</PRE><DD>
This allows you to dump the contents of a <CODE>ULONG</CODE> as
bits. This is great for picking apart bit masks.

<DT><PRE>void <A NAME="OutputVariant">OutputVariant</A>( LPCTSTR message, const VARIANT&amp; value ) const
void OutputVariant( LPCTSTR message, const VARIANT * value ) const</PRE><DD>
This allows you to dump the contents of a <CODE>VARIANT</CODE>.

<DT><PRE>void <A NAME="ReportError">ReportError</A>( DWORD error_code )</PRE><DD>
Translates error codes to a humanly readable string.

<DT><PRE>void <A NAME="ReportSocketError">ReportSocketError</A>( DWORD error_code )</PRE><DD>
Translates error codes from TCP/IP socket API's to a humanly readable string.

</DL>

<H2><A NAME="Macros">Macros</A></H2>

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;

void print_message( void )
{
   <B>WFCTRACEINIT</B>( TEXT( &quot;print_message()&quot; ) );
   <B>WFCTRACEVAL</B>( TEXT( &quot;Test number &quot; ), 1 );
}

void main( void )
{
   <B>WFCTRACEINIT</B>( TEXT( &quot;main()&quot; ) );

   print_message();
}</CODE></PRE>

Will produce this output:

<PRE><CODE>
Entering main()
   Entering print_message()
      print_message() : Test number 1
   Leaving  print_message()
Leaving  main()
</CODE></PRE>

<H2>API's Used</H2>

<B>CWfcTrace</B> uses the following API's:

<UL>
<LI>OutputDebugString
<LI>ZeroMemory
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: WfcTrace.cpp $<BR>
$Modtime: 10/10/01 3:16a $
</BODY>

</HTML>
The following line should go in AUTOEXP.DAT so the debugging tooltips will format properly
ToolTipFormatLine=CWfcTrace=name=<m_FunctionName.m_pchData,s>
*/
