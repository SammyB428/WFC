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
** $Workfile: Mfc2Stl.h $
** $Revision: 14 $
** $Modtime: 9/16/01 11:56a $
*/

#if ! defined( MFC_TO_STL_HEADER_FILE )

#define MFC_TO_STL_HEADER_FILE

// Don't use min/max macros
#define NOMINMAX
// Include all of the windows header files that MFC would have included
#include <windows.h>
#include <tchar.h>
#include <stdint.h>

#include <string>
#include <vector>
#include <algorithm>
#include <codecvt>
#include <memory>

// Now try to include stuff is a desparate attempt to get <charconv> to compile
#include <string_view>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <charconv>
#include <iso646.h>

#if ! defined( WFC_DONT_USE_NAMESPACES )
#if ( _MSC_VER > 1000 )
namespace Win32FoundationClasses
{
#endif // _MSC_VER > 1000
#endif // WFC_DONT_USE_NAMESPACES

    void PASCAL ASCII_to_UNICODE(_In_z_ std::string_view ansi_string, __out_z LPWSTR unicode_string) noexcept;
    void PASCAL UNICODE_to_ASCII(_In_ std::wstring_view unicode_string, __out_z LPSTR ansi_string, _In_ SSIZE_T number_of_unicode_characters_to_convert = (-1), _In_ const UINT code_page = CP_ACP) noexcept;

#if ! defined( WFC_DONT_USE_NAMESPACES )
#if ( _MSC_VER > 1000 )
    // Terminate the namespace declaration
}
#endif // _MSC_VER > 1000
#endif // WFC_DONT_USE_NAMESPACES

// Get rid of a ton of warnings

#pragma warning( disable : 4710 4702 4172 )

// 2000-10-30
// Thanks go to Danny Smith (danny_r_smith_2001@yahoo.co.nz) for
// making this compilable in GCC. GCC doesn't have C runtime support
// for memory leak detection.
#if defined( _MSC_VER )
#include <crtdbg.h>

void * __cdecl operator new( size_t number_of_bytes, LPCSTR filename, int line_number );
#define DEBUG_NEW new( THIS_FILE, __LINE__ )
#if _MSC_VER >= 1200
void __cdecl operator delete( void* pointer, LPCSTR filename, int line_number );
#endif
#endif // _DEBUG

// 2000-10-30
// Thanks go to Danny Smith (danny_r_smith_2001@yahoo.co.nz) for
// making this compilable in GCC.
#if defined (__MINGW32__)
#  include <assert.h>
#  define _ASSERT( x ) assert(x)
#endif // __MINGW32__

#if defined( _DEBUG )
#define ASSERT( x ) _ASSERT( x )
#else
#define ASSERT( x ) ((void)0)
#endif

#include "wfc_validate.h"

// Now start picking up the STL stuff

#define STRING_NOT_FOUND (-1)
#define CHARACTER_NOT_FOUND (-1)

#if ! defined( WFC_DONT_USE_NAMESPACES )
#if ( _MSC_VER > 1000 )
namespace Win32FoundationClasses
{
#endif // _MSC_VER > 1000
#endif // WFC_DONT_USE_NAMESPACES

    class CString
    {
    private:

#if defined( UNICODE )

        // UNICODE build
        std::wstring m_String;

#else // UNICODE

        // ASCII build
        std::string m_String;

#endif // UNICODE

        LPTSTR m_Buffer{ nullptr };

    protected:

        void m_Format( _In_z_ __format_string LPCTSTR format_string, va_list argument_list ) noexcept;

    public:

        // Constructors

        // WARNING! The order of the constructors matters!!!!
        inline CString() noexcept
        {
            WFC_VALIDATE_POINTER( this );
            m_Buffer = nullptr;
        }

        inline CString( _In_ const CString& source ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            m_Buffer = nullptr;
            Copy( source );
        }

        inline CString( _In_ TCHAR character, _In_ size_t number_of_characters = 1 ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            m_Buffer = nullptr;
            m_String.append( number_of_characters, character );
        }

        // ANSI Strings
        inline CString( _In_z_ LPCSTR source ) noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( source );

            m_Buffer = nullptr;
            Copy( source );
        }

        inline CString( _In_z_ LPCSTR ansi_string, _In_ size_t number_of_characters ) noexcept
        {
            m_Buffer = nullptr;
            Copy( ansi_string, number_of_characters );
        }

        // UNICODE Strings
        inline CString( _In_z_ LPCWSTR source ) noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( source );

