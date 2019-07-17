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
** $Workfile: structs.hpp $
** $Revision: 17 $
** $Modtime: 6/26/01 11:07a $
*/

#if ! defined( STRUCTS_CLASS_HEADER )

#define STRUCTS_CLASS_HEADER

class CAccessAllowedEntry : public _ACCESS_ALLOWED_ACE
{
   public:

      CAccessAllowedEntry();
      CAccessAllowedEntry( __in CAccessAllowedEntry const& source );
      CAccessAllowedEntry( __in ACCESS_ALLOWED_ACE const* source );
      virtual ~CAccessAllowedEntry();
      virtual void Copy( __in CAccessAllowedEntry const& source ) noexcept;
      virtual void Copy( __in ACCESS_ALLOWED_ACE const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CAccessAllowedEntry& operator=( __in CAccessAllowedEntry const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CAccessControlEntryHeader : public _ACE_HEADER
{
   public:

      CAccessControlEntryHeader();
      CAccessControlEntryHeader( __in CAccessControlEntryHeader const& source );
      CAccessControlEntryHeader( __in ACE_HEADER const * source );
      virtual ~CAccessControlEntryHeader();
      virtual void Copy( __in CAccessControlEntryHeader const& source ) noexcept;
      virtual void Copy( __in ACE_HEADER const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CAccessControlEntryHeader& operator=( __in CAccessControlEntryHeader const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CAccessControlList : public _ACL
{
   public:

      CAccessControlList();
      CAccessControlList( __in CAccessControlList const& source );
      CAccessControlList( __in ACL const * source );
      virtual ~CAccessControlList();
      virtual void Copy( __in CAccessControlList const& source ) noexcept;
      virtual void Copy( __in ACL const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CAccessControlList& operator=( __in CAccessControlList const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CAccessDeniedEntry : public _ACCESS_DENIED_ACE
{
   public:

      CAccessDeniedEntry();
      CAccessDeniedEntry( __in CAccessDeniedEntry const& source );
      CAccessDeniedEntry( __in ACCESS_DENIED_ACE const * source );
      virtual ~CAccessDeniedEntry();
      virtual void Copy( __in CAccessDeniedEntry const& source ) noexcept;
      virtual void Copy( __in ACCESS_DENIED_ACE const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CAccessDeniedEntry& operator=( __in CAccessDeniedEntry const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CBitmapCoreHeader : public tagBITMAPCOREHEADER
{
   public:

      CBitmapCoreHeader();
      CBitmapCoreHeader( __in CBitmapCoreHeader const& source );
      CBitmapCoreHeader( __in tagBITMAPCOREHEADER const * source );
      virtual ~CBitmapCoreHeader();
      virtual void Copy( __in CBitmapCoreHeader const& source ) noexcept;
      virtual void Copy( __in tagBITMAPCOREHEADER const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CBitmapCoreHeader& operator=( __in CBitmapCoreHeader const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CBitmapFileHeader : public tagBITMAPFILEHEADER
{
   public:

      CBitmapFileHeader();
      CBitmapFileHeader( __in CBitmapFileHeader const& source );
      CBitmapFileHeader( __in tagBITMAPFILEHEADER const * source );
      virtual ~CBitmapFileHeader();
      virtual void Copy( __in CBitmapFileHeader const& source ) noexcept;
       virtual void Copy( __in tagBITMAPFILEHEADER const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CBitmapFileHeader& operator=( __in CBitmapFileHeader const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CBitmapInfoHeader : public tagBITMAPINFOHEADER
{
   public:

      CBitmapInfoHeader();
      CBitmapInfoHeader( __in CBitmapInfoHeader const& source );
      CBitmapInfoHeader( __in tagBITMAPINFOHEADER const * source );
      virtual ~CBitmapInfoHeader();
      virtual void Copy( __in CBitmapInfoHeader const& source ) noexcept;
      virtual void Copy( __in tagBITMAPINFOHEADER const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CBitmapInfoHeader& operator=( __in CBitmapInfoHeader const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CColorAdjustment : public tagCOLORADJUSTMENT
{
   public:

      CColorAdjustment();
      CColorAdjustment( __in CColorAdjustment const& source );
      CColorAdjustment( __in tagCOLORADJUSTMENT const * source );
      virtual ~CColorAdjustment();
      virtual void Copy( __in CColorAdjustment const& source ) noexcept;
      virtual void Copy( __in tagCOLORADJUSTMENT const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CColorAdjustment& operator=( __in CColorAdjustment const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CCommunicationsConfiguration : public _COMMCONFIG
{
   public:

      CCommunicationsConfiguration();
      CCommunicationsConfiguration( __in CCommunicationsConfiguration const& source );
      CCommunicationsConfiguration( __in COMMCONFIG const& source );
      virtual ~CCommunicationsConfiguration();

      virtual void Copy( __in CCommunicationsConfiguration const& source ) noexcept;
      virtual void Copy( __in COMMCONFIG const& source ) noexcept;
      virtual void Empty( void ) noexcept;

      virtual _Check_return_ CCommunicationsConfiguration& operator=( __in CCommunicationsConfiguration const& source ) noexcept;
      virtual _Check_return_ CCommunicationsConfiguration& operator=( __in COMMCONFIG const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CCommunicationProperties : public _COMMPROP
{

   public:

      CCommunicationProperties();
      CCommunicationProperties( __in CCommunicationProperties const& source );
      CCommunicationProperties( __in _COMMPROP const& source );
      virtual ~CCommunicationProperties();
      virtual void ConvertBaudRateToString( __in DWORD const baud_rate, __out std::wstring& human_readable_string ) const noexcept;
      virtual void ConvertBaudRateListToString( __in DWORD const baud_rate, __out std::wstring& human_readable_string ) const noexcept;
      virtual void ConvertDataBitsToString( __in DWORD const data_bits, __out std::wstring& human_readable_string ) const noexcept;
      virtual void ConvertProviderCapabilitiesToString( __in DWORD const capabilities, __out std::wstring& human_readable_string ) const noexcept;
      virtual void ConvertProviderSubtypeToString( __in DWORD const baud_rate, __out std::wstring& human_readable_string ) const noexcept;
      virtual void ConvertSettableParametersToString( __in DWORD const parameters, __out std::wstring& human_readable_string ) const noexcept;
      virtual void ConvertStopParityToString( __in DWORD const stop_parity, __out std::wstring& human_readable_string ) const noexcept;
      virtual void Copy( __in CCommunicationProperties const& source ) noexcept;
      virtual void Copy( __in  _COMMPROP const& source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CCommunicationProperties& operator=( __in CCommunicationProperties const& source ) noexcept;
      virtual _Check_return_ CCommunicationProperties& operator=( __in _COMMPROP const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class COutlineTextMetricA : public _OUTLINETEXTMETRICA
{
   public:

      COutlineTextMetricA();
      COutlineTextMetricA( __in COutlineTextMetricA const& source );
      COutlineTextMetricA( __in _OUTLINETEXTMETRICA const * source );
      virtual ~COutlineTextMetricA();
      virtual void Copy( __in COutlineTextMetricA const& source ) noexcept;
      virtual void Copy( __in _OUTLINETEXTMETRICA const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ COutlineTextMetricA& operator=( __in COutlineTextMetricA const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class COutlineTextMetricW : public _OUTLINETEXTMETRICW
{
   public:

      COutlineTextMetricW();
      COutlineTextMetricW( __in COutlineTextMetricW const& source );
      COutlineTextMetricW( __in _OUTLINETEXTMETRICW const * source );
      virtual ~COutlineTextMetricW();
      virtual void Copy( __in COutlineTextMetricW const& source ) noexcept;
      virtual void Copy( __in _OUTLINETEXTMETRICW const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ COutlineTextMetricW& operator=( __in COutlineTextMetricW const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

#if defined( UNICODE )
#define COutlineTextMetric COutlineTextMetricW
#else
#define COutlineTextMetric COutlineTextMetricA
#endif // UNICODE

class CPixelFormatDescriptor : public tagPIXELFORMATDESCRIPTOR
{
   public:

      CPixelFormatDescriptor();
      CPixelFormatDescriptor( __in CPixelFormatDescriptor const& source );
      CPixelFormatDescriptor( __in tagPIXELFORMATDESCRIPTOR const * source );
      virtual ~CPixelFormatDescriptor();
      virtual void Copy( __in CPixelFormatDescriptor const& source ) noexcept;
      virtual void Copy( __in tagPIXELFORMATDESCRIPTOR const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CPixelFormatDescriptor& operator=( __in CPixelFormatDescriptor const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CRasterizerStatus : public _RASTERIZER_STATUS
{
   public:

      CRasterizerStatus();
      virtual ~CRasterizerStatus();
      virtual void Empty( void ) noexcept;
};

class CTextMetricA : public tagTEXTMETRICA
{
   public:

      CTextMetricA();
      CTextMetricA( __in CTextMetricA const& source );
      CTextMetricA( __in tagTEXTMETRICA const * source );
      virtual ~CTextMetricA();
      virtual void Copy( __in CTextMetricA const& source ) noexcept;
      virtual void Copy( __in tagTEXTMETRICA const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CTextMetricA& operator=( __in CTextMetricA const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CTextMetricW : public tagTEXTMETRICW
{
   public:

      CTextMetricW();
      CTextMetricW( __in CTextMetricW const& source );
      CTextMetricW( __in tagTEXTMETRICW const * source );
      virtual ~CTextMetricW();
      virtual void Copy( __in CTextMetricW const& source ) noexcept;
      virtual void Copy( __in tagTEXTMETRICW const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CTextMetricW& operator=( __in CTextMetricW const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

#if defined( UNICODE )
#define CTextMetric CTextMetricW
#else
#define CTextMetric CTextMetricA
#endif // UNICODE

class COFStruct : public _OFSTRUCT
{
   public:

      COFStruct();
      COFStruct( __in COFStruct const& source );
      COFStruct( __in _OFSTRUCT const * source );
      virtual ~COFStruct();
      virtual void Copy( __in COFStruct const& source ) noexcept;
      virtual void Copy( __in _OFSTRUCT const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ COFStruct& operator=( __in COFStruct const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CMemoryStatus : public _MEMORYSTATUS
{
   public:

      CMemoryStatus();
      CMemoryStatus( __in CMemoryStatus const& source );
      CMemoryStatus( __in MEMORYSTATUS const * source );
      virtual ~CMemoryStatus();
      virtual void Copy( __in CMemoryStatus const& source ) noexcept;
      virtual void Copy( __in MEMORYSTATUS const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CMemoryStatus& operator=( __in CMemoryStatus const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CSecurityQualityOfService : public _SECURITY_QUALITY_OF_SERVICE
{
   public:

      CSecurityQualityOfService();
      virtual ~CSecurityQualityOfService();
      virtual void Empty( void ) noexcept;
};

class CPerfCounterDefinition : public _PERF_COUNTER_DEFINITION
{
   public:

      CPerfCounterDefinition();
      virtual ~CPerfCounterDefinition();
      virtual void Empty( void ) noexcept;
};

class CPerfInstanceDefinition : public _PERF_INSTANCE_DEFINITION
{
   public:

      CPerfInstanceDefinition();
      virtual ~CPerfInstanceDefinition();
      virtual void Empty( void ) noexcept;
};

class CWindowPlacement : public tagWINDOWPLACEMENT
{
   public:

      CWindowPlacement();
      CWindowPlacement( __in CWindowPlacement const& source );
      CWindowPlacement( __in tagWINDOWPLACEMENT const * source );
      virtual ~CWindowPlacement();
      virtual void Copy( __in CWindowPlacement const& source ) noexcept;
      virtual void Copy( __in tagWINDOWPLACEMENT const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CWindowPlacement& operator=( __in CWindowPlacement const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CSecurityAttributes : public _SECURITY_ATTRIBUTES
{
   public:

      CSecurityAttributes();
      virtual ~CSecurityAttributes();
      virtual void Empty( void );
};

class CSystemAuditEntry : public _SYSTEM_AUDIT_ACE
{
   public:

      CSystemAuditEntry();
      CSystemAuditEntry( __in CSystemAuditEntry const& source );
      CSystemAuditEntry( __in _SYSTEM_AUDIT_ACE const * source );
      virtual ~CSystemAuditEntry();
      virtual void Copy( __in CSystemAuditEntry const& source ) noexcept;
      virtual void Copy( __in _SYSTEM_AUDIT_ACE const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ CSystemAuditEntry& operator=( __in CSystemAuditEntry const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

// 2000-11-10
// Thanks go to Danny Smith for adding GCC specific defines checking...

#if defined( _WINUSER_ ) || defined( _WINUSER_H )

class CFilterKeys : public tagFILTERKEYS
{
   public:

      CFilterKeys()
      {
         ::ZeroMemory( (tagFILTERKEYS *) this, sizeof( tagFILTERKEYS ) );
         cbSize = sizeof( tagFILTERKEYS );
      }

     ~CFilterKeys()
      {
         ::ZeroMemory( (tagFILTERKEYS *) this, sizeof( tagFILTERKEYS ) );
      }
};

class CStickyKeys : public tagSTICKYKEYS
{
   public:

      CStickyKeys()
      {
         ::ZeroMemory( (tagSTICKYKEYS *) this, sizeof( tagSTICKYKEYS ) );
         cbSize = sizeof( tagSTICKYKEYS );
      }

     ~CStickyKeys()
      {
         ::ZeroMemory( (tagSTICKYKEYS *) this, sizeof( tagSTICKYKEYS ) );
      }
};

class CMouseKeys : public tagMOUSEKEYS
{
   public:

      CMouseKeys()
      {
         ::ZeroMemory( (tagMOUSEKEYS *) this, sizeof( tagMOUSEKEYS ) );
         cbSize = sizeof( tagMOUSEKEYS );
      }

     ~CMouseKeys()
      {
         ::ZeroMemory( (tagMOUSEKEYS *) this, sizeof( tagMOUSEKEYS ) );
      }
};

class CToggleKeys : public tagTOGGLEKEYS
{
   public:

      CToggleKeys()
      {
         ::ZeroMemory( (tagTOGGLEKEYS *) this, sizeof( tagTOGGLEKEYS ) );
         cbSize = sizeof( tagTOGGLEKEYS );
      }

     ~CToggleKeys()
      {
         ::ZeroMemory( (tagTOGGLEKEYS *) this, sizeof( tagTOGGLEKEYS ) );
      }
};

class CAccessTimeout : public tagACCESSTIMEOUT
{
   public:

      CAccessTimeout()
      {
         ::ZeroMemory( (tagACCESSTIMEOUT *) this, sizeof( tagACCESSTIMEOUT ) );
         cbSize = sizeof( tagACCESSTIMEOUT );
      }

     ~CAccessTimeout()
      {
         ::ZeroMemory( (tagACCESSTIMEOUT *) this, sizeof( tagACCESSTIMEOUT ) );
      }
};

class CSoundSentryA : public tagSOUNDSENTRYA
{
   public:

      CSoundSentryA()
      {
         ::ZeroMemory( (tagSOUNDSENTRYA *) this, sizeof( tagSOUNDSENTRYA ) );
         cbSize = sizeof( tagSOUNDSENTRYA );
      }

     ~CSoundSentryA()
      {
         ::ZeroMemory( (tagSOUNDSENTRYA *) this, sizeof( tagSOUNDSENTRYA ) );
      }
};

class CSoundSentryW : public tagSOUNDSENTRYW
{
   public:

      CSoundSentryW()
      {
         ::ZeroMemory( (tagSOUNDSENTRYW *) this, sizeof( tagSOUNDSENTRYW ) );
         cbSize = sizeof( tagSOUNDSENTRYW );
      }

     ~CSoundSentryW()
      {
         ::ZeroMemory( (tagSOUNDSENTRYW *) this, sizeof( tagSOUNDSENTRYW ) );
      }
};

#if defined( UNICODE )
#define CSoundSentry CSoundSentryW
#else
#define CSoundSentry CSoundSentryA
#endif // UNICODE

#endif // _WINUSER_

#if defined( _INC_VFW )

class CCompVars : public COMPVARS
{
   public:

      CCompVars()
      {
         ::ZeroMemory( (COMPVARS *) this, sizeof( COMPVARS ) );
         cbSize = sizeof( COMPVARS );
      }

     ~CCompVars()
      {
         ::ZeroMemory( (COMPVARS *) this, sizeof( COMPVARS ) );
      }
};

#endif // _INC_VFW

// 2000-11-10
// Thanks go to Danny Smith for adding GCC specific defines checking...

#if defined( _WINGDI_ ) || defined( _WINGDI_H )

class CDocInfoA : public _DOCINFOA
{
   public:

      CDocInfoA()
      {
         ::ZeroMemory( (LPDOCINFOA) this, sizeof( DOCINFOA ) );
         cbSize = sizeof( DOCINFOA );
      }

     ~CDocInfoA()
      {
         ::ZeroMemory( (LPDOCINFOA) this, sizeof( DOCINFOA ) );
      }
};

class CDocInfoW : public _DOCINFOW
{
   public:

      CDocInfoW()
      {
         ::ZeroMemory( (LPDOCINFOW) this, sizeof( DOCINFOW ) );
         cbSize = sizeof( DOCINFOW );
      }

     ~CDocInfoW()
      {
         ::ZeroMemory( (LPDOCINFOW) this, sizeof( DOCINFOW ) );
      }
};

#if defined( UNICODE )
#define CDocInfo CDocInfoW
#else
#define CDocInfo CDocInfoA
#endif // UNICODE

#endif // _WINGDI_

class COperatingSystemVersionInformationA : public _OSVERSIONINFOA
{
   public:

      COperatingSystemVersionInformationA()
      {
         ::ZeroMemory( (POSVERSIONINFOA) this, sizeof( OSVERSIONINFOA ) );
         dwOSVersionInfoSize = sizeof( OSVERSIONINFOA );
      }

     ~COperatingSystemVersionInformationA()
      {
         ::ZeroMemory( (POSVERSIONINFOA) this, sizeof( OSVERSIONINFOA ) );
      }

      _Check_return_ bool Fill( void )
      {
#pragma warning( push )
#pragma warning(disable: 4996)
          return( ::GetVersionExA( this ) == FALSE ? false : true );
#pragma warning( pop )
      }

      operator POSVERSIONINFOA(){ return( (POSVERSIONINFOA) this ); };
};

class COperatingSystemVersionInformationW : public _OSVERSIONINFOW
{
   public:

      COperatingSystemVersionInformationW()
      {
         ::ZeroMemory( (POSVERSIONINFOW) this, sizeof( OSVERSIONINFOW ) );
         dwOSVersionInfoSize = sizeof( OSVERSIONINFOW );
      }

     ~COperatingSystemVersionInformationW()
      {
         ::ZeroMemory( (POSVERSIONINFOW) this, sizeof( OSVERSIONINFOW ) );
      }

      _Check_return_ bool Fill( void )
      {
#pragma warning( push )
#pragma warning(disable: 4996)
          return( ::GetVersionExW( this ) == FALSE ? false : true );
#pragma warning( pop )
      }

      operator POSVERSIONINFOW(){ return( (POSVERSIONINFOW) this ); };
};

#if defined( UNICODE )
#define COperatingSystemVersionInformation COperatingSystemVersionInformationW
#else
#define COperatingSystemVersionInformation COperatingSystemVersionInformationA
#endif // UNICODE

class CStartupInformationA : public _STARTUPINFOA
{
   public:

      CStartupInformationA()
      {
         ::ZeroMemory( (LPSTARTUPINFOA) this, sizeof( _STARTUPINFOA ) );
         cb = sizeof( _STARTUPINFOA );
      }

     ~CStartupInformationA()
      {
         ::ZeroMemory( (LPSTARTUPINFOA) this, sizeof( _STARTUPINFOA ) );
      }

      operator LPSTARTUPINFOA(){ return( (LPSTARTUPINFOA) this ); };
};

class CStartupInformationW : public _STARTUPINFOW
{
   public:

      CStartupInformationW()
      {
         ::ZeroMemory( (LPSTARTUPINFOW) this, sizeof( _STARTUPINFOW ) );
         cb = sizeof( _STARTUPINFOW );
      }

     ~CStartupInformationW()
      {
         ::ZeroMemory( (LPSTARTUPINFOW) this, sizeof( _STARTUPINFOW ) );
      }
     
     inline constexpr void HideWindow(void) noexcept
     {
         dwFlags = STARTF_USESHOWWINDOW;
         wShowWindow = SW_HIDE;
     }

     inline constexpr void NormalWindow(void) noexcept
     {
         dwFlags = STARTF_USESHOWWINDOW;
         wShowWindow = SW_SHOWNORMAL;
     }
     
     operator LPSTARTUPINFOW(){ return( (LPSTARTUPINFOW) this ); };
};

#if defined( UNICODE )
#define CStartupInformation CStartupInformationW
#else
#define CStartupInformation CStartupInformationA
#endif // UNICODE

#endif // STRUCTS_CLASS_HEADER
