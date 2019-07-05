/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2018, Samuel R. Blackburn
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
** $Workfile: wfc_undocumented.h $
** $Revision: 3 $
** $Modtime: 2/04/00 6:46p $
*/

#if ! defined( WFC_UNDOCUMENTED_HEADER_FILE )

#define WFC_UNDOCUMENTED_HEADER_FILE

/*
** Data structures and functions in this header file
** can go away at any time. Use them at your own risk.
*/

// This information came out of a Dr. Dobb's Journal article
// #305 November 1999, "Inside Windows NT System Data" by
// Sven B. Schreiber, pp 40-48
// MANY THANKS!!!!!! I renamed the structures to avoid any
// name collisions in the future.

struct WFC_CLIENT_ID
{
    HANDLE ProcessHandle{ 0 };
    HANDLE ThreadHandle{ 0 };
};

typedef enum _WFC_KWAIT_REASON
{
   Executive,
   FreePage,
   PageIn,
   PoolAllocation,
   DelayExecution,
   Suspended,
   UserRequest,
   WrExecutive,
   WrFreePage,
   WrPageIn,
   WrPoolAllocation,
   WrDelayExecution,
   WrSuspended,
   WrUserRequest,
   WrEventPair,
   WrQueue,
   WrLpcReceive,
   WrLpcReply,
   WrVirtualMemory,
   WrPageOut,
   WrRendezvous,
   Spare2,
   Spare3,
   Spare4,
   Spare5,
   Spare6,
   WrKernel,
   MaximumWaitReason
}
WFC_KWAIT_REASON;

struct WFC_SYSTEM_THREAD
{
   ULONGLONG        KernelTime;
   ULONGLONG        UserTime;
   ULONGLONG        CreateTime;
   DWORD            WaitTime;
   VOID *           StartAddress;
   WFC_CLIENT_ID    ClientID;
   DWORD            Priority;
   DWORD            BasePriority;
   DWORD            ContextSwitches;
   DWORD            ThreadState;
   WFC_KWAIT_REASON WaitReason;
   DWORD            Reserved;
};

struct WFC_VM_COUNTERS
{
   DWORD PeakVirtualSize;
   DWORD VirtualSize;
   DWORD PageFaultCount;
   DWORD PeakWorkingSetSize;
   DWORD WorkingSetSize;
   DWORD QuotaPeakPagedPoolUsage;
   DWORD QuotaPagedPoolUsage;
   DWORD QuotaPeakNonPagedPoolUsage;
   DWORD QuotaNonPagedPoolUsage;
   DWORD PagefileUsage;
   DWORD PeakPagefileUsage;
};

inline _Check_return_ const ::SYSTEM_PROCESS_INFORMATION * GetNext( _In_ ::SYSTEM_PROCESS_INFORMATION const * information_p ) noexcept
{
    if ( information_p == nullptr || information_p->NextEntryOffset == 0 )
    {
        return( nullptr );
    }

    auto return_value = reinterpret_cast<::SYSTEM_PROCESS_INFORMATION const *> ( reinterpret_cast<uint64_t const>(information_p) + static_cast<uint64_t>(information_p->NextEntryOffset) );

    return( return_value );
}

_Check_return_ bool PASCAL wfc_undocumented_get_system_process_list( __out_bcount( size_of_buffer ) BYTE * buffer, __in DWORD const size_of_buffer ) noexcept;
_Check_return_ bool PASCAL wfc_get_process_command_line(_In_ HANDLE const process_id, __inout std::wstring& command_line) noexcept;
void PASCAL wfc_calculate_lanman_hash(__in_z char const * ascii_string, __out_bcount(16) uint8_t * hash_value) noexcept;
void PASCAL wfc_calculate_nt_hash(__in_z wchar_t const * unicode_string, __out_bcount(16) uint8_t * hash_value) noexcept;
void PASCAL wfc_debug_error_code( __in DWORD const error_code ) noexcept;

#endif // WFC_UNDOCUMENTED_HEADER_FILE
