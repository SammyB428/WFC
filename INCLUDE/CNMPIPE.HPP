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
** $Workfile: cnmpipe.hpp $
** $Revision: 13 $
** $Modtime: 8/08/01 4:56a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

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

      enum class NamedPipeType : uint32_t
      {
         ClientEnd = PIPE_CLIENT_END,
         ServerEnd = PIPE_SERVER_END,
         Message   = PIPE_TYPE_MESSAGE
      };

   private:

      void m_Initialize( _In_ uint32_t const input_buffer_size, _In_ uint32_t const output_buffer_size ) noexcept;

   protected:

       HANDLE m_PipeHandle{ INVALID_HANDLE_VALUE };

       bool m_AutomaticallyDelete{ false };

       NamedPipeType m_TypeOfPipe{ NamedPipeType::ClientEnd }; // From GetInfo()

      std::unique_ptr<uint8_t[]> m_InputBuffer;
      std::unique_ptr<uint8_t[]> m_OutputBuffer;

      DWORD m_MaximumNumberOfInstances{ 0 }; // From GetInfo()

      DWORD m_InputBufferSize{ 0 };
      DWORD m_OutputBufferSize{ 0 };
      DWORD m_NumberOfBytesToWrite{ 0 };
      DWORD m_NumberOfBytesRead{ 0 };
      DWORD m_Timeout{ NMPWAIT_USE_DEFAULT_WAIT };

      std::wstring m_PipeName;

      CUniversalNamingConvention m_FullPipeName;

      /*
      ** Information returned from GetNamedPipeHandleState()
      */

      DWORD m_PipeState{ 0 };
      DWORD m_NumberOfInstances{ 0 };
      DWORD m_MaximumNumberOfBytesBeforeRemoteTransmission{ 0 };
      DWORD m_MaximumNumberOfMillisecondsBeforeRemoteTransmission{ 0 };

      std::wstring m_UserNameOfClientProcess;
      
   public:

       CNamedPipe(CNamedPipe const&) = delete;
       CNamedPipe& operator=(CNamedPipe const&) = delete;

       CNamedPipe( _In_ uint32_t const input_buffer_size = 4096, _In_ uint32_t const output_buffer_size = 4096 ) noexcept;

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CNamedPipe() noexcept;

      /*
      ** The Win32 API
      */

      virtual _Check_return_ bool Call( __inout    std::wstring& name_of_pipe,
                                    LPVOID   write_buffer,
                         _In_ DWORD const   size_of_write_buffer,
                                    LPVOID   read_buffer,
                         _In_ DWORD const   size_of_read_buffer,
                                    DWORD   *address_of_number_of_bytes_read,
                         _In_ DWORD const   number_of_milliseconds_to_wait = NMPWAIT_WAIT_FOREVER ) noexcept; // CallNamedPipe

      virtual _Check_return_ bool Connect( __inout_opt LPOVERLAPPED overlapped_p = nullptr ) noexcept; // ConnectNamedPipe

      virtual _Check_return_ bool Create( __in_z LPCTSTR server_name,
                           __in_z LPCTSTR name_of_pipe,
                           _In_ DWORD const open_mode       = PIPE_ACCESS_DUPLEX,
                           _In_ DWORD const type_of_pipe    = PIPE_TYPE_BYTE,
                           _In_ DWORD const number_of_pipes = PIPE_UNLIMITED_INSTANCES,
                           __inout_opt LPSECURITY_ATTRIBUTES security_attributes_p = nullptr ) noexcept;

      virtual _Check_return_ bool Disconnect( void ) noexcept; // DisconnectNamedPipe
      virtual _Check_return_ bool GetState( void ) noexcept; // GetNamedPipeHandleState
      virtual _Check_return_ bool GetInformation( void ) noexcept; // GetNamedPipeInfo

      virtual _Check_return_ bool Peek( __out_bcount( size_of_buffer ) LPVOID buffer_address,
                         _In_  DWORD const size_of_buffer,
                         _Out_ DWORD& number_of_bytes_read,
                         _Out_ DWORD& number_of_bytes_available,
                         _Out_ DWORD& number_of_bytes_remaining_in_message ) noexcept;
      
      virtual _Check_return_ bool SetState( _In_ uint32_t new_pipe_mode,
                             _In_ uint32_t maximum_number_of_bytes_before_transmission,
                             _In_ uint32_t maximum_number_of_milliseconds_before_transmission ) noexcept;
      
      virtual _Check_return_ bool Transact( __inout_opt LPOVERLAPPED overlapped_p = nullptr ) noexcept; // TransactNamedPipe
      
      virtual _Check_return_ bool Wait( __in_z_opt LPCTSTR server_name,
                         __in_z LPCTSTR name_of_pipe, 
                         _In_ uint32_t const number_of_milliseconds = NMPWAIT_USE_DEFAULT_WAIT ) noexcept;

      /*
      ** API's to make life a little easier 
      */

      _Check_return_ bool Open(_In_ std::filesystem::path const& pipe_name, _In_ UINT const open_flags = PIPE_CLIENT_END ) noexcept override;
      void Close( void ) noexcept override;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // NAMED_PIPE_CLASS_HEADER
