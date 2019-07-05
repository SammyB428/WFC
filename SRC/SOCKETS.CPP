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
** $Workfile: sockets.cpp $
** $Revision: 60 $
** $Modtime: 6/26/01 10:56a $
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

long CSimpleSocket::m_ReferenceCount = 0;
WFC_SOCKET_THUNK_P CSimpleSocket::m_Thunk_p = nullptr;

static __checkReturn HANDLE inline _create_socket_thunk_mutex( void )
{
   // We need to create a mutex that *ANY* process on the system can
   // get a handle to regardless of security. To do this, we need to
   // create the mutex with a NULL DACL. Please read Knowledge Base
   // article Q106387

   LPCTSTR mutex_name = TEXT( "CSimpleSocketWinsockThunkInitializationMutex" );

   SECURITY_DESCRIPTOR * security_descriptor_p = nullptr;

   security_descriptor_p = wfc_create_null_dacl();

   if ( security_descriptor_p == nullptr )
   {
      // Oh well, hope for the best...
      //WFCTRACEERROR( GetLastError() );
      //WFCTRACE( TEXT( "Can't allocate memory for security descriptor." ) );
      return( CreateMutex( nullptr, FALSE, mutex_name ) );
   }

   SECURITY_ATTRIBUTES security_attributes;

   security_attributes.nLength              = sizeof( security_attributes );
   security_attributes.lpSecurityDescriptor = security_descriptor_p;
   security_attributes.bInheritHandle       = TRUE;

   const HANDLE mutex_handle = CreateMutex( &security_attributes, FALSE, mutex_name );

   wfc_destroy_null_dacl( security_descriptor_p );

   // Unlike other handle-returning-APIs, CreateMutex returns a NULL value 
   // to represent an invalid handle instead of INVALID_HANDLE_VALUE
   if ( mutex_handle == nullptr )
   {
      // Oh well, hope for the best...
      //WFCTRACEERROR( GetLastError() );
      //WFCTRACE( TEXT( "Can't create mutex with NULL DACL." ) );
      return( CreateMutex( nullptr, FALSE, mutex_name ) );
   }

   return( mutex_handle );
}

CSimpleSocket::CSimpleSocket()
{
   WFC_VALIDATE_POINTER( this );

   m_Initialize();

   if ( m_ReferenceCount == 0 )
   {
      const HANDLE mutex_handle = _create_socket_thunk_mutex();

      if ( mutex_handle != static_cast< HANDLE >( NULL ) )
      {
         //WFCTRACE( TEXT( "Mutex created." ) );

         // Try to take ownership of the mutex.
         // If you're wondering why I didn't use a critical section, it is
         // because of the olde chicken-or-the-egg problem. I can't create
         // a critical section because it must be created via InitializeCriticalSection().
         // There's no way (without changing the interface and breaking everyone's code)
         // to intialize one critical section by two threads executing simultaneously
         // (a feat not possible on single-CPU machines). With a mutex, when you "create"
         // one, you will get a handle to an existing mutex if it is already created.
         // This solves that problem but presents another. Since the mutex is named, it is
         // a system-wide mutex. That means another application using WFC's socket class
         // can block this application. What we really need is some serialization 
         // mechanism that is process-wide and named (something a little more than a
         // critical sectiona and a little less than a named mutex).

         if ( ::WaitForSingleObject( mutex_handle, INFINITE ) == WAIT_OBJECT_0 )
         {
            //WFCTRACE( TEXT( "Protected code entered" ) );

            // We are now the only thread executing this section of the code

            if ( m_ReferenceCount == 0 )
            {
               // Yup. The thunk still needs to be initialized
               //WFCTRACE( TEXT( "Starting sockets." ) );
               StartWindowsSockets();
            }
            else
            {
               //WFCTRACE( TEXT( "Mutex acquired but not needed." ) );
            }

            m_ReferenceCount++;
            ::ReleaseMutex( mutex_handle );
            (void) Win32FoundationClasses::wfc_close_handle( mutex_handle );
            return;
         }
         else
         {
            //WFCTRACE( TEXT( "Couldn't wait for single object." ) );
         }
         
         // We couldn't acquire the mutex

         (void) Win32FoundationClasses::wfc_close_handle( mutex_handle );
      }
      else
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACE( TEXT( "Couldn't create mutex. This is very bad and should not happen." ) );

         StartWindowsSockets();
         m_ReferenceCount++;
         return;
      }
   }
   else
   {
      //WFCTRACEVAL( TEXT( "Thunk initialization skipped because reference count is " ), m_ReferenceCount );
   }

   m_ReferenceCount++;
}

CSimpleSocket::CSimpleSocket( __in const SOCKET socket_id )
{
   WFC_VALIDATE_POINTER( this );

   m_Initialize();

   if ( m_ReferenceCount == 0 )
   {
      const HANDLE mutex_handle = _create_socket_thunk_mutex();

      if ( mutex_handle != static_cast< HANDLE >( NULL ) )
      {
         // Try to take ownership of the mutex.
         // If you're wondering why I didn't use a critical section, it is
         // because of the olde chicken-or-the-egg problem. I can't create
         // a critical section because it must be created via InitializeCriticalSection().
         // There's no way (without changing the interface and breaking everyone's code)
         // to intialize one critical section by two threads executing simultaneously
         // (a feat not possible on single-CPU machines). With a mutex, when you "create"
         // one, you will get a handle to an existing mutex if it is already created.
         // This solves that problem but presents another. Since the mutex is named, it is
         // a system-wide mutex. That means another application using WFC's socket class
         // can block this application. What we really need is some serialization 
         // mechanism that is process-wide and named (something a little more than a
         // critical sectiona and a little less than a named mutex).

         if ( ::WaitForSingleObject( mutex_handle, INFINITE ) == WAIT_OBJECT_0 )
         {
            //WFCTRACE( TEXT( "Protected code entered" ) );

            // We are now the only thread executing this section of the code

            if ( m_ReferenceCount == 0 )
            {
               // Yup. The thunk still needs to be initialized
               //WFCTRACE( TEXT( "Starting sockets." ) );
               StartWindowsSockets();
            }
            else
            {
               //WFCTRACE( TEXT( "Mutex acquired but not needed." ) );
            }

            m_ReferenceCount++;
            ::ReleaseMutex( mutex_handle );
            (void) Win32FoundationClasses::wfc_close_handle( mutex_handle );
            SetID( socket_id );
            return;
         }
         else
         {
            //WFCTRACE( TEXT( "Couldn't wait for single object." ) );
         }
         
         // We couldn't acquire the mutex

         ::ReleaseMutex( mutex_handle );
         (void) Win32FoundationClasses::wfc_close_handle( mutex_handle );
      }
      else
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACE( TEXT( "Couldn't create mutex. This is very bad and should not happen." ) );

         StartWindowsSockets();
         m_ReferenceCount++;
         SetID( socket_id );
         return;
      }
   }
   else
   {
      //WFCTRACEVAL( TEXT( "Thunk initialization skipped because reference count is " ), m_ReferenceCount );
   }

   m_ReferenceCount++;

   SetID( socket_id );
}

CSimpleSocket::~CSimpleSocket()
{
   WFC_VALIDATE_POINTER( this );

   Close();

   m_ReferenceCount--;

   if ( m_ReferenceCount == 0 )
   {
      const HANDLE mutex_handle = _create_socket_thunk_mutex();

      if ( mutex_handle != static_cast< HANDLE >( NULL ) )
      {
         if ( ::WaitForSingleObject( mutex_handle, INFINITE ) == WAIT_OBJECT_0 )
         {
            // We are now the only thread executing this section of the code

            if ( m_ReferenceCount == 0 )
            {
               // Yup. The thunk still needs to be initialized
               //WFCTRACE( TEXT( "Stopping sockets." ) );
               StopWindowsSockets();
            }
            else
            {
               //WFCTRACE( TEXT( "Mutex acquired but not needed." ) );
            }
         }

         ::ReleaseMutex( mutex_handle );
         (void) Win32FoundationClasses::wfc_close_handle( mutex_handle );
      }
      else
      {
         //WFCTRACEERROR( GetLastError() );
         //WFCTRACE( TEXT( "Couldn't create mutex. This is very bad and should not happen." ) );

         StopWindowsSockets();
      }
   }
}

