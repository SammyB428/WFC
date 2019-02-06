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

// CXMLExplorerDocument.cpp : implementation of the CXMLExplorerDocument class
//

#include "XMLExplorer.h"
#pragma hdrstop

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXMLExplorerDocument

IMPLEMENT_DYNCREATE(CXMLExplorerDocument, CDocument)

BEGIN_MESSAGE_MAP(CXMLExplorerDocument, CDocument)
	//{{AFX_MSG_MAP(CXMLExplorerDocument)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMLExplorerDocument construction/destruction

CXMLExplorerDocument::CXMLExplorerDocument()
{
   WFCTRACEINIT( TEXT( "CXMLExplorerDocument::CXMLExplorerDocument()" ) );
   XML = NULL;
}

CXMLExplorerDocument::~CXMLExplorerDocument()
{
   WFCTRACEINIT( TEXT( "CXMLExplorerDocument::~CXMLExplorerDocument()" ) );

   if ( XML != NULL )
   {
      delete XML;
      XML = NULL;
   }
}

BOOL CXMLExplorerDocument::OnNewDocument()
{
   WFCTRACEINIT( TEXT( "CXMLExplorerDocument::OnNewDocument()" ) );

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
// CXMLExplorerDocument serialization

void CXMLExplorerDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXMLExplorerDocument diagnostics

#ifdef _DEBUG
void CXMLExplorerDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CXMLExplorerDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXMLExplorerDocument commands

BOOL CXMLExplorerDocument::OnOpenDocument( LPCTSTR filename )
{
   WFCTRACEINIT( TEXT( "CXMLExplorerDocument::OnOpenDocument()" ) );

   if ( ! CDocument::OnOpenDocument( filename ) )
   {
      return( FALSE );
   }

   if ( XML == NULL )
   {
      XML = new CExtensibleMarkupLanguageDocument;
   }

   DWORD parsing_options = XML->GetParseOptions();

   parsing_options |= WFC_XML_LOOSE_COMMENT_PARSING;

   XML->SetParseOptions( parsing_options );
   XML->Empty();

   if ( wfc_read_xml_from_file( filename, *XML ) == false )
   {
      WFCTRACE( TEXT( "Can't parse" ) );
      TreeView->PostMessage( WM_COMMAND, 99782, (LPARAM) XML );
      return( FALSE );
   }

   WFCTRACEVAL( TEXT( "XML parsed OK " ), filename );

   // TreeView->Populate( XML );

   // We post a message so CDocument's document path name will be filled in when we need it
   TreeView->PostMessage( WM_COMMAND, 99782, (LPARAM) XML );

   return( TRUE );
}

void CXMLExplorerDocument::ShowDocument( CExtensibleMarkupLanguageDocument * xml_p )
{
   WFCTRACEINIT( TEXT( "CXMLExplorerDocument::ShowDocument()" ) );

   if ( ListView != NULL && xml_p != NULL )
   {
      ListView->ShowDocument( xml_p );
   }
}

void CXMLExplorerDocument::ShowElement( CExtensibleMarkupLanguageElement * element_p )
{
   WFCTRACEINIT( TEXT( "CXMLExplorerDocument::ShowElement()" ) );

   if ( ListView != NULL && element_p != NULL )
   {
      ListView->ShowElement( element_p );
   }
}

void CXMLExplorerDocument::ShowEntities( CExtensibleMarkupLanguageDocument * xml_p )
{
   WFCTRACEINIT( TEXT( "CXMLExplorerDocument::ShowEntities()" ) );

   if ( ListView != NULL && xml_p != NULL )
   {
      ListView->ShowEntities( xml_p );
   }
}

void CXMLExplorerDocument::ShowDefault( void )
{
   WFCTRACEINIT( TEXT( "CXMLExplorerDocument::ShowDefault()" ) );

   if ( ListView != NULL )
   {
      ListView->ShowDefault();
   }
}

void CXMLExplorerDocument::ShowAttribute( CExtensibleMarkupLanguageAttribute * xml_p )
{
   WFCTRACEINIT( TEXT( "CXMLExplorerDocument::ShowAttribute()" ) );

   if ( ListView != NULL && xml_p != NULL )
   {
      ListView->ShowAttribute( xml_p );
   }
}

void CXMLExplorerDocument::ShowListOfAttributes( CExtensibleMarkupLanguageElement * xml_p )
{
   WFCTRACEINIT( TEXT( "CXMLExplorerDocument::ShowListOfAttributes()" ) );

   if ( ListView != NULL && xml_p != NULL )
   {
      ListView->ShowListOfAttributes( xml_p );
   }
}

