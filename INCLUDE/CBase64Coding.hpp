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
** $Workfile: CBase64Coding.hpp $
** $Revision: 5 $
** $Modtime: 6/26/01 11:00a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( BASE_64_CODING_CLASS_HEADER )

#define BASE_64_CODING_CLASS_HEADER

#define END_OF_BASE64_ENCODED_DATA           ('=')
#define BASE64_END_OF_BUFFER                 (0xFD)
#define BASE64_IGNORABLE_CHARACTER           (0xFE)
#define BASE64_UNKNOWN_VALUE                 (0xFF)
#define BASE64_NUMBER_OF_CHARACTERS_PER_LINE (72)

class CBase64Coding
{
   protected:

      uint8_t m_DecoderTable[ 256 ]{ 0 };

      void m_InitializeDecoderTable( void ) noexcept;

   public:

       CBase64Coding(_In_ CBase64Coding const&) = delete;
       _Check_return_ CBase64Coding& operator=(_In_ CBase64Coding const&) = delete;

       // Construction

      CBase64Coding() noexcept;

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CBase64Coding();

      static inline constexpr _Check_return_ std::size_t DecodeReserveSize(_In_ std::size_t const number_of_encoded_bytes) noexcept
      {
          return(static_cast<std::size_t>(static_cast<double>(number_of_encoded_bytes) * static_cast<double>(0.75)) + 1);
      }

      static inline constexpr _Check_return_ std::size_t EncodeReserveSize(_In_ std::size_t const number_of_bytes_to_encode) noexcept
      {
          std::size_t return_value = static_cast<std::size_t>(static_cast<double>(number_of_bytes_to_encode) / static_cast<double>(0.75)) + 1;

          return_value += ((number_of_bytes_to_encode % 3) + 1);

          // Now add in the CR/LF pairs, each line is truncated at 72 characters
          std::size_t const number_of_lines_of_output = (return_value / BASE64_NUMBER_OF_CHARACTERS_PER_LINE) + 1;

          return_value += (number_of_lines_of_output * 2); // For the carriage return and line feed after each line

          return(return_value);
      }

      _Check_return_ std::size_t Decode(_In_reads_bytes_(number_of_bytes) uint8_t const * source, _In_ std::size_t const number_of_bytes, _Inout_ uint8_t * destination, _In_ std::size_t destination_size) const noexcept;
      _Check_return_ bool Decode(_In_reads_bytes_(number_of_bytes) uint8_t const * source, _In_ std::size_t const number_of_bytes, _Inout_ std::vector<uint8_t>& destination) const noexcept;
      _Check_return_ bool Decode(_In_ std::vector<uint8_t> const& source, _Inout_ std::vector<uint8_t>& destination ) const noexcept;
      _Check_return_ bool Decode(_In_ std::wstring_view source, _Inout_ std::vector<uint8_t>& destination ) const noexcept;
      _Check_return_ bool Encode(_In_ std::vector<uint8_t> const& source, _Inout_ std::vector<uint8_t>& destination ) const noexcept;
      _Check_return_ bool Encode(_In_ std::vector<uint8_t> const& source, _Inout_ std::wstring& destination ) const noexcept;
      _Check_return_ bool Encode(_In_reads_bytes_(number_of_bytes) uint8_t const * source, _In_ std::size_t const number_of_bytes, _Inout_ std::wstring& destination ) const noexcept;
      _Check_return_ bool Encode(_In_reads_bytes_(number_of_bytes) uint8_t const * source, _In_ std::size_t const number_of_bytes, _Inout_ std::string& destination ) const noexcept;
      _Check_return_ bool Encode(_Inout_ HANDLE input_file, _In_ std::size_t const number_of_bytes_to_encode, _Inout_ HANDLE output_file_handle) const noexcept;
};

#endif // BASE_64_CODING_CLASS_HEADER
