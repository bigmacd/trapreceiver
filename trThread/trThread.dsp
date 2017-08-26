# Microsoft Developer Studio Project File - Name="trThread" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=trThread - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "trThread.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "trThread.mak" CFG="trThread - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "trThread - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "trThread - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "trThread - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\pktlib" /I "..\..\common" /I "..\..\ebase\include" /I ".." /I "..\..\mibParse\mibdb" /I "..\..\sqlitesrc" /I "..\..\smtpLibrary\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 crypt32.lib rpcrt4.lib wininet.lib dnsapi.lib ..\..\pktlib\nt\libs\pktlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ..\..\mibparse\mibdb\release\mibdb.lib ..\..\sqlitesrc\sqlite3.lib ..\..\smtpLibrary\lib\CkBaseRelMt.lib ..\..\smtpLibrary\lib\ChilkatRelMt.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcp.lib" /force

!ELSEIF  "$(CFG)" == "trThread - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\pktlib" /I "..\..\common" /I "..\..\ebase\include" /I ".." /I "..\..\mibParse\mibdb" /I "..\..\sqlitesrc" /I "..\..\smtpLibrary\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 nafxcwd.lib crypt32.lib rpcrt4.lib wininet.lib dnsapi.lib wsock32.lib ../../pktlib/nt/libs/pktlibd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ..\..\mibparse\mibdb\debug\mibdb.lib ..\..\sqlitesrc\sqlite3.lib ..\..\smtpLibrary\lib\CkBaseDbgMt.lib ..\..\smtpLibrary\lib\ChilkatDbgMt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcpd.lib" /pdbtype:sept
# SUBTRACT LINK32 /force

!ENDIF 

# Begin Target

# Name "trThread - Win32 Release"
# Name "trThread - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ActionMgrThread.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionThread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\Base64.cpp

!IF  "$(CFG)" == "trThread - Win32 Release"

!ELSEIF  "$(CFG)" == "trThread - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\common\ClientSocket.cpp

!IF  "$(CFG)" == "trThread - Win32 Release"

!ELSEIF  "$(CFG)" == "trThread - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FilterQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\FlusherThread.cpp

!IF  "$(CFG)" == "trThread - Win32 Release"

!ELSEIF  "$(CFG)" == "trThread - Win32 Debug"

# ADD CPP /FR

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\NTService.cpp

!IF  "$(CFG)" == "trThread - Win32 Release"

!ELSEIF  "$(CFG)" == "trThread - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\common\QueueServer.cpp

!IF  "$(CFG)" == "trThread - Win32 Release"

!ELSEIF  "$(CFG)" == "trThread - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\Smtp.cpp
# End Source File
# Begin Source File

SOURCE=.\TrapFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\TrapMatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\TrapRcvrMsgs.rc
# End Source File
# Begin Source File

SOURCE=..\..\common\TrapRcvrThread.cpp
# End Source File
# Begin Source File

SOURCE=.\TrapReceiverService.cpp
# End Source File
# Begin Source File

SOURCE=.\TrQueueClient.cpp
# End Source File
# Begin Source File

SOURCE=.\TrThread.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
