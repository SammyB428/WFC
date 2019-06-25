/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2016, Samuel R. Blackburn
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
** $Workfile: CDataFile.hpp $
** $Revision: 6 $
** $Modtime: 6/26/01 11:01a $
*/

#if ! defined( DATA_FILE_CLASS_HEADER )

#define DATA_FILE_CLASS_HEADER

class CDataFile : public CFile64
{
   public:

      CDataFile(const CDataFile&) = delete;
      CDataFile& operator=(const CDataFile&) = delete;
      CDataFile();
      virtual ~CDataFile();

      virtual __checkReturn bool AddData( __in uint32_t const identifier, __in  std::vector<uint8_t> const& data ) noexcept;
      virtual __checkReturn bool AddData( __in uint32_t const identifier, __in_bcount( number_of_bytes ) uint8_t const * data, __in std::size_t const number_of_bytes ) noexcept;
      virtual __checkReturn bool AddData( __in CDataChunk const& data ) noexcept;
      virtual __checkReturn bool GetData( __out uint32_t& identifier, __out std::vector<uint8_t>& data ) noexcept;
      virtual __checkReturn bool GetData( __out CDataChunk& data ) noexcept;
};

#endif // DATA_FILE_CLASS_HEADER
