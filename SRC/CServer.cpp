/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2017, Samuel R. Blackburn
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
** $Workfile: cserver.cpp $
** $Revision: 28 $
** $Modtime: 6/26/01 10:51a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

USING_WFC_NAMESPACE

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

CServer::CServer()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CServer::CServer(_In_opt_z_ LPCWSTR machine_name )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( machine_name );
   Open( machine_name );
}

CServer::~CServer()
{
   WFC_VALIDATE_POINTER( this );
   Close();
}

void CServer::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   CNetwork::Close();
   m_Initialize();
}

void CServer::GetComment( _Out_ std::wstring& comment ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   comment.clear();

   /*
   ** Test m_ServerName of emptiness because a lot of comments are blank
   */

   if ( m_Retrieved102 == false )
   {
      m_Get_102_Data();
   }

   comment = m_Comment;
}

void CServer::GetDomain( _Out_ std::wstring& name ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   name.clear();

   /*
   ** Yes, domain is a member of the 503 structure, but that call doesn't work.
   ** We've got to use 599. Although for numsessions we have to use 503, go figure
   */

   if ( m_Retrieved503 == false )
   {
      m_Get_503_Data();
   }

   name = m_Domain;
}

_Check_return_ DWORD CServer::GetMajorVersion( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Retrieved102 == false )
   {
      m_Get_102_Data();
   }

   return( m_MajorVersion );
}

_Check_return_ DWORD CServer::GetMinorVersion( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Retrieved102 == false )
   {
      m_Get_102_Data();
   }

   return( m_MinorVersion );
}

_Check_return_ DWORD CServer::GetNumberOfOpens( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Retrieved503 == false )
   {
      m_Get_503_Data();
   }

   return( m_NumberOfOpens );
}

_Check_return_ DWORD CServer::GetNumberOfUsers( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Retrieved503 == false )
   {
      m_Get_503_Data();
   }

   return( m_NumberOfUsers );
}

void CServer::GetName( _Out_ std::wstring& name ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   name.clear();

   if ( m_Retrieved102 == false )
   {
      m_Get_102_Data();
   }

   name = m_MachineName;
}

_Check_return_ DWORD CServer::GetPlatform( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Retrieved102 == false )
   {
      m_Get_102_Data();
   }

   return( m_Platform );
}

void CServer::GetPlatformName( _Out_ std::wstring& name ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   name.clear();

   /*
   ** Test m_ServerName of emptiness because a lot of comments are blank
   */

   if ( m_Retrieved102 == false )
   {
      m_Get_102_Data();
   }

   switch( m_Platform )
   {
      case SV_PLATFORM_ID_OS2:

         name.assign(WSTRING_VIEW(L"OS/2"));
         return;

      case SV_PLATFORM_ID_NT:

         name.assign(WSTRING_VIEW(L"NT"));
         return;

      default:

         format( name, L"Unknown Type %d", (int)m_Platform);
         return;
   }
}

void CServer::GetPlatformNameAndVersion( _Out_ std::wstring& name ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   name.clear();

   std::wstring temp_name;

   GetPlatformName( temp_name );

   format( name, L"%s %d.%d", temp_name.c_str(), (int) m_MajorVersion, (int) m_MinorVersion );
}

_Check_return_ DWORD CServer::GetType( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Retrieved102 == false )
   {
      m_Get_102_Data();
   }

   return( m_Type );
}

void CServer::GetUserPath( _Out_ std::wstring& path ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   path.clear();

   if ( m_Retrieved102 == false )
   {
      m_Get_102_Data();
   }

   path = m_UserPath;
}

_Check_return_ DWORD CServer::GetUsers( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   if ( m_Retrieved102 == false)
   {
      m_Get_102_Data();
   }

   return( m_Users );
}

