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
** $Workfile: wfc.h $
** $Revision: 76 $
** $Modtime: 1/05/02 7:14a $
*/

#if ! defined( WIN32_FOUNDATION_CLASSES ) && ! defined( SUBSET_STLXML )

#define WIN32_FOUNDATION_CLASSES

#define WFC_RELEASE_NUMBER (77)
#if ! defined( WFC_SILENT )
#pragma message( "WFC Release 77." )
#endif // WFC_SILENT

#define TAB_CHARACTER   (9)
#define CARRIAGE_RETURN (13)
#define LINE_FEED       (10)

#pragma warning( disable: 4710 )  /* function selected for inlining */
#pragma warning( disable: 4711 )  /* function selected for inlining */
#pragma warning( disable: 4350 )  /* STL behavior change */

#include <SDKDDKVer.h>

#if ! defined( ONE_KILOBYTE )
#define ONE_KILOBYTE (1024)
#endif // ONE_KILOBYTE

#if ! defined( ONE_MEGABYTE )
#define ONE_MEGABYTE (1048576)
#endif // ONE_MEGABYTE

#if ! defined( ONE_GIGABYTE )
#define ONE_GIGABYTE (1073741824)
#endif // ONE_GIGABYTE

#if ! defined( ONE_TERABYTE )
#define ONE_TERABYTE (1099511627776I64)
#endif // ONE_TERABYTE

#if ! defined( ONE_PETABYTE )
#define ONE_PETABYTE (1125899906842624I64)
#endif // ONE_PETABYTE

#if ! defined( ONE_EXABYTE )
#define ONE_EXABYTE (1152921504606846976I64)
#endif // ONE_EXABYTE

#if ( _MSC_VER > 1000 )
#pragma once
#endif // _MSV_VER

#if defined( UNICODE )
#if ! defined( _UNICODE )
#define _UNICODE
#endif // _UNICODE
#endif // UNICODE

#if defined( _UNICODE )
#if ! defined( UNICODE )
#define UNICODE
#endif // UNICODE
#endif // _UNICODE

#if ( WINVER < 0x401 )
#pragma message( "WARNING! You are compiling for a version of NT before 4.0, you may have problems in the RAS stuff." )
#pragma message( "You will probably have problems in the RAS stuff." )
#pragma message( "To get rid of this warning, add WINVER=0x401 to the Preprocessor definitions in the project settings." )
#else
#if ! defined( _WIN32_WINNT )
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#endif // _WIN32_WINNT
#endif // WINVER

#if ( WINVER >= 0x0401 ) && ! defined( _WIN32_WINNT )
#pragma message( "Enabling TryEnterCriticalSection compilation." )
//#define _WIN32_WINNT 0x0400
#elif ! defined( _WIN32_WINNT ) || ( _WIN32_WINNT < 0x400 )
#pragma message( "Warning! TryEnterCriticalSection will not be compiled. #define _WIN32_WINNT to be 0x400 or greater before including wfc.h." )
#endif

/*
** Need to define NO_ANSI_ONLY (an undocumented "feature", someone in Redmond let the
** children play in the header files again...)
*/

#define NO_ANSIUNI_ONLY

/*
** Yet another undocumented feature we gotta worry about
*/

#if defined( WIN32_LEAN_AND_MEAN )
#undef WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

/*
** Need to un-define VC_EXTRALEAN, another undocumented feature...
*/

#if defined( VC_EXTRALEAN )
#undef VC_EXTRALEAN
#endif // _VC_EXTRALEAN

#if defined( _DEBUG )
#define _CRTDBG_MAP_ALLOC
#endif // _DEBUG

#if ! defined( WFC_DEPRECATED )
#if ! defined( _WIN64 ) && ( _MSC_VER >= 1300 )
#define WFC_DEPRECATED __declspec(deprecated)
#else // _WIN64
#define WFC_DEPRECATED
#endif // _WIN64
#endif // WFC_DEPRECATED

#define ANY_SIZE_ARRAY (ANYSIZE_ARRAY)

#include "wfc_exceptions.h"

#if ! defined( _WIN32_WINNT )
#define _WIN32_WINNT 0x0602
#endif

#if defined( WFC_STL )
// We are building an MFC-free version of the library

#if ! defined( WFC_USE_SMALL_SIZES )
// 64-bit friendly

#if ! defined( CFILE_HFILE )
#define CFILE_HFILE HANDLE
#endif

#else // WFC_USE_SMALL_SIZES

// 32-bit friendly

#if ! defined( CFILE_HFILE )
#define CFILE_HFILE UINT
#endif

#endif // WFC_USE_SMALL_SIZES

// The STL classes generate a horrendous number of warnings
#pragma warning( push, 3 )

#define BYTES_NOT_FOUND (-1)
#define CHARACTER_NOT_FOUND (-1)
#define STRING_NOT_FOUND (-1)
#define GUID_NOT_FOUND (-1)

#define I_AM_LESS_THAN_THAT (-1)
#define I_AM_GREATER_THAN_THAT (1)
#define I_AM_EQUAL_TO_THAT (0)

#define WFC_NO_DUMPING
#define WFC_NO_SERIALIZATION
#define WFC_NO_RAS

#include "Mfc2Stl.h"

#else

// MFC Build, we need to include this here so wfc_linker_setup will
// do the right thing (i.e. set the _MFC_VER constant). 

#if ! defined( _AFX )
#include <afxver_.h>
#endif // _AFX

#include <stdint.h>

#endif // WFC_STL

/*
** Now setup the linker, we must include our .lib before the
** Microsoft libraries in order to avoid LNK2005 errors
*/

#if defined( _MSC_VER ) && ! defined( WFC_NOFORCE_LIBS )
#include "wfc_linker_setup.h"
#endif // WFC_NOFORCE_LIBS

#if defined( WFC_STL )
#pragma warning( push )
#pragma warning( disable: 4917 )
#include <Shlobj.h>
#pragma warning( pop )
#include <intrin.h>
#include <process.h>
#include <time.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h> // for GCC isdigit and friends
#include <tchar.h>
#include <filesystem>
#include <winternl.h>
#define _NTDEF_

inline constexpr _Check_return_ bool fast_is_digit(_In_ int const character) noexcept
{
    return(static_cast<unsigned>(character - '0') < 10);
}

class RangeIterator
{
private:

    std::size_t m_Position{ 0 };

public:
    inline constexpr RangeIterator(std::size_t const position) noexcept : m_Position(position) {}

    // these three methods form the basis of an iterator for use with
    // a range-based for loop
    inline constexpr bool operator!= (RangeIterator const& other) const noexcept
    {
        return m_Position != other.m_Position;
    }

    // this method must be defined after the definition of IntVector
    // since it needs to use it
    inline constexpr std::size_t operator* () const noexcept
    {
        return(m_Position);
    }

