# Microsoft Developer Studio Project File - Name="sabre99" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=sabre99 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sabre99.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sabre99.mak" CFG="sabre99 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sabre99 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "sabre99 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sabre99 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "sabre99_"
# PROP BASE Intermediate_Dir "sabre99_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "sabre99_"
# PROP Intermediate_Dir "sabre99_"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /Gr /W3 /GX /O2 /I ".\\" /I ".\src\\" /I "\mssdk_7\include" /D "NDEBUG" /D "USES_DSOUND" /D "WIN32" /D "_WINDOWS" /D "SABREWIN" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libzip.lib winmm.lib dsound.lib /nologo /subsystem:windows /pdb:none /map /machine:I386 /libpath:".\lib" /libpath:"\mssdk_7\lib"
# SUBTRACT LINK32 /debug
# Begin Custom Build
InputPath=.\sabre99_\sabre99.exe
SOURCE=$(InputPath)

"c:\sabre\sabre.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy .\sabre99_\sabre99.exe c:\sabre\sabre.exe

# End Custom Build

!ELSEIF  "$(CFG)" == "sabre99 - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I ".\\" /I ".\src\\" /I "\mssdk\inc" /D "_DEBUG" /D "USING_DSOUND" /D "USES_DSOUND" /D "WIN32" /D "_WINDOWS" /D "SABREWIN" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libzip.lib winmm.lib dsound.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:".\lib" /libpath:"\dx5sdk\sdk\lib"

!ENDIF 

# Begin Target

# Name "sabre99 - Win32 Release"
# Name "sabre99 - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\aaaunit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\about.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\About.h
# End Source File
# Begin Source File

SOURCE=.\Src\ai2sbr.h
# End Source File
# Begin Source File

SOURCE=.\Src\aiarray.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aiarray.h
# End Source File
# Begin Source File

SOURCE=.\Src\aiattkr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aiattkr.h
# End Source File
# Begin Source File

SOURCE=.\Src\aibase.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aibase.h
# End Source File
# Begin Source File

SOURCE=.\Src\aicomm.h
# End Source File
# Begin Source File

SOURCE=.\Src\aiField.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aifield.h
# End Source File
# Begin Source File

SOURCE=.\Src\aiflite.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aiflite.h
# End Source File
# Begin Source File

SOURCE=.\Src\aifltmdl.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aifltmdl.h
# End Source File
# Begin Source File

SOURCE=.\Src\aifrmtn.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aifrmtn.h
# End Source File
# Begin Source File

SOURCE=.\Src\aigunner.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aigunner.h
# End Source File
# Begin Source File

SOURCE=.\Src\aimath.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\aimath.h
# End Source File
# Begin Source File

SOURCE=.\Src\aimnvrst.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aimnvrst.h
# End Source File
# Begin Source File

SOURCE=.\Src\aiobject.h
# End Source File
# Begin Source File

SOURCE=.\Src\aipid.c
# End Source File
# Begin Source File

SOURCE=.\Src\aipid.h
# End Source File
# Begin Source File

SOURCE=.\Src\aipilcaps.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aipilcaps.h
# End Source File
# Begin Source File

SOURCE=.\Src\aipildef.h
# End Source File
# Begin Source File

SOURCE=.\Src\aipilot.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aipilot.h
# End Source File
# Begin Source File

SOURCE=.\Src\aipilot2.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aipilot3.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aipilot4.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aipilot5.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aipilot6.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aiplncaps.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aiplncaps.h
# End Source File
# Begin Source File

SOURCE=.\Src\airunway.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\airunway.h
# End Source File
# Begin Source File

SOURCE=.\Src\aislewer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aislewer.h
# End Source File
# Begin Source File

SOURCE=.\Src\aitact.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aitact.h
# End Source File
# Begin Source File

SOURCE=.\Src\aitarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aitarget.h
# End Source File
# Begin Source File

SOURCE=.\Src\aitime.c
# End Source File
# Begin Source File

SOURCE=.\Src\aitime.h
# End Source File
# Begin Source File

SOURCE=.\Src\aitypes.h
# End Source File
# Begin Source File

SOURCE=.\Src\aiwaypnt.h
# End Source File
# Begin Source File

SOURCE=.\Src\aiweapon.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aiweapon.h
# End Source File
# Begin Source File

SOURCE=.\Src\bits.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Bits.h
# End Source File
# Begin Source File

SOURCE=.\Src\Brsnline.h
# End Source File
# Begin Source File

SOURCE=.\Src\clip.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Clip.h
# End Source File
# Begin Source File

SOURCE=.\Src\Clstypes.h
# End Source File
# Begin Source File

SOURCE=.\Src\cockpit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Cockpit.h
# End Source File
# Begin Source File

SOURCE=.\Src\colormap.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Colormap.h
# End Source File
# Begin Source File

SOURCE=.\Src\colorspc.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Colorspc.h
# End Source File
# Begin Source File

SOURCE=.\Src\convpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Convpoly.h
# End Source File
# Begin Source File

SOURCE=.\Src\copoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Copoly.h
# End Source File
# Begin Source File

SOURCE=.\Src\cpoly.cpp
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

SOURCE=.\Src\dhlist.c
# End Source File
# Begin Source File

SOURCE=.\Src\Dhlist.h
# End Source File
# Begin Source File

