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
** $Workfile: ServiceConfiguration.hpp $
** $Revision: 12 $
** $Modtime: 6/26/01 11:07a $
*/

#if ! defined( SERVICE_CONFIGURATION_CLASS_HEADER )

#define SERVICE_CONFIGURATION_CLASS_HEADER

class CServiceConfigurationA
{
   protected:

      DWORD   m_TypeOfService{ 0 };
      DWORD   m_WhenToStart{ 0 };
      DWORD   m_ErrorControl{ 0 };
      DWORD   m_Tag{ 0 };
      std::string m_NameOfExecutableFile;
      std::string m_LoadOrderGroup;
      std::string m_StartName;
      std::string m_DisplayName;

      std::vector<std::string> m_Dependencies;

   public:

      CServiceConfigurationA();
      CServiceConfigurationA( __in _QUERY_SERVICE_CONFIGA const& source );
      CServiceConfigurationA( __in _QUERY_SERVICE_CONFIGA const * source );
      CServiceConfigurationA( __in CServiceConfigurationA const& source );
      CServiceConfigurationA( __in CServiceConfigurationA const * source );
      virtual ~CServiceConfigurationA();

      virtual void  Copy( __in _QUERY_SERVICE_CONFIGA const&  source ) noexcept;
      virtual void  Copy( __in _QUERY_SERVICE_CONFIGA const * source ) noexcept;
      virtual void  Copy( __in CServiceConfigurationA const&  source ) noexcept;
      virtual void  Copy( __in CServiceConfigurationA const * source ) noexcept;
      virtual void  Empty( void ) noexcept;
      virtual void  GetDependencies( __out std::vector<std::string>& dependencies ) const noexcept;
      virtual void  GetDisplayName( __out std::string& display_name ) const noexcept;
      virtual _Check_return_ DWORD GetErrorControl( void ) const noexcept;
      virtual void  GetLoadOrderGroup( __out std::string& load_order_group ) const noexcept;
      virtual void  GetNameOfExecutableFile( __out std::string& name_of_executable ) const noexcept;
      virtual void  GetStartName( __out std::string& start_name ) const noexcept;
      virtual _Check_return_ DWORD GetTag( void ) const noexcept;
      virtual _Check_return_ DWORD GetTypeOfService( void ) const noexcept;
      virtual _Check_return_ DWORD GetWhenToStart( void ) const noexcept;

      virtual _Check_return_ CServiceConfigurationA& operator=( __in CServiceConfigurationA const& source ) noexcept;
      virtual _Check_return_ CServiceConfigurationA& operator=( __in _QUERY_SERVICE_CONFIGA const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

class CServiceConfigurationW
{
   protected:

       DWORD   m_TypeOfService{0};
       DWORD   m_WhenToStart{0};
       DWORD   m_ErrorControl{0};
       DWORD   m_Tag{0};
      std::wstring m_NameOfExecutableFile;
      std::wstring m_LoadOrderGroup;
      std::wstring m_StartName;
      std::wstring m_DisplayName;

      std::vector<std::wstring> m_Dependencies;

   public:

      CServiceConfigurationW();
      CServiceConfigurationW( __in _QUERY_SERVICE_CONFIGW const&  source );
      CServiceConfigurationW( __in _QUERY_SERVICE_CONFIGW const * source );
      CServiceConfigurationW( __in CServiceConfigurationW const&  source );
      CServiceConfigurationW( __in CServiceConfigurationW const * source );
      virtual ~CServiceConfigurationW();

      virtual void  Copy( __in _QUERY_SERVICE_CONFIGW const&  source ) noexcept;
      virtual void  Copy( __in _QUERY_SERVICE_CONFIGW const * source ) noexcept;
      virtual void  Copy( __in CServiceConfigurationW const&  source ) noexcept;
      virtual void  Copy( __in CServiceConfigurationW const * source ) noexcept;
      virtual void  Empty( void ) noexcept;
      virtual void  GetDependencies( __out std::vector<std::wstring>& dependencies ) const noexcept;
      virtual void  GetDisplayName( __out std::wstring& display_name ) const noexcept;
      inline constexpr _Check_return_ DWORD GetErrorControl(void) const noexcept { return(m_ErrorControl); }
      virtual void  GetLoadOrderGroup( __out std::wstring& load_order_group ) const noexcept;
      virtual void  GetNameOfExecutableFile( __out std::wstring& name_of_executable ) const noexcept;
      virtual void  GetStartName( __out std::wstring& start_name ) const noexcept;
      inline constexpr _Check_return_ DWORD GetTag(void) const noexcept { return(m_Tag); }
      inline constexpr _Check_return_ DWORD GetTypeOfService(void) const noexcept { return(m_TypeOfService); }
      inline constexpr _Check_return_ DWORD GetWhenToStart(void) const noexcept { return(m_WhenToStart); }

      virtual _Check_return_ CServiceConfigurationW& operator=( __in CServiceConfigurationW const& source ) noexcept;
      virtual _Check_return_ CServiceConfigurationW& operator=( __in _QUERY_SERVICE_CONFIGW const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};


#if defined( UNICODE )
#define CServiceConfiguration CServiceConfigurationW
#else
#define CServiceConfiguration CServiceConfigurationA
#endif // UNICODE

#endif // SERVICE_CONFIGURATION_CLASS_HEADER
