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
** $Workfile: CRandomNumberGenerator2.hpp $
** $Revision: 6 $
** $Modtime: 6/26/01 11:03a $
*/

#if ! defined ( RANDOM_NUMBER_GENERATOR_2_CLASS_HEADER )

#define RANDOM_NUMBER_GENERATOR_2_CLASS_HEADER

typedef struct _value_range
{
    uint64_t low;
    uint64_t high;
}
VALUE_RANGE;

class CRandomNumberGenerator2
{
   protected:

      uint32_t  m_State[ 625 ];
      uint32_t *m_Next;
      long      m_Left;
      bool      m_Disabled;

      _Check_return_ uint32_t m_LoadMersenneTwister( void ) noexcept;

   public:

      CRandomNumberGenerator2() noexcept;
      CRandomNumberGenerator2( _In_ const uint32_t new_seed ) noexcept;
     ~CRandomNumberGenerator2() noexcept;

      /*
      ** Methods
      */

     _Check_return_ uint32_t GetInteger( void ) noexcept;
     _Check_return_ double GetFloat( void ) noexcept;
      void   InitializeSeed( void ) noexcept;
      void   SetSeed( __in const uint32_t new_seed ) noexcept;
      void   Disable( __in const bool disable = true ) noexcept;
      _Check_return_ bool   Fill( __out_bcount( number_of_bytes ) uint8_t * buffer, __in const size_t number_of_bytes ) noexcept;
      _Check_return_ bool   Fill( __out GUID& guid ) noexcept
      {
         return( Fill( (uint8_t *) &guid, sizeof( guid ) ) );
      };

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

      _Check_return_ double Double(__in const double minimum_value, __in const double maximum_value) noexcept;
      _Check_return_ uint32_t Uint32( __in const uint32_t minimum_value, __in const uint32_t maximum_value ) noexcept;
      _Check_return_ uint64_t Uint64( __in const uint64_t minimum_value, __in const uint64_t maximum_value ) noexcept;
      _Check_return_ uint64_t Uint64(__in_ecount(number_of_values) const VALUE_RANGE * values, _In_ const size_t number_of_values) noexcept;
      _Check_return_ bool IsTrue( __in const double percentage ) noexcept;
      void Fill( __in const uint32_t minimum_length, __in const uint32_t maximum_length, __out std::wstring& destination ) noexcept;
      void Fill( __in const FILETIME& minimum_value, __in const FILETIME& maximum_value, __out FILETIME& destination ) noexcept;
      _Check_return_ uint32_t OneOf( __in_ecount( number_of_values ) const uint32_t * values, __in const size_t number_of_values ) noexcept;
      _Check_return_ uint64_t OneOf( __in_ecount( number_of_values ) const uint64_t * values, __in const size_t number_of_values ) noexcept;
      _Check_return_ uint64_t Integer64( void ) noexcept
      {
          const uint64_t high = ( (uint64_t) GetInteger() << 32 );
          return( (uint64_t) ( high + GetInteger() ) );
      }

      // Fill the string with a short repeatable value based on seed. This value is NOT random.
      static void Fill( __in const uint32_t seed, __out std::wstring& destination ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // RANDOM_NUMBER_GENERATOR_2_CLASS_HEADER
