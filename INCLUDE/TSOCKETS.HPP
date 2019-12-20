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
** $Workfile: tsockets.hpp $
** $Revision: 13 $
** $Modtime: 6/26/01 11:07a $
*/

#if ! defined( TALKING_SOCKET_CLASS_HEADER )

#define TALKING_SOCKET_CLASS_HEADER

class CTalkingSocket : public CSimpleSocket
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CTalkingSocket(CTalkingSocket const&) = delete;
      CTalkingSocket& operator=(CTalkingSocket const&) = delete;
       
      CTalkingSocket() noexcept;
      CTalkingSocket(_In_ std::wstring_view address, __in short const p ) noexcept;
      CTalkingSocket(_In_ std::wstring_view host, __in std::wstring const& p ) noexcept;

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CTalkingSocket();

      _Check_return_ bool Open( void ) noexcept override;
      virtual _Check_return_ bool Open( _In_ std::wstring_view address, _In_ std::wstring const& port_name ) noexcept;
      virtual _Check_return_ bool Open(_In_ std::wstring_view address, _In_ short const p_number ) noexcept;
      _Check_return_ bool Open(_In_ std::filesystem::path const& name, _In_ UINT const port_number = 23 ) noexcept override;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dc ) const;

#endif // _DEBUG
};

#endif // TALKING_SOCKET_CLASS_HEADER
