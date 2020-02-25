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
** $Workfile: CNetworkUser.cpp $
** $Revision: 29 $
** $Modtime: 6/26/01 10:49a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CNetworkUsers::CNetworkUsers(_In_ std::wstring_view machine_name) noexcept : CNetwork(machine_name)
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CNetworkUsers::~CNetworkUsers() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Close();
   m_Initialize();
}

bool Win32FoundationClasses::CNetworkUsers::Add( _In_ Win32FoundationClasses::CNetworkUserInformation const& user_to_add ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   USER_INFO_2 user_information;

   WCHAR wide_name[ 1024 ];
   WCHAR wide_password[ 1024 ];
   WCHAR wide_home_directory[ 1024 ];
   WCHAR wide_comment[ 1024 ];
   WCHAR wide_script_path[ 1024 ];
   WCHAR wide_full_name[ 1024 ];
   WCHAR wide_user_comment[ 1024 ];
   WCHAR wide_parameters[ 1024 ];
   WCHAR wide_workstations[ 1024 ];
   WCHAR wide_logon_server[ 1024 ];

   ::ZeroMemory( &user_information,   sizeof( user_information    ) );
   ::ZeroMemory( wide_name,           sizeof( wide_name           ) );
   ::ZeroMemory( wide_password,       sizeof( wide_password       ) );
   ::ZeroMemory( wide_home_directory, sizeof( wide_home_directory ) );
   ::ZeroMemory( wide_comment,        sizeof( wide_comment        ) );
   ::ZeroMemory( wide_script_path,    sizeof( wide_script_path    ) );
   ::ZeroMemory( wide_full_name,      sizeof( wide_full_name      ) );
   ::ZeroMemory( wide_user_comment,   sizeof( wide_user_comment   ) );
   ::ZeroMemory( wide_parameters,     sizeof( wide_parameters     ) );
   ::ZeroMemory( wide_workstations,   sizeof( wide_workstations   ) );
   ::ZeroMemory( wide_logon_server,   sizeof( wide_logon_server   ) );

#if ! defined( UNICODE )

   // We need to convert the ASCII strings to UNICODE

   ::ASCII_to_UNICODE( user_to_add.Name,          wide_name           );
   ::ASCII_to_UNICODE( user_to_add.Password,      wide_password       );
   ::ASCII_to_UNICODE( user_to_add.HomeDirectory, wide_home_directory );
   ::ASCII_to_UNICODE( user_to_add.Comment,       wide_comment        );
   ::ASCII_to_UNICODE( user_to_add.ScriptPath,    wide_script_path    );
   ::ASCII_to_UNICODE( user_to_add.FullName,      wide_full_name      );
   ::ASCII_to_UNICODE( user_to_add.UserComment,   wide_user_comment   );
   ::ASCII_to_UNICODE( user_to_add.Parameters,    wide_parameters     );
   ::ASCII_to_UNICODE( user_to_add.Workstations,  wide_workstations   );
   ::ASCII_to_UNICODE( user_to_add.LogonServer,   wide_logon_server   );

#else

   // We're already UNICODE

   ::wcsncpy_s( (LPTSTR) wide_name,           std::size(wide_name),           user_to_add.Name.c_str()         , _TRUNCATE );
   ::wcsncpy_s( (LPTSTR) wide_password,       std::size(wide_password),       user_to_add.Password.c_str(), _TRUNCATE );
   ::wcsncpy_s( (LPTSTR) wide_home_directory, std::size(wide_home_directory), user_to_add.HomeDirectory.c_str(), _TRUNCATE );
   ::wcsncpy_s( (LPTSTR) wide_comment,        std::size(wide_comment),        user_to_add.Comment.c_str(), _TRUNCATE );
   ::wcsncpy_s( (LPTSTR) wide_script_path,    std::size(wide_script_path),    user_to_add.ScriptPath.c_str(), _TRUNCATE );
   ::wcsncpy_s( (LPTSTR) wide_full_name,      std::size(wide_full_name),      user_to_add.FullName.c_str(), _TRUNCATE );
   ::wcsncpy_s( (LPTSTR) wide_user_comment,   std::size(wide_user_comment),   user_to_add.UserComment.c_str(), _TRUNCATE );
   ::wcsncpy_s( (LPTSTR) wide_parameters,     std::size(wide_parameters),     user_to_add.Parameters.c_str(), _TRUNCATE );
   ::wcsncpy_s( (LPTSTR) wide_workstations,   std::size(wide_workstations),   user_to_add.Workstations.c_str(), _TRUNCATE );
   ::wcsncpy_s( (LPTSTR) wide_logon_server,   std::size(wide_logon_server),   user_to_add.LogonServer.c_str(), _TRUNCATE );

#endif

   user_information.usri2_name           = wide_name;
   user_information.usri2_password       = wide_password;
   user_information.usri2_password_age   = 0;
   user_information.usri2_priv           = user_to_add.Privileges;
   user_information.usri2_home_dir       = wide_home_directory;
   user_information.usri2_comment        = wide_comment;
   user_information.usri2_flags          = user_to_add.Flags;
   user_information.usri2_script_path    = wide_script_path;
   user_information.usri2_auth_flags     = user_to_add.AuthenticationFlags;
   user_information.usri2_full_name      = wide_full_name;
   user_information.usri2_usr_comment    = wide_user_comment;
   user_information.usri2_parms          = wide_parameters;
   user_information.usri2_workstations   = wide_workstations;
   user_information.usri2_last_logon     = 0;
   user_information.usri2_last_logoff    = 0;
   user_information.usri2_acct_expires   = 0;
   user_information.usri2_max_storage    = user_to_add.MaximumStorage;
   user_information.usri2_units_per_week = user_to_add.UnitsPerWeek;
//   user_information.usri2_logon_hours    = user_to_add.LogonHours.GetData();
   user_information.usri2_bad_pw_count   = user_to_add.BadPasswordCount;
   user_information.usri2_num_logons     = user_to_add.NumberOfLogons;
   user_information.usri2_logon_server   = wide_logon_server;
   user_information.usri2_country_code   = user_to_add.CountryCode;
   user_information.usri2_code_page      = user_to_add.CodePage;

   BYTE logon_hours[ 21 ];

   for ( auto const index : Range(user_to_add.LogonHours.size()) )
   {
      if ( index < 21 )
      {
         logon_hours[ index ] = user_to_add.LogonHours.at( index );
      }
   }

   std::size_t index = user_to_add.LogonHours.size();

   while(index < 21) // Can't be converted to range loop
   {
      logon_hours[index] = 0xFF;
   }

   DWORD parameter_causing_the_error = 0;

   m_ErrorCode = ::NetUserAdd( m_WideDoubleBackslashPreceededMachineName.get(),
                               2,
                     reinterpret_cast<BYTE *>(&user_information),
                              &parameter_causing_the_error );

   if ( m_ErrorCode != NERR_Success ) 
   {
#if defined( _DEBUG )
      if ( m_ErrorCode == ERROR_INVALID_PARAMETER )
      {
         switch( parameter_causing_the_error )
         {
            case USER_NAME_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_NAME_PARMNUM parameter" ) );
               break;

            case USER_PASSWORD_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_PASSWORD_PARMNUM parameter" ) );
               break;

            case USER_PASSWORD_AGE_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_PASSWORD_AGE_PARMNUM parameter" ) );
               break;

            case USER_PRIV_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_PRIV_PARMNUM parameter" ) );
               break;

            case USER_HOME_DIR_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_HOME_DIR_PARMNUM parameter" ) );
               break;

            case USER_COMMENT_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_COMMENT_PARMNUM parameter" ) );
               break;

            case USER_FLAGS_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_FLAGS_PARMNUM parameter" ) );
               break;

            case USER_SCRIPT_PATH_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_SCRIPT_PATH_PARMNUM parameter" ) );
               break;

            case USER_AUTH_FLAGS_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_AUTH_FLAGS_PARMNUM parameter" ) );
               break;

            case USER_FULL_NAME_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_FULL_NAME_PARMNUM parameter" ) );
               break;

            case USER_USR_COMMENT_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_USR_COMMENT_PARMNUM parameter" ) );
               break;

            case USER_PARMS_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_PARMS_PARMNUM parameter" ) );
               break;

            case USER_WORKSTATIONS_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_WORKSTATIONS_PARMNUM parameter" ) );
               break;

            case USER_LAST_LOGON_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_LAST_LOGON_PARMNUM parameter" ) );
               break;

            case USER_LAST_LOGOFF_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_LAST_LOGOFF_PARMNUM parameter" ) );
               break;

            case USER_ACCT_EXPIRES_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_ACCT_EXPIRES_PARMNUM parameter" ) );
               break;

            case USER_MAX_STORAGE_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_MAX_STORAGE_PARMNUM parameter" ) );
               break;

            case USER_UNITS_PER_WEEK_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_UNITS_PER_WEEK_PARMNUM parameter" ) );
               break;

            case USER_LOGON_HOURS_PARMNUM:

              // WFCTRACE( TEXT( "Invalid USER_LOGON_HOURS_PARMNUM parameter" ) );
               break;

            case USER_PAD_PW_COUNT_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_PAD_PW_COUNT_PARMNUM parameter" ) );
               break;

            case USER_NUM_LOGONS_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_NUM_LOGONS_PARMNUM parameter" ) );
               break;

            case USER_LOGON_SERVER_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_LOGON_SERVER_PARMNUM parameter" ) );
               break;

            case USER_COUNTRY_CODE_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_COUNTRY_CODE_PARMNUM parameter" ) );
               break;

            case USER_CODE_PAGE_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_CODE_PAGE_PARMNUM parameter" ) );
               break;

            case USER_PRIMARY_GROUP_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_PRIMARY_GROUP_PARMNUM parameter" ) );
               break;

            case USER_PROFILE_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_PROFILE_PARMNUM parameter" ) );
               break;

            case USER_HOME_DIR_DRIVE_PARMNUM:

               //WFCTRACE( TEXT( "Invalid USER_HOME_DIR_DRIVE_PARMNUM parameter" ) );
               break;

            default:

               //WFCTRACE( TEXT( "Unknown parameter error" ) );
               break;
         }
      }
#endif

      return( false );
   }

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CNetworkUsers::ChangePassword( _In_ std::wstring const& user_name, _In_ std::wstring const& old_password, _In_ std::wstring const& new_password ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   //WFCTRACEVAL( TEXT( "Changing password for " ), user_name );

   WCHAR wide_user_name[ 1024 ];
   WCHAR wide_old_password[ 1024 ];
   WCHAR wide_new_password[ 1024 ];

   ::ZeroMemory( wide_user_name,    sizeof( wide_user_name    ) );
   ::ZeroMemory( wide_old_password, sizeof( wide_old_password ) );
   ::ZeroMemory( wide_new_password, sizeof( wide_new_password ) );

#if ! defined( UNICODE )

   // We need to convert the ASCII strings to unicode

   ::ASCII_to_UNICODE( user_name,    wide_user_name    );
   ::ASCII_to_UNICODE( old_password, wide_old_password );
   ::ASCII_to_UNICODE( new_password, wide_new_password );

#else

   ::_tcsncpy_s( (LPTSTR) wide_user_name,    std::size( wide_user_name ),    user_name.c_str(),    std::size( wide_user_name    ) );
   ::_tcsncpy_s( (LPTSTR) wide_old_password, std::size( wide_old_password ), old_password.c_str(), std::size( wide_old_password ) );
   ::_tcsncpy_s( (LPTSTR) wide_new_password, std::size( wide_new_password ), new_password.c_str(), std::size( wide_new_password ) );

#endif // UNICODE

   m_ErrorCode = ::NetUserChangePassword( m_WideDoubleBackslashPreceededMachineName.get(),
                                          wide_user_name,
                                          wide_old_password,
                                          wide_new_password );

   if ( m_ErrorCode == NERR_Success )
   {
      return( true );
   }

   return( false );
}

void Win32FoundationClasses::CNetworkUsers::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Win32FoundationClasses::CNetwork::Close();

   if ( m_3InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_3InformationBuffer );
      m_3InformationBuffer = nullptr;
   }

   if ( m_2InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_2InformationBuffer );
      m_2InformationBuffer = nullptr;
   }

   if ( m_1InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_1InformationBuffer );
      m_1InformationBuffer = nullptr;
   }

   if ( m_10InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_10InformationBuffer );
      m_10InformationBuffer = nullptr;
   }

   if ( m_0InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_0InformationBuffer );
      m_0InformationBuffer = nullptr;
   }
}

