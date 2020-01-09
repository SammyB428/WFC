/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2015, Samuel R. Blackburn
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
** $Workfile: cras.hpp $
** $Revision: 19 $
** $Modtime: 6/26/01 11:03a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( REMOTE_ACCESS_SERVICE_CLASS_HEADER )

#define REMOTE_ACCESS_SERVICE_CLASS_HEADER

struct WFC_RAS_THUNK
{
   HINSTANCE dll_instance_handle;
   TCHAR     dll_name[ MAX_PATH ];
   DWORD (APIENTRY * RasDialA)( LPRASDIALEXTENSIONS, LPSTR, LPRASDIALPARAMSA, DWORD, LPVOID, LPHRASCONN );
   DWORD (APIENTRY * RasDialW)( LPRASDIALEXTENSIONS, LPWSTR, LPRASDIALPARAMSW, DWORD, LPVOID, LPHRASCONN );
   DWORD (APIENTRY * RasEnumConnectionsA)( LPRASCONNA, LPDWORD, LPDWORD );
   DWORD (APIENTRY * RasEnumConnectionsW)( LPRASCONNW, LPDWORD, LPDWORD );
   DWORD (APIENTRY * RasEnumEntriesA)( LPSTR, LPSTR, LPRASENTRYNAMEA, LPDWORD, LPDWORD );
   DWORD (APIENTRY * RasEnumEntriesW)( LPWSTR, LPWSTR, LPRASENTRYNAMEW, LPDWORD, LPDWORD );
   DWORD (APIENTRY * RasGetConnectStatusA)( HRASCONN, LPRASCONNSTATUSA );
   DWORD (APIENTRY * RasGetConnectStatusW)( HRASCONN, LPRASCONNSTATUSW );
   DWORD (APIENTRY * RasGetErrorStringA)( UINT, LPSTR, DWORD );
   DWORD (APIENTRY * RasGetErrorStringW)( UINT, LPWSTR, DWORD );
   DWORD (APIENTRY * RasHangUpA)( HRASCONN );
   DWORD (APIENTRY * RasHangUpW)( HRASCONN );
   DWORD (APIENTRY * RasGetProjectionInfoA)( HRASCONN, RASPROJECTION, LPVOID, LPDWORD );
   DWORD (APIENTRY * RasGetProjectionInfoW)( HRASCONN, RASPROJECTION, LPVOID, LPDWORD );
   DWORD (APIENTRY * RasCreatePhonebookEntryA)( HWND, LPSTR );
   DWORD (APIENTRY * RasCreatePhonebookEntryW)( HWND, LPWSTR );
   DWORD (APIENTRY * RasEditPhonebookEntryA)( HWND, LPSTR, LPSTR );
   DWORD (APIENTRY * RasEditPhonebookEntryW)( HWND, LPWSTR, LPWSTR );
   DWORD (APIENTRY * RasSetEntryDialParamsA)( LPSTR, LPRASDIALPARAMSA, BOOL );
   DWORD (APIENTRY * RasSetEntryDialParamsW)( LPWSTR, LPRASDIALPARAMSW, BOOL );
   DWORD (APIENTRY * RasGetEntryDialParamsA)( LPSTR, LPRASDIALPARAMSA, LPBOOL );
   DWORD (APIENTRY * RasGetEntryDialParamsW)( LPWSTR, LPRASDIALPARAMSW, LPBOOL );
   DWORD (APIENTRY * RasEnumDevicesA)( LPRASDEVINFOA, LPDWORD, LPDWORD );
   DWORD (APIENTRY * RasEnumDevicesW)( LPRASDEVINFOW, LPDWORD, LPDWORD );
   DWORD (APIENTRY * RasGetCountryInfoA)( LPRASCTRYINFOA, LPDWORD );
   DWORD (APIENTRY * RasGetCountryInfoW)( LPRASCTRYINFOW, LPDWORD );
   DWORD (APIENTRY * RasGetEntryPropertiesA)( LPSTR, LPSTR, LPRASENTRYA, LPDWORD, LPBYTE, LPDWORD );
   DWORD (APIENTRY * RasGetEntryPropertiesW)( LPWSTR, LPWSTR, LPRASENTRYW, LPDWORD, LPBYTE, LPDWORD );
   DWORD (APIENTRY * RasSetEntryPropertiesA)( LPSTR, LPSTR, LPRASENTRYA, DWORD, LPBYTE, DWORD );
   DWORD (APIENTRY * RasSetEntryPropertiesW)( LPWSTR, LPWSTR, LPRASENTRYW, DWORD, LPBYTE, DWORD );
   DWORD (APIENTRY * RasRenameEntryA)( LPSTR, LPSTR, LPSTR );
   DWORD (APIENTRY * RasRenameEntryW)( LPWSTR, LPWSTR, LPWSTR );
   DWORD (APIENTRY * RasDeleteEntryA)( LPSTR, LPSTR );
   DWORD (APIENTRY * RasDeleteEntryW)( LPWSTR, LPWSTR );
   DWORD (APIENTRY * RasValidateEntryNameA)( LPSTR, LPSTR );
   DWORD (APIENTRY * RasValidateEntryNameW)( LPWSTR, LPWSTR );

#if ( WINVER >= 0x401 )

