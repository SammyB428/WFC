/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2015, Samuel R. Blackburn
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
** $Workfile: CRandomNumberGenerator.hpp $
** $Revision: 10 $
** $Modtime: 6/26/01 11:03a $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined ( RANDOM_NUMBER_GENERATOR_CLASS_HEADER )

#define RANDOM_NUMBER_GENERATOR_CLASS_HEADER

class CRandomNumberGenerator
{
   protected:

      double m_Array[97]{ 0 };
      double m_C{ 0 };
      double m_Cd{ 0 };
      double m_Cm{ 0 };

      SSIZE_T m_Index_1{ 0 };
      SSIZE_T m_Index_2{ 0 };

   public:

      CRandomNumberGenerator() noexcept;
      explicit CRandomNumberGenerator( _In_ DWORD new_seed ) noexcept;

      /*
      ** Methods
      */

      _Check_return_ DWORD  GetInteger( void ) noexcept;
      _Check_return_ double GetFloat( void ) noexcept;
      void   InitializeSeed( void ) noexcept;
      void   SetSeed( _In_ DWORD new_seed ) noexcept;

      operator char() noexcept;
      operator unsigned char() noexcept;
      operator int() noexcept;
      operator unsigned int() noexcept;
      operator short() noexcept;
      operator unsigned short() noexcept;
      operator long() noexcept;
      operator unsigned long() noexcept;
      operator float() noexcept;
      operator double() noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // RANDOM_NUMBER_GENERATOR_CLASS_HEADER
