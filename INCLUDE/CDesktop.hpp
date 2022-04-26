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
** $Workfile: CDesktop.hpp $
** $Revision: 10 $
** $Modtime: 6/26/01 11:01a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( DESKTOP_CLASS_HEADER )

#define DESKTOP_CLASS_HEADER

class CDesktop
{
   protected:

       HDESK m_DesktopHandle{ nullptr };

       DWORD m_ErrorCode{ 0 };

       bool m_AutomaticallyClose{ false };

   public:

      // Construction

       CDesktop(_In_ CDesktop const&) = delete;
       CDesktop& operator=(_In_ CDesktop const&) = delete;

       CDesktop() noexcept;
       explicit CDesktop( _In_ HDESK desktop_handle ) noexcept;

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CDesktop();

      enum _Accesses
      {
         accessCreateMenu      = DESKTOP_CREATEMENU,
         accessCreateWindow    = DESKTOP_CREATEWINDOW,
         accessEnumerate       = DESKTOP_ENUMERATE,
         accessHookControl     = DESKTOP_HOOKCONTROL,
         accessJournalPlayback = DESKTOP_JOURNALPLAYBACK,
         accessJournalRecord   = DESKTOP_JOURNALRECORD,
         accessReadObjects     = DESKTOP_READOBJECTS,
         accessSwitchDesktop   = DESKTOP_SWITCHDESKTOP,
         accessWriteObjects    = DESKTOP_WRITEOBJECTS
      };

      /*
      ** Methods
      */

      virtual void  Attach( _In_ HDESK desktop_handle ) noexcept;
      virtual _Check_return_ bool Close( void ) noexcept;
      virtual _Check_return_ bool Create( _In_ std::wstring_view name_of_desktop,
                                  _In_ DWORD    desired_access = DESKTOP_CREATEWINDOW,
                     __in_opt LPSECURITY_ATTRIBUTES security_attributes_p = nullptr, 
                                  _In_ DWORD    flags = 0 ) noexcept;
      inline constexpr [[nodiscard]] _Check_return_ bool  GetAutomaticallyClose(void) const noexcept { return(m_AutomaticallyClose); }
      inline constexpr [[nodiscard]] _Check_return_ DWORD GetErrorCode(void) const noexcept { return(m_ErrorCode); };
      inline constexpr [[nodiscard]] _Check_return_ HDESK GetHandle(void) const noexcept { return(m_DesktopHandle); }
      virtual void  GetThread( _In_ DWORD thread_id = 0 ) noexcept;
      virtual _Check_return_ bool  GetWindows( _Out_ std::vector<HWND>& window_handles ) noexcept; // EnumDesktopWindows
      virtual _Check_return_ bool  Open( _In_ std::wstring_view desktop_name, _In_ DWORD desired_access = DESKTOP_CREATEWINDOW, _In_ DWORD flags = 0, _In_ bool inherit = false ) noexcept;
      virtual _Check_return_ bool  OpenInput( _In_ DWORD desired_access = accessCreateWindow, _In_ DWORD flags = 0, _In_ bool inherit = false ) noexcept;
      virtual void  SetAutomaticallyClose( _In_ bool const automatically_close = true ) noexcept;
      virtual _Check_return_ bool  SetThread( void ) noexcept;
      virtual _Check_return_ bool  SwitchTo( _In_ CDesktop const& desktop_to_switch_to ) noexcept;

      /*
      ** Operators
      */

      operator HDESK () const noexcept;
};

#endif // DESKTOP_CLASS_HEADER
