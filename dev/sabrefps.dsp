# Microsoft Developer Studio Project File - Name="sabrefps" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=sabrefps - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sabrefps.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sabrefps.mak" CFG="sabrefps - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sabrefps - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "sabrefps - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sabrefps - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /Gd /W3 /GX /O2 /I ".\\" /I ".\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "SABREWIN" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libzip.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"Release/sabre.exe" /libpath:".\lib"
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Desc=copy
PostBuild_Cmds=copy release\sabre.exe c:\sabre\sabre.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "sabrefps - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I ".\src" /I ".\src98" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "SABREWIN" /D VERION="0.2.3" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libzip.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "sabrefps - Win32 Release"
# Name "sabrefps - Win32 Debug"
# Begin Source File

SOURCE=.\Src\Aaaunit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\About.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\About.h
# End Source File
# Begin Source File

SOURCE=.\Src\Bits.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Bits.h
# End Source File
# Begin Source File

SOURCE=.\Src\Brsnline.h
# End Source File
# Begin Source File

SOURCE=.\Src\Clip.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Clip.h
# End Source File
# Begin Source File

SOURCE=.\Src\Clstypes.h
# End Source File
# Begin Source File

SOURCE=.\Src\Cockpit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Cockpit.h
# End Source File
# Begin Source File

SOURCE=.\Src\Colormap.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Colormap.h
# End Source File
# Begin Source File

SOURCE=.\Src\Colorspc.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Colorspc.h
# End Source File
# Begin Source File

SOURCE=.\Src\Comp.h
# End Source File
# Begin Source File

SOURCE=.\Src\Convert.h
# End Source File
# Begin Source File

SOURCE=.\Src\Convpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Convpoly.h
# End Source File
# Begin Source File

SOURCE=.\Src\Copoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Copoly.h
# End Source File
# Begin Source File

SOURCE=.\Src\Cpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Cpoly.h
# End Source File
# Begin Source File

SOURCE=.\Src\Ddaline.h
# End Source File
# Begin Source File

SOURCE=.\Src\Defs.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dibsect.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dibsect.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dvector.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dvmath.h
# End Source File
# Begin Source File

SOURCE=.\Src\Earth.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Earth.h
# End Source File
# Begin Source File

SOURCE=.\Src\Flight.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Flight.h
# End Source File
# Begin Source File

SOURCE=.\Src\Flite.h
# End Source File
# Begin Source File

SOURCE=.\Src\Fltlite.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Fltlite.h
# End Source File
# Begin Source File

SOURCE=.\Src\Fltmngr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Fltmngr.h
# End Source File
# Begin Source File

SOURCE=.\Src\Fltobj.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Fltobj.h
# End Source File
# Begin Source File

SOURCE=.\Src\Fltzview.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Fltzview.h
# End Source File
# Begin Source File

SOURCE=.\Src\Font8x8.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Font8x8.h
# End Source File
# Begin Source File

SOURCE=.\Src\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Game.h
# End Source File
# Begin Source File

SOURCE=.\Src\Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Globals.h
# End Source File
# Begin Source File

SOURCE=.\Src\Grafix.h
# End Source File
# Begin Source File

SOURCE=.\Src\Grndlevl.h
# End Source File
# Begin Source File

SOURCE=.\Src\Grndunit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Grndunit.h
# End Source File
# Begin Source File

SOURCE=.\Src\Group_3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Group_3d.h
# End Source File
# Begin Source File

SOURCE=.\Src\Hud.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Hud.h
# End Source File
# Begin Source File

SOURCE=.\Src\Input.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Input.h
# End Source File
# Begin Source File

SOURCE=.\Src\Instrmnt.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Instrmnt.h
# End Source File
# Begin Source File

SOURCE=.\Src\Led2.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Mission.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Mission.h
# End Source File
# Begin Source File

SOURCE=.\Src\Moveable.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Moveable.h
# End Source File
# Begin Source File

SOURCE=.\Src\Mytimer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Mytimer.h
# End Source File
# Begin Source File

SOURCE=.\Src\Obj_3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Obj_3d.h
# End Source File
# Begin Source File

