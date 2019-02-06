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
** $Workfile: CServiceApplication.hpp $
** $Revision: 3 $
** $Modtime: 6/26/01 11:04a $
*/

#if ! defined( SERVICE_APPLICATION_CLASS_HEADER )

#define SERVICE_APPLICATION_CLASS_HEADER

using SHOW_USAGE_ROUTINE = void (*)( LPCTSTR );
using SET_DEFAULTS_ROUTINE = void (*)( void );

class CServiceApplication
{
   protected:

      virtual __checkReturn int m_Main( __in const int number_of_command_line_arguments,
                          __in_ecount_z_opt( number_of_command_line_arguments ) LPCTSTR command_line_arguments[],
                          __in_z LPCTSTR service_name,
                          __in_z LPCTSTR friendly_service_name,
                          __in_z LPCTSTR executable_path,
                          __in const DWORD installation_configuration,
                          __callback LPTHREAD_START_ROUTINE worker_thread,
                          __callback SHOW_USAGE_ROUTINE show_usage,
                          __callback SET_DEFAULTS_ROUTINE set_defaults ) noexcept;

   public:

       CServiceApplication() = delete;
       CServiceApplication(const CServiceApplication&) = delete;
       CServiceApplication& operator=(const CServiceApplication&) = delete;
       
       CServiceApplication( __in const int number_of_command_line_arguments,
                           __in_ecount_z_opt( number_of_command_line_arguments ) LPCTSTR command_line_arguments[],
                           __in_z LPCTSTR service_name,
                           __in_z LPCTSTR friendly_service_name,
                           __in_z LPCTSTR executable_path,
                           __callback LPTHREAD_START_ROUTINE worker_thread,
                           __callback SHOW_USAGE_ROUTINE show_usage = nullptr,
                           __callback SET_DEFAULTS_ROUTINE set_defaults = nullptr,
                           __in const DWORD installation_configuration = SERVICE_DEMAND_START );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CServiceApplication( void );
};

#endif // SERVICE_APPLICATION_CLASS_HEADER
