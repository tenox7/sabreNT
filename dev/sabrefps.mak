# Microsoft Developer Studio Generated NMAKE File, Based on sabrefps.dsp
!IF "$(CFG)" == ""
CFG=sabrefps - Win32 Debug
!MESSAGE No configuration specified. Defaulting to sabrefps - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "sabrefps - Win32 Release" && "$(CFG)" !=\
 "sabrefps - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "sabrefps - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\sabre.exe"

!ELSE 

ALL : "$(OUTDIR)\sabre.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Aaaunit.obj"
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Bits.obj"
	-@erase "$(INTDIR)\Clip.obj"
	-@erase "$(INTDIR)\Cockpit.obj"
	-@erase "$(INTDIR)\Colormap.obj"
	-@erase "$(INTDIR)\Colorspc.obj"
	-@erase "$(INTDIR)\Convpoly.obj"
	-@erase "$(INTDIR)\Copoly.obj"
	-@erase "$(INTDIR)\Cpoly.obj"
	-@erase "$(INTDIR)\Dibsect.obj"
	-@erase "$(INTDIR)\Dvector.obj"
	-@erase "$(INTDIR)\Earth.obj"
	-@erase "$(INTDIR)\Flight.obj"
	-@erase "$(INTDIR)\Fltlite.obj"
	-@erase "$(INTDIR)\Fltmngr.obj"
	-@erase "$(INTDIR)\Fltobj.obj"
	-@erase "$(INTDIR)\Fltzview.obj"
	-@erase "$(INTDIR)\Font8x8.obj"
	-@erase "$(INTDIR)\Game.obj"
	-@erase "$(INTDIR)\Globals.obj"
	-@erase "$(INTDIR)\Grndunit.obj"
	-@erase "$(INTDIR)\Group_3d.obj"
	-@erase "$(INTDIR)\Hud.obj"
	-@erase "$(INTDIR)\Input.obj"
	-@erase "$(INTDIR)\Instrmnt.obj"
	-@erase "$(INTDIR)\Key_map.obj"
	-@erase "$(INTDIR)\Led2.obj"
	-@erase "$(INTDIR)\Mission.obj"
	-@erase "$(INTDIR)\Moveable.obj"
	-@erase "$(INTDIR)\Mytimer.obj"
	-@erase "$(INTDIR)\Obj_3d.obj"
	-@erase "$(INTDIR)\Pen.obj"
	-@erase "$(INTDIR)\Pilot.obj"
	-@erase "$(INTDIR)\Plltt.obj"
	-@erase "$(INTDIR)\Port_3d.obj"
	-@erase "$(INTDIR)\Portkey.obj"
	-@erase "$(INTDIR)\Ppm.obj"
	-@erase "$(INTDIR)\Rendpoly.obj"
	-@erase "$(INTDIR)\Rndrpoly.obj"
	-@erase "$(INTDIR)\Rndzpoly.obj"
	-@erase "$(INTDIR)\Rotate.obj"
	-@erase "$(INTDIR)\Sabrewin.obj"
	-@erase "$(INTDIR)\Sabrewin.res"
	-@erase "$(INTDIR)\Scenario.obj"
	-@erase "$(INTDIR)\Scnedit.obj"
	-@erase "$(INTDIR)\Simfile.obj"
	-@erase "$(INTDIR)\Simfilex.obj"
	-@erase "$(INTDIR)\Siminput.obj"
	-@erase "$(INTDIR)\Simmath.obj"
	-@erase "$(INTDIR)\Smath.obj"
	-@erase "$(INTDIR)\Target.obj"
	-@erase "$(INTDIR)\Terrain.obj"
	-@erase "$(INTDIR)\Transblt.obj"
	-@erase "$(INTDIR)\Traveler.obj"
	-@erase "$(INTDIR)\Txtrmap.obj"
	-@erase "$(INTDIR)\Unguided.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\Viewobj.obj"
	-@erase "$(INTDIR)\Vmath.obj"
	-@erase "$(INTDIR)\Vtable2.obj"
	-@erase "$(INTDIR)\Waypoint.obj"
	-@erase "$(INTDIR)\Weapons.obj"
	-@erase "$(INTDIR)\Wvga_13.obj"
	-@erase "$(INTDIR)\Zview.obj"
	-@erase "$(OUTDIR)\sabre.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "SABREWIN" /Fp"$(INTDIR)\sabrefps.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Sabrewin.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sabrefps.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib libzip.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\sabre.pdb" /machine:I386 /out:"$(OUTDIR)\sabre.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Aaaunit.obj" \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Bits.obj" \
	"$(INTDIR)\Clip.obj" \
	"$(INTDIR)\Cockpit.obj" \
	"$(INTDIR)\Colormap.obj" \
	"$(INTDIR)\Colorspc.obj" \
	"$(INTDIR)\Convpoly.obj" \
	"$(INTDIR)\Copoly.obj" \
	"$(INTDIR)\Cpoly.obj" \
	"$(INTDIR)\Dibsect.obj" \
	"$(INTDIR)\Dvector.obj" \
	"$(INTDIR)\Earth.obj" \
	"$(INTDIR)\Flight.obj" \
	"$(INTDIR)\Fltlite.obj" \
	"$(INTDIR)\Fltmngr.obj" \
	"$(INTDIR)\Fltobj.obj" \
	"$(INTDIR)\Fltzview.obj" \
	"$(INTDIR)\Font8x8.obj" \
	"$(INTDIR)\Game.obj" \
	"$(INTDIR)\Globals.obj" \
	"$(INTDIR)\Grndunit.obj" \
	"$(INTDIR)\Group_3d.obj" \
	"$(INTDIR)\Hud.obj" \
	"$(INTDIR)\Input.obj" \
	"$(INTDIR)\Instrmnt.obj" \
	"$(INTDIR)\Key_map.obj" \
	"$(INTDIR)\Led2.obj" \
	"$(INTDIR)\Mission.obj" \
	"$(INTDIR)\Moveable.obj" \
	"$(INTDIR)\Mytimer.obj" \
	"$(INTDIR)\Obj_3d.obj" \
	"$(INTDIR)\Pen.obj" \
	"$(INTDIR)\Pilot.obj" \
	"$(INTDIR)\Plltt.obj" \
	"$(INTDIR)\Port_3d.obj" \
	"$(INTDIR)\Portkey.obj" \
	"$(INTDIR)\Ppm.obj" \
	"$(INTDIR)\Rendpoly.obj" \
	"$(INTDIR)\Rndrpoly.obj" \
	"$(INTDIR)\Rndzpoly.obj" \
	"$(INTDIR)\Rotate.obj" \
	"$(INTDIR)\Sabrewin.obj" \
	"$(INTDIR)\Sabrewin.res" \
	"$(INTDIR)\Scenario.obj" \
	"$(INTDIR)\Scnedit.obj" \
	"$(INTDIR)\Simfile.obj" \
	"$(INTDIR)\Simfilex.obj" \
	"$(INTDIR)\Siminput.obj" \
	"$(INTDIR)\Simmath.obj" \
	"$(INTDIR)\Smath.obj" \
	"$(INTDIR)\Target.obj" \
	"$(INTDIR)\Terrain.obj" \
	"$(INTDIR)\Transblt.obj" \
	"$(INTDIR)\Traveler.obj" \
	"$(INTDIR)\Txtrmap.obj" \
	"$(INTDIR)\Unguided.obj" \
	"$(INTDIR)\Viewobj.obj" \
	"$(INTDIR)\Vmath.obj" \
	"$(INTDIR)\Vtable2.obj" \
	"$(INTDIR)\Waypoint.obj" \
	"$(INTDIR)\Weapons.obj" \
	"$(INTDIR)\Wvga_13.obj" \
	"$(INTDIR)\Zview.obj"