   DWORD (APIENTRY * RasGetSubEntryHandleA)( HRASCONN, DWORD, LPHRASCONN );
   DWORD (APIENTRY * RasGetSubEntryHandleW)( HRASCONN, DWORD, LPHRASCONN );
   DWORD (APIENTRY * RasGetCredentialsA)( LPSTR, LPSTR, LPRASCREDENTIALSA);
   DWORD (APIENTRY * RasGetCredentialsW)( LPWSTR, LPWSTR, LPRASCREDENTIALSW );
   DWORD (APIENTRY * RasSetCredentialsA)( LPSTR, LPSTR, LPRASCREDENTIALSA, BOOL );
   DWORD (APIENTRY * RasSetCredentialsW)( LPWSTR, LPWSTR, LPRASCREDENTIALSW, BOOL );
   DWORD (APIENTRY * RasConnectionNotificationA)( HRASCONN, HANDLE, DWORD );
   DWORD (APIENTRY * RasConnectionNotificationW)( HRASCONN, HANDLE, DWORD );
   DWORD (APIENTRY * RasGetSubEntryPropertiesA)( LPSTR, LPSTR, DWORD, LPRASSUBENTRYA, LPDWORD, LPBYTE, LPDWORD );
   DWORD (APIENTRY * RasGetSubEntryPropertiesW)( LPWSTR, LPWSTR, DWORD, LPRASSUBENTRYW, LPDWORD, LPBYTE, LPDWORD );
   DWORD (APIENTRY * RasSetSubEntryPropertiesA)( LPSTR, LPSTR, DWORD, LPRASSUBENTRYA, DWORD, LPBYTE, DWORD );
   DWORD (APIENTRY * RasSetSubEntryPropertiesW)( LPWSTR, LPWSTR, DWORD, LPRASSUBENTRYW, DWORD, LPBYTE, DWORD );
   DWORD (APIENTRY * RasGetAutodialAddressA)( LPSTR, LPDWORD, LPRASAUTODIALENTRYA, LPDWORD, LPDWORD );
   DWORD (APIENTRY * RasGetAutodialAddressW)( LPWSTR, LPDWORD, LPRASAUTODIALENTRYW, LPDWORD, LPDWORD);
   DWORD (APIENTRY * RasSetAutodialAddressA)( LPSTR, DWORD, LPRASAUTODIALENTRYA, DWORD, DWORD );
   DWORD (APIENTRY * RasSetAutodialAddressW)( LPWSTR, DWORD, LPRASAUTODIALENTRYW, DWORD, DWORD );
   DWORD (APIENTRY * RasEnumAutodialAddressesA)( LPSTR *, LPDWORD, LPDWORD );
   DWORD (APIENTRY * RasEnumAutodialAddressesW)( LPWSTR *, LPDWORD, LPDWORD );
   DWORD (APIENTRY * RasGetAutodialEnableA)( DWORD, LPBOOL );
   DWORD (APIENTRY * RasGetAutodialEnableW)( DWORD, LPBOOL );
   DWORD (APIENTRY * RasSetAutodialEnableA)( DWORD, BOOL );
   DWORD (APIENTRY * RasSetAutodialEnableW)( DWORD, BOOL );
   DWORD (APIENTRY * RasGetAutodialParamA)( DWORD, LPVOID, LPDWORD );
   DWORD (APIENTRY * RasGetAutodialParamW)( DWORD, LPVOID, LPDWORD );
   DWORD (APIENTRY * RasSetAutodialParamA)( DWORD, LPVOID, DWORD );
   DWORD (APIENTRY * RasSetAutodialParamW)( DWORD, LPVOID, DWORD );

#endif // ( WINVER >= 0x401 )

};

#pragma warning ( disable : 4097 )

/*
** We have to declare a typedef because of the funcky way Microsoft declared RASAMB in RAS.H
*/

typedef RASAMB _rasamb;

