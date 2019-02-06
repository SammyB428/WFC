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

#if ! defined( DESKTOP_CLASS_HEADER )

#define DESKTOP_CLASS_HEADER

class CDesktop
{
   protected:

      HDESK m_DesktopHandle;

      DWORD m_ErrorCode;

      bool m_AutomaticallyClose;

   public:

      // Construction

       CDesktop(__in const CDesktop&) = delete;
       CDesktop& operator=(__in const CDesktop&) = delete;
       CDesktop();
      CDesktop( __in HDESK desktop_handle );

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

      virtual void  Attach( __in HDESK desktop_handle ) noexcept;
      virtual __checkReturn bool Close( void ) noexcept;
      virtual __checkReturn bool Create( __in const std::wstring& name_of_desktop,
                                  __in DWORD    desired_access = DESKTOP_CREATEWINDOW,
                     __in_opt LPSECURITY_ATTRIBUTES security_attributes_p = nullptr, 
                                  __in DWORD    flags = 0 ) noexcept;
      virtual __checkReturn bool  GetAutomaticallyClose( void ) const noexcept;
      virtual __checkReturn DWORD GetErrorCode( void ) const noexcept;
      virtual __checkReturn HDESK GetHandle( void ) const noexcept;
      virtual void  GetThread( __in DWORD thread_id = 0 ) noexcept;
      virtual __checkReturn bool  GetWindows( _Out_ std::vector<HWND>& window_handles ) noexcept; // EnumDesktopWindows
      virtual __checkReturn bool  Open( __in const std::wstring& desktop_name, __in DWORD desired_access = DESKTOP_CREATEWINDOW, __in DWORD flags = 0, __in bool inherit = false ) noexcept;
      virtual __checkReturn bool  OpenInput( __in DWORD desired_access = accessCreateWindow, __in DWORD flags = 0, __in bool inherit = false ) noexcept;
      virtual void  SetAutomaticallyClose( __in const bool automatically_close = true ) noexcept;
      virtual __checkReturn bool  SetThread( void ) noexcept;
      virtual __checkReturn bool  SwitchTo( __in const CDesktop& desktop_to_switch_to ) noexcept;

      /*
      ** Operators
      */

      operator HDESK () const noexcept;
};

#endif // DESKTOP_CLASS_HEADER
