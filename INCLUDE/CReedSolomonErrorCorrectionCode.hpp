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
** $Workfile: CReedSolomonErrorCorrectionCode.hpp $
** $Revision: 7 $
** $Modtime: 6/26/01 11:04a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( REED_SOLOMON_ERROR_CORRECTION_CODE_CLASS_HEADER )

#define REED_SOLOMON_ERROR_CORRECTION_CODE_CLASS_HEADER

/*
** This class is derived from source code found at
** http://hideki.iis.u-tokyo.ac.jp/~robert/codes.html
**
** Specifically, the work done by Phil Karn KA9Q, September 1996
*/

/* Global definitions for Reed-Solomon encoder/decoder
 * Phil Karn KA9Q, September 1996
 *
 * The parameters m_NumberOfBitsPerSymbol and m_NumberOfSymbolsPerBlock specify the Reed-Solomon code parameters.
 *
 * Set m_NumberOfBitsPerSymbol to be the size of each code symbol in bits. The Reed-Solomon
 * block size will then be m_BlockSize = 2**M - 1 symbols. Supported values are
 * defined in rs.c.
 *
 * Set m_NumberOfSymbolsPerBlock to be the number of data symbols in each block, which must be
 * less than the block size. The code will then be able to correct up
 * to m_BlockSize-m_NumberOfSymbolsPerBlock erasures or (m_BlockSize-m_NumberOfSymbolsPerBlock)/2 errors, or combinations thereof with
 * each error counting as two erasures.
 */

class CReedSolomonErrorCorrectionCode
{
   protected:

      static constexpr int const m_NumberOfBitsPerSymbol = 8; // 8 bits per symbol
      static constexpr int const m_BlockSize = 255; // 255

      int m_NumberOfSymbolsPerBlock{ 0 };
      std::unique_ptr<int[]> m_Alpha_to; // array of integers of size (m_BlockSize + 1) elements
      std::unique_ptr<int[]> m_GeneratorPolynomial; // array of integers size (m_BlockSize - m_NumberOfSymbolsPerBlock + 1) elements
      std::unique_ptr<int[]> m_Index_of; // array of integers of size (m_BlockSize + 1) elements

      int m_PrimitivePolynomials[9]{ 0 }; // Array of integers

      virtual _Check_return_ bool m_EncodeChunk( __in std::vector<uint8_t> const& input, __out std::vector<uint8_t>& parity ) noexcept;
      virtual _Check_return_ int  m_DecodeChunk( __inout std::vector<uint8_t>& chunk, __out std::vector<int>& error_polynomials, __in int no_eras = 1 ) noexcept;
      virtual void m_GenerateGaloisField( void ) noexcept;
      virtual void m_GeneratePolynomial( void ) noexcept;
      virtual void m_Initialize( _In_ int const number_of_symbols_per_block ) noexcept;
      inline  _Check_return_ int m_Mod( __in int x ) noexcept;

   public:

      CReedSolomonErrorCorrectionCode();
      virtual ~CReedSolomonErrorCorrectionCode();

      virtual _Check_return_ SSIZE_T Decode( __in std::vector<uint8_t> const& encoded_data, __out std::vector<uint8_t>& decoded_data ) noexcept;
      virtual _Check_return_ bool    Encode( __in std::vector<uint8_t> const& data, __out std::vector<uint8_t>& encoded_data ) noexcept;
};

#endif // REED_SOLOMON_ERROR_CORRECTION_CODE_CLASS_HEADER
