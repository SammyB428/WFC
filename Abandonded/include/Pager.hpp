/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2003, Samuel R. Blackburn
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
** $Workfile: Pager.hpp $
** $Revision: 13 $
** $Modtime: 6/26/01 11:06a $
*/

#if ! defined( PAGER_CLASS_HEADER )

#define PAGER_CLASS_HEADER

class CPager
#if ! defined(  WFC_NO_SERIALIZATION )
: public CObject
#endif // WFC_NO_SERIALIZATION
{
   protected:

      CString m_Account;
      CString m_PhoneNumber;
      CString m_Message;
      CString m_SerialSettings;

      DWORD m_ErrorCode;
      DWORD m_NumberOfRetries;

      virtual BOOL m_Send( void ) = 0;

   public:

      CPager();
      CPager( const CPager& source );
      CPager( const CPager * source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CPager();

      virtual void  Copy( const CPager& source ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  Copy( const CPager * source ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  Empty( void ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  GetAccount( CString& account ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual DWORD GetErrorCode( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  GetMessage( CString& message ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual DWORD GetNumberOfRetries( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  GetPhoneNumber( CString& phone_number ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  GetSerialPort( CString& serial_settings ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual BOOL  Send( void ) WFC_DOESNT_THROW_EXCEPTIONS;
#if ! defined(  WFC_NO_SERIALIZATION )
      virtual void  Serialize( CArchive& archive );
#endif // WFC_NO_SERIALIZATION
      virtual void  SetAccount( const CString& account ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetMessage( const CString& message ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetNumberOfRetries( const DWORD retries ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetPhoneNumber( const CString& phone_number ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetSerialPort( const CString& serial_settings ) WFC_DOESNT_THROW_EXCEPTIONS;

      // Operators

      virtual CPager& operator = ( const CPager& source ) WFC_DOESNT_THROW_EXCEPTIONS;
      
#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

#endif // PAGER_CLASS_HEADER
