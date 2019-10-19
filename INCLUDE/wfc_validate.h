/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2002-2014, Samuel R. Blackburn
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
** $Revision: 76 $
** $Modtime: 1/05/02 7:14a $
*/

#if ! defined( WFC_VALIDATE_HEADER_FILE_INCLUDED )

#define WFC_VALIDATE_HEADER_FILE_INCLUDED

// 2000-11-15
/*
** New for Release 51, pointer validation. The funny values in these
** macros are explained in WFC Tech Note 6. In short:
** 0xABABABAB - memory following a block allocated by LocalAlloc()
** 0xBAADF00D - memory allocated via LocalAlloc( LMEM_FIXED, ... ) but
**              not yet written to.
** 0xFEEEFEEE - Memory dedicated to a heap but not yet allocated by
**              HeapAlloc() or LocalAlloc()
** 0xCCCCCCCC - Microsoft Visual C++ compiled code with the /GZ option
**              is automatically initialized the uninitialized variable
**              with this value.
** 0xCDCDCDCD - Microsoft Visual C++ compiled code with memory leak detection
**              turned on. Usually, DEBUG_NEW was defined. Memory with this
**              tag signifies memory that has been allocated (by malloc() or
**              new) but never written to the application.
** 0xDDDDDDDD - Microsoft Visual C++ compiled code with memory leak detection
**              turned on. Usually, DEBUG_NEW was defined. Memory with this
**              tag signifies memory that has been freed (by free() or delete)
**              by the application. It is how you can detect writing to memory
**              that has already been freed. For example, if you look at an
**              allocated memory structure (or C++ class) and most of the
**              members contain this tag value, you are probably writing to
**              a structure that has been freed.
** 0xFDFDFDFD - Microsoft Visual C++ compiled code with memory leak detection
**              turned on. Usually, DEBUG_NEW was defined. Memory with this
**              tag signifies memory that is in "no-mans-land." These are
**              bytes just before and just after an allocated block. They are
**              used to detect array-out-of-bounds errors. This is great for
**              detecting off-by-one errors.
*/

#define MEMORY_FOLLOWING_A_BLOCK_ALLOCATED_BY_LocalAlloc    (0xABABABAB)
#define MEMORY_FOLLOWING_A_BLOCK_ALLOCATED_BY_LocalAlloc_64 (0xABABABABABABABAB)
#define MEMORY_ALLOCATED_BY_LocalAlloc_LMEM_FIXED_BUT_NOT_YET_WRITTEN_TO    (0xBAADF00D)
#define MEMORY_ALLOCATED_BY_LocalAlloc_LMEM_FIXED_BUT_NOT_YET_WRITTEN_TO_64 (0xBAADF00DBAADF00D)
#define MEMORY_DEDICATED_TO_A_HEAP_BUT_NOT_YET_ALLOCATED_BY_HeapAlloc_OR_LocalAlloc    (0xFEEEFEEE)
#define MEMORY_DEDICATED_TO_A_HEAP_BUT_NOT_YET_ALLOCATED_BY_HeapAlloc_OR_LocalAlloc_64 (0xFEEEFEEEFEEEFEEE)
#define VARIABLE_INITIALIZED_BUT_NOT_YET_WRITTEN_TO    (0xCCCCCCCC)
#define VARIABLE_INITIALIZED_BUT_NOT_YET_WRITTEN_TO_64 (0xCCCCCCCCCCCCCCCC)
#define MEMORY_ALLOCATED_BY_malloc_BUT_NOT_YET_WRITTEN_TO    (0xCDCDCDCD)
#define MEMORY_ALLOCATED_BY_malloc_BUT_NOT_YET_WRITTEN_TO_64 (0xCDCDCDCDCDCDCDCD)
#define FREED_MEMORY    (0xDDDDDDDD)
#define FREED_MEMORY_64 (0xDDDDDDDDDDDDDDDD)
#define NO_MANS_LAND    (0xFDFDFDFD)
#define NO_MANS_LAND_64 (0xFDFDFDFDFDFDFDFD)

