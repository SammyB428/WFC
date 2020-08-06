/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2020, Samuel R. Blackburn
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

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STRUCTS_CLASS_HEADER )

#define STRUCTS_CLASS_HEADER

class CAccessAllowedEntry : public _ACCESS_ALLOWED_ACE
{
   public:

       inline CAccessAllowedEntry() noexcept { Empty(); }
       inline explicit CAccessAllowedEntry(_In_ CAccessAllowedEntry const& source) noexcept { Copy(source); }
       inline explicit CAccessAllowedEntry(_In_ ACCESS_ALLOWED_ACE const* source) noexcept { Copy(source); }

      inline constexpr void Copy(_In_ CAccessAllowedEntry const& source) noexcept
      {
          Header.AceType = source.Header.AceType;
          Header.AceFlags = source.Header.AceFlags;
          Header.AceSize = source.Header.AceSize;
          Mask = source.Mask;
          SidStart = source.SidStart;
      }

      inline constexpr void Copy(_In_ ACCESS_ALLOWED_ACE const* source) noexcept
      {
          Header.AceType = source->Header.AceType;
          Header.AceFlags = source->Header.AceFlags;
          Header.AceSize = source->Header.AceSize;
          Mask = source->Mask;
          SidStart = source->SidStart;
      }

      inline constexpr void Empty(void) noexcept
      {
          Header.AceType = 0;
          Header.AceFlags = 0;
          Header.AceSize = 0;
          Mask = 0;
          SidStart = 0;
      }

