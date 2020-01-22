/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2014, Samuel R. Blackburn
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
** $Workfile: TCTape.cpp $
** $Revision: 4 $
** $Modtime: 9/14/00 5:11a $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

void list_features( DWORD low, DWORD high );

void test_CTape( __in const UINT tape_drive_number_to_test ) noexcept
{
   WFCTRACEINIT( TEXT( "test_CTape()" ) );

   CMicrosoftTape tape;

   if ( tape.Open( tape_drive_number_to_test ) == FALSE )
   {
      WFCTRACEVAL( TEXT( "Can't open TAPE" ), tape_drive_number_to_test );
      ReportError( tape.LastError() );
      return;
   }
   else
   {
      WFCTRACE( TEXT( "Tape opened" ) );
   }

   CTapeGetDriveParameters drive_parameters;

   if ( tape.GetParameters( drive_parameters ) == FALSE )
   {
      WFCTRACEVAL( TEXT( "Can't get drive parameters TAPE" ), tape_drive_number_to_test );
      ReportError( tape.LastError() );
      return;
   }

   WFCTRACE( TEXT( "Drive Parameters:\n" ) );
   WFCTRACEVAL( TEXT( "  ECC                   = " ), ( drive_parameters.ECC            != FALSE ) ? TEXT( "True" ) : TEXT( "False" ) );
   WFCTRACEVAL( TEXT( "  Compression           = " ), ( drive_parameters.Compression    != FALSE ) ? TEXT( "True" ) : TEXT( "False" ) );
   WFCTRACEVAL( TEXT( "  DataPadding           = " ), ( drive_parameters.DataPadding    != FALSE ) ? TEXT( "True" ) : TEXT( "False" ) );
   WFCTRACEVAL( TEXT( "  ReportSetmarks        = " ), ( drive_parameters.ReportSetmarks != FALSE ) ? TEXT( "True" ) : TEXT( "False" ) );
   WFCTRACEVAL( TEXT( "  DefaultBlockSize      = " ), drive_parameters.DefaultBlockSize );
   WFCTRACEVAL( TEXT( "  MaximumBlockSize      = " ), drive_parameters.MaximumBlockSize );
   WFCTRACEVAL( TEXT( "  MinimumBlockSize      = " ), drive_parameters.MinimumBlockSize );
   WFCTRACEVAL( TEXT( "  MaximumPartitionCount = " ), drive_parameters.MaximumPartitionCount );
   WFCTRACEVAL( TEXT( "  FeaturesLow           = " ), drive_parameters.FeaturesLow );
   WFCTRACEVAL( TEXT( "  FeaturesHigh          = " ), drive_parameters.FeaturesHigh );
   WFCTRACEVAL( TEXT( "  EOTWarningZoneSize    = " ), drive_parameters.EOTWarningZoneSize );

   list_features( drive_parameters.FeaturesLow, drive_parameters.FeaturesHigh );

   CTapeGetMediaParameters media_parameters;

   if ( tape.GetParameters( media_parameters ) == FALSE )
   {
      WFCTRACEVAL( TEXT( "Can't get media parameters TAPE" ), tape_drive_number_to_test );
      ReportError( tape.LastError() );
      return;
   }
   else
   {
      WFCTRACE( TEXT( "Media Parameters:" ) );
      WFCTRACEVAL( TEXT( "  Capacity.Low   = " ), media_parameters.Capacity.LowPart   );
      WFCTRACEVAL( TEXT( "  Capacity.High  = " ), media_parameters.Capacity.HighPart  );
      WFCTRACEVAL( TEXT( "  Remaining.Low  = " ), media_parameters.Remaining.LowPart  );
      WFCTRACEVAL( TEXT( "  Remaining.High = " ), media_parameters.Remaining.HighPart );
      WFCTRACEVAL( TEXT( "  PartitionCount = " ), media_parameters.PartitionCount     );
      WFCTRACEVAL( TEXT( "  WriteProtected = " ), ( media_parameters.WriteProtected != FALSE ) ? TEXT( "True" ) : TEXT( "False" ) );
   }

#if defined( _DEBUG ) && ! defined( WFC_STL )
   tape.Dump( afxDump );
#endif
}

