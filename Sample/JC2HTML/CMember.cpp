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

CMember::CMember(CClassFile *paClassFile)
{
   m_ClassFile_p     = paClassFile;
   AccessFlag      = 0;
   DescriptorIndex = 0;
   NameIndex       = 0;
   Deprecated      = false;
   Synthetic       = false;
}

CMember::~CMember()
{
}

std::wstring CMember::getName(void)
{
   return( m_ClassFile_p->GetName( NameIndex ) );
}

ACCESSFLAG CMember::GetAccessFlag( void ) const
{
   if ( (AccessFlag & ACC_PUBLIC) == ACC_PUBLIC )
   {
      return( PUBLIC );
   }
   else if ( (AccessFlag & ACC_PROTECTED) == ACC_PROTECTED )
   {
      return( PROTECTED );
   }
   else
   {
      return( PRIVATE );
   }
}

std::wstring CMember::m_staticResolveType( std::wstring aType )
{
   std::wstring result;

   int dimension = 0;
   int i         = 0;

   std::wstring::iterator it = std::begin(aType);

   for(; it != std::end(aType) && *it == _T('['); it++)
   {
      dimension++;
   }

   switch( *it )
   {
      case _T('B'):

         result = _T("byte");
         break;

      case _T('C'):

         result = _T("char");
         break;

      case _T('D'):

         result = _T("double");
         break;

      case _T('F'):

         result = _T("float");
         break;

      case _T('I'):

         result = _T("int");
         break;

      case _T('J'):

         result = _T("long");
         break;

      case _T('S'):

         result = _T("short");
         break;

      case _T('Z'):

         result = _T("boolean");
         break;

      case _T('L'):

         {
            int pos = aType.find( _T(';') );
            result = aType.substr( dimension + 1, pos - 1 - dimension );
            replaceChar( result, _T('/'), _T('.') );
         }

         break;

      case _T('V'):

         result = _T("void");
         break;
   }

   for( i = 0; i < dimension; i++ )
   {
      result += _T("[]");
   }

   return( result );
}

std::wstring CMember::m_GetDescriptor( void )
{
   return( m_ClassFile_p->GetName( DescriptorIndex ) );
}
