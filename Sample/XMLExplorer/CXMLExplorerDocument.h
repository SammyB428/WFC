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

// CXMLExplorerDocument.h : interface of the CXMLExplorerDocument class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CXMLEXPLORERDOCUMENT_H__41D52534_19F6_11D3_BF11_00A0C9CCC695__INCLUDED_)
#define AFX_CXMLEXPLORERDOCUMENT_H__41D52534_19F6_11D3_BF11_00A0C9CCC695__INCLUDED_

class CXMLExplorerDocument : public CDocument
{
protected: // create from serialization only
	CXMLExplorerDocument();
	DECLARE_DYNCREATE(CXMLExplorerDocument)

// Attributes
public:

   CMyTreeView * TreeView;
   CMyListView * ListView;

   Win32FoundationClasses::CExtensibleMarkupLanguageDocument * XML;

// Operations
public:

   void ShowDocument( Win32FoundationClasses::CExtensibleMarkupLanguageDocument  * document_p );
   void ShowElement(  Win32FoundationClasses::CExtensibleMarkupLanguageElement   * element_p );
   void ShowEntities( Win32FoundationClasses::CExtensibleMarkupLanguageDocument  * document_p );
   void ShowAttribute( Win32FoundationClasses::CExtensibleMarkupLanguageAttribute * attribute_p );
   void ShowListOfAttributes( Win32FoundationClasses::CExtensibleMarkupLanguageElement * xml_p );
   void ShowDefault( void );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMLExplorerDocument)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXMLExplorerDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CXMLExplorerDocument)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CXMLEXPLORERDOCUMENT_H__41D52534_19F6_11D3_BF11_00A0C9CCC695__INCLUDED_)
