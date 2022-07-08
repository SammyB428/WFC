/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2022, Samuel R. Blackburn
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
** $Workfile: CQueue_integrity_test.cpp $
** $Revision: 1 $
** $Modtime: 5/21/00 7:58a $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

// If you don't have more than one CPU, don't bother
// to run this test. You will never get into a
// deadlock.

 struct QUEUE_INTEGRITY_WORKSPACE
{
    Win32FoundationClasses::CQueue * queue_p{ nullptr };
    uint64_t number_of_operations{ 0 };
    uint32_t number_of_loops{ 0 };
    uint32_t checksum{ 0 };
    bool     thread_exitted{ false };
};

void queue_integrity_add_thread( void * p ) noexcept
{
   auto workspace_p = static_cast<QUEUE_INTEGRITY_WORKSPACE *>(p);

   if ( workspace_p == NULL )
   {
      return;
   }

   workspace_p->number_of_operations = 0;
   workspace_p->checksum             = 0;
   workspace_p->thread_exitted       = false;

   auto queue_p{ workspace_p->queue_p };

   uint32_t item{ 0 };
   uint32_t checksum_index{ 0 };

   Win32FoundationClasses::CRandomNumberGenerator2 random;

   for( auto const loop_index : Range(workspace_p->number_of_loops) )
   {
      item = random.GetInteger();

      std::ignore = queue_p->Add( item );

      workspace_p->number_of_operations++;

      checksum_index = 0;

      while( _bittest( reinterpret_cast<LONG const *>(&workspace_p->checksum), 31 ) == 0 and
             checksum_index < 32 )
      {
         workspace_p->checksum <<= 1;
         checksum_index++;
      }

      workspace_p->checksum xor_eq item;
   }

   _tprintf( TEXT( "Add: %lu items checksummed to %08lX\n" ),
             workspace_p->number_of_loops,
             workspace_p->checksum );

   workspace_p->thread_exitted = true;
}

void queue_integrity_get_thread( void * p ) noexcept
{
    auto workspace_p{ static_cast<QUEUE_INTEGRITY_WORKSPACE*>(p) };

   if ( workspace_p == NULL )
   {
      return;
   }

   workspace_p->number_of_operations = 0;
   workspace_p->checksum             = 0;
   workspace_p->thread_exitted       = false;

   auto queue_p{ workspace_p->queue_p };

   uint32_t checksum_index{ 0 };

   SIZE_T item{ 0 };

   for( auto const loop_index : Range(workspace_p->number_of_loops) )
   {
      // Only process items that we got. We need to account for
      // getting from an empty queue.
      if ( queue_p->Get( item ) == true )
      {
         workspace_p->number_of_operations++;

         checksum_index = 0;

         while( _bittest( reinterpret_cast<LONG const *>(&workspace_p->checksum), 31 ) == 0 and
                checksum_index < 32 )
         {
            workspace_p->checksum <<= 1;
            checksum_index++;
         }

         workspace_p->checksum xor_eq item;
      }
      else
      {
         // Give the Add thread a little time to add items
         Sleep( 1000 );
      }
   }

   _tprintf( TEXT( "Get: %lu items checksummed to %08lX\n" ),
             workspace_p->number_of_loops,
             workspace_p->checksum );

   workspace_p->thread_exitted = true;
}

_Check_return_ bool CQueue_integrity_test( _Out_ std::string& class_name, _Out_ int& test_number_that_failed ) noexcept
{
   WFCTRACEINIT( TEXT( "CQueue_integrity_test()" ) );

   class_name.assign(STRING_VIEW("CQueue Integrity"));

   test_number_that_failed = 1; // There's only one test

   QUEUE_INTEGRITY_WORKSPACE get_workspace;
   QUEUE_INTEGRITY_WORKSPACE add_workspace;

   Win32FoundationClasses::CQueue queue;

   get_workspace.queue_p = &queue;
   add_workspace.queue_p = &queue;

   get_workspace.thread_exitted = false;
   add_workspace.thread_exitted = false;

   get_workspace.number_of_loops = 50000000; // fifty million
   add_workspace.number_of_loops = get_workspace.number_of_loops;

   SYSTEM_INFO system_information;

   ZeroMemory( &system_information, sizeof( system_information ) );

   GetSystemInfo( &system_information );

   if ( system_information.dwNumberOfProcessors < 2 )
   {
      _tprintf( TEXT( "The CQueue integrity test is not valid on this machine since you have only 1 CPU.\n" ) );
      _tprintf( TEXT( "With 1 CPU, it is impossible to achieve true concurrency.\n" ) );
      _tprintf( TEXT( "Skipping test.\n" ) );
      return( TRUE );
   }

   auto get_thread_handle{ (HANDLE)_beginthread(queue_integrity_get_thread, DEFAULT_THREAD_STACK_SIZE, &get_workspace) };
   auto add_thread_handle{ (HANDLE)_beginthread(queue_integrity_add_thread, DEFAULT_THREAD_STACK_SIZE, &add_workspace) };

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

   Sleep( 2000 );

   while( add_workspace.thread_exitted == false )
   {
      Sleep( 2000 );
   }

   while( get_workspace.thread_exitted == false )
   {
      Sleep( 1000 );
   }

   if ( add_workspace.checksum == get_workspace.checksum )
   {
      return( true );
   }
   else
   {
      return( false );
   }
}
