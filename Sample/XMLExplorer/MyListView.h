/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright (c) 1996-2003, Samuel R. Blackburn
** All rights reserved.
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
*/

#if !defined(AFX_MYLISTVIEW_H__41D5253E_19F6_11D3_BF11_00A0C9CCC695__INCLUDED_)
#define AFX_MYLISTVIEW_H__41D5253E_19F6_11D3_BF11_00A0C9CCC695__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyListView view

class CMyListView : public CListView
{
protected:
	CMyListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyListView)

   DWORD m_CurrentDisplay;


// Attributes
public:

// Operations
public:

   BOOL AddColumn( LPCTSTR column_name,
                   int     column_number,
                   int     width = (-1),
                   int     nSubItem = (-1),
                   int     mask = LVCF_FMT  | LVCF_WIDTH |
                                  LVCF_TEXT | LVCF_SUBITEM,
                   int     format = LVCFMT_LEFT );

   BOOL AddItem( int     row_number,
                 int     column_number,
                 LPCTSTR text,
                 int     image_index = (-1) );

   void Empty( void );

   void ShowDocument( Win32FoundationClasses::CExtensibleMarkupLanguageDocument * document_p );
   void ShowElement( Win32FoundationClasses::CExtensibleMarkupLanguageElement * element_p );
   void ShowEntities( Win32FoundationClasses::CExtensibleMarkupLanguageDocument * document_p );
   void ShowAttribute( Win32FoundationClasses::CExtensibleMarkupLanguageAttribute * attribute_p );
   void ShowDefault( void );
   void ShowListOfAttributes( Win32FoundationClasses::CExtensibleMarkupLanguageElement * element_p );
//   void ShowEntity( CExtensibleMarkupLanguageDocument * document_p );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTVIEW_H__41D5253E_19F6_11D3_BF11_00A0C9CCC695__INCLUDED_)