    inline constexpr RangeIterator const& operator++ () noexcept
    {
        ++m_Position;
        // although not strictly necessary for a range-based for loop
        // following the normal convention of returning a value from
        // operator++ is a good idea.
        return *this;
    }
};

class Range
{
private:

    std::size_t const m_NumberOfValues{ 0 };
    std::size_t const m_StartingPosition{ 0 };

public:
    // This creates a series of std::size_t values ranging from zero to number_of_values
    inline constexpr Range(std::size_t const number_of_values, std::size_t const starting_position = 0) noexcept : m_NumberOfValues(number_of_values), m_StartingPosition(starting_position)
    {
    }

    inline constexpr RangeIterator begin() const noexcept
    {
        return RangeIterator(m_StartingPosition);
    }

    inline constexpr RangeIterator cbegin() const noexcept
    {
        return RangeIterator(m_StartingPosition);
    }

    inline constexpr RangeIterator end() const noexcept
    {
        return RangeIterator(m_NumberOfValues);
    }

    inline constexpr RangeIterator cend() const noexcept
    {
        return RangeIterator(m_NumberOfValues);
    }
};

#if ! defined( __MINGW32__ )
// 2001-09-15
// Putting crtdbg.h here (outside of _DEBUG) means _ASSERTE macros will compile

// 2001-03-22
// Thanks go to C.Y. Liu (inete32@panasonic.com.tw) for finding out how
// to prevent linker errors here.

#define _MFC_OVERRIDES_NEW
#include <crtdbg.h>
#endif // __MINGW32__

#if defined( _DEBUG ) && ! defined( __MINGW32__ )

inline void * __cdecl operator new(std::size_t number_of_bytes, LPCSTR filename, int line_number )
{
   return ::operator new( number_of_bytes, _NORMAL_BLOCK, filename, line_number );
}

inline void __cdecl operator delete( void * address, const char *, int )
{
   ::operator delete( address );
}

#define WFC_DUMP_MEMORY_LEAKS_AT_PROGRAM_EXIT() _CrtSetDbgFlag( ( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ) bitor _CRTDBG_LEAK_CHECK_DF ) )

#else // _DEBUG

#define WFC_DUMP_MEMORY_LEAKS_AT_PROGRAM_EXIT()

#endif // _DEBUG

#else //  defined( WFC_STL )

#define WFC_DUMP_MEMORY_LEAKS_AT_PROGRAM_EXIT()

/*
** Include all the needed MFC stuff
*/

#include <afx.h>
#include <afxwin.h>
#include <afxcoll.h>
#include <afxmt.h>
#include <afxdisp.h> // For COleDateTime

// Thanks go to COLIN N EBERHARDT <phycne@phys-irc.novell.leeds.ac.uk>
// for finding a bug here (I had a trailing semicolon)

#if ( _MSC_VER < 1020 )
#pragma message( "WFC was designed to be compiled with Visual C++ 4.2 or later, some classes won't be compiled" )
#else // _MSC_VER < 1020
#include <afxinet.h>
#endif // _MSC_VER < 1020

#endif // WFC_STL

// new for 52
// The children at Microsoft screwed up CFile...
#if ( _MFC_VER < 0x0700 )

#if ! defined( CFILE_HFILE )
#define CFILE_HFILE UINT
#endif

#else // _MFC_VER < 0x0700

#if ! defined( CFILE_SEEK_OFFSET )
#define CFILE_SEEK_OFFSET LONGLONG
#endif

#if ! defined( CFILE_HFILE )
#define CFILE_HFILE HANDLE
#endif

#endif // // _MFC_VER < 0x0700

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define WFC_REPORT_MEMORY_LEAKS() _CrtSetDbgFlag( _CRTDBG_LEAK_CHECK_DF bitor _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ) )
#else
#define WFC_REPORT_MEMORY_LEAKS()
#endif

#include "wfc_validate.h"

/*
** Include all the NT stuff
*/

#include <winspool.h>
#include <winperf.h>
#include <winsock.h>

#if ! defined( _WINUSER_ )
#include <winuser.h>
#endif // _WINUSER_

#if ( _MSC_VER > 1000 ) || defined( WFC_HAVE_WINCRYPT )
#include <Softpub.h>
#include <wincrypt.h>
#include <wintrust.h>
#include <mscat.h>
#endif // _MSC_VER

#include <WinNls.h>

#if ! defined( WFC_NO_RAS )

#include <ras.h>
#include <raserror.h>

#if ( _MSC_VER > 1000 ) || defined( WFC_HAVE_RASSAPI )
extern "C" {
//#include <rassapi.h>
//#include <Mprapi.h>
}
#endif // _MSC_VER

#endif // WFC_NO_RAS

#if ! defined( PDWORD_PTR )
#define PDWORD_PTR LPDWORD
#endif

#include <winuser.h>
#include <lm.h>
#include <lmat.h>
#include <lzexpand.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <math.h> // used in CRandomNumberGenerator
#include <process.h> // used in beginthreadex() in CService
#include <sqltypes.h> // For CSystemTime's support of TIMESTAMP_STRUCT
#include <winioctl.h> // For CVolume stuff
#include <commdlg.h> // For GetFileTitle() in CFile64
#include <wininet.h> // For InternetOpenUrl() etc

inline constexpr _Check_return_ bool is_flagged(_In_ uint64_t const _x, _In_ uint64_t const _y) noexcept
{
    return(((_x bitand _y) == _y) ? true : false);
}

#if defined ( __MINGW32__ ) // windows.h doesn't ole2.h include by default 
#include <ole2.h>           // Note: COM only works with new ABI (-fvtable-thunks)
#endif

#if ! defined( _NTSECAPI_ )
#include <ntsecapi.h>
#endif // _NTSECAPI_

// Thus endeth the Microsoft specific section, let's start our own
// declarations now.

// 2000-03-12
// Thanks go to Nick Britzman for clueing me in on the LMSTR data type
// that is new for Win2000 (NT5). For years (and many bug reports from
// this author that obviously fell on deaf ears), Microsoft has screwed up
// the LanManager API. In NT, you could pass only UNICODE strings even though
// the API's were prototyped to take either ANSI or UNICODE strings. In the
// Jan 2000 Platform SDK, they created a new string type called LMSTR
// (LanManager string). This broke my class library. :-(

#if ! defined( LMSTR )

// We are not compiling with the new LMSTR data type. Let's use the
// old data type

#define LMSTR LPTSTR

#endif // LMSTR

// 1999-08-17
// Thanks go to Brad Hubbard (hygbradh@mpx.com.au) for finding
// a TCHAR.H bug when using Visual C++ 5.0 (KB Q168433)

#if ( _MSC_VER < 1200 )
#ifdef _UNICODE
   #ifdef  _ttoi64
      #undef  _ttoi64
      #define _ttoi64 _wtoi64
   #endif
