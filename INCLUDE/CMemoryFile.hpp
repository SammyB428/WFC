/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2017, Samuel R. Blackburn
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
** $Workfile: CMemoryFile.hpp $
** $Revision: 9 $
** $Modtime: 6/26/01 11:02a $
*/

#if ! defined( MEMORY_FILE_CLASS_HEADER )

#define MEMORY_FILE_CLASS_HEADER

class CMemoryFile
{
protected:

    SECURITY_ATTRIBUTES * m_SecurityAttributes_p{ nullptr };

    SECURITY_DESCRIPTOR * m_SecurityDescriptor_p{ nullptr };

    HANDLE m_MapHandle{ INVALID_HANDLE_VALUE };

    BY_HANDLE_FILE_INFORMATION m_FileInformation;

    void * m_MappedPointer{ nullptr };
    void * m_Pointer{ nullptr };

    std::wstring m_Filename;

    std::size_t m_AllocationGranularity{ 0 };
    std::size_t m_Length{ 0 };
    std::size_t m_MappedLength{ 0 };

    uint64_t m_FileLength{ 0 };

    uint64_t m_MapOffsetParameter{ 0 };
    uint64_t m_MapLengthParameter{ 0 };
    DWORD m_Access{ 0 };
    DWORD m_Protections{ 0 };
    void * m_DesiredAddress{ nullptr };

    uint32_t m_Attributes{ 0 };
    HANDLE m_TemplateFile{ INVALID_HANDLE_VALUE };

    uint32_t m_LastError{ 0 };

    virtual void m_Initialize( void ) noexcept;
    virtual _Check_return_ bool m_MapTheFile( __in HANDLE const file_handle, __in UINT const open_flags, __in uint64_t const begining_at_offset, __in std::size_t const number_of_bytes_to_map, __in_opt void const * desired_pointer ) noexcept;
    virtual void m_Uninitialize( void ) noexcept;

public:

    CMemoryFile(_In_ CMemoryFile const&) = delete;
    const CMemoryFile& operator=(_In_ CMemoryFile const&) = delete;

    // Construction

    CMemoryFile();
    CMemoryFile( __in std::size_t const allocation_granularity,
        __in_opt SECURITY_ATTRIBUTES * security_attributes = nullptr,
        __in_opt SECURITY_DESCRIPTOR * security_descriptor = nullptr );

    virtual ~CMemoryFile();

    // Properties

    std::size_t Size{ 0 };

    // Methods

    inline constexpr _Check_return_ uint32_t GetErrorCode( void ) const noexcept
    {
        return( m_LastError );
    }

    virtual void      Close( void ) noexcept;
    virtual _Check_return_ bool Flush( void ) noexcept;
    virtual _Check_return_ bool FromHandle( __in HANDLE file_handle,
        __in UINT open_flags                     = (UINT)( (UINT)CFile64::OpenFlags::modeRead | (UINT)CFile64::OpenFlags::shareDenyNone),
        __in uint64_t const beginning_at         = 0,
        __in std::size_t const number_of_bytes_to_map = 0,
        __in_opt void const * desired_address    = nullptr ) noexcept;
    inline  constexpr _Check_return_ uint64_t GetFileLength( void ) const noexcept { return( m_FileLength ); }
    virtual void      GetFilename( __out std::wstring& filename ) const noexcept;
    virtual _Check_return_ bool GetInformation( __inout BY_HANDLE_FILE_INFORMATION& information ) const noexcept;
    inline  constexpr _Check_return_ std::size_t GetLength( void ) const noexcept { return( m_Length ); };
    inline  constexpr _Check_return_ void * GetPointer( void ) const noexcept { return( m_Pointer ); };
    virtual _Check_return_ void * Map( __in uint64_t const offset, __in std::size_t const length ) noexcept;
    virtual _Check_return_ bool Open( __in_z LPCWSTR filename,
        __in UINT open_flags                     = (UINT)( (UINT)CFile64::OpenFlags::modeRead | (UINT)CFile64::OpenFlags::shareDenyNone),
        __in uint64_t const beginning_at         = 0,
        __in std::size_t const number_of_bytes_to_map = 0,
        __in_opt void const * desired_address    = nullptr ) noexcept;
    virtual _Check_return_ bool Open( __in_z LPCSTR filename,
        __in UINT const open_flags               = (UINT)( (UINT)CFile64::OpenFlags::modeRead | (UINT)CFile64::OpenFlags::shareDenyNone),
        __in uint64_t const beginning_at         = 0,
        __in std::size_t const number_of_bytes_to_map = 0,
        __in_opt void const * desired_address    = nullptr ) noexcept;

