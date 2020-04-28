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
** $Workfile: CFileDirectory.cpp $
** $Revision: 10 $
** $Modtime: 6/26/01 10:46a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

Win32FoundationClasses::CFileDirectory::CFileDirectory() noexcept
{
    WFC_VALIDATE_POINTER(this);
    m_Wildcard.assign(WSTRING_VIEW(L"*.*"));
    m_IncludeDirectoriesInCallback = false;
}

Win32FoundationClasses::CFileDirectory::~CFileDirectory()
{
    WFC_VALIDATE_POINTER(this);
    Close();
}

void Win32FoundationClasses::CFileDirectory::Close(void) noexcept
{
    WFC_VALIDATE_POINTER(this);
    m_Name.clear();
}

_Check_return_ bool Win32FoundationClasses::CFileDirectory::Open(_In_ std::wstring_view directory_name) noexcept
{
    WFC_VALIDATE_POINTER(this);

    m_Name.assign(directory_name);

    if (m_Name.empty() == false)
    {
        if (ends_with( m_Name, '/') == false and
            ends_with( m_Name, '\\') == false )
        {
            m_Name.push_back('/');
        }

        return(true);
    }

    return(false); // User passed us an empty string
}

_Check_return_ bool Win32FoundationClasses::CFileDirectory::Read(_Out_ std::vector<std::wstring>& filenames) noexcept
{
    WFC_VALIDATE_POINTER(this);

    filenames.clear();

    WIN32_FIND_DATAW find_data;

    ZeroMemory(&find_data, sizeof(find_data));

    std::wstring name_and_wildcard(m_Name);
    name_and_wildcard.append(m_Wildcard);

    auto file_find_handle = ::FindFirstFileW(name_and_wildcard.c_str(), &find_data);

    if (file_find_handle not_eq static_cast<HANDLE>(INVALID_HANDLE_VALUE))
    {
        std::wstring complete_filename;

        if (is_flagged( find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true)
        {
            // Skip this file
        }
        else
        {
            complete_filename.assign(m_Name);
            complete_filename.append(find_data.cFileName);

            (void)filenames.push_back(complete_filename);
        }

        while (::FindNextFileW(file_find_handle, &find_data) not_eq FALSE)
        {
            if (is_flagged( find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true )
            {
                // Skip this file
            }
            else
            {
                complete_filename.assign(m_Name);
                complete_filename.append(find_data.cFileName);

                (void)filenames.push_back(complete_filename);
            }
        }

        if (::FindClose(file_find_handle) == FALSE)
        {
            //WFCTRACEERROR( ::GetLastError() );
        }

        file_find_handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);

        return(true);
    }

    return(false);
}

_Check_return_ bool Win32FoundationClasses::CFileDirectory::Read(__callback Win32FoundationClasses::WIDE_FILE_ACTION_FUNCTION action_to_take, _In_ void * action_parameter) noexcept
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER(action_to_take);

    WIN32_FIND_DATA find_data;

    ZeroMemory(&find_data, sizeof(find_data));

    std::wstring filename_and_wildcard(m_Name);
    filename_and_wildcard.append(m_Wildcard);

    auto file_find_handle = ::FindFirstFileW(filename_and_wildcard.c_str(), &find_data);

    // The user passed us a couple of pointers, don't trust them

    std::wstring complete_filename;

    WFC_TRY
    {
       if (file_find_handle not_eq static_cast<HANDLE>(INVALID_HANDLE_VALUE))
       {
          if (is_flagged(find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true)
          {
             if (m_IncludeDirectoriesInCallback == true)
             {
                complete_filename.assign(m_Name);
                complete_filename.append(find_data.cFileName);

                if (action_to_take(action_parameter, complete_filename.c_str(), &find_data) == false)
                {
                   if (::FindClose(file_find_handle) == FALSE)
                   {
                       //WFCTRACEERROR( ::GetLastError() );
                    }

                    file_find_handle = INVALID_HANDLE_VALUE;
                    return(true);
                 }
              }
           }
           else
           {
              complete_filename.assign(m_Name);
              complete_filename.append(find_data.cFileName);

              if (action_to_take(action_parameter, complete_filename.c_str(), &find_data) == false)
              {
                 if (::FindClose(file_find_handle) == FALSE)
                 {
                     //WFCTRACEERROR( ::GetLastError() );
                  }

                  file_find_handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);

                  return(true);
               }
            }

            while (::FindNextFile(file_find_handle, &find_data) not_eq FALSE)
            {
               if (is_flagged(find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true)
               {
                  if (m_IncludeDirectoriesInCallback == true)
                  {
                     complete_filename.assign(m_Name);
                     complete_filename.append(find_data.cFileName);

                     if (action_to_take(action_parameter, complete_filename.c_str(), &find_data) == false)
                     {
                        if (::FindClose(file_find_handle) == FALSE)
                        {
                            //WFCTRACEERROR( ::GetLastError() );
                         }

                         file_find_handle = INVALID_HANDLE_VALUE;
                         return(true);
                      }
                   }
                }
                else
                {
                   complete_filename.assign(m_Name);
                   complete_filename.append(find_data.cFileName);

                   if (action_to_take(action_parameter, complete_filename.c_str(), &find_data) == false)
                   {
                      if (::FindClose(file_find_handle) == FALSE)
                      {
                          //WFCTRACEERROR( ::GetLastError() );
                       }

                       file_find_handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);

                       return(true);
                    }
                 }
              }

              if (::FindClose(file_find_handle) == FALSE)
              {
                  //WFCTRACEERROR( ::GetLastError() );
               }

               file_find_handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);

               return(true);
            }
    }
        WFC_CATCH_ALL
    {
        // Do Nothing
    }
    WFC_END_CATCH_ALL

        return(false);
}

