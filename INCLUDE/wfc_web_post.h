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
** $Workfile: wfc_linker_setup.h $
** $Revision: 10 $
** $Modtime: 6/26/01 11:09a $
*/

#if ! defined( WFC_WEB_POST_HEADER_FILE )

#define WFC_WEB_POST_HEADER_FILE

#define WFC_WEB_POST_BUFFER      (1)
#define WFC_WEB_POST_CBYTEARRAY  (2)
#define WFC_WEB_POST_STRING      (3)
#define WFC_WEB_POST_FILE_HANDLE (4)

struct WFC_WEB_POST_DATA
{
    const char* variable_name{ nullptr };
    DWORD       data_type{ 0 };

   union
   {
      // WFC_WEB_POST_BUFFER

      struct
      {
          DWORD  buffer_length{ 0 };
          BYTE* buffer{ nullptr };
      } byte_buffer;

      // WFC_WEB_POST_CBYTEARRAY
      std::vector<uint8_t>* bytes{ nullptr };

      // WFC_WEB_POST_STRING
      const char * ascii_string;

      // WFC_WEB_POST_FILE_HANDLE
      struct
      {
          const char* ascii_filename{ nullptr };
          HANDLE         file_handle{ nullptr };
          ULARGE_INTEGER file_offset{ 0,0 };
          ULARGE_INTEGER number_of_bytes{ 0,0 };
      } file;
   };
};

_Check_return_ bool PASCAL wfc_web_post( _In_ std::wstring_view url, _In_ WFC_WEB_POST_DATA ** data, __inout_opt std::wstring * response = nullptr ) noexcept;

#endif // WFC_WEB_POST_HEADER_FILE
