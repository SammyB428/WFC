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
** $Workfile: test_CQueue.cpp $
** $Revision: 4 $
** $Modtime: 5/16/00 6:27p $
*/

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

__checkReturn bool test_CQueue( __out std::string& class_name, __out int& test_number_that_failed ) noexcept
{
   class_name.assign(STRING_VIEW("CQueue"));

   CQueue queue( 1 ); // A really small queue

   if ( queue.Add( 1 ) == false )
   {
      test_number_that_failed = 1;
      return( failure() );
   }

   if ( queue.Add( 2 ) == false )
   {
      test_number_that_failed = 2;
      return(failure());
   }

   if ( queue.Add( 3 ) == false )
   {
      test_number_that_failed = 3;
      return(failure());
   }

   if ( queue.Add( 4 ) == false )
   {
      test_number_that_failed = 4;
      return(failure());
   }

   if ( queue.Add( 5 ) == false )
   {
      test_number_that_failed = 5;
      return(failure());
   }

   std::size_t item = 0;

   if ( queue.Get( item ) == false )
   {
      test_number_that_failed = 6;
      return(failure());
   }

   if ( item != 1 )
   {
      test_number_that_failed = 7;
      return(failure());
   }

   if ( queue.Get( item ) == false )
   {
      test_number_that_failed = 8;
      return(failure());
   }

   if ( item != 2 )
   {
      test_number_that_failed = 9;
      return(failure());
   }

   if ( queue.Get( item ) == false )
   {
      test_number_that_failed = 10;
      return(failure());
   }

   if ( item != 3 )
   {
      test_number_that_failed = 11;
      return(failure());
   }

   if ( queue.Get( item ) == false )
   {
      test_number_that_failed = 12;
      return(failure());
   }

   if ( item != 4 )
   {
      test_number_that_failed = 13;
      return(failure());
   }

   if ( queue.Get( item ) == false )
   {
      test_number_that_failed = 14;
      return(failure());
   }

   if ( item != 5 )
   {
      test_number_that_failed = 15;
      return(failure());
   }

   // Queue is now empty, it should not Get() anything

   if ( queue.Get( item ) != false )
   {
      test_number_that_failed = 16;
      return(failure());
   }

   if ( queue.GetLength() != 0 )
   {
      test_number_that_failed = 17;
      return(failure());
   }

   if ( queue.Add( 6 ) == false )
   {
      test_number_that_failed = 18;
      return(failure());
   }

   if ( queue.Add( 7 ) == false )
   {
      test_number_that_failed = 19;
      return(failure());
   }

   if ( queue.Add( 8 ) == false )
   {
      test_number_that_failed = 20;
      return(failure());
   }

   if ( queue.GetLength() != 3 )
   {
      test_number_that_failed = 21;
      return(failure());
   }

   if ( queue.Get( item ) == false )
   {
      test_number_that_failed = 22;
      return(failure());
   }

   if ( item != 6 )
   {
      test_number_that_failed = 23;
      return(failure());
   }

   if ( queue.Add( 9 ) == false )
   {
      test_number_that_failed = 24;
      return(failure());
   }

   if ( queue.Add( 10 ) == false )
   {
      test_number_that_failed = 25;
      return(failure());
   }

   if ( queue.GetLength() != 4 )
   {
      test_number_that_failed = 26;
      return(failure());
   }

   if ( queue.Get( item ) == false )
   {
      test_number_that_failed = 27;
      return(failure());
   }

   if ( item != 7 )
   {
      test_number_that_failed = 28;
      return(failure());
   }

   if ( queue.Get( item ) == false )
   {
      test_number_that_failed = 29;
      return(failure());
   }

   if ( item != 8 )
   {
      test_number_that_failed = 30;
      return(failure());
   }

   if ( queue.Get( item ) == false )
   {
      test_number_that_failed = 31;
      return(failure());
   }

   if ( item != 9 )
   {
      test_number_that_failed = 32;
      return(failure());
   }

   if ( queue.Get( item ) == false )
   {
      test_number_that_failed = 33;
      return(failure());
   }

   if ( item != 10 )
   {
      test_number_that_failed = 34;
      return(failure());
   }

   // Queue is now empty, it should not Get() anything

   if ( queue.Get( item ) != false )
   {
      test_number_that_failed = 35;
      return(failure());
   }

   if ( queue.GetLength() != 0 )
   {
      test_number_that_failed = 36;
      return(failure());
   }

   // OK, we passed the simple tests

   CQueue queue2( 5 );

   if ( queue2.Add( 1 ) == false ) // m_AddIndex is now 1
   {
      test_number_that_failed = 37;
      return(failure());
   }

   if ( queue2.Add( 2 ) == false ) // m_AddIndex is now 2
   {
      test_number_that_failed = 38;
      return(failure());
   }

   if ( queue2.Add( 3 ) == false ) // m_AddIndex is now 3
   {
      test_number_that_failed = 39;
      return(failure());
   }

   if ( queue2.Get( item ) == false ) // m_GetIndex is now 1
   {
      test_number_that_failed = 40;
      return(failure());
   }

   if ( item != 1 )
   {
      test_number_that_failed = 41;
      return(failure());
   }

   if ( queue2.Get( item ) == false ) // m_GetIndex is now 2
   {
      test_number_that_failed = 42;
      return(failure());
   }

   if ( item != 2 )
   {
      test_number_that_failed = 43;
      return(failure());
   }

   if ( queue2.Add( 4 ) == false )
   {
      test_number_that_failed = 44;
      return(failure());
   }

   if ( queue2.Add( 5 ) == false )
   {
      test_number_that_failed = 45;
      return(failure());
   }

   if ( queue2.Add( 6 ) == false )
   {
      test_number_that_failed = 46;
      return(failure());
   }

   if ( queue2.Add( 7 ) == false ) // Should cause growth via the ReAlloc method
   {
      test_number_that_failed = 47;
      return(failure());
   }

   if ( queue2.Get( item ) == false )
   {
      test_number_that_failed = 48;
      return(failure());
   }

   if ( item != 3 )
   {
      test_number_that_failed = 49;
      return(failure());
   }

   if ( queue2.Get( item ) == false )
   {
      test_number_that_failed = 50;
      return(failure());
   }

   if ( item != 4 )
   {
      test_number_that_failed = 51;
      return(failure());
   }

   if ( queue2.Get( item ) == false )
   {
      test_number_that_failed = 52;
      return(failure());
   }

   if ( item != 5 )
   {
      test_number_that_failed = 53;
      return(failure());
   }

   if ( queue2.Get( item ) == false )
   {
      test_number_that_failed = 54;
      return(failure());
   }

   if ( item != 6 )
   {
      test_number_that_failed = 55;
      return(failure());
   }

   if ( queue2.Get( item ) == false )
   {
      test_number_that_failed = 56;
      return(failure());
   }

   if ( item != 7 )
   {
      test_number_that_failed = 57;
      return(failure());
   }

   // Queue is now empty, it should not Get() anything

   if ( queue2.Get( item ) != false )
   {
      test_number_that_failed = 58;
      return(failure());
   }

   if ( queue2.GetLength() != 0 )
   {
      test_number_that_failed = 59;
      return(failure());
   }

   test_number_that_failed = 59;
   return( true );
}
