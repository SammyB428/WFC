/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2018, Samuel R. Blackburn
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
** $Workfile: CUUCoding.cpp $
** $Revision: 11 $
** $Modtime: 9/19/01 4:05a $
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

#define UU_DECODE_CHARACTER( _c ) static_cast< BYTE >( ( (_c) - 0x20 ) bitand 0x3F )
#define UU_ENCODE_BYTE(      _c ) static_cast< BYTE >( (_c) ? ( (_c) bitand 0x3F ) + 0x20 : '`' )

Win32FoundationClasses::CUUCoding::CUUCoding() noexcept
{
    WFC_VALIDATE_POINTER( this );
}

_Check_return_ bool Win32FoundationClasses::CUUCoding::OutputFilename(_In_ std::vector<uint8_t> const& source, _Out_ std::wstring& filename) const noexcept
{
    filename.clear();

    std::size_t index = 0;
    auto const number_of_bytes_to_decode = source.size();

    // Since we're decoding, we are most likely in a performance-minded
    // part of an application, let's go for a speedy method for accessing
    // the source data.

    auto input_buffer = source.data();

    while (index < number_of_bytes_to_decode)
    {
        // Skip space characters...

        while (index < number_of_bytes_to_decode and
            (input_buffer[index] == CARRIAGE_RETURN or
                input_buffer[index] == LINE_FEED or
                input_buffer[index] == ' ' or
                input_buffer[index] == 0x09))
        {
            index++;
        }

        if ((number_of_bytes_to_decode - index) > 5)
        {
            if ((input_buffer[index] == 'B' or input_buffer[index] == 'b') and
                (input_buffer[index + 1] == 'E' or input_buffer[index + 1] == 'e') and
                (input_buffer[index + 2] == 'G' or input_buffer[index + 2] == 'g') and
                (input_buffer[index + 3] == 'I' or input_buffer[index + 3] == 'i') and
                (input_buffer[index + 4] == 'N' or input_buffer[index + 4] == 'n'))
            {
                index += 5;

                // Skip spaces to get to the mode number
                while (index < number_of_bytes_to_decode and
                    (input_buffer[index] == ' ' or input_buffer[index] == 0x09))
                {
                    index++;
                }

                // Now skip the mode numbers until we get to another space
                while (index < number_of_bytes_to_decode and
                    (input_buffer[index] not_eq ' ' and input_buffer[index] not_eq 0x09))
                {
                    index++;
                }

                // Now skip the spaces between the mode and the filename
                while (index < number_of_bytes_to_decode and
                    (input_buffer[index] == ' ' or input_buffer[index] == 0x09))
                {
                    index++;
                }

                // FINALLY! We are at the filename
                while (index < number_of_bytes_to_decode and
                    (input_buffer[index] not_eq CARRIAGE_RETURN and input_buffer[index] not_eq LINE_FEED))
                {
                    filename.push_back(input_buffer[index]);
                    index++;
                }

                return(true);
            }
        }
        else
        {
            return(false);
        }

        index++;
    }

    return(true);
}

