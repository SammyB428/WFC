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
** $Workfile: csvcmgr.hpp $
** $Revision: 19 $
** $Modtime: 6/26/01 11:04a $
*/

#if ! defined( SERVICE_MANAGER_CLASS_HEADER )

#define SERVICE_MANAGER_CLASS_HEADER

class CServiceNameAndStatusA : public _ENUM_SERVICE_STATUSA
{
   public:

      CServiceNameAndStatusA();
      virtual ~CServiceNameAndStatusA();
      virtual void Copy( _In_ _ENUM_SERVICE_STATUSA const * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CServiceNameAndStatusW : public _ENUM_SERVICE_STATUSW
{
   public:

      CServiceNameAndStatusW();
      virtual ~CServiceNameAndStatusW();
      virtual void Copy(_In_ _ENUM_SERVICE_STATUSW const * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#if defined( UNICODE )
#define CServiceNameAndStatus CServiceNameAndStatusW
#else
#define CServiceNameAndStatus CServiceNameAndStatusA
#endif // UNICODE

class CServiceControlManager
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      SC_HANDLE m_ManagerHandle{ nullptr };

      SC_LOCK m_DatabaseLockHandle{ nullptr };

      std::unique_ptr<uint8_t[]> m_Buffer;

      uint32_t m_BufferSize{ 0 };
      uint32_t m_CurrentEntryNumber{ 0 };
      uint32_t m_ErrorCode{ 0 };
      uint32_t m_NumberOfEntries{ 0 };

      std::wstring m_MachineName;
      std::wstring m_ErrorMessage;

   public:

       CServiceControlManager(_In_ CServiceControlManager const&) = delete;
       CServiceControlManager& operator=(_In_ CServiceControlManager const&) = delete;

       static inline constexpr uint32_t const QueryStatusPermissions(void) noexcept
       {
           return(READ_CONTROL | SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE);
       }

       CServiceControlManager();
      virtual ~CServiceControlManager();

      void Close( void ) noexcept;
      _Check_return_ bool  Continue( _In_z_ wchar_t const * service_name ) noexcept;
      _Check_return_ bool  EnableInteractiveServices( _In_ bool enable_interactive_services = true ) noexcept;
      _Check_return_ bool  EnumerateStatus( _In_ uint32_t state = SERVICE_ACTIVE, _In_ uint32_t type = SERVICE_WIN32 ) noexcept;
      _Check_return_ bool  GetConfiguration(_In_z_ wchar_t const * service_name, __out CServiceConfiguration& configuration ) noexcept;
      _Check_return_ bool  GetDependencies(_In_z_ wchar_t const * service_name, __out std::vector<std::wstring>& dependencies ) noexcept;
      _Check_return_ bool  GetDisplayName(_In_z_ wchar_t const * real_name, __out std::wstring& friendly_name ) noexcept;
      _Check_return_ uint32_t GetErrorCode( void ) const noexcept;
                    void  GetErrorMessage( __inout std::wstring& error_message ) const noexcept;
      _Check_return_ SC_HANDLE GetHandle( void ) const noexcept;
      _Check_return_ bool  GetKeyName(_In_z_ wchar_t const * friendly_name, __out std::wstring& real_name ) noexcept;
      _Check_return_ bool  GetNext( __out CServiceNameAndStatus& status ) noexcept;
      _Check_return_ bool  Install(_In_z_ wchar_t const * service_name, _In_z_ wchar_t const * friendly_name, __in_z_opt wchar_t const * name_of_executable_file = nullptr ) noexcept;
      _Check_return_ bool  IsDatabaseLocked( __out std::wstring& who_locked_it, __out CTimeSpan& how_long_it_has_been_locked ) noexcept;
      _Check_return_ bool  LockDatabase( void ) noexcept;
      _Check_return_ bool  Open( __in uint32_t const what_to_open  = SC_MANAGER_ALL_ACCESS,
                          __in_z_opt wchar_t const * database_name = nullptr,
                          __in_z_opt wchar_t const * machine_name  = nullptr ) noexcept;
      _Check_return_ bool  Pause(_In_z_ wchar_t const * service_name ) noexcept;
      _Check_return_ bool  Remove(_In_z_ wchar_t const * service_name ) noexcept;
      _Check_return_ bool  WaitForStop(_In_z_ wchar_t const * service_name) noexcept;
      _Check_return_ bool  SetConfiguration(_In_z_ wchar_t const * service_name,
                                      _In_ uint32_t const  when_to_start           = SERVICE_NO_CHANGE,
                                      _In_ uint32_t const  type_of_service         = SERVICE_NO_CHANGE,
                                      _In_ uint32_t const  error_control           = SERVICE_NO_CHANGE,
                                      __in_z_opt wchar_t const * name_of_executable_file = nullptr,
                                      __in_z_opt wchar_t const * load_order_group        = nullptr,
                                      __in_z_opt wchar_t const * dependencies            = nullptr,
                                      __in_z_opt wchar_t const * start_name              = nullptr,
                                      __in_z_opt wchar_t const * password                = nullptr,
                                      __in_z_opt wchar_t const * display_name            = nullptr ) noexcept;
      _Check_return_ bool  Start(_In_z_ wchar_t const * service_name, _In_ uint32_t const service_argc = 0, __in_ecount_z_opt( service_argc ) wchar_t const * * service_argv = nullptr ) noexcept;
      _Check_return_ bool  Stop(_In_z_ wchar_t const * service_name ) noexcept;
      _Check_return_ bool  UnlockDatabase( void ) noexcept;
};

#endif // SERVICE_MANAGER_CLASS_HEADER
