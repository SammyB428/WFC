/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_COPY_HEADER_FILE )

#define STL_COPY_HEADER_FILE

inline _Check_return_ std::string to_utf8(_In_ std::wstring const& s) noexcept
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_converter;
    return utf8_converter.to_bytes(s);
}

/// Copy a byte buffer into a wstring without converting to Unicode will terminate at the first null.
inline void copy(_Inout_ std::wstring& s, _In_ uint8_t const * buffer, _In_ std::size_t const buffer_size) noexcept
{
    s.clear();

    if (buffer == nullptr or buffer_size < 1)
    {
        return;
    }

    s.reserve(buffer_size);

    for (auto const buffer_index : Range(buffer_size))
    {
        if (buffer[buffer_index] == 0x00)
        {
            return;
        }

        s.push_back(static_cast<wchar_t>(buffer[buffer_index]));
    }
}

inline void copy(_Inout_ std::wstring& s, std::string_view ascii_string, _In_ SSIZE_T number_of_characters = (-1), _In_ std::size_t const beginning_at = (0)) noexcept
{
    s.clear();

    if (ascii_string.empty() == true)
    {
        return;
    }

    if (number_of_characters < 0) // used to be -1 but that opened us up to errors
    {
        std::size_t loop_index = beginning_at;
        std::size_t string_size = ascii_string.length();

        if (loop_index >= string_size)
        {
            return;
        }

#if ! defined( WE_ARE_BUILDING_WFC_ON_UNIX )
        _ASSERTE(loop_index <= string_size);
#endif // WE_ARE_BUILDING_WFC_ON_UNIX

        string_size -= loop_index;

        s.reserve(string_size);

        while (loop_index < ascii_string.length() and ascii_string[loop_index] != 0)
        {
            if (ascii_string[loop_index] == 0x00)
            {
                return;
            }

            // We do all this casting to prevent "negative" characters from going in as Unicode
            // This stems from converting a signed char to a wchar_t.
            s.push_back(static_cast<wchar_t>(static_cast<uint8_t>(ascii_string[loop_index])));
            loop_index++;
        }
    }
    else
    {
        // Only append a finite number of characters...

        std::size_t const ascii_string_length = ascii_string.length();

        if (beginning_at > 0)
        {
            if (beginning_at >= ascii_string_length)
            {
                return;
            }
        }

        if (number_of_characters > static_cast<SSIZE_T>(ascii_string_length - beginning_at))
        {
            number_of_characters = (ascii_string_length - beginning_at);
        }

        s.resize(number_of_characters);

        std::size_t destination_string_index = 0;

        wchar_t* pointer = &s[0];

        for (auto const loop_index : Range(number_of_characters))
        {
            pointer[destination_string_index] = static_cast<uint8_t>(ascii_string[beginning_at + loop_index]);
#if ! defined( WE_ARE_BUILDING_WFC_ON_UNIX )
            _ASSERTE(pointer[destination_string_index] > 0 and pointer[destination_string_index] < 256);
#endif // WE_ARE_BUILDING_WFC_ON_UNIX
            destination_string_index++;
        }
    }
}

inline void copy_not_null(_Inout_ std::wstring& s, _In_z_ char const* ascii_string, _In_ SSIZE_T number_of_characters = (-1), _In_ std::size_t const beginning_at = (0)) noexcept
{
    if (ascii_string == nullptr)
    {
        s.clear();
        return;
    }

    copy(s, std::string_view(ascii_string), number_of_characters, beginning_at);
}

inline void copy(_Inout_ std::wstring& s, _In_ std::u16string_view string_view, _In_ SSIZE_T const number_of_characters = (-1), _In_ std::size_t const beginning_at = 0) noexcept
{
    s.clear();

    if (string_view.empty() == true or beginning_at >= string_view.length())
    {
        return;
    }

    std::size_t number_of_characters_to_copy = string_view.length() - beginning_at;

    if (number_of_characters > 0)
    {
        if (static_cast<std::size_t>(number_of_characters) < number_of_characters_to_copy)
        {
            number_of_characters_to_copy = static_cast<std::size_t>(number_of_characters);
        }
    }

    s.reserve(number_of_characters_to_copy);
    // Only append a finite number of characters...

    for (auto const loop_index : Range(number_of_characters_to_copy))
    {
        if ((beginning_at + loop_index) >= string_view.length())
        {
            return;
        }

        if (string_view[beginning_at + loop_index] == 0x00)
        {
            return;
        }

        s.push_back(string_view[beginning_at + loop_index]);
    }
}

inline void copy_not_null(_Inout_ std::wstring& s, _In_z_ char16_t const * wide_string, _In_ SSIZE_T number_of_characters = (-1), _In_ std::size_t const beginning_at = (0)) noexcept
{
    if (wide_string == nullptr or number_of_characters == 0)
    {
        s.clear();
        return;
    }

    copy(s, std::u16string_view(wide_string), number_of_characters, beginning_at);
}

