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
** $Workfile: cnetuser.hpp $
** $Revision: 17 $
** $Modtime: 6/26/01 11:03a $
*/

#if ! defined( NETWORK_USER_CLASS_HEADER )

#define NETWORK_USER_CLASS_HEADER

class CNetworkUserInformation
{
   private:

      void m_Initialize( void );

   public:

      CNetworkUserInformation();
      CNetworkUserInformation( __in const USER_INFO_0  * information_p );
      CNetworkUserInformation( __in const USER_INFO_1  * information_p );
      CNetworkUserInformation( __in const USER_INFO_2  * information_p );
      CNetworkUserInformation( __in const USER_INFO_3  * information_p );
      CNetworkUserInformation( __in const USER_INFO_10 * information_p );
      CNetworkUserInformation( __in const USER_INFO_11 * information_p );
      CNetworkUserInformation( __in const USER_INFO_20 * information_p );
      CNetworkUserInformation( __in const USER_INFO_21 * information_p );
      CNetworkUserInformation( __in const USER_INFO_22 * information_p );
      CNetworkUserInformation( __in const CNetworkUserInformation&  source );
      CNetworkUserInformation( __in const CNetworkUserInformation * source );

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
      DWORD        Privileges;
      std::wstring HomeDirectory;
      std::wstring Comment;
      DWORD        Flags;
      std::wstring ScriptPath;
      DWORD        AuthenticationFlags;
      std::wstring FullName;
      std::wstring UserComment;
      std::wstring Parameters;
      std::wstring Workstations;
      CTime        LastLogon;
      CTime        LastLogoff;
      CTime        AccountExpires;
      DWORD        MaximumStorage;
      DWORD        UnitsPerWeek;
      std::vector<uint8_t>   LogonHours;
      DWORD        BadPasswordCount;
      DWORD        NumberOfLogons;
      std::wstring LogonServer;
      DWORD        CountryCode;
      DWORD        CodePage;
      DWORD        UserID;
      DWORD        PrimaryGroupID;
      std::wstring HomeDirectoryDrive;
      std::wstring Profile;
      DWORD        PasswordHasExpired;

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      virtual void Copy( __in const USER_INFO_0  * source ) noexcept;
      virtual void Copy( __in const USER_INFO_1  * source ) noexcept;
      virtual void Copy( __in const USER_INFO_2  * source ) noexcept;
      virtual void Copy( __in const USER_INFO_3  * source ) noexcept;
      virtual void Copy( __in const USER_INFO_10 * source ) noexcept;
      virtual void Copy( __in const USER_INFO_11 * source ) noexcept;
      virtual void Copy( __in const USER_INFO_20 * source ) noexcept;
      virtual void Copy( __in const USER_INFO_21 * source ) noexcept;
      virtual void Copy( __in const USER_INFO_22 * source ) noexcept;
      virtual void Copy( __in const CNetworkUserInformation&  source ) noexcept;
      virtual void Copy( __in const CNetworkUserInformation * source ) noexcept;
      virtual void Empty( void );
      virtual void SetAddDefaults( void ) noexcept;
      virtual const CNetworkUserInformation& operator = ( __in const CNetworkUserInformation& source ) noexcept;

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

      __checkReturn BOOL m_GetChunk( void ) noexcept;

   public:

       CNetworkUsers(__in const CNetworkUsers&) = delete;
       CNetworkUsers& operator=(__in const CNetworkUsers&) = delete;
       CNetworkUsers();
      CNetworkUsers( __in_z_opt LPCTSTR machine_name );
      virtual ~CNetworkUsers();

      enum _AccountTypes
      {
         accountBackupDomainController = UF_SERVER_TRUST_ACCOUNT,
         accountMachine                = UF_WORKSTATION_TRUST_ACCOUNT,
         accountInterdomain            = UF_INTERDOMAIN_TRUST_ACCOUNT
      };

      virtual bool  Add( __in const CNetworkUserInformation& user_to_add ) noexcept;
      virtual __checkReturn BOOL ChangePassword( __in const std::wstring& user_name, __in const std::wstring& old_password, __in const std::wstring& new_password ) noexcept;
      virtual void  Close( void ) noexcept;
      virtual __checkReturn BOOL CreateComputerAccount( __in_z_opt LPCTSTR computer_name = nullptr, __in DWORD type = accountMachine ) noexcept;
      virtual BOOL  Delete( __in const CNetworkUserInformation& user_to_delete ) noexcept;
      virtual BOOL  Delete( __in const std::wstring& user_to_delete ) noexcept;
      virtual __checkReturn BOOL Enumerate( void ) noexcept;
      virtual BOOL  ExpirePassword( __in const std::wstring& user_name ) noexcept;
      virtual __checkReturn DWORD GetLevel( void ) const noexcept;
      virtual __checkReturn BOOL  GetNext( __inout CNetworkUserInformation& information ) noexcept;
};

#endif // NETWORK_USER_CLASS_HEADER
