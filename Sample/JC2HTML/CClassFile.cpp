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

#include <fstream>
//#include <math.h>
//#include <float.h>

union DOUBLE_CONVERT
{
   double dValue;
   struct
   {
      long lowBytes;
      long highBytes;
   } lValue;
};

union FLOAT_CONVERT
{
   float fValue;
   long lValue;
};

struct ConstantString
{
   short index;
   short stringIndex;
};

typedef std::vector<ConstantString *> ConstantStringVector;

CClassFile::CClassFile()
{
   m_Initialize();
   ctor();
}

void CClassFile::m_Initialize( void )
{
   m_JavaClass    = false;
   m_MinorVersion = 0;
   m_MajorVersion = 0;
   m_AccessFlag   = 0;
}

CClassFile::CClassFile(std::wstring &aFileName)
{
   setFileName(aFileName);
   ctor();
}

CClassFile::~CClassFile()
{
   for( String2FieldMap::iterator it = std::begin(m_Fields); it != std::end(m_Fields); it++)
   {
      delete it->second;
   }

   m_Fields.clear();

   for(Int2ConstantValueMap::iterator it2 = std::begin(m_ConstantValues); it2 != std::end(m_ConstantValues); it2++)
   {
      delete it2->second;
   }

   m_ConstantValues.clear();

   m_Interfaces.clear();
}

void CClassFile::ctor(void)
{
   m_JavaClass = false;
   m_AccessFlag = 0;
}

