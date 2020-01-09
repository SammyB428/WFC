/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_APPEND_HEADER_FILE )

#define STL_APPEND_HEADER_FILE

inline void append(_Inout_ std::vector<uint8_t>& s, _In_reads_bytes_(number_of_bytes) uint8_t const* __restrict byte_buffer, _In_ std::size_t const number_of_bytes) noexcept
{
    if (byte_buffer == nullptr or number_of_bytes == 0)
    {
        return;
    }

    std::size_t const original_size = s.size();

    s.resize(original_size + number_of_bytes);

    uint8_t * __restrict memory_address = &s[original_size];

    CopyMemory(memory_address, byte_buffer, number_of_bytes);
}

inline void append(_Inout_ std::wstring& s, _In_ uint64_t const value) noexcept
{
    wchar_t number_string[64];

    _ui64tow_s(value, number_string, std::size(number_string), 10);

    s.append(number_string);
}

inline void append(_Inout_ std::wstring& s, _In_ int64_t const value) noexcept
{
    wchar_t number_string[64];

    _i64tow_s(value, number_string, std::size(number_string), 10);

    s.append(number_string);
}

inline void append_to_bytes(_In_ std::wstring_view source, _Inout_ std::vector<uint8_t>& bytes) noexcept
{
    std::size_t const number_of_bytes_in_string = source.length() * sizeof(wchar_t);
    std::size_t const original_array_size = bytes.size();

    bytes.resize(number_of_bytes_in_string + original_array_size);

    auto buffer = bytes.data();

    CopyMemory(&buffer[original_array_size], source.data(), number_of_bytes_in_string);
}

inline void append(_Inout_ std::wstring& s, _In_ int const integer) noexcept
{
    wchar_t integer_string[40]{ 0 };

    _itow_s(integer, integer_string, std::size(integer_string), 10);

    s.append(integer_string);
}

inline void append(_Inout_ std::wstring& s, _In_ long const integer) noexcept
{
    wchar_t integer_string[40]{ 0 };

    _ltow_s(integer, integer_string, std::size(integer_string), 10);

    s.append(integer_string);
}

inline void append(_Inout_ std::wstring& s, _In_ unsigned long const integer) noexcept
{
    wchar_t integer_string[40]{ 0 };

    _ultow_s(integer, integer_string, std::size(integer_string), 10);

    s.append(integer_string);
}

inline void append_integer(_Inout_ std::wstring& s, _In_ uint64_t const value) noexcept
{
    wchar_t number_string[65]{ 0 };
    (void)_ui64tow_s(value, number_string, std::size(number_string), 10);

    s.append(number_string);
}

inline void append_integer(_Inout_ std::wstring& s, _In_ int64_t const value) noexcept
{
    wchar_t number_string[65];
    (void)_i64tow_s(value, number_string, std::size(number_string), 10);

    s.append(number_string);
}

inline void append(_Inout_ std::vector<uint8_t>& bytes, _In_z_ char const* string, _In_ std::size_t const string_length) noexcept
{
    if (string_length < 1 or string == nullptr or string[0] == 0x00)
    {
        return;
    }

    if (bytes.empty() == true)
    {
        bytes.resize(string_length);
        memcpy_s(bytes.data(), bytes.size(), string, string_length);
        return;
    }

    std::size_t const original_byte_length = bytes.size();

    bytes.resize(original_byte_length + string_length);
    memcpy_s(&bytes.data()[original_byte_length], string_length, string, string_length);
}

inline void append(_Inout_ std::vector<uint8_t>& bytes, _In_z_ char const* string) noexcept
{
    if (string == nullptr or string[0] == 0x00)
    {
        return;
    }

    append(bytes, string, strlen(string));
}

inline void append_string_to_byte_array(_In_ char const* the_string, _Inout_ std::vector<uint8_t>& bytes) noexcept
{
    std::size_t const original_array_size = bytes.size();
    std::size_t const string_length = strlen(the_string);

    bytes.resize(string_length + original_array_size);

    auto buffer = bytes.data();

    CopyMemory(&buffer[original_array_size], the_string, string_length);
}

inline void append_string_to_byte_array(_In_ std::string_view the_string, _Inout_ std::vector<uint8_t>& bytes) noexcept
{
    std::size_t const original_array_size = bytes.size();

    bytes.resize(the_string.length() + original_array_size);

    auto buffer = bytes.data();

    CopyMemory(&buffer[original_array_size], the_string.data(), the_string.length());
}

#endif // STL_APPEND_HEADER_FILE
