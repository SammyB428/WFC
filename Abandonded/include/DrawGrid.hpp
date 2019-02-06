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
** $Workfile: drawgrid.hpp $
** $Revision: 13 $
** $Modtime: 6/26/01 11:05a $
*/

#if ! defined ( DRAWING_OBJECTS_GRID_CLASS_HEADER )

#define DRAWING_OBJECTS_GRID_CLASS_HEADER

#if ! defined( WFC_STL )

class CDrawingObjectGrid
#if ! defined(  WFC_NO_SERIALIZATION )
: public CObject
#endif // WFC_NO_SERIALIZATION
{
#if ! defined(  WFC_NO_SERIALIZATION )
   DECLARE_SERIAL( CDrawingObjectGrid )
#endif // WFC_NO_SERIALIZATION

   private:

      // Don't allow canonical behavior (i.e. don't allow this class
      // to be passed by value)

      CDrawingObjectGrid( const CDrawingObjectGrid& ) {};
      CDrawingObjectGrid& operator=( const CDrawingObjectGrid& ) { return( *this ); };

   protected:

      void m_Initialize( void ) WFC_DOESNT_THROW_EXCEPTIONS;
      void m_SetRectangles( void ) WFC_DOESNT_THROW_EXCEPTIONS;

      DWORD m_NumberOfRows;
      DWORD m_NumberOfColumns;
      DWORD m_VerticalSpacing;
      DWORD m_HorizontalSpacing;

      CPtrArray m_ObjectArray;

      CString m_Name;

   public:

      CDrawingObjectGrid();
      CDrawingObjectGrid( DWORD number_of_rows, DWORD number_of_columns );

      virtual ~CDrawingObjectGrid();

      virtual void         Draw( CDC& device_context ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual CRectangle*& ElementAt( DWORD row_number, DWORD column_number ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual CRectangle*  GetAt( DWORD row_number, DWORD column_number ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual DWORD        GetHeight( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual int          GetHorizontalSpacing( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual BOOL         GetIndexFromPoint( DWORD& row_index, DWORD& column_index, const CPoint& point ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void         GetName( CString& name_of_grid ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual DWORD        GetNumberOfColumns( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual DWORD        GetNumberOfRows( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void         GetRectangle( CRect& rect ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual DWORD        GetVerticalSpacing( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual DWORD        GetWidth( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void         RemoveAll( void ) WFC_DOESNT_THROW_EXCEPTIONS;
#if ! defined(  WFC_NO_SERIALIZATION )
      virtual void         Serialize( CArchive& archive );
#endif // WFC_NO_SERIALIZATION
      virtual void         SetAt( DWORD row_number, DWORD column_number, CRectangle *new_element ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void         SetFillColor( DWORD row_number, DWORD column_number, COLORREF color ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void         SetHorizontalSpacing( DWORD horizontal_spacing ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void         SetLineColor( DWORD row_number, DWORD columen_number, COLORREF color ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void         SetName( LPCTSTR name_of_grid = NULL ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void         SetRectangle( const CRect& source ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void         SetSize( DWORD number_of_rows, DWORD number_of_columns ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void         SetVerticalSpacing( DWORD vertical_spacing ) WFC_DOESNT_THROW_EXCEPTIONS;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

#define LABELED_GRID_ROW_NAMES     1
#define LABELED_GRID_COLUMN_NAMES  2
#define LABELED_GRID_ROWS_TITLE    4
#define LABELED_GRID_COLUMNS_TITLE 8
#define LABELED_GRID_ALL_TITLES ( LABELED_GRID_ROW_NAMES | LABELED_GRID_COLUMN_NAMES | LABELED_GRID_ROWS_TITLE | LABELED_GRID_COLUMNS_TITLE )

class CLabeledGrid : public CDrawingObjectGrid
{
#if ! defined(  WFC_NO_SERIALIZATION )
   DECLARE_SERIAL( CLabeledGrid )
#endif // WFC_NO_SERIALIZATION

   private:

      // Don't allow canonical behavior (i.e. don't allow this class
      // to be passed by value)

      CLabeledGrid( const CLabeledGrid& ) {};
      CLabeledGrid& operator=( const CLabeledGrid& ) { return( *this ); };

   private:

      void m_Initialize( void ) WFC_DOESNT_THROW_EXCEPTIONS;

   protected:

      CStringArray m_ColumnNames;
      CStringArray m_RowNames;

      CString m_RowsTitle;
      CString m_ColumnsTitle;

      DWORD m_ColumnFontSize;
      DWORD m_ColumnsTitleFontSize;
      DWORD m_RowFontSize;
      DWORD m_RowsTitleFontSize;
      DWORD m_NumberOfLogicalUnitsInLongestRowName;
      DWORD m_NumberOfLogicalUnitsInLongestColumnName;
      DWORD m_NumberOfLogicalUnitsInRowsTitle;
      DWORD m_NumberOfLogicalUnitsInColumnsTitle;
      DWORD m_LabelOptions;

      CFont m_RowNamesFont;
      CFont m_ColumnNamesFont;
      CFont m_RowsTitleFont;
      CFont m_ColumnsTitleFont;

      virtual void m_SetColumnFontSize( CDC& device_context, DWORD font_size ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void m_SetColumnsTitleFontSize( CDC& device_context, DWORD font_size ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void m_SetRowFontSize( CDC& device_context, DWORD font_size ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void m_SetRowsTitleFontSize( CDC& device_context, DWORD font_size ) WFC_DOESNT_THROW_EXCEPTIONS;

#if defined( _DEBUG )

      BOOL m_PrepareForPaintingWasCalled;

#endif // _DEBUG

   public:

      CLabeledGrid();
      CLabeledGrid( DWORD number_of_rows, DWORD number_of_columns );
      virtual ~CLabeledGrid();

      virtual void Draw( CDC& device_context ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void GetColumnName( DWORD column_number, CString& column_name ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void GetColumnsTitle( CString& columns_title ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void GetRowName( DWORD row_number, CString& row_name ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void GetRowsTitle( CString& rows_title ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void PrepareForPainting( CDC&  device_context,
                                       DWORD row_font_size           = 8,
                                       DWORD column_font_size        = 8,
                                       DWORD rows_title_font_size    = 14,
                                       DWORD columns_title_font_size = 14 ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void RemoveAll( void ) WFC_DOESNT_THROW_EXCEPTIONS;
#if ! defined(  WFC_NO_SERIALIZATION )
      virtual void Serialize( CArchive& archive );
#endif // WFC_NO_SERIALIZATION
      virtual void SetColumnName( DWORD column_number, LPCTSTR column_name ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void SetColumnsTitle( LPCTSTR columns_title = NULL ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void SetLabelOptions( DWORD options ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void SetRowName( DWORD row_number, LPCTSTR row_name ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void SetRowsTitle( LPCTSTR rows_title = NULL ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void SetSize( DWORD number_of_rows, DWORD number_of_columns ) WFC_DOESNT_THROW_EXCEPTIONS;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

#endif // WFC_STL

#endif // DRAWING_OBJECTS_GRID_CLASS_HEADER