inline void copy(_Inout_ std::wstring& s, _In_ std::wstring_view string_view, _In_ SSIZE_T const number_of_characters = (-1), _In_ std::size_t const beginning_at = 0) noexcept
{
    s.clear();

    if (string_view.empty() == true or beginning_at >= string_view.length())
    {
        return;
    }

    std::size_t number_of_characters_to_copy = string_view.length() - beginning_at;

    if (number_of_characters > 0)
    {
        if (static_cast<std::size_t>(number_of_characters) < number_of_characters_to_copy)
        {
            number_of_characters_to_copy = static_cast<std::size_t>(number_of_characters);
        }
    }

    s.reserve(number_of_characters_to_copy);
    // Only append a finite number of characters...

    for (auto const loop_index : Range(number_of_characters_to_copy))
    {
        if ((beginning_at + loop_index) >= string_view.length())
        {
            return;
        }

        if (string_view[beginning_at + loop_index] == 0x00)
        {
            return;
        }

        s.push_back(string_view[beginning_at + loop_index]);
    }
}

inline void copy_not_null(_Inout_ std::wstring& s, _In_z_ wchar_t const* wide_string, _In_ SSIZE_T number_of_characters = (-1), _In_ std::size_t const beginning_at = (0)) noexcept
{
    if (wide_string == nullptr)
    {
        s.clear();
        return;
    }

    copy(s, std::wstring_view(wide_string), number_of_characters, beginning_at);
}

#if ! defined( WE_ARE_BUILDING_WFC_ON_UNIX )

inline void copy(_Inout_ std::wstring& s, _In_ int const code_page, _In_reads_bytes_(number_of_bytes) uint8_t const* buffer, _In_ std::size_t const number_of_bytes) noexcept
{
    if (buffer == nullptr or number_of_bytes < 1)
    {
        s.resize(0);
        return;
    }

    std::size_t const new_buffer_size = (number_of_bytes * 3) + 2;

    auto output_buffer = std::make_unique<wchar_t[]>(new_buffer_size);

    ZeroMemory(output_buffer.get(), new_buffer_size * 2);

    std::size_t const number_of_characters_copied = ::MultiByteToWideChar(code_page, 0, reinterpret_cast<LPCCH>(buffer), static_cast<int>(number_of_bytes), output_buffer.get(), static_cast<int>(new_buffer_size));

    if (number_of_characters_copied > 0)
    {
        s.assign(output_buffer.get());
    }
    else
    {
        s.resize(0);
    }
}

#endif // WE_ARE_BUILDING_WFC_ON_UNIX

/// This will copy the Unicode string to a UTF-8 string.
inline void copy_to(_In_ std::wstring_view s, _Out_ char * ascii_buffer, _In_ std::size_t const buffer_size) noexcept
{
    WFC_VALIDATE_POINTER(ascii_buffer);

    if (buffer_size == 0 or ascii_buffer == nullptr)
    {
        // No buffer to write to, just give up
        return;
    }

    // NULL terminate the buffer
    ascii_buffer[0] = 0x00;
    ascii_buffer[buffer_size - 1] = 0x00;

#if ! defined( WE_ARE_BUILDING_WFC_ON_UNIX )
    // Windows version

    if (buffer_size == 1)
    {
        // No room for a single character, give up
        return;
    }

    // Looks like we may succeed, even for a single character.

    int const size_of_ascii_buffer = static_cast<int>(buffer_size);

    int const number_of_bytes_written = ::WideCharToMultiByte(CP_UTF8,
        0,
        s.data(),
        static_cast<int>(s.length()),
        ascii_buffer,
        static_cast<int>(size_of_ascii_buffer),
        nullptr,
        nullptr);

#if ! defined(WE_ARE_BUILDING_WFC_ON_UNIX)
#if defined( _DEBUG )
    if (number_of_bytes_written == 0)
    {
        DWORD const last_error = ::GetLastError();
        WFCTRACEINIT(L"ss");
        WFCTRACEERROR(last_error);
    }
#endif // _DEBUG
#endif // WE_ARE_BUILDING_WFC_ON_UNIX

    // Make sure we are null terminated
    ascii_buffer[number_of_bytes_written] = 0x00;
#else // WE_ARE_BUILDING_WFC_ON_UNIX
    // Unix version
    std::string const narrow_string = to_utf8(std::wstring(s));

    strncpy(ascii_buffer, narrow_string.c_str(), std::min(narrow_string.length(), buffer_size - 1));
#endif // WE_ARE_BUILDING_WFC_ON_UNIX
}

