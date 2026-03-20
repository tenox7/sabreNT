# Microsoft Developer Studio Project File - Name="sabredx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=sabredx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sabredx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sabredx.mak" CFG="sabredx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sabredx - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "sabredx - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sabredx - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "sabredx_"
# PROP BASE Intermediate_Dir "sabredx_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "sabredx_"
# PROP Intermediate_Dir "sabredx_"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp16 /W3 /GX /O2 /I ".\\" /I ".\src\\" /I "\mssdk_7\include" /I ".\d3dframe" /D "NDEBUG" /D "USES_DSOUND" /D "WIN32" /D "_WINDOWS" /D "SABREWIN" /D "USES_DDRAW" /D "USES_D3D" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 dsound.lib d3drm.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libzip.lib winmm.lib ddraw.lib d3dframe.lib /nologo /subsystem:windows /machine:I386 /libpath:".\lib" /libpath:"\mssdk_7\lib" /libpath:".\d3dframe"

!ELSEIF  "$(CFG)" == "sabredx - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I ".\\" /I ".\src\\" /I "\mssdk_7\include" /I ".\d3dframe" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "SABREWIN" /D "USES_DDRAW" /D "USES_D3D" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libzip.lib winmm.lib ddraw.lib d3dframe.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:".\lib" /libpath:"\mssdk_7\lib" /libpath:".\d3dframe"

!ENDIF 

# Begin Target

# Name "sabredx - Win32 Release"
# Name "sabredx - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\aaaunit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\About.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\About.h
# End Source File
# Begin Source File

SOURCE=.\Src\ai2sbr.h
# End Source File
# Begin Source File

SOURCE=.\src\aiarray.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aiarray.h
# End Source File
# Begin Source File

SOURCE=.\src\aiattkr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aiattkr.h
# End Source File
# Begin Source File

SOURCE=.\src\aibase.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aibase.h
# End Source File
# Begin Source File

SOURCE=.\Src\aicomm.h
# End Source File
# Begin Source File

SOURCE=.\src\aiField.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aifield.h
# End Source File
# Begin Source File

SOURCE=.\src\aiflite.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aiflite.h
# End Source File
# Begin Source File

SOURCE=.\src\aifltmdl.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aifltmdl.h
# End Source File
# Begin Source File

SOURCE=.\src\aifrmtn.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aifrmtn.h
# End Source File
# Begin Source File

SOURCE=.\src\aigunner.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aigunner.h
# End Source File
# Begin Source File

SOURCE=.\src\aimath.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\aimath.h
# End Source File
# Begin Source File

SOURCE=.\src\aimnvrst.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aimnvrst.h
# End Source File
# Begin Source File

SOURCE=.\Src\aiobject.h
# End Source File
# Begin Source File

SOURCE=.\src\aipid.c
# End Source File
# Begin Source File

SOURCE=.\Src\aipid.h
# End Source File
# Begin Source File

SOURCE=.\src\aipilcaps.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aipilcaps.h
# End Source File
# Begin Source File

SOURCE=.\Src\aipildef.h
# End Source File
# Begin Source File

SOURCE=.\src\aipilot.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aipilot.h
# End Source File
# Begin Source File

SOURCE=.\src\aipilot2.cpp
# End Source File
# Begin Source File

SOURCE=.\src\aipilot3.cpp
# End Source File
# Begin Source File

SOURCE=.\src\aipilot4.cpp
# End Source File
# Begin Source File

SOURCE=.\src\aipilot5.cpp
# End Source File
# Begin Source File

SOURCE=.\src\aipilot6.cpp
# End Source File
# Begin Source File

SOURCE=.\src\aiplncaps.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aiplncaps.h
# End Source File
# Begin Source File

SOURCE=.\src\airunway.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\airunway.h
# End Source File
# Begin Source File

SOURCE=.\src\aislewer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aislewer.h
# End Source File
# Begin Source File

SOURCE=.\src\aitact.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aitact.h
# End Source File
# Begin Source File

SOURCE=.\src\aitarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aitarget.h
# End Source File
# Begin Source File

SOURCE=.\src\aitime.c
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

SOURCE=.\src\aiweapon.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aiweapon.h
# End Source File
# Begin Source File

SOURCE=.\src\Axidlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Axisdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Axisdlg.h
# End Source File
# Begin Source File

SOURCE=.\src\bits.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Bits.h
# End Source File
# Begin Source File