struct RECURSIVE_DIRECTORY_READ_PARAMETERS
{
    HANDLE find_file_handle{ INVALID_HANDLE_VALUE };
    WIN32_FIND_DATA find_data;
    std::wstring complete_filename;
    std::wstring directory_to_open;
    std::wstring new_directory_name;
    std::wstring directory_name_ending_in_a_slash;
};

static void __read_recursively(_In_ std::wstring const& directory_name, _In_ std::wstring const& wildcard, _Out_ std::vector<std::wstring>& filenames) noexcept
{
    auto parameters_p = std::make_unique<RECURSIVE_DIRECTORY_READ_PARAMETERS>();

    if (parameters_p == nullptr)
    {
        return;
    }

    ZeroMemory(&parameters_p->find_data, sizeof(parameters_p->find_data));

    parameters_p->directory_to_open.assign(directory_name);

    if (parameters_p->directory_to_open.empty() == true)
    {
        return;
    }

    if (Win32FoundationClasses::ends_with( parameters_p->directory_to_open, '/' ) == false and
        Win32FoundationClasses::ends_with( parameters_p->directory_to_open, '\\') == false )
    {
        parameters_p->directory_to_open.push_back('/');
    }

    parameters_p->directory_name_ending_in_a_slash.assign(parameters_p->directory_to_open);
    parameters_p->directory_to_open.append(wildcard);

    parameters_p->find_file_handle = ::FindFirstFileW(parameters_p->directory_to_open.c_str(), &parameters_p->find_data);

    if (parameters_p->find_file_handle not_eq static_cast<HANDLE>(INVALID_HANDLE_VALUE))
    {
        if (is_flagged( parameters_p->find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true)
        {
            if (_tcscmp(parameters_p->find_data.cFileName, TEXT(".")) == I_AM_EQUAL_TO_THAT or
                _tcscmp(parameters_p->find_data.cFileName, TEXT("..")) == I_AM_EQUAL_TO_THAT)
            {
                // Do nothing, to do something would cause infinite recursion and that is a bad thing
            }
            else
            {
                parameters_p->new_directory_name.assign(parameters_p->directory_name_ending_in_a_slash);
                parameters_p->new_directory_name.append(parameters_p->find_data.cFileName);

                __read_recursively(parameters_p->new_directory_name, wildcard, filenames);
            }
        }
        else
        {
            parameters_p->complete_filename.assign(parameters_p->directory_name_ending_in_a_slash);
            parameters_p->complete_filename.append(parameters_p->find_data.cFileName);

            (void)filenames.push_back(parameters_p->complete_filename);
        }

        while (FindNextFile(parameters_p->find_file_handle, &parameters_p->find_data) not_eq FALSE)
        {
            if (is_flagged(parameters_p->find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true)
            {
                if (_tcscmp(parameters_p->find_data.cFileName, TEXT(".")) == I_AM_EQUAL_TO_THAT or
                    _tcscmp(parameters_p->find_data.cFileName, TEXT("..")) == I_AM_EQUAL_TO_THAT)
                {
                    // Do nothing, to do something would cause infinite recursion and that is a bad thing
                }
                else
                {
                    parameters_p->new_directory_name.assign(parameters_p->directory_name_ending_in_a_slash);
                    parameters_p->new_directory_name.append(parameters_p->find_data.cFileName);

                    __read_recursively(parameters_p->new_directory_name, wildcard, filenames);
                }
            }
            else
            {
                parameters_p->complete_filename.assign(parameters_p->directory_name_ending_in_a_slash);
                parameters_p->complete_filename.append(parameters_p->find_data.cFileName);

                (void)filenames.push_back(parameters_p->complete_filename);
            }
        }

        if (::FindClose(parameters_p->find_file_handle) == FALSE)
        {
            //WFCTRACEERROR( GetLastError() );
            //WFCTRACE( TEXT( "Couldn't close handle" ) );
        }

        parameters_p->find_file_handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);
    }
}

struct WIDE_RECURSIVE_DIRECTORY_READ_PARAMETERS
{
    HANDLE find_file_handle{ INVALID_HANDLE_VALUE };
    WIN32_FIND_DATAW find_data;
    std::wstring complete_filename;
    std::wstring directory_to_open;
    std::wstring new_directory_name;
    std::wstring directory_name_ending_in_a_slash;
};

void __wide_read_recursively(_In_ std::wstring const& directory_name, _In_ std::wstring const& wildcard, _Out_ std::vector<std::wstring>& filenames)
{
    auto parameters_p = std::make_unique<WIDE_RECURSIVE_DIRECTORY_READ_PARAMETERS>();

    ZeroMemory(&parameters_p->find_data, sizeof(parameters_p->find_data));

    parameters_p->directory_to_open.assign(directory_name);

    if (parameters_p->directory_to_open.empty() == true)
    {
        return;
    }

    if (Win32FoundationClasses::ends_with(parameters_p->directory_to_open, '/') == false and
        Win32FoundationClasses::ends_with(parameters_p->directory_to_open, '\\' ) == false )
    {
        parameters_p->directory_to_open.push_back('/');
    }

    parameters_p->directory_name_ending_in_a_slash.assign(parameters_p->directory_to_open);

    parameters_p->directory_to_open.append(wildcard);

    parameters_p->find_file_handle = FindFirstFileW(parameters_p->directory_to_open.c_str(), &parameters_p->find_data);

    if (parameters_p->find_file_handle not_eq static_cast<HANDLE>(INVALID_HANDLE_VALUE))
    {
        if (is_flagged(parameters_p->find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true)
        {
            if (wcscmp(parameters_p->find_data.cFileName, L".") == I_AM_EQUAL_TO_THAT or
                wcscmp(parameters_p->find_data.cFileName, L"..") == I_AM_EQUAL_TO_THAT)
            {
                // Do nothing, to do something would cause infinite recursion and that is a bad thing
            }
            else
            {
                parameters_p->new_directory_name.assign(parameters_p->directory_name_ending_in_a_slash);
                parameters_p->new_directory_name.append(parameters_p->find_data.cFileName);

                __wide_read_recursively(parameters_p->new_directory_name, wildcard, filenames);
            }
        }
        else
        {
            parameters_p->complete_filename.assign(parameters_p->directory_name_ending_in_a_slash);
            parameters_p->complete_filename.append(parameters_p->find_data.cFileName);

            (void)filenames.push_back(parameters_p->complete_filename);
        }

        while (FindNextFileW(parameters_p->find_file_handle, &parameters_p->find_data) not_eq FALSE)
        {
            if (is_flagged(parameters_p->find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true)
            {
                if (wcscmp(parameters_p->find_data.cFileName, L".") == I_AM_EQUAL_TO_THAT or
                    wcscmp(parameters_p->find_data.cFileName, L"..") == I_AM_EQUAL_TO_THAT)
                {
                    // Do nothing, to do something would cause infinite recursion and that is a bad thing
                }
                else
                {
                    parameters_p->new_directory_name.assign(parameters_p->directory_name_ending_in_a_slash);
                    parameters_p->new_directory_name.append(parameters_p->find_data.cFileName);

                    __wide_read_recursively(parameters_p->new_directory_name, wildcard, filenames);
                }
            }
            else
            {
                parameters_p->complete_filename.assign(parameters_p->directory_name_ending_in_a_slash);
                parameters_p->complete_filename.append(parameters_p->find_data.cFileName);

                (void)filenames.push_back(parameters_p->complete_filename);
            }
        }

        if (::FindClose(parameters_p->find_file_handle) == FALSE)
        {
            //WFCTRACEERROR( GetLastError() );
            //WFCTRACE( TEXT( "Couldn't close handle" ) );
        }

        parameters_p->find_file_handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);
    }
}

static _Check_return_ bool __wide_read_recursively(_In_ std::wstring const& directory_name, _In_ std::wstring const& wildcard, _In_ bool const include_directories, __callback Win32FoundationClasses::WIDE_FILE_ACTION_FUNCTION action_to_take, _In_ void * action_parameter) noexcept
{
    WFC_VALIDATE_POINTER(action_to_take);

    auto parameters_p = std::make_unique<WIDE_RECURSIVE_DIRECTORY_READ_PARAMETERS>();

    ZeroMemory(&parameters_p->find_data, sizeof(parameters_p->find_data));

    parameters_p->directory_to_open.assign(directory_name);

    if (parameters_p->directory_to_open.empty() == true)
    {
        return(false);
    }

    if (Win32FoundationClasses::ends_with( parameters_p->directory_to_open, '/' ) == false and
        Win32FoundationClasses::ends_with( parameters_p->directory_to_open, '\\') == false )
    {
        parameters_p->directory_to_open.push_back('/');
    }

    parameters_p->directory_name_ending_in_a_slash.assign(parameters_p->directory_to_open);
    parameters_p->directory_to_open.append(wildcard);

    parameters_p->find_file_handle = ::FindFirstFileW(parameters_p->directory_to_open.c_str(), &parameters_p->find_data);

    if (parameters_p->find_file_handle not_eq static_cast<HANDLE>(INVALID_HANDLE_VALUE))
    {
        if (is_flagged(parameters_p->find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true)
        {
            if (wcscmp(parameters_p->find_data.cFileName, L".") == I_AM_EQUAL_TO_THAT or
                wcscmp(parameters_p->find_data.cFileName, L"..") == I_AM_EQUAL_TO_THAT)
            {
                // Do nothing, to do something would cause infinite recursion and that is a bad thing
            }
            else
            {
                parameters_p->new_directory_name.assign(parameters_p->directory_name_ending_in_a_slash);
                parameters_p->new_directory_name.append(parameters_p->find_data.cFileName);

                if (include_directories == true)
                {
                    parameters_p->complete_filename.assign(parameters_p->new_directory_name);

                    if (action_to_take(action_parameter, parameters_p->complete_filename.c_str(), &parameters_p->find_data) == false)
                    {
                        if (::FindClose(parameters_p->find_file_handle) == FALSE)
                        {
                            //WFCTRACEERROR( GetLastError() );
                            //WFCTRACEVAL( TEXT( "Couldn't close handle at line " ), __LINE__ );
                        }

                        return(false);
                    }
                }

                if (__wide_read_recursively(parameters_p->new_directory_name, wildcard, include_directories, action_to_take, action_parameter) == false)
                {
                    if (::FindClose(parameters_p->find_file_handle) == FALSE)
                    {
                        //WFCTRACEERROR( ::GetLastError() );
                        //WFCTRACE( TEXT( "Couldn't close handle" ) );
                    }

                    parameters_p->find_file_handle = INVALID_HANDLE_VALUE;

                    return(false);
                }
            }
        }
        else
        {
            parameters_p->complete_filename.assign(parameters_p->directory_name_ending_in_a_slash);
            parameters_p->complete_filename.append(parameters_p->find_data.cFileName);

            WFC_TRY
            {
               if (action_to_take(action_parameter, parameters_p->complete_filename.c_str(), &parameters_p->find_data) == false)
               {
                  if (::FindClose(parameters_p->find_file_handle) == FALSE)
                  {
                      //WFCTRACEERROR( ::GetLastError() );
                      //WFCTRACE( TEXT( "Couldn't close handle" ) );
                   }

                   parameters_p->find_file_handle = INVALID_HANDLE_VALUE;

                   return(false);
                }
            }
            WFC_CATCH_ALL
            {
            }
            WFC_END_CATCH_ALL
        }

        while (::FindNextFileW(parameters_p->find_file_handle, &parameters_p->find_data) not_eq FALSE)
        {
            if (is_flagged(parameters_p->find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true)
            {
                if (wcscmp(parameters_p->find_data.cFileName, L".") == I_AM_EQUAL_TO_THAT or
                    wcscmp(parameters_p->find_data.cFileName, L"..") == I_AM_EQUAL_TO_THAT)
                {
                    // Do nothing, to do something would cause infinite recursion and that is a bad thing
                }
                else
                {
                    parameters_p->new_directory_name.assign(parameters_p->directory_name_ending_in_a_slash);
                    parameters_p->new_directory_name.append(parameters_p->find_data.cFileName);

                    if (include_directories not_eq FALSE)
                    {
                        parameters_p->complete_filename.assign(parameters_p->new_directory_name);

                        if (action_to_take(action_parameter, parameters_p->complete_filename.c_str(), &parameters_p->find_data) == false)
                        {
                            if (::FindClose(parameters_p->find_file_handle) == FALSE)
                            {
                                //WFCTRACEERROR( GetLastError() );
                                //WFCTRACEVAL( TEXT( "Couldn't close handle at line " ), __LINE__ );
                            }

                            parameters_p->find_file_handle = INVALID_HANDLE_VALUE;

                            return(false);
                        }
                    }

                    if (__wide_read_recursively(parameters_p->new_directory_name, wildcard, include_directories, action_to_take, action_parameter) == false)
                    {
                        if (::FindClose(parameters_p->find_file_handle) == FALSE)
                        {
                            //WFCTRACEERROR( ::GetLastError() );
                            //WFCTRACE( TEXT( "Couldn't close handle" ) );
                        }

                        parameters_p->find_file_handle = INVALID_HANDLE_VALUE;

                        return(false);
                    }
                }
            }
            else
            {
                parameters_p->complete_filename.assign(parameters_p->directory_name_ending_in_a_slash);
                parameters_p->complete_filename.append(parameters_p->find_data.cFileName);

                WFC_TRY
                {
                   if (action_to_take(action_parameter, parameters_p->complete_filename.c_str(), &parameters_p->find_data) == false)
                   {
                      if (::FindClose(parameters_p->find_file_handle) == FALSE)
                      {
                          //WFCTRACEERROR( ::GetLastError() );
                          //WFCTRACE( TEXT( "Couldn't close handle" ) );
                       }

                       parameters_p->find_file_handle = INVALID_HANDLE_VALUE;

                       return(false);
                    }
                }
                    WFC_CATCH_ALL
                {
                }
                WFC_END_CATCH_ALL
            }
        }

        if (::FindClose(parameters_p->find_file_handle) == FALSE)
        {
            //WFCTRACEERROR( GetLastError() );
            //WFCTRACE( TEXT( "Couldn't close handle" ) );
        }

        parameters_p->find_file_handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);
    }

    return(true);
}

_Check_return_ bool Win32FoundationClasses::CFileDirectory::ReadRecursively(__callback Win32FoundationClasses::WIDE_FILE_ACTION_FUNCTION action_to_take, _In_ void * action_parameter) noexcept
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER(action_to_take);

    WIN32_FIND_DATA find_data;

    ZeroMemory(&find_data, sizeof(find_data));

    std::wstring filename_and_wildcard(m_Name);
    filename_and_wildcard.append(m_Wildcard);

    auto file_find_handle = ::FindFirstFileW(filename_and_wildcard.c_str(), &find_data);

    if (file_find_handle not_eq static_cast<HANDLE>(INVALID_HANDLE_VALUE))
    {
        std::wstring complete_filename;
        std::wstring new_directory_name;

        if (is_flagged( find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true )
        {
            if (::_tcscmp(find_data.cFileName, TEXT(".")) == I_AM_EQUAL_TO_THAT or
                ::_tcscmp(find_data.cFileName, TEXT("..")) == I_AM_EQUAL_TO_THAT)
            {
                // Avoid infinite recursion here.
            }
            else
            {
                new_directory_name.assign(m_Name);
                new_directory_name.append(find_data.cFileName);

                if (m_IncludeDirectoriesInCallback == true)
                {
                    complete_filename.assign(new_directory_name);

                    if (action_to_take(action_parameter, complete_filename.c_str(), &find_data) == false)
                    {
                        if (::FindClose(file_find_handle) == FALSE)
                        {
                            //WFCTRACEERROR( ::GetLastError() );
                        }

                        return(true);
                    }
                }

                if (::__wide_read_recursively(new_directory_name, m_Wildcard, m_IncludeDirectoriesInCallback, action_to_take, action_parameter) == false)
                {
                    if (::FindClose(file_find_handle) == FALSE)
                    {
                        //WFCTRACEERROR( ::GetLastError() );
                    }

                    file_find_handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);

                    return(true);
                }
            }
        }
        else
        {
            complete_filename.assign(m_Name);
            complete_filename.append(find_data.cFileName);

            // We were passed pointers by the caller, don't trust them

            WFC_TRY
            {
               if (action_to_take(action_parameter, complete_filename.c_str(), &find_data) == false)
               {
                  if (::FindClose(file_find_handle) == FALSE)
                  {
                      //WFCTRACEERROR( ::GetLastError() );
                   }

                   file_find_handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);

                   return(true);
                }
            }
                WFC_CATCH_ALL
            {
            }
            WFC_END_CATCH_ALL
        }

        while (::FindNextFile(file_find_handle, &find_data) not_eq FALSE)
        {
            if (is_flagged( find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true)
            {
                if (::_tcscmp(find_data.cFileName, TEXT(".")) == I_AM_EQUAL_TO_THAT or
                    ::_tcscmp(find_data.cFileName, TEXT("..")) == I_AM_EQUAL_TO_THAT)
                {
                    // Avoid infinite recursion here.
                }
                else
                {
                    new_directory_name.assign(m_Name);
                    new_directory_name.append(find_data.cFileName);

                    if (m_IncludeDirectoriesInCallback == true)
                    {
                        complete_filename.assign(new_directory_name);

                        if (action_to_take(action_parameter, complete_filename.c_str(), &find_data) == false)
                        {
                            if (::FindClose(file_find_handle) == FALSE)
                            {
                                //WFCTRACEERROR( ::GetLastError() );
                            }

                            return(true);
                        }
                    }

                    if (::__wide_read_recursively(new_directory_name, m_Wildcard, m_IncludeDirectoriesInCallback, action_to_take, action_parameter) == false)
                    {
                        if (::FindClose(file_find_handle) == FALSE)
                        {
                            //WFCTRACEERROR( ::GetLastError() );
                        }

                        file_find_handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);

                        return(true);
                    }
                }
            }
            else
            {
                complete_filename.assign(m_Name);
                complete_filename.append(find_data.cFileName);

                // We were passed a couple of pointers by the caller, don't trust them

                WFC_TRY
                {
                   if (action_to_take(action_parameter, complete_filename.c_str(), &find_data) == false)
                   {
                      if (::FindClose(file_find_handle) == FALSE)
                      {
                          //WFCTRACEERROR( ::GetLastError() );
                       }

                       file_find_handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);

                       return(true);
                    }
                }
                    WFC_CATCH_ALL
                {
                }
                WFC_END_CATCH_ALL
            }
        }

        if (::FindClose(file_find_handle) == FALSE)
        {
            //WFCTRACEERROR( ::GetLastError() );
        }

        file_find_handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);

        return(true);
    }

    return(false);
}

