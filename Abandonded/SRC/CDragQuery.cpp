/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2003, Samuel R. Blackburn
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
** $Workfile: CDragQuery.cpp $
** $Revision: 5 $
** $Modtime: 6/26/01 10:45a $
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

CDragQuery::CDragQuery( HDROP handle )
{
   WFCLTRACEINIT( TEXT( "CDragQuery::CDragQuery()" ) );
   WFC_VALIDATE_POINTER( this );
   m_Handle = handle;
}

CDragQuery::CDragQuery( const CDragQuery& source )
{
   WFCLTRACEINIT( TEXT( "CDragQuery::CDragQuery( CDragQuery )" ) );
   WFC_VALIDATE_POINTER( this );
   m_Handle = NULL;
   Copy( source );
}

CDragQuery::~CDragQuery()
{
   WFCLTRACEINIT( TEXT( "CDragQuery::~CDragQuery()" ) );
   WFC_VALIDATE_POINTER( this );
   Finish();
}

void CDragQuery::Copy( const CDragQuery& source )
{
   WFCLTRACEINIT( TEXT( "CDragQuery::Copy()" ) );
   WFC_VALIDATE_POINTER( this );

   Finish();
   SetHandle( source.GetHandle() );
}

void CDragQuery::Finish( void )
{
   WFCLTRACEINIT( TEXT( "CDragQuery::Finish()" ) );
   WFC_VALIDATE_POINTER( this );

   if ( m_Handle != (HDROP) NULL )
   {
      ::DragFinish( m_Handle );
      m_Handle = (HDROP) NULL;
   }
}

void CDragQuery::GetFilenames( CStringArray& filenames ) const
{
   WFCLTRACEINIT( TEXT( "CDragQuery::GetFilenames()" ) );
   WFC_VALIDATE_POINTER( this );

   filenames.RemoveAll();

   if ( m_Handle == (HDROP) NULL )
   {
      WFCTRACE( TEXT( "SetHandle() has not yet been called." ) );
      return;
   }

   UINT number_of_files = GetNumberOfFiles();
   UINT index           = 0;

   TCHAR filename[ _MAX_PATH ];

   while( index < number_of_files )
   {
      ZeroMemory( filename, sizeof( filename ) );

      if ( ::DragQueryFile( m_Handle, index, filename, DIMENSION_OF( filename ) ) > 0 )
      {
         filenames.Add( filename );
      }

      index++;
   }
}

HDROP CDragQuery::GetHandle( void ) const
{
   WFCLTRACEINIT( TEXT( "CDragQuery::GetHandle()" ) );
   WFC_VALIDATE_POINTER( this );
   return( m_Handle );
}

UINT CDragQuery::GetNumberOfFiles( void ) const
{
   WFCLTRACEINIT( TEXT( "CDragQuery::GetNumberOfFiles()" ) );
   WFC_VALIDATE_POINTER( this );

   if ( m_Handle == NULL )
   {
      return( 0 );
   }

   UINT return_value = 0;

   return_value = ::DragQueryFile( m_Handle, 0xFFFFFFFF, NULL, 0 );

   return( return_value );
}

BOOL CDragQuery::GetPoint( POINT& point ) const
{
   WFCLTRACEINIT( TEXT( "CDragQuery::GetPoint()" ) );
   WFC_VALIDATE_POINTER( this );

   point.x = 0;
   point.y = 0;

   if ( m_Handle == NULL )
   {
      return( FALSE );
   }

   if ( ::DragQueryPoint( m_Handle, &point ) == FALSE )
   {
      WFCTRACE( TEXT( "DragQueryPoint() failed")  );
      WFCTRACEERROR( GetLastError() );
      point.x = 0;
      point.y = 0;
      return( FALSE );
   }

   return( TRUE );
}

void CDragQuery::SetHandle( HDROP handle )
{
   WFCLTRACEINIT( TEXT( "CDragQuery::SetHandle()" ) );
   WFC_VALIDATE_POINTER( this );
   m_Handle = handle;
}

CDragQuery& CDragQuery::operator=( const CDragQuery& source )
{
   WFCLTRACEINIT( TEXT( "CDragQuery::operator=()" ) );
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

// End of source
