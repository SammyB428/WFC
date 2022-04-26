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
** $Workfile: CConstellation.cpp $
** $Revision: 15 $
** $Modtime: 6/26/01 10:44a $
** $Reuse Tracing Code: 1 $
*/

#if ! defined( WFC_STL )

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

USING_WFC_NAMESPACE

#if ! defined(  WFC_NO_SERIALIZATION )
IMPLEMENT_SERIAL( CConstellationData, CObject, 1 )
IMPLEMENT_SERIAL( CConstellation, CRectangle, 1 )
#endif // WFC_NO_SERIALIZATION

#if defined( _DEBUG )
#define new DEBUG_NEW
#endif // _DEBUG

/*
** The data that will be displayed in a CConstellation...
*/

// Construction

CConstellationData::CConstellationData()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CConstellationData::CConstellationData( const CConstellationData * source )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source );

   m_Initialize();
   Copy( source );
}

CConstellationData::CConstellationData( const CConstellationData& source )
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
   Copy( source );
}

CConstellationData::~CConstellationData()
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( Data );

   if ( Data != NULL )
   {
      delete [] Data;
      Data = NULL;
   }

   m_Initialize();
}

// Methods

void CConstellationData::Copy( const CConstellationData& source )
{
   WFC_VALIDATE_POINTER( this );

   // Copying ourself is a silly thing to do

   if ( &source == this )
   {
      return;
   }

   Start              = source.Start;
   Stop               = source.Stop;
   Time               = source.Time;
   MinimumWidthValue  = source.MinimumWidthValue;
   MaximumWidthValue  = source.MaximumWidthValue;
   MinimumHeightValue = source.MinimumHeightValue;
   MaximumHeightValue = source.MaximumHeightValue;

   // See if our memory area is already large enough to hold all of the incoming so avoid a malloc
   
   if ( NumberOfPoints < source.NumberOfPoints )
   {
      // DANG! We gotta malloc

      delete [] Data;
      Data = NULL;

      WFC_TRY
      {
         Data = new POINT[ source.NumberOfPoints ];
      }
      WFC_CATCH_ALL
      {
         Data = NULL;
      }
      WFC_END_CATCH_ALL

      if ( Data == NULL )
      {
         return;
      }
   }

   ::memcpy( Data, source.Data, source.NumberOfPoints * sizeof( POINT ) );
   NumberOfPoints = source.NumberOfPoints;
}

void CConstellationData::Copy( const CConstellationData *source_p )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source_p );

   // Copying ourself is a silly thing to do

   if ( source_p == this )
   {
      return;
   }

   // A pointer has been passed to us, this cannot be trusted

   WFC_TRY
   {
      Copy( *source_p );
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void CConstellationData::Empty( void )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( Data );

   if ( Data != NULL )
   {
      delete [] Data;
      Data = NULL;
   }

   m_Initialize();
}

void CConstellationData::m_Initialize( void )
{
   WFC_VALIDATE_POINTER( this );
   Start.Empty();
   Stop.Empty();
   Time = CTime( 0 );
   MinimumWidthValue  = 0;
   MaximumWidthValue  = 0;
   MinimumHeightValue = 0;
   MaximumHeightValue = 0;
   NumberOfPoints     = 0;
   Data               = NULL;
}

#if ! defined(  WFC_NO_SERIALIZATION )

void CConstellationData::Serialize( CArchive& archive )
{
   CObject::Serialize( archive );

   if ( archive.IsStoring() )
   {
      archive << Start;
      archive << Stop;
      archive << Time;
      archive << MinimumWidthValue;
      archive << MaximumWidthValue;
      archive << MinimumHeightValue;
      archive << MaximumHeightValue;
      archive << NumberOfPoints;
   }
   else
   {
      archive >> Start;
      archive >> Stop;
      archive >> Time;
      archive >> MinimumWidthValue;
      archive >> MaximumWidthValue;
      archive >> MinimumHeightValue;
      archive >> MaximumHeightValue;
      archive >> NumberOfPoints;
   }

   // Data.Serialize( archive );
}

#endif // WFC_NO_SERIALIZATION

