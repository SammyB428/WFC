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
** $Workfile: dumyfile.hpp $
** $Revision: 14 $
** $Modtime: 6/26/01 11:05a $
*/

#if ! defined( DUMMY_FILE_CLASS_HEADER )

#define DUMMY_FILE_CLASS_HEADER

class CDummyFile : public CFile64
{
   protected:

       HANDLE m_TemplateHandle{ INVALID_HANDLE_VALUE };

#if ! defined( WFC_STL )
      // In the STL builds of WFC, CFile has the m_FileHandle member.
      HANDLE m_FileHandle;
#endif // WFC_STL

      virtual _Check_return_ DWORD m_CreateDesiredAccess( void ) const noexcept;
      virtual _Check_return_ DWORD m_CreateFlagsAndAttributes( void ) const noexcept;
      virtual _Check_return_ DWORD m_CreateShareMode( void ) const noexcept;
      virtual _Check_return_ LPSECURITY_ATTRIBUTES m_CreateSecurityAttributes( void ) const noexcept;
      virtual _Check_return_ DWORD m_CreateCreationDistribution( void ) const noexcept;
      virtual _Check_return_ HANDLE m_CreateTemplateHandle( void ) const noexcept;

   public:

       CDummyFile(CDummyFile const&) = delete;
       CDummyFile& operator=(CDummyFile const&) = delete;

       CDummyFile();

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CDummyFile();

      /*
      ** Interception functions (would blow up if we let CFile64 get them)
      */

      void  Abort( void ) noexcept override;
      void  Close( void ) noexcept override;
      void  Flush( void ) noexcept override;
      _Check_return_ uint64_t GetLength( void ) const noexcept override;
      _Check_return_ uint64_t GetPosition( void ) const noexcept override;

#if ! defined( WFC_STL )
      virtual _Check_return_ BOOL  GetStatus( CFileStatus& status ) noexcept;
      virtual _Check_return_ BOOL  GetStatus( LPCTSTR name, CFileStatus& status ) noexcept;
#endif // WFC_STL

      _Check_return_ uint64_t Seek( __in int64_t const offset, __in CFile64::SeekPosition const from ) noexcept override;

      void SetLength( __in uint64_t const length ) noexcept override;

      _Check_return_ bool LockRange( __in uint64_t const position, __in uint64_t const number_of_bytes_to_lock ) noexcept override;
      void UnlockRange( __in uint64_t const position, __in uint64_t const number_of_bytes_to_unlock ) noexcept override;

      /*
      ** A couple of utility functions to make life easier
      */

      void Write( _In_ std::string_view string_to_write  ) noexcept override;
      virtual void Write( __in std::vector<uint8_t> const& data_to_write ) noexcept;

      // Abstraction Additions

#if defined( WFC_STL )
      inline constexpr void SetHandle( __in HANDLE file_handle ) noexcept { m_FileHandle = (CFILE_HFILE) file_handle; }
#else
      inline void SetHandle( __in HANDLE const file_handle ) noexcept { m_hFile = (CFILE_HFILE) file_handle; }
#endif

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // DUMMY_FILE_CLASS_HEADER
