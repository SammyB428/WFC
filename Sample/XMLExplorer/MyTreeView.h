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

#if !defined(AFX_MYTREEVIEW_H__41D5253F_19F6_11D3_BF11_00A0C9CCC695__INCLUDED_)
#define AFX_MYTREEVIEW_H__41D5253F_19F6_11D3_BF11_00A0C9CCC695__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTreeView.h : header file

#define IMAGE_ROOT                   0
#define IMAGE_TILE_2                 1
#define IMAGE_TILE_3                 2
#define IMAGE_PROCESSING_INSTRUCTION 3
#define IMAGE_TEXT_SEGMENT           4
#define IMAGE_LIST_OF_ENTITIES       5
#define IMAGE_ENTITY                 6
#define IMAGE_ATTRIBUTES             7
#define IMAGE_ATTRIBUTE              8
#define IMAGE_META_DATA              9

/////////////////////////////////////////////////////////////////////////////
// CMyTreeView view

class CMyTreeView : public CTreeView
{
   public:

      CImageList ImageList;

      HTREEITEM RootItem;
      HTREEITEM CurrentItem;

protected:

	CMyTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyTreeView)

// Attributes
public:

   enum _what_it_is
   {
      IT_IS_THE_ROOT = 1,
      IT_IS_AN_XML_DOCUMENT,
      IT_IS_AN_ELEMENT,
      IT_IS_AN_ENTITY,
      IT_IS_AN_ATTRIBUTE,
      IT_IS_A_LIST_OF_ATTRIBUTES
   };

// Operations
public:

   void DeleteChildren( HTREEITEM parent_item );
   DWORD ImageToWhatItIs( DWORD image_id ) const;
   void Populate( Win32FoundationClasses::CExtensibleMarkupLanguageDocument * xml_p );
   void AddElement( CTreeCtrl& tree_control, HTREEITEM parent_item, Win32FoundationClasses::CExtensibleMarkupLanguageElement * element_p );
   void AddEntities( CTreeCtrl& tree_control, HTREEITEM parent_item, Win32FoundationClasses::CExtensibleMarkupLanguageDocument * element_p );
   void AddAttributes( CTreeCtrl& tree_control, HTREEITEM parent_item, Win32FoundationClasses::CExtensibleMarkupLanguageElement * element_p );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTreeView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTREEVIEW_H__41D5253F_19F6_11D3_BF11_00A0C9CCC695__INCLUDED_)
