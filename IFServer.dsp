# Microsoft Developer Studio Project File - Name="IFServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=IFServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IFServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IFServer.mak" CFG="IFServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IFServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "IFServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IFServer - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "IFServer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 debug/libpcap.lib user32.lib wsock32.lib advapi32.lib iphlpapi.lib ole32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "IFServer - Win32 Release"
# Name "IFServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AlertScreenDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AlertSMB.cpp
# End Source File
# Begin Source File

SOURCE=.\AlertSysLog.cpp
# End Source File
# Begin Source File

SOURCE=.\AlertUnixSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\BalloonHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigA.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigB.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigC.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigD.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigE.cpp
# End Source File
# Begin Source File

SOURCE=.\DatabaseLog.cpp
# End Source File
# Begin Source File

SOURCE=.\Decode.cpp
# End Source File
# Begin Source File

SOURCE=.\Defrag.cpp
# End Source File
# Begin Source File

SOURCE=.\DSizeCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\FastAlert.cpp
# End Source File
# Begin Source File

SOURCE=.\FileManagement.cpp
# End Source File
# Begin Source File

SOURCE=.\FullAlert.cpp
# End Source File
# Begin Source File

SOURCE=.\HttpDecode.cpp
# End Source File
# Begin Source File

SOURCE=.\IcmpCodeCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\IcmpIdCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\IcmpSeqCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\IcmpTypeCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\IFServer.cpp
# End Source File
# Begin Source File

SOURCE=.\IFServer.rc
# End Source File
# Begin Source File

SOURCE=.\IFServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IFServerStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\IFServerSubDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IpIdCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\IpOptionCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\LibCap.cpp
# End Source File
# Begin Source File

SOURCE=.\LineColl.cpp
# End Source File
# Begin Source File

SOURCE=.\Log.cpp
# End Source File
# Begin Source File

SOURCE=.\Manipulate.cpp
# End Source File
# Begin Source File

SOURCE=.\Message.cpp
# End Source File
# Begin Source File

SOURCE=.\MinFrag.cpp
# End Source File
# Begin Source File

SOURCE=.\PatternMatch.cpp
# End Source File
# Begin Source File

SOURCE=.\PortScan.cpp
# End Source File
# Begin Source File

SOURCE=.\Preprocessor.cpp
# End Source File
# Begin Source File

SOURCE=.\Respond.cpp
# End Source File
# Begin Source File

SOURCE=.\RpcCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\Rules.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectAdaptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Session.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StringMan.cpp
# End Source File
# Begin Source File

SOURCE=.\StrMan.cpp
# End Source File
# Begin Source File

SOURCE=.\SysLog.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpAckCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpDumpLog.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpFlagCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpSeqCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\TrafficControl.cpp
# End Source File
# Begin Source File

SOURCE=.\TtlCheck.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AlertScreenDlg.h
# End Source File
# Begin Source File

SOURCE=.\AlertSMB.h
# End Source File
# Begin Source File

SOURCE=.\AlertSysLog.h
# End Source File
# Begin Source File

SOURCE=.\AlertUnixSocket.h
# End Source File
# Begin Source File

SOURCE=.\BalloonHelp.h
# End Source File
# Begin Source File

SOURCE=.\BPF.H
# End Source File
# Begin Source File

SOURCE=.\CapDll.h
# End Source File
# Begin Source File

SOURCE=.\ConfigA.h
# End Source File
# Begin Source File

SOURCE=.\ConfigB.h
# End Source File
# Begin Source File

SOURCE=.\ConfigC.h
# End Source File
# Begin Source File

SOURCE=.\ConfigD.h
# End Source File
# Begin Source File

SOURCE=.\ConfigE.h
# End Source File
# Begin Source File

SOURCE=.\DatabaseLog.h
# End Source File
# Begin Source File

SOURCE=.\Decode.h
# End Source File
# Begin Source File

SOURCE=.\Defrag.h
# End Source File
# Begin Source File