"$(OUTDIR)\sabre.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=$(InputPath)
PostBuild_Desc=copy
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\sabre.exe"
   copy release\sabre.exe c:\sabre\sabre.exe
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "sabrefps - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\sabrefps.exe"

!ELSE 

ALL : "$(OUTDIR)\sabrefps.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Aaaunit.obj"
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Bits.obj"
	-@erase "$(INTDIR)\Clip.obj"
	-@erase "$(INTDIR)\Cockpit.obj"
	-@erase "$(INTDIR)\Colormap.obj"
	-@erase "$(INTDIR)\Colorspc.obj"
	-@erase "$(INTDIR)\Convpoly.obj"
	-@erase "$(INTDIR)\Copoly.obj"
	-@erase "$(INTDIR)\Cpoly.obj"
	-@erase "$(INTDIR)\Dibsect.obj"
	-@erase "$(INTDIR)\Dvector.obj"
	-@erase "$(INTDIR)\Earth.obj"
	-@erase "$(INTDIR)\Flight.obj"
	-@erase "$(INTDIR)\Fltlite.obj"
	-@erase "$(INTDIR)\Fltmngr.obj"
	-@erase "$(INTDIR)\Fltobj.obj"
	-@erase "$(INTDIR)\Fltzview.obj"
	-@erase "$(INTDIR)\Font8x8.obj"
	-@erase "$(INTDIR)\Game.obj"
	-@erase "$(INTDIR)\Globals.obj"
	-@erase "$(INTDIR)\Grndunit.obj"
	-@erase "$(INTDIR)\Group_3d.obj"
	-@erase "$(INTDIR)\Hud.obj"
	-@erase "$(INTDIR)\Input.obj"
	-@erase "$(INTDIR)\Instrmnt.obj"
	-@erase "$(INTDIR)\Key_map.obj"
	-@erase "$(INTDIR)\Led2.obj"
	-@erase "$(INTDIR)\Mission.obj"
	-@erase "$(INTDIR)\Moveable.obj"
	-@erase "$(INTDIR)\Mytimer.obj"
	-@erase "$(INTDIR)\Obj_3d.obj"
	-@erase "$(INTDIR)\Pen.obj"
	-@erase "$(INTDIR)\Pilot.obj"
	-@erase "$(INTDIR)\Plltt.obj"
	-@erase "$(INTDIR)\Port_3d.obj"
	-@erase "$(INTDIR)\Portkey.obj"
	-@erase "$(INTDIR)\Ppm.obj"
	-@erase "$(INTDIR)\Rendpoly.obj"
	-@erase "$(INTDIR)\Rndrpoly.obj"
	-@erase "$(INTDIR)\Rndzpoly.obj"
	-@erase "$(INTDIR)\Rotate.obj"
	-@erase "$(INTDIR)\Sabrewin.obj"
	-@erase "$(INTDIR)\Sabrewin.res"
	-@erase "$(INTDIR)\Scenario.obj"
	-@erase "$(INTDIR)\Scnedit.obj"
	-@erase "$(INTDIR)\Simfile.obj"
	-@erase "$(INTDIR)\Simfilex.obj"
	-@erase "$(INTDIR)\Siminput.obj"
	-@erase "$(INTDIR)\Simmath.obj"
	-@erase "$(INTDIR)\Smath.obj"
	-@erase "$(INTDIR)\Target.obj"
	-@erase "$(INTDIR)\Terrain.obj"
	-@erase "$(INTDIR)\Transblt.obj"
	-@erase "$(INTDIR)\Traveler.obj"
	-@erase "$(INTDIR)\Txtrmap.obj"
	-@erase "$(INTDIR)\Unguided.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\Viewobj.obj"
	-@erase "$(INTDIR)\Vmath.obj"
	-@erase "$(INTDIR)\Vtable2.obj"
	-@erase "$(INTDIR)\Waypoint.obj"
	-@erase "$(INTDIR)\Weapons.obj"
	-@erase "$(INTDIR)\Wvga_13.obj"
	-@erase "$(INTDIR)\Zview.obj"
	-@erase "$(OUTDIR)\sabrefps.exe"
	-@erase "$(OUTDIR)\sabrefps.ilk"
	-@erase "$(OUTDIR)\sabrefps.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "SABREWIN" /D VERION="0.2.3" /Fp"$(INTDIR)\sabrefps.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Sabrewin.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sabrefps.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib libzip.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\sabrefps.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\sabrefps.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Aaaunit.obj" \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Bits.obj" \
	"$(INTDIR)\Clip.obj" \
	"$(INTDIR)\Cockpit.obj" \
	"$(INTDIR)\Colormap.obj" \
	"$(INTDIR)\Colorspc.obj" \
	"$(INTDIR)\Convpoly.obj" \
	"$(INTDIR)\Copoly.obj" \
	"$(INTDIR)\Cpoly.obj" \
	"$(INTDIR)\Dibsect.obj" \
	"$(INTDIR)\Dvector.obj" \
	"$(INTDIR)\Earth.obj" \
	"$(INTDIR)\Flight.obj" \
	"$(INTDIR)\Fltlite.obj" \
	"$(INTDIR)\Fltmngr.obj" \
	"$(INTDIR)\Fltobj.obj" \
	"$(INTDIR)\Fltzview.obj" \
	"$(INTDIR)\Font8x8.obj" \
	"$(INTDIR)\Game.obj" \
	"$(INTDIR)\Globals.obj" \
	"$(INTDIR)\Grndunit.obj" \
	"$(INTDIR)\Group_3d.obj" \
	"$(INTDIR)\Hud.obj" \
	"$(INTDIR)\Input.obj" \
	"$(INTDIR)\Instrmnt.obj" \
	"$(INTDIR)\Key_map.obj" \
	"$(INTDIR)\Led2.obj" \
	"$(INTDIR)\Mission.obj" \
	"$(INTDIR)\Moveable.obj" \
	"$(INTDIR)\Mytimer.obj" \
	"$(INTDIR)\Obj_3d.obj" \
	"$(INTDIR)\Pen.obj" \
	"$(INTDIR)\Pilot.obj" \
	"$(INTDIR)\Plltt.obj" \
	"$(INTDIR)\Port_3d.obj" \
	"$(INTDIR)\Portkey.obj" \
	"$(INTDIR)\Ppm.obj" \
	"$(INTDIR)\Rendpoly.obj" \
	"$(INTDIR)\Rndrpoly.obj" \
	"$(INTDIR)\Rndzpoly.obj" \
	"$(INTDIR)\Rotate.obj" \
	"$(INTDIR)\Sabrewin.obj" \
	"$(INTDIR)\Sabrewin.res" \
	"$(INTDIR)\Scenario.obj" \
	"$(INTDIR)\Scnedit.obj" \
	"$(INTDIR)\Simfile.obj" \
	"$(INTDIR)\Simfilex.obj" \
	"$(INTDIR)\Siminput.obj" \
	"$(INTDIR)\Simmath.obj" \
	"$(INTDIR)\Smath.obj" \
	"$(INTDIR)\Target.obj" \
	"$(INTDIR)\Terrain.obj" \
	"$(INTDIR)\Transblt.obj" \
	"$(INTDIR)\Traveler.obj" \
	"$(INTDIR)\Txtrmap.obj" \
	"$(INTDIR)\Unguided.obj" \
	"$(INTDIR)\Viewobj.obj" \
	"$(INTDIR)\Vmath.obj" \
	"$(INTDIR)\Vtable2.obj" \
	"$(INTDIR)\Waypoint.obj" \
	"$(INTDIR)\Weapons.obj" \
	"$(INTDIR)\Wvga_13.obj" \
	"$(INTDIR)\Zview.obj"

