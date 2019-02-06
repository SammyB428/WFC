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

   if ( low & TAPE_DRIVE_COMPRESSION )
   {
      WFCTRACE( TEXT( "Device supports hardware data compression." ) );
   }

   if ( low & TAPE_DRIVE_ECC )
   {
      WFCTRACE( TEXT( "Device supports hardware error correction." ) );
   }

   if ( low & TAPE_DRIVE_ERASE_BOP_ONLY )
   {
      WFCTRACE( TEXT( "Device performs the erase operation from the beginning-of-partition marker only." ) );
   }

   if ( low & TAPE_DRIVE_ERASE_LONG )
   {
      WFCTRACE( TEXT( "Device performs a long erase operation." ) );
   }

   if ( low & TAPE_DRIVE_ERASE_IMMEDIATE )
   {
      WFCTRACE( TEXT( "Device performs an immediate erase operation that is, it returns when the erase operation begins." ) );
   }

   if ( low & TAPE_DRIVE_ERASE_SHORT )
   {
      WFCTRACE( TEXT( "Device performs a short erase operation." ) );
   }

   if ( low & TAPE_DRIVE_FIXED )
   {
      WFCTRACE( TEXT( "Device creates fixed data partitions." ) );
   }

   if ( low & TAPE_DRIVE_FIXED_BLOCK )
   {
      WFCTRACE( TEXT( "Device supports fixed-length block mode." ) );
   }

   if ( low & TAPE_DRIVE_INITIATOR )
   {
      WFCTRACE( TEXT( "Device creates initiator-defined partitions." ) );
   }

   if ( low & TAPE_DRIVE_PADDING )
   {
      WFCTRACE( TEXT( "Device supports data padding." ) );
   }

   if ( low & TAPE_DRIVE_GET_ABSOLUTE_BLK )
   {
      WFCTRACE( TEXT( "Device provides the current device-specific block address." ) );
   }

   if ( low & TAPE_DRIVE_GET_LOGICAL_BLK )
   {
      WFCTRACE( TEXT( "Device provides the current logical block address (and logical tape partition)." ) );
   }

   if ( low & TAPE_DRIVE_REPORT_SMKS )
   {
      WFCTRACE( TEXT( "Device supports setmark reporting." ) );
   }

   if ( low & TAPE_DRIVE_SELECT )
   {
      WFCTRACE( TEXT( "Device creates select data partitions." ) );
   }

   if ( low & TAPE_DRIVE_SET_EOT_WZ_SIZE )
   {
      WFCTRACE( TEXT( "Device supports setting the end-of-medium warning size." ) );
   }

   if ( low & TAPE_DRIVE_TAPE_CAPACITY )
   {
      WFCTRACE( TEXT( "Device returns the maximum capacity of the tape." ) );
   }

   if ( low & TAPE_DRIVE_TAPE_REMAINING )
   {
      WFCTRACE( TEXT( "Device returns the remaining capacity of the tape." ) );
   }

   if ( low & TAPE_DRIVE_VARIABLE_BLOCK )
   {
      WFCTRACE( TEXT( "Device supports variable-length block mode." ) );
   }

   if ( low & TAPE_DRIVE_WRITE_PROTECT )
   {
      WFCTRACE( TEXT( "Device returns an error if the tape is write-enabled or write-protected." ) );
   }

   if ( low & TAPE_DRIVE_EOT_WZ_SIZE )
   {
      WFCTRACE( TEXT( "TAPE_DRIVE_EOT_WZ_SIZE" ) );
   }

   if ( low & TAPE_DRIVE_EJECT_MEDIA )
   {
      WFCTRACE( TEXT( "TAPE_DRIVE_EJECT_MEDIA" ) );
   }

   if ( low & TAPE_DRIVE_CLEAN_REQUESTS )
   {
      WFCTRACE( TEXT( "TAPE_DRIVE_CLEAN_REQUESTS" ) );
   }

   if ( low & TAPE_DRIVE_SET_CMP_BOP_ONLY )
   {
      WFCTRACE( TEXT( "TAPE_DRIVE_SET_CMP_BOP_ONLY" ) );
   }

   if ( high & TAPE_DRIVE_ABS_BLK_IMMED )
   {
      WFCTRACE( TEXT( "Device moves the tape to a device-specific block address and returns as soon as the move begins." ) );
   }

   if ( high & TAPE_DRIVE_ABSOLUTE_BLK )
   {
      WFCTRACE( TEXT( "Device moves the tape to a device specific block address." ) );
   }

   if ( high & TAPE_DRIVE_END_OF_DATA )
   {
      WFCTRACE( TEXT( "Device moves the tape to the end-of-data marker in a partition." ) );
   }

   if ( high & TAPE_DRIVE_FILEMARKS )
   {
      WFCTRACE( TEXT( "Device moves the tape forward (or backward) a specified number of filemarks." ) );
   }

   if ( high & TAPE_DRIVE_LOAD_UNLOAD )
   {
      WFCTRACE( TEXT( "Device enables and disables the device for further operations." ) );
   }

   if ( high & TAPE_DRIVE_LOAD_UNLD_IMMED )
   {
      WFCTRACE( TEXT( "Device supports immediate load and unload operations." ) );
   }

   if ( high & TAPE_DRIVE_LOCK_UNLOCK )
   {
      WFCTRACE( TEXT( "Device enables and disables the tape ejection mechanism." ) );
   }

   if ( high & TAPE_DRIVE_LOCK_UNLK_IMMED )
   {
      WFCTRACE( TEXT( "Device supports immediate lock and unlock operations." ) );
   }

   if ( high & TAPE_DRIVE_LOG_BLK_IMMED )
   {
      WFCTRACE( TEXT( "Device moves the tape to a logical block address in a partition and returns as soon as the move begins." ) );
   }

   if ( high & TAPE_DRIVE_LOGICAL_BLK )
   {
      WFCTRACE( TEXT( "Device moves the tape to a logical block address in a partition." ) );
   }

   if ( high & TAPE_DRIVE_RELATIVE_BLKS )
   {
      WFCTRACE( TEXT( "Device moves the tape forward (or backward) a specified number of blocks." ) );
   }

   if ( high & TAPE_DRIVE_REVERSE_POSITION )
   {
      WFCTRACE( TEXT( "Device moves the tape backward over blocks, filemarks, or setmarks." ) );
   }

   if ( high & TAPE_DRIVE_REWIND_IMMEDIATE )
   {
      WFCTRACE( TEXT( "Device supports immediate rewind operation." ) );
   }

   if ( high & TAPE_DRIVE_SEQUENTIAL_FMKS )
   {
      WFCTRACE( TEXT( "Device moves the tape forward (or backward) to the first occurrence of a specified number of consecutive filemarks." ) );
   }

   if ( high & TAPE_DRIVE_SEQUENTIAL_SMKS )
   {
      WFCTRACE( TEXT( "Device moves the tape forward (or backward) to the first occurrence of a specified number of consecutive setmarks." ) );
   }

   if ( high & TAPE_DRIVE_SET_BLOCK_SIZE )
   {
      WFCTRACE( TEXT( "Device supports setting the size of a fixed-length logical block or setting the variable-length block mode." ) );
   }

   if ( high & TAPE_DRIVE_SET_COMPRESSION )
   {
      WFCTRACE( TEXT( "Device enables and disables hardware data compression." ) );
   }

   if ( high & TAPE_DRIVE_SET_ECC )
   {
      WFCTRACE( TEXT( "Device enables and disables hardware error correction." ) );
   }

   if ( high & TAPE_DRIVE_SET_PADDING )
   {
      WFCTRACE( TEXT( "Device enables and disables data padding." ) );
   }

   if ( high & TAPE_DRIVE_SET_REPORT_SMKS )
   {
      WFCTRACE( TEXT( "Device enables and disables the reporting of setmarks." ) );
   }

   if ( high & TAPE_DRIVE_SETMARKS )
   {
      WFCTRACE( TEXT( "Device moves the tape forward (or reverse) a specified number of setmarks." ) );
   }

   if ( high & TAPE_DRIVE_SPACE_IMMEDIATE )
   {
      WFCTRACE( TEXT( "Device supports immediate spacing." ) );
   }

   if ( high & TAPE_DRIVE_TENSION )
   {
      WFCTRACE( TEXT( "Device supports tape tensioning." ) );
   }

   if ( high & TAPE_DRIVE_TENSION_IMMED )
   {
      WFCTRACE( TEXT( "Device supports immediate tape tensioning." ) );
   }

   if ( high & TAPE_DRIVE_WRITE_FILEMARKS )
   {
      WFCTRACE( TEXT( "Device writes filemarks." ) );
   }

   if ( high & TAPE_DRIVE_WRITE_LONG_FMKS )
   {
      WFCTRACE( TEXT( "Device writes long filemarks." ) );
   }

   if ( high & TAPE_DRIVE_WRITE_MARK_IMMED )
   {
      WFCTRACE( TEXT( "Device supports immediate writing of short and long filemarks." ) );
   }

   if ( high & TAPE_DRIVE_WRITE_SETMARKS )
   {
      WFCTRACE( TEXT( "Device writes setmarks." ) );
   }

   if ( high & TAPE_DRIVE_WRITE_SHORT_FMKS )
   {
      WFCTRACE( TEXT( "Device writes short filemarks." ) );
   }

   if ( high & TAPE_DRIVE_FORMAT )
   {
      WFCTRACE( TEXT( "TAPE_DRIVE_FORMAT" ) );
   }

   if ( high & TAPE_DRIVE_FORMAT_IMMEDIATE )
   {
      WFCTRACE( TEXT( "TAPE_DRIVE_FORMAT_IMMEDIATE" ) );
   }
}
