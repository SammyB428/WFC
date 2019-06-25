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
** $Workfile: cregisty.hpp $
** $Revision: 15 $
** $Modtime: 6/26/01 11:04a $
*/

#if ! defined( REGISTRY_CLASS_HEADER )

#define REGISTRY_CLASS_HEADER

#define THIS_SUB_KEY FALSE
#define ALL_SUB_KEYS TRUE

#define SIGNAL_EVENT    true
#define WAIT_FOR_CHANGE false

class CRegistry
{
   private:

      void m_Initialize( void );

   protected:

      HKEY m_KeyHandle{ nullptr };
      HKEY m_RegistryHandle{ nullptr };

      LONG m_ErrorCode{ 0 };

      std::wstring m_ClassName;
      std::wstring m_ComputerName;
      std::wstring m_KeyName;
      std::wstring m_RegistryName;
      DWORD   m_NumberOfSubkeys{ 0 };
      DWORD   m_NumberOfValues{ 0 };

      /*
      ** Data items filled in by QueryInfo
      */

      DWORD    m_LongestSubkeyNameLength{ 0 };
      DWORD    m_LongestClassNameLength{ 0 };
      DWORD    m_LongestValueNameLength{ 0 };
      DWORD    m_LongestValueDataLength{ 0 };
      DWORD    m_SecurityDescriptorLength{ 0 };
      FILETIME m_LastWriteTime{ 0, 0 };

   public:

      enum _Keys
      {
         keyLocalMachine         = (size_t) HKEY_LOCAL_MACHINE,
         keyClassesRoot          = (size_t) HKEY_CLASSES_ROOT,
         keyPerformanceData      = (size_t) HKEY_PERFORMANCE_DATA,
         keyUsers                = (size_t) HKEY_USERS,
         keyCurrentUser          = (size_t) HKEY_CURRENT_USER,
#if ( WINVER >= 0x400 )
         keyCurrentConfiguration = (size_t) HKEY_CURRENT_CONFIG,
         keyDynamicData          = (size_t) HKEY_DYN_DATA
#endif
      };

      enum KeyValueTypes
      {
         typeBinary                 = REG_BINARY,
         typeDoubleWord             = REG_DWORD,
         typeDoubleWordLittleEndian = REG_DWORD_LITTLE_ENDIAN,
         typeDoubleWordBigEndian    = REG_DWORD_BIG_ENDIAN,
         typeUnexpandedString       = REG_EXPAND_SZ,
         typeSymbolicLink           = REG_LINK,
         typeMultipleString         = REG_MULTI_SZ,
         typeNone                   = REG_NONE,
         typeResourceList           = REG_RESOURCE_LIST,
         typeString                 = REG_SZ
      };

      enum CreateOptions
      {
         optionsNonVolatile = REG_OPTION_NON_VOLATILE,
         optionsVolatile    = REG_OPTION_VOLATILE
      };

      enum CreatePermissions
      {
         permissionAllAccess            = KEY_ALL_ACCESS,
         permissionCreateLink           = KEY_CREATE_LINK,
         permissionCreateSubKey         = KEY_CREATE_SUB_KEY,
         permissionEnumerateSubKeys     = KEY_ENUMERATE_SUB_KEYS,
         permissionExecute              = KEY_EXECUTE,
         permissionNotify               = KEY_NOTIFY,
         permissionQueryValue           = KEY_QUERY_VALUE,
         permissionRead                 = KEY_READ,
         permissionSetValue             = KEY_SET_VALUE,
         permissionWrite                = KEY_WRITE,
         permissionDelete               = DELETE,
         permissionReadControl          = READ_CONTROL,
         permissionWriteDAC             = WRITE_DAC,
         permissionWriteOwner           = WRITE_OWNER,
         permissionAccessSystemSecurity = ACCESS_SYSTEM_SECURITY,
#if defined( KEY_WOW64_64KEY )
         permission64Key                = KEY_WOW64_64KEY,
         permission32Key                = KEY_WOW64_32KEY,
#endif
      };

      enum CreationDisposition
      {
         dispositionCreatedNewKey     = REG_CREATED_NEW_KEY,
         dispositionOpenedExistingKey = REG_OPENED_EXISTING_KEY
      };

      enum NotifyChangeFlag
      {
         changeKeyAndSubkeys    = true,
         changeSpecifiedKeyOnly = false
      };

