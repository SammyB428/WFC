/*
** Internet: wfc@pobox.com
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
*/

/* SPDX-License-Identifier: BSD-2-Clause */

// 2000-11-21
// Thanks go to Daniel Lanovaz [daniel@precedia.com] for sending
// me this header file. It allows excpetion handling on platforms
// that are too stupid to understand C++'s try/catch statement.
// Windows CE cannot support try/catch.

// The following macros are used to provide a implementation
// dependent mechanism to specify try/catch/throw semantcis in WFC.
//
// WFC_TRY {                 // Replacement for try
//   ...
// WFC_THROW(x);             // Replacement for throw(x)
//   ...
// } WFC_CATCH (Type, var) { // Replacement for catch (Type var)
//      ...
// } WFC_CATCH_TYPE (Type) { // Replacement for catch (Type)
//      ...
// } WFC_CATCH_ALL {         // Replacement for catch (...)
//   ...
//   WFC_RETHROW;            // Replacement for throw;
//   ...
// } WFC_END_TRY             // Should be placed after last catch
// } WFC_END_CATCH_ALL       // Should be placed after last WFC_CATCH_ALL
//                           // and used instead of WFC_END_TRY
//
// This abstraction was created to support platforms such as
// Windows CE that do not support C++ exception handling (although
// Windows CE does support structured exception handling [SEH]).
//
// Use of these macros permits souce code to compile across
// multiple WIN32 platforms. However, the runtime semantics vary between
// platforms when using different exception handling implementations. For
// example, using MFC-style exceptions will result in application
// code that cannot catch low-level memory exceptions. On Windows CE
// platforms the only solution for low-level exceptions is to use
// structured exception handling. However, SEH unwind semantics
// is not compatible with C++ code. Compiler errors result if C++ objects
// that require unwinding are included in the same function that uses SEH.
//
// There are three ways to define which exception handling
// mechanism the WFC library should be compiled using:
//
// WFC_USE_EMULATED_EXCEPTIONS
//   Emulate C++ exceptions (with some caveats, of course).
//   Fedor Sherstyuk ported the system described in [1] to
//   Windows CE and and it is publicly available from the WWW
//   site at http://www.halcyon.com/ast/dload/exclemu.zip (and
//   from the DDJ code archive?).
//
//   [1] Vladimir Belkin, "Exception processing in C++: What,
//   When, How," PC Magazine (Russian Edition), April 1995.
//
// WFC_USE_MFC_EXCEPTIONS
//   Use the MFC exception semantics as implemented by
//   the MFC macros TRY, CATCH, CATCH_ALL, THROW, etc.
//   On Windows CE platforms these macros emulate exceptions
//   using calls to setjmp/longjmp. Use of this exception
//   implementation is portable to Windows CE code that is
//   compiled for CEF (_WIN32_WCE_CEF). Please note that low-level
//   exceptions--such as memory exceptions--are not caught.
//   This is the default when compiled on Windows CE platforms.
//
// WFC_USE_CPP_EXCEPTIONS
//   Use native C++ exceptions as implemented by the
//   try/catch/throw keywords. This is the default when
//   compiled on non Windows CE platforms.

#if ! defined( WFC_EXCEPTIONS_HEADER )

#define WFC_EXCEPTIONS_HEADER

#if ! defined( WFC_USED_EMULATED_EXCEPTIONS ) && \
    ! defined( WFC_USE_MFC_EXCEPTIONS ) && \
    ! defined( WFC_USE_CPP_EXCEPTIONS )

#if defined( _WIN32_WCE )
#define WFC_USE_MFC_EXCEPTIONS
#else
#define WFC_USE_CPP_EXCEPTIONS
#endif

#endif

#if defined( WFC_USED_EMULATED_EXCEPTIONS )

#pragma message( "Using emulated C++ exceptions." )

#define WFC_TRY                  TCU_X_TRY
#define WFC_THROW( object )      TCU_X_THROW( object )
#define WFC_CATCH( Type, var )   TCU_X_CATCH( Type, var )
#define WFC_CATCH_TYPE( Type )   TCU_X_CATCH_TYPE( Type )
#define WFC_CATCH_ALL            TCU_X_CATCH_ALL
#define WFC_RETHROW              TCU_X_RETHROW
#define WFC_END_TRY              TCU_X_END_TRY
#define WFC_END_CATCH_ALL        TCU_X_END_TRY

#if ! defined( noexcept )
#define noexcept
#endif

#endif // WFC_USED_EMULATED_EXCEPTIONS

#if defined( WFC_USE_MFC_EXCEPTIONS )

#pragma message( "Using MFC exceptions." )

#define WFC_TRY                TRY
#define WFC_THROW( object )    THROW( object )
#define WFC_CATCH( Type, var ) CATCH( Type, var )
#define WFC_CATCH_TYPE( Type ) CATCH( Type, __e )
#define WFC_CATCH_ALL          CATCH_ALL( __e )
#define WFC_RETHROW            THROW_LAST()
#define WFC_END_TRY            END_TRY
#define WFC_END_CATCH_ALL      END_CATCH_ALL
#if ! defined( noexcept )
#define noexcept
#endif

#endif // WFC_USE_MFC_EXCEPTIONS

#if defined ( WFC_USE_CPP_EXCEPTIONS )

#if ! defined( WFC_SILENT )
#pragma message( "Using native C++ exceptions." )
#endif // WFC_SILENT

#define WFC_TRY                try
#define WFC_THROW( object )    throw( object )
#define WFC_CATCH( Type, var ) catch( Type var )
#define WFC_CATCH_TYPE( Type ) catch( Type __e )
#define WFC_CATCH_ALL          catch( ... )
#define WFC_RETHROW            throw
#define WFC_END_TRY
#define WFC_END_CATCH_ALL

#endif // WFC_USE_CPP_EXCEPTIONS

#endif // WFC_EXCEPTIONS_HEADER