#else
   #ifdef  _ttoi64
      #undef  _ttoi64
      #define _ttoi64 _atoi64
   #endif
#endif // _UNICODE

#if ! defined( atoi64 )
#define atoi64 _atoi64
#endif // atoi64

#endif // _MSC_VER < 1200

// 2000-11-10
// Thanks to Danny Smith for the MINGW stuff

#if defined( __MINGW32__ )
// GCC compiler has different names for structures than Visual C++
#define _COMMCONFIG  _COMM_CONFIG
#define tagWINDOWPLACEMENT _WINDOWPLACEMENT
#endif // __MINGW32__

#if ! defined( NO_WFC_CONVERT_TO_UNICODE )
#include <mlang.h>
#endif

#define SECURITY_WIN32
#include <Sspi.h>
#include <SecExt.h>
#include <Sddl.h> // For ConvertSidToStringSid

#if ! defined( NUMBER_OF_MICROSECONDS_IN_ONE_SECOND )
#define NUMBER_OF_MICROSECONDS_IN_ONE_SECOND (1000000)
#endif // NUMBER_OF_MICROSECONDS_IN_ONE_SECOND

#if ! defined( NUMBER_OF_MILLISECONDS_IN_ONE_SECOND )
#define NUMBER_OF_MILLISECONDS_IN_ONE_SECOND (1000)
#endif // NUMBER_OF_MILLISECONDS_IN_ONE_SECOND

#if ! defined( WFC_DONT_USE_NAMESPACES )
#if ( _MSC_VER > 1000 )
namespace Win32FoundationClasses
{
#endif // _MSC_VER > 1000
#endif // WFC_DONT_USE_NAMESPACES

#if ! defined( WFC_DONT_USE_NAMESPACES )
#define USING_WFC_NAMESPACE using namespace Win32FoundationClasses;
#else // WFC_DONT_USE_NAMESPACES
#define USING_WFC_NAMESPACE
#endif // WFC_DONT_USE_NAMESPACES

#include "wfcfloat.h" // For UNICODE floating point routines
#include "wfctrace.hpp"

#define WFC_STANDARD_TIME_FORMAT TEXT( "%H:%M:%S %d %b %Y" ) 

#define WFC_NULL_GUID  {0,0,0,{0,0,0,0,0,0,0,0}}
#define WFC_IS_NULL_GUID(id) ((id.Data1==0)and(id.Data2==0)and(id.Data3==0)and\
                              (id.Data4[0]==0)and(id.Data4[1]==0)and(id.Data4[2]==0)and\
                              (id.Data4[3]==0)and(id.Data4[4]==0)and(id.Data4[5]==0)and\
                              (id.Data4[6]==0)and(id.Data4[7]==0))
#define WFC_IS_NOT_NULL_GUID(id) ((id.Data1!=0)or(id.Data2!=0)or(id.Data3!=0)or\
                                  (id.Data4[0]!=0)or(id.Data4[1]!=0)or(id.Data4[2]!=0)or\
                                  (id.Data4[3]!=0)or(id.Data4[4]!=0)or(id.Data4[5]!=0)or\
                                  (id.Data4[6]!=0)or(id.Data4[7]!=0))
#define WFC_IS_HEXADECIMAL( _x ) ((_x >= '0' and _x <= '9')or(_x >= 'a' and _x <= 'f')or(_x >= 'A' and _x <= 'F'))
#define WFC_IS_OCTAL( _x ) (_x >= '0' and _x <= '7')
#define WFC_IS_DECIMAL( _x ) (_x >= '0' and _x <= '9')

    /*
** An easy way to refer to colors . . .
*/

#define BLACK     RGB( 0x00, 0x00, 0x00 )
#define RED       RGB( 0xFF, 0x00, 0x00 )
#define GREEN     RGB( 0x00, 0xFF, 0x00 )
#define YELLOW    RGB( 0xFF, 0xFF, 0x00 )
#define DKGRAY    RGB( 0x7F, 0x7F, 0x7F )
#define DARK_GRAY DKGRAY
#define LTGRAY    RGB( 0xC0, 0xC0, 0xC0 )
#define LIGHT_GRAY LTGRAY
#define DKRED     RGB( 0x7F, 0x00, 0x00 )
#define DARK_RED  DKRED
#define MAGENTA   RGB( 0xFF, 0x00, 0xFF )
#define PURPLE    RGB( 0x7F, 0x00, 0x7F )
#define BLUE      RGB( 0x00, 0x00, 0xFF )
#define DKBLUE    RGB( 0x00, 0x00, 0x7F )
#define DARK_BLUE DKBLUE
#define INDIGO    RGB( 0x00, 0xFF, 0xFF )
#define DKINDIGO  RGB( 0x00, 0x7F, 0x7F )
#define DARK_INDIGO DKINDIGO
#define WHITE     RGB( 0xFF, 0xFF, 0xFF )
#define DKGREEN   RGB( 0x00, 0x7F, 0x00 )
#define DARK_GREEN DKGREEN
#define DKTYELLOW RGB( 0x7F, 0x7F, 0x00 )
#define DARK_YELLOW DKYELLOW
#define LTYELLOW  RGB( 0xFF, 0xFF, 0x7F )
#define LIGHT_YELLOW LTYELLOW

// Bits...

#define BIT_0  (0x1)
#define BIT_1  (0x2)
#define BIT_2  (0x4)
#define BIT_3  (0x8)
#define BIT_4  (0x10)
#define BIT_5  (0x20)
#define BIT_6  (0x40)
#define BIT_7  (0x80)
#define BIT_8  (0x100)
#define BIT_9  (0x200)
#define BIT_10 (0x400)
#define BIT_11 (0x800)
#define BIT_12 (0x1000)
#define BIT_13 (0x2000)
#define BIT_14 (0x4000)
#define BIT_15 (0x8000)
#define BIT_16 (0x10000)
#define BIT_17 (0x20000)
#define BIT_18 (0x40000)
#define BIT_19 (0x80000)
#define BIT_20 (0x100000)
#define BIT_21 (0x200000)
#define BIT_22 (0x400000)
#define BIT_23 (0x800000)
#define BIT_24 (0x1000000)
#define BIT_25 (0x2000000)
#define BIT_26 (0x4000000)
#define BIT_27 (0x8000000)
#define BIT_28 (0x10000000)
#define BIT_29 (0x20000000)
#define BIT_30 (0x40000000)
#define BIT_31 (0x80000000)

#if ! defined( MFC_TO_STL_HEADER_FILE )
void PASCAL ASCII_to_UNICODE( __in_z LPCSTR ansi_string, __out_z LPWSTR unicode_string ) noexcept;
void PASCAL UNICODE_to_ASCII( __in_z LPCWSTR unicode_string, __out_z LPSTR ansi_string, long number_of_unicode_characters_to_convert = (-1), UINT code_page = CP_ACP ) noexcept;
#endif

