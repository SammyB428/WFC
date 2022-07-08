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
** $Workfile: modem.cpp $
** $Revision: 20 $
** $Modtime: 6/26/01 10:53a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#endif // _DEBUG

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CModem::CModem()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CModem::~CModem()
{
   WFC_VALIDATE_POINTER( this );

   if ( IsConnected() == true )
   {
       std::ignore = Disconnect();
   }

   m_Initialize();
}

_Check_return_ bool Win32FoundationClasses::CModem::Connect( _In_ DWORD number_of_seconds_to_wait ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( m_IsOpen == true );

   if ( m_IsOpen == false )
   {
      m_LastError = ERROR_INVALID_HANDLE;
      return( false );
   }

   std::string string_to_send_to_modem( "ATM0\r\n" );

   Write( string_to_send_to_modem );

   std::this_thread::sleep_for(std::chrono::milliseconds(500));

   string_to_send_to_modem.assign(STRING_VIEW("ATDT"));
   string_to_send_to_modem.append( m_PhoneNumber );
   string_to_send_to_modem.append(STRING_VIEW("\r\n"));

   Write( string_to_send_to_modem );

   std::string what_was_read( "");

   if ( WaitForString( "CONNECT", number_of_seconds_to_wait, &what_was_read ) == false )
   {
       std::ignore = Disconnect();
      return( false );
   }

   m_LastError = ERROR_SUCCESS;

   // 1999-07-22
   // Thanks go to Don Kennedy (dkennedy@hightower.com) for finding
   // a bug here, I was not setting m_IsConnected

   m_IsConnected = true;

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CModem::Disconnect( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( m_IsOpen == true );

   if ( m_IsOpen == true )
   {
       std::string string_to_send( "+++ATH\r\n" );

      Write( string_to_send );
      std::ignore = WaitForString( "OK" );
   }

   m_IsConnected = false;

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CModem::IsConnected( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_IsConnected );
}

void Win32FoundationClasses::CModem::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_IsConnected     = FALSE;
   m_UsePulseDialing = FALSE;
   m_PhoneNumber.clear();
}

void Win32FoundationClasses::CModem::GetPhoneNumber( _Out_ std::string& phone_number ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   phone_number.assign( m_PhoneNumber );
}

void Win32FoundationClasses::CModem::SetPhoneNumber( _In_ std::string const& phone_number ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_PhoneNumber.assign( phone_number );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CModem::Dump( CDumpContext& dump_context ) const
{
   CSerialFile::Dump( dump_context );

   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   m_IsConnected     = "  ) << m_IsConnected           << TEXT( "\n" );
   dump_context << TEXT( "   m_UsePulseDialing = "  ) << m_UsePulseDialing       << TEXT( "\n" );
   dump_context << TEXT( "   m_PhoneNumber is   \"" ) << (LPCTSTR) m_PhoneNumber << TEXT( "\"\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

// End of source
