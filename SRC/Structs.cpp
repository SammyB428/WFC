/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2020, Samuel R. Blackburn
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
** $Workfile: structs.cpp $
** $Revision: 22 $
** $Modtime: 6/26/01 10:57a $
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

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CAccessAllowedEntry::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CAccessAllowedEntry at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   Header.AceType  = " ) << Header.AceType  << TEXT( " (" );

   switch( Header.AceType )
   {
      case ACCESS_ALLOWED_ACE_TYPE:

         dump_context << TEXT( "ACCESS_ALLOWED_ACE_TYPE" );
         break;

      case ACCESS_DENIED_ACE_TYPE:

         dump_context << TEXT( "ACCESS_DENIED_ACE_TYPE" );
         break;

      case SYSTEM_AUDIT_ACE_TYPE:

         dump_context << TEXT( "SYSTEM_AUDIT_ACE_TYPE" );
         break;

      case SYSTEM_ALARM_ACE_TYPE:

         dump_context << TEXT( "SYSTEM_ALARM_ACE_TYPE" );
         break;
   }

   dump_context << TEXT( ")\n" );

   dump_context << TEXT( "   Header.AceFlags = " ) << Header.AceFlags << TEXT( " (" );

   std::wstring temp_string;

   if ( Header.AceFlags bitand CONTAINER_INHERIT_ACE )
   {
      temp_string = TEXT( "CONTAINER_INHERIT_ACE" );
   }

   if ( Header.AceFlags bitand INHERIT_ONLY_ACE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "INHERIT_ONLY_ACE" );
   }

   if ( Header.AceFlags bitand NO_PROPAGATE_INHERIT_ACE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "NO_PROPAGATE_INHERIT_ACE" );
   }

   if ( Header.AceFlags bitand OBJECT_INHERIT_ACE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "OBJECT_INHERIT_ACE" );
   }

   if ( Header.AceFlags bitand FAILED_ACCESS_ACE_FLAG )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "FAILED_ACCESS_ACE_FLAG" );
   }

   if ( Header.AceFlags bitand SUCCESSFUL_ACCESS_ACE_FLAG )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "SUCCESSFUL_ACCESS_ACE_FLAG" );
   }

   dump_context << temp_string << TEXT( ")\n" );

   dump_context << TEXT( "   Header.AceSize  = " ) << Header.AceSize  << TEXT( "\n" );
   dump_context << TEXT( "   Mask            = " ) << Mask            << TEXT( "\n" );
   dump_context << TEXT( "   SidStart        = " ) << SidStart        << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

Win32FoundationClasses::CAccessControlEntryHeader::CAccessControlEntryHeader() noexcept
{
   Empty();
}

Win32FoundationClasses::CAccessControlEntryHeader::CAccessControlEntryHeader( _In_ Win32FoundationClasses::CAccessControlEntryHeader const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::CAccessControlEntryHeader::CAccessControlEntryHeader( _In_ ACE_HEADER const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::CAccessControlEntryHeader::Copy(_In_ Win32FoundationClasses::CAccessControlEntryHeader const& source ) noexcept
{
   Copy( static_cast<ACE_HEADER const *>(&source) );
}

void Win32FoundationClasses::CAccessControlEntryHeader::Copy(_In_ ACE_HEADER const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      AceType  = source->AceType;
      AceFlags = source->AceFlags;
      AceSize  = source->AceSize;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CAccessControlEntryHeader::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CAccessControlEntryHeader at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   AceType  = " ) << AceType  << TEXT( " (" );

   switch( AceType )
   {
      case ACCESS_ALLOWED_ACE_TYPE:

         dump_context << TEXT( "ACCESS_ALLOWED_ACE_TYPE" );
         break;

      case ACCESS_DENIED_ACE_TYPE:

         dump_context << TEXT( "ACCESS_DENIED_ACE_TYPE" );
         break;

      case SYSTEM_AUDIT_ACE_TYPE:

         dump_context << TEXT( "SYSTEM_AUDIT_ACE_TYPE" );
         break;

      case SYSTEM_ALARM_ACE_TYPE:

         dump_context << TEXT( "SYSTEM_ALARM_ACE_TYPE" );
         break;
   }

   dump_context << TEXT( ")\n" );

   dump_context << TEXT( "   AceFlags = " ) << AceFlags << TEXT( " (" );

   std::wstring temp_string;

   if ( AceFlags bitand CONTAINER_INHERIT_ACE )
   {
      temp_string = TEXT( "CONTAINER_INHERIT_ACE" );
   }

   if ( AceFlags bitand INHERIT_ONLY_ACE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "INHERIT_ONLY_ACE" );
   }

   if ( AceFlags bitand NO_PROPAGATE_INHERIT_ACE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "NO_PROPAGATE_INHERIT_ACE" );
   }

   if ( AceFlags bitand OBJECT_INHERIT_ACE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "OBJECT_INHERIT_ACE" );
   }

   if ( AceFlags bitand FAILED_ACCESS_ACE_FLAG )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "FAILED_ACCESS_ACE_FLAG" );
   }

   if ( AceFlags bitand SUCCESSFUL_ACCESS_ACE_FLAG )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "SUCCESSFUL_ACCESS_ACE_FLAG" );
   }

   dump_context << temp_string << TEXT( ")\n" );

   dump_context << TEXT( "   AceSize  = " ) << AceSize     << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CAccessControlEntryHeader::Empty( void ) noexcept
{
   AceType  = 0;
   AceFlags = 0;
   AceSize  = 0;
}

