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
      CAccessAllowedEntry( __in const CAccessAllowedEntry& source );
      CAccessAllowedEntry( __in const ACCESS_ALLOWED_ACE * source );
      virtual ~CAccessAllowedEntry();
      virtual void Copy( __in const CAccessAllowedEntry& source );
      virtual void Copy( __in const ACCESS_ALLOWED_ACE * source );
      virtual void Empty( void );
      virtual __checkReturn CAccessAllowedEntry& operator=( __in const CAccessAllowedEntry& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CAccessControlEntryHeader : public _ACE_HEADER
{
   public:

      CAccessControlEntryHeader();
      CAccessControlEntryHeader( __in const CAccessControlEntryHeader& source );
      CAccessControlEntryHeader( __in const ACE_HEADER * source );
      virtual ~CAccessControlEntryHeader();
      virtual void Copy( __in const CAccessControlEntryHeader& source );
      virtual void Copy( __in const ACE_HEADER * source );
      virtual void Empty( void );
      virtual __checkReturn CAccessControlEntryHeader& operator=( __in const CAccessControlEntryHeader& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CAccessControlList : public _ACL
{
   public:

      CAccessControlList();
      CAccessControlList( __in const CAccessControlList& source );
      CAccessControlList( __in const ACL * source );
      virtual ~CAccessControlList();
      virtual void Copy( __in const CAccessControlList& source );
      virtual void Copy( __in const ACL * source );
      virtual void Empty( void );
      virtual __checkReturn CAccessControlList& operator=( __in const CAccessControlList& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CAccessDeniedEntry : public _ACCESS_DENIED_ACE
{
   public:

      CAccessDeniedEntry();
      CAccessDeniedEntry( __in const CAccessDeniedEntry& source );
      CAccessDeniedEntry( __in const ACCESS_DENIED_ACE * source );
      virtual ~CAccessDeniedEntry();
      virtual void Copy( __in const CAccessDeniedEntry& source );
      virtual void Copy( __in const ACCESS_DENIED_ACE * source );
      virtual void Empty( void );
      virtual __checkReturn CAccessDeniedEntry& operator=( __in const CAccessDeniedEntry& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CBitmapCoreHeader : public tagBITMAPCOREHEADER
{
   public:

      CBitmapCoreHeader();
      CBitmapCoreHeader( __in const CBitmapCoreHeader& source );
      CBitmapCoreHeader( __in const tagBITMAPCOREHEADER * source );
      virtual ~CBitmapCoreHeader();
      virtual void Copy( __in const CBitmapCoreHeader& source );
      virtual void Copy( __in const tagBITMAPCOREHEADER * source );
      virtual void Empty( void );
      virtual __checkReturn CBitmapCoreHeader& operator=( __in const CBitmapCoreHeader& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CBitmapFileHeader : public tagBITMAPFILEHEADER
{
   public:

      CBitmapFileHeader();
      CBitmapFileHeader( __in const CBitmapFileHeader& source );
      CBitmapFileHeader( __in const tagBITMAPFILEHEADER * source );
      virtual ~CBitmapFileHeader();
      virtual void Copy( __in const CBitmapFileHeader& source );
      virtual void Copy( __in const tagBITMAPFILEHEADER * source );
      virtual void Empty( void );
      virtual __checkReturn CBitmapFileHeader& operator=( __in const CBitmapFileHeader& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CBitmapInfoHeader : public tagBITMAPINFOHEADER
{
   public:

      CBitmapInfoHeader();
      CBitmapInfoHeader( __in const CBitmapInfoHeader& source );
      CBitmapInfoHeader( __in const tagBITMAPINFOHEADER * source );
      virtual ~CBitmapInfoHeader();
      virtual void Copy( __in const CBitmapInfoHeader& source );
      virtual void Copy( __in const tagBITMAPINFOHEADER * source );
      virtual void Empty( void );
      virtual __checkReturn CBitmapInfoHeader& operator=( __in const CBitmapInfoHeader& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CColorAdjustment : public tagCOLORADJUSTMENT
{
   public:

      CColorAdjustment();
      CColorAdjustment( __in const CColorAdjustment& source );
      CColorAdjustment( __in const tagCOLORADJUSTMENT * source );
      virtual ~CColorAdjustment();
      virtual void Copy( __in const CColorAdjustment& source );
      virtual void Copy( __in const tagCOLORADJUSTMENT * source );
      virtual void Empty( void );
      virtual __checkReturn CColorAdjustment& operator=( __in const CColorAdjustment& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CCommunicationsConfiguration : public _COMMCONFIG
{
   public:

      CCommunicationsConfiguration();
      CCommunicationsConfiguration( __in const CCommunicationsConfiguration& source );
      CCommunicationsConfiguration( __in const COMMCONFIG& source );
      virtual ~CCommunicationsConfiguration();

      virtual void Copy( __in const CCommunicationsConfiguration& source );
      virtual void Copy( __in const COMMCONFIG& source );
      virtual void Empty( void );

      virtual __checkReturn CCommunicationsConfiguration& operator=( __in const CCommunicationsConfiguration& source );
      virtual __checkReturn CCommunicationsConfiguration& operator=( __in const COMMCONFIG& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CCommunicationProperties : public _COMMPROP
{

   public:

      CCommunicationProperties();
      CCommunicationProperties( __in const CCommunicationProperties& source );
      CCommunicationProperties( __in const _COMMPROP& source );
      virtual ~CCommunicationProperties();
      virtual void ConvertBaudRateToString( __in const DWORD baud_rate, __out std::wstring& human_readable_string ) const;
      virtual void ConvertBaudRateListToString( __in const DWORD baud_rate, __out std::wstring& human_readable_string ) const;
      virtual void ConvertDataBitsToString( __in const DWORD data_bits, __out std::wstring& human_readable_string ) const;
      virtual void ConvertProviderCapabilitiesToString( __in const DWORD capabilities, __out std::wstring& human_readable_string ) const;
      virtual void ConvertProviderSubtypeToString( __in const DWORD baud_rate, __out std::wstring& human_readable_string ) const;
      virtual void ConvertSettableParametersToString( __in const DWORD parameters, __out std::wstring& human_readable_string ) const;
      virtual void ConvertStopParityToString( __in const DWORD stop_parity, __out std::wstring& human_readable_string ) const;
      virtual void Copy( __in const CCommunicationProperties& source );
      virtual void Copy( __in const _COMMPROP& source );
      virtual void Empty( void );
      virtual __checkReturn CCommunicationProperties& operator=( __in const CCommunicationProperties& source );
      virtual __checkReturn CCommunicationProperties& operator=( __in const _COMMPROP& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class COutlineTextMetricA : public _OUTLINETEXTMETRICA
{
   public:

      COutlineTextMetricA();
      COutlineTextMetricA( __in const COutlineTextMetricA& source );
      COutlineTextMetricA( __in const _OUTLINETEXTMETRICA * source );
      virtual ~COutlineTextMetricA();
      virtual void Copy( __in const COutlineTextMetricA& source );
      virtual void Copy( __in const _OUTLINETEXTMETRICA * source );
      virtual void Empty( void );
      virtual __checkReturn COutlineTextMetricA& operator=( __in const COutlineTextMetricA& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class COutlineTextMetricW : public _OUTLINETEXTMETRICW
{
   public:

      COutlineTextMetricW();
      COutlineTextMetricW( __in const COutlineTextMetricW& source );
      COutlineTextMetricW( __in const _OUTLINETEXTMETRICW * source );
      virtual ~COutlineTextMetricW();
      virtual void Copy( __in const COutlineTextMetricW& source );
      virtual void Copy( __in const _OUTLINETEXTMETRICW * source );
      virtual void Empty( void );
      virtual __checkReturn COutlineTextMetricW& operator=( __in const COutlineTextMetricW& source );

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
      CPixelFormatDescriptor( __in const CPixelFormatDescriptor& source );
      CPixelFormatDescriptor( __in const tagPIXELFORMATDESCRIPTOR * source );
      virtual ~CPixelFormatDescriptor();
      virtual void Copy( __in const CPixelFormatDescriptor& source );
      virtual void Copy( __in const tagPIXELFORMATDESCRIPTOR * source );
      virtual void Empty( void );
      virtual __checkReturn CPixelFormatDescriptor& operator=( __in const CPixelFormatDescriptor& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CRasterizerStatus : public _RASTERIZER_STATUS
{
   public:

      CRasterizerStatus();
      virtual ~CRasterizerStatus();
      virtual void Empty( void );
};

class CTextMetricA : public tagTEXTMETRICA
{
   public:

      CTextMetricA();
      CTextMetricA( __in const CTextMetricA& source );
      CTextMetricA( __in const tagTEXTMETRICA * source );
      virtual ~CTextMetricA();
      virtual void Copy( __in const CTextMetricA& source );
      virtual void Copy( __in const tagTEXTMETRICA * source );
      virtual void Empty( void );
      virtual __checkReturn CTextMetricA& operator=( __in const CTextMetricA& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CTextMetricW : public tagTEXTMETRICW
{
   public:

      CTextMetricW();
      CTextMetricW( __in const CTextMetricW& source );
      CTextMetricW( __in const tagTEXTMETRICW * source );
      virtual ~CTextMetricW();
      virtual void Copy( __in const CTextMetricW& source );
      virtual void Copy( __in const tagTEXTMETRICW * source );
      virtual void Empty( void );
      virtual __checkReturn CTextMetricW& operator=( __in const CTextMetricW& source );

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
      COFStruct( __in const COFStruct& source );
      COFStruct( __in const _OFSTRUCT * source );
      virtual ~COFStruct();
      virtual void Copy( __in const COFStruct& source );
      virtual void Copy( __in const _OFSTRUCT * source );
      virtual void Empty( void );
      virtual __checkReturn COFStruct& operator=( __in const COFStruct& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CMemoryStatus : public _MEMORYSTATUS
{
   public:

      CMemoryStatus();
      CMemoryStatus( __in const CMemoryStatus& source );
      CMemoryStatus( __in const MEMORYSTATUS * source );
      virtual ~CMemoryStatus();
      virtual void Copy( __in const CMemoryStatus & source );
      virtual void Copy( __in const MEMORYSTATUS * source );
      virtual void Empty( void );
      virtual __checkReturn CMemoryStatus& operator=( __in const CMemoryStatus& source );

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CSecurityQualityOfService : public _SECURITY_QUALITY_OF_SERVICE
{
   public:

      CSecurityQualityOfService();
      virtual ~CSecurityQualityOfService();
      virtual void Empty( void );
};

class CPerfCounterDefinition : public _PERF_COUNTER_DEFINITION
{
   public:

      CPerfCounterDefinition();
      virtual ~CPerfCounterDefinition();
      virtual void Empty( void );
};

class CPerfInstanceDefinition : public _PERF_INSTANCE_DEFINITION
{
   public:

      CPerfInstanceDefinition();
      virtual ~CPerfInstanceDefinition();
      virtual void Empty( void );
};

class CWindowPlacement : public tagWINDOWPLACEMENT
{
   public:

      CWindowPlacement();
      CWindowPlacement( __in const CWindowPlacement& source );
      CWindowPlacement( __in const tagWINDOWPLACEMENT * source );
      virtual ~CWindowPlacement();
      virtual void Copy( __in const CWindowPlacement& source );
      virtual void Copy( __in const tagWINDOWPLACEMENT * source );
      virtual void Empty( void );
      virtual __checkReturn CWindowPlacement& operator=( __in const CWindowPlacement& source );

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
      CSystemAuditEntry( __in const CSystemAuditEntry& source );
      CSystemAuditEntry( __in const _SYSTEM_AUDIT_ACE * source );
      virtual ~CSystemAuditEntry();
      virtual void Copy( __in const CSystemAuditEntry& source );
      virtual void Copy( __in const _SYSTEM_AUDIT_ACE * source );
      virtual void Empty( void );
      virtual __checkReturn CSystemAuditEntry& operator=( __in const CSystemAuditEntry& source );

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

      __checkReturn bool Fill( void )
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

      __checkReturn bool Fill( void )
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
