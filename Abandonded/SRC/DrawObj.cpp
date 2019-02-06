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
** $Workfile: drawobj.cpp $
** $Revision: 15 $
** $Modtime: 6/26/01 10:53a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if ! defined( WFC_STL )

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

USING_WFC_NAMESPACE

#if ! defined( WFC_NO_SERIALIZATION )
IMPLEMENT_SERIAL( CRectangle,        CObject,    1 )
IMPLEMENT_SERIAL( CRoundedRectangle, CRectangle, 1 )
IMPLEMENT_SERIAL( CSquare,           CRectangle, 1 )
IMPLEMENT_SERIAL( CEllipse,          CRectangle, 1 )
IMPLEMENT_SERIAL( CCircle,           CSquare,    1 )
#endif // WFC_NO_SERIALIZATION


#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#endif // _DEBUG

/*
** A Rectangle drawing object
*/

CRectangle::CRectangle()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CRectangle::CRectangle( const CRectangle& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
}

CRectangle::CRectangle( DWORD height, DWORD width, const CPoint& location, COLORREF fill_color, COLORREF line_color )
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();

   SetHeight( height );
   SetWidth( width );
   SetLocation( location );
   SetFillColor( fill_color );
   SetLineColor( line_color );
}

CRectangle::~CRectangle()
{
   WFC_VALIDATE_POINTER( this );
   SetLocation( CPoint( 0, 0 ) );
}

void CRectangle::Copy( const CRectangle& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( &source );
}

void CRectangle::Copy( const CRectangle * source_p )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source_p );

   m_Height        = source_p->m_Height;
   m_Width         = source_p->m_Width;
   m_LineColor     = source_p->m_LineColor;
   m_FillColor     = source_p->m_FillColor;
   m_Location      = source_p->m_Location;
   m_LineRectangle = source_p->m_LineRectangle;
   m_FillRectangle = source_p->m_FillRectangle;
}

