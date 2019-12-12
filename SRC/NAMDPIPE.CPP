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
** $Workfile: namdpipe.cpp $
** $Revision: 26 $
** $Modtime: 8/08/01 4:56a $
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

/*
** 2001-08-08
** If you are going to use named pipes in your project, you need to be
** aware of their drawbacks. Namely, they cannot send more than 64KB of
** data at a time. Knowledge Base article Q119218 states this (after
** I discovered the limitation on my own). Like many things from Microsoft,
** named pipes are *almost* useful.
*/

CNamedPipe::CNamedPipe( __in uint32_t const _input_buffer_size, __in uint32_t const _output_buffer_size ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize( _input_buffer_size, _output_buffer_size );
}

CNamedPipe::~CNamedPipe()
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AutomaticallyDelete != FALSE )
   {
      if ( m_PipeHandle != static_cast< HANDLE >( NULL ) )
      {
         Close();
      }
   }

   m_InputBufferSize = 0L;
   m_OutputBufferSize = 0L;
}

void CNamedPipe::m_Initialize( __in uint32_t const _input_buffer_size, __in uint32_t const _output_buffer_size ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_AutomaticallyDelete = TRUE;
   m_LastError           = 0L;
   m_PipeHandle          = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
   m_Timeout             = NMPWAIT_USE_DEFAULT_WAIT;
   m_PipeName.clear();

   m_InputBufferSize  = _input_buffer_size;
   m_OutputBufferSize = _output_buffer_size;

   m_InputBuffer = std::make_unique<uint8_t[]>(m_InputBufferSize);
   m_OutputBuffer = std::make_unique<uint8_t[]>(m_OutputBufferSize);
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CNamedPipe::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "m_AutomaticallyDelete = "      ) << m_AutomaticallyDelete      << TEXT( "\n" );
   dump_context << TEXT( "m_PipeHandle = "               ) << m_PipeHandle               << TEXT( "\n" );
   dump_context << TEXT( "m_MaximumNumberOfInstances = " ) << m_MaximumNumberOfInstances << TEXT( "\n" );
   dump_context << TEXT( "m_InputBufferSize = "          ) << m_InputBufferSize          << TEXT( "\n" );
   dump_context << TEXT( "m_OutputBufferSize = "         ) << m_OutputBufferSize         << TEXT( "\n" );
   dump_context << TEXT( "m_NumberOfBytesToWrite = "     ) << m_NumberOfBytesToWrite     << TEXT( "\n" );
   dump_context << TEXT( "m_NumberOfBytesRead = "        ) << m_NumberOfBytesRead        << TEXT( "\n" );
   dump_context << TEXT( "m_Timeout = "                  ) << m_Timeout                  << TEXT( "\n" );
   dump_context << TEXT( "m_PipeName = \""               ) << m_PipeName                 << TEXT( "\"\n" );
   dump_context << TEXT( "m_FullPipeName is " );
   m_FullPipeName.Dump( dump_context );
   dump_context << TEXT( "m_PipeState = "                ) << m_PipeState                << TEXT( "\n" );
   dump_context << TEXT( "m_NumberOfInstances = "        ) << m_NumberOfInstances        << TEXT( "\n" );
   dump_context << TEXT( "m_MaximumNumberOfBytesBeforeRemoteTransmission = "           ) << m_MaximumNumberOfBytesBeforeRemoteTransmission << TEXT( "\n" );
   dump_context << TEXT( "m_MaximumNumberOfMillisecondsBeforeRemoteTransmission = "    ) << m_MaximumNumberOfMillisecondsBeforeRemoteTransmission << TEXT( "\n" );
   dump_context << TEXT( "m_UserNameOfClientProcess = \"" ) << m_UserNameOfClientProcess << TEXT( "\"\n" );
}

#endif // _DEBUG

_Check_return_  bool CNamedPipe::Call( __inout std::wstring& pipe_name,
                       LPVOID   write_buffer, 
                 __in DWORD const   size_of_write_buffer,
                       LPVOID   read_buffer, 
                 __in DWORD const   size_of_read_buffer,
                       DWORD   *address_of_number_of_bytes_read,
                 __in DWORD const   number_of_milliseconds_to_wait ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( write_buffer );
   WFC_VALIDATE_POINTER( read_buffer );

   if ( write_buffer == nullptr or read_buffer == nullptr )
   {
      m_LastError = ERROR_INVALID_PARAMETER;
      return( false );
   }

   /*
   ** Writes to the pipe then reads from it
   */

   BOOL const return_value = ::CallNamedPipeW( pipe_name.c_str(), 
                                   write_buffer,
                                   size_of_write_buffer,
                                   read_buffer,
                                   size_of_read_buffer,
                                   address_of_number_of_bytes_read,
                                   number_of_milliseconds_to_wait );

   if ( return_value == false )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );

      return( false );
   }

   return( true );
}