_Check_return_ bool Win32FoundationClasses::CNetworkUsers::CreateComputerAccount( __in_z_opt LPCTSTR computer_name, _In_ DWORD type_of_account ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( computer_name );

   // This method is described in Knowledge Base Article Q136867

   // Creating a computer account establishes the "permitted to
   // trust this domain" side of the domain trust relationship.
   // Article Q145697 discusses the manipulation of the "Trusted Domains" 
   // side of the domain trust relationship, at the domain controller level.

   if ( type_of_account != accountMachine and
        type_of_account != accountBackupDomainController and
        type_of_account != accountInterdomain )
   {
      m_ErrorCode = ERROR_INVALID_PARAMETER;
      return( false );
   }

   std::wstring account_name;

   CNetworkUserInformation information;

   information.SetAddDefaults();

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if (computer_name != nullptr)
      {
          information.Name.assign(computer_name);
      }
      
      /*
      ** The computer account name should be all uppercase for consistency
      ** with Windows NT account management utilities.
      */

      make_upper(information.Name);

      /*
      ** The maximum computer name length is MAX_COMPUTERNAME_LENGTH (15).
      ** This length does not include the trailing dollar sign ($). 
      */

      if ( information.Name.length() > MAX_COMPUTERNAME_LENGTH )
      {
         m_ErrorCode = ERROR_INVALID_ACCOUNT_NAME;
         return( false );
      }

      /*
      ** The password for a new computer account should be the lowercase 
      ** representation of the computer account name, without the trailing 
      ** dollar sign ($). For interdomain trust, the password can be an 
      ** arbitrary value that matches the value specified on the trust 
      ** side of the relationship.
      */

      information.Password.assign(information.Name);
      make_lower(information.Password);

      /*
      ** The maximum password length is LM20_PWLEN (14). The password
      ** should be truncated to this length if the computer account name
      ** exceeds this length.
      */

      if ( information.Password.length() > LM20_PWLEN )
      {
         information.Password.erase(LM20_PWLEN);
      }

      /*
      ** A computer account name always has a trailing dollar sign ($). 
      ** Any APIs used to manage computer accounts must build the computer
      ** name such that the last character of the computer account name 
      ** is a dollar sign ($). For interdomain trust, the account name is 
      ** TrustingDomainName$. 
      */

      if ( information.Name.at( information.Name.length() - 1 ) != '$' )
      {
         information.Name.push_back('$');
      }

      /*
      ** The SeMachineAccountPrivilege can be granted on the target computer
      ** to give specified users the ability to create computer accounts.
      ** This gives non-administrators the ability to create computer
      ** accounts. The caller needs to enable this privilege prior to adding
      ** the computer account.
      */

      /*
      ** Computer account management should take place on the primary
      ** domain controller for the target domain.
      */

      BYTE * buffer_p = nullptr;

      if ( ::NetGetDCName( nullptr, nullptr, &buffer_p ) != NERR_Success )
      {
         return( false );
      }

      information.Privileges = USER_PRIV_USER;
      information.Flags = type_of_account bitor UF_SCRIPT;

      std::wstring primary_domain_controller;

#if ! defined( UNICODE )

      // We ain't UNICODE so we need to convert

      ::wfc_convert_lpcwstr_to_cstring( (LPCWSTR) buffer_p, primary_domain_controller );

#else

      // We're already UNICODE so we should be OK

      primary_domain_controller.assign(reinterpret_cast<LPCTSTR>(buffer_p));

#endif // UNICODE

      // We no longer need the buffer

      (void) ::NetApiBufferFree( buffer_p );
      buffer_p = nullptr;

      CNetworkUsers user_to_add( primary_domain_controller.c_str() );

      (void) user_to_add.SetPrivilege( SE_MACHINE_ACCOUNT_NAME, true );
      
      if ( user_to_add.Add( information ) == false )
      {
         m_ErrorCode = user_to_add.GetErrorCode();
         return( false );
      }

      return( true );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( false );
   }
   WFC_END_CATCH_ALL
}

