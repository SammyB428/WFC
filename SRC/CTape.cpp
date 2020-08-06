/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2019, Samuel R. Blackburn
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
** $Workfile: ctape.cpp $
** $Revision: 34 $
** $Modtime: 6/26/01 10:51a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

/*
** CTapeGetDriveParameters
*/

Win32FoundationClasses::CTapeGetDriveParameters::CTapeGetDriveParameters() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CTapeGetDriveParameters::CTapeGetDriveParameters( _In_ Win32FoundationClasses::CTapeGetDriveParameters const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CTapeGetDriveParameters::CTapeGetDriveParameters( _In_ TAPE_GET_DRIVE_PARAMETERS const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

void Win32FoundationClasses::CTapeGetDriveParameters::Copy(_In_ Win32FoundationClasses::CTapeGetDriveParameters const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( static_cast<TAPE_GET_DRIVE_PARAMETERS const *>(&source) );
}

void Win32FoundationClasses::CTapeGetDriveParameters::Copy(_In_ TAPE_GET_DRIVE_PARAMETERS const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      ECC                   = source->ECC;
      Compression           = source->Compression;
      DataPadding           = source->DataPadding;
      ReportSetmarks        = source->ReportSetmarks;
      DefaultBlockSize      = source->DefaultBlockSize;
      MaximumBlockSize      = source->MaximumBlockSize;
      MinimumBlockSize      = source->MinimumBlockSize;
      MaximumPartitionCount = source->MaximumPartitionCount;
      FeaturesLow           = source->FeaturesLow;
      FeaturesHigh          = source->FeaturesHigh;
      EOTWarningZoneSize    = source->EOTWarningZoneSize;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CTapeGetDriveParameters::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CTapeGetDriveParameters at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   ECC = "                   ) << ECC                   << TEXT( "\n" );
   dump_context << TEXT( "   Compression = "           ) << Compression           << TEXT( "\n" );
   dump_context << TEXT( "   DataPadding = "           ) << DataPadding           << TEXT( "\n" );
   dump_context << TEXT( "   ReportSetmarks = "        ) << ReportSetmarks        << TEXT( "\n" );
   dump_context << TEXT( "   DefaultBlockSize = "      ) << DefaultBlockSize      << TEXT( "\n" );
   dump_context << TEXT( "   MaximumBlockSize = "      ) << MaximumBlockSize      << TEXT( "\n" );
   dump_context << TEXT( "   MinimumBlockSize = "      ) << MinimumBlockSize      << TEXT( "\n" );
   dump_context << TEXT( "   MaximumPartitionCount = " ) << MaximumPartitionCount << TEXT( "\n" );
   dump_context << TEXT( "   FeaturesLow = "           ) << FeaturesLow           << TEXT( " (" );

   std::wstring temp_string;

   if ( FeaturesLow bitand TAPE_DRIVE_COMPRESSION )
   {
      temp_string = TEXT( "TAPE_DRIVE_COMPRESSION" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_ECC )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_ECC" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_EJECT_MEDIA )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_EJECT_MEDIA" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_ERASE_BOP_ONLY )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_ERASE_BOP_ONLY" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_ERASE_LONG )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_ERASE_LONG" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_ERASE_IMMEDIATE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_ERASE_IMMEDIATE" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_ERASE_SHORT )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_ERASE_SHORT" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_FIXED )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_FIXED" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_FIXED_BLOCK )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_FIXED_BLOCK" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_INITIATOR )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_INITIATOR" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_PADDING )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_PADDING" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_GET_ABSOLUTE_BLK )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_GET_ABSOLUTE_BLK" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_GET_LOGICAL_BLK )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_GET_LOGICAL_BLK" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_REPORT_SMKS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_REPORT_SMKS" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_SELECT )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_SELECT" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_SET_EOT_WZ_SIZE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_SET_EOT_WZ_SIZE" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_EOT_WZ_SIZE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_EOT_WZ_SIZE" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_TAPE_CAPACITY )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_TAPE_CAPACITY" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_TAPE_REMAINING )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_TAPE_REMAINING" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_VARIABLE_BLOCK )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_VARIABLE_BLOCK" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_WRITE_PROTECT )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_WRITE_PROTECT" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_CLEAN_REQUESTS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_CLEAN_REQUESTS" );
   }

   if ( FeaturesLow bitand TAPE_DRIVE_SET_CMP_BOP_ONLY )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_SET_CMP_BOP_ONLY" );
   }

   dump_context << temp_string << TEXT( " )\n" );

   dump_context << TEXT( "   FeaturesHigh = "          ) << FeaturesHigh          << TEXT( " (" );

   DWORD features_high = FeaturesHigh;

   features_high or_eq TAPE_DRIVE_HIGH_FEATURES;

   temp_string.Empty();

   if ( features_high bitand TAPE_DRIVE_LOAD_UNLOAD )
   {
      temp_string = TEXT( "TAPE_DRIVE_LOAD_UNLOAD" );
   }

   if ( features_high bitand TAPE_DRIVE_TENSION )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_TENSION" );
   }

   if ( features_high bitand TAPE_DRIVE_LOCK_UNLOCK )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_LOCK_UNLOCK" );
   }

   if ( features_high bitand TAPE_DRIVE_REWIND_IMMEDIATE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_REWIND_IMMEDIATE" );
   }

   if ( features_high bitand TAPE_DRIVE_SET_BLOCK_SIZE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_SET_BLOCK_SIZE" );
   }

   if ( features_high bitand TAPE_DRIVE_LOAD_UNLD_IMMED )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_LOAD_UNLD_IMMED" );
   }

   if ( features_high bitand TAPE_DRIVE_TENSION_IMMED )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_TENSION_IMMED" );
   }

   if ( features_high bitand TAPE_DRIVE_LOCK_UNLK_IMMED )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_LOCK_UNLK_IMMED" );
   }

   if ( features_high bitand TAPE_DRIVE_SET_ECC )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_SET_ECC" );
   }

   if ( features_high bitand TAPE_DRIVE_SET_COMPRESSION )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_SET_COMPRESSION" );
   }

   if ( features_high bitand TAPE_DRIVE_SET_PADDING )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_SET_PADDING" );
   }

   if ( features_high bitand TAPE_DRIVE_SET_REPORT_SMKS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_SET_REPORT_SMKS" );
   }

   if ( features_high bitand TAPE_DRIVE_ABSOLUTE_BLK )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_ABSOLUTE_BLK" );
   }

   if ( features_high bitand TAPE_DRIVE_ABS_BLK_IMMED )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_ABS_BLK_IMMED" );
   }

   if ( features_high bitand TAPE_DRIVE_LOGICAL_BLK )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_LOGICAL_BLK" );
   }

   if ( features_high bitand TAPE_DRIVE_LOG_BLK_IMMED )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_LOG_BLK_IMMED" );
   }

   if ( features_high bitand TAPE_DRIVE_END_OF_DATA )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_END_OF_DATA" );
   }

   if ( features_high bitand TAPE_DRIVE_RELATIVE_BLKS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_RELATIVE_BLKS" );
   }

   if ( features_high bitand TAPE_DRIVE_FILEMARKS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_FILEMARKS" );
   }

   if ( features_high bitand TAPE_DRIVE_SEQUENTIAL_FMKS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_SEQUENTIAL_FMKS" );
   }

   if ( features_high bitand TAPE_DRIVE_SETMARKS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_SETMARKS" );
   }

   if ( features_high bitand TAPE_DRIVE_SEQUENTIAL_SMKS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_SEQUENTIAL_SMKS" );
   }

   if ( features_high bitand TAPE_DRIVE_REVERSE_POSITION )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_REVERSE_POSITION" );
   }

   if ( features_high bitand TAPE_DRIVE_SPACE_IMMEDIATE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_SPACE_IMMEDIATE" );
   }

   if ( features_high bitand TAPE_DRIVE_WRITE_SETMARKS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_WRITE_SETMARKS" );
   }

   if ( features_high bitand TAPE_DRIVE_WRITE_FILEMARKS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_WRITE_FILEMARKS" );
   }

   if ( features_high bitand TAPE_DRIVE_WRITE_SHORT_FMKS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_WRITE_SHORT_FMKS" );
   }

   if ( features_high bitand TAPE_DRIVE_WRITE_LONG_FMKS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_WRITE_LONG_FMKS" );
   }

   if ( features_high bitand TAPE_DRIVE_WRITE_MARK_IMMED )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_WRITE_MARK_IMMED" );
   }

   if ( features_high bitand TAPE_DRIVE_FORMAT )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_FORMAT" );
   }

   if ( features_high bitand TAPE_DRIVE_FORMAT_IMMEDIATE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "TAPE_DRIVE_FORMAT_IMMEDIATE" );
   }

   dump_context << temp_string << TEXT( " )\n" );

   dump_context << TEXT( "   EOTWarningZoneSize = "    ) << EOTWarningZoneSize    << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CTapeGetDriveParameters::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ECC                   = 0;
   Compression           = 0;
   DataPadding           = 0;
   ReportSetmarks        = 0;
   DefaultBlockSize      = 0;
   MaximumBlockSize      = 0;
   MinimumBlockSize      = 0;
   MaximumPartitionCount = 0;
   FeaturesLow           = 0;
   FeaturesHigh          = 0;
   EOTWarningZoneSize    = 0;
}