inline void copy_to(_In_ std::wstring_view s, wchar_t * wide_string, _In_ std::size_t const maximum_length_in_characters_including_null_terminator) noexcept
{
    WFC_VALIDATE_POINTER(wide_string);

    if (maximum_length_in_characters_including_null_terminator == 0)
    {
        // No buffer to write to, just give up
        return;
    }

    (void)wcsncpy_s(wide_string, maximum_length_in_characters_including_null_terminator, s.data(), std::min((maximum_length_in_characters_including_null_terminator - 1), s.length()));
}

inline void from_flags(_Inout_ std::wstring& s, _In_ uint64_t const flags, _In_reads_z_(number_of_names) wchar_t const* const names_of_bits[], _In_ std::size_t const number_of_names) noexcept
{
    s.resize(0);

    for ( auto const bit_number : Range(64) )
    {
        if (bit_test(flags, bit_number) == 1)
        {
            if (bit_number < number_of_names and names_of_bits != nullptr and names_of_bits[bit_number] != nullptr and names_of_bits[bit_number][0] != 0x00)
            {
                s.append(names_of_bits[bit_number]);
            }
            else
            {
                wchar_t temp_string[64];

#if defined(WE_ARE_BUILDING_WFC_ON_UNIX)
                int const number_of_characters = swprintf(temp_string, std::size(temp_string), L"Undefined(%d)", static_cast<int>(bit_number));
#else
                int const number_of_characters = swprintf_s(temp_string, std::size(temp_string), L"Undefined(%zd)", bit_number);
#endif
                s.append(temp_string, number_of_characters);
            }

            s.push_back(',');
            s.push_back(' ');
        }
    }

    trim(s);
    trim_right(s, ',');
}

inline void copy_utf8(_Inout_ std::wstring& s, _In_opt_z_ char const* ascii_string_p) noexcept
{
    if (ascii_string_p == nullptr or ascii_string_p[0] == 0x00)
    {
        s.resize(0);
        return;
    }

    // WARNING!!!! The C++ way will cause your program to blow up with an exception
    //std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_converter;
    //s = utf8_converter.from_bytes(ascii_string_p);

#if ! defined( WE_ARE_BUILDING_WFC_ON_UNIX )
    copy(s, CP_UTF8, reinterpret_cast<uint8_t const*>(ascii_string_p), strlen(ascii_string_p));
#else
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_converter;
    s = utf8_converter.from_bytes(ascii_string_p);
#endif // WE_ARE_BUILDING_WFC_ON_UNIX
}

inline void copy_utf8(_Inout_ std::wstring& s, _In_ std::vector<uint8_t> const& bytes) noexcept
{
    if (bytes.empty() == true)
    {
        s.resize(0);
        return;
    }

#if ! defined( WE_ARE_BUILDING_WFC_ON_UNIX )
    copy(s, CP_UTF8, bytes.data(), bytes.size());
#else
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_converter;
    std::string const narrow_string(reinterpret_cast<char const *>(bytes.data()), bytes.size());
    s = utf8_converter.from_bytes(narrow_string);
#endif
}

inline void copy_utf8(_Inout_ std::wstring& s, _In_ uint8_t const* bytes, _In_ std::size_t const number_of_bytes) noexcept
{
    if (bytes == nullptr or number_of_bytes < 1)
    {
        s.resize(0);
        return;
    }

#if ! defined( WE_ARE_BUILDING_WFC_ON_UNIX )
    copy(s, CP_UTF8, bytes, number_of_bytes);
#else // WE_ARE_BUILDING_WFC_ON_UNIX
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_converter;
    std::string const narrow_string(reinterpret_cast<char const*>(bytes), number_of_bytes);
    s = utf8_converter.from_bytes(narrow_string);
#endif // WE_ARE_BUILDING_WFC_ON_UNIX
}

inline void copy_beginning_at(_Inout_ std::vector<std::wstring>& destination, _In_ std::vector<std::wstring> const& source, _In_ std::size_t const starting_index) noexcept
{
    destination.clear();

    for ( auto const index : Range(source.size(), StartingRangePosition(starting_index)) )
    {
        destination.push_back(source[index]);
    }
}

inline void copy_to_clipboard(_In_ std::wstring_view the_string) noexcept
{
#if ! defined( WE_ARE_BUILDING_WFC_ON_UNIX )
    if (OpenClipboard(NULL) != FALSE)
    {
        EmptyClipboard();

        auto global_memory_handle = GlobalAlloc(GHND, (the_string.length() * sizeof(wchar_t)) + 2);

        if (global_memory_handle != NULL)
        {
            auto output_string = static_cast<wchar_t*>(GlobalLock(global_memory_handle));

            if (output_string != nullptr)
            {
                copy_to(the_string, output_string, the_string.length() + 1);

                GlobalUnlock(global_memory_handle);

                SetClipboardData(CF_UNICODETEXT, global_memory_handle);
            }
        }

        CloseClipboard();
    }
#endif // WE_ARE_BUILDING_WFC_ON_UNIX
}

#endif // STL_COPY_HEADER_FILE