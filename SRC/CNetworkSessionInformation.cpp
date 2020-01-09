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
** $Workfile: CNetworkSessionInformation.cpp $
** $Revision: 9 $
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

USING_WFC_NAMESPACE

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

CNetworkSessionInformation::CNetworkSessionInformation()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CNetworkSessionInformation::CNetworkSessionInformation( __in SESSION_INFO_502 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CNetworkSessionInformation::CNetworkSessionInformation( __in CNetworkSessionInformation const& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CNetworkSessionInformation::~CNetworkSessionInformation()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void CNetworkSessionInformation::Copy( __in SESSION_INFO_502 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      m_Initialize();
      return;
   }

   // We were passed a pointer, don't trust

   WFC_TRY
   {
      WFC_VALIDATE_POINTER( source->sesi502_cname );
      WFC_VALIDATE_POINTER( source->sesi502_username );
      WFC_VALIDATE_POINTER( source->sesi502_cltype_name );
      WFC_VALIDATE_POINTER( source->sesi502_transport );

      ClientName.assign( source->sesi502_cname );
      UserName.assign( source->sesi502_username );
      ClientTypeName.assign( source->sesi502_cltype_name );
      Transport.assign( source->sesi502_transport );

      NumberOfOpens  = source->sesi502_num_opens;
      Time           = CTimeSpan( source->sesi502_time      );
      IdleTime       = CTimeSpan( source->sesi502_idle_time );
      UserFlags      = source->sesi502_user_flags;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void CNetworkSessionInformation::Copy( __in CNetworkSessionInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this != &source );

   /*
   ** Make sure we ain't copying ourselves
   */

   if ( this == &source )
   {
      return;
   }

   ClientName.assign( source.ClientName );
   UserName.assign( source.UserName );
   ClientTypeName.assign(source.ClientTypeName);
   Transport.assign( source.Transport );
   NumberOfOpens  = source.NumberOfOpens;
   Time           = source.Time;
   IdleTime       = source.IdleTime;
   UserFlags      = source.UserFlags;
}

void CNetworkSessionInformation::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void CNetworkSessionInformation::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ClientName.clear();
   UserName.clear();
   ClientTypeName.clear();
   Transport.clear();
   NumberOfOpens = 0;
   Time          = CTimeSpan( 0 );
   IdleTime      = CTimeSpan( 0 );
   UserFlags     = 0;
}

// 2000-05-29
// Thanks go to Nathan Brown (nathanbrown@letu.edu) for noticing I had a missing = operator

CNetworkSessionInformation& CNetworkSessionInformation::operator=( __in CNetworkSessionInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
