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
      CRemoteAccessServiceConnectionProjection( __in const CRemoteAccessServiceConnectionProjection& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRemoteAccessServiceConnectionProjection();

      // Properties

      DWORD ErrorCode;

      // Methods

      virtual void Copy( __in const CRemoteAccessServiceConnectionProjection& source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CRemoteAccessServiceConnectionProjection& operator = ( __in const CRemoteAccessServiceConnectionProjection& source ) noexcept;
};

class CNetBEUIFramerProjectionResult : public CRemoteAccessServiceConnectionProjection
{
   public:

      CNetBEUIFramerProjectionResult();
      CNetBEUIFramerProjectionResult( __in const RAS_PPP_NBFCP_RESULT * source );
      CNetBEUIFramerProjectionResult( __in const CNetBEUIFramerProjectionResult& source );
      CNetBEUIFramerProjectionResult( __in const CRemoteAccessServiceConnectionProjection& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CNetBEUIFramerProjectionResult();

      // Properties

      DWORD   NetBiosError;
      std::wstring Name;
      std::wstring WorkstationName;

      // Methods

      virtual void Copy( __in const CRemoteAccessServiceConnectionProjection& source ) noexcept; // Parent's method
      virtual void Copy( __in const CNetBEUIFramerProjectionResult& source ) noexcept;
      virtual void Copy( __in const RAS_PPP_NBFCP_RESULT * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CNetBEUIFramerProjectionResult& operator = ( __in const CNetBEUIFramerProjectionResult& source ) noexcept;
};

class CInternetworkPacketExchangeProjectionResult : public CRemoteAccessServiceConnectionProjection
{
   public:

      CInternetworkPacketExchangeProjectionResult();
      CInternetworkPacketExchangeProjectionResult( __in const RAS_PPP_IPXCP_RESULT * source );
      CInternetworkPacketExchangeProjectionResult( __in const CInternetworkPacketExchangeProjectionResult& source );
      CInternetworkPacketExchangeProjectionResult( __in const CRemoteAccessServiceConnectionProjection& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CInternetworkPacketExchangeProjectionResult();

      // Properties

      std::wstring Address;

      // Methods

      virtual void Copy( const CRemoteAccessServiceConnectionProjection& source ) noexcept; // Parent's method
      virtual void Copy( const CInternetworkPacketExchangeProjectionResult& source ) noexcept;
      virtual void Copy( const RAS_PPP_IPXCP_RESULT * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CInternetworkPacketExchangeProjectionResult& operator = ( const CInternetworkPacketExchangeProjectionResult& source ) noexcept;
};

class CInternetProtocolProjectionResult : public CRemoteAccessServiceConnectionProjection
{
   public:

      CInternetProtocolProjectionResult();
      CInternetProtocolProjectionResult( __in const RAS_PPP_IPCP_RESULT * source );
      CInternetProtocolProjectionResult( __in const CInternetProtocolProjectionResult& source );
      CInternetProtocolProjectionResult( __in const CRemoteAccessServiceConnectionProjection& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CInternetProtocolProjectionResult();

      // Properties

      std::wstring Address;

      // Methods

      virtual void Copy( __in const CRemoteAccessServiceConnectionProjection& source ) noexcept; // Parent's method
      virtual void Copy( __in const CInternetProtocolProjectionResult& source ) noexcept;
      virtual void Copy( __in const RAS_PPP_IPCP_RESULT * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CInternetProtocolProjectionResult& operator = ( __in const CInternetProtocolProjectionResult& source ) noexcept;
};

class CAppleTalkProjectionResult : public CRemoteAccessServiceConnectionProjection
{
   public:

      CAppleTalkProjectionResult();
      CAppleTalkProjectionResult( __in const RAS_PPP_ATCP_RESULT * source );
      CAppleTalkProjectionResult( __in const CAppleTalkProjectionResult& source );
      CAppleTalkProjectionResult( __in const CRemoteAccessServiceConnectionProjection& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CAppleTalkProjectionResult();

      // Properties

      std::wstring Address;

      // Methods

      virtual void Copy( __in const CRemoteAccessServiceConnectionProjection& source ) noexcept; // Parent's method
      virtual void Copy( __in const CAppleTalkProjectionResult& source ) noexcept;
      virtual void Copy( __in const RAS_PPP_ATCP_RESULT * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CAppleTalkProjectionResult& operator = ( __in const CAppleTalkProjectionResult& source ) noexcept;
};

class CPointToPointProtocolProjectionResult
{
   public:

      CPointToPointProtocolProjectionResult();
      CPointToPointProtocolProjectionResult( __in const CPointToPointProtocolProjectionResult& source );
      CPointToPointProtocolProjectionResult( __in const RAS_PPP_PROJECTION_RESULT * source );

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

      virtual void Copy( __in const CPointToPointProtocolProjectionResult& source ) noexcept;
      virtual void Copy( __in  const RAS_PPP_PROJECTION_RESULT * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CPointToPointProtocolProjectionResult& operator = ( __in const CPointToPointProtocolProjectionResult& source ) noexcept;
};

class CRemoteAccessServiceUser
{
   public:

      CRemoteAccessServiceUser();
      CRemoteAccessServiceUser( __in const RAS_USER_0 * source );
      CRemoteAccessServiceUser( __in const CRemoteAccessServiceUser& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRemoteAccessServiceUser();

      // Properties

      BYTE    Privileges;
      std::wstring PhoneNumber;

      // Methods

      virtual void Copy( __in const RAS_USER_0 * source ) noexcept;
      virtual void Copy( __in const CRemoteAccessServiceUser& source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CRemoteAccessServiceUser& operator = ( __in const CRemoteAccessServiceUser& source ) noexcept;
};

class CRemoteAccessServicePort
{
   public:

      CRemoteAccessServicePort();
      CRemoteAccessServicePort( __in const RAS_PORT_0 * source );
      CRemoteAccessServicePort( __in const RAS_PORT_1 * source );
      CRemoteAccessServicePort( __in const CRemoteAccessServicePort& source );

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
      DWORD   MediaID;                  // RAS_PORT_0.MediaID
      DWORD   Flags;                    // RAS_PORT_0.Flags
      std::wstring UserName;                 // RAS_PORT_0.wszUserName
      std::wstring ComputerName;             // RAS_PORT_0.wszComputer
      CTime   SessionStartTime;         // RAS_PORT_0.dwStartSessionTime
      std::wstring LogonDomainName;          // RAS_PORT_0.wszLogonDomain
      BOOL    IsAdvancedServer;         // RAS_PORT_0.fAdvancedServer
      DWORD   LineCondition;            // RAS_PORT_1.LineCondition
      DWORD   HardwareCondition;        // RAS_PORT_1.HardwareCondition
      DWORD   LineSpeedInBitsPerSecond; // RAS_PORT_1.LineSpeed
      WORD    NumberOfStatistics;       // RAS_PORT_1.NumStatistics
      WORD    NumberOfMediaParameters;  // RAS_PORT_1.NumMediaParms
      DWORD   SizeOfMediaParameters;    // RAS_PORT_1.SizeMediaParms
      CPointToPointProtocolProjectionResult ProjectionResult;

      // Methods

      virtual void Copy( __in const CRemoteAccessServicePort& source ) noexcept;
      virtual void Copy( __in const RAS_PORT_0 * source ) noexcept;
      virtual void Copy( __in const RAS_PORT_1 * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CRemoteAccessServicePort& operator = ( __in const CRemoteAccessServicePort& source ) noexcept;
};

class CRemoteAccessServicePortStatistics
{
   public:

      CRemoteAccessServicePortStatistics();
      CRemoteAccessServicePortStatistics( __in const RAS_PORT_STATISTICS * source );
      CRemoteAccessServicePortStatistics( __in const CRemoteAccessServicePortStatistics& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CRemoteAccessServicePortStatistics();

      // Properties
      // Patterned after RAS_PORT_STATISTICS

      DWORD NumberOfBytesTransmitted;
      DWORD NumberOfBytesReceived;
      DWORD NumberOfFramesTransmitted;
      DWORD NumberOfFramesReceived;
      DWORD NumberOfCRCErrors;
      DWORD NumberOfTimeoutErrors;
      DWORD NumberOfAlignmentErrors;
      DWORD NumberOfHardwareOverrunErrors;
      DWORD NumberOfFramingErrors;
      DWORD NumberOfBufferOverrunErrors;
      DWORD NumberOfBytesTransmittedUncompressed;
      DWORD NumberOfBytesReceivedUncompressed;
      DWORD NumberOfBytesTransmittedCompressed;
      DWORD NumberOfBytesReceivedCompressed;

      // Methods

      virtual __checkReturn int Compare( __in const CRemoteAccessServicePortStatistics& source ) const noexcept;
      virtual void Copy( __in const RAS_PORT_STATISTICS * source ) noexcept;
      virtual void Copy( __in const CRemoteAccessServicePortStatistics& source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG

      // Operators

      virtual CRemoteAccessServicePortStatistics& operator = ( __in const CRemoteAccessServicePortStatistics& source ) noexcept;
      virtual bool operator == ( __in const CRemoteAccessServicePortStatistics& right_port ) const noexcept;
      virtual bool operator <  ( __in const CRemoteAccessServicePortStatistics& right_port ) const noexcept;
      virtual bool operator >  ( __in const CRemoteAccessServicePortStatistics& right_port ) const noexcept;
      virtual bool operator <= ( __in const CRemoteAccessServicePortStatistics& right_port ) const noexcept;
      virtual bool operator >= ( __in const CRemoteAccessServicePortStatistics& right_port ) const noexcept;
};

#endif // REMOTE_ACCESS_SERVICE_PORT_DATA_CLASS_HEADER