bool Win32FoundationClasses::CNetworkUsers::Delete( _In_ Win32FoundationClasses::CNetworkUserInformation const& user_to_delete ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   WCHAR wide_name[ 1024 ];

   ::ZeroMemory( wide_name, sizeof( wide_name ) );

#if ! defined( UNICODE )

   // We need to convert the ASCII strings to UNICODE

   ::ASCII_to_UNICODE( user_to_delete.Name, wide_name );

#else

   // We're already UNICODE

   ::_tcsncpy_s( (LPTSTR) wide_name, std::size( wide_name ), user_to_delete.Name.c_str(), std::size( wide_name ) );

#endif // UNICODE

   m_ErrorCode = ::NetUserDel( m_WideDoubleBackslashPreceededMachineName.get(), wide_name );

   if ( m_ErrorCode == NERR_Success )
   {
      return( true );
   }

   return( false );
}

bool Win32FoundationClasses::CNetworkUsers::Delete( _In_ std::wstring const& user_to_delete ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Win32FoundationClasses::CNetworkUserInformation user;
   
   user.Name.assign( user_to_delete );

   return( Delete( user ) );
}

_Check_return_ bool Win32FoundationClasses::CNetworkUsers::Enumerate( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Search order is 3, 2, 1, 10, 0

   if ( m_3InformationBuffer != nullptr )
   {
      (void) ::NetApiBufferFree( m_3InformationBuffer );
      m_3InformationBuffer = nullptr;
   }

   m_3CurrentEntryNumber       = 0;
   m_3Index                    = 0;
   m_3NumberOfEntriesRead      = 0;
   m_3ResumeHandle             = 0;
   m_3TotalNumberOfEntries     = 0;

   if ( m_2InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_2InformationBuffer );
      m_2InformationBuffer = nullptr;
   }

   m_2CurrentEntryNumber       = 0;
   m_2Index                    = 0;
   m_2NumberOfEntriesRead      = 0;
   m_2ResumeHandle             = 0;
   m_2TotalNumberOfEntries     = 0;

   if ( m_1InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_1InformationBuffer );
      m_1InformationBuffer = nullptr;
   }

   m_1CurrentEntryNumber       = 0;
   m_1Index                    = 0;
   m_1NumberOfEntriesRead      = 0;
   m_1ResumeHandle             = 0;
   m_1TotalNumberOfEntries     = 0;

   if ( m_10InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_10InformationBuffer );
      m_10InformationBuffer = nullptr;
   }

   m_10CurrentEntryNumber       = 0;
   m_10Index                    = 0;
   m_10NumberOfEntriesRead      = 0;
   m_10ResumeHandle             = 0;
   m_10TotalNumberOfEntries     = 0;

   if ( m_0InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_0InformationBuffer );
      m_0InformationBuffer = nullptr;
   }

   m_0CurrentEntryNumber       = 0;
   m_0Index                    = 0;
   m_0NumberOfEntriesRead      = 0;
   m_0ResumeHandle             = 0;
   m_0TotalNumberOfEntries     = 0;

   return( m_GetChunk() );
}