    // New for Release 50, full control of CreateFile!

    inline virtual _Check_return_ uint32_t GetAttributes( void ) const noexcept { return( m_Attributes ); }
    inline virtual void SetAttributes( __in uint32_t const attributes ) noexcept { m_Attributes = attributes; }

    inline virtual _Check_return_ HANDLE GetTemplateFile( void ) const noexcept { return( m_TemplateFile ); }
    inline virtual void SetTemplateFile( _In_ HANDLE const file_handle ) noexcept { m_TemplateFile = file_handle; };

    // Operators

    inline constexpr operator void * ( void ) const noexcept { return( m_Pointer ); };
};

class CSharedMemory
{
protected:

    HANDLE m_Handle{ INVALID_HANDLE_VALUE };

    std::size_t m_Size{ 0 };

    uint8_t * m_Buffer{ nullptr };

    uint32_t m_LastError{ 0 };

public:

    CSharedMemory(_In_ CSharedMemory const&) = delete;
    CSharedMemory const& operator=(_In_ CSharedMemory const&) = delete;

    std::wstring Name;

    // Construction

    CSharedMemory();
    virtual ~CSharedMemory();

    static inline _Check_return_ bool CreateServiceDACL( _Inout_ SECURITY_ATTRIBUTES * sa) noexcept
    {
        static wchar_t const *sdd = L"D:"
            L"(D;OICI;GA;;;BG)" //Deny guests
            L"(D;OICI;GA;;;AN)" //Deny anonymous
            L"(A;OICI;GRGWGX;;;AU)" //Allow read, write and execute for Users
            L"(A;OICI;GRGWGX;;;IU)" //Allow read, write and execute for Interactive Users
            L"(A;OICI;GA;;;BA)" //Allow all for Administrators
            L"S:(ML;;NW;;;LW)"; // Low integrity SACL

        return( ConvertStringSecurityDescriptorToSecurityDescriptorW(sdd, SDDL_REVISION_1, &sa->lpSecurityDescriptor, nullptr) == TRUE );
    }

    _Check_return_ bool Create( __in_z wchar_t const * name, __in std::size_t const number_of_bytes ) noexcept;
    _Check_return_ bool Create( __in_z wchar_t const * name, __in std::size_t const number_of_bytes, __in_opt SECURITY_ATTRIBUTES * security_attributes = nullptr ) noexcept;
    _Check_return_ bool Open( __in_z wchar_t const * name, __in std::size_t const number_of_bytes, __in bool const read_only ) noexcept;

    void Close( void ) noexcept;

    // We no longer want CSharedMemory to manage the lifetime of the shared memory. I hope you called GetHandle otherwise you will leak a resource.
    inline void Detach( void ) noexcept
    {
        m_Handle = INVALID_HANDLE_VALUE;
        m_Size = 0;
        m_Buffer = nullptr;
    }

    inline constexpr _Check_return_ uint8_t * GetPointer( void ) const noexcept
    {
        return( m_Buffer );
    }

    inline constexpr _Check_return_ std::size_t GetSize( void ) const noexcept
    {
        return( m_Size );
    }

    inline constexpr _Check_return_ uint32_t GetErrorCode( void ) const noexcept
    {
        return( m_LastError );
    }

    inline constexpr _Check_return_ HANDLE GetHandle( void ) const noexcept
    {
        return( m_Handle );
    }
};

#endif // MEMORY_FILE_CLASS_HEADER