_Check_return_ bool Win32FoundationClasses::CUUCoding::Decode(_In_ std::vector<uint8_t> const& source, _Out_ std::vector<uint8_t>& destination) const noexcept
{
    WFC_VALIDATE_POINTER(this);

    // Start with an empty destination

    destination.clear();

    uint8_t byte_to_add = 0;
    uint8_t character_1 = 0;
    uint8_t character_2 = 0;
    uint8_t character_3 = 0;
    uint8_t character_4 = 0;

    std::size_t index = 0;
    std::size_t number_of_characters_in_this_line = 0;
    std::size_t line_index = 0;

    auto number_of_bytes_to_decode = source.size();

    // Since we're decoding, we are most likely in a performance-minded
    // part of an application, let's go for a speedy method for accessing
    // the source data.

    auto input_buffer = source.data();

    while (index < number_of_bytes_to_decode)
    {
        // Skip space characters...

        while (index < number_of_bytes_to_decode and
            (input_buffer[index] == CARRIAGE_RETURN or
                input_buffer[index] == LINE_FEED or
                input_buffer[index] == ' ' or
                input_buffer[index] == 0x09))
        {
            index++;
        }

        if ((number_of_bytes_to_decode - index) > 5)
        {
            if ((input_buffer[index] == 'B' or input_buffer[index] == 'b') and
                (input_buffer[index + 1] == 'E' or input_buffer[index + 1] == 'e') and
                (input_buffer[index + 2] == 'G' or input_buffer[index + 2] == 'g') and
                (input_buffer[index + 3] == 'I' or input_buffer[index + 3] == 'i') and
                (input_buffer[index + 4] == 'N' or input_buffer[index + 4] == 'n'))
            {
                index += 5;

                while (index < number_of_bytes_to_decode and
                    (input_buffer[index] not_eq CARRIAGE_RETURN and
                        input_buffer[index] not_eq LINE_FEED))
                {
                    index++;
                }
            }
        }

        // Skip space characters...

        while (index < number_of_bytes_to_decode and
            (input_buffer[index] == CARRIAGE_RETURN or
                input_buffer[index] == LINE_FEED or
                input_buffer[index] == ' ' or
                input_buffer[index] == 0x09))
        {
            index++;
        }

        if (index >= number_of_bytes_to_decode)
        {
            return(true);
        }

        number_of_characters_in_this_line = UU_DECODE_CHARACTER(input_buffer[index]);

        if (number_of_characters_in_this_line == 0)
        {
            return(true);
        }

        // See if we're at the "end"s

        if ((index + 3) < number_of_bytes_to_decode)
        {
            if ((input_buffer[index] == 'e' or input_buffer[index] == 'E') and
                (input_buffer[index + 1] == 'n' or input_buffer[index + 1] == 'N') and
                (input_buffer[index + 2] == 'd' or input_buffer[index + 2] == 'D'))
            {
                return(true);
            }
        }

        index++;

        line_index = 0;

        while (line_index < number_of_characters_in_this_line)
        {
            if (index < number_of_bytes_to_decode)
            {
                character_1 = UU_DECODE_CHARACTER(input_buffer[index]);
                index++;
            }
            else
            {
                character_1 = 0xFF; // Invalid character
            }

            if (index < number_of_bytes_to_decode)
            {
                character_2 = UU_DECODE_CHARACTER(input_buffer[index]);
                index++;
            }
            else
            {
                character_2 = 0xFF; // Invalid character
            }

            if (index < number_of_bytes_to_decode)
            {
                character_3 = UU_DECODE_CHARACTER(input_buffer[index]);
                index++;
            }
            else
            {
                character_3 = 0xFF; // Invalid character
            }

            if (index < number_of_bytes_to_decode)
            {
                character_4 = UU_DECODE_CHARACTER(input_buffer[index]);
                index++;
            }
            else
            {
                character_4 = 0xFF; // Invalid character
            }

            // Whew! If we get here it means we can translate the 4 characters
            // to three bytes.

            line_index++;

            if (line_index <= number_of_characters_in_this_line)
            {
                byte_to_add = static_cast<uint8_t>((character_1 << 2) bitor (character_2 >> 4));
                destination.push_back(byte_to_add);
            }

            line_index++;

            if (line_index <= number_of_characters_in_this_line)
            {
                byte_to_add = static_cast<uint8_t>((character_2 << 4) bitor (character_3 >> 2));
                destination.push_back(byte_to_add);
            }

            line_index++;

            if (line_index <= number_of_characters_in_this_line)
            {
                byte_to_add = static_cast<uint8_t>((character_3 << 6) bitor (character_4));
                (void)destination.push_back(byte_to_add);
            }
        }
    }

    return(false);
}