void CSimpleSocket::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   if ( m_SocketID != INVALID_SOCKET )
   {
      //WFCTRACE( TEXT( "Calling closesocket()" ) );

      // Darin Greaham (greaham@cyberramp.com) had a problem with a client
      // not reading data before closing the socket.
      // 1998-08-18

      m_Thunk_p->shutdown( m_SocketID, 1 ); // SD_SEND, don't allow any more sends

      // This should be a BYTE buffer but recv is mis-prototyped.
      char buffer[ 4096 ];

      int number_of_bytes_read = 1;

#if defined( _DEBUG )

      DWORD total_number_of_bytes_read = 0;

#endif // _DEBUG

      while( number_of_bytes_read > 0 && ( number_of_bytes_read != SOCKET_ERROR ) )
      {
         // Read the buffer 4Kb at a time.
         number_of_bytes_read = m_Thunk_p->recv( m_SocketID, buffer, sizeof( buffer ), 0 );

#if defined( _DEBUG )

         if ( number_of_bytes_read > 0 )
         {
            total_number_of_bytes_read += number_of_bytes_read;
         }

#endif // _DEBUG
      }

      //WFCTRACEVAL( TEXT( "Read this many bytes before closing " ), total_number_of_bytes_read );

      m_Thunk_p->closesocket( m_SocketID );
      m_SocketID = INVALID_SOCKET;
      SetHandle( static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   }
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CSimpleSocket::Dump( CDumpContext &dump_context ) const
{
   CDummyFile::Dump( dump_context );

   dump_context << TEXT( "Address                        = \"" ) << Address    << TEXT( "\"\n" );
   dump_context << TEXT( "Name                           = \"" ) << Name       << TEXT( "\"\n" );
   dump_context << TEXT( "m_PortName                     = \"" ) << m_PortName << TEXT( "\"\n" );
   dump_context << TEXT( "m_PortNumberInNetworkByteOrder = "   ) << m_Thunk_p->ntohs( m_PortNumberInNetworkByteOrder ) << TEXT( "\n" );
   dump_context << TEXT( "m_SocketID                     = "   ) << m_SocketID       << TEXT( "\n" );
   dump_context << TEXT( "m_ReferenceCount               = "   ) << m_ReferenceCount << TEXT( "\n" );

   std::size_t loop_index        = 0;
   std::size_t number_of_aliases = AliasList.GetSize();

   dump_context << TEXT( "AliasList conains " ) << (long) number_of_aliases << TEXT( " aliases.\n" );

   std::wstring string;

   while( loop_index < number_of_aliases )
   {
      string = AliasList.GetAt( loop_index );
      dump_context << loop_index << TEXT( ". \"" ) << string << TEXT( "\"\n" );

      loop_index++;
   }
}

#endif // _DEBUG

void CSimpleSocket::GetAddress( __out std::wstring& _address ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   _address.assign( Address );
}

__checkReturn SOCKET CSimpleSocket::GetID( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   return( m_SocketID );
}

void CSimpleSocket::GetMyAddress( _Out_ std::wstring& ip_address ) noexcept
{
   ip_address.clear();

   BOOL i_started_winsock = FALSE;

   char my_name[ 4096 ];

   ZeroMemory( my_name, sizeof( my_name ) );

   DWORD error_code = 0;

   if ( m_Thunk_p == nullptr )
   {
      m_InitializeThunk();
   }

   if ( m_Thunk_p == nullptr )
   {
       return;
   }

   int return_value = m_Thunk_p->gethostname( my_name, static_cast<int>(std::size( my_name )) );

   if ( return_value != 0 )
   {
      error_code = m_Thunk_p->WSAGetLastError();
      //WFCTRACESOCKETERROR( error_code );

      if ( error_code != WSANOTINITIALISED )
      {
         return;
      }

      // Let's start sockets

      StartWindowsSockets();

      i_started_winsock = TRUE;

      // Now let's retry the gethostname

      return_value = m_Thunk_p->gethostname( my_name, static_cast<int>(std::size( my_name )) );

      if ( return_value != 0 )
      {
         // Give up
         StopWindowsSockets();
         return;
      }
   }

   // We now have our name

   LPHOSTENT host_entry_p = (LPHOSTENT) nullptr;

   host_entry_p = m_Thunk_p->gethostbyname( my_name );

   if ( host_entry_p == nullptr )
   {
      if ( i_started_winsock != FALSE )
      {
         StopWindowsSockets();
      }

      // Give up

      return;
   }

   LPSTR dotted_ip_address = nullptr;

   /*
   ** You just gotta love the way Unix people thought . . . NOT!
   */

   struct in_addr internet_address;

   internet_address.S_un.S_un_b.s_b1 = host_entry_p->h_addr_list[ 0 ][ 0 ];
   internet_address.S_un.S_un_b.s_b2 = host_entry_p->h_addr_list[ 0 ][ 1 ];
   internet_address.S_un.S_un_b.s_b3 = host_entry_p->h_addr_list[ 0 ][ 2 ];
   internet_address.S_un.S_un_b.s_b4 = host_entry_p->h_addr_list[ 0 ][ 3 ];

   dotted_ip_address = m_Thunk_p->inet_ntoa( internet_address );

   if ( dotted_ip_address == nullptr )
   {
      if ( i_started_winsock != FALSE )
      {
         StopWindowsSockets();
      }

      return;
   }

   copy( ip_address, dotted_ip_address);

   if ( i_started_winsock != FALSE )
   {
      StopWindowsSockets();
   }
}

void CSimpleSocket::GetMyAddress( __out_bcount( 4 ) uint8_t * as_bytes ) noexcept
{
   as_bytes[ 0 ] = 0;
   as_bytes[ 1 ] = 0;
   as_bytes[ 2 ] = 0;
   as_bytes[ 3 ] = 0;

   BOOL i_started_winsock = FALSE;

   char my_name[ 4096 ];

   ZeroMemory( my_name, sizeof( my_name ) );

   DWORD error_code = 0;

   if ( m_Thunk_p == nullptr )
   {
      m_InitializeThunk();
   }

   if ( m_Thunk_p == nullptr )
   {
       return;
   }

   int return_value = m_Thunk_p->gethostname( my_name, static_cast<int>(std::size( my_name )) );

   if ( return_value != 0 )
   {
      error_code = m_Thunk_p->WSAGetLastError();
      //WFCTRACESOCKETERROR( error_code );

      if ( error_code != WSANOTINITIALISED )
      {
         return;
      }

      // Let's start sockets

      StartWindowsSockets();

      i_started_winsock = TRUE;

      // Now let's retry the gethostname

      return_value = m_Thunk_p->gethostname( my_name, static_cast<int>(std::size( my_name )) );

      if ( return_value != 0 )
      {
         // Give up
         StopWindowsSockets();
         return;
      }
   }

   // We now have our name

   LPHOSTENT host_entry_p = nullptr;

   host_entry_p = m_Thunk_p->gethostbyname( my_name );

   if ( host_entry_p == nullptr )
   {
      if ( i_started_winsock != FALSE )
      {
         StopWindowsSockets();
      }

      // Give up

      return;
   }

   LPSTR dotted_ip_address = nullptr;

   /*
   ** You just gotta love the way Unix people thought . . . NOT!
   */

   as_bytes[ 0 ] = host_entry_p->h_addr_list[ 0 ][ 0 ];
   as_bytes[ 1 ] = host_entry_p->h_addr_list[ 0 ][ 1 ];
   as_bytes[ 2 ] = host_entry_p->h_addr_list[ 0 ][ 2 ];
   as_bytes[ 3 ] = host_entry_p->h_addr_list[ 0 ][ 3 ];

   if ( i_started_winsock != FALSE )
   {
      StopWindowsSockets();
   }
}

void CSimpleSocket::GetMyName( __out std::wstring& host_name ) noexcept
{
   host_name.clear();

   BOOL i_started_winsock = FALSE;

   DWORD error_code = 0;

   if ( m_Thunk_p == nullptr )
   {
      m_InitializeThunk();
   }

   if ( m_Thunk_p == nullptr )
   {
       return;
   }

   char my_name[4096];

   ZeroMemory(my_name, sizeof(my_name));

   int return_value = m_Thunk_p->gethostname( my_name, static_cast<int>(std::size( my_name )) );

   if ( return_value != 0 )
   {
      error_code = m_Thunk_p->WSAGetLastError();
      //WFCTRACESOCKETERROR( error_code );

      if ( error_code != WSANOTINITIALISED )
      {
         return;
      }

      // Let's start sockets

      StartWindowsSockets();

      i_started_winsock = TRUE;

      // Now let's retry the gethostname

      return_value = m_Thunk_p->gethostname( my_name, static_cast<int>(std::size( my_name )) );

      if ( return_value != 0 )
      {
         // Give up
         StopWindowsSockets();
         return;
      }
   }

   // We now have our name
   copy( host_name, my_name );

   if ( i_started_winsock != FALSE )
   {
      StopWindowsSockets();
   }
}

void CSimpleSocket::GetName( _Out_ std::wstring& _host_name ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   _host_name.assign( Name );
}

__checkReturn DWORD CSimpleSocket::GetOptions( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_Options );
}

