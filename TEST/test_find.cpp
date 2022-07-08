/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2022, Samuel R. Blackburn
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

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#define new DEBUG_NEW
#endif // _DEBUG

#define TEST_FIND_BUFFER_SIZE (129 * ONE_MEGABYTE)
#define HAY (0x55)
#define NEEDLE (0x5A)

static _Check_return_ std::size_t fill_buffer_with_test_pattern(_Inout_ uint8_t * buffer, _In_ std::size_t const buffer_size, _In_ uint8_t const needle, _In_ std::size_t const test_number) noexcept
{
    std::size_t number_of_bytes_written{ 0 };

    std::size_t const frequency{ test_number + 1 };

    for ( auto const buffer_index : Range(buffer_size))
    {
        if ((buffer_index % frequency) == 0)
        {
            buffer[buffer_index] = needle;
            number_of_bytes_written++;
        }
    }

    return(number_of_bytes_written);
}

static _Check_return_ std::size_t prepare_buffer(_Inout_ uint8_t * buffer, _In_ std::size_t const buffer_size) noexcept
{
    static constexpr uint8_t const _pattern[]
    {//  0     1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31 
/*  0 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
       //  0     1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31 
/*  1 */ NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/*  2 */ HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/*  3 */ HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/*  4 */ HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/*  5 */ HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/*  6 */ HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/*  7 */ HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/*  8 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/*  9 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 10 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 11 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 12 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 13 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 14 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 15 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 16 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 17 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 18 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 19 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 20 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 21 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 22 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 23 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 24 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 25 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY,
/* 26 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY,
/* 27 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY, HAY,
/* 28 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY, HAY,
/* 29 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY, HAY,
/* 30 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY, HAY,
/* 31 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, HAY,
/* 32 */ HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE,

// Edge conditions
         //0       1    2    3    4    5    6    7    8    9   10   11   12   13   14      15      16   17   18   19   20   21   22   23   24   25   26   27   28   29   30  31 
/* 33 */ NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE, NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE,
/* 34 */ NEEDLE, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, HAY, NEEDLE,

        //   0       1       2       3       4       5       6       7       8       9      10      11      12      13      14      15      16      17      18      19      20      21      22      23      24      25      26      27      28      29      30      31 
/* 35 */ NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE, NEEDLE
    };

    static_assert(sizeof(_pattern) == (36 * 32), "pattern buffer is not a multiple of 32");

    static constexpr std::size_t const number_of_needles{ 32 + 32 + 2 + 4 };

    std::size_t return_value{ 0 };

    memset(buffer, HAY, buffer_size);

    _ASSERTE(buffer_size >= sizeof(_pattern));

    if (buffer_size < sizeof(_pattern))
    {
        return(0);
    }

    std::size_t buffer_index{ 0 };
    std::size_t chunk_index{ 0 };
    std::size_t number_of_bytes_remaining{ 0 };

    std::size_t end_of_buffer{ buffer_size };

    if ((buffer_size % sizeof(_pattern)) not_eq 0)
    {
        end_of_buffer -= (buffer_size % sizeof(_pattern));
    }

    _ASSERTE((end_of_buffer % sizeof(_pattern)) == 0);

    while (buffer_index < end_of_buffer)
    {
        _ASSERTE(buffer_size >= buffer_index);
        (void) memcpy_s(&buffer[buffer_index], buffer_size - buffer_index, _pattern, sizeof(_pattern));
        return_value += number_of_needles;

        buffer_index += sizeof(_pattern);
    }

    return(return_value);
}

static _Check_return_ std::size_t search_buffer(_Inout_ uint8_t * buffer, _In_ std::size_t const buffer_size, _In_ uint8_t const value ) noexcept
{
    std::size_t number_of_times_found{ 0 };
    std::size_t buffer_index{ 0 };
    std::size_t number_of_bytes_remaining{ buffer_size };

    uint8_t needle[1];

    needle[0] = value;

    auto found_at{ Win32FoundationClasses::find_in_memory(&buffer[buffer_index], number_of_bytes_remaining, needle, sizeof(needle)) };

    while (found_at not_eq BYTES_NOT_FOUND)
    {
        number_of_times_found++;

        buffer_index += found_at;
        buffer_index++;

        number_of_bytes_remaining = buffer_size - buffer_index;

        found_at = Win32FoundationClasses::find_in_memory(&buffer[buffer_index], number_of_bytes_remaining, needle, sizeof(needle));
    }

    return(number_of_times_found);
}

__checkReturn bool execute_test(_In_ uint8_t const * buffer, _In_ int64_t const should_be_found_at) noexcept
{
    auto const found_at{ Win32FoundationClasses::find_byte(NEEDLE, buffer, 32) };

    if (found_at not_eq should_be_found_at)
    {
        printf("Failed to find %02X at offset %" PRId64 ", it was found at offset %" PRId64 "\n", NEEDLE, should_be_found_at, found_at);

        if (buffer[should_be_found_at] == NEEDLE)
        {
            printf("The byte at offset %" PRId64 " is NEEDLE. The algorithm is flawed.\n", should_be_found_at);
        }
        else
        {
            printf("The byte at offset %" PRId64 " is %02X (not NEEDLE). The test buffer is flawed\n", should_be_found_at, (int)buffer[should_be_found_at]);
        }

        int loop_index{ 0 };

        while (loop_index < 32)
        {
            printf("%2d ", loop_index);
            loop_index++;
        }

        printf("\n");

        loop_index = 0;

        while (loop_index < 32)
        {
            printf("%02X ", (int)buffer[loop_index]);
            loop_index++;
        }

        printf("\n");

        return(false);
    }

    return(true);
}