_Check_return_ Win32FoundationClasses::CAccessControlEntryHeader& Win32FoundationClasses::CAccessControlEntryHeader::operator=( _In_ Win32FoundationClasses::CAccessControlEntryHeader const& source ) noexcept
{
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CAccessControlList::CAccessControlList() noexcept
{
   Empty();
}

Win32FoundationClasses::CAccessControlList::CAccessControlList( _In_ Win32FoundationClasses::CAccessControlList const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::CAccessControlList::CAccessControlList( _In_ ACL const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::CAccessControlList::Copy(_In_ Win32FoundationClasses::CAccessControlList const& source ) noexcept
{
   Copy( static_cast<ACL const *>(&source) );
}

void Win32FoundationClasses::CAccessControlList::Copy(_In_ ACL const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      AclRevision = source->AclRevision;
      Sbz1        = source->Sbz1;
      AclSize     = source->AclSize;
      AceCount    = source->AceCount;
      Sbz2        = source->Sbz1;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CAccessControlList::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CAccessControlList at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   AclRevision = " ) << AclRevision << TEXT( "\n" );
   dump_context << TEXT( "   Sbz1        = " ) << Sbz1        << TEXT( "\n" );
   dump_context << TEXT( "   AclSize     = " ) << AclSize     << TEXT( "\n" );
   dump_context << TEXT( "   AceCount    = " ) << AceCount    << TEXT( "\n" );
   dump_context << TEXT( "   Sbz2        = " ) << Sbz2        << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CAccessControlList::Empty( void ) noexcept
{
   AclRevision = ACL_REVISION;
   Sbz1        = 0;
   AclSize     = 0;
   AceCount    = 0;
   Sbz2        = 0;
}

_Check_return_ Win32FoundationClasses::CAccessControlList& Win32FoundationClasses::CAccessControlList::operator=( _In_ Win32FoundationClasses::CAccessControlList const& source ) noexcept
{
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CAccessDeniedEntry::CAccessDeniedEntry() noexcept
{
   Empty();
}

Win32FoundationClasses::CAccessDeniedEntry::CAccessDeniedEntry( _In_ Win32FoundationClasses::CAccessDeniedEntry const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::CAccessDeniedEntry::CAccessDeniedEntry( _In_ ACCESS_DENIED_ACE const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::CAccessDeniedEntry::Copy(_In_ Win32FoundationClasses::CAccessDeniedEntry const& source ) noexcept
{
   Copy( static_cast<ACCESS_DENIED_ACE const*>(&source) );
}

void Win32FoundationClasses::CAccessDeniedEntry::Copy(_In_ ACCESS_DENIED_ACE const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      Header.AceType  = source->Header.AceType;
      Header.AceFlags = source->Header.AceFlags;
      Header.AceSize  = source->Header.AceSize;
      Mask            = source->Mask;
      SidStart        = source->SidStart;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CAccessDeniedEntry::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CAccessDeniedEntry at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   Header.AceType  = " ) << Header.AceType  << TEXT( " (" );

   switch( Header.AceType )
   {
      case ACCESS_ALLOWED_ACE_TYPE:

         dump_context << TEXT( "ACCESS_ALLOWED_ACE_TYPE" );
         break;

      case ACCESS_DENIED_ACE_TYPE:

         dump_context << TEXT( "ACCESS_DENIED_ACE_TYPE" );
         break;

      case SYSTEM_AUDIT_ACE_TYPE:

         dump_context << TEXT( "SYSTEM_AUDIT_ACE_TYPE" );
         break;

      case SYSTEM_ALARM_ACE_TYPE:

         dump_context << TEXT( "SYSTEM_ALARM_ACE_TYPE" );
         break;
   }

   dump_context << TEXT( ")\n" );

   dump_context << TEXT( "   Header.AceFlags = " ) << Header.AceFlags << TEXT( " (" );

   std::wstring temp_string;

   if ( Header.AceFlags bitand CONTAINER_INHERIT_ACE )
   {
      temp_string = TEXT( "CONTAINER_INHERIT_ACE" );
   }

   if ( Header.AceFlags bitand INHERIT_ONLY_ACE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "INHERIT_ONLY_ACE" );
   }

   if ( Header.AceFlags bitand NO_PROPAGATE_INHERIT_ACE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "NO_PROPAGATE_INHERIT_ACE" );
   }

   if ( Header.AceFlags bitand OBJECT_INHERIT_ACE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "OBJECT_INHERIT_ACE" );
   }

   if ( Header.AceFlags bitand FAILED_ACCESS_ACE_FLAG )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "FAILED_ACCESS_ACE_FLAG" );
   }

   if ( Header.AceFlags bitand SUCCESSFUL_ACCESS_ACE_FLAG )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "SUCCESSFUL_ACCESS_ACE_FLAG" );
   }

   dump_context << temp_string << TEXT( ")\n" );

   dump_context << TEXT( "   Header.AceSize  = " ) << Header.AceSize  << TEXT( "\n" );
   dump_context << TEXT( "   Mask            = " ) << Mask            << TEXT( "\n" );
   dump_context << TEXT( "   SidStart        = " ) << SidStart        << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CAccessDeniedEntry::Empty( void ) noexcept
{
   // ACE_HEADER
   Header.AceType  = 0;
   Header.AceFlags = 0;
   Header.AceSize  = 0;

   // ACE_MASK
   Mask            = 0;
   SidStart        = 0;
}

_Check_return_ Win32FoundationClasses::CAccessDeniedEntry& Win32FoundationClasses::CAccessDeniedEntry::operator=( _In_ Win32FoundationClasses::CAccessDeniedEntry const& source ) noexcept
{
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CBitmapCoreHeader::CBitmapCoreHeader() noexcept
{
   Empty();
}

Win32FoundationClasses::CBitmapCoreHeader::CBitmapCoreHeader( _In_ Win32FoundationClasses::CBitmapCoreHeader const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::CBitmapCoreHeader::CBitmapCoreHeader( _In_ tagBITMAPCOREHEADER const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::CBitmapCoreHeader::Copy( _In_ Win32FoundationClasses::CBitmapCoreHeader const& source ) noexcept
{
   Copy( reinterpret_cast<CBitmapCoreHeader const *>(&source) );
}

void Win32FoundationClasses::CBitmapCoreHeader::Copy( _In_ tagBITMAPCOREHEADER const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      bcSize     = source->bcSize;
      bcWidth    = source->bcWidth;
      bcHeight   = source->bcHeight;
      bcPlanes   = source->bcPlanes;
      bcBitCount = source->bcBitCount;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CBitmapCoreHeader::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CBitmapCoreHeader at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   bcSize     = " ) << bcSize     << TEXT( "\n" );
   dump_context << TEXT( "   bcWidth    = " ) << bcWidth    << TEXT( "\n" );
   dump_context << TEXT( "   bcHeight   = " ) << bcHeight   << TEXT( "\n" );
   dump_context << TEXT( "   bcPlanes   = " ) << bcPlanes   << TEXT( "\n" );
   dump_context << TEXT( "   bcBitCount = " ) << bcBitCount << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CBitmapCoreHeader::Empty( void ) noexcept
{
   bcSize     = sizeof( tagBITMAPCOREHEADER );
   bcWidth    = 0;
   bcHeight   = 0;
   bcPlanes   = 0;
   bcBitCount = 0;
}

_Check_return_ Win32FoundationClasses::CBitmapCoreHeader& Win32FoundationClasses::CBitmapCoreHeader::operator=( _In_ Win32FoundationClasses::CBitmapCoreHeader const& source ) noexcept
{
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CBitmapFileHeader::CBitmapFileHeader() noexcept
{
   Empty();
}

Win32FoundationClasses::CBitmapFileHeader::CBitmapFileHeader( _In_ Win32FoundationClasses::CBitmapFileHeader const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::CBitmapFileHeader::CBitmapFileHeader( _In_ tagBITMAPFILEHEADER const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::CBitmapFileHeader::Copy(_In_ Win32FoundationClasses::CBitmapFileHeader const& source ) noexcept
{
   Copy( static_cast<tagBITMAPFILEHEADER const *>(&source) );
}

void Win32FoundationClasses::CBitmapFileHeader::Copy(_In_ tagBITMAPFILEHEADER const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      bfType      = source->bfType;
      bfSize      = source->bfSize;
      bfReserved1 = source->bfReserved1;
      bfReserved2 = source->bfReserved2;
      bfOffBits   = source->bfOffBits;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CBitmapFileHeader::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CBitmapFileHeader at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   bfType      = " ) << bfType      << TEXT( "\n" );
   dump_context << TEXT( "   bfSize      = " ) << bfSize      << TEXT( "\n" );
   dump_context << TEXT( "   bfReserved1 = " ) << bfReserved1 << TEXT( "\n" );
   dump_context << TEXT( "   bfReserved2 = " ) << bfReserved2 << TEXT( "\n" );
   dump_context << TEXT( "   bfOffBits   = " ) << bfOffBits   << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CBitmapFileHeader::Empty( void ) noexcept
{
   // 2000-11-06
   // Thanks go to Danny Smith for finding a bug in GCC here

   bfType      = 0x4D42; // Visual C++ initialized 'BM' to 0x4D42, GCC consideres 'BM' to be equal to 0x424D
   bfSize      = 0;
   bfReserved1 = 0;
   bfReserved2 = 0;
   bfOffBits   = 0;
}

_Check_return_ Win32FoundationClasses::CBitmapFileHeader& Win32FoundationClasses::CBitmapFileHeader::operator=( _In_ Win32FoundationClasses::CBitmapFileHeader const& source ) noexcept
{
   Copy( source );
   return( *this );
}

/*
** CBitmapInfoHeader
*/

Win32FoundationClasses::CBitmapInfoHeader::CBitmapInfoHeader() noexcept
{
   Empty();
}

Win32FoundationClasses::CBitmapInfoHeader::CBitmapInfoHeader( _In_ Win32FoundationClasses::CBitmapInfoHeader const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::CBitmapInfoHeader::CBitmapInfoHeader( _In_ tagBITMAPINFOHEADER const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::CBitmapInfoHeader::Copy( _In_ Win32FoundationClasses::CBitmapInfoHeader const& source ) noexcept
{
   Copy( static_cast<tagBITMAPINFOHEADER const *>(&source) );
}

void Win32FoundationClasses::CBitmapInfoHeader::Copy( _In_ tagBITMAPINFOHEADER const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      biSize          = source->biSize;
      biWidth         = source->biWidth;
      biHeight        = source->biHeight;
      biPlanes        = source->biPlanes;
      biBitCount      = source->biBitCount;
      biCompression   = source->biCompression;
      biSizeImage     = source->biSizeImage;
      biXPelsPerMeter = source->biXPelsPerMeter;
      biYPelsPerMeter = source->biYPelsPerMeter;
      biClrUsed       = source->biClrUsed;
      biClrImportant  = source->biClrImportant;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CBitmapInfoHeader::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CBitmapInfoHeader at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   biSize          = " ) << biSize          << TEXT( "\n" );
   dump_context << TEXT( "   biWidth         = " ) << biWidth         << TEXT( "\n" );
   dump_context << TEXT( "   biHeight        = " ) << biHeight        << TEXT( "\n" );
   dump_context << TEXT( "   biPlanes        = " ) << biPlanes        << TEXT( "\n" );
   dump_context << TEXT( "   biBitCount      = " ) << biBitCount      << TEXT( "\n" );
   dump_context << TEXT( "   biCompression   = " ) << biCompression   << TEXT( "\n" );
   dump_context << TEXT( "   biSizeImage     = " ) << biSizeImage     << TEXT( "\n" );
   dump_context << TEXT( "   biXPelsPerMeter = " ) << biXPelsPerMeter << TEXT( "\n" );
   dump_context << TEXT( "   biYPelsPerMeter = " ) << biYPelsPerMeter << TEXT( "\n" );
   dump_context << TEXT( "   biClrUsed       = " ) << biClrUsed       << TEXT( "\n" );
   dump_context << TEXT( "   biClrImportant  = " ) << biClrImportant  << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CBitmapInfoHeader::Empty( void ) noexcept
{
   biSize          = sizeof( tagBITMAPINFOHEADER );
   biWidth         = 0;
   biHeight        = 0;
   biPlanes        = 0;
   biBitCount      = 0;
   biCompression   = 0;
   biSizeImage     = 0;
   biXPelsPerMeter = 0;
   biYPelsPerMeter = 0;
   biClrUsed       = 0;
   biClrImportant  = 0;
}

_Check_return_ Win32FoundationClasses::CBitmapInfoHeader& Win32FoundationClasses::CBitmapInfoHeader::operator=( _In_ Win32FoundationClasses::CBitmapInfoHeader const& source ) noexcept
{
   Copy( source );
   return( *this );
}

/*
** CColorAdjustment
*/

Win32FoundationClasses::CColorAdjustment::CColorAdjustment() noexcept
{
   Empty();
}

Win32FoundationClasses::CColorAdjustment::CColorAdjustment( _In_ Win32FoundationClasses::CColorAdjustment const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::CColorAdjustment::CColorAdjustment( _In_ tagCOLORADJUSTMENT const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::CColorAdjustment::Copy( _In_ Win32FoundationClasses::CColorAdjustment const& source ) noexcept
{
   Copy( static_cast<tagCOLORADJUSTMENT const *>(&source) );
}

void Win32FoundationClasses::CColorAdjustment::Copy(_In_ tagCOLORADJUSTMENT const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      caSize            = source->caSize;
      caFlags           = source->caFlags;
      caIlluminantIndex = source->caIlluminantIndex;
      caRedGamma        = source->caRedGamma;
      caGreenGamma      = source->caGreenGamma;
      caBlueGamma       = source->caBlueGamma;
      caReferenceBlack  = source->caReferenceBlack;
      caReferenceWhite  = source->caReferenceWhite;
      caContrast        = source->caContrast;
      caBrightness      = source->caBrightness;
      caColorfulness    = source->caColorfulness;
      caRedGreenTint    = source->caRedGreenTint;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CColorAdjustment::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CColorAdjustment at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   caSize =            " ) << caSize             << TEXT( "\n" );
   dump_context << TEXT( "   caFlags =           " ) << caFlags            << TEXT( "\n" );
   dump_context << TEXT( "   caIlluminantIndex = " ) << caIlluminantIndex  << TEXT( "\n" );
   dump_context << TEXT( "   caRedGamma =        " ) << caRedGamma         << TEXT( "\n" );
   dump_context << TEXT( "   caGreenGamma =      " ) << caGreenGamma       << TEXT( "\n" );
   dump_context << TEXT( "   caBlueGamma =       " ) << caBlueGamma        << TEXT( "\n" );
   dump_context << TEXT( "   caReferenceBlack =  " ) << caReferenceBlack   << TEXT( "\n" );
   dump_context << TEXT( "   caReferenceWhite =  " ) << caReferenceWhite   << TEXT( "\n" );
   dump_context << TEXT( "   caContrast =        " ) << caContrast         << TEXT( "\n" );
   dump_context << TEXT( "   caBrightness =      " ) << caBrightness       << TEXT( "\n" );
   dump_context << TEXT( "   caColorfulness =    " ) << caColorfulness     << TEXT( "\n" );
   dump_context << TEXT( "   caRedGreenTint =    " ) << caRedGreenTint     << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CColorAdjustment::Empty( void ) noexcept
{
   caSize            = sizeof( tagCOLORADJUSTMENT );
   caFlags           = 0;
   caIlluminantIndex = 0;
   caRedGamma        = 0;
   caGreenGamma      = 0;
   caBlueGamma       = 0;
   caReferenceBlack  = 0;
   caReferenceWhite  = 0;
   caContrast        = 0;
   caBrightness      = 0;
   caColorfulness    = 0;
   caRedGreenTint    = 0;
}

_Check_return_ Win32FoundationClasses::CColorAdjustment& Win32FoundationClasses::CColorAdjustment::operator=( _In_ Win32FoundationClasses::CColorAdjustment const& source ) noexcept
{
   Copy( source );
   return( *this );
}

/*
** CCommunicationsConfiguration
*/

Win32FoundationClasses::CCommunicationsConfiguration::CCommunicationsConfiguration() noexcept
{
   Empty();
   dwSize        = sizeof( COMMCONFIG );
   dcb.DCBlength = sizeof( DCB );
}

Win32FoundationClasses::CCommunicationsConfiguration::CCommunicationsConfiguration( _In_ Win32FoundationClasses::CCommunicationsConfiguration const& source ) noexcept
{
   Empty();
   dwSize        = sizeof( COMMCONFIG );
   dcb.DCBlength = sizeof( DCB );
   Copy( source );
}

Win32FoundationClasses::CCommunicationsConfiguration::CCommunicationsConfiguration( _In_ COMMCONFIG const& source ) noexcept
{
   Empty();
   dwSize        = sizeof( COMMCONFIG );
   dcb.DCBlength = sizeof( DCB );
   Copy( source );
}

void Win32FoundationClasses::CCommunicationsConfiguration::Copy( _In_ Win32FoundationClasses::CCommunicationsConfiguration const& source ) noexcept
{
   ASSERT( &source not_eq this );

   if ( &source == this )
   {
      return;
   }

   dwSize    = source.dwSize;
   wVersion  = source.wVersion;
   wReserved = source.wReserved;

   // The device control block

   dcb.DCBlength         = source.dcb.DCBlength;
   dcb.BaudRate          = source.dcb.BaudRate;
   dcb.fBinary           = source.dcb.fBinary;
   dcb.fParity           = source.dcb.fParity;
   dcb.fOutxCtsFlow      = source.dcb.fOutxCtsFlow;
   dcb.fOutxDsrFlow      = source.dcb.fOutxDsrFlow;
   dcb.fDtrControl       = source.dcb.fDtrControl;
   dcb.fDsrSensitivity   = source.dcb.fDsrSensitivity;
   dcb.fTXContinueOnXoff = source.dcb.fTXContinueOnXoff;
   dcb.fOutX             = source.dcb.fOutX;
   dcb.fInX              = source.dcb.fInX;
   dcb.fErrorChar        = source.dcb.fErrorChar;
   dcb.fNull             = source.dcb.fNull;
   dcb.fRtsControl       = source.dcb.fRtsControl;
   dcb.fAbortOnError     = source.dcb.fAbortOnError;
   dcb.fDummy2           = source.dcb.fDummy2;
   dcb.wReserved         = source.dcb.wReserved;
   dcb.XonLim            = source.dcb.XonLim;
   dcb.XoffLim           = source.dcb.XoffLim;
   dcb.ByteSize          = source.dcb.ByteSize;
   dcb.Parity            = source.dcb.Parity;
   dcb.StopBits          = source.dcb.StopBits;
   dcb.XonChar           = source.dcb.XonChar;
   dcb.XoffChar          = source.dcb.XoffChar;
   dcb.ErrorChar         = source.dcb.ErrorChar;
   dcb.EofChar           = source.dcb.EofChar;
   dcb.EvtChar           = source.dcb.EvtChar;
   dcb.wReserved1        = source.dcb.wReserved1;

   dwProviderSubType = source.dwProviderSubType;
   dwProviderOffset  = source.dwProviderOffset;
   dwProviderSize    = source.dwProviderSize;
}

void Win32FoundationClasses::CCommunicationsConfiguration::Copy( _In_ COMMCONFIG const& source ) noexcept
{
   dwSize    = source.dwSize;
   wVersion  = source.wVersion;
   wReserved = source.wReserved;

   // The device control block

   dcb.DCBlength         = source.dcb.DCBlength;
   dcb.BaudRate          = source.dcb.BaudRate;
   dcb.fBinary           = source.dcb.fBinary;
   dcb.fParity           = source.dcb.fParity;
   dcb.fOutxCtsFlow      = source.dcb.fOutxCtsFlow;
   dcb.fOutxDsrFlow      = source.dcb.fOutxDsrFlow;
   dcb.fDtrControl       = source.dcb.fDtrControl;
   dcb.fDsrSensitivity   = source.dcb.fDsrSensitivity;
   dcb.fTXContinueOnXoff = source.dcb.fTXContinueOnXoff;
   dcb.fOutX             = source.dcb.fOutX;
   dcb.fInX              = source.dcb.fInX;
   dcb.fErrorChar        = source.dcb.fErrorChar;
   dcb.fNull             = source.dcb.fNull;
   dcb.fRtsControl       = source.dcb.fRtsControl;
   dcb.fAbortOnError     = source.dcb.fAbortOnError;
   dcb.fDummy2           = source.dcb.fDummy2;
   dcb.wReserved         = source.dcb.wReserved;
   dcb.XonLim            = source.dcb.XonLim;
   dcb.XoffLim           = source.dcb.XoffLim;
   dcb.ByteSize          = source.dcb.ByteSize;
   dcb.Parity            = source.dcb.Parity;
   dcb.StopBits          = source.dcb.StopBits;
   dcb.XonChar           = source.dcb.XonChar;
   dcb.XoffChar          = source.dcb.XoffChar;
   dcb.ErrorChar         = source.dcb.ErrorChar;
   dcb.EofChar           = source.dcb.EofChar;
   dcb.EvtChar           = source.dcb.EvtChar;
   dcb.wReserved1        = source.dcb.wReserved1;

   dwProviderSubType = source.dwProviderSubType;
   dwProviderOffset  = source.dwProviderOffset;
   dwProviderSize    = source.dwProviderSize;
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CCommunicationsConfiguration::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( "a CCommunicationsConfiguration at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   dwSize is            " ) << dwSize            << TEXT( "\n" );
   dump_context << TEXT( "   wVersion is          " ) << wVersion          << TEXT( "\n" );
   dump_context << TEXT( "   wReserved is         " ) << wReserved         << TEXT( "\n" );
   dump_context << TEXT( "   dwProviderSubType is " ) << dwProviderSubType << TEXT( " " );

   switch( dwProviderSubType )
   {
      case PST_FAX:

         dump_context << TEXT( "(PST_FAX)\n" );
         break;

      case PST_LAT:

         dump_context << TEXT( "(PST_LAT)\n" );
         break;

      case PST_MODEM:

         dump_context << TEXT( "(PST_MODEM)\n" );
         break;

      case PST_NETWORK_BRIDGE:

         dump_context << TEXT( "(PST_NETWORK_BRIDGE)\n" );
         break;

      case PST_PARALLELPORT:

         dump_context << TEXT( "(PST_PARALLELPORT)\n" );
         break;

      case PST_RS232:

         dump_context << TEXT( "(PST_RS232)\n" );
         break;

      case PST_RS422:

         dump_context << TEXT( "(PST_RS422)\n" );
         break;

      case PST_RS423:

         dump_context << TEXT( "(PST_RS423)\n" );
         break;

      case PST_RS449:

         dump_context << TEXT( "(PST_RS449)\n" );
         break;

      case PST_SCANNER:

         dump_context << TEXT( "(PST_SCANNER)\n" );
         break;

      case PST_TCPIP_TELNET:

         dump_context << TEXT( "(PST_TCPIP_TELNET)\n" );
         break;

      case PST_UNSPECIFIED:

         dump_context << TEXT( "(PST_UNSPECIFIED)\n" );
         break;

      case PST_X25:

         dump_context << TEXT( "(PST_X25)\n" );
         break;

      default:

          std::wstring debug_string;

         debug_string.Format( TEXT( "(Unknown value %lu)\n" ), dwProviderSubType );
         dump_context << debug_string;
         break;
   }

   dump_context << TEXT( "   dwProviderOffset is  " ) << dwProviderOffset  << TEXT( "\n" );
   dump_context << TEXT( "   dwProviderSize is    " ) << dwProviderSize    << TEXT( "\n" );

   CDeviceControlBlock device_control_block( dcb );

   dump_context << TEXT( "   dcb is " );

   device_control_block.Dump( dump_context );

   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CCommunicationsConfiguration::Empty( void ) noexcept
{
   dwSize              = 0;
   wVersion            = 0;
   wReserved           = 0;
   dwProviderSubType   = 0;
   dwProviderOffset    = 0;
   dwProviderSize      = 0;
   wcProviderData[ 0 ] = 0;
   ZeroMemory( &dcb, sizeof( dcb ) );
}

_Check_return_ Win32FoundationClasses::CCommunicationsConfiguration& Win32FoundationClasses::CCommunicationsConfiguration::operator=( _In_ Win32FoundationClasses::CCommunicationsConfiguration const& source ) noexcept
{
   Copy( source );
   return( *this );
}

_Check_return_ Win32FoundationClasses::CCommunicationsConfiguration& Win32FoundationClasses::CCommunicationsConfiguration::operator=( _In_ COMMCONFIG const& source ) noexcept
{
   Copy( source );
   return( *this );
}

/*
** CCommunicationProperties
*/

Win32FoundationClasses::CCommunicationProperties::CCommunicationProperties() noexcept
{
   Empty();
}

Win32FoundationClasses::CCommunicationProperties::CCommunicationProperties( _In_ Win32FoundationClasses::CCommunicationProperties const& source ) noexcept
{
   Copy( source );
}

Win32FoundationClasses::CCommunicationProperties::CCommunicationProperties( _In_ _COMMPROP const& source ) noexcept
{
   Copy( source );
}

void Win32FoundationClasses::CCommunicationProperties::ConvertBaudRateToString( _In_ DWORD const baud_rate, _Out_ std::wstring& human_readable_string ) const noexcept
{
   switch( baud_rate )
   {
      case BAUD_075:

         human_readable_string.assign(WSTRING_VIEW(L"75 bps"));
         break;

      case BAUD_110:

         human_readable_string.assign(WSTRING_VIEW(L"110 bps" ) );
         break;

      case BAUD_134_5:

         human_readable_string.assign(WSTRING_VIEW(L"134.5 bps" ) );
         break;

      case BAUD_150:

         human_readable_string.assign(WSTRING_VIEW(L"150 bps" ) );
         break;

      case BAUD_300:

         human_readable_string.assign(WSTRING_VIEW(L"300 bps" ) );
         break;

      case BAUD_600:

         human_readable_string.assign(WSTRING_VIEW(L"600 bps" ) );
         break;

      case BAUD_1200:

         human_readable_string.assign(WSTRING_VIEW(L"1200 bps" ) );
         break;

      case BAUD_1800:

         human_readable_string.assign(WSTRING_VIEW(L"1800 bps" ) );
         break;

      case BAUD_2400:

         human_readable_string.assign(WSTRING_VIEW(L"2400 bps" ) );
         break;

      case BAUD_4800:

         human_readable_string.assign(WSTRING_VIEW(L"4800 bps" ) );
         break;

      case BAUD_7200:

         human_readable_string.assign(WSTRING_VIEW(L"7200 bps" ) );
         break;

      case BAUD_9600:

         human_readable_string.assign(WSTRING_VIEW(L"9600 bps" ) );
         break;

      case BAUD_14400:

         human_readable_string.assign(WSTRING_VIEW(L"14400 bps" ) );
         break;

      case BAUD_19200:

         human_readable_string.assign(WSTRING_VIEW(L"19200 bps" ) );
         break;

      case BAUD_38400:

         human_readable_string.assign(WSTRING_VIEW(L"38400 bps" ) );
         break;

      case BAUD_56K:

         human_readable_string.assign(WSTRING_VIEW(L"56 Kbps" ) );
         break;

      case BAUD_128K:

         human_readable_string.assign(WSTRING_VIEW(L"128 Kbps" ) );
         break;

      case BAUD_115200:

         human_readable_string.assign(WSTRING_VIEW(L"115200 bps" ) );
         break;

      case BAUD_57600:

         human_readable_string.assign(WSTRING_VIEW(L"57600 bps" ) );
         break;

      case BAUD_USER:

         human_readable_string.assign(WSTRING_VIEW(L"Programmable baud rates available" ) );
         break;

      default:

         human_readable_string.assign(WSTRING_VIEW(L"Unknown constant "));
         human_readable_string.append(std::to_wstring(baud_rate));
         break;
   }
}

void Win32FoundationClasses::CCommunicationProperties::ConvertBaudRateListToString( _In_ DWORD const baud_rate, _Out_ std::wstring& human_readable_string ) const noexcept
{
   human_readable_string.clear();

   if (is_flagged(baud_rate, BAUD_075) == true)
   {
      human_readable_string.assign(WSTRING_VIEW(L"75 bps"));
   }

   if (is_flagged(baud_rate, BAUD_110) == true )
   {
      if ( human_readable_string.empty() == false )
      {
         human_readable_string.append(WSTRING_VIEW(L", ") );
      }

      human_readable_string.append(WSTRING_VIEW(L"110 bps") );
   }

   if (is_flagged(baud_rate, BAUD_134_5) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"134.5 bps" ) );
   }

   if (is_flagged(baud_rate, BAUD_150) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"150 bps" ) );
   }

   if (is_flagged(baud_rate, BAUD_300) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"300 bps" ) );
   }

   if (is_flagged(baud_rate, BAUD_600) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"600 bps" ) );
   }

   if (is_flagged(baud_rate, BAUD_1200) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"1200 bps" ) );
   }

   if (is_flagged(baud_rate, BAUD_1800) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"1800 bps" ) );
   }

   if (is_flagged(baud_rate, BAUD_2400) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"2400 bps" ) );
   }

   if (is_flagged(baud_rate,BAUD_4800) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"4800 bps" ) );
   }

   if (is_flagged(baud_rate,BAUD_7200) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"7200 bps" ) );
   }

   if (is_flagged(baud_rate, BAUD_9600) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

       human_readable_string.append(WSTRING_VIEW(L"9600 bps" ) );
   }

   if (is_flagged(baud_rate, BAUD_14400) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"14400 bps" ) );
   }

   if (is_flagged(baud_rate, BAUD_19200) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"19200 bps" ) );
   }

   if (is_flagged(baud_rate, BAUD_38400) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"38400 bps" ) );
   }

   if (is_flagged(baud_rate, BAUD_56K) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"56 Kbps" ) );
   }

   if (is_flagged(baud_rate, BAUD_128K) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"128 Kbps" ) );
   }

   if (is_flagged(baud_rate, BAUD_115200) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"115200 bps" ) );
   }

   if (is_flagged(baud_rate, BAUD_57600) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"57600 bps" ) );
   }

   if (is_flagged(baud_rate, BAUD_USER) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Programmable baud rates available"));
   }
}

