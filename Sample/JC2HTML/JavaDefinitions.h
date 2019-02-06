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

#if ! defined( JAVA_DEFINITIONS_HEADER_FILE_INCLUDED )

#define JAVA_DEFINITIONS_HEADER_FILE_INCLUDED

#define CLASSFILE_MAGIC (0xCAFEBABE)

#define ACC_PUBLIC        (0x0001)
#define ACC_PRIVATE       (0x0002)
#define ACC_PROTECTED     (0x0004)
#define ACC_STATIC        (0x0008)
#define ACC_FINAL         (0x0010)
#define ACC_SUPER         (0x0020)
#define ACC_SYNCHRONIZED  (0x0020)
#define ACC_VOLATILE      (0x0040)
#define ACC_TRANSIENT     (0x0080)
#define ACC_NATIVE        (0x0100)
#define ACC_INTERFACE     (0x0200)
#define ACC_ABSTRACT      (0x0400)
#define ACC_STRICT        (0x0800)

#define CONSTANT_Class               (7)
#define CONSTANT_Fieldref            (9)
#define CONSTANT_Methodref          (10)
#define CONSTANT_InterfaceMethodref (11)
#define CONSTANT_String              (8)
#define CONSTANT_Integer             (3)
#define CONSTANT_Float               (4)
#define CONSTANT_Long                (5)
#define CONSTANT_Double              (6)
#define CONSTANT_NameAndType        (12)
#define CONSTANT_Utf8                (1)

enum ACCESSFLAG { PUBLIC, PROTECTED, PRIVATE };

#endif // JAVA_DEFINITIONS_HEADER_FILE_INCLUDED

