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
** $Workfile: dumyfile.hpp $
** $Revision: 14 $
** $Modtime: 6/26/01 11:05a $
*/

#if ! defined( DUMMY_FILE_CLASS_HEADER )

#define DUMMY_FILE_CLASS_HEADER

class CDummyFile : public CFile64
{
   protected:

      HANDLE m_TemplateHandle;

#if ! defined( WFC_STL )
      // In the STL builds of WFC, CFile has the m_FileHandle member.
      HANDLE m_FileHandle;
#endif // WFC_STL

      virtual __checkReturn DWORD m_CreateDesiredAccess( void ) const noexcept;
      virtual __checkReturn DWORD m_CreateFlagsAndAttributes( void ) const noexcept;
      virtual __checkReturn DWORD m_CreateShareMode( void ) const noexcept;
      virtual __checkReturn LPSECURITY_ATTRIBUTES m_CreateSecurityAttributes( void ) const noexcept;
      virtual __checkReturn DWORD m_CreateCreationDistribution( void ) const noexcept;
      virtual __checkReturn HANDLE m_CreateTemplateHandle( void ) const noexcept;

   public:

       CDummyFile(const CDummyFile&) = delete;
       CDummyFile& operator=(const CDummyFile&) = delete;

       CDummyFile();

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CDummyFile();

      /*
      ** Interception functions (would blow up if we let CFile64 get them)
      */

      virtual void  Abort( void ) noexcept override;
      virtual void  Close( void ) noexcept override;
      virtual void  Flush( void ) noexcept override;
      virtual __checkReturn uint64_t GetLength( void ) const noexcept override;
      virtual __checkReturn uint64_t GetPosition( void ) const noexcept override;

#if ! defined( WFC_STL )
      virtual __checkReturn BOOL  GetStatus( CFileStatus& status ) noexcept;
      virtual __checkReturn BOOL  GetStatus( LPCTSTR name, CFileStatus& status ) noexcept;
#endif // WFC_STL

      virtual void  Remove( __in_z LPCTSTR name ) noexcept;
      virtual void  Rename( __in_z LPCTSTR OldName, __in_z LPCTSTR NewName ) noexcept;
      virtual __checkReturn uint64_t Seek( __in const int64_t offset, __in const CFile64::SeekPosition from ) noexcept override;

      virtual void SetLength( __in const uint64_t length ) noexcept override;

      virtual __checkReturn bool LockRange( __in const uint64_t position, __in const uint64_t number_of_bytes_to_lock ) noexcept override;
      virtual void UnlockRange( __in const uint64_t position, __in const uint64_t number_of_bytes_to_unlock ) noexcept override;

      /*
      ** A couple of utility functions to make life easier
      */

      virtual void Write( __in const std::string& string_to_write  ) noexcept override;
      virtual void Write( __in const std::vector<uint8_t>& data_to_write ) noexcept;

      // Abstraction Additions

      inline __checkReturn HANDLE GetHandle( void ) const noexcept { return( m_FileHandle ); }

#if defined( WFC_STL )
      inline void SetHandle( __in HANDLE file_handle ) noexcept { m_FileHandle = (CFILE_HFILE) file_handle; }
#else
      inline void SetHandle( __in const HANDLE file_handle ) noexcept { m_hFile = (CFILE_HFILE) file_handle; }
#endif

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // DUMMY_FILE_CLASS_HEADER
