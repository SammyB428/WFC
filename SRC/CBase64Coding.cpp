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
** $Workfile: CBase64Coding.cpp $
** $Revision: 22 $
** $Modtime: 6/26/01 10:43a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

static inline constexpr _Check_return_ uint8_t __get_character( _In_reads_( size_of_buffer ) uint8_t const * buffer, __in_ecount( 256 ) uint8_t const * decoder_table, _Inout_ std::size_t& index, _In_ std::size_t const size_of_buffer ) noexcept
{
   WFC_VALIDATE_POINTER( buffer );
   WFC_VALIDATE_POINTER( decoder_table );

   uint8_t return_value = 0;

   do
   {
      if ( index >= size_of_buffer )
      {
         return( BASE64_END_OF_BUFFER );
      }

      return_value = buffer[ index ];
      index++;
   }
   while( return_value not_eq END_OF_BASE64_ENCODED_DATA and
          decoder_table[ return_value ] == BASE64_IGNORABLE_CHARACTER );

   return( return_value );
}

static inline constexpr _Check_return_ uint8_t __get_character( __in_ecount( size_of_buffer ) wchar_t const * buffer, __in_ecount( 256 ) uint8_t const * decoder_table, _Inout_ std::size_t& index, _In_ std::size_t const size_of_buffer ) noexcept
{
   WFC_VALIDATE_POINTER( buffer );
   WFC_VALIDATE_POINTER( decoder_table );

   uint8_t return_value = 0;

   do
   {
      if ( index >= size_of_buffer )
      {
         return( BASE64_END_OF_BUFFER );
      }

      return_value = static_cast<uint8_t>(buffer[ index ]);
      index++;
   }
   while( return_value not_eq END_OF_BASE64_ENCODED_DATA and
          decoder_table[ return_value ] == BASE64_IGNORABLE_CHARACTER );

   return( return_value );
}

Win32FoundationClasses::CBase64Coding::CBase64Coding() noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_InitializeDecoderTable();
}

void Win32FoundationClasses::CBase64Coding::m_InitializeDecoderTable( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // Fill the table with an invalid value. If the decoder receives
   // this value as a result of a lookup, it knows that the input
   // character must have been an invalid one.

   ::memset( m_DecoderTable, BASE64_UNKNOWN_VALUE, sizeof( m_DecoderTable ) );

   // Now let's fill the table with translatable values
   // The table is loaded with table[ ASCII_VALUE ] = TRANSLATED_VALUE

    m_DecoderTable[  43 ] = 62;
    m_DecoderTable[  47 ] = 63;
    m_DecoderTable[  48 ] = 52;
    m_DecoderTable[  49 ] = 53;
    m_DecoderTable[  50 ] = 54;
    m_DecoderTable[  51 ] = 55;
    m_DecoderTable[  52 ] = 56;
    m_DecoderTable[  53 ] = 57;
    m_DecoderTable[  54 ] = 58;
    m_DecoderTable[  55 ] = 59;
    m_DecoderTable[  56 ] = 60;
    m_DecoderTable[  57 ] = 61;
    m_DecoderTable[  65 ] = 0;
    m_DecoderTable[  66 ] = 1;
    m_DecoderTable[  67 ] = 2;
    m_DecoderTable[  68 ] = 3;
    m_DecoderTable[  69 ] = 4;
    m_DecoderTable[  70 ] = 5;
    m_DecoderTable[  71 ] = 6;
    m_DecoderTable[  72 ] = 7;
    m_DecoderTable[  73 ] = 8;
    m_DecoderTable[  74 ] = 9;
    m_DecoderTable[  75 ] = 10;
    m_DecoderTable[  76 ] = 11;
    m_DecoderTable[  77 ] = 12;
    m_DecoderTable[  78 ] = 13;
    m_DecoderTable[  79 ] = 14;
    m_DecoderTable[  80 ] = 15;
    m_DecoderTable[  81 ] = 16;
    m_DecoderTable[  82 ] = 17;
    m_DecoderTable[  83 ] = 18;
    m_DecoderTable[  84 ] = 19;
    m_DecoderTable[  85 ] = 20;
    m_DecoderTable[  86 ] = 21;
    m_DecoderTable[  87 ] = 22;
    m_DecoderTable[  88 ] = 23;
    m_DecoderTable[  89 ] = 24;
    m_DecoderTable[  90 ] = 25;
    m_DecoderTable[  97 ] = 26;
    m_DecoderTable[  98 ] = 27;
    m_DecoderTable[  99 ] = 28;
    m_DecoderTable[ 100 ] = 29;
    m_DecoderTable[ 101 ] = 30;
    m_DecoderTable[ 102 ] = 31;
    m_DecoderTable[ 103 ] = 32;
    m_DecoderTable[ 104 ] = 33;
    m_DecoderTable[ 105 ] = 34;
    m_DecoderTable[ 106 ] = 35;
    m_DecoderTable[ 107 ] = 36;
    m_DecoderTable[ 108 ] = 37;
    m_DecoderTable[ 109 ] = 38;
    m_DecoderTable[ 110 ] = 39;
    m_DecoderTable[ 111 ] = 40;
    m_DecoderTable[ 112 ] = 41;
    m_DecoderTable[ 113 ] = 42;
    m_DecoderTable[ 114 ] = 43;
    m_DecoderTable[ 115 ] = 44;
    m_DecoderTable[ 116 ] = 45;
    m_DecoderTable[ 117 ] = 46;
    m_DecoderTable[ 118 ] = 47;
    m_DecoderTable[ 119 ] = 48;
    m_DecoderTable[ 120 ] = 49;
    m_DecoderTable[ 121 ] = 50;
    m_DecoderTable[ 122 ] = 51;

    // OK, there's our translation table, now let's be a little
    // forgiving about end-of-lines, tabs, spaces, etc.

    m_DecoderTable[ 9               ] = BASE64_IGNORABLE_CHARACTER; // TAB character (Tabs suck!)
    m_DecoderTable[ 32              ] = BASE64_IGNORABLE_CHARACTER; // Space character
    m_DecoderTable[ CARRIAGE_RETURN ] = BASE64_IGNORABLE_CHARACTER;
    m_DecoderTable[ LINE_FEED       ] = BASE64_IGNORABLE_CHARACTER;
}