void CNamedPipe::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_PipeHandle != static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      (void) Win32FoundationClasses::wfc_close_handle( m_PipeHandle );
      m_PipeHandle = static_cast< HANDLE >( INVALID_HANDLE_VALUE );
   }
}

_Check_return_ bool CNamedPipe::Connect(__inout_opt LPOVERLAPPED overlapped_p ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( overlapped_p );

   // overlapped_p can be nullptr

   BOOL const return_value = ::ConnectNamedPipe( m_PipeHandle, overlapped_p );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );

      return( false );
   }

   return( true );
}

_Check_return_ bool CNamedPipe::Create( __in_z LPCTSTR server_name,
                         __in_z LPCTSTR pipe_name,
                   __in DWORD const open_mode,
                   __in DWORD const type_of_pipe,
                   __in DWORD const number_of_pipes,
           __inout_opt LPSECURITY_ATTRIBUTES security_attributes_p ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( pipe_name );

   /*
   ** Check parameters for errors
   */

   if ( pipe_name == nullptr )
   {
      m_LastError = ERROR_INVALID_PARAMETER;
      return( false );
   }

   m_PipeName.assign(WSTRING_VIEW(L"\\\\"));

   if ( server_name == nullptr )
   {
      m_PipeName.push_back( '.' );
   }
   else
   {
      m_PipeName.append( server_name );
   }

   m_PipeName.append(WSTRING_VIEW(L"\\PIPE\\"));
   m_PipeName.append( pipe_name );

   CSecurityAttributes security_attributes;

   if ( security_attributes_p == nullptr )
   {
      security_attributes.bInheritHandle = FALSE;
      security_attributes.lpSecurityDescriptor = wfc_create_null_dacl();

      security_attributes_p = (SECURITY_ATTRIBUTES *) &security_attributes;
   }

   m_PipeHandle = ::CreateNamedPipeW( m_PipeName.c_str(),
                                     open_mode,
                                     type_of_pipe, 
                                     number_of_pipes,
                                     m_OutputBufferSize,
                                     m_InputBufferSize,
                                     m_Timeout,
                                     security_attributes_p );

   wfc_destroy_null_dacl( security_attributes.lpSecurityDescriptor );

   if ( m_PipeHandle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      m_LastError = ::GetLastError();

      //WFCTRACEERROR( m_LastError );

      return( false );
   }

   return( true );
}

_Check_return_ bool CNamedPipe::Disconnect( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   BOOL const return_value = ::DisconnectNamedPipe( m_PipeHandle );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );

      return( false );
   }

   return( true );
}

_Check_return_ bool CNamedPipe::GetInformation( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD type_of_pipe                = 0;
   DWORD size_of_output_buffer       = 0;
   DWORD size_of_input_buffer        = 0;
   DWORD maximum_number_of_instances = 0;

   BOOL const return_value = ::GetNamedPipeInfo( m_PipeHandle,
                                     &type_of_pipe,
                                     &size_of_output_buffer,
                                     &size_of_input_buffer,
                                     &maximum_number_of_instances );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );

      return( false );
   }

   return( true );
}

_Check_return_ bool CNamedPipe::GetState( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   TCHAR temp_string[ 513 ];

   ::ZeroMemory( temp_string, sizeof( temp_string ) );

   BOOL const return_value = ::GetNamedPipeHandleStateW( m_PipeHandle,
                                            &m_PipeState,
                                            &m_NumberOfInstances,
                                            &m_MaximumNumberOfBytesBeforeRemoteTransmission,
                                            &m_MaximumNumberOfMillisecondsBeforeRemoteTransmission,
                                             temp_string,
                                             static_cast<DWORD>(std::size( temp_string )) );

   m_UserNameOfClientProcess.assign( temp_string );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );

      return( false );
   }

   return( true );
}