void list_features( DWORD low, DWORD high )
{
   WFCTRACEINIT( TEXT( "list_features()" ) );

   if ( is_flagged(low, TAPE_DRIVE_COMPRESSION) == true )
   {
      WFCTRACE( TEXT( "Device supports hardware data compression." ) );
   }

   if ( is_flagged(low, TAPE_DRIVE_ECC ) == true )
   {
      WFCTRACE( TEXT( "Device supports hardware error correction." ) );
   }

   if (is_flagged( low, TAPE_DRIVE_ERASE_BOP_ONLY ) == true )
   {
      WFCTRACE( TEXT( "Device performs the erase operation from the beginning-of-partition marker only." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_ERASE_LONG ) == true )
   {
      WFCTRACE( TEXT( "Device performs a long erase operation." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_ERASE_IMMEDIATE ) == true )
   {
      WFCTRACE( TEXT( "Device performs an immediate erase operation that is, it returns when the erase operation begins." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_ERASE_SHORT ) == true )
   {
      WFCTRACE( TEXT( "Device performs a short erase operation." ) );
   }

   if (is_flagged( low, TAPE_DRIVE_FIXED ) == true )
   {
      WFCTRACE( TEXT( "Device creates fixed data partitions." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_FIXED_BLOCK ) == true )
   {
      WFCTRACE( TEXT( "Device supports fixed-length block mode." ) );
   }

   if (is_flagged( low, TAPE_DRIVE_INITIATOR ) == true )
   {
      WFCTRACE( TEXT( "Device creates initiator-defined partitions." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_PADDING ) == true )
   {
      WFCTRACE( TEXT( "Device supports data padding." ) );
   }

   if (is_flagged(low,  TAPE_DRIVE_GET_ABSOLUTE_BLK ) == true )
   {
      WFCTRACE( TEXT( "Device provides the current device-specific block address." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_GET_LOGICAL_BLK ) == true )
   {
      WFCTRACE( TEXT( "Device provides the current logical block address (and logical tape partition)." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_REPORT_SMKS ) == true )
   {
      WFCTRACE( TEXT( "Device supports setmark reporting." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_SELECT ) == true )
   {
      WFCTRACE( TEXT( "Device creates select data partitions." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_SET_EOT_WZ_SIZE ) == true )
   {
      WFCTRACE( TEXT( "Device supports setting the end-of-medium warning size." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_TAPE_CAPACITY ) == true )
   {
      WFCTRACE( TEXT( "Device returns the maximum capacity of the tape." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_TAPE_REMAINING ) == true )
   {
      WFCTRACE( TEXT( "Device returns the remaining capacity of the tape." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_VARIABLE_BLOCK ) == true )
   {
      WFCTRACE( TEXT( "Device supports variable-length block mode." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_WRITE_PROTECT ) == true )
   {
      WFCTRACE( TEXT( "Device returns an error if the tape is write-enabled or write-protected." ) );
   }

   if (is_flagged(low, TAPE_DRIVE_EOT_WZ_SIZE ) == true )
   {
      WFCTRACE( TEXT( "TAPE_DRIVE_EOT_WZ_SIZE" ) );
   }

   if (is_flagged(low, TAPE_DRIVE_EJECT_MEDIA ) == true )
   {
      WFCTRACE( TEXT( "TAPE_DRIVE_EJECT_MEDIA" ) );
   }

   if (is_flagged(low, TAPE_DRIVE_CLEAN_REQUESTS ) == true )
   {
      WFCTRACE( TEXT( "TAPE_DRIVE_CLEAN_REQUESTS" ) );
   }

   if (is_flagged(low, TAPE_DRIVE_SET_CMP_BOP_ONLY ) == true )
   {
      WFCTRACE( TEXT( "TAPE_DRIVE_SET_CMP_BOP_ONLY" ) );
   }

   if (is_flagged(high,  TAPE_DRIVE_ABS_BLK_IMMED ) == true )
   {
      WFCTRACE( TEXT( "Device moves the tape to a device-specific block address and returns as soon as the move begins." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_ABSOLUTE_BLK ) == true )
   {
      WFCTRACE( TEXT( "Device moves the tape to a device specific block address." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_END_OF_DATA ) == true )
   {
      WFCTRACE( TEXT( "Device moves the tape to the end-of-data marker in a partition." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_FILEMARKS ) == true )
   {
      WFCTRACE( TEXT( "Device moves the tape forward (or backward) a specified number of filemarks." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_LOAD_UNLOAD ) == true )
   {
      WFCTRACE( TEXT( "Device enables and disables the device for further operations." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_LOAD_UNLD_IMMED ) == true )
   {
      WFCTRACE( TEXT( "Device supports immediate load and unload operations." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_LOCK_UNLOCK ) == true )
   {
      WFCTRACE( TEXT( "Device enables and disables the tape ejection mechanism." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_LOCK_UNLK_IMMED ) == true )
   {
      WFCTRACE( TEXT( "Device supports immediate lock and unlock operations." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_LOG_BLK_IMMED ) == true )
   {
      WFCTRACE( TEXT( "Device moves the tape to a logical block address in a partition and returns as soon as the move begins." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_LOGICAL_BLK ) == true )
   {
      WFCTRACE( TEXT( "Device moves the tape to a logical block address in a partition." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_RELATIVE_BLKS ) == true )
   {
      WFCTRACE( TEXT( "Device moves the tape forward (or backward) a specified number of blocks." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_REVERSE_POSITION ) == true )
   {
      WFCTRACE( TEXT( "Device moves the tape backward over blocks, filemarks, or setmarks." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_REWIND_IMMEDIATE ) == true )
   {
      WFCTRACE( TEXT( "Device supports immediate rewind operation." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_SEQUENTIAL_FMKS ) == true )
   {
      WFCTRACE( TEXT( "Device moves the tape forward (or backward) to the first occurrence of a specified number of consecutive filemarks." ) );
   }

   if (is_flagged(high , TAPE_DRIVE_SEQUENTIAL_SMKS ) == true )
   {
      WFCTRACE( TEXT( "Device moves the tape forward (or backward) to the first occurrence of a specified number of consecutive setmarks." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_SET_BLOCK_SIZE ) == true )
   {
      WFCTRACE( TEXT( "Device supports setting the size of a fixed-length logical block or setting the variable-length block mode." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_SET_COMPRESSION ) == true )
   {
      WFCTRACE( TEXT( "Device enables and disables hardware data compression." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_SET_ECC ) == true )
   {
      WFCTRACE( TEXT( "Device enables and disables hardware error correction." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_SET_PADDING ) == true )
   {
      WFCTRACE( TEXT( "Device enables and disables data padding." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_SET_REPORT_SMKS ) == true )
   {
      WFCTRACE( TEXT( "Device enables and disables the reporting of setmarks." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_SETMARKS ) == true )
   {
      WFCTRACE( TEXT( "Device moves the tape forward (or reverse) a specified number of setmarks." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_SPACE_IMMEDIATE ) == true )
   {
      WFCTRACE( TEXT( "Device supports immediate spacing." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_TENSION ) == true )
   {
      WFCTRACE( TEXT( "Device supports tape tensioning." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_TENSION_IMMED ) == true )
   {
      WFCTRACE( TEXT( "Device supports immediate tape tensioning." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_WRITE_FILEMARKS ) == true )
   {
      WFCTRACE( TEXT( "Device writes filemarks." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_WRITE_LONG_FMKS ) == true )
   {
      WFCTRACE( TEXT( "Device writes long filemarks." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_WRITE_MARK_IMMED ) == true )
   {
      WFCTRACE( TEXT( "Device supports immediate writing of short and long filemarks." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_WRITE_SETMARKS ) == true )
   {
      WFCTRACE( TEXT( "Device writes setmarks." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_WRITE_SHORT_FMKS ) == true )
   {
      WFCTRACE( TEXT( "Device writes short filemarks." ) );
   }

   if (is_flagged(high, TAPE_DRIVE_FORMAT ) == true )
   {
      WFCTRACE( TEXT( "TAPE_DRIVE_FORMAT" ) );
   }

   if (is_flagged(high, TAPE_DRIVE_FORMAT_IMMEDIATE ) == true )
   {
      WFCTRACE( TEXT( "TAPE_DRIVE_FORMAT_IMMEDIATE" ) );
   }
}
