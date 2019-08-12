/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2015, Samuel R. Blackburn
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

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

static LPCTSTR wait_reason( Win32FoundationClasses::WFC_KWAIT_REASON reason ) noexcept
{
   switch( reason )
   {
   case Win32FoundationClasses::WFC_KWAIT_REASON::Executive:

         return( TEXT( "Executive" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::FreePage:

         return( TEXT( "FreePage" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::PageIn:

         return( TEXT( "PageIn" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::PoolAllocation:

         return( TEXT( "PoolAllocation" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::DelayExecution:

         return( TEXT( "DelayExecution" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::Suspended:

         return( TEXT( "Suspended" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::UserRequest:

         return( TEXT( "UserRequest" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrExecutive:

         return( TEXT( "WrExecutive" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrFreePage:

         return( TEXT( "WrFreePage" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrPageIn:

         return( TEXT( "WrPageIn" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrPoolAllocation:

         return( TEXT( "WrPoolAllocation" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrDelayExecution:

         return( TEXT( "WrDelayExecution" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrSuspended:

         return( TEXT( "WrSuspended" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrUserRequest:

         return( TEXT( "WrUserRequest" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrEventPair:

         return( TEXT( "WrEventPair" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrQueue:

         return( TEXT( "WrQueue" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrLpcReceive:

         return( TEXT( "WrLpcReceive" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrLpcReply:

         return( TEXT( "WrLpcReply" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrVirtualMemory:

         return( TEXT( "WrVirtualMemory" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrPageOut:

         return( TEXT( "WrPageOut" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrRendezvous:

         return( TEXT( "WrRendezvous" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::Spare2:

         return( TEXT( "Spare2" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::Spare3:

         return( TEXT( "Spare3" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::Spare4:

         return( TEXT( "Spare4" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::Spare5:

         return( TEXT( "Spare5" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::Spare6:

         return( TEXT( "Spare6" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::WrKernel:

         return( TEXT( "WrKernel" ) );

   case Win32FoundationClasses::WFC_KWAIT_REASON::MaximumWaitReason:

         return( TEXT( "MaximumWaitReason" ) );

      default:

         return( TEXT( "Unknown" ) );
   }
}

static void print_thread( Win32FoundationClasses::WFC_SYSTEM_THREAD * thread_p ) noexcept
{
   _tprintf( TEXT( "   KernelTime             = %I64u\n" ), thread_p->KernelTime );
   _tprintf( TEXT( "   UserTime               = %I64u\n" ), thread_p->UserTime );
   _tprintf( TEXT( "   CreateTime             = %I64u\n" ), thread_p->CreateTime );
   _tprintf( TEXT( "   WaitTime               = %lu\n" ), thread_p->WaitTime );
   _tprintf( TEXT( "   StartAddress           = %p\n" ), thread_p->StartAddress );
   _tprintf( TEXT( "   ClientID\n   {\n" ) );
   _tprintf( TEXT( "      ProcessHandle = %p\n" ), thread_p->ClientID.ProcessHandle );
   _tprintf( TEXT( "      ThreadHandle  = %p\n" ), thread_p->ClientID.ThreadHandle );
   _tprintf( TEXT( "   }\n" ) );
   _tprintf( TEXT( "   Priority              = %lu\n" ), thread_p->Priority );
   _tprintf( TEXT( "   BasePriority          = %lu\n" ), thread_p->BasePriority );
   _tprintf( TEXT( "   ContextSwitches       = %lu\n" ), thread_p->ContextSwitches );
   _tprintf( TEXT( "   ThreadState           = %lu\n" ), thread_p->ThreadState );
   _tprintf( TEXT( "   WaitReason            = %s\n" ),  wait_reason( thread_p->WaitReason ) );
   _tprintf( TEXT( "   Reserved              = %lu\n" ), thread_p->Reserved );
}

#if 0
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
#endif

void print_system_information( _In_ const ::SYSTEM_PROCESS_INFORMATION * information_p ) noexcept
{
#if 0
   _tprintf( TEXT( "ThreadCount                  = %lu\n" ), information_p->ThreadCount );
   _tprintf( TEXT( "WorkingsetPrivateSize        = %I64u\n" ), information_p->WorkingsetPrivateSize );
   _tprintf( TEXT( "HardFaultCount               = %lu\n" ), information_p->HardFaultCount );
   _tprintf( TEXT( "NumberOfThreadsHighWatermark = %lu\n" ), information_p->NumberOfThreadsHighWatermark );
   _tprintf( TEXT( "CycleTime                    = %I64u\n" ), information_p->CycleTime );
   _tprintf( TEXT( "CreateTime                   = %I64u\n" ), information_p->CreateTime );
   _tprintf( TEXT( "UserTime                     = %I64u\n" ), information_p->UserTime );
   _tprintf( TEXT( "KernelTime                   = %I64u\n" ), information_p->KernelTime );
       wprintf(   L"Name                         = %s\n", information_p->Name.Buffer );
   _tprintf( TEXT( "BasePriority                 = %lu\n" ), information_p->BasePriority );
   _tprintf( TEXT( "UniqueProcessID              = %lu\n" ), information_p->UniqueProcessID );
   _tprintf( TEXT( "InheritedID                  = %lu\n" ), information_p->InheritedFromUniqueProcessID );
   _tprintf( TEXT( "HandleCount                  = %lu\n" ), information_p->HandleCount );
#endif

   std::wstring command_line;

   if ( wfc_get_process_command_line( information_p->UniqueProcessId, command_line ) == true )
   {
#if defined( _DEBUG )
      wprintf( L"Command line: %s\n", command_line.c_str() );
#endif
   }

   //_tprintf( TEXT( "VmCounters = \n{\n" ) );
   //print_vm_counters( &information_p->VmCounters );
   //_tprintf( TEXT( "}\n" ) );

   //_tprintf( TEXT( "CommitChargeInBytes = %lu\n" ), information_p->CommitChargeInBytes );

   for( auto const loop_index : Range(information_p->NumberOfThreads) )
   {
      //_tprintf( TEXT( "Thread %lu\n{\n" ), loop_index );
    //  print_thread( &information_p->Threads[ loop_index ] );
    //  _tprintf( TEXT( "}\n" ) );
   }
}

static void print_process_list( BYTE * data_buffer ) noexcept
{
   try
   {
      auto information_p = reinterpret_cast<::SYSTEM_PROCESS_INFORMATION const *>(data_buffer);

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

__checkReturn bool test_process_list( __out std::string& class_name, __out int& test_number_that_failed ) noexcept
{
    class_name.assign( "process list" );

    // The first thing we need to do is get a buffer large
    // enough to hold the raw data. I like overkill so I choose

    DWORD const buffer_size = 4 * ONE_MEGABYTE;

    std::unique_ptr<uint8_t[]> allocated_buffer = std::make_unique<uint8_t[]>(buffer_size);

    if (allocated_buffer.get() == nullptr )
    {
        test_number_that_failed = 1;
        return( failure() );
    }

    if ( Win32FoundationClasses::wfc_undocumented_get_system_process_list(allocated_buffer.get(), buffer_size) != FALSE )
    {
        print_process_list(allocated_buffer.get());
    }
    else
    {
        test_number_that_failed = 2;
        return( failure() );
    }

    test_number_that_failed = 2;
    return( true );
}
