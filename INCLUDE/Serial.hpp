/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2019, Samuel R. Blackburn
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

       bool m_IsOpen{ false };
       bool m_PurgeBufferOnError{ false };

       DWORD m_CommunicationErrorCodes{ 0 };
       DWORD m_InputBufferSize{ 0 };
       DWORD m_OutputBufferSize{ 0 };

      COMSTAT m_CommunicationsStatus;

      virtual void m_ClearError( __in int const line_number = 0 ) noexcept;

   public:

       CSerialFile(CSerialFile const&) = delete;
       CSerialFile& operator=(CSerialFile const&) = delete;
       
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
         purgeAll                     = PURGE_TXCLEAR bitor PURGE_RXCLEAR bitor PURGE_RXABORT bitor PURGE_TXABORT
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

      HANDLE FileHandle{ INVALID_HANDLE_VALUE };

      virtual _Check_return_ BOOL  Attach( __in HANDLE new_handle, __out_opt HANDLE * old_handle ) noexcept;
      virtual _Check_return_ BOOL  CancelWaitFor( void ) noexcept;
      virtual _Check_return_ BOOL  ClearBreak( void ) noexcept;
      virtual void  Close( void ) noexcept;
      virtual _Check_return_ BOOL  ConfigurationDialog( __inout CCommunicationsConfiguration& configuration, __in BOOL const save_changes = TRUE, __in HWND const parent_window_handle = static_cast< HWND >( NULL ) ) noexcept;
      virtual _Check_return_ HANDLE Detach( void ) noexcept;
      virtual _Check_return_ DWORD GetBaudRate( void ) noexcept;
      virtual _Check_return_ BOOL  GetConfiguration( __inout CCommunicationsConfiguration& configuration ) noexcept;
      virtual _Check_return_ DWORD GetFlowControl( void ) noexcept;
      virtual _Check_return_ DWORD GetInputBufferSize( void ) noexcept;
      virtual _Check_return_ DWORD GetModemStatus( void ) noexcept;
      virtual _Check_return_ DWORD GetOutputBufferSize( void ) noexcept;
      virtual _Check_return_ BOOL  GetProperties( __inout CCommunicationProperties& properties ) noexcept;
      virtual _Check_return_ BOOL  GetState( __inout CDeviceControlBlock& device_control_block ) noexcept;
      virtual _Check_return_ BOOL  GetTimeouts( __inout COMMTIMEOUTS& timeouts ) noexcept;
      virtual _Check_return_ bool  IsDataWaiting( void ) noexcept;
      virtual _Check_return_ DWORD NumberOfBytesWaitingToBeRead( void ) noexcept;
      virtual _Check_return_ DWORD NumberOfBytesWaitingToBeWritten( void ) noexcept;
      virtual _Check_return_ bool  Open( void ) noexcept; // Name already filled, used in re-opening an existing session
      _Check_return_ bool  Open( __in_z LPCTSTR channel_name, __in UINT const open_flags = 0 ) noexcept override;

      virtual _Check_return_ BOOL  Purge( __in DWORD const what_to_purge = purgeAll ) noexcept;
      virtual _Check_return_ UINT  Read( __out_bcount( length ) void * buffer, __in UINT const length ) noexcept;
      virtual void ReplaceGarbledCharacter( __in bool const yes_or_no = TRUE, __in uint8_t const character_to_replace_the_garbled_one_with = ' ' ) noexcept;

      virtual _Check_return_ bool  SetBaudRate( __in uint32_t const baud_rate ) noexcept;
      virtual _Check_return_ BOOL  SetBreak( void ) noexcept;
      virtual _Check_return_ BOOL  SetCharacterToWaitFor( __in uint8_t const character_to_wait_for ) noexcept;
      virtual _Check_return_ BOOL  SetDataTerminalReady( __in BOOL const set_DTR_on = TRUE ) noexcept;
      virtual void  SetFlowControl( __in DWORD const flow_control ) noexcept;
      virtual void  SetInputBufferSize( __in DWORD const buffer_size ) noexcept;
      virtual void  SetOutputBufferSize( __in DWORD const buffer_size ) noexcept;
      virtual _Check_return_ bool  SetPurgeBufferOnError( __in bool const purge_buffer = true ) noexcept;
      virtual _Check_return_ BOOL  SetRequestToSend( __in BOOL const set_RTS_on = TRUE ) noexcept;
      virtual _Check_return_ BOOL  SetState( __inout CDeviceControlBlock& device_control_block ) noexcept;
      virtual _Check_return_ BOOL  SetTimeouts( __in_opt COMMTIMEOUTS const * timeouts_p = nullptr ) noexcept;
      virtual _Check_return_ BOOL  TransmitCharacter( __in char const character_to_transmit ) noexcept;
      virtual _Check_return_ BOOL  WaitFor( __inout DWORD& stuff_you_can_wait_for ) noexcept;
      virtual _Check_return_ bool  WaitForString( __in std::string const& string_to_wait_for, __in DWORD const seconds = 5, __inout_opt std::string * what_was_read = nullptr ) noexcept;
      void  Write( __in std::vector<uint8_t> const& bytes ) noexcept override;
      void  Write( __in std::string const& data_to_write ) noexcept override;
      void  Write( __in_bcount( number_of_bytes ) void const * buffer, __in UINT const number_of_bytes ) noexcept override;
      virtual void  Write( __in BYTE const byte_to_write ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // SERIAL_FILE_CLASS_HEADER