#if ! defined( WFC_VALIDATE_POINTER )
#if defined( _W64 )
// 64-bit version
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_POINTER( _pointer ) _ASSERTE( (SIZE_T) _pointer != NULL ); \
_ASSERTE( (SIZE_T) _pointer != 1 ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_FOLLOWING_A_BLOCK_ALLOCATED_BY_LocalAlloc ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_ALLOCATED_BY_LocalAlloc_LMEM_FIXED_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_DEDICATED_TO_A_HEAP_BUT_NOT_YET_ALLOCATED_BY_HeapAlloc_OR_LocalAlloc ); \
_ASSERTE( (SIZE_T) _pointer != VARIABLE_INITIALIZED_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_ALLOCATED_BY_malloc_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (SIZE_T) _pointer != FREED_MEMORY ); \
_ASSERTE( (SIZE_T) _pointer != NO_MANS_LAND ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_FOLLOWING_A_BLOCK_ALLOCATED_BY_LocalAlloc_64 ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_ALLOCATED_BY_LocalAlloc_LMEM_FIXED_BUT_NOT_YET_WRITTEN_TO_64 ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_DEDICATED_TO_A_HEAP_BUT_NOT_YET_ALLOCATED_BY_HeapAlloc_OR_LocalAlloc_64 ); \
_ASSERTE( (SIZE_T) _pointer != VARIABLE_INITIALIZED_BUT_NOT_YET_WRITTEN_TO_64 ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_ALLOCATED_BY_malloc_BUT_NOT_YET_WRITTEN_TO_64 ); \
_ASSERTE( (SIZE_T) _pointer != FREED_MEMORY_64 ); \
_ASSERTE( (SIZE_T) _pointer != NO_MANS_LAND_64 )
#else // _DEBUG
#define WFC_VALIDATE_POINTER( _pointer )
#endif // _DEBUG
#else // _W64
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_POINTER( _pointer ) _ASSERTE( (SIZE_T) _pointer != nullptr ); \
_ASSERTE( (SIZE_T) _pointer != 1 ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_FOLLOWING_A_BLOCK_ALLOCATED_BY_LocalAlloc ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_ALLOCATED_BY_LocalAlloc_LMEM_FIXED_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_DEDICATED_TO_A_HEAP_BUT_NOT_YET_ALLOCATED_BY_HeapAlloc_OR_LocalAlloc ); \
_ASSERTE( (SIZE_T) _pointer != VARIABLE_INITIALIZED_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_ALLOCATED_BY_malloc_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (SIZE_T) _pointer != FREED_MEMORY ); \
_ASSERTE( (SIZE_T) _pointer != NO_MANS_LAND )
#else // _DEBUG
#define WFC_VALIDATE_POINTER( _pointer )
#endif // _DEBUG
#endif // _W64
#endif // WFC_VALIDATE_POINTER

#if ! defined( WFC_VALIDATE_POINTER_NULL_OK )
#if defined( _W64 )
// 64-bit version
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_POINTER_NULL_OK( _pointer ) \
_ASSERTE( (SIZE_T) _pointer != 1 ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_FOLLOWING_A_BLOCK_ALLOCATED_BY_LocalAlloc ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_ALLOCATED_BY_LocalAlloc_LMEM_FIXED_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_DEDICATED_TO_A_HEAP_BUT_NOT_YET_ALLOCATED_BY_HeapAlloc_OR_LocalAlloc ); \
_ASSERTE( (SIZE_T) _pointer != VARIABLE_INITIALIZED_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_ALLOCATED_BY_malloc_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (SIZE_T) _pointer != FREED_MEMORY ); \
_ASSERTE( (SIZE_T) _pointer != NO_MANS_LAND ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_FOLLOWING_A_BLOCK_ALLOCATED_BY_LocalAlloc_64 ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_ALLOCATED_BY_LocalAlloc_LMEM_FIXED_BUT_NOT_YET_WRITTEN_TO_64 ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_DEDICATED_TO_A_HEAP_BUT_NOT_YET_ALLOCATED_BY_HeapAlloc_OR_LocalAlloc_64 ); \
_ASSERTE( (SIZE_T) _pointer != VARIABLE_INITIALIZED_BUT_NOT_YET_WRITTEN_TO_64 ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_ALLOCATED_BY_malloc_BUT_NOT_YET_WRITTEN_TO_64 ); \
_ASSERTE( (SIZE_T) _pointer != FREED_MEMORY_64 ); \
_ASSERTE( (SIZE_T) _pointer != NO_MANS_LAND_64 )
#else // _DEBUG
#define WFC_VALIDATE_POINTER_NULL_OK( _pointer )
#endif // _DEBUG
#else // _W64
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_POINTER_NULL_OK( _pointer ) \
_ASSERTE( (SIZE_T) _pointer != 1 ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_FOLLOWING_A_BLOCK_ALLOCATED_BY_LocalAlloc ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_ALLOCATED_BY_LocalAlloc_LMEM_FIXED_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_DEDICATED_TO_A_HEAP_BUT_NOT_YET_ALLOCATED_BY_HeapAlloc_OR_LocalAlloc ); \
_ASSERTE( (SIZE_T) _pointer != VARIABLE_INITIALIZED_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (SIZE_T) _pointer != MEMORY_ALLOCATED_BY_malloc_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (SIZE_T) _pointer != FREED_MEMORY ); \
_ASSERTE( (SIZE_T) _pointer != NO_MANS_LAND )
#else // _DEBUG
#define WFC_VALIDATE_POINTER_NULL_OK( _pointer )
#endif // _DEBUG
#endif // _W64
#endif // WFC_VALIDATE_POINTER_NULL_OK

