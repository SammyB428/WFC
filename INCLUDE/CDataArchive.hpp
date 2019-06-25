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

       CDataArchive(__in CDataArchive const&) = delete;
       CDataArchive& operator=(__in CDataArchive const&) = delete;
 
       CDataArchive();
      virtual ~CDataArchive();

      virtual __checkReturn bool IsLoading( void ) const noexcept;
      virtual __checkReturn bool IsStoring( void ) const noexcept;

      virtual void Read( __out char&     value ) noexcept;
      virtual void Read( __out uint8_t&  value ) noexcept;
      virtual void Read( __out uint16_t& value ) noexcept;
      virtual void Read( __out wchar_t&  value ) noexcept;
      virtual void Read( __out int32_t&  value ) noexcept;
      virtual void Read( __out uint32_t& value ) noexcept;
      virtual void Read( __out uint64_t& value ) noexcept;
      virtual void Read( __out float&    value ) noexcept;
      virtual void Read( __out double&   value ) noexcept;
      virtual void Read( __out std::wstring&  value ) noexcept;
      virtual void Read( __out std::vector<uint8_t>&   array ) noexcept;
      virtual void Read( __out std::vector<std::wstring>& array ) noexcept;
      virtual void Read( __out std::vector<uint32_t>& array ) noexcept;
      virtual void Read( __out std::vector<uint16_t>& array ) noexcept;

      virtual void ReadFrom( __in CDataChunk const * chunk_p ) noexcept;

      virtual void Write( __in char     const value ) noexcept;
      virtual void Write( __in uint8_t  const value ) noexcept;
      virtual void Write( __in uint16_t const value ) noexcept;
      virtual void Write( __in wchar_t  const value ) noexcept;
      virtual void Write( __in int32_t  const value ) noexcept;
      virtual void Write( __in uint32_t const value ) noexcept;
      virtual void Write( __in uint64_t const value ) noexcept;
      virtual void Write( __in float    const value ) noexcept;
      virtual void Write( __in double   value ) noexcept;
      virtual void Write( __in std::wstring const& value ) noexcept;
      virtual void Write( __in std::vector<uint8_t> const& array ) noexcept;
      virtual void Write( __in std::vector<std::wstring> const& array ) noexcept;
      virtual void Write( __in std::vector<uint32_t> const& array ) noexcept;
      virtual void Write( __in std::vector<uint16_t> const& array ) noexcept;

      virtual void WriteTo( __inout CDataChunk * chunk_p ) noexcept;
};

#endif // DATA_ARCHIVE_CLASS_HEADER