bool Win32FoundationClasses::CNetworkUsers::ExpirePassword( _In_ std::wstring const& user_name ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   WCHAR wide_name[ 1024 ];

   ::ZeroMemory( wide_name, sizeof( wide_name ) );

#if ! defined( UNICODE )

   // We need to convert the ASCII strings to UNICODE

   ::ASCII_to_UNICODE( user_name, wide_name );

#else

   // We're already UNICODE

   ::_tcsncpy_s( (LPTSTR) wide_name, std::size( wide_name ), user_name.c_str(), std::size( wide_name ) );

#endif // UNICODE

   USER_INFO_3 * user_information_p = nullptr;

   m_ErrorCode = ::NetUserGetInfo( m_WideDoubleBackslashPreceededMachineName.get(),
                                   wide_name,
                                   3,
    reinterpret_cast< LPBYTE * >( &user_information_p ) );

   if ( m_ErrorCode == NERR_Success )
   {
      if ( user_information_p != nullptr )
      {
         DWORD parameter_error = 0;

         user_information_p->usri3_password_expired = TRUE;

         m_ErrorCode = ::NetUserSetInfo( m_WideDoubleBackslashPreceededMachineName.get(),
                                         wide_name,
                                         3,
             reinterpret_cast< LPBYTE >( user_information_p ),
                                        &parameter_error );

         if ( m_ErrorCode == NERR_Success )
         {
            ::NetApiBufferFree( user_information_p );
            user_information_p = nullptr;
            return( true );
         }

         ::NetApiBufferFree( user_information_p );
         user_information_p = nullptr;

#if defined( _DEBUG )

         switch( parameter_error )
         {
            case USER_NAME_PARMNUM:

               //WFCTRACE( TEXT( "usri3_name field was invalid." ) );
               break;

            case USER_PASSWORD_PARMNUM:

               //WFCTRACE( TEXT( "usri3_password field was invalid." ) );
               break;

            case USER_PASSWORD_AGE_PARMNUM:

               //WFCTRACE( TEXT( "usri3_password_age field was invalid." ) );
               break;

            case USER_PRIV_PARMNUM:

               //WFCTRACE( TEXT( "usri3_priv field was invalid." ) );
               break;

            case USER_HOME_DIR_PARMNUM:

               //WFCTRACE( TEXT( "usri3_home_dir field was invalid." ) );
               break;

            case USER_COMMENT_PARMNUM:

               //WFCTRACE( TEXT( "usri3_comment field was invalid." ) );
               break;

            case USER_FLAGS_PARMNUM:

               //WFCTRACE( TEXT( "usri3_flags field was invalid." ) );
               break;

            case USER_SCRIPT_PATH_PARMNUM:

               //WFCTRACE( TEXT( "usri3_script_path field was invalid." ) );
               break;

            case USER_AUTH_FLAGS_PARMNUM:

               //WFCTRACE( TEXT( "usri3_auth_flags field was invalid." ) );
               break;

            case USER_FULL_NAME_PARMNUM:

               //WFCTRACE( TEXT( "usri3_full_name field was invalid." ) );
               break;

            case USER_USR_COMMENT_PARMNUM:

               //WFCTRACE( TEXT( "usri3_usr_comment field was invalid." ) );
               break;

            case USER_PARMS_PARMNUM:

               //WFCTRACE( TEXT( "usri3_parms field was invalid." ) );
               break;

            case USER_WORKSTATIONS_PARMNUM:

               //WFCTRACE( TEXT( "usri3_workstations field was invalid." ) );
               break;

            case USER_LAST_LOGON_PARMNUM:

               //WFCTRACE( TEXT( "usri3_last_logon field was invalid." ) );
               break;

            case USER_LAST_LOGOFF_PARMNUM:

               //WFCTRACE( TEXT( "usri3_last_logoff field was invalid." ) );
               break;

            case USER_ACCT_EXPIRES_PARMNUM:

               //WFCTRACE( TEXT( "usri3_acct_expires field was invalid." ) );
               break;

            case USER_MAX_STORAGE_PARMNUM:

               //WFCTRACE( TEXT( "usri3_max_storage field was invalid." ) );
               break;

            case USER_UNITS_PER_WEEK_PARMNUM:

               //WFCTRACE( TEXT( "usri3_units_per_week field was invalid." ) );
               break;

            case USER_LOGON_HOURS_PARMNUM:

               //WFCTRACE( TEXT( "usri3_logon_hours field was invalid." ) );
               break;

            case USER_PAD_PW_COUNT_PARMNUM:

               //WFCTRACE( TEXT( "usri3_bad_pw_count field was invalid." ) );
               break;

            case USER_NUM_LOGONS_PARMNUM:

               //WFCTRACE( TEXT( "usri3_num_logons field was invalid." ) );
               break;

            case USER_LOGON_SERVER_PARMNUM:

               //WFCTRACE( TEXT( "usri3_logon_server field was invalid." ) );
               break;

            case USER_COUNTRY_CODE_PARMNUM:

               //WFCTRACE( TEXT( "usri3_country_code field was invalid." ) );
               break;

            case USER_CODE_PAGE_PARMNUM:

               //WFCTRACE( TEXT( "usri3_code_page field was invalid." ) );
               break;

            case USER_PRIMARY_GROUP_PARMNUM:

               //WFCTRACE( TEXT( "usri3_primary_group_id field was invalid." ) );
               break;

            case USER_PROFILE_PARMNUM:

               //WFCTRACE( TEXT( "usri3_profile field was invalid." ) );
               break;

            case USER_HOME_DIR_DRIVE_PARMNUM:

               //WFCTRACE( TEXT( "usri3_home_dir_drive field was invalid." ) );
               break;

            default:

               //WFCTRACEVAL( TEXT( "Unknown parameter number " ), parameter_error );
               //WFCTRACE( TEXT( "Look in the lmaccess.h header file and see if there's a new USER_xxx_PARMNUM with this value." ) );
               break;
         }

#endif // _DEBUG
      }
   }

   return( false );
}

