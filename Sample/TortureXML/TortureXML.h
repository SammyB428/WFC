#if ! defined( XML_TORTURE_TEST_HEADER_FILE )

#define XML_TORTURE_TEST_HEADER_FILE

/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 1996-2003, Samuel R. Blackburn
** All rights reserved.
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
*/

#define WFC_STL
#define WINVER 0x0401
#define _WIN32_WINNT 0x0401
#include <wfc.h>

USING_WFC_NAMESPACE

// For generating random characters of different classes

// BaseChar characters
#define MINIMUM_BaseChar (0x41)
#define MAXIMUM_BaseChar (0xD7A3)
#define GET_RANDOM_BaseChar( r, x ) { do { x = (WCHAR) ( ( r.GetInteger() % \
( MAXIMUM_BaseChar - MINIMUM_BaseChar ) ) + MINIMUM_BaseChar ); \
while( is_xml_BaseChar( x ) == FALSE ); }

// CombiningChar characters
#define MINIMUM_CombiningChar (0x0300)
#define MAXIMUM_CombiningChar (0x309A)
#define GET_RANDOM_CombiningChar( r, x ) { do { x = (WCHAR) ( ( r.GetInteger() % \
( MAXIMUM_CombiningChar - MINIMUM_CombiningChar ) ) + MINIMUM_CombiningChar ); \
while( is_xml_CombiningChar( x ) == FALSE ); }

// Digit characters
#define MINIMUM_Digit (0x0030)
#define MAXIMUM_Digit (0x0F29)
#define GET_RANDOM_Digit( r, x ) { do { x = (WCHAR) ( ( r.GetInteger() % \
( MAXIMUM_Digit - MINIMUM_Digit ) ) + MINIMUM_Digit ); \
while( is_xml_Digit( x ) == FALSE ); }

// Extender characters
#define MINIMUM_Extender (0x00B7)
#define MAXIMUM_Extender (0x30FE)
#define GET_RANDOM_Extender( r, x ) { do { x = (WCHAR) ( ( r.GetInteger() % \
( MAXIMUM_Extender - MINIMUM_Extender ) ) + MINIMUM_Extender ); \
while( is_xml_Extender( x ) == FALSE ); }

// Ideographic characters
#define MINIMUM_Ideographic (0x3007)
#define MAXIMUM_Ideographic (0x9FA5)
#define GET_RANDOM_Ideographic( r, x ) { do { x = (WCHAR) ( ( r.GetInteger() % \
( MAXIMUM_Ideographic - MINIMUM_Ideographic ) ) + MINIMUM_Ideographic ); \
while( is_xml_Ideographic( x ) == FALSE ); }

// Letter characters
#define MINIMUM_Letter (0x41)
#define MAXIMUM_Letter (0x9FA5)
#define GET_RANDOM_Letter( r, x ) { do { x = (WCHAR) ( ( r.GetInteger() % \
( MAXIMUM_Letter - MINIMUM_Letter ) ) + MINIMUM_Letter ); \
while( is_xml_Letter( x ) == FALSE ); }

// NameChar characters
#define MINIMUM_NameChar (0x41)
#define MAXIMUM_NameChar (0x9FA5)
#define GET_RANDOM_NameChar( r, x ) { do { x = (WCHAR) ( ( r.GetInteger() % \
( MAXIMUM_NameChar - MINIMUM_NameChar ) ) + MINIMUM_NameChar ); \
while( is_xml_NameChar( x ) == FALSE ); }

// Char characters
#define MINIMUM_Char (0x09)
#define MAXIMUM_Char (0x10FFFF)
#define GET_RANDOM_Char( r, x ) { do { x = (DWORD) ( ( r.GetInteger() % \
( MAXIMUM_Char - MINIMUM_Char ) ) + MINIMUM_Char ); \
while( is_xml_NameChar( x ) == FALSE ); }

void create_huge_character_reference(   LPCTSTR filename, DWORD size, CRandomNumberGenerator2& random );
void create_huge_element(               LPCTSTR filename, DWORD size, CRandomNumberGenerator2& random );
void create_huge_element_and_attribute( LPCTSTR filename, DWORD size, CRandomNumberGenerator2& random );
void create_huge_element_name(          LPCTSTR filename, DWORD size, CRandomNumberGenerator2& random );
void create_huge_encoding(              LPCTSTR filename, DWORD size, CRandomNumberGenerator2& random );
void create_huge_version(               LPCTSTR filename, DWORD size, CRandomNumberGenerator2& random );

#endif // XML_TORTURE_TEST_HEADER_FILE