_Check_return_ std::size_t Win32FoundationClasses::CBase64Coding::Decode(_In_reads_bytes_(number_of_bytes) uint8_t const * buffer, _In_ std::size_t const number_of_bytes, _Inout_ uint8_t * destination, _In_ std::size_t destination_size ) const noexcept
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER_NULL_OK(buffer);

    if (buffer == nullptr or number_of_bytes < 1)
    {
        return(0);
    }

    bool return_value = false;

    uint8_t byte_to_add = 0;
    uint8_t character_1 = 0;
    uint8_t character_2 = 0;
    uint8_t character_3 = 0;
    uint8_t character_4 = 0;

    auto const required_destination_size = DecodeReserveSize(number_of_bytes);

    _ASSERTE(required_destination_size <= destination_size);

    std::size_t index = 0;
    std::size_t add_index = 0;

    // Since we're decoding, we are most likely in a performance-minded
    // part of an application, let's go for a speedy method for accessing
    // the source data.

    WFC_VALIDATE_POINTER(buffer);

    while (index < number_of_bytes)
    {
        character_1 = __get_character(buffer, m_DecoderTable, index, number_of_bytes);

        if (character_1 not_eq END_OF_BASE64_ENCODED_DATA and character_1 not_eq BASE64_END_OF_BUFFER)
        {
            if (m_DecoderTable[character_1] == BASE64_UNKNOWN_VALUE)
            {
                //WFCTRACEVAL( TEXT( "Character 1 Failed translation at index " ), (uint32_t) index );
                return(add_index);
            }

            character_2 = __get_character(buffer, m_DecoderTable, index, number_of_bytes);

            if (character_2 not_eq END_OF_BASE64_ENCODED_DATA and character_2 not_eq BASE64_END_OF_BUFFER)
            {
                if (m_DecoderTable[character_2] == BASE64_UNKNOWN_VALUE)
                {
                    //WFCTRACEVAL( TEXT( "Character 2 Failed translation at index " ), (uint32_t) index );
                    return(add_index);
                }

                character_3 = __get_character(buffer, m_DecoderTable, index, number_of_bytes);

                if (character_3 not_eq END_OF_BASE64_ENCODED_DATA and character_3 not_eq BASE64_END_OF_BUFFER)
                {
                    if (m_DecoderTable[character_3] == BASE64_UNKNOWN_VALUE)
                    {
                        //WFCTRACEVAL( TEXT( "Character 3 Failed translation at index " ), (uint32_t) index );
                        return(add_index);
                    }

                    character_4 = __get_character(buffer, m_DecoderTable, index, number_of_bytes);

                    if (character_4 not_eq END_OF_BASE64_ENCODED_DATA and character_4 not_eq BASE64_END_OF_BUFFER)
                    {
                        if (m_DecoderTable[character_4] == BASE64_UNKNOWN_VALUE)
                        {
                            //WFCTRACEVAL( TEXT( "Character 4 Failed translation at index " ), (uint32_t) index );
                            return(add_index);
                        }
                    }

                    if (character_2 == BASE64_END_OF_BUFFER or
                        character_3 == BASE64_END_OF_BUFFER or
                        character_4 == BASE64_END_OF_BUFFER)
                    {
                        //WFCTRACE( TEXT( "Not enough translatable data" ) );
                        return(add_index);
                    }
                }
                else
                {
                    character_4 = END_OF_BASE64_ENCODED_DATA;
                }
            }
            else
            {
                character_3 = END_OF_BASE64_ENCODED_DATA;
                character_4 = END_OF_BASE64_ENCODED_DATA;
            }
        }
        else
        {
            character_2 = END_OF_BASE64_ENCODED_DATA;
            character_3 = END_OF_BASE64_ENCODED_DATA;
            character_4 = END_OF_BASE64_ENCODED_DATA;
        }

        if (character_1 == END_OF_BASE64_ENCODED_DATA or
            character_2 == END_OF_BASE64_ENCODED_DATA)
        {
            return(add_index);
        }

        character_1 = m_DecoderTable[character_1];
        character_2 = m_DecoderTable[character_2];

        byte_to_add = static_cast<uint8_t>(((character_1 << 2) bitor ((character_2 bitand 0x30) >> 4)));

        _ASSERTE(add_index < destination_size);
        destination[add_index] = byte_to_add;
        add_index++;

        if (character_3 == END_OF_BASE64_ENCODED_DATA)
        {
            return(add_index);
        }

        character_3 = m_DecoderTable[character_3];

        byte_to_add = static_cast<uint8_t>(((((character_2 bitand 0x0F) << 4) bitor ((character_3 bitand 0x3C) >> 2))));

        _ASSERTE(add_index < destination_size);
        destination[add_index] = byte_to_add;
        add_index++;

        if (character_4 == END_OF_BASE64_ENCODED_DATA)
        {
            return(add_index);
        }

        character_4 = m_DecoderTable[character_4];

        byte_to_add = static_cast<uint8_t>((((character_3 bitand 0x03) << 6) bitor character_4));

        _ASSERTE(add_index < destination_size);
        destination[add_index] = byte_to_add;
        add_index++;
    }

    return(add_index);
}