_Check_return_ bool Win32FoundationClasses::CFileDirectory::ReadRecursively(_Out_ std::vector<std::wstring>& filenames) noexcept
{
    WFC_VALIDATE_POINTER(this);

    std::wstring wildcard(m_Wildcard);

    WIN32_FIND_DATAW find_data;

    ZeroMemory(&find_data, sizeof(find_data));

    std::wstring filename_and_wildcard(m_Name);
    filename_and_wildcard.append(m_Wildcard);

    auto file_find_handle = ::FindFirstFileW(filename_and_wildcard.c_str(), &find_data);

    if (file_find_handle not_eq static_cast<HANDLE>(INVALID_HANDLE_VALUE))
    {
        std::wstring complete_filename;
        std::wstring new_directory_name;

        if (is_flagged(find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true)
        {
            if (::wcscmp(find_data.cFileName, L".") == I_AM_EQUAL_TO_THAT or
                ::wcscmp(find_data.cFileName, L"..") == I_AM_EQUAL_TO_THAT)
            {
                // Avoid infinite recursion here.
            }
            else
            {
                new_directory_name.assign(m_Name);
                new_directory_name.append(find_data.cFileName);

                ::__wide_read_recursively(new_directory_name, wildcard, filenames);
            }
        }
        else
        {
            complete_filename.assign(m_Name);
            complete_filename.append(find_data.cFileName);

            (void)filenames.push_back(complete_filename);
        }

        while (::FindNextFileW(file_find_handle, &find_data) not_eq FALSE)
        {
            if (is_flagged( find_data.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true)
            {
                if (::wcscmp(find_data.cFileName, L".") == I_AM_EQUAL_TO_THAT or
                    ::wcscmp(find_data.cFileName, L"..") == I_AM_EQUAL_TO_THAT)
                {
                    // Avoid infinite recursion here.
                }
                else
                {
                    new_directory_name.assign(m_Name);
                    new_directory_name.append(find_data.cFileName);

                    ::__wide_read_recursively(new_directory_name, wildcard, filenames);
                }
            }
            else
            {
                complete_filename.assign(m_Name);
                complete_filename.append(find_data.cFileName);

                (void)filenames.push_back(complete_filename);
            }
        }

        if (::FindClose(file_find_handle) == FALSE)
        {
            //WFCTRACEERROR( ::GetLastError() );
        }

        file_find_handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);

        return(true);
    }

    return(false);
}

void Win32FoundationClasses::CFileDirectory::SetWildcard(_In_ std::wstring_view wildcard) noexcept
{
    WFC_VALIDATE_POINTER(this);
    m_Wildcard.assign(wildcard);
}

void Win32FoundationClasses::CFileDirectory::GetWildcard(_Out_ std::wstring& wildcard) const noexcept
{
    WFC_VALIDATE_POINTER(this);
    wildcard.assign(m_Wildcard);
}

void Win32FoundationClasses::CFileDirectory::SetIncludeDirectoriesInCallback(_In_ bool const include_directories) noexcept
{
    WFC_VALIDATE_POINTER(this);
    m_IncludeDirectoriesInCallback = include_directories;
}

_Check_return_ bool Win32FoundationClasses::CFileDirectory::GetIncludeDirectoriesInCallback(void) const noexcept
{
    WFC_VALIDATE_POINTER(this);
    return(m_IncludeDirectoriesInCallback);
}

struct SUM_COUNT
{
    uint64_t number_of_files{ 0 };
    uint64_t sum_of_file_sizes{ 0 };
};

static _Check_return_ bool sum_action(_In_ void * parameter, _In_ wchar_t const * const filename, _In_ WIN32_FIND_DATA const * data_p) noexcept
{
    SUM_COUNT * sum_count = static_cast<SUM_COUNT *>(parameter);

    sum_count->number_of_files++;

    ULARGE_INTEGER file_size;

    file_size.HighPart = data_p->nFileSizeHigh;
    file_size.LowPart = data_p->nFileSizeLow;

    sum_count->sum_of_file_sizes += file_size.QuadPart;

    return(true);
}

void Win32FoundationClasses::CFileDirectory::GetStatistics(_Out_ uint64_t& number_of_files, _Out_ uint64_t& sum) noexcept
{
    WFC_VALIDATE_POINTER(this);

    SUM_COUNT sum_count;

    sum_count.number_of_files = 0;
    sum_count.sum_of_file_sizes = 0;

    (void)ReadRecursively(sum_action, &sum_count);

    number_of_files = sum_count.number_of_files;
    sum = sum_count.sum_of_file_sizes;
}

static _Check_return_ bool __delete_file_action(_Inout_ void * parameter, _In_z_ wchar_t const * const filename, _In_ WIN32_FIND_DATAW const * data_p) noexcept
{
    if (is_flagged( data_p->dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) == true)
    {
        (void)Win32FoundationClasses::CFileDirectory::RecursivelyDestroy(filename);
    }
    else
    {
        Win32FoundationClasses::CFile64::Remove(filename);
    }

    return(true);
}

_Check_return_ bool Win32FoundationClasses::CFileDirectory::RecursivelyDestroy(_In_z_ wchar_t const * const folder_name) noexcept// static
{
    if (folder_name == nullptr or folder_name[0] == 0x00)
    {
        return(false);
    }

    CFileDirectory dir;

    if (dir.Open(folder_name) == true)
    {
        dir.SetIncludeDirectoriesInCallback(true);
        dir.SetWildcard(L"*.*");
        (void) dir.ReadRecursively(__delete_file_action, nullptr);
        dir.Close();
    }

    (void)Win32FoundationClasses::CFileDirectory::Destroy(folder_name);

    return(true);
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CFileDirectory</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The class that lets you search directories recursively or just get a list of files in a directory.">
</HEAD>

<BODY>

<H1>CFileDirectory</H1>

$Revision: 10 $

<HR>

<H2>Description</H2>

This class lets you get a list of filenames (complete path)
contianed in a directory. It will also recurse meaning it will
retrieve all filenames for the directory you want and any
subdirectories too.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CFileDirectory</B>()</PRE><DD>
Constructs the object.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
In staying with the file motif, we should be able
to close that which is opened.

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( const std::wstring&amp; directory_name )</PRE><DD>
Returns TRUE if <CODE>directory_name</CODE> is not empty.

<DT><PRE>BOOL <B><A NAME="Read">Read</A></B>( std::vector&lt;std::wstring&gt;&amp; filenames )</PRE><DD>
This method will fill <CODE>filenames</CODE> with the complete path to
the files in the directory specified in the <B>Open</B>() call.

<DT><PRE>BOOL <B>Read</B>( FILE_ACTION_FUNCTION action_to_take, void * action_parameter )</PRE><DD>
This method will call the <CODE>action_to_take</CODE> function with the
supplied <CODE>action_parameter</CODE> for every file in the directory
specified in the <B>Open</B>() call.
<CODE>action_to_take</CODE> will be passed a string containing the complete
path to the file.

<DT><PRE>BOOL <B><A NAME="ReadRecursively">ReadRecursively</A></B>( std::vectgor&lt;std::wstring&gt;&amp; filenames )</PRE><DD>
This method will fill <CODE>filenames</CODE> with the complete path to
the files in the directory and all subdirectories of the directory
specified in the <B>Open</B>() call.

<DT><PRE>BOOL <B>ReadRecursively</B>( FILE_ACTION_FUNCTION action_to_take, void * action_parameter )</PRE><DD>
This method will call the <CODE>action_to_take</CODE> function with the
supplied <CODE>action_parameter</CODE> for every file in the directory
and all subdirectories of the directory specified in the <B>Open</B>() call.
<CODE>action_to_take</CODE> will be passed a string containing the complete
path to the file.

</DL>

<H2>Example</H2><PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

BOOL action_to_take( void * action_parameter, const std::wstring&amp; complete_filename )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;action_to_take()&quot; ) );

   DWORD * file_counter_p = reinterpret_cast&lt; DWORD * &gt;( action_parameter );

   (*file_counter_p)++; // increment the counter

   // Print the filename
   _tprintf( TEXT( &quot;%s\n&quot; ), (LPCTSTR) complete_filename );
}

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   int argument_index = 1;

   DWORD total_number_of_files = 0;

   <B>CFileDirectory</B> directory;

   while( argument_index &lt; number_of_command_line_arguments )
   {
      if ( directory.Open( command_line_arguments[ argument_index ] ) not_eq FALSE )
      {
         directory.Read( action_to_take, &amp;total_number_of_files );
      }
   }

   _tprintf( &quot;Processed %lu files.\n&quot;, total_number_of_files );

   return( EXIT_SUCCESS );
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CFileDirectory.cpp $<BR>
$Modtime: 6/26/01 10:46a $
</BODY>
</HTML>
*/