_Check_return_ bool CNamedPipe::Open(_In_ std::wstring_view pipe_name, __in UINT const) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( pipe_name.empty() == true )
   {
      m_LastError = ERROR_INVALID_PARAMETER;
      return( false );
   }

   /*
   ** pipe_name can be:
   ** "pipe_name" where the pipe will be openend on the local machine
   ** "\\server_name\pipe_name" where the pipe will be opened on the server
   ** "\\server_name\PIPE\pipe_name" which is a fully qualified pipe name
   */

   CUniversalNamingConvention unc( pipe_name );

   if ( unc.ServerName.empty() == true )
   {
      unc.ServerName.assign(WSTRING_VIEW(L"."));
   }

   if ( compare_no_case( unc.ShareName, WSTRING_VIEW(L"PIPE") ) != I_AM_EQUAL_TO_THAT )
   {
      std::wstring temp_string = unc.ShareName;

      unc.ShareName.assign(WSTRING_VIEW(L"PIPE"));

      if ( temp_string.empty() == false )
      {
          std::wstring temp_path(temp_string);
          temp_path.push_back('\\');
          temp_path.append(unc.PathName);
          unc.PathName.assign(temp_path);
      }
   }

   unc.Make();

   m_FullPipeName.Copy( unc );

   return( Create( unc.ServerName.c_str(), unc.PathName.c_str() ) );
}

_Check_return_ bool CNamedPipe::Peek( __out_bcount( size_of_buffer ) LPVOID buffer_address,
                 __in DWORD const size_of_buffer,
                      __out DWORD& number_of_bytes_read,
                      __out DWORD& number_of_bytes_available,
                      __out DWORD& number_of_bytes_remaining_in_message ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( buffer_address );

   /*
   ** buffer_address can be nullptr
   */

   BOOL const return_value = ::PeekNamedPipe( m_PipeHandle, 
                                   buffer_address, 
                                   size_of_buffer, 
                                  &number_of_bytes_read,
                                  &number_of_bytes_available,
                                  &number_of_bytes_remaining_in_message );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );

      return( false );
   }

   return( true );
}

_Check_return_ bool CNamedPipe::SetState( __in uint32_t new_pipe_mode,
                           __in uint32_t maximum_number_of_bytes_before_transmission,
                           __in uint32_t maximum_number_of_milliseconds_before_transmission ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD npm = new_pipe_mode;
   DWORD mnbbt = maximum_number_of_bytes_before_transmission;
   DWORD mnmbt = maximum_number_of_milliseconds_before_transmission;

   BOOL const return_value = ::SetNamedPipeHandleState( m_PipeHandle,
                                            &npm,
                                            &mnbbt,
                                            &mnmbt);

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );

      return( false );
   }

   return( true );
}

_Check_return_ bool CNamedPipe::Transact( __inout_opt LPOVERLAPPED overlapped_p ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( overlapped_p );

   // overlapped_p can be nullptr

   BOOL const return_value = ::TransactNamedPipe( m_PipeHandle,
                                       m_OutputBuffer.get(),
                                       m_NumberOfBytesToWrite,
                                       m_InputBuffer.get(),
                                       m_InputBufferSize,
                                      &m_NumberOfBytesRead,
                                       overlapped_p );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );

      return( false );
   }

   return( true );
}

_Check_return_ bool CNamedPipe::Wait( __in_z_opt LPCTSTR name_of_server, __in_z LPCTSTR name_of_pipe, __in uint32_t const number_of_milliseconds ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( name_of_pipe );

   /*
   ** Check parameters for errors
   */

   if ( name_of_pipe == nullptr )
   {
      m_LastError = ERROR_INVALID_PARAMETER;
      return( false );
   }

   std::wstring full_pipe_name(WSTRING_VIEW(L"\\\\")); // like \\servername\pipe\pipename

   if ( name_of_server == nullptr )
   {
      full_pipe_name.push_back( '.' );
   }
   else
   {
      full_pipe_name.append( name_of_server );
   }

   full_pipe_name.append(WSTRING_VIEW(L"\\pipe\\"));
   full_pipe_name.append( name_of_pipe );

   BOOL const return_value = ::WaitNamedPipeW( full_pipe_name.c_str(), number_of_milliseconds );

   if ( return_value == FALSE )
   {
      m_LastError = ::GetLastError();
      //WFCTRACEERROR( m_LastError );

      return(false);
   }

   return( true );
}

// End of source