void PASCAL Convert_NERR_Code_to_String( __in DWORD const error_code, __out std::wstring& error_message ) noexcept;
void PASCAL wfc_append_text_to_edit_control( __in HWND edit_control_handle, __in_z LPCTSTR text_string ) noexcept;

#include "samtf.h"

#if ! defined( BITBOOL )
#define BITBOOL(x) (!(!(x)))
#endif

#if defined( _WIN64 )
#define ONE_BIT (1i64)
#else
#define ONE_BIT (1L)
#endif

#if ! defined( bit_set )
#define bit_set( arg, posn ) ((arg) or_eq (ONE_BIT << (posn)))
#endif

#if ! defined( bit_clear )
#define bit_clear( arg, posn ) ((arg) and_eq compl(ONE_BIT << (posn)))
#endif

#if ! defined( bit_test )
#define bit_test( arg, posn ) BITBOOL((arg) bitand (ONE_BIT << (posn)))
#endif

constexpr int EAX = (0);
constexpr int EBX = (1);
constexpr int ECX = (2);
constexpr int EDX = (3);

 inline _Check_return_ bool is_proccessor_feature_supported( __in int const information_type, __in int const which_register, __in int const which_bit ) noexcept
{
    int registers[ 4 ] = { 0, 0, 0, 0 }; // output: eax, ebx, ecx, edx

    __cpuid( registers, information_type );

    if ( bit_test( registers[ which_register ], which_bit ) == 1 )
    {
        return( true );
    }

    return( false );
}

inline _Check_return_ bool is_sse41_supported( void ) noexcept
{
    return( is_proccessor_feature_supported( 1, ECX, 19 ) );
}

inline _Check_return_ bool is_avx2_supported( void ) noexcept
{
    return( is_proccessor_feature_supported( 7, EBX, 5 ) );
}

inline _Check_return_ bool is_cpu_sha_supported( void ) noexcept
{
    return( is_proccessor_feature_supported( 7, EBX, 29 ) );
}

inline _Check_return_ bool is_avx_supported( void ) noexcept
{
    // http://insufficientlycomplicated.wordpress.com/2011/11/07/detecting-intel-advanced-vector-extensions-avx-in-visual-studio/

    bool const os_uses_XSAVE_XRSTORE = is_proccessor_feature_supported( 1, ECX, 27 );
    bool const cpu_supports_AVX = is_proccessor_feature_supported( 1, ECX, 28 );

    if ( os_uses_XSAVE_XRSTORE == true and cpu_supports_AVX == true )
    {
        // Check to see if the OS will save the YMM registers 
        auto const xcr_feature_mask = _xgetbv( _XCR_XFEATURE_ENABLED_MASK );

        if (is_flagged( xcr_feature_mask, 6 ) == true )
        {
            return( true );
        }
    }

    return( false );
}

inline _Check_return_ bool is_cpu_aes_supported( void ) noexcept
{
    return( is_proccessor_feature_supported( 1, ECX, 25 ) );
}

inline _Check_return_ bool is_sse2_supported( void ) noexcept
{
    return( ::IsProcessorFeaturePresent( PF_XMMI64_INSTRUCTIONS_AVAILABLE ) != 0 ? true : false );
}

inline constexpr _Check_return_ bool is_bad_handle( _In_opt_ HANDLE const handle_value ) noexcept
{
   if ( handle_value == static_cast<HANDLE>( NULL ) or handle_value == INVALID_HANDLE_VALUE )
   {
       return( true );
   }

   return( false );
}

#include "stl_string_helpers.h"

static inline _Check_return_ wchar_t const * DefinedStringConstant(_In_ uint64_t const value, __in_ecount(number_of_definitions) DEFINED_WIDE_STRING const definitions[], _In_ std::size_t const number_of_definitions) noexcept
{
    if (definitions == nullptr or number_of_definitions < 1)
    {
        return(nullptr);
    }

    DEFINED_WIDE_STRING const value_to_find{ value, nullptr };

    auto entry = reinterpret_cast<DEFINED_WIDE_STRING const *>(std::bsearch(&value_to_find, definitions, number_of_definitions, sizeof(DEFINED_WIDE_STRING), compare_defines));

    if (entry == nullptr)
    {
        return(nullptr);
    }

    return(entry->StringValue);
}

inline void from_defined(_Inout_ std::wstring& s, _In_ uint64_t const value, __in_ecount(number_of_definitions) DEFINED_WIDE_STRING const definitions[], __in std::size_t const number_of_definitions) noexcept
{
    s.resize(0);

    auto defined_string = DefinedStringConstant(value, definitions, number_of_definitions);

    if (defined_string != nullptr && defined_string[0] != 0x00)
    {
        s.assign(defined_string);
    }
    else
    {
        format( s, L"Undefined(%I64u)", value);
    }
}

#include "CTimeEx.hpp"

#if defined( WFC_STL )
#include "CCriticalSection.hpp"
#include "CFile.hpp"
#endif // WFC_STL

inline constexpr _Check_return_ bool PASCAL wfc_is_ascii( uint8_t const byte_value ) noexcept
{
    if ( byte_value == CARRIAGE_RETURN or
         byte_value == LINE_FEED or
         byte_value == TAB_CHARACTER )
    {
        return( true );
    }

    if ( byte_value >= 0x20 and byte_value <= 0x7E )
    {
        return( true );
    }

    return( false );
}

inline constexpr _Check_return_ uint64_t round_down_to_a_multiple_of(_In_ uint64_t const value, _In_ uint64_t const multiple) noexcept
{
    if (value < multiple or multiple == 0)
    {
        return(0);
    }

    if ((value % multiple) == 0)
    {
        // Already a multiple, no adjustment needed
        return(value);
    }

    return(value - (value % multiple));
}

#include "CParsePoint.hpp"
#include "CDataParser.hpp"
#include "CExtensibleMarkupLanguage.hpp"
#include "CFile64.hpp"

/*
** Structure Helpers
*/

#include "Structs.hpp"
#include "Guards.hpp"

/*
** The Registration Database API
*/

#include "CRegisty.hpp"

/*
** Event Logging
*/

#include "CEventLogRecord.hpp"
#include "CEvntLog.hpp"

#include "CUNC.hpp"
#include "CURL.hpp"


#if ! defined( WFC_NO_RAS )
/*
** Remote Access Service
*/

#include "CRASPORT.hpp"
#include "CRAS.hpp"

#endif // WFC_NO_RAS

/*
** CFile stuff
*/

#include "DumyFile.hpp"
#include "Serial.hpp"
#include "Modem.hpp"

/*
** Simple Sockets
*/

#include "sockets.hpp"
#include "lsockets.hpp"
#include "tsockets.hpp"
#include "SockFile.hpp"
#include "CPing.hpp"

/*
** Named Pipes
*/

#include "cnmpipe.hpp"

