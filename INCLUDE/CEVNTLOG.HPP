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
** $Workfile: cevntlog.hpp $
** $Revision: 17 $
** $Modtime: 6/26/01 11:01a $
*/

#if ! defined( EVENT_LOG_CLASS )

#define EVENT_LOG_CLASS

class CEventLog
{
   protected:

       HANDLE m_LogHandle{ nullptr };
       HANDLE m_EventSourceHandle{ nullptr };

      DWORD m_ErrorCode{ 0 };
      DWORD m_NumberOfBytesRead{ 0 };
      DWORD m_NumberOfBytesInNextRecord{ 0 };

      void m_Initialize( void ) noexcept;

   public:

      enum class EventType
      {
         eventError       = EVENTLOG_ERROR_TYPE,
         eventWarning     = EVENTLOG_WARNING_TYPE,
         eventInformation = EVENTLOG_INFORMATION_TYPE,
         eventSuccess     = EVENTLOG_AUDIT_SUCCESS,
         eventFailure     = EVENTLOG_AUDIT_FAILURE
      };

      static constexpr DWORD messageNumberZero = 0x40000000L;

      CEventLog(CEventLog const&) = delete;
      CEventLog& operator=(CEventLog const&) = delete;

      CEventLog() noexcept;
      CEventLog( __in_z wchar_t const * source_name ) noexcept;

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CEventLog();

      /*
      ** Data
      */

      std::wstring ComputerName;
      std::wstring LogName;

      /*
      ** Methods
      */

      virtual _Check_return_ bool  Backup( __in_z LPCTSTR name_of_backup_file ) noexcept;
      virtual _Check_return_ bool  Clear(  __in_z LPCTSTR name_of_backup_file ) noexcept;
      virtual _Check_return_ bool  Close( void ) noexcept;
      virtual _Check_return_ bool  CreateApplicationLog( __in_z wchar_t const * application_name,
                                          __in_z wchar_t const * file_containing_message_table_resource,
                                          __in DWORD   supported_types ) noexcept;
      virtual _Check_return_ bool  CreateCustomLog( __in_z LPCTSTR log_name, __in_z_opt LPCTSTR log_filename = nullptr, __in DWORD maximum_file_size = 0x80000, __in DWORD overwrite_after_this_many_seconds = 604800 ) noexcept;
      virtual _Check_return_ bool  Delete( __in_z wchar_t const * log_name ) noexcept; // Very DANGEROUS! Will delete any log you specify
      virtual _Check_return_ bool  DeleteApplicationLog( __in_z wchar_t const * application_name ) noexcept;
      virtual _Check_return_ bool  DeregisterSource( void ) noexcept;
      virtual _Check_return_ bool  EnumerateLogs( __out DWORD& enumerator ) const noexcept;
      inline constexpr _Check_return_ DWORD GetErrorCode(void) const noexcept { return(m_ErrorCode); }
      inline constexpr _Check_return_ HANDLE GetHandle(void) const noexcept { return(m_LogHandle); }
      virtual _Check_return_ bool  GetNextLog( __inout DWORD& enumerator, __out std::wstring& log_name ) noexcept;
      virtual _Check_return_ bool  GetNumberOfRecords( __out DWORD& number_of_records ) noexcept;
      virtual _Check_return_ DWORD GetOldestRecordNumber( void ) noexcept;
      virtual _Check_return_ bool  NotifyChange( __in HANDLE event_handle, __in_opt HANDLE log_handle = nullptr ) noexcept;
      virtual _Check_return_ bool  OpenBackup( __in_z LPCTSTR name_of_backup_file, __in_z_opt LPCTSTR name_of_computer = nullptr ) noexcept;
      virtual _Check_return_ bool  Open( __in_z LPCTSTR log_name, __in_z_opt LPCTSTR name_of_computer = nullptr ) noexcept;
      virtual _Check_return_ bool  Read( __in DWORD  record_number,
                          __out CEventLogRecord& record,
                          __in DWORD  how_to_read = EVENTLOG_FORWARDS_READ bitor EVENTLOG_SEQUENTIAL_READ ) noexcept;
      virtual _Check_return_ bool  Read( __in DWORD  record_number, 
                          __out_bcount_part( number_of_bytes_to_read, number_of_bytes_to_read ) LPVOID buffer, 
                          __inout DWORD& number_of_bytes_to_read, 
                          __in DWORD  how_to_read = EVENTLOG_FORWARDS_READ bitor EVENTLOG_SEQUENTIAL_READ ) noexcept;
      virtual _Check_return_ bool  RegisterSource( __in_z wchar_t const * source_name, __in_z_opt wchar_t const * name_of_computer = nullptr ) noexcept;
      virtual _Check_return_ bool  Report( __in EventType event_type,
                            __in WORD      category,
                            __in DWORD     event_id,
                            __in WORD      number_of_strings        = 0,
                            __in_ecount_z_opt( number_of_strings ) LPCTSTR * string_array = nullptr,
                            __in DWORD     number_of_raw_data_bytes = 0,
                            __in_bcount_opt( number_of_raw_data_bytes ) LPVOID    raw_data_buffer = nullptr,
                            __in_opt PSID      user_sid  = nullptr ) noexcept;
      virtual _Check_return_ bool  Report( __in_z LPCTSTR   log_name, 
                            __in DWORD     message_string_resource_id,
                            __in WORD      number_of_strings = 0,
                            __in_ecount_z_opt( number_of_strings ) LPCTSTR * string_array = nullptr ) noexcept;
      virtual void ReportError(_In_z_ wchar_t const * string_to_report ) noexcept;
      virtual void ReportInformation( _In_z_ wchar_t const * string_to_report ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING ) 

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // EVENT_LOG_CLASS