SOURCE=.\Src\Pen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Pen.h
# End Source File
# Begin Source File

SOURCE=.\Src\Pilot.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Pilot.h
# End Source File
# Begin Source File

SOURCE=.\Src\Plltt.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Plltt.h
# End Source File
# Begin Source File

SOURCE=.\Src\Port_3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Port_3d.h
# End Source File
# Begin Source File

SOURCE=.\Src\Portkey.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Portkey.h
# End Source File
# Begin Source File

SOURCE=.\Src\Ppm.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Rendpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Rendpoly.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Src\Rndrpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Rndrpoly.h
# End Source File
# Begin Source File

SOURCE=.\Src\Rndzpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Rndzpoly.h
# End Source File
# Begin Source File

SOURCE=.\Src\Rotate.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Rotate.h
# End Source File
# Begin Source File

SOURCE=.\Src\Rpoint.h
# End Source File
# Begin Source File

SOURCE=.\Src\Rtkey.h
# End Source File
# Begin Source File

SOURCE=.\sabre.ico
# End Source File
# Begin Source File

SOURCE=.\Src\Sabrewin.cpp
# End Source File
# Begin Source File

SOURCE=.\Sabrewin.rc
# End Source File
# Begin Source File

SOURCE=.\Src\sbrvkeys.h
# End Source File
# Begin Source File

SOURCE=.\Src\Scenario.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Scenario.h
# End Source File
# Begin Source File

SOURCE=.\Src\Scnedit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Scnedit.h
# End Source File
# Begin Source File

SOURCE=.\Src\Sim.h
# End Source File
# Begin Source File

SOURCE=.\Src\Simerr.h
# End Source File
# Begin Source File

SOURCE=.\Src\Simfile.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Simfile.h
# End Source File
# Begin Source File

SOURCE=.\Src\Simfilex.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Simfilex.h
# End Source File
# Begin Source File

SOURCE=.\Src\Siminc.h
# End Source File
# Begin Source File

SOURCE=.\Src\Siminput.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Siminput.h
# End Source File
# Begin Source File

SOURCE=.\Src\Simmath.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Simmath.h
# End Source File
# Begin Source File

SOURCE=.\Src\simsnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Simsnd.h
# End Source File
# Begin Source File

SOURCE=.\Src\Target.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Target.h
# End Source File
# Begin Source File

SOURCE=.\Src\Terrain.cpp

!IF  "$(CFG)" == "sabrefps - Win32 Release"

# SUBTRACT CPP /O<none>

!ELSEIF  "$(CFG)" == "sabrefps - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Src\Terrain.h
# End Source File
# Begin Source File

SOURCE=.\Src\Transblt.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Transblt.h
# End Source File
# Begin Source File

SOURCE=.\Src\Traveler.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Traveler.h
# End Source File
# Begin Source File

SOURCE=.\Src\Txtrmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Txtrmap.h
# End Source File
# Begin Source File

SOURCE=.\Src\Unc.h
# End Source File
# Begin Source File

SOURCE=.\Src\Unguided.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Unguided.h
# End Source File
# Begin Source File

SOURCE=.\Src\Values.h
# End Source File
# Begin Source File

SOURCE=.\Src\Vga_13.h
# End Source File
# Begin Source File

SOURCE=.\Src\Viewobj.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Viewobj.h
# End Source File
# Begin Source File

SOURCE=.\Src\Vmath.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Vmath.h
# End Source File
# Begin Source File

SOURCE=.\Src\Vtable2.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Waypoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Waypoint.h
# End Source File
# Begin Source File

SOURCE=.\Src\Weapons.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Weapons.h
# End Source File
# Begin Source File

SOURCE=.\Src\wincntrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\wincntrl.h
# End Source File
# Begin Source File

SOURCE=.\Src\winjoy.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Winjoy.h
# End Source File
# Begin Source File

SOURCE=.\Src\winkeys.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\winkeys.h
# End Source File
# Begin Source File

SOURCE=.\Src\Wvga_13.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Wvga_13.h
# End Source File
# Begin Source File

SOURCE=.\Src\Zview.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Zview.h
# End Source File
# End Target
# End Project