SOURCE=.\Src\dibsect.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dibsect.h
# End Source File
# Begin Source File

SOURCE=.\Src\earth.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Earth.h
# End Source File
# Begin Source File

SOURCE=.\Src\flight.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Flight.h
# End Source File
# Begin Source File

SOURCE=.\Src\fltlite.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Fltlite.h
# End Source File
# Begin Source File

SOURCE=.\Src\fltmngr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Fltmngr.h
# End Source File
# Begin Source File

SOURCE=.\Src\fltobj.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Fltobj.h
# End Source File
# Begin Source File

SOURCE=.\Src\fltzview.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Fltzview.h
# End Source File
# Begin Source File

SOURCE=.\Src\font8x8.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Font8x8.h
# End Source File
# Begin Source File

SOURCE=.\Src\game.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Game.h
# End Source File
# Begin Source File

SOURCE=.\Src\globals.cpp
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

SOURCE=.\Src\grndunit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Grndunit.h
# End Source File
# Begin Source File

SOURCE=.\Src\group_3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Group_3d.h
# End Source File
# Begin Source File

SOURCE=.\Src\hud.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Hud.h
# End Source File
# Begin Source File

SOURCE=.\Src\input.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Input.h
# End Source File
# Begin Source File

SOURCE=.\Src\instrmnt.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Instrmnt.h
# End Source File
# Begin Source File

SOURCE=.\Src\kbdhit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\kbdhit.h
# End Source File
# Begin Source File

SOURCE=.\Src\led2.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\mission.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Mission.h
# End Source File
# Begin Source File

SOURCE=.\Src\moveable.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Moveable.h
# End Source File
# Begin Source File

SOURCE=.\Src\mytimer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Mytimer.h
# End Source File
# Begin Source File

SOURCE=.\Src\obj_3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Obj_3d.h
# End Source File
# Begin Source File

SOURCE=.\Src\Pc_keys.h
# End Source File
# Begin Source File

SOURCE=.\Src\pen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Pen.h
# End Source File
# Begin Source File

SOURCE=.\Src\pilobj.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\pilobj.h
# End Source File
# Begin Source File

SOURCE=.\Src\pilot.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Pilot.h
# End Source File
# Begin Source File

SOURCE=.\Src\plltt.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Plltt.h
# End Source File
# Begin Source File

SOURCE=.\Src\port_3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Port_3d.h
# End Source File
# Begin Source File

SOURCE=.\Src\portkey.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Portkey.h
# End Source File
# Begin Source File

SOURCE=.\Src\ppm.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\rendpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Rendpoly.h
# End Source File
# Begin Source File

SOURCE=.\Src\rndrplyx.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\rndrpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Rndrpoly.h
# End Source File
# Begin Source File

SOURCE=.\Src\rndzpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Rndzpoly.h
# End Source File
# Begin Source File

SOURCE=.\Src\rotate.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Rotate.h
# End Source File
# Begin Source File

SOURCE=.\Src\rpoint.h
# End Source File
# Begin Source File

SOURCE=.\Src\Rtkey.h
# End Source File
# Begin Source File

SOURCE=.\Src\sabrewin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\sbfltmdl.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\sbfltmdl.h
# End Source File
# Begin Source File

SOURCE=.\Src\sbrvkeys.h
# End Source File
# Begin Source File

SOURCE=.\Src\scenario.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Scenario.h
# End Source File
# Begin Source File

SOURCE=.\Src\scnedit.cpp
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

SOURCE=.\Src\simfile.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Simfile.h
# End Source File
# Begin Source File

SOURCE=.\Src\simfilex.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Simfilex.h
# End Source File
# Begin Source File

SOURCE=.\Src\Simgraf.h
# End Source File
# Begin Source File

SOURCE=.\Src\Siminc.h
# End Source File
# Begin Source File

SOURCE=.\Src\siminput.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Siminput.h
# End Source File
# Begin Source File

SOURCE=.\Src\simmath.cpp
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

SOURCE=.\Src\target.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Target.h
# End Source File
# Begin Source File

SOURCE=.\Src\terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Terrain.h
# End Source File
# Begin Source File

SOURCE=.\Src\textures.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\textures.h
# End Source File
# Begin Source File

SOURCE=.\Src\transblt.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Transblt.h
# End Source File
# Begin Source File

SOURCE=.\Src\traveler.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Traveler.h
# End Source File
# Begin Source File

SOURCE=.\Src\txtrmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Txtrmap.h
# End Source File
# Begin Source File

SOURCE=.\Src\Unc.h
# End Source File
# Begin Source File

SOURCE=.\Src\unguided.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Unguided.h
# End Source File
# Begin Source File

SOURCE=.\Src\Values.h
# End Source File
# Begin Source File

SOURCE=.\Src\viewobj.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Viewobj.h
# End Source File
# Begin Source File

SOURCE=.\Src\vmath.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Vmath.h
# End Source File
# Begin Source File

SOURCE=.\Src\vtable2.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\weapons.cpp
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

SOURCE=.\Src\winsndx.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\winsndx.h
# End Source File
# Begin Source File

SOURCE=.\Src\wvga_13.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Wvga_13.h
# End Source File
# Begin Source File

SOURCE=.\Src\zview.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Zview.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\sabre.ico
# End Source File
# Begin Source File

SOURCE=.\Sabrewin.rc
# End Source File
# End Target
# End Project
