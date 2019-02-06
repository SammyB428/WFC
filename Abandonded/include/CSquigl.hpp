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
** $Workfile: csquigl.hpp $
** $Revision: 15 $
** $Modtime: 6/26/01 11:04a $
*/

#if ! defined( SQUIGGLE_CLASS_HEADER )

#define SQUIGGLE_CLASS_HEADER

#if ! defined( WFC_STL )

class CSquiggleData
#if ! defined(  WFC_NO_SERIALIZATION )
: public CObject
#endif // WFC_NO_SERIALIZATION
{
#if ! defined(  WFC_NO_SERIALIZATION )
   DECLARE_SERIAL( CSquiggleData )
#endif // WFC_NO_SERIALIZATION

   private:

      void m_Initialize( void ) WFC_DOESNT_THROW_EXCEPTIONS;

   public:

      // Construction

      CSquiggleData();
      CSquiggleData( const CSquiggleData * source_p );
      virtual ~CSquiggleData();

      /*
      ** Data Members
      */

      CString    Start;
      CString    Stop;
      CTime      Time;
      WORD       MinimumValue;
      WORD       MaximumValue;
      CWordArray Data;

      // Methods

      virtual void Copy( const CSquiggleData& source ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void Copy( const CSquiggleData * source_p ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void Empty( void ) WFC_DOESNT_THROW_EXCEPTIONS;
#if ! defined(  WFC_NO_SERIALIZATION )
      virtual void Serialize( CArchive& archive );
#endif // WFC_NO_SERIALIZATION
      virtual WORD ValidRange( void ) WFC_DOESNT_THROW_EXCEPTIONS;

      // Operators

      virtual CSquiggleData& operator=( const CSquiggleData& source ) WFC_DOESNT_THROW_EXCEPTIONS;
};

class CSquiggle : public CRectangle
{
#if ! defined(  WFC_NO_SERIALIZATION )
   DECLARE_SERIAL( CSquiggle )
#endif // WFC_NO_SERIALIZATION

   private:

      // Don't allow canonical behavior (i.e. don't allow this class
      // to be passed by value)

      CSquiggle( const CSquiggle& ) {};
      CSquiggle& operator=( const CSquiggle& ) { return( *this ); };

   private:

      void m_Initialize( void ) WFC_DOESNT_THROW_EXCEPTIONS;

   protected:

      BOOL m_AutomaticallyDelete;

      CSquiggleData * m_SquiggleData_p;

      int m_NumberOfPoints;

      WORD m_NumberOfXGridLines;
      WORD m_NumberOfYGridLines;

      DWORD m_LineThickness;
      DWORD m_GridLineType;

      COLORREF m_GridLineColor;

      POINT * m_PointArray;

      BOOL m_DataIsDirty;

   public:

      CSquiggle();
      CSquiggle( DWORD height, DWORD width, const CPoint& location, COLORREF fill_color, COLORREF line_color );

      virtual      ~CSquiggle();
      virtual void  Copy( const CSquiggle * source_p ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  Draw( CDC& device_context ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  Empty( void ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual DWORD GetLineThickness( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  PrepareData( void ) WFC_DOESNT_THROW_EXCEPTIONS;
#if ! defined(  WFC_NO_SERIALIZATION )
      virtual void  Serialize( CArchive& archive );
#endif // WFC_NO_SERIALIZATION
      virtual void  SetAutomaticallyDelete( BOOL new_value = TRUE ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetDirtyData( BOOL new_value = TRUE ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetGridLineColor( COLORREF grid_line_color ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetGridLineType( DWORD type ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetHeight( DWORD height ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetLineThickness( DWORD thickness ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetNumberOfGridLines( WORD number_of_x_lines, WORD number_of_y_lines ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetSize( const CSize& size ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetSquiggleData( CSquiggleData * source_p, BOOL auto_delete = FALSE ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetWidth( DWORD width ) WFC_DOESNT_THROW_EXCEPTIONS;
};

#endif // WFC_STL

#endif // SQUIGGLE_CLASS_HEADER
