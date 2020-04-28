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
** $Workfile: CNetworkResourceInformation.cpp $
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

Win32FoundationClasses::CNetworkResourceInformation::CNetworkResourceInformation()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CNetworkResourceInformation::CNetworkResourceInformation( _In_ NETRESOURCE const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkResourceInformation::CNetworkResourceInformation( _In_ Win32FoundationClasses::CNetworkResourceInformation const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CNetworkResourceInformation::~CNetworkResourceInformation()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void Win32FoundationClasses::CNetworkResourceInformation::Copy( _In_ NETRESOURCE const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      m_Initialize();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      WFC_VALIDATE_POINTER_NULL_OK( source->lpLocalName );

      if ( source->lpLocalName not_eq nullptr )
      {
         LocalName.assign(source->lpLocalName);
      }
      else
      {
         LocalName.clear();
      }

      WFC_VALIDATE_POINTER_NULL_OK( source->lpRemoteName );

      if ( source->lpRemoteName not_eq nullptr )
      {
         RemoteName.assign(source->lpRemoteName);
      }
      else
      {
         RemoteName.clear();
      }

      WFC_VALIDATE_POINTER_NULL_OK( source->lpComment );

      if ( source->lpComment not_eq nullptr )
      {
         Comment.assign(source->lpComment);
      }
      else
      {
         Comment.clear();
      }

      WFC_VALIDATE_POINTER_NULL_OK( source->lpProvider );

      if ( source->lpProvider not_eq nullptr )
      {
         Provider.assign(source->lpProvider);
      }
      else
      {
         Provider.clear();
      }

      Scope       = source->dwScope;
      Type        = source->dwType;
      DisplayType = source->dwDisplayType;
      Usage       = source->dwUsage;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkResourceInformation::Copy( _In_ Win32FoundationClasses::CNetworkResourceInformation const& source ) noexcept
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

   LocalName.assign(source.LocalName);
   RemoteName.assign(source.RemoteName);
   Comment.assign(source.Comment);
   Provider.assign(source.Provider);
   Scope       = source.Scope;
   Type        = source.Type;
   DisplayType = source.DisplayType;
   Usage       = source.Usage;
}

void Win32FoundationClasses::CNetworkResourceInformation::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void Win32FoundationClasses::CNetworkResourceInformation::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   LocalName.clear();
   RemoteName.clear();
   Comment.clear();
   Provider.clear();
   Scope       = 0;
   Type        = 0;
   DisplayType = 0;
   Usage       = 0;
}

// End of source
