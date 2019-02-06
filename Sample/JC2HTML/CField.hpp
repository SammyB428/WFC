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

#if ! defined( FIELD_CLASS_HEADER_FILE_INCLUDED )

#define FIELD_CLASS_HEADER_FILE_INCLUDED

class CField : public CMember
{
   private:

      CField( const CField& ) {};
      CField& operator=( const CField& ) { return( *this ); }

   public:

      CField( CClassFile *class_file_p );
      virtual ~CField();

      CConstantValue * ConstantValue; // *constantValue; // Pointer to a constantValue, kept by CClassFile

      bool isVolatile(  void ) { return ( (AccessFlag & ACC_VOLATILE)  == ACC_VOLATILE  ) ? true : false; }
      bool isTransient( void ) { return ( (AccessFlag & ACC_TRANSIENT) == ACC_TRANSIENT ) ? true : false; }

      inline CConstantValue * GetConstantValue( void ) const { return ( ConstantValue ); }
      std::wstring getType( void );
};

#endif // FIELD_CLASS_HEADER_FILE_INCLUDED
