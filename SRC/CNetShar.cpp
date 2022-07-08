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
** $Workfile: cnetshar.cpp $
** $Revision: 31 $
** $Modtime: 6/26/01 10:49a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#endif // _DEBUG

/*
** CNetworkShareInformation stuff
*/

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CNetworkShareInformation::CNetworkShareInformation() noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CNetworkShareInformation::CNetworkShareInformation( _In_ SHARE_INFO_1 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkShareInformation::CNetworkShareInformation( _In_ SHARE_INFO_2 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

Win32FoundationClasses::CNetworkShareInformation::CNetworkShareInformation( _In_ Win32FoundationClasses::CNetworkShareInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

void Win32FoundationClasses::CNetworkShareInformation::Copy( _In_ SHARE_INFO_1 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   m_Initialize();

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      NetworkName.assign( source->shi1_netname );
      Remark.assign( source->shi1_remark );

      Type = source->shi1_type;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkShareInformation::Copy( _In_ SHARE_INFO_2 const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      m_Initialize();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      NetworkName.assign( source->shi2_netname == nullptr ? L"" : source->shi2_netname);
      Remark.assign( source->shi2_remark == nullptr ? L"" : source->shi2_remark );
      PathName.assign( source->shi2_path == nullptr ? L"" : source->shi2_path);
      Password.assign( source->shi2_passwd == nullptr ? L"" : source->shi2_passwd);

      Type                = source->shi2_type;
      Permissions         = source->shi2_permissions;
      MaximumNumberOfUses = source->shi2_max_uses;
      CurrentNumberOfUses = source->shi2_current_uses;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

void Win32FoundationClasses::CNetworkShareInformation::Copy( _In_ Win32FoundationClasses::CNetworkShareInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this not_eq &source );

   /*
   ** Make sure we ain't copying ourselves
   */

   if ( this == &source )
   {
      return;
   }

   NetworkName = source.NetworkName;
   Type                = source.Type;
   Remark = source.Remark;
   Permissions         = source.Permissions;
   MaximumNumberOfUses = source.MaximumNumberOfUses;
   CurrentNumberOfUses = source.CurrentNumberOfUses;
   PathName = source.PathName;
   Password = source.Password;
}

void Win32FoundationClasses::CNetworkShareInformation::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

void Win32FoundationClasses::CNetworkShareInformation::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   NetworkName.clear();
   Type                = 0;
   Remark.clear();
   Permissions         = 0;
   MaximumNumberOfUses = 0;
   CurrentNumberOfUses = 0;
   PathName.clear();
   Password.clear();
}

Win32FoundationClasses::CNetworkShareInformation const& Win32FoundationClasses::CNetworkShareInformation::operator = ( _In_ Win32FoundationClasses::CNetworkShareInformation const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   ASSERT( this not_eq &source );

   if ( this not_eq &source )
   {
      Copy( source );
   }

   return( *this );
}

/*
** CNetworkConnections Stuff
*/

Win32FoundationClasses::CNetworkShares::CNetworkShares(_In_ std::wstring_view machine_name) : CNetwork(machine_name)
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

Win32FoundationClasses::CNetworkShares::~CNetworkShares()
{
   WFC_VALIDATE_POINTER( this );
   Close();
   m_Initialize();
}

_Check_return_ bool Win32FoundationClasses::CNetworkShares::Add(_Inout_ Win32FoundationClasses::CNetworkShareInformation& share_to_add ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Network APIs think only in UNICODE. Yes, Microsoft screwed up the
   // function prototypes...

   wchar_t wide_network_name[ 1024 ];
   wchar_t wide_remark[ 1024 ];
   wchar_t wide_path[ 1024 ];
   wchar_t wide_password[ 1024 ];

   wcscpy_s( wide_network_name, std::size( wide_network_name ), share_to_add.NetworkName.c_str() );
   wcscpy_s( wide_remark, std::size( wide_remark ),       share_to_add.Remark.c_str()      );
   wcscpy_s( wide_path, std::size( wide_path ),         share_to_add.PathName.c_str()    );
   wcscpy_s( wide_password, std::size( wide_password ),     share_to_add.Password.c_str()    );

   // Thanks go to Mark Brakstor (markb@vdr.com) for finding a bug here.

#if ! defined( UNICODE )
   ::ASCII_to_UNICODE( reinterpret_cast<char const *>(share_to_add.NetworkName), wide_network_name );
   ::ASCII_to_UNICODE( reinterpret_cast<char const *>(share_to_add.Remark),      wide_remark       );
   ::ASCII_to_UNICODE( reinterpret_cast<char const *>(share_to_add.PathName),    wide_path         );
   ::ASCII_to_UNICODE( reinterpret_cast<char const *>(share_to_add.Password),    wide_password     );
#endif // UNICODE

   SHARE_INFO_2 share_info_2;

   ::ZeroMemory( &share_info_2, sizeof( share_info_2 ) );

#if defined( _MSC_VER )
   share_info_2.shi2_netname      = (LMSTR) wide_network_name;
   share_info_2.shi2_remark       = (LMSTR) wide_remark;
   share_info_2.shi2_path         = (LMSTR) wide_path;
   share_info_2.shi2_passwd       = (LMSTR) wide_password;
#else
   share_info_2.shi2_netname      = (LPTSTR) wide_network_name;
   share_info_2.shi2_remark       = (LPTSTR) wide_remark;
   share_info_2.shi2_path         = (LPTSTR) wide_path;
   share_info_2.shi2_passwd       = (LPTSTR) wide_password;
#endif
   share_info_2.shi2_type         = share_to_add.Type;
   share_info_2.shi2_permissions  = share_to_add.Permissions;
   share_info_2.shi2_max_uses     = share_to_add.MaximumNumberOfUses;
   share_info_2.shi2_current_uses = share_to_add.CurrentNumberOfUses;

   DWORD parameter_error{ 0 };

   m_ErrorCode = ::NetShareAdd( (LMSTR) m_WideMachineName.get(),
                                        2,
                              (LPBYTE) &share_info_2,
                                       &parameter_error );

   if ( m_ErrorCode == NERR_Success )
   {
      return( true );
   }
   else
   {
      return( false );
   }
}

_Check_return_ DWORD Win32FoundationClasses::CNetworkShares::Check( _In_ std::wstring_view name_of_device ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if (name_of_device.empty() == true)
   {
       return(0);
   }

   // Network APIs think only in UNICODE. Yes, Microsoft screwed up the
   // function prototypes...

   wchar_t wide_device_name[ 4096 ];

   ::ZeroMemory( wide_device_name, sizeof( wide_device_name ) );

   wcsncpy(wide_device_name, name_of_device.data(), std::min(std::size(wide_device_name) - 1, name_of_device.length()));

   DWORD device_type{ 0 };

   m_ErrorCode = ::NetShareCheck( (LMSTR) m_WideMachineName.get(),
                                  (LMSTR) wide_device_name,
                                         &device_type );
   
   if ( m_ErrorCode == NERR_Success )
   {
      return( device_type );
   }
   else
   {
      return( 0 );
   }
}

void Win32FoundationClasses::CNetworkShares::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Win32FoundationClasses::CNetwork::Close();

   if ( m_2InformationBuffer not_eq nullptr )
   {
      ::NetApiBufferFree( m_2InformationBuffer );
      m_2InformationBuffer = nullptr;
   }

   if ( m_1InformationBuffer not_eq nullptr )
   {
      ::NetApiBufferFree( m_1InformationBuffer );
      m_1InformationBuffer = nullptr;
   }
}

_Check_return_ bool Win32FoundationClasses::CNetworkShares::Delete( _Inout_ Win32FoundationClasses::CNetworkShareInformation& share_to_delete ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Network APIs think only in UNICODE. Yes, Microsoft screwed up the
   // function prototypes...

   wchar_t wide_network_name[ 1024 ];

   wcscpy_s( wide_network_name, std::size( wide_network_name ), share_to_delete.NetworkName.c_str() );

#if ! defined( UNICODE )
   ::ASCII_to_UNICODE(reinterpret_cast<char const*>(share_to_delete.NetworkName), wide_network_name );
#endif // UNICODE

   m_ErrorCode = ::NetShareDel( (LMSTR) m_WideMachineName.get(),
                                (LMSTR) wide_network_name,
                                        0 );

   if ( m_ErrorCode == NERR_Success )
   {
      return( true );
   }
   else
   {
      return( false );
   }
}

void Win32FoundationClasses::CNetworkShares::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_ErrorCode             = 0;
   m_2Index                = 0;
   m_2InformationBuffer    = nullptr;
   m_2ResumeHandle         = 0;
   m_2CurrentEntryNumber   = 0;
   m_2NumberOfEntriesRead  = 0;
   m_2TotalNumberOfEntries = 0;
   m_1Index                = 0;
   m_1InformationBuffer    = nullptr;
   m_1ResumeHandle         = 0;
   m_1CurrentEntryNumber   = 0;
   m_1NumberOfEntriesRead  = 0;
   m_1TotalNumberOfEntries = 0;
}

_Check_return_ bool Win32FoundationClasses::CNetworkShares::Enumerate( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_2InformationBuffer not_eq nullptr )
   {
      ::NetApiBufferFree( m_2InformationBuffer );
      m_2InformationBuffer = nullptr;
   }

   m_2Index                = 0;
   m_2CurrentEntryNumber   = 0;
   m_2NumberOfEntriesRead  = 0;
   m_2ResumeHandle         = 0;
   m_2TotalNumberOfEntries = 0;

   if ( m_1InformationBuffer not_eq nullptr )
   {
      ::NetApiBufferFree( m_1InformationBuffer );
      m_1InformationBuffer = nullptr;
   }

   m_1Index                = 0;
   m_1CurrentEntryNumber   = 0;
   m_1NumberOfEntriesRead  = 0;
   m_1ResumeHandle         = 0;
   m_1TotalNumberOfEntries = 0;

   return( m_GetChunk() );
}