void CSimpleSocket::GetPort(_Out_ short& _port_number ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   _port_number = m_Thunk_p->ntohs( m_PortNumberInNetworkByteOrder );
}

void CSimpleSocket::GetPort( _Out_ std::wstring& _port_name ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   _port_name.assign( m_PortName );
}

__checkReturn TCHAR CSimpleSocket::GetStringReadTerminatorCharacter( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   return( m_StringReadTerminatorCharacter );
}

__checkReturn BOOL CSimpleSocket::IsBroken( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_SocketID == INVALID_SOCKET )
   {
      return( TRUE );
   }

   return( FALSE );
}

__checkReturn BOOL CSimpleSocket::IsDataWaiting( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   if ( m_SocketID == INVALID_SOCKET )
   {
      return( FALSE );
   }

   int bytes_for_this_socket = 0;
   int socket_status         = 0;

   FD_SET socket_in;

   TIMEVAL time_out;

   /*
   ** Initialize the data structures
   */

   FD_ZERO( (LPFD_SET) &socket_in );

   time_out.tv_sec  = 0;
   time_out.tv_usec = 0;

   /*
   ** Set socket_in to specify that we are looking for data on socket port_id
   */

   FD_SET( m_SocketID, (LPFD_SET) &socket_in );

   /*
   ** See if data is waiting
   */

   socket_status = m_Thunk_p->select( 0, &socket_in, nullptr, nullptr, &time_out );

   if ( socket_status == SOCKET_ERROR )
   {
      m_LastError = m_Thunk_p->WSAGetLastError();
      //WFCTRACESOCKETERROR( m_LastError );
      return( FALSE );
   }

   if ( socket_status == 0 )
   {
      /*
      ** No Data is waiting on any socket in the OS
      */

      return( FALSE );
   }

   /*
   ** Welp, data is waiting for *A* socket. It may not be *our* socket. A socket in the
   ** operating system has data waiting in it. Let's see if it happens to be *our* socket.
   */

   bytes_for_this_socket = m_Thunk_p->__WSAFDIsSet( m_SocketID, &socket_in );
   
   if ( bytes_for_this_socket == 0 )
   {
      /*
      ** There is data for a socket somewhere in the system but not for our socket
      */

      return( FALSE );
   }

   return( TRUE );
}

void CSimpleSocket::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   /*
   ** Make sure everything is empty
   */

   m_PortNumberInNetworkByteOrder  = 0;
   m_SocketID                      = INVALID_SOCKET;
   SetHandle( static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
   m_StringReadTerminatorCharacter = LINE_FEED;
   m_Options                       = optionsDefault;

   Address.clear();
   Name.clear();
   m_PortName.clear();
   AliasList.clear();
}

#define WFC_LOAD_SOCKET_THUNK( dll_instance_handle, structure_pointer, function_name, thunk_function ) \
{\
   *( (FARPROC *) &structure_pointer->function_name ) = GetProcAddress( dll_instance_handle, #function_name );\
   if ( structure_pointer->function_name == nullptr ) \
{\
   *( (FARPROC *) &structure_pointer->function_name ) = (FARPROC) thunk_function;\
}\
}

static SOCKET PASCAL FAR accept_thunk( void )
{
   SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
   return( INVALID_SOCKET );
}

static int PASCAL FAR generic_thunk( void )
{
   SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
   return( SOCKET_ERROR );
}

static u_long PASCAL FAR return_u_long_zero( void )
{
   SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
   return( 0 );
}

static u_short PASCAL FAR return_u_short_zero( void )
{
   SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
   return( 0 );
}

static void * PASCAL FAR return_NULL( void )
{
   SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
   return( nullptr );
}

static unsigned long PASCAL FAR return_addr_none( void )
{
   SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
   return( INADDR_NONE );
}

static int PASCAL FAR return_not_implemented( void )
{
   SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
   return( ERROR_CALL_NOT_IMPLEMENTED );
}

static void PASCAL FAR return_nothing( void )
{
   SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
   return;
}

void CSimpleSocket::m_InitializeThunk( void ) noexcept
{
   if ( m_Thunk_p != nullptr )
   {
      //WFCTRACE( TEXT( "Thunk layer already initialized." ) );
      return;
   }

   WFC_TRY
   {
      m_Thunk_p = new WFC_SOCKET_THUNK;
   }
   WFC_CATCH_ALL
   {
      m_Thunk_p = nullptr;
   }
   WFC_END_CATCH_ALL

   if ( m_Thunk_p == nullptr )
   {
      //WFCTRACE( TEXT( "Can't allocate memory for thunk layer." ) );
      return;
   }

   ZeroMemory( m_Thunk_p, sizeof( WFC_SOCKET_THUNK ) );

   OSVERSIONINFO operating_system_version_information;

   ZeroMemory( &operating_system_version_information, sizeof( operating_system_version_information ) );
   operating_system_version_information.dwOSVersionInfoSize = sizeof( operating_system_version_information );

   if ( ::GetVersionEx( &operating_system_version_information ) != FALSE )
   {
      // Now figure out which Win32 we're running

      if ( operating_system_version_information.dwPlatformId == VER_PLATFORM_WIN32_NT )
      {
         // YIPPEE!! A real operating system!

         if ( operating_system_version_information.dwMajorVersion == 4 )
         {
            _tcsncpy_s( m_Thunk_p->dll_name, std::size( m_Thunk_p->dll_name ), TEXT( "ws2_32.dll" ), std::size( m_Thunk_p->dll_name ) );
         }
         else
         {
            _tcsncpy_s( m_Thunk_p->dll_name, std::size( m_Thunk_p->dll_name ), TEXT( "wsock32.dll" ), std::size( m_Thunk_p->dll_name ) );
         }
      }
      else if ( operating_system_version_information.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
      {
         // Piece of crap Windows95 garbage
         _tcsncpy_s( m_Thunk_p->dll_name, std::size( m_Thunk_p->dll_name ), TEXT( "wsock32.dll" ), std::size( m_Thunk_p->dll_name ) );
      }
   }

   if ( _tcslen( m_Thunk_p->dll_name ) > 0 )
   {
      m_Thunk_p->dll_instance_handle = ::LoadLibrary( m_Thunk_p->dll_name );
   }

#pragma warning(disable:4191)
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, accept, accept_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, bind, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, closesocket, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, connect, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, ioctlsocket, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, getpeername, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, getsockname, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, getsockopt, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, htonl, return_u_long_zero );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, htons, return_u_short_zero );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, inet_addr, return_addr_none );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, inet_ntoa, return_NULL );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, listen, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, ntohl, return_u_long_zero );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, ntohs, return_u_short_zero );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, recv, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, recvfrom, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, select, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, send, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, sendto, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, setsockopt, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, shutdown, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, socket, accept_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, gethostbyaddr, return_NULL );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, gethostbyname, return_NULL );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, gethostname, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, getservbyport, return_NULL );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, getservbyname, return_NULL );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, getprotobynumber, return_NULL );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, getprotobyname, return_NULL );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, WSAStartup, return_not_implemented );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, WSACleanup, generic_thunk );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, WSASetLastError, return_nothing );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, WSAGetLastError, return_not_implemented );
   WFC_LOAD_SOCKET_THUNK( m_Thunk_p->dll_instance_handle, m_Thunk_p, __WSAFDIsSet, return_u_long_zero );
