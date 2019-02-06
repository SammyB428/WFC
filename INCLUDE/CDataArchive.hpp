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

      uint32_t m_Position;

      bool m_AmIWriting;

      const CDataChunk * m_ReadFromDataChunk_p;
      CDataChunk * m_WriteToDataChunk_p;

   public:

       CDataArchive(__in const CDataArchive&) = delete;
       CDataArchive& operator=(__in const CDataArchive&) = delete;
 
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

      virtual void ReadFrom( __in const CDataChunk * chunk_p ) noexcept;

      virtual void Write( __in const char     value ) noexcept;
      virtual void Write( __in const uint8_t  value ) noexcept;
      virtual void Write( __in const uint16_t value ) noexcept;
      virtual void Write( __in const wchar_t  value ) noexcept;
      virtual void Write( __in const int32_t  value ) noexcept;
      virtual void Write( __in const uint32_t value ) noexcept;
      virtual void Write( __in const uint64_t value ) noexcept;
      virtual void Write( __in const float    value ) noexcept;
      virtual void Write( __in const double   value ) noexcept;
      virtual void Write( __in const std::wstring& value ) noexcept;
      virtual void Write( __in const std::vector<uint8_t>   &   array ) noexcept;
      virtual void Write( __in const std::vector<std::wstring>& array ) noexcept;
      virtual void Write( __in const std::vector<uint32_t>& array ) noexcept;
      virtual void Write( __in const std::vector<uint16_t>& array ) noexcept;

      virtual void WriteTo( __inout CDataChunk * chunk_p ) noexcept;
};

#endif // DATA_ARCHIVE_CLASS_HEADER
