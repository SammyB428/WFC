/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( STL_FORMAT_HEADER_FILE )

#define STL_FORMAT_HEADER_FILE

#define STRING_VIEW(_x)  std::string_view(  _x, std::size(_x) - 1)
#define WSTRING_VIEW(_x) std::wstring_view( _x, std::size(_x) - 1)

// sprintf replacement code
// https://msdn.microsoft.com/en-us/magazine/dn913181.aspx
// http://en.cppreference.com/w/cpp/io/c/fprintf

template <typename T>
T PrintfArgument(T value) noexcept
{
    return value;
}

template <typename T>
T const* PrintfArgument(std::basic_string<T> const& value) noexcept
{
    return value.c_str();
}

inline wchar_t const* PrintfArgument(std::filesystem::path const& value) noexcept
{
    return(value.c_str());
}

template <typename ... Args>
void format(_Inout_ std::wstring& result, _In_z_ _Printf_format_string_ wchar_t const* format_string, Args const& ... args) noexcept
{
#pragma warning( push )
#pragma warning( disable : 4996 )
    int const number_of_characters_to_allocate = _snwprintf(nullptr, 0, format_string, PrintfArgument(args) ...);
#pragma warning( pop )

    if (number_of_characters_to_allocate < 1)
    {
        result.clear();
        return;
    }

    result.resize(static_cast<std::size_t>(number_of_characters_to_allocate) + 1);

    std::size_t const number_of_characters_written = _snwprintf_s(&result[0], static_cast<std::size_t>(number_of_characters_to_allocate) + 1, _TRUNCATE, format_string, PrintfArgument(args) ...);

    if (number_of_characters_written >= 0)
    {
        // Get rid of the trailing null
        result.resize(number_of_characters_written);
    }
}

template <typename ... Args>
void format(_Inout_ std::string& result, _In_z_ _Printf_format_string_ char const* format_string, Args const& ... args) noexcept
{
#pragma warning( push )
#pragma warning( disable : 4996 )
    int const number_of_characters_to_allocate = _snprintf(nullptr, 0, format_string, PrintfArgument(args) ...);
#pragma warning( pop )

    if (number_of_characters_to_allocate < 1)
    {
        result.clear();
        return;
    }

    result.resize(number_of_characters_to_allocate + 1);

    int const number_of_characters_written = _snprintf_s(&result[0], number_of_characters_to_allocate + 1, _TRUNCATE, format_string, PrintfArgument(args) ...);

    if (number_of_characters_written >= 0)
    {
        // Get rid of the trailing null
        result.resize(number_of_characters_written);
    }
}

inline void format_number(_Inout_ std::wstring& s, _In_ uint64_t const source) noexcept
{
    wchar_t number_string[64];

    swprintf_s(number_string, std::size(number_string), L"%I64u", source);

    wchar_t formatted_string[65];

    formatted_string[0] = 0x00;
    formatted_string[64] = 0x00;

    NUMBERFMT number_format;

    ZeroMemory(&number_format, sizeof(number_format));

    number_format.Grouping = 3;
    number_format.NegativeOrder = 1;

    wchar_t decimal_separator[16];

    decimal_separator[0] = 0x00;
    decimal_separator[15] = 0x00;

    ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_SDECIMAL, decimal_separator, static_cast<int>(std::size(decimal_separator)));

    number_format.lpDecimalSep = decimal_separator;

    wchar_t thousands_separator[16];

    thousands_separator[0] = 0x00;
    thousands_separator[15] = 0x00;

    ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_STHOUSAND, thousands_separator, static_cast<int>(std::size(thousands_separator)));

    number_format.lpThousandSep = thousands_separator;

    ::GetNumberFormatEx(LOCALE_NAME_USER_DEFAULT,
        0,
        number_string,
        &number_format,
        formatted_string,
        static_cast<int>(std::size(formatted_string) - 1));

    s.assign(formatted_string);
}

