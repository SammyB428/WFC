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
** $Workfile: CPasswordChangeFilter.hpp $
** $Revision: 6 $
** $Modtime: 6/26/01 11:03a $
*/

#if ! defined( PASSWORD_CHANGE_FILTER_CLASS_HEADER )

#define PASSWORD_CHANGE_FILTER_CLASS_HEADER

struct PASSWORD_FILTER_STRING
{
    USHORT  Length{ 0 };
    USHORT  MaximumLength{ 0 };
    WCHAR* UnicodeString{ nullptr };
};

class CPasswordChangeFilter
{
   public:

       CPasswordChangeFilter(__in CPasswordChangeFilter const&) = delete;
       CPasswordChangeFilter& operator=(__in CPasswordChangeFilter const&) = delete;
       
       CPasswordChangeFilter();

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CPasswordChangeFilter();

      virtual _Check_return_ bool AddFilter(_In_ std::wstring const& dll_name ) noexcept;
      virtual _Check_return_ bool GetCurrentFilters( __out std::vector<std::wstring>& current_filters ) noexcept;
      virtual _Check_return_ bool OnChanged( __in PASSWORD_FILTER_STRING * user_name, __in ULONG relative_id, __in PASSWORD_FILTER_STRING * new_password ) noexcept;
      virtual _Check_return_ bool OnFilter( __in PASSWORD_FILTER_STRING * user_name, __in PASSWORD_FILTER_STRING * full_user_name, __in PASSWORD_FILTER_STRING * new_password, __in bool was_set ) noexcept;
      virtual _Check_return_ bool OnInitialize( void ) noexcept;
      virtual _Check_return_ bool RemoveFilter( __in std::wstring const& dll_name ) noexcept;
};

#endif // PASSWORD_CHANGE_FILTER_CLASS_HEADER
