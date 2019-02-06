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

#if ! defined( METHOD_CLASS_HEADER_FILE_INCLUDED )

#define METHOD_CLASS_HEADER_FILE_INCLUDED

class CClassFile;

//typedef std::vector<std::wstring> StringVector;

class CMethod : public CMember
{
   private:

      CMethod( const CMethod& ) {};
      CMethod& operator=( const CMethod& ) { return( *this ); }

   public:

      CMethod( CClassFile *paClassFile = NULL );

      virtual ~CMethod();

      StringVector Exceptions;
      StringVector Parameters;

      bool ParametersResolved;

      bool isSynchronized( void ) const { return ( (AccessFlag & ACC_SYNCHRONIZED) == ACC_SYNCHRONIZED ) ? true : false; }
      bool isNative(       void ) const { return ( (AccessFlag & ACC_NATIVE)       == ACC_NATIVE       ) ? true : false; }
      bool isAbstract(     void ) const { return ( (AccessFlag & ACC_ABSTRACT)     == ACC_ABSTRACT     ) ? true : false; }
      bool isStrict(       void ) const { return ( (AccessFlag & ACC_STRICT)       == ACC_STRICT       ) ? true : false; }
      std::wstring getType(void);
      StringVector &getParameters(void);
      StringVector &getExceptions(void) { return Exceptions; }
};

#endif // METHOD_CLASS_HEADER_FILE_INCLUDED
