/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2014, Samuel R. Blackburn
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

#if ! defined( DATA_DECOMPRESSION_CLASS_HEADER )

#define DATA_DECOMPRESSION_CLASS_HEADER

class CLZFile : public CDummyFile
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      INT m_LZFileHandle;

      COFStruct m_OpenFileStructure;
      
   public:

       CLZFile(const CLZFile&) = delete;
       CLZFile& operator=(const CLZFile&) = delete;

       CLZFile();

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CLZFile();

      static void __stdcall TranslateErrorCode(__in const int error_code, __out std::wstring& error_message) noexcept;

      /*
      ** The Win32 API
      */

      virtual void Close( void ) noexcept;
      virtual __checkReturn BOOL Copy( __in const CLZFile&  source ) noexcept;
      virtual __checkReturn BOOL Copy( __in const CLZFile * source ) noexcept;
      virtual __checkReturn bool GetExpandedName( __in_z wchar_t * name_of_compressed_file, __out std::wstring& original_file_name ) noexcept;
      virtual __checkReturn bool Open( __in_z LPCTSTR channel_name, __in const UINT style = OF_READ ) noexcept override;
      virtual __checkReturn UINT Read( __out_bcount( size_of_buffer ) void * buffer, __in const UINT size_of_buffer ) noexcept override;
      virtual __checkReturn uint64_t Seek( __in const int64_t offset, __in const CFile64::SeekPosition from ) noexcept override;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // DATA_DECOMPRESSION_CLASS_HEADER