void Win32FoundationClasses::CCommunicationProperties::ConvertDataBitsToString( _In_ DWORD const data_bits, _Out_ std::wstring& human_readable_string ) const noexcept
{
   human_readable_string.clear();

   if (is_flagged(data_bits, DATABITS_5) == true)
   {
      human_readable_string.assign(WSTRING_VIEW(L"5 data bits" ) );
   }

   if (is_flagged(data_bits, DATABITS_6) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"6 data bits" ) );
   }

   if (is_flagged(data_bits, DATABITS_7) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"7 data bits" ) );
   }

   if (is_flagged(data_bits, DATABITS_8) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"8 data bits" ) );
   }

   if (is_flagged(data_bits, DATABITS_16) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"16 data bits" ) );
   }

   if (is_flagged(data_bits, DATABITS_16X) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Special wide path through serial hardware" ) );
   }
}

void Win32FoundationClasses::CCommunicationProperties::ConvertProviderCapabilitiesToString( _In_ DWORD const capabilities, _Out_ std::wstring& human_readable_string ) const noexcept
{
   human_readable_string.clear();

   if (is_flagged(capabilities, PCF_DTRDSR) == true )
   {
      human_readable_string.assign(WSTRING_VIEW(L"DTR/DSR supported") );
   }

   if (is_flagged(capabilities, PCF_RTSCTS) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"RTS/CTS supported" ) );
   }

   if (is_flagged(capabilities, PCF_RLSD) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"RLSD supported" ) );
   }

   if (is_flagged(capabilities, PCF_PARITY_CHECK) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Parity checking supported" ) );
   }

   if (is_flagged(capabilities, PCF_XONXOFF) == true )
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"XON/XOFF supported" ) );
   }

   if (is_flagged(capabilities, PCF_SETXCHAR) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Settable XON/XOFF supported" ) );
   }

   if (is_flagged(capabilities, PCF_TOTALTIMEOUTS) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Total (elapsed) time-outs supported" ) );
   }

   if (is_flagged(capabilities, PCF_INTTIMEOUTS) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Interval time-outs supported" ) );
   }

   if (is_flagged(capabilities, PCF_SPECIALCHARS) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Special character support provided" ) );
   }

   if (is_flagged(capabilities, PCF_16BITMODE) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Special 16-bit mode supported" ) );
   }
}

void Win32FoundationClasses::CCommunicationProperties::ConvertProviderSubtypeToString( _In_ DWORD const type, _Out_ std::wstring& human_readable_string ) const noexcept
{
   switch( type )
   {
      case PST_UNSPECIFIED:

         human_readable_string.assign(WSTRING_VIEW(L"Unspecified" ) );
         break;

      case PST_RS232:

         human_readable_string.assign(WSTRING_VIEW(L"RS-232 serial port" ) );
         break;

      case PST_PARALLELPORT:

         human_readable_string.assign(WSTRING_VIEW(L"Parallel port" ) );
         break;

      case PST_RS422:

         human_readable_string.assign(WSTRING_VIEW(L"RS-422 port" ) );
         break;

      case PST_RS423:

         human_readable_string.assign(WSTRING_VIEW(L"RS-423 port" ) );
         break;

      case PST_RS449:

         human_readable_string.assign(WSTRING_VIEW(L"RS-449 port" ) );
         break;

      case PST_MODEM:

         human_readable_string.assign(WSTRING_VIEW(L"Modem device" ) );
         break;

      case PST_FAX:

         human_readable_string.assign(WSTRING_VIEW(L"FAX device" ) );
         break;

      case PST_SCANNER:

         human_readable_string.assign(WSTRING_VIEW(L"Scanner device" ) );
         break;

      case PST_NETWORK_BRIDGE:

         human_readable_string.assign(WSTRING_VIEW(L"Unspecified network bridge" ) );
         break;

      case PST_LAT:

         human_readable_string.assign(WSTRING_VIEW(L"LAT protocol" ) );
         break;

      case PST_TCPIP_TELNET:

         human_readable_string.assign(WSTRING_VIEW(L"TCP/IP Telnet protocol" ) );
         break;

      case PST_X25:

         human_readable_string.assign(WSTRING_VIEW(L"X.25 standards" ) );
         break;

      default:

         human_readable_string.assign(WSTRING_VIEW(L"Unknown constant "));
         human_readable_string.append(std::to_wstring(type));
         break;
   }
}

void Win32FoundationClasses::CCommunicationProperties::ConvertSettableParametersToString( _In_ DWORD const parameters, _Out_ std::wstring& human_readable_string ) const noexcept
{
   human_readable_string.clear();

   if ( is_flagged(parameters, SP_PARITY) == true )
   {
      human_readable_string.assign(WSTRING_VIEW(L"Parity" ) );
   }

   if (is_flagged(parameters, SP_BAUD) == true )
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Baud rate" ) );
   }

   if (is_flagged(parameters, SP_DATABITS) == true )
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Data bits" ) );
   }

   if (is_flagged(parameters, SP_STOPBITS) == true )
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Stop bits" ) );
   }

   if (is_flagged(parameters, SP_HANDSHAKING) == true )
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Handshaking (flow control)" ) );
   }

   if (is_flagged(parameters, SP_PARITY_CHECK) == true )
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Parity checking" ) );
   }

   if (is_flagged(parameters, SP_RLSD) == true )
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Receive-line-signal-detect (RLSD)" ) );
   }
}

void Win32FoundationClasses::CCommunicationProperties::ConvertStopParityToString( _In_ DWORD const stop_parity, _Out_ std::wstring& human_readable_string ) const noexcept
{
   human_readable_string.clear();

   if ( is_flagged(stop_parity, STOPBITS_10) == true )
   {
      human_readable_string.assign(WSTRING_VIEW(L"1 stop bit" ) );
   }

   if (is_flagged(stop_parity, STOPBITS_15) == true )
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"1.5 stop bits" ) );
   }

   if (is_flagged(stop_parity, STOPBITS_20) == true )
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"2 stop bits" ) );
   }

   if (is_flagged(stop_parity, PARITY_NONE) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"No parity" ) );
   }

   if (is_flagged(stop_parity, PARITY_ODD) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Odd parity" ) );
   }

   if (is_flagged(stop_parity, PARITY_EVEN) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Even parity" ) );
   }

   if (is_flagged(stop_parity, PARITY_MARK) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Mark parity" ) );
   }

   if (is_flagged(stop_parity, PARITY_SPACE) == true)
   {
       if (human_readable_string.empty() == false)
       {
           human_readable_string.append(WSTRING_VIEW(L", "));
       }

      human_readable_string.append(WSTRING_VIEW(L"Space parity" ) );
   }
}

void Win32FoundationClasses::CCommunicationProperties::Copy( _In_ Win32FoundationClasses::CCommunicationProperties const& source ) noexcept
{
   ASSERT( &source not_eq this );

   if ( &source == this )
   {
      return;
   }

   wPacketLength       = source.wPacketLength;
   wPacketVersion      = source.wPacketVersion;
   dwServiceMask       = source.dwServiceMask;
   dwReserved1         = source.dwReserved1;
   dwMaxTxQueue        = source.dwMaxTxQueue;
   dwMaxRxQueue        = source.dwMaxRxQueue;
   dwMaxBaud           = source.dwMaxBaud;
   dwProvSubType       = source.dwProvSubType;
   dwProvCapabilities  = source.dwProvCapabilities;
   dwSettableParams    = source.dwSettableParams;
   dwSettableBaud      = source.dwSettableBaud;
   wSettableData       = source.wSettableData;
   wSettableStopParity = source.wSettableStopParity;
   dwCurrentTxQueue    = source.dwCurrentTxQueue;
   dwCurrentRxQueue    = source.dwCurrentRxQueue;
   dwProvSpec1         = source.dwProvSpec1;
   dwProvSpec2         = source.dwProvSpec2;
   wcProvChar[ 0 ]     = source.wcProvChar[ 0 ];
}