void CRectangle::Draw( CDC& device_context )
{
   WFC_VALIDATE_POINTER( this );
   CBrush line_brush( m_LineColor );
   CBrush fill_brush( m_FillColor );

   device_context.FrameRect( m_LineRectangle, &line_brush );
   device_context.FillRect(  m_FillRectangle, &fill_brush );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CRectangle::Dump( CDumpContext& dump_context ) const
{
   CObject::Dump( dump_context );

   dump_context << TEXT( "m_Height = "         ) << m_Height        << TEXT( "\n" );
   dump_context << TEXT( "m_Width = "          ) << m_Width         << TEXT( "\n" );
   dump_context << TEXT( "m_LineColor = RGB( " ) << GetRValue( m_LineColor ) << TEXT( ", " ) << GetGValue( m_LineColor ) << TEXT( ", " ) << GetBValue( m_LineColor ) << TEXT( " )\n" );
   dump_context << TEXT( "m_FillColor = RGB( " ) << GetRValue( m_FillColor ) << TEXT( ", " ) << GetGValue( m_FillColor ) << TEXT( ", " ) << GetBValue( m_FillColor ) << TEXT( " )\n" );
   dump_context << TEXT( "m_Location = "       ) << m_Location      << TEXT( "\n" );
   dump_context << TEXT( "m_LineRectangle = "  ) << m_LineRectangle << TEXT( "\n" );
   dump_context << TEXT( "m_FillRectangle = "  ) << m_FillRectangle << TEXT( "\n" );
}

#endif // _DEBUG

COLORREF CRectangle::GetFillColor( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( m_FillColor );
}

DWORD CRectangle::GetHeight( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( m_Height );
}

COLORREF CRectangle::GetLineColor( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( m_LineColor );
}

void CRectangle::GetRectangle( CRect& destination ) const
{
   WFC_VALIDATE_POINTER( this );
   destination = m_LineRectangle;
}

DWORD CRectangle::GetWidth( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( m_Width );
}

void CRectangle::m_Initialize( void )
{
   WFC_VALIDATE_POINTER( this );
   m_Location.x = 0;
   m_Location.y = 0;
   m_LineRectangle.SetRectEmpty();
   m_FillRectangle.SetRectEmpty();
   SetFillColor( WHITE );
   SetLineColor( BLACK );
}

void CRectangle::m_SetRectangles( void )
{
   WFC_VALIDATE_POINTER( this );
   m_LineRectangle = CRect( m_Location.x, m_Location.y, m_Location.x + m_Width, m_Location.y + m_Height );
   m_FillRectangle = m_LineRectangle;
   m_FillRectangle.InflateRect( -1, -1 );
}

void CRectangle::OnClick( void )
{
   WFC_VALIDATE_POINTER( this );
   return;
}

#if ! defined(  WFC_NO_SERIALIZATION )

void CRectangle::Serialize( CArchive& archive )
{
   CObject::Serialize( archive );

   if ( archive.IsStoring() )
   {
      archive << m_Height;
      archive << m_Width;
      archive << m_LineColor;
      archive << m_FillColor;
      archive << m_Location;
   }
   else
   {
      archive >> m_Height;
      archive >> m_Width;
      archive >> m_LineColor;
      archive >> m_FillColor;
      archive >> m_Location;

      SetHeight( m_Height );
      SetWidth( m_Width );
      SetLineColor( m_LineColor );
      SetFillColor( m_FillColor );
      SetLocation( m_Location );
   }
}

#endif // WFC_NO_SERIALIZATION

void CRectangle::SetFillColor( COLORREF color )
{
   WFC_VALIDATE_POINTER( this );
   m_FillColor = color;
}

void CRectangle::SetHeight( DWORD height )
{
   WFC_VALIDATE_POINTER( this );
   m_Height = height;
   m_SetRectangles();
}

void CRectangle::SetLineColor( COLORREF color )
{
   WFC_VALIDATE_POINTER( this );
   m_LineColor = color;
}

void CRectangle::SetLocation( const CPoint& source )
{
   WFC_VALIDATE_POINTER( this );
   m_Location = source;
   m_SetRectangles();
}

void CRectangle::SetSize( const CSize& size )
{
   WFC_VALIDATE_POINTER( this );
   SetWidth(  size.cx );
   SetHeight( size.cy );
   m_SetRectangles();
}

void CRectangle::SetWidth( DWORD width )
{
   WFC_VALIDATE_POINTER( this );
   m_Width = width;
   m_SetRectangles();
}

CRectangle& CRectangle::operator=( const CRectangle& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( source );
   return( *this );
}

/*
** Rounded Rectangle object
*/

CRoundedRectangle::CRoundedRectangle()
{
   WFC_VALIDATE_POINTER( this );
}

CRoundedRectangle::~CRoundedRectangle()
{
   WFC_VALIDATE_POINTER( this );
}

void CRoundedRectangle::Copy( const CRoundedRectangle& source )
{
   WFC_VALIDATE_POINTER( this );
   Copy( &source );
}

void CRoundedRectangle::Copy( const CRoundedRectangle * source_p )
{
   WFC_VALIDATE_POINTER( this );
   WFC_VALIDATE_POINTER( source_p );

   CRectangle::Copy( source_p );

   m_RoundingPoint = source_p->m_RoundingPoint;
}

void CRoundedRectangle::Draw( CDC& device_context )
{
   WFC_VALIDATE_POINTER( this );
   device_context.RoundRect( &m_LineRectangle, m_RoundingPoint );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CRoundedRectangle::Dump( CDumpContext& dump_context ) const
{
   CRectangle::Dump( dump_context );

   dump_context << TEXT( "m_RoundingPoint = " ) << m_RoundingPoint << TEXT( "\n" );
}

#endif // _DEBUG

DWORD CRoundedRectangle::GetRoundingSize( void ) const
{
   WFC_VALIDATE_POINTER( this );
   return( m_RoundingPoint.x );
}

#if ! defined(  WFC_NO_SERIALIZATION )

void CRoundedRectangle::Serialize( CArchive& archive )
{
   CRectangle::Serialize( archive );

   if ( archive.IsStoring() )
   {
      archive << m_RoundingPoint.x;
   }
   else
   {
      archive >> m_RoundingPoint.x;

      SetRoundingSize( m_RoundingPoint.x );
   }
}

#endif // WFC_NO_SERIALIZATION

void CRoundedRectangle::SetRoundingSize( DWORD rounding_size )
{
   WFC_VALIDATE_POINTER( this );
   m_RoundingPoint.x = rounding_size;
   m_RoundingPoint.y = rounding_size;
}

/*
** A Square object
*/

CSquare::CSquare()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();

   SetHeight( 10 );
}

CSquare::CSquare( DWORD size, const CPoint& location, COLORREF fill_color, COLORREF line_color )
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();

   SetHeight( size );
   SetLocation( location );
   SetFillColor( fill_color );
   SetLineColor( line_color );
}

