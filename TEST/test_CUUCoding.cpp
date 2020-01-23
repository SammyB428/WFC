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
** $Workfile: test_CUUCoding.cpp $
** $Revision: 1 $
** $Modtime: 6/22/98 9:23p $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

__checkReturn bool test_CUUCoding( __out std::string& class_name, __out int& test_number_that_failed ) noexcept
{
    class_name.assign(STRING_VIEW("CUUCoding"));

    std::vector<uint8_t> bytes_to_encode;

    // Generate some test data

    for ( auto const index : Range(1024) )
    {
        (void) bytes_to_encode.push_back( static_cast< uint8_t >( index ) );
    }

    std::vector<uint8_t> encoded_bytes;

    CUUCoding coder;

    // Encode the test data
    if ( coder.Encode( bytes_to_encode, encoded_bytes ) == false )
    {
        test_number_that_failed = 1;
        return(failure());
    }

#if 0
    // Save the encoded bytes off for easy viewing

    CFile64 output_file;

    if ( output_file.Open( TEXT( "Output.uue" ), CFile64::modeCreate bitor CFile64::modeWrite ) != false )
    {
        output_file.Write( encoded_bytes.GetData(), encoded_bytes.GetSize() );
        output_file.Close();
    }

    if ( output_file.Open( TEXT( "Input.bin" ), CFile64::modeCreate bitor CFile64::modeWrite ) != false )
    {
        output_file.Write( bytes_to_encode.GetData(), bytes_to_encode.GetSize() );
        output_file.Close();
    }
#endif

    std::vector<uint8_t> decoded_bytes;

    // Decode the test data
    if ( coder.Decode( encoded_bytes, decoded_bytes ) == false )
    {
        test_number_that_failed = 2;
        return( failure() );
    }

    // See if what was decoded matches the test data (what was encoded)

    std::size_t number_of_decoded_bytes = decoded_bytes.size();

    if ( number_of_decoded_bytes != bytes_to_encode.size() )
    {
        //WFCTRACE( TEXT( "FAILED!!! What was decoded is a different size than what was encoded!" ) );
        //WFCTRACEVAL( TEXT( "Number of bytes decoded is " ), number_of_decoded_bytes );
        //WFCTRACEVAL( TEXT( "Number of bytes encoded is " ), bytes_to_encode.GetSize() );

        test_number_that_failed = 3;
        return( failure() );
    }

    for ( auto const index : Range(number_of_decoded_bytes) )
    {
        if ( decoded_bytes.at( index ) != bytes_to_encode.at( index ) )
        {
            //WFCTRACEVAL( TEXT( "FAILED! Comparison failed at index " ), index );
            //WFCTRACEVAL( TEXT( "This was decoded " ), decoded_bytes.GetAt( index ) );
            //WFCTRACEVAL( TEXT( "This was encoded " ), bytes_to_encode.GetAt( index ) );
            test_number_that_failed = 4;
            return(failure());
        }
    }

    test_number_that_failed = 4;
    return( true );
}