_Check_return_ DWORD Win32FoundationClasses::CNetworkUsers::GetLevel( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_3InformationBuffer != nullptr )
   {
      return( 3 );
   }

   if ( m_2InformationBuffer != nullptr )
   {
      return( 2 );
   }

   if ( m_1InformationBuffer != nullptr )
   {
      return( 1 );
   }

   if ( m_10InformationBuffer != nullptr )
   {
      return( 10 );
   }

   if ( m_0InformationBuffer != nullptr )
   {
      return( 0 );
   }

   return( 0xFFFFFFFF );
}

_Check_return_ bool Win32FoundationClasses::CNetworkUsers::GetNext( __inout Win32FoundationClasses::CNetworkUserInformation& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Rapoport Sergey [radio-city@utn.e-burg.ru]

   // We need to see which buffer we're working with...

   if ( m_3InformationBuffer != nullptr )
   {
      if ( m_3Index < m_3NumberOfEntriesRead )
      {
         //WFCTRACEVAL( TEXT( "Copying index " ), m_3Index );
         //WFCTRACEVAL( TEXT( "Number of entries read is " ), m_3NumberOfEntriesRead );

         information.Copy( &m_3InformationBuffer[ m_3Index ] );
         m_3Index++;

         return( true );
      }
      else
      {
         // We need another chunk

         if ( m_GetChunk() == true)
         {
            return( GetNext( information ) );
         }
         else
         {
            return( false );
         }
      }
   }
   else if ( m_2InformationBuffer != nullptr )
   {
      if ( m_2Index < m_2NumberOfEntriesRead )
      {
         information.Copy( &m_2InformationBuffer[ m_2Index ] );
         m_2Index++;
         return( true );
      }
      else
      {
         // We need another chunk

         if ( m_GetChunk() == true)
         {
            return( GetNext( information ) );
         }
         else
         {
            return( false );
         }
      }
   }
   else if ( m_1InformationBuffer != nullptr )
   {
      if ( m_1Index < m_1NumberOfEntriesRead )
      {
         information.Copy( &m_1InformationBuffer[ m_1Index ] );
         m_1Index++;
         return( true );
      }
      else
      {
         // We need another chunk

         if ( m_GetChunk() == true)
         {
            return( GetNext( information ) );
         }
         else
         {
            return( false );
         }
      }
   }
   else if ( m_10InformationBuffer != nullptr )
   {
      if ( m_10Index < m_10NumberOfEntriesRead )
      {
         information.Copy( &m_10InformationBuffer[ m_10Index ] );
         m_10Index++;
         return( true );
      }
      else
      {
         // We need another chunk

         if ( m_GetChunk() == true)
         {
            return( GetNext( information ) );
         }
         else
         {
            return( false );
         }
      }
   }
   else if ( m_0InformationBuffer != nullptr )
   {
      if ( m_0Index < m_0NumberOfEntriesRead )
      {
         information.Copy( &m_0InformationBuffer[ m_0Index ] );
         m_0Index++;
         return( true );
      }
      else
      {
         // We need another chunk

         if ( m_GetChunk() == true)
         {
            return( GetNext( information ) );
         }
         else
         {
            return( false );
         }
      }
   }

   information.Empty();
   return( false );
}