#if ! defined( WFC_VALIDATE_POINTER_NULL_OK )
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_POINTER_NULL_OK( _pointer ) \
_ASSERTE( (DWORD) _pointer != MEMORY_FOLLOWING_A_BLOCK_ALLOCATED_BY_LocalAlloc ); \
_ASSERTE( (DWORD) _pointer != MEMORY_ALLOCATED_BY_LocalAlloc_LMEM_FIXED_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (DWORD) _pointer != MEMORY_DEDICATED_TO_A_HEAP_BUT_NOT_YET_ALLOCATED_BY_HeapAlloc_OR_LocalAlloc ); \
_ASSERTE( (DWORD) _pointer != VARIABLE_INITIALIZED_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (DWORD) _pointer != MEMORY_ALLOCATED_BY_malloc_BUT_NOT_YET_WRITTEN_TO ); \
_ASSERTE( (DWORD) _pointer != FREED_MEMORY ); \
_ASSERTE( (DWORD) _pointer != NO_MANS_LAND )
#else // _DEBUG
#define WFC_VALIDATE_POINTER_NULL_OK( _pointer )
#endif // _DEBUG
#endif // WFC_VALIDATE_POINTER_NULL_OK

#if ! defined( WFC_VALIDATE_BOOL )
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_BOOL( _BOOLean_value ) \
_ASSERTE( _BOOLean_value == TRUE or _BOOLean_value == FALSE )
#else // _DEBUG
#define WFC_VALIDATE_BOOL( _BOOLean_value )
#endif // _DEBUG
#endif // WFC_VALIDATE_BOOL

#if ! defined( WFC_VALIDATE_bool )
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_bool( _BOOLean_value ) \
_ASSERTE( _BOOLean_value == true or _BOOLean_value == false )
#else // _DEBUG
#define WFC_VALIDATE_bool( _BOOLean_value )
#endif // _DEBUG
#endif // WFC_VALIDATE_bool

#if ! defined( WFC_VALIDATE_SHORT )
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_SHORT( _SHORT_value ) \
_ASSERTE( (short) _SHORT_value != 0xABAB ); \
_ASSERTE( (short) _SHORT_value != 0xBAAD ); \
_ASSERTE( (short) _SHORT_value != 0xF00D ); \
_ASSERTE( (short) _SHORT_value != 0xFEEE ); \
_ASSERTE( (short) _SHORT_value != 0xCCCC ); \
_ASSERTE( (short) _SHORT_value != 0xCDCD ); \
_ASSERTE( (short) _SHORT_value != 0xDDDD ); \
_ASSERTE( (short) _SHORT_value != 0xFDFD )
#else // _DEBUG
#define WFC_VALIDATE_SHORT( _SHORT_value )
#endif // _DEBUG
#endif // WFC_VALIDATE_SHORT

#if ! defined( WFC_VALIDATE_GUID )
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_GUID( g ) \
WFC_VALIDATE_POINTER( g.Data1 ); \
WFC_VALIDATE_SHORT( g.Data2 ); \
WFC_VALIDATE_SHORT( g.Data3 )
#else // _DEBUG
#define WFC_VALIDATE_GUID( g )
#endif // _DEBUG
#endif // WFC_VALIDATE_GUID

#endif // WFC_VALIDATE_HEADER_FILE_INCLUDED
