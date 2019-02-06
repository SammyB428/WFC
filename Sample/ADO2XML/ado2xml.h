#if ! defined( ADO2XML_HEADER_FILE )

/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2003, Samuel R. Blackburn
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
** $Workfile: ado2xml.h $
** $Revision: 1 $
** $Modtime: 3/19/00 7:44a $
** $Reuse Tracing Code: 1 $
*/

#define ADO2XML_HEADER_FILE

#define _WIN32_WINNT (0x0400)

#define WFC_STL
#include <wfc.h>

#define _INC_MALLOC
#import "c:\program files\Common Files\System\ADO\msado15.dll" no_namespace rename( "EOF", "EndOfFile" )

#include <icrsint.h>

USING_WFC_NAMESPACE

#include "CDatabaseParameters.hpp"

void convert_DataTypeEnum_to_string( DWORD data_type_enum, CString& string_to_fill );
void convert_desired_tables( const CExtensibleMarkupLanguageElement * settings, const CDatabaseParameters& database_parameters );
void get_blob( FieldPtr& field_p, CByteArray& blob );
void normalize_string( CString& string_to_normalize );
void output_field( FieldPtr& field );
void output_types( DWORD data_type_enum );
void print_COM_error( _com_error& exception );
void print_provider_error( _ConnectionPtr );
void replace( CString& string, const CString& what_to_replace, const CString& what_to_replace_it_with );

bool convert_table( _ConnectionPtr& connection_p, const CString& table_name, const CString& where_clause );
bool get_table_names( _ConnectionPtr& connection_p, CStringArray& table_names );
bool open_connection_to_database( const CDatabaseParameters& database_parameters, _ConnectionPtr& connection_p );
bool output_schema( _ConnectionPtr& connection_p, const CString& table_name, const CString& where_clause );

#endif // ADO2XML_HEADER_FILE