_Check_return_ bool Win32FoundationClasses::CNetworkShares::GetNext( _Inout_ Win32FoundationClasses::CNetworkShareInformation& information ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // We need to see which buffer we're working with...

   if ( m_2InformationBuffer not_eq nullptr )
   {
      if ( m_2Index < m_2NumberOfEntriesRead )
      {
         information.Copy( &m_2InformationBuffer[ m_2Index ] );
         m_2Index++;

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
   else if ( m_1InformationBuffer not_eq nullptr )
   {
      if ( m_1Index < m_1NumberOfEntriesRead )
      {
         information.Copy( &m_1InformationBuffer[ m_1Index ] );
         m_1Index++;
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

_Check_return_ bool Win32FoundationClasses::CNetworkShares::m_GetChunk( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Try to get the most information as possible. Start off by asking for
   // data that requires lots of security access privileges

   m_2Index = 0;

   // Let's see if we have already been reading

   if ( m_2NumberOfEntriesRead not_eq 0 and m_2ResumeHandle == 0 )
   {
      // Yup, we've finished reading
      //WFCTRACE( TEXT( "Freeing Buffer 2" ) );
      ::NetApiBufferFree( m_2InformationBuffer );
      m_2InformationBuffer        = nullptr;
      m_2CurrentEntryNumber       = 0;
      m_2NumberOfEntriesRead      = 0;
      m_2ResumeHandle             = 0;
      m_2TotalNumberOfEntries     = 0;
      return( false );
   }

   m_ErrorCode = ::NetShareEnum( (LMSTR) m_WideMachineName.get(),
                                         2, 
                             (LPBYTE *) &m_2InformationBuffer,
                                         64 * 1024,
                                        &m_2NumberOfEntriesRead,
                                        &m_2TotalNumberOfEntries,
                                        &m_2ResumeHandle );

   if ( m_ErrorCode == NERR_Success or m_ErrorCode == ERROR_MORE_DATA )
   {
      if ( m_ErrorCode == NERR_Success )
      {
         m_2ResumeHandle = 0;
      }

      return( true );
   }

   // Make really sure the OS didn't modify our variables...

   if ( m_2InformationBuffer not_eq nullptr )
   {
      //WFCTRACE( TEXT( "Freeing Buffer 2" ) );
      ::NetApiBufferFree( m_2InformationBuffer );
      m_2InformationBuffer = nullptr;
   }

   m_2CurrentEntryNumber   = 0;
   m_2NumberOfEntriesRead  = 0;
   m_2ResumeHandle         = 0;
   m_2TotalNumberOfEntries = 0;

   if ( m_ErrorCode == ERROR_ACCESS_DENIED )
   {
      //WFCTRACE( TEXT( "Access is denied, trying at a lower level" ) );

      m_1Index = 0;

      // Let's see if we have already been reading

      if ( m_1NumberOfEntriesRead not_eq 0 and m_1ResumeHandle == 0 )
      {
         // Yup, we've finished reading
         //WFCTRACE( TEXT( "Freeing Buffer 1" ) );
         ::NetApiBufferFree( m_1InformationBuffer );
         m_1InformationBuffer        = nullptr;
         m_1CurrentEntryNumber       = 0;
         m_1NumberOfEntriesRead      = 0;
         m_1ResumeHandle             = 0;
         m_1TotalNumberOfEntries     = 0;
         return( false );
      }

      m_ErrorCode = ::NetShareEnum( (LMSTR) m_WideMachineName.get(),
                                            1, 
                                (LPBYTE *) &m_1InformationBuffer,
                                            64 * 1024,
                                           &m_1NumberOfEntriesRead,
                                           &m_1TotalNumberOfEntries,
                                           &m_1ResumeHandle );


      if ( m_ErrorCode == NERR_Success or m_ErrorCode == ERROR_MORE_DATA )
      {
         //WFCTRACE( TEXT( "Got lower level information" ) );

         if ( m_ErrorCode == NERR_Success )
         {
            m_1ResumeHandle = 0;
         }

         return( true );
      }
   }

   return( false );
}

void Win32FoundationClasses::CNetworkShares::GetAll(std::vector<Win32FoundationClasses::CNetworkShareInformation>& shares) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (Enumerate() == true)
    {
        Win32FoundationClasses::CNetworkShareInformation information;

        while (GetNext(information) == true)
        {
            shares.push_back(information);
        }
    }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CNetworkShares</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles network shares.">
</HEAD>

<BODY>

<H1>CNetworkShares : <A HREF="CNetwork.htm">CNetwork</A></H1>
$Revision: 31 $<BR>
<HR>

<H2>Description</H2>
This class allows you to add, delete or enumerate shares in Win32.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CNetworkShares</B>()
<B>CNetworkShares</B>( LPCTSTR machine_name )</PRE><DD>
Creates the object and let's you specify which machine to 
execute on (i.e. which machine's shares you want to play with).

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="Add">Add</A></B>( const CNetworkShareInformation&amp; share_to_add )</PRE><DD>
Creates a new share with the options you specify.
The return value will be TRUE if the share was created, FALSE otherwise.

<DT><PRE>BOOL <B><A NAME="Check">Check</A></B>( LPCTSTR computer_name = nullptr, DWORD type = accountMachine )</PRE><DD>
Checks whether or not a computer is sharing a device.
The return value will be TRUE if the device is shared, FALSE otherwise.

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the connection with the machine and cleans up any
internal buffers used.

<DT><PRE>bool <B><A NAME="Delete">Delete</A></B>( const CNetworkShareInformation&amp; share_to_delete )</PRE><DD>
Deletes a share.
The return value will be TRUE if the share was deleted, FALSE otherwise.

<DT><PRE>bool <B><A NAME="Enumerate">Enumerate</A></B>( void )</PRE><DD>
Initializes the object for enumeration.

<DT><PRE>bool <B><A NAME="GetNext">GetNext</A></B>( CNetworkShareInformation&amp; information )</PRE><DD>
Retrieves the next CNetworkShareInformation.
When you reach the end of the list, <B>GetNext</B>() will return FALSE.

</DL>

<H2>Example</H2>

<PRE><CODE>void test_CNetworkShares( LPCTSTR machine_name )
{
   <A HREF="Wfctrace.htm">WFCTRACEINIT</A>( TEXT( &quot;test_CNetworkShares()&quot; ) );

   <B>CNetworkShares</B> shares( machine_name );

   CNetworkShareInformation share_information;

   if ( shares.Enumerate() == true )
   {
      _tprintf( TEXT( &quot;Share Information for %s:\n&quot; ), (LPCTSTR) shares.GetMachineName() );

      while( shares.GetNext( share_information ) == true )
      {
         _tprintf( TEXT( &quot; NetworkName         - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) share_information.NetworkName         );
         _tprintf( TEXT( &quot; Type                - %lu &quot; ),               share_information.Type                );

         switch( share_information.Type )
         {
            case STYPE_DISKTREE:

               _tprintf( TEXT( &quot;Disk&quot; ) );
               break;

            case STYPE_PRINTQ:

               _tprintf( TEXT( &quot;Printer&quot; ) );
               break;

            case STYPE_DEVICE:

               _tprintf( TEXT( &quot;Communications Device&quot; ) );
               break;

            case STYPE_IPC:

               _tprintf( TEXT( &quot;IPC&quot; ) );
               break;

            case STYPE_DFS:

               _tprintf( TEXT( &quot;Distributed File System&quot; ) );
               break;

            case STYPE_SPECIAL:

               _tprintf( TEXT( &quot;Special&quot; ) );
               break;

            default:

               _tprintf( TEXT( &quot;Unknown Type&quot; ) );
               break;
         }

         _tprintf( TEXT( &quot;\n&quot; ) );
         _tprintf( TEXT( &quot; Remark              - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) share_information.Remark              );
         _tprintf( TEXT( &quot; Permissions         - %lu\n&quot; ),              share_information.Permissions         );
         _tprintf( TEXT( &quot; MaximumNumberOfUses - %d\n&quot; ),               share_information.MaximumNumberOfUses );
         _tprintf( TEXT( &quot; CurrentNumberOfUses - %d\n&quot; ),               share_information.CurrentNumberOfUses );
         _tprintf( TEXT( &quot; PathName            - \&quot;%s\&quot;\n&quot; ), (LPCTSTR) share_information.PathName            );
         _tprintf( TEXT( &quot; Password            - \&quot;%s\&quot;\n\n&quot; ), (LPCTSTR) share_information.Password          );
      }
   }
   else
   {
      DWORD error_code {shares.GetErrorCode()};

      std::wstring error_message;

      Convert_NERR_Code_to_String( error_code, error_message );

      _tprintf( TEXT( &quot;CNetworkShares.Enumerate( share_information ), ErrorCode == %d \&quot;%s\&quot;\n&quot; ), error_code, (LPCTSTR) error_message );
   }
}</CODE></PRE>

<H2>API's Used</H2>

<B>CNetworkShares</B> uses the following API's:

<UL>
<LI>NetApiBufferFree
<LI>NetShareAdd
<LI>NetShareCheck
<LI>NetShareDel
<LI>NetShareEnum
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: cnetshar.cpp $<BR>
$Modtime: 6/26/01 10:49a $
</BODY>

</HTML>
*/
