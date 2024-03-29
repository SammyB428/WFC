/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2016, Samuel R. Blackburn
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
** $Workfile: test_CReedSolomonErrorCorrectionCode.cpp $
** $Revision: 3 $
** $Modtime: 8/23/99 8:13p $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#define new DEBUG_NEW
#endif // _DEBUG

_Check_return_ bool test_CReedSolomonErrorCorrectionCode( __out std::string& class_name, __out int& test_number_that_failed ) noexcept
{
    class_name.assign(STRING_VIEW("CReedSolomonErrorCorrectionCode"));

    test_number_that_failed = 1;

    Win32FoundationClasses::CReedSolomonErrorCorrectionCode transmitter;

    Win32FoundationClasses::CRandomNumberGenerator2 random_number; // 1853265048

    int test_set_size{ 4096 };

    std::vector<uint8_t> data;

    data.resize( test_set_size ); // for speed

    for ( auto const loop_index : Range(test_set_size) )
    {
        data.push_back( static_cast< BYTE >( random_number.GetInteger() ) );
    }

    std::vector<uint8_t> encoded_data;

    if ( transmitter.Encode( data, encoded_data ) == FALSE )
    {
        test_number_that_failed = 1;
        return( failure() );
    }

    // We have encoded the data. Time to transmit it.

    // Here's our sloppy communications path. It adds errors to the data
    // Reed-Solomon is able to withstand one-bit-per-symbol errors.

    auto number_of_errors_to_introduce{ encoded_data.size() };

    int error_number{ 0 };
    int bit_number_to_smash{ 0 };
    int number_of_errors_introduced{ 0 };

    DWORD value{ 0 };

    while( error_number < number_of_errors_to_introduce ) // Cannot be converted to a Range loop
    {
        value = encoded_data.at( error_number );

        bit_number_to_smash = random_number.GetInteger() % 8;

        if ( _bittest( reinterpret_cast<LONG const *>(&value), bit_number_to_smash ) == 0 )
        {
            _bittestandset(reinterpret_cast<LONG *>(&value), bit_number_to_smash );
        }
        else
        {
            _bittestandreset(reinterpret_cast<LONG *>(&value), bit_number_to_smash );
        }

        ASSERT( value not_eq encoded_data.at( error_number ) );

        encoded_data.at( error_number ) = (uint8_t) value;
        number_of_errors_introduced++;

        error_number += ( ( ( random_number.GetInteger() % 8 ) + 1 ) + 16 );
    }

    //WFCTRACEVAL( TEXT( "Whacked this many bits " ), number_of_errors_introduced );

    // We would now transmit data to the receiver

    Win32FoundationClasses::CReedSolomonErrorCorrectionCode receiver;

    std::vector<uint8_t> decoded_data;

    auto number_of_errors_corrected{ receiver.Decode(encoded_data, decoded_data) };

    if ( number_of_errors_corrected not_eq (-1) )
    {
        // SUCCESS!
        return( true );
    }

    //WFCTRACEVAL( TEXT( "Number of errors corrected " ), number_of_errors_corrected );
    //WFCTRACEVAL( TEXT( "Number of bytes in decoded data " ), decoded_data.GetSize() );

    if ( data.size() not_eq decoded_data.size() )
    {
        //WFCTRACE( TEXT( "FAILED length test" ) );
        //WFCTRACEVAL( TEXT( "Original length was " ), data.GetSize() );
        //WFCTRACEVAL( TEXT( "Decoded length was " ), decoded_data.GetSize() );
    }

    for ( auto const loop_index : Range(decoded_data.size()) )
    {
        if ( data.at( loop_index ) not_eq decoded_data.at( loop_index ) )
        {
            //WFCTRACEVAL( TEXT( "Comparison failed at byte " ), loop_index );
        }
    }

    return( failure() );
}

