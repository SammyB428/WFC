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
** $Workfile: CFileTransferProtocol.hpp $
** $Revision: 10 $
** $Modtime: 6/26/01 11:01a $
*/

#if ! defined( FILE_TRANSFER_CLASS_HEADER )

#define FILE_TRANSFER_CLASS_HEADER

#if ! defined( WFC_STL )

class CFileTransferProtocol
{
   protected:

      std::wstring m_UserName;
      std::wstring m_Password;

   public:

       CFileTransferProtocol(CFileTransferProtocol const&) = delete;
       CFileTransferProtocol& operator=(CFileTransferProtocol const&) = delete;
       CFileTransferProtocol( __in_z_opt LPCTSTR user_name = nullptr, __in_z_opt LPCTSTR password = nullptr );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CFileTransferProtocol();

      virtual void GetDirectory( __in CUniformResourceLocator const& source, __inout std::vector<std::wstring>& directory_contents, __in_z_opt LPCTSTR filter = nullptr ) noexcept;
      virtual void GetFile( __in CUniformResourceLocator const& source, __inout std::vector<uint8_t>& file_contents ) noexcept;
      virtual void GetPassword( __inout std::wstring& password ) const noexcept;
      virtual void GetUserName( __inout std::wstring& account  ) const noexcept;
      virtual BOOL PutFile( __in CUniformResourceLocator const& url, __in std::vector<uint8_t> const& file_contents ) noexcept;
      virtual void SetPassword( __in std::wstring const& password ) noexcept;
      virtual void SetUserName( __in std::wstring const& account  ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // WFC_STL

#endif // FILE_TRANSFER_CLASS_HEADER
