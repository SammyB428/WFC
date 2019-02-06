/*
** Author: Samuel R. Blackburn
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
** $Workfile: CFile64.hpp $
** $Revision: 6 $
** $Modtime: 4/01/01 10:30a $
** $Reuse Tracing Code: 1001 $
*/

/*
** NOTICE NOTICE NOTICE
**
** Many of the things you will see in this class go against the coding
** standards as well as good coding practices. The reason for this is we
** are trying to make a CFile drop-in replacement. This means we have to
** minimize the damage a replacement will make. So, we have to duplicate
** the bad habits of Microsoft. Where possible, we will do the right thing.
** One obvious break point will be using ULONGLONG instead of DWORD for
** things like file position.
*/

#if ! defined( FILE_64_CLASS_HEADER )

#define FILE_64_CLASS_HEADER

#if ! defined( FILE_ATTRIBUTE_ENCRYPTED )
#define FILE_ATTRIBUTE_ENCRYPTED (0x00000040)
#endif

#if ! defined( FILE_ATTRIBUTE_REPARSE_POINT )
#define FILE_ATTRIBUTE_REPARSE_POINT (0x00000400)
#endif

#if ! defined( FILE_ATTRIBUTE_SPARSE_FILE )
#define FILE_ATTRIBUTE_SPARSE_FILE (0x00000200)
#endif

#if ! defined( FILE_ATTRIBUTE_DEVICE )
#define FILE_ATTRIBUTE_DEVICE (0x00000040)
#endif

#if ! defined( FILE_ATTRIBUTE_NOT_CONTENT_INDEXED )
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED (0x00002000)
#endif

class CExtensibleMarkupLanguageDocument;

#define SAFE_WRITE_OK   (0)
#define SAFE_WRITE_WAIT (1)
#define SAFE_WRITE_FAIL (2)

using SAFE_WRITE_CALLBACK = _Check_return_ int (*)( _Inout_ void * parameter, _In_ const uint32_t error_code, _In_z_ LPCTSTR filename );
using PROCESS_BUFFER_CALLBACK = bool (*)( void * context_p, const uint8_t * buffer, const size_t number_of_bytes_in_buffer );

class CFile64
{
   protected:

       SECURITY_ATTRIBUTES * m_SecurityAttributes_p{ nullptr };
       SECURITY_DESCRIPTOR * m_SecurityDescriptor_p{ nullptr };
       SAFE_WRITE_CALLBACK m_SafeWriteCallback{ nullptr };
       void *              m_SafeWriteContext{ nullptr };

       HANDLE m_FileHandle{ INVALID_HANDLE_VALUE };
       HANDLE m_TemplateFile{ INVALID_HANDLE_VALUE };

       std::wstring m_PathName;
       std::wstring m_FileName;
       std::wstring m_FileTitle;

       DWORD m_LastError{ 0 };
       uint32_t m_Attributes{ 0 };

       bool m_CloseOnDelete{ true };

#if ! defined( WFC_STL )

      // MFC Build.
      // Exceptions are just too stupid to use. I'll keep backwards
      // compatibility to MFC but the user has to say they want it.
      // They must inherit from CFile64 and provide a method to set
      // m_BeStupidAndUseExceptions to non-false.

      bool m_BeStupidAndUseExceptions{ false };

#endif // WFC_STL

      void m_Initialize( void );
      void m_Uninitialize( void );

   public:

      // Flag values
      enum class OpenFlags : uint32_t
      {
         modeRead         = 0x0000,
         modeWrite        = 0x0001,
         modeReadWrite    = 0x0002,
         shareCompat      = 0x0000,
         shareExclusive   = 0x0010,
         shareDenyWrite   = 0x0020,
         shareDenyRead    = 0x0030,
         shareDenyNone    = 0x0040,
         modeNoInherit    = 0x0080,
         shareAllowDelete = 0x0100,
         modeCreate       = 0x1000,
         modeNoTruncate   = 0x2000,
         typeText         = 0x4000,
         typeBinary       = 0x8000,
         osNoBuffer       = 0x10000,
         osWriteThrough   = 0x20000,
         osRandomAccess   = 0x40000,
         osSequentialScan = 0x80000,
         wfcDeleteOnClose = 0x100000,
      };

      enum class Attribute : uint32_t
      {
         normal    = 0x00,
         readOnly  = 0x01,
         hidden    = 0x02,
         system    = 0x04,
         volume    = 0x08,
         directory = 0x10,
         archive   = 0x20,
      };

      enum class SeekPosition : uint32_t
      {
         begin   = 0x0,
         current = 0x1,
         end     = 0x2,
      };

      CFile64(_In_ CFile64 const&) = delete;
      _Check_return_ CFile64& operator=(_In_ CFile64 const&) = delete;

      CFile64();
      CFile64( _In_ HANDLE const file_handle );
      CFile64( _In_z_ LPCTSTR filename, _In_ UINT const open_flags );
      virtual ~CFile64();

