#include "jc2html.h"
#pragma hdrstop

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

// Based on Braem Franky's work in hsi ClassBrowser WTL sample app
// Author : Braem Franky
// e-mail : Franky.Braem17@yucom.be
// web    : www.braem17.yucom.be

CConstantValue::CConstantValue(long aValue)
{
   m_Type = CONSTANT_Long;
   m_Value.longValue = aValue;
}
CConstantValue::CConstantValue(int aValue)
{
   m_Type = CONSTANT_Integer;
   m_Value.integerValue = aValue;
}
CConstantValue::CConstantValue(float aValue)
{
   m_Type = CONSTANT_Float;
   m_Value.floatValue = aValue;
}
CConstantValue::CConstantValue(double aValue)
{
   m_Type = CONSTANT_Double;
   m_Value.doubleValue = aValue;
}

CConstantValue::CConstantValue(std::wstring aValue)
{
   m_Type = CONSTANT_String;
   m_Value.stringValue = new wchar_t[aValue.length() + 1];
   wcscpy( m_Value.stringValue, aValue.c_str() );
}

CConstantValue::~CConstantValue()
{
   if ( m_Type == CONSTANT_String )
   {
      delete m_Value.stringValue;
      m_Value.stringValue = NULL;
   }
}

std::wstring CConstantValue::toString(void)
{
   std::wstring result;

   switch( m_Type )
   {
      case CONSTANT_Integer:

         {
            TCHAR temp_string[ 30 ];

            _itot( m_Value.integerValue, temp_string, 10 );
            result = temp_string;
         }

         break;

      case CONSTANT_Long:

         {
            TCHAR temp_string[ 30 ];
            _ltot( m_Value.longValue, temp_string, 10 );
            result = temp_string;
         }

         break;

      case CONSTANT_Float:

         {
            TCHAR temp_string[ 30 ];
            _stprintf( temp_string, TEXT( "%f" ), m_Value.floatValue );
            result = temp_string;
         }

         break;

      case CONSTANT_Double:

         {
            TCHAR temp_string[ 30 ];
            _stprintf( temp_string, TEXT( "%e" ), m_Value.doubleValue );
            result = temp_string;
         }

         break;

      case CONSTANT_String:

         result = _T('"');
         result += m_Value.stringValue;
         result += _T('"');

         break;
   }

   return( result );
}
