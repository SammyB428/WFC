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
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

_Check_return_ bool Win32FoundationClasses::wfc_is_dotted_ip_address( _In_ std::string_view ip_address ) noexcept
{
    if ( ip_address.empty() == true or ip_address.length() < 7 )
    {
        return( false );
    }

    std::wstring a;
    std::wstring b;
    std::wstring c;
    std::wstring d;

    int string_index{ 0 };

    for( auto const ip_address_index : Range(ip_address.length()))
    {
        if ( ip_address[ ip_address_index ] not_eq '.' and
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
    }

    if ( a.empty() == true or
         b.empty() == true or
         c.empty() == true or
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

_Check_return_ bool Win32FoundationClasses::wfc_is_dotted_ip_address(_In_ std::u16string_view ip_address) noexcept
{
    if (ip_address.empty() == true or ip_address.length() < 7)
    {
        return(false);
    }

    std::wstring a;
    std::wstring b;
    std::wstring c;
    std::wstring d;

    int string_index{ 0 };

    for (auto const ip_address_index : Range(ip_address.length()))
    {
        if (ip_address[ip_address_index] not_eq '.' and
            isdigit(static_cast<int>(ip_address[ip_address_index])) == 0)
        {
            // Illegal character
            return(false);
        }

        if (ip_address[ip_address_index] == '.')
        {
            string_index++;

            if (string_index > 3)
            {
                // Too many dots
                return(false);
            }
        }
        else
        {
            if (string_index == 0)
            {
                a.push_back(ip_address[ip_address_index]);

                if (a.length() > 3)
                {
                    // Too many digits
                    return(false);
                }
            }
            else if (string_index == 1)
            {
                b.push_back(ip_address[ip_address_index]);

                if (b.length() > 3)
                {
                    // Too many digits
                    return(false);
                }
            }
            else if (string_index == 2)
            {
                c.push_back(ip_address[ip_address_index]);

                if (c.length() > 3)
                {
                    // Too many digits
                    return(false);
                }
            }
            else
            {
                d.push_back(ip_address[ip_address_index]);

                if (d.length() > 3)
                {
                    // Too many digits
                    return(false);
                }
            }
        }
    }

    if (a.empty() == true or
        b.empty() == true or
        c.empty() == true or
        d.empty() == true)
    {
        return(false);
    }

    // OK, nothing but digits and dots of the right lengths, check the values

    if (as_integer(a) > 255)
    {
        return(false);
    }

    if (as_integer(b) > 255)
    {
        return(false);
    }

    if (as_integer(c) > 255)
    {
        return(false);
    }

    if (as_integer(d) > 255)
    {
        return(false);
    }

    return(true);
}

_Check_return_ bool Win32FoundationClasses::wfc_is_dotted_ip_address( _In_ std::wstring_view ip_address ) noexcept
{
    if ( ip_address.empty() == true or ip_address.length() < 7)
    {
        return( false );
    }

    std::wstring a;
    std::wstring b;
    std::wstring c;
    std::wstring d;

    int string_index{ 0 };

    for ( auto const ip_address_index : Range(ip_address.length()))
    {
        if ( ip_address[ ip_address_index ] not_eq '.' and
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
    }

    if ( a.empty() == true or
         b.empty() == true or
         c.empty() == true or
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

_Check_return_ bool Win32FoundationClasses::wfc_is_guid( _In_ std::string_view characters ) noexcept
{
    //           11111111112222222222333333333
    // 012345678901234567890123456789012345678
    // 0AEE2A92-BCBB-11d0-8C72-00C04FC2B085
    if (characters.length() < 36)
    {
        return(false);
    }

    if (characters[8] not_eq '-' or
        characters[13] not_eq '-' or
        characters[18] not_eq '-' or
        characters[23] not_eq '-')
    {
        return(false);
    }

    if (not WFC_IS_HEXADECIMAL(characters[0]) or
        not WFC_IS_HEXADECIMAL(characters[1]) or
        not WFC_IS_HEXADECIMAL(characters[2]) or
        not WFC_IS_HEXADECIMAL(characters[3]) or
        not WFC_IS_HEXADECIMAL(characters[4]) or
        not WFC_IS_HEXADECIMAL(characters[5]) or
        not WFC_IS_HEXADECIMAL(characters[6]) or
        not WFC_IS_HEXADECIMAL(characters[7]) or
        not WFC_IS_HEXADECIMAL(characters[9]) or
        not WFC_IS_HEXADECIMAL(characters[10]) or
        not WFC_IS_HEXADECIMAL(characters[11]) or
        not WFC_IS_HEXADECIMAL(characters[12]) or
        not WFC_IS_HEXADECIMAL(characters[14]) or
        not WFC_IS_HEXADECIMAL(characters[15]) or
        not WFC_IS_HEXADECIMAL(characters[16]) or
        not WFC_IS_HEXADECIMAL(characters[17]) or
        not WFC_IS_HEXADECIMAL(characters[19]) or
        not WFC_IS_HEXADECIMAL(characters[20]) or
        not WFC_IS_HEXADECIMAL(characters[21]) or
        not WFC_IS_HEXADECIMAL(characters[22]) or
        not WFC_IS_HEXADECIMAL(characters[24]) or
        not WFC_IS_HEXADECIMAL(characters[25]) or
        not WFC_IS_HEXADECIMAL(characters[26]) or
        not WFC_IS_HEXADECIMAL(characters[27]) or
        not WFC_IS_HEXADECIMAL(characters[28]) or
        not WFC_IS_HEXADECIMAL(characters[29]) or
        not WFC_IS_HEXADECIMAL(characters[30]) or
        not WFC_IS_HEXADECIMAL(characters[31]) or
        not WFC_IS_HEXADECIMAL(characters[32]) or
        not WFC_IS_HEXADECIMAL(characters[33]) or
        not WFC_IS_HEXADECIMAL(characters[34]) or
        not WFC_IS_HEXADECIMAL(characters[35]))
    {
        return(false);
    }

    return(true);
}

_Check_return_ bool Win32FoundationClasses::wfc_is_guid(_In_ std::u16string_view characters) noexcept
{
    //           11111111112222222222333333333
    // 012345678901234567890123456789012345678
    // 0AEE2A92-BCBB-11d0-8C72-00C04FC2B085
    if (characters.length() < 36)
    {
        return(false);
    }

    if (characters[8] not_eq '-' or
        characters[13] not_eq '-' or
        characters[18] not_eq '-' or
        characters[23] not_eq '-')
    {
        return(false);
    }

    if (not WFC_IS_HEXADECIMAL(characters[0]) or
        not WFC_IS_HEXADECIMAL(characters[1]) or
        not WFC_IS_HEXADECIMAL(characters[2]) or
        not WFC_IS_HEXADECIMAL(characters[3]) or
        not WFC_IS_HEXADECIMAL(characters[4]) or
        not WFC_IS_HEXADECIMAL(characters[5]) or
        not WFC_IS_HEXADECIMAL(characters[6]) or
        not WFC_IS_HEXADECIMAL(characters[7]) or
        not WFC_IS_HEXADECIMAL(characters[9]) or
        not WFC_IS_HEXADECIMAL(characters[10]) or
        not WFC_IS_HEXADECIMAL(characters[11]) or
        not WFC_IS_HEXADECIMAL(characters[12]) or
        not WFC_IS_HEXADECIMAL(characters[14]) or
        not WFC_IS_HEXADECIMAL(characters[15]) or
        not WFC_IS_HEXADECIMAL(characters[16]) or
        not WFC_IS_HEXADECIMAL(characters[17]) or
        not WFC_IS_HEXADECIMAL(characters[19]) or
        not WFC_IS_HEXADECIMAL(characters[20]) or
        not WFC_IS_HEXADECIMAL(characters[21]) or
        not WFC_IS_HEXADECIMAL(characters[22]) or
        not WFC_IS_HEXADECIMAL(characters[24]) or
        not WFC_IS_HEXADECIMAL(characters[25]) or
        not WFC_IS_HEXADECIMAL(characters[26]) or
        not WFC_IS_HEXADECIMAL(characters[27]) or
        not WFC_IS_HEXADECIMAL(characters[28]) or
        not WFC_IS_HEXADECIMAL(characters[29]) or
        not WFC_IS_HEXADECIMAL(characters[30]) or
        not WFC_IS_HEXADECIMAL(characters[31]) or
        not WFC_IS_HEXADECIMAL(characters[32]) or
        not WFC_IS_HEXADECIMAL(characters[33]) or
        not WFC_IS_HEXADECIMAL(characters[34]) or
        not WFC_IS_HEXADECIMAL(characters[35]))
    {
        return(false);
    }

    return(true);
}

_Check_return_ bool Win32FoundationClasses::wfc_is_guid( _In_ std::wstring_view characters ) noexcept
{
    //           11111111112222222222333333333
    // 012345678901234567890123456789012345678
    // 0AEE2A92-BCBB-11d0-8C72-00C04FC2B085
    if ( characters.length() < 36 )
    {
        return( false );
    }

    if (characters[8] not_eq '-' or
        characters[13] not_eq '-' or
        characters[18] not_eq '-' or
        characters[23] not_eq '-')
    {
        return(false);
    }

    if (not WFC_IS_HEXADECIMAL(characters[0]) or
        not WFC_IS_HEXADECIMAL(characters[1]) or
        not WFC_IS_HEXADECIMAL(characters[2]) or
        not WFC_IS_HEXADECIMAL(characters[3]) or
        not WFC_IS_HEXADECIMAL(characters[4]) or
        not WFC_IS_HEXADECIMAL(characters[5]) or
        not WFC_IS_HEXADECIMAL(characters[6]) or
        not WFC_IS_HEXADECIMAL(characters[7]) or
        not WFC_IS_HEXADECIMAL(characters[9]) or
        not WFC_IS_HEXADECIMAL(characters[10]) or
        not WFC_IS_HEXADECIMAL(characters[11]) or
        not WFC_IS_HEXADECIMAL(characters[12]) or
        not WFC_IS_HEXADECIMAL(characters[14]) or
        not WFC_IS_HEXADECIMAL(characters[15]) or
        not WFC_IS_HEXADECIMAL(characters[16]) or
        not WFC_IS_HEXADECIMAL(characters[17]) or
        not WFC_IS_HEXADECIMAL(characters[19]) or
        not WFC_IS_HEXADECIMAL(characters[20]) or
        not WFC_IS_HEXADECIMAL(characters[21]) or
        not WFC_IS_HEXADECIMAL(characters[22]) or
        not WFC_IS_HEXADECIMAL(characters[24]) or
        not WFC_IS_HEXADECIMAL(characters[25]) or
        not WFC_IS_HEXADECIMAL(characters[26]) or
        not WFC_IS_HEXADECIMAL(characters[27]) or
        not WFC_IS_HEXADECIMAL(characters[28]) or
        not WFC_IS_HEXADECIMAL(characters[29]) or
        not WFC_IS_HEXADECIMAL(characters[30]) or
        not WFC_IS_HEXADECIMAL(characters[31]) or
        not WFC_IS_HEXADECIMAL(characters[32]) or
        not WFC_IS_HEXADECIMAL(characters[33]) or
        not WFC_IS_HEXADECIMAL(characters[34]) or
        not WFC_IS_HEXADECIMAL(characters[35]) )
    {
        return(false);
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::wfc_is_guid_with_curlies( _In_ std::string_view characters ) noexcept
{
    //           11111111112222222222333333333
    // 012345678901234567890123456789012345678
    // {0AEE2A92-BCBB-11d0-8C72-00C04FC2B085}
    if (characters.length() < 38)
    {
        return(false);
    }

    if (characters[0] not_eq '{' or characters[37] not_eq '}')
    {
        return(false);
    }

    if (characters[9] not_eq '-' or
        characters[14] not_eq '-' or
        characters[19] not_eq '-' or
        characters[24] not_eq '-')
    {
        return(false);
    }

    if (not WFC_IS_HEXADECIMAL(characters[1]) or
        not WFC_IS_HEXADECIMAL(characters[2]) or
        not WFC_IS_HEXADECIMAL(characters[3]) or
        not WFC_IS_HEXADECIMAL(characters[4]) or
        not WFC_IS_HEXADECIMAL(characters[5]) or
        not WFC_IS_HEXADECIMAL(characters[6]) or
        not WFC_IS_HEXADECIMAL(characters[7]) or
        not WFC_IS_HEXADECIMAL(characters[8]) or
        not WFC_IS_HEXADECIMAL(characters[10]) or
        not WFC_IS_HEXADECIMAL(characters[11]) or
        not WFC_IS_HEXADECIMAL(characters[12]) or
        not WFC_IS_HEXADECIMAL(characters[13]) or
        not WFC_IS_HEXADECIMAL(characters[15]) or
        not WFC_IS_HEXADECIMAL(characters[16]) or
        not WFC_IS_HEXADECIMAL(characters[17]) or
        not WFC_IS_HEXADECIMAL(characters[18]) or
        not WFC_IS_HEXADECIMAL(characters[20]) or
        not WFC_IS_HEXADECIMAL(characters[21]) or
        not WFC_IS_HEXADECIMAL(characters[22]) or
        not WFC_IS_HEXADECIMAL(characters[23]) or
        not WFC_IS_HEXADECIMAL(characters[25]) or
        not WFC_IS_HEXADECIMAL(characters[26]) or
        not WFC_IS_HEXADECIMAL(characters[27]) or
        not WFC_IS_HEXADECIMAL(characters[28]) or
        not WFC_IS_HEXADECIMAL(characters[29]) or
        not WFC_IS_HEXADECIMAL(characters[30]) or
        not WFC_IS_HEXADECIMAL(characters[31]) or
        not WFC_IS_HEXADECIMAL(characters[32]) or
        not WFC_IS_HEXADECIMAL(characters[33]) or
        not WFC_IS_HEXADECIMAL(characters[34]) or
        not WFC_IS_HEXADECIMAL(characters[35]) or
        not WFC_IS_HEXADECIMAL(characters[36]))
    {
        return(false);
    }

    return(true);
}

_Check_return_ bool Win32FoundationClasses::wfc_is_guid_with_curlies(_In_ std::u16string_view characters) noexcept
{
    //           11111111112222222222333333333
    // 012345678901234567890123456789012345678
    // {0AEE2A92-BCBB-11d0-8C72-00C04FC2B085}
    if (characters.length() < 38)
    {
        return(false);
    }

    if (characters[0] not_eq '{' or characters[37] not_eq '}')
    {
        return(false);
    }

    if (characters[9] not_eq '-' or
        characters[14] not_eq '-' or
        characters[19] not_eq '-' or
        characters[24] not_eq '-')
    {
        return(false);
    }

    if (not WFC_IS_HEXADECIMAL(characters[1]) or
        not WFC_IS_HEXADECIMAL(characters[2]) or
        not WFC_IS_HEXADECIMAL(characters[3]) or
        not WFC_IS_HEXADECIMAL(characters[4]) or
        not WFC_IS_HEXADECIMAL(characters[5]) or
        not WFC_IS_HEXADECIMAL(characters[6]) or
        not WFC_IS_HEXADECIMAL(characters[7]) or
        not WFC_IS_HEXADECIMAL(characters[8]) or
        not WFC_IS_HEXADECIMAL(characters[10]) or
        not WFC_IS_HEXADECIMAL(characters[11]) or
        not WFC_IS_HEXADECIMAL(characters[12]) or
        not WFC_IS_HEXADECIMAL(characters[13]) or
        not WFC_IS_HEXADECIMAL(characters[15]) or
        not WFC_IS_HEXADECIMAL(characters[16]) or
        not WFC_IS_HEXADECIMAL(characters[17]) or
        not WFC_IS_HEXADECIMAL(characters[18]) or
        not WFC_IS_HEXADECIMAL(characters[20]) or
        not WFC_IS_HEXADECIMAL(characters[21]) or
        not WFC_IS_HEXADECIMAL(characters[22]) or
        not WFC_IS_HEXADECIMAL(characters[23]) or
        not WFC_IS_HEXADECIMAL(characters[25]) or
        not WFC_IS_HEXADECIMAL(characters[26]) or
        not WFC_IS_HEXADECIMAL(characters[27]) or
        not WFC_IS_HEXADECIMAL(characters[28]) or
        not WFC_IS_HEXADECIMAL(characters[29]) or
        not WFC_IS_HEXADECIMAL(characters[30]) or
        not WFC_IS_HEXADECIMAL(characters[31]) or
        not WFC_IS_HEXADECIMAL(characters[32]) or
        not WFC_IS_HEXADECIMAL(characters[33]) or
        not WFC_IS_HEXADECIMAL(characters[34]) or
        not WFC_IS_HEXADECIMAL(characters[35]) or
        not WFC_IS_HEXADECIMAL(characters[36]))
    {
        return(false);
    }

    return(true);
}

_Check_return_ bool Win32FoundationClasses::wfc_is_guid_with_curlies( _In_ std::wstring_view characters ) noexcept
{
    //           11111111112222222222333333333
    // 012345678901234567890123456789012345678
    // {0AEE2A92-BCBB-11d0-8C72-00C04FC2B085}
    if ( characters.length() < 38 )
    {
        return( false );
    }

    if (characters[0] not_eq '{' or characters[37] not_eq '}')
    {
        return(false);
    }

    if (characters[9] not_eq '-' or
        characters[14] not_eq '-' or
        characters[19] not_eq '-' or
        characters[24] not_eq '-')
    {
        return(false);
    }

    if (not WFC_IS_HEXADECIMAL(characters[1]) or
        not WFC_IS_HEXADECIMAL(characters[2]) or
        not WFC_IS_HEXADECIMAL(characters[3]) or
        not WFC_IS_HEXADECIMAL(characters[4]) or
        not WFC_IS_HEXADECIMAL(characters[5]) or
        not WFC_IS_HEXADECIMAL(characters[6]) or
        not WFC_IS_HEXADECIMAL(characters[7]) or
        not WFC_IS_HEXADECIMAL(characters[8]) or
        not WFC_IS_HEXADECIMAL(characters[10]) or
        not WFC_IS_HEXADECIMAL(characters[11]) or
        not WFC_IS_HEXADECIMAL(characters[12]) or
        not WFC_IS_HEXADECIMAL(characters[13]) or
        not WFC_IS_HEXADECIMAL(characters[15]) or
        not WFC_IS_HEXADECIMAL(characters[16]) or
        not WFC_IS_HEXADECIMAL(characters[17]) or
        not WFC_IS_HEXADECIMAL(characters[18]) or
        not WFC_IS_HEXADECIMAL(characters[20]) or
        not WFC_IS_HEXADECIMAL(characters[21]) or
        not WFC_IS_HEXADECIMAL(characters[22]) or
        not WFC_IS_HEXADECIMAL(characters[23]) or
        not WFC_IS_HEXADECIMAL(characters[25]) or
        not WFC_IS_HEXADECIMAL(characters[26]) or
        not WFC_IS_HEXADECIMAL(characters[27]) or
        not WFC_IS_HEXADECIMAL(characters[28]) or
        not WFC_IS_HEXADECIMAL(characters[29]) or
        not WFC_IS_HEXADECIMAL(characters[30]) or
        not WFC_IS_HEXADECIMAL(characters[31]) or
        not WFC_IS_HEXADECIMAL(characters[32]) or
        not WFC_IS_HEXADECIMAL(characters[33]) or
        not WFC_IS_HEXADECIMAL(characters[34]) or
        not WFC_IS_HEXADECIMAL(characters[35]) or
        not WFC_IS_HEXADECIMAL(characters[36]))
    {
        return(false);
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::wfc_is_octal( __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr or number_of_bytes < 1 )
    {
        return( false );
    }

    for ( auto const buffer_index : Range(number_of_bytes) )
    {
        if (not WFC_IS_OCTAL( buffer[ buffer_index ] ) )
        {
            return( false );
        }
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::wfc_is_wide_octal( __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr or number_of_bytes < 2 )
    {
        return( false );
    }

    auto wide_string{ reinterpret_cast<wchar_t const*>(buffer) };

    auto buffer_size{ (int)(number_of_bytes / sizeof(wchar_t)) };

    for ( auto const buffer_index : Range(buffer_size) )
    {
        if (not WFC_IS_OCTAL( wide_string[ buffer_index ] ) )
        {
            return( false );
        }
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::wfc_is_octal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr or number_of_bytes < 1 )
    {
        return( false );
    }

    int parser_state{ 0 };

    for ( auto const buffer_index : Range(number_of_bytes) )
    {
        switch( parser_state )
        {
            case 0: // digits

                if (not WFC_IS_OCTAL( buffer[ buffer_index ] ) )
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

                if ( buffer[ buffer_index ] not_eq ' ' )
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

                if ( buffer[ buffer_index ] not_eq 0x00 )
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

_Check_return_ bool Win32FoundationClasses::wfc_is_wide_octal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr or number_of_bytes < 2 )
    {
        return( false );
    }

    auto wide_string{ reinterpret_cast<wchar_t const*>(buffer) };

    auto buffer_size{ (int)(number_of_bytes / sizeof(wchar_t)) };

    int parser_state{ 0 };

    for ( auto const buffer_index : Range(buffer_size) )
    {
        switch( parser_state )
        {
            case 0: // digits

                if (not WFC_IS_OCTAL( wide_string[ buffer_index ] ) )
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

                if ( wide_string[ buffer_index ] not_eq ' ' )
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

                if ( wide_string[ buffer_index ] not_eq 0x00 )
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

_Check_return_ bool Win32FoundationClasses::wfc_is_hexadecimal( __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr or number_of_bytes < 1 )
    {
        return( false );
    }

    for ( auto const buffer_index : Range(number_of_bytes) )
    {
        if (not WFC_IS_HEXADECIMAL( buffer[ buffer_index ] ) )
        {
            return( false );
        }
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::wfc_is_wide_hexadecimal( __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr or number_of_bytes < 2 )
    {
        return( false );
    }

    auto wide_string{ reinterpret_cast<wchar_t const*>(buffer) };

    auto buffer_size{ (int)(number_of_bytes / sizeof(wchar_t)) };

    for ( auto const buffer_index : Range(buffer_size) )
    {
        if (not WFC_IS_HEXADECIMAL( wide_string[ buffer_index ] ) )
        {
            return( false );
        }
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::wfc_is_hexadecimal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr or number_of_bytes < 1 )
    {
        return( false );
    }

    int parser_state{ 0 };

    for ( auto const buffer_index : Range(number_of_bytes) )
    {
        switch( parser_state )
        {
            case 0: // digits

                if (not WFC_IS_HEXADECIMAL( buffer[ buffer_index ] ) )
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

                if ( buffer[ buffer_index ] not_eq ' ' )
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

                if ( buffer[ buffer_index ] not_eq 0x00 )
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

_Check_return_ bool Win32FoundationClasses::wfc_is_wide_hexadecimal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr or number_of_bytes < 2 )
    {
        return( false );
    }

    auto wide_string{ reinterpret_cast<wchar_t const*>(buffer) };

    auto buffer_size{ (int)(number_of_bytes / sizeof(wchar_t)) };

    int parser_state{ 0 };

    for ( auto const buffer_index : Range(buffer_size) )
    {
        switch( parser_state )
        {
            case 0: // digits

                if (not WFC_IS_HEXADECIMAL( wide_string[ buffer_index ] ) )
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

                if ( wide_string[ buffer_index ] not_eq ' ' )
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

                if ( wide_string[ buffer_index ] not_eq 0x00 )
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

_Check_return_ bool Win32FoundationClasses::wfc_is_decimal( __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr or number_of_bytes < 1 )
    {
        return( false );
    }

    for( auto const buffer_index : Range(number_of_bytes) )
    {
        if (not WFC_IS_DECIMAL( buffer[ buffer_index ] ) )
        {
            return( false );
        }
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::wfc_is_wide_decimal( __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr or number_of_bytes < 2 )
    {
        return( false );
    }

    auto wide_string{ reinterpret_cast<wchar_t const*>(buffer) };

    auto buffer_size{ (int)(number_of_bytes / sizeof(wchar_t)) };

    for ( auto const buffer_index : Range(buffer_size) )
    {
        if (not WFC_IS_DECIMAL( wide_string[ buffer_index ] ) )
        {
            return( false );
        }
    }

    return( true );
}

_Check_return_ bool Win32FoundationClasses::wfc_is_decimal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr or number_of_bytes < 1 )
    {
        return( false );
    }

    int parser_state{ 0 };

    for ( auto const buffer_index : Range(number_of_bytes) )
    {
        switch( parser_state )
        {
            case 0: // digits

                if (not WFC_IS_DECIMAL( buffer[ buffer_index ] ) )
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

                if ( buffer[ buffer_index ] not_eq ' ' )
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

                if ( buffer[ buffer_index ] not_eq 0x00 )
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

_Check_return_ bool Win32FoundationClasses::wfc_is_wide_decimal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes ) noexcept
{
    if ( buffer == nullptr or number_of_bytes < 2 )
    {
        return( false );
    }

    auto wide_string{ reinterpret_cast<wchar_t const*>(buffer) };

    auto buffer_size{ (int)(number_of_bytes / sizeof(wchar_t)) };

    int parser_state{ 0 };

    for ( auto const buffer_index : Range(buffer_size) )
    {
        switch( parser_state )
        {
            case 0: // digits

                if (not WFC_IS_DECIMAL( wide_string[ buffer_index ] ) )
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

                if ( wide_string[ buffer_index ] not_eq ' ' )
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

                if ( wide_string[ buffer_index ] not_eq 0x00 )
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

_Check_return_ int Win32FoundationClasses::wfc_find_curly_guid( __in_bcount( number_of_bytes_in_buffer ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes_in_buffer ) noexcept
{
    if ( buffer == nullptr or number_of_bytes_in_buffer < 38 )
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

_Check_return_ int Win32FoundationClasses::wfc_find_wide_curly_guid( __in_bcount( number_of_bytes_in_buffer ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes_in_buffer ) noexcept
{
    if ( buffer == nullptr or number_of_bytes_in_buffer < 76 )
    {
        return( GUID_NOT_FOUND );
    }

    for ( auto const buffer_index : Range( number_of_bytes_in_buffer - 75 ) )
    {
        if ( buffer[ buffer_index ] == '{' )
        {
            if ( Win32FoundationClasses::wfc_is_guid_with_curlies( std::wstring_view(reinterpret_cast<wchar_t const *>(&buffer[ buffer_index ]), 38 ) ) == true )
            {
                return( static_cast<int>(buffer_index) );
            }
        }
    }

    return( GUID_NOT_FOUND );
}

// End of source
