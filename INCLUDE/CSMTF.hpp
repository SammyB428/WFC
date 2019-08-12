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

       CUINT64() noexcept;
      CUINT64( __in CUINT64 const& source ) noexcept;
      CUINT64( __in SIXTY_FOUR_BIT_UNSIGNED_INTEGER const&  source ) noexcept;
      CUINT64( __in SIXTY_FOUR_BIT_UNSIGNED_INTEGER const * source ) noexcept;
      virtual ~CUINT64();

      // Methods

      virtual void Copy( __in CUINT64 const& source ) noexcept;
      virtual void Copy( __in SIXTY_FOUR_BIT_UNSIGNED_INTEGER const * source ) noexcept;
      virtual void Copy( __in SIXTY_FOUR_BIT_UNSIGNED_INTEGER const&  source ) noexcept;
      virtual void Empty( void ) noexcept;

      // Operators

      _Check_return_ CUINT64& operator=( __in CUINT64 const& source ) noexcept;
      _Check_return_ CUINT64& operator=( __in SIXTY_FOUR_BIT_UNSIGNED_INTEGER const& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CMTFDateTime : public ___MTF_DATE_TIME
{
   public:

      // Construction

       CMTFDateTime() noexcept;
      CMTFDateTime( __in CMTFDateTime const&   source ) noexcept;
      CMTFDateTime( __in MTF_DATE_TIME const * source ) noexcept;
      CMTFDateTime( __in MTF_DATE_TIME const&  source ) noexcept;
      CMTFDateTime( __in CTime const&          source ) noexcept;
      virtual ~CMTFDateTime();

      // Methods

      virtual void Convert( __out CTime& destination ) const noexcept;
      virtual void Copy( __in CMTFDateTime const& source ) noexcept;
      virtual void Copy( __in MTF_DATE_TIME const * source ) noexcept;
      virtual void Copy( __in CTime const& source ) noexcept;
      virtual void Empty( void ) noexcept;

      // Operators

      CMTFDateTime& operator=( __in CMTFDateTime const&  source ) noexcept;
      CMTFDateTime& operator=( __in MTF_DATE_TIME const& source ) noexcept;
      CMTFDateTime& operator=( __in CTime const&         source ) noexcept;

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
      CMTFTapeAddress( __in CMTFTapeAddress const& source );
      CMTFTapeAddress( __in MTF_TAPE_ADDRESS const * source );
      virtual ~CMTFTapeAddress();

      // Methods

      virtual void Copy( __in CMTFTapeAddress const& source ) noexcept;
      virtual void Copy( __in MTF_TAPE_ADDRESS const * source ) noexcept;
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
      CMTFDataBlockHeader( __in CMTFDataBlockHeader const& source );
      CMTFDataBlockHeader( __in MTF_DB_HDR const * source );
      virtual ~CMTFDataBlockHeader();

      // Methods

      virtual void Copy( __in CMTFDataBlockHeader const& source ) noexcept;
      virtual void Copy( __in MTF_DB_HDR const * source ) noexcept;
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
      CMTFTape( __in CMTFTape const& source );
      CMTFTape( __in MTF_TAPE const * source );
      virtual ~CMTFTape();

      // Methods

      virtual void Copy( __in CMTFTape const& source ) noexcept;
      virtual void Copy( __in MTF_TAPE const * source ) noexcept;
      virtual void Empty( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

#endif // CMSTF_CLASS_HEADER