void CClassFile::read(void)
{
   typedef std::map<int, int> Int2IntMap;

   Int2IntMap classes;

   std::ifstream javaClassFile;
   ConstantStringVector constantStrings;

   javaClassFile.open(m_FileName.c_str(), std::ios_base::in | std::ios_base::binary);

   if ( javaClassFile.is_open() )
   {
      long magic = 0;

      readByte(javaClassFile, (char *) &magic, sizeof(magic));

      if ( magic == CLASSFILE_MAGIC )
      {
         m_JavaClass = true;

         readByte(javaClassFile, (char *) &m_MinorVersion, sizeof(m_MinorVersion));
         readByte(javaClassFile, (char *) &m_MajorVersion, sizeof(m_MajorVersion));

         short constantPoolSize = 0;
         readByte(javaClassFile, (char *) &constantPoolSize, sizeof(constantPoolSize));

         // Read the constant pool
         char constantPoolTag = 0;

         for(short i = 1; i < constantPoolSize; i++)
         {
            short constantValueIndex = i; // Store the index, because i could change for long, double values

            javaClassFile.read(&constantPoolTag, 1);

            CConstantValue *pconstantValue = NULL;

            switch( constantPoolTag )
            {
               case CONSTANT_Class:

                  {
                     short classNameIndex = 0;
                     readByte(javaClassFile, (char *) &classNameIndex, sizeof(classNameIndex));
                     classes.insert(Int2IntMap::value_type(i, classNameIndex));
                  }

                  break;

               case CONSTANT_Fieldref:
               case CONSTANT_Methodref:
               case CONSTANT_InterfaceMethodref:

                  {
                     short classIndex = 0;
                     short nameTypeIndex = 0;
                     readByte(javaClassFile, (char *) &classIndex, sizeof(classIndex));
                     readByte(javaClassFile, (char *) &nameTypeIndex, sizeof(nameTypeIndex));
                  }

                  break;

               case CONSTANT_String:

                  {
                     ConstantString *pconstantString = new ConstantString;
                     pconstantString->index = constantValueIndex;
                     readByte(javaClassFile, (char *) &(pconstantString->stringIndex), sizeof(pconstantString->stringIndex));
                     constantStrings.push_back(pconstantString);
                  }

                  break;

               case CONSTANT_Integer:
               case CONSTANT_Float:

                  {
                     FLOAT_CONVERT integerOrFloat;
                     readByte(javaClassFile, (char *) &integerOrFloat.lValue, sizeof(integerOrFloat.lValue));

                     pconstantValue = ( constantPoolTag == CONSTANT_Integer ) ? new CConstantValue((int) integerOrFloat.lValue)
                        : new CConstantValue(integerOrFloat.fValue);
                  }

                  break;

               case CONSTANT_Long:
               case CONSTANT_Double:

                  {
                     long highBytes = 0;
                     long lowBytes  = 0;

                     readByte(javaClassFile, (char *) &highBytes, sizeof(highBytes));
                     readByte(javaClassFile, (char *) &lowBytes, sizeof(lowBytes));

                     if ( constantPoolTag == CONSTANT_Long )
                     {
                        long constant = (highBytes << 32) + lowBytes;
                        pconstantValue = new CConstantValue(constant);
                     }
                     else
                     {
                        DOUBLE_CONVERT r;
                        r.lValue.highBytes = highBytes;
                        r.lValue.lowBytes = lowBytes;
                        pconstantValue = new CConstantValue(r.dValue);
                     }

                     // These take two spots in the connection pool;
                     i++;
                  }

                  break;

               case CONSTANT_NameAndType:

                  {
                     short nameIndex        = 0;
                     short descriptorIndex = 0;
                     readByte(javaClassFile, (char *) &nameIndex, sizeof(nameIndex));
                     readByte(javaClassFile, (char *) &descriptorIndex, sizeof(descriptorIndex));
                  }

                  break;

               case CONSTANT_Utf8:

                  {
                     short length = 0;

                     readByte(javaClassFile, (char *) &length, sizeof(length));
                     char *str = new char[length + 1];

                     javaClassFile.read(str, length);
                     str[length] = '\0';
                     m_Names.insert(Int2StringMap::value_type(i, std::wstring(str)));
                     delete str;
                  }

                  break;
            }

            // When we've read a constant value, add it to the map
            if ( pconstantValue != NULL )
            {
               m_ConstantValues.insert( Int2ConstantValueMap::value_type( constantValueIndex, pconstantValue));
            }
         }

         // Resolve string constants
         for( ConstantStringVector::iterator itIndex = std::begin(constantStrings); itIndex != std::end(constantStrings); itIndex++ )
         {
            std::wstring constantString = GetName( (*itIndex)->stringIndex );
            m_ConstantValues.insert( Int2ConstantValueMap::value_type( (*itIndex)->index, new CConstantValue( constantString.c_str() ) ) );
            delete *itIndex;
         }

         constantStrings.clear();

         // AccessFlag
         readByte( javaClassFile, (char *) &m_AccessFlag, sizeof( m_AccessFlag ) );

         // ThisClass
         short thisClass = 0;

         readByte(javaClassFile, (char *)&thisClass, sizeof(thisClass));

         Int2IntMap::iterator itClass = classes.find(thisClass);

         if ( itClass != std::end(classes) )
         {
            m_Name = GetName(itClass->second);
            replaceChar(m_Name, _T('/'), _T('.'));
         }

         // SuperClass
         short superClass = 0;

         readByte(javaClassFile, (char *)&superClass, sizeof(superClass));

         itClass = classes.find( superClass );

         if ( itClass != std::end(classes) )
         {
            m_SuperClassName = GetName(itClass->second);
            replaceChar(m_SuperClassName, _T('/'), _T('.'));
         }

         // Interfaces
         short interfaceCount = 0;

         readByte(javaClassFile, (char *)&interfaceCount, sizeof(interfaceCount));
         for(i = 0; i < interfaceCount; i++)
         {
            short interfaceIndex = 0;

            readByte(javaClassFile, (char *)&interfaceIndex, sizeof(interfaceIndex));
            std::wstring interfaceName;
            itClass = classes.find(interfaceIndex);

            if ( itClass != std::end(classes) )
            {
               std::wstring interfaceName = GetName(itClass->second);
               replaceChar(interfaceName, _T('/'), _T('.'));
               m_Interfaces.push_back( interfaceName );
            }
         }

         // Fields
         short fieldsCount = 0;

         readByte( javaClassFile, (char *) &fieldsCount, sizeof( fieldsCount ) );

         for(i = 0; i < fieldsCount; i++)
         {
            CField *field = new CField( this );

            readByte( javaClassFile, (char *) &field->AccessFlag,      sizeof( field->AccessFlag      ) );
            readByte( javaClassFile, (char *) &field->NameIndex,       sizeof( field->NameIndex       ) );
            readByte( javaClassFile, (char *) &field->DescriptorIndex, sizeof( field->DescriptorIndex ) );

            short attributeCount = 0;

            readByte( javaClassFile, (char *) &attributeCount, sizeof( attributeCount ) );

            for( int j = 0; j < attributeCount; j++ )
            {
               short attributeNameIndex = 0;
               long attributeLength     = 0;

               readByte( javaClassFile, (char *) &attributeNameIndex, sizeof( attributeNameIndex ) );
               readByte( javaClassFile, (char *) &attributeLength,    sizeof( attributeLength    ) );

               std::wstring attributeName = GetName( attributeNameIndex );

               if ( attributeName == "ConstantValue" )
               {
                  short valueIndex = 0;

                  readByte( javaClassFile, (char *) &valueIndex, sizeof( valueIndex ) );
                  Int2ConstantValueMap::iterator itConstant = m_ConstantValues.find( valueIndex );

                  if ( itConstant != std::end(m_ConstantValues) )
                  {
                     field->ConstantValue = itConstant->second;
                  }
               }
               else if ( attributeName == L"Synthetic" )
               {
                  field->Synthetic = true;
               }
               else if ( attributeName == L"Deprecated" )
               {
                  field->Deprecated = true;
               }
            }

            m_Fields.insert( String2FieldMap::value_type( field->getName(), field ) );
         }

         // Methods
         short methodCount = 0;

         readByte( javaClassFile, (char *) &methodCount, sizeof( methodCount ) );

         for( i = 0; i < methodCount; i++ )
         {
            CMethod * method = new CMethod( this );

            readByte( javaClassFile, (char *) &method->AccessFlag,      sizeof( method->AccessFlag      ) );
            readByte( javaClassFile, (char *) &method->NameIndex,       sizeof( method->NameIndex       ) );
            readByte( javaClassFile, (char *) &method->DescriptorIndex, sizeof( method->DescriptorIndex ) );

            short attributeCount = 0;

            readByte( javaClassFile, (char *) &attributeCount, sizeof( attributeCount ) );

            for( int j = 0; j < attributeCount; j++ )
            {
               short attributeNameIndex = 0;
               long attributeLength     = 0;

               readByte(javaClassFile, (char *)&attributeNameIndex, sizeof(attributeNameIndex));
               readByte(javaClassFile, (char *)&attributeLength, sizeof(attributeLength));
               std::wstring attributeName = GetName( attributeNameIndex );

               if ( attributeName == L"Synthetic" )
               {
                  method->Synthetic = true;
               }
               else if ( attributeName == L"Deprecated" )
               {
                  method->Deprecated = true;
               }
               else if ( attributeName == L"Code" )
               {
                  // Skip code
                  javaClassFile.seekg(attributeLength, std::ios::cur);
               }
               else if ( attributeName == "Exceptions" )
               {
                  short exceptionCount = 0;

                  readByte( javaClassFile, (char *) &exceptionCount, sizeof( exceptionCount ) );

                  for( short k = 0; k < exceptionCount; k++ )
                  {
                     short exceptionIndex = 0;

                     readByte( javaClassFile, (char *) &exceptionIndex, sizeof( exceptionIndex ) );

                     Int2IntMap::iterator itClasses = classes.find(exceptionIndex);

                     if ( itClasses != std::end(classes) )
                     {
                        std::wstring exception = GetName( itClasses->second );

                        for( int m = exception.find( _T('/') ); m != std::wstring::npos; m = exception.find( _T('/'), ++m ) )
                        {
                           exception[ m ] = _T('.');
                        }

                        method->Exceptions.push_back( exception );
                     }
                  }
               }
            }

            m_Methods.insert( String2MethodMap::value_type( method->getName(), method ) );
         }

         short attributeCount = 0;

         readByte( javaClassFile, (char *) &attributeCount, sizeof( attributeCount ) );

         for( int j = 0; j < attributeCount; j++ )
         {
            short attributeNameIndex = 0;
            long attributeLength     = 0;

            readByte( javaClassFile, (char *)&attributeNameIndex, sizeof(attributeNameIndex));
            readByte( javaClassFile, (char *)&attributeLength, sizeof(attributeLength));

            std::wstring attributeName = GetName( attributeNameIndex );

            if ( attributeName == L"SourceFile" )
            {
               short sourceFileIndex = 0;

               readByte(javaClassFile, (char *)&sourceFileIndex, sizeof(sourceFileIndex));
               std::wstring sourceFileName = GetName(sourceFileIndex);
            }
            else if ( attributeName == L"Deprecated" )
            {
            }
            else if ( attributeName == L"Synthetic" )
            {
            }
            else if ( attributeName == L"InnerClasses" )
            {
               // Innerclasses
               short innerClassCount = 0;
               int unnamedCount = 0;

               readByte(javaClassFile, (char *)&innerClassCount, sizeof(innerClassCount));

               for(int k = 0; k < innerClassCount; k++)
               {
                  short innerClassInfoIndex   = 0;
                  short outerClassInfoIndex   = 0;
                  short innerNameIndex        = 0;
                  short innerClassAccessFlags = 0;

                  readByte(javaClassFile, (char *)&innerClassInfoIndex, sizeof(innerClassInfoIndex));
                  readByte(javaClassFile, (char *)&outerClassInfoIndex, sizeof(outerClassInfoIndex));
                  readByte(javaClassFile, (char *)&innerNameIndex, sizeof(innerNameIndex));
                  readByte(javaClassFile, (char *)&innerClassAccessFlags, sizeof(innerClassAccessFlags));

                  if ( innerNameIndex > 0 )
                  {
                     m_InnerClasses.push_back( GetName( innerNameIndex ) );
                  }
               }
            }
         }
      }

      classes.clear();
      javaClassFile.close();
   }
}

std::wstring CClassFile::GetName( short aIndex )
{
   Int2StringMap::iterator it = m_Names.find( aIndex );

   if ( it != std::end(m_Names) )
   {
      return( it->second );
   }
   else
   {
      return L"";
   }
}

CConstantValue *CClassFile::getConstantValue(short aIndex)
{
   Int2ConstantValueMap::iterator it = m_ConstantValues.find(aIndex);

   if ( it != std::end(m_ConstantValues) )
   {
      return( it->second );
   }
   else
   {
      return( NULL );
   }
}

void CClassFile::readByte( std::ifstream &aInputStream, char *aByte, int size )
{
   aInputStream.read( aByte, size );
   reverseByte( aByte, size );
}


