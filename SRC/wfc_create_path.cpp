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
** $Workfile: wfc_create_path.cpp $
** $Revision: 2 $
** $Modtime: 7/30/01 5:36p $
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
static bool inline _Check_return_ does_directory_exist( __in_z LPCTSTR full_path ) noexcept
{
   auto directory_handle = CreateFileW( full_path,
                                         0, // zero means we want NO access, we just want to query the device
                                         0,
                                         nullptr,
                                         OPEN_EXISTING,
                                         FILE_ATTRIBUTE_NORMAL bitor FILE_FLAG_BACKUP_SEMANTICS, // We need FILE_FLAG_BACKUP_SEMANTICS to open the root directory
                                         nullptr );

   if ( directory_handle ==INVALID_HANDLE_VALUE )
   {
      return( false );
   }

   CloseHandle( directory_handle );
   return( true );
}

static bool inline _Check_return_ does_wide_directory_exist( __in_z wchar_t const * full_path ) noexcept
{
   auto directory_handle = ::CreateFileW( full_path,
                                            0, // zero means we want NO access, we just want to query the device
                                            0,
                                            nullptr,
                                            OPEN_EXISTING,
                                            FILE_ATTRIBUTE_NORMAL bitor FILE_FLAG_BACKUP_SEMANTICS, // We need FILE_FLAG_BACKUP_SEMANTICS to open the root directory
                                            nullptr );

   if ( directory_handle == INVALID_HANDLE_VALUE )
   {
      return( false );
   }

   (void) ::CloseHandle( directory_handle );
   return( true );
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_create_path( __in_z LPCTSTR path_name, __in_opt LPSECURITY_ATTRIBUTES const security_attributes_p ) noexcept
{
   WFC_TRY
   {
      WFC_VALIDATE_POINTER( path_name );

      if ( path_name == nullptr )
      {
         return( false );
      }

      if ( does_directory_exist( path_name ) == true )
      {
         return( true );
      }

      if ( CreateDirectory( path_name, security_attributes_p ) == FALSE )
      {
         DWORD last_error = GetLastError();

         if ( last_error != ERROR_PATH_NOT_FOUND and
              last_error != ERROR_ALREADY_EXISTS )
         {
            //WFCTRACEERROR( last_error );
            return( false );
         }

         if ( last_error != ERROR_ALREADY_EXISTS )
         {
            // The path could not be created. Create all directories in the path

            std::wstring directory_path( path_name );

            (void) replace( directory_path, '/', '\\' );

            std::wstring this_directory;

            std::size_t location_of_backslash = directory_path.find( '\\' );

            if ( location_of_backslash == 0 )
            {
               // We could be a UNC

               if ( directory_path.at( 1 ) == '\\' )
               {
                  // Yep, this is a UNC

                  location_of_backslash = directory_path.find( '\\', 2 );

                  if ( location_of_backslash == std::wstring::npos )
                  {
                     return( false );
                  }

                  // We are now at the separator between server name and share name

                  location_of_backslash = directory_path.find( '\\', location_of_backslash + 1 );
               }
            }

            if ( location_of_backslash == std::wstring::npos)
            {
               return( false );
            }

            location_of_backslash = directory_path.find( '\\', location_of_backslash + 1 );

            while( location_of_backslash != std::wstring::npos )
            {
               this_directory.assign( directory_path.substr( 0, location_of_backslash ) );

               if ( this_directory.empty() == false )
               {
                  CreateDirectoryW( this_directory.c_str(), security_attributes_p );
                  location_of_backslash = directory_path.find( '\\', location_of_backslash + 1 );
               }
               else
               {
                  location_of_backslash = std::wstring::npos;
               }
            }

            if ( CreateDirectoryW( directory_path.c_str(), security_attributes_p ) == FALSE )
            {
               last_error = GetLastError();

               if ( last_error != ERROR_PATH_NOT_FOUND )
               {
                  //WFCTRACEERROR( last_error );
                  //WFCTRACE( TEXT( "Last CreateDirectory() failed." ) );
                  return( false );
               }
            }
         }
      }

      return( true );
   }
   WFC_CATCH_ALL
   {
      return( false );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_create_wide_path( _In_z_ wchar_t const * path_name, _In_opt_ LPSECURITY_ATTRIBUTES const security_attributes_p ) noexcept
{
   WFC_TRY
   {
      WFC_VALIDATE_POINTER( path_name );

      if ( path_name == nullptr )
      {
         return( false );
      }

      if ( does_wide_directory_exist( path_name ) == true )
      {
         return( true );
      }

      if ( CreateDirectoryW( path_name, security_attributes_p ) == FALSE )
      {
         DWORD last_error = GetLastError();

         if ( last_error != ERROR_PATH_NOT_FOUND and
              last_error != ERROR_ALREADY_EXISTS )
         {
            //WFCTRACEERROR( last_error );
            //WFCTRACE( TEXT( "CreateDirectory() failed." ) );
            return( false );
         }

         if ( last_error != ERROR_ALREADY_EXISTS )
         {
            // The path could not be created. Create all directories in the path

            std::wstring directory_path( path_name );

            (void) replace( directory_path, '/', '\\');

            std::wstring this_directory;

            std::size_t location_of_backslash = directory_path.find( '\\' );

            if ( location_of_backslash == 0 )
            {
               // We could be a UNC

               if ( directory_path.at( 1 ) == '\\' )
               {
                  // Yep, this is a UNC

                  location_of_backslash = directory_path.find( '\\', 2 );

                  if ( location_of_backslash == std::wstring::npos )
                  {
                     return( false );
                  }

                  // We are now at the separator between server name and share name

                  location_of_backslash = directory_path.find( '\\', location_of_backslash + 1 );
               }
            }

            if ( location_of_backslash == std::wstring::npos )
            {
               return( false );
            }

            location_of_backslash = directory_path.find( '\\', location_of_backslash + 1 );

            while( location_of_backslash != std::wstring::npos)
            {
               this_directory.assign( directory_path.substr(0, location_of_backslash) );

               if ( this_directory.empty() == false )
               {
                  CreateDirectoryW( this_directory.c_str(), security_attributes_p );
                  location_of_backslash = directory_path.find( '\\', location_of_backslash + 1 );
               }
               else
               {
                  location_of_backslash = std::wstring::npos;
               }
            }

            if ( CreateDirectoryW( directory_path.c_str(), security_attributes_p ) == FALSE )
            {
               last_error = GetLastError();

               if ( last_error != ERROR_PATH_NOT_FOUND )
               {
                  //WFCTRACEERROR( last_error );
                  //WFCTRACE( TEXT( "Last CreateDirectory() failed." ) );
                  return( false );
               }
            }
         }
      }

      return( true );
   }
   WFC_CATCH_ALL
   {
      return( false );
   }
   WFC_END_CATCH_ALL
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_create_path</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that creates all directories in a path.">
</HEAD>

<BODY>

<H1>wfc_create_path</H1>
$Revision: 2 $<HR>

<H2>Declaration</H2>
<PRE>BOOL <B>wfc_create_path</B>( LPCTSTR new_filename, LPSECURITY_ATTRIBUTES security_attributes_p )</PRE>

<H2>Description</H2>
This function creates all directories in a given path.
This little function is great for installation programs.

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   if ( number_of_command_line_arguments &lt; 2 )
   {
      _tprintf( TEXT( &quot;Usage: mkpath directory_path\n&quot; ) );
      _tprintf( TEXT( &quot;Sample: mkpath \&quot;c:\\Program Files\\My Directory\\Data Files\&quot;\n&quot; ) );
      return( EXIT_SUCCESS );
   }

   if ( <B>wfc_create_path</B>( command_line_arguments[ 1 ], NULL ) == false )
   {
      _tprintf( TEXT( &quot;Failed.\n&quot; ) );
      return( EXIT_FAILURE );
   }

   return( EXIT_SUCCESS );
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_create_path</B>() uses the following API's:
<UL>
<LI>CloseHandle
<LI>CreateDirectory
<LI>CreateFile
</UL>

<HR><I>Copyright, 1995-2001, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_create_path.cpp $<BR>
$Modtime: 7/30/01 5:36p $
</BODY>

</HTML>
*/
