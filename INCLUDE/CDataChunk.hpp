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
** $Workfile: CDataChunk.hpp $
** $Revision: 10 $
** $Modtime: 6/26/01 11:01a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( DATA_CHUNK_CLASS_HEADER )

#define DATA_CHUNK_CLASS_HEADER

inline constexpr [[nodiscard]] _Check_return_ uint32_t MAKE_DATA_ID(uint8_t const byte_1, uint8_t const byte_2, uint8_t const byte_3, uint8_t const byte_4) noexcept
{
    return((uint32_t)(byte_1) bitor ((uint32_t)(byte_2) << 8) bitor ((uint32_t)(byte_3) << 16) bitor ((uint32_t)(byte_4) << 24));
}

#define MAKE_DATA_DWORD( byte_1, byte_2, byte_3, byte_4 ) MAKE_DATA_ID( byte_1, byte_2, byte_3, byte_4 )

#define MAKE_DATA_LENGTH( byte_1, byte_2, byte_3, byte_4 ) MAKE_DATA_ID( byte_1, byte_2, byte_3, byte_4 )

class CDataChunk
{
   public:

      uint32_t Identifier{ 0 };
      std::vector<uint8_t> Data;

      inline CDataChunk() noexcept
      {
         Identifier = 0;
      };

      inline explicit CDataChunk( _In_ CDataChunk const& source ) noexcept
      {
         Copy( source );
      };

      static void GetIdentifier( _In_ uint32_t const identifier, _Out_ std::wstring& string ) noexcept;

      inline void Copy( _In_ CDataChunk const& source ) noexcept
      {
         Identifier = source.Identifier;
         Data = source.Data;
      };

      inline void Copy( _In_ uint32_t const identifier, _In_ std::vector<uint8_t> const& source ) noexcept
      {
         Identifier = identifier;
         Data = source;
      };

      inline void Empty( void ) noexcept
      {
         Identifier = 0;
         Data.clear();
      };

      inline _Check_return_ CDataChunk& operator=( _In_ CDataChunk const& source ) noexcept
      {
         Copy( source );
         return( *this );
      };

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // DATA_CHUNK_CLASS_HEADER