      static _Check_return_ bool Exists( _In_z_ LPCWSTR file_name ) noexcept
      {
          const DWORD attributes = ::GetFileAttributesW( file_name );

          return( attributes == INVALID_FILE_ATTRIBUTES ? false : true );
      }

      static _Check_return_ bool Exists( _In_z_ LPCSTR file_name ) noexcept
      {
          const DWORD attributes = ::GetFileAttributesA( file_name );

          return( attributes == INVALID_FILE_ATTRIBUTES ? false : true );
      }

      static void CreatePathTo(_In_z_ LPCWSTR file_name) noexcept;

      static _Check_return_ bool IsDirectory( _In_z_ LPCWSTR file_name ) noexcept
      {
          const DWORD attributes = ::GetFileAttributesW( file_name );

          if ( attributes == INVALID_FILE_ATTRIBUTES )
          {
              return( false );
          }

          if ( attributes & FILE_ATTRIBUTE_DIRECTORY )
          {
              return( true );
          }

          return( false );
      }

      static _Check_return_ bool IsDirectory( __in_z LPCSTR file_name ) noexcept
      {
          const DWORD attributes = ::GetFileAttributesA( file_name );

          if ( attributes == INVALID_FILE_ATTRIBUTES )
          {
              return( false );
          }

          if ( attributes & FILE_ATTRIBUTE_DIRECTORY )
          {
              return( true );
          }

          return( false );
      }

      virtual void                                 Abort( void ) noexcept;
      virtual void                                 Close( void ) noexcept;
      virtual _Check_return_ CFile64 *             Duplicate( void ) const noexcept;
      virtual void                                 Flush( void ) noexcept;
      virtual _Check_return_ std::wstring          GetFileName( void ) const noexcept;
      virtual _Check_return_ std::wstring          GetFilePath( void ) const noexcept;
      virtual _Check_return_ std::wstring          GetFileTitle( void ) const noexcept;
      virtual _Check_return_ HANDLE                GetHandle( void ) const noexcept;
      virtual _Check_return_ bool                  GetInformation( __inout BY_HANDLE_FILE_INFORMATION& information ) const noexcept;
      virtual _Check_return_ uint64_t              GetLength( void ) const noexcept;
      virtual _Check_return_ uint64_t              GetPosition( void ) const noexcept;
      virtual _Check_return_ SECURITY_ATTRIBUTES * GetSecurityAttributes( void ) const noexcept;
      virtual _Check_return_ SECURITY_DESCRIPTOR * GetSecurityDescriptor( void ) const noexcept;
      virtual _Check_return_ bool                  LockRange(_In_ uint64_t const position, _In_ uint64_t const number_of_bytes_to_lock ) noexcept;
      virtual _Check_return_ bool                  Open( _In_z_ LPCTSTR filename, _In_ UINT const open_flags ) noexcept;

      _Check_return_ uint32_t AtomicRead(_In_ const uint64_t file_offset, __out_bcount( number_of_bytes_to_read ) void * buffer, _In_ uint32_t const number_of_bytes_to_read ) const noexcept;
      _Check_return_ uint32_t AtomicWrite(_In_ const uint64_t file_offset, __in_bcount(number_of_bytes_to_write) void const * buffer, _In_ uint32_t const number_of_bytes_to_write) const noexcept;

      virtual _Check_return_ uint32_t Read( __out_bcount( number_of_bytes_to_read ) void * buffer, _In_ uint32_t const number_of_bytes_to_read ) noexcept;
      virtual _Check_return_ uint32_t ReadHuge( __out_bcount( number_of_bytes_to_read ) void * buffer, _In_ uint32_t const number_of_bytes_to_read ) noexcept;
      virtual _Check_return_ bool    Read(_Inout_ std::vector<uint8_t>& bytes_from_file, _In_ uint32_t const number_of_bytes_to_read ) noexcept;
      virtual _Check_return_ bool    Read(_Inout_ std::string& string_from_file ) noexcept;
      virtual _Check_return_ bool    Read(_Inout_ std::vector<std::string>& file_contents ) noexcept;
      virtual _Check_return_ bool    Read(_Inout_ std::wstring& string_from_file) noexcept;
      virtual _Check_return_ bool    Read(_Inout_ std::vector<std::wstring>& file_contents) noexcept;

      inline _Check_return_ bool Read( _Inout_ CExtensibleMarkupLanguageDocument& xml ) noexcept
      {
         std::vector<uint8_t> bytes;

         bytes.resize( GetLength() );

         if ( Read( bytes.data(), static_cast<UINT>(bytes.size()) ) == bytes.size() )
         {
            CDataParser parser;

            (void) parser.Initialize(&bytes);

            return( xml.Parse( parser ) );
         }

         return( false );
      }

      inline constexpr _Check_return_ DWORD LastError(void) const noexcept
      {
          return(m_LastError);
      }

