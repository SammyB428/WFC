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
** $Workfile: CDataSocket.hpp $
** $Revision: 6 $
** $Modtime: 6/26/01 11:01a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( DATA_SOCKET_CLASS_HEADER )

#define DATA_SOCKET_CLASS_HEADER

class CDataSocket final : public CDataFile
{
   protected:

       CSimpleSocket * m_Socket_p{ nullptr };

   public:

       CDataSocket(_In_ CDataSocket const&) = delete;
       CDataSocket& operator=(_In_ CDataSocket const&) = delete;

       CDataSocket() noexcept;
      CDataSocket( __inout CSimpleSocket * socket_p ) noexcept;
      virtual ~CDataSocket();

      virtual void Attach( __inout CSimpleSocket * socket_p ) noexcept;
      _Check_return_ uint32_t Read( __out_bcount( number_of_bytes_to_read ) void * buffer, _In_ uint32_t const number_of_bytes_to_read ) noexcept override;
      _Check_return_ uint64_t Seek( _In_ int64_t const offset, _In_ CFile64::SeekPosition const from ) noexcept override;
      void Write( __in_bcount( number_of_bytes_to_write ) void const * buffer, _In_ uint32_t const number_of_bytes_to_write ) noexcept override;
};

#endif // DATA_SOCKET_CLASS_HEADER