void CConstellationData::SetSize( DWORD number_of_points )
{
   WFC_VALIDATE_POINTER( this );

   if ( number_of_points == 0 )
   {
      delete [] Data;
      Data = NULL;
   }
   else if ( number_of_points > NumberOfPoints )
   {
      // We tried to avoid this but . . .

      delete [] Data;
      Data = NULL;

      WFC_TRY
      {
         Data = new POINT[ number_of_points ];
      }
      WFC_CATCH_ALL
      {
         Data = NULL;
      }
      WFC_END_CATCH_ALL
   }

   NumberOfPoints = number_of_points;
}

DWORD CConstellationData::ValidHeight( void )
{
   WFC_VALIDATE_POINTER( this );

   if ( MinimumHeightValue > MaximumHeightValue )
   {
      DWORD temp_word = MaximumHeightValue;
      MaximumHeightValue = MinimumHeightValue;
      MinimumHeightValue = temp_word;
   }

   return( ( MaximumHeightValue - MinimumHeightValue ) );
}

DWORD CConstellationData::ValidWidth( void )
{
   WFC_VALIDATE_POINTER( this );

   if ( MinimumWidthValue > MaximumWidthValue )
   {
      DWORD temp_word = MaximumWidthValue;
      MaximumWidthValue = MinimumWidthValue;
      MinimumWidthValue = temp_word;
   }

   return( ( MaximumWidthValue - MinimumWidthValue ) );
}

// Operators