void Win32FoundationClasses::CCommunicationProperties::Copy( _In_ _COMMPROP const& source ) noexcept
{
   wPacketLength       = source.wPacketLength;
   wPacketVersion      = source.wPacketVersion;
   dwServiceMask       = source.dwServiceMask;
   dwReserved1         = source.dwReserved1;
   dwMaxTxQueue        = source.dwMaxTxQueue;
   dwMaxRxQueue        = source.dwMaxRxQueue;
   dwMaxBaud           = source.dwMaxBaud;
   dwProvSubType       = source.dwProvSubType;
   dwProvCapabilities  = source.dwProvCapabilities;
   dwSettableParams    = source.dwSettableParams;
   dwSettableBaud      = source.dwSettableBaud;
   wSettableData       = source.wSettableData;
   wSettableStopParity = source.wSettableStopParity;
   dwCurrentTxQueue    = source.dwCurrentTxQueue;
   dwCurrentRxQueue    = source.dwCurrentRxQueue;
   dwProvSpec1         = source.dwProvSpec1;
   dwProvSpec2         = source.dwProvSpec2;
   wcProvChar[ 0 ]     = source.wcProvChar[ 0 ];
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CCommunicationProperties::Dump( CDumpContext& dump_context ) const
{
    std::wstring temp_string;

   dump_context << TEXT( " a CCommunicationProperties at " ) << (VOID *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   wPacketLength       = " ) << wPacketLength       << TEXT( "\n" );
   dump_context << TEXT( "   wPacketVersion      = " ) << wPacketVersion      << TEXT( "\n" );
   dump_context << TEXT( "   dwServiceMask       = " ) << dwServiceMask       << TEXT( "\n" );
   dump_context << TEXT( "   dwReserved1         = " ) << dwReserved1         << TEXT( "\n" );
   dump_context << TEXT( "   dwMaxTxQueue        = " ) << dwMaxTxQueue        << TEXT( "\n" );
   dump_context << TEXT( "   dwMaxRxQueue        = " ) << dwMaxRxQueue        << TEXT( "\n" );
   ConvertBaudRateToString( dwMaxBaud, temp_string );
   dump_context << TEXT( "   dwMaxBaud           = " ) << dwMaxBaud << TEXT( " (" ) << temp_string << TEXT( ")\n" );
   ConvertProviderSubtypeToString( dwProvSubType, temp_string );
   dump_context << TEXT( "   dwProvSubType       = " ) << dwProvSubType << TEXT( " (" ) << temp_string << TEXT( ")\n" );
   ConvertProviderCapabilitiesToString( dwProvCapabilities, temp_string );
   dump_context << TEXT( "   dwProvCapabilities  = " ) << dwProvCapabilities << TEXT( " (" ) << temp_string << TEXT( ")\n" );
   ConvertSettableParametersToString( dwSettableParams, temp_string );
   dump_context << TEXT( "   dwSettableParams    = " ) << dwSettableParams << TEXT( " (" ) << temp_string << TEXT( ")\n" );
   ConvertBaudRateListToString( dwSettableBaud, temp_string );
   dump_context << TEXT( "   dwSettableBaud      = " ) << dwSettableBaud << TEXT( " (" ) << temp_string << TEXT( ")\n" );
   ConvertDataBitsToString( wSettableData, temp_string );
   dump_context << TEXT( "   wSettableData       = " ) << wSettableData << TEXT( " (" ) << temp_string << TEXT( ")\n" );
   ConvertStopParityToString( wSettableStopParity, temp_string );
   dump_context << TEXT( "   wSettableStopParity = " ) << wSettableStopParity << TEXT( " (" ) << temp_string << TEXT( ")\n" );
   dump_context << TEXT( "   dwCurrentTxQueue    = " ) << dwCurrentTxQueue    << TEXT( "\n" );
   dump_context << TEXT( "   dwCurrentRxQueue    = " ) << dwCurrentRxQueue    << TEXT( "\n" );
   dump_context << TEXT( "   dwProvSpec1         = " ) << dwProvSpec1         << TEXT( "\n" );
   dump_context << TEXT( "   dwProvSpec2         = " ) << dwProvSpec2         << TEXT( "\n" );
   dump_context << TEXT( "   wcProvChar          = " ) << wcProvChar          << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CCommunicationProperties::Empty( void ) noexcept
{
   wPacketLength       = 0;
   wPacketVersion      = 0;
   dwServiceMask       = 0;
   dwReserved1         = 0;
   dwMaxTxQueue        = 0;
   dwMaxRxQueue        = 0;
   dwMaxBaud           = 0;
   dwProvSubType       = 0;
   dwProvCapabilities  = 0;
   dwSettableParams    = 0;
   dwSettableBaud      = 0;
   wSettableData       = 0;
   wSettableStopParity = 0;
   dwCurrentTxQueue    = 0;
   dwCurrentRxQueue    = 0;
   dwProvSpec1         = 0;
   dwProvSpec2         = 0;
   wcProvChar[ 0 ]     = 0;
}

_Check_return_ Win32FoundationClasses::CCommunicationProperties& Win32FoundationClasses::CCommunicationProperties::operator=( _In_ Win32FoundationClasses::CCommunicationProperties const& source ) noexcept
{
   Copy( source );
   return( *this );
}

_Check_return_ Win32FoundationClasses::CCommunicationProperties& Win32FoundationClasses::CCommunicationProperties::operator=( _In_ _COMMPROP const& source ) noexcept
{
   Copy( source );
   return( *this );
}

/*
** CMemoryStatus
*/

Win32FoundationClasses::CMemoryStatus::CMemoryStatus() noexcept
{
   Empty();
}

Win32FoundationClasses::CMemoryStatus::CMemoryStatus( _In_ Win32FoundationClasses::CMemoryStatus const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::CMemoryStatus::CMemoryStatus( _In_ MEMORYSTATUS const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::CMemoryStatus::Copy(_In_ Win32FoundationClasses::CMemoryStatus const& source ) noexcept
{
   Copy( static_cast<MEMORYSTATUS const *>(&source) );
}

void Win32FoundationClasses::CMemoryStatus::Copy(_In_ MEMORYSTATUS const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );

   if ( source == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      dwLength        = source->dwLength;
      dwMemoryLoad    = source->dwMemoryLoad;
      dwTotalPhys     = source->dwTotalPhys;
      dwAvailPhys     = source->dwAvailPhys;
      dwTotalPageFile = source->dwTotalPageFile;
      dwAvailPageFile = source->dwAvailPageFile;
      dwTotalVirtual  = source->dwTotalVirtual;
      dwAvailVirtual  = source->dwAvailVirtual;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CMemoryStatus::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CMemoryStatus at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   dwLength        = " ) << dwLength        << TEXT( "\n" );
   dump_context << TEXT( "   dwMemoryLoad    = " ) << dwMemoryLoad    << TEXT( "\n" );
   dump_context << TEXT( "   dwTotalPhys     = " ) << dwTotalPhys     << TEXT( "\n" );
   dump_context << TEXT( "   dwAvailPhys     = " ) << dwAvailPhys     << TEXT( "\n" );
   dump_context << TEXT( "   dwTotalPageFile = " ) << dwTotalPageFile << TEXT( "\n" );
   dump_context << TEXT( "   dwAvailPageFile = " ) << dwAvailPageFile << TEXT( "\n" );
   dump_context << TEXT( "   dwTotalVirtual  = " ) << dwTotalVirtual  << TEXT( "\n" );
   dump_context << TEXT( "   dwAvailVirtual  = " ) << dwAvailVirtual  << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CMemoryStatus::Empty( void ) noexcept
{
   dwLength        = sizeof( MEMORYSTATUS );
   dwMemoryLoad    = 0;
   dwTotalPhys     = 0;
   dwAvailPhys     = 0;
   dwTotalPageFile = 0;
   dwAvailPageFile = 0;
   dwTotalVirtual  = 0;
   dwAvailVirtual  = 0;
}

_Check_return_ Win32FoundationClasses::CMemoryStatus& Win32FoundationClasses::CMemoryStatus::operator=( _In_ Win32FoundationClasses::CMemoryStatus const& source ) noexcept
{
   Copy( source );
   return( *this );
}

/*
** COFStruct
*/

Win32FoundationClasses::COFStruct::COFStruct() noexcept
{
   Empty();
}

Win32FoundationClasses::COFStruct::COFStruct( _In_ Win32FoundationClasses::COFStruct const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::COFStruct::COFStruct( _In_ _OFSTRUCT const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::COFStruct::Copy(_In_ Win32FoundationClasses::COFStruct const& source ) noexcept
{
   Copy( static_cast<_OFSTRUCT const *>(&source) );
}

void Win32FoundationClasses::COFStruct::Copy(_In_ _OFSTRUCT const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      cBytes     = source->cBytes;
      fFixedDisk = source->fFixedDisk;
      nErrCode   = source->nErrCode;
      Reserved1  = source->Reserved1;
      Reserved2  = source->Reserved2;
      ::strcpy_s( szPathName, std::size( szPathName ), source->szPathName ); // Not a UNICODE string
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::COFStruct::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a COFStruct at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   cBytes     = "   ) << cBytes     << TEXT( "\n" );
   dump_context << TEXT( "   fFixedDisk = "   ) << fFixedDisk << TEXT( "\n" );
   dump_context << TEXT( "   nErrCode   = "   ) << nErrCode   << TEXT( "\n" );
   dump_context << TEXT( "   Reserved1  = "   ) << Reserved1  << TEXT( "\n" );
   dump_context << TEXT( "   Reserved2  = "   ) << Reserved2  << TEXT( "\n" );
   dump_context << TEXT( "   szPathName = \"" ) << szPathName << TEXT( "\"\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::COFStruct::Empty( void ) noexcept
{
   cBytes     = sizeof( _OFSTRUCT );
   fFixedDisk = 0;
   nErrCode   = 0;
   Reserved1  = 0;
   Reserved2  = 0;

   for( auto const index : Range(OFS_MAXPATHNAME) )
   {
      szPathName[ index ] = 0;
   }
}

_Check_return_ Win32FoundationClasses::COFStruct& Win32FoundationClasses::COFStruct::operator=( _In_ Win32FoundationClasses::COFStruct const& source ) noexcept
{
   Copy( source );
   return( *this );
}

/*
** COutlineTextMetricA
*/

Win32FoundationClasses::COutlineTextMetricA::COutlineTextMetricA() noexcept
{
   Empty();
}

Win32FoundationClasses::COutlineTextMetricA::COutlineTextMetricA( _In_ Win32FoundationClasses::COutlineTextMetricA const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::COutlineTextMetricA::COutlineTextMetricA( _In_ _OUTLINETEXTMETRICA const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::COutlineTextMetricA::Copy(_In_ Win32FoundationClasses::COutlineTextMetricA const& source ) noexcept
{
   Copy( static_cast<_OUTLINETEXTMETRICA const *>(&source) );
}

void Win32FoundationClasses::COutlineTextMetricA::Copy(_In_ _OUTLINETEXTMETRICA const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      otmSize = source->otmSize;

      // TEXTMETRICA
      otmTextMetrics.tmHeight           = source->otmTextMetrics.tmHeight;
      otmTextMetrics.tmAscent           = source->otmTextMetrics.tmAscent;
      otmTextMetrics.tmDescent          = source->otmTextMetrics.tmDescent;
      otmTextMetrics.tmInternalLeading  = source->otmTextMetrics.tmInternalLeading;
      otmTextMetrics.tmExternalLeading  = source->otmTextMetrics.tmExternalLeading;
      otmTextMetrics.tmAveCharWidth     = source->otmTextMetrics.tmAveCharWidth;
      otmTextMetrics.tmMaxCharWidth     = source->otmTextMetrics.tmMaxCharWidth;
      otmTextMetrics.tmWeight           = source->otmTextMetrics.tmWeight;
      otmTextMetrics.tmOverhang         = source->otmTextMetrics.tmOverhang;
      otmTextMetrics.tmDigitizedAspectX = source->otmTextMetrics.tmDigitizedAspectX;
      otmTextMetrics.tmDigitizedAspectY = source->otmTextMetrics.tmDigitizedAspectY;
      otmTextMetrics.tmFirstChar        = source->otmTextMetrics.tmFirstChar;
      otmTextMetrics.tmLastChar         = source->otmTextMetrics.tmLastChar;
      otmTextMetrics.tmDefaultChar      = source->otmTextMetrics.tmDefaultChar;
      otmTextMetrics.tmBreakChar        = source->otmTextMetrics.tmBreakChar;
      otmTextMetrics.tmItalic           = source->otmTextMetrics.tmItalic;
      otmTextMetrics.tmUnderlined       = source->otmTextMetrics.tmUnderlined;
      otmTextMetrics.tmStruckOut        = source->otmTextMetrics.tmStruckOut;
      otmTextMetrics.tmPitchAndFamily   = source->otmTextMetrics.tmPitchAndFamily;
      otmTextMetrics.tmCharSet          = source->otmTextMetrics.tmCharSet;
   
      otmFiller = source->otmFiller;
   
      // PANOSE
      otmPanoseNumber.bFamilyType      = source->otmPanoseNumber.bFamilyType;
      otmPanoseNumber.bSerifStyle      = source->otmPanoseNumber.bSerifStyle;
      otmPanoseNumber.bWeight          = source->otmPanoseNumber.bWeight;
      otmPanoseNumber.bProportion      = source->otmPanoseNumber.bProportion;
      otmPanoseNumber.bContrast        = source->otmPanoseNumber.bContrast;
      otmPanoseNumber.bStrokeVariation = source->otmPanoseNumber.bStrokeVariation;
      otmPanoseNumber.bArmStyle        = source->otmPanoseNumber.bArmStyle;
      otmPanoseNumber.bLetterform      = source->otmPanoseNumber.bLetterform;
      otmPanoseNumber.bMidline         = source->otmPanoseNumber.bMidline;
      otmPanoseNumber.bXHeight         = source->otmPanoseNumber.bXHeight;

      otmfsSelection    = source->otmfsSelection;
      otmfsType         = source->otmfsType;
      otmsCharSlopeRise = source->otmsCharSlopeRise;
      otmsCharSlopeRun  = source->otmsCharSlopeRun;
      otmItalicAngle    = source->otmItalicAngle;
      otmEMSquare       = source->otmEMSquare;
      otmAscent         = source->otmAscent;
      otmDescent        = source->otmDescent;
      otmLineGap        = source->otmLineGap;
      otmsCapEmHeight   = source->otmsCapEmHeight;
      otmsXHeight       = source->otmsXHeight;
   
      // RECT
      otmrcFontBox.left   = source->otmrcFontBox.left;
      otmrcFontBox.top    = source->otmrcFontBox.top;
      otmrcFontBox.right  = source->otmrcFontBox.right;
      otmrcFontBox.bottom = source->otmrcFontBox.bottom;

      otmMacAscent     = source->otmMacAscent;
      otmMacDescent    = source->otmMacDescent;
      otmMacLineGap    = source->otmMacLineGap;
      otmusMinimumPPEM = source->otmusMinimumPPEM;

      // POINT
      otmptSubscriptSize.x     = source->otmptSubscriptSize.x;
      otmptSubscriptSize.y     = source->otmptSubscriptSize.y;
      otmptSubscriptOffset.x   = source->otmptSubscriptOffset.x;
      otmptSubscriptOffset.y   = source->otmptSubscriptOffset.y;
      otmptSuperscriptSize.x   = source->otmptSuperscriptSize.x;
      otmptSuperscriptSize.y   = source->otmptSuperscriptSize.y;
      otmptSuperscriptOffset.x = source->otmptSuperscriptOffset.x;
      otmptSuperscriptOffset.y = source->otmptSuperscriptOffset.y;

      otmsStrikeoutSize      = source->otmsStrikeoutSize;
      otmsStrikeoutPosition  = source->otmsStrikeoutPosition;
      otmsUnderscoreSize     = source->otmsUnderscoreSize;
      otmsUnderscorePosition = source->otmsUnderscorePosition;
      otmpFamilyName         = source->otmpFamilyName;
      otmpFaceName           = source->otmpFaceName;
      otmpStyleName          = source->otmpStyleName;
      otmpFullName           = source->otmpFullName;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::COutlineTextMetricA::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a COutlineTextMetricA at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   otmSize                           = " ) << otmSize                           << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmHeight           = " ) << otmTextMetrics.tmHeight           << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmAscent           = " ) << otmTextMetrics.tmAscent           << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmDescent          = " ) << otmTextMetrics.tmDescent          << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmInternalLeading  = " ) << otmTextMetrics.tmInternalLeading  << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmExternalLeading  = " ) << otmTextMetrics.tmExternalLeading  << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmAveCharWidth     = " ) << otmTextMetrics.tmAveCharWidth     << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmMaxCharWidth     = " ) << otmTextMetrics.tmMaxCharWidth     << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmWeight           = " ) << otmTextMetrics.tmWeight           << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmOverhang         = " ) << otmTextMetrics.tmOverhang         << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmDigitizedAspectX = " ) << otmTextMetrics.tmDigitizedAspectX << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmDigitizedAspectY = " ) << otmTextMetrics.tmDigitizedAspectY << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmFirstChar        = " ) << otmTextMetrics.tmFirstChar        << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmLastChar         = " ) << otmTextMetrics.tmLastChar         << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmDefaultChar      = " ) << otmTextMetrics.tmDefaultChar      << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmBreakChar        = " ) << otmTextMetrics.tmBreakChar        << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmItalic           = " ) << otmTextMetrics.tmItalic           << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmUnderlined       = " ) << otmTextMetrics.tmUnderlined       << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmStruckOut        = " ) << otmTextMetrics.tmStruckOut        << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmPitchAndFamily   = " ) << otmTextMetrics.tmPitchAndFamily   << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmCharSet          = " ) << otmTextMetrics.tmCharSet          << TEXT( "\n" );
   dump_context << TEXT( "   otmFiller                         = " ) << otmFiller                         << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bFamilyType       = " ) << otmPanoseNumber.bFamilyType       << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bSerifStyle       = " ) << otmPanoseNumber.bSerifStyle       << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bWeight           = " ) << otmPanoseNumber.bWeight           << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bProportion       = " ) << otmPanoseNumber.bProportion       << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bContrast         = " ) << otmPanoseNumber.bContrast         << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bStrokeVariation  = " ) << otmPanoseNumber.bStrokeVariation  << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bArmStyle         = " ) << otmPanoseNumber.bArmStyle         << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bLetterform       = " ) << otmPanoseNumber.bLetterform       << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bMidline          = " ) << otmPanoseNumber.bMidline          << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bXHeight          = " ) << otmPanoseNumber.bXHeight          << TEXT( "\n" );
   dump_context << TEXT( "   otmfsSelection                    = " ) << otmfsSelection                    << TEXT( "\n" );
   dump_context << TEXT( "   otmfsType                         = " ) << otmfsType                         << TEXT( "\n" );
   dump_context << TEXT( "   otmsCharSlopeRise                 = " ) << otmsCharSlopeRise                 << TEXT( "\n" );
   dump_context << TEXT( "   otmsCharSlopeRun                  = " ) << otmsCharSlopeRun                  << TEXT( "\n" );
   dump_context << TEXT( "   otmItalicAngle                    = " ) << otmItalicAngle                    << TEXT( "\n" );
   dump_context << TEXT( "   otmEMSquare                       = " ) << otmEMSquare                       << TEXT( "\n" );
   dump_context << TEXT( "   otmAscent                         = " ) << otmAscent                         << TEXT( "\n" );
   dump_context << TEXT( "   otmDescent                        = " ) << otmDescent                        << TEXT( "\n" );
   dump_context << TEXT( "   otmLineGap                        = " ) << otmLineGap                        << TEXT( "\n" );
   dump_context << TEXT( "   otmsCapEmHeight                   = " ) << otmsCapEmHeight                   << TEXT( "\n" );
   dump_context << TEXT( "   otmsXHeight                       = " ) << otmsXHeight                       << TEXT( "\n" );
   dump_context << TEXT( "   otmrcFontBox.left                 = " ) << otmrcFontBox.left                 << TEXT( "\n" );
   dump_context << TEXT( "   otmrcFontBox.top                  = " ) << otmrcFontBox.top                  << TEXT( "\n" );
   dump_context << TEXT( "   otmrcFontBox.right                = " ) << otmrcFontBox.right                << TEXT( "\n" );
   dump_context << TEXT( "   otmrcFontBox.bottom               = " ) << otmrcFontBox.bottom               << TEXT( "\n" );
   dump_context << TEXT( "   otmMacAscent                      = " ) << otmMacAscent                      << TEXT( "\n" );
   dump_context << TEXT( "   otmMacDescent                     = " ) << otmMacDescent                     << TEXT( "\n" );
   dump_context << TEXT( "   otmMacLineGap                     = " ) << otmMacLineGap                     << TEXT( "\n" );
   dump_context << TEXT( "   otmusMinimumPPEM                  = " ) << otmusMinimumPPEM                  << TEXT( "\n" );
   dump_context << TEXT( "   otmptSubscriptSize.x              = " ) << otmptSubscriptSize.x              << TEXT( "\n" );
   dump_context << TEXT( "   otmptSubscriptSize.y              = " ) << otmptSubscriptSize.y              << TEXT( "\n" );
   dump_context << TEXT( "   otmptSubscriptOffset.x            = " ) << otmptSubscriptOffset.x            << TEXT( "\n" );
   dump_context << TEXT( "   otmptSubscriptOffset.y            = " ) << otmptSubscriptOffset.y            << TEXT( "\n" );
   dump_context << TEXT( "   otmptSuperscriptSize.x            = " ) << otmptSuperscriptSize.x            << TEXT( "\n" );
   dump_context << TEXT( "   otmptSuperscriptSize.y            = " ) << otmptSuperscriptSize.y            << TEXT( "\n" );
   dump_context << TEXT( "   otmptSuperscriptOffset.x          = " ) << otmptSuperscriptOffset.x          << TEXT( "\n" );
   dump_context << TEXT( "   otmptSuperscriptOffset.y          = " ) << otmptSuperscriptOffset.y          << TEXT( "\n" );
   dump_context << TEXT( "   otmsStrikeoutSize                 = " ) << otmsStrikeoutSize                 << TEXT( "\n" );
   dump_context << TEXT( "   otmsStrikeoutPosition             = " ) << otmsStrikeoutPosition             << TEXT( "\n" );
   dump_context << TEXT( "   otmsUnderscoreSize                = " ) << otmsUnderscoreSize                << TEXT( "\n" );
   dump_context << TEXT( "   otmsUnderscorePosition            = " ) << otmsUnderscorePosition            << TEXT( "\n" );
   dump_context << TEXT( "   otmpFamilyName                    = " ) << otmpFamilyName                    << TEXT( "\n" );
   dump_context << TEXT( "   otmpFaceName                      = " ) << otmpFaceName                      << TEXT( "\n" );
   dump_context << TEXT( "   otmpStyleName                     = " ) << otmpStyleName                     << TEXT( "\n" );
   dump_context << TEXT( "   otmpFullName                      = " ) << otmpFullName                      << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::COutlineTextMetricA::Empty( void ) noexcept
{
   otmSize = sizeof( _OUTLINETEXTMETRICA );

   // TEXTMETRICA
   otmTextMetrics.tmHeight           = 0;
   otmTextMetrics.tmAscent           = 0;
   otmTextMetrics.tmDescent          = 0;
   otmTextMetrics.tmInternalLeading  = 0;
   otmTextMetrics.tmExternalLeading  = 0;
   otmTextMetrics.tmAveCharWidth     = 0;
   otmTextMetrics.tmMaxCharWidth     = 0;
   otmTextMetrics.tmWeight           = 0;
   otmTextMetrics.tmOverhang         = 0;
   otmTextMetrics.tmDigitizedAspectX = 0;
   otmTextMetrics.tmDigitizedAspectY = 0;
   otmTextMetrics.tmFirstChar        = 0;
   otmTextMetrics.tmLastChar         = 0;
   otmTextMetrics.tmDefaultChar      = 0;
   otmTextMetrics.tmBreakChar        = 0;
   otmTextMetrics.tmItalic           = 0;
   otmTextMetrics.tmUnderlined       = 0;
   otmTextMetrics.tmStruckOut        = 0;
   otmTextMetrics.tmPitchAndFamily   = 0;
   otmTextMetrics.tmCharSet          = 0;

   otmFiller = 0;

   // PANOSE
   otmPanoseNumber.bFamilyType      = 0;
   otmPanoseNumber.bSerifStyle      = 0;
   otmPanoseNumber.bWeight          = 0;
   otmPanoseNumber.bProportion      = 0;
   otmPanoseNumber.bContrast        = 0;
   otmPanoseNumber.bStrokeVariation = 0;
   otmPanoseNumber.bArmStyle        = 0;
   otmPanoseNumber.bLetterform      = 0;
   otmPanoseNumber.bMidline         = 0;
   otmPanoseNumber.bXHeight         = 0;

   otmfsSelection    = 0;
   otmfsType         = 0;
   otmsCharSlopeRise = 0;
   otmsCharSlopeRun  = 0;
   otmItalicAngle    = 0;
   otmEMSquare       = 0;
   otmAscent         = 0;
   otmDescent        = 0;
   otmLineGap        = 0;
   otmsCapEmHeight   = 0;
   otmsXHeight       = 0;

   // RECT
   otmrcFontBox.left   = 0;
   otmrcFontBox.top    = 0;
   otmrcFontBox.right  = 0;
   otmrcFontBox.bottom = 0;

   otmMacAscent     = 0;
   otmMacDescent    = 0;
   otmMacLineGap    = 0;
   otmusMinimumPPEM = 0;

   // POINT
   otmptSubscriptSize.x     = 0;
   otmptSubscriptSize.y     = 0;
   otmptSubscriptOffset.x   = 0;
   otmptSubscriptOffset.y   = 0;
   otmptSuperscriptSize.x   = 0;
   otmptSuperscriptSize.y   = 0;
   otmptSuperscriptOffset.x = 0;
   otmptSuperscriptOffset.y = 0;

   otmsStrikeoutSize      = 0;
   otmsStrikeoutPosition  = 0;
   otmsUnderscoreSize     = 0;
   otmsUnderscorePosition = 0;
   otmpFamilyName         = nullptr;
   otmpFaceName           = nullptr;
   otmpStyleName          = nullptr;
   otmpFullName           = nullptr;
}