_Check_return_ bool Win32FoundationClasses::CBase64Coding::Decode(_In_reads_bytes_(number_of_bytes) uint8_t const * buffer, _In_ std::size_t const number_of_bytes, _Inout_ std::vector<uint8_t>& destination) const noexcept
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER_NULL_OK(buffer);

    if (buffer == nullptr or number_of_bytes < 1)
    {
        destination.clear();
        return(true);
    }

    bool return_value = false;

    uint8_t byte_to_add = 0;
    uint8_t character_1 = 0;
    uint8_t character_2 = 0;
    uint8_t character_3 = 0;
    uint8_t character_4 = 0;

    std::size_t index = 0;

    destination.resize(DecodeReserveSize(number_of_bytes));

    uint32_t add_index = 0;

    // Since we're decoding, we are most likely in a performance-minded
    // part of an application, let's go for a speedy method for accessing
    // the source data.

    WFC_VALIDATE_POINTER(buffer);

    while (index < number_of_bytes)
    {
        character_1 = __get_character(buffer, m_DecoderTable, index, number_of_bytes);

        if (character_1 not_eq END_OF_BASE64_ENCODED_DATA and character_1 not_eq BASE64_END_OF_BUFFER)
        {
            if (m_DecoderTable[character_1] == BASE64_UNKNOWN_VALUE)
            {
                //WFCTRACEVAL( TEXT( "Character 1 Failed translation at index " ), (uint32_t) index );
                destination.resize(add_index);
                return(false);
            }

            character_2 = __get_character(buffer, m_DecoderTable, index, number_of_bytes);

            if (character_2 not_eq END_OF_BASE64_ENCODED_DATA and character_2 not_eq BASE64_END_OF_BUFFER)
            {
                if (m_DecoderTable[character_2] == BASE64_UNKNOWN_VALUE)
                {
                    //WFCTRACEVAL( TEXT( "Character 2 Failed translation at index " ), (uint32_t) index );
                    destination.resize(add_index);
                    return(false);
                }

                character_3 = __get_character(buffer, m_DecoderTable, index, number_of_bytes);

                if (character_3 not_eq END_OF_BASE64_ENCODED_DATA and character_3 not_eq BASE64_END_OF_BUFFER)
                {
                    if (m_DecoderTable[character_3] == BASE64_UNKNOWN_VALUE)
                    {
                        //WFCTRACEVAL( TEXT( "Character 3 Failed translation at index " ), (uint32_t) index );
                        destination.resize(add_index);
                        return(false);
                    }

                    character_4 = __get_character(buffer, m_DecoderTable, index, number_of_bytes);

                    if (character_4 not_eq END_OF_BASE64_ENCODED_DATA and character_4 not_eq BASE64_END_OF_BUFFER)
                    {
                        if (m_DecoderTable[character_4] == BASE64_UNKNOWN_VALUE)
                        {
                            //WFCTRACEVAL( TEXT( "Character 4 Failed translation at index " ), (uint32_t) index );
                            destination.resize(add_index);
                            return(false);
                        }
                    }

                    if (character_2 == BASE64_END_OF_BUFFER or
                        character_3 == BASE64_END_OF_BUFFER or
                        character_4 == BASE64_END_OF_BUFFER)
                    {
                        //WFCTRACE( TEXT( "Not enough translatable data" ) );
                        destination.resize(add_index);
                        return(false);
                    }
                }
                else
                {
                    character_4 = END_OF_BASE64_ENCODED_DATA;
                }
            }
            else
            {
                character_3 = END_OF_BASE64_ENCODED_DATA;
                character_4 = END_OF_BASE64_ENCODED_DATA;
            }
        }
        else
        {
            character_2 = END_OF_BASE64_ENCODED_DATA;
            character_3 = END_OF_BASE64_ENCODED_DATA;
            character_4 = END_OF_BASE64_ENCODED_DATA;
        }

        if (character_1 == END_OF_BASE64_ENCODED_DATA or
            character_2 == END_OF_BASE64_ENCODED_DATA)
        {
            destination.resize(add_index);
            return(true);
        }

        character_1 = m_DecoderTable[character_1];
        character_2 = m_DecoderTable[character_2];

        byte_to_add = static_cast<uint8_t>(((character_1 << 2) bitor ((character_2 bitand 0x30) >> 4)));

        destination[add_index] = byte_to_add;
        add_index++;

        if (character_3 == END_OF_BASE64_ENCODED_DATA)
        {
            destination.resize(add_index);
            return(true);
        }

        character_3 = m_DecoderTable[character_3];

        byte_to_add = static_cast<uint8_t>(((((character_2 bitand 0x0F) << 4) bitor ((character_3 bitand 0x3C) >> 2))));

        destination[add_index] = byte_to_add;
        add_index++;

        if (character_4 == END_OF_BASE64_ENCODED_DATA)
        {
            destination.resize(add_index);
            return(true);
        }

        character_4 = m_DecoderTable[character_4];

        byte_to_add = static_cast<uint8_t>((((character_3 bitand 0x03) << 6) bitor character_4));

        destination[add_index] = byte_to_add;
        add_index++;
    }

    destination.resize(add_index);

    return(return_value);
}

