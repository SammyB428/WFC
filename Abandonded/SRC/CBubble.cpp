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
** $Workfile: cbubble.cpp $
** $Revision: 29 $
** $Modtime: 6/26/01 10:44a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if ! defined( WFC_STL )

#if defined( _DEBUG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

CString CBubble::m_StaticClassName;

CFont CBubble::m_StaticFont;

DWORD CBubble::m_StaticHeight = 0;

IMPLEMENT_DYNAMIC( CBubble, CWnd );

CBubble::CBubble()
{
   WFCLTRACEINIT( TEXT( "CBubble::CBubble()" ) );
   WFC_VALIDATE_POINTER( this );
   m_FontSize = (-8);
   m_Created  = FALSE;
}

CBubble::CBubble( int font_size )
{
   WFCLTRACEINIT( TEXT( "CBubble::CBubble( font_size )" ) );
   WFC_VALIDATE_POINTER( this );
   m_Created  = FALSE;
   m_FontSize = font_size;
}

CBubble::~CBubble()
{
   WFCLTRACEINIT( TEXT( "CBubble::~CBubble()" ) );
   WFC_VALIDATE_POINTER( this );
}

BOOL CBubble::Create( void )
{
   WFCLTRACEINIT( TEXT( "CBubble::Create()" ) );
   WFC_VALIDATE_POINTER( this );

   /*
   ** Thanks go to Matt Gradwohl (mattgr@microsoft.com) for finding a Win95
   ** bug in the window styles...
   */

   if ( m_StaticClassName.GetLength() == 0 )
   {
      WFCLTRACEINIT( TEXT( "Class name was empty" ) );

      CBrush brush;

      TRY
      {
         brush.CreateSolidBrush( ::GetSysColor( COLOR_INFOBK ) );
      }
      CATCH( CResourceException, e )
      {
         return( FALSE );
      }
      END_CATCH

      WFCTRACE( TEXT( "Registering class" ) );

      m_StaticClassName = ::AfxRegisterWndClass( WS_EX_TOOLWINDOW | CS_BYTEALIGNCLIENT | CS_SAVEBITS | CS_HREDRAW | CS_VREDRAW, 0, (HBRUSH) brush.Detach() );

      if ( m_StaticClassName == TEXT( "" ) )
      {
         WFCTRACE( TEXT( "AfxRegisterWndClass() failed" ) );
         return( FALSE );
      }
   }

   CRect rectangle;

   rectangle.SetRectEmpty();

   m_Created = CreateEx( 0,
                         m_StaticClassName,
                         TEXT( "" ),
                         WS_POPUP | WS_BORDER,
                         rectangle.left,
                         rectangle.top,
                         rectangle.right,
                         rectangle.bottom,
                         NULL,
                 (HMENU) NULL );

   if ( m_StaticFont.GetSafeHandle() == NULL )
   {
      m_SetFont();
   }

   return( m_Created );
}

BOOL CBubble::DestroyWindow()
{
   WFCLTRACEINIT( TEXT( "CBubble::DestroyWindow()" ) );
   WFC_VALIDATE_POINTER( this );
   m_Created = FALSE;
   return( CWnd::DestroyWindow() );
}

DWORD CBubble::GetHeight( void ) const
{
   WFCLTRACEINIT( TEXT( "CBubble::GetHeight()" ) );
   WFC_VALIDATE_POINTER( this );
   return( m_StaticHeight );
}

void CBubble::Hide( void )
{
   WFCLTRACEINIT( TEXT( "CBubble::Hide()" ) );
   WFC_VALIDATE_POINTER( this );
   ShowWindow( SW_HIDE );
}

BOOL CBubble::IsCreated( void ) const
{
   WFCLTRACEINIT( TEXT( "CBubble::IsCreated()" ) );
   WFC_VALIDATE_POINTER( this );
   return( m_Created );
}

void CBubble::m_SetFont( void )
{
   WFCLTRACEINIT( TEXT( "CBubble::m_SetFont()" ) );
   WFC_VALIDATE_POINTER( this );

   CClientDC device_context( this );

   TRY
   {
      LOGFONT font_attributes;

      ::ZeroMemory( &font_attributes, sizeof( font_attributes ) );

      font_attributes.lfHeight         = -::MulDiv( m_FontSize, device_context.GetDeviceCaps( LOGPIXELSY ), 72 );
      font_attributes.lfCharSet        = DEFAULT_CHARSET;
      font_attributes.lfQuality        = DEFAULT_QUALITY;
      font_attributes.lfClipPrecision  = CLIP_LH_ANGLES | CLIP_STROKE_PRECIS;
      font_attributes.lfPitchAndFamily = FF_SWISS;

      m_StaticFont.CreateFontIndirect( &font_attributes );
   }
   CATCH( CResourceException, e )
   {
      WFCTRACE( TEXT( "font creation failed\n" ) );
      return;
   }
   END_CATCH

   CFont *old_font = device_context.SelectObject( &m_StaticFont );

   TEXTMETRIC text_metrics;

   device_context.GetTextMetrics( &text_metrics );
   device_context.SelectObject( old_font );
   m_StaticHeight = text_metrics.tmHeight + text_metrics.tmExternalLeading + ( 6 * GetSystemMetrics( SM_CYBORDER ) );
}