      inline _Check_return_ CAccessAllowedEntry& operator=(_In_ CAccessAllowedEntry const& source) noexcept { Copy(source); }

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CAccessControlEntryHeader : public _ACE_HEADER
{
   public:

      CAccessControlEntryHeader() noexcept;
      explicit CAccessControlEntryHeader( _In_ CAccessControlEntryHeader const& source ) noexcept;
      explicit CAccessControlEntryHeader( _In_ ACE_HEADER const * source ) noexcept;
      void Copy( _In_ CAccessControlEntryHeader const& source ) noexcept;
      void Copy( _In_ ACE_HEADER const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CAccessControlEntryHeader& operator=( _In_ CAccessControlEntryHeader const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class CAccessControlList : public _ACL
{
   public:

      CAccessControlList() noexcept;
      explicit CAccessControlList( _In_ CAccessControlList const& source ) noexcept;
      explicit CAccessControlList( _In_ ACL const * source ) noexcept;
      void Copy( _In_ CAccessControlList const& source ) noexcept;
      void Copy( _In_ ACL const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CAccessControlList& operator=( _In_ CAccessControlList const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class CAccessDeniedEntry : public _ACCESS_DENIED_ACE
{
   public:

      CAccessDeniedEntry() noexcept;
      explicit CAccessDeniedEntry( _In_ CAccessDeniedEntry const& source ) noexcept;
      explicit CAccessDeniedEntry( _In_ ACCESS_DENIED_ACE const * source ) noexcept;
      void Copy( _In_ CAccessDeniedEntry const& source ) noexcept;
      void Copy( _In_ ACCESS_DENIED_ACE const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CAccessDeniedEntry& operator=( _In_ CAccessDeniedEntry const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class CBitmapCoreHeader : public tagBITMAPCOREHEADER
{
   public:

      CBitmapCoreHeader() noexcept;
      explicit CBitmapCoreHeader( _In_ CBitmapCoreHeader const& source ) noexcept;
      explicit CBitmapCoreHeader( _In_ tagBITMAPCOREHEADER const * source ) noexcept;
      void Copy( _In_ CBitmapCoreHeader const& source ) noexcept;
      void Copy( _In_ tagBITMAPCOREHEADER const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CBitmapCoreHeader& operator=( _In_ CBitmapCoreHeader const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class CBitmapFileHeader : public tagBITMAPFILEHEADER
{
   public:

      CBitmapFileHeader() noexcept;
      explicit CBitmapFileHeader( _In_ CBitmapFileHeader const& source ) noexcept;
      explicit CBitmapFileHeader( _In_ tagBITMAPFILEHEADER const * source ) noexcept;
      void Copy( _In_ CBitmapFileHeader const& source ) noexcept;
      void Copy( _In_ tagBITMAPFILEHEADER const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CBitmapFileHeader& operator=( _In_ CBitmapFileHeader const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class CBitmapInfoHeader : public tagBITMAPINFOHEADER
{
   public:

      CBitmapInfoHeader() noexcept;
      explicit CBitmapInfoHeader( _In_ CBitmapInfoHeader const& source ) noexcept;
      explicit CBitmapInfoHeader( _In_ tagBITMAPINFOHEADER const * source ) noexcept;
      void Copy( _In_ CBitmapInfoHeader const& source ) noexcept;
      void Copy( _In_ tagBITMAPINFOHEADER const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CBitmapInfoHeader& operator=( _In_ CBitmapInfoHeader const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class CColorAdjustment : public tagCOLORADJUSTMENT
{
   public:

      CColorAdjustment() noexcept;
      explicit CColorAdjustment( _In_ CColorAdjustment const& source ) noexcept;
      explicit CColorAdjustment( _In_ tagCOLORADJUSTMENT const * source ) noexcept;
      void Copy( _In_ CColorAdjustment const& source ) noexcept;
      void Copy( _In_ tagCOLORADJUSTMENT const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CColorAdjustment& operator=( _In_ CColorAdjustment const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class CCommunicationsConfiguration : public _COMMCONFIG
{
   public:

      CCommunicationsConfiguration() noexcept;
      explicit CCommunicationsConfiguration( _In_ CCommunicationsConfiguration const& source ) noexcept;
      explicit CCommunicationsConfiguration( _In_ COMMCONFIG const& source ) noexcept;

      void Copy( _In_ CCommunicationsConfiguration const& source ) noexcept;
      void Copy( _In_ COMMCONFIG const& source ) noexcept;
      void Empty( void ) noexcept;

      _Check_return_ CCommunicationsConfiguration& operator=( _In_ CCommunicationsConfiguration const& source ) noexcept;
      _Check_return_ CCommunicationsConfiguration& operator=( _In_ COMMCONFIG const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

class CCommunicationProperties : public _COMMPROP
{

   public:

      CCommunicationProperties() noexcept;
      explicit CCommunicationProperties( _In_ CCommunicationProperties const& source ) noexcept;
      explicit CCommunicationProperties( _In_ _COMMPROP const& source ) noexcept;
      void ConvertBaudRateToString( _In_ DWORD const baud_rate, _Out_ std::wstring& human_readable_string ) const noexcept;
      void ConvertBaudRateListToString( _In_ DWORD const baud_rate, _Out_ std::wstring& human_readable_string ) const noexcept;
      void ConvertDataBitsToString( _In_ DWORD const data_bits, _Out_ std::wstring& human_readable_string ) const noexcept;
      void ConvertProviderCapabilitiesToString( _In_ DWORD const capabilities, _Out_ std::wstring& human_readable_string ) const noexcept;
      void ConvertProviderSubtypeToString( _In_ DWORD const baud_rate, _Out_ std::wstring& human_readable_string ) const noexcept;
      void ConvertSettableParametersToString( _In_ DWORD const parameters, _Out_ std::wstring& human_readable_string ) const noexcept;
      void ConvertStopParityToString( _In_ DWORD const stop_parity, _Out_ std::wstring& human_readable_string ) const noexcept;
      void Copy( _In_ CCommunicationProperties const& source ) noexcept;
      void Copy( _In_ _COMMPROP const& source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CCommunicationProperties& operator=( _In_ CCommunicationProperties const& source ) noexcept;
      _Check_return_ CCommunicationProperties& operator=( _In_ _COMMPROP const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class COutlineTextMetricA : public _OUTLINETEXTMETRICA
{
   public:

      COutlineTextMetricA() noexcept;
      explicit COutlineTextMetricA( _In_ COutlineTextMetricA const& source ) noexcept;
      explicit COutlineTextMetricA( _In_ _OUTLINETEXTMETRICA const * source ) noexcept;
      void Copy( _In_ COutlineTextMetricA const& source ) noexcept;
      void Copy( _In_ _OUTLINETEXTMETRICA const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ COutlineTextMetricA& operator=( _In_ COutlineTextMetricA const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class COutlineTextMetricW : public _OUTLINETEXTMETRICW
{
   public:

      COutlineTextMetricW() noexcept;
      explicit COutlineTextMetricW( _In_ COutlineTextMetricW const& source ) noexcept;
      explicit COutlineTextMetricW( _In_ _OUTLINETEXTMETRICW const * source ) noexcept;

      void Copy( _In_ COutlineTextMetricW const& source ) noexcept;
      void Copy( _In_ _OUTLINETEXTMETRICW const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ COutlineTextMetricW& operator=( _In_ COutlineTextMetricW const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
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

      CPixelFormatDescriptor() noexcept;
      explicit CPixelFormatDescriptor( _In_ CPixelFormatDescriptor const& source ) noexcept;
      explicit CPixelFormatDescriptor( _In_ tagPIXELFORMATDESCRIPTOR const * source ) noexcept;
      void Copy( _In_ CPixelFormatDescriptor const& source ) noexcept;
      void Copy( _In_ tagPIXELFORMATDESCRIPTOR const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CPixelFormatDescriptor& operator=( _In_ CPixelFormatDescriptor const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class CRasterizerStatus : public _RASTERIZER_STATUS
{
   public:

      CRasterizerStatus() noexcept;
      void Empty( void ) noexcept;
};

class CTextMetricA : public tagTEXTMETRICA
{
   public:

      CTextMetricA() noexcept;
      explicit CTextMetricA( _In_ CTextMetricA const& source ) noexcept;
      explicit CTextMetricA( _In_ tagTEXTMETRICA const * source ) noexcept;
      void Copy( _In_ CTextMetricA const& source ) noexcept;
      void Copy( _In_ tagTEXTMETRICA const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CTextMetricA& operator=( _In_ CTextMetricA const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class CTextMetricW : public tagTEXTMETRICW
{
   public:

      CTextMetricW() noexcept;
      explicit CTextMetricW( _In_ CTextMetricW const& source ) noexcept;
      explicit CTextMetricW( _In_ tagTEXTMETRICW const * source ) noexcept;
      void Copy( _In_ CTextMetricW const& source ) noexcept;
      void Copy( _In_ tagTEXTMETRICW const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CTextMetricW& operator=( _In_ CTextMetricW const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
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

      COFStruct() noexcept;
      explicit COFStruct( _In_ COFStruct const& source ) noexcept;
      explicit COFStruct( _In_ _OFSTRUCT const * source ) noexcept;
      void Copy( _In_ COFStruct const& source ) noexcept;
      void Copy( _In_ _OFSTRUCT const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ COFStruct& operator=( _In_ COFStruct const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class CMemoryStatus : public _MEMORYSTATUS
{
   public:

      CMemoryStatus() noexcept;
      explicit CMemoryStatus( _In_ CMemoryStatus const& source ) noexcept;
      explicit CMemoryStatus( _In_ MEMORYSTATUS const * source ) noexcept;
      void Copy( _In_ CMemoryStatus const& source ) noexcept;
      void Copy( _In_ MEMORYSTATUS const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CMemoryStatus& operator=( _In_ CMemoryStatus const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class CSecurityQualityOfService : public _SECURITY_QUALITY_OF_SERVICE
{
   public:

      CSecurityQualityOfService() noexcept;
      void Empty( void ) noexcept;
};

class CPerfCounterDefinition : public _PERF_COUNTER_DEFINITION
{
   public:

      CPerfCounterDefinition() noexcept;
      void Empty( void ) noexcept;
};

class CPerfInstanceDefinition : public _PERF_INSTANCE_DEFINITION
{
   public:

      CPerfInstanceDefinition() noexcept;
      void Empty( void ) noexcept;
};

class CWindowPlacement : public tagWINDOWPLACEMENT
{
   public:

      CWindowPlacement() noexcept;
      explicit CWindowPlacement( _In_ CWindowPlacement const& source ) noexcept;
      explicit CWindowPlacement( _In_ tagWINDOWPLACEMENT const * source ) noexcept;

      void Copy( _In_ CWindowPlacement const& source ) noexcept;
      void Copy( _In_ tagWINDOWPLACEMENT const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CWindowPlacement& operator=( _In_ CWindowPlacement const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

class CSecurityAttributes : public _SECURITY_ATTRIBUTES
{
   public:

      CSecurityAttributes() noexcept;
      void Empty( void ) noexcept;
};

class CSystemAuditEntry : public _SYSTEM_AUDIT_ACE
{
   public:

      CSystemAuditEntry() noexcept;
      explicit CSystemAuditEntry( _In_ CSystemAuditEntry const& source ) noexcept;
      explicit CSystemAuditEntry( _In_ _SYSTEM_AUDIT_ACE const * source ) noexcept;

      void Copy( _In_ CSystemAuditEntry const& source ) noexcept;
      void Copy( _In_ _SYSTEM_AUDIT_ACE const * source ) noexcept;
      void Empty( void ) noexcept;
      _Check_return_ CSystemAuditEntry& operator=( _In_ CSystemAuditEntry const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      void Dump( CDumpContext& dump_context ) const;
#endif
};

// 2000-11-10
// Thanks go to Danny Smith for adding GCC specific defines checking...

#if defined( _WINUSER_ ) || defined( _WINUSER_H )

class CFilterKeys : public tagFILTERKEYS
{
   public:

     inline constexpr CFilterKeys() noexcept : tagFILTERKEYS{ sizeof(tagFILTERKEYS), 0, 0, 0, 0, 0 } {}
};

class CStickyKeys : public tagSTICKYKEYS
{
   public:

      inline constexpr CStickyKeys() noexcept : tagSTICKYKEYS{ sizeof(tagSTICKYKEYS), 0 } {}
};

class CMouseKeys : public tagMOUSEKEYS
{
   public:

      inline constexpr CMouseKeys() noexcept : tagMOUSEKEYS{ sizeof(tagMOUSEKEYS), 0, 0, 0, 0, 0, 0 } {}
};

class CToggleKeys : public tagTOGGLEKEYS
{
   public:

      inline constexpr CToggleKeys() noexcept : tagTOGGLEKEYS{ sizeof(tagTOGGLEKEYS), 0 } {}
};

class CAccessTimeout : public tagACCESSTIMEOUT
{
   public:

      inline constexpr CAccessTimeout() noexcept : tagACCESSTIMEOUT{ sizeof(tagACCESSTIMEOUT), 0, 0} {}
};

class CSoundSentryA : public tagSOUNDSENTRYA
{
   public:

      inline CSoundSentryA() noexcept
      {
         ::ZeroMemory( (tagSOUNDSENTRYA *) this, sizeof( tagSOUNDSENTRYA ) );
         cbSize = sizeof( tagSOUNDSENTRYA );
      }
};

class CSoundSentryW : public tagSOUNDSENTRYW
{
   public:

      inline CSoundSentryW() noexcept
      {
         ::ZeroMemory( (tagSOUNDSENTRYW *) this, sizeof( tagSOUNDSENTRYW ) );
         cbSize = sizeof( tagSOUNDSENTRYW );
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
};

#endif // _INC_VFW

// 2000-11-10
// Thanks go to Danny Smith for adding GCC specific defines checking...

#if defined( _WINGDI_ ) || defined( _WINGDI_H )

class CDocInfoA : public _DOCINFOA
{
   public:

      inline CDocInfoA() noexcept
      {
         ::ZeroMemory( (LPDOCINFOA) this, sizeof( DOCINFOA ) );
         cbSize = sizeof( DOCINFOA );
      }
};

class CDocInfoW : public _DOCINFOW
{
   public:

      inline CDocInfoW() noexcept
      {
         ::ZeroMemory( (LPDOCINFOW) this, sizeof( DOCINFOW ) );
         cbSize = sizeof( DOCINFOW );
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

      inline constexpr COperatingSystemVersionInformationA() noexcept : _OSVERSIONINFOA{ sizeof(OSVERSIONINFOA), 0, 0, 0, 0, 0 } {}

      inline _Check_return_ bool Fill( void ) noexcept
      {
#pragma warning( push )
#pragma warning(disable: 4996 28159)
          return( ::GetVersionExA( this ) == FALSE ? false : true );
#pragma warning( pop )
      }

      inline operator POSVERSIONINFOA() noexcept { return( (POSVERSIONINFOA) this ); };
};

class COperatingSystemVersionInformationW : public _OSVERSIONINFOW
{
   public:

      inline constexpr COperatingSystemVersionInformationW() noexcept : _OSVERSIONINFOW{ sizeof(OSVERSIONINFOW), 0, 0, 0, 0, 0 } {}

      inline _Check_return_ bool Fill( void ) noexcept
      {
#pragma warning( push )
#pragma warning(disable: 4996 28159)

          return( ::GetVersionExW( this ) == FALSE ? false : true );
#pragma warning( pop )
      }

      inline operator POSVERSIONINFOW() noexcept { return( (POSVERSIONINFOW) this ); };
};

#if defined( UNICODE )
#define COperatingSystemVersionInformation COperatingSystemVersionInformationW
#else
#define COperatingSystemVersionInformation COperatingSystemVersionInformationA
#endif // UNICODE

class CStartupInformationA : public _STARTUPINFOA
{
   public:

      inline CStartupInformationA() noexcept
      {
         ::ZeroMemory( (LPSTARTUPINFOA) this, sizeof( _STARTUPINFOA ) );
         cb = sizeof( _STARTUPINFOA );
      }

      operator LPSTARTUPINFOA() noexcept { return( (LPSTARTUPINFOA) this ); };
};

class CStartupInformationW : public _STARTUPINFOW
{
   public:

      inline CStartupInformationW() noexcept
      {
         ::ZeroMemory( (LPSTARTUPINFOW) this, sizeof( _STARTUPINFOW ) );
         cb = sizeof( _STARTUPINFOW );
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
     
     operator LPSTARTUPINFOW() noexcept { return( (LPSTARTUPINFOW) this ); };
};

#if defined( UNICODE )
#define CStartupInformation CStartupInformationW
#else
#define CStartupInformation CStartupInformationA
#endif // UNICODE

#endif // STRUCTS_CLASS_HEADER