#include "CBitArray.hpp"
#include "CBase64Coding.hpp"
#include "CUUCoding.hpp"
#include "CQueue.hpp"
#include "CSystemTime.hpp"
#include "CFileTime.hpp"
#include "lzfile.hpp"
#include "PortInformation.hpp"
#include "CNetwork.hpp"
#include "crasadm.hpp"
#include "CServer.hpp"
#include "CNetRsrc.hpp"
#include "CNetSess.hpp"
#include "CNetWkst.hpp"
#include "CNetFile.hpp"
#include "CNetConn.hpp"
#include "CNetShar.hpp"
#include "CNetInfo.hpp"
#include "CNetUser.hpp"
#include "Ovrlpd.hpp"
#include "dcb.hpp"
#include "ServiceConfiguration.hpp"
#include "CService.hpp"
#include "CSvcMgr.hpp"
//#include "CWait.hpp"
//#include "CBubble.hpp" // abandoned in release 73

#if ! defined( WFC_STL )
// Sorry, there's no GUI stuff in the STL build of WFC
// #include <DrawObj.hpp> // abandoned in release 73
//#include <DrawGrid.hpp> // abandoned in release 73
//#include <CSquigl.hpp> // abandoned in release 73
//#include <CConstellation.hpp> // abandoned in release 73
#endif // WFC_STL

#include "CSMTF.hpp"
#include "CTape.hpp"
#include "CMicrosoftTape.hpp"
// #include "Pager.hpp" // // abandoned in release 73
// #include "SkyWord.hpp" // abandoned in release 73
// #include "SprintSpectrum.hpp" // abandoned in release 73
#include "CMixer.hpp"
#include "CCrypto.hpp"
// #include "CWeather.hpp" // abandoned in release 73
#include "CRandomNumberGenerator.hpp"
#include "CRandomNumberGenerator2.hpp"
#include "CSuperRandomNumberGenerator.hpp"
#include "CDesktop.hpp"
#include "CGarbageCollector.hpp"
#include "CDataChunk.hpp"
#include "CDataFile.hpp"
#include "CDataMemory.hpp"
#include "CDataSocket.hpp"
#include "CDataArchive.hpp"
#include "CPasswordChangeFilter.hpp"
#include "CReedSolomonErrorCorrectionCode.hpp"
#include "CFileTransferProtocol.hpp"
#include "CNetworkScheduleJob.hpp"

#include "CXMLArchive.hpp"
// #include "CDragQuery.hpp" // abandoned in release 73
#include "CVolume.hpp"
#include "CServiceApplication.hpp"
#include "CPhysicalDiskFile.hpp"
#include "CFileDirectory.hpp"
#include "CMemoryFile.hpp"
#include "CJulianDate.hpp"
#include "CSharedMemoryObject.hpp"
#include "wfc_thread_information_block.h"
#include "wfc_undocumented.h"
#include "CCOMSucks.hpp"
#include "wfc_web_post.h"
// #include "CVeryLargeArray.hpp" // New for 70 // abandoned in release 73
// #include "CVeryLargeMemoryArray.hpp" // New for 70 // abandoned in release 73
// #include "CVeryLargeDiskArray.hpp" // New for 70 // abandoned in release 73

#if ! defined( MANUAL_RESET_EVENT )
#define MANUAL_RESET_EVENT() CreateEvent( nullptr, TRUE, FALSE, nullptr )
#endif // MANUAL_RESET_EVENT

#if ! defined( NAMED_MANUAL_RESET_EVENT )
#define NAMED_MANUAL_RESET_EVENT( _name ) CreateEvent( nullptr, TRUE, FALSE, _name )
#endif // NAMED_MANUAL_RESET_EVENT

#if ! defined( AUTO_RESET_EVENT )
#define AUTO_RESET_EVENT() CreateEvent( nullptr, FALSE, FALSE, nullptr )
#endif // AUTO_RESET_EVENT

#if ! defined( NAMED_AUTO_RESET_EVENT )
#define NAMED_AUTO_RESET_EVENT( _name  ) CreateEvent( nullptr, FALSE, FALSE, _name )
#endif // NAMED_AUTO_RESET_EVENT

#define HANDLE_CANNOT_BE_INHERITED_BY_CHILD_PROCESSES (nullptr)
#define NAMELESS (nullptr)
#define I_AM_THE_OWNER (TRUE)
#define NO_OWNER (FALSE)

#if ! defined( SIMPLE_MUTEX )
#define SIMPLE_MUTEX() CreateMutex(HANDLE_CANNOT_BE_INHERITED_BY_CHILD_PROCESSES, NO_OWNER, NAMELESS);
#endif // SIMPLE_MUTEX

// Some new NT5 privileges

#if ! defined( SE_DENY_INTERACTIVE_LOGON_NAME )
#define SE_DENY_INTERACTIVE_LOGON_NAME TEXT( "SeDenyInteractiveLogonRight" )
#endif // SE_DENY_INTERACTIVE_LOGON_NAME

#if ! defined( SE_DENY_NETWORK_LOGON_NAME )
#define SE_DENY_NETWORK_LOGON_NAME TEXT( "SeDenyNetworkLogonRight" )
#endif // SE_DENY_NETWORK_LOGON_NAME

#if ! defined( SE_DENY_BATCH_LOGON_NAME )
#define SE_DENY_BATCH_LOGON_NAME TEXT( "SeDenyBatchLogonRight" )
#endif // SE_DENY_BATCH_LOGON_NAME

#if ! defined( SE_DENY_SERVICE_LOGON_NAME )
#define SE_DENY_SERVICE_LOGON_NAME TEXT( "SeDenyServiceLogonRight" )
#endif // SE_DENY_SERVICE_LOGON_NAME

#if ! defined( SE_UNDOCK_NAME )
#define SE_UNDOCK_NAME TEXT( "SeUndockPrivilege" )
#endif // SE_UNDOCK_NAME

#if ! defined( SE_SYNC_AGENT_NAME )
#define SE_SYNC_AGENT_NAME TEXT( "SeSyncAgentPrivilege" )
#endif // SE_SYNC_AGENT_NAME

#if ! defined( SE_ENABLE_DELEGATION_NAME )
#define SE_ENABLE_DELEGATION_NAME TEXT( "SeEnableDelegationPrivilege" )
#endif // SE_ENABLE_DELEGATION_NAME

#if ! defined( SE_MANAGE_VOLUME_NAME )
#define SE_MANAGE_VOLUME_NAME TEXT( "SeManageVolumePrivilege" )
#endif // SE_SYNC_AGENT_NAME

// Some new Longhorn Privileges
#if ! defined( SE_REMOTE_INTERACTIVE_LOGON_NAME )
#define SE_REMOTE_INTERACTIVE_LOGON_NAME    TEXT("SeRemoteInteractiveLogonRight")
#endif // SE_REMOTE_INTERACTIVE_LOGON_NAME

