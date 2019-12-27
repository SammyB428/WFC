/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2019, Samuel R. Blackburn
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
** $Workfile: test_CBase64Coding.cpp $
** $Revision: 4 $
** $Modtime: 5/12/00 7:17p $
*/

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

_Check_return_ bool test_CBase64Coding( _Out_ std::string& class_name, _Out_ int& test_number_that_failed ) noexcept
{
    class_name.assign(STRING_VIEW("CBase64Coding"));

    test_number_that_failed = 0;

    CBase64Coding coder;

    std::vector<uint8_t> bytes_to_encode;
    std::vector<uint8_t> encoded_bytes;
    std::vector<uint8_t> decoded_bytes;

    (void) bytes_to_encode.push_back( '1' );
    (void) bytes_to_encode.push_back( '1' );
    (void) bytes_to_encode.push_back( '1' );
    (void) bytes_to_encode.push_back( '0' );

    std::wstring encoded_string;

    (void) coder.Encode( bytes_to_encode, encoded_string );

    if ( encoded_string.compare(WSTRING_VIEW(L"MTExMA==")) != I_AM_EQUAL_TO_THAT)
    {
        //WFCTRACE( TEXT( "Coder failed." ) );
        test_number_that_failed = 1;
        return( failure() );
    }

    // 2000-09-14
    // Test scenario from a bug report by Petr Stejskal [stejsky@volny.cz]

    std::wstring data_string(WSTRING_VIEW(L"SNIMAC_01_________________________________________________________KONEC"));

    std::vector<uint8_t> data;

    for ( auto const string_index : Range(data_string.length()))
    {
#if defined( WFC_STL )
        (void) data.push_back( (uint8_t) data_string[ string_index ] );
#else
        (void) data.push_back( (uint8_t) data_string.GetAt( string_index ) );
#endif
    }

    std::wstring test_encoded_data;

    CBase64Coding string_coder;

    // This should throw an exception
    (void) string_coder.Encode( data, test_encoded_data );

    CRandomNumberGenerator2 random;

    int number_of_tests = 72 * 5; // 72 characters per line

    test_number_that_failed = 3;

    for ( auto const test_index : Range(number_of_tests, StartingRangePosition(3)) )
    {
        // Prepare to test

        bytes_to_encode.clear();
        encoded_bytes.clear();
        decoded_bytes.clear();

        // Generate some test data

        for( auto const loop_index : Range(test_index) )
        {
            (void) bytes_to_encode.push_back( static_cast< uint8_t >( random.GetInteger() ) );
        }

        // Encode the test data
        (void) coder.Encode( bytes_to_encode, encoded_bytes );

        // Decode the test data
        (void) coder.Decode( encoded_bytes, decoded_bytes );

        // See if what was decoded matches the test data (what was encoded)

        if ( decoded_bytes.size() != bytes_to_encode.size() )
        {
            return( failure() );
        }

        for( auto const loop_index : Range(decoded_bytes.size()) )
        {
            if ( decoded_bytes.at( loop_index ) != bytes_to_encode.at( loop_index ) )
            {
                return( failure() );
            }
        }

        test_number_that_failed++;
    }

    for ( auto const test_index : Range(number_of_tests, StartingRangePosition(2)) )
    {
        // Prepare to test

        bytes_to_encode.clear();
        encoded_string.clear();
        decoded_bytes.clear();

        // Generate some test data

        for ( auto const loop_index : Range(test_index) )
        {
            (void) bytes_to_encode.push_back( static_cast< uint8_t >( random.GetInteger() ) );
        }

        // Encode the test data
        (void) coder.Encode( bytes_to_encode, encoded_bytes );

        // Decode the test data
        (void) coder.Decode( encoded_bytes, decoded_bytes );

        // See if what was decoded matches the test data (what was encoded)

        if ( decoded_bytes.size() != bytes_to_encode.size() )
        {
            return( failure() );
        }

        for ( auto const loop_index : Range(decoded_bytes.size()) )
        {
            if ( decoded_bytes.at( loop_index ) != bytes_to_encode.at( loop_index ) )
            {
                return( failure() );
            }
        }

        test_number_that_failed++;
    }

    uint8_t bytes[6] = { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA };

    if (wfc_parse_hex_string(STRING_VIEW("0001FFfE"), bytes, sizeof(bytes)) == false)
    {
        test_number_that_failed = 44;
        return(failure());
    }

    if (bytes[ 0 ] != 0 or
        bytes[ 1 ] != 1 or
        bytes[ 2 ] != 0xFF or
        bytes[ 3 ] != 0xFE )
    {
        test_number_that_failed = 45;
        return(failure());
    }

    bytes[0] = 0xAA;
    bytes[1] = 0x55;
    bytes[2] = 0xAA;
    bytes[3] = 0x55;
    bytes[4] = 0xAA;
    bytes[5] = 0x55;

    if (wfc_parse_hex_string(WSTRING_VIEW(L"AaBB080910"), bytes, sizeof(bytes)) == false)
    {
        test_number_that_failed = 46;
        return(failure());
    }

    if (bytes[0] != 0xAA or
        bytes[1] != 0xBB or
        bytes[2] != 8 or
        bytes[3] != 9 or
        bytes[4] != 0x10 )
    {
        test_number_that_failed = 47;
        return(failure());
    }

    // c9e55affa92a20e6d0cb546cbd262715

    CFile64 input_file;

    if (input_file.Open(WSTRING_VIEW(L"C:\\Temp\\Test.b64"), read_file_open_mode()) == true)
    {
        std::size_t const buffer_size = input_file.GetLength();

        std::unique_ptr<uint8_t[]> allocated_buffer = std::make_unique<uint8_t[]>(buffer_size);

        if (input_file.Read(allocated_buffer.get(), static_cast<uint32_t>(buffer_size)) == buffer_size)
        {
            input_file.Close();

            decoded_bytes.clear();

            if (coder.Decode(allocated_buffer.get(), buffer_size, decoded_bytes) == false)
            {
                uint8_t const * data_buffer = decoded_bytes.data();
                //_ASSERTE(FALSE);
            }
        }
        else
        {
            //_ASSERTE(FALSE);
        }
    }

    bytes_to_encode.clear();

    for ( auto const loop_index : Range(75) )
    {
        (void)bytes_to_encode.push_back(static_cast<uint8_t>(random.GetInteger()));
    }

    std::string encoded_buffer;

    coder.Encode(bytes_to_encode.data(), bytes_to_encode.size(), encoded_buffer);

    std::size_t number_of_bytes_to_allocate = CBase64Coding::DecodeReserveSize(encoded_buffer.size());
    auto decoded_buffer = std::make_unique<uint8_t[]>(number_of_bytes_to_allocate);

    ZeroMemory(decoded_buffer.get(), number_of_bytes_to_allocate);

    std::size_t number_of_bytes_decoded = coder.Decode(reinterpret_cast<uint8_t const *>(encoded_buffer.data()), encoded_buffer.size(), decoded_buffer.get(), number_of_bytes_to_allocate);

    if (memcmp(bytes_to_encode.data(), decoded_buffer.get(), bytes_to_encode.size()) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 48;
        return(failure());
    }

    encoded_buffer.clear();

    bytes_to_encode.clear();

    for ( auto const loop_index : Range(76) )
    {
        (void)bytes_to_encode.push_back(static_cast<uint8_t>(random.GetInteger()));
    }

    coder.Encode(bytes_to_encode.data(), bytes_to_encode.size(), encoded_buffer);

    number_of_bytes_to_allocate = CBase64Coding::DecodeReserveSize(encoded_buffer.size());
    decoded_buffer = std::make_unique<uint8_t[]>(number_of_bytes_to_allocate);

    ZeroMemory(decoded_buffer.get(), number_of_bytes_to_allocate);

    number_of_bytes_decoded = coder.Decode(reinterpret_cast<uint8_t const *>(encoded_buffer.data()), encoded_buffer.size(), decoded_buffer.get(), number_of_bytes_to_allocate);

    if (memcmp(bytes_to_encode.data(), decoded_buffer.get(), bytes_to_encode.size()) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 49;
        return(failure());
    }

    encoded_buffer.clear();
    bytes_to_encode.clear();

    for ( auto const loop_index : Range(77) )
    {
        (void)bytes_to_encode.push_back(static_cast<uint8_t>(random.GetInteger()));
    }

    coder.Encode(bytes_to_encode.data(), bytes_to_encode.size(), encoded_buffer);

    number_of_bytes_to_allocate = CBase64Coding::DecodeReserveSize(encoded_buffer.size());
    decoded_buffer = std::make_unique<uint8_t[]>(number_of_bytes_to_allocate);

    ZeroMemory(decoded_buffer.get(), number_of_bytes_to_allocate);

    number_of_bytes_decoded = coder.Decode(reinterpret_cast<uint8_t const *>(encoded_buffer.data()), encoded_buffer.size(), decoded_buffer.get(), number_of_bytes_to_allocate);

    if (memcmp(bytes_to_encode.data(), decoded_buffer.get(), bytes_to_encode.size()) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 50;
        return(failure());
    }

    encoded_buffer.clear();

    bytes_to_encode.clear();

    for ( auto const loop_index : Range(78) )
    {
        (void)bytes_to_encode.push_back(static_cast<uint8_t>(random.GetInteger()));
    }

    coder.Encode(bytes_to_encode.data(), bytes_to_encode.size(), encoded_buffer);

    number_of_bytes_to_allocate = CBase64Coding::DecodeReserveSize(encoded_buffer.size());
    decoded_buffer = std::make_unique<uint8_t[]>(number_of_bytes_to_allocate);

    ZeroMemory(decoded_buffer.get(), number_of_bytes_to_allocate);

    number_of_bytes_decoded = coder.Decode(reinterpret_cast<uint8_t const *>(encoded_buffer.data()), encoded_buffer.size(), decoded_buffer.get(), number_of_bytes_to_allocate);

    if (memcmp(bytes_to_encode.data(), decoded_buffer.get(), bytes_to_encode.size()) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 51;
        return(failure());
    }

    encoded_buffer.clear();

    return( true );
}