#pragma warning(default:4191)
}

__checkReturn UINT CSimpleSocket::Read( __out_bcount(size_of_buffer) VOID * buffer, __in const UINT size_of_buffer ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   if ( size_of_buffer == 0 )
   {
      return( 0 );
   }

   if ( buffer == nullptr )
   {
      m_LastError = ERROR_INVALID_PARAMETER;
      return( 0 );
   }

   WFC_VALIDATE_POINTER( buffer );

   // We were passed a pointer, don't trust it . . .

   WFC_TRY
   {
      if ( m_SocketID != INVALID_SOCKET )
      {
         int number_of_bytes_read = 0;

         ::ZeroMemory( buffer, size_of_buffer );
   
         number_of_bytes_read = m_Thunk_p->recv( m_SocketID, (char *) buffer, size_of_buffer, 0 );
   
         if ( number_of_bytes_read == SOCKET_ERROR )
         {
            //WFCTRACEVAL( TEXT( "Error in recv() at line " ), __LINE__ );

            m_LastError = m_Thunk_p->WSAGetLastError();
            //WFCTRACESOCKETERROR( m_LastError );
            return( 0 );
         }
         else if ( number_of_bytes_read == 0 )
         {
            // Thanks to Angelini Fabio [fabioang@iol.it] for
            // finding a bug here. I was ignoring the ZERO case. OOPS!

            // The connection was gracefully closed.

            Close();
         }
      
         return( number_of_bytes_read );   
      }
      else
      {
         return( 0 );
      }
   }
   WFC_CATCH_ALL
   {
      return( 0 );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ bool CSimpleSocket::Read( _Out_ std::string& line_to_read ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   line_to_read.clear(); // Always begin from a known state

   if ( m_SocketID != INVALID_SOCKET )
   {
      char character = 0;

      int number_of_bytes_read = 0;

      /*
      ** Thanks go to A. Mackay (eddie@tag.co.uk) for finding
      ** a stupidity point here. I was terminating a read on
      ** a carriage return, line feed or a NULL. This broke existing
      ** code. Now the string read terminator is user settable.
      */

      do
      {
         // 2000-08-01
         // Thanks go to Tony Lam (tony@digitcom.net) for finding a loop-logic
         // bomb here. If the socket broke, the loop would last forever.

         character = 0;

         number_of_bytes_read = m_Thunk_p->recv( m_SocketID, &character, 1, 0 );

         if ( character != 0x00 )
         {
            if ( number_of_bytes_read == SOCKET_ERROR )
            {
               //WFCTRACEVAL( TEXT( "Error in recv() at line " ), __LINE__ );

               m_LastError = m_Thunk_p->WSAGetLastError();
               //WFCTRACESOCKETERROR( m_LastError );
               return( true );
            }

            line_to_read.push_back( character );
         }
      }
      while( character != m_StringReadTerminatorCharacter &&
             character != 0x00 );

      if ( number_of_bytes_read == SOCKET_ERROR )
      {
         //WFCTRACEVAL( TEXT( "Error in recv() at line " ), __LINE__ );

         m_LastError = m_Thunk_p->WSAGetLastError();
         //WFCTRACESOCKETERROR( m_LastError );
		 return( false );
      }

	  return( true );
   }
   else
   {
	   return( false );
   }
}

void CSimpleSocket::SetAddress( _In_z_ LPCTSTR address_string ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   /*
   ** Called when a client connects to us. This function also fills in the
   ** client's name for security checking.
   */

   ASSERT( address_string != nullptr );

   Name.clear();
   Address.clear();
   AliasList.clear();

   if ( address_string == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      std::wstring address( address_string );

      /*
      ** Address may contain "131.26.31.92" or "cheetah.sed.pobox.com"
      ** Given either, we must fill in host_name and IP_address
      */

      LPHOSTENT host_entry_p = nullptr;

      /*
      ** See if it is in xxx.xxx.xxx.xxx form
      */

      SIZE_T index = 0;

      BOOL exit_loop = FALSE;

      while( index < address.length() && exit_loop == FALSE )
      {
         if ( address.at( index ) != '.' && ! iswdigit( address.at( index ) ) )
         {
            /*
            ** The character is not a period and not a digit so it cannot meet the requirements
            ** of xxx.xxx.xxx.xxx type address and therefore must be a host name
            */

            exit_loop = TRUE;
         }
         else
         {
            index++;
         }
      }

      char ascii_address_string[ 4096 ];

      ZeroMemory( ascii_address_string, sizeof( ascii_address_string ) );

#if defined( UNICODE )

      copy_to( address, ascii_address_string, std::size(ascii_address_string));

#else

      strcpy_s( ascii_address_string, std::size( ascii_address_string ), address );

#endif

      if ( exit_loop != FALSE ) // exit_loop is FALSE when we do not have a dotted IP address
      {
         if ( ! ( m_Options & optionsDontResolveHostName ) )
         {
            host_entry_p = m_Thunk_p->gethostbyname( ascii_address_string );
         }
         else
         {
            host_entry_p = nullptr;
         }
      }
      else
      {
         /*
         ** The string was an IP address in the form of "xxx.xxx.xxx.xxx"
         */

#if ! defined( UNICODE )
         Name    = ascii_address_string;
         Address = ascii_address_string;
#else
         {
            WCHAR unicode_address_string[ 512 ];

            ::ZeroMemory( unicode_address_string, sizeof( unicode_address_string ) );

            ::ASCII_to_UNICODE( ascii_address_string, unicode_address_string );

            Name.assign( unicode_address_string );
            Address.assign( unicode_address_string );
         }
#endif

         if ( ! ( m_Options & optionsDontResolveHostName ) )
         {
            ULONG internet_address = 0L;

            internet_address = m_Thunk_p->inet_addr( ascii_address_string );
            host_entry_p = m_Thunk_p->gethostbyaddr( (const char *) &internet_address, 4, PF_INET );
         }
      }

      if ( host_entry_p == nullptr )
      {
         //WFCTRACEVAL( TEXT( "gethostbyxxx() failed at line " ), __LINE__ );

         m_LastError = m_Thunk_p->WSAGetLastError();
         //WFCTRACESOCKETERROR( m_LastError );
         return;
      }

      if ( exit_loop == FALSE )
      {
         Address.assign( address );
      }
      else
      {
         LPSTR dotted_ip_address = nullptr;

         /*
         ** You just gotta love the way Unix people thought . . . NOT!
         */

         struct in_addr internet_address;

         internet_address.S_un.S_un_b.s_b1 = host_entry_p->h_addr_list[ 0 ][ 0 ];
         internet_address.S_un.S_un_b.s_b2 = host_entry_p->h_addr_list[ 0 ][ 1 ];
         internet_address.S_un.S_un_b.s_b3 = host_entry_p->h_addr_list[ 0 ][ 2 ];
         internet_address.S_un.S_un_b.s_b4 = host_entry_p->h_addr_list[ 0 ][ 3 ];

         dotted_ip_address = m_Thunk_p->inet_ntoa( internet_address );

         if ( dotted_ip_address == nullptr )
         {
            return;
         }

         Address.clear();

         index = 0;

         while( dotted_ip_address[ index ] != 0x00 )
         {
            Address.push_back( dotted_ip_address[ index ] );
            index++;
         }

         //WFCTRACEVAL( TEXT( "ip_address == " ), Address );
      }

      /*
      ** We don't call set_name() because that function will call this function and we'll go into an endless loop
      */

      copy( Name, host_entry_p->h_name );

      /*
      ** Now lets get the aliases for this fella
      */

      AliasList.clear();

      index = 0;

      std::wstring new_string;

      int index_2 = 0;

      while( host_entry_p->h_aliases[ index ] != nullptr )
      {
         index_2 = 0;
         new_string.clear();

         while( host_entry_p->h_aliases[ index ][ index_2 ] != 0x00 )
         {
            new_string.push_back( host_entry_p->h_aliases[ index ][ index_2 ] );
            index_2++;
         }

         (void) AliasList.push_back( new_string );
         index++;
      }
   }
   WFC_CATCH_ALL
   {
      ; // Do Nothing
   }
   WFC_END_CATCH_ALL
}

void CSimpleSocket::SetID( __in const SOCKET id ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_SocketID != INVALID_SOCKET )
   {
      Close();
   }

   m_SocketID = id;
   SetHandle( (HANDLE) id );
}

void CSimpleSocket::SetName( __in_z LPCTSTR host_string ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( host_string );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( host_string == nullptr )
      {
         Name.clear();
      }
      else
      {
         Name.assign( host_string );
      }
   }
   WFC_CATCH_ALL
   {
      Name.clear();
   }
   WFC_END_CATCH_ALL
}