#if ! defined( SE_DENY_REMOTE_INTERACTIVE_LOGON_NAME )
#define SE_DENY_REMOTE_INTERACTIVE_LOGON_NAME TEXT("SeDenyRemoteInteractiveLogonRight")
#endif // SE_DENY_REMOTE_INTERACTIVE_LOGON_NAME

#if ! defined( SE_IMPERSONATE_NAME )
#define SE_IMPERSONATE_NAME               TEXT("SeImpersonatePrivilege")
#endif

#if ! defined( SE_CREATE_GLOBAL_NAME )
#define SE_CREATE_GLOBAL_NAME             TEXT("SeCreateGlobalPrivilege")
#endif

// Some new Windows 10 privileges

#if ! defined( SE_TRUSTED_CREDMAN_ACCESS_NAME )
#define SE_TRUSTED_CREDMAN_ACCESS_NAME    TEXT("SeTrustedCredManAccessPrivilege")
#endif

#if ! defined( SE_RELABEL_NAME )
#define SE_RELABEL_NAME    TEXT("SeRelabelPrivilege")
#endif

#if ! defined( SE_INC_WORKING_SET_NAME )
#define SE_INC_WORKING_SET_NAME    TEXT("SeIncreaseWorkingSetPrivilege")
#endif

#if ! defined( SE_TIME_ZONE_NAME )
#define SE_TIME_ZONE_NAME    TEXT("SeTimeZonePrivilege")
#endif

#if ! defined( SE_CREATE_SYMBOLIC_LINK_NAME )
#define SE_CREATE_SYMBOLIC_LINK_NAME    TEXT("SeCreateSymbolicLinkPrivilege")
#endif

#if ! defined( SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME )
#define SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME    TEXT("SeDelegateSessionUserImpersonatePrivilege")
#endif

#define WAIT_FOR_ANY_OBJECT_TO_SIGNAL (FALSE)
#define WAIT_FOR_ALL_OBJECTS_TO_SIGNAL (TRUE)

// C Helper functions...

inline constexpr _Check_return_ uint8_t hex_val( _In_ wchar_t const character ) noexcept
{
    if ( character >= '0' and character <= '9' )
    {
        return( character - '0' );
    }

    if ( character >= 'a' and character <= 'f' )
    {
        return( ( character - 'a' ) + 10 );
    }

    if ( character >= 'A' and character <= 'F' )
    {
        return( ( character - 'A' ) + 10 );
    }

    return( 0 );
}

_Check_return_ bool PASCAL wfc_am_i_administrator( void ) noexcept;
_Check_return_ bool PASCAL wfc_close_handle( __in_opt HANDLE handle ) noexcept;
_Check_return_ bool PASCAL wfc_create_hard_link( __in_z LPCTSTR new_filename, __in_z LPCTSTR existing_filename, __in_opt LPSECURITY_ATTRIBUTES security_attributes_p ) noexcept;
_Check_return_ SECURITY_DESCRIPTOR * PASCAL wfc_create_null_dacl( void ) noexcept;
_Check_return_ bool PASCAL wfc_delete_oldest_file( _In_z_ wchar_t const * directory_path ) noexcept;
              void PASCAL wfc_destroy_null_dacl( __in_opt void * null_dacl ) noexcept;
_Check_return_ bool PASCAL wfc_disable_privilege( __in_z LPCTSTR privilege_name ) noexcept;
_Check_return_ bool PASCAL wfc_enable_privilege( __in_z LPCTSTR privilege_name ) noexcept;
_Check_return_ bool PASCAL wfc_enable_all_privileges( void ) noexcept;
_Check_return_ bool PASCAL wfc_get_command_line( __out std::wstring& full_command_line ) noexcept;
_Check_return_ bool PASCAL wfc_get_day_of_week( __in int year_with_century, __in int month, __in int day, __out int& day_of_week ) noexcept;
_Check_return_ bool PASCAL wfc_get_filename_from_handle( __in HANDLE const file_handle, __out std::wstring& filename ) noexcept;
_Check_return_ bool PASCAL wfc_move_file_at_reboot( __in std::wstring const& existing_file, __in std::wstring const& file_to_move ) noexcept;
_Check_return_ bool PASCAL wfc_parse_hex_string(_In_ std::wstring_view hex_string, __out_bcount( buffer_size ) uint8_t * buffer, _In_ std::size_t const buffer_size ) noexcept;
_Check_return_ bool PASCAL wfc_parse_hex_string(_In_ std::string_view hex_string, __out_bcount( buffer_size ) uint8_t * buffer, _In_ std::size_t const buffer_size ) noexcept;
_Check_return_ bool PASCAL wfc_parse_iso_8601_string( _In_ std::wstring_view time_string, __out CFileTime&    the_time ) noexcept;
_Check_return_ bool PASCAL wfc_parse_iso_8601_string(_In_ std::wstring_view time_string, __out CSystemTime&  the_time ) noexcept;
_Check_return_ bool PASCAL wfc_parse_iso_8601_string(_In_ std::wstring_view time_string, __out CTime& the_time) noexcept;
_Check_return_ bool PASCAL wfc_process_buffer( __in uint8_t const * buffer, __in std::size_t const number_of_bytes_in_buffer, __in std::size_t const step_size, __inout PROCESS_BUFFER_CALLBACK function_to_call, __inout_opt void * callback_context ) noexcept;
_Check_return_ bool wfcGenRandom( _Out_writes_bytes_(RandomBufferLength) PVOID RandomBuffer, _In_ ULONG const RandomBufferLength ) noexcept;
_Check_return_ bool PASCAL wfc_parse_xml( __in_z LPCTSTR filename, __inout CExtensibleMarkupLanguageDocument& xml ) noexcept;

_Check_return_ uint32_t PASCAL wfc_get_directory_usage_percentage( __in std::wstring const& directory_path ) noexcept;

void PASCAL wfc_append_string_to_byte_array( __in std::wstring const& string_to_append, __out std::vector<uint8_t>& bytes ) noexcept;
void PASCAL wfc_append_ascii_string_to_byte_array( __in_z char const * string_to_append, __inout std::vector<uint8_t>& bytes ) noexcept;
void PASCAL wfc_append_unicode_string_to_byte_array( __in_z wchar_t const * string_to_append, __inout std::vector<uint8_t>& bytes ) noexcept;

#if ! defined( WFC_DEFAULT_WEB_PAGE_OPTIONS )
#define WFC_DEFAULT_WEB_PAGE_OPTIONS (INTERNET_FLAG_RELOAD bitor INTERNET_FLAG_TRANSFER_ASCII bitor INTERNET_FLAG_NO_CACHE_WRITE bitor INTERNET_FLAG_IGNORE_CERT_CN_INVALID bitor INTERNET_FLAG_IGNORE_CERT_DATE_INVALID)
#endif // WFC_DEFAULT_WEB_PAGE_OPTIONS

