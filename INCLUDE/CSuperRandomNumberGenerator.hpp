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
** $Workfile: CSuperRandomNumberGenerator.hpp $
** $Revision: 4 $
** $Modtime: 6/26/01 11:04a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( SUPER_RANDOM_NUMBER_GENERATOR_CLASS_HEADER )

#define SUPER_RANDOM_NUMBER_GENERATOR_CLASS_HEADER

class CSuperRandomNumberGenerator
{
   protected:

      int    m_IntegerSeeds[ 25 ]{ 0 };
      int    m_NextValue[ 25 ]{ 0 };
      double m_Seeds[ 25 ]{ 0 };

      double m_Carry{ 0 };
      double m_TwoM12{ 0 };
      double m_TwoM24{ 0 };

      int m_Index_I24{ 0 };
      int m_Index_J24{ 0 };
      int m_Index24{ 0 };

      void m_Initialize( void );

      enum
      {
         TWELVE_BIT_VALUE         = 4096,
         TWENTY_FOUR_BIT_VALUE    = 16777216,
         NUMBER_OF_VALUES_TO_SKIP = 365,
         DEFAULT_SEED_VALUE       = 314159265,
         ICONS                    = 2147483563
      }
      constants;

   public:

      CSuperRandomNumberGenerator( _In_ DWORD seed = 0 );
     ~CSuperRandomNumberGenerator();

      _Check_return_ double GetValue( void ) noexcept;
      _Check_return_ DWORD  GetInteger( void ) noexcept;
      void SetSeed( _In_ DWORD seed ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // SUPER_RANDOM_NUMBER_GENERATOR_CLASS_HEADER
