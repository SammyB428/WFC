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
** $Workfile: CDataArchive.cpp $
** $Revision: 16 $
** $Modtime: 6/26/01 10:44a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

CDataArchive::CDataArchive()
{
    WFC_VALIDATE_POINTER(this);

    m_AmIWriting = false;
    m_ReadFromDataChunk_p = nullptr;
    m_WriteToDataChunk_p = nullptr;
    m_Position = 0;
}

CDataArchive::~CDataArchive()
{
    WFC_VALIDATE_POINTER(this);

    m_AmIWriting = false;
    m_ReadFromDataChunk_p = nullptr;
    m_WriteToDataChunk_p = nullptr;
    m_Position = 0;
}

_Check_return_ bool CDataArchive::IsLoading(void) const noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false)
    {
        return(true);
    }

    return(false);
}

_Check_return_ bool CDataArchive::IsStoring(void) const noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting != false)
    {
        return(true);
    }

    return(false);
}

void CDataArchive::Read(__out uint8_t& value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        value = 0;
        return;
    }

    WFC_VALIDATE_POINTER(m_ReadFromDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       value = m_ReadFromDataChunk_p->Data.at(m_Position);
       m_Position++;
    }
        WFC_CATCH_ALL
    {
       return;
    }
    WFC_END_CATCH_ALL
}

