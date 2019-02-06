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
*/

// ClassFile.h: interface for the CClassFile class.
//
// Author : Braem Franky
// Date   : 08-2000
// (c) 2000 You're free to use this code in non-commercial programs,
//          Commercial programs should add a remark in there documentation
//          that they use a program of me.
// e-mail : Franky.Braem17@yucom.be
// web    : www.braem17.yucom.be
//////////////////////////////////////////////////////////////////////

#if ! defined( CLASS_FILE_HEADER_FILE_INCLUDED )

#define CLASS_FILE_HEADER_FILE_INCLUDED

//#include "Field.h"
//#include "ConstantValue.h"
//#include "Method.h"

class CClassFile
{
   private:

      typedef std::map<short, CConstantValue *> Int2ConstantValueMap;

      static void readByte(std::ifstream &aInputStream, char *aByte, int size);

      CConstantValue *getConstantValue(short aIndex);

      void ctor(void);

      std::wstring m_FileName;
      std::wstring m_Name;
      std::wstring m_SuperClassName;
      bool m_JavaClass;

      Int2ConstantValueMap m_ConstantValues;
      Int2StringMap m_Names;

      short m_MinorVersion;
      short m_MajorVersion;
      short m_AccessFlag;

      StringVector     m_Interfaces;
      String2FieldMap  m_Fields;
      String2MethodMap m_Methods;
      StringVector     m_InnerClasses;

      void m_Initialize( void );

   public:

      CClassFile();
      CClassFile(std::wstring &aFileName);
      virtual ~CClassFile();

      typedef std::map<std::wstring, CClassFile *> String2ClassFileMap;
      //friend class CMember;

      std::wstring GetName(short index);

      inline void setFileName(std::wstring &aFileName) { m_FileName = aFileName; }
      inline std::wstring getFileName(void) { return m_FileName; }
      inline int getMinorVersion(void) const { return m_MinorVersion; }
      inline int getMajorVersion(void) const { return m_MajorVersion; }
      inline int isPublic(void) const {return (m_AccessFlag & ACC_PUBLIC) == ACC_PUBLIC; }
      inline int isFinal(void) const { return (m_AccessFlag & ACC_FINAL) == ACC_FINAL; }
      inline int isSuper(void) const {return (m_AccessFlag & ACC_SUPER) == ACC_SUPER; }
      inline int isInterface(void) { return (m_AccessFlag & ACC_INTERFACE) == ACC_INTERFACE; }
      inline int isAbstract(void) { return (m_AccessFlag & ACC_ABSTRACT) == ACC_ABSTRACT; }
      inline std::wstring getName(void) const { return m_Name; }
      inline std::wstring getSuper(void) const { return m_SuperClassName; }
      inline StringVector &getInterfaces(void) { return m_Interfaces; }
      inline StringVector &getInnerClasses(void) { return m_InnerClasses; }
      void read(void);
      inline isJavaClassFile(void) { return m_JavaClass; }
      inline String2FieldMap& getFields(void) { return m_Fields; }
      inline String2MethodMap& getMethods(void) { return m_Methods; }

};

#endif // CLASS_FILE_HEADER_FILE_INCLUDED