void CServer::m_Get_102_Data( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   LPBYTE buffer = nullptr;

   /*
   ** One of the children got loose in the header files again...
   **
   ** Also, we can't get 101 information because it doesn't work if you supply
   ** a machine name... Go Figure...
   */

   m_ErrorCode = ::NetServerGetInfo( (LMSTR) m_WideMachineName.get(), 102, &buffer );

   if ( m_ErrorCode != NERR_Success )
   {
      //WFCTRACEERROR( m_ErrorCode );
   }

   if ( buffer != nullptr )
   {
      SERVER_INFO_102 * information_p = (SERVER_INFO_102 *) buffer;

      m_MachineName.assign( information_p->sv102_name == nullptr ? L"" : information_p->sv102_name );
      m_UserPath.assign( information_p->sv102_userpath == nullptr ? L"" : information_p->sv102_userpath );
      m_Comment.assign( information_p->sv102_comment == nullptr ? L"" : information_p->sv102_comment );

      m_Users        = information_p->sv102_users;
      m_Platform     = information_p->sv102_platform_id;
      m_MajorVersion = information_p->sv102_version_major;
      m_MinorVersion = information_p->sv102_version_minor;
      m_Type         = information_p->sv102_type;
      m_Retrieved102 = true;
   }
}

void CServer::m_Get_503_Data( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   LPBYTE buffer = nullptr;

   /*
   ** One of the children got loose in the header files again...
   */

   m_ErrorCode = ::NetServerGetInfo( (LMSTR) m_WideMachineName.get(), 503, &buffer );

   if ( m_ErrorCode != NERR_Success )
   {
      //WFCTRACEERROR( m_ErrorCode );
   }

   if ( buffer != nullptr )
   {
      SERVER_INFO_503 *information_p = (SERVER_INFO_503 *) buffer;

      /*
      ** Now store the info we want...
      */

      m_NumberOfUsers = information_p->sv503_sessusers;
      m_NumberOfOpens = information_p->sv503_sessopens;
      m_Domain.assign( information_p->sv503_domain == nullptr ? L"" : information_p->sv503_domain );
      m_Retrieved503  = true;
   }
}

void CServer::m_Get_599_Data( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   LPBYTE buffer = (LPBYTE) nullptr;

   /*
   ** One of the children got loose in the header files again...
   */

   m_ErrorCode = ::NetServerGetInfo( (LMSTR) m_WideMachineName.get(), 599, &buffer );

   if ( m_ErrorCode != NERR_Success )
   {
      //WFCTRACEERROR( m_ErrorCode );
   }

   if ( buffer != nullptr )
   {
      SERVER_INFO_599 * information_p = (SERVER_INFO_599 *) buffer;

      if ( information_p->sv599_domain == nullptr )
      {
         information_p->sv599_domain = (LMSTR) L" ";
      }

      /*
      ** Now store the info we want...
      */

      m_Domain.assign(information_p->sv599_domain == nullptr ? L"" : information_p->sv599_domain);
      m_Retrieved599 = true;
   }
}

void CServer::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   CNetwork::m_Initialize();

   m_Comment.clear();
   m_UserPath.clear();

   m_Retrieved102 = false;
   m_Retrieved503 = false;
   m_Retrieved599 = false;

   m_MajorVersion  = 0;
   m_MinorVersion  = 0;
   m_NumberOfUsers = 0;
   m_NumberOfOpens = 0;
   m_Platform      = 0;
   m_Type          = 0;
   m_Users         = 0;
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CServer</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class gives you information about NT domain servers.">
</HEAD>

<BODY>

<H1>CServer : <A HREF="CNetwork.htm">CNetwork</A></H1>
$Revision: 28 $<BR>
<HR>

<H2>Description</H2>

This class allows you to gather information about a server. It can get you information
similiar to the Server Control Panel applet. I am not proud of this class. It is 
inconsistent and ugly. I'll rewrite it one of these days.

<H2>Data Members</H2>
None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the connection with the machine and cleans up any
internal buffers used.

<DT><PRE>void <B><A NAME="GetComment">GetComment</A></B>( std::wstring&amp; comment )</PRE><DD>
Retrieves the comment about the server.

<DT><PRE>void <B><A NAME="GetDomain">GetDomain</A></B>( std::wstring&amp; domain_name )</PRE><DD>
Retrieves the name of the domain the server is a member of.

<DT><PRE>DWORD <B><A NAME="GetMajorVersion">GetMajorVersion</A></B>( void )</PRE><DD>
Retrieves the major portion of the operating system 
version number. For example, in NT 3.51, 3 would be the major version portion.