            m_Buffer = nullptr;
            Copy( source );
        }

        inline CString( _In_z_ LPCWSTR unicode_string, _In_ size_t number_of_characters ) noexcept
        {
            m_Buffer = nullptr;
            Copy( unicode_string, number_of_characters );
        }

        // Bytes that happend to be zero terminated and considered to be an ANSI string
        inline CString( _In_z_ const unsigned char* ansi_string ) noexcept
        {
            m_Buffer = nullptr;
            Copy( (LPCSTR) ansi_string );
        }

        inline ~CString()
        {
            WFC_VALIDATE_POINTER( this );

            WFC_TRY
            {
                if ( m_Buffer != nullptr )
                {
                    delete m_Buffer;
                    m_Buffer = nullptr;
                }
            }
            WFC_CATCH_ALL
            {
            }
            WFC_END_CATCH_ALL
        }

        // Methods

        inline _Check_return_ BSTR AllocSysString( _In_ DWORD code_page = CP_ACP ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

#if defined( _UNICODE ) || defined( OLE2ANSI )

            BSTR return_value = ::SysAllocStringLen( m_String.c_str(), (UINT) m_String.length() );

#else

            int length = ::MultiByteToWideChar( code_page,
                0,
                m_String.c_str(),
                (int) m_String.length(),
                nullptr,
                0 );

            BSTR return_value = ::SysAllocStringLen( nullptr, length );

            if ( return_value != nullptr )
            {
                ::MultiByteToWideChar( code_page,
                    0,
                    m_String.c_str(),
                    (int) m_String.length(),
                    return_value,
                    length );
            }

#endif

            return( return_value );
        }

        inline void Append( _In_z_ LPCTSTR string_p ) noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( string_p );

            WFC_TRY
            {
                m_String.append( string_p );
            }
            WFC_CATCH_ALL
            {
            }
            WFC_END_CATCH_ALL
        }

        inline void Append( _In_ const TCHAR text_character, _In_ const size_t number_of_times = 1 ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            WFC_TRY
            {
                size_t loop_index = 0;

                TCHAR temp_string[ 2 ];

                temp_string[ 0 ] = text_character;
                temp_string[ 1 ] = 0;

                while( loop_index < number_of_times )
                {
                    m_String.append( temp_string );
                    loop_index++;
                }
            }
            WFC_CATCH_ALL
            {
            }
            WFC_END_CATCH_ALL
        }

        inline void Append( _In_ const CString& source ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            WFC_TRY
            {
                m_String.append( source.m_String );
            }
            WFC_CATCH_ALL
            {
            }
            WFC_END_CATCH_ALL
        }

        inline int Collate( _In_z_ LPCTSTR string_pointer ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( string_pointer );

            WFC_TRY
            {
                return( ( string_pointer == nullptr ) ? 1 : _tcscoll( m_String.c_str(), string_pointer ) );
            }
            WFC_CATCH_ALL
            {
                return( 0 );
            }
            WFC_END_CATCH_ALL
        }

        inline _Check_return_ int Compare( _In_z_ LPCTSTR string_p ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( string_p );

            WFC_TRY
            {
                if ( string_p != nullptr )
                {
                    return( m_String.compare( string_p ) );
                }
                else
                {
                    // We are greater than nothing (nullptr)
                    return( 1 );
                }
            }
            WFC_CATCH_ALL
            {
                return( 0 );
            }
            WFC_END_CATCH_ALL
        }

        inline _Check_return_ int CompareNoCase( _In_z_ LPCTSTR string_p ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( string_p );

            WFC_TRY
            {
                if ( string_p != nullptr )
                {
                    return( _tcsicmp( m_String.c_str(), string_p ) );
                }
                else
                {
                    // We are greater than nothing (nullptr)
                    return( 1 );
                }
            }
            WFC_CATCH_ALL
            {
                return( 0 );
            }
            WFC_END_CATCH_ALL
        }

        inline void Copy( _In_z_ LPCSTR ascii_string_p, _In_ SSIZE_T number_of_characters = (-1), _In_ size_t beginning_at = 0 ) noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( ascii_string_p );

#if defined( UNICODE )
            // We are a UNICODE build trying to copy an ASCII string, this is easy...
            WFC_TRY
            {
                m_String.resize( 0 );

                if ( ascii_string_p == nullptr )
                {
                    return;
                }

                if ( beginning_at < 0 )
                {
                    beginning_at = 0;
                }

                if ( number_of_characters < 0 ) // used to be -1 but that opened us up to errors
                {
                    size_t loop_index   = beginning_at;
                    size_t string_index = 0;

                    size_t string_size = strlen( ascii_string_p );

                    if ( loop_index >= strlen( ascii_string_p ) )
                    {
                        return;
                    }

                    _ASSERTE( loop_index <= string_size );

                    string_size -= loop_index;

                    m_String.resize( string_size );

                    while( ascii_string_p[ loop_index ] != 0 )
                    {
                        m_String.at( string_index ) = ascii_string_p[ loop_index ];
                        loop_index++;
                        string_index++;
                    }
                }
                else
                {
                    // Only append a finite number of characters...

                    std::size_t loop_index   = 0;
                    std::size_t string_index = 0;

                    if ( beginning_at > 0 )
                    {
                        if ( beginning_at >= strlen( ascii_string_p ) )
                        {
                            Empty();
                            return;
                        }
                    }

                    SSIZE_T true_number_of_characters_to_copy = strlen( ascii_string_p ) - beginning_at;

                    if ( true_number_of_characters_to_copy > number_of_characters )
                    {
                        true_number_of_characters_to_copy = number_of_characters;
                    }

                    m_String.resize( true_number_of_characters_to_copy );

                    while( (SSIZE_T) loop_index < true_number_of_characters_to_copy )
                    {
                        m_String.at( string_index ) = ascii_string_p[ beginning_at + loop_index ];
                        loop_index++;
                        string_index++;
                    }

                    m_String.resize( string_index );
                }
            }
            WFC_CATCH_ALL
            {
            }
            WFC_END_CATCH_ALL
#else // UNICODE

            // We are an ASCII build copying an ascii string, we don't need to do anything special
            WFC_TRY
            {
                m_String.resize( 0 );

                if ( ascii_string_p == nullptr )
                {
                    return;
                }

                if ( number_of_characters == (-1) )
                {
                    m_String.append( &ascii_string_p[ beginning_at ] );
                }
                else
                {
                    // Only append a finite number of characters...

                    SSIZE_T loop_index = 0;

                    while( loop_index < number_of_characters )
                    {
                        Append( ascii_string_p[ beginning_at + loop_index ] );
                        loop_index++;
                    }
                }
            }
            WFC_CATCH_ALL
            {
            }
            WFC_END_CATCH_ALL