void CSimpleSocket::SetOptions( __in DWORD const new_options ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_Options = new_options;
}

void CSimpleSocket::SetPort( __in_z LPCTSTR name_string ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( name_string );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   if ( name_string == nullptr )
   {
      m_PortName.clear();
      m_PortNumberInNetworkByteOrder = 0;
      return;
   }

   std::wstring name;

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // Let's see if the user passed us a number as a string

      int number_value = _ttoi( name_string );

      if ( number_value != 0 )
      {
         // Well whaddya know, they sent us a number disguised as a string

         SetPort( (short) number_value );
         return;
      }

      name.assign( name_string );

      /*
      ** Although not documented anywhere, the name of the protocol must be in lower case
      ** If you look in the data file for getservbyname() [winnt/system32/drivers/etc/services]
      ** you will notice that everything is in lower case. Gotta love Unix . . .
      */

      make_lower(name);

      /*
      ** This routine sets port_name and m_PortNumberInNetworkByteOrder
      */

      LPSERVENT service_entry_p = nullptr;

#if defined( UNICODE )

      char ascii_name_string[ 1024 ];

      ::ZeroMemory( ascii_name_string, sizeof( ascii_name_string ) );

      copy_to( name, ascii_name_string, sizeof(ascii_name_string) );

      service_entry_p = m_Thunk_p->getservbyname( ascii_name_string, nullptr );

#else

      service_entry_p = m_Thunk_p->getservbyname( name, nullptr );

#endif

      if ( service_entry_p == nullptr )
      {
         m_LastError = m_Thunk_p->WSAGetLastError();

         //WFCTRACEVAL( TEXT( "getservbyname() failed at line " ), __LINE__ );
         //WFCTRACESOCKETERROR( m_LastError );

         return;
      }

      m_PortName.assign( name );
      m_PortNumberInNetworkByteOrder = service_entry_p->s_port;
   }
   WFC_CATCH_ALL
   {
      return;
   }
   WFC_END_CATCH_ALL
}

void CSimpleSocket::SetPort( __in short const p ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   /*
   ** This routine sets port_name and m_PortNumberInNetworkByteOrder
   */

   m_PortNumberInNetworkByteOrder = m_Thunk_p->htons( p );

   /*
   ** Now go find a name for this port . . .
   */

   if ( m_Options & optionsDontResolvePortNumber )
   {
      // The user doesn't care what the friendly name of the port is,
      // they are concerned with speed.
      format( m_PortName, L"Port #%d", (int) p );
   }
   else
   {
      LPSERVENT service_entry_p = nullptr;

      service_entry_p = m_Thunk_p->getservbyport( m_PortNumberInNetworkByteOrder, nullptr );

      if ( service_entry_p == nullptr )
      {
         //WFCTRACEVAL( TEXT( "getservbyport() failed at line " ), __LINE__ );

         m_LastError = m_Thunk_p->WSAGetLastError();
         //WFCTRACESOCKETERROR( m_LastError );

         format( m_PortName, L"Port #%d", (int) p );
         return;
      }

      copy( m_PortName, service_entry_p->s_name );
   }
}

__checkReturn BOOL CSimpleSocket::SetReadTimeout( __in DWORD const number_of_milliseconds ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( m_SocketID != INVALID_SOCKET );

   return( SetSocketOption( SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char const *>(&number_of_milliseconds), sizeof( number_of_milliseconds ) ) );
}

__checkReturn BOOL CSimpleSocket::SetSocketOption( __in int const level, __in int const option_name, __in_z char const * option_value, __in int const option_length ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   if ( m_Thunk_p == nullptr )
   {
      //WFCTRACE( TEXT( "Thunk not yet initialized." ) );
      return( FALSE );
   }

   // One popular mistake with using setsockopt is to use BOOL types like the
   // documentation seems to say. What they really mean is "address of a BOOL"

   ASSERT( option_value != (const char *) 0 );
   ASSERT( option_value != (const char *) 1 );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( m_Thunk_p->setsockopt( m_SocketID, level, option_name, option_value, option_length ) == SOCKET_ERROR )
      {
         m_LastError = m_Thunk_p->WSAGetLastError();
         //WFCTRACESOCKETERROR( m_LastError );
         return( FALSE );
      }
   }
   WFC_CATCH_ALL
   {
      m_LastError = ERROR_EXCEPTION_IN_SERVICE;
      return( FALSE );
   }
   WFC_END_CATCH_ALL

   return( TRUE );
}

void CSimpleSocket::SetStringReadTerminatorCharacter( __in TCHAR const terminator ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_StringReadTerminatorCharacter = terminator;
}

__checkReturn BOOL CSimpleSocket::SetWriteTimeout( __in DWORD const number_of_milliseconds ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( m_SocketID != INVALID_SOCKET );

   return( SetSocketOption( SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char const *>(&number_of_milliseconds), sizeof( number_of_milliseconds ) ) );
}

void __stdcall CSimpleSocket::StartWindowsSockets( void ) noexcept
{
   m_InitializeThunk();

   /*
   ** Start WINSOCK
   */

   WSADATA winsock_data;

   int socket_error = 0;

   WORD desired_winsock_version = 0x0101; // We'd like WINSOCK v1.1 at least

   BYTE major_version_required = 0;
   BYTE minor_version_required = 0;

   ::ZeroMemory( &winsock_data, sizeof( winsock_data ) );

   socket_error = m_Thunk_p->WSAStartup( desired_winsock_version, (LPWSADATA) &winsock_data );

   if ( socket_error != 0 )
   {
      //WFCTRACE( TEXT( "WSAStartup failed with an error code of %d at line %d of %s\n" ), socket_error, __LINE__, __FILE__ );
      return;
   }

   major_version_required = HIBYTE( desired_winsock_version );
   minor_version_required = LOBYTE( desired_winsock_version );

   if ( (   LOBYTE( winsock_data.wVersion ) <  major_version_required ) ||
        (   LOBYTE( winsock_data.wVersion ) == major_version_required ) &&
        ( ( HIBYTE( winsock_data.wVersion ) <  minor_version_required ) ) )
   {
      //WFCTRACE( TEXT( "Need a later version of Winsock" ) );
   }
}

void __stdcall CSimpleSocket::StopWindowsSockets( void ) noexcept
{  
   if ( m_Thunk_p != nullptr )
   {
      m_Thunk_p->WSACleanup();

      if ( m_Thunk_p->dll_instance_handle != static_cast< HINSTANCE >( NULL ) )
      {
         ::FreeLibrary( m_Thunk_p->dll_instance_handle );
      }

      ZeroMemory( m_Thunk_p, sizeof( WFC_SOCKET_THUNK ) );
      delete m_Thunk_p;
      m_Thunk_p = nullptr;
   }
}

