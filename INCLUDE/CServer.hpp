/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2014, Samuel R. Blackburn
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
** $Workfile: cserver.hpp $
** $Revision: 11 $
** $Modtime: 6/26/01 11:04a $
*/

#if ! defined( SERVER_CLASS_HEADER )

#define SERVER_CLASS_HEADER

class CServer : public CNetwork
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      std::wstring m_Comment;
      std::wstring m_UserPath;
      std::wstring m_Domain;

      bool m_Retrieved102{ false };
      bool m_Retrieved503{ false };
      bool m_Retrieved599{ false };

      DWORD m_MajorVersion{ 0 };
      DWORD m_MinorVersion{ 0 };
      DWORD m_NumberOfUsers{ 0 };
      DWORD m_NumberOfOpens{ 0 };
      DWORD m_Platform{ 0 };
      DWORD m_Type{ 0 };
      DWORD m_Users{ 0 };

      virtual void m_Get_102_Data( void ) noexcept;
      virtual void m_Get_503_Data( void ) noexcept;
      virtual void m_Get_599_Data( void ) noexcept;

   public:


       CServer(__in CServer const&) = delete;
       _Check_return_ CServer& operator=(__in CServer const&) = delete;
       
       CServer();
      CServer( _In_opt_z_ LPCWSTR machine_name );
      virtual ~CServer();

      void  Close( void ) noexcept override;
      virtual void  GetComment( _Out_ std::wstring& comment ) noexcept;
      virtual void  GetDomain(_Out_ std::wstring& domain ) noexcept;
      virtual void  GetName(_Out_ std::wstring& name ) noexcept;
      virtual _Check_return_ DWORD GetMajorVersion( void ) noexcept;
      virtual _Check_return_ DWORD GetMinorVersion( void ) noexcept;
      virtual _Check_return_ DWORD GetNumberOfOpens( void ) noexcept;
      virtual _Check_return_ DWORD GetNumberOfUsers( void ) noexcept;
      virtual _Check_return_ DWORD GetPlatform( void ) noexcept;
      virtual void  GetPlatformName(_Out_ std::wstring& name ) noexcept;
      virtual void  GetPlatformNameAndVersion(_Out_ std::wstring& name ) noexcept;
      virtual _Check_return_ DWORD GetType( void ) noexcept;
      virtual void  GetUserPath(_Out_ std::wstring& path ) noexcept;
      virtual _Check_return_ DWORD GetUsers( void ) noexcept;
};

#endif // SERVER_CLASS_HEADER