#endif // UNICODE
        }

        inline void Copy( _In_z_ LPCWSTR string_p, _In_ SSIZE_T number_of_characters = (-1), _In_ size_t beginning_at = 0 ) noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( string_p );

#if defined( UNICODE )

            // We're already UNICODE so we don't need to do anything special
            WFC_TRY
            {
                m_String.resize( 0 );

                if ( string_p == nullptr )
                {
                    return;
                }

                if ( number_of_characters == (-1) )
                {
                    m_String.append( &string_p[ beginning_at ] );
                }
                else
                {
                    // Only append a finite number of characters...

                    std::size_t loop_index = 0;

                    while( (SSIZE_T) loop_index < number_of_characters )
                    {
                        Append( string_p[ beginning_at + loop_index ] );
                        loop_index++;
                    }
                }
            }
            WFC_CATCH_ALL
            {
            }
            WFC_END_CATCH_ALL
#else // UNICODE

            // We are ASCII build, we need to convert ASCII to UNICODE
            WFC_TRY
            {
                m_String.resize( 0 );

                if ( string_p == nullptr )
                {
                    return;
                }

                std::size_t number_of_characters_to_copy = 0;

                if ( number_of_characters == (-1) )
                {
                    number_of_characters_to_copy = wcslen( string_p );
                }
                else
                {
                    number_of_characters_to_copy = number_of_characters;
                }

                number_of_characters_to_copy -= beginning_at;

                char  * ascii_string = nullptr;

                if ( number_of_characters_to_copy <= 0 )
                {
                    return;
                }

                WFC_TRY
                {
                    ascii_string = new char[ ( number_of_characters_to_copy + 1 ) * sizeof( WCHAR ) ];
                }
                WFC_CATCH_ALL
                {
                    ascii_string = nullptr;
                }
                WFC_END_CATCH_ALL

                    if ( ascii_string == nullptr )
                    {
                        Empty();
                        return;
                    }

                    UNICODE_to_ASCII( &string_p[ beginning_at ], ascii_string, number_of_characters_to_copy );
                    ascii_string[ number_of_characters_to_copy ] = 0x00;

                    m_String.append( ascii_string );

                    delete [] ascii_string;
            }
            WFC_CATCH_ALL
            {
            }
            WFC_END_CATCH_ALL
