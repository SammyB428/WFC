/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2014, Samuel R. Blackburn
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
** $Workfile: CFileTransferProtocol.cpp $
** $Revision: 19 $
** $Modtime: 6/26/01 10:46a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if ! defined( WFC_STL )

#if defined( _DEBUG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

CFileTransferProtocol::CFileTransferProtocol( LPCTSTR user_name, LPCTSTR password )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( user_name );
   WFC_VALIDATE_POINTER_NULL_OK( password );

   if ( user_name == nullptr )
   {
      SetUserName( TEXT( "anonymous" ) );
   }
   else
   {
      // We were passed a pointer, don't trust it

      WFC_TRY
      {
         SetUserName( user_name );
      }
      WFC_CATCH_ALL
      {
         SetUserName( TEXT( "anonymous" ) );
      }
      WFC_END_CATCH_ALL
   }

   TCHAR default_user_name[ 512 ];
   DWORD size_of_user_name = 0;

   ZeroMemory( default_user_name, sizeof( default_user_name ) );

   size_of_user_name = std::size( default_user_name );

   ::GetUserName( default_user_name, &size_of_user_name );

   std::wstring ip_address;

   CSimpleSocket::GetMyAddress( ip_address );

   std::wstring default_password( default_user_name );

   default_password += TEXT( "@" );
   default_password += ip_address;

   SetPassword( default_password );

   if ( password != nullptr )
   {
      // We were passed a pointer, don't trust it

      WFC_TRY
      {
         SetPassword( password );
      }
      WFC_CATCH_ALL
      {
         // Do Nothing
      }
      WFC_END_CATCH_ALL
   }
}

CFileTransferProtocol::~CFileTransferProtocol()
{
   WFC_VALIDATE_POINTER( this );

   m_UserName.Empty();
   m_Password.Empty();
}

void CFileTransferProtocol::GetDirectory( _In_ CUniformResourceLocator const& url, __inout std::vector<std::wstring>& directory, __in_z LPCTSTR filter )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( filter );

   CInternetSession * internet_session_p = nullptr;

   CFtpConnection * connection_p = nullptr;

   CFtpFileFind * file_find_p = nullptr;

   std::vector<uint8_t> read_buffer;

   WFC_TRY
   {
      std::wstring temp_string;

      temp_string.Format( TEXT( "CFileTransferProtocol::GetDirectory( \"%s\" ) : %lu" ), (LPCTSTR) url, (DWORD) ::GetTickCount() );

      WFC_TRY
      {
         internet_session_p = new CInternetSession( temp_string );
      }
      WFC_CATCH( CInternetException *, exception_p )
      {
#if defined( _DEBUG )

         TCHAR error_message[ 513 ];

         ZeroMemory( error_message, sizeof( error_message ) );

         exception_p->GetErrorMessage( error_message, std::size( error_message ) );

         //WFCTRACEVAL( TEXT( "Error is " ), error_message );

#endif // _DEBUG

         exception_p->Delete();
         return;
      }
      WFC_CATCH_ALL
      {
         return;
      }
      WFC_END_CATCH_ALL

      WFC_TRY
      {
         connection_p = internet_session_p->GetFtpConnection( url.MachineName, m_UserName, m_Password );
      }
      WFC_CATCH( CInternetException *, exception_p )
      {
#if defined( _DEBUG )

         TCHAR error_message[ 513 ];

         ZeroMemory( error_message, sizeof( error_message ) );

         exception_p->GetErrorMessage( error_message, std::size( error_message ) );

         WFCTRACEVAL( TEXT( "Error is " ), error_message );

#endif // _DEBUG
         exception_p->Delete();
      }
      WFC_CATCH_ALL
      {
      }
      WFC_END_CATCH_ALL

      if ( connection_p == nullptr )
      {
         internet_session_p->Close();
         delete internet_session_p;
         internet_session_p = reinterpret_cast< CInternetSession * >( nullptr );

         return;
      }

      if ( url.PathName.GetLength() != 0 )
      {
         WFC_TRY
         {
            if ( connection_p->SetCurrentDirectory( url.PathName ) == FALSE )
            {
               WFCTRACEVAL( TEXT( "Can't change directory to " ), url.PathName );
            }
         }
         WFC_CATCH_ALL
         {
         }
         WFC_END_CATCH_ALL
      }

      WFC_TRY
      {
         file_find_p = new CFtpFileFind( connection_p );
      }
      WFC_CATCH( CInternetException *, exception_p )
      {
#if defined( _DEBUG )

         TCHAR error_message[ 513 ];

         ZeroMemory( error_message, sizeof( error_message ) );

         exception_p->GetErrorMessage( error_message, std::size( error_message ) );

         WFCTRACEVAL( TEXT( "Error is " ), error_message );

#endif // _DEBUG
         exception_p->Delete();
      }
      WFC_CATCH_ALL
      {
      }
      WFC_END_CATCH_ALL

      if ( file_find_p == nullptr )
      {
         connection_p->Close();
         delete connection_p;
         connection_p = nullptr;

         internet_session_p->Close();
         delete internet_session_p;
         internet_session_p = nullptr;

         return;
      }

      if ( file_find_p->FindFile( filter ) != FALSE )
      {
         directory.Add( file_find_p->GetFileName() );

         while( file_find_p->FindNextFile() != FALSE )
         {
            WFCTRACEVAL( TEXT( "Now Adding " ), file_find_p->GetFileName() );
            directory.Add( file_find_p->GetFileName() );
         }
      }

      delete file_find_p;
      file_find_p = nullptr;

      connection_p->Close();
      delete connection_p;
      connection_p = nullptr;

      internet_session_p->Close();
      delete internet_session_p;
      internet_session_p = nullptr;
   }
   WFC_CATCH_ALL
   {
   }
   WFC_END_CATCH_ALL
}