_Check_return_ bool Win32FoundationClasses::CNetworkUsers::m_GetChunk( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Try to get the most information as possible. Start off by asking for
   // data that requires lots of security access privileges

   m_3Index = 0;

   // Let's see if we have already been reading

   if ( m_3NumberOfEntriesRead != 0 and m_3ResumeHandle == 0 )
   {
      // Yup, we've finished reading
      ::NetApiBufferFree( m_3InformationBuffer );
      m_3InformationBuffer        = nullptr;
      m_3CurrentEntryNumber       = 0;
      m_3NumberOfEntriesRead      = 0;
      m_3ResumeHandle             = 0;
      m_3TotalNumberOfEntries     = 0;
      return( false );
   }

   m_ErrorCode = ::NetUserEnum( m_WideDoubleBackslashPreceededMachineName.get(),
                                3,
                                0,
                    (LPBYTE *) &m_3InformationBuffer,
                                64 * 1024,
                               &m_3NumberOfEntriesRead,
                               &m_3TotalNumberOfEntries,
                               &m_3ResumeHandle );

   if ( m_ErrorCode == NERR_Success or m_ErrorCode == ERROR_MORE_DATA )
   {
      // Thanks go to Dirk Tolson (dirk.tolson@srs.gov) for finding a bug
      // here. When there were more accounts than could be read in one read,
      // I would go into an endless loop. DOH!

      if ( m_ErrorCode == NERR_Success )
      {
         m_3ResumeHandle = 0;
      }

      return( true );
   }
    
   if ( m_3InformationBuffer == nullptr )
   {
      //WFCTRACE( TEXT( "well, that's funny. m_3InformationBuffer is NULL!\n" ) );
   }

   // Make really sure the OS didn't modify our variables...

   if ( m_3InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_3InformationBuffer );
      m_3InformationBuffer = nullptr;
   }

   m_3CurrentEntryNumber   = 0;
   m_3NumberOfEntriesRead  = 0;
   m_3ResumeHandle         = 0;
   m_3TotalNumberOfEntries = 0;

   if ( m_ErrorCode != ERROR_ACCESS_DENIED )
   {
      return( false );
   }

   m_2Index = 0;

   // Let's see if we have already been reading

   if ( m_2NumberOfEntriesRead != 0 and m_2ResumeHandle == 0 )
   {
      // Yup, we've finished reading
      ::NetApiBufferFree( m_2InformationBuffer );
      m_2InformationBuffer        = nullptr;
      m_2CurrentEntryNumber       = 0;
      m_2NumberOfEntriesRead      = 0;
      m_2ResumeHandle             = 0;
      m_2TotalNumberOfEntries     = 0;
      return( false );
   }

   m_ErrorCode = ::NetUserEnum(           m_WideDoubleBackslashPreceededMachineName.get(),
                                          2,
                                          0,
                              (LPBYTE *) &m_2InformationBuffer,
                                          65535,
                                         &m_2NumberOfEntriesRead,
                                         &m_2TotalNumberOfEntries,
                                         &m_2ResumeHandle );

   if ( m_ErrorCode == NERR_Success and m_2InformationBuffer != nullptr )
   {
      // Thanks go to Dirk Tolson (dirk.tolson@srs.gov) for finding a bug
      // here. When there were more accounts than could be read in one read,
      // I would go into an endless loop. DOH!

      if ( m_ErrorCode == NERR_Success )
      {
         m_2ResumeHandle = 0;
      }

      return( true );
   }

   // Make really sure the OS didn't modify our variables...

   if ( m_2InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_2InformationBuffer );
      m_2InformationBuffer = nullptr;
   }

   m_2CurrentEntryNumber   = 0;
   m_2NumberOfEntriesRead  = 0;
   m_2ResumeHandle         = 0;
   m_2TotalNumberOfEntries = 0;

   if ( m_ErrorCode != ERROR_ACCESS_DENIED )
   {
      return( false );
   }

   m_1Index = 0;

   // Let's see if we have already been reading

   if ( m_1NumberOfEntriesRead != 0 and m_1ResumeHandle == 0 )
   {
      // Yup, we've finished reading
      ::NetApiBufferFree( m_1InformationBuffer );
      m_1InformationBuffer        = nullptr;
      m_1CurrentEntryNumber       = 0;
      m_1NumberOfEntriesRead      = 0;
      m_1ResumeHandle             = 0;
      m_1TotalNumberOfEntries     = 0;
      return( false );
   }

   m_ErrorCode = ::NetUserEnum(           m_WideDoubleBackslashPreceededMachineName.get(),
                                          1, 
                                          0,
                              (LPBYTE *) &m_1InformationBuffer,
                                          65535,
                                         &m_1NumberOfEntriesRead,
                                         &m_1TotalNumberOfEntries,
                                         &m_1ResumeHandle );

   if ( m_ErrorCode == NERR_Success and m_1InformationBuffer != nullptr )
   {
      // Thanks go to Dirk Tolson (dirk.tolson@srs.gov) for finding a bug
      // here. When there were more accounts than could be read in one read,
      // I would go into an endless loop. DOH!

      if ( m_ErrorCode == NERR_Success )
      {
         m_1ResumeHandle = 0;
      }

      return( true );
   }

   // Make really sure the OS didn't modify our variables...

   if ( m_1InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_1InformationBuffer );
      m_1InformationBuffer = nullptr;
   }

   m_1CurrentEntryNumber   = 0;
   m_1NumberOfEntriesRead  = 0;
   m_1ResumeHandle         = 0;
   m_1TotalNumberOfEntries = 0;

   if ( m_ErrorCode != ERROR_ACCESS_DENIED )
   {
      return( false );
   }

   // Go after level 10 data
   m_10Index = 0;

   // Let's see if we have already been reading

   if ( m_10NumberOfEntriesRead != 0 and m_10ResumeHandle == 0 )
   {
      // Yup, we've finished reading
      ::NetApiBufferFree( m_10InformationBuffer );
      m_10InformationBuffer        = nullptr;
      m_10CurrentEntryNumber       = 0;
      m_10NumberOfEntriesRead      = 0;
      m_10ResumeHandle             = 0;
      m_10TotalNumberOfEntries     = 0;
      return( false );
   }

   m_ErrorCode = ::NetUserEnum(          m_WideDoubleBackslashPreceededMachineName.get(),
                                         10, 
                                         0,
                             (LPBYTE *) &m_10InformationBuffer,
                                         65535,
                                        &m_10NumberOfEntriesRead,
                                        &m_10TotalNumberOfEntries,
                                        &m_10ResumeHandle );

   if ( m_ErrorCode == NERR_Success and m_10InformationBuffer != nullptr )
   {
      // Thanks go to Dirk Tolson (dirk.tolson@srs.gov) for finding a bug
      // here. When there were more accounts than could be read in one read,
      // I would go into an endless loop. DOH!

      if ( m_ErrorCode == NERR_Success )
      {
         m_10ResumeHandle = 0;
      }

      return( true );
   }

   // Make really sure the OS didn't modify our variables...

   if ( m_10InformationBuffer != nullptr )
   {
     ::NetApiBufferFree( m_10InformationBuffer );
      m_10InformationBuffer = nullptr;
   }

   m_10CurrentEntryNumber   = 0;
   m_10NumberOfEntriesRead  = 0;
   m_10ResumeHandle         = 0;
   m_10TotalNumberOfEntries = 0;

   if ( m_ErrorCode != ERROR_ACCESS_DENIED )
   {
      return( false );
   }

   // Go after level 0 data
   m_0Index = 0;

   // Let's see if we have already been reading

   if ( m_0NumberOfEntriesRead != 0 and m_0ResumeHandle == 0 )
   {
      // Yup, we've finished reading
      ::NetApiBufferFree( m_0InformationBuffer );
      m_0InformationBuffer        = nullptr;
      m_0CurrentEntryNumber       = 0;
      m_0NumberOfEntriesRead      = 0;
      m_0ResumeHandle             = 0;
      m_0TotalNumberOfEntries     = 0;
      return( false );
   }

   m_ErrorCode = ::NetUserEnum(          m_WideDoubleBackslashPreceededMachineName.get(),
                                         0,
                                         0,
                             (LPBYTE *) &m_0InformationBuffer,
                                         65535,
                                        &m_0NumberOfEntriesRead,
                                        &m_0TotalNumberOfEntries,
                                        &m_0ResumeHandle );

   if ( m_ErrorCode == NERR_Success and m_0InformationBuffer != nullptr )
   {
      // Thanks go to Dirk Tolson (dirk.tolson@srs.gov) for finding a bug
      // here. When there were more accounts than could be read in one read,
      // I would go into an endless loop. DOH!

      if ( m_ErrorCode == NERR_Success )
      {
         m_0ResumeHandle = 0;
      }

      return( true );
   }

   // Make really sure the OS didn't modify our variables...

   if ( m_0InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_0InformationBuffer );
      m_0InformationBuffer = nullptr;
   }

   m_0CurrentEntryNumber   = 0;
   m_0NumberOfEntriesRead  = 0;
   m_0ResumeHandle         = 0;
   m_0TotalNumberOfEntries = 0;

   return( false );
}