_Check_return_ bool Win32FoundationClasses::CBase64Coding::Decode( _In_ std::vector<uint8_t> const& source, _Inout_ std::vector<uint8_t>& destination ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   return(Decode(source.data(), source.size(), destination));
}

_Check_return_ bool Win32FoundationClasses::CBase64Coding::Decode(_In_ std::wstring_view source, _Inout_ std::vector<uint8_t>& destination ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   bool return_value = false;

   uint8_t byte_to_add = 0;
   uint8_t character_1 = 0;
   uint8_t character_2 = 0;
   uint8_t character_3 = 0;
   uint8_t character_4 = 0;

   std::size_t index = 0;

   destination.resize(DecodeReserveSize(source.length()) );

   uint32_t add_index = 0;

   // Since we're decoding, we are most likely in a performance-minded
   // part of an application, let's go for a speedy method for accessing
   // the source data.

   wchar_t const * input_buffer = source.data();

   WFC_VALIDATE_POINTER( input_buffer );

   while( index < source.length())
   {
      character_1 = __get_character( input_buffer, m_DecoderTable, index, source.length());

      if ( character_1 not_eq END_OF_BASE64_ENCODED_DATA )
      {
         if ( m_DecoderTable[ character_1 ] == BASE64_UNKNOWN_VALUE and character_1 not_eq BASE64_END_OF_BUFFER )
         {
            //WFCTRACEVAL( TEXT( "Character 1 Failed translation at index " ), (uint32_t) index );
            destination.resize( add_index );
            return( false );
         }

         character_2 = __get_character( input_buffer, m_DecoderTable, index, source.length());

         if ( character_2 not_eq END_OF_BASE64_ENCODED_DATA )
         {
            if ( m_DecoderTable[ character_2 ] == BASE64_UNKNOWN_VALUE and character_2 not_eq BASE64_END_OF_BUFFER )
            {
               //WFCTRACEVAL( TEXT( "Character 2 Failed translation at index " ), (uint32_t) index );
               destination.resize( add_index );
               return( false );
            }

            character_3 = __get_character( input_buffer, m_DecoderTable, index, source.length());

            if ( character_3 not_eq END_OF_BASE64_ENCODED_DATA )
            {
               if ( m_DecoderTable[ character_3 ] == BASE64_UNKNOWN_VALUE and character_3 not_eq BASE64_END_OF_BUFFER )
               {
                  //WFCTRACEVAL( TEXT( "Character 3 Failed translation at index " ), (uint32_t) index );
                  destination.resize( add_index );
                  return( false );
               }

               character_4 = __get_character( input_buffer, m_DecoderTable, index, source.length());

               if ( character_4 not_eq END_OF_BASE64_ENCODED_DATA )
               {
                  if ( m_DecoderTable[ character_4 ] == BASE64_UNKNOWN_VALUE and character_4 not_eq BASE64_END_OF_BUFFER )
                  {
                     //WFCTRACEVAL( TEXT( "Character 4 Failed translation at index " ), (uint32_t) index );
                     destination.resize( add_index );
                     return( false );
                  }
               }

               if ( character_2 == BASE64_END_OF_BUFFER or
                    character_3 == BASE64_END_OF_BUFFER or
                    character_4 == BASE64_END_OF_BUFFER )
               { 
                   if (destination.size() == add_index)
                   {
                       return(true);
                   }

                   destination.resize( add_index );
                   return( false );
               }
            }
            else
            {
               character_4 = END_OF_BASE64_ENCODED_DATA;
            }
         }
         else
         {
            character_3 = END_OF_BASE64_ENCODED_DATA;
            character_4 = END_OF_BASE64_ENCODED_DATA;
         }
      }
      else
      {
         character_2 = END_OF_BASE64_ENCODED_DATA;
         character_3 = END_OF_BASE64_ENCODED_DATA;
         character_4 = END_OF_BASE64_ENCODED_DATA;
      }

      if ( character_1 == END_OF_BASE64_ENCODED_DATA or
           character_2 == END_OF_BASE64_ENCODED_DATA )
      {
         destination.resize( add_index );
         return( true );
      }

      character_1 = m_DecoderTable[ character_1 ];
      character_2 = m_DecoderTable[ character_2 ];

      byte_to_add = static_cast<uint8_t>( ( ( character_1 << 2 ) bitor ( ( character_2 bitand 0x30 ) >> 4 ) ) );

      destination[ add_index ] = byte_to_add;
      add_index++;

      if ( character_3 == END_OF_BASE64_ENCODED_DATA )
      {
         destination.resize( add_index );
         return( true );
      }

      character_3 = m_DecoderTable[ character_3 ];

      byte_to_add = static_cast<uint8_t>( ( ( ( ( character_2 bitand 0x0F ) << 4 ) bitor ( ( character_3 bitand 0x3C ) >> 2 ) ) ) );

      destination[ add_index ] = byte_to_add;
      add_index++;

      if ( character_4 == END_OF_BASE64_ENCODED_DATA )
      {
         destination.resize( add_index );
         return( true );
      }

      character_4 = m_DecoderTable[ character_4 ];

      byte_to_add = static_cast<uint8_t>( ( ( ( character_3 bitand 0x03 ) << 6 ) bitor character_4 ) );

      destination[ add_index ] = byte_to_add;
      add_index++;
   }

   destination.resize( add_index );

   return( return_value );
}