_Check_return_ Win32FoundationClasses::COutlineTextMetricA& Win32FoundationClasses::COutlineTextMetricA::operator=( _In_ Win32FoundationClasses::COutlineTextMetricA const& source ) noexcept
{
   Copy( source );
   return( *this );
}

Win32FoundationClasses::COutlineTextMetricW::COutlineTextMetricW() noexcept
{
   Empty();
}

Win32FoundationClasses::COutlineTextMetricW::COutlineTextMetricW( _In_ Win32FoundationClasses::COutlineTextMetricW const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::COutlineTextMetricW::COutlineTextMetricW( _In_ _OUTLINETEXTMETRICW const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::COutlineTextMetricW::Copy(_In_ Win32FoundationClasses::COutlineTextMetricW const& source ) noexcept
{
   Copy( static_cast<_OUTLINETEXTMETRICW const *>(&source) );
}

void Win32FoundationClasses::COutlineTextMetricW::Copy(_In_ _OUTLINETEXTMETRICW const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      otmSize = source->otmSize;

      // TEXTMETRICA
      otmTextMetrics.tmHeight           = source->otmTextMetrics.tmHeight;
      otmTextMetrics.tmAscent           = source->otmTextMetrics.tmAscent;
      otmTextMetrics.tmDescent          = source->otmTextMetrics.tmDescent;
      otmTextMetrics.tmInternalLeading  = source->otmTextMetrics.tmInternalLeading;
      otmTextMetrics.tmExternalLeading  = source->otmTextMetrics.tmExternalLeading;
      otmTextMetrics.tmAveCharWidth     = source->otmTextMetrics.tmAveCharWidth;
      otmTextMetrics.tmMaxCharWidth     = source->otmTextMetrics.tmMaxCharWidth;
      otmTextMetrics.tmWeight           = source->otmTextMetrics.tmWeight;
      otmTextMetrics.tmOverhang         = source->otmTextMetrics.tmOverhang;
      otmTextMetrics.tmDigitizedAspectX = source->otmTextMetrics.tmDigitizedAspectX;
      otmTextMetrics.tmDigitizedAspectY = source->otmTextMetrics.tmDigitizedAspectY;
      otmTextMetrics.tmFirstChar        = source->otmTextMetrics.tmFirstChar;
      otmTextMetrics.tmLastChar         = source->otmTextMetrics.tmLastChar;
      otmTextMetrics.tmDefaultChar      = source->otmTextMetrics.tmDefaultChar;
      otmTextMetrics.tmBreakChar        = source->otmTextMetrics.tmBreakChar;
      otmTextMetrics.tmItalic           = source->otmTextMetrics.tmItalic;
      otmTextMetrics.tmUnderlined       = source->otmTextMetrics.tmUnderlined;
      otmTextMetrics.tmStruckOut        = source->otmTextMetrics.tmStruckOut;
      otmTextMetrics.tmPitchAndFamily   = source->otmTextMetrics.tmPitchAndFamily;
      otmTextMetrics.tmCharSet          = source->otmTextMetrics.tmCharSet;

      otmFiller = source->otmFiller;

      // PANOSE
      otmPanoseNumber.bFamilyType      = source->otmPanoseNumber.bFamilyType;
      otmPanoseNumber.bSerifStyle      = source->otmPanoseNumber.bSerifStyle;
      otmPanoseNumber.bWeight          = source->otmPanoseNumber.bWeight;
      otmPanoseNumber.bProportion      = source->otmPanoseNumber.bProportion;
      otmPanoseNumber.bContrast        = source->otmPanoseNumber.bContrast;
      otmPanoseNumber.bStrokeVariation = source->otmPanoseNumber.bStrokeVariation;
      otmPanoseNumber.bArmStyle        = source->otmPanoseNumber.bArmStyle;
      otmPanoseNumber.bLetterform      = source->otmPanoseNumber.bLetterform;
      otmPanoseNumber.bMidline         = source->otmPanoseNumber.bMidline;
      otmPanoseNumber.bXHeight         = source->otmPanoseNumber.bXHeight;

      otmfsSelection    = source->otmfsSelection;
      otmfsType         = source->otmfsType;
      otmsCharSlopeRise = source->otmsCharSlopeRise;
      otmsCharSlopeRun  = source->otmsCharSlopeRun;
      otmItalicAngle    = source->otmItalicAngle;
      otmEMSquare       = source->otmEMSquare;
      otmAscent         = source->otmAscent;
      otmDescent        = source->otmDescent;
      otmLineGap        = source->otmLineGap;
      otmsCapEmHeight   = source->otmsCapEmHeight;
      otmsXHeight       = source->otmsXHeight;

      // RECT
      otmrcFontBox.left   = source->otmrcFontBox.left;
      otmrcFontBox.top    = source->otmrcFontBox.top;
      otmrcFontBox.right  = source->otmrcFontBox.right;
      otmrcFontBox.bottom = source->otmrcFontBox.bottom;

      otmMacAscent     = source->otmMacAscent;
      otmMacDescent    = source->otmMacDescent;
      otmMacLineGap    = source->otmMacLineGap;
      otmusMinimumPPEM = source->otmusMinimumPPEM;

      // POINT
      otmptSubscriptSize.x     = source->otmptSubscriptSize.x;
      otmptSubscriptSize.y     = source->otmptSubscriptSize.y;
      otmptSubscriptOffset.x   = source->otmptSubscriptOffset.x;
      otmptSubscriptOffset.y   = source->otmptSubscriptOffset.y;
      otmptSuperscriptSize.x   = source->otmptSuperscriptSize.x;
      otmptSuperscriptSize.y   = source->otmptSuperscriptSize.y;
      otmptSuperscriptOffset.x = source->otmptSuperscriptOffset.x;
      otmptSuperscriptOffset.y = source->otmptSuperscriptOffset.y;

      otmsStrikeoutSize      = source->otmsStrikeoutSize;
      otmsStrikeoutPosition  = source->otmsStrikeoutPosition;
      otmsUnderscoreSize     = source->otmsUnderscoreSize;
      otmsUnderscorePosition = source->otmsUnderscorePosition;
      otmpFamilyName         = source->otmpFamilyName;
      otmpFaceName           = source->otmpFaceName;
      otmpStyleName          = source->otmpStyleName;
      otmpFullName           = source->otmpFullName;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::COutlineTextMetricW::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a COutlineTextMetricW at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   otmSize                           = " ) << otmSize                           << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmHeight           = " ) << otmTextMetrics.tmHeight           << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmAscent           = " ) << otmTextMetrics.tmAscent           << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmDescent          = " ) << otmTextMetrics.tmDescent          << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmInternalLeading  = " ) << otmTextMetrics.tmInternalLeading  << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmExternalLeading  = " ) << otmTextMetrics.tmExternalLeading  << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmAveCharWidth     = " ) << otmTextMetrics.tmAveCharWidth     << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmMaxCharWidth     = " ) << otmTextMetrics.tmMaxCharWidth     << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmWeight           = " ) << otmTextMetrics.tmWeight           << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmOverhang         = " ) << otmTextMetrics.tmOverhang         << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmDigitizedAspectX = " ) << otmTextMetrics.tmDigitizedAspectX << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmDigitizedAspectY = " ) << otmTextMetrics.tmDigitizedAspectY << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmFirstChar        = " ) << otmTextMetrics.tmFirstChar        << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmLastChar         = " ) << otmTextMetrics.tmLastChar         << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmDefaultChar      = " ) << otmTextMetrics.tmDefaultChar      << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmBreakChar        = " ) << otmTextMetrics.tmBreakChar        << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmItalic           = " ) << otmTextMetrics.tmItalic           << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmUnderlined       = " ) << otmTextMetrics.tmUnderlined       << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmStruckOut        = " ) << otmTextMetrics.tmStruckOut        << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmPitchAndFamily   = " ) << otmTextMetrics.tmPitchAndFamily   << TEXT( "\n" );
   dump_context << TEXT( "   otmTextMetrics.tmCharSet          = " ) << otmTextMetrics.tmCharSet          << TEXT( "\n" );
   dump_context << TEXT( "   otmFiller                         = " ) << otmFiller                         << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bFamilyType       = " ) << otmPanoseNumber.bFamilyType       << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bSerifStyle       = " ) << otmPanoseNumber.bSerifStyle       << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bWeight           = " ) << otmPanoseNumber.bWeight           << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bProportion       = " ) << otmPanoseNumber.bProportion       << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bContrast         = " ) << otmPanoseNumber.bContrast         << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bStrokeVariation  = " ) << otmPanoseNumber.bStrokeVariation  << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bArmStyle         = " ) << otmPanoseNumber.bArmStyle         << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bLetterform       = " ) << otmPanoseNumber.bLetterform       << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bMidline          = " ) << otmPanoseNumber.bMidline          << TEXT( "\n" );
   dump_context << TEXT( "   otmPanoseNumber.bXHeight          = " ) << otmPanoseNumber.bXHeight          << TEXT( "\n" );
   dump_context << TEXT( "   otmfsSelection                    = " ) << otmfsSelection                    << TEXT( "\n" );
   dump_context << TEXT( "   otmfsType                         = " ) << otmfsType                         << TEXT( "\n" );
   dump_context << TEXT( "   otmsCharSlopeRise                 = " ) << otmsCharSlopeRise                 << TEXT( "\n" );
   dump_context << TEXT( "   otmsCharSlopeRun                  = " ) << otmsCharSlopeRun                  << TEXT( "\n" );
   dump_context << TEXT( "   otmItalicAngle                    = " ) << otmItalicAngle                    << TEXT( "\n" );
   dump_context << TEXT( "   otmEMSquare                       = " ) << otmEMSquare                       << TEXT( "\n" );
   dump_context << TEXT( "   otmAscent                         = " ) << otmAscent                         << TEXT( "\n" );
   dump_context << TEXT( "   otmDescent                        = " ) << otmDescent                        << TEXT( "\n" );
   dump_context << TEXT( "   otmLineGap                        = " ) << otmLineGap                        << TEXT( "\n" );
   dump_context << TEXT( "   otmsCapEmHeight                   = " ) << otmsCapEmHeight                   << TEXT( "\n" );
   dump_context << TEXT( "   otmsXHeight                       = " ) << otmsXHeight                       << TEXT( "\n" );
   dump_context << TEXT( "   otmrcFontBox.left                 = " ) << otmrcFontBox.left                 << TEXT( "\n" );
   dump_context << TEXT( "   otmrcFontBox.top                  = " ) << otmrcFontBox.top                  << TEXT( "\n" );
   dump_context << TEXT( "   otmrcFontBox.right                = " ) << otmrcFontBox.right                << TEXT( "\n" );
   dump_context << TEXT( "   otmrcFontBox.bottom               = " ) << otmrcFontBox.bottom               << TEXT( "\n" );
   dump_context << TEXT( "   otmMacAscent                      = " ) << otmMacAscent                      << TEXT( "\n" );
   dump_context << TEXT( "   otmMacDescent                     = " ) << otmMacDescent                     << TEXT( "\n" );
   dump_context << TEXT( "   otmMacLineGap                     = " ) << otmMacLineGap                     << TEXT( "\n" );
   dump_context << TEXT( "   otmusMinimumPPEM                  = " ) << otmusMinimumPPEM                  << TEXT( "\n" );
   dump_context << TEXT( "   otmptSubscriptSize.x              = " ) << otmptSubscriptSize.x              << TEXT( "\n" );
   dump_context << TEXT( "   otmptSubscriptSize.y              = " ) << otmptSubscriptSize.y              << TEXT( "\n" );
   dump_context << TEXT( "   otmptSubscriptOffset.x            = " ) << otmptSubscriptOffset.x            << TEXT( "\n" );
   dump_context << TEXT( "   otmptSubscriptOffset.y            = " ) << otmptSubscriptOffset.y            << TEXT( "\n" );
   dump_context << TEXT( "   otmptSuperscriptSize.x            = " ) << otmptSuperscriptSize.x            << TEXT( "\n" );
   dump_context << TEXT( "   otmptSuperscriptSize.y            = " ) << otmptSuperscriptSize.y            << TEXT( "\n" );
   dump_context << TEXT( "   otmptSuperscriptOffset.x          = " ) << otmptSuperscriptOffset.x          << TEXT( "\n" );
   dump_context << TEXT( "   otmptSuperscriptOffset.y          = " ) << otmptSuperscriptOffset.y          << TEXT( "\n" );
   dump_context << TEXT( "   otmsStrikeoutSize                 = " ) << otmsStrikeoutSize                 << TEXT( "\n" );
   dump_context << TEXT( "   otmsStrikeoutPosition             = " ) << otmsStrikeoutPosition             << TEXT( "\n" );
   dump_context << TEXT( "   otmsUnderscoreSize                = " ) << otmsUnderscoreSize                << TEXT( "\n" );
   dump_context << TEXT( "   otmsUnderscorePosition            = " ) << otmsUnderscorePosition            << TEXT( "\n" );
   dump_context << TEXT( "   otmpFamilyName                    = " ) << otmpFamilyName                    << TEXT( "\n" );
   dump_context << TEXT( "   otmpFaceName                      = " ) << otmpFaceName                      << TEXT( "\n" );
   dump_context << TEXT( "   otmpStyleName                     = " ) << otmpStyleName                     << TEXT( "\n" );
   dump_context << TEXT( "   otmpFullName                      = " ) << otmpFullName                      << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::COutlineTextMetricW::Empty( void ) noexcept
{
   otmSize = sizeof( _OUTLINETEXTMETRICW );

   // TEXTMETRICW
   otmTextMetrics.tmHeight           = 0;
   otmTextMetrics.tmAscent           = 0;
   otmTextMetrics.tmDescent          = 0;
   otmTextMetrics.tmInternalLeading  = 0;
   otmTextMetrics.tmExternalLeading  = 0;
   otmTextMetrics.tmAveCharWidth     = 0;
   otmTextMetrics.tmMaxCharWidth     = 0;
   otmTextMetrics.tmWeight           = 0;
   otmTextMetrics.tmOverhang         = 0;
   otmTextMetrics.tmDigitizedAspectX = 0;
   otmTextMetrics.tmDigitizedAspectY = 0;
   otmTextMetrics.tmFirstChar        = 0;
   otmTextMetrics.tmLastChar         = 0;
   otmTextMetrics.tmDefaultChar      = 0;
   otmTextMetrics.tmBreakChar        = 0;
   otmTextMetrics.tmItalic           = 0;
   otmTextMetrics.tmUnderlined       = 0;
   otmTextMetrics.tmStruckOut        = 0;
   otmTextMetrics.tmPitchAndFamily   = 0;
   otmTextMetrics.tmCharSet          = 0;

   otmFiller = 0;

   // PANOSE
   otmPanoseNumber.bFamilyType      = 0;
   otmPanoseNumber.bSerifStyle      = 0;
   otmPanoseNumber.bWeight          = 0;
   otmPanoseNumber.bProportion      = 0;
   otmPanoseNumber.bContrast        = 0;
   otmPanoseNumber.bStrokeVariation = 0;
   otmPanoseNumber.bArmStyle        = 0;
   otmPanoseNumber.bLetterform      = 0;
   otmPanoseNumber.bMidline         = 0;
   otmPanoseNumber.bXHeight         = 0;

   otmfsSelection    = 0;
   otmfsType         = 0;
   otmsCharSlopeRise = 0;
   otmsCharSlopeRun  = 0;
   otmItalicAngle    = 0;
   otmEMSquare       = 0;
   otmAscent         = 0;
   otmDescent        = 0;
   otmLineGap        = 0;
   otmsCapEmHeight   = 0;
   otmsXHeight       = 0;

   // RECT
   otmrcFontBox.left   = 0;
   otmrcFontBox.top    = 0;
   otmrcFontBox.right  = 0;
   otmrcFontBox.bottom = 0;

   otmMacAscent     = 0;
   otmMacDescent    = 0;
   otmMacLineGap    = 0;
   otmusMinimumPPEM = 0;

   // POINT
   otmptSubscriptSize.x     = 0;
   otmptSubscriptSize.y     = 0;
   otmptSubscriptOffset.x   = 0;
   otmptSubscriptOffset.y   = 0;
   otmptSuperscriptSize.x   = 0;
   otmptSuperscriptSize.y   = 0;
   otmptSuperscriptOffset.x = 0;
   otmptSuperscriptOffset.y = 0;

   otmsStrikeoutSize      = 0;
   otmsStrikeoutPosition  = 0;
   otmsUnderscoreSize     = 0;
   otmsUnderscorePosition = 0;
   otmpFamilyName         = nullptr;
   otmpFaceName           = nullptr;
   otmpStyleName          = nullptr;
   otmpFullName           = nullptr;
}

