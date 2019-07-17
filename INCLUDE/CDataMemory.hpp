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
** $Workfile: CDataMemory.hpp $
** $Revision: 7 $
** $Modtime: 6/26/01 11:01a $
*/

#if ! defined( DATA_MEMORY_CLASS_HEADER )

#define DATA_MEMORY_CLASS_HEADER

class CDataMemory : public CDataFile
{
   protected:

       uint64_t m_Position{ 0 };

      std::vector<uint8_t> m_Data;

   public:

      CDataMemory();
      CDataMemory( __in CDataMemory const& source );
      CDataMemory( __in_bcount( number_of_bytes ) uint8_t const * buffer_p, __in std::size_t const number_of_bytes );
      CDataMemory( __in std::vector<uint8_t> const& source );
      virtual ~CDataMemory();

      virtual void  Append( __in std::vector<uint8_t> const&  source ) noexcept;
      virtual void  Append( __in CDataMemory const& source ) noexcept;
      virtual void  AppendTo( __out std::vector<uint8_t>& data ) noexcept;
      void  Close( void ) noexcept override;
      virtual void  Copy( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept;
      virtual void  Copy( __in std::vector<uint8_t> const& source ) noexcept;
      virtual void  Copy( __in CDataMemory const& source ) noexcept;
      virtual void  CopyTo( __out std::vector<uint8_t>& data ) noexcept;
      virtual void  Empty( void ) noexcept;
      void  Flush( void ) noexcept override;
      virtual _Check_return_ uint8_t const * GetData( void ) const noexcept;
      _Check_return_ uint64_t GetLength( void ) const noexcept override;
      _Check_return_ uint64_t GetPosition( void ) const noexcept override;
      _Check_return_ bool Open( __in_z LPCTSTR filename, __in UINT const mode ) noexcept override;
      _Check_return_ UINT Read( __out_bcount( number_of_bytes_to_read ) void * buffer, __in UINT const number_of_bytes_to_read ) noexcept override;
      _Check_return_ uint64_t Seek( __in int64_t const offset, __in CFile64::SeekPosition const from ) noexcept override;
      void  Write( __in_bcount( number_of_bytes_to_write ) void const * buffer, __in UINT const number_of_bytes_to_write ) noexcept override;

      virtual _Check_return_ CDataMemory const& operator=(  __in CDataMemory const& source ) noexcept;
      virtual _Check_return_ CDataMemory const& operator=(  __in std::vector<uint8_t> const& source ) noexcept;
      virtual _Check_return_ CDataMemory const& operator+=( __in CDataMemory const& source ) noexcept;
      virtual _Check_return_ CDataMemory const& operator+=( __in std::vector<uint8_t> const& source ) noexcept;
};

#endif // DATA_FILE_CLASS_HEADER