void Win32FoundationClasses::CNetworkUsers::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_ErrorCode             = 0;

   m_3InformationBuffer    = nullptr;
   m_3ResumeHandle         = 0;
   m_3CurrentEntryNumber   = 0;
   m_3NumberOfEntriesRead  = 0;
   m_3TotalNumberOfEntries = 0;

   m_2InformationBuffer    = nullptr;
   m_2ResumeHandle         = 0;
   m_2CurrentEntryNumber   = 0;
   m_2NumberOfEntriesRead  = 0;
   m_2TotalNumberOfEntries = 0;

   m_1InformationBuffer    = nullptr;
   m_1ResumeHandle         = 0;
   m_1CurrentEntryNumber   = 0;
   m_1NumberOfEntriesRead  = 0;
   m_1TotalNumberOfEntries = 0;

   m_10InformationBuffer    = nullptr;
   m_10ResumeHandle         = 0;
   m_10CurrentEntryNumber   = 0;
   m_10NumberOfEntriesRead  = 0;
   m_10TotalNumberOfEntries = 0;

   m_0InformationBuffer    = nullptr;
   m_0ResumeHandle         = 0;
   m_0CurrentEntryNumber   = 0;
   m_0NumberOfEntriesRead  = 0;
   m_0TotalNumberOfEntries = 0;
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CNetworkUsers</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles network users. You can use this class to add or delete user accounts.">
</HEAD>

<BODY>

<H1>CNetworkUsers : <A HREF="CNetwork.htm">CNetwork</A></H1>
$Revision: 29 $<BR>
<HR>