_Check_return_ Win32FoundationClasses::COutlineTextMetricW& Win32FoundationClasses::COutlineTextMetricW::operator=( _In_ Win32FoundationClasses::COutlineTextMetricW const& source ) noexcept
{
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CPerfCounterDefinition::CPerfCounterDefinition() noexcept
{
   Empty();
}

void Win32FoundationClasses::CPerfCounterDefinition::Empty( void ) noexcept
{
   ByteLength            = sizeof( PERF_COUNTER_DEFINITION );
   CounterNameTitleIndex = 0;
   CounterNameTitle      = 0;
   CounterHelpTitleIndex = 0;
   CounterHelpTitle      = 0;
   DefaultScale          = 0;
   DetailLevel           = 0;
   CounterType           = 0;
   CounterSize           = 0;
   CounterOffset         = 0;
}

Win32FoundationClasses::CPerfInstanceDefinition::CPerfInstanceDefinition() noexcept
{
   Empty();
}

void Win32FoundationClasses::CPerfInstanceDefinition::Empty( void ) noexcept
{
   ByteLength             = sizeof( PERF_INSTANCE_DEFINITION );
   ParentObjectTitleIndex = 0;
   ParentObjectInstance   = 0;
   UniqueID               = 0;
   NameOffset             = 0;
   NameLength             = 0;
}

Win32FoundationClasses::CPixelFormatDescriptor::CPixelFormatDescriptor() noexcept
{
   Empty();
}

Win32FoundationClasses::CPixelFormatDescriptor::CPixelFormatDescriptor( _In_ Win32FoundationClasses::CPixelFormatDescriptor const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::CPixelFormatDescriptor::CPixelFormatDescriptor( _In_ tagPIXELFORMATDESCRIPTOR const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::CPixelFormatDescriptor::Copy(_In_ Win32FoundationClasses::CPixelFormatDescriptor const& source ) noexcept
{
   Copy( static_cast<tagPIXELFORMATDESCRIPTOR const *>(&source) );
}

void Win32FoundationClasses::CPixelFormatDescriptor::Copy(_In_ tagPIXELFORMATDESCRIPTOR const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      nSize           = source->nSize;
      nVersion        = source->nVersion;
      dwFlags         = source->dwFlags;
      iPixelType      = source->iPixelType;
      cColorBits      = source->cColorBits;
      cRedBits        = source->cRedBits;
      cRedShift       = source->cRedShift;
      cGreenBits      = source->cGreenBits;
      cGreenShift     = source->cGreenShift;
      cBlueBits       = source->cBlueBits;
      cBlueShift      = source->cBlueShift;
      cAlphaBits      = source->cAlphaBits;
      cAlphaShift     = source->cAlphaShift;
      cAccumBits      = source->cAccumBits;
      cAccumRedBits   = source->cAccumRedBits;
      cAccumGreenBits = source->cAccumGreenBits;
      cAccumBlueBits  = source->cAccumBlueBits;
      cAccumAlphaBits = source->cAccumAlphaBits;
      cDepthBits      = source->cDepthBits;
      cStencilBits    = source->cStencilBits;
      cAuxBuffers     = source->cAuxBuffers;
      iLayerType      = source->iLayerType;
      bReserved       = source->bReserved;
      dwLayerMask     = source->dwLayerMask;
      dwVisibleMask   = source->dwVisibleMask;
      dwDamageMask    = source->dwDamageMask;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CPixelFormatDescriptor::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CPixelFormatDescriptor at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   nSize           = " ) << nSize           << TEXT( "\n" );
   dump_context << TEXT( "   nVersion        = " ) << nVersion        << TEXT( "\n" );
   dump_context << TEXT( "   dwFlags         = " ) << dwFlags         << TEXT( " (" );

   std::wstring temp_string;

   if ( dwFlags bitand PFD_DRAW_TO_WINDOW )
   {
      temp_string = TEXT( "PFD_DRAW_TO_WINDOW" );
   }

   if ( dwFlags bitand PFD_DRAW_TO_BITMAP )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "PFD_DRAW_TO_BITMAP" );
   }

   if ( dwFlags bitand PFD_SUPPORT_GDI )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "PFD_SUPPORT_GDI" );
   }

   if ( dwFlags bitand PFD_SUPPORT_OPENGL )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "PFD_SUPPORT_OPENGL" );
   }

   if ( dwFlags bitand PFD_GENERIC_ACCELERATED )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "PFD_GENERIC_ACCELERATED" );
   }

   if ( dwFlags bitand PFD_GENERIC_FORMAT )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "PFD_GENERIC_FORMAT" );
   }

   if ( dwFlags bitand PFD_NEED_PALETTE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "PFD_NEED_PALETTE" );
   }

   if ( dwFlags bitand PFD_NEED_SYSTEM_PALETTE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "PFD_NEED_SYSTEM_PALETTE" );
   }

   if ( dwFlags bitand PFD_DOUBLEBUFFER )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "PFD_DOUBLEBUFFER" );
   }

   if ( dwFlags bitand PFD_STEREO )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "PFD_STEREO" );
   }

   if ( dwFlags bitand PFD_SWAP_LAYER_BUFFERS )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "PFD_SWAP_LAYER_BUFFERS" );
   }

   dump_context << temp_string << TEXT( " )\n" );

   dump_context << TEXT( "   iPixelType      = " ) << iPixelType      << TEXT( " (" );

   switch( iPixelType )
   {
      case PFD_TYPE_RGBA:

         temp_string = TEXT( "PFD_TYPE_RGBA" );
         break;

      case PFD_TYPE_COLORINDEX:

         temp_string = TEXT( "PFD_TYPE_COLORINDEX" );
         break;
   }

   dump_context << temp_string << TEXT( " )\n" );

   dump_context << TEXT( "   cColorBits      = " ) << cColorBits      << TEXT( "\n" );
   dump_context << TEXT( "   cRedBits        = " ) << cRedBits        << TEXT( "\n" );
   dump_context << TEXT( "   cRedShift       = " ) << cRedShift       << TEXT( "\n" );
   dump_context << TEXT( "   cGreenBits      = " ) << cGreenBits      << TEXT( "\n" );
   dump_context << TEXT( "   cGreenShift     = " ) << cGreenShift     << TEXT( "\n" );
   dump_context << TEXT( "   cBlueBits       = " ) << cBlueBits       << TEXT( "\n" );
   dump_context << TEXT( "   cBlueShift      = " ) << cBlueShift      << TEXT( "\n" );
   dump_context << TEXT( "   cAlphaBits      = " ) << cAlphaBits      << TEXT( "\n" );
   dump_context << TEXT( "   cAlphaShift     = " ) << cAlphaShift     << TEXT( "\n" );
   dump_context << TEXT( "   cAccumBits      = " ) << cAccumBits      << TEXT( "\n" );
   dump_context << TEXT( "   cAccumRedBits   = " ) << cAccumRedBits   << TEXT( "\n" );
   dump_context << TEXT( "   cAccumGreenBits = " ) << cAccumGreenBits << TEXT( "\n" );
   dump_context << TEXT( "   cAccumBlueBits  = " ) << cAccumBlueBits  << TEXT( "\n" );
   dump_context << TEXT( "   cAccumAlphaBits = " ) << cAccumAlphaBits << TEXT( "\n" );
   dump_context << TEXT( "   cDepthBits      = " ) << cDepthBits      << TEXT( "\n" );
   dump_context << TEXT( "   cStencilBits    = " ) << cStencilBits    << TEXT( "\n" );
   dump_context << TEXT( "   cAuxBuffers     = " ) << cAuxBuffers     << TEXT( "\n" );
   dump_context << TEXT( "   iLayerType      = " ) << iLayerType      << TEXT( "\n" );
   dump_context << TEXT( "   bReserved       = " ) << bReserved       << TEXT( "\n" );
   dump_context << TEXT( "   dwLayerMask     = " ) << dwLayerMask     << TEXT( "\n" );
   dump_context << TEXT( "   dwVisibleMask   = " ) << dwVisibleMask   << TEXT( "\n" );
   dump_context << TEXT( "   dwDamageMask    = " ) << dwDamageMask    << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CPixelFormatDescriptor::Empty( void ) noexcept
{
   nSize           = sizeof( tagPIXELFORMATDESCRIPTOR );
   nVersion        = 0;
   dwFlags         = 0;
   iPixelType      = 0;
   cColorBits      = 0;
   cRedBits        = 0;
   cRedShift       = 0;
   cGreenBits      = 0;
   cGreenShift     = 0;
   cBlueBits       = 0;
   cBlueShift      = 0;
   cAlphaBits      = 0;
   cAlphaShift     = 0;
   cAccumBits      = 0;
   cAccumRedBits   = 0;
   cAccumGreenBits = 0;
   cAccumBlueBits  = 0;
   cAccumAlphaBits = 0;
   cDepthBits      = 0;
   cStencilBits    = 0;
   cAuxBuffers     = 0;
   iLayerType      = 0;
   bReserved       = 0;
   dwLayerMask     = 0;
   dwVisibleMask   = 0;
   dwDamageMask    = 0;
}