void CBubble::SetFontSize( int font_size )
{
   WFCLTRACEINIT( TEXT( "CBubble::SetFontSize()" ) );
   WFC_VALIDATE_POINTER( this );
   m_FontSize = font_size;
}

void CBubble::Show( CPoint point, const CString& string )
{
   WFCLTRACEINIT( TEXT( "CBubble::Show()" ) );
   WFC_VALIDATE_POINTER( this );

   Hide();
   UpdateWindow();

   SetWindowText( string );

   CRect bubble;
   GetWindowRect( &bubble );
   CRect screen;

   ::GetWindowRect( ::GetDesktopWindow(), &screen );

   /*
   ** Let's make sure the window is always visible (on screen)
   */

   if ( point.y < 0 )
   {
      point.y = 0;
   }

   if ( ( point.y + bubble.Height() ) > screen.bottom )
   {
      point.y = screen.bottom - ( bubble.Height() + 1 );
   }

   if ( point.x < 0 )
   {
      point.x = abs( point.x ) - ( bubble.Width() / 2 );
   }

   if ( point.x > screen.right )
   {
      point.x = screen.right - ( bubble.Width() + 1 );
   }

   SetWindowPos( &wndTop, point.x, point.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE );

   ShowWindow( SW_SHOWNOACTIVATE );
}

BEGIN_MESSAGE_MAP( CBubble, CWnd )
   ON_WM_PAINT()
   ON_MESSAGE( WM_SETTEXT, OnSetText )
END_MESSAGE_MAP()

void CBubble::OnPaint()
{
   WFCLTRACEINIT( TEXT( "CBubble::OnPaint()" ) );
   WFC_VALIDATE_POINTER( this );

   CPaintDC device_context( this );

   CRect rectangle;

   GetClientRect( rectangle );

   CFont *old_font = device_context.SelectObject( &m_StaticFont );
   device_context.SetTextAlign( TA_CENTER );
   device_context.SetBkMode( TRANSPARENT );
   device_context.SetTextColor( ::GetSysColor( COLOR_INFOTEXT ) );

   CString text;
   GetWindowText( text );
   CSize size = device_context.GetTextExtent( text, text.GetLength() );
   device_context.TextOut( rectangle.right / 2, ( rectangle.bottom - size.cy ) / 2, text );
   device_context.SelectObject( old_font );
}

afx_msg LONG CBubble::OnSetText( UINT, LONG lParam )
{
   WFCLTRACEINIT( TEXT( "CBubble::OnSetText()" ) );
   WFC_VALIDATE_POINTER( this );

   CRect rectangle;
   GetWindowRect( rectangle );
   
   CClientDC device_context( this );

   CFont *old_font = device_context.SelectObject( &m_StaticFont );

   WFC_TRY
   {
      CSize size = device_context.GetTextExtent( (LPCTSTR) lParam, _tcslen( (LPCTSTR) lParam ) );

      rectangle.right = rectangle.left + size.cx + ( 6 * GetSystemMetrics( SM_CXBORDER ) );

      rectangle.bottom = rectangle.top + m_StaticHeight;
      MoveWindow( &rectangle );
      device_context.SelectObject( old_font );
   }
   WFC_CATCH_ALL
   {
      ;
   }
   WFC_END_CATCH_ALL

   return( CWnd::Default() );
}

#endif // WFC_STL

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CBubble</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="The class that lets you create your own ToolTip windows.">
</HEAD>

<BODY>
<H1>CBubble : CWnd</H1>

$Revision: 29 $<HR>

<H2>Description</H2>

This class creates a tooltip bubble window. You can put it anywhere you want.

<H2>Constructors</H2>

<DL COMPACT>

<DT><PRE><B>CBubble</B>()
<B>CBubble</B>( int font_size )</PRE><DD>
Constructs the object.
</DL>
<H2>Methods</H2>
<DL COMPACT>
<DT><PRE>BOOL <B>Create</B>( void )</PRE><DD>
Just like CWnd.
<DT><PRE>BOOL <B>DestroyWindow</B>( void )</PRE><DD>
Just like CWnd.
<DT><PRE>DWORD <B>GetHeight</B>( void )</PRE><DD>
Returns the height of the bubble.
<DT><PRE>void <B>Hide</B>( void )</PRE><DD>
Makes the tooltip invisible.
<DT><PRE>BOOL <B>IsCreated</B>( void )</PRE><DD>
Returns TRUE if <B>Create</B>() has been called.
<DT><PRE>void <B>SetFontSize</B>( int font_size )</PRE><DD>
Sets the size of the font.
<DT><PRE>void <B>Show</B>( CPoint screen_coordinates, const CString&amp; text_to_display )</PRE><DD>
You give it a CPoint of where you want the window
and the text to display and VIOLA!
</DL>
<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>
<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: cbubble.cpp $<BR>
$Modtime: 6/26/01 10:44a $
</BODY>
</HTML>
*/
