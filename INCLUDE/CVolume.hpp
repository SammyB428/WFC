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
** $Workfile: CVolume.hpp $
** $Revision: 4 $
** $Modtime: 6/26/01 11:05a $
*/

#if ! defined( VOLUME_CLASS_HEADER )

#define VOLUME_CLASS_HEADER

class CVolume
{
   protected:

      HANDLE m_Handle;

      UINT m_DriveType;

      std::wstring m_Name;

      wchar_t m_DriveLetter;

   public:

       CVolume(const CVolume&) = delete;
       CVolume& operator=(const CVolume&) = delete;
 
       CVolume();
      virtual ~CVolume();

      _Check_return_ bool   AutomaticallyEject( void ) noexcept;
      _Check_return_ bool   Close( void ) noexcept;
      _Check_return_ bool   Dismount( void ) noexcept;
      _Check_return_ bool   Eject( void ) noexcept;
      _Check_return_ HANDLE GetHandle( void ) const noexcept;
      _Check_return_ bool   GetType( __out UINT& drive_type ) noexcept;
      _Check_return_ bool   Load( void ) noexcept;
      _Check_return_ bool   Lock( void ) noexcept;
      _Check_return_ bool   Open( __in wchar_t const drive_letter ) noexcept;
      _Check_return_ bool   PreventRemoval( __in const bool prevent_removal = true ) noexcept;
      _Check_return_ bool   Unlock( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // VOLUME_CLASS_HEADER