#endif // UNICODE
        }

        inline _Check_return_ int Delete( _In_ const size_t index, _In_ size_t number_of_characters_to_delete = 1 )
        {
            WFC_VALIDATE_POINTER( this );

            if ( index < 0 or number_of_characters_to_delete < 1 )
            {
                return( (int) m_String.length() );
            }

            int const number_of_characters_remaining = (int)( (int) m_String.length() - index );

            if ( number_of_characters_to_delete > number_of_characters_remaining )
            {
                number_of_characters_to_delete = number_of_characters_remaining;
            }

            m_String = m_String.erase( index, number_of_characters_to_delete );

            return( (int) m_String.length() );
        }

        inline void Empty( void ) noexcept
        {
            WFC_VALIDATE_POINTER( this );
            m_String.resize( 0 );
        }

        inline _Check_return_ SSIZE_T Find( _In_ TCHAR character ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );
            return( (int) m_String.find( character ) );
        }

        inline _Check_return_ SSIZE_T Find( _In_ TCHAR character, _In_ size_t start_index ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );

            if ( start_index >= m_String.length() )
            {
                return( CHARACTER_NOT_FOUND );
            }

            if ( start_index == 0 )
            {
                return( m_String.find( character ) );
            }

            // STL's find is whacko. Put some sense back into it.
            // This is a hack until I can decipher what STL is attempting to do

            SSIZE_T found_at = Mid( start_index ).Find( character );

            if ( found_at >= 0 )
            {
                return( start_index + found_at );
            }

            return( CHARACTER_NOT_FOUND );
        }

        inline _Check_return_ SSIZE_T Find( _In_z_ LPCTSTR string_p ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( string_p );

            if ( string_p != nullptr )
            {
                return( m_String.find( string_p ) );
            }
            else
            {
                return( STRING_NOT_FOUND );
            }
        }

        inline _Check_return_ SSIZE_T Find( _In_z_ LPCTSTR string_p, _In_ size_t start_index ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( string_p );

            if ( string_p != nullptr )
            {
                return( (int) m_String.find( string_p, start_index ) );
            }
            else
            {
                return( STRING_NOT_FOUND );
            }
        }

        inline SSIZE_T FindOneOf( LPCTSTR string_p ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( string_p );

            if ( string_p != nullptr )
            {
                return( m_String.find_first_of( string_p ) );
            }
            else
            {
                return( CHARACTER_NOT_FOUND );
            }
        }

        inline _Check_return_ TCHAR GetAt( _In_ const size_t character_index ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );

            if ( character_index >= m_String.length() )
            {
                return( 0 );
            }

            return( m_String.at( character_index ) );
        }

        inline _Check_return_ LPTSTR GetBuffer( _In_ const size_t minimum_buffer_length ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            WFC_TRY
            {
                if ( m_Buffer != nullptr )
                {
                    delete m_Buffer;
                    m_Buffer = nullptr;
                }

                WFC_TRY
                {
                    m_Buffer = new TCHAR[ minimum_buffer_length + 1 ];
                }
                WFC_CATCH_ALL
                {
                    m_Buffer = nullptr;
                }
                WFC_END_CATCH_ALL

                    if ( m_Buffer == nullptr )
                    {
                        return( nullptr );
                    }

                    m_Buffer[ minimum_buffer_length ] = 0; // zero terminate the string

                    // Copy the contents of our current string into the new buffer

                    if ( minimum_buffer_length < m_String.length() )
                    {
                        // The currently held string is longer than the minimum buffer length
                        memcpy( m_Buffer, m_String.data(), minimum_buffer_length * sizeof( TCHAR ) );
                    }
                    else
                    {
                        memcpy( m_Buffer, m_String.data(), m_String.length() * sizeof( TCHAR ) );
                    }

                    return( m_Buffer );
            }
            WFC_CATCH_ALL
            {
                return( nullptr );
            }
            WFC_END_CATCH_ALL
        }

        inline bool GetEnvironmentVariable( _In_z_ LPCTSTR variable_name ) noexcept
        {
            wchar_t temp_string[ 4096 ];

            DWORD number_of_characters = ::GetEnvironmentVariableW( variable_name, temp_string, (DWORD) ( std::size( temp_string ) - 1 ) );

            if ( number_of_characters > (std::size( temp_string ) - 1 ) )
            {
                auto buffer = std::make_unique<wchar_t[]>(static_cast<std::size_t>(number_of_characters * 2));

                number_of_characters = ::GetEnvironmentVariableW( variable_name, buffer.get(), (DWORD) ( ( number_of_characters * 2 ) - 1 ) );

                buffer[ number_of_characters ] = 0x00;
                Copy( buffer.get() );
            }
            else
            {
                temp_string[ number_of_characters ] = 0x00;
                Copy( temp_string );
            }

            return( ( number_of_characters > 0 ) ? true : false );
        }

        inline _Check_return_ size_t GetLength( void ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );
            return( m_String.length() );
        }

        inline _Check_return_ size_t GetSize( void ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );
            return( m_String.length() );
        }

        inline size_t Insert( _In_ SSIZE_T string_index, _In_ TCHAR character_to_insert ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            int const string_length = (int) m_String.length();

            if ( string_index >= string_length )
            {
                Append( character_to_insert );
            }
            else
            {
                if ( string_index < 0 )
                {
                    string_index = 0;
                }

                m_String.insert( string_index, 1, character_to_insert );
            }

            return( m_String.length() );
        }

        inline size_t Insert( _In_ SSIZE_T string_index, _In_z_ LPCTSTR string_to_insert ) noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( string_to_insert );

            if ( string_to_insert != nullptr )
            {
                int const string_length = (int) m_String.length();

                if ( string_index >= string_length )
                {
                    Append( string_to_insert );
                }
                else
                {
                    if ( string_index < 0 )
                    {
                        string_index = 0;
                    }

                    m_String.insert( string_index, string_to_insert );
                }
            }

            return( m_String.length() );
        }

        inline _Check_return_ bool IsEmpty( void ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );
            return( m_String.empty() );
        }

        inline _Check_return_ CString Left( _In_ size_t count ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );

            CString return_value;

            return_value.m_String = m_String.substr( 0, count );

            return( return_value );
        }

        inline void MakeLower( void ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            // 2000-05-01
            // Thanks go to Kevin Godden (k.godden@mvt.ie) for sending me a more terse implementation.
            // It looked like this:
            // std::ctype<TCHAR>().tolower( reinterpret_cast< text_string * >( m_String )->begin(), reinterpret_cast< text_string * >( m_String )->end() );
            // 2000-07-19
            // However, in Visual Studio 7.0, Microsoft made the destructor for std::ctype
            // protected. This broke the compile so I had to revert back to my 2000-04-21
            // implementation. Silly me, I thought the S in STL stood for "STANDARD!"

            //std::vector< TCHAR > character_array( m_String.data(), m_String.data() + m_String.length() );
            //std::transform( character_array.begin(), character_array.end(), character_array.begin(), tolower );

#if defined( UNICODE) 
            // UNICODE build
            //Copy( std::wstring( character_array.begin(), character_array.end() ).c_str() );

#else // UNICODE

            // ASCII build
            //Copy( std::string( character_array.begin(), character_array.end() ).c_str() );
#endif // UNICODE

            // 2011-12-28 SRB - I give up. I cannot get a STL version to compile without warnings. I'll just do it the C way...
            for ( size_t string_index = 0; string_index < m_String.length(); string_index++ )
            {
                m_String[ string_index ] = (TCHAR) tolower( m_String[ string_index ] );
            }
        }

        inline void MakeReverse( void ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            if ( m_String.empty() == true )
            {
                return;
            }

            size_t last_character_index = m_String.length() - 1;
            size_t loop_index           = 0;

            TCHAR character_to_move = 0;

            while( loop_index < last_character_index )
            {
                character_to_move = m_String.at( loop_index );
                m_String.at( loop_index ) = m_String.at( last_character_index );
                m_String.at( last_character_index ) = character_to_move;
                loop_index++;
                last_character_index--;
            }
        }

        inline void MakeUpper( void ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            // 2000-05-01
            // Thanks go to Kevin Godden (k.godden@mvt.ie) for sending me a more terse implementation.
            // It looked like this:
            // std::ctype<TCHAR>().toupper( reinterpret_cast< text_string * >( m_String )->begin(), reinterpret_cast< text_string * >( m_String )->end() );
            // 2000-07-19
            // However, in Visual Studio 7.0, Microsoft made the destructor for std::ctype
            // protected. This broke the compile so I had to revert back to my 2000-04-21
            // implementation. Silly me, I thought the S in STL stood for "STANDARD!"

            //std::vector< TCHAR > character_array( m_String.data(), m_String.data() + m_String.length() );
            //std::transform( character_array.begin(), character_array.end(), character_array.begin(), toupper );

#if defined( UNICODE) 
            // UNICODE build
            //Copy( std::wstring( character_array.begin(), character_array.end() ).c_str() );

#else // UNICODE

            // ASCII build
            //Copy( std::string( character_array.begin(), character_array.end() ).c_str() );
#endif // UNICODE

            // 2011-12-28 SRB - I give up. I cannot get a STL version to compile without warnings. I'll just do it the C way...
            for ( size_t string_index = 0; string_index < m_String.length(); string_index++ )
            {
                m_String[ string_index ] = (TCHAR) toupper( m_String[ string_index ] );
            }
        }

        inline _Check_return_ CString Mid( _In_ size_t first, _In_ size_t count ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );

            CString return_value;

            return_value.m_String = m_String.substr( first, count );

            return( return_value );
        }

        inline _Check_return_ CString Mid( _In_ size_t first ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );

            if ( first == 0 )
            {
                return( CString( *this ) );
            }

            // "ABCD1234" Mid( 2 ) should return "CD1234"
            CString return_value;

            return_value.m_String = m_String.substr( first, m_String.length() - first );

            return( return_value );
        }

        inline void ReleaseBuffer( _In_ SSIZE_T new_length = (-1) ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            WFC_TRY
            {
                if ( m_Buffer == nullptr )
                {
                    return;
                }

                if ( new_length < 0 )
                {
                    // zero terminated string buffer

                    Copy( CString( m_Buffer ) );
                }
                else
                {
                    m_String.resize( 0 ); // Empty the string
                    m_String.assign( m_Buffer, new_length );
                }

                delete m_Buffer;
                m_Buffer = nullptr;
            }
            WFC_CATCH_ALL
            {
            }
            WFC_END_CATCH_ALL
        }

        // 2000-12-05
        // Thanks go to Matt Foltz for noticing that I forgot to implement Remove()
        inline _Check_return_ size_t Remove( _In_ const TCHAR character_to_remove ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            const size_t string_length = m_String.length();

            if ( string_length == 0 )
            {
                return( 0 );
            }

            CString new_string;

            size_t loop_index = 0;

            TCHAR character_to_test = 0;

            while( loop_index < string_length )
            {
                character_to_test = m_String.at( loop_index );

                if ( character_to_test != character_to_remove )
                {
                    new_string.Append( character_to_test, 1 );
                }

                loop_index++;
            }

            loop_index = string_length - new_string.GetLength();

            Copy( new_string );

            return( loop_index );
        }

        inline _Check_return_ size_t Replace( _In_ const TCHAR old_character, _In_ const TCHAR new_character ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            size_t number_of_replacements = 0;
            size_t string_index           = 0;
            const size_t string_length = m_String.length();

            while( string_index < string_length )
            {
                if ( m_String.at( string_index ) == old_character )
                {
                    m_String.at( string_index ) = new_character;
                    number_of_replacements++;
                }

                string_index++;
            }

            return( number_of_replacements );
        }

        inline _Check_return_ size_t Replace( _In_z_ LPCTSTR old_string, _In_z_ LPCTSTR replacement_string ) noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER( old_string );
            WFC_VALIDATE_POINTER_NULL_OK( replacement_string );

            std::size_t number_of_replacements = 0;

            WFC_TRY
            {
                std::size_t const old_string_length = _tcslen( old_string );

                if ( old_string_length == 0 )
                {
                    return( 0 );
                }

                CString new_string;

                SSIZE_T found_at = 0;

                found_at = Find( old_string );

                while( found_at != STRING_NOT_FOUND )
                {
                    new_string.Append( Left( found_at ) );

                    if ( replacement_string != nullptr )
                    {
                        new_string.Append( replacement_string );
                    }

                    Copy( Right( m_String.length() - ( found_at + old_string_length ) ) );

                    number_of_replacements++;

                    found_at = Find( old_string );
                }

                // The old string is no longer found, pick up any data left in the string

                if ( number_of_replacements > 0 )
                {
                    new_string.Append( *this );
                    Copy( new_string );
                }
            }
            WFC_CATCH_ALL
            {
            }
            WFC_END_CATCH_ALL

            return( number_of_replacements );
        }

        inline _Check_return_ SSIZE_T ReverseFind( _In_ const TCHAR character ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );
            return( m_String.rfind( character ) );
        }

        _Check_return_ bool EndsWith( _In_z_ LPCTSTR ending ) const noexcept
        {
            if ( ending == nullptr or ending[ 0 ] == 0x00 )
            {
                return( false );
            }

            const size_t ending_length = _tcslen( ending );

            if ( ending_length > m_String.length() )
            {
                return( false );
            }

            CString our_ending = Right( ending_length );

            int const comparison_result = our_ending.Compare( ending );

            if ( comparison_result == I_AM_EQUAL_TO_THAT )
            {
                return( true );
            }

            return( false );
        }

        inline _Check_return_ bool EndsWithNoCase( _In_z_ LPCTSTR ending ) const noexcept
        {
            if ( ending == nullptr or ending[ 0 ] == 0x00 )
            {
                return( false );
            }

            const size_t ending_length = _tcslen( ending );

            if ( ending_length > m_String.length() )
            {
                return( false );
            }

            CString our_ending = Right( ending_length );

            int const comparison_result = our_ending.CompareNoCase( ending );

            if ( comparison_result == I_AM_EQUAL_TO_THAT )
            {
                return( true );
            }

            return( false );
        }

        inline _Check_return_ CString Right( _In_ const size_t count ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );

            CString return_value;

            if ( m_String.length() > count )
            {
                return_value.m_String = m_String.substr( m_String.length() - count );
            }
            else
            {
                // 2000-08-05
                // Thanks go to Tony Lam (tony@digitcom.net), I was not handling counts
                // were as long as or longer than the string the same way MFC was. DOH!

                return_value.m_String = m_String;
            }

            return( return_value );
        }

        inline void SetAt( _In_ const size_t character_index, _In_ const TCHAR character ) noexcept
        {
            WFC_VALIDATE_POINTER( this );
            _ASSERTE( character_index < m_String.length() );

            if ( character_index < m_String.length() )
            {
                m_String.at( character_index ) = character;
            }
        }

        inline void SetLength( _In_ const size_t new_length ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            if ( new_length < m_String.length() )
            {
                Copy( Left( new_length ) );
            }
            else if ( new_length > m_String.length() )
            {
                while( new_length > m_String.length() )
                {
                    Append( (TCHAR) ' ' );
                }
            }
        }

        inline _Check_return_ BSTR SetSysString( __inout BSTR * b_string_p, _In_ DWORD code_page = CP_ACP ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );

#if defined( _UNICODE ) || defined( OLE2ANSI )

            (void) ::SysReAllocStringLen( b_string_p, m_String.c_str(),(UINT)  m_String.length() );

#else

            int length = ::MultiByteToWideChar( code_page,
                0,
                m_String.c_str(),
                (int) m_String.length(),
                nullptr,
                0 );

            ::SysReAllocStringLen( b_string_p, nullptr, length );

            ::MultiByteToWideChar( code_page,
                0,
                m_String.c_str(),
                (int) m_String.length(),
                *b_string_p,
                length );

#endif

            return( *b_string_p );
        }

        inline _Check_return_ CString SpanExcluding( _In_z_ LPCTSTR character_set ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( character_set );

            if ( character_set != nullptr and m_String.empty() == false )
            {
                return( Left( ::_tcscspn( m_String.c_str(), character_set ) ) );
            }
            else
            {
                CString return_value;
                return( return_value );
            }
        }

        inline _Check_return_ CString SpanIncluding( _In_z_ LPCTSTR character_set ) const noexcept
        {
            WFC_VALIDATE_POINTER( this );
            WFC_VALIDATE_POINTER_NULL_OK( character_set );

            if ( character_set != nullptr )
            {
                return( Left( ::_tcsspn( m_String.c_str(), character_set ) ) );
            }
            else
            {
                CString return_value;
                return( return_value );
            }
        }

        inline _Check_return_ bool StartsWith( _In_z_ LPCTSTR beginning ) const noexcept
        {
            CString our_beginning = Left( _tcslen( beginning ) );

            int const comparison_result = our_beginning.Compare( beginning );

            if ( comparison_result == I_AM_EQUAL_TO_THAT )
            {
                return( true );
            }

            return( false );
        }

        inline _Check_return_ bool StartsWithNoCase( _In_z_ LPCTSTR beginning ) const noexcept
        {
            CString our_beginning = Left( _tcslen( beginning ) );

            int const comparison_result = our_beginning.CompareNoCase( beginning );

            if ( comparison_result == I_AM_EQUAL_TO_THAT )
            {
                return( true );
            }

            return( false );
        }

        inline void TrimLeft( void ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            std::size_t const string_length = m_String.length();

            if ( string_length > 0 )
            {
                std::size_t number_of_elements_to_erase = 0;

                while( number_of_elements_to_erase < string_length and
                    _istspace( m_String.at( number_of_elements_to_erase ) ) )
                {
                    number_of_elements_to_erase++;
                }

                if ( number_of_elements_to_erase >= string_length )
                {
                    m_String.resize( 0 );
                }
                else
                {
                    m_String.erase( 0, number_of_elements_to_erase );
                }
            }
        }

        inline void TrimLeft( _In_ TCHAR character ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            std::size_t const string_length = m_String.length();

            if ( string_length > 0 )
            {
                std::size_t number_of_elements_to_erase = 0;

                while( number_of_elements_to_erase < string_length and
                    character == m_String.at( number_of_elements_to_erase ) )
                {
                    number_of_elements_to_erase++;
                }

                if ( number_of_elements_to_erase >= string_length )
                {
                    m_String.resize( 0 );
                }
                else
                {
                    m_String.erase( 0, number_of_elements_to_erase );
                }
            }
        }

        inline void TrimRight( void ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            if ( m_String.length() <= 0 )
            {
                return;
            }

            while( _istspace( m_String.at( m_String.length() - 1 ) ) )
            {
                m_String.resize( m_String.length() - 1 );

                if ( m_String.length() <= 0 )
                {
                    return;
                }
            }
        }

        inline void TrimRight( _In_ TCHAR character ) noexcept
        {
            WFC_VALIDATE_POINTER( this );

            if ( m_String.empty() == true )
            {
                return;
            }

            while( character == m_String.at( m_String.length() - 1 ) )
            {
                m_String.erase( m_String.length() - 1, 1 );

                if ( m_String.length() <= 0 )
                {
                    return;
                }
            }
        }

        void __cdecl Format( _In_z_ __format_string LPCTSTR printf_format_string, ... ) noexcept;

        // Operators

        inline operator LPCTSTR() const noexcept
        {
            return( m_String.c_str() );
        }

        inline _Check_return_ TCHAR operator[]( _In_ const size_t array_index ) const noexcept
        {
            return( GetAt( array_index ) );
        }

        inline _Check_return_ const CString& operator = ( _In_ const CString& source ) noexcept
        {
            Copy( source );
            return( *this );
        }

        inline _Check_return_ const CString& operator = ( _In_z_ LPCSTR source ) noexcept
        {
            Copy( source );
            return( *this );
        }

        inline _Check_return_ const CString& operator = ( _In_z_ LPCWSTR source ) noexcept
        {
            Copy( source );
            return( *this );
        }

        inline _Check_return_ const CString& operator = ( _In_z_ const unsigned char * source ) noexcept
        {
            Copy( (LPCSTR) source );
            return( *this );
        }

        inline _Check_return_ const CString& operator = ( _In_ TCHAR character ) noexcept
        {
            m_String.resize( 0 );
            m_String.append( 1, character );
            return( *this );
        }

        inline _Check_return_ const CString& operator += ( _In_ const CString& source ) noexcept
        {
            Append( source );
            return( *this );
        }

        inline _Check_return_ const CString& operator += ( _In_ TCHAR character ) noexcept
        {
            m_String.append( 1, character );
            return( *this );
        }

        inline friend _Check_return_ CString operator + ( _In_ const CString& string1, _In_ const CString& string2 ) noexcept
        {
            CString return_value( string1 );
            return_value.Append( string2 );
            return( return_value );
        }

        inline friend _Check_return_ CString operator + ( _In_ const CString& string1, _In_ TCHAR character ) noexcept
        {
            CString return_value( string1 );
            return_value.Append( character, 1 );
            return( return_value );
        }

        inline friend _Check_return_ CString operator + ( _In_ TCHAR character, _In_ const CString& string2 ) noexcept
        {
            CString return_value;
            return_value.Append( character, 1 );
            return_value.Append( string2 );
            return( return_value );
        }

        inline friend _Check_return_ CString operator + ( _In_ const CString& string1, _In_z_ LPCTSTR string2 ) noexcept
        {
            CString return_value( string1 );
            return_value.Append( string2 );
            return( return_value );
        }

        inline friend _Check_return_ CString operator + ( _In_z_ LPCTSTR string1, _In_ const CString& string2 ) noexcept
        {
            CString return_value( string1 );
            return_value.Append( string2 );
            return( return_value );
        }
    };

    inline _Check_return_ bool operator==( _In_ const CString& s1, _In_ const CString& s2 ) noexcept
    {
        return( ( s1.Compare( s2 ) == I_AM_EQUAL_TO_THAT) ? true : false );
    };

    inline _Check_return_ bool operator==( _In_ const CString& s1, _In_z_ LPCTSTR s2 ) noexcept
    {
        return( ( s1.Compare( s2 ) == I_AM_EQUAL_TO_THAT) ? true : false );
    };

    inline _Check_return_ bool operator==( _In_z_ LPCTSTR s1, _In_ const CString& s2 ) noexcept
    {
        return( ( s2.Compare( s1 ) == I_AM_EQUAL_TO_THAT) ? true : false );
    };

    inline _Check_return_ bool operator!=( _In_ const CString& s1, _In_ const CString& s2 ) noexcept
    {
        return( ( s1.Compare( s2 ) == I_AM_EQUAL_TO_THAT) ? false : true );
    };

    inline _Check_return_ bool operator!=( _In_ const CString& s1, _In_z_ LPCTSTR s2 ) noexcept
    {
        return( ( s1.Compare( s2 ) == I_AM_EQUAL_TO_THAT) ? false : true );
    };

    inline _Check_return_ bool operator!=( _In_z_ LPCTSTR s1, _In_ const CString& s2 ) noexcept
    {
        return( ( s2.Compare( s1 ) == I_AM_EQUAL_TO_THAT) ? false : true );
    };

    inline _Check_return_ bool operator<( _In_ const CString& s1, _In_ const CString& s2 ) noexcept
    {
        return( ( s1.Compare( s2 ) < I_AM_EQUAL_TO_THAT) ? true : false );
    };

    inline _Check_return_ bool operator<( _In_ const CString& s1, _In_z_ LPCTSTR s2 ) noexcept
    {
        return( ( s1.Compare( s2 ) < I_AM_EQUAL_TO_THAT) ? true : false );
    };

    inline _Check_return_ bool operator<( _In_z_ LPCTSTR s1, _In_ const CString& s2 ) noexcept
    {
        return( ( s2.Compare( s1 ) > I_AM_EQUAL_TO_THAT) ? true : false );
    };

    inline _Check_return_ bool operator>( _In_ const CString& s1, _In_ const CString& s2 ) noexcept
    {
        return( ( s1.Compare( s2 ) > I_AM_EQUAL_TO_THAT) ? true : false );
    };

    inline _Check_return_ bool operator>( _In_ const CString& s1, _In_z_ LPCTSTR s2 ) noexcept
    {
        return( ( s1.Compare( s2 ) > I_AM_EQUAL_TO_THAT) ? true : false );
    };

    inline _Check_return_ bool operator>( _In_z_ LPCTSTR s1, _In_ const CString& s2 ) noexcept
    {
        return( ( s2.Compare( s1 ) < I_AM_EQUAL_TO_THAT ) ? true : false );
    };

    inline _Check_return_ bool operator<=( _In_ const CString& s1, _In_ const CString& s2 ) noexcept
    {
        return( ( s1.Compare( s2 ) <= I_AM_EQUAL_TO_THAT) ? true : false );
    };

    inline _Check_return_ bool operator<=( _In_ const CString& s1, _In_z_ LPCTSTR s2 ) noexcept
    {
        return( ( s1.Compare( s2 ) <= I_AM_EQUAL_TO_THAT) ? true : false );
    };

    inline _Check_return_ bool operator<=( _In_z_ LPCTSTR s1, _In_ const CString& s2 ) noexcept
    {
        return( ( s2.Compare( s1 ) >= I_AM_EQUAL_TO_THAT) ? true : false );
    };

    inline _Check_return_ bool operator>=( _In_ const CString& s1, _In_ const CString& s2 ) noexcept
    {
        return( ( s1.Compare( s2 ) >= I_AM_EQUAL_TO_THAT) ? true : false );
    };

    inline _Check_return_ bool operator>=( _In_ const CString& s1, _In_z_ LPCTSTR s2 ) noexcept
    {
        return( ( s1.Compare( s2 ) >= I_AM_EQUAL_TO_THAT) ? true : false );
    };

    inline _Check_return_ bool operator>=( _In_z_ LPCTSTR s1, _In_ const CString& s2 ) noexcept
    {
        return( ( s2.Compare( s1 ) <= I_AM_EQUAL_TO_THAT) ? true : false );
    };

    class CWideString;

    _Check_return_ int64_t find_byte( _In_ uint8_t const byte_value, _In_reads_bytes_( buffer_size ) uint8_t const * __restrict buffer, _In_ int64_t const buffer_size ) noexcept;

    inline _Check_return_ int64_t find_in_memory(_In_reads_bytes_( buffer_size ) uint8_t const * __restrict buffer, _In_ int64_t const buffer_size, _In_reads_bytes_( pattern_size ) uint8_t const * __restrict pattern, _In_ int64_t const pattern_size ) noexcept
    {
        if ( buffer == nullptr or buffer_size < 1 or pattern == nullptr or pattern_size < 1 or pattern_size > buffer_size )
        {
            return( BYTES_NOT_FOUND );
        }

        uint8_t const first_byte = pattern[ 0 ];

        int64_t buffer_index = 0;
        int64_t found_at = 0;

        while( buffer_index < buffer_size )
        {
            found_at = find_byte( first_byte, &buffer[ buffer_index ], (int64_t) ( buffer_size - buffer_index ) );

            if ( found_at == BYTES_NOT_FOUND )
            {
                return( BYTES_NOT_FOUND );
            }

            buffer_index += found_at;

            if ( pattern_size == 1 )
            {
                return( buffer_index );
            }

            int64_t pattern_index = 1;

            while( pattern_index < pattern_size )
            {
                if ((buffer_index + pattern_index) >= buffer_size)
                {
                    return(BYTES_NOT_FOUND);
                }

                if ( buffer[ buffer_index + pattern_index ] == pattern[ pattern_index ] )
                {
                    if ( pattern_index == ( pattern_size - 1 ) )
                    {
                        return( buffer_index );
                    }
                }
                else
                {
                    pattern_index = pattern_size; // exit the loop
                }

                pattern_index++;
            }

            buffer_index++;
        }

        return( BYTES_NOT_FOUND );
    }

    inline void find_all_in_memory(_In_reads_bytes_(buffer_size) uint8_t const * __restrict buffer, _In_ int64_t const buffer_size, _In_reads_bytes_(pattern_size) uint8_t const * __restrict pattern, _In_ int64_t const pattern_size, std::vector<uint64_t>& results) noexcept
    {
        results.clear();

        int64_t found_at_offset = 0;
        int64_t buffer_index = 0;
        int64_t number_of_bytes_remaining_in_buffer = buffer_size;

        while (number_of_bytes_remaining_in_buffer >= pattern_size)
        {
            found_at_offset = find_in_memory(&buffer[buffer_index], number_of_bytes_remaining_in_buffer, pattern, pattern_size);

            if (found_at_offset == BYTES_NOT_FOUND)
            {
                return;
            }

            // We've got a hit!

            results.push_back(found_at_offset + buffer_index);

            // Now take a step into the buffer...

            buffer_index += found_at_offset + pattern_size;
            number_of_bytes_remaining_in_buffer = buffer_size - buffer_index;
        }
    }

    inline _Check_return_ bool is_all_space( _In_ std::vector<uint8_t> const& s ) noexcept
    {
        std::size_t const number_of_characters = s.size();

        if (number_of_characters == 0)
        {
            return(false);
        }

        auto buffer = s.data();

        std::size_t here = 0;

        char character_to_test = 0;

        while (here < number_of_characters)
        {
            character_to_test = buffer[here];

            if (character_to_test == 0x20 or
                character_to_test == TAB_CHARACTER or
                character_to_test == CARRIAGE_RETURN or
                character_to_test == LINE_FEED)
            {
                here++;
            }
            else
            {
                return(false);
            }
        }

        return(true);
    }

#if ! defined( WFC_DONT_USE_NAMESPACES )
#if ( _MSC_VER > 1000 )
    // Terminate the namespace declaration
}
#endif // _MSC_VER > 1000
#endif // WFC_DONT_USE_NAMESPACES

#endif // MFC_TO_STL_HEADER_FILE