void CFileTransferProtocol::GetFile( CUniformResourceLocator const& url, std::vector<uint8_t>& file_contents )
{
   WFC_VALIDATE_POINTER( this );

   // Start with an empty array
   file_contents.RemoveAll();

   CInternetSession * internet_session_p = nullptr;

   CFtpConnection * connection_p = nullptr;

   std::vector<uint8_t> read_buffer;

   WFC_TRY
   {
      std::wstring temp_string;

      temp_string.Format( TEXT( "CFileTransferProtocol::GetFile( %s ) : %lu" ), (LPCTSTR) url, (DWORD) ::GetTickCount() );

      WFCTRACEVAL( TEXT( "Unique ID is " ), temp_string );

      WFC_TRY
      {
         internet_session_p = new CInternetSession( temp_string );
      }
      WFC_CATCH( CInternetException *, exception_p )
      {
#if defined( _DEBUG )

         TCHAR error_message[ 513 ];

         ZeroMemory( error_message, sizeof( error_message ) );

         exception_p->GetErrorMessage( error_message, std::size( error_message ) );

         WFCTRACEVAL( TEXT( "Error is " ), error_message );

#endif // _DEBUG
         exception_p->Delete();
         return;
      }
      WFC_CATCH_ALL
      {
         return;
      }
      WFC_END_CATCH_ALL

      WFC_TRY
      {
         connection_p = internet_session_p->GetFtpConnection( url.MachineName, m_UserName, m_Password );
      }
      WFC_CATCH( CInternetException *, exception_p )
      {
#if defined( _DEBUG )

         TCHAR error_message[ 513 ];

         ZeroMemory( error_message, sizeof( error_message ) );

         exception_p->GetErrorMessage( error_message, std::size( error_message ) );

         WFCTRACEVAL( TEXT( "Error is " ), error_message );

#endif // _DEBUG
         exception_p->Delete();
      }
      WFC_CATCH_ALL
      {
      }
      WFC_END_CATCH_ALL

      if ( connection_p == nullptr )
      {
         internet_session_p->Close();
         delete internet_session_p;
         internet_session_p = nullptr;

         return;
      }

      CInternetFile * source_file_p = nullptr;

      WFC_TRY
      {
         source_file_p = connection_p->OpenFile( url.PathName );
      }
      WFC_CATCH( CInternetException *, exception_p )
      {
#if defined( _DEBUG )

         TCHAR error_message[ 513 ];

         ZeroMemory( error_message, sizeof( error_message ) );

         exception_p->GetErrorMessage( error_message, std::size( error_message ) );

         WFCTRACEVAL( TEXT( "Error is " ), error_message );

#endif // _DEBUG
         exception_p->Delete();
      }
      WFC_CATCH_ALL
      {
      }
      WFC_END_CATCH_ALL

      if ( source_file_p == nullptr )
      {
         connection_p->Close();
         delete connection_p;
         connection_p = nullptr;

         internet_session_p->Close();
         delete internet_session_p;
         internet_session_p = nullptr;

         return;
      }

      read_buffer.SetSize( 65535 * 4 ); // A really big buffer...
      source_file_p->SetReadBufferSize( read_buffer.GetSize() );

      UINT number_of_bytes_read = 0;

      WFC_TRY
      {
         do
         {
            number_of_bytes_read = source_file_p->Read( read_buffer.GetData(), read_buffer.GetSize() );

            if ( number_of_bytes_read > 0 )
            {
               read_buffer.SetSize( number_of_bytes_read );
               file_contents.Append( read_buffer );
               read_buffer.SetSize( 65535 * 4 );
            }
         }
         while( number_of_bytes_read > 0 );
      }
      WFC_CATCH( CInternetException *, exception_p )
      {
#if defined( _DEBUG )

         TCHAR error_message[ 513 ];

         ZeroMemory( error_message, sizeof( error_message ) );

         exception_p->GetErrorMessage( error_message, std::size( error_message ) );

         WFCTRACEVAL( TEXT( "Error is " ), error_message );

#endif // _DEBUG
         exception_p->Delete();
      }
      WFC_CATCH_ALL
      {
      }
      WFC_END_CATCH_ALL

      source_file_p->Close();
      delete source_file_p;
      source_file_p = nullptr;

      connection_p->Close();
      delete connection_p;
      connection_p = nullptr;

      internet_session_p->Close();
      delete internet_session_p;
      internet_session_p = nullptr;
   }
   WFC_CATCH_ALL
   {
      WFC_TRY
      {
         internet_session_p->Close();
      }
      WFC_CATCH_ALL
      {
         ;
      }
      WFC_END_CATCH_ALL

      WFC_TRY
      {
         delete internet_session_p;
         internet_session_p = nullptr;
      }
      WFC_CATCH_ALL
      {
         ;
      }
      WFC_END_CATCH_ALL
   }
   WFC_END_CATCH_ALL
}