inline void format_number(_Inout_ std::wstring& s, _In_ int64_t const source) noexcept
{
    wchar_t number_string[64];

    swprintf_s(number_string, std::size(number_string), L"%I64d", source);

    wchar_t formatted_string[65];

    formatted_string[0] = 0x00;
    formatted_string[64] = 0x00;

    NUMBERFMT number_format;

    ZeroMemory(&number_format, sizeof(number_format));

    number_format.Grouping = 3;
    number_format.NegativeOrder = 1;

    wchar_t decimal_separator[16];

    decimal_separator[0] = 0x00;
    decimal_separator[15] = 0x00;

    ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_SDECIMAL, decimal_separator, static_cast<int>(std::size(decimal_separator)));

    number_format.lpDecimalSep = decimal_separator;

    wchar_t thousands_separator[16];

    thousands_separator[0] = 0x00;
    thousands_separator[15] = 0x00;

    ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_STHOUSAND, thousands_separator, static_cast<int>(std::size(thousands_separator)));

    number_format.lpThousandSep = thousands_separator;

    ::GetNumberFormatEx(LOCALE_NAME_USER_DEFAULT,
        0,
        number_string,
        &number_format,
        formatted_string,
        static_cast<int>(std::size(formatted_string) - 1));

    s.assign(formatted_string);
}

inline void format_number(_Inout_ std::wstring& s, _In_ double const source, _In_ std::size_t const number_of_decimal_places) noexcept
{
    wchar_t number_string[64];

    swprintf_s(number_string, std::size(number_string), L"%f", source);

    wchar_t formatted_string[65];

    formatted_string[0] = 0x00;
    formatted_string[64] = 0x00;

    NUMBERFMT number_format;

    ZeroMemory(&number_format, sizeof(number_format));

    number_format.NumDigits = static_cast<UINT>(number_of_decimal_places);
    number_format.Grouping = 3;
    number_format.NegativeOrder = 1;

    wchar_t decimal_separator[16];

    decimal_separator[0] = 0x00;
    decimal_separator[15] = 0x00;

    ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_SDECIMAL, decimal_separator, static_cast<int>(std::size(decimal_separator)));

    number_format.lpDecimalSep = decimal_separator;

    wchar_t thousands_separator[16];

    thousands_separator[0] = 0x00;
    thousands_separator[15] = 0x00;

    ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_STHOUSAND, thousands_separator, static_cast<int>(std::size(thousands_separator)));

    number_format.lpThousandSep = thousands_separator;

    ::GetNumberFormatEx(LOCALE_NAME_USER_DEFAULT,
        0,
        number_string,
        &number_format,
        formatted_string,
        static_cast<int>(std::size(formatted_string) - 1));

    s.assign(formatted_string);
}

inline void friendly_byte_count(_Inout_ std::wstring& s, _In_ uint64_t const byte_count) noexcept
{
    if (byte_count < ONE_KILOBYTE)
    {
        format_number(s, byte_count);
        s.append(WSTRING_VIEW(L" bytes"));
    }
    else if (byte_count < ONE_MEGABYTE)
    {
        double const units = (double)((double)byte_count / (double)ONE_KILOBYTE);

        format(s, L"%.02lfKB", units);
    }
    else if (byte_count < ONE_GIGABYTE)
    {
        double const units = (double)((double)byte_count / (double)ONE_MEGABYTE);

        format(s, L"%.02lfMB", units);
    }
    else if (byte_count < ONE_TERABYTE)
    {
        double const units = (double)((double)byte_count / (double)ONE_GIGABYTE);

        format(s, L"%.02lfGB", units);
    }
    else if (byte_count < ONE_PETABYTE)
    {
        double const units = (double)((double)byte_count / (double)ONE_TERABYTE);

        format(s, L"%.02lfTB", units);
    }
    else if (byte_count < ONE_EXABYTE)
    {
        double const units = (double)((double)byte_count / (double)ONE_PETABYTE);

        format(s, L"%.02lfPB", units);
    }
    else
    {
        double const units = (double)((double)byte_count / (double)ONE_EXABYTE);

        format(s, L"%.02lfEB", units);
    }
}

#endif // STL_FORMAT_HEADER_FILE