Win32FoundationClasses::CTapeGetDriveParameters& Win32FoundationClasses::CTapeGetDriveParameters::operator=( _In_ Win32FoundationClasses::CTapeGetDriveParameters const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CTapeGetDriveParameters& Win32FoundationClasses::CTapeGetDriveParameters::operator=( _In_ TAPE_GET_DRIVE_PARAMETERS const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( &source );
   return( *this );
}

/*
** CTapeSetDriveParameters
*/

Win32FoundationClasses::CTapeSetDriveParameters::CTapeSetDriveParameters() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CTapeSetDriveParameters::CTapeSetDriveParameters( _In_ Win32FoundationClasses::CTapeSetDriveParameters const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CTapeSetDriveParameters::CTapeSetDriveParameters( _In_ TAPE_SET_DRIVE_PARAMETERS const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

void Win32FoundationClasses::CTapeSetDriveParameters::Copy(_In_ Win32FoundationClasses::CTapeSetDriveParameters const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( static_cast<TAPE_SET_DRIVE_PARAMETERS const *>(&source) );
}

void Win32FoundationClasses::CTapeSetDriveParameters::Copy(_In_ TAPE_SET_DRIVE_PARAMETERS const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      ECC                = source->ECC;
      Compression        = source->Compression;
      ReportSetmarks     = source->ReportSetmarks;
      EOTWarningZoneSize = source->EOTWarningZoneSize;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CTapeSetDriveParameters::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CTapeSetDriveParameters at " )<< (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   ECC = "                ) << ECC                << TEXT( "\n" );
   dump_context << TEXT( "   Compression = "        ) << Compression        << TEXT( "\n" );
   dump_context << TEXT( "   ReportSetmarks = "     ) << ReportSetmarks     << TEXT( "\n" );
   dump_context << TEXT( "   EOTWarningZoneSize = " ) << EOTWarningZoneSize << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CTapeSetDriveParameters::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   ECC                = 0;
   Compression        = 0;
   DataPadding        = 0;
   ReportSetmarks     = 0;
   EOTWarningZoneSize = 0;
}

Win32FoundationClasses::CTapeSetDriveParameters& Win32FoundationClasses::CTapeSetDriveParameters::operator=( _In_ Win32FoundationClasses::CTapeSetDriveParameters const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CTapeSetDriveParameters& Win32FoundationClasses::CTapeSetDriveParameters::operator=( _In_ TAPE_SET_DRIVE_PARAMETERS const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( &source );
   return( *this );
}

/*
** CTapeSetMediaParameters
*/

Win32FoundationClasses::CTapeSetMediaParameters::CTapeSetMediaParameters() noexcept
{
   WFC_VALIDATE_POINTER( this );
   Empty();
}

Win32FoundationClasses::CTapeSetMediaParameters::CTapeSetMediaParameters( _In_ Win32FoundationClasses::CTapeSetMediaParameters const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

Win32FoundationClasses::CTapeSetMediaParameters::CTapeSetMediaParameters( _In_ TAPE_SET_MEDIA_PARAMETERS const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );
   Copy( source );
}

void Win32FoundationClasses::CTapeSetMediaParameters::Copy(_In_ Win32FoundationClasses::CTapeSetMediaParameters const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( static_cast<TAPE_SET_MEDIA_PARAMETERS const *>(&source) );
}

void Win32FoundationClasses::CTapeSetMediaParameters::Copy(_In_ TAPE_SET_MEDIA_PARAMETERS const * source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      Empty();
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      BlockSize = source->BlockSize;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CTapeSetMediaParameters::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CTapeSetMediaParameters at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   BlockSize = " ) << BlockSize << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CTapeSetMediaParameters::Empty( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   BlockSize = 0;
}

Win32FoundationClasses::CTapeSetMediaParameters& Win32FoundationClasses::CTapeSetMediaParameters::operator=( _In_ Win32FoundationClasses::CTapeSetMediaParameters const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CTapeSetMediaParameters& Win32FoundationClasses::CTapeSetMediaParameters::operator=( _In_ TAPE_SET_MEDIA_PARAMETERS const& source ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   Copy( &source );
   return( *this );
}

/*
** CTape
*/

Win32FoundationClasses::CTape::CTape() noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
   m_AutomaticallyClose = false;
}

Win32FoundationClasses::CTape::~CTape()
{
   WFC_VALIDATE_POINTER( this );

   if ( m_AutomaticallyClose not_eq FALSE )
   {
      Close();
   }

   m_Initialize();
}

void Win32FoundationClasses::CTape::m_Initialize( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   m_BackupReadContextPointer  = nullptr;
   m_BackupWriteContextPointer = nullptr;
}

void Win32FoundationClasses::CTape::Close( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   (void) Unlock();
   (void) GetStatus();

   if ( m_FileHandle not_eq static_cast< HANDLE >( NULL ) )
   {
      CDummyFile::Close();
   }
}

bool Win32FoundationClasses::CTape::CreatePartition( _In_ TypeOfPartition const type_of_partition,
                             _In_ DWORD           const number_of_partitions, 
                             _In_ DWORD           const number_of_megabytes_in_each_partition ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_LastError = ::CreateTapePartition( m_FileHandle, (DWORD) type_of_partition, number_of_partitions, number_of_megabytes_in_each_partition );

   if ( m_LastError == NO_ERROR )
   {
      return( true );
   }

   return( false );
}

bool Win32FoundationClasses::CTape::Erase( _In_ TypeOfErasure const type_of_erasure, _In_ bool const return_immediately ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_LastError = ::EraseTape( m_FileHandle, static_cast<DWORD>(type_of_erasure), return_immediately );

   if ( m_LastError == NO_ERROR )
   {
      return( true );
   }

   return( false );
}

bool Win32FoundationClasses::CTape::GetParameters( _Out_ Win32FoundationClasses::CTapeGetDriveParameters& parameters ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD size = sizeof( TAPE_GET_DRIVE_PARAMETERS );

   TAPE_GET_DRIVE_PARAMETERS drive_parameters;

   ::ZeroMemory( &drive_parameters, size );

   m_LastError = ::GetTapeParameters( m_FileHandle, GET_TAPE_DRIVE_INFORMATION, &size, &drive_parameters );

   if ( m_LastError == NO_ERROR )
   {
      parameters.Copy( &drive_parameters );
      return( true );
   }

   return( false );
}

bool Win32FoundationClasses::CTape::GetParameters( _Out_ Win32FoundationClasses::CTapeGetMediaParameters& parameters ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   DWORD size = sizeof( TAPE_GET_MEDIA_PARAMETERS );

   TAPE_GET_MEDIA_PARAMETERS media_parameters;

   ::ZeroMemory( &media_parameters, size );

   m_LastError = ::GetTapeParameters( m_FileHandle, GET_TAPE_MEDIA_INFORMATION, &size, &media_parameters );

   if ( m_LastError == NO_ERROR )
   {
      parameters.Copy( &media_parameters );
      return( true );
   }

   return( false );
}

bool Win32FoundationClasses::CTape::GetPosition( _In_ Position const type_of_position_to_get, _Out_ DWORD& partition_number, _Out_ DWORD& position_low, _Out_ DWORD& position_high ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_LastError = ::GetTapePosition( m_FileHandle, static_cast<DWORD>(type_of_position_to_get), &partition_number, &position_low, &position_high );

   if ( m_LastError == NO_ERROR )
   {
      return( true );
   }

   return( false );
}

_Check_return_ DWORD Win32FoundationClasses::CTape::GetStatus( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( ::GetTapeStatus( m_FileHandle ) );
}

_Check_return_ DWORD Win32FoundationClasses::CTape::GetDriveNumber( void ) const noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( m_TapeDriveNumberStartingAtZero );
}

_Check_return_ bool Win32FoundationClasses::CTape::Mark( _In_ TypeOfMark const type_of_mark, _In_ DWORD const number_of_marks, _In_ bool const return_immediately ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_LastError = ::WriteTapemark( m_FileHandle, static_cast<DWORD>(type_of_mark), number_of_marks, return_immediately == true ? TRUE : FALSE );

   if ( m_LastError == NO_ERROR )
   {
      return( true );
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CTape::Load( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( Prepare(Operation::LoadTape) );
}

_Check_return_ bool Win32FoundationClasses::CTape::Lock( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( Prepare(Operation::LockTape) );
}

_Check_return_ bool Win32FoundationClasses::CTape::Open( _In_ UINT const tape_drive_number_starting_at_zero ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   TCHAR tape_drive_name[ MAX_PATH ];

   ::ZeroMemory( tape_drive_name, sizeof( tape_drive_name ) );

   _stprintf_s( tape_drive_name, std::size( tape_drive_name ), TEXT( "\\\\.\\TAPE%u" ), tape_drive_number_starting_at_zero );

   HANDLE const file_handle = ::CreateFile( tape_drive_name,
                               GENERIC_READ bitor GENERIC_WRITE,
                               0,
                               nullptr,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               static_cast< HANDLE >( NULL ) );

   if ( file_handle == static_cast< HANDLE >( INVALID_HANDLE_VALUE ) )
   {
      m_LastError = ::GetLastError();

      //WFCTRACEERROR( m_LastError );

      return( false );
   }

#if ! defined( WFC_STL )
   m_strFileName = tape_drive_name;
   m_hFile = (CFILE_HFILE) file_handle;
#else
#pragma warning( disable : 4302 )
   m_FileHandle = file_handle;
#pragma warning( default : 4302 )
   m_FileName.assign( tape_drive_name );
   m_FileHandle = file_handle;
#endif // WFC_STL

   m_TapeDriveNumberStartingAtZero = tape_drive_number_starting_at_zero;

   (void) GetStatus();
   (void) Lock(); // This will fail, but is doesn't matter...
   (void) GetStatus();

   return( true );
}

_Check_return_ bool Win32FoundationClasses::CTape::Prepare( _In_ Operation const what_to_do, _In_ bool const return_immediately ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_LastError = ::PrepareTape( m_FileHandle, static_cast<DWORD>(what_to_do), return_immediately == true ? TRUE : FALSE );

   if ( m_LastError == NO_ERROR )
   {
      return( true );
   }
   else
   {
      return( false );
   }
}

_Check_return_ bool Win32FoundationClasses::CTape::BackupRead(__out_bcount(number_of_bytes_to_read) uint8_t * buffer, _In_ DWORD const number_of_bytes_to_read, _Out_ LPDWORD number_of_bytes_read, _In_ bool abort, _In_ bool restore_security_data ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      BOOL return_value = ::BackupRead( m_FileHandle,
                                        buffer,
                                        number_of_bytes_to_read,
                                        number_of_bytes_read,
                                        abort == true ? TRUE : FALSE,
                                        restore_security_data == true ? TRUE : FALSE,
                                       &m_BackupReadContextPointer );

      if ( return_value == FALSE )
      {
         m_LastError = ::GetLastError();
         //WFCTRACEERROR( m_LastError );
         return(false);
      }

      return( true );
   }
   WFC_CATCH_ALL
   {
      m_LastError = ERROR_EXCEPTION_IN_SERVICE;
      return( false );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CTape::Rewind( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( SetPosition(PositionMethod::RewindTape, 0, 0, 0, false ) );
}

_Check_return_ bool Win32FoundationClasses::CTape::BackupSeek( _In_ DWORD const seek_low, _In_ DWORD const seek_high, _Out_ LPDWORD seeked_low, _Out_ LPDWORD seeked_high ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      BOOL const return_value = ::BackupSeek( m_FileHandle, seek_low, seek_high, seeked_low, seeked_high, &m_BackupReadContextPointer );

      if ( return_value == FALSE )
      {
         m_LastError = ::GetLastError();
         //WFCTRACEERROR( m_LastError );
         return(false);
      }

      return( true );
   }
   WFC_CATCH_ALL
   {
      m_LastError = ERROR_EXCEPTION_IN_SERVICE;
      return( false );
   }
   WFC_END_CATCH_ALL
}

_Check_return_ bool Win32FoundationClasses::CTape::SetParameters( _In_ Win32FoundationClasses::CTapeSetMediaParameters const& parameters ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Win32FoundationClasses::CTapeSetMediaParameters media_parameters( parameters );

   m_LastError = ::SetTapeParameters( m_FileHandle, SET_TAPE_MEDIA_INFORMATION, static_cast<TAPE_SET_MEDIA_PARAMETERS *>(&media_parameters) );

   if ( m_LastError == NO_ERROR )
   {
      return( true );
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CTape::SetParameters( _In_ Win32FoundationClasses::CTapeSetDriveParameters const& parameters ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   Win32FoundationClasses::CTapeSetDriveParameters drive_parameters( parameters );

   m_LastError = ::SetTapeParameters( m_FileHandle, SET_TAPE_DRIVE_INFORMATION, static_cast<TAPE_SET_DRIVE_PARAMETERS *>(&drive_parameters) );

   if ( m_LastError == NO_ERROR )
   {
      return( true );
   }

   return( false );
}

_Check_return_ bool Win32FoundationClasses::CTape::SetPosition( _In_ PositionMethod const how_to_get_there, _In_ DWORD const partition_number, _In_ DWORD const position_low, _In_ DWORD const position_high, _In_ bool const return_immediately ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   m_LastError = ::SetTapePosition( m_FileHandle, static_cast<DWORD>(how_to_get_there), partition_number, position_low, position_high, return_immediately == true ? TRUE : FALSE );

   if ( m_LastError == NO_ERROR )
   {
      return( true );
   }
   else
   {
      return( false );
   }
}

_Check_return_ bool Win32FoundationClasses::CTape::Unload( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( Prepare( Operation::UnloadTape ) );
}

_Check_return_ bool Win32FoundationClasses::CTape::Unlock( void ) noexcept
{
   WFC_VALIDATE_POINTER( this );
   return( Prepare( Operation::UnlockTape ) );
}

_Check_return_ bool Win32FoundationClasses::CTape::BackupWrite(__in_bcount(number_of_bytes_to_write) LPBYTE buffer, _In_ DWORD const number_of_bytes_to_write, _Out_ LPDWORD number_of_bytes_written, _In_ bool abort, _In_ bool restore_security_data ) noexcept
{
   WFC_VALIDATE_POINTER( this );

   // We were passed a pointer, do not trust it

   WFC_TRY
   {
      BOOL const return_value = ::BackupWrite( m_FileHandle,
                                         buffer,
                                         number_of_bytes_to_write,
                                         number_of_bytes_written,
                                         abort == true ? TRUE : FALSE,
                                         restore_security_data == true ? TRUE : FALSE,
                                        &m_BackupWriteContextPointer );
      if ( return_value == FALSE )
      {
         m_LastError = ::GetLastError();
         //WFCTRACEERROR( m_LastError );
         return(false);
      }

      return( true );
   }
   WFC_CATCH_ALL
   {
      m_LastError = ERROR_EXCEPTION_IN_SERVICE;
      return( false );
   }
   WFC_END_CATCH_ALL
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CTape</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that encapsulates the Win32 tape backup API.">
</HEAD>

<BODY>

<H1>CTape : <A HREF="DumyFile.htm">CDummyFile</A></H1>
$Revision: 34 $ <BR>
<HR>

<H2>Description</H2>

This class wraps the Tape API of Win32.

<H2>Data Members</H2>

None.

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>BOOL <B><A NAME="BackupRead">BackupRead</A></B>( LPBYTE      buffer, 
           DWORD const number_of_bytes_to_read,
           LPDWORD     number_of_bytes_read,
           BOOL        abort = FALSE,
           BOOL        restore_security_data = TRUE )</PRE><DD>
Reads data from the tape.
If you want to read raw data from the tape, use the CFile::Read() method.

<DT><PRE>BOOL <B><A NAME="BackupSeek">BackupSeek</A></B>( DWORD const seek_low,
           DWORD const seek_high,
           LPDWORD seeked_low,
           LPDWORD seeked_high )</PRE><DD>
Repositions the tape for reading/writing.

<DT><PRE>BOOL <B><A NAME="BackupWrite">BackupWrite</A></B>( LPBYTE      buffer,
            DWORD const number_of_bytes_to_write,
            LPDWORD     number_of_bytes_written,
            BOOL        abort = FALSE,
            BOOL        restore_security_data = TRUE )</PRE><DD>
Writes data to the tape.
If you want to write raw data to the tape, use the CFile::Write() method.

<DT><PRE>void <B><A NAME="Close">Close</A></B>( void )</PRE><DD>
Closes the tape drive. Basically calls <B><A HREF="#Prepare">Prepare</A></B>( UnloadTape ).

<DT><PRE>BOOL <B><A NAME="CreatePartition">CreatePartition</A></B>( TypeOfPartition const type_of_partition,
                      DWORD const number_of_partitions,
                      DWORD const number_of_megabytes_in_each_partition )</PRE><DD>
Creates a tape partition. <CODE>type_of_partition</CODE> can be one of the following:

<UL>
<LI>Fixed<LI>Initiator<LI>Select
</UL>

<DT><PRE>BOOL <B><A NAME="Erase">Erase</A></B>( const TypeOfErasure type_of_erasure, const BOOL return_immediately = FALSE )</PRE><DD>
Erases the tape. <CODE>type_of_erasure</CODE> can be one of the following:
<UL>
<LI>Long<LI>Short
</UL>

<DT><PRE>DWORD <B><A NAME="GetDriveNumber">GetDriveNumber</A></B>( void ) const</PRE><DD>
Retrieves the drive number of an opened tape.

<DT><PRE>BOOL <B><A NAME="GetParameters">GetParameters</A></B>( CTapeGetDriveParameters&amp; parameters )
BOOL <B>GetParameters</B>( <A HREF="CTapeGetMediaParameters.htm">CTapeGetMediaParameters</A>&amp; parameters )</PRE><DD>
Retrieves either drive parameters or
media parameters.

<DT><PRE>BOOL <B><A NAME="GetPosition">GetPosition</A></B>( const Position type_of_position_to_get,
                  DWORD&amp; partition_number,
                  DWORD&amp; position_low,
                  DWORD&amp; position_high )</PRE><DD>
Retrieves the postion of the tape. <CODE>type_of_position_to_get</CODE>
can be one of the following:
<UL>
<LI>Absolute<LI>Logical
</UL>

<DT><PRE>DWORD <B><A NAME="GetStatus">GetStatus</A></B>( void ) const</PRE><DD>
Retrieves the status of the tape.

<DT><PRE>BOOL <B><A NAME="Load">Load</A></B>( void )</PRE><DD>
Loads a tape.

<DT><PRE>BOOL <B><A NAME="Lock">Lock</A></B>( void )</PRE><DD>
Locks the tape into place.

<DT><PRE>BOOL <B><A NAME="Mark">Mark</A></B>( TypeOfMark const type_of_mark, 
           DWORD const      number_of_marks_to_write, 
           BOOL const      return_immediately = FALSE )</PRE><DD>
Makes tape marks. <CODE>type_of_mark</CODE> can be one of the following:
<UL>
<LI>File<LI>LongFile<LI>Set<LI>ShortFile
</UL>

<DT><PRE>BOOL <B><A NAME="Open">Open</A></B>( const UINT tape_drive_number_starting_at_zero = 0 )</PRE><DD>
Opens the tape drive number you specify.

<DT><PRE>BOOL <B><A NAME="Prepare">Prepare</A></B>( const Operation what_to_do, const BOOL return_immediately = FALSE )</PRE><DD>
Prepares a tape for reading/writing. <CODE>what_to_do</CODE> can be one of the
following:

<UL>
<LI>FormatTape
<LI>LoadTape
<LI>LockTape
<LI>TensionTape
<LI>UnloadTape
<LI>UnlockTape
</UL>

<DT><PRE>BOOL <B><A NAME="SetAutomaticallyClose">SetAutomaticallyClose</A></B>( BOOL automatically_close )</PRE><DD>
When this is set to TRUE, the tape
drive will be closed when the destructor is called. It returns
the previous setting.

<DT><PRE>BOOL <B><A NAME="SetParameters">SetParameters</A></B>( const CTapeSetDriveParameters&amp; parameters )
BOOL <B>SetParameters</B>( cosnt CTapeSetMediaParameters&amp; parameters )</PRE><DD>
Sets either the drive or media parameters.

<DT><PRE>BOOL <B><A NAME="SetPosition">SetPosition</A></B>( PositionMethod const how_to_get_there, 
                  DWORD const         partition_number,
                  DWORD const         position_low,
                  DWORD const         position_high,
                  BOOL  const         return_immediately = FALSE )</PRE><DD>
Repositions the tape for reading/writing. <CODE>how_to_get_there</CODE>
can be one of the following:

<UL>
<LI>AbsoluteBlock
<LI>LogicalBlock
<LI>Rewind
<LI>EndOfData
<LI>FileMarks
<LI>RelativeBlocks
<LI>SequentialFileMarks
<LI>SequentialSetMarks
<LI>SetMarks
</UL>

<DT><PRE>BOOL <B><A NAME="Unload">Unload</A></B>( void )</PRE><DD>Ejects the tape.

<DT><PRE>BOOL <B><A NAME="Unlock">Unlock</A></B>( void )</PRE><DD>Unlocks the locking mechanism.

</DL>

<H2>Example</H2>

<PRE><CODE>Sorry.</CODE></PRE>

<H2>API's Used</H2>

<B>CTape</B> uses the following API's:
<UL>
<LI>BackupRead
<LI>BackupSeek
<LI>BackupWrite
<LI>CreateFile
<LI>CreateTapePartition
<LI>EraseTape
<LI>GetTapeParameters
<LI>GetTapePosition
<LI>GetTapeStatus
<LI>PrepareTape
<LI>SetTapeParameters
<LI>SetTapePosition
<LI>WriteTapemark
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: ctape.cpp $<BR>
$Modtime: 6/26/01 10:51a $
</BODY>

</HTML>
The following line should go in AUTOEXP.DAT so the debugging tooltips will format properly
ToolTipFormatLine=CTape=hFile=<m_hFile> name=<m_strFileName.m_pchData,s>
*/