CConstellationData& CConstellationData::operator=( const CConstellationData& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** The Squiggle that you see on the screen
*/

CConstellation::CConstellation()
{
   WFC_VALIDATE_POINTER( this );
   m_AutomaticallyDelete = FALSE;
   m_PointArray          = (POINT *) NULL;
   m_Initialize();
}

CConstellation::CConstellation( DWORD height, DWORD width, const CPoint& location, COLORREF fill_color, COLORREF line_color )
          :CRectangle( height, width, location, fill_color, line_color )
{
   WFC_VALIDATE_POINTER( this );
   m_AutomaticallyDelete = FALSE;
   m_PointArray          = (POINT *) NULL;
   m_Initialize();
}

CConstellation::~CConstellation()
{
   WFC_VALIDATE_POINTER( this );

   WFC_TRY
   {
      if ( m_PointArray != (POINT *) NULL )
      {
         delete [] m_PointArray;
         m_PointArray = (POINT *) NULL;
      }

      if ( m_AutomaticallyDelete != FALSE )
      {
         delete m_Data_p;
      }

      m_Initialize();
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void CConstellation::Copy( const CConstellation *source_p )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source_p );

   // A pointer has been passed to us, do not trust it

   WFC_TRY
   {
      m_Initialize();

      CConstellationData *data_p = NULL;

      WFC_TRY
      {
         data_p = new CConstellationData;
      }
      WFC_CATCH_ALL
      {
         data_p = NULL;
      }
      WFC_END_CATCH_ALL

      if ( data_p == NULL )
      {
         return;
      }

      data_p->Copy( source_p->m_Data_p );

      SetData( data_p, TRUE );

      CRectangle::Copy( source_p );
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void CConstellation::Draw( CDC& device_context )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( m_Data_p );

   if ( m_Data_p == (CConstellationData *) NULL )
   {
      ASSERT( FALSE );
      return;
   }

   if ( m_DataIsDirty != FALSE )
   {
      PrepareData();
   }

   CBitmap bitmap;

   CDC temporary_device_context;

   CBrush brush( m_FillColor );

   DWORD width{ GetWidth() };
   DWORD height{ GetHeight() };

   BOOL result = bitmap.CreateCompatibleBitmap( &device_context, (int) width, (int) height );

   if ( result == 0 )
   {
      ASSERT( FALSE );
      return;
   }

   result = temporary_device_context.CreateCompatibleDC( &device_context );

   if ( result == 0 )
   {
      ASSERT( FALSE );
      return;
   }

   CBitmap *original_bitmap = temporary_device_context.SelectObject( &bitmap );

   temporary_device_context.FillRect( CRect( 0, 0, (int) width, (int) height ), &brush );

   /*
   ** Draw the grid if we need to
   */

   if ( m_NumberOfXGridLines > 0 || m_NumberOfYGridLines > 0 )
   {
      CPen grid_line_pen( (int) m_GridLineType, 1, m_GridLineColor );

      CPen *original_pen = temporary_device_context.SelectObject( &grid_line_pen );

      /*
      ** Now turn the background color into the same color as our FillRect to prevent
      ** dotted pens from being color-white-color...
      */

      COLORREF original_background_color = temporary_device_context.SetBkColor( m_FillColor );

      int index = 0;

      for( index = 1; index < m_NumberOfXGridLines; index++ )
      {
         temporary_device_context.MoveTo( (int) ( index * width / m_NumberOfXGridLines ), 0            );
         temporary_device_context.LineTo( (int) ( index * width / m_NumberOfXGridLines ), (int) height );
      }

      for( index = 1; index < m_NumberOfYGridLines; index++ )
      {
         temporary_device_context.MoveTo( 0,           (int) ( index * height / m_NumberOfYGridLines ) );
         temporary_device_context.LineTo( (int) width, (int) ( index * height / m_NumberOfYGridLines ) );
      }

      temporary_device_context.SetBkColor( original_background_color );
      temporary_device_context.SelectObject( original_pen );
      grid_line_pen.DeleteObject();
   }

   CPen line_pen( PS_SOLID, (int) m_LineThickness, m_LineColor );

   CPen *original_pen = temporary_device_context.SelectObject( &line_pen );

   DWORD index = 0;

   while( index < m_NumberOfPoints )
   {
      temporary_device_context.SetPixelV( m_PointArray[ index ], m_LineColor );
      index++;
   }

   /*
   ** Copy to the screen
   */

   device_context.BitBlt( m_Location.x, m_Location.y, (int) width, (int) height, &temporary_device_context, 0, 0, SRCCOPY );

   /*
   ** Clean up
   */

   temporary_device_context.SelectObject( original_pen    );
   temporary_device_context.SelectObject( original_bitmap );
   line_pen.DeleteObject();
   bitmap.DeleteObject();
}

void CConstellation::Empty( void )
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CConstellationData * CConstellation::GetData( void ) const
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER_NULL_OK( m_Data_p );
   return( m_Data_p );
}

DWORD CConstellation::GetLineThickness( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( m_LineThickness );
}

void CConstellation::m_Initialize( void )
{
   WFC_VALIDATE_POINTER( this );
   m_Data_p             = (CConstellationData *) NULL;
   m_PointArray         = (POINT *) NULL;
   m_NumberOfPoints     = 0;
   m_NumberOfXGridLines = 0;
   m_NumberOfYGridLines = 0;
   m_GridLineColor      = DARK_GREEN;
   m_LineThickness      = 1;
   m_GridLineType       = PS_DOT;
   m_DataIsDirty        = TRUE;
}

void CConstellation::PrepareData( void )
{
   WFC_VALIDATE_POINTER( this );

   if ( m_PointArray == (POINT *) NULL )
   {
      return;
   }

   ASSERT( m_Data_p != NULL );

   if ( m_Data_p == NULL )
   {
      return;
   }

   int number_of_data_points = m_NumberOfPoints;

   // Avoid divide by zero errors

   if ( number_of_data_points == 0 )
   {
      number_of_data_points = 1;
   }

   DWORD valid_width  = m_Data_p->ValidWidth();
   DWORD valid_height = m_Data_p->ValidHeight();


   // Avoid divide by zero errors

   if ( valid_width == 0 )
   {
      valid_width = 1;
   }

   if ( valid_height == 0 )
   {
      valid_height = 1;
   }

   double x_ratio{ (double)m_Width / (double)valid_width };
   double y_ratio{ (double)m_Height / (double)valid_height };

   int origin_y{ (int)((double)m_Data_p->MaximumHeightValue * y_ratio) };

   for ( DWORD index = 0; index < m_NumberOfPoints; index++ )
   {
      m_PointArray[ index ].x = (int) ( (double) ( m_Data_p->Data[ index ].x ) * x_ratio );
      m_PointArray[ index ].y = origin_y - (int) ( (double) ( m_Data_p->Data[ index ].y ) * y_ratio );
   }

   m_DataIsDirty = FALSE;
}

#if ! defined(  WFC_NO_SERIALIZATION )

void CConstellation::Serialize( CArchive& archive )
{
   CRectangle::Serialize( archive );

   if ( archive.IsStoring() )
   {
      archive << m_NumberOfXGridLines;
      archive << m_NumberOfYGridLines;
      archive << m_GridLineColor;
      archive << m_LineThickness;
      archive << m_GridLineType;

      DWORD do_we_have_data = 0;

      if ( m_Data_p != (CConstellationData *) NULL )
      {
         do_we_have_data = 1;
      }

      archive << do_we_have_data;

      if ( do_we_have_data == 1 )
      {
         m_Data_p->Serialize( archive );
      }
   }
   else
   {
      archive >> m_NumberOfXGridLines;
      archive >> m_NumberOfYGridLines;
      archive >> m_GridLineColor;
      archive >> m_LineThickness;
      archive >> m_GridLineType;

      DWORD is_there_data = 0;

      archive >> is_there_data;

      if ( is_there_data == 1 )
      {
         CConstellationData *data_p = NULL;

         WFC_TRY
         {
            data_p = new CConstellationData;
         }
         WFC_CATCH_ALL
         {
            data_p = NULL;
         }
         WFC_END_CATCH_ALL

         if ( data_p != NULL )
         {
            data_p->Serialize( archive );

            SetData( data_p, TRUE );
         }
      }
   }
}

#endif // WFC_NO_SERIALIZATION

void CConstellation::SetAutomaticallyDelete( BOOL new_value )
{
   WFC_VALIDATE_POINTER( this );

   if ( new_value != FALSE )
   {
      m_AutomaticallyDelete = TRUE;
   }
   else
   {
      m_AutomaticallyDelete = FALSE;
   }
}

void CConstellation::SetDirtyData( BOOL new_value )
{
   WFC_VALIDATE_POINTER( this );

   if ( new_value != FALSE )
   {
      m_DataIsDirty = TRUE;
   }
   else
   {
      m_DataIsDirty = FALSE;
   }
}

void CConstellation::SetGridLineColor( COLORREF grid_line_color )
{
   WFC_VALIDATE_POINTER( this );
   m_GridLineColor = grid_line_color;
}

void CConstellation::SetGridLineType( DWORD type )
{
   WFC_VALIDATE_POINTER( this );
   m_GridLineType = type;
}

void CConstellation::SetHeight( DWORD height )
{
   WFC_VALIDATE_POINTER( this );
   CRectangle::SetHeight( height );
   m_DataIsDirty = TRUE;
}

void CConstellation::SetLineThickness( DWORD thickness )
{
   WFC_VALIDATE_POINTER( this );
   m_LineThickness = thickness;
}

void CConstellation::SetNumberOfGridLines( WORD number_of_x_lines, WORD number_of_y_lines )
{
   WFC_VALIDATE_POINTER( this );
   m_NumberOfXGridLines = number_of_x_lines;
   m_NumberOfYGridLines = number_of_y_lines;
}

void CConstellation::SetSize( const CSize& size )
{
   WFC_VALIDATE_POINTER( this );
   CRectangle::SetSize( size );
   m_DataIsDirty = TRUE;
}

void CConstellation::SetData( CConstellationData *source_p, BOOL auto_delete )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source_p );

   // We have been passed a pointer, do not trust it

   WFC_TRY
   {
      if ( m_PointArray != (POINT *) NULL )
      {
         delete [] m_PointArray;
         m_PointArray = (POINT *) NULL;
      }
 
      m_NumberOfPoints = 0;

      if ( m_AutomaticallyDelete != FALSE )
      {
         delete m_Data_p;
         m_Data_p = (CConstellationData *) NULL;
         m_AutomaticallyDelete = FALSE;
      }

      if ( source_p != (CConstellationData *) NULL )
      {
         m_Data_p      = source_p;
         m_AutomaticallyDelete = auto_delete;

         m_NumberOfPoints = source_p->NumberOfPoints;

         WFC_TRY
         {
            m_PointArray = new POINT[ m_NumberOfPoints ];
         }
         WFC_CATCH_ALL
         {
            m_PointArray = NULL;
         }
         WFC_END_CATCH_ALL

         if ( m_PointArray == (POINT *) NULL )
         {
            /*
            ** Out of Memory
            */

            m_NumberOfPoints = 0;
            return;
         }

         m_DataIsDirty = TRUE;
      }
   }
   WFC_CATCH_ALL
   {
      m_Initialize();
   }
   WFC_END_CATCH_ALL
}

void CConstellation::SetWidth( DWORD width )
{
   WFC_VALIDATE_POINTER( this );
   CRectangle::SetWidth( width );
   m_DataIsDirty = TRUE;
}

#endif // WFC_STL

// End of source