<H2>Description</H2>
This class allows you to add, delete or enumerate user accounts in Win32.
It will also create computer accounts.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CNetworkUsers</B>()
<B>CNetworkUsers</B>( LPCTSTR machine_name )</PRE><DD>
Creates the object and let's you specify which machine to 
execute on (i.e. which machine's accounts you want to play with).

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="Add">Add</A></B>( const CNetworkUserInformation&amp; user_to_add )</PRE><DD>
Creates a new user account with the options you specify.
The return value will be TRUE if the account was created, FALSE otherwise.

<DT><PRE>BOOL <B><A NAME="ChangePassword">ChangePassword</A></B>( const std::wstring&amp; account_name, const std::wstring&amp; old_password, const std::wstring&amp; new_password )</PRE><DD>
Returns TRUE if the account&#39;s password was successfully changed. Returns
FALSE if it fails.

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the connection with the machine and cleans up any
internal buffers used.

<DT><PRE>BOOL <B><A NAME="CreateComputerAccount">CreateComputerAccount</A></B>( LPCTSTR computer_name = nullptr, DWORD type = accountMachine )</PRE><DD>
Creates a computer account. If <CODE>computer_name</CODE> is nullptr, the name
of the computer where this function executes will be used. <CODE>type</CODE>
can be one of the following:

<UL>
<LI>accountBackupDomainController
<LI>accountInterdomain
<LI>accountMachine
</UL>

The return value will be TRUE if the account was created, FALSE otherwise.
<B>CreateComputerAccount</B>() uses the method described in the Microsoft
Knowledge Base article
<A HREF="http://www.microsoft.com/kb/articles/q136/8/67.htm">Q136867</A>
to create the account.

<DT><PRE>BOOL <B><A NAME="Delete">Delete</A></B>( const CNetworkUserInformation&amp; user_to_delete )
BOOL <B>Delete</B>( const std::wstring&amp; user_to_delete )</PRE><DD>
Deletes a user's account.
The return value will be TRUE if the account was deleted, FALSE otherwise.

<DT><PRE>BOOL <B><A NAME="Enumerate">Enumerate</A></B>( void )</PRE><DD>
Initializes the object for enumeration.

<DT><PRE>BOOL <B><A NAME="ExpirePassword">ExpirePassword</A></B>( const std::wstring&amp; user_name )</PRE><DD>
Expires this user&#39;s password. This forces the user to change their
password the next time they log on.

<DT><PRE>DWORD <B><A NAME="GetLevel">GetLevel</A></B>( void ) const</PRE><DD>
When <B>Enumerate</B>() has successfully been called, this
method will return the level of information it is returning.
If it returns 0xFFFFFFFF, then it has failed. The return
value maps to the <CODE>USER_INFO_xxx</CODE> data structures.
For example, if <B>GetLevel</B>() returns 3, it means it is retrieving
<CODE>USER_INFO_3</CODE> data structures from the server.

<DT><PRE>BOOL <B><A NAME="GetNext">GetNext</A></B>( CNetworkUserInformation&amp; information )</PRE><DD>
Retrieves the next CNetworkUserInformation.
When you reach the end of the list, <B>GetNext</B>() will return FALSE.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

void test_CNetworkUsers( LPCTSTR machine_name )
{
   <A HREF="WfcTrace.htm">WFCLTRACEINIT</A>( TEXT( &quot;test_CNetworkUsers()&quot; ) );

   <B>CNetworkUsers</B> users( machine_name );

   CNetworkUserInformation user_information;

   if ( users.Enumerate() != FALSE )
   {
      _tprintf( TEXT( &quot;User Information for %s:\n&quot; ), (LPCTSTR) users.GetMachineName() );

      while( users.GetNext( user_information ) == true )
      {
         _tprintf( TEXT( &quot; Name                 - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.Name          );
         _tprintf( TEXT( &quot; Full Name            - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.FullName      );
         _tprintf( TEXT( &quot; Comment              - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.Comment       );
         _tprintf( TEXT( &quot; User Comment         - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.UserComment   );
         _tprintf( TEXT( &quot; ID                   - %lu\n&quot; ),              user_information.UserID        );
         _tprintf( TEXT( &quot; Flags                - %lX\n&quot; ),              user_information.Flags         );
         _tprintf( TEXT( &quot; Privileges           - %lX\n&quot; ),              user_information.Privileges    );
         _tprintf( TEXT( &quot; Password             - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.Password      );
         _tprintf( TEXT( &quot; Password Age         - %d Days %d Hours %d Minutes %d seconds\n&quot; ),
                   user_information.PasswordAge.GetDays(),
                   user_information.PasswordAge.GetHours(),
                   user_information.PasswordAge.GetMinutes(),
                   user_information.PasswordAge.GetSeconds() );
         _tprintf( TEXT( &quot; HomeDirectory        - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.HomeDirectory );
         _tprintf( TEXT( &quot; Script Path          - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.ScriptPath    );
         _tprintf( TEXT( &quot; Parameters           - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.Parameters    );
         _tprintf( TEXT( &quot; Workstations         - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.Workstations  );
         _tprintf( TEXT( &quot; LastLogon            - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.LastLogon.Format( &quot;%a %d %b %y, %H:%M:%S&quot; ) );
         _tprintf( TEXT( &quot; LastLogoff           - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.LastLogoff.Format( &quot;%a %d %b %y, %H:%M:%S&quot; ) );
         _tprintf( TEXT( &quot; Account Expires      - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.AccountExpires.Format( &quot;%a %d %b %y, %H:%M:%S&quot; ) );
         _tprintf( TEXT( &quot; Maximum Storage      - %lu\n&quot; ),              user_information.MaximumStorage     );
         _tprintf( TEXT( &quot; Bad Password Count   - %lu\n&quot; ),              user_information.BadPasswordCount   );
         _tprintf( TEXT( &quot; Number Of Logons     - %lu\n&quot; ),              user_information.NumberOfLogons     );
         _tprintf( TEXT( &quot; Logon Server         - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.LogonServer        );
         _tprintf( TEXT( &quot; Country Code         - %lX\n&quot; ),              user_information.CountryCode        );
         _tprintf( TEXT( &quot; Code Page            - %lX\n&quot; ),              user_information.CodePage           );
         _tprintf( TEXT( &quot; Primary Group ID     - %lu\n&quot; ),              user_information.PrimaryGroupID     );
         _tprintf( TEXT( &quot; Home Directory Drive - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.HomeDirectoryDrive );
         _tprintf( TEXT( &quot; Profile              - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) user_information.Profile            );
         _tprintf( TEXT( &quot; Password Has Expired - %lu\n&quot; ),              user_information.PasswordHasExpired );
         _tprintf( TEXT( &quot; Encrypted Password: &quot; ) );

         int index = 0;

         while( index &lt; user_information.EncryptedPassword.GetSize() )
         {
            _tprintf( TEXT( &quot;%02X&quot; ), (int) user_information.EncryptedPassword.GetAt( index ) );
            index++;
         }

         _tprintf( TEXT( &quot;\n\n&quot; ) );
      }
   }
   else
   {
      DWORD error_code = users.GetErrorCode();

      std::wstring error_message;

      Convert_NERR_Code_to_String( error_code, error_message );

      _tprintf( TEXT( &quot;CNetworkUsers.Enumerate( user_information ), ErrorCode == %d \&quot;%s\&quot;\n&quot; ), error_code, error_message.c_str() );
   }

   user_information.Empty();

   user_information.Name     = TEXT( &quot;Laura&quot; );
   user_information.Password = TEXT( &quot;LovesSammy&quot; );

   user_information.SetAddDefaults();

   if ( users.Add( user_information ) != FALSE )
   {
      _tprintf( TEXT( &quot;Laura Added.\n&quot; ) );
   }
   else
   {
      _tprintf( TEXT( &quot;Can't Add User\n&quot; ) );
   }

   if ( users.Delete( &quot;Laura&quot; ) != FALSE )
   {
      _tprintf( TEXT( &quot;Deleted Laura\n&quot; ) );
   }
   else
   {
      _tprintf( TEXT( &quot;Can't delete Laura\n&quot; ) );
   }
}</CODE></PRE>

<H2>API's Used</H2>

<B>CNetworkUsers</B> uses the following API's:

<UL>
<LI>NetApiBufferFree
<LI>NetGetDCName
<LI>NetUserAdd
(LI>NetUserChangePassword
<LI>NetUserDel
<LI>NetUserEnum
<LI>NetUserGetInfo
<LI>NetUserSetInfo
<LI>ZeroMemory
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CNetworkUser.cpp $<BR>
$Modtime: 6/26/01 10:49a $
</BODY>

</HTML>
*/
