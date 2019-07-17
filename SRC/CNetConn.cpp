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
** $Workfile: cnetconn.cpp $
** $Revision: 27 $
** $Modtime: 6/26/01 10:48a $
** $Reuse Tracing Code: 1 $
*/

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

CNetworkConnections::CNetworkConnections()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CNetworkConnections::CNetworkConnections(__in_z_opt LPCTSTR machine_name )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( machine_name );
   m_Initialize();
   Open( machine_name );
}

CNetworkConnections::~CNetworkConnections()
{
   WFC_VALIDATE_POINTER( this );
   Close();
   m_Initialize();
}

void CNetworkConnections::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   CNetwork::Close();

   if ( m_1InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_1InformationBuffer );
      m_1InformationBuffer = nullptr;
   }
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CNetworkConnections::Dump( CDumpContext& dump_context ) const
{
   CNetwork::Dump( dump_context );

   dump_context << TEXT( "m_1ResumeHandle = "         ) << m_1ResumeHandle         << TEXT( "\n" );
   dump_context << TEXT( "m_1CurrentEntryNumber = "   ) << m_1CurrentEntryNumber   << TEXT( "\n" );
   dump_context << TEXT( "m_1NumberOfEntriesRead = "  ) << m_1NumberOfEntriesRead  << TEXT( "\n" );
   dump_context << TEXT( "m_1TotalNumberOfEntries = " ) << m_1TotalNumberOfEntries << TEXT( "\n" );
}

#endif // _DEBUG

_Check_return_ bool CNetworkConnections::Enumerate(__in_z_opt LPCTSTR share_or_computer_name ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( share_or_computer_name );

   if ( m_1InformationBuffer != nullptr )
   {
      ::NetApiBufferFree( m_1InformationBuffer );
      m_1InformationBuffer = nullptr;
   }

   if ( share_or_computer_name == nullptr )
   {
      m_ErrorCode = ERROR_INVALID_PARAMETER;
      return( false );
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
#if ! defined( UNICODE )
      ::ASCII_to_UNICODE( share_or_computer_name, (LPWSTR) m_WideShareOrComputerName );
#else
      _tcscpy_s( m_WideShareOrComputerName, std::size( m_WideShareOrComputerName ), share_or_computer_name );
#endif // UNICODE

      m_1Index                = 0;
      m_1CurrentEntryNumber   = 0;
      m_1NumberOfEntriesRead  = 0;
      m_1ResumeHandle         = 0;
      m_1TotalNumberOfEntries = 0;

      return( m_GetChunk() );
   }
   WFC_CATCH_ALL
   {
      m_ErrorCode = ERROR_EXCEPTION_IN_SERVICE;
      return( false );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ bool CNetworkConnections::GetNext( __out CNetworkConnectionInformation& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   information.Empty();

   if ( m_1InformationBuffer != nullptr )
   {
      if ( m_1Index < m_1NumberOfEntriesRead )
      {
         information.Copy( &m_1InformationBuffer[ m_1Index ] );
         m_1Index++;

         return( true );
      }
      else
      {
         // We need another chunk

         if ( m_GetChunk() != false )
         {
            return( GetNext( information ) );
         }
         else
         {
            return( false );
         }
      }
   }

   information.Empty();
   return( false );
}

_Check_return_ bool CNetworkConnections::m_GetChunk( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_1Index = 0;

   if ( m_1NumberOfEntriesRead != 0 && m_1ResumeHandle == 0 )
   {
      // Yup, we've finished reading
      ::NetApiBufferFree( m_1InformationBuffer );
      m_1InformationBuffer        = nullptr;
      m_1CurrentEntryNumber       = 0;
      m_1NumberOfEntriesRead      = 0;
      m_1ResumeHandle             = 0;
      m_1TotalNumberOfEntries     = 0;
      return( false );
   }

   m_ErrorCode = ::NetConnectionEnum( (LMSTR) m_WideMachineName.get(),
                                      (LMSTR) m_WideShareOrComputerName,
                                              1, 
                                  (LPBYTE *) &m_1InformationBuffer,
                                              64 * 1024,
                                             &m_1NumberOfEntriesRead,
                                             &m_1TotalNumberOfEntries,
                                             &m_1ResumeHandle );

   if ( m_ErrorCode == NERR_Success || m_ErrorCode == ERROR_MORE_DATA )
   {
      return( true );
   }

   return( false );
}

void CNetworkConnections::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_ErrorCode             = 0;
   m_1InformationBuffer    = nullptr;
   m_1ResumeHandle         = 0;
   m_1CurrentEntryNumber   = 0;
   m_1NumberOfEntriesRead  = 0;
   m_1TotalNumberOfEntries = 0;
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CNetworkConnections</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles network connections.">
</HEAD>

<BODY>

<H1>CNetworkConnections : <A HREF="CNetwork.htm">CNetwork</A></H1>
$Revision: 27 $<BR>
<HR>

<H2>Description</H2>

This class allows you to enumerate the network connections.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CNetworkConnections</B>()
<B>CNetworkConnections</B>( LPCTSTR machine_name )</PRE><DD>
Constructs the object.

</DL>

<H2>Data Members</H2>
None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the connection to the machine.

<DT><PRE>BOOL <B><A NAME="Enumerate">Enumerate</A></B>( LPCTSTR share_or_computer_name )</PRE><DD>
Initializes the object for enumeration.

<DT><PRE>BOOL <B><A NAME="GetNext">GetNext</A></B>( CNetworkConnectionInformation&amp; information )</PRE><DD>
Retrieves the next set of connection information.

<DT><PRE>void <B><A NAME="Serialize">Serialize</A></B>( CArchive&amp; archive )</PRE><DD>
After all we are a CObject...

</DL>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<H2>API's Used</H2>

<B>CNetworkConnections</B> uses the following API's:

<UL>
<LI>NetApiBufferFree
<LI>NetConnectionEnum
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: cnetconn.cpp $<BR>
$Modtime: 6/26/01 10:48a $
</BODY>

</HTML>
*/
