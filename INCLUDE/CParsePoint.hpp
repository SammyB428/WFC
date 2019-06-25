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
** $Workfile: CParsePoint.hpp $
** $Revision: 9 $
** $Modtime: 6/26/01 11:03a $
*/

#if ! defined ( PARSE_POINT_CLASS_HEADER )

#define PARSE_POINT_CLASS_HEADER

class CParsePoint
{
   protected:

       uint64_t m_ByteIndex{ 0 };
       uint64_t m_LineIndex{ 1 };
       uint64_t m_LineNumber{ 1 };

   public:

      inline constexpr CParsePoint() noexcept
      {
         Empty();
      };

      inline constexpr CParsePoint( _In_ CParsePoint const& source ) noexcept
      {
         Copy( source );
      };

      inline ~CParsePoint() noexcept
      {
         Empty();
      };

      inline constexpr _Check_return_ CParsePoint& operator=( _In_ CParsePoint const& source ) noexcept
      {
         Copy( source );
         return( *this );
      };

      inline constexpr _Check_return_ std::size_t Distance( _In_ CParsePoint const& source ) const noexcept
      {
         if ( m_ByteIndex > source.m_ByteIndex )
         {
            return( m_ByteIndex - source.m_ByteIndex );
         }

         return( source.m_ByteIndex - m_ByteIndex );
      };

      inline constexpr void Copy( _In_ CParsePoint const& source ) noexcept
      {
         m_ByteIndex  = source.m_ByteIndex;
         m_LineIndex  = source.m_LineIndex;
         m_LineNumber = source.m_LineNumber;
      };

      inline constexpr void Empty( void ) noexcept
      {
         m_ByteIndex  = 0;
         m_LineIndex  = 1;
         m_LineNumber = 1;
      };

      inline constexpr void DecrementIndex(_In_ uint64_t const decrement_by = 1) noexcept
      {
          if (decrement_by >= m_ByteIndex)
          {
              m_ByteIndex = 0;
          }
          else
          {
              m_ByteIndex -= decrement_by;
          }
      }

      inline constexpr _Check_return_ uint64_t GetIndex( void ) const noexcept
      {
         return( m_ByteIndex );
      };

      inline constexpr _Check_return_ uint64_t GetLineIndex( void ) const noexcept
      {
         return( m_LineIndex );
      };

      inline constexpr _Check_return_ uint64_t GetLineNumber( void ) const noexcept
      {
         return( m_LineNumber );
      };

      inline constexpr void SetIndex( _In_ uint64_t const index ) noexcept
      {
         m_ByteIndex = index;
      };

      inline constexpr void SetLineIndex( _In_ uint64_t const index ) noexcept
      {
         m_LineIndex = index;
      };

      inline constexpr void SetLineNumber( _In_ uint64_t const index ) noexcept
      {
         m_LineNumber = index;
      };

      inline constexpr void IncrementIndex( _In_ uint64_t const steps = 1 ) noexcept
      {
         m_ByteIndex += steps;
      };

      inline constexpr void IncrementLineIndex( _In_ uint64_t const steps = 1 ) noexcept
      {
         m_LineIndex += steps;
      };

      inline constexpr void IncrementLineNumber( _In_ uint64_t const steps = 1 ) noexcept
      {
         m_LineNumber += steps;
      };

      inline constexpr void AutoIncrement( _In_ uint8_t const character ) noexcept
      {
         m_ByteIndex++;
         m_LineIndex++;

         if ( character == LINE_FEED )
         {
            m_LineNumber++;
            m_LineIndex = 1;
         }
      };
};   

#endif // PARSE_POINT_CLASS_HEADER
