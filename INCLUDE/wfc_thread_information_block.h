/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2014, Samuel R. Blackburn
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
** $Workfile: wfc_thread_information_block.h $
** $Revision: 8 $
** $Modtime: 6/26/01 11:24a $
*/

#if ! defined( WFC_THREAD_INFORMATION_BLOCK_HEADER_FILE )

#define WFC_THREAD_INFORMATION_BLOCK_HEADER_FILE

/*
** This structure is derived from the TIB.H header
** file that Matt Peitrek published. I've renamed
** most things to avoid name collisions should
** Microsoft get around to documenting these things.
*/

#pragma pack( push, 1 )

typedef struct _WFC_EXCEPTION_RECORD
{
   struct _WFC_EXCEPTION_RECORD * Next;
   FARPROC                        Handler;
}
WFC_EXCEPTION_RECORD;

typedef struct _WFC_PROCESS_DATABASE
{
   DWORD Unknown_0x00;
   DWORD Unknown_0x04;
   PVOID ExecutableBaseAddress; // Offset 0x08
   DWORD Unknown_0x0C;
   DWORD Unknown_0x10;
   DWORD Unknown_0x14;
   DWORD StackUserTop;
   DWORD Unknown_0x1C;
   PVOID NTDLL_RtlEnterCriticalSection;
   PVOID NTDLL_RtlLeaveCriticalSection;
}
WFC_PROCESS_DATABASE;

/*
** 2001-06-26
** The website http://www.multimania.com/christalpage/DAdossier9/debugApis.htm
** describes the TIB as:
**
** Les champs du TIB sont les suivants : 
**
** 00h   DWORD   pvExcept
** 04h   DWORD   TopOfStack
** 08h   DWORD   StackLow
** 0Ch    WORD   W16TDB
** 0Eh    WORD   StackSelector16
** 10h   DWORD   Selmanlist
** 14h   DWORD   UserPointer
** 18h    PTIB   pTIB
** 1Ch    WORD   TIBFlag
** 1Eh    WORD   Win16MutexCount
** 20h   DWORD   CebugContext
** 24h  PDWORD   pCurrentProcess
** 28h   DWORD   MessageQueue
** 2Ch  PDWORD   PTLSArray
** 30h   DWORD   pProcess (Process DataBase Pointer)  
**
*/

typedef struct _WFC_THREAD_INFORMATION_BLOCK
{
   WFC_EXCEPTION_RECORD * ExceptionRecordList; // Offset 0x00, aka NT_TIB.ExceptionList
   VOID * StackUserTop; // Offset 0x04, aka NT_TIB.StackBase
   VOID * StackUserBase; // Offset 0x08, aka NT_TIB.StackLimit

   union
   {
      struct
      {
         WORD  TDB; // Offset  0x0C
         WORD  SSSelectorForThunkingTo16Bits; // Offset 0x0E
         DWORD Unknown1; // Offset 0x10
      }
      WFC_WIN95;

      struct
      {
         VOID * SubSystemThreadInformationBlock; // Offset 0x0C, aka NT_TIB.SubSystemTib
         DWORD  FiberData; // Offset 0x10, aka NT_TIB.FiberData or NT_TIB.Version (its a union)
      }
      WFC_WINNT;
   }
   FIRST_UNION;

   VOID * Arbitrary; // Offset 0x14, aka NT_TIB.ArbitraryUserPointer
   struct _WFC_THREAD_INFORMATION_BLOCK * Self; // Offset 0x18

   union
   {
      struct
      {
         WORD  ThreadInformationBlockFlags; // Offset 0x1C
         WORD  Win16MutexCount; // Offset 0x1E
         DWORD DebugContext; // Offset 0x20
         DWORD CurrentPriority; // Offset 0x24
         DWORD MessageQueueSelector; // Offset 0x28
      }
      WFC_WIN95;

      struct
      {
         VOID * EnvironmentPtr;  // Offset 0x1C, according to Inside Windows 2000, page 328
         DWORD  ProcessID; // Offset 0x20
         DWORD  ThreadID;  // Offset 0x24
         HANDLE RpcHandle;  // Offset 0x28, , according to Inside Windows 2000, page 328
      }
      WFC_WINNT;
   }
   SECOND_UNION;

   VOID * ThreadLocalStorageArray; // Offset 0x2C

   union
   {
      WFC_PROCESS_DATABASE * OwningProcessDatabase; // Offset 0x30
   }
   THIRD_UNION;

   DWORD LastError; // Offset 0x34
   DWORD LastStatus; // Offset 0x38, according to Inside Windows 2000, page 329
}
WFC_THREAD_INFORMATION_BLOCK;

#pragma pack( pop )

#endif // WFC_THREAD_INFORMATION_BLOCK_HEADER_FILE
