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
** $Workfile: serial.hpp $
** $Revision: 27 $
** $Modtime: 6/26/01 11:06a $
*/

#if ! defined( SERIAL_FILE_CLASS_HEADER )

#define SERIAL_FILE_CLASS_HEADER

class CDeviceControlBlock;

class CSerialFile : public CDummyFile
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      bool m_IsOpen;
      bool m_PurgeBufferOnError;

      DWORD m_CommunicationErrorCodes;
      DWORD m_InputBufferSize;
      DWORD m_OutputBufferSize;

      COMSTAT m_CommunicationsStatus;

      virtual void m_ClearError( __in const int line_number = 0 ) noexcept;

   public:

       CSerialFile(const CSerialFile&) = delete;
       CSerialFile& operator=(const CSerialFile&) = delete;
       
       CSerialFile();

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CSerialFile();

      enum _Flows
      {
         flowNone     = 0,
         flowRtsCts   = 1,
         flowDtrDsr   = 2,
         flowXonXoff  = 4,
         flowSoftware = flowXonXoff,
         flowHardware = flowRtsCts
      };

      enum _Purges
      {
         purgeTerminateWriteOperation = PURGE_TXABORT,
         purgeTerminateReadOperation  = PURGE_RXABORT,
         purgeClearInputBuffer        = PURGE_RXCLEAR,
         purgeClearOutputBuffer       = PURGE_TXCLEAR,
         purgeAll                     = PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_RXABORT | PURGE_TXABORT
      };

      enum _StuffYouCanWaitFor
      {
         waitBreakDetected                       = EV_BREAK,
         waitClearToSendChangedState             = EV_CTS,
         waitDataSetReadyChangedState            = EV_DSR,
         waitLineStatusError                     = EV_ERR,
         waitRing                                = EV_RING,
         waitReceiveLineSignalDetectChangedState = EV_RLSD,
         waitAnyCharacterReceived                = EV_RXCHAR,
         waitParticularCharacterReceived         = EV_RXFLAG,
         waitTransmitBufferEmpty                 = EV_TXEMPTY,
         waitPrinterErrorOccured                 = EV_PERR,
         waitReceiveBuffer80PercentFull          = EV_RX80FULL,
         waitProviderSpecificEvent1              = EV_EVENT1,
         waitProviderSpecificEvent2              = EV_EVENT2
      };

      std::wstring Name; // COM1:9600,n,8,1

      HANDLE FileHandle;

      virtual __checkReturn BOOL  Attach( __in HANDLE new_handle, __out_opt HANDLE * old_handle ) noexcept;
      virtual __checkReturn BOOL  CancelWaitFor( void ) noexcept;
      virtual __checkReturn BOOL  ClearBreak( void ) noexcept;
      virtual void  Close( void ) noexcept;
      virtual __checkReturn BOOL  ConfigurationDialog( __inout CCommunicationsConfiguration& configuration, __in const BOOL save_changes = TRUE, __in const HWND parent_window_handle = static_cast< HWND >( NULL ) ) noexcept;
      virtual __checkReturn HANDLE Detach( void ) noexcept;
      virtual __checkReturn DWORD GetBaudRate( void ) noexcept;
      virtual __checkReturn BOOL  GetConfiguration( __inout CCommunicationsConfiguration& configuration ) noexcept;
      virtual __checkReturn DWORD GetFlowControl( void ) noexcept;
      virtual __checkReturn DWORD GetInputBufferSize( void ) noexcept;
      virtual __checkReturn DWORD GetModemStatus( void ) noexcept;
      virtual __checkReturn DWORD GetOutputBufferSize( void ) noexcept;
      virtual __checkReturn BOOL  GetProperties( __inout CCommunicationProperties& properties ) noexcept;
      virtual __checkReturn BOOL  GetState( __inout CDeviceControlBlock& device_control_block ) noexcept;
      virtual __checkReturn BOOL  GetTimeouts( __inout COMMTIMEOUTS& timeouts ) noexcept;
      virtual __checkReturn bool  IsDataWaiting( void ) noexcept;
      virtual __checkReturn DWORD NumberOfBytesWaitingToBeRead( void ) noexcept;
      virtual __checkReturn DWORD NumberOfBytesWaitingToBeWritten( void ) noexcept;
      virtual __checkReturn bool  Open( void ) noexcept; // Name already filled, used in re-opening an existing session
      __checkReturn bool  Open( __in_z LPCTSTR channel_name, __in const UINT open_flags = 0 ) noexcept override;

      virtual __checkReturn BOOL  Purge( __in const DWORD what_to_purge = purgeAll ) noexcept;
      virtual __checkReturn UINT  Read( __out_bcount( length ) void * buffer, __in const UINT length ) noexcept;
      virtual void ReplaceGarbledCharacter( __in const bool yes_or_no = TRUE, __in const BYTE character_to_replace_the_garbled_one_with = ' ' ) noexcept;

      virtual __checkReturn bool  SetBaudRate( __in const uint32_t baud_rate ) noexcept;
      virtual __checkReturn BOOL  SetBreak( void ) noexcept;
      virtual __checkReturn BOOL  SetCharacterToWaitFor( __in const BYTE character_to_wait_for ) noexcept;
      virtual __checkReturn BOOL  SetDataTerminalReady( __in const BOOL set_DTR_on = TRUE ) noexcept;
      virtual void  SetFlowControl( __in const DWORD flow_control ) noexcept;
      virtual void  SetInputBufferSize( __in const DWORD buffer_size ) noexcept;
      virtual void  SetOutputBufferSize( __in const DWORD buffer_size ) noexcept;
      virtual __checkReturn BOOL  SetPurgeBufferOnError( __in const BOOL purge_buffer = TRUE ) noexcept;
      virtual __checkReturn BOOL  SetRequestToSend( __in const BOOL set_RTS_on = TRUE ) noexcept;
      virtual __checkReturn BOOL  SetState( __inout CDeviceControlBlock& device_control_block ) noexcept;
      virtual __checkReturn BOOL  SetTimeouts( __in_opt const COMMTIMEOUTS * timeouts_p = nullptr ) noexcept;
      virtual __checkReturn BOOL  TransmitCharacter( __in const char character_to_transmit ) noexcept;
      virtual __checkReturn BOOL  WaitFor( __inout DWORD& stuff_you_can_wait_for ) noexcept;
      virtual __checkReturn bool  WaitForString( __in const std::string& string_to_wait_for, __in const DWORD seconds = 5, __inout_opt std::string * what_was_read = nullptr ) noexcept;
      void  Write( __in const std::vector<uint8_t>& bytes ) noexcept override;
      void  Write( __in const std::string& data_to_write ) noexcept override;
      void  Write( __in_bcount( number_of_bytes ) const void *buffer, __in const UINT number_of_bytes ) noexcept override;
      virtual void  Write( __in const BYTE byte_to_write ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // SERIAL_FILE_CLASS_HEADER