SOURCE=.\DSizeCheck.h
# End Source File
# Begin Source File

SOURCE=.\FastAlert.h
# End Source File
# Begin Source File

SOURCE=.\FileManagement.h
# End Source File
# Begin Source File

SOURCE=.\FullAlert.h
# End Source File
# Begin Source File

SOURCE=.\Gnuc.h
# End Source File
# Begin Source File

SOURCE=.\HttpDecode.h
# End Source File
# Begin Source File

SOURCE=.\IcmpCodeCheck.h
# End Source File
# Begin Source File

SOURCE=.\IcmpIdCheck.h
# End Source File
# Begin Source File

SOURCE=.\IcmpSeqCheck.h
# End Source File
# Begin Source File

SOURCE=.\IcmpTypeCheck.h
# End Source File
# Begin Source File

SOURCE=.\IFServer.h
# End Source File
# Begin Source File

SOURCE=.\IFServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\IFServerStatic.h
# End Source File
# Begin Source File

SOURCE=.\IFServerSubDlg.h
# End Source File
# Begin Source File

SOURCE=.\IPExport.h
# End Source File
# Begin Source File

SOURCE=.\IPHlpApi.h
# End Source File
# Begin Source File

SOURCE=.\IpIdCheck.h
# End Source File
# Begin Source File

SOURCE=.\IpOptionCheck.h
# End Source File
# Begin Source File

SOURCE=.\IPTypes.h
# End Source File
# Begin Source File

SOURCE=.\LibCap.h
# End Source File
# Begin Source File

SOURCE=.\LineColl.h
# End Source File
# Begin Source File

SOURCE=.\Log.h
# End Source File
# Begin Source File

SOURCE=.\Manipulate.h
# End Source File
# Begin Source File

SOURCE=.\Message.h
# End Source File
# Begin Source File

SOURCE=.\MinFrag.h
# End Source File
# Begin Source File

SOURCE=.\PatternMatch.h
# End Source File
# Begin Source File

SOURCE=".\pcap-namedb.h"
# End Source File
# Begin Source File

SOURCE=.\PCAP.H
# End Source File
# Begin Source File

SOURCE=.\PortScan.h
# End Source File
# Begin Source File

SOURCE=.\Preprocessor.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Respond.h
# End Source File
# Begin Source File

SOURCE=.\RpcCheck.h
# End Source File
# Begin Source File

SOURCE=.\Rules.h
# End Source File
# Begin Source File

SOURCE=.\SelectAdaptor.h
# End Source File
# Begin Source File

SOURCE=.\Session.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StringMan.h
# End Source File
# Begin Source File

SOURCE=.\StrMan.h
# End Source File
# Begin Source File

SOURCE=.\Structure.h
# End Source File
# Begin Source File

SOURCE=.\SysLog.h
# End Source File
# Begin Source File

SOURCE=.\TcpAckCheck.h
# End Source File
# Begin Source File

SOURCE=.\TcpDumpLog.h
# End Source File
# Begin Source File

SOURCE=.\TcpFlagCheck.h
# End Source File
# Begin Source File

SOURCE=.\TcpSeqCheck.h
# End Source File
# Begin Source File

SOURCE=.\TrafficControl.h
# End Source File
# Begin Source File

SOURCE=.\TtlCheck.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\Cap_Wizi.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon_sta.ico
# End Source File
# Begin Source File

SOURCE=.\res\IFServer.ico
# End Source File
# Begin Source File

SOURCE=.\res\IFServer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\indicato.bmp
# End Source File
# Begin Source File

SOURCE=I:\Rizwan\saprefs\res\prefs.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\Debug\LIBPCAP.LIB
# End Source File
# Begin Source File

SOURCE=.\WS2_32.LIB
# End Source File
# Begin Source File

SOURCE=.\WSOCK32.LIB
# End Source File
# Begin Source File

SOURCE=.\IPHlpApi.lib
# End Source File
# End Target
# End Project
