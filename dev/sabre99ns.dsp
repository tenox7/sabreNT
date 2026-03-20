# Microsoft Developer Studio Project File - Name="sabre99ns" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=sabre99ns - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sabre99ns.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sabre99ns.mak" CFG="sabre99ns - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sabre99ns - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "sabre99ns - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sabre99ns - Win32 Release"

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
# ADD CPP /nologo /Gr /W3 /GX /O2 /I ".\\" /I ".\src\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "SABREWIN" /D "USING_NEWPILOT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libzip.lib winmm.lib /nologo /subsystem:windows /pdb:none /map /machine:I386 /libpath:".\lib"
# Begin Custom Build
InputPath=.\Release\sabre99ns.exe
SOURCE=$(InputPath)

"c:\sabre\sabrens.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy .\Release\sabre99ns.exe c:\sabre\sabrens.exe

# End Custom Build

!ELSEIF  "$(CFG)" == "sabre99ns - Win32 Debug"

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
# ADD CPP /nologo /Gr /W3 /Gm /GX /Zi /Od /I ".\\" /I ".\src\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "SABREWIN" /D "USING_NEWPILOT" /D "SHOW_TEXTURES_DLG" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libzip.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:".\lib"

!ENDIF 

# Begin Target

# Name "sabre99ns - Win32 Release"
# Name "sabre99ns - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter ""
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

SOURCE=.\Src\aibase.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aibase.h
# End Source File
# Begin Source File

SOURCE=.\Src\aicomm.h
# End Source File
# Begin Source File

SOURCE=.\Src\aiflite.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aiflite.h
# End Source File
# Begin Source File

SOURCE=.\Src\aigunner.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\aigunner.h
# End Source File
# Begin Source File

SOURCE=.\SRC\aimath.h
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

SOURCE=.\Src\Dhlist.c
# End Source File
# Begin Source File

SOURCE=.\Src\Dhlist.h
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

SOURCE=.\Src\hud.cpp
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

SOURCE=.\Src\Pc_keys.h
# End Source File
# Begin Source File

SOURCE=.\Src\Pen.cpp
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

SOURCE=.\Src\Rtkey.h
# End Source File
# Begin Source File

SOURCE=.\Src\Sabrewin.cpp
# End Source File
# Begin Source File

SOURCE=.\Sabrewin.rc

!IF  "$(CFG)" == "sabre99ns - Win32 Release"

!ELSEIF  "$(CFG)" == "sabre99ns - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Src\sairfld.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\sairfld.h
# End Source File
# Begin Source File

SOURCE=.\Src\Sarray.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Sarray.h
# End Source File
# Begin Source File

SOURCE=.\Src\sattkr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\sattkr.h
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

SOURCE=.\Src\scntrl.h
# End Source File
# Begin Source File

SOURCE=.\Src\Sfltmdl.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Sfltmdl.h
# End Source File
# Begin Source File

SOURCE=.\Src\sfrmtn.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\sfrmtn.h
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

SOURCE=.\Src\Simgraf.h
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

SOURCE=.\Src\Smath.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Smath.h
# End Source File
# Begin Source File

SOURCE=.\Src\smnvrst.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\smnvrst.h
# End Source File
# Begin Source File

SOURCE=.\Src\Sobject.h
# End Source File
# Begin Source File

SOURCE=.\Src\Spid.c
# End Source File
# Begin Source File

SOURCE=.\Src\Spid.h
# End Source File
# Begin Source File

SOURCE=.\Src\spilcaps.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Spilcaps.h
# End Source File
# Begin Source File

SOURCE=.\Src\splncaps.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Splncaps.h
# End Source File
# Begin Source File

SOURCE=.\Src\srunway.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\srunway.h
# End Source File
# Begin Source File

SOURCE=.\Src\Sslewer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Sslewer.h
# End Source File
# Begin Source File

SOURCE=.\Src\Stact.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Stact.h
# End Source File
# Begin Source File

SOURCE=.\Src\Starget.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Starget.h
# End Source File
# Begin Source File

SOURCE=.\Src\stime.c
# End Source File
# Begin Source File

SOURCE=.\Src\stime.h
# End Source File
# Begin Source File

SOURCE=.\Src\Stypes.h
# End Source File
# Begin Source File

SOURCE=.\Src\swaypnt.h
# End Source File
# Begin Source File

SOURCE=.\Src\sweapon.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\sweapon.h
# End Source File
# Begin Source File

SOURCE=.\Src\Target.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Target.h
# End Source File
# Begin Source File

SOURCE=.\Src\Terrain.cpp
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
# End Group
# Begin Source File

SOURCE=.\src\Rndrplyx.cpp
# End Source File
# End Target
# End Project