void CDataArchive::Read(__out std::vector<uint8_t>& array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        array.clear();
        return;
    }

    WFC_VALIDATE_POINTER(m_ReadFromDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       uint32_t number_of_array_entries = 0;

       Read(number_of_array_entries);

       // Make sure there are enough bytes left in the chunk to complete the read
       ASSERT((m_Position + number_of_array_entries) <= (uint32_t)m_ReadFromDataChunk_p->Data.size());

       if ((m_Position + number_of_array_entries) > (uint32_t)m_ReadFromDataChunk_p->Data.size())
       {
          array.clear();
          return;
       }

       array.resize(number_of_array_entries);

       auto buffer = m_ReadFromDataChunk_p->Data.data();

       ::memcpy(array.data(), &buffer[m_Position], number_of_array_entries);

       m_Position += number_of_array_entries;
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Read(__out std::vector<uint32_t>& array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        array.clear();
        return;
    }

    WFC_VALIDATE_POINTER(m_ReadFromDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       uint32_t number_of_array_entries = 0;

       Read(number_of_array_entries);

       // Make sure there are enough bytes left in the chunk to complete the read
       ASSERT((m_Position + (number_of_array_entries * sizeof(uint32_t))) <= (uint32_t)m_ReadFromDataChunk_p->Data.size());

       if ((m_Position + (number_of_array_entries * sizeof(uint32_t))) > (uint32_t)m_ReadFromDataChunk_p->Data.size())
       {
          array.clear();
          return;
       }

       array.resize(number_of_array_entries);

       auto buffer = m_ReadFromDataChunk_p->Data.data();

       ::memcpy(array.data(), &buffer[m_Position], number_of_array_entries * sizeof(uint32_t));

       m_Position += (number_of_array_entries * sizeof(uint32_t));
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Read(__out char& value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        value = 0;
        return;
    }

    WFC_VALIDATE_POINTER(m_ReadFromDataChunk_p);

    uint8_t character = 0;

    Read(character);

    value = character;
}

void CDataArchive::Read(__out std::vector<std::wstring>& array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    array.clear();

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        return;
    }

    WFC_VALIDATE_POINTER(m_ReadFromDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       uint32_t number_of_array_entries = 0;

       Read(number_of_array_entries);

       uint32_t index = 0;

       std::wstring string;

       while (index < number_of_array_entries)
       {
          Read(string);

          (void) array.push_back(string);
          string.clear();

          index++;
       }
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Read(__out std::wstring& value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    value.clear();

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        return;
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        wchar_t character = 0;

        Read(character);

        while (character != 0)
        {
           value.push_back( character );
           Read(character);
        }
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Read(__out std::vector<uint16_t>& array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        array.clear();
        return;
    }

    WFC_VALIDATE_POINTER(m_ReadFromDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       uint32_t number_of_array_entries = 0;

       Read(number_of_array_entries);

       // Make sure there are enough bytes left in the chunk to complete the read
       ASSERT((m_Position + (number_of_array_entries * sizeof(uint16_t))) <= (uint32_t)m_ReadFromDataChunk_p->Data.size());

       if ((m_Position + (number_of_array_entries * sizeof(uint16_t))) > (uint32_t)m_ReadFromDataChunk_p->Data.size())
       {
          array.clear();
          return;
       }

       array.resize(number_of_array_entries);

       auto buffer = m_ReadFromDataChunk_p->Data.data();

       ::memcpy(array.data(), &buffer[m_Position], number_of_array_entries * sizeof(uint16_t));

       m_Position += (number_of_array_entries * sizeof(uint16_t));
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Read(__out double& value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        value = 0.0;
        return;
    }

    WFC_VALIDATE_POINTER(m_ReadFromDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = m_ReadFromDataChunk_p->Data.data();

       value = *((double *)&buffer[m_Position]);
       m_Position += sizeof(double);
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Read(__out uint32_t& value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        value = 0;
        return;
    }

    WFC_VALIDATE_POINTER(m_ReadFromDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = m_ReadFromDataChunk_p->Data.data();

       value = *((uint32_t *)&buffer[m_Position]);

       m_Position += sizeof(uint32_t);
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Read(__out float& value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        value = 0.0;
        return;
    }

    WFC_VALIDATE_POINTER(m_ReadFromDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = m_ReadFromDataChunk_p->Data.data();

       value = *((float *)&buffer[m_Position]);
       m_Position += sizeof(float);
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Read( __out int32_t& value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        value = 0;
        return;
    }

    WFC_VALIDATE_POINTER(m_ReadFromDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = m_ReadFromDataChunk_p->Data.data();

       value = *((int32_t *)&buffer[m_Position]);
       m_Position += sizeof(int32_t);
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Read(__out uint64_t& value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        value = 0;
        return;
    }

    WFC_VALIDATE_POINTER(m_ReadFromDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = m_ReadFromDataChunk_p->Data.data();

       value = *((uint64_t *)&buffer[m_Position]);
       m_Position += sizeof(uint64_t);
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Read(__out uint16_t& value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        value = 0;
        return;
    }

    WFC_VALIDATE_POINTER(m_ReadFromDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = m_ReadFromDataChunk_p->Data.data();

       value = *((uint16_t *)&buffer[m_Position]);
       m_Position += sizeof(uint16_t);
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Read(__out wchar_t& value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting != false || m_ReadFromDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called ReadFrom() or you passed a NULL to ReadFrom()" ) );
        value = 0;
        return;
    }

    WFC_VALIDATE_POINTER(m_ReadFromDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = m_ReadFromDataChunk_p->Data.data();

       value = *((wchar_t *)&buffer[m_Position]);
       m_Position += sizeof(wchar_t);
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::ReadFrom(__in CDataChunk const * chunk_p) noexcept
{
    WFC_VALIDATE_POINTER(this);

    m_AmIWriting = false;
    m_ReadFromDataChunk_p = chunk_p;
    m_WriteToDataChunk_p = nullptr;
    m_Position = 0;
}

void CDataArchive::Write( __in uint8_t const value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    WFC_VALIDATE_POINTER(m_WriteToDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
        (void) m_WriteToDataChunk_p->Data.push_back(value);
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Write(__in char const value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    uint8_t byte = (uint8_t)value;

    Write(byte);
}

void CDataArchive::Write(__in std::vector<uint8_t> const& array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    WFC_VALIDATE_POINTER(m_WriteToDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       uint32_t number_of_array_entries = (uint32_t)array.size();

       Write(number_of_array_entries);

       m_WriteToDataChunk_p->Data.insert( std::end(m_WriteToDataChunk_p->Data), std::cbegin(array), std::cend(array));
    }
    WFC_CATCH_ALL
    {
       return;
    }
    WFC_END_CATCH_ALL
}

void CDataArchive::Write(__in std::vector<uint32_t> const& array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    Write(static_cast<uint32_t>(array.size()));

    for ( auto const entry : array )
    {
        Write(entry);
    }
}

void CDataArchive::Write(__in std::wstring const& value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    for ( auto const entry : value )
    {
        Write(entry);
    }

    // zero terminate

    wchar_t character = 0;

    Write(character);
}

void CDataArchive::Write(__in std::vector<std::wstring> const& array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    Write(static_cast<uint32_t>(array.size()));

    for ( auto const& entry : array)
    {
        Write(entry);
    }
}

void CDataArchive::Write(__in std::vector<uint16_t> const& array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    Write(static_cast<uint32_t>(array.size()));

    for (auto const entry : array)
    {
        Write(entry);
    }
}

void CDataArchive::Write(__in double const value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    WFC_VALIDATE_POINTER(m_WriteToDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = reinterpret_cast<uint8_t const *>(&value);

       int index = 0;

       while (index < sizeof(value))
       {
          (void) m_WriteToDataChunk_p->Data.push_back(buffer[index]);
          index++;
       }
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Write(__in uint32_t const value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    WFC_VALIDATE_POINTER(m_WriteToDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = reinterpret_cast<uint8_t const *>(&value);

       for (auto const index : Range(sizeof(value)))
       {
          (void) m_WriteToDataChunk_p->Data.push_back(buffer[index]);
       }
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Write(__in wchar_t const value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    WFC_VALIDATE_POINTER(m_WriteToDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = reinterpret_cast<uint8_t const *>(&value);

       int index = 0;

       while (index < sizeof(value))
       {
          (void) m_WriteToDataChunk_p->Data.push_back(buffer[index]);
          index++;
       }
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Write(__in float const value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    WFC_VALIDATE_POINTER(m_WriteToDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = reinterpret_cast<uint8_t const *>(&value);

       int index = 0;

       while (index < sizeof(value))
       {
          (void) m_WriteToDataChunk_p->Data.push_back(buffer[index]);
          index++;
       }
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Write(__in int32_t const value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    WFC_VALIDATE_POINTER(m_WriteToDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = reinterpret_cast<uint8_t const *>(&value);

       for (auto const index : Range(sizeof(value)))
       {
          (void) m_WriteToDataChunk_p->Data.push_back(buffer[index]);
       }
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Write(__in uint64_t const value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    WFC_VALIDATE_POINTER(m_WriteToDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = reinterpret_cast<uint8_t const *>(&value);

       for (auto const index : Range(sizeof(value)))
       {
          (void) m_WriteToDataChunk_p->Data.push_back(buffer[index]);
       }
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::Write(__in uint16_t const value) noexcept
{
    WFC_VALIDATE_POINTER(this);

    if (m_AmIWriting == false || m_WriteToDataChunk_p == nullptr)
    {
        //WFCTRACE( TEXT( "You haven't called WriteTo() or you passed a NULL to WriteTo()" ) );
        return;
    }

    WFC_VALIDATE_POINTER(m_WriteToDataChunk_p);

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       auto buffer = reinterpret_cast<uint8_t const *>(&value);

       for (auto const index : Range(sizeof(value)))
       {
          (void) m_WriteToDataChunk_p->Data.push_back(buffer[index]);
       }
    }
        WFC_CATCH_ALL
    {
       return;
    }
        WFC_END_CATCH_ALL
}

void CDataArchive::WriteTo(__inout CDataChunk * chunk_p) noexcept
{
    WFC_VALIDATE_POINTER(this);

    m_AmIWriting = TRUE;
    m_WriteToDataChunk_p = chunk_p;
    m_ReadFromDataChunk_p = nullptr;
    m_Position = 0;
}

// End of source