void CFileTransferProtocol::GetPassword(std::wstring& password ) const
{
   WFC_VALIDATE_POINTER( this );
   password = m_Password;
}

void CFileTransferProtocol::GetUserName(std::wstring& user_name ) const
{
   WFC_VALIDATE_POINTER( this );
   user_name = m_UserName;
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CFileTransferProtocol::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CFileTransferProtocol at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   m_UserName is " ) << m_UserName << TEXT( "\n" );
   dump_context << TEXT( "   m_Passowrd is " ) << m_Password << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

_Check_return_ BOOL CFileTransferProtocol::PutFile( _In_ CUniformResourceLocator const& url, _In_ std::vector<uint8_t> const& file_contents )
{
   WFC_VALIDATE_POINTER( this );

   CInternetSession * internet_session_p = nullptr;

   CFtpConnection * connection_p = nullptr;

   std::vector<uint8_t> read_buffer;

   BOOL return_value = FALSE;

   WFC_TRY
   {
      std::wstring temp_string;

      temp_string.Format( TEXT( "CFileTransferProtocol::PutFile( %s ) : %lu" ), (LPCTSTR) url, (DWORD) ::GetTickCount() );

      WFCTRACEVAL( TEXT( "Unique ID is " ), temp_string );

      WFC_TRY
      {
         internet_session_p = new CInternetSession( temp_string );
      }
      WFC_CATCH( CInternetException *, exception_p )
      {
#if defined( _DEBUG )

         TCHAR error_message[ 513 ];

         ZeroMemory( error_message, sizeof( error_message ) );

         exception_p->GetErrorMessage( error_message, std::size( error_message ) );

         WFCTRACEVAL( TEXT( "Error is " ), error_message );

#endif // _DEBUG
         exception_p->Delete();
         return( FALSE );
      }
      WFC_CATCH_ALL
      {
         return( FALSE );
      }
      WFC_END_CATCH_ALL

      WFC_TRY
      {
         connection_p = internet_session_p->GetFtpConnection( url.MachineName, m_UserName, m_Password );
      }
      WFC_CATCH( CInternetException *, exception_p )
      {
#if defined( _DEBUG )

         TCHAR error_message[ 513 ];

         ZeroMemory( error_message, sizeof( error_message ) );

         exception_p->GetErrorMessage( error_message, std::size( error_message ) );

         WFCTRACEVAL( TEXT( "Error is " ), error_message );

#endif // _DEBUG
         exception_p->Delete();
         connection_p = nullptr;
      }
      WFC_CATCH_ALL
      {
         connection_p = nullptr;
      }
      WFC_END_CATCH_ALL

      if ( connection_p == nullptr )
      {
         internet_session_p->Close();
         delete internet_session_p;
         internet_session_p = nullptr;

         return( FALSE );
      }

      CInternetFile * source_file_p = nullptr;

      WFC_TRY
      {
         source_file_p = connection_p->OpenFile( url.PathName, GENERIC_WRITE );
      }
      WFC_CATCH( CInternetException *, exception_p )
      {
#if defined( _DEBUG )

         TCHAR error_message[ 513 ];

         ZeroMemory( error_message, sizeof( error_message ) );

         exception_p->GetErrorMessage( error_message, std::size( error_message ) );

         WFCTRACEVAL( TEXT( "Error is " ),  error_message );

#endif // _DEBUG
         exception_p->Delete();
         source_file_p = nullptr;
      }
      WFC_CATCH_ALL
      {
         source_file_p = nullptr;
      }
      WFC_END_CATCH_ALL

      if ( source_file_p == nullptr )
      {
         connection_p->Close();
         delete connection_p;
         connection_p = nullptr;

         internet_session_p->Close();
         delete internet_session_p;
         internet_session_p = nullptr;

         return( FALSE );
      }

      WFC_TRY
      {
         source_file_p->Write( file_contents.GetData(), file_contents.GetSize() );
      }
      WFC_CATCH( CInternetException *, exception_p )
      {
#if defined( _DEBUG )

         TCHAR error_message[ 513 ];

         ZeroMemory( error_message, sizeof( error_message ) );

         exception_p->GetErrorMessage( error_message, std::size( error_message ) );

         WFCTRACEVAL( TEXT( "Error is " ), error_message );

#endif // _DEBUG
         exception_p->Delete();
         return_value = FALSE;
      }
      WFC_CATCH_ALL
      {
         return_value = FALSE;
      }
      WFC_END_CATCH_ALL

      return_value = TRUE;

      source_file_p->Close();
      delete source_file_p;
      source_file_p = nullptr;

      connection_p->Close();
      delete connection_p;
      connection_p = nullptr;

      internet_session_p->Close();
      delete internet_session_p;
      internet_session_p = nullptr;
   }
   WFC_CATCH_ALL
   {
      WFC_TRY
      {
         internet_session_p->Close();
      }
      WFC_CATCH_ALL
      {
         ;
      }
      WFC_END_CATCH_ALL

      WFC_TRY
      {
         delete internet_session_p;
         internet_session_p = nullptr;
      }
      WFC_CATCH_ALL
      {
         ;
      }
      WFC_END_CATCH_ALL
   }
   WFC_END_CATCH_ALL

   return( return_value );
}

void CFileTransferProtocol::SetPassword( std::wstring const& password )
{
   WFC_VALIDATE_POINTER( this );
   m_Password = password;
}

void CFileTransferProtocol::SetUserName( std::wstring const& user_name )
{
   WFC_VALIDATE_POINTER( this );
   m_UserName = user_name;
}

#endif // WFC_STL

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CFileTransferProtocol</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that encapsulates FTP (file transfer protocol).">
</HEAD>

<BODY>

<H1>CFileTransferProtocol</H1>

$Revision: 19 $
<HR>

<H2>Description</H2>

This class simplifies FTP. It solves the problem of getting a single
file or a directory listing from an FTP server.

<H2>Constructors</H2>

<DL COMPACT>

<DT><B>CFileTransferProtocol</B>( LPCTSTR user_name = nullptr, LPCTSTR password = nullptr )</PRE><DD>
Constructs the object. If <CODE>user_name</CODE> is nullptr, &quot;anonymous&quot;
will be used instead. If <CODE>password</CODE> is nullptr, an e-mail address
will be created using the current user
name (retrieved via <B>GetUsername</B>()) and the dotted IP address
(retrieved via <A HREF="SOCKETS.htm">CSimpleSocket</A>::GetMyAddress())
of the computer. For example, if your user name is &quot;Bob&quot; and
your IP address is 1.2.3.4, the default password will be
&quot;Bob@1.2.3.4&quot;.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="GetDirectory">GetDirectory</A></B>( const <A HREF="CURL.htm">CUniformResourceLocator</A>&amp; url, std::vector&lt;std::wstring&gt;&amp; directory )</PRE><DD>
Retrieves the directory listing from the URL given. For example,
&quot;ftp://ftp.microsoft.com/bussys&quot;

<DT><PRE>void <B><A NAME="GetFile">GetFile</A></B>( const <A HREF="CURL.htm">CUniformResourceLocator</A>&amp; url, std::vector&lt;uint8_t&gt;&amp; file_contents )</PRE><DD>
Retrieves the file specified in <CODE>url</CODE> and puts it into <CODE>file_contents</CODE>.

<DT><PRE>void <B><A NAME="GetPassword">GetPassword</A></B>( std::wstring&amp; password ) const</PRE><DD>
Retrieves the password to be used to log in to the ftp servers.

<DT><PRE>void <B><A NAME="GetUserName">GetUserName</A></B>( std::wstring&amp; user_name ) const</PRE><DD>
Retrieves the login ID to be used when logging in to ftp servers.

<DT><PRE>BOOL <B><A NAME="PutFile">PutFile</A></B>( const <A HREF="CURL.htm">CUniformResourceLocator</A>&amp; url, const std::vector&lt;uint8_t&gt;&amp; file_contents )</PRE><DD>
Sends the contents of <CODE>file_contents</CODE> to the ftp server specified in <CODE>url</CODE>.
It will return TRUE if the contents were sent, FALSE if it didn't.

<DT><PRE>void <B><A NAME="SetPassword">SetPassword</A></B>( const std::wstring&amp; password )</PRE><DD>
Sets the password to be used to log in to the ftp servers.

<DT><PRE>void <B><A NAME="SetUserName">SetUserName</A></B>( const std::wstring&amp; user_name )</PRE><DD>
Sets the login ID to be used when logging in to ftp servers.

</DL>

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;

void test_CFileTransferProtocol( void )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;test_CFileTransferProtocol()&quot; ) );

   <B>CFileTransferProtocol</B> ftp;

   std::vector&lt;std::wstring&gt; directory;

   <A HREF="CURL.htm">CUniformResourceLocator</A> url( TEXT( &quot;ftp://ftp.microsoft.com/&quot; ) );

   ftp.GetDirectory( url, directory );

   _tprintf( TEXT( &quot;Directory for %s\n&quot; ), (LPCTSTR) url );

   int index = 0;

   while( index &lt; directory.GetSize() )
   {
      WFCTRACE( directory.GetAt( index ) );
      _tprintf( TEXT( &quot;%s\n&quot; ), (LPCTSTR) directory.GetAt( index ) );
      index++;
   }

   std::vector&lt;uint8_t&gt; file_contents;

   url = TEXT( &quot;ftp://ftp.microsoft.com/disclaimer.txt&quot; );

   ftp.GetFile( url, file_contents );

   _tprintf( TEXT( &quot;Retrieved %d bytes\n&quot; ), (int) file_contents.GetSize() );
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CFileTransferProtocol.cpp $<BR>
$Modtime: 6/26/01 10:46a $
</BODY>

</HTML>
*/
