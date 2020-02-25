/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2016, Samuel R. Blackburn
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
** $Workfile: CQueue_stress_test.cpp $
** $Revision: 3 $
** $Modtime: 5/21/00 7:12a $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

// If you don't have more than one CPU, don't bother
// to run this test. You will never get into a
// deadlock.

 struct QUEUE_WORKSPACE
{
    Win32FoundationClasses::CQueue * queue_p{ nullptr };
    uint64_t number_of_operations{ 0 };
    bool     exit_thread{ false };
};

void queue_add_thread( void * p )
{
   auto workspace_p = static_cast<QUEUE_WORKSPACE *>(p);

   if ( workspace_p == nullptr)
   {
      return;
   }

   workspace_p->number_of_operations = 0;

   auto queue_p = workspace_p->queue_p;

#pragma warning( disable : 4312 )
   void * item = (void *) 0xFFFFFFFF;

   while( workspace_p->exit_thread == false )
   {
      if ( queue_p->Add( item ) != false )
      {
         workspace_p->number_of_operations++;
      }
   }
}

void queue_get_thread( void * p )
{
   auto workspace_p = static_cast<QUEUE_WORKSPACE *>(p);

   if ( workspace_p == nullptr )
   {
      return;
   }

   workspace_p->number_of_operations = 0;

   auto queue_p = workspace_p->queue_p;

   void * item = nullptr;

   while( workspace_p->exit_thread == false )
   {
      if ( queue_p->TryGet( item ) != false )
      {
         workspace_p->number_of_operations++;
      }
   }
}

_Check_return_ bool CQueue_stress_test( _Out_ std::string& class_name, _Out_ int& test_number_that_failed ) noexcept
{
   class_name.assign(STRING_VIEW("CQueue Stress"));

   test_number_that_failed = 1; // There's only one test...

   QUEUE_WORKSPACE get_workspace;
   QUEUE_WORKSPACE add_workspace;

   Win32FoundationClasses::CQueue queue;

   get_workspace.queue_p = &queue;
   add_workspace.queue_p = &queue;

   get_workspace.exit_thread = false;
   add_workspace.exit_thread = false;

   uint32_t start_time = (uint32_t) time(nullptr);

   SYSTEM_INFO system_information;

   ZeroMemory( &system_information, sizeof( system_information ) );

   GetSystemInfo( &system_information );

   if ( system_information.dwNumberOfProcessors < 2 )
   {
      _tprintf( TEXT( "The CQueue stress test is not valid on this machine since you have only 1 CPU.\n" ) );
      _tprintf( TEXT( "With 1 CPU, it is impossible to achieve true concurrency.\n" ) );
      _tprintf( TEXT( "Skipping test.\n" ) );
      return( true );
   }
   
   _tprintf( TEXT( "You have %d CPUs\n" ), system_information.dwNumberOfProcessors );

   HANDLE get_thread_handle = (HANDLE) _beginthread( queue_get_thread, DEFAULT_THREAD_STACK_SIZE, &get_workspace );
   HANDLE add_thread_handle = (HANDLE) _beginthread( queue_add_thread, DEFAULT_THREAD_STACK_SIZE, &add_workspace );

   // If we have more than one CPU, set the thread affinity masks for
   // the threads so they will stick to different CPUs and therefore
   // execute faster.

   SetThreadIdealProcessor( get_thread_handle, 0 );

   if ( system_information.dwNumberOfProcessors > 2 )
   {
      // More than two CPU's which means hyperthreading is probably
      // enabled. Select CPU #2 so we will execute on two different chips.
      SetThreadIdealProcessor( add_thread_handle, 2 );
   }
   else
   {
      // Only two CPUs in this box, use both
      SetThreadIdealProcessor( add_thread_handle, 1 );
   }

   HANDLE add_thread_handles[ 1 ] = { INVALID_HANDLE_VALUE };
   HANDLE get_thread_handles[ 4 ] = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };

   if ( system_information.dwNumberOfProcessors > 17 )
   {
	   for ( auto const array_index : Range(std::size( get_thread_handles ) ) )
	   {
		   get_thread_handles[ array_index ] = (HANDLE) _beginthread( queue_get_thread, DEFAULT_THREAD_STACK_SIZE, &get_workspace );
	   }

       for (auto const array_index : Range(std::size( add_thread_handles ) ) )
	   {
		   //add_thread_handles[ array_index ] = (HANDLE) _beginthread( queue_add_thread, DEFAULT_THREAD_STACK_SIZE, &get_workspace );
	   }
   }

   // Now run until the user presses the Enter key on the keyboard
   Sleep( 29989 * 3 ); // almost seconds

   add_workspace.exit_thread = true;
   get_workspace.exit_thread = true;

   DWORD end_time = (DWORD) time(nullptr);
   DWORD number_of_seconds_we_ran = end_time - start_time;

   // Now print the results

   _tprintf( TEXT( "We ran for %lu seconds.\n" ), number_of_seconds_we_ran );
   _tprintf( TEXT( "Added %I64u items (%lu operations per second)\n" ),
             add_workspace.number_of_operations,
             (DWORD) ( add_workspace.number_of_operations / number_of_seconds_we_ran ) );
   _tprintf( TEXT( "Got %I64u items (%lu operations per second)\n" ),
             get_workspace.number_of_operations,
             (DWORD) ( get_workspace.number_of_operations / number_of_seconds_we_ran ) );
   _tprintf( TEXT( "Left %lu items on the queue.\n" ), (uint32_t) queue.GetLength() );
   _tprintf( TEXT( "Queue grew to %lu\n" ), (uint32_t) queue.GetMaximumLength() );

   return( true );
}
