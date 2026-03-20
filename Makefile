all: sabre.exe

dev\libzip\libzip.lib: dev\lib\BITS.C dev\lib\CRC.C dev\lib\DEFLATE.C dev\lib\INFLATE.C dev\lib\TREES.C dev\lib\UNC.C
	cl /nologo /O2 /Idev\lib /DWIN32 /c /Fodev\libzip\bits.obj dev\lib\BITS.C
	cl /nologo /O2 /Idev\lib /DWIN32 /c /Fodev\libzip\crc.obj dev\lib\CRC.C
	cl /nologo /O2 /Idev\lib /DWIN32 /c /Fodev\libzip\deflate.obj dev\lib\DEFLATE.C
	cl /nologo /O2 /Idev\lib /DWIN32 /c /Fodev\libzip\inflate.obj dev\lib\INFLATE.C
	cl /nologo /O2 /Idev\lib /DWIN32 /c /Fodev\libzip\trees.obj dev\lib\TREES.C
	cl /nologo /O2 /Idev\lib /DWIN32 /c /Fodev\libzip\unc.obj dev\lib\UNC.C
	lib /nologo /out:dev\libzip\libzip.lib dev\libzip\bits.obj dev\libzip\crc.obj dev\libzip\deflate.obj dev\libzip\inflate.obj dev\libzip\trees.obj dev\libzip\unc.obj

sabre.exe: \
	dev\src\aaaunit.obj \
	dev\src\about.obj \
	dev\src\aiarray.obj \
	dev\src\aiattkr.obj \
	dev\src\aibase.obj \
	dev\src\aifield.obj \
	dev\src\aiflite.obj \
	dev\src\aifltmdl.obj \
	dev\src\aifrmtn.obj \
	dev\src\aigunner.obj \
	dev\src\aimath.obj \
	dev\src\aimnvrst.obj \
	dev\src\aipid.obj \
	dev\src\aipilcaps.obj \
	dev\src\aipilot.obj \
	dev\src\aipilot2.obj \
	dev\src\aipilot3.obj \
	dev\src\aipilot4.obj \
	dev\src\aipilot5.obj \
	dev\src\aipilot6.obj \
	dev\src\aiplncaps.obj \
	dev\src\airunway.obj \
	dev\src\aislewer.obj \
	dev\src\aitact.obj \
	dev\src\aitarget.obj \
	dev\src\aitime.obj \
	dev\src\aiweapon.obj \
	dev\src\bits.obj \
	dev\src\clip.obj \
	dev\src\cockpit.obj \
	dev\src\colormap.obj \
	dev\src\colorspc.obj \
	dev\src\convpoly.obj \
	dev\src\copoly.obj \
	dev\src\cpoly.obj \
	dev\src\dhlist.obj \
	dev\src\dibsect.obj \
	dev\src\earth.obj \
	dev\src\flight.obj \
	dev\src\fltlite.obj \
	dev\src\fltmngr.obj \
	dev\src\fltobj.obj \
	dev\src\fltzview.obj \
	dev\src\font8x8.obj \
	dev\src\game.obj \
	dev\src\globals.obj \
	dev\src\grndunit.obj \
	dev\src\group_3d.obj \
	dev\src\hud.obj \
	dev\src\input.obj \
	dev\src\instrmnt.obj \
	dev\src\kbdhit.obj \
	dev\src\led2.obj \
	dev\src\mission.obj \
	dev\src\moveable.obj \
	dev\src\mytimer.obj \
	dev\src\obj_3d.obj \
	dev\src\pen.obj \
	dev\src\pilobj.obj \
	dev\src\pilot.obj \
	dev\src\plltt.obj \
	dev\src\port_3d.obj \
	dev\src\portkey.obj \
	dev\src\ppm.obj \
	dev\src\rendpoly.obj \
	dev\src\rndrpoly.obj \
	dev\src\rndrplyx.obj \
	dev\src\rndzpoly.obj \
	dev\src\rotate.obj \
	dev\src\sabrewin.obj \
	dev\src\sbfltmdl.obj \
	dev\src\scenario.obj \
	dev\src\scnedit.obj \
	dev\src\simfile.obj \
	dev\src\simfilex.obj \
	dev\src\siminput.obj \
	dev\src\simmath.obj \
	dev\src\simsnd.obj \
	dev\src\target.obj \
	dev\src\terrain.obj \
	dev\src\textures.obj \
	dev\src\transblt.obj \
	dev\src\traveler.obj \
	dev\src\txtrmap.obj \
	dev\src\unguided.obj \
	dev\src\viewobj.obj \
	dev\src\vmath.obj \
	dev\src\vtable2.obj \
	dev\src\weapons.obj \
	dev\src\wincntrl.obj \
	dev\src\winjoy.obj \
	dev\src\winkeys.obj \
	dev\src\wvga_13.obj \
	dev\src\zview.obj \
	dev\Sabrewin.res \
	dev\libzip\libzip.lib
	link /nologo /subsystem:windows /out:sabre.exe @<<
$**
kernel32.lib
user32.lib
gdi32.lib
comdlg32.lib
advapi32.lib
shell32.lib
winmm.lib
<<

{dev\src\}.cpp{dev\src\}.obj:
	cl /nologo /O2 /Idev\src /Idev /DWIN32 /D_WINDOWS /DSABREWIN /c /Fo$@ $<

dev\src\aipid.obj: dev\src\aipid.c
	cl /nologo /O2 /Idev\src /Idev /DWIN32 /D_WINDOWS /DSABREWIN /c /Fodev\src\aipid.obj dev\src\aipid.c

dev\src\aitime.obj: dev\src\aitime.c
	cl /nologo /O2 /Idev\src /Idev /DWIN32 /D_WINDOWS /DSABREWIN /c /Fodev\src\aitime.obj dev\src\aitime.c

dev\src\dhlist.obj: dev\src\dhlist.c
	cl /nologo /O2 /Idev\src /Idev /DWIN32 /D_WINDOWS /DSABREWIN /c /Fodev\src\dhlist.obj dev\src\dhlist.c

dev\Sabrewin.res: dev\Sabrewin.rc dev\resource.h
	rc /r /Idev /fodev\Sabrewin.res dev\Sabrewin.rc

clean:
	-del dev\src\*.obj
	-del dev\libzip\*.obj
	-del dev\libzip\libzip.lib
	-del dev\Sabrewin.res
	-del sabre.exe