_Check_return_ bool Win32FoundationClasses::CUUCoding::Decode( _In_ std::wstring const& source, _Out_ std::vector<uint8_t>& destination ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    // Start with an empty destination

    bool return_value = false;

    std::size_t const number_of_bytes_to_decode = source.length();

    // Since we're decoding, we are most likely in a performance-minded
    // part of an application, let's go for a speedy method for accessing
    // the source data.

    auto input_buffer = source.c_str();

    std::vector<uint8_t> bytes_to_decode;

    bytes_to_decode.resize( number_of_bytes_to_decode );

    auto buffer = bytes_to_decode.data();

    WFC_TRY
    {
        for ( auto const loop_index : Range(number_of_bytes_to_decode) )
        {
            buffer[ loop_index ] = static_cast< uint8_t >( input_buffer[ loop_index ] );
        }

        return_value = Decode( bytes_to_decode, destination );
    }
    WFC_CATCH_ALL
    {
        return( false );
    }
    WFC_END_CATCH_ALL

    return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CUUCoding::Encode( _In_ std::vector<uint8_t> const& source, _Out_ std::vector<uint8_t>& destination ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    // Always start with an empty destination
    destination.clear();

    // Output bytes 45 at a time

    auto const number_of_bytes_to_encode = source.size();

    std::size_t index      = 0;
    std::size_t line_index = 0;

    uint8_t byte_to_encode_1 = 0;
    uint8_t byte_to_encode_2 = 0;
    uint8_t byte_to_encode_3 = 0;

    uint8_t character_to_output_1 = 0;
    uint8_t character_to_output_2 = 0;
    uint8_t character_to_output_3 = 0;
    uint8_t character_to_output_4 = 0;

    auto input_buffer = source.data();

    while( index < number_of_bytes_to_encode ) // Cannot be converted to a Range loop
    {
        line_index = 0;

        if ( index > 0 and ( ( index % 44 ) == 0 ) )
        {
            //WFCTRACE( TEXT( "TROUBLE!" ) );
        }

        if ( ( number_of_bytes_to_encode - index ) >= 45 )
        {
            // Output the number of bytes in this line
            destination.push_back( UU_ENCODE_BYTE( 45 ) );

            while( line_index < 45 ) // Cannot be converted to a Range loop
            {
                byte_to_encode_1 = input_buffer[ index ];
                line_index++;
                index++;

                byte_to_encode_2 = input_buffer[ index ];
                line_index++;
                index++;

                byte_to_encode_3 = input_buffer[ index ];
                line_index++;
                index++;

                character_to_output_1 = static_cast<uint8_t>( byte_to_encode_1 >> 2 );
                character_to_output_2 = static_cast<uint8_t>( ( ( byte_to_encode_1 << 4 ) bitand 0x30 ) bitor ( ( byte_to_encode_2 >> 4 ) bitand 0x0F ) );
                character_to_output_3 = static_cast<uint8_t>( ( ( byte_to_encode_2 << 2 ) bitand 0x3C ) bitor ( ( byte_to_encode_3 >> 6 ) bitand 0x03 ) );
                character_to_output_4 = static_cast<uint8_t>( byte_to_encode_3 bitand 0x3F );

                // These lines are here for easy debugging

                character_to_output_1 = UU_ENCODE_BYTE( character_to_output_1 );
                character_to_output_2 = UU_ENCODE_BYTE( character_to_output_2 );
                character_to_output_3 = UU_ENCODE_BYTE( character_to_output_3 );
                character_to_output_4 = UU_ENCODE_BYTE( character_to_output_4 );

                destination.push_back( character_to_output_1 );
                destination.push_back( character_to_output_2 );
                destination.push_back( character_to_output_3 );
                destination.push_back( character_to_output_4 );
            }
        }
        else
        {
            // Output the number of bytes in this line
            destination.push_back( UU_ENCODE_BYTE( number_of_bytes_to_encode - index ) );

            while( index < number_of_bytes_to_encode ) // Cannot be converted to a Range loop
            {
                if ( index < number_of_bytes_to_encode )
                {
                    byte_to_encode_1 = input_buffer[ index ];
                }
                else
                {
                    byte_to_encode_1 = 0x00;
                }

                index++;

                if ( index < number_of_bytes_to_encode )
                {
                    byte_to_encode_2 = input_buffer[ index ];
                }
                else
                {
                    byte_to_encode_2 = 0x00;
                }

                index++;

                if ( index < number_of_bytes_to_encode )
                {
                    byte_to_encode_3 = input_buffer[ index ];
                }
                else
                {
                    byte_to_encode_3 = 0x00;
                }

                index++;

                character_to_output_1 = static_cast<uint8_t>( byte_to_encode_1 >> 2 );
                character_to_output_2 = static_cast<uint8_t>( ( ( byte_to_encode_1 << 4 ) bitand 0x30 ) bitor ( ( byte_to_encode_2 >> 4 ) bitand 0x0F ) );
                character_to_output_3 = static_cast<uint8_t>( ( ( byte_to_encode_2 << 2 ) bitand 0x3C ) bitor ( ( byte_to_encode_3 >> 6 ) bitand 0x03 ) );
                character_to_output_4 = static_cast<uint8_t>( byte_to_encode_3 bitand 0x3F );

                destination.push_back( UU_ENCODE_BYTE( character_to_output_1 ) );
                destination.push_back( UU_ENCODE_BYTE( character_to_output_2 ) );
                destination.push_back( UU_ENCODE_BYTE( character_to_output_3 ) );
                destination.push_back( UU_ENCODE_BYTE( character_to_output_4 ) );
            }
        }

        destination.push_back( CARRIAGE_RETURN );
    }

    return( true );
}


#if 0
_Check_return_ bool CUUCoding::Encode( _In_ std::vector<uint8_t> const& source, _Out_ std::wstring& destination ) const noexcept
{
    WFC_VALIDATE_POINTER( this );

    destination.clear();

    std::vector<uint8_t> encoded_bytes;

    bool return_value = Encode( source, encoded_bytes );

    if ( return_value == false )
    {
        return( false );
    }

    // We have to do this the slow way to preserve UNICODE...

    std::size_t loop_index           = 0;
    std::size_t const number_of_characters = encoded_bytes.GetSize();

    BYTE const * encoded_buffer = encoded_bytes.GetData();

    while( loop_index < number_of_characters )
    {
        destination.Append( static_cast<wchar_t>( encoded_buffer[ loop_index ] ) );
        loop_index++;
    }

    return( return_value );
}
#endif

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CUUCoding</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, UUENCODE, UUDECODE, source code">
<META name="description" content="This C++ class let's you encode and decode bytes to text using Unix-to-Unix (UU) coding.">
</HEAD>

<BODY>

<H1>CUUCoding</H1>
$Revision: 11 $<BR><HR>

<H2>Description</H2>
This class gives you the ability to encode/decode data using Unix-to-Unix (UU).

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CUUCoding</B>()<DD>
Constructs this object.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="Decode">Decode</A></B>( const std::vector&lt;uint8_t&gt;&amp; source, std::vector&lt;uint8_t&gt;&amp; destination )
BOOL <B>Decode</B>( const std::wstring&amp;    source, std::vector&lt;uint8_t&gt;&amp; destination )</PRE><DD>
This method takes UU encoded text and produces the bytes. It decodes
the Unix-to-Unix encoding.

<DT><PRE>BOOL <B><A NAME="Encode">Encode</A></B>( const std::vector&lt;uint8_t&gt;&amp; source, std::vector&lt;uint8_t&gt;&amp; destination )
BOOL <B>Encode</B>( const std::vector&lt;uint8_t&gt;&amp; source, std::wstring&amp;    destination )</PRE><DD>
This method takes bytes and turns them into UU text.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
<A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

std::vector&lt;uint8_t&gt; bytes;

get_file_contents( command_line_arguments[ 0 ], bytes );

<B>CUUCoding</B> encoder;

std::wstring encoded_data;

if ( encoder.Encode( bytes, encoded_data ) not_eq FALSE )
{
_tprintf( TEXT( &quot;%s\n&quot;, (LPCTSTR) encoded_data );
}
}</CODE></PRE>

<HR><I>Copyright, 1995-2002, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CUUCoding.cpp $<BR>
$Modtime: 9/19/01 4:05a $
</BODY>

</HTML>
*/
