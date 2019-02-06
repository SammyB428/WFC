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

#if ! defined( MEMBER_CLASS_HEADER_FILE_INCLUDED )

#define MEMBER_CLASS_HEADER_FILE_INCLUDED

class CClassFile;

class CMember
{
   private:

      CMember( const CMember& ) {};
      CMember& operator=( const CMember& ) { return( *this ); }

   protected:

      CClassFile * m_ClassFile_p; // pClassFile;

      static std::wstring m_staticResolveType( std::wstring a_type ); // resolveType(std::wstring aType);
      std::wstring m_GetDescriptor( void ); // getDescriptor(void);

   public:

      CMember( CClassFile *paClassFile = NULL );
      virtual ~CMember();

      short AccessFlag; // accessFlag;
      short NameIndex; // nameIndex;
      short DescriptorIndex; // descriptorIndex;
      bool  Deprecated; // deprecated;
      bool  Synthetic; // synthetic;

      ACCESSFLAG GetAccessFlag( void ) const;
      std::wstring getName( void );
      virtual std::wstring getType( void ) = 0;

      inline bool isFinal(     void ) const { return ( (AccessFlag & ACC_FINAL)     == ACC_FINAL     ) ? true : false; }
      inline bool isStatic(    void ) const { return ( (AccessFlag & ACC_STATIC)    == ACC_STATIC    ) ? true : false; }
      inline bool isPublic(    void ) const { return ( (AccessFlag & ACC_PUBLIC)    == ACC_PUBLIC    ) ? true : false; }
      inline bool isPrivate(   void ) const { return ( (AccessFlag & ACC_PRIVATE)   == ACC_PRIVATE   ) ? true : false; }
      inline bool isProtected( void ) const { return ( (AccessFlag & ACC_PROTECTED) == ACC_PROTECTED ) ? true : false; }
};

#endif // MEMBER_CLASS_HEADER_FILE_INCLUDED
