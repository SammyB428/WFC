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
** $Workfile: CNetworkUserInformation.cpp $
** $Revision: 8 $
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

Win32FoundationClasses::CNetworkUserInformation::CNetworkUserInformation() noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CNetworkUserInformation::CNetworkUserInformation( _In_ USER_INFO_0 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkUserInformation::CNetworkUserInformation( _In_ USER_INFO_1 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkUserInformation::CNetworkUserInformation( _In_ USER_INFO_2 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkUserInformation::CNetworkUserInformation( _In_ USER_INFO_3 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkUserInformation::CNetworkUserInformation( _In_ USER_INFO_10 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkUserInformation::CNetworkUserInformation( _In_ USER_INFO_11 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkUserInformation::CNetworkUserInformation( _In_ USER_INFO_20 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkUserInformation::CNetworkUserInformation( _In_ USER_INFO_21 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkUserInformation::CNetworkUserInformation( _In_ USER_INFO_22 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkUserInformation::CNetworkUserInformation( _In_ Win32FoundationClasses::CNetworkUserInformation const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkUserInformation::CNetworkUserInformation( _In_ Win32FoundationClasses::CNetworkUserInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

void Win32FoundationClasses::CNetworkUserInformation::Copy( _In_ USER_INFO_0 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   m_Initialize();

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      WFC_VALIDATE_POINTER( source->usri0_name );

      if ( source->usri0_name == nullptr )
      {
         Name.clear();
         return;
      }

      Name.assign( source->usri0_name);
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkUserInformation::Copy( _In_ USER_INFO_1 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   m_Initialize();

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      Comment.assign( source->usri1_comment );
      HomeDirectory.assign( source->usri1_home_dir );
      Name.assign( source->usri1_name );
      Password.assign( source->usri1_password );
      ScriptPath.assign( source->usri1_script_path );

      Flags       = source->usri1_flags;
      PasswordAge = CTimeSpan( source->usri1_password_age );
      Privileges  = source->usri1_priv;
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkUserInformation::Copy( _In_ USER_INFO_2 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   m_Initialize();

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      Comment.assign( source->usri2_comment );
      FullName.assign( source->usri2_full_name );
      HomeDirectory.assign( source->usri2_home_dir );
      LogonServer.assign( source->usri2_logon_server );
      Name.assign( source->usri2_name );
      Parameters.assign( source->usri2_parms );
      Password.assign( source->usri2_password );
      ScriptPath.assign( source->usri2_script_path );
      UserComment.assign( source->usri2_usr_comment );
      Workstations.assign( source->usri2_workstations );
 
      AccountExpires      = CTime( (time_t) source->usri2_acct_expires );
      AuthenticationFlags = source->usri2_auth_flags;
      BadPasswordCount    = source->usri2_bad_pw_count;
      CodePage            = source->usri2_code_page;
      CountryCode         = source->usri2_country_code;
      Flags               = source->usri2_flags;
      LastLogoff          = CTime( (time_t) source->usri2_last_logoff );
      LastLogon           = CTime( (time_t) source->usri2_last_logon );
      MaximumStorage      = source->usri2_max_storage;
      NumberOfLogons      = source->usri2_num_logons;
      PasswordAge         = CTimeSpan( source->usri2_password_age );
      Privileges          = source->usri2_priv;
      UnitsPerWeek        = source->usri2_units_per_week;
   
      for ( auto const index : Range(21) )
      {
         LogonHours.push_back( source->usri2_logon_hours[ index ] );
      }
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkUserInformation::Copy( _In_ USER_INFO_3 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   m_Initialize();

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      Comment.assign( source->usri3_comment );
      FullName.assign( source->usri3_full_name );
      HomeDirectory.assign( source->usri3_home_dir );
      HomeDirectoryDrive.assign( source->usri3_home_dir_drive );
      Name.assign( source->usri3_name );
      LogonServer.assign( source->usri3_logon_server );
      Parameters.assign( source->usri3_parms );
      Password.assign( source->usri3_password );
      Profile.assign( source->usri3_profile );
      ScriptPath.assign( source->usri3_script_path );
      UserComment.assign( source->usri3_usr_comment );
      Workstations.assign( source->usri3_workstations );
   
      AccountExpires      = CTime( (time_t) source->usri3_acct_expires );
      AuthenticationFlags = source->usri3_auth_flags;
      BadPasswordCount    = source->usri3_bad_pw_count;
      CodePage            = source->usri3_code_page;
      CountryCode         = source->usri3_country_code;
      Flags               = source->usri3_flags;
      LastLogoff          = CTime( (time_t) source->usri3_last_logoff );
      LastLogon           = CTime( (time_t) source->usri3_last_logon );
      MaximumStorage      = source->usri3_max_storage;
      NumberOfLogons      = source->usri3_num_logons;
      PasswordAge         = CTimeSpan( source->usri3_password_age );
      PasswordHasExpired  = source->usri3_password_expired;
      PrimaryGroupID      = source->usri3_primary_group_id;
      Privileges          = source->usri3_priv;
      UnitsPerWeek        = source->usri3_units_per_week;
      UserID              = source->usri3_user_id;

      for ( auto const index : Range(21) )
      {
         LogonHours.push_back( source->usri3_logon_hours[ index ] );
      }
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkUserInformation::Copy( _In_ USER_INFO_10 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   m_Initialize();

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      Comment.assign( source->usri10_comment );
      FullName.assign( source->usri10_full_name);
      Name.assign( source->usri10_name);
      UserComment.assign( source->usri10_usr_comment);
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkUserInformation::Copy( _In_ USER_INFO_11 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   m_Initialize();

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      Comment.assign( source->usri11_comment );
      FullName.assign( source->usri11_full_name );
      HomeDirectory.assign( source->usri11_home_dir );
      LogonServer.assign( source->usri11_logon_server );
      Name.assign( source->usri11_name );
      Parameters.assign( source->usri11_parms );
      UserComment.assign( source->usri11_usr_comment );
      Workstations.assign( source->usri11_workstations );

      AuthenticationFlags = source->usri11_auth_flags;
      BadPasswordCount    = source->usri11_bad_pw_count;
      CodePage            = source->usri11_code_page;
      CountryCode         = source->usri11_country_code;
      PasswordAge         = CTimeSpan( source->usri11_password_age );
      Privileges          = source->usri11_priv;
      LastLogon           = CTime( (time_t) source->usri11_last_logon );
      LastLogoff          = CTime( (time_t) source->usri11_last_logoff );
      MaximumStorage      = source->usri11_max_storage;
      NumberOfLogons      = source->usri11_num_logons;
      UnitsPerWeek        = source->usri11_units_per_week;
   
      for ( auto const index : Range(21) )
      {
         LogonHours.push_back( source->usri11_logon_hours[ index ] );
      }
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkUserInformation::Copy( _In_ USER_INFO_20 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   m_Initialize();

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      Comment.assign( source->usri20_comment );
      FullName.assign( source->usri20_full_name );
      Name.assign( source->usri20_name );

      Flags  = source->usri20_flags;
      UserID = source->usri20_user_id;
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkUserInformation::Copy( _In_ USER_INFO_21 const *source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   m_Initialize();

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      for ( auto const index : Range(ENCRYPTED_PWLEN) )
      {
         EncryptedPassword.push_back( source->usri21_password[ index ] );
      }
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkUserInformation::Copy( _In_ USER_INFO_22 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   m_Initialize();

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      Comment.assign( source->usri22_comment);
      FullName.assign( source->usri22_full_name );
      HomeDirectory.assign( source->usri22_home_dir );
      LogonServer.assign( source->usri22_logon_server );
      Name.assign( source->usri22_name );
      Parameters.assign( source->usri22_parms );
      ScriptPath.assign( source->usri22_script_path );
      UserComment.assign( source->usri22_usr_comment );
      Workstations.assign( source->usri22_workstations );
    
      AccountExpires      = CTime( (time_t) source->usri22_acct_expires );
      AuthenticationFlags = source->usri22_auth_flags;
      BadPasswordCount    = source->usri22_bad_pw_count;
      CodePage            = source->usri22_code_page;
      CountryCode         = source->usri22_country_code;
      Flags               = source->usri22_flags;
      LastLogoff          = CTime( (time_t) source->usri22_last_logoff );
      LastLogon           = CTime( (time_t) source->usri22_last_logon );
      MaximumStorage      = source->usri22_max_storage;
      NumberOfLogons      = source->usri22_num_logons;
      PasswordAge         = CTimeSpan( source->usri22_password_age );
      Privileges          = source->usri22_priv;
      UnitsPerWeek        = source->usri22_units_per_week;
 
      for ( auto const index : Range(ENCRYPTED_PWLEN) )
      {
         EncryptedPassword.push_back( source->usri22_password[ index ] );
      }
   
      for ( auto const index : Range(21) )
      {
         LogonHours.push_back( source->usri22_logon_hours[ index ] );
      }
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkUserInformation::Copy( _In_ Win32FoundationClasses::CNetworkUserInformation const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   /*
   ** Make sure we ain't copying ourselves
   */

   if ( this == nullptr or this == source )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      Copy( *source );
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkUserInformation::Copy( _In_ Win32FoundationClasses::CNetworkUserInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this not_eq &source );

   /*
   ** Make sure we ain't copying ourselves
   */

   if ( this == &source )
   {
      return;
   }

   AccountExpires      = source.AccountExpires;
   AuthenticationFlags = source.AuthenticationFlags;
   BadPasswordCount    = source.BadPasswordCount;
   CodePage            = source.CodePage;
   Comment.assign( source.Comment );
   CountryCode         = source.CountryCode;
   Flags               = source.Flags;
   FullName.assign( source.FullName );
   HomeDirectory.assign(source.HomeDirectory);
   LastLogoff          = source.LastLogoff;
   LastLogon           = source.LastLogon;
   LogonServer.assign(source.LogonServer);
   MaximumStorage      = source.MaximumStorage;
   Name.assign(source.Name);
   NumberOfLogons      = source.NumberOfLogons;
   Parameters.assign(source.Parameters);
   Password.assign(source.Password);
   PasswordHasExpired  = source.PasswordHasExpired;
   PasswordAge         = source.PasswordAge;
   PrimaryGroupID      = source.PrimaryGroupID;
   Privileges          = source.Privileges;
   Profile.assign(source.Profile);
   ScriptPath.assign(source.ScriptPath);
   UnitsPerWeek        = source.UnitsPerWeek;
   UserComment.assign(source.UserComment);
   UserID              = source.UserID;
   Workstations.assign(source.Workstations);
   HomeDirectoryDrive.assign(source.HomeDirectoryDrive);

   EncryptedPassword = source.EncryptedPassword;
   LogonHours = source.LogonHours;
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CNetworkUserInformation::Dump( CDumpContext& dump_context ) const
{
   WFC_VALIDATE_POINTER( this );

   dump_context << TEXT( "a CNetworkUserInformation at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   AccountExpires is " ) << AccountExpires << TEXT( "\n" );
   dump_context << TEXT( "   AuthenticationFlags is " ) << AuthenticationFlags << TEXT( " (" );

   std::wstring temp_string;

   if ( AuthenticationFlags bitand AF_OP_PRINT )
   {
      temp_string = TEXT( "AF_OP_PRINT" );
   }

   if ( AuthenticationFlags bitand AF_OP_COMM )
   {
      if ( temp_string.empty() == false )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "AF_OP_COMM" );
   }
   
   if ( AuthenticationFlags bitand AF_OP_SERVER )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "AF_OP_SERVER" );
   }
   
   if ( AuthenticationFlags bitand AF_OP_ACCOUNTS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "AF_OP_ACCOUNTS" );
   }

   dump_context << temp_string << TEXT( " )\n" );
   
   dump_context << TEXT( "   BadPasswordCount is " ) << BadPasswordCount << TEXT( "\n" );
   dump_context << TEXT( "   CodePage is "         ) << CodePage         << TEXT( "\n" );
   dump_context << TEXT( "   Comment is "          ) << Comment          << TEXT( "\n" );
   dump_context << TEXT( "   CountryCode is "      ) << CountryCode      << TEXT( "\n" );
   // EncryptedPassword
   
   dump_context << TEXT( "   Flags is "            ) << Flags            << TEXT( " ( " );

   temp_string.Empty();

   if ( Flags bitand UF_SCRIPT )
   {
      temp_string = TEXT( "UF_SCRIPT" );
   }

   if ( Flags bitand UF_ACCOUNTDISABLE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "UF_ACCOUNTDISABLE" );
   }

   if ( Flags bitand UF_HOMEDIR_REQUIRED )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "UF_HOMEDIR_REQUIRED" );
   }

   if ( Flags bitand UF_PASSWD_NOTREQD )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "UF_PASSWD_NOTREQD" );
   }

   if ( Flags bitand UF_PASSWD_CANT_CHANGE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "UF_PASSWD_CANT_CHANGE" );
   }

   if ( Flags bitand UF_LOCKOUT )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "UF_LOCKOUT" );
   }

   if ( Flags bitand UF_DONT_EXPIRE_PASSWD )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "UF_DONT_EXPIRE_PASSWD" );
   }

   if ( Flags bitand UF_NORMAL_ACCOUNT )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "UF_NORMAL_ACCOUNT" );
   }

   if ( Flags bitand UF_TEMP_DUPLICATE_ACCOUNT )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "UF_TEMP_DUPLICATE_ACCOUNT" );
   }

   if ( Flags bitand UF_WORKSTATION_TRUST_ACCOUNT )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "UF_WORKSTATION_TRUST_ACCOUNT" );
   }

   if ( Flags bitand UF_SERVER_TRUST_ACCOUNT )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "UF_SERVER_TRUST_ACCOUNT" );
   }

   if ( Flags bitand UF_INTERDOMAIN_TRUST_ACCOUNT )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "UF_INTERDOMAIN_TRUST_ACCOUNT" );
   }

   dump_context << temp_string << TEXT( " )\n" );

   dump_context << TEXT( "   FullName is " ) << FullName << TEXT( "\n" );
   dump_context << TEXT( "   HomeDirectory is " ) << HomeDirectory << TEXT( "\n" );
   dump_context << TEXT( "   HomeDirectoryDrive is " ) << HomeDirectoryDrive << TEXT( "\n" );
   dump_context << TEXT( "   LastLogoff is " ) << LastLogoff << TEXT( "\n" );
   dump_context << TEXT( "   LastLogon  is " ) << LastLogon  << TEXT( "\n" );
   //LogonHours.RemoveAll();
   dump_context << TEXT( "   LogonServer is " ) << LogonServer << TEXT( "\n" );
   dump_context << TEXT( "   MaximumStorage is " ) << MaximumStorage << TEXT( "\n" );
   dump_context << TEXT( "   Name is " ) << Name << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfLogons is " ) << NumberOfLogons << TEXT( "\n" );
   dump_context << TEXT( "   Parameters is " ) << Parameters << TEXT( "\n" );
   dump_context << TEXT( "   Password is " ) << Password << TEXT( "\n" );
   dump_context << TEXT( "   PasswordAge is " ) << PasswordAge << TEXT( "\n" );
   dump_context << TEXT( "   PasswordHasExpired is " ) << PasswordHasExpired << TEXT( "\n" );
   dump_context << TEXT( "   PrimaryGroupID is " ) << PrimaryGroupID << TEXT( " ( " );

   temp_string.Empty();

   if ( PrimaryGroupID == DOMAIN_GROUP_RID_USERS )
   {
      temp_string = TEXT( "DOMAIN_GROUP_RID_USERS" );
   }
   else if ( PrimaryGroupID == DOMAIN_GROUP_RID_ADMINS )
   {
      temp_string = TEXT( "DOMAIN_GROUP_RID_ADMINS" );
   }
   else if ( PrimaryGroupID == DOMAIN_GROUP_RID_GUESTS )
   {
      temp_string = TEXT( "DOMAIN_GROUP_RID_GUESTS" );
   }
   else
   {
      temp_string = TEXT( "unknown" );
   }

   dump_context << temp_string << TEXT( " )\n" );

   dump_context << TEXT( "   Privileges is " ) << Privileges << TEXT( " ( " );

   temp_string.Empty();

   switch( Privileges )
   {
      case USER_PRIV_GUEST:

         temp_string = TEXT( "USER_PRIV_GUEST" );
         break;

      case USER_PRIV_USER:

         temp_string = TEXT( "USER_PRIV_USER" );
         break;

      case USER_PRIV_ADMIN:

         temp_string = TEXT( "USER_PRIV_ADMIN" );
         break;

      default:

         temp_string = TEXT( "unknown" );
         break;
   }

   dump_context << temp_string << TEXT( " )\n" );

   dump_context << TEXT( "   Profile is " ) << Profile << TEXT( "\n" );
   dump_context << TEXT( "   ScriptPath is " ) << ScriptPath << TEXT( "\n" );
   dump_context << TEXT( "   UnitsPerWeek is " ) << UnitsPerWeek << TEXT( " ( " );

   temp_string.Empty();

   switch( UnitsPerWeek )
   {
      case UNITS_PER_WEEK:

         temp_string = TEXT( "UNITS_PER_WEEK" );
         break;

#if 0
      case SAM_DAYS_PER_WEEK:

         temp_string = TEXT( "SAM_DAYS_PER_WEEK" );
         break;

      case SAM_HOURS_PER_WEEK:

         temp_string = TEXT( "SAM_HOURS_PER_WEEK" );
         break;

      case SAM_MINUTES_PER_WEEK:

         temp_string = TEXT( "SAM_MINUTES_PER_WEEK" );
         break;
#endif 
      default:

         temp_string = TEXT( "unknown" );
         break;
   }

   dump_context << temp_string << TEXT( " )\n" );

   dump_context << TEXT( "   UserComment is " ) << UserComment << TEXT( "\n" );
   dump_context << TEXT( "   UserID is " ) << UserID << TEXT( "\n" );
   dump_context << TEXT( "   Workstations is " ) << Workstations << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CNetworkUserInformation::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void Win32FoundationClasses::CNetworkUserInformation::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   AccountExpires = CTime( (time_t) 0 );
   AuthenticationFlags = 0;
   BadPasswordCount = 0;
   CodePage = 0;
   Comment.clear();
   CountryCode = 0;
   EncryptedPassword.clear();
   Flags = 0;
   FullName.clear();
   HomeDirectory.clear();
   HomeDirectoryDrive.clear();
   LastLogoff = CTime( (time_t) 0 );
   LastLogon = CTime( (time_t) 0 );
   LogonHours.clear();
   LogonServer.clear();
   MaximumStorage = 0;
   Name.clear();
   NumberOfLogons = 0;
   Parameters.clear();
   Password.clear();
   PasswordAge = CTimeSpan( 0 );
   PasswordHasExpired = 0;
   PrimaryGroupID = 0;
   Privileges = 0;
   Profile.clear();
   ScriptPath.clear();
   UnitsPerWeek = 0;
   UserComment.clear();
   UserID = 0;
   Workstations.clear();
}

void Win32FoundationClasses::CNetworkUserInformation::SetAddDefaults( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   AccountExpires      = TIMEQ_FOREVER;
   AuthenticationFlags = 0;
   CodePage            = 0;
   CountryCode         = 0;
   Flags               = UF_DONT_EXPIRE_PASSWD bitor
                         UF_NORMAL_ACCOUNT bitor
                         UF_SCRIPT;
   FullName.assign(Name);
   MaximumStorage      = USER_MAXSTORAGE_UNLIMITED;
   Privileges          = privilegeUser;
   UserComment.assign(WSTRING_VIEW(L"Added by WFC"));

   Parameters.clear();
   Workstations.clear();
   LogonServer.clear();

   for ( auto const index : Range(21) )
   {
      LogonHours.push_back( 0xFF );
   }
}

Win32FoundationClasses::CNetworkUserInformation const& Win32FoundationClasses::CNetworkUserInformation::operator = ( _In_ Win32FoundationClasses::CNetworkUserInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this not_eq &source );

   if ( this not_eq &source )
   {
      Copy( source );
   }

   return( *this );
}

// End of source
