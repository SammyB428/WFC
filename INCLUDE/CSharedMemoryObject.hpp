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
** $Workfile: CSharedMemoryObject.hpp $
** $Revision: 5 $
** $Modtime: 6/26/01 11:04a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( SHARED_MEMORY_OBJECT_CLASS_HEADER )

#define SHARED_MEMORY_OBJECT_CLASS_HEADER

class CSharedMemoryObject
{
   protected:

      HANDLE m_MapHandle{ INVALID_HANDLE_VALUE };

      std::size_t m_Size{ 0 };

      void * m_Pointer{ nullptr };

      std::wstring m_Name;

   public:

       CSharedMemoryObject(CSharedMemoryObject const&) = delete;
       CSharedMemoryObject& operator=(CSharedMemoryObject const&) = delete;
 
       CSharedMemoryObject() noexcept;
     ~CSharedMemoryObject();

      void          Close( void ) noexcept;
      _Check_return_ bool  Create( _In_ std::wstring_view object_name, _In_ std::size_t const size_in_bytes ) noexcept;
      inline constexpr _Check_return_ HANDLE GetHandle( void ) const noexcept { return( m_MapHandle ); };
      inline void   GetName( __out std::wstring& name ) const noexcept { name.assign( m_Name ); };
      inline constexpr _Check_return_ void * GetPointer( void ) const noexcept   { return( m_Pointer ); };
      inline constexpr _Check_return_ std::size_t GetSize( void ) const noexcept { return( m_Size ); }
};

#endif // SHARED_MEMORY_OBJECT_CLASS_HEADER