void __stdcall CSimpleSocket::TranslateErrorCode( __in DWORD const error_code, __out_z LPTSTR destination_string, __in DWORD const size_of_destination_string ) noexcept
{
   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      switch( error_code )
      {
         /*
         ** Following are Windows Sockets Library errors
         */

         case WSAENOTSOCK:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAENOTSOCK, Socket operation on non-socket. A socket created in one process is used by another process." ), size_of_destination_string );
           return;

         case WSAEDESTADDRREQ:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEDESTADDRREQ, Destination address required." ), size_of_destination_string );
           return;

         case WSAEMSGSIZE:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEMSGSIZE, Message too long." ), size_of_destination_string );
           return;

         case WSAEPROTOTYPE:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEPROTOTYPE, Protocol wrong type for socket." ), size_of_destination_string );
           return;

         case WSAENOPROTOOPT:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAENOPROTOOPT, Protocol not available." ), size_of_destination_string );
           return;

         case WSAEPROTONOSUPPORT:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEPROTONOSUPPORT, Protocol not supported." ), size_of_destination_string );
           return;

         case WSAESOCKTNOSUPPORT:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAESOCKTNOSUPPORT, Socket type not supported." ), size_of_destination_string );
           return;

         case WSAEOPNOTSUPP:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEOPNOTSUPP, Operation not supported on socket." ), size_of_destination_string );
           return;

         case WSAEPFNOSUPPORT:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEPFNOSUPPORT, Protocol family not supported." ), size_of_destination_string );
           return;

         case WSAEAFNOSUPPORT:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSEAFNOSUPPORT, Address family not supported by protocol family." ), size_of_destination_string );
           return;

         case WSAEADDRINUSE:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEADDRINUSE, Triggered by bind() because a process went down without closing a socket." ), size_of_destination_string );
           return;

         case WSAEADDRNOTAVAIL:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEADDRNOTAVAIL, Can't assign requested address." ), size_of_destination_string );
           return;

         case WSAENETDOWN:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAENETDOWN, Network is down." ), size_of_destination_string );
           return;

         case WSAENETUNREACH:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAENETUNREACH, Network is unreachable." ), size_of_destination_string );
           return;

         case WSAENETRESET:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAENETRESET, Network dropped connection or reset." ), size_of_destination_string );
           return;

         case WSAECONNABORTED:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAECONNABORTED, Software caused connection abort." ), size_of_destination_string );
           return;

         case WSAECONNRESET:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAECONNRESET, Connection reset by peer." ), size_of_destination_string );
           return;

         case WSAENOBUFS:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAENOBUFS, No buffer space available." ), size_of_destination_string );
           return;

         case WSAEISCONN:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEISCONN, Socket is already connected." ), size_of_destination_string );
           return;

         case WSAENOTCONN:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAENOTCONN, Socket is not connected." ), size_of_destination_string );
           return;

         case WSAESHUTDOWN:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAESHUTDOWN, Can't send after socket shutdown." ), size_of_destination_string );
           return;

         case WSAETIMEDOUT:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAETIMEDOUT, Connection timed out." ), size_of_destination_string );
           return;

         case WSAECONNREFUSED:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAECONNREFUSED, Connection refused." ), size_of_destination_string );
           return;

         case WSAEHOSTDOWN:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEHOSTDOWN, Networking subsystem not started." ), size_of_destination_string );
           return;

         case WSAEHOSTUNREACH:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEHOSTUNREACH, No route to host." ), size_of_destination_string );
           return;

         case WSAEWOULDBLOCK:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEWOULDBLOCK, Operation would block." ), size_of_destination_string );
           return;

         case WSAEINPROGRESS:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEINPROGRESS, Operation now in progress." ), size_of_destination_string );
           return;

         case WSAEALREADY:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEALREADY, Operation already in progress." ), size_of_destination_string );
           return;

         case WSAEINTR:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEALREADY, Operation was interrupted." ), size_of_destination_string );
           return;

         case WSAEBADF:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEBADF, Bad file number." ), size_of_destination_string );
           return;

         case WSAEACCES:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEACCES, Access is denied." ), size_of_destination_string );
           return;

         case WSAEFAULT:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEFAULT, Bad memory address." ), size_of_destination_string );
           return;

         case WSAEINVAL:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEINVAL, The socket has not been bound with bind() or is already connected." ), size_of_destination_string );
           return;

         case WSAEMFILE:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEMFILE, No more file descriptors are available." ), size_of_destination_string );
           return;

         case WSAETOOMANYREFS:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAETOOMANYREFS, Undocumented WinSock error." ), size_of_destination_string );
           return;

         case WSAENAMETOOLONG:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAENAMETOOLONG, Undocumented WinSock error." ), size_of_destination_string );
           return;

         case WSAENOTEMPTY:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAENOTEMPTY, Undocumented WinSock error." ), size_of_destination_string );
           return;

         case WSAEPROCLIM:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEPROCLIM, Undocumented WinSock error." ), size_of_destination_string );
           return;

         case WSAEUSERS:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEUSERS, Undocumented WinSock error." ), size_of_destination_string );
           return;

         case WSAEDQUOT:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEDQUOT, Undocumented WinSock error." ), size_of_destination_string );
           return;

         case WSAESTALE:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAESTALE, Undocumented WinSock error." ), size_of_destination_string );
           return;

         case WSAEREMOTE:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEREMOTE, Undocumented WinSock error." ), size_of_destination_string );
           return;

         case WSAEDISCON:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAEDISCON, Circuit was gracefully terminated." ), size_of_destination_string );
           return;

         case WSASYSNOTREADY:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSASYSNOTREADY, The underlying network subsystem is not ready for network communication." ), size_of_destination_string );
           return;

         case WSAVERNOTSUPPORTED:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAVERNOTSUPPORTED, The version of Windows Sockets API support requested is not provided by this particular Windows Sockets implementation." ), size_of_destination_string );
           return;

         case WSANOTINITIALISED:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSANOTINITIALISED, WSAStartup() has not been called." ), size_of_destination_string );
           return;

         case WSAHOST_NOT_FOUND:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSAHOST_NOT_FOUND, Authoritative answer host not found." ), size_of_destination_string );
           return;

         case WSATRY_AGAIN:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSATRY_AGAIN, Non-authoritative answer host not found or SERVERFAIL." ), size_of_destination_string );
           return;

         case WSANO_RECOVERY:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSANO_RECOVERY, Non recoverable errors, FORMERR, REFUSED, NOTIMP." ), size_of_destination_string );
           return;

         case WSANO_DATA:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSANO_DATA or WSANO_ADDRESS, Valid name, no data record of requested type." ), size_of_destination_string );
           return;

         case WSABASEERR + 1005:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSA_QOS_RECEIVERS, at least one Reserve has arrived." ), size_of_destination_string );
           return;

         case WSABASEERR + 1006:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSA_QOS_SENDERS, at least one Path has arrived." ), size_of_destination_string );
           return;

         case WSABASEERR + 1007:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSA_QOS_NO_SENDERS, there are no senders." ), size_of_destination_string );
           return;

         case WSABASEERR + 1008:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSA_QOS_NO_RECEIVERS, there are no receivers." ), size_of_destination_string );
           return;

         case WSABASEERR + 1009:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSA_QOS_REQUEST_CONFIRMED, Reserve has been confirmed." ), size_of_destination_string );
           return;

         case WSABASEERR + 1010:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSA_QOS_ADMISSION_FAILURE, error due to lack of resources." ), size_of_destination_string );
           return;

         case WSABASEERR + 1011:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSA_QOS_POLICY_FAILURE, rejected for administrative reasons - bad credentials." ), size_of_destination_string );
           return;

         case WSABASEERR + 1012:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSA_QOS_BAD_STYLE, unknown or conflicting style." ), size_of_destination_string );
           return;

         case WSABASEERR + 1013:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSA_QOS_BAD_OBJECT, problem with some part of the filterspec or provider specific buffer in general." ), size_of_destination_string );
           return;

         case WSABASEERR + 1014:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSA_QOS_TRAFFIC_CTRL_ERROR, problem with some part of the flowspec." ), size_of_destination_string );
           return;

         case WSABASEERR + 1015:

           _tcsncpy_s( destination_string, size_of_destination_string, TEXT( "WSA_QOS_GENERIC_ERROR, general error." ), size_of_destination_string );
           return;

         default:

            {
               TCHAR message_string[ 129 ];

               if ( error_code >= WSABASEERR )
               {
                  _stprintf_s( (LPTSTR) message_string, std::size( message_string ), (LPCTSTR) TEXT( "Unknown WinSock Error Number %d (WSABASEERR + %d)" ), (int) error_code, (int)( error_code - WSABASEERR) );
               }
               else
               {
                  _stprintf_s( (LPTSTR) message_string, std::size( message_string ), (LPCTSTR) TEXT( "Unknown WinSock Error Number %d" ), (int) error_code );
               }

               _tcsncpy_s( destination_string, size_of_destination_string, message_string, size_of_destination_string );
            }

            return;
      }
   }
   WFC_CATCH_ALL
   {
      return;
   }
   WFC_END_CATCH_ALL
}