inline constexpr _Check_return_ double wfc_get_percentage( _In_ double const sample, _In_ double const total_samples ) noexcept
{
   if ( total_samples == (double) 0.0 )
   {
      return( (double) 0.0 );
   }

   return ( (double) ( sample / total_samples ) * (double) 100.0 );
}

_Check_return_ double PASCAL wfc_calculate_entropy(__in_ecount(number_of_counts) uint64_t const* counts, _In_ std::size_t const number_of_counts) noexcept; // New for 52
_Check_return_ double PASCAL wfc_calculate_entropy(__in_ecount(number_of_counts) uint32_t const* counts, _In_ std::size_t const number_of_counts) noexcept; // New for 52
_Check_return_ double PASCAL wfc_calculate_entropy( __in_ecount( number_of_counts ) uint16_t const * counts, _In_ std::size_t const number_of_counts ) noexcept; // New for 52
_Check_return_ double PASCAL wfc_get_buffer_entropy(__in_bcount(buffer_size) uint8_t const * buffer, _In_ std::size_t const buffer_size) noexcept;
_Check_return_ bool PASCAL wfc_check_digital_signature( __in_z LPCTSTR filename ) noexcept;
void PASCAL wfc_convert_double_null_terminated_UNICODE_string(__in WORD const * const buffer, __inout std::vector<std::wstring>& string_array) noexcept; // New for 45
void PASCAL wfc_read_string_array(_In_ wchar_t const * const buffer, _In_ std::size_t const number_of_characters, _Inout_ std::vector<std::wstring>& string_array) noexcept;
_Check_return_ bool PASCAL wfc_create_path( __in_z LPCTSTR path_name, __in_opt LPSECURITY_ATTRIBUTES const security_attributes_p = nullptr ) noexcept; // New for 51
_Check_return_ bool PASCAL wfc_create_wide_path( __in_z wchar_t const * path_name, __in_opt LPSECURITY_ATTRIBUTES const security_attributes_p = nullptr ) noexcept; // New for 52
void PASCAL wfc_exit_process_and_delete_exe( void ) noexcept;
_Check_return_ int  PASCAL wfc_find_curly_guid( __in_bcount( number_of_bytes_in_buffer ) uint8_t const * buffer, __in std::size_t const number_of_bytes_in_buffer ) noexcept;
_Check_return_ int  PASCAL wfc_find_wide_curly_guid( __in_bcount( number_of_bytes_in_buffer ) uint8_t const * buffer, __in std::size_t const number_of_bytes_in_buffer ) noexcept;
_Check_return_ bool PASCAL wfc_get_activex_servers(_Out_ std::vector<std::wstring>& server_list ) noexcept;
void PASCAL wfc_get_bios_serial_number(_Out_ std::wstring& serial_number ) noexcept;
void PASCAL wfc_get_bios_uuid(_Out_ GUID& uuid ) noexcept;
void PASCAL wfc_get_error_string( _In_ DWORD const error_code, _Out_ std::wstring& error_string ) noexcept;
void PASCAL wfc_get_executable_directory(_Out_ std::wstring& directory_name_ending_in_a_slash) noexcept;
void PASCAL wfc_get_dll_directory(_Out_ std::wstring& directory_name_ending_in_a_slash) noexcept;
void PASCAL wfc_get_version( _In_z_ wchar_t const * file_name, _Out_ uint16_t& major, _Out_ uint16_t& minor, _Out_ uint16_t& build, _Out_ uint16_t& revision ) noexcept;
void PASCAL wfc_get_my_version(_Out_ uint16_t& major, _Out_ uint16_t& minor, _Out_ uint16_t& build, _Out_ uint16_t& revision ) noexcept;
void PASCAL wfc_get_my_version(_Inout_ std::wstring& version ) noexcept;
_Check_return_ uint64_t PASCAL wfc_get_my_packed_version( void ) noexcept;
void PASCAL wfc_get_operating_system_version_string( __out std::wstring& version_string ) noexcept;
void PASCAL wfc_get_program_data_directory(__out std::wstring& program_data_directory) noexcept;
_Check_return_ bool PASCAL wfc_get_string_from_clipboard( __out std::wstring& string_to_return ) noexcept;
void PASCAL wfc_get_thread_name( __out std::wstring& thread_name ) noexcept;
void PASCAL wfc_get_web_page( __in CUniformResourceLocator const& url, __out std::vector<std::wstring>& page_contents, __in DWORD const options = WFC_DEFAULT_WEB_PAGE_OPTIONS ) noexcept;
void PASCAL wfc_get_web_page( __in CUniformResourceLocator const& url, __out std::vector<uint8_t>& page_contents, __in DWORD const options = WFC_DEFAULT_WEB_PAGE_OPTIONS ) noexcept;
_Check_return_ bool PASCAL wfc_is_dotted_ip_address( _In_ std::string_view ip_address ) noexcept;
_Check_return_ bool PASCAL wfc_is_dotted_ip_address( _In_ std::wstring_view ip_address ) noexcept;
_Check_return_ bool PASCAL wfc_is_guid( _In_ std::string_view characters ) noexcept;
_Check_return_ bool PASCAL wfc_is_guid( _In_ std::wstring_view characters ) noexcept;
_Check_return_ bool PASCAL wfc_is_guid_with_curlies( _In_ std::string_view characters ) noexcept;
_Check_return_ bool PASCAL wfc_is_guid_with_curlies( _In_ std::wstring_view character ) noexcept;
_Check_return_ bool PASCAL wfc_is_octal( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_is_wide_octal( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_is_octal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_is_wide_octal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_is_hexadecimal( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_is_wide_hexadecimal( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_is_hexadecimal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_is_wide_hexadecimal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_is_decimal( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_is_wide_decimal( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_is_decimal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_is_wide_decimal_then_NULLs( __in_bcount( number_of_bytes ) uint8_t const * buffer, __in std::size_t const number_of_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_make_ntfs_go_fast( void ) noexcept;
_Check_return_ bool PASCAL wfc_protect_data( __in_bcount( number_of_bytes_in_plaintext ) uint8_t const * plaintext, __in std::size_t const number_of_bytes_in_plaintext, __inout_opt uint8_t * ciphertext, __inout std::size_t * address_of_number_of_cipher_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_unprotect_data( __in_bcount( number_of_bytes_in_ciphertext ) uint8_t const * ciphertext, __in std::size_t const number_of_bytes_in_ciphertext,__inout_opt uint8_t * plaintext, __inout std::size_t * address_of_number_of_plain_bytes ) noexcept;
_Check_return_ bool PASCAL wfc_set_activex_servers( __in std::vector<std::wstring> const& server_list ) noexcept;
_Check_return_ uint32_t PASCAL wfc_number_of_files_in_directory( _In_z_ wchar_t const * directory_path ) noexcept;
_Check_return_ uint64_t PASCAL wfc_number_of_bytes_in_directory(_In_z_ wchar_t const * directory_path ) noexcept;
void PASCAL wfc_set_thread_name( __in_z LPCSTR thread_name ) noexcept;
void PASCAL wfc_start_screen_saver( void ) noexcept;
_Check_return_ bool PASCAL wfc_kill_most_recently_used_document_list( void ) noexcept;

inline _Check_return_ bool PASCAL wfc_does_file_exist( __in_z LPCTSTR file_name ) noexcept
{
    return( CFile64::Exists( file_name ) );
}

inline void PASCAL wfc_get_current_filetime( _Inout_ FILETIME * file_time ) noexcept
{
   ::GetSystemTimePreciseAsFileTime( file_time );
}

// Coverage analysis stuff
#define WFC_COVERAGE_DECLARE
#define WFC_COVERAGE( x )
#define WFC_COVERAGE_WITH_NAME
#define WFC_COVERAGE_REPORT()
#define WFC_COVERAGE_SET_NUMBER_OF_POINTS
#define WFC_COVERAGE_EXTERN_DECLARE
#define WFC_COVERAGE_RECORD_TIME
#define WFC_SET_COVERAGE_POINT_NAME

#if ! defined( NO_WFC_CONVERT_TO_UNICODE )

#if ! defined( WFC_AUTOMATICALLY_SELECT_CODEPAGE )
#define WFC_AUTOMATICALLY_SELECT_CODEPAGE (50001)
#endif // WFC_AUTOMATICALLY_SELECT_CODEPAGE

#if ! defined( WFC_DEFAULT_CODEPAGE )
#define WFC_DEFAULT_CODEPAGE (WFC_AUTOMATICALLY_SELECT_CODEPAGE)
#endif // WFC_DEFAULT_CODEPAGE

#define DEFAULT_THREAD_STACK_SIZE (0)
#define DEFAULT_THREAD_SECURITY_CONTEXT (nullptr)
#define DEFAULT_THREAD_ARGUMENT_LIST (nullptr)
#define DEFAULT_THREAD_INIT_FLAG (0) // Run Immediately

#define RETURN_ALL_POSSIBLE_ENCODINGS (0)

_Check_return_ bool PASCAL wfc_read_xml_from_file( _In_ std::wstring_view filename, _Inout_ CExtensibleMarkupLanguageDocument& xml ) noexcept;
void PASCAL wfc_free_unicode_conversion_context( _Inout_ VOID * unicode_conversion_context ) noexcept;
_Check_return_ VOID * PASCAL wfc_get_unicode_conversion_context( _In_ bool const is_com_already_started_in_this_thread ) noexcept;
_Check_return_ bool PASCAL wfc_detect_code_page( _Inout_ VOID * unicode_conversion_context, _In_reads_bytes_( number_of_bytes_in_buffer ) uint8_t const * buffer, _In_ std::size_t const number_of_bytes_in_buffer, _In_ uint32_t const encoding_hints, _Out_ uint32_t& code_page ) noexcept;
_Check_return_ bool PASCAL wfc_convert_to_unicode( __inout VOID *            unicode_conversion_context,
                                    __in std::vector<uint8_t> const& bytes,
                                    __out_ecount_z( number_of_wide_characters ) wchar_t * unicode_string,
                                    __in std::size_t const           number_of_wide_characters,
                                    __in DWORD const            suggested_code_page,
                                    __in DWORD const            encoding_hints   = 0xFFFFFFFF,
                                    __out_opt DWORD *      real_code_page_p = nullptr ) noexcept; // New for 52
_Check_return_ bool PASCAL wfc_convert_to_unicode( __inout VOID *       unicode_conversion_context,
                                    __in_bcount( number_of_bytes_in_buffer ) BYTE const * byte_buffer,
                                    __in std::size_t const      number_of_bytes_in_buffer,
                                    __out_ecount_z( number_of_wide_characters ) wchar_t * unicode_string,
                                    __in std::size_t const      number_of_wide_characters,
                                    __in DWORD const       suggested_code_page,
                                    __in DWORD const       encoding_hints   = 0xFFFFFFFF,
                                    __out_opt DWORD *      real_code_page_p = nullptr ) noexcept; // New for 52
#endif // NO_WFC_CONVERT_TO_UNICODE

#define WFC_INITIALIZE_SINGLE_THREADED_COM() ::CoInitialize( nullptr )
#define WFC_INITIALIZE_SINGLE_THREADED_COM_NO_DDE() ::CoInitializeEx( nullptr, COINIT_APARTMENTTHREADED bitor COINIT_DISABLE_OLE1DDE bitor COINIT_SPEED_OVER_MEMORY )
#define WFC_INITIALIZE_MULTITHREADED_COM() ::CoInitializeEx( nullptr, COINIT_MULTITHREADED bitor COINIT_DISABLE_OLE1DDE bitor COINIT_SPEED_OVER_MEMORY )

/*
** Last but not least, let's see if we are building a UNICODE console
** application, if so, we need to do one last thing for the user:
*/

// 2001-07-25
// If the user is building a UNICODE DLL (which I can't detect automatically)
// we need to NOT specify an entry point.

#if ! defined( WFC_DONT_FIX_UNICODE_LINK )
#if defined( _UNICODE )
#if defined( _CONSOLE )
// We're building a console application
#if ! defined( WFC_SILENT )
#pragma message( "NOTE: If you get a linker problem about wmainCRTStartup you are probably building a Unicode DLL." )
#endif // WFC_SILENT
#pragma comment( linker, "/ENTRY:wmainCRTStartup" )
#else // _CONSOLE
// We're building a GUI application
#if ! defined( WFC_SILENT )
#pragma message( "NOTE: If you get a linker problem about wWinMainCRTStartup you are probably building a Unicode DLL." )
#endif // WFC_SILENT
//#if ( _MSC_VER < 1700 )
//#pragma comment( linker, "/ENTRY:wWinMainCRTStartup" )
//#else
//#pragma comment( linker, "/ENTRY:wmainCRTStartup" )
//#endif // _MSC_VER
#endif // _CONSOLE
#if ! defined( WFC_SILENT )
#pragma message( "      To fix the problem. #define WFC_DONT_FIX_UNICODE_LINK before including wfc.h" )
#endif // WFC_SILENT
#endif // _UNICODE
#endif // WFC_DONT_FIX_UNICODE_LINK

#if ! defined( WFC_DONT_USE_NAMESPACES )
#if ( _MSC_VER > 1000 )
// Terminate the namespace declaration
}
#endif // _MSC_VER > 1000
#endif // WFC_DONT_USE_NAMESPACES

//#pragma warning( disable : 4699 )

#else // WIN32_FOUNDATION_CLASSES

#include <stlxml.h>

#endif // WIN32_FOUNDATION_CLASSES
