# Microsoft Developer Studio Project File - Name="TrapRcvr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TrapRcvr - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TrapRcvr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TrapRcvr.mak" CFG="TrapRcvr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TrapRcvr - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TrapRcvr - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\pktlib" /I "..\mibparse\mibparse" /I "..\mibparse\mibdb" /I "..\common" /I "..\sqlitesrc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\pktlib\nt\libs\pktlib.lib wsock32.lib snmpapi.lib mgmtapi.lib ..\mibparse\release\mibparse.lib ..\mibparse\mibdb\release\mibdb.lib ..\sqlitesrc\sqlite3.lib /nologo /subsystem:windows /map /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcp.lib"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\pktlib" /I "..\mibparse\mibparse" /I "..\mibparse\mibdb" /I "..\common" /I "..\sqlitesrc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 nafxcwd.lib ..\pktlib\nt\libs\pktlibd.lib wsock32.lib snmpapi.lib mgmtapi.lib ..\mibparse\debug\mibparse.lib ..\mibparse\mibdb\debug\mibdb.lib ..\sqlitesrc\sqlite3.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"nafxcwd.lib" /nodefaultlib:"libc.lib" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TrapRcvr - Win32 Release"
# Name "TrapRcvr - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ApplicationLogging.cpp
# End Source File
# Begin Source File

SOURCE=..\common\Base64.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\CmdLineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigureSound.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigureTrapData.cpp
# End Source File
# Begin Source File

SOURCE=.\DetailDlg.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\DlgResizeHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\EmailRecipients.cpp
# End Source File
# Begin Source File

SOURCE=.\EnvVarsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecuteAdd.cpp

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ExploderDestinations.cpp
# End Source File
# Begin Source File

SOURCE=.\LogQueueClient.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\MibFileBrowse.cpp
# End Source File
# Begin Source File

SOURCE=.\MibImportPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MibLoadStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\MibUnloadStatus.cpp
# End Source File
# Begin Source File

SOURCE=..\common\QueueServer.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\RawTrapData.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\SmtpAuthDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToolTipListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TrActionAdd.cpp

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TrActions.cpp

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TrAlert.cpp

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\trThread\TrapMatcher.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\TrapRcvr.cpp

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TrapRcvr.rc
# End Source File
# Begin Source File

SOURCE=.\TrapRcvrDlg.cpp

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TrapRcvrException.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\TrEmail.cpp

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TrEmailAdd.cpp

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TrExploder.cpp

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TrExploderAdd.cpp

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TrLogging.cpp

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TrMisc.cpp

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TrMonitor.cpp

!IF  "$(CFG)" == "TrapRcvr - Win32 Release"

!ELSEIF  "$(CFG)" == "TrapRcvr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\V2DetailDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ApplicationLogging.h
# End Source File
# Begin Source File

SOURCE=.\CmdLineDlg.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureSound.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureTrapData.h
# End Source File
# Begin Source File

SOURCE=.\EmailRecipients.h
# End Source File
# Begin Source File

SOURCE=.\EnvVarsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExploderDestinations.h
# End Source File
# Begin Source File

SOURCE=.\MibFileBrowse.h
# End Source File
# Begin Source File

SOURCE=.\MibImportPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\MibLoadStatus.h
# End Source File
# Begin Source File

SOURCE=.\MibUnloadStatus.h
# End Source File
# Begin Source File

SOURCE=.\RawTrapData.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SmtpAuthDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TrapRcvr.h
# End Source File
# Begin Source File

SOURCE=.\TrapRcvrDlg.h
# End Source File
# Begin Source File

SOURCE=.\V2DetailDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\TrapRcvr.ico
# End Source File
# Begin Source File

SOURCE=.\res\TrapRcvr.rc2
# End Source File
# End Group
# End Target
# End Project
