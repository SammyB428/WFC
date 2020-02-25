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
** $Workfile: modem.hpp $
** $Revision: 11 $
** $Modtime: 6/26/01 11:06a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( MODEM_CLASS_HEADER )

#define MODEM_CLASS_HEADER

class CModem : public CSerialFile
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

       bool m_IsConnected{ false };
       bool m_UsePulseDialing{ false };

      std::string m_PhoneNumber;

   public:

       CModem(CModem const&) = delete;
       CModem& operator=(CModem const&) = delete;

       CModem();

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CModem();

      virtual _Check_return_ bool Connect( _In_ DWORD number_of_seonds_to_wait = 30 ) noexcept;
      virtual _Check_return_ bool Disconnect( void ) noexcept; // Sends +++ATH
      virtual _Check_return_ bool IsConnected( void ) const noexcept;
      virtual void GetPhoneNumber( _Out_ std::string& phone_number_to_dial ) const noexcept;
      virtual void SetPhoneNumber( _In_ std::string const& phone_number_to_dial ) noexcept;
      
#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // MODEM_CLASS_HEADER