SOURCE=.\Src\Brsnline.h
# End Source File
# Begin Source File

SOURCE=.\src\clip.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Clip.h
# End Source File
# Begin Source File

SOURCE=.\Src\Clstypes.h
# End Source File
# Begin Source File

SOURCE=.\src\cmprtxtr.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cockpit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Cockpit.h
# End Source File
# Begin Source File

SOURCE=.\src\colormap.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Colormap.h
# End Source File
# Begin Source File

SOURCE=.\src\colorspc.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Colorspc.h
# End Source File
# Begin Source File

SOURCE=.\Src\Comp.h
# End Source File
# Begin Source File

SOURCE=.\src\convert.cpp
# End Source File
# Begin Source File

SOURCE=.\src\convert.h
# End Source File
# Begin Source File

SOURCE=.\src\convpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Convpoly.h
# End Source File
# Begin Source File

SOURCE=.\src\copoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Copoly.h
# End Source File
# Begin Source File

SOURCE=.\src\cpoly.cpp
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

SOURCE=.\src\Dhlist.c
# End Source File
# Begin Source File

SOURCE=.\Src\Dhlist.h
# End Source File
# Begin Source File

SOURCE=.\src\Dibsect.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dibsect.h
# End Source File
# Begin Source File

SOURCE=.\src\dvector.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dvmath.h
# End Source File
# Begin Source File

SOURCE=.\src\earth.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Earth.h
# End Source File
# Begin Source File

SOURCE=.\src\flight.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Flight.h
# End Source File
# Begin Source File

SOURCE=.\src\flightorg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\flipcube.cpp
# End Source File
# Begin Source File

SOURCE=.\src\fltlite.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Fltlite.h
# End Source File
# Begin Source File

SOURCE=.\src\fltmngr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Fltmngr.h
# End Source File
# Begin Source File

SOURCE=.\src\fltobj.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Fltobj.h
# End Source File
# Begin Source File

SOURCE=.\src\fltzview.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Fltzview.h
# End Source File
# Begin Source File

SOURCE=.\src\font8x8.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Font8x8.h
# End Source File
# Begin Source File

SOURCE=.\src\game.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Game.h
# End Source File
# Begin Source File

SOURCE=.\src\globals.cpp
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

SOURCE=.\src\grndunit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Grndunit.h
# End Source File
# Begin Source File

SOURCE=.\src\group_3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Group_3d.h
# End Source File
# Begin Source File

SOURCE=.\src\guineapig.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hud.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Hud.h
# End Source File
# Begin Source File

SOURCE=.\src\input.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Input.h
# End Source File
# Begin Source File

SOURCE=.\src\instrmnt.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Instrmnt.h
# End Source File
# Begin Source File

SOURCE=.\src\joydummy.h
# End Source File
# Begin Source File

SOURCE=.\src\kbdhit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\kbdhit.h
# End Source File
# Begin Source File

SOURCE=.\src\kbdmap.cpp
# End Source File
# Begin Source File

SOURCE=.\src\key_map.cpp
# End Source File
# Begin Source File

SOURCE=.\src\key_map.h
# End Source File
# Begin Source File

SOURCE=.\src\led2.cpp
# End Source File
# Begin Source File

SOURCE=.\src\linux_joy.cpp
# End Source File
# Begin Source File

SOURCE=.\src\linux_joy.h
# End Source File
# Begin Source File

SOURCE=.\src\main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Mission.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Mission.h
# End Source File
# Begin Source File

SOURCE=.\src\moveable.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Moveable.h
# End Source File
# Begin Source File

SOURCE=.\src\mytimer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Mytimer.h
# End Source File
# Begin Source File

SOURCE=.\src\obj_3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Obj_3d.h
# End Source File
# Begin Source File

SOURCE=.\Src\Pc_keys.h
# End Source File
# Begin Source File

SOURCE=.\src\pen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Pen.h
# End Source File
# Begin Source File

SOURCE=.\src\pilobj.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\pilobj.h
# End Source File
# Begin Source File

SOURCE=.\src\pilot.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Pilot.h
# End Source File
# Begin Source File

SOURCE=.\src\plltt.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Plltt.h
# End Source File
# Begin Source File

SOURCE=.\src\Polydlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Polydlg.h
# End Source File
# Begin Source File

SOURCE=.\src\port_3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Port_3d.h
# End Source File
# Begin Source File

