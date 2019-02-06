/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 2000-2003, Samuel R. Blackburn
** All rights reserved.
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
** $Workfile: print_process_list.cpp $
** $Revision: 1 $
** $Modtime: 1/15/00 10:23a $
*/

#include "ListProcesses.h"
#pragma hdrstop

LPCTSTR wait_reason( WFC_KWAIT_REASON reason )
{
   switch( reason )
   {
      case Executive:

         return( TEXT( "Executive" ) );

      case FreePage:

         return( TEXT( "FreePage" ) );

      case PageIn:

         return( TEXT( "PageIn" ) );

      case PoolAllocation:

         return( TEXT( "PoolAllocation" ) );

      case DelayExecution:

         return( TEXT( "DelayExecution" ) );

      case Suspended:

         return( TEXT( "Suspended" ) );

      case UserRequest:

         return( TEXT( "UserRequest" ) );

      case WrExecutive:

         return( TEXT( "WrExecutive" ) );

      case WrFreePage:

         return( TEXT( "WrFreePage" ) );

      case WrPageIn:

         return( TEXT( "WrPageIn" ) );

      case WrPoolAllocation:

         return( TEXT( "WrPoolAllocation" ) );

      case WrDelayExecution:

         return( TEXT( "WrDelayExecution" ) );

      case WrSuspended:

         return( TEXT( "WrSuspended" ) );

      case WrUserRequest:

         return( TEXT( "WrUserRequest" ) );

      case WrEventPair:

         return( TEXT( "WrEventPair" ) );

      case WrQueue:

         return( TEXT( "WrQueue" ) );

      case WrLpcReceive:

         return( TEXT( "WrLpcReceive" ) );

      case WrLpcReply:

         return( TEXT( "WrLpcReply" ) );

      case WrVirtualMemory:

         return( TEXT( "WrVirtualMemory" ) );

      case WrPageOut:

         return( TEXT( "WrPageOut" ) );

      case WrRendezvous:

         return( TEXT( "WrRendezvous" ) );

      case Spare2:

         return( TEXT( "Spare2" ) );

      case Spare3:

         return( TEXT( "Spare3" ) );

      case Spare4:

         return( TEXT( "Spare4" ) );

      case Spare5:

         return( TEXT( "Spare5" ) );

      case Spare6:

         return( TEXT( "Spare6" ) );

      case WrKernel:

         return( TEXT( "WrKernel" ) );

      case MaximumWaitReason:

         return( TEXT( "MaximumWaitReason" ) );

      default:

         return( TEXT( "Unknown" ) );
   }
}

void print_thread( WFC_SYSTEM_THREAD * thread_p )
{
   _tprintf( TEXT( "   KernelTime             = %I64u\n" ), thread_p->KernelTime );
   _tprintf( TEXT( "   UserTime               = %I64u\n" ), thread_p->UserTime );
   _tprintf( TEXT( "   CreateTime             = %I64u\n" ), thread_p->CreateTime );
   _tprintf( TEXT( "   UnknownValueAtOffset18 = %lu\n" ), thread_p->UnknownValueAtOffset18 );
   _tprintf( TEXT( "   StartAddress           = %lu\n" ), thread_p->StartAddress );
   _tprintf( TEXT( "   ClientID\n   {\n" ) );
   _tprintf( TEXT( "      ProcessHandle = %lu\n" ), thread_p->ClientID.ProcessHandle );
   _tprintf( TEXT( "      ThreadHandle  = %lu\n" ), thread_p->ClientID.ThreadHandle );
   _tprintf( TEXT( "   }\n" ) );
   _tprintf( TEXT( "   Priority              = %lu\n" ), thread_p->Priority );
   _tprintf( TEXT( "   BasePriority          = %lu\n" ), thread_p->BasePriority );
   _tprintf( TEXT( "   ContextSwitches       = %lu\n" ), thread_p->ContextSwitches );
   _tprintf( TEXT( "   ThreadState           = %lu\n" ), thread_p->ThreadState );
   _tprintf( TEXT( "   WaitReason            = %s\n" ),  wait_reason( thread_p->WaitReason ) );
   _tprintf( TEXT( "   Reserved              = %lu\n" ), thread_p->Reserved );
}

