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
** $Workfile: CRASPORT.HPP $
** $Revision: 12 $
** $Modtime: 6/26/01 11:04a $
*/

#if ! defined( REMOTE_ACCESS_SERVICE_PORT_DATA_CLASS_HEADER )

#define REMOTE_ACCESS_SERVICE_PORT_DATA_CLASS_HEADER

class CRemoteAccessServiceConnectionProjection
{
   public:

      // Construction

      CRemoteAccessServiceConnectionProjection();
      CRemoteAccessServiceConnectionProjection( __in CRemoteAccessServiceConnectionProjection const& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRemoteAccessServiceConnectionProjection();

      // Properties

      DWORD ErrorCode{ 0 };

      // Methods

      virtual void Copy( __in CRemoteAccessServiceConnectionProjection const& source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CRemoteAccessServiceConnectionProjection& operator = ( __in CRemoteAccessServiceConnectionProjection const& source ) noexcept;
};

class CNetBEUIFramerProjectionResult : public CRemoteAccessServiceConnectionProjection
{
   public:

      CNetBEUIFramerProjectionResult();
      CNetBEUIFramerProjectionResult( __in RAS_PPP_NBFCP_RESULT const * source );
      CNetBEUIFramerProjectionResult( __in CNetBEUIFramerProjectionResult const& source );
      CNetBEUIFramerProjectionResult( __in CRemoteAccessServiceConnectionProjection const& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CNetBEUIFramerProjectionResult();

      // Properties

      DWORD NetBiosError{ 0 };
      std::wstring Name;
      std::wstring WorkstationName;

      // Methods

      virtual void Copy( __in CRemoteAccessServiceConnectionProjection const& source ) noexcept; // Parent's method
      virtual void Copy( __in CNetBEUIFramerProjectionResult const& source ) noexcept;
      virtual void Copy( __in RAS_PPP_NBFCP_RESULT const * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CNetBEUIFramerProjectionResult& operator = ( __in CNetBEUIFramerProjectionResult const& source ) noexcept;
};

class CInternetworkPacketExchangeProjectionResult : public CRemoteAccessServiceConnectionProjection
{
   public:

      CInternetworkPacketExchangeProjectionResult();
      CInternetworkPacketExchangeProjectionResult( __in RAS_PPP_IPXCP_RESULT const * source );
      CInternetworkPacketExchangeProjectionResult( __in CInternetworkPacketExchangeProjectionResult const& source );
      CInternetworkPacketExchangeProjectionResult( __in CRemoteAccessServiceConnectionProjection const& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CInternetworkPacketExchangeProjectionResult();

      // Properties

      std::wstring Address;

      // Methods

      virtual void Copy( CRemoteAccessServiceConnectionProjection const& source ) noexcept; // Parent's method
      virtual void Copy( CInternetworkPacketExchangeProjectionResult const& source ) noexcept;
      virtual void Copy( RAS_PPP_IPXCP_RESULT const * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CInternetworkPacketExchangeProjectionResult& operator = ( CInternetworkPacketExchangeProjectionResult const& source ) noexcept;
};

class CInternetProtocolProjectionResult : public CRemoteAccessServiceConnectionProjection
{
   public:

      CInternetProtocolProjectionResult();
      CInternetProtocolProjectionResult( __in RAS_PPP_IPCP_RESULT const * source );
      CInternetProtocolProjectionResult( __in CInternetProtocolProjectionResult const& source );
      CInternetProtocolProjectionResult( __in CRemoteAccessServiceConnectionProjection const& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CInternetProtocolProjectionResult();

      // Properties

      std::wstring Address;

      // Methods

      virtual void Copy( __in CRemoteAccessServiceConnectionProjection const& source ) noexcept; // Parent's method
      virtual void Copy( __in CInternetProtocolProjectionResult const& source ) noexcept;
      virtual void Copy( __in RAS_PPP_IPCP_RESULT const * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CInternetProtocolProjectionResult& operator = ( __in CInternetProtocolProjectionResult const& source ) noexcept;
};

class CAppleTalkProjectionResult : public CRemoteAccessServiceConnectionProjection
{
   public:

      CAppleTalkProjectionResult();
      CAppleTalkProjectionResult( __in RAS_PPP_ATCP_RESULT const * source );
      CAppleTalkProjectionResult( __in CAppleTalkProjectionResult const& source );
      CAppleTalkProjectionResult( __in CRemoteAccessServiceConnectionProjection const& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CAppleTalkProjectionResult();

      // Properties

      std::wstring Address;

      // Methods

      virtual void Copy( __in CRemoteAccessServiceConnectionProjection const& source ) noexcept; // Parent's method
      virtual void Copy( __in CAppleTalkProjectionResult const& source ) noexcept;
      virtual void Copy( __in RAS_PPP_ATCP_RESULT const * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CAppleTalkProjectionResult& operator = ( __in CAppleTalkProjectionResult const& source ) noexcept;
};

class CPointToPointProtocolProjectionResult
{
   public:

      CPointToPointProtocolProjectionResult();
      CPointToPointProtocolProjectionResult( __in CPointToPointProtocolProjectionResult const& source );
      CPointToPointProtocolProjectionResult( __in RAS_PPP_PROJECTION_RESULT const * source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CPointToPointProtocolProjectionResult();

      // Properties

      CAppleTalkProjectionResult                  AppleTalk;
      CInternetProtocolProjectionResult           IP;
      CInternetworkPacketExchangeProjectionResult IPX;
      CNetBEUIFramerProjectionResult              NetBEUI;

      // Methods

      virtual void Copy( __in CPointToPointProtocolProjectionResult const& source ) noexcept;
      virtual void Copy( __in RAS_PPP_PROJECTION_RESULT const * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CPointToPointProtocolProjectionResult& operator = ( __in CPointToPointProtocolProjectionResult const& source ) noexcept;
};

class CRemoteAccessServiceUser
{
   public:

      CRemoteAccessServiceUser();
      CRemoteAccessServiceUser( __in RAS_USER_0 const * source );
      CRemoteAccessServiceUser( __in CRemoteAccessServiceUser const& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRemoteAccessServiceUser();

      // Properties

      BYTE    Privileges{ 0 };
      std::wstring PhoneNumber;

      // Methods

      virtual void Copy( __in RAS_USER_0 const * source ) noexcept;
      virtual void Copy( __in CRemoteAccessServiceUser const& source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CRemoteAccessServiceUser& operator = ( __in CRemoteAccessServiceUser const& source ) noexcept;
};

class CRemoteAccessServicePort
{
   public:

      CRemoteAccessServicePort();
      CRemoteAccessServicePort( __in RAS_PORT_0 const * source );
      CRemoteAccessServicePort( __in RAS_PORT_1 const * source );
      CRemoteAccessServicePort( __in CRemoteAccessServicePort const& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRemoteAccessServicePort();

      // Possible values for MediaID

      enum _Media
      {
         mediaUnknown     = MEDIA_UNKNOWN,
         mediaSerial      = MEDIA_SERIAL,
         mediaRAS10Serial = MEDIA_RAS10_SERIAL,
         mediaX25         = MEDIA_X25,
         mediaISDN        = MEDIA_ISDN
      };

      // Properties
      // Patterned after RAS_PORT_x structures

      std::wstring Name;                     // RAS_PORT_0.wszPortName
      std::wstring DeviceType;               // RAS_PORT_0.wszDeviceType
      std::wstring DeviceName;               // RAS_PORT_0.wszDeviceName
      std::wstring MediaName;                // RAS_PORT_0.wszMediaName
      DWORD   MediaID{ 0 };                  // RAS_PORT_0.MediaID
      DWORD   Flags{ 0 };                    // RAS_PORT_0.Flags
      std::wstring UserName;                 // RAS_PORT_0.wszUserName
      std::wstring ComputerName;             // RAS_PORT_0.wszComputer
      CTime   SessionStartTime;         // RAS_PORT_0.dwStartSessionTime
      std::wstring LogonDomainName;          // RAS_PORT_0.wszLogonDomain
      BOOL    IsAdvancedServer{ FALSE };         // RAS_PORT_0.fAdvancedServer
      DWORD   LineCondition{ 0 };            // RAS_PORT_1.LineCondition
      DWORD   HardwareCondition{ 0 };        // RAS_PORT_1.HardwareCondition
      DWORD   LineSpeedInBitsPerSecond{ 0 }; // RAS_PORT_1.LineSpeed
      WORD    NumberOfStatistics{ 0 };       // RAS_PORT_1.NumStatistics
      WORD    NumberOfMediaParameters{ 0 };  // RAS_PORT_1.NumMediaParms
      DWORD   SizeOfMediaParameters{ 0 };    // RAS_PORT_1.SizeMediaParms
      CPointToPointProtocolProjectionResult ProjectionResult;

      // Methods

      virtual void Copy( __in CRemoteAccessServicePort const& source ) noexcept;
      virtual void Copy( __in RAS_PORT_0 const * source ) noexcept;
      virtual void Copy( __in RAS_PORT_1 const * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CRemoteAccessServicePort& operator = ( __in CRemoteAccessServicePort const& source ) noexcept;
};

class CRemoteAccessServicePortStatistics
{
   public:

      CRemoteAccessServicePortStatistics();
      CRemoteAccessServicePortStatistics( __in RAS_PORT_STATISTICS const * source );
      CRemoteAccessServicePortStatistics( __in CRemoteAccessServicePortStatistics const& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRemoteAccessServicePortStatistics();

      // Properties
      // Patterned after RAS_PORT_STATISTICS

      DWORD NumberOfBytesTransmitted{ 0 };
      DWORD NumberOfBytesReceived{ 0 };
      DWORD NumberOfFramesTransmitted{ 0 };
      DWORD NumberOfFramesReceived{ 0 };
      DWORD NumberOfCRCErrors{ 0 };
      DWORD NumberOfTimeoutErrors{ 0 };
      DWORD NumberOfAlignmentErrors{ 0 };
      DWORD NumberOfHardwareOverrunErrors{ 0 };
      DWORD NumberOfFramingErrors{ 0 };
      DWORD NumberOfBufferOverrunErrors{ 0 };
      DWORD NumberOfBytesTransmittedUncompressed{ 0 };
      DWORD NumberOfBytesReceivedUncompressed{ 0 };
      DWORD NumberOfBytesTransmittedCompressed{ 0 };
      DWORD NumberOfBytesReceivedCompressed{ 0 };

      // Methods

      virtual _Check_return_ int Compare( __in CRemoteAccessServicePortStatistics const& source ) const noexcept;
      virtual void Copy( __in RAS_PORT_STATISTICS const * source ) noexcept;
      virtual void Copy( __in CRemoteAccessServicePortStatistics const& source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CRemoteAccessServicePortStatistics& operator = ( __in CRemoteAccessServicePortStatistics const& source ) noexcept;
      virtual bool operator == ( __in CRemoteAccessServicePortStatistics const& right_port ) const noexcept;
      virtual bool operator <  ( __in CRemoteAccessServicePortStatistics const& right_port ) const noexcept;
      virtual bool operator >  ( __in CRemoteAccessServicePortStatistics const& right_port ) const noexcept;
      virtual bool operator <= ( __in CRemoteAccessServicePortStatistics const& right_port ) const noexcept;
      virtual bool operator >= ( __in CRemoteAccessServicePortStatistics const& right_port ) const noexcept;
};

#endif // REMOTE_ACCESS_SERVICE_PORT_DATA_CLASS_HEADER
