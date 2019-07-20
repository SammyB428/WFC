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

#if ! defined( WFC_TRACE_CLASS_HEADER )

#define WFC_TRACE_CLASS_HEADER

#if defined( _DEBUG )

#define WFCTRACEINIT( string )           CWfcTrace ___trace( string, BIT_0 );
#define WFCLTRACEINIT( string )          CWfcTrace ___trace( string, BIT_31 );
#define WFCTRACEINITLEVEL( string, x )   CWfcTrace ___trace( string, x );
#define WFCTRACE( string )               { ___trace.Output( string );               }
#define WFCTRACEVAL( string, value )     { ___trace.Output( string, value );        }
#define WFCTRACEVARIANT( string, value ) { ___trace.OutputVariant( string, value ); }
#define WFCTRACEBINVAL( string, value )  { ___trace.OutputBinary( string, value );  }
#define WFCTRACEDUMP( variable )         { ___trace.Output( #variable, variable );  }
#define WFCTRACEERROR( error_code )      { ___trace.ReportError( error_code );      }
#define WFCTRACESOCKETERROR( errorcode ) { ___trace.ReportSocketError( errorcode ); }
#define WFCTRACEON()                     { CWfcTrace::TraceOn();                    }
#define WFCTRACEOFF()                    { CWfcTrace::TraceOff();                   }
#define WFCTRACEALLON()                  { CWfcTrace::TraceAllOn();                 }
#define WFCTRACEALLOFF()                 { CWfcTrace::TraceAllOff();                }
#define WFCTRACELEVELON( x )             { CWfcTrace::TraceLevelOn( x );            }
#define WFCTRACELEVELOFF( x )            { CWfcTrace::TraceLevelOff( x );           }

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

      static int   Indent;
      static std::size_t Levels;
      static bool  Tracing;

      CWfcTrace() = delete;
      CWfcTrace(CWfcTrace const&) = delete;
      CWfcTrace& operator=(CWfcTrace const&) = delete;

      CWfcTrace( __in_z LPCTSTR function_name, __in std::size_t const tracing_level ) noexcept;

      virtual ~CWfcTrace() noexcept;

      static void TraceAllOn( void ) noexcept;
      static void TraceAllOff( void ) noexcept;
      static void TraceLevelOn( __in std::size_t const level ) noexcept;
      static void TraceLevelOff( __in std::size_t const level ) noexcept;
      static void TraceOn( void ) noexcept;
      static void TraceOff( void ) noexcept;
      void Output( __in_z LPCTSTR message ) const noexcept;
      void Output( __in_z LPCTSTR message, __in int const integer      ) const noexcept;
      void Output( __in_z LPCTSTR message, __in UINT const integer     ) const noexcept;
      void Output( __in_z LPCTSTR message, __in long const a_long      ) const noexcept;
      void Output( __in_z LPCTSTR message, __in ULONG const a_long     ) const noexcept;
      void Output( __in_z LPCTSTR message, __in LONGLONG const a_long  ) const noexcept;
      void Output( __in_z LPCTSTR message, __in ULONGLONG const a_long ) const noexcept;
      void Output( __in_z LPCTSTR message, __in double const a_double  ) const noexcept;
      void Output( __in_z LPCTSTR message, __in std::wstring const& string  ) const noexcept;
      void Output( __in_z LPCTSTR message, __in VOID const * pointer   ) const noexcept;
      void Output( __in_z LPCTSTR message, __in LARGE_INTEGER const& value ) const noexcept;
      void Output( __in_z LPCTSTR message, __in ULARGE_INTEGER const& value ) const noexcept;
      void Output( __in_z LPCTSTR message, __in RECT const& value ) const noexcept;
      void Output( __in_z LPCTSTR message, __in POINT const& value ) const noexcept;
      void Output( __in_z LPCTSTR message, __in SIZE const& value ) const noexcept;
      void Output( __in_z LPCTSTR message, __in GUID const& value ) const noexcept; // New for Release 52
      void Output( __in_z LPCTSTR message, __in FILETIME const& value ) const noexcept; // New for Release 52
      void Output( __in_z LPCTSTR message, __in SYSTEMTIME const& value ) const noexcept; // New for Release 52
      void OutputBinary( __in_z LPCTSTR message, __in ULONG const a_long ) const noexcept;
      void OutputVariant( __in_z LPCTSTR message, __in VARIANT const& value ) const noexcept;
      void OutputVariant( __in_z LPCTSTR message, __in VARIANT const * value ) const noexcept;
      void ReportError( __in DWORD const error_code ) noexcept;
      void ReportSocketError( __in DWORD const error_code ) noexcept;
};

#endif // WFC_TRACE_CLASS_HEADER
