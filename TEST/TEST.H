/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2020, Samuel R. Blackburn
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
** $Workfile: TEST.H $
** $Revision: 13 $
** $Modtime: 5/21/00 7:37a $
*/

#if ! defined( TESTING_HEADER_FILE_INCLUDED )

#define TESTING_HEADER_FILE_INCLUDED

#define WFC_SILENT
#include <ciso646>
#include "../include/wfc.h"

// Standard testing functions

_Check_return_ bool failure( void ) noexcept;

using TEST_FUNCTION = bool(*)( __out std::string&, __out int& );

_Check_return_ bool CQueue_integrity_test( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool CQueue_stress_test( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_CBase64Coding( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_CBitArray( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_CDataParser( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_CFile64( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_CSystemTime( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_CWideString( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_CQueue( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_CJulianDate( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_CRandomNumberGenerator( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_CReedSolomonErrorCorrectionCode( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_CUniformResourceLocator( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_CUUCoding( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_CXMLDocument( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_process_list( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_wfc_protect( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_is( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
_Check_return_ bool test_find(__out std::string& class_name, __out int& test_number_that_failed) noexcept;

void memory( void );
void ReportError( __in DWORD const error_code ) noexcept;
void test_CloseHandle( void ) noexcept;
void test_CEventLog( void ) noexcept;
void test_CFileTransferProtocol( void ) noexcept;
void test_CLZFile( _In_ std::wstring_view lz_file_name ) noexcept;
_Check_return_ bool test_CNetResource( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
void test_CNetSession( LPCTSTR machine_name ) noexcept;
void test_CNetwork( LPCTSTR machine_name ) noexcept;
_Check_return_ bool test_CNetWorkstation( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
void test_CNetworkConnections( __in_z LPCTSTR machine_name ) noexcept;
void test_CNetworkFiles( __in_z LPCTSTR machine_name ) noexcept;
void test_CNetworkShares( __in_z LPCTSTR machine_name ) noexcept;
void test_CNetworkUsers( __in_z LPCTSTR machine_name ) noexcept;
void test_CReedSolomonErrorCorrectionCode( void ) noexcept;
_Check_return_ bool test_CRegistry( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
void test_CRAS( void ) noexcept;
void test_CSerialFile( void ) noexcept;
void test_CServer( __in_z LPCTSTR machine_name ) noexcept;
_Check_return_ bool test_CServiceControlManager( __out std::string& class_name, __out int& test_number_that_failed ) noexcept;
void test_CTape( __in const UINT tape_drive_number_starting_at_zero ) noexcept;

_Check_return_ LPTSTR BigNumToString( LONG lNum, LPTSTR string ) noexcept;

#endif // TESTING_HEADER_FILE_INCLUDED