      enum NotifyChangeFilter
      {
         notifyName       = REG_NOTIFY_CHANGE_NAME,
         notifyAttributes = REG_NOTIFY_CHANGE_ATTRIBUTES,
         notifyLastSet    = REG_NOTIFY_CHANGE_LAST_SET,
         notifySecurity   = REG_NOTIFY_CHANGE_SECURITY
      };

      CRegistry(CRegistry const&) = delete;
      CRegistry& operator=(CRegistry const&) = delete;

      CRegistry();

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRegistry();

      /*
      ** Methods
      */

      virtual __checkReturn bool Close( void ) noexcept;
      virtual __checkReturn bool Connect( __in HKEY    key_to_open   = (HKEY) keyCurrentUser,
                            __in_z_opt wchar_t const * computer_name = nullptr ) noexcept;
      virtual __checkReturn bool Create( __in_z wchar_t const *             name_of_subkey,
                           __in_z_opt wchar_t const *         name_of_class         = nullptr,
                           __in CreateOptions         options               = optionsNonVolatile,
                           __in CreatePermissions     permissions           = permissionAllAccess,
                           __in_opt LPSECURITY_ATTRIBUTES security_attributes_p = nullptr,
                           __in_opt CreationDisposition * disposition_p         = nullptr ) noexcept;
      virtual __checkReturn bool DeleteKey( __in_z wchar_t const * name_of_subkey_to_delete ) noexcept;
      virtual __checkReturn bool DeleteValue( __in_z wchar_t const * name_of_value_to_delete ) noexcept;
      virtual __checkReturn bool EnumerateKeys( __in DWORD const subkey_index,
                                  __out std::wstring&    subkey_name,
                                  __out std::wstring&    class_name ) noexcept;
      virtual __checkReturn bool EnumerateValues( __in DWORD const  value_index,
                                    __out std::wstring&       name_of_value,
                                    __out KeyValueTypes& type_code,
                                    __out_bcount_part( size_of_data_buffer, size_of_data_buffer) LPBYTE data_buffer,
                                    __inout DWORD& size_of_data_buffer ) noexcept;
      virtual __checkReturn bool Flush( void ) noexcept;
      virtual __checkReturn bool GetBinaryValue( __in_z wchar_t const * name_of_value, __out std::vector<uint8_t>& return_array ) noexcept;