__checkReturn bool test_find(__out std::string& class_name, __out int& test_number_that_failed) noexcept
{
    class_name.assign(STRING_VIEW("test_find"));

    uint8_t test_buffer[65];

    memset(test_buffer, 'A', sizeof(test_buffer));

    for( auto const loop_index : Range( 34, StartingRangePosition(1) ))
    {
        test_buffer[loop_index - 1] = 'B';

        if (Win32FoundationClasses::find_byte('B', test_buffer, loop_index) not_eq (loop_index - 1))
        {
            printf("Failed find_byte() test %d\n", (int)loop_index);
            test_number_that_failed = (int) loop_index;
            return(false);
        }

        test_buffer[loop_index - 1] = 'A';
    }

    auto buffer{ reinterpret_cast<uint8_t*>(_aligned_malloc(TEST_FIND_BUFFER_SIZE, 4096)) };

    memset(buffer, 0xFF, TEST_FIND_BUFFER_SIZE);

    prepare_buffer(buffer, 36 * 32);

    if (execute_test(buffer, BYTES_NOT_FOUND) == false )
    {
        test_number_that_failed = 1;
        return(failure());
    }

    if (execute_test(&buffer[ 1 * 32 ], 0) == false)
    {
        test_number_that_failed = 2;
        return(failure());
    }

    if (execute_test(&buffer[2 * 32], 1) == false)
    {
        test_number_that_failed = 3;
        return(failure());
    }

    if (execute_test(&buffer[3 * 32], 2) == false)
    {
        test_number_that_failed = 4;
        return(failure());
    }

    if (execute_test(&buffer[4 * 32], 3) == false)
    {
        test_number_that_failed = 5;
        return(failure());
    }

    if (execute_test(&buffer[5 * 32], 4) == false)
    {
        test_number_that_failed = 6;
        return(failure());
    }

    if (execute_test(&buffer[6 * 32], 5) == false)
    {
        test_number_that_failed = 7;
        return(failure());
    }

    if (execute_test(&buffer[7 * 32], 6) == false)
    {
        test_number_that_failed = 8;
        return(failure());
    }

    if (execute_test(&buffer[8 * 32], 7) == false)
    {
        test_number_that_failed = 9;
        return(failure());
    }

    if (execute_test(&buffer[9 * 32], 8) == false)
    {
        test_number_that_failed = 10;
        return(failure());
    }

    if (execute_test(&buffer[10 * 32], 9) == false)
    {
        test_number_that_failed = 11;
        return(failure());
    }

    if (execute_test(&buffer[11 * 32], 10) == false)
    {
        test_number_that_failed = 12;
        return(failure());
    }

    if (execute_test(&buffer[12 * 32], 11) == false)
    {
        test_number_that_failed = 13;
        return(failure());
    }

    if (execute_test(&buffer[13 * 32], 12) == false)
    {
        test_number_that_failed = 14;
        return(failure());
    }

    if (execute_test(&buffer[14 * 32], 13) == false)
    {
        test_number_that_failed = 15;
        return(failure());
    }

    if (execute_test(&buffer[15 * 32], 14) == false)
    {
        test_number_that_failed = 16;
        return(failure());
    }

    if (execute_test(&buffer[16 * 32], 15) == false)
    {
        test_number_that_failed = 17;
        return(failure());
    }

    if (execute_test(&buffer[17 * 32], 16) == false)
    {
        test_number_that_failed = 18;
        return(failure());
    }

    if (execute_test(&buffer[18 * 32], 17) == false)
    {
        test_number_that_failed = 19;
        return(failure());
    }

    if (execute_test(&buffer[19 * 32], 18) == false)
    {
        test_number_that_failed = 20;
        return(failure());
    }

    if (execute_test(&buffer[20 * 32], 19) == false)
    {
        test_number_that_failed = 21;
        return(failure());
    }

    if (execute_test(&buffer[21 * 32], 20) == false)
    {
        test_number_that_failed = 22;
        return(failure());
    }

    if (execute_test(&buffer[22 * 32], 21) == false)
    {
        test_number_that_failed = 23;
        return(failure());
    }

    if (execute_test(&buffer[23 * 32], 22) == false)
    {
        test_number_that_failed = 24;
        return(failure());
    }

    if (execute_test(&buffer[24 * 32], 23) == false)
    {
        test_number_that_failed = 25;
        return(failure());
    }

    if (execute_test(&buffer[25 * 32], 24) == false)
    {
        test_number_that_failed = 26;
        return(failure());
    }

    if (execute_test(&buffer[26 * 32], 25) == false)
    {
        test_number_that_failed = 27;
        return(failure());
    }

    if (execute_test(&buffer[27 * 32], 26) == false)
    {
        test_number_that_failed = 28;
        return(failure());
    }

    if (execute_test(&buffer[28 * 32], 27) == false)
    {
        test_number_that_failed = 29;
        return(failure());
    }

    if (execute_test(&buffer[29 * 32], 28) == false)
    {
        test_number_that_failed = 30;
        return(failure());
    }

    if (execute_test(&buffer[30 * 32], 29) == false)
    {
        test_number_that_failed = 31;
        return(failure());
    }

    if (execute_test(&buffer[31 * 32], 30) == false)
    {
        test_number_that_failed = 32;
        return(failure());
    }

    if (execute_test(&buffer[32 * 32], 31) == false)
    {
        test_number_that_failed = 33;
        return(failure());
    }

    std::size_t offset{ 0 };

    auto const number_of_tests{ prepare_buffer(buffer, TEST_FIND_BUFFER_SIZE) };
    auto const number_of_times_found{ search_buffer(buffer, TEST_FIND_BUFFER_SIZE, NEEDLE) };

#if 0
    // Now gather some timings from the different search methods.

    memset(buffer, 0x55, TEST_FIND_BUFFER_SIZE);

    std::size_t const needle_index = TEST_FIND_BUFFER_SIZE - 3;

    buffer[needle_index] = 0xAA;

    std::size_t const desired_needle_index = needle_index - 1;

    LARGE_INTEGER start_1 { 0, 0 };
    LARGE_INTEGER end_1 { 0, 0 };
    LARGE_INTEGER end_2 { 0, 0 };

    QueryPerformanceCounter(&start_1);
    std::size_t const found_1{ _find_byte_SSE41_1(0xAA, &buffer[1], TEST_FIND_BUFFER_SIZE - 1) };
    QueryPerformanceCounter(&end_1);
    std::size_t const found_2{ _find_byte_SSE41_2(0xAA, &buffer[1], TEST_FIND_BUFFER_SIZE - 1) };
    QueryPerformanceCounter(&end_2);

    if (found_1 not_eq desired_needle_index)
    {
        _ASSERT_EXPR(FALSE, _CRT_WIDE("found_1 is not the desired index"));
    }

    if (found_1 not_eq found_2)
    {
        _ASSERT_EXPR(FALSE, _CRT_WIDE("found_1 is not equal to found_2"));
    }

    int64_t const time_1{ end_1.QuadPart - start_1.QuadPart };
    int64_t const time_2{ end_2.QuadPart - end_1.QuadPart };

    //printf("Method 1: %" PRId64 "\nMethod 2: %" PRId64 "\n", time_1, time_2);

    if (time_2 < time_1)
    {
        int64_t const beating_ticks{ time_1 - time_2 };
        //printf("_find_byte_SSE41_2 wins by %" PRId64 " ticks\n", beating_ticks);
    }
    else
    {
        int64_t const beating_ticks{ time_2 - time_1 };
        //printf("_find_byte_SSE41 wins by %" PRId64 " ticks\n", beating_ticks);
    }

#endif

    if (number_of_times_found not_eq number_of_tests)
    {
        test_number_that_failed = 34;
        _aligned_free(buffer);
        return(failure());
    }

    // Now test find all...

    memset(buffer, 0xFF, TEST_FIND_BUFFER_SIZE);

    buffer[1] = 'S';
    buffer[2] = 'a';
    buffer[3] = 'm';
    buffer[4] = 'B';
    buffer[5] = 'S';
    buffer[6] = 'a';
    buffer[7] = 'm';
    buffer[8] = 'B';

    buffer[TEST_FIND_BUFFER_SIZE - 1] = 'B'; // Last byte of the buffer is the last byte of our pattern
    buffer[TEST_FIND_BUFFER_SIZE - 2] = 'm';
    buffer[TEST_FIND_BUFFER_SIZE - 3] = 'a';
    buffer[TEST_FIND_BUFFER_SIZE - 4] = 'S';

    std::vector<uint64_t> results;

    constexpr uint8_t const sam_needle[4]{ 'S', 'a', 'm', 'B' };

    Win32FoundationClasses::find_all_in_memory(buffer, TEST_FIND_BUFFER_SIZE, sam_needle, sizeof(sam_needle), results);

    if (results.size() not_eq 3)
    {
        test_number_that_failed = 35;
        _aligned_free(buffer);
        return(failure());
    }

    if (results[0] not_eq 1)
    {
        test_number_that_failed = 36;
        _aligned_free(buffer);
        return(failure());
    }

    if (results[1] not_eq 5)
    {
        test_number_that_failed = 37;
        _aligned_free(buffer);
        return(failure());
    }

    if (results[2] not_eq (TEST_FIND_BUFFER_SIZE - 4))
    {
        test_number_that_failed = 38;
        _aligned_free(buffer);
        return(failure());
    }

    _aligned_free(buffer);
    buffer = nullptr;

    test_number_that_failed = 38;
    return(true);
}