<DT><PRE>DWORD <B><A NAME="GetMinorVersion">GetMinorVersion</A></B>( void )</PRE><DD>
Retrieves the minor portion of the operating system 
version number. For example, in NT 3.51, 51 would be the minor version portion.

<DT><PRE>void <B><A NAME="GetName">GetName</A></B>( std::wstring&amp; name )</PRE><DD>
Retrieves the name of the server.

<DT><PRE>DWORD <B><A NAME="GetNumberOfOpens">GetNumberOfOpens</A></B>( void )</PRE><DD>
Retrieves the number of open files/pipes on the server.

<DT><PRE>DWORD <B><A NAME="GetNumberOfUsers">GetNumberOfUsers</A></B>( void )</PRE><DD>
Retrieves the users currently using the server.

<DT><PRE>DWORD <B><A NAME="GetPlatform">GetPlatform</A></B>( void )</PRE><DD>
Tells you what kind of machine the server is.

<DT><PRE>void <B><A NAME="GetPlatformName">GetPlatformName</A></B>( std::wstring&amp; name )</PRE><DD>
Tells you what kind of machine the server is in 
a human readable form.

<DT><PRE>void <B><A NAME="GetPlatformNameAndVersion">GetPlatformNameAndVersion</A></B>( std::wstring&amp; name_and_platform )</PRE><DD>
An even more human friendly way of getting
the server's operating system and version in a a human readable form.

<DT><PRE>DWORD <B><A NAME="GetType">GetType</A></B>( void )</PRE><DD>
Retrieves type of the server.

<DT><PRE>void <B><A NAME="GetUserPath">GetUserPath</A></B>( std::wstring&amp; path )</PRE><DD>
Retrieves the user's path.

<DT><PRE>DWORD <B><A NAME="GetUsers">GetUsers</A></B>( void )</PRE><DD>
Retrieves the number of users of the server.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

void test_CServer( LPCTSTR machine_name )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;test_CServer()&quot; ) );

   <B>CServer</B> server;

   server.Open( machine_name );

   std::wstring name;
   std::wstring comment;
   std::wstring path;
   std::wstring os;
   std::wstring domain;

   DWORD major_version   = 0;
   DWORD minor_version   = 0;
   DWORD type            = 0;
   DWORD users           = 0;
   DWORD number_of_users = 0;
   DWORD number_of_opens = 0;

   server.GetUserPath( path );
   server.GetName( name );
   server.GetComment( comment );
   server.GetPlatformName( os );
   server.GetDomain( domain );

   major_version   = server.GetMajorVersion();
   minor_version   = server.GetMinorVersion();
   type            = server.GetType();
   users           = server.GetUsers();
   number_of_users = server.GetNumberOfUsers();
   number_of_opens = server.GetNumberOfOpens();

   _tprintf( TEXT( &quot;Server Name:      \&quot;%s\&quot;\n&quot; ), (LPCTSTR) name    );
   _tprintf( TEXT( &quot;Server Domain:    \&quot;%s\&quot;\n&quot; ), (LPCTSTR) domain  );
   _tprintf( TEXT( &quot;Server Comment:   \&quot;%s\&quot;\n&quot; ), (LPCTSTR) comment );
   _tprintf( TEXT( &quot;Server User Path: \&quot;%s\&quot;\n&quot; ), (LPCTSTR) path    );
   _tprintf( TEXT( &quot;Server OS:        \&quot;%s\&quot;\n&quot; ), (LPCTSTR) os      );
   _tprintf( TEXT( &quot;Server Version:    %d.%d\n&quot; ), major_version, minor_version );
   _tprintf( TEXT( &quot;Server Users:      %d\n&quot; ),    users );
   _tprintf( TEXT( &quot;Number Of Users:   %d\n&quot; ),    number_of_users );
   _tprintf( TEXT( &quot;Number Of Opens:   %d\n&quot; ),    number_of_opens );
}</CODE></PRE>

<H2>API's Used</H2>

<DL COMPACT>
<LI>NetServerGetInfo
</DL>

<HR><I>Copyright, 2000, <A HREF="wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: cserver.cpp $<BR>
$Modtime: 6/26/01 10:51a $
</BODY>

</HTML>
*/