      virtual _Check_return_ bool     ProcessContent( __in std::size_t const step_size, __inout PROCESS_BUFFER_CALLBACK function_to_call, __inout_opt void * callback_context ) const noexcept;
      virtual void                    RegisterSafeWrite( __inout void * context, __callback SAFE_WRITE_CALLBACK callback ) noexcept;
      virtual _Check_return_ uint64_t Seek( __in int64_t const offset, __in CFile64::SeekPosition const from ) noexcept;
      virtual void                    SeekToBegin( void ) noexcept;
      virtual _Check_return_ uint64_t SeekToEnd( void ) noexcept;
      virtual _Check_return_ bool     SetEndOfFile( __in uint64_t const length ) noexcept;
      virtual void                    SetFilePath( __in_z LPCTSTR new_name ) noexcept;
      virtual void                    SetLength( __in uint64_t const length ) noexcept;
      virtual _Check_return_ bool     SetPosition( __in uint64_t const position ) noexcept;
      virtual _Check_return_ bool     SparsifyRegion( __in uint64_t const position, __in uint64_t const number_of_bytes_to_mark_as_empty ) noexcept;
      virtual void                    UnlockRange( __in uint64_t const position, __in uint64_t const number_of_bytes_to_unlock ) noexcept;
      virtual void                    Write( __in_bcount( number_of_bytes_to_write ) void const * buffer, __in uint32_t const number_of_bytes_to_write ) noexcept;
      virtual void                    Write(_In_ std::string const& string_to_write ) noexcept;
      virtual void                    Write(_In_ std::vector<std::string> const& string_to_write, __in bool const include_crlf_after_every_string ) noexcept;
      virtual void                    Write(_In_ std::wstring const& string_to_write) noexcept;
      virtual void                    Write(_In_ std::vector<std::wstring> const& string_to_write, _In_ bool const include_crlf_after_every_string) noexcept;
      virtual void                    WriteHuge( __in_bcount( number_of_bytes_to_write ) void const * buffer, __in DWORD const number_of_bytes_to_write ) noexcept;

      // New for Release 50, full control of CreateFile!

      inline virtual _Check_return_ uint32_t GetAttributes( void ) const noexcept { return( m_Attributes ); }
      inline virtual void SetAttributes( __in uint32_t attributes ) noexcept { m_Attributes = attributes; }

      inline virtual _Check_return_ HANDLE GetTemplateFile( void ) const noexcept { return( m_TemplateFile ); }
      inline virtual void SetTemplateFile( __in HANDLE file_handle ) noexcept { m_TemplateFile = file_handle; };

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void                  Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG

      static void PASCAL            Rename( __in_z LPCTSTR old_name, __in_z LPCTSTR new_name ) noexcept;
      static void PASCAL            Remove( __in_z LPCTSTR filename ) noexcept;

      _Check_return_ bool OpenWide( __in_z wchar_t const * unicode_filename, __in UINT const open_flags ) noexcept;
      virtual _Check_return_ bool SetShortName( __in_z LPCTSTR new_short_name ) noexcept; // New for 73
      virtual __checkReturn bool SetValidData( __in int64_t const valid_data_length ) noexcept; // New for 73

      /*
      Generating SDDL strings is a royal pain. The easiest way to do it is to use
      explorer to set the permissions on a file as you want the SDDL for. Then
      spawn a command prompt and issue the command:
      cacls myfile.txt /S
      It will print out the SDDL string that you can cut and paste into your code.
      */
      /// Will use the SDDL in the creation of files
      virtual void SetSecurity(_In_z_ wchar_t const * sddl) noexcept;
};

// Now some popular file open modes...

// Open the file for reading, deny none, allow deleting
inline constexpr _Check_return_ UINT read_file_open_mode(void) noexcept
{
    return(static_cast<UINT>(static_cast<UINT>(CFile64::OpenFlags::modeRead) | static_cast<UINT>(CFile64::OpenFlags::shareDenyNone) | static_cast<UINT>(CFile64::OpenFlags::shareAllowDelete)));
}

// Create the file for writing, deny writes, allow deleting
inline constexpr _Check_return_ UINT create_for_writing(void) noexcept
{
    return(static_cast<UINT>(static_cast<UINT>(CFile64::OpenFlags::modeCreate) | static_cast<UINT>(CFile64::OpenFlags::modeWrite) | static_cast<UINT>(CFile64::OpenFlags::shareAllowDelete) | static_cast<UINT>(CFile64::OpenFlags::shareDenyWrite)));
}

// Open the file for reading, deny none and delete on close
inline constexpr _Check_return_ UINT read_delete_on_close(void) noexcept
{
    return(read_file_open_mode() | static_cast<UINT>(CFile64::OpenFlags::wfcDeleteOnClose));
}

// Create a readwrite file that will be deleted when it is closed
inline constexpr _Check_return_ UINT create_temporary_delete_on_close(void) noexcept
{
    return(static_cast<UINT>(static_cast<UINT>(CFile64::OpenFlags::modeCreate) | static_cast<UINT>(CFile64::OpenFlags::modeReadWrite) | static_cast<UINT>(CFile64::OpenFlags::wfcDeleteOnClose) | static_cast<UINT>(CFile64::OpenFlags::shareDenyNone) | static_cast<UINT>(CFile64::OpenFlags::shareAllowDelete)));
}

#endif // FILE_64_CLASS_HEADER
