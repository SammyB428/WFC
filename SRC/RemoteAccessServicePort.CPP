/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2015, Samuel R. Blackburn
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
** $Workfile: RemoteAccessServicePort.CPP $
** $Revision: 20 $
** $Modtime: 6/26/01 10:56a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#endif // _DEBUG

#if ! defined( WFC_NO_RAS )

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

// Construction

CRemoteAccessServicePort::CRemoteAccessServicePort()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

CRemoteAccessServicePort::CRemoteAccessServicePort( CRemoteAccessServicePort const& source )
{
   WFC_VALIDATE_POINTER( this );
   Empty();
   Copy( source );
}

CRemoteAccessServicePort::CRemoteAccessServicePort( RAS_PORT_0 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CRemoteAccessServicePort::CRemoteAccessServicePort( RAS_PORT_1 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

CRemoteAccessServicePort::~CRemoteAccessServicePort()
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

// Methods

void CRemoteAccessServicePort::Copy( RAS_PORT_0 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   // Choose to live
   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      ::wfc_convert_lpcwstr_to_cstring( source->wszPortName,    Name       );
      ::wfc_convert_lpcwstr_to_cstring( source->wszDeviceType,  DeviceType );
      ::wfc_convert_lpcwstr_to_cstring( source->wszDeviceName,  DeviceName );
      ::wfc_convert_lpcwstr_to_cstring( source->wszMediaName,   MediaName  );
      Flags   = source->Flags;
      ::wfc_convert_lpcwstr_to_cstring( source->wszUserName,    UserName        );
      ::wfc_convert_lpcwstr_to_cstring( source->wszComputer,    ComputerName    );
      SessionStartTime = CTime( source->dwStartSessionTime );
      ::wfc_convert_lpcwstr_to_cstring( source->wszLogonDomain, LogonDomainName );
      IsAdvancedServer = source->fAdvancedServer;

      // Now set the rest of the field to zero
      LineCondition            = 0;
      HardwareCondition        = 0;
      LineSpeedInBitsPerSecond = 0;
      NumberOfStatistics       = 0;
      NumberOfMediaParameters  = 0;
      SizeOfMediaParameters    = 0;

      /*
      ** WARNING!!!!
      ** Someone at Microsoft let the children play in the header files again.
      ** In version 4.2 of Visual C++, the following RAS_PORT_0 structure member
      ** was called "MediaID", in the 4.2b patch, it was renamed to "reserved"
      ** There was no mention of it anywhere. I didn't find out about it until
      ** Patrick Clarke (pclarke@trintech.com) reported the problem. My fix is
      ** a real hack. I detest hacks! If anyone has a non-hacking way of dealing
      ** with data structures (not unions) containing random field names, please
      ** let me know.
      */

      SIZE_T temp_dword{ (((DWORD)source) + offsetof(RAS_PORT_0, Flags)) - sizeof(DWORD) };

      auto address_of_randomly_named_field{ (DWORD*)temp_dword };
      
      MediaID = *address_of_randomly_named_field;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void CRemoteAccessServicePort::Copy( _In_ RAS_PORT_1 const * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   // Choose to live
   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      Copy( &source->rasport0 );

      // Now copy the non-RAS_PORT_0 stuff

      LineCondition            = source->LineCondition;
      HardwareCondition        = source->HardwareCondition;
      LineSpeedInBitsPerSecond = source->LineSpeed;
      NumberOfStatistics       = source->NumStatistics;
      NumberOfMediaParameters  = source->NumMediaParms;
      SizeOfMediaParameters    = source->SizeMediaParms;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void CRemoteAccessServicePort::Copy( _In_ CRemoteAccessServicePort const& source )
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   Name                     = source.Name;
   DeviceType               = source.DeviceType;
   DeviceName               = source.DeviceName;
   MediaName                = source.MediaName;
   MediaID                  = source.MediaID;
   Flags                    = source.Flags;
   UserName                 = source.UserName;
   ComputerName             = source.ComputerName;
   SessionStartTime         = source.SessionStartTime;
   LogonDomainName          = source.LogonDomainName;
   IsAdvancedServer         = source.IsAdvancedServer;
   LineCondition            = source.LineCondition;
   HardwareCondition        = source.HardwareCondition;
   LineSpeedInBitsPerSecond = source.LineSpeedInBitsPerSecond;
   NumberOfStatistics       = source.NumberOfStatistics;
   NumberOfMediaParameters  = source.NumberOfMediaParameters;
   SizeOfMediaParameters    = source.SizeOfMediaParameters;
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CRemoteAccessServicePort::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CRemoteAccessServicePort at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   Name is                     " ) << Name                     << TEXT( "\n" );
   dump_context << TEXT( "   DeviceType is               " ) << DeviceType               << TEXT( "\n" );
   dump_context << TEXT( "   DeviceName is               " ) << DeviceName               << TEXT( "\n" );
   dump_context << TEXT( "   MediaName is                " ) << MediaName                << TEXT( "\n" );
   dump_context << TEXT( "   MediaID is                  " ) << MediaID;

   switch( MediaID )
   {
      case mediaSerial:

         dump_context << TEXT( " (Serial)\n" );
         break;

      case mediaUnknown:

         dump_context << TEXT( " (Unknown)\n" );
         break;

      case mediaRAS10Serial:

         dump_context << TEXT( " (RAS10 Serial)\n" );
         break;

      case mediaX25:

         dump_context << TEXT( " (X.25)\n" );
         break;

      case mediaISDN:

         dump_context << TEXT( " (ISDN)\n" );
         break;

      default:

         dump_context << TEXT( " (really unknown)\n" );
         break;
   }

   dump_context << TEXT( "   Flags is                    " ) << Flags;

   std::wstring flags;

   if ( Flags bitand USER_AUTHENTICATED )
   {
      flags = TEXT( "USER_AUTHENTICATED" );
   }

   if ( Flags bitand MESSENGER_PRESENT )
   {
      if ( flags.GetLength() > 0 )
      {
         flags += TEXT( " | " );
      }

      flags += TEXT( "MESSENGER_PRESENT" );
   }

   if ( Flags bitand PPP_CLIENT )
   {
      if ( flags.GetLength() > 0 )
      {
         flags += TEXT( " | " );
      }

      flags += TEXT( "PPP_CLIENT" );
   }

   if ( Flags bitand GATEWAY_ACTIVE )
   {
      if ( flags.GetLength() > 0 )
      {
         flags += TEXT( " | " );
      }

      flags += TEXT( "GATEWAY_ACTIVE" );
   }

   if ( Flags bitand REMOTE_LISTEN )
   {
      if ( flags.GetLength() > 0 )
      {
         flags += TEXT( " | " );
      }

      flags += TEXT( "REMOTE_LISTEN" );
   }

   if ( Flags bitand PORT_MULTILINKED )
   {
      if ( flags.GetLength() > 0 )
      {
         flags += TEXT( " | " );
      }

      flags += TEXT( "PORT_MULTILINKED" );
   }

   dump_context << TEXT( "(" ) << flags << TEXT( ")\n" );

   dump_context << TEXT( "   UserName is                 " ) << UserName                 << TEXT( "\n" );
   dump_context << TEXT( "   ComputerName is             " ) << ComputerName             << TEXT( "\n" );
   dump_context << TEXT( "   SessionStartTime is         " ) << SessionStartTime         << TEXT( "\n" );
   dump_context << TEXT( "   LogonDomainName is          " ) << LogonDomainName          << TEXT( "\n" );
   
   if ( IsAdvancedServer not_eq FALSE )
   {
      dump_context << TEXT( "   IsAdvancedServer is         TRUE\n" );
   }
   else
   {
      dump_context << TEXT( "   IsAdvancedServer is         FALSE\n" );
   }

   dump_context << TEXT( "   LineCondition is            " ) << LineCondition            << TEXT( "\n" );
   dump_context << TEXT( "   HardwareCondition is        " ) << HardwareCondition        << TEXT( "\n" );
   dump_context << TEXT( "   LineSpeedInBitsPerSecond is " ) << LineSpeedInBitsPerSecond << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfStatistics is       " ) << NumberOfStatistics       << TEXT( "\n" );
   dump_context << TEXT( "   NumberOfMediaParameters is  " ) << NumberOfMediaParameters  << TEXT( "\n" );
   dump_context << TEXT( "   SizeOfMediaParameters is    " ) << SizeOfMediaParameters    << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void CRemoteAccessServicePort::Empty( void )
{
   WFC_VALIDATE_POINTER( this );

   Name.Empty();
   DeviceType.Empty();
   DeviceName.Empty();
   MediaName.Empty();
   MediaID                  = 0;
   Flags                    = 0;
   UserName.Empty();
   ComputerName.Empty();
   //SessionStartTime.Empty();
   LogonDomainName.Empty();
   IsAdvancedServer         = FALSE;
   LineCondition            = 0;
   HardwareCondition        = 0;
   LineSpeedInBitsPerSecond = 0;
   NumberOfStatistics       = 0;
   NumberOfMediaParameters  = 0;
   SizeOfMediaParameters    = 0;
}

// Operators

CRemoteAccessServicePort& CRemoteAccessServicePort::operator=( _In_ CRemoteAccessServicePort const& source )
{
   Copy( source );
   WFC_VALIDATE_POINTER( this );
   return( *this );
}

#endif // WFC_NO_RAS

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CRemoteAccessServicePort</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, RAS, Remote Access Service, Dialup networking">
</HEAD>

<BODY>

<H1>CRemoteAccessServicePort : CObject</H1>
$Revision: 20 $
<HR>

<H2>Description</H2>

This class is based on the RAS_USER_0 structure.

<H2>Data Members</H2>

<B>None.</B>

<H2>Methods</H2>

<DL COMPACT>

<DT><B><A NAME="Copy">Copy</A></B><DD>Copies another CRemoteAccessServicePort or a RAS_USER_0 structure.
<DT><B><A NAME="Empty">Empty</A></B><DD>Clears all data members.
<DT><B><A NAME="Serialize">Serialize</A></B><DD>After all, we are a CObject . . .

</DL>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: RemoteAccessServicePort.CPP $<BR>
$Modtime: 6/26/01 10:56a $
</BODY>

</HTML>
*/