class CRemoteAccessServiceAuthenticationMessageBlock : public _rasamb
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CRemoteAccessServiceAuthenticationMessageBlock();
      CRemoteAccessServiceAuthenticationMessageBlock( __in RASAMB const * source );
      CRemoteAccessServiceAuthenticationMessageBlock( __in CRemoteAccessServiceAuthenticationMessageBlock const& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRemoteAccessServiceAuthenticationMessageBlock();

      void  Copy( __in RASAMB const * source ) noexcept;
      void  Copy( __in CRemoteAccessServiceAuthenticationMessageBlock const& source ) noexcept;
      _Check_return_ BYTE  GetLana( void ) const noexcept;
      inline constexpr _Check_return_ DWORD GetErrorCode(void) const noexcept { return(m_ErrorCode); };
      void  GetErrorString( __out std::wstring& return_string ) const noexcept;
      CRemoteAccessServiceAuthenticationMessageBlock& operator=( __in CRemoteAccessServiceAuthenticationMessageBlock const& source ) noexcept;
};

/*
** We have to declare a typedef because of the funcky way Microsoft declared RASAMB in RAS.H
*/

typedef RASPPPNBF _raspppnbf;

class CRemoteAccessServiceNetBEUIFramer : public _raspppnbf
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CRemoteAccessServiceNetBEUIFramer();
      CRemoteAccessServiceNetBEUIFramer( __in RASPPPNBF const * source );
      CRemoteAccessServiceNetBEUIFramer( __in CRemoteAccessServiceNetBEUIFramer const& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRemoteAccessServiceNetBEUIFramer();

      void  Copy( __in RASPPPNBF const * source ) noexcept;
      void  Copy( __in CRemoteAccessServiceNetBEUIFramer const& source ) noexcept;
      _Check_return_ BYTE  GetLana( void ) const noexcept;
      inline constexpr _Check_return_ DWORD GetErrorCode(void) const noexcept { return(m_ErrorCode); };
      _Check_return_ DWORD GetNetBiosErrorCode( void ) const noexcept;
      void GetErrorString( __out std::wstring& return_string ) const noexcept;
      void GetWorkstationName( __out std::wstring& return_string ) const noexcept;
      CRemoteAccessServiceNetBEUIFramer& operator=( __in CRemoteAccessServiceNetBEUIFramer const& source ) noexcept;
};

/*
** We have to declare a typedef because of the funcky way Microsoft declared RASAMB in RAS.H
*/

typedef RASPPPIPX _raspppipx;

class CRemoteAccessServiceInternetworkPacketExchange : public _raspppipx
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CRemoteAccessServiceInternetworkPacketExchange();
      CRemoteAccessServiceInternetworkPacketExchange( __in RASPPPIPX const * source );
      CRemoteAccessServiceInternetworkPacketExchange( __in CRemoteAccessServiceInternetworkPacketExchange const& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRemoteAccessServiceInternetworkPacketExchange();

      void Copy( __in RASPPPIPX const * source ) noexcept;
      void Copy( __in CRemoteAccessServiceInternetworkPacketExchange const& source ) noexcept;
      inline constexpr _Check_return_ DWORD GetErrorCode(void) const noexcept { return(m_ErrorCode); };
      void GetIPXAddress( __out std::wstring& return_string ) const noexcept;
      CRemoteAccessServiceInternetworkPacketExchange& operator=( __in CRemoteAccessServiceInternetworkPacketExchange const& source ) noexcept;
};

/*
** We have to declare a typedef because of the funcky way Microsoft declared RASAMB in RAS.H
*/

typedef RASPPPIP _raspppip;

class CRemoteAccessServiceInternetProtocol : public _raspppip
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CRemoteAccessServiceInternetProtocol();
      CRemoteAccessServiceInternetProtocol( __in RASPPPIP const * source );
      CRemoteAccessServiceInternetProtocol( __in CRemoteAccessServiceInternetProtocol const& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRemoteAccessServiceInternetProtocol();

      void Copy( __in RASPPPIP const * source ) noexcept;
      void Copy( __in CRemoteAccessServiceInternetProtocol const& source ) noexcept;
      inline constexpr _Check_return_ DWORD GetErrorCode(void) const noexcept { return(m_ErrorCode); };
      void GetIPAddress( __inout std::wstring& return_string ) const noexcept;

#if ! defined( WINNT35COMPATIBLE )

      void GetServerIPAddress( __inout std::wstring& return_string ) const noexcept;

#endif // WINNT35COMPATIBLE

      CRemoteAccessServiceInternetProtocol& operator=( CRemoteAccessServiceInternetProtocol const& source ) noexcept;
};

#pragma warning ( default : 4097 )

class CRemoteAccessService
{
   private:

      void m_Initialize( void ) noexcept;
      void m_InitializeThunk( void ) noexcept;
      void m_GetConnectionsIntoMemory( __out LPRASCONN& return_value, __out DWORD& number_of_entries ) noexcept;

   protected:

       std::wstring m_Name;

