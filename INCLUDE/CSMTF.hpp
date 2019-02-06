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
** $Workfile: csmtf.hpp $
** $Revision: 13 $
** $Modtime: 6/26/01 11:04a $
*/

#if ! defined( CMSTF_CLASS_HEADER )

#define CMSTF_CLASS_HEADER

#if ! defined( SAMS_MICROSOFT_TAPE_FORMAT_HEADER )
#include "samtf.h"
#endif

class CUINT64 : public ___SIXTY_FOUR_BIT_UNSIGNED_INTEGER
{
   public:

      // Construction

      CUINT64();
      CUINT64( __in const CUINT64& source );
      CUINT64( __in const SIXTY_FOUR_BIT_UNSIGNED_INTEGER&  source );
      CUINT64( __in const SIXTY_FOUR_BIT_UNSIGNED_INTEGER * source );
      virtual ~CUINT64();

      // Methods

      virtual void Copy( __in const CUINT64& source ) noexcept;
      virtual void Copy( __in const SIXTY_FOUR_BIT_UNSIGNED_INTEGER * source ) noexcept;
      virtual void Copy( __in const SIXTY_FOUR_BIT_UNSIGNED_INTEGER&  source ) noexcept;
      virtual void Empty( void ) noexcept;

      // Operators

      __checkReturn CUINT64& operator=( __in const CUINT64& source ) noexcept;
      __checkReturn CUINT64& operator=( __in const SIXTY_FOUR_BIT_UNSIGNED_INTEGER& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CMTFDateTime : public ___MTF_DATE_TIME
{
   public:

      // Construction

      CMTFDateTime();
      CMTFDateTime( __in const CMTFDateTime&   source );
      CMTFDateTime( __in const MTF_DATE_TIME * source );
      CMTFDateTime( __in const MTF_DATE_TIME&  source );
      CMTFDateTime( __in const CTime&          source );
      virtual ~CMTFDateTime();

      // Methods

      virtual void Convert( __out CTime& destination ) const noexcept;
      virtual void Copy( __in const CMTFDateTime& source ) noexcept;
      virtual void Copy( __in const MTF_DATE_TIME * source ) noexcept;
      virtual void Copy( __in const CTime& source ) noexcept;
      virtual void Empty( void ) noexcept;

      // Operators

      CMTFDateTime& operator=( __in const CMTFDateTime&  source ) noexcept;
      CMTFDateTime& operator=( __in const MTF_DATE_TIME& source ) noexcept;
      CMTFDateTime& operator=( __in const CTime&         source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CMTFTapeAddress : public ___MTF_TAPE_ADDRESS
{
   public:

      // Construction

      CMTFTapeAddress();
      CMTFTapeAddress( __in const CMTFTapeAddress& source );
      CMTFTapeAddress( __in const MTF_TAPE_ADDRESS * source );
      virtual ~CMTFTapeAddress();

      // Methods

      virtual void Copy( __in const CMTFTapeAddress& source ) noexcept;
      virtual void Copy( __in const MTF_TAPE_ADDRESS * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CMTFDataBlockHeader : public ___MTF_DB_HDR
{
   public:

      // Construction

      CMTFDataBlockHeader();
      CMTFDataBlockHeader( __in const CMTFDataBlockHeader& source );
      CMTFDataBlockHeader( __in const MTF_DB_HDR * source );
      virtual ~CMTFDataBlockHeader();

      // Methods

      virtual void Copy( __in const CMTFDataBlockHeader& source ) noexcept;
      virtual void Copy( __in const MTF_DB_HDR * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CMTFTape : public ___MTF_TAPE
{
   public:

      // Construction

      CMTFTape();
      CMTFTape( __in const CMTFTape& source );
      CMTFTape( __in const MTF_TAPE * source );
      virtual ~CMTFTape();

      // Methods

      virtual void Copy( __in const CMTFTape& source ) noexcept;
      virtual void Copy( __in const MTF_TAPE * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

#endif // CMSTF_CLASS_HEADER
