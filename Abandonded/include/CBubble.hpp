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
** $Workfile: cbubble.hpp $
** $Revision: 13 $
** $Modtime: 6/26/01 11:00a $
*/

#if ! defined ( BUBBLE_CLASS_HEADER )

#define BUBBLE_CLASS_HEADER

#if ! defined( WFC_STL )

class CBubble : public CWnd
{
   DECLARE_DYNAMIC( CBubble );

   private:

      // Don't allow canonical behavior (i.e. don't allow this class
      // to be passed by value)

      CBubble( const CBubble& ) {};
      CBubble& operator=( const CBubble& ) { return( *this ); };

   private:

      static CString m_StaticClassName;
      static CFont   m_StaticFont;
      static DWORD   m_StaticHeight;

      int            m_FontSize;
      BOOL           m_Created;

      void m_SetFont( void ) WFC_DOESNT_THROW_EXCEPTIONS;

   public:

      CBubble();
      CBubble( int font_size );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CBubble();

      virtual BOOL  Create( void ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual BOOL  DestroyWindow( void ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual DWORD GetHeight( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  Hide( void ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual BOOL  IsCreated( void ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  SetFontSize( int font_size ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void  Show( CPoint screen_coordinates, const CString& string ) WFC_DOESNT_THROW_EXCEPTIONS;

   protected:

      afx_msg void OnPaint( void );
      afx_msg LONG OnSetText( UINT, LONG lParam );

      DECLARE_MESSAGE_MAP()
};

#endif // WFC_STL

#endif // BUBBLE_CLASS_HEADER
