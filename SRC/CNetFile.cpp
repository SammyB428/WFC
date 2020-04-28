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
** $Workfile: cnetfile.cpp $
** $Revision: 26 $
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

Win32FoundationClasses::CNetworkFiles::CNetworkFiles(_In_ std::wstring_view machine_name) : CNetwork(machine_name)
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CNetworkFiles::~CNetworkFiles()
{
   WFC_VALIDATE_POINTER( this );
   Close();
   m_Initialize();
}

void Win32FoundationClasses::CNetworkFiles::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   CNetwork::Close();

   if ( m_103InformationBuffer not_eq nullptr )
   {
      ::NetApiBufferFree( m_103InformationBuffer );
      m_103InformationBuffer = nullptr;
   }
}

bool Win32FoundationClasses::CNetworkFiles::Close( __inout Win32FoundationClasses::CNetworkFileInformation& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_ErrorCode = ::NetFileClose( (LMSTR) m_WideMachineName.get(), information.ID );

   if ( m_ErrorCode == NERR_Success )
   {
      return( true );
   }
   else
   {
      return( false );
   }
}

void Win32FoundationClasses::CNetworkFiles::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_ErrorCode               = 0;
   m_103Index                = 0;
   m_103InformationBuffer    = nullptr;
   m_103ResumeHandle         = 0;
   m_103CurrentEntryNumber   = 0;
   m_103NumberOfEntriesRead  = 0;
   m_103TotalNumberOfEntries = 0;
}

_Check_return_ bool Win32FoundationClasses::CNetworkFiles::Enumerate( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_103InformationBuffer not_eq nullptr )
   {
      ::NetApiBufferFree( m_103InformationBuffer );
      m_103InformationBuffer = nullptr;
   }

   m_103Index                = 0;
   m_103CurrentEntryNumber   = 0;
   m_103NumberOfEntriesRead  = 0;
   m_103ResumeHandle         = 0;
   m_103TotalNumberOfEntries = 0;

   return( m_GetChunk() );
}

_Check_return_ bool Win32FoundationClasses::CNetworkFiles::GetNext( __inout Win32FoundationClasses::CNetworkFileInformation& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_103InformationBuffer not_eq nullptr )
   {
      if ( m_103Index < m_103NumberOfEntriesRead )
      {
         information.Copy( &m_103InformationBuffer[ m_103Index ] );
         m_103Index++;

         return( true );
      }
      else
      {
         // We need another chunk

         if ( m_GetChunk() == true)
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

bool Win32FoundationClasses::CNetworkFiles::m_GetChunk( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_103Index = 0;

   if ( m_103NumberOfEntriesRead not_eq 0 and m_103ResumeHandle == 0 )
   {
      // Yup, we've finished reading
      ::NetApiBufferFree( m_103InformationBuffer );
      m_103InformationBuffer    = nullptr;
      m_103CurrentEntryNumber   = 0;
      m_103NumberOfEntriesRead  = 0;
      m_103ResumeHandle         = 0;
      m_103TotalNumberOfEntries = 0;
      return( false );
   }

   m_ErrorCode = ::NetFileEnum( (LMSTR) m_WideMachineName.get(),
                                        nullptr,
                                        nullptr,
                                        3,
                            (LPBYTE *) &m_103InformationBuffer,
                                        64 * 1024,
                                       &m_103NumberOfEntriesRead,
                                       &m_103TotalNumberOfEntries,
                                       &m_103ResumeHandle );

   if ( m_ErrorCode not_eq NERR_Success or m_103InformationBuffer == nullptr )
   {
      return( false );
   }

   return( true );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CNetworkFiles</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles network files.">
</HEAD>

<BODY>

<H1>CNetworkFiles : <A HREF="CNetwork.htm">CNetwork</A></H1>
$Revision: 26 $<BR>
<HR>

<H2>Description</H2>
This class allows you to NetFilexxx API in Win32.

<H2>Data Members</H2>
None.

<H2>Methods</H2>

<DL COMPACT>

<DT><DFN><B><A NAME="Close">Close</A></B></DFN><DD>Closes the connection to the machine. If you pass
a CNetworkFileInformation object to Close, it will close that opened file.

<DT><DFN><B><A NAME="Enumerate">Enumerate</A></B></DFN><DD>Initializes the object for enumeration.

<DT><DFN><B><A NAME="GetNext">GetNext</A></B></DFN><DD>Retrieves the next CNetworkFileInformation.
When you reach the end of the list, GetNext() will return FALSE.

<DT><DFN><B><A NAME="Serialize">Serialize</A></B></DFN><DD>After all we are a CObject...

</DL>

<H2>Example</H2>
<PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: cnetfile.cpp $<BR>
$Modtime: 6/26/01 10:49a $
</BODY>

</HTML>
*/