_Check_return_ bool Win32FoundationClasses::CBase64Coding::Encode( _In_ std::vector<uint8_t> const& source, _Inout_ std::vector<uint8_t>& destination ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   auto const number_of_bytes_to_encode = source.size();

   if ( number_of_bytes_to_encode == 0 )
   {
      destination.clear();
      return( true );
   }

   // We don't want to make this static so we can reduce our
   // footprint in the library

   char const alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

   std::size_t source_index = 0;

   uint8_t byte_to_add = 0;
   uint8_t byte_1      = 0;
   uint8_t byte_2      = 0;
   uint8_t byte_3      = 0;

   auto input_buffer = source.data();

   WFC_VALIDATE_POINTER( input_buffer );

   destination.resize(EncodeReserveSize(number_of_bytes_to_encode));
   std::size_t add_index = 0;

   while( source_index < number_of_bytes_to_encode )
   {
      // Output the first byte

      byte_1 = input_buffer[ source_index ];
      byte_to_add = alphabet[ ( byte_1 >> 2 ) ];

      destination[ add_index ] = byte_to_add;
      add_index++;

      source_index++;

      if ( source_index >= number_of_bytes_to_encode )
      {
         // We're at the end of the data to encode

         byte_2 = 0;
         byte_to_add = alphabet[ ( ( ( byte_1 bitand 0x03 ) << 4 ) bitor ( ( byte_2 bitand 0xF0 ) >> 4 ) ) ];

         destination[ add_index ] = byte_to_add;
         add_index++;

         destination.resize( add_index );

         // .Net will puke on itself if the buffer isn't a length of 4
         while ((destination.size() % 4) not_eq 0)
         {
             destination.push_back(static_cast<uint8_t>(END_OF_BASE64_ENCODED_DATA));
         }

         return( true );
      }
      else
      {
         byte_2 = input_buffer[ source_index ];
      }

      byte_to_add = alphabet[ ( ( ( byte_1 bitand 0x03 ) << 4 ) bitor ( ( byte_2 bitand 0xF0 ) >> 4 ) ) ];
      destination[ add_index ] = byte_to_add;
      add_index++;

      source_index++;

      if ( source_index >= number_of_bytes_to_encode )
      {
         // We ran out of bytes, we need to add the last half of byte_2 and pad
         byte_3 = 0;

         byte_to_add = alphabet[ ( ( ( byte_2 bitand 0x0F ) << 2 ) bitor ( ( byte_3 bitand 0xC0 ) >> 6 ) ) ];

         destination[ add_index ] = byte_to_add;
         add_index++;

         destination.resize( add_index );

         // .Net will puke on itself if the buffer isn't a length of 4
         while ((destination.size() % 4) not_eq 0)
         {
             destination.push_back(static_cast<uint8_t>(END_OF_BASE64_ENCODED_DATA));
         }

         return( true );
      }
      else
      {
         byte_3 = input_buffer[ source_index ];
      }

      source_index++;

      byte_to_add = alphabet[ ( ( ( byte_2 bitand 0x0F ) << 2 ) bitor ( ( byte_3 bitand 0xC0 ) >> 6 ) ) ];

      destination[ add_index ] = byte_to_add;
      add_index++;

      byte_to_add = alphabet[ ( byte_3 bitand 0x3F ) ];

      destination[ add_index ] = byte_to_add;
      add_index++;
   }

   destination.resize( add_index );

   // .Net will puke on itself if the buffer isn't a length of 4
   while ((destination.size() % 4) not_eq 0)
   {
       destination.push_back(static_cast<uint8_t>(END_OF_BASE64_ENCODED_DATA));
   }

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CBase64Coding::Encode( _In_ std::vector<uint8_t> const& source, _Inout_ std::wstring& destination_string ) const noexcept
{
   WFC_VALIDATE_POINTER( this );

   return( Encode( source.data(), source.size(), destination_string ) );
}

_Check_return_ bool Win32FoundationClasses::CBase64Coding::Encode( __in_bcount( number_of_bytes_to_encode ) uint8_t const * input_buffer, _In_ std::size_t const number_of_bytes_to_encode, _Inout_ std::wstring& destination_string ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( input_buffer );

   if ( number_of_bytes_to_encode < 1 or input_buffer == nullptr )
   {
      destination_string.clear();
      return( true );
   }

   std::string the_string;

   if (Encode(input_buffer, number_of_bytes_to_encode, the_string) == true)
   {
       copy( destination_string, the_string);
       return(true);
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CBase64Coding::Encode(__in_bcount(number_of_bytes_to_encode) uint8_t const * input_buffer, _In_ std::size_t const number_of_bytes_to_encode, _Inout_ std::string& destination_string) const noexcept
{
    WFC_VALIDATE_POINTER(this);
    WFC_VALIDATE_POINTER_NULL_OK(input_buffer);

    if (number_of_bytes_to_encode < 1 or input_buffer == nullptr)
    {
        destination_string.clear();
        return(true);
    }

    char const alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::size_t loop_index = 0;
    std::size_t number_of_space_characters_added = 0;

    uint8_t byte_to_add = 0;
    uint8_t byte_1 = 0;
    uint8_t byte_2 = 0;
    uint8_t byte_3 = 0;

    destination_string.resize(EncodeReserveSize(number_of_bytes_to_encode));

    uint32_t character_index_in_this_line = 0;
    uint32_t number_of_bytes_encoded = 0;

    while (loop_index < number_of_bytes_to_encode)
    {
        // Output the first byte

        byte_1 = input_buffer[loop_index];
        byte_to_add = alphabet[(byte_1 >> 2)];

        destination_string[number_of_bytes_encoded] = static_cast< char >(byte_to_add);
        number_of_bytes_encoded++;

        character_index_in_this_line++;

        if ((character_index_in_this_line % BASE64_NUMBER_OF_CHARACTERS_PER_LINE) == 0)
        {
            destination_string[number_of_bytes_encoded] = static_cast< char >(CARRIAGE_RETURN);
            number_of_bytes_encoded++;
            destination_string[number_of_bytes_encoded] = static_cast< char >(LINE_FEED);
            number_of_bytes_encoded++;
            number_of_space_characters_added += 2;
            character_index_in_this_line = 0;
        }

        loop_index++;

        if (loop_index >= number_of_bytes_to_encode)
        {
            // We're at the end of the data to encode

            byte_2 = 0;
            byte_to_add = alphabet[(((byte_1 bitand 0x03) << 4) bitor ((byte_2 bitand 0xF0) >> 4))];

            destination_string[number_of_bytes_encoded] = static_cast< char >(byte_to_add);
            number_of_bytes_encoded++;

            character_index_in_this_line++;

            if ((character_index_in_this_line % BASE64_NUMBER_OF_CHARACTERS_PER_LINE) == 0)
            {
                destination_string[number_of_bytes_encoded] = static_cast< char >(CARRIAGE_RETURN);
                number_of_bytes_encoded++;
                destination_string[number_of_bytes_encoded] = static_cast< char >(LINE_FEED);
                number_of_bytes_encoded++;
                number_of_space_characters_added += 2;
                character_index_in_this_line = 0;
            }

            destination_string[number_of_bytes_encoded] = static_cast< char >(END_OF_BASE64_ENCODED_DATA);
            number_of_bytes_encoded++;

            character_index_in_this_line++;

            if ((character_index_in_this_line % BASE64_NUMBER_OF_CHARACTERS_PER_LINE) == 0)
            {
                destination_string[number_of_bytes_encoded] = static_cast<char>(CARRIAGE_RETURN);
                number_of_bytes_encoded++;
                destination_string[number_of_bytes_encoded] = static_cast<char>(LINE_FEED);
                number_of_bytes_encoded++;
                number_of_space_characters_added += 2;
                character_index_in_this_line = 0;
            }

            destination_string.resize(number_of_bytes_encoded);

            // .Net will puke on itself if the number of non-space characters isn't a multiple of 4
            while (((destination_string.length() - number_of_space_characters_added) % 4) not_eq 0)
            {
                destination_string.push_back(static_cast<char>(END_OF_BASE64_ENCODED_DATA));
            }

            return(true);
        }
        else
        {
            byte_2 = input_buffer[loop_index];
        }

        byte_to_add = alphabet[(((byte_1 bitand 0x03) << 4) bitor ((byte_2 bitand 0xF0) >> 4))];

        destination_string[number_of_bytes_encoded] = static_cast<char>(byte_to_add);
        number_of_bytes_encoded++;

        character_index_in_this_line++;

        if ((character_index_in_this_line % BASE64_NUMBER_OF_CHARACTERS_PER_LINE) == 0)
        {
            destination_string[number_of_bytes_encoded] = static_cast<char>(CARRIAGE_RETURN);
            number_of_bytes_encoded++;
            destination_string[number_of_bytes_encoded] = static_cast<char>(LINE_FEED);
            number_of_bytes_encoded++;
            number_of_space_characters_added += 2;
            character_index_in_this_line = 0;
        }

        loop_index++;

        if (loop_index >= number_of_bytes_to_encode)
        {
            // We ran out of bytes, we need to add the last half of byte_2 and pad
            byte_3 = 0;

            byte_to_add = alphabet[(((byte_2 bitand 0x0F) << 2) bitor ((byte_3 bitand 0xC0) >> 6))];

            destination_string[number_of_bytes_encoded] = static_cast< char >(byte_to_add);
            number_of_bytes_encoded++;

            character_index_in_this_line++;

            if ((character_index_in_this_line % BASE64_NUMBER_OF_CHARACTERS_PER_LINE) == 0)
            {
                destination_string[number_of_bytes_encoded] = static_cast<char>(CARRIAGE_RETURN);
                number_of_bytes_encoded++;
                destination_string[number_of_bytes_encoded] = static_cast<char>(LINE_FEED);
                number_of_bytes_encoded++;
                number_of_space_characters_added += 2;
                character_index_in_this_line = 0;
            }

            destination_string.resize(number_of_bytes_encoded);

            // .Net will puke on itself if the number of non-space characters isn't a multiple of 4
            while (((destination_string.length() - number_of_space_characters_added) % 4) not_eq 0)
            {
                destination_string.push_back(static_cast<char>(END_OF_BASE64_ENCODED_DATA));
            }

            return(true);
        }
        else
        {
            byte_3 = input_buffer[loop_index];
        }

        loop_index++;

        byte_to_add = alphabet[(((byte_2 bitand 0x0F) << 2) bitor ((byte_3 bitand 0xC0) >> 6))];

        destination_string[number_of_bytes_encoded] = static_cast<char>(byte_to_add);
        number_of_bytes_encoded++;

        character_index_in_this_line++;

        if ((character_index_in_this_line % BASE64_NUMBER_OF_CHARACTERS_PER_LINE) == 0)
        {
            destination_string[number_of_bytes_encoded] = static_cast<char>(CARRIAGE_RETURN);
            number_of_bytes_encoded++;
            destination_string[number_of_bytes_encoded] = static_cast<char>(LINE_FEED);
            number_of_bytes_encoded++;
            number_of_space_characters_added += 2;
            character_index_in_this_line = 0;
        }

        byte_to_add = alphabet[(byte_3 bitand 0x3F)];

        destination_string[number_of_bytes_encoded] = static_cast< char >(byte_to_add);
        number_of_bytes_encoded++;

        character_index_in_this_line++;

        if ((character_index_in_this_line % BASE64_NUMBER_OF_CHARACTERS_PER_LINE) == 0)
        {
            destination_string[number_of_bytes_encoded] = static_cast<char>(CARRIAGE_RETURN);
            number_of_bytes_encoded++;
            destination_string[number_of_bytes_encoded] = static_cast<char>(LINE_FEED);
            number_of_bytes_encoded++;
            number_of_space_characters_added += 2;
            character_index_in_this_line = 0;
        }
    }

    destination_string.resize(number_of_bytes_encoded);

    // .Net will puke on itself if the number of non-space characters isn't a multiple of 4
    while (((destination_string.length() - number_of_space_characters_added) % 4) not_eq 0)
    {
        destination_string.push_back(static_cast<char>(END_OF_BASE64_ENCODED_DATA));
    }

    return(true);
}

// Perform a single 3 to 4 conversion
static inline void __encode(_In_ uint8_t const * input_buffer, _In_ std::size_t const number_of_bytes, _Out_ uint8_t * output_buffer) noexcept
{
    char const alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    if (number_of_bytes > 2)
    {
        // Three Bytes to encode
        output_buffer[0] = alphabet[(input_buffer[0] >> 2)];
        output_buffer[1] = alphabet[(((input_buffer[0] bitand 0x03) << 4) bitor ((input_buffer[1] bitand 0xF0) >> 4))];
        output_buffer[2] = alphabet[(((input_buffer[1] bitand 0x0F) << 2) bitor ((input_buffer[2] bitand 0xC0) >> 6))];
        output_buffer[3] = alphabet[(input_buffer[2] bitand 0x3F)];
    }
    else if (number_of_bytes == 2)
    {
        output_buffer[0] = alphabet[(input_buffer[0] >> 2)];
        output_buffer[1] = alphabet[(((input_buffer[0] bitand 0x03) << 4) bitor ((input_buffer[1] bitand 0xF0) >> 4))];
        output_buffer[2] = alphabet[((input_buffer[1] bitand 0x0F) << 2) ];
        output_buffer[3] = '=';
    }
    else if (number_of_bytes == 1)
    {
        output_buffer[0] = alphabet[(input_buffer[0] >> 2)];
        output_buffer[1] = alphabet[((input_buffer[0] bitand 0x03) << 4)];
        output_buffer[2] = '=';
        output_buffer[3] = '=';
    }
    else
    {
        output_buffer[0] = '=';
        output_buffer[1] = '=';
        output_buffer[2] = '=';
        output_buffer[3] = '=';
    }
}

#if ! defined( WE_ARE_BUILDING_WFC_ON_UNIX )
_Check_return_ bool Win32FoundationClasses::CBase64Coding::Encode(_Inout_ HANDLE input_file_handle, _In_ std::size_t const number_of_bytes_to_encode, _Inout_ HANDLE output_file_handle ) const noexcept
{
    WFC_VALIDATE_POINTER(this);

    DWORD ignored = 0;

    if (number_of_bytes_to_encode < 1 )
    {
        uint8_t end_of_buffer[6] = { '=', '=', '=', '=', CARRIAGE_RETURN, LINE_FEED };

        if (::WriteFile(output_file_handle, end_of_buffer, sizeof(end_of_buffer), &ignored, nullptr) == FALSE)
        {
            _ASSERT_EXPR(FALSE, _CRT_WIDE("Could not write base64 encoded buffer to output file from empty input file."));
            return(false);
        }

        return(true);
    }

    uint8_t input_buffer[54]{ 0 };
    uint8_t output_buffer[74]{ 0 };

    std::size_t number_of_bytes_remaining = number_of_bytes_to_encode;

    while (number_of_bytes_remaining > 0)
    {
        std::size_t const number_of_bytes_to_read = std::min(sizeof(input_buffer), number_of_bytes_remaining);

        if (::ReadFile(input_file_handle, input_buffer, static_cast<DWORD>(number_of_bytes_to_read), &ignored, nullptr) not_eq 0)
        {
            // Now encode this buffer...
            std::size_t output_buffer_index = 0;
            std::size_t input_buffer_index = 0;

            while (input_buffer_index < number_of_bytes_to_read)
            {
                std::size_t const number_of_bytes_in_this_block = std::min(static_cast<std::size_t>(3), number_of_bytes_to_read - input_buffer_index);

                if (number_of_bytes_in_this_block == 0)
                {
                    _ASSERT_EXPR(FALSE, _CRT_WIDE("The number of bytes to base64 encode is zero."));
                    return(false);
                }

                __encode(&input_buffer[input_buffer_index], number_of_bytes_in_this_block, &output_buffer[output_buffer_index]);
                output_buffer_index += 4;
                input_buffer_index += number_of_bytes_in_this_block;
            }

            output_buffer[output_buffer_index] = CARRIAGE_RETURN;
            output_buffer_index++;
            output_buffer[output_buffer_index] = LINE_FEED;
            output_buffer_index++;

            if (::WriteFile(output_file_handle, output_buffer, static_cast<DWORD>(output_buffer_index), &ignored, nullptr) == FALSE)
            {
                _ASSERT_EXPR(FALSE, _CRT_WIDE("Could not write base64 encoded buffer to output file."));
            }
        }
        else
        {
            _ASSERT_EXPR(FALSE, _CRT_WIDE("Could not read from file to base64 encode."));
            return(false);
        }

        number_of_bytes_remaining -= number_of_bytes_to_read;
    }

    return(true);
}
#endif // WE_ARE_BUILDING_WFC_ON_UNIX

// End of source

#if ! defined( GENERATING_DOCUMENTATION )

/*
<HTML>

<HEAD>
<TITLE>WFC - CBase64Coding</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, MIME encoding, base 64, source code">
<META name="description" content="This C++ class let's you MIME encode bytes to text using base64.">
</HEAD>

<BODY>

<H1>CBase64Coding</H1>

$Revision: 22 $<BR><HR>

<H2>Description</H2>

This class gives you the ability to encode/decode data using base64.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CBase64Coding</B>()</PRE><DD>
Constructs this object.

</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>bool <B><A NAME="Decode">Decode</A></B>( const std::vector&lt;uint8_t&gt;&amp; source, std::vector&lt;uint8_t&gt;&amp; destination )
bool <B>Decode</B>( const std::wstring&amp;    source, std::vector&lt;uint8_t&gt;&amp; destination )</PRE><DD>
This method takes base64 encoded text and produces the bytes. It decodes
the base64 encoding.

<DT><PRE>bool <B><A NAME="Encode">Encode</A></B>( const std::vector&lt;uint8_t&gt;&amp; source, std::vector&lt;uint8_t&gt;&amp; destination )
bool <B>Encode</B>( const std::vector&lt;uint8_t&gt;&amp; source, std::wstring&amp;    destination )</PRE><DD>
This method takes bytes and turns them into base64 text.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   std::vector<uint8_t> bytes;

   get_file_contents( command_line_arguments[ 0 ], bytes );

   <B>CBase64Coding</B> encoder;

   std::wstring encoded_data;

   if ( encoder.Encode( bytes, encoded_data ) == true )
   {
      _tprintf( TEXT( &quot;%s\n&quot;, (LPCTSTR) encoded_data );
   }
}</CODE></PRE>
<hr /><i>Copyright, 2000-2005, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</a></i><br />
$Workfile: CBase64Coding.cpp $<BR>
$Modtime: 6/26/01 10:43a $
</BODY>

</HTML>
*/

#endif // GENERATING_DOCUMENTATION