"$(OUTDIR)\sabrefps.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "sabrefps - Win32 Release" || "$(CFG)" ==\
 "sabrefps - Win32 Debug"
SOURCE=.\Src98\Smath.cpp
DEP_CPP_SMATH=\
	".\Src98\Smath.h"\
	

"$(INTDIR)\Smath.obj" : $(SOURCE) $(DEP_CPP_SMATH) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Aaaunit.cpp
DEP_CPP_AAAUN=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Flite.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltobj.h"\
	".\Src\Fltzview.h"\
	".\Src\Grafix.h"\
	".\Src\Grndunit.h"\
	".\Src\Moveable.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pilot.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Simsnd.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Unguided.h"\
	".\Src\Vga_13.h"\
	".\Src\Viewobj.h"\
	".\Src\Vmath.h"\
	".\Src\Waypoint.h"\
	".\Src\Weapons.h"\
	".\Src\Zview.h"\
	

"$(INTDIR)\Aaaunit.obj" : $(SOURCE) $(DEP_CPP_AAAUN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\About.cpp
DEP_CPP_ABOUT=\
	".\Src\About.h"\
	".\Src\sabrewin.rh"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Bits.cpp
DEP_CPP_BITS_=\
	".\Src\Bits.h"\
	".\Src\Clip.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Port_3d.h"\
	".\Src\Rtkey.h"\
	".\Src\Simerr.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Bits.obj" : $(SOURCE) $(DEP_CPP_BITS_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Clip.cpp
DEP_CPP_CLIP_=\
	".\Src\Clip.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Clip.obj" : $(SOURCE) $(DEP_CPP_CLIP_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Cockpit.cpp
DEP_CPP_COCKP=\
	".\Src\Bits.h"\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Cockpit.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Fltobj.h"\
	".\Src\Font8x8.h"\
	".\Src\Grafix.h"\
	".\Src\Instrmnt.h"\
	".\Src\Pen.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rotate.h"\
	".\Src\Rtkey.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Target.h"\
	".\Src\Transblt.h"\
	".\Src\Traveler.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Cockpit.obj" : $(SOURCE) $(DEP_CPP_COCKP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Colormap.cpp
DEP_CPP_COLOR=\
	".\Src\Colormap.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	

"$(INTDIR)\Colormap.obj" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Colorspc.cpp
DEP_CPP_COLORS=\
	".\Src\Colorspc.h"\
	

"$(INTDIR)\Colorspc.obj" : $(SOURCE) $(DEP_CPP_COLORS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Convpoly.cpp
DEP_CPP_CONVP=\
	".\Src\Bits.h"\
	".\Src\Convpoly.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Pen.h"\
	".\Src\Simerr.h"\
	".\Src\Traveler.h"\
	".\Src\Vga_13.h"\
	

"$(INTDIR)\Convpoly.obj" : $(SOURCE) $(DEP_CPP_CONVP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Copoly.cpp
DEP_CPP_COPOL=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Pen.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Simerr.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Copoly.obj" : $(SOURCE) $(DEP_CPP_COPOL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Cpoly.cpp
DEP_CPP_CPOLY=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Convpoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rndzpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Cpoly.obj" : $(SOURCE) $(DEP_CPP_CPOLY) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Dibsect.cpp
DEP_CPP_DIBSE=\
	".\Src\Dibsect.h"\
	

"$(INTDIR)\Dibsect.obj" : $(SOURCE) $(DEP_CPP_DIBSE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Dvector.cpp
DEP_CPP_DVECT=\
	".\Src\Dvmath.h"\
	".\Src\Vmath.h"\
	
NODEP_CPP_DVECT=\
	".\Src\fixpoint.h"\
	

"$(INTDIR)\Dvector.obj" : $(SOURCE) $(DEP_CPP_DVECT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Earth.cpp
DEP_CPP_EARTH=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Convpoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Earth.h"\
	".\Src\Grafix.h"\
	".\Src\Grndlevl.h"\
	".\Src\Group_3d.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pen.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rotate.h"\
	".\Src\Rtkey.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Target.h"\
	".\Src\Terrain.h"\
	".\Src\Traveler.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Earth.obj" : $(SOURCE) $(DEP_CPP_EARTH) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Flight.cpp
DEP_CPP_FLIGH=\
	".\Src\Clip.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Fltobj.h"\
	".\Src\Grafix.h"\
	".\Src\Pc_keys.h"\
	".\Src\Port_3d.h"\
	".\Src\Rtkey.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Flight.obj" : $(SOURCE) $(DEP_CPP_FLIGH) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Fltlite.cpp
DEP_CPP_FLTLI=\
	".\Src\Clip.h"\
	".\Src\Colorspc.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltobj.h"\
	".\Src\Grafix.h"\
	".\Src\Port_3d.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Fltlite.obj" : $(SOURCE) $(DEP_CPP_FLTLI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Fltmngr.cpp
DEP_CPP_FLTMN=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Flite.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltmngr.h"\
	".\Src\Fltobj.h"\
	".\Src\Fltzview.h"\
	".\Src\Grafix.h"\
	".\Src\Moveable.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pilot.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Simfilex.h"\
	".\Src\Siminput.h"\
	".\Src\Simsnd.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Unguided.h"\
	".\Src\Vga_13.h"\
	".\Src\Viewobj.h"\
	".\Src\Vmath.h"\
	".\Src\Waypoint.h"\
	".\Src\Weapons.h"\
	".\Src\Zview.h"\
	

"$(INTDIR)\Fltmngr.obj" : $(SOURCE) $(DEP_CPP_FLTMN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Fltobj.cpp
DEP_CPP_FLTOB=\
	".\Src\Clip.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Fltobj.h"\
	".\Src\Grafix.h"\
	".\Src\Pc_keys.h"\
	".\Src\Port_3d.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Fltobj.obj" : $(SOURCE) $(DEP_CPP_FLTOB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Fltzview.cpp
DEP_CPP_FLTZV=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltobj.h"\
	".\Src\Fltzview.h"\
	".\Src\Font8x8.h"\
	".\Src\Grafix.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pc_keys.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	".\Src\Weapons.h"\
	".\Src\Zview.h"\
	

"$(INTDIR)\Fltzview.obj" : $(SOURCE) $(DEP_CPP_FLTZV) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Font8x8.cpp
DEP_CPP_FONT8=\
	".\Src\Defs.h"\
	".\Src\Font8x8.h"\
	".\Src\Grafix.h"\
	".\Src\Simerr.h"\
	".\Src\Vga_13.h"\
	
NODEP_CPP_FONT8=\
	".\Src\fontdev.h"\
	

"$(INTDIR)\Font8x8.obj" : $(SOURCE) $(DEP_CPP_FONT8) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Game.cpp
DEP_CPP_GAME_=\
	".\Src\Bits.h"\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Cockpit.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Earth.h"\
	".\Src\Flight.h"\
	".\Src\Flite.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltmngr.h"\
	".\Src\Fltobj.h"\
	".\Src\Fltzview.h"\
	".\Src\Font8x8.h"\
	".\Src\Game.h"\
	".\Src\Globals.h"\
	".\Src\Grafix.h"\
	".\Src\Grndunit.h"\
	".\Src\Group_3d.h"\
	".\Src\Hud.h"\
	".\Src\Input.h"\
	".\Src\Instrmnt.h"\
	".\Src\Kbdhit.h"\
	".\Src\Key_map.h"\
	".\Src\Moveable.h"\
	".\Src\Mytimer.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pc_keys.h"\
	".\Src\Pen.h"\
	".\Src\Pilot.h"\
	".\Src\Plltt.h"\
	".\Src\Port_3d.h"\
	".\Src\Portkey.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rndzpoly.h"\
	".\Src\Rotate.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Siminc.h"\
	".\Src\Siminput.h"\
	".\Src\Target.h"\
	".\Src\Terrain.h"\
	".\Src\Transblt.h"\
	".\Src\Traveler.h"\
	".\Src\Txtrmap.h"\
	".\Src\Unguided.h"\
	".\Src\Vga_13.h"\
	".\Src\Viewobj.h"\
	".\Src\Vmath.h"\
	".\Src\Waypoint.h"\
	".\Src\Weapons.h"\
	".\Src\Winjoy.h"\
	".\Src\Zview.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_GAME_=\
	".\Src\linux_joy.h"\
	

"$(INTDIR)\Game.obj" : $(SOURCE) $(DEP_CPP_GAME_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Globals.cpp
DEP_CPP_GLOBA=\
	".\Src\Clip.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Flite.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltobj.h"\
	".\Src\Font8x8.h"\
	".\Src\Globals.h"\
	".\Src\Grafix.h"\
	".\Src\Input.h"\
	".\Src\Pilot.h"\
	".\Src\Plltt.h"\
	".\Src\Port_3d.h"\
	".\Src\Rtkey.h"\
	".\Src\Transblt.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	".\Src\Waypoint.h"\
	".\Src\Weapons.h"\
	".\Src\Winjoy.h"\
	
NODEP_CPP_GLOBA=\
	".\Src\linux_joy.h"\
	

"$(INTDIR)\Globals.obj" : $(SOURCE) $(DEP_CPP_GLOBA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Grndunit.cpp
DEP_CPP_GRNDU=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Flite.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltobj.h"\
	".\Src\Fltzview.h"\
	".\Src\Grafix.h"\
	".\Src\Grndunit.h"\
	".\Src\Moveable.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pilot.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Simsnd.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Unguided.h"\
	".\Src\Vga_13.h"\
	".\Src\Viewobj.h"\
	".\Src\Vmath.h"\
	".\Src\Waypoint.h"\
	".\Src\Weapons.h"\
	".\Src\Zview.h"\
	

"$(INTDIR)\Grndunit.obj" : $(SOURCE) $(DEP_CPP_GRNDU) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Group_3d.cpp
DEP_CPP_GROUP=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Convpoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Group_3d.h"\
	".\Src\Obj_3d.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Simerr.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Group_3d.obj" : $(SOURCE) $(DEP_CPP_GROUP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Hud.cpp
DEP_CPP_HUD_C=\
	".\Src\Bits.h"\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Earth.h"\
	".\Src\Flight.h"\
	".\Src\Flite.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltmngr.h"\
	".\Src\Fltobj.h"\
	".\Src\Fltzview.h"\
	".\Src\Font8x8.h"\
	".\Src\Globals.h"\
	".\Src\Grafix.h"\
	".\Src\Hud.h"\
	".\Src\Input.h"\
	".\Src\Moveable.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pc_keys.h"\
	".\Src\Pen.h"\
	".\Src\Pilot.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Siminput.h"\
	".\Src\Target.h"\
	".\Src\Terrain.h"\
	".\Src\Traveler.h"\
	".\Src\Txtrmap.h"\
	".\Src\Unguided.h"\
	".\Src\Vga_13.h"\
	".\Src\Viewobj.h"\
	".\Src\Vmath.h"\
	".\Src\Waypoint.h"\
	".\Src\Weapons.h"\
	".\Src\Winjoy.h"\
	".\Src\Zview.h"\
	
NODEP_CPP_HUD_C=\
	".\Src\linux_joy.h"\
	

"$(INTDIR)\Hud.obj" : $(SOURCE) $(DEP_CPP_HUD_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Input.cpp
DEP_CPP_INPUT=\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Input.h"\
	".\Src\Vga_13.h"\
	".\Src\Winjoy.h"\
	
NODEP_CPP_INPUT=\
	".\Src\linux_joy.h"\
	".\Src\vga.h"\
	".\Src\vgagl.h"\
	".\Src\vgamouse.h"\
	

"$(INTDIR)\Input.obj" : $(SOURCE) $(DEP_CPP_INPUT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Instrmnt.cpp
DEP_CPP_INSTR=\
	".\Src\Bits.h"\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Fltobj.h"\
	".\Src\Font8x8.h"\
	".\Src\Grafix.h"\
	".\Src\Instrmnt.h"\
	".\Src\Pen.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rotate.h"\
	".\Src\Rtkey.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Target.h"\
	".\Src\Traveler.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	
NODEP_CPP_INSTR=\
	".\Src\fixpoint.h"\
	".\Src\trigtab.h"\
	

"$(INTDIR)\Instrmnt.obj" : $(SOURCE) $(DEP_CPP_INSTR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Key_map.cpp
DEP_CPP_KEY_M=\
	".\Src\Key_map.h"\
	".\Src\Pc_keys.h"\
	

"$(INTDIR)\Key_map.obj" : $(SOURCE) $(DEP_CPP_KEY_M) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Led2.cpp

"$(INTDIR)\Led2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Mission.cpp
DEP_CPP_MISSI=\
	".\Src\Mission.h"\
	

"$(INTDIR)\Mission.obj" : $(SOURCE) $(DEP_CPP_MISSI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Moveable.cpp
DEP_CPP_MOVEA=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltobj.h"\
	".\Src\Font8x8.h"\
	".\Src\Grafix.h"\
	".\Src\Moveable.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pc_keys.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Viewobj.h"\
	".\Src\Vmath.h"\
	".\Src\Weapons.h"\
	".\Src\Zview.h"\
	
NODEP_CPP_MOVEA=\
	".\Src\fixpoint.h"\
	".\Src\trigtab.h"\
	

"$(INTDIR)\Moveable.obj" : $(SOURCE) $(DEP_CPP_MOVEA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Mytimer.cpp
DEP_CPP_MYTIM=\
	".\Src\Mytimer.h"\
	

"$(INTDIR)\Mytimer.obj" : $(SOURCE) $(DEP_CPP_MYTIM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Obj_3d.cpp
DEP_CPP_OBJ_3=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Convpoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Earth.h"\
	".\Src\Grafix.h"\
	".\Src\Group_3d.h"\
	".\Src\Kbdhit.h"\
	".\Src\Obj_3d.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rndzpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Target.h"\
	".\Src\Terrain.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\Obj_3d.obj" : $(SOURCE) $(DEP_CPP_OBJ_3) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Pen.cpp
DEP_CPP_PEN_C=\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Pen.h"\
	".\Src\Traveler.h"\
	".\Src\Vga_13.h"\
	

"$(INTDIR)\Pen.obj" : $(SOURCE) $(DEP_CPP_PEN_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Pilot.cpp
DEP_CPP_PILOT=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Flite.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltobj.h"\
	".\Src\Grafix.h"\
	".\Src\Moveable.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pilot.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Unguided.h"\
	".\Src\Vga_13.h"\
	".\Src\Viewobj.h"\
	".\Src\Vmath.h"\
	".\Src\Waypoint.h"\
	".\Src\Weapons.h"\
	".\Src\Zview.h"\
	

"$(INTDIR)\Pilot.obj" : $(SOURCE) $(DEP_CPP_PILOT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Plltt.cpp
DEP_CPP_PLLTT=\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Plltt.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Vga_13.h"\
	

"$(INTDIR)\Plltt.obj" : $(SOURCE) $(DEP_CPP_PLLTT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Port_3d.cpp
DEP_CPP_PORT_=\
	".\Src\Clip.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Port_3d.h"\
	".\Src\Rtkey.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Port_3d.obj" : $(SOURCE) $(DEP_CPP_PORT_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Portkey.cpp
DEP_CPP_PORTK=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Earth.h"\
	".\Src\Font8x8.h"\
	".\Src\Grafix.h"\
	".\Src\Group_3d.h"\
	".\Src\Kbdhit.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pc_keys.h"\
	".\Src\Pen.h"\
	".\Src\Port_3d.h"\
	".\Src\Portkey.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Target.h"\
	".\Src\Terrain.h"\
	".\Src\Traveler.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\Portkey.obj" : $(SOURCE) $(DEP_CPP_PORTK) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Ppm.cpp
DEP_CPP_PPM_C=\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Vga_13.h"\
	

"$(INTDIR)\Ppm.obj" : $(SOURCE) $(DEP_CPP_PPM_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Rendpoly.cpp
DEP_CPP_RENDP=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Rendpoly.obj" : $(SOURCE) $(DEP_CPP_RENDP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Rndrpoly.cpp
DEP_CPP_RNDRP=\
	".\Src\Brsnline.h"\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Ddaline.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Pen.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Target.h"\
	".\Src\Traveler.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Rndrpoly.obj" : $(SOURCE) $(DEP_CPP_RNDRP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Rndzpoly.cpp
DEP_CPP_RNDZP=\
	".\Src\Brsnline.h"\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Ddaline.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Pen.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rndzpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Target.h"\
	".\Src\Traveler.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Rndzpoly.obj" : $(SOURCE) $(DEP_CPP_RNDZP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Rotate.cpp
DEP_CPP_ROTAT=\
	".\Src\Convpoly.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Pen.h"\
	".\Src\Rotate.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Rotate.obj" : $(SOURCE) $(DEP_CPP_ROTAT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Sabrewin.cpp
DEP_CPP_SABRE=\
	".\Src\About.h"\
	".\Src\Bits.h"\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Cockpit.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dibsect.h"\
	".\Src\Dvmath.h"\
	".\Src\Earth.h"\
	".\Src\Flight.h"\
	".\Src\Flite.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltmngr.h"\
	".\Src\Fltobj.h"\
	".\Src\Fltzview.h"\
	".\Src\Font8x8.h"\
	".\Src\Game.h"\
	".\Src\Globals.h"\
	".\Src\Grafix.h"\
	".\Src\Grndunit.h"\
	".\Src\Group_3d.h"\
	".\Src\Hud.h"\
	".\Src\Input.h"\
	".\Src\Instrmnt.h"\
	".\Src\Kbdhit.h"\
	".\Src\Key_map.h"\
	".\Src\Mission.h"\
	".\Src\Moveable.h"\
	".\Src\Mytimer.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pc_keys.h"\
	".\Src\Pen.h"\
	".\Src\Pilot.h"\
	".\Src\Plltt.h"\
	".\Src\Port_3d.h"\
	".\Src\Portkey.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rndzpoly.h"\
	".\Src\Rotate.h"\
	".\Src\Rtkey.h"\
	".\Src\sabrewin.rh"\
	".\Src\Scenario.h"\
	".\Src\Scnedit.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Siminc.h"\
	".\Src\Siminput.h"\
	".\Src\Simsnd.h"\
	".\Src\Target.h"\
	".\Src\Terrain.h"\
	".\Src\Transblt.h"\
	".\Src\Traveler.h"\
	".\Src\Txtrmap.h"\
	".\Src\Unguided.h"\
	".\Src\Vga_13.h"\
	".\Src\Viewobj.h"\
	".\Src\Vmath.h"\
	".\Src\Waypoint.h"\
	".\Src\Weapons.h"\
	".\Src\Winjoy.h"\
	".\Src\Wvga_13.h"\
	".\Src\Zview.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_SABRE=\
	".\Src\linux_joy.h"\
	

"$(INTDIR)\Sabrewin.obj" : $(SOURCE) $(DEP_CPP_SABRE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Sabrewin.rc
DEP_RSC_SABREW=\
	".\sabre.ico"\
	".\Sabre001.bmp"\
	".\Sbrico.bmp"\
	".\Sbrsplsh.bmp"\
	

"$(INTDIR)\Sabrewin.res" : $(SOURCE) $(DEP_RSC_SABREW) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Src\Scenario.cpp
DEP_CPP_SCENA=\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Scenario.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Scenario.obj" : $(SOURCE) $(DEP_CPP_SCENA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Scnedit.cpp
DEP_CPP_SCNED=\
	".\Src\Bits.h"\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Cockpit.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Earth.h"\
	".\Src\Flight.h"\
	".\Src\Flite.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltmngr.h"\
	".\Src\Fltobj.h"\
	".\Src\Fltzview.h"\
	".\Src\Font8x8.h"\
	".\Src\Globals.h"\
	".\Src\Grafix.h"\
	".\Src\Grndunit.h"\
	".\Src\Group_3d.h"\
	".\Src\Hud.h"\
	".\Src\Input.h"\
	".\Src\Instrmnt.h"\
	".\Src\Kbdhit.h"\
	".\Src\Key_map.h"\
	".\Src\Moveable.h"\
	".\Src\Mytimer.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pc_keys.h"\
	".\Src\Pen.h"\
	".\Src\Pilot.h"\
	".\Src\Plltt.h"\
	".\Src\Port_3d.h"\
	".\Src\Portkey.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rndzpoly.h"\
	".\Src\Rotate.h"\
	".\Src\Rtkey.h"\
	".\Src\Scnedit.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Siminc.h"\
	".\Src\Siminput.h"\
	".\Src\Target.h"\
	".\Src\Terrain.h"\
	".\Src\Transblt.h"\
	".\Src\Traveler.h"\
	".\Src\Txtrmap.h"\
	".\Src\Unguided.h"\
	".\Src\Vga_13.h"\
	".\Src\Viewobj.h"\
	".\Src\Vmath.h"\
	".\Src\Waypoint.h"\
	".\Src\Weapons.h"\
	".\Src\Winjoy.h"\
	".\Src\Zview.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_SCNED=\
	".\Src\linux_joy.h"\
	

"$(INTDIR)\Scnedit.obj" : $(SOURCE) $(DEP_CPP_SCNED) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Simfile.cpp
DEP_CPP_SIMFI=\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Defs.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Simfile.obj" : $(SOURCE) $(DEP_CPP_SIMFI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Simfilex.cpp
DEP_CPP_SIMFIL=\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Simfilex.h"\
	

"$(INTDIR)\Simfilex.obj" : $(SOURCE) $(DEP_CPP_SIMFIL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Siminput.cpp
DEP_CPP_SIMIN=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Flite.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltmngr.h"\
	".\Src\Fltobj.h"\
	".\Src\Fltzview.h"\
	".\Src\Globals.h"\
	".\Src\Grafix.h"\
	".\Src\Input.h"\
	".\Src\Kbdhit.h"\
	".\Src\Key_map.h"\
	".\Src\Moveable.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pc_keys.h"\
	".\Src\Pilot.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Siminput.h"\
	".\Src\Target.h"\
	".\Src\Terrain.h"\
	".\Src\Txtrmap.h"\
	".\Src\Unguided.h"\
	".\Src\Vga_13.h"\
	".\Src\Viewobj.h"\
	".\Src\Vmath.h"\
	".\Src\Waypoint.h"\
	".\Src\Weapons.h"\
	".\Src\Winjoy.h"\
	".\Src\Zview.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_SIMIN=\
	".\Src\linux_joy.h"\
	

"$(INTDIR)\Siminput.obj" : $(SOURCE) $(DEP_CPP_SIMIN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Simmath.cpp
DEP_CPP_SIMMA=\
	".\Src\Simmath.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Simmath.obj" : $(SOURCE) $(DEP_CPP_SIMMA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Target.cpp
DEP_CPP_TARGE=\
	".\Src\Clip.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Target.h"\
	".\Src\Values.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Target.obj" : $(SOURCE) $(DEP_CPP_TARGE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Terrain.cpp
DEP_CPP_TERRA=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Convpoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Grndlevl.h"\
	".\Src\Group_3d.h"\
	".\Src\Obj_3d.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rndzpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Target.h"\
	".\Src\Terrain.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

!IF  "$(CFG)" == "sabrefps - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "SABREWIN" /Fp"$(INTDIR)\sabrefps.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 

"$(INTDIR)\Terrain.obj" : $(SOURCE) $(DEP_CPP_TERRA) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "sabrefps - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "SABREWIN" /D VERION="0.2.3" /Fp"$(INTDIR)\sabrefps.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Terrain.obj" : $(SOURCE) $(DEP_CPP_TERRA) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Src\Transblt.cpp
DEP_CPP_TRANS=\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Transblt.h"\
	".\Src\Vga_13.h"\
	

"$(INTDIR)\Transblt.obj" : $(SOURCE) $(DEP_CPP_TRANS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Traveler.cpp
DEP_CPP_TRAVE=\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Traveler.h"\
	

"$(INTDIR)\Traveler.obj" : $(SOURCE) $(DEP_CPP_TRAVE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Txtrmap.cpp
DEP_CPP_TXTRM=\
	".\Src\Colormap.h"\
	".\Src\Comp.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Txtrmap.h"\
	".\Src\Unc.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Txtrmap.obj" : $(SOURCE) $(DEP_CPP_TXTRM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Unguided.cpp
DEP_CPP_UNGUI=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltobj.h"\
	".\Src\Fltzview.h"\
	".\Src\Grafix.h"\
	".\Src\Grndlevl.h"\
	".\Src\Moveable.h"\
	".\Src\Obj_3d.h"\
	".\Src\Pc_keys.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Simmath.h"\
	".\Src\Simsnd.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Unguided.h"\
	".\Src\Vga_13.h"\
	".\Src\Viewobj.h"\
	".\Src\Vmath.h"\
	".\Src\Weapons.h"\
	".\Src\Zview.h"\
	

"$(INTDIR)\Unguided.obj" : $(SOURCE) $(DEP_CPP_UNGUI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Viewobj.cpp
DEP_CPP_VIEWO=\
	".\Src\Bits.h"\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltobj.h"\
	".\Src\Grafix.h"\
	".\Src\Moveable.h"\
	".\Src\Obj_3d.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rndzpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Unguided.h"\
	".\Src\Vga_13.h"\
	".\Src\Viewobj.h"\
	".\Src\Vmath.h"\
	".\Src\Weapons.h"\
	".\Src\Zview.h"\
	

"$(INTDIR)\Viewobj.obj" : $(SOURCE) $(DEP_CPP_VIEWO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Vmath.cpp
DEP_CPP_VMATH=\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Vmath.h"\
	

"$(INTDIR)\Vmath.obj" : $(SOURCE) $(DEP_CPP_VMATH) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Vtable2.cpp

"$(INTDIR)\Vtable2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Waypoint.cpp
DEP_CPP_WAYPO=\
	".\Src\Defs.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Vmath.h"\
	".\Src\Waypoint.h"\
	

"$(INTDIR)\Waypoint.obj" : $(SOURCE) $(DEP_CPP_WAYPO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Weapons.cpp
DEP_CPP_WEAPO=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Dvmath.h"\
	".\Src\Flight.h"\
	".\Src\Fltlite.h"\
	".\Src\Fltobj.h"\
	".\Src\Grafix.h"\
	".\Src\Moveable.h"\
	".\Src\Obj_3d.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Sim.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Simfilex.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Unguided.h"\
	".\Src\Vga_13.h"\
	".\Src\Viewobj.h"\
	".\Src\Vmath.h"\
	".\Src\Weapons.h"\
	".\Src\Zview.h"\
	

"$(INTDIR)\Weapons.obj" : $(SOURCE) $(DEP_CPP_WEAPO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Wvga_13.cpp
DEP_CPP_WVGA_=\
	".\Src\Clip.h"\
	".\Src\Ddaline.h"\
	".\Src\Defs.h"\
	".\Src\Dibsect.h"\
	".\Src\Grafix.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	".\Src\Wvga_13.h"\
	

"$(INTDIR)\Wvga_13.obj" : $(SOURCE) $(DEP_CPP_WVGA_) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\Zview.cpp
DEP_CPP_ZVIEW=\
	".\Src\Clip.h"\
	".\Src\Clstypes.h"\
	".\Src\Colormap.h"\
	".\Src\Colorspc.h"\
	".\Src\Convpoly.h"\
	".\Src\Copoly.h"\
	".\Src\Cpoly.h"\
	".\Src\Defs.h"\
	".\Src\Grafix.h"\
	".\Src\Group_3d.h"\
	".\Src\Obj_3d.h"\
	".\Src\Port_3d.h"\
	".\Src\Rendpoly.h"\
	".\Src\Rndrpoly.h"\
	".\Src\Rtkey.h"\
	".\Src\Simerr.h"\
	".\Src\Simfile.h"\
	".\Src\Target.h"\
	".\Src\Txtrmap.h"\
	".\Src\Vga_13.h"\
	".\Src\Vmath.h"\
	".\Src\Zview.h"\
	

"$(INTDIR)\Zview.obj" : $(SOURCE) $(DEP_CPP_ZVIEW) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