       WFC_RAS_THUNK_P m_Thunk_p{ nullptr };

       bool m_AutomaticallyClose{ false };

       LONG m_ErrorCode{ 0 };

       DWORD m_NotifierType{ 0 };

       LPVOID m_ConnectionCallbackFunctionPointer{ nullptr };

       HRASCONN m_ConnectionHandle{ nullptr };

      RASCONNSTATUS m_ConnectionStatus;

      RASDIALEXTENSIONS m_DialExtensions;

      DWORD m_UnusedPadBytesBecauseMicrosoftChangesTheSizeOfStructures[16]{ 0 };

   public:

      enum Protocols
      {
         protocolAuthenticationMessageBlock = RASP_Amb,
         protocolNetBEUIFramer              = RASP_PppNbf,
         protocolInternetworkPacketExchange = RASP_PppIpx,
         protocolInternetProtocol           = RASP_PppIp
      };

      enum DialOptions
      {
         dialUsePrefixAndSuffix        = RDEOPT_UsePrefixSuffix,
         dialAcceptPausedStates        = RDEOPT_PausedStates,
         dialIgnoreModemSpeaker        = RDEOPT_IgnoreModemSpeaker,
         dialSetModemSpeaker           = RDEOPT_SetModemSpeaker,
         dialIgnoreSoftwareCompression = RDEOPT_IgnoreSoftwareCompression,
         dialSetSoftwareCompression    = RDEOPT_SetSoftwareCompression
      };

      CRemoteAccessService(CRemoteAccessService const&) = delete;
      CRemoteAccessService& operator=(CRemoteAccessService const&) = delete;

      CRemoteAccessService();
      CRemoteAccessService( __in DWORD input_buffer_size );
      CRemoteAccessService( __in DWORD input_buffer_size, __in DWORD output_buffer_size );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRemoteAccessService();

      /*
      ** The Win32 API
      */

      virtual BOOL     Dial( __in_z LPCTSTR who_to_call ) noexcept;
      virtual HRASCONN GetConnection( __in_z LPCTSTR name_of_connection ) noexcept;
      virtual BOOL     GetConnections( __inout std::vector<std::wstring>& connections ) noexcept;
      virtual _Check_return_ BOOL     GetConnectionStatus( void ) noexcept;
      virtual _Check_return_ LONG     GetErrorCode( void ) const noexcept;
      virtual _Check_return_ BOOL     GetErrorString( __inout std::wstring& error_string ) noexcept;
      virtual void     GetName( __inout std::wstring& name ) const noexcept;
      virtual BOOL     GetPhoneBookEntries( __inout std::vector<std::wstring>& phone_book_entries ) noexcept;
      virtual BOOL     GetProtocolInformation( __inout CRemoteAccessServiceAuthenticationMessageBlock& data_to_get ) noexcept;
      virtual BOOL     GetProtocolInformation( __inout CRemoteAccessServiceNetBEUIFramer& data_to_get ) noexcept;
      virtual BOOL     GetProtocolInformation( __inout CRemoteAccessServiceInternetworkPacketExchange& data_to_get ) noexcept;
      virtual BOOL     GetProtocolInformation( __inout CRemoteAccessServiceInternetProtocol& data_to_get ) noexcept;
      virtual BOOL     HangUp( __in_z_opt LPCTSTR name_of_connection = nullptr ) noexcept;
      virtual BOOL     IsConnected( void ) noexcept;
      virtual BOOL     SetAutomaticallyClose( __in BOOL auto_close ) noexcept; // Returns replaces setting
      virtual LPVOID   SetConnectionCallback( __in LPVOID RasDialFunc1_function_pointer ) noexcept;
      virtual HWND     SetConnectionCallbackWindow( __in HWND window_handle ) noexcept;
      virtual DWORD    SetDialOptions( __in DWORD dial_options ) noexcept;

      /*
      ** API's to make life a little easier 
      */

      virtual BOOL Open( __in_z LPCTSTR who_to_call ) noexcept;
      virtual BOOL Close( __in_z_opt LPCTSTR name_of_connection = nullptr ) noexcept; // returns when RAS really hangs up

      /*
      ** Functions related to RAS
      */

      static BOOL EnableLogging( __in BOOL enable_logging, __in_z_opt LPCTSTR machine_name = nullptr ) noexcept; // Creates DEVICE.LOG
      static BOOL GetKeepConnectionsAfterLogoff( __in_z_opt LPCTSTR machine_name = nullptr ) noexcept;
      static BOOL SetKeepConnectionsAfterLogoff( __in BOOL keep_connections = TRUE, __in_z_opt LPCTSTR machine_name = nullptr ) noexcept;
};

#endif // REMOTE_ACCESS_SERVICE_CLASS_HEADER
