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
** $Workfile: drawobj.hpp $
** $Revision: 12 $
** $Modtime: 6/26/01 11:05a $
*/

#if ! defined ( DRAWING_OBJECTS_CLASS_HEADER )

#define DRAWING_OBJECTS_CLASS_HEADER

#if ! defined( WFC_STL )

class CRectangle
#if ! defined(  WFC_NO_SERIALIZATION )
: public CObject
#endif // WFC_NO_SERIALIZATION
{
#if ! defined(  WFC_NO_SERIALIZATION )
   DECLARE_SERIAL( CRectangle )
#endif // WFC_NO_SERIALIZATION

   protected:

      void m_Initialize( void ) WFC_DOESNT_THROW_EXCEPTIONS;
      void m_SetRectangles( void ) WFC_DOESNT_THROW_EXCEPTIONS;

      DWORD m_Height;
      DWORD m_Width;

      COLORREF m_LineColor;
      COLORREF m_FillColor;

      CPoint m_Location;

      CRect m_LineRectangle;
      CRect m_FillRectangle;

   public:

      CRectangle();
      CRectangle( const CRectangle& source );
      CRectangle( DWORD height, DWORD width, const CPoint& location, COLORREF fill_color = WHITE, COLORREF line_color = BLACK );

      virtual ~CRectangle();

      virtual void     Copy( const CRectangle&  source ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void     Copy( const CRectangle * source_p ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void     Draw( CDC& device_context ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual COLORREF GetFillColor( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual DWORD    GetHeight( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual COLORREF GetLineColor( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void     GetRectangle( CRect& destination ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual DWORD    GetWidth( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void     OnClick( void ) WFC_DOESNT_THROW_EXCEPTIONS;
#if ! defined(  WFC_NO_SERIALIZATION )
      virtual void     Serialize( CArchive& archive );
#endif // WFC_NO_SERIALIZATION
      virtual void     SetFillColor( COLORREF color ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void     SetHeight( DWORD height ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void     SetLineColor( COLORREF color ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void     SetLocation( const CPoint& size ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void     SetSize( const CSize& size ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void     SetWidth( DWORD width ) WFC_DOESNT_THROW_EXCEPTIONS;

      /*
      ** Operators
      */

      virtual CRectangle& operator=( const CRectangle& source ) WFC_DOESNT_THROW_EXCEPTIONS;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

class CRoundedRectangle : public CRectangle
{
#if ! defined(  WFC_NO_SERIALIZATION )
   DECLARE_SERIAL( CRoundedRectangle )
#endif // WFC_NO_SERIALIZATION

   protected:

      CPoint m_RoundingPoint;

   public:

      CRoundedRectangle();

      virtual ~CRoundedRectangle();

      virtual void  Copy( const CRoundedRectangle& source ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  Copy( const CRoundedRectangle * source_p ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  Draw( CDC& device_context ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual DWORD GetRoundingSize( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
#if ! defined(  WFC_NO_SERIALIZATION )
      virtual void  Serialize( CArchive& archive );
#endif // WFC_NO_SERIALIZATION
      virtual void  SetRoundingSize( DWORD rounding_size ) WFC_DOESNT_THROW_EXCEPTIONS;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

class CSquare : public CRectangle
{
#if ! defined(  WFC_NO_SERIALIZATION )
   DECLARE_SERIAL( CSquare )
#endif // WFC_NO_SERIALIZATION

   public:

      CSquare();
      CSquare( DWORD size, const CPoint& location, COLORREF fill_color = WHITE, COLORREF line_color = BLACK );
     
      virtual ~CSquare();

#if ! defined(  WFC_NO_SERIALIZATION )
      virtual void Serialize( CArchive& archive );
#endif // WFC_NO_SERIALIZATION

      virtual void SetHeight( DWORD height ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void SetSize( const CSize& size ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void SetWidth( DWORD width ) WFC_DOESNT_THROW_EXCEPTIONS;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

class CEllipse : public CRectangle
{
#if ! defined(  WFC_NO_SERIALIZATION )
   DECLARE_SERIAL( CEllipse )
#endif // WFC_NO_SERIALIZATION

   public:

      CEllipse();
      CEllipse( DWORD height, DWORD width, const CPoint& location, COLORREF fill_color = WHITE, COLORREF line_color = BLACK );
      
      virtual ~CEllipse();

      virtual void Draw( CDC& device_context ) WFC_DOESNT_THROW_EXCEPTIONS;

#if ! defined(  WFC_NO_SERIALIZATION )
      virtual void Serialize( CArchive& archive );
#endif // WFC_NO_SERIALIZATION

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

class CCircle : public CSquare
{
#if ! defined(  WFC_NO_SERIALIZATION )
   DECLARE_SERIAL( CCircle )
#endif // WFC_NO_SERIALIZATION

   public:

      CCircle();
      CCircle( DWORD size, const CPoint& location, COLORREF fill_color = WHITE, COLORREF line_color = BLACK );
      
      virtual ~CCircle();

      virtual void Draw( CDC& device_context ) WFC_DOESNT_THROW_EXCEPTIONS;

#if ! defined(  WFC_NO_SERIALIZATION )
      virtual void Serialize( CArchive& archive );
#endif // WFC_NO_SERIALIZATION

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

#endif // WFC_STL

#endif // DRAWING_OBJECTS_CLASS_HEADER