void print_vm_counters( WFC_VM_COUNTERS * counters_p )
{
   _tprintf( TEXT( "   PeakVirtualSize            = %lu\n" ), counters_p->PeakVirtualSize );
   _tprintf( TEXT( "   VirtualSize                = %lu\n" ), counters_p->VirtualSize );
   _tprintf( TEXT( "   PageFaultCount             = %lu\n" ), counters_p->PageFaultCount );
   _tprintf( TEXT( "   PeakWorkingSetSize         = %lu\n" ), counters_p->PeakWorkingSetSize );
   _tprintf( TEXT( "   WorkingSetSize             = %lu\n" ), counters_p->WorkingSetSize );
   _tprintf( TEXT( "   QuotaPeakPagedPoolUsage    = %lu\n" ), counters_p->QuotaPeakPagedPoolUsage );
   _tprintf( TEXT( "   QuotaPagedPoolUsage        = %lu\n" ), counters_p->QuotaPagedPoolUsage );
   _tprintf( TEXT( "   QuotaPeakNonPagedPoolUsage = %lu\n" ), counters_p->QuotaPeakNonPagedPoolUsage );
   _tprintf( TEXT( "   QuotaNonPagedPoolUsage     = %lu\n" ), counters_p->QuotaNonPagedPoolUsage );
   _tprintf( TEXT( "   PagefileUsage              = %lu\n" ), counters_p->PagefileUsage );
   _tprintf( TEXT( "   PeakPagefileUsage          = %lu\n" ), counters_p->PeakPagefileUsage );
}

void print_system_information( WFC_SYSTEM_PROCESS_INFORMATION * information_p )
{
   WFCTRACEINIT( TEXT( "print_system_information()" ) );

   _tprintf( TEXT( "ThreadCount     = %lu\n" ), information_p->ThreadCount );
   _tprintf( TEXT( "Reserved1       = %lu\n" ), information_p->Reserved1  );
   _tprintf( TEXT( "Reserved2       = %lu\n" ), information_p->Reserved2  );
   _tprintf( TEXT( "Reserved3       = %lu\n" ), information_p->Reserved3  );
   _tprintf( TEXT( "Reserved4       = %lu\n" ), information_p->Reserved4  );
   _tprintf( TEXT( "Reserved5       = %lu\n" ), information_p->Reserved5  );
   _tprintf( TEXT( "Reserved6       = %lu\n" ), information_p->Reserved6  );
   _tprintf( TEXT( "CreateTime      = %I64u\n" ), information_p->CreateTime );
   _tprintf( TEXT( "UserTime        = %I64u\n" ), information_p->UserTime );
   _tprintf( TEXT( "KernelTime      = %I64u\n" ), information_p->KernelTime );
   _tprintf( TEXT( "Name            = %S\n" ), information_p->Name.Buffer );
   _tprintf( TEXT( "BasePriority    = %lu\n" ), information_p->BasePriority );
   _tprintf( TEXT( "UniqueProcessID = %lu\n" ), information_p->UniqueProcessID );
   _tprintf( TEXT( "InheritedID     = %lu\n" ), information_p->InheritedFromUniqueProcessID );
   _tprintf( TEXT( "HandleCount     = %lu\n" ), information_p->HandleCount );

   _tprintf( TEXT( "VmCounters = \n{\n" ) );
   print_vm_counters( &information_p->VmCounters );
   _tprintf( TEXT( "}\n" ) );

   _tprintf( TEXT( "CommitChargeInBytes = %lu\n" ), information_p->CommitChargeInBytes );

   DWORD loop_index = 0;

   while( loop_index < information_p->ThreadCount )
   {
      _tprintf( TEXT( "Thread %lu\n{\n" ), loop_index );
      print_thread( &information_p->Threads[ loop_index ] );
      _tprintf( TEXT( "}\n" ) );

      loop_index++;
   }
}

void print_process_list( BYTE * data_buffer )
{
   WFCTRACEINIT( TEXT( "print_process_list()" ) );

   try
   {
       SYSTEM_PROCESS_INFORMATION * information_p = (SYSTEM_PROCESS_INFORMATION *) data_buffer;

      while( information_p != nullptr )
      {
         print_system_information( information_p );
         information_p = GetNext( information_p );
      }
   }
   catch( ... )
   {
   }
}
