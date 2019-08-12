/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2014, Samuel R. Blackburn
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
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_dotted_ip_address( __in_z_opt const char * ip_address ) noexcept
{
    if ( ip_address == nullptr || ip_address[0] == 0x00 )
    {
        return( false );
    }

    std::wstring a;
    std::wstring b;
    std::wstring c;
    std::wstring d;

    int string_index = 0;
    int ip_address_index = 0;

    while( ip_address[ ip_address_index ] != 0x00 )
    {
        if ( ip_address[ ip_address_index ] != '.' &&
             isdigit( (unsigned char) ip_address[ ip_address_index ] ) == 0 )
        {
            // Illegal character
            return( false );
        }

        if ( ip_address[ ip_address_index ] == '.' )
        {
            string_index++;

            if ( string_index > 3 )
            {
                // Too many dots
                return( false );
            }
        }
        else
        {
            if ( string_index == 0 )
            {
                a.push_back( ip_address[ ip_address_index ] );

                if ( a.length() > 3 )
                {
                    // Too many digits
                    return( false );
                }
            }
            else if ( string_index == 1 )
            {
                b.push_back( ip_address[ ip_address_index ] );

                if ( b.length() > 3 )
                {
                    // Too many digits
                    return( false );
                }
            }
            else if ( string_index == 2 )
            {
                c.push_back( ip_address[ ip_address_index ] );

                if ( c.length() > 3 )
                {
                    // Too many digits
                    return( false );
                }
            }
            else
            {
                d.push_back( ip_address[ ip_address_index ] );

                if ( d.length() > 3 )
                {
                    // Too many digits
                    return( false );
                }
            }
        }

        ip_address_index++;
    }

    if ( a.empty() == true ||
         b.empty() == true ||
         c.empty() == true ||
         d.empty() == true)
    {
        return( false );
    }

    // OK, nothing but digits and dots of the right lengths, check the values

    if ( as_integer(a) > 255 )
    {
        return( false );
    }

    if (as_integer(b) > 255 )
    {
        return( false );
    }

    if (as_integer(c) > 255 )
    {
        return( false );
    }

    if (as_integer(d) > 255 )
    {
        return( false );
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_dotted_ip_address( __in_z_opt wchar_t const * ip_address ) noexcept
{
    if ( ip_address == nullptr || ip_address[0] == 0x00 )
    {
        return( false );
    }

    std::wstring a;
    std::wstring b;
    std::wstring c;
    std::wstring d;

    int string_index = 0;
    int ip_address_index = 0;

    while( ip_address[ ip_address_index ] != 0x00 )
    {
        if ( ip_address[ ip_address_index ] != '.' &&
             iswdigit( ip_address[ ip_address_index ] ) == 0 )
        {
            // Illegal character
            return( false );
        }

        if ( ip_address[ ip_address_index ] == '.' )
        {
            string_index++;

            if ( string_index > 3 )
            {
                // Too many dots
                return( false );
            }
        }
        else
        {
            if ( string_index == 0 )
            {
                a.push_back( ip_address[ ip_address_index ] );

                if ( a.length() > 3 )
                {
                    // Too many digits
                    return( false );
                }
            }
            else if ( string_index == 1 )
            {
                b.push_back( ip_address[ ip_address_index ] );

                if ( b.length() > 3 )
                {
                    // Too many digits
                    return( false );
                }
            }
            else if ( string_index == 2 )
            {
                c.push_back( ip_address[ ip_address_index ] );

                if ( c.length() > 3 )
                {
                    // Too many digits
                    return( false );
                }
            }
            else
            {
                d.push_back( ip_address[ ip_address_index ] );

                if ( d.length() > 3 )
                {
                    // Too many digits
                    return( false );
                }
            }
        }

        ip_address_index++;
    }

    if ( a.empty() == true ||
         b.empty() == true ||
         c.empty() == true ||
         d.empty() == true)
    {
        return( false );
    }

    // OK, nothing but digits and dots of the right lengths, check the values

    if (as_integer(a) > 255 )
    {
        return( false );
    }

    if (as_integer(b) > 255 )
    {
        return( false );
    }

    if (as_integer(c) > 255 )
    {
        return( false );
    }

    if (as_integer(d) > 255 )
    {
        return( false );
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_guid( __in_z char const * characters ) noexcept
{
    //           11111111112222222222333333333
    // 012345678901234567890123456789012345678
    // 0AEE2A92-BCBB-11d0-8C72-00C04FC2B085
    if ( characters == nullptr || characters[ 0 ] == 0x00 )
    {
        return( false );
    }

    for ( auto const array_index : Range(35) )
    {
        if ( characters[ array_index ] == 0 )
        {
            return( false );
        }

        if ( array_index ==  8 ||
             array_index == 13 ||
             array_index == 18 ||
             array_index == 23 )
        {
            if ( characters[ array_index ] != '-' )
            {
                return( false );
            }
        }
        else
        {
            if ( ! WFC_IS_HEXADECIMAL( characters[ array_index ] )  )
            {
                return( false );
            }
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_guid( __in_z wchar_t const * characters ) noexcept
{
    //           11111111112222222222333333333
    // 012345678901234567890123456789012345678
    // 0AEE2A92-BCBB-11d0-8C72-00C04FC2B085
    if ( characters == nullptr || characters[ 0 ] == 0x00 )
    {
        return( false );
    }

    for ( auto const array_index : Range(35) )
    {
        if ( characters[ array_index ] == 0 )
        {
            return( false );
        }

        if ( array_index ==  8 ||
             array_index == 13 ||
             array_index == 18 ||
             array_index == 23 )
        {
            if ( characters[ array_index ] != '-' )
            {
                return( false );
            }
        }
        else
        {
            if ( ! WFC_IS_HEXADECIMAL( characters[ array_index ] )  )
            {
                return( false );
            }
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_guid_with_curlies( __in_z char const * characters ) noexcept
{
    //           11111111112222222222333333333
    // 012345678901234567890123456789012345678
    // {0AEE2A92-BCBB-11d0-8C72-00C04FC2B085}
    if ( characters == nullptr || characters[ 0 ] != '{' )
    {
        return( false );
    }

    for( auto const array_index : Range(38, 1) )
    {
        if ( array_index == 37 )
        {
            if ( characters[ array_index ] != '}' )
            {
                return( false );
            }
        }
        else if ( array_index ==  9 ||
                  array_index == 14 ||
                  array_index == 19 ||
                  array_index == 24 )
        {
            if ( characters[ array_index ] != '-' )
            {
                return( false );
            }
        }
        else
        {
            if ( ! WFC_IS_HEXADECIMAL( characters[ array_index ] )  )
            {
                return( false );
            }
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_guid_with_curlies( __in_z wchar_t const * characters ) noexcept
{
    //           11111111112222222222333333333
    // 012345678901234567890123456789012345678
    // {0AEE2A92-BCBB-11d0-8C72-00C04FC2B085}
    if ( characters == nullptr || characters[ 0 ] != '{' )
    {
        return( false );
    }

    for ( auto const array_index : Range(38, 1) )
    {
        if ( array_index == 37 )
        {
            if ( characters[ array_index ] != '}' )
            {
                return( false );
            }
        }
        else if ( array_index ==  9 ||
                  array_index == 14 ||
                  array_index == 19 ||
                  array_index == 24 )
        {
            if ( characters[ array_index ] != '-' )
            {
                return( false );
            }
        }
        else
        {
            if ( ! WFC_IS_HEXADECIMAL( characters[ array_index ] )  )
            {
                return( false );
            }
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_octal( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr || number_of_bytes < 1 )
    {
        return( false );
    }

    for ( auto const buffer_index : Range(number_of_bytes) )
    {
        if ( ! WFC_IS_OCTAL( buffer[ buffer_index ] ) )
        {
            return( false );
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_wide_octal( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr || number_of_bytes < 2 )
    {
        return( false );
    }

    auto wide_string = reinterpret_cast<wchar_t const *>(buffer);

    int buffer_size = (int) ( number_of_bytes / sizeof( wchar_t ) );

    for ( auto const buffer_index : Range(buffer_size) )
    {
        if ( ! WFC_IS_OCTAL( wide_string[ buffer_index ] ) )
        {
            return( false );
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_octal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr || number_of_bytes < 1 )
    {
        return( false );
    }

    int parser_state = 0;

    for ( auto const buffer_index : Range(number_of_bytes) )
    {
        switch( parser_state )
        {
            case 0: // digits

                if ( ! WFC_IS_OCTAL( buffer[ buffer_index ] ) )
                {
                    // Not a digit, do we advance the state or return false?

                    if ( buffer[ buffer_index ] == ' ' )
                    {
                        parser_state = 1;
                    }
                    else if ( buffer[ buffer_index ] == 0x00 )
                    {
                        parser_state = 2;
                    }
                    else
                    {
                        return( false );
                    }
                }

                break;

            case 1: // space

                if ( buffer[ buffer_index ] != ' ' )
                {
                    // Do we advance the state or return false?

                    if ( buffer[ buffer_index ] == 0x00 )
                    {
                        parser_state = 2;
                    }
                    else
                    {
                        return( false );
                    }
                }

                break;

            case 2: // NULLs

                if ( buffer[ buffer_index ] != 0x00 )
                {
                    return( false );
                }

                break;

            default:

                // We are in a bad state
                return( false );
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_wide_octal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr || number_of_bytes < 2 )
    {
        return( false );
    }

    auto wide_string = reinterpret_cast<wchar_t const *>(buffer);

    int const buffer_size = (int) (number_of_bytes / sizeof( wchar_t ));

    int parser_state = 0;

    for ( auto const buffer_index : Range(buffer_size) )
    {
        switch( parser_state )
        {
            case 0: // digits

                if ( ! WFC_IS_OCTAL( wide_string[ buffer_index ] ) )
                {
                    // Not a digit, do we advance the state or return false?

                    if ( wide_string[ buffer_index ] == ' ' )
                    {
                        parser_state = 1;
                    }
                    else if ( wide_string[ buffer_index ] == 0x00 )
                    {
                        parser_state = 2;
                    }
                    else
                    {
                        return( false );
                    }
                }

                break;

            case 1: // space

                if ( wide_string[ buffer_index ] != ' ' )
                {
                    // Do we advance the state or return false?

                    if ( wide_string[ buffer_index ] == 0x00 )
                    {
                        parser_state = 2;
                    }
                    else
                    {
                        return( false );
                    }
                }

                break;

            case 2: // NULLs

                if ( wide_string[ buffer_index ] != 0x00 )
                {
                    return( false );
                }

                break;

            default:

                // We are in a bad state
                return( false );
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_hexadecimal( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr || number_of_bytes < 1 )
    {
        return( false );
    }

    for ( auto const buffer_index : Range(number_of_bytes) )
    {
        if ( ! WFC_IS_HEXADECIMAL( buffer[ buffer_index ] ) )
        {
            return( false );
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_wide_hexadecimal( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr || number_of_bytes < 2 )
    {
        return( false );
    }

    auto wide_string = reinterpret_cast<wchar_t const *>(buffer);

    int buffer_size = (int) (number_of_bytes / sizeof( wchar_t ));

    for ( auto const buffer_index : Range(buffer_size) )
    {
        if ( ! WFC_IS_HEXADECIMAL( wide_string[ buffer_index ] ) )
        {
            return( false );
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_hexadecimal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr || number_of_bytes < 1 )
    {
        return( false );
    }

    int parser_state = 0;

    for ( auto const buffer_index : Range(number_of_bytes) )
    {
        switch( parser_state )
        {
            case 0: // digits

                if ( ! WFC_IS_HEXADECIMAL( buffer[ buffer_index ] ) )
                {
                    // Not a digit, do we advance the state or return false?

                    if ( buffer[ buffer_index ] == ' ' )
                    {
                        parser_state = 1;
                    }
                    else if ( buffer[ buffer_index ] == 0x00 )
                    {
                        parser_state = 2;
                    }
                    else
                    {
                        return( false );
                    }
                }

                break;

            case 1: // space

                if ( buffer[ buffer_index ] != ' ' )
                {
                    // Do we advance the state or return false?

                    if ( buffer[ buffer_index ] == 0x00 )
                    {
                        parser_state = 2;
                    }
                    else
                    {
                        return( false );
                    }
                }

                break;

            case 2: // NULLs

                if ( buffer[ buffer_index ] != 0x00 )
                {
                    return( false );
                }

                break;

            default:

                // We are in a bad state
                return( false );
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_wide_hexadecimal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr || number_of_bytes < 2 )
    {
        return( false );
    }

    auto wide_string = reinterpret_cast<wchar_t const *>(buffer);

    int buffer_size = (int) (number_of_bytes / sizeof( wchar_t ));

    int parser_state = 0;

    for ( auto const buffer_index : Range(buffer_size) )
    {
        switch( parser_state )
        {
            case 0: // digits

                if ( ! WFC_IS_HEXADECIMAL( wide_string[ buffer_index ] ) )
                {
                    // Not a digit, do we advance the state or return false?

                    if ( wide_string[ buffer_index ] == ' ' )
                    {
                        parser_state = 1;
                    }
                    else if ( wide_string[ buffer_index ] == 0x00 )
                    {
                        parser_state = 2;
                    }
                    else
                    {
                        return( false );
                    }
                }

                break;

            case 1: // space

                if ( wide_string[ buffer_index ] != ' ' )
                {
                    // Do we advance the state or return false?

                    if ( wide_string[ buffer_index ] == 0x00 )
                    {
                        parser_state = 2;
                    }
                    else
                    {
                        return( false );
                    }
                }

                break;

            case 2: // NULLs

                if ( wide_string[ buffer_index ] != 0x00 )
                {
                    return( false );
                }

                break;

            default:

                // We are in a bad state
                return( false );
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_decimal( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr || number_of_bytes < 1 )
    {
        return( false );
    }

    for( auto const buffer_index : Range(number_of_bytes) )
    {
        if ( ! WFC_IS_DECIMAL( buffer[ buffer_index ] ) )
        {
            return( false );
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_wide_decimal( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr || number_of_bytes < 2 )
    {
        return( false );
    }

    auto wide_string = reinterpret_cast<wchar_t const *>(buffer);

    int buffer_size = (int) (number_of_bytes / sizeof( wchar_t ));

    for ( auto const buffer_index : Range(buffer_size) )
    {
        if ( ! WFC_IS_DECIMAL( wide_string[ buffer_index ] ) )
        {
            return( false );
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_decimal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr || number_of_bytes < 1 )
    {
        return( false );
    }

    int parser_state = 0;

    for ( auto const buffer_index : Range(number_of_bytes) )
    {
        switch( parser_state )
        {
            case 0: // digits

                if ( ! WFC_IS_DECIMAL( buffer[ buffer_index ] ) )
                {
                    // Not a digit, do we advance the state or return false?

                    if ( buffer[ buffer_index ] == ' ' )
                    {
                        parser_state = 1;
                    }
                    else if ( buffer[ buffer_index ] == 0x00 )
                    {
                        parser_state = 2;
                    }
                    else
                    {
                        return( false );
                    }
                }

                break;

            case 1: // space

                if ( buffer[ buffer_index ] != ' ' )
                {
                    // Do we advance the state or return false?

                    if ( buffer[ buffer_index ] == 0x00 )
                    {
                        parser_state = 2;
                    }
                    else
                    {
                        return( false );
                    }
                }

                break;

            case 2: // NULLs

                if ( buffer[ buffer_index ] != 0x00 )
                {
                    return( false );
                }

                break;

            default:

                // We are in a bad state
                return( false );
        }
    }

    return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_is_wide_decimal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr || number_of_bytes < 2 )
    {
        return( false );
    }

    auto wide_string = reinterpret_cast<wchar_t const *>(buffer);

    int buffer_size = (int) (number_of_bytes / sizeof( wchar_t ));

    int parser_state = 0;

    for ( auto const buffer_index : Range(buffer_size) )
    {
        switch( parser_state )
        {
            case 0: // digits

                if ( ! WFC_IS_DECIMAL( wide_string[ buffer_index ] ) )
                {
                    // Not a digit, do we advance the state or return false?

                    if ( wide_string[ buffer_index ] == ' ' )
                    {
                        parser_state = 1;
                    }
                    else if ( wide_string[ buffer_index ] == 0x00 )
                    {
                        parser_state = 2;
                    }
                    else
                    {
                        return( false );
                    }
                }

                break;

            case 1: // space

                if ( wide_string[ buffer_index ] != ' ' )
                {
                    // Do we advance the state or return false?

                    if ( wide_string[ buffer_index ] == 0x00 )
                    {
                        parser_state = 2;
                    }
                    else
                    {
                        return( false );
                    }
                }

                break;

            case 2: // NULLs

                if ( wide_string[ buffer_index ] != 0x00 )
                {
                    return( false );
                }

                break;

            default:

                // We are in a bad state
                return( false );
        }
    }

    return( true );
}

_Check_return_ int Win32FoundationClasses::wfc_find_curly_guid( __in_bcount( number_of_bytes_in_buffer ) uint8_t const * buffer, __in std::size_t const number_of_bytes_in_buffer ) noexcept
{
    if ( buffer == nullptr || number_of_bytes_in_buffer < 38 )
    {
        return( GUID_NOT_FOUND );
    }

    for ( auto const buffer_index : Range( number_of_bytes_in_buffer - 37 ) )
    {
        if ( buffer[ buffer_index ] == '{' )
        {
            if ( Win32FoundationClasses::wfc_is_guid_with_curlies( reinterpret_cast<char const *>(&buffer[ buffer_index ]) ) == true )
            {
                return( static_cast<int>(buffer_index) );
            }
        }
    }

    return( GUID_NOT_FOUND );
}

_Check_return_ int Win32FoundationClasses::wfc_find_wide_curly_guid( __in_bcount( number_of_bytes_in_buffer ) uint8_t const * buffer, __in std::size_t const number_of_bytes_in_buffer ) noexcept
{
    if ( buffer == nullptr || number_of_bytes_in_buffer < 76 )
    {
        return( GUID_NOT_FOUND );
    }

    for ( auto const buffer_index : Range( number_of_bytes_in_buffer - 75 ) )
    {
        if ( buffer[ buffer_index ] == '{' )
        {
            if ( Win32FoundationClasses::wfc_is_guid_with_curlies( reinterpret_cast<wchar_t const *>(&buffer[ buffer_index ]) ) == true )
            {
                return( static_cast<int>(buffer_index) );
            }
        }
    }

    return( GUID_NOT_FOUND );
}

// End of source
