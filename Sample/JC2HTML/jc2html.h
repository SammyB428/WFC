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
** $Workfile: wfc.h $
** $Revision: 68 $
** $Modtime: 8/08/00 6:19p $
*/

#if ! defined( JC_TO_HTML_HEADER_FILE_INCLUDED )

#define JC_TO_HTML_HEADER_FILE_INCLUDED

#if ! defined( WINVER )
#define WINVER 0x401
#endif // WINVER

#define WFC_STL
#include <wfc.h>

#include <vector>
#include <map>
#include <string>

USING_WFC_NAMESPACE

typedef std::vector<std::wstring> StringVector;
typedef std::map<short, std::wstring> Int2StringMap;

inline void replaceChar(std::wstring &aString, TCHAR aReplace, TCHAR aChange)
{
   for( int i  = aString.find( aReplace ); i != std::wstring::npos; i = aString.find( aReplace, ++i ) )
   {
      aString[ i ] = aChange;
   }
}

inline void reverseByte( void *value, int size )
{
    unsigned char *p1 = (unsigned char *)value;
    unsigned char *p2 = p1 + size - 1;
    unsigned char temp = 0;
    while (p2 > p1)
    {
       temp = *p1;
       *p1++ = *p2;
       *p2-- = temp;
    }
}

#include "JavaDefinitions.h"
#include "CConstantValue.hpp"
#include "CMember.hpp"
#include "CMethod.hpp"
typedef std::map<std::wstring, CMethod *> String2MethodMap;
#include "CField.hpp"
typedef std::map<std::wstring, CField *> String2FieldMap;
#include "CClassFile.hpp"

#endif // JC_TO_HTML_HEADER_FILE_INCLUDED