#if defined( TF_DISCONNECT )

__checkReturn BOOL CSimpleSocket::Transmit( __inout CFile64& file_to_send, __in DWORD const flags, __in_opt std::vector<uint8_t> * send_before, __in_opt std::vector<uint8_t> * send_after ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( send_before );
   WFC_VALIDATE_POINTER_NULL_OK( send_after );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      TRANSMIT_FILE_BUFFERS transmit_file_buffers;

      ::ZeroMemory( &transmit_file_buffers, sizeof( transmit_file_buffers ) );

      TRANSMIT_FILE_BUFFERS *buffers_p = nullptr;

      if ( send_before != nullptr )
      {
         buffers_p                        = &transmit_file_buffers;
         transmit_file_buffers.Head       = (PVOID) send_before->data();
         transmit_file_buffers.HeadLength = (DWORD) send_before->size();
      }

      if ( send_after != nullptr )
      {
         buffers_p                        = &transmit_file_buffers;
         transmit_file_buffers.Tail       = (PVOID) send_after->data();
         transmit_file_buffers.TailLength = (DWORD) send_after->size();
      }

      BOOL return_value = m_Thunk_p->TransmitFile( m_SocketID, 
                                                   file_to_send.GetHandle(),
                                                   0,
                                                   0,
                                                   nullptr,
                                                   buffers_p,
                                                   flags );

      if ( return_value == FALSE )
      {
         m_LastError = ::GetLastError();
         //WFCTRACEERROR( m_LastError );
      }
      else
      {
         return_value = TRUE;
      }

      return( return_value );
   }
   WFC_CATCH_ALL
   {
      return( FALSE );
   }
   WFC_END_CATCH_ALL
}

#endif // TF_DISCONNECT

void CSimpleSocket::Write( __in_bcount( number_of_bytes_to_write ) const VOID * buffer, __in const UINT number_of_bytes_to_write ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( buffer );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      if ( buffer == nullptr )
      {
         m_LastError = ERROR_INVALID_PARAMETER;
         return;
      }

      if ( m_SocketID == INVALID_SOCKET )
      {
         m_LastError = ERROR_INVALID_HANDLE;
         return;
      }

      BYTE *byte_buffer = nullptr;

      byte_buffer = (BYTE *) buffer;

      if ( byte_buffer == nullptr )
      {
         m_LastError = ERROR_INVALID_ADDRESS;
         return;
      }

      /*
      ** Loop until the bytes are sent or until we give up
      */

      BOOL bytes_were_sent = FALSE;

      int number_of_bytes_sent = 0;
      int loop_count           = 0;

      while( bytes_were_sent == FALSE && loop_count < 100 )
      {
         number_of_bytes_sent = m_Thunk_p->send( m_SocketID, (const char *) byte_buffer, number_of_bytes_to_write, 0 );

         if ( number_of_bytes_sent == SOCKET_ERROR )
         {
            m_LastError = m_Thunk_p->WSAGetLastError();
            //WFCTRACESOCKETERROR( m_LastError );

            if ( m_LastError != WSAENOBUFS && m_LastError != WSAEINPROGRESS )
            {
               if ( m_LastError == WSAENOTCONN  ||
                    m_LastError == WSAENETRESET ||
                    m_LastError == WSAESHUTDOWN )
               {
                  /*
                  ** Someone hung up on us or unplugged our lan cable
                  */

                  m_SocketID = INVALID_SOCKET;
                  SetHandle( static_cast< HANDLE >( INVALID_HANDLE_VALUE ) );
               }

               return;
            }
            else
            {
               loop_count++;
            }
         }
         else
         {
            bytes_were_sent = TRUE;
         }
      }
   }
   WFC_CATCH_ALL
   {
      return;
   }
   WFC_END_CATCH_ALL
}

void CSimpleSocket::Write( __in const std::string& string_to_write ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( m_Thunk_p );

   std::string temp_string( string_to_write );

   // Thanks go to zschen (zschen@ms.chttl.com.tw) for finding a bug here.
   // I wasn't sending the trailing NULL to the socket. DOH!

   Write( temp_string.c_str(), static_cast<UINT>(temp_string.length() + 1) );
}

#pragma warning( default : 4100 )

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CSimpleSocket</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that encapsulates TCP/IP socket I/O.">
</HEAD>

<BODY>

<H1>CSimpleSocket : <A HREF="DumyFile.htm">CDummyFile</A></H1>
$Revision: 60 $
<HR>

<H2>Description</H2>

This class is inherited from <B><A HREF="DumyFile.htm">CDummyFile</A></B>. When I first saw the socket API, it seemed to be
the fruit of a weird, twisted mind. Then I remembered sockets came from the Unix world. No one ever accused of Unix being 
straightforward. With this in mind I wrote <B>CSimpleSocket</B> to try to bring some sanity to sockets. There are two parts
to a socket. The IP address and a port number.

<H2>Data Members</H2>

<B>Address</B> - The &quotdotted&quot IP address. Example: &quot20.2.1.3&quot<P>
<B>Name</B> - The name of the machine. This will be empty if the address has no name.<P>
<B>AliasList</B> - This contains the alternate names for the IP address. Some machines have
multiple names.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the socket.

<DT><PRE>void <B><A NAME="GetAddress">GetAddress</A></B>( std::wstring&amp; dotted_ip_address_string ) const</PRE><DD>
Retrieves the <B>Address</B> data member.

<DT><PRE>SOCKET <B><A NAME="GetID">GetID</A></B>( void ) const</PRE><DD>
Retrieves the Socket ID so you can call the socket API directly.

<DT><PRE>void <B><A NAME="GetMyAddress">GetMyAddress</A></B>( std::wstring&amp; dotted_ip_address_string )</PRE><DD>
Retrieves your IP address.

<DT><PRE>void <B><A NAME="GetMyName">GetMyName</A></B>( std::wstring&amp; machine_name )</PRE><DD>
Retrieves your IP host name.

<DT><PRE>void <B><A NAME="GetName">GetName</A></B>( std::wstring&amp; host_name ) const</PRE><DD>
Retrieves the <B>Name</B> data member.

<DT><PRE>DWORD <B><A NAME="GetOptions">GetOptions</A></B>( void ) const</PRE><DD>
Returns the options for this socket. See <A HREF="#SetOptions"><B>SetOptions</B></A>()
for a list of valid options bits. Combine the options together with the OR operator.

<DT><PRE>void <B><A NAME="GetPort">GetPort</A></B>( short&amp; port_number ) const
void <B>GetPort</B>( std::wstring&amp; port_name ) const</PRE><DD>
Retrieves the port number or port name of this socket. For example,
if you have a telnet socket (i.e. port number 23), <B>GetPort</B> will
return 23 in <CODE>port_number</CODE> and &quot;telnet&quot;
in <CODE>port_name</CODE>. If no name can be found for the port number,
<CODE>port_name</CODE> will be filled with a string representation of
the port number.

<DT><PRE>TCHAR <B><A NAME="GetStringReadTerminatorCharacter">GetStringReadTerminatorCharacter</A></B>( void ) const</PRE><DD>
Returns the character that will cause a <A HREF="#Read"><B>Read</B></A>() to terminate when
reading a <B>std::wstring</B>.