_Check_return_ Win32FoundationClasses::CPixelFormatDescriptor& Win32FoundationClasses::CPixelFormatDescriptor::operator=( _In_ Win32FoundationClasses::CPixelFormatDescriptor const& source ) noexcept
{
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CRasterizerStatus::CRasterizerStatus() noexcept
{
   Empty();
}

void Win32FoundationClasses::CRasterizerStatus::Empty( void ) noexcept
{
   nSize       = sizeof( _RASTERIZER_STATUS );
   wFlags      = 0;
   nLanguageID = 0;
}

Win32FoundationClasses::CSecurityAttributes::CSecurityAttributes() noexcept
{
   Empty();
}

void Win32FoundationClasses::CSecurityAttributes::Empty( void ) noexcept
{
   nLength              = sizeof( SECURITY_ATTRIBUTES );
   lpSecurityDescriptor = nullptr;
   bInheritHandle       = FALSE;
}

Win32FoundationClasses::CSecurityQualityOfService::CSecurityQualityOfService() noexcept
{
   Empty();
}

void Win32FoundationClasses::CSecurityQualityOfService::Empty( void ) noexcept
{
   Length = sizeof( SECURITY_QUALITY_OF_SERVICE );

   // SECURITY_IMPERSONATION_LEVEL
   ImpersonationLevel = SecurityAnonymous;
   
   // SECURITY_CONTEXT_TRACKING_MODE
   ContextTrackingMode = FALSE;

   //BOOLEAN
   EffectiveOnly = FALSE;
}

/*
** CSystemAuditEntry
*/

Win32FoundationClasses::CSystemAuditEntry::CSystemAuditEntry() noexcept
{
   Empty();
}

Win32FoundationClasses::CSystemAuditEntry::CSystemAuditEntry( _In_ Win32FoundationClasses::CSystemAuditEntry const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::CSystemAuditEntry::CSystemAuditEntry( _In_ _SYSTEM_AUDIT_ACE const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::CSystemAuditEntry::Copy(_In_ Win32FoundationClasses::CSystemAuditEntry const& source ) noexcept
{
   Copy( static_cast<_SYSTEM_AUDIT_ACE const *>(&source) );
}

void Win32FoundationClasses::CSystemAuditEntry::Copy(_In_ _SYSTEM_AUDIT_ACE const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      // ACE_HEADER
      Header.AceType  = source->Header.AceType;
      Header.AceFlags = source->Header.AceFlags;
      Header.AceSize  = source->Header.AceSize;

      // ACCESS_MASK
      Mask            = source->Mask;
      SidStart        = source->SidStart;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CSystemAuditEntry::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CSystemAuditEntry at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   Header.AceType  = " ) << Header.AceType  << TEXT( " (" );

   switch( Header.AceType )
   {
      case ACCESS_ALLOWED_ACE_TYPE:

         dump_context << TEXT( "ACCESS_ALLOWED_ACE_TYPE" );
         break;

      case ACCESS_DENIED_ACE_TYPE:

         dump_context << TEXT( "ACCESS_DENIED_ACE_TYPE" );
         break;

      case SYSTEM_AUDIT_ACE_TYPE:

         dump_context << TEXT( "SYSTEM_AUDIT_ACE_TYPE" );
         break;

      case SYSTEM_ALARM_ACE_TYPE:

         dump_context << TEXT( "SYSTEM_ALARM_ACE_TYPE" );
         break;
   }

   dump_context << TEXT( ")\n" );

   dump_context << TEXT( "   Header.AceFlags = " ) << Header.AceFlags << TEXT( " (" );

   std::wstring temp_string;

   if ( Header.AceFlags bitand CONTAINER_INHERIT_ACE )
   {
      temp_string = TEXT( "CONTAINER_INHERIT_ACE" );
   }

   if ( Header.AceFlags bitand INHERIT_ONLY_ACE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "INHERIT_ONLY_ACE" );
   }

   if ( Header.AceFlags bitand NO_PROPAGATE_INHERIT_ACE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "NO_PROPAGATE_INHERIT_ACE" );
   }

   if ( Header.AceFlags bitand OBJECT_INHERIT_ACE )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "OBJECT_INHERIT_ACE" );
   }

   if ( Header.AceFlags bitand FAILED_ACCESS_ACE_FLAG )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "FAILED_ACCESS_ACE_FLAG" );
   }

   if ( Header.AceFlags bitand SUCCESSFUL_ACCESS_ACE_FLAG )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "SUCCESSFUL_ACCESS_ACE_FLAG" );
   }

   dump_context << temp_string << TEXT( ")\n" );

   dump_context << TEXT( "   Header.AceSize  = " ) << Header.AceSize  << TEXT( "\n" );
   dump_context << TEXT( "   Mask            = " ) << Mask            << TEXT( "\n" );
   dump_context << TEXT( "   SidStart        = " ) << SidStart        << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CSystemAuditEntry::Empty( void ) noexcept
{
   // ACE_HEADER
   Header.AceType  = 0;
   Header.AceFlags = 0;
   Header.AceSize  = 0;

   // ACCESS_MASK
   Mask            = 0;

   SidStart        = 0;
}

_Check_return_ Win32FoundationClasses::CSystemAuditEntry& Win32FoundationClasses::CSystemAuditEntry::operator=( _In_ Win32FoundationClasses::CSystemAuditEntry const& source ) noexcept
{
   Copy( source );
   return( *this );
}

/*
** CTextMetricA
*/

Win32FoundationClasses::CTextMetricA::CTextMetricA() noexcept
{
   Empty();
}

