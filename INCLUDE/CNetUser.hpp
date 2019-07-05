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
** $Workfile: cnetuser.hpp $
** $Revision: 17 $
** $Modtime: 6/26/01 11:03a $
*/

#if ! defined( NETWORK_USER_CLASS_HEADER )

#define NETWORK_USER_CLASS_HEADER

class CNetworkUserInformation
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CNetworkUserInformation();
      CNetworkUserInformation( __in USER_INFO_0  const * information_p );
      CNetworkUserInformation( __in USER_INFO_1  const * information_p );
      CNetworkUserInformation( __in USER_INFO_2  const * information_p );
      CNetworkUserInformation( __in USER_INFO_3  const * information_p );
      CNetworkUserInformation( __in USER_INFO_10 const * information_p );
      CNetworkUserInformation( __in USER_INFO_11 const * information_p );
      CNetworkUserInformation( __in USER_INFO_20 const * information_p );
      CNetworkUserInformation( __in USER_INFO_21 const * information_p );
      CNetworkUserInformation( __in USER_INFO_22 const * information_p );
      CNetworkUserInformation( __in CNetworkUserInformation const&  source );
      CNetworkUserInformation( __in CNetworkUserInformation const * source );

      virtual ~CNetworkUserInformation();

      enum _Priveleges
      {
         privilegeGuest         = USER_PRIV_GUEST,
         privilegeUser          = USER_PRIV_USER,
         privilegeAdministrator = USER_PRIV_ADMIN
      };

      std::wstring Name;
      std::wstring Password;
      std::vector<uint8_t>   EncryptedPassword;
      CTimeSpan    PasswordAge;
      DWORD        Privileges{ 0 };
      std::wstring HomeDirectory;
      std::wstring Comment;
      DWORD        Flags{ 0 };
      std::wstring ScriptPath;
      DWORD        AuthenticationFlags{ 0 };
      std::wstring FullName;
      std::wstring UserComment;
      std::wstring Parameters;
      std::wstring Workstations;
      CTime        LastLogon;
      CTime        LastLogoff;
      CTime        AccountExpires;
      DWORD        MaximumStorage{ 0 };
      DWORD        UnitsPerWeek{ 0 };
      std::vector<uint8_t> LogonHours;
      DWORD        BadPasswordCount{ 0 };
      DWORD        NumberOfLogons{ 0 };
      std::wstring LogonServer;
      DWORD        CountryCode{ 0 };
      DWORD        CodePage{ 0 };
      DWORD        UserID{ 0 };
      DWORD        PrimaryGroupID{ 0 };
      std::wstring HomeDirectoryDrive;
      std::wstring Profile;
      DWORD        PasswordHasExpired{ 0 };

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      virtual void Copy( __in USER_INFO_0  const * source ) noexcept;
      virtual void Copy( __in USER_INFO_1  const * source ) noexcept;
      virtual void Copy( __in USER_INFO_2  const * source ) noexcept;
      virtual void Copy( __in USER_INFO_3  const * source ) noexcept;
      virtual void Copy( __in USER_INFO_10 const * source ) noexcept;
      virtual void Copy( __in USER_INFO_11 const * source ) noexcept;
      virtual void Copy( __in USER_INFO_20 const * source ) noexcept;
      virtual void Copy( __in USER_INFO_21 const * source ) noexcept;
      virtual void Copy( __in USER_INFO_22 const * source ) noexcept;
      virtual void Copy( __in CNetworkUserInformation const&  source ) noexcept;
      virtual void Copy( __in CNetworkUserInformation const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual void SetAddDefaults( void ) noexcept;
      virtual const CNetworkUserInformation& operator = ( __in CNetworkUserInformation const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

class CNetworkUsers : public CNetwork
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      /*
      ** Connection information variables
      */

      USER_INFO_0  * m_0InformationBuffer{ nullptr };
      USER_INFO_1  * m_1InformationBuffer{ nullptr };
      USER_INFO_2  * m_2InformationBuffer{ nullptr };
      USER_INFO_3  * m_3InformationBuffer{ nullptr };
      USER_INFO_10 * m_10InformationBuffer{ nullptr };

      /*
      ** File Information enumeration variables
      */

      DWORD m_0Index{ 0 };
      DWORD m_0ResumeHandle{ 0 };
      DWORD m_0CurrentEntryNumber{ 0 };
      DWORD m_0NumberOfEntriesRead{ 0 };
      DWORD m_0TotalNumberOfEntries{ 0 };

      DWORD m_1Index{ 0 };
      DWORD m_1ResumeHandle{ 0 };
      DWORD m_1CurrentEntryNumber{ 0 };
      DWORD m_1NumberOfEntriesRead{ 0 };
      DWORD m_1TotalNumberOfEntries{ 0 };

      DWORD m_2Index{ 0 };
      DWORD m_2ResumeHandle{ 0 };
      DWORD m_2CurrentEntryNumber{ 0 };
      DWORD m_2NumberOfEntriesRead{ 0 };
      DWORD m_2TotalNumberOfEntries{ 0 };

      DWORD m_3Index{ 0 };
      DWORD m_3ResumeHandle{ 0 };
      DWORD m_3CurrentEntryNumber{ 0 };
      DWORD m_3NumberOfEntriesRead{ 0 };
      DWORD m_3TotalNumberOfEntries{ 0 };

      DWORD m_10Index{ 0 };
      DWORD m_10ResumeHandle{ 0 };
      DWORD m_10CurrentEntryNumber{ 0 };
      DWORD m_10NumberOfEntriesRead{ 0 };
      DWORD m_10TotalNumberOfEntries{ 0 };

      _Check_return_ bool m_GetChunk( void ) noexcept;

   public:

       CNetworkUsers(__in CNetworkUsers const&) = delete;
       CNetworkUsers& operator=(__in CNetworkUsers const&) = delete;
       CNetworkUsers();
      CNetworkUsers( __in_z_opt LPCTSTR machine_name );
      virtual ~CNetworkUsers();

      enum _AccountTypes
      {
         accountBackupDomainController = UF_SERVER_TRUST_ACCOUNT,
         accountMachine                = UF_WORKSTATION_TRUST_ACCOUNT,
         accountInterdomain            = UF_INTERDOMAIN_TRUST_ACCOUNT
      };

      virtual bool  Add( __in CNetworkUserInformation const& user_to_add ) noexcept;
      virtual _Check_return_ BOOL ChangePassword( __in std::wstring const& user_name, __in std::wstring const& old_password, __in std::wstring const& new_password ) noexcept;
      virtual void  Close( void ) noexcept;
      virtual _Check_return_ BOOL CreateComputerAccount( __in_z_opt LPCTSTR computer_name = nullptr, __in DWORD type = accountMachine ) noexcept;
      virtual BOOL  Delete( __in CNetworkUserInformation const& user_to_delete ) noexcept;
      virtual BOOL  Delete( __in std::wstring const& user_to_delete ) noexcept;
      virtual _Check_return_ bool Enumerate( void ) noexcept;
      virtual bool  ExpirePassword( __in std::wstring const& user_name ) noexcept;
      virtual _Check_return_ DWORD GetLevel( void ) const noexcept;
      virtual _Check_return_ bool  GetNext( __inout CNetworkUserInformation& information ) noexcept;
};

#endif // NETWORK_USER_CLASS_HEADER
