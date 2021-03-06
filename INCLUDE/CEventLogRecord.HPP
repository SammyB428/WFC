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
** $Workfile: CEventLogRecord.HPP $
** $Revision: 8 $
** $Modtime: 6/26/01 11:01a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined ( EVENT_LOG_RECORD_CLASS_HEADER )

#define EVENT_LOG_RECORD_CLASS_HEADER

class CEventLogRecord
{
   public:

      CEventLogRecord() noexcept;
      CEventLogRecord( _In_ CEventLogRecord const& source ) noexcept;
      CEventLogRecord( _In_ EVENTLOGRECORD const * source ) noexcept;

      /*
      ** Data
      */

      std::wstring Source;
      std::wstring ComputerName;
      DWORD      RecordNumber{ 0 };
      DWORD      Reserved{ 0 };
      CTime      TimeGenerated;
      CTime      TimeWritten;
      DWORD      EventID{ 0 };
      WORD       EventType{ 0 };
      WORD       EventCategory{ 0 };
      WORD       PairedEventFlags{ 0 };
      DWORD      ClosingRecordNumber{ 0 };
      std::vector<uint8_t> UserSID;
      std::vector<uint8_t> Data;

      std::vector<std::wstring> Strings;

      /*
      ** Methods
      */

      _Check_return_ int Compare( _In_ CEventLogRecord const& source ) const noexcept;
      void Copy( _In_ EVENTLOGRECORD const * source ) noexcept;
      void Copy( _In_ CEventLogRecord const& source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ bool operator == ( _In_ CEventLogRecord const& right_record ) const noexcept;
      _Check_return_ CEventLogRecord& operator = ( _In_ CEventLogRecord const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};   

#endif // EVENT_LOG_RECORD_CLASS_HEADER
