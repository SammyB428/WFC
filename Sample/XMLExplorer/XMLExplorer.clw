; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CXMLExplorerDocument
LastTemplate=CTreeView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "XMLExplorer.h"
LastPage=0

ClassCount=7
Class1=CXMLExplorerApplication
Class2=CXMLExplorerDocument
Class3=CXMLExplorerView
Class4=CMainFrame

ResourceCount=2
Resource1=IDD_ABOUTBOX
Class5=CAboutDlg
Class6=CMyListView
Class7=CMyTreeView
Resource2=IDR_MAINFRAME

[CLS:CXMLExplorerApplication]
Type=0
HeaderFile=XMLExplorer.h
ImplementationFile=XMLExplorer.cpp
Filter=N

[CLS:CXMLExplorerDocument]
Type=0
HeaderFile=CXMLExplorerDocument.h
ImplementationFile=CXMLExplorerDocument.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CXMLExplorerDocument

[CLS:CXMLExplorerView]
Type=0
HeaderFile=CXMLExplorerView.h
ImplementationFile=CXMLExplorerView.cpp
Filter=C


[CLS:CMainFrame]
Type=0
HeaderFile=CMainFrame.h
ImplementationFile=CMainFrame.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=XMLExplorer.cpp
ImplementationFile=XMLExplorer.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[CLS:CMyListView]
Type=0
HeaderFile=MyListView.h
ImplementationFile=MyListView.cpp
BaseClass=CListView
Filter=C
LastObject=CMyListView
VirtualFilter=VWC

[CLS:CMyTreeView]
Type=0
HeaderFile=MyTreeView.h
ImplementationFile=MyTreeView.cpp
BaseClass=CTreeView
Filter=C
LastObject=CMyTreeView
VirtualFilter=VWC

