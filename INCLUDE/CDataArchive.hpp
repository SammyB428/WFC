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
** $Workfile: CDataArchive.hpp $
** $Revision: 6 $
** $Modtime: 6/26/01 11:01a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( DATA_ARCHIVE_CLASS_HEADER )

#define DATA_ARCHIVE_CLASS_HEADER

class CDataArchive
{
   protected:

       uint32_t m_Position{ 0 };

       bool m_AmIWriting{ false };

       CDataChunk const * m_ReadFromDataChunk_p{ nullptr };
       CDataChunk* m_WriteToDataChunk_p{ nullptr };

   public:

       CDataArchive(_In_ CDataArchive const&) = delete;
       CDataArchive& operator=(_In_ CDataArchive const&) = delete;
 
       CDataArchive() noexcept;
      virtual ~CDataArchive();

      virtual _Check_return_ bool IsLoading( void ) const noexcept;
      virtual _Check_return_ bool IsStoring( void ) const noexcept;

      virtual void Read( _Out_ char&     value ) noexcept;
      virtual void Read( _Out_ uint8_t&  value ) noexcept;
      virtual void Read( _Out_ uint16_t& value ) noexcept;
      virtual void Read( _Out_ wchar_t&  value ) noexcept;
      virtual void Read( _Out_ int32_t&  value ) noexcept;
      virtual void Read( _Out_ uint32_t& value ) noexcept;
      virtual void Read( _Out_ uint64_t& value ) noexcept;
      virtual void Read( _Out_ float&    value ) noexcept;
      virtual void Read( _Out_ double&   value ) noexcept;
      virtual void Read( _Out_ std::wstring&  value ) noexcept;
      virtual void Read( _Out_ std::vector<uint8_t>&   array ) noexcept;
      virtual void Read( _Out_ std::vector<std::wstring>& array ) noexcept;
      virtual void Read( _Out_ std::vector<uint32_t>& array ) noexcept;
      virtual void Read( _Out_ std::vector<uint16_t>& array ) noexcept;

      virtual void ReadFrom( _In_ CDataChunk const * chunk_p ) noexcept;

      virtual void Write( _In_ char     const value ) noexcept;
      virtual void Write( _In_ uint8_t  const value ) noexcept;
      virtual void Write( _In_ uint16_t const value ) noexcept;
      virtual void Write( _In_ wchar_t  const value ) noexcept;
      virtual void Write( _In_ int32_t  const value ) noexcept;
      virtual void Write( _In_ uint32_t const value ) noexcept;
      virtual void Write( _In_ uint64_t const value ) noexcept;
      virtual void Write( _In_ float    const value ) noexcept;
      virtual void Write( _In_ double   value ) noexcept;
      virtual void Write( _In_ std::wstring const& value ) noexcept;
      virtual void Write( _In_ std::vector<uint8_t> const& array ) noexcept;
      virtual void Write( _In_ std::vector<std::wstring> const& array ) noexcept;
      virtual void Write( _In_ std::vector<uint32_t> const& array ) noexcept;
      virtual void Write( _In_ std::vector<uint16_t> const& array ) noexcept;

      virtual void WriteTo( __inout CDataChunk * chunk_p ) noexcept;
};

#endif // DATA_ARCHIVE_CLASS_HEADER
