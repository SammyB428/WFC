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
** $Workfile: cunc.hpp $
** $Revision: 12 $
** $Modtime: 6/26/01 11:05a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined ( UNC_CLASS_HEADER )

#define UNC_CLASS_HEADER

class CUniformResourceLocator;

class CUniversalNamingConvention
{
   public:

      CUniversalNamingConvention() noexcept;
      explicit CUniversalNamingConvention( _In_ CUniversalNamingConvention const& source ) noexcept;
      explicit CUniversalNamingConvention( _In_ CUniformResourceLocator const& source ) noexcept;
      explicit CUniversalNamingConvention( __in_z LPCTSTR source ) noexcept;

      /*
      ** Parse stuff like:
      ** \\buckaroo\public\neato\readme.txt
      */

      /*
      ** Data
      */

      std::wstring ServerName;
      std::wstring ShareName;
      std::wstring PathName;
      std::wstring UNC;

      /*
      ** Methods
      */

      _Check_return_ int Compare( _In_ CUniversalNamingConvention const& source ) const noexcept;
      void Copy( __in_z LPCTSTR source ) noexcept;
      void Copy( _In_ CUniversalNamingConvention const& source ) noexcept;
      void Copy( _In_ CUniformResourceLocator const& source ) noexcept;
      void Empty( void ) noexcept;
      void Make( void ) noexcept;

      _Check_return_ bool operator == ( _In_ CUniversalNamingConvention const& right_unc ) const noexcept;
      _Check_return_ CUniversalNamingConvention& operator = ( _In_ CUniversalNamingConvention const& source ) noexcept;
      _Check_return_ CUniversalNamingConvention& operator = ( _In_ CUniformResourceLocator const& source ) noexcept;
      _Check_return_ CUniversalNamingConvention& operator = ( __in_z LPCTSTR source ) noexcept;

      _Check_return_ operator LPCTSTR() const noexcept { return( UNC.c_str() ); }

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};   

#endif // UNC_CLASS_HEADER
