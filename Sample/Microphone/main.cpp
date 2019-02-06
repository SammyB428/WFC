/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 1998-2003, Samuel R. Blackburn
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
*/

#define WFC_STL
#define WINVER 0x0401
#define _WIN32_WINNT 0x0401
#include <wfc.h>
#pragma hdrstop

#pragma comment( lib, "winmm.lib" )

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

void turn_microphone_off( void )
{
   WFCTRACELEVELON( 31 );

   CMixer mixer;

   if ( mixer.Open() == FALSE )
   {
      WFCTRACE( TEXT( "Can't open mixer." ) );
      return;
   }

#if 0

   CMixerControlInstance selector;

   if ( selector.Open() == FALSE )
   {
      WFCTRACE( TEXT( "Can't open instance." ) );
      return;
   }

   selector.Dump( afxDump );

#endif

   CMixerLine microphone;

   if ( mixer.GetByComponent( CMixerLine::destinationSpeakers, microphone ) == FALSE )
   {
      WFCTRACE( TEXT( "Can't get microphone" ) );
      return;
   }

   CPtrArray array;

   if ( mixer.GetAllControls( microphone, array ) == FALSE )
   {
      WFCTRACE( TEXT( "GetAllControls() failed" ) );
      return;
   }

   WFCTRACEVAL( TEXT( "Number of controls is " ), array.GetSize() );

   BOOL volume_control_was_found = FALSE;
   BOOL mute_control_was_found   = FALSE;

   int loop_index = 0;
   
   CMixerControl * control_p = NULL;

   while( loop_index < array.GetSize() )
   {
      control_p = (CMixerControl *) array.GetAt( loop_index );

      if ( control_p != NULL )
      {
#if 0
         if ( control_p->Type == CMixerControl::Mixer )
         {
            WFCTRACE( TEXT( "Creating m_SourceSelector_p" ) );

            try
            {
               m_SourceSelector_p = new CMixerSourceSelector( m_MixerLine, *control_p );
            }
            catch( ... )
            {
               m_SourceSelector_p = NULL;
            }

            ASSERT( m_SourceSelector_p != NULL );

            if ( m_SourceSelector_p != NULL )
            {
               m_SourceSelector_p->Open( device_number, what_to_notify, who_to_notify, notify_data );
            }
         }

         if ( control_p->Type == CMixerControl::Volume )
         {
            if ( volume_control_was_found == FALSE )
            {
               m_Volume.SetLine( m_MixerLine );
               m_Volume.SetControl( *control_p );
               m_Volume.Open( device_number, what_to_notify, who_to_notify, notify_data );
               volume_control_was_found = TRUE;
            }
            else
            {
               WFCTRACE( TEXT( "More than one volume control was found" ) );
            }
         }

         if ( control_p->Type == CMixerControl::Mute )
         {
            if ( mute_control_was_found == FALSE )
            {
               m_Mute.SetLine( m_MixerLine );
               m_Mute.SetControl( *control_p );
               m_Mute.Open( device_number, what_to_notify, who_to_notify, notify_data );
               mute_control_was_found = TRUE;
            }
            else
            {
               WFCTRACE( TEXT( "More than one volume control was found" ) );
            }
         }
#endif

         delete control_p;
         array.SetAt( loop_index, NULL );
      }

      loop_index++;
   }

   array.RemoveAll();

   Sleep( 500 );
}

void main( void )
{
   turn_microphone_off();
}
