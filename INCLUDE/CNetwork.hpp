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
** $Workfile: cnetwork.hpp $
** $Revision: 20 $
** $Modtime: 6/26/01 11:03a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( NETWORK_CLASS_HEADER )

#define NETWORK_CLASS_HEADER

#if ! defined( WNNC_NET_CLEARCASE )
#define WNNC_NET_CLEARCASE 0x00160000
#endif // WNNC_NET_CLEARCASE

class CNetwork
{
   protected:

      std::unique_ptr<wchar_t[]> m_WideMachineName;
      std::unique_ptr<wchar_t[]> m_WideDoubleBackslashPreceededMachineName;

      std::wstring m_MachineName;
      std::wstring m_FriendlyMachineName;

      DWORD m_ErrorCode{ 0 };

      std::unique_ptr<uint8_t[]> m_PortBuffer;
      DWORD   m_NumberOfPorts{ 0 };
      DWORD   m_PortNumber{ 0 };

      virtual void m_Initialize( void ) noexcept;

   public:

      enum class NetworkType : uint32_t//  From WINNETWK.H
      {
         MSNet      = WNNC_NET_MSNET,
         LanMan     = WNNC_NET_LANMAN,
         Netware    = WNNC_NET_NETWARE,
         Vines      = WNNC_NET_VINES,
         TenNet     = WNNC_NET_10NET,
         Locus      = WNNC_NET_LOCUS,
         SunPcNFS   = WNNC_NET_SUN_PC_NFS,
         LanStep    = WNNC_NET_LANSTEP,
         NineTiles  = WNNC_NET_9TILES,
         Lantastic  = WNNC_NET_LANTASTIC,
         AS400      = WNNC_NET_AS400,
         FtpNFS     = WNNC_NET_FTP_NFS,
         PathWorks  = WNNC_NET_PATHWORKS,
         LifeNet    = WNNC_NET_LIFENET,
         PowerLan   = WNNC_NET_POWERLAN,
         BWNFS      = WNNC_NET_BWNFS,
         Cogent     = WNNC_NET_COGENT,
         Farallon   = WNNC_NET_FARALLON,
         AppleTalk  = WNNC_NET_APPLETALK,
         Intergraph = WNNC_NET_INTERGRAPH,
         SymfoNet   = WNNC_NET_SYMFONET,
         ClearCase  = WNNC_NET_CLEARCASE
      };

      CNetwork(_In_ CNetwork const&) = delete;
      _Check_return_ CNetwork& operator=(_In_ CNetwork const&) = delete;
      CNetwork(_In_ std::wstring_view machine_name = { nullptr, 0 });
      virtual ~CNetwork();

      virtual _Check_return_ bool AbortShutdown( void ) noexcept;
      virtual void    Close( void ) noexcept;
      virtual _Check_return_ bool  EnumeratePorts( void ) noexcept;
      inline constexpr _Check_return_ DWORD GetErrorCode(void) const noexcept { return(m_ErrorCode); };
      virtual _Check_return_ LPCWSTR GetMachineName( void ) const noexcept;
      virtual void    GetFriendlyMachineName( _Out_ std::wstring& name ) const noexcept;
      virtual _Check_return_ bool GetNext( _Inout_ CPortInformation& port ) noexcept;
      virtual _Check_return_ bool GetTime(_Inout_ CTime& machine_time ) noexcept;
      virtual _Check_return_ bool GetTime(_Inout_ CSystemTime& machine_time ) noexcept;
      virtual _Check_return_ bool IsRebootable( void ) noexcept;
      virtual void    Open(_In_ std::wstring_view machine_name = { nullptr, 0 } ) noexcept;
      virtual _Check_return_ bool SetPrivilege( _In_z_ LPCWSTR privilege_name, _In_ bool const add_privilege = true ) noexcept;
      virtual _Check_return_ bool Shutdown( _In_ bool const reboot                            = true,
                                            _In_ bool const force_applications_to_close       = true,
                                      _In_opt_z_ LPCWSTR message_to_display                = nullptr,
                                            _In_ DWORD const  number_of_seconds_before_shutdown = 0 ) noexcept;
      operator LPCWSTR() const noexcept { return( m_WideMachineName.get() ); }
      operator LPWSTR()  const noexcept { return( m_WideMachineName.get() ); }

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // NETWORK_CLASS_HEADER
