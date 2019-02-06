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
** $Workfile: CFileDirectory.hpp $
** $Revision: 5 $
** $Modtime: 6/26/01 11:01a $
*/

#if ! defined( FILE_DIRECTORY_CLASS_HEADER )

#define FILE_DIRECTORY_CLASS_HEADER

using WIDE_FILE_ACTION_FUNCTION = __checkReturn bool (*)( _Inout_ void * parameter, _In_z_ const wchar_t * const filename, _In_ const WIN32_FIND_DATAW * data_p );

class CFileDirectory
{
   protected:

      std::wstring m_Name;
      std::wstring m_Wildcard;

      bool m_IncludeDirectoriesInCallback{ false };

   public:

      CFileDirectory(_In_ const CFileDirectory&) = delete;
      CFileDirectory& operator=(_In_ const CFileDirectory&) = delete;

      // Construction

      CFileDirectory() noexcept;

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CFileDirectory() noexcept;

      /*
      ** Methods
      */

      static __checkReturn bool Create(__in const std::wstring& directory_name) noexcept
      {
          return(Create(directory_name.c_str()));
      }

      static __checkReturn bool Destroy(__in_z const wchar_t * const directory_name) noexcept
      {
          WFC_VALIDATE_POINTER(directory_name);

          if (directory_name == nullptr || directory_name[0] == 0x00)
          {
              return(false);
          }

          if (::RemoveDirectoryW(directory_name) == FALSE)
          {
              return(false);
          }

          return(true);
      }

      static __checkReturn bool Destroy(__in const std::wstring& directory_name) noexcept
      {
          return(Destroy(directory_name.c_str()));
      }

      static __checkReturn bool RecursivelyDestroy(__in_z const wchar_t * const directory_name) noexcept;
          
      static __checkReturn bool RecursivelyDestroy(__in const std::wstring& directory_name) noexcept
      {
          return(RecursivelyDestroy(directory_name.c_str()));
      }

      static __checkReturn bool Create( _In_z_ const wchar_t * const directory_name ) noexcept
      {
          WFC_VALIDATE_POINTER( directory_name );

          if ( directory_name == nullptr || directory_name[ 0 ] == 0x00 )
          {
              return( false );
          }

          if ( ::CreateDirectoryW( directory_name, nullptr ) == FALSE )
          {
              return( false );
          }

          return( true );
      }

      static __checkReturn bool IsDirectory( __in_z const wchar_t * const file_name ) noexcept
      {
          WFC_VALIDATE_POINTER( file_name );

          if ( file_name == nullptr || file_name[ 0 ] == 0x00 )
          {
              return( false );
          }

          const DWORD attributes = ::GetFileAttributesW( file_name );

          if ( attributes != INVALID_FILE_ATTRIBUTES )
          {
              if ( attributes & FILE_ATTRIBUTE_DIRECTORY )
              {
                  return( true );
              }
          }

          return( false );
      }

      static __checkReturn bool IsDirectory(__in const std::wstring& file_name) noexcept
      {
          return(IsDirectory(file_name.c_str()));
      }

      virtual void Close( void ) noexcept;
      virtual __checkReturn bool Open( _In_z_ const wchar_t * const directory_name ) noexcept;
      virtual __checkReturn bool Read(__out std::vector<std::wstring>& filenames) noexcept;
      virtual __checkReturn bool Read( __callback WIDE_FILE_ACTION_FUNCTION action_to_take, __in void * action_parameter ) noexcept;
      virtual __checkReturn bool ReadRecursively(__out std::vector<std::wstring>& filenames) noexcept;
      virtual __checkReturn bool ReadRecursively( __callback WIDE_FILE_ACTION_FUNCTION action_to_take, __in void * action_parameter ) noexcept;
      virtual void SetWildcard( _In_z_ const wchar_t * const wilcard ) noexcept;
      virtual void GetWildcard( __out std::wstring& wildcard ) const noexcept;
      virtual void SetIncludeDirectoriesInCallback( __in const bool include_directories = true ) noexcept;
      virtual __checkReturn bool GetIncludeDirectoriesInCallback( void ) const noexcept;
      virtual void GetStatistics( __out uint64_t& number_of_files, __out uint64_t& total_storage ) noexcept;
};

#endif // FILE_DIRECTORY_CLASS_HEADER
