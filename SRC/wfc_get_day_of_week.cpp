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
** $Workfile: wfc_get_day_of_week.cpp $
** $Revision: 7 $
** $Modtime: 6/26/01 10:58a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

_Check_return_ bool PASCAL Win32FoundationClasses::wfc_get_day_of_week( __in int year_with_century, __in int month, __in int day, __out int& day_of_week ) noexcept
{
   day_of_week = 0;

   // This function is good only for Gregorian dates (until I find
   // source code that will handle ANY date). For now, all dates since
   // 1582 should suffice. The Gregorian calendar bagan on October 15, 1582.

   if ( year_with_century < 1582 )
   {
      //WFCTRACE( TEXT( "Can't handle dates prior to 1582." ) );
      return( false );
   }

   if ( year_with_century == 1582 )
   {
      if ( month < 10 )
      {
         //WFCTRACE( TEXT( "Can't handle dates prior to October, 1582." ) );
         return( false );
      }

      if ( month == 10 and day < 15 )
      {
         //WFCTRACE( TEXT( "Can't handle dates prior to October 15, 1582." ) );
         return( false );
      }
   }

   // Now, there's another little quirk, September 3rd through 13th did not
   // exist in 1752.

   if ( year_with_century == 1752 and month == 9 )
   {
      if ( day >= 3 and day <= 13 )
      {
         //WFCTRACE( TEXT( "September 3rd-13th did not exist in 1752." ) );
         return( false );
      }
   }

   if ( month <= 2 )
   {
      year_with_century++;
      month += 12;
   }

   uint32_t temporary_day_of_week = 0;

   double term_1 = static_cast< double >( month + 1 ) * 3.0;
   term_1 /= 5.0;

   temporary_day_of_week  = ( day + ( month * 2 ) + static_cast< int >( term_1 ) + year_with_century + ( year_with_century / 4 ) );
   temporary_day_of_week -= ( year_with_century / 100 );
   temporary_day_of_week += ( year_with_century / 400 );
   temporary_day_of_week++;

   // Let's fix things for the Sept 3-13th that don't exist in 1752
   if ( year_with_century == 1752 )
   {
      if ( month < 9 )
      {
         temporary_day_of_week -= 10;
      }
      else if ( month == 9 )
      {
         if ( day <= 2 )
         {
            temporary_day_of_week -= 10;
         }
      }
   }
   else if ( year_with_century < 1752 )
   {
      temporary_day_of_week -= 10;
   }

   day_of_week = ( static_cast< int >( temporary_day_of_week % 7 ) );

   return( true );
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_get_day_of_week</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that computes the day of the week (taking into account September 3rd through 13th did not exist in 1752).">
</HEAD>

<BODY>

<H1>wfc_get_day_of_week</H1>
$Revision: 7 $<HR>

<H2>Declaration</H2>
<PRE>BOOL <B>wfc_get_day_of_week</B>( int year_with_century, int month, int day, int&amp; day_of_week )</PRE>

<H2>Description</H2>
This function returns TRUE if the day of week was successfully computed.
It will return 0 (Sunday) through 6 (Saturday).

<H2>Example</H2>
<PRE><CODE>bool is_monday( int year, int month, int day )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;is_monday()&quot; ) );


   int day_of_week = 0;

   if ( <B>wfc_get_day_of_week</B>( year, month, day, day_of_week ) == FALSE )
   {
      return( FALSE );
   }

   if ( day_of_week == 1 )
   {
      return( TRUE );
   }

   return( FALSE );
}</CODE></PRE>

<H2>API's Used</H2>
<B>wfc_get_day_of_week</B>() doesn't use any API's.

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_get_day_of_week.cpp $<BR>
$Modtime: 6/26/01 10:58a $
</BODY>

</HTML>
*/
