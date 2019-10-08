/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2019, Samuel R. Blackburn
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
** $Workfile: test_CXMLDocument.cpp $
** $Revision: 2 $
** $Modtime: 5/13/00 8:22a $
*/

#include "test.h"
#pragma hdrstop

// This is not a good test, it is here to simply test
// encoding options when writing xml out

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

_Check_return_ bool test_CXMLDocument( _Out_ std::string& class_name, _Out_ int& test_number_that_failed ) noexcept
{
    class_name.assign( "CExtensibleMarkupLanguageDocument" );
    test_number_that_failed = 0;

    CExtensibleMarkupLanguageDocument xml;

    std::vector<uint8_t> bytes;

    (void) xml.SetWriteOptions( WFC_XML_WRITE_AS_UNICODE | WFC_XML_WRITE_AS_BIG_ENDIAN );

    xml.WriteTo( bytes );

    if ( bytes.empty() == true )
    {
        test_number_that_failed = 1;
        return( failure() );
    }

    ASSERT( bytes.size() > 5 );

    // Check to make sure the Big End was In first
    // Big End In, big endian, get it?

    if ( bytes.at( 0 ) != 0xFE )// fe
    {
        test_number_that_failed = 2;
        return( failure() );
    }

    if ( bytes.at( 1 ) != 0xFF )// ff
    {
        test_number_that_failed = 3;
        return( failure() );
    }

    if ( bytes.at( 2 ) != 0x00 )
    {
        test_number_that_failed = 4;
        return( failure() );
    }

    if ( bytes.at( 3 ) != 0x3C )
    {
        test_number_that_failed = 5;
        return( failure() );
    }

    if ( bytes.at( 4 ) != 0x00 )
    {
        test_number_that_failed = 6;
        return( failure() );
    }

    if ( bytes.at( 5 ) != 0x3F )
    {
        test_number_that_failed = 7;
        return( failure() );
    }

    bytes.clear();

    // UNICODE, little endian

    (void) xml.SetWriteOptions( WFC_XML_WRITE_AS_UNICODE );

    xml.WriteTo( bytes );

    if ( bytes.size() == 0 )
    {
        test_number_that_failed = 8;
        return( failure() );
    }

    ASSERT( bytes.size() > 5 );

    // Check to make sure the Little End was In first
    // Little End In, little endian, get it?

    if ( bytes.at( 0 ) != 0xFF )
    {
        test_number_that_failed = 9;
        return( failure() );
    }

    if ( bytes.at( 1 ) != 0xFE )
    {
        test_number_that_failed = 10;
        return( failure() );
    }

    if ( bytes.at( 2 ) != 0x3C )
    {
        test_number_that_failed = 11;
        return( failure() );
    }

    if ( bytes.at( 3 ) != 0x00 )
    {
        test_number_that_failed = 12;
        return( failure() );
    }

    if ( bytes.at( 4 ) != 0x3F )
    {
        test_number_that_failed = 13;
        return( failure() );
    }

    if ( bytes.at( 5 ) != 0x00 )
    {
        test_number_that_failed = 14;
        return( failure() );
    }

    bytes.clear();

    // UCS4, Big Endian

    (void) xml.SetWriteOptions( WFC_XML_WRITE_AS_UCS4 | WFC_XML_WRITE_AS_BIG_ENDIAN );

    xml.WriteTo( bytes );

    if ( bytes.size() == 0 )
    {
        test_number_that_failed = 15;
        return( failure() );
    }

    ASSERT( bytes.size() > 3 );

    if ( bytes.at( 0 ) != 0x00 )
    {
        test_number_that_failed = 16;
        return( failure() );
    }

    if ( bytes.at( 1 ) != 0x00 )
    {
        test_number_that_failed = 17;
        return( failure() );
    }

    if ( bytes.at( 2 ) != 0x00 )
    {
        test_number_that_failed = 18;
        return( failure() );
    }

    if ( bytes.at( 3 ) != 0x3C )
    {
        test_number_that_failed = 19;
        return( failure() );
    }

    bytes.clear();

    // UCS4, Little Endian

    (void) xml.SetWriteOptions( WFC_XML_WRITE_AS_UCS4 );

    xml.WriteTo( bytes );

    if ( bytes.size() == 0 )
    {
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 16\n" ) );
        return( failure() );
    }

    ASSERT( bytes.size() > 3 );

    if ( bytes.at( 0 ) != 0x3C )
    {
        ASSERT( FALSE );
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 17\n" ) );
        return( failure() );
    }

    if ( bytes.at( 1 ) != 0x00 )
    {
        ASSERT( FALSE );
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 18\n" ) );
        return( failure() );
    }

    if ( bytes.at( 2 ) != 0x00 )
    {
        ASSERT( FALSE );
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 19\n" ) );
        return( failure() );
    }

    if ( bytes.at( 3 ) != 0x00 )
    {
        ASSERT( FALSE );
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 20\n" ) );
        return( failure() );
    }

    bytes.clear();

    // UCS4, Unusal 2143

    (void) xml.SetWriteOptions( WFC_XML_WRITE_AS_UCS4_UNUSUAL_2143 );

    xml.WriteTo( bytes );

    if ( bytes.size() == 0 )
    {
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 21\n" ) );
        return( failure() );
    }

    ASSERT( bytes.size() > 3 );

    if ( bytes.at( 0 ) != 0x00 )
    {
        ASSERT( FALSE );
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 22\n" ) );
        return( failure() );
    }

    if ( bytes.at( 1 ) != 0x00 )
    {
        ASSERT( FALSE );
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 23\n" ) );
        return( failure() );
    }

    if ( bytes.at( 2 ) != 0x3C )
    {
        ASSERT( FALSE );
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 24\n" ) );
        return( failure() );
    }

    if ( bytes.at( 3 ) != 0x00 )
    {
        ASSERT( FALSE );
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 25\n" ) );
        return( failure() );
    }

    bytes.clear();

    // UCS4, Unusal 3412

    (void) xml.SetWriteOptions( WFC_XML_WRITE_AS_UCS4_UNUSUAL_3412 );

    xml.WriteTo( bytes );

    if ( bytes.size() == 0 )
    {
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 26\n" ) );
        return( failure() );
    }

    ASSERT( bytes.size() > 3 );

    if ( bytes.at( 0 ) != 0x00 )
    {
        ASSERT( FALSE );
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 27\n" ) );
        return( failure() );
    }

    if ( bytes.at( 1 ) != 0x3C )
    {
        ASSERT( FALSE );
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 28\n" ) );
        return( failure() );
    }

    if ( bytes.at( 2 ) != 0x00 )
    {
        ASSERT( FALSE );
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 29\n" ) );
        return( failure() );
    }

    if ( bytes.at( 3 ) != 0x00 )
    {
        ASSERT( FALSE );
        _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 30\n" ) );
        return( failure() );
    }

    //bytes.Append( (const BYTE *) ( (LPCTSTR) test_text ), test_text.GetLength() * sizeof( TCHAR ) );

    CDataParser parser;

    (void) parser.Initialize( &bytes );

    DWORD parse_options = xml.GetParseOptions();

    (void) xml.SetParseOptions( parse_options | WFC_XML_ALLOW_AMPERSANDS_IN_ELEMENTS );

//    if ( xml.Parse( parser ) != true )
  // {
        //ASSERT( FALSE );
 //       _tprintf( TEXT( "CExtensibleMarkupLanguageDocument failed test 31\n" ) );
   //     return( failure() );
    //}

    std::vector<uint8_t> output;

    xml.WriteTo( output );
    (void) output.push_back( 0 );
    (void) output.push_back( 0 ); // NULL terminate the string

    CString test_string_2;

    test_string_2.Copy( (LPCTSTR) output.data() );

    //WFCTRACEVAL( TEXT( "Value is " ), test_string_2 );

    char const * part_1 = "<?xml version=\"1.0\"  encoding=\"utf-8\" standalone=\"yes\"?><mhmd>\0";
    char const * part_2 = "</mhmd>\0";

    output.clear();

    int part_index = 0;

    while (part_1[part_index] != 0x00)
    {
        output.push_back(part_1[part_index]);
        part_index++;
    }

    static constexpr uint8_t const mhmd[8] = {
        0xD9, 0x85, 0xD8, 0xAD, 0xD9, 0x85, 0xD8, 0xAF
    };

    for ( auto const mhmd_index : Range(sizeof(mhmd)))
    {
        output.push_back(mhmd[mhmd_index]);
    }

    part_index = 0;

    while (part_2[part_index] != 0x00)
    {
        output.push_back(part_2[part_index]);
        part_index++;
    }

    (void)parser.Initialize(output.data(), output.size());

    parser.SetTextToASCII(true);

    xml.Empty();

    if (xml.Parse(parser) == false)
    {
        std::wstring parser_error_message;
        xml.GetParsingErrorInformation(parser_error_message);

        wprintf(L"%s\n", parser_error_message.c_str());

        test_number_that_failed = 20;
        ASSERT(FALSE);
        _tprintf(TEXT("CExtensibleMarkupLanguageDocument failed test 39\n"));
        return(failure());
    }

    auto root = xml.GetRootElement();

    if (root == nullptr)
    {
        test_number_that_failed = 21;
        ASSERT(FALSE);
        _tprintf(TEXT("CExtensibleMarkupLanguageDocument failed test 40\n"));
        return(failure());
    }

    auto mhmd_element = root->GetChild(L"mhmd");

    if (mhmd_element == nullptr)
    {
        test_number_that_failed = 22;
        ASSERT(FALSE);
        _tprintf(TEXT("CExtensibleMarkupLanguageDocument failed test 41\n"));
        return(failure());
    }

    std::wstring mhmd_string;

    mhmd_element->GetText(mhmd_string);

    if (mhmd_string.length() != 4)
    {
        test_number_that_failed = 23;
        ASSERT(FALSE);
        return(failure());
    }

    if (mhmd_string.at(0) != 0x645)
    {
        test_number_that_failed = 24;
        ASSERT(FALSE);
        return(failure());
    }

    if (mhmd_string.at(1) != 0x62D)
    {
        test_number_that_failed = 25;
        ASSERT(FALSE);
        return(failure());
    }

    if (mhmd_string.at(2) != 0x645)
    {
        test_number_that_failed = 26;
        ASSERT(FALSE);
        return(failure());
    }

    if (mhmd_string.at(3) != 0x62F)
    {
        test_number_that_failed = 27;
        ASSERT(FALSE);
        return(failure());
    }

    std::wstring sam(L"Sam");

    bytes.clear();

    bytes.push_back(1);

    append_to_bytes(sam, bytes);

    if (bytes.size() != 7)
    {
        test_number_that_failed = 28;
        ASSERT(FALSE);
        return(failure());
    }

    if (bytes.at(0) != 1)
    {
        test_number_that_failed = 29;
        ASSERT(FALSE);
        return(failure());
    }

    if (bytes.at(1) != 'S')
    {
        test_number_that_failed = 30;
        ASSERT(FALSE);
        return(failure());
    }

    if (bytes.at(2) != 0)
    {
        test_number_that_failed = 31;
        ASSERT(FALSE);
        return(failure());
    }

    if (bytes.at(3) != 'a')
    {
        test_number_that_failed = 32;
        ASSERT(FALSE);
        return(failure());
    }

    if (bytes.at(4) != 0)
    {
        test_number_that_failed = 33;
        ASSERT(FALSE);
        return(failure());
    }

    if (bytes.at(5) != 'm')
    {
        test_number_that_failed = 34;
        ASSERT(FALSE);
        return(failure());
    }

    if (bytes.at(6) != 0)
    {
        test_number_that_failed = 35;
        ASSERT(FALSE);
        return(failure());
    }

    // 2000-05-07
    // Test provided by Juro Gottweis (juro@asc.sk)

    xml.Empty();

    bytes.clear();

    std::string test_text("<?xml version=\"1.0\" standalone=\"yes\"?>\n");

    test_text.append("<prop>Hello</prop> <prop>World</prop>");

    test_text.assign("<?xml version=\"1.0\" standalone=\"yes\"?>\n");
    test_text.append("<root><prop>Hello</prop> <prop>World</prop></root>");

    (void)parser.Initialize(reinterpret_cast<uint8_t const *>(test_text.data()), test_text.length());

    parser.SetTextToASCII();

    xml.Parse(parser);
    xml.Trim();

    root = xml.GetRootElement();

    std::wstring current_text;

    root->GetText(current_text);

    if (current_text.compare(WSTRING_VIEW(L"HelloWorld")) != I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 36;
        ASSERT(FALSE);
        return(failure());
    }

    test_number_that_failed = 36;
    return( true );
}
