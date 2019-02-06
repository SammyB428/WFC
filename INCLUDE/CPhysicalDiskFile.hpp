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
** $Workfile: CPhysicalDiskFile.hpp $
** $Revision: 8 $
** $Modtime: 6/26/01 11:03a $
*/

#if ! defined( PHYSICAL_DISK_FILE_CLASS_HEADER )

#define PHYSICAL_DISK_FILE_CLASS_HEADER

class CPhysicalDiskFile : public CFile64
{
   protected:

       DWORD m_BufferSize{ 0 };
       DWORD m_BufferOffset{ 0 };

      DISK_GEOMETRY m_DiskGeometry;

      uint8_t * m_Buffer{ nullptr };

      virtual _Check_return_ bool m_SetSectorSize( void ) noexcept;

   public:

       CPhysicalDiskFile(const CPhysicalDiskFile&) = delete;
       CPhysicalDiskFile& operator=(const CPhysicalDiskFile&) = delete;

      /*
      ** Constructors
      */

      CPhysicalDiskFile();

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CPhysicalDiskFile();

      /*
      ** Methods
      */

      // CFile64 overrides
      virtual void  Close( void ) noexcept override;
      virtual _Check_return_ uint64_t GetLength( void ) const noexcept override;
      virtual _Check_return_ bool Open(_In_z_ LPCTSTR filename, _In_ const UINT open_flags) noexcept override;
      virtual _Check_return_ UINT Read( __out_bcount( count ) void * buffer, _In_ const UINT count ) noexcept override;
      virtual void  SetLength( __in const uint64_t length ) noexcept override;
      virtual void  Write( __in_bcount( count ) const void * buffer, _In_ const UINT count ) noexcept override;

      // New Methods
      virtual _Check_return_ DWORD GetMediaType( void ) const noexcept;
      virtual _Check_return_ bool  GetSectorSize( __out size_t& number_of_bytes_per_sector ) const noexcept;
      virtual void  GetLength( _Out_ ULARGE_INTEGER& size_in_bytes ) const noexcept;
      virtual _Check_return_ bool Open(_In_ int physical_disk_number, _In_ const UINT open_flags) noexcept;
      virtual _Check_return_ bool OpenRead(_In_ int physical_disk_number ) noexcept;
      virtual _Check_return_ bool OpenRead(_In_z_ const wchar_t * physical_drive_name) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // PHYSICAL_DISK_FILE_CLASS_HEADER