Win32FoundationClasses::CTextMetricA::CTextMetricA( _In_ Win32FoundationClasses::CTextMetricA const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::CTextMetricA::CTextMetricA( _In_ tagTEXTMETRICA const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::CTextMetricA::Copy(_In_ Win32FoundationClasses::CTextMetricA const& source ) noexcept
{
   Copy( static_cast<tagTEXTMETRICA const *>(&source) );
}

void Win32FoundationClasses::CTextMetricA::Copy(_In_ tagTEXTMETRICA const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      tmHeight           = source->tmHeight;
      tmAscent           = source->tmAscent;
      tmDescent          = source->tmDescent;
      tmInternalLeading  = source->tmInternalLeading;
      tmExternalLeading  = source->tmExternalLeading;
      tmAveCharWidth     = source->tmAveCharWidth;
      tmMaxCharWidth     = source->tmMaxCharWidth;
      tmWeight           = source->tmWeight;
      tmOverhang         = source->tmOverhang;
      tmDigitizedAspectX = source->tmDigitizedAspectX;
      tmDigitizedAspectY = source->tmDigitizedAspectY;
      tmFirstChar        = source->tmFirstChar;
      tmLastChar         = source->tmLastChar;
      tmDefaultChar      = source->tmDefaultChar;
      tmBreakChar        = source->tmBreakChar;
      tmItalic           = source->tmItalic;
      tmUnderlined       = source->tmUnderlined;
      tmStruckOut        = source->tmStruckOut;
      tmPitchAndFamily   = source->tmPitchAndFamily;
      tmCharSet          = source->tmCharSet;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CTextMetricA::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CTextMetricA at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   tmHeight           = " ) << tmHeight           << TEXT( "\n" );
   dump_context << TEXT( "   tmAscent           = " ) << tmAscent           << TEXT( "\n" );
   dump_context << TEXT( "   tmDescent          = " ) << tmDescent          << TEXT( "\n" );
   dump_context << TEXT( "   tmInternalLeading  = " ) << tmInternalLeading  << TEXT( "\n" );
   dump_context << TEXT( "   tmExternalLeading  = " ) << tmExternalLeading  << TEXT( "\n" );
   dump_context << TEXT( "   tmAveCharWidth     = " ) << tmAveCharWidth     << TEXT( "\n" );
   dump_context << TEXT( "   tmMaxCharWidth     = " ) << tmMaxCharWidth     << TEXT( "\n" );
   dump_context << TEXT( "   tmWeight           = " ) << tmWeight           << TEXT( "\n" );
   dump_context << TEXT( "   tmOverhang         = " ) << tmOverhang         << TEXT( "\n" );
   dump_context << TEXT( "   tmDigitizedAspectX = " ) << tmDigitizedAspectX << TEXT( "\n" );
   dump_context << TEXT( "   tmDigitizedAspectY = " ) << tmDigitizedAspectY << TEXT( "\n" );
   dump_context << TEXT( "   tmFirstChar        = " ) << tmFirstChar        << TEXT( "\n" );
   dump_context << TEXT( "   tmLastChar         = " ) << tmLastChar         << TEXT( "\n" );
   dump_context << TEXT( "   tmDefaultChar      = " ) << tmDefaultChar      << TEXT( "\n" );
   dump_context << TEXT( "   tmBreakChar        = " ) << tmBreakChar        << TEXT( "\n" );
   dump_context << TEXT( "   tmItalic           = " ) << tmItalic           << TEXT( "\n" );
   dump_context << TEXT( "   tmUnderlined       = " ) << tmUnderlined       << TEXT( "\n" );
   dump_context << TEXT( "   tmStruckOut        = " ) << tmStruckOut        << TEXT( "\n" );
   dump_context << TEXT( "   tmPitchAndFamily   = " ) << tmPitchAndFamily   << TEXT( "\n" );
   dump_context << TEXT( "   tmCharSet          = " ) << tmCharSet          << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CTextMetricA::Empty( void ) noexcept
{
   tmHeight           = 0;
   tmAscent           = 0;
   tmDescent          = 0;
   tmInternalLeading  = 0;
   tmExternalLeading  = 0;
   tmAveCharWidth     = 0;
   tmMaxCharWidth     = 0;
   tmWeight           = 0;
   tmOverhang         = 0;
   tmDigitizedAspectX = 0;
   tmDigitizedAspectY = 0;
   tmFirstChar        = 0;
   tmLastChar         = 0;
   tmDefaultChar      = 0;
   tmBreakChar        = 0;
   tmItalic           = 0;
   tmUnderlined       = 0;
   tmStruckOut        = 0;
   tmPitchAndFamily   = 0;
   tmCharSet          = 0;
}

_Check_return_ Win32FoundationClasses::CTextMetricA& Win32FoundationClasses::CTextMetricA::operator=( _In_ Win32FoundationClasses::CTextMetricA const& source ) noexcept
{
   Copy( source );
   return( *this );
}

/*
** CTextMetricW
*/

Win32FoundationClasses::CTextMetricW::CTextMetricW() noexcept
{
   Empty();
}

Win32FoundationClasses::CTextMetricW::CTextMetricW( _In_ Win32FoundationClasses::CTextMetricW const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::CTextMetricW::CTextMetricW( _In_ tagTEXTMETRICW const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::CTextMetricW::Copy(_In_ Win32FoundationClasses::CTextMetricW const& source ) noexcept
{
   Copy( static_cast<tagTEXTMETRICW const *>(&source) );
}

void Win32FoundationClasses::CTextMetricW::Copy(_In_ tagTEXTMETRICW const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      tmHeight           = source->tmHeight;
      tmAscent           = source->tmAscent;
      tmDescent          = source->tmDescent;
      tmInternalLeading  = source->tmInternalLeading;
      tmExternalLeading  = source->tmExternalLeading;
      tmAveCharWidth     = source->tmAveCharWidth;
      tmMaxCharWidth     = source->tmMaxCharWidth;
      tmWeight           = source->tmWeight;
      tmOverhang         = source->tmOverhang;
      tmDigitizedAspectX = source->tmDigitizedAspectX;
      tmDigitizedAspectY = source->tmDigitizedAspectY;
      tmFirstChar        = source->tmFirstChar;
      tmLastChar         = source->tmLastChar;
      tmDefaultChar      = source->tmDefaultChar;
      tmBreakChar        = source->tmBreakChar;
      tmItalic           = source->tmItalic;
      tmUnderlined       = source->tmUnderlined;
      tmStruckOut        = source->tmStruckOut;
      tmPitchAndFamily   = source->tmPitchAndFamily;
      tmCharSet          = source->tmCharSet;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CTextMetricW::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CTextMetricW at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   tmHeight           = " ) << tmHeight           << TEXT( "\n" );
   dump_context << TEXT( "   tmAscent           = " ) << tmAscent           << TEXT( "\n" );
   dump_context << TEXT( "   tmDescent          = " ) << tmDescent          << TEXT( "\n" );
   dump_context << TEXT( "   tmInternalLeading  = " ) << tmInternalLeading  << TEXT( "\n" );
   dump_context << TEXT( "   tmExternalLeading  = " ) << tmExternalLeading  << TEXT( "\n" );
   dump_context << TEXT( "   tmAveCharWidth     = " ) << tmAveCharWidth     << TEXT( "\n" );
   dump_context << TEXT( "   tmMaxCharWidth     = " ) << tmMaxCharWidth     << TEXT( "\n" );
   dump_context << TEXT( "   tmWeight           = " ) << tmWeight           << TEXT( "\n" );
   dump_context << TEXT( "   tmOverhang         = " ) << tmOverhang         << TEXT( "\n" );
   dump_context << TEXT( "   tmDigitizedAspectX = " ) << tmDigitizedAspectX << TEXT( "\n" );
   dump_context << TEXT( "   tmDigitizedAspectY = " ) << tmDigitizedAspectY << TEXT( "\n" );
   dump_context << TEXT( "   tmFirstChar        = " ) << tmFirstChar        << TEXT( "\n" );
   dump_context << TEXT( "   tmLastChar         = " ) << tmLastChar         << TEXT( "\n" );
   dump_context << TEXT( "   tmDefaultChar      = " ) << tmDefaultChar      << TEXT( "\n" );
   dump_context << TEXT( "   tmBreakChar        = " ) << tmBreakChar        << TEXT( "\n" );
   dump_context << TEXT( "   tmItalic           = " ) << tmItalic           << TEXT( "\n" );
   dump_context << TEXT( "   tmUnderlined       = " ) << tmUnderlined       << TEXT( "\n" );
   dump_context << TEXT( "   tmStruckOut        = " ) << tmStruckOut        << TEXT( "\n" );
   dump_context << TEXT( "   tmPitchAndFamily   = " ) << tmPitchAndFamily   << TEXT( "\n" );
   dump_context << TEXT( "   tmCharSet          = " ) << tmCharSet          << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CTextMetricW::Empty( void ) noexcept
{
   tmHeight           = 0;
   tmAscent           = 0;
   tmDescent          = 0;
   tmInternalLeading  = 0;
   tmExternalLeading  = 0;
   tmAveCharWidth     = 0;
   tmMaxCharWidth     = 0;
   tmWeight           = 0;
   tmOverhang         = 0;
   tmDigitizedAspectX = 0;
   tmDigitizedAspectY = 0;
   tmFirstChar        = 0;
   tmLastChar         = 0;
   tmDefaultChar      = 0;
   tmBreakChar        = 0;
   tmItalic           = 0;
   tmUnderlined       = 0;
   tmStruckOut        = 0;
   tmPitchAndFamily   = 0;
   tmCharSet          = 0;
}

_Check_return_ Win32FoundationClasses::CTextMetricW& Win32FoundationClasses::CTextMetricW::operator=( _In_ Win32FoundationClasses::CTextMetricW const& source ) noexcept
{
   Copy( source );
   return( *this );
}

Win32FoundationClasses::CWindowPlacement::CWindowPlacement() noexcept
{
   Empty();
}

Win32FoundationClasses::CWindowPlacement::CWindowPlacement( _In_ Win32FoundationClasses::CWindowPlacement const& source ) noexcept
{
   Empty();
   Copy( source );
}

Win32FoundationClasses::CWindowPlacement::CWindowPlacement( _In_ tagWINDOWPLACEMENT const * source ) noexcept
{
   Empty();
   Copy( source );
}

void Win32FoundationClasses::CWindowPlacement::Copy(_In_ Win32FoundationClasses::CWindowPlacement const& source ) noexcept
{
   Copy( static_cast<tagWINDOWPLACEMENT const *>(&source) );
}

void Win32FoundationClasses::CWindowPlacement::Copy(_In_ tagWINDOWPLACEMENT const * source ) noexcept
{
   WFC_VALIDATE_POINTER( source );
   ASSERT( source not_eq this );

   if ( source == nullptr or source == this )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      length  = source->length;
      flags   = source->flags;
      showCmd = source->showCmd;

      // POINT
      ptMinPosition.x = source->ptMinPosition.x;
      ptMinPosition.y = source->ptMinPosition.y;
      ptMaxPosition.x = source->ptMaxPosition.x;
      ptMaxPosition.y = source->ptMaxPosition.y;

      // RECT
      rcNormalPosition.left   = source->rcNormalPosition.left;
      rcNormalPosition.top    = source->rcNormalPosition.top;
      rcNormalPosition.right  = source->rcNormalPosition.right;
      rcNormalPosition.bottom = source->rcNormalPosition.bottom;
   }
   WFC_CATCH_ALL
   {
      Empty();
   }
   WFC_END_CATCH_ALL
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void Win32FoundationClasses::CWindowPlacement::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " a CWindowPlacement at " ) << (void *) this << TEXT( "\n" );
   dump_context << TEXT( "{\n" );
   dump_context << TEXT( "   length                  = " ) << length                  << TEXT( "\n" );
   dump_context << TEXT( "   flags                   = " ) << flags                   << TEXT( " (" );

   std::wstring temp_string;

   if ( flags bitand WPF_SETMINPOSITION )
   {
      temp_string = TEXT( "WPF_SETMINPOSITION" );
   }

   if ( flags bitand WPF_RESTORETOMAXIMIZED )
   {
      if ( temp_string.GetLength() > 0 )
      {
         temp_string += TEXT( " | " );
      }

      temp_string += TEXT( "WPF_RESTORETOMAXIMIZED" );
   }

   dump_context << temp_string << TEXT( ")\n" );

   dump_context << TEXT( "   showCmd                 = " ) << showCmd                 << TEXT( " (" );

   temp_string.Empty();

   switch( showCmd )
   {
      case SW_HIDE:

         temp_string = TEXT( "SW_HIDE" );
         break;

      case SW_MINIMIZE:

         temp_string = TEXT( "SW_MINIMIZE" );
         break;

      case SW_RESTORE:

         temp_string = TEXT( "SW_RESTORE" );
         break;

      case SW_SHOW:

         temp_string = TEXT( "SW_SHOW" );
         break;

      case SW_SHOWMAXIMIZED:

         temp_string = TEXT( "SW_SHOWMAXIMIZED" );
         break;

      case SW_SHOWMINIMIZED:

         temp_string = TEXT( "SW_SHOWMINIMIZED" );
         break;

      case SW_SHOWMINNOACTIVE:

         temp_string = TEXT( "SW_SHOWMINNOACTIVE" );
         break;

      case SW_SHOWNA:

         temp_string = TEXT( "SW_SHOWNA" );
         break;

      case SW_SHOWNOACTIVATE:

         temp_string = TEXT( "SW_SHOWNOACTIVATE" );
         break;

      case SW_SHOWNORMAL:

         temp_string = TEXT( "SW_SHOWNORMAL" );
         break;
   }

   dump_context << temp_string << TEXT( ")\n" );

   dump_context << TEXT( "   ptMinPosition.x         = " ) << ptMinPosition.x         << TEXT( "\n" );
   dump_context << TEXT( "   ptMinPosition.y         = " ) << ptMinPosition.y         << TEXT( "\n" );
   dump_context << TEXT( "   ptMaxPosition.x         = " ) << ptMaxPosition.x         << TEXT( "\n" );
   dump_context << TEXT( "   ptMaxPosition.y         = " ) << ptMaxPosition.y         << TEXT( "\n" );
   dump_context << TEXT( "   rcNormalPosition.left   = " ) << rcNormalPosition.left   << TEXT( "\n" );
   dump_context << TEXT( "   rcNormalPosition.top    = " ) << rcNormalPosition.top    << TEXT( "\n" );
   dump_context << TEXT( "   rcNormalPosition.right  = " ) << rcNormalPosition.right  << TEXT( "\n" );
   dump_context << TEXT( "   rcNormalPosition.bottom = " ) << rcNormalPosition.bottom << TEXT( "\n" );
   dump_context << TEXT( "}\n" );
}

#endif // _DEBUG

void Win32FoundationClasses::CWindowPlacement::Empty( void ) noexcept
{
   length  = sizeof( tagWINDOWPLACEMENT );
   flags   = 0;
   showCmd = 0;

   // POINT
   ptMinPosition.x = 0;
   ptMinPosition.y = 0;
   ptMaxPosition.x = 0;
   ptMaxPosition.y = 0;

   // RECT
   rcNormalPosition.left   = 0;
   rcNormalPosition.top    = 0;
   rcNormalPosition.right  = 0;
   rcNormalPosition.bottom = 0;
}

_Check_return_ Win32FoundationClasses::CWindowPlacement& Win32FoundationClasses::CWindowPlacement::operator=( _In_ Win32FoundationClasses::CWindowPlacement const& source ) noexcept
{
   Copy( source );
   return( *this );
}

// End of source
