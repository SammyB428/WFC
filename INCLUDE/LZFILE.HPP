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
** $Workfile: lzfile.hpp $
** $Revision: 13 $
** $Modtime: 6/26/01 11:05a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( DATA_DECOMPRESSION_CLASS_HEADER )

#define DATA_DECOMPRESSION_CLASS_HEADER

class CLZFile : public CDummyFile
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

       INT m_LZFileHandle{ 0 };

      COFStruct m_OpenFileStructure;

   public:

       CLZFile(CLZFile const&) = delete;
       CLZFile& operator=(CLZFile const&) = delete;

       CLZFile() noexcept;

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CLZFile();

      static void __stdcall TranslateErrorCode(__in int const error_code, __out std::wstring& error_message) noexcept;

      /*
      ** The Win32 API
      */

      virtual void Close( void ) noexcept;
      virtual _Check_return_ bool Copy( __in CLZFile const&  source ) noexcept;
      virtual _Check_return_ bool Copy( __in CLZFile const * source ) noexcept;
      virtual _Check_return_ bool GetExpandedName(_In_ std::wstring_view name_of_compressed_file, __out std::wstring& original_file_name ) noexcept;
      _Check_return_ bool Open(_In_ std::filesystem::path const& channel_name, __in UINT const style = OF_READ ) noexcept override;
      _Check_return_ UINT Read( __out_bcount( size_of_buffer ) void * buffer, __in UINT const size_of_buffer ) noexcept override;
      _Check_return_ uint64_t Seek( __in int64_t const offset, __in CFile64::SeekPosition const from ) noexcept override;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // DATA_DECOMPRESSION_CLASS_HEADER
