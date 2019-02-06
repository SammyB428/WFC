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
** $Workfile: Pager.cpp $
** $Revision: 27 $
** $Modtime: 6/26/01 10:55a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

CPager::CPager()
{
   WFCLTRACEINIT( TEXT( "CPager::CPager()" ) );
   WFCTRACEVAL( TEXT( "pointer is " ), (VOID *) this );
   WFC_VALIDATE_POINTER( this );
   Empty();
   SetNumberOfRetries( 3 );
}

CPager::CPager( const CPager& source )
{
   WFCLTRACEINIT( TEXT( "CPager::CPager( CPager * )" ) );
   WFCTRACEVAL( TEXT( "pointer is " ), (VOID *) this );
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CPager::CPager( const CPager * source )
{
   WFCLTRACEINIT( TEXT( "CPager::CPager( CPager * )" ) );
   WFCTRACEVAL( TEXT( "pointer is " ), (VOID *) this );
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CPager::~CPager()
{
   WFCLTRACEINIT( TEXT( "CPager::~CPager()" ) );
   WFCTRACEVAL( TEXT( "pointer is " ), (VOID *) this );
   WFC_VALIDATE_POINTER( this );
   Empty();
}

void CPager::Copy( const CPager& source )
{
   WFCLTRACEINIT( TEXT( "CPager::Copy( CPager & )" ) );
   WFC_VALIDATE_POINTER( this );

   m_Account         = source.m_Account;
   m_ErrorCode       = source.m_ErrorCode;
   m_PhoneNumber     = source.m_PhoneNumber;
   m_Message         = source.m_Message;
   m_SerialSettings  = source.m_SerialSettings;
   m_NumberOfRetries = source.m_NumberOfRetries;
}

void CPager::Copy( const CPager * source )
{
   WFCLTRACEINIT( TEXT( "CPager::Copy( CPager * )" ) );
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   ASSERT( source != this );

   if ( source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( source != NULL )
      {
         Copy( *source );
      }
      else
      {
         Empty();
      }
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void CPager::Empty( void )
{
   WFCLTRACEINIT( TEXT( "CPager::Empty()" ) );
   WFC_VALIDATE_POINTER( this );

   m_Account.Empty();
   m_ErrorCode = 0;
   m_PhoneNumber.Empty();
   m_Message.Empty();
   m_SerialSettings.Empty();
   m_NumberOfRetries = 1;
}

void CPager::GetAccount( CString& account ) const
{
   WFCLTRACEINIT( TEXT( "CPager::GetAccount()" ) );
   WFC_VALIDATE_POINTER( this );
   account = m_Account;
}

DWORD CPager::GetErrorCode( void ) const
{
   WFCLTRACEINIT( TEXT( "CPager::GetErrorCode()" ) );
   WFC_VALIDATE_POINTER( this );
   return( m_ErrorCode );
}

void CPager::GetMessage( CString& message ) const
{
   WFCLTRACEINIT( TEXT( "CPager::GetMessage()" ) );
   WFC_VALIDATE_POINTER( this );
   message = m_Message;
}

DWORD CPager::GetNumberOfRetries( void ) const
{
   WFCLTRACEINIT( TEXT( "CPager::GetNumberOfRetries()" ) );
   WFC_VALIDATE_POINTER( this );
   return( m_NumberOfRetries );
}

void CPager::GetPhoneNumber( CString& phone_number ) const
{
   WFCLTRACEINIT( TEXT( "CPager::GetPhoneNumber()" ) );
   WFC_VALIDATE_POINTER( this );
   phone_number = m_PhoneNumber;
}

void CPager::GetSerialPort( CString& serial_settings ) const
{
   WFCLTRACEINIT( TEXT( "CPager::GetSerialPort()" ) );
   WFC_VALIDATE_POINTER( this );
   serial_settings = m_SerialSettings;
}

BOOL CPager::Send( void )
{
   WFCLTRACEINIT( TEXT( "CPager::Send()" ) );
   WFC_VALIDATE_POINTER( this );

   DWORD tries = 0;

   BOOL return_value = FALSE;

   // Number of Retries < 1 ain't valid, we will always try at least once

   DWORD number_of_tries = m_NumberOfRetries;

   if ( number_of_tries < 1 )
   {
      WFCTRACE( TEXT( "m_NumberOfRetries < 1 so setting tries to 1." ) );
      number_of_tries = 1;
   }

   WFCTRACEVAL( TEXT( "number of tries is " ), number_of_tries );

   while( tries < number_of_tries )
   {
      WFCTRACE( TEXT( "Calling m_Send()" ) );
      return_value = m_Send();

      if ( return_value != FALSE )
      {
         WFCTRACE( TEXT( "m_Send() returned TRUE." ) );
         return( return_value );
      }
      else
      {
         WFCTRACE( TEXT( "m_Send() returned FALSE, retrying" ) );
         tries++;
      }
   }

   return( return_value );
}

#if ! defined(  WFC_NO_SERIALIZATION )

void CPager::Serialize( CArchive& archive )
{
   WFCLTRACEINIT( TEXT( "CPager::Serialize()" ) );
   CObject::Serialize( archive );

   if ( archive.IsStoring() )
   {
      WFCTRACE( TEXT( "Storing" ) );
      archive << m_Account;
      archive << m_PhoneNumber;
      archive << m_Message;
      archive << m_SerialSettings;
      archive << m_ErrorCode;
      archive << m_NumberOfRetries;
   }
   else
   {
      WFCTRACE( TEXT( "Restoring" ) );
      archive >> m_Account;
      archive >> m_PhoneNumber;
      archive >> m_Message;
      archive >> m_SerialSettings;
      archive >> m_ErrorCode;
      archive >> m_NumberOfRetries;
   }
}

#endif // WFC_NO_SERIALIZATION

void CPager::SetAccount( const CString& account )
{
   WFCLTRACEINIT( TEXT( "CPager::SetAccount()" ) );
   WFC_VALIDATE_POINTER( this );
   m_Account = account;
}

void CPager::SetMessage( const CString& message )
{
   WFCLTRACEINIT( TEXT( "CPager::SetMessage()" ) );
   WFC_VALIDATE_POINTER( this );
   m_Message = message;
}

void CPager::SetNumberOfRetries( const DWORD retries )
{
   WFCLTRACEINIT( TEXT( "CPager::SetNumberOfRetries()" ) );
   WFC_VALIDATE_POINTER( this );
   m_NumberOfRetries = retries;
}

void CPager::SetPhoneNumber( const CString& phone_number )
{
   WFCLTRACEINIT( TEXT( "CPager::SetPhoneNumber()" ) );
   WFC_VALIDATE_POINTER( this );
   m_PhoneNumber = phone_number;
}

void CPager::SetSerialPort( const CString& serial_settings )
{
   WFCLTRACEINIT( TEXT( "CPager::SetSerialPort()" ) );
   WFC_VALIDATE_POINTER( this );
   m_SerialSettings = serial_settings;
}

CPager& CPager::operator = ( const CPager& source )
{
   WFCLTRACEINIT( TEXT( "CPager::operator = ( const CPager& )" ) );
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CPager::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );

   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   m_Account is         \"" ) << (LPCTSTR) m_Account        << TEXT( "\"\n" );
   dump_context << TEXT( "   m_PhoneNumber is     \"" ) << (LPCTSTR) m_PhoneNumber    << TEXT( "\"\n" );
   dump_context << TEXT( "   m_Message is         \"" ) << (LPCTSTR) m_Message        << TEXT( "\"\n" );
   dump_context << TEXT( "   m_SerialSettings is  \"" ) << (LPCTSTR) m_SerialSettings << TEXT( "\"\n" );
   dump_context << TEXT( "   m_NumberOfRetries is  "  ) << m_NumberOfRetries          << TEXT( "\n" );
   dump_context << TEXT( "   m_ErrorCode is        "  ) << m_ErrorCode                << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CPager</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ base class for sending messages to pagers.">
</HEAD>

<BODY>

<H1>CPager : CObject</H1>
$Revision: 27 $
<HR>

<H2>Description</H2>

This class is a virtual base class that describes the methods of dealing with pagers.

<H2>Data Members</H2>

None.

<H2>Methods</H2>

<DL COMPACT>

<DT><DFN><B><A NAME="Copy">Copy</A></B></DFN><DD>Copies another CPager.

<DT><DFN><B><A NAME="Empty">Empty</A></B></DFN><DD>Clears all internal data members.

<DT><DFN><B><A NAME="GetAccount">GetAccount</A></B></DFN><DD>Retrieves the account string.

<DT><DFN><B><A NAME="GetErrorCode">GetErrorCode</A></B></DFN><DD>Retrieves the error code for the
last function that failed.

<DT><DFN><B><A NAME="GetMessage">GetMessage</A></B></DFN><DD>Retrieves the message that was/may be 
sent to the pager.

<DT><DFN><B><A NAME="GetNumberOfRetries">GetNumberOfRetries</A></B></DFN><DD>Retrieves the number of times
the class will attempt to resend the message if it fails.

<DT><DFN><B><A NAME="GetPhoneNumber">GetPhoneNumber</A></B></DFN><DD>Retrieves the phone number of the
pager company's computer.

<DT><DFN><B><A NAME="GetSerialPort">GetSerialPort</A></B></DFN><DD>Retrieves the name of the serial port in 
the &quot;COM1:9600,n,8,1&quot; format.

<DT><DFN><B><A NAME="Send">Send</A></B></DFN><DD>Method to call after all data members have been set.
This will actually dial the phone, log onto the service, send the message and
log off.

<DT><DFN><B><A NAME="Serialize">Serialize</A></B></DFN><DD>Saves/restores the object.

<DT><DFN><B><A NAME="SetAccount">SetAccount</A></B></DFN><DD>Stores the account number to send a message to.

<DT><DFN><B><A NAME="SetMessage">SetMessage</A></B></DFN><DD>Stores the message to be sent.

<DT><DFN><B><A NAME="SetNumberOfRetries">SetNumberOfRetries</A></B></DFN><DD>Sets the number of times Send() is to
try to get the message through before giving up.

<DT><DFN><B><A NAME="SetPhoneNumber">SetPhoneNumber</A></B></DFN><DD>Sets the phone number for the pager company.

<DT><DFN><B><A NAME="SetSerialPort">SetSerialPort</A></B></DFN><DD>Sets the serial port to use to contact the 
pager company.

</DL>

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

void wait_for_ring( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;wait_for_ring()&quot; ) );

   <A HREF="Serial.htm">CSerialFile</A> serial;

   while( 1 )
   {
      _tprintf( TEXT( &quot;Opening serial port\n&quot; ) );

      if ( serial.Open( TEXT( &quot;COM1:57600,n,8,1&quot; ) ) != FALSE )
      {
         CString response;

         response = TEXT( &quot;ATM0\r\n&quot; );

         serial.Write( response );

         // See if the phone rang

         while ( serial.WaitForString( &quot;RING&quot;, 2, &amp;response ) == FALSE )
         {
            if ( response.GetLength() &gt; 0 )
            {
               _tprintf( TEXT( &quot;Didn't get RING, going to sleep \&quot;%s\&quot;\n&quot; ), (LPCTSTR) response );
            }

            ::Sleep( 10000 );
         }

         _tprintf( TEXT( &quot;Received a RING, waiting for them to end\n&quot; ) );

         while( serial.WaitForString( TEXT( &quot;RING&quot; ), 7 ) != FALSE )
         {
            // Do Nothing
         }

         _tprintf( TEXT( &quot;Closing serial port\n&quot; ) );

         serial.Close();

         CTime time_now( CTime::GetCurrentTime() );

         ::Sleep( 1000 );

         CString message;

         message.Format( TEXT( &quot;The phone rang at %s&quot; ), (LPCTSTR) time_now.Format( &quot;%H:%M:%S %d %b %y&quot; ) );

         CSprintSpectrum pager;

         pager.SetAccount( TEXT( &quot;4101234567&quot; ) );
         pager.SetMessage( message );

         _tprintf( TEXT( &quot;Calling pager.Send()\n&quot; ) );
         pager.Send();
      }
      else
      {
         _tprintf( TEXT( &quot;Can't open serial port\n&quot; ) );
      }

      ::Sleep( 1000 );
   }
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: Pager.cpp $<BR>
$Modtime: 6/26/01 10:55a $
</BODY>

</HTML>
*/