SOURCE=.\src\portkey.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Portkey.h
# End Source File
# Begin Source File

SOURCE=.\src\ppm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\pursue.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rendpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Rendpoly.h
# End Source File
# Begin Source File

SOURCE=.\src\Rndrplyx.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rndrpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Rndrpoly.h
# End Source File
# Begin Source File

SOURCE=.\src\rndzpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Rndzpoly.h
# End Source File
# Begin Source File

SOURCE=.\src\rotate.cpp
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

SOURCE=.\src\Sabrewin.cpp
# End Source File
# Begin Source File

SOURCE=.\src\sbfltmdl.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\sbfltmdl.h
# End Source File
# Begin Source File

SOURCE=.\src\sbrkeys.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\sbrvkeys.h
# End Source File
# Begin Source File

SOURCE=.\src\Scenario.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Scenario.h
# End Source File
# Begin Source File

SOURCE=.\src\scnedit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Scnedit.h
# End Source File
# Begin Source File

SOURCE=.\Src\scntrl.h
# End Source File
# Begin Source File

SOURCE=.\src\sfacei.h
# End Source File
# Begin Source File

SOURCE=.\src\sFaceInfo.h
# End Source File
# Begin Source File

SOURCE=.\Src\Sim.h
# End Source File
# Begin Source File

SOURCE=.\Src\Simerr.h
# End Source File
# Begin Source File

SOURCE=.\src\simfile.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Simfile.h
# End Source File
# Begin Source File

SOURCE=.\src\Simfilex.cpp
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

SOURCE=.\src\siminput.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Siminput.h
# End Source File
# Begin Source File

SOURCE=.\src\Simmath.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Simmath.h
# End Source File
# Begin Source File

SOURCE=.\src\simsnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Simsnd.h
# End Source File
# Begin Source File

SOURCE=.\src\Target.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Target.h
# End Source File
# Begin Source File

SOURCE=.\src\terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Terrain.h
# End Source File
# Begin Source File

SOURCE=.\src\textures.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\textures.h
# End Source File
# Begin Source File

SOURCE=.\src\transblt.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Transblt.h
# End Source File
# Begin Source File

SOURCE=.\src\traveler.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Traveler.h
# End Source File
# Begin Source File

SOURCE=.\src\txtrmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Txtrmap.h
# End Source File
# Begin Source File

SOURCE=.\Src\Unc.h
# End Source File
# Begin Source File

SOURCE=.\src\unguided.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Unguided.h
# End Source File
# Begin Source File

SOURCE=.\Src\Values.h
# End Source File
# Begin Source File

SOURCE=.\src\vga_13.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Vga_13.h
# End Source File
# Begin Source File

SOURCE=.\src\vga_gdev.cpp
# End Source File
# Begin Source File

SOURCE=.\src\viewobj.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Viewobj.h
# End Source File
# Begin Source File

SOURCE=.\src\vmath.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Vmath.h
# End Source File
# Begin Source File

SOURCE=.\src\vtable2.cpp
# End Source File
# Begin Source File

SOURCE=.\src\weapons.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Weapons.h
# End Source File
# Begin Source File

SOURCE=.\src\wincntrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\wincntrl.h
# End Source File
# Begin Source File

SOURCE=.\src\winjoy.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Winjoy.h
# End Source File
# Begin Source File

SOURCE=.\src\winkeys.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\winkeys.h
# End Source File
# Begin Source File

SOURCE=.\src\winsndx.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\winsndx.h
# End Source File
# Begin Source File

SOURCE=.\Src\Wvga_13.h
# End Source File
# Begin Source File

SOURCE=.\src\zvedit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\zvedit.h
# End Source File
# Begin Source File

SOURCE=.\src\zview.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Zview.h
# End Source File
# End Group
# Begin Group "d3dframe"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\d3dframe\D3denum.h
# End Source File
# Begin Source File

SOURCE=.\d3dframe\D3dframe.h
# End Source File
# Begin Source File

SOURCE=.\d3dframe\D3dmath.h
# End Source File
# Begin Source File

SOURCE=.\d3dframe\D3dtextr.h
# End Source File
# Begin Source File

SOURCE=.\d3dframe\D3dutil.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\d3dtex.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\d3dtex.h
# End Source File
# Begin Source File

SOURCE=.\Src\Sabred3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\wvga13dx.cpp
# End Source File
# End Target
# End Project
