/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2019, Samuel R. Blackburn
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
** $Workfile: WFCTrace.hpp $
** $Revision: 18 $
** $Modtime: 8/27/01 5:02a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( WFC_TRACE_CLASS_HEADER )

#define WFC_TRACE_CLASS_HEADER

#if defined( _DEBUG )

#define WFCTRACEINIT( string )           Win32FoundationClasses::CWfcTrace ___trace( string, BIT_0 );
#define WFCLTRACEINIT( string )          Win32FoundationClasses::CWfcTrace ___trace( string, BIT_31 );
#define WFCTRACEINITLEVEL( string, x )   Win32FoundationClasses::CWfcTrace ___trace( string, x );
#define WFCTRACE( string )               { ___trace.Output( string );               }
#define WFCTRACEVAL( string, value )     { ___trace.Output( string, value );        }
#define WFCTRACEVARIANT( string, value ) { ___trace.OutputVariant( string, value ); }
#define WFCTRACEBINVAL( string, value )  { ___trace.OutputBinary( string, value );  }
#define WFCTRACEDUMP( variable )         { ___trace.Output( #variable, variable );  }
#define WFCTRACEERROR( error_code )      { ___trace.ReportError( error_code );      }
#define WFCTRACESOCKETERROR( errorcode ) { ___trace.ReportSocketError( errorcode ); }
#define WFCTRACEON()                     { Win32FoundationClasses::CWfcTrace::TraceOn();          }
#define WFCTRACEOFF()                    { Win32FoundationClasses::CWfcTrace::TraceOff();         }
#define WFCTRACEALLON()                  { Win32FoundationClasses::CWfcTrace::TraceAllOn();       }
#define WFCTRACEALLOFF()                 { Win32FoundationClasses::CWfcTrace::TraceAllOff();      }
#define WFCTRACELEVELON( x )             { Win32FoundationClasses::CWfcTrace::TraceLevelOn( x );  }
#define WFCTRACELEVELOFF( x )            { Win32FoundationClasses::CWfcTrace::TraceLevelOff( x ); }

#else

#define WFCTRACEINIT( string )           {;}
#define WFCLTRACEINIT( string )          {;}
#define WFCTRACEINITLEVEL( string, x )   {;}
#define WFCTRACE( string )               {;}
#define WFCTRACEVAL( string, value )     {;}
#define WFCTRACEVARIANT( string, value ) {;}
#define WFCTRACEBINVAL( string, value )  {;}
#define WFCTRACEDUMP( variable )         {;}
#define WFCTRACEERROR( error_code )      {;}
#define WFCTRACESOCKETERROR( errorcode ) {;}
#define WFCTRACEON()                     {;}
#define WFCTRACEOFF()                    {;}
#define WFCTRACEALLON()                  {;}
#define WFCTRACEALLOFF()                 {;}
#define WFCTRACELEVELON( x )             {;}
#define WFCTRACELEVELOFF( x )            {;}

#endif // _DEBUG

class CWfcTrace
{
   protected:

      LPCTSTR m_FunctionName;
      std::size_t m_TracingLevel;

      void m_Indent( void ) const noexcept;

   public:

      static int  Indent;
      static LONG Levels;
      static bool Tracing;

      CWfcTrace() = delete;
      CWfcTrace(CWfcTrace const&) = delete;
      CWfcTrace& operator=(CWfcTrace const&) = delete;

      CWfcTrace( __in_z LPCTSTR function_name, _In_ std::size_t const tracing_level ) noexcept;

      virtual ~CWfcTrace();

      static void TraceAllOn( void ) noexcept;
      static void TraceAllOff( void ) noexcept;
      static void TraceLevelOn( _In_ std::size_t const level ) noexcept;
      static void TraceLevelOff( _In_ std::size_t const level ) noexcept;
      static void TraceOn( void ) noexcept;
      static void TraceOff( void ) noexcept;
      void Output( __in_z LPCTSTR message ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ int const integer      ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ UINT const integer     ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ long const a_long      ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ ULONG const a_long     ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ LONGLONG const a_long  ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ ULONGLONG const a_long ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ double const a_double  ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ std::wstring const& string  ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ VOID const * pointer   ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ LARGE_INTEGER const& value ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ ULARGE_INTEGER const& value ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ RECT const& value ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ POINT const& value ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ SIZE const& value ) const noexcept;
      void Output( __in_z LPCTSTR message, _In_ GUID const& value ) const noexcept; // New for Release 52
      void Output( __in_z LPCTSTR message, _In_ FILETIME const& value ) const noexcept; // New for Release 52
      void Output( __in_z LPCTSTR message, _In_ SYSTEMTIME const& value ) const noexcept; // New for Release 52
      void OutputBinary( __in_z LPCTSTR message, _In_ ULONG const a_long ) const noexcept;
      void OutputVariant( __in_z LPCTSTR message, _In_ VARIANT const& value ) const noexcept;
      void OutputVariant( __in_z LPCTSTR message, _In_ VARIANT const * value ) const noexcept;
      void ReportError( _In_ DWORD const error_code ) noexcept;
      void ReportSocketError( _In_ DWORD const error_code ) noexcept;
};

#endif // WFC_TRACE_CLASS_HEADER