      // 2012-05-18 GetClassName gets #defined to be GetClassNameW which causes problems with managed C++ builds
      virtual void GetTheClassName( __out std::wstring& class_name ) const noexcept;
      virtual void GetComputerName( __out std::wstring& computer_name ) const noexcept;
      virtual __checkReturn bool GetDoubleWordValue( __in_z wchar_t const * name_of_value, __out DWORD& return_value ) noexcept;
      virtual __checkReturn DWORD GetErrorCode( void ) const noexcept;
      inline  __checkReturn HKEY GetHandle( void ) const noexcept { return( m_KeyHandle ); }
      virtual void GetKeyName( __out std::wstring& key_name ) const noexcept;
      virtual __checkReturn DWORD GetNumberOfSubkeys( void ) const noexcept;
      virtual __checkReturn DWORD GetNumberOfValues( void ) const noexcept;
      virtual void GetRegistryName( __out std::wstring& registry_name ) const noexcept;
      virtual __checkReturn bool GetSecurity( __in SECURITY_INFORMATION const what_you_want_to_know,
                                __out_bcount_part( size_of_data_buffer, size_of_data_buffer ) PSECURITY_DESCRIPTOR data_buffer,
                                __inout DWORD& size_of_data_buffer ) noexcept;
      virtual __checkReturn bool GetStringValue( __in_z wchar_t const * name_of_value, __out std::wstring& return_string ) noexcept;
      virtual __checkReturn bool GetStringArrayValue(__in_z wchar_t const * name_of_value, __out std::vector<std::wstring>& return_array) noexcept;
      virtual __checkReturn bool GetValue( __in_z wchar_t const * name_of_value, __out std::vector<uint8_t>& return_array ) noexcept;
      virtual __checkReturn bool GetValue( __in_z wchar_t const * name_of_value, __out DWORD& return_value ) noexcept;
      virtual __checkReturn bool GetValue( __in_z wchar_t const * name_of_value, __out std::wstring& return_string ) noexcept;
      virtual __checkReturn bool GetValue( __in_z wchar_t const * name_of_value, __out std::vector<std::wstring>& return_array ) noexcept;
      virtual __checkReturn bool Load( __in_z wchar_t const * name_of_subkey,
                         __in_z wchar_t const * name_of_file_containing_information ) noexcept;
      virtual __checkReturn bool NotifyChange( __in HANDLE const event_handle         = static_cast< HANDLE >( NULL ),
                                 __in NotifyChangeFilter const changes_to_be_reported = notifyLastSet,
                                 __in bool const this_subkey_or_all_subkeys           = changeSpecifiedKeyOnly,
                                 __in bool const wait_for_change_or_signal_event      = WAIT_FOR_CHANGE ) noexcept;
      virtual __checkReturn bool Open( __in_z wchar_t const * name_of_subkey_to_open,
                         __in const CreatePermissions security_access_mask = permissionAllAccess ) noexcept;
      virtual __checkReturn bool QueryInfo( void ) noexcept;
      virtual __checkReturn bool QueryValue( __in_z wchar_t const * name_of_value,
                               __inout KeyValueTypes& value_type,
                               __out_bcount_part( size_of_buffer, size_of_buffer ) LPBYTE address_of_buffer,
                               __inout SIZE_T&        size_of_buffer ) noexcept;
      virtual __checkReturn bool Replace( __in_z wchar_t const * name_of_subkey,
                            __in_z wchar_t const * name_of_file_with_new_data,
                            __in_z wchar_t const * name_of_backup_file ) noexcept;
      virtual __checkReturn bool Restore( __in_z wchar_t const * name_of_file_holding_saved_tree, __in const DWORD volitility_flags = 0 ) noexcept;
      virtual __checkReturn bool Save( __in_z wchar_t const * name_of_file_to_hold_tree, __in_opt LPSECURITY_ATTRIBUTES security_attributes_p = nullptr ) noexcept;
      virtual __checkReturn bool SetBinaryValue( __in_z wchar_t const * name_of_value, __in const std::vector<uint8_t>& bytes_to_write ) noexcept;
      virtual __checkReturn bool SetDoubleWordValue( __in_z wchar_t const * name_of_value, __in DWORD value_to_write ) noexcept;
      virtual __checkReturn bool SetSecurity( __in const SECURITY_INFORMATION& security_information,
                                __in PSECURITY_DESCRIPTOR const security_descriptor_p ) noexcept;
      virtual __checkReturn bool SetStringValue( __in_z wchar_t const * name_of_value, __in const std::wstring& string_value ) noexcept;
      virtual __checkReturn bool SetStringArrayValue( __in_z wchar_t const * name_of_value, __in const std::vector<std::wstring>& string_array ) noexcept;
      virtual __checkReturn bool SetValue( __in_z wchar_t const * name_of_value, __in const std::vector<uint8_t>& bytes_to_write ) noexcept;
      virtual __checkReturn bool SetValue( __in_z wchar_t const * name_of_value, __in DWORD value ) noexcept;
      virtual __checkReturn bool SetValue( __in_z wchar_t const * name_of_value, __in const std::vector<std::wstring>& strings_to_write ) noexcept;
      virtual __checkReturn bool SetValue( __in_z wchar_t const * name_of_value, __in const std::wstring& string_to_write ) noexcept;

      virtual __checkReturn bool SetValue( __in_z const wchar_t const *             name_of_subkey,
                             __in KeyValueTypes const type_of_value_to_set,
                             __in_bcount_opt( size_of_data ) uint8_t const * address_of_value_data,
                             __in std::size_t const       size_of_data ) noexcept;

      virtual __checkReturn bool UnLoad( __in_z wchar_t const * name_of_subkey_to_unload ) noexcept;

      inline void GetLastWriteTime( __out FILETIME& last_write_time ) const noexcept
      {
         last_write_time.dwHighDateTime = m_LastWriteTime.dwHighDateTime;
         last_write_time.dwLowDateTime  = m_LastWriteTime.dwLowDateTime;
      }

      /*
      ** Operators
      */

      inline operator HKEY() const noexcept { return( GetHandle() ); }

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

#endif // REGISTRY_CLASS_HEADER
