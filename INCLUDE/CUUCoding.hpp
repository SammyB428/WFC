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
** $Workfile: CUUCoding.hpp $
** $Revision: 3 $
** $Modtime: 6/26/01 11:05a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( UU_CODING_CLASS_HEADER )

#define UU_CODING_CLASS_HEADER

class CUUCoding
{
   public:

       CUUCoding(CUUCoding const&) = delete;
       CUUCoding& operator=(CUUCoding const&) = delete;

       // Construction

      CUUCoding() noexcept;

      _Check_return_ bool OutputFilename(_In_ std::vector<uint8_t> const& source, _Out_ std::wstring& destination) const noexcept;
      _Check_return_ bool Decode(_In_ std::vector<uint8_t> const& source, _Out_ std::vector<uint8_t>& destination) const noexcept;
      _Check_return_ bool Decode(_In_ std::wstring const& source, _Out_ std::vector<uint8_t>& destination ) const noexcept;
      _Check_return_ bool Encode(_In_ std::vector<uint8_t> const& source, _Out_ std::vector<uint8_t>& destination ) const noexcept;
};

#endif // UU_CODING_CLASS_HEADER
