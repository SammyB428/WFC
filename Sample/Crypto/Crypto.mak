# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=Crypto - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Crypto - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Crypto - Win32 Release" && "$(CFG)" != "Crypto - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Crypto.mak" CFG="Crypto - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Crypto - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Crypto - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Crypto - Win32 Debug"
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "Crypto - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Crypto.exe"

CLEAN : 
	-@erase "$(INTDIR)\Crypto.obj"
	-@erase "$(OUTDIR)\Crypto.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "\wfc\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MD /W4 /GX /O2 /I "\wfc\include" /D "WIN32" /D "NDEBUG" /D\
 "_CONSOLE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Crypto.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Crypto.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 \wfc\lib\winrel\wfc.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=\wfc\lib\winrel\wfc.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/Crypto.pdb" /machine:I386 /out:"$(OUTDIR)/Crypto.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Crypto.obj"

"$(OUTDIR)\Crypto.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Crypto - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Crypto.exe"

CLEAN : 
	-@erase "$(INTDIR)\Crypto.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Crypto.exe"
	-@erase "$(OUTDIR)\Crypto.ilk"
	-@erase "$(OUTDIR)\Crypto.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "\wfc\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /I "\wfc\include" /D "WIN32" /D\
 "_DEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Crypto.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Crypto.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 \wfc\lib\windebug\wfc.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=\wfc\lib\windebug\wfc.lib /nologo /subsystem:console\
 /incremental:yes /pdb:"$(OUTDIR)/Crypto.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Crypto.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Crypto.obj"

"$(OUTDIR)\Crypto.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Crypto - Win32 Release"
# Name "Crypto - Win32 Debug"

!IF  "$(CFG)" == "Crypto - Win32 Release"

!ELSEIF  "$(CFG)" == "Crypto - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Crypto.cpp

!IF  "$(CFG)" == "Crypto - Win32 Release"

DEP_CPP_CRYPT=\
	"..\..\include\samtf.h"\
	"..\..\include\wfctrace.hpp"\
	"\wfc\include\CEventLogRecord.hpp"\
	"\wfc\include\CEvntLog.hpp"\
	"\wfc\include\CFileTime.hpp"\
	"\wfc\include\CNetConn.hpp"\
	"\wfc\include\CNetFile.hpp"\
	"\wfc\include\CNetInfo.hpp"\
	"\wfc\include\CNetRsrc.hpp"\
	"\wfc\include\CNetSess.hpp"\
	"\wfc\include\CNetShar.hpp"\
	"\wfc\include\CNetUser.hpp"\
	"\wfc\include\CNetWkst.hpp"\
	"\wfc\include\CNetwork.hpp"\
	"\wfc\include\cnmpipe.hpp"\
	"\wfc\include\CPing.hpp"\
	"\wfc\include\CRAS.hpp"\
	"\wfc\include\crasadm.hpp"\
	"\wfc\include\CRASPORT.hpp"\
	"\wfc\include\CRegisty.hpp"\
	"\wfc\include\CServer.hpp"\
	"\wfc\include\CSystemTime.hpp"\
	"\wfc\include\CUNC.hpp"\
	"\wfc\include\CURL.hpp"\
	"\wfc\include\dcb.hpp"\
	"\wfc\include\DumyFile.hpp"\
	"\wfc\include\lsockets.hpp"\
	"\wfc\include\lzfile.hpp"\
	"\wfc\include\Modem.hpp"\
	"\wfc\include\Ovrlpd.hpp"\
	"\wfc\include\PortInformation.hpp"\
	"\wfc\include\Serial.hpp"\
	"\wfc\include\ServiceConfiguration.hpp"\
	"\wfc\include\sockets.hpp"\
	"\wfc\include\SockFile.hpp"\
	"\wfc\include\Structs.hpp"\
	"\wfc\include\tsockets.hpp"\
	"\wfc\include\wfc.h"\
	

"$(INTDIR)\Crypto.obj" : $(SOURCE) $(DEP_CPP_CRYPT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Crypto - Win32 Debug"

DEP_CPP_CRYPT=\
	"..\..\include\samtf.h"\
	"..\..\include\wfctrace.hpp"\
	"\wfc\include\CEventLogRecord.hpp"\
	"\wfc\include\CEvntLog.hpp"\
	"\wfc\include\CRegisty.hpp"\
	"\wfc\include\CUNC.hpp"\
	"\wfc\include\Structs.hpp"\
	"\wfc\include\wfc.h"\
	

"$(INTDIR)\Crypto.obj" : $(SOURCE) $(DEP_CPP_CRYPT) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
