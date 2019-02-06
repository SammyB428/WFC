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
** $Workfile: cnmpipe.hpp $
** $Revision: 13 $
** $Modtime: 8/08/01 4:56a $
*/

#if ! defined( NAMED_PIPE_CLASS_HEADER )

#define NAMED_PIPE_CLASS_HEADER

/*
** 2001-08-08
** If you are going to use named pipes in your project, you need to be
** aware of their drawbacks. Namely, they cannot send more than 64KB of
** data at a time. Knowledge Base article Q119218 states this (after
** I discovered the limitation on my own). Like many things from Microsoft,
** named pipes are *almost* useful.
*/

class CNamedPipe : public CDummyFile
{
   public:

      enum NamedPipeTypes
      {
         typeClientEnd = PIPE_CLIENT_END,
         typeServerEnd = PIPE_SERVER_END,
         typeMessage   = PIPE_TYPE_MESSAGE
      };

   private:

      void m_Initialize( __in const uint32_t input_buffer_size, __in const uint32_t output_buffer_size ) noexcept;

   protected:

      HANDLE m_PipeHandle;

      BOOL m_AutomaticallyDelete;

      NamedPipeTypes m_TypeOfPipe; // From GetInfo()

      std::unique_ptr<uint8_t[]> m_InputBuffer;
      std::unique_ptr<uint8_t[]> m_OutputBuffer;

      DWORD m_MaximumNumberOfInstances; // From GetInfo()

      DWORD m_InputBufferSize;
      DWORD m_OutputBufferSize;
      DWORD m_NumberOfBytesToWrite;
      DWORD m_NumberOfBytesRead;
      DWORD m_Timeout;

      std::wstring m_PipeName;

      CUniversalNamingConvention m_FullPipeName;

      /*
      ** Information returned from GetNamedPipeHandleState()
      */

      DWORD m_PipeState;
      DWORD m_NumberOfInstances;
      DWORD m_MaximumNumberOfBytesBeforeRemoteTransmission;
      DWORD m_MaximumNumberOfMillisecondsBeforeRemoteTransmission;

      std::wstring m_UserNameOfClientProcess;
      
   public:

       CNamedPipe(const CNamedPipe&) = delete;
       CNamedPipe& operator=(const CNamedPipe&) = delete;

       CNamedPipe( __in const uint32_t input_buffer_size = 4096, __in const uint32_t output_buffer_size = 4096 );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CNamedPipe();

      /*
      ** The Win32 API
      */

      virtual __checkReturn BOOL Call( __inout    std::wstring& name_of_pipe,
                                    LPVOID   write_buffer,
                         __in const DWORD    size_of_write_buffer,
                                    LPVOID   read_buffer,
                         __in const DWORD    size_of_read_buffer,
                                    DWORD   *address_of_number_of_bytes_read,
                         __in const DWORD    number_of_milliseconds_to_wait = NMPWAIT_WAIT_FOREVER ) noexcept; // CallNamedPipe

      virtual __checkReturn bool Connect( __inout_opt LPOVERLAPPED overlapped_p = nullptr ) noexcept; // ConnectNamedPipe

      virtual __checkReturn bool Create( __in_z LPCTSTR server_name,
                           __in_z LPCTSTR name_of_pipe,
                           __in const DWORD   open_mode       = PIPE_ACCESS_DUPLEX,
                           __in const DWORD   type_of_pipe    = PIPE_TYPE_BYTE,
                           __in const DWORD   number_of_pipes = PIPE_UNLIMITED_INSTANCES,
                           __inout_opt LPSECURITY_ATTRIBUTES security_attributes_p = nullptr ) noexcept;

      virtual __checkReturn bool Disconnect( void ) noexcept; // DisconnectNamedPipe
      virtual __checkReturn bool GetState( void ) noexcept; // GetNamedPipeHandleState
      virtual __checkReturn bool GetInformation( void ) noexcept; // GetNamedPipeInfo

      virtual __checkReturn BOOL Peek( __out_bcount( size_of_buffer ) LPVOID buffer_address,
                         __in const DWORD  size_of_buffer,
                         __out DWORD& number_of_bytes_read,
                         __out DWORD& number_of_bytes_available,
                         __out DWORD& number_of_bytes_remaining_in_message ) noexcept;
      
      virtual __checkReturn bool SetState( __in uint32_t new_pipe_mode,
                             __in uint32_t maximum_number_of_bytes_before_transmission,
                             __in uint32_t maximum_number_of_milliseconds_before_transmission ) noexcept;
      
      virtual __checkReturn bool Transact( __inout_opt LPOVERLAPPED overlapped_p = nullptr ) noexcept; // TransactNamedPipe
      
      virtual __checkReturn bool Wait( __in_z_opt LPCTSTR server_name,
                         __in_z LPCTSTR name_of_pipe, 
                         __in const uint32_t number_of_milliseconds = NMPWAIT_USE_DEFAULT_WAIT ) noexcept;

      /*
      ** API's to make life a little easier 
      */

      virtual __checkReturn bool Open( __in_z LPCTSTR pipe_name, __in const UINT open_flags = PIPE_CLIENT_END ) noexcept override;
      virtual void Close( void ) noexcept override;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // NAMED_PIPE_CLASS_HEADER