CSquare::~CSquare()
{
   WFC_VALIDATE_POINTER( this );
   SetLocation( CPoint( 0, 0 ) );
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CSquare::Dump( CDumpContext& dump_context ) const
{
   CRectangle::Dump( dump_context );
}

#endif // _DEBUG

#if ! defined(  WFC_NO_SERIALIZATION )

void CSquare::Serialize( CArchive& archive )
{
   CRectangle::Serialize( archive );
}

#endif // WFC_NO_SERIALIZATION

void CSquare::SetHeight( DWORD height )
{
   WFC_VALIDATE_POINTER( this );
   m_Height = height;
   m_Width  = height;
   m_SetRectangles();
}

void CSquare::SetSize( const CSize& size )
{
   WFC_VALIDATE_POINTER( this );
   m_Height = size.cx;
   m_Width  = size.cx;
   m_SetRectangles();
}

void CSquare::SetWidth( DWORD width )
{
   WFC_VALIDATE_POINTER( this );
   m_Width  = width;
   m_Height = width;
   m_SetRectangles();
}

/*
** An ellipse...
*/

CEllipse::CEllipse()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();
}

CEllipse::CEllipse( DWORD height, DWORD width, const CPoint& location, COLORREF fill_color, COLORREF line_color )
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();

   SetHeight( height );
   SetWidth( width );
   SetLocation( location );
   SetFillColor( fill_color );
   SetLineColor( line_color );
}

CEllipse::~CEllipse()
{
   WFC_VALIDATE_POINTER( this );
   SetLocation( CPoint( 0, 0 ) );
}

void CEllipse::Draw( CDC& device_context )
{
   WFC_VALIDATE_POINTER( this );
   CPen line_pen( PS_SOLID, 1, m_LineColor );

   CBrush fill_brush( m_FillColor );

   CPen   *old_pen   = device_context.SelectObject( &line_pen );
   CBrush *old_brush = device_context.SelectObject( &fill_brush );

   device_context.Ellipse( m_LineRectangle );

   device_context.SelectObject( old_brush );
   device_context.SelectObject( old_pen );

   fill_brush.DeleteObject();
   line_pen.DeleteObject();
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CEllipse::Dump( CDumpContext& dump_context ) const
{
   CRectangle::Dump( dump_context );
}

#endif // _DEBUG

#if ! defined(  WFC_NO_SERIALIZATION )

void CEllipse::Serialize( CArchive& archive )
{
   CRectangle::Serialize( archive );

   if ( archive.IsStoring() == FALSE )
   {
      SetLineColor( m_LineColor );
   }
}

#endif // WFC_NO_SERIALIZATION

/*
** A Circle
*/

CCircle::CCircle()
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();

   SetHeight( 10 );
}

CCircle::CCircle( DWORD size, const CPoint& location, COLORREF fill_color, COLORREF line_color )
{
   WFC_VALIDATE_POINTER( this );
   m_Initialize();

   SetHeight( size );
   SetLocation( location );
   SetFillColor( fill_color );
   SetLineColor( line_color );
}

CCircle::~CCircle()
{
   WFC_VALIDATE_POINTER( this );
   SetLocation( CPoint( 0, 0 ) );
}

void CCircle::Draw( CDC& device_context )
{
   WFC_VALIDATE_POINTER( this );
   CPen line_pen( PS_SOLID, 1, m_LineColor );

   CBrush fill_brush( m_FillColor );

   CPen   *old_pen   = device_context.SelectObject( &line_pen   );
   CBrush *old_brush = device_context.SelectObject( &fill_brush );

   device_context.Ellipse( m_LineRectangle );

   device_context.SelectObject( old_brush );
   device_context.SelectObject( old_pen );

   fill_brush.DeleteObject();
   line_pen.DeleteObject();
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CCircle::Dump( CDumpContext& dump_context ) const
{
   CSquare::Dump( dump_context );
}

#endif // _DEBUG

#if ! defined(  WFC_NO_SERIALIZATION )

void CCircle::Serialize( CArchive& archive )
{
   CSquare::Serialize( archive );

   if ( archive.IsStoring() == FALSE )
   {
      SetLineColor( m_LineColor );
   }
}

#endif // WFC_NO_SERIALIZATION

#endif // WFC_STL

// End of source