<DT><PRE>BOOL <B><A NAME="IsBroken">IsBroken</A></B>( void ) const</PRE><DD>
Returns TRUE if the socket is broken.

<DT><PRE>BOOL <B><A NAME="IsDataWaiting">IsDataWaiting</A></B>( void )</PRE><DD>
Returns TRUE if there are bytes waiting to be <A HREF="#Read"><B>Read</B></A>().

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( void ) = 0
BOOL <B>Open</B>( LPCTSTR machine_name_or_address, UINT port_number = 23, CFileException * = nullptr ) = 0</PRE><DD>
Pure virtual function that opens the socket. You have to implement these functions.

<DT><PRE>void <B><A NAME="Read">Read</A></B>( std::wstring&amp; line_to_read )
UINT <B>Read</B>( VOID * buffer, UINT maximum_number_of_bytes_to_read )</PRE><DD>
Pulls data out of the socket. If you are reading a string, the character
set by <A HREF="#SetStringReadTerminatorCharacter"><B>SetStringReadTerminatorCharacter</B></A>()
will terminate the read. A NULL (character zero) will always terminate
a string read. This function will block the execution of the thread until the
other side of the socket writes something. You can cause the read to return after
a time period has expired and no data has arrived, see
<A HREF="#SetReadTimeout">SetReadTimeout()</A>.

<DT><PRE>void <B><A NAME="SetAddress">SetAddress</A></B>( LPCTSTR address )</PRE><DD>
This sets the <B>Address</B>, <B>Name</B> and <B>AliasList</B>
data members. You use either a &quot;dotted&quot; IP address or a machine name, <B>SetAddress</B> will
figure out what you mean.

<DT><PRE>void <B><A NAME="SetID">SetID</A></B>( const SOCKET socket_id )</PRE><DD>
Use this function if you've created a socket using the native socket
API and want to play with it via <B>CSimpleSocket</B>. It does not set 
any of the address/name members. If the class is allready being used to play
with an existing socket, it will be closed before the <CODE>socket_id</CODE>
is attached.

<DT><PRE>void <B><A NAME="SetName">SetName</A></B>( LPCTSTR machine_name )</PRE><DD>
Simply sets the <B>Name</B> member.

<DT><PRE>void <B><A NAME="SetOptions">SetOptions</A></B>( DWORD options )</PRE><DD>
Simply sets the options for the socket. This is useful only before
opening the socket. These are the valid options:

<UL>

<LI><B><CODE><A NAME="optionsDefault">optionsDefault</A></CODE></B> - Does nothing.

<LI><B><CODE><A NAME="optionsDontResolveHostName">optionsDontResolveHostName</A></CODE></B> - When this option is set and
<A HREF="#Open"><B>Open</B></A>() is called with an IP address, it will not
be resolved to a machine name.

<LI><B><CODE><A NAME="optionsDontResolvePortNumber">optionsDontResolvePortNumber</A></CODE></B> - When this option is set and
<A HREF="#SetPort"><B>SetPort</B></A>() is called with a port number, it will not
be resolved to a port name.

</UL>

<DT><PRE>void <B><A NAME="SetPort">SetPort</A></B>( const short port_number )
void <B>SetPort</B>( const char * port_name )</PRE><DD>
Sets the port number and name.

<DT><PRE>void <B><A NAME="SetReadTimeout">SetReadTimeout</A></B>( DWORD number_of_milliseconds )</PRE><DD>
Sets the number of milliseconds before a <A HREF="#Read">Read()</A> call will
timeout. This is how you solve the problem of both sides of a socket connection
reading from the socket at the same time. Since neither one is writing to the
socket, the read will never return, resulting in a deadlock.

<DT><PRE>void <B><A NAME="SetStringReadTerminatorCharacter">SetStringReadTerminatorCharacter</A></B>( TCHAR character )</PRE><DD>
Sets the character that will terminate a string <B>Read</B>().
A NULL (character zero) will always terminate a string read.

<DT><PRE>void <B><A NAME="SetWriteTimeout">SetWriteTimeout</A></B>( DWORD number_of_milliseconds )</PRE><DD>
Sets the number of milliseconds before a <A HREF="#Write">Write()</A> call will
timeout.

<DT><PRE>void <B><A NAME="StartWindowsSockets">StartWindowsSockets</A></B>( void )</PRE><DD>
Starts sockets.
It is not recommended that you call this function. <B>CSimpleSocket</B>
will call it as needed.

<DT><PRE>void <B><A NAME="StopWindowsSockets">StopWindowsSockets</A></B>( void )</PRE><DD>
Stops sockets.
It is not recommended that you call this function. <B>CSimpleSocket</B>
will call it as needed.

<DT><PRE>void <B><A NAME="TranslateErrorCode">TranslateErrorCode</A></B>( DWORD error_code, LPSTR destination_string, DWORD size_of_string )</PRE><DD>
Did you ever wonder what 10014 means? This function translates a socket
error code into something humanly readable.

<DT><PRE>BOOL <B><A NAME="Transmit">Transmit</A></B>( CFile&amp; file_to_send, DWORD flags = TF_DISCONNECT, std::vector&lt;uint8_t&gt; * send_before = nullptr, std::vector&lt;uint8_t&gt; * send_after )</PRE><DD>
Will transmit the <CODE>file_to_send</CODE> down the socket. If you want to
send some bytes before the file, set <CODE>send_before</CODE> to the address
of a std::vector&lt;uint8_t&gt;. If you want some bytes sent after the file, set <CODE>send_after</CODE>
to the address of a std::vector&lt;uint8_t&gt;. Read up on the TransmitFile() API for a
discussion of the <CODE>flags</CODE> parameter.

<DT><PRE>void <B><A NAME="Write">Write</A></B>( const std::wstring&amp; string_to_write )
void <B>Write</B>( const VOID * buffer, const long number_of_bytes_to_write )</PRE><DD>
Sends data out of the socket.

</DL>

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

int close_the_socket( <B>CSimpleSocket</B>&amp; the_socket )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;close_the_socket()&quot; ) );

   DWORD socket_options = the_socket.<A HREF="#GetOptions">GetOptions</A>();

   if ( socket_options &amp; <A HREF="#optionsDontResolveHostName">optionsDontResolveHostName</A> )
   {
      WFCTRACE( TEXT( &quot;Host name was not resolved.&quot; ) );
   }

   if ( socket_options &amp; <A HREF="#optionsDontResolvePortNumber">optionsDontResolvePortNumber</A> )
   {
      WFCTRACE( TEXT( &quot;Port number was not resolved.&quot; ) );
   }

   the_socket.<A HREF=#Close">Close</A>();

   // OK, this is a really lame sample but the class
   // is pure virtual!
}</CODE></PRE>
<H2>API's Used</H2>

<B>CSimpleSocket</B> uses the following API's:
<UL>
<LI>closesocket
<LI>CreateMutex
<LI>FD_ISSET
<LI>FD_SET
<LI>FD_ZERO
<LI>FreeLibrary
<LI>gethostbyname
<LI>gethostbyaddr
<LI>gethostname
<LI>GetLastError
<LI>GetProcAddress
<LI>getservbyname
<LI>getservbyport
<LI>GetVersionEx
<LI>htons
<LI>inet_ntoa
<LI>InitializeSecurityDescriptor
<LI>IsValidSecurityDescriptor
<LI>LoadLibrary
<LI>LocalAlloc
<LI>LocalFree
<LI>ntohs
<LI>recv
<LI>ReleaseMutex
<LI>select
<LI>send
<LI>SetSecurityDescriptorDacl
<LI>TransmitFile
<LI>WaitForSingleObject
<LI>WSACleanup
<LI>WSAStartup
<LI>ZeroMemory
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: sockets.cpp $<BR>
$Modtime: 6/26/01 10:56a $
</BODY>

</HTML>
The following line should go in AUTOEXP.DAT so the debugging tooltips will format properly
ToolTipFormatLine=CSimpleSocket=hFile=<m_hFile> name=<m_strFileName.m_pchData,s>
*/
