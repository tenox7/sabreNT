mv sslewer.h aislewer.h
mv sslewer.cpp aislewer.cpp
mv sairfld.h aifield.h
mv sairfld.cpp aifield.cpp
mv sfltmdl.h aifltmdl.h
mv sfltmdl.cpp aifltmdl.cpp
mv sattkr.h aiattkr.h
mv sattkr.cpp aiattkr.cpp
mv sfrmtn.h aifrmtn.h
mv sfrmtn.cpp aifrmtn.cpp
mv smnvrst.h aimnvrst.h
mv smnvrst.cpp aimnvrst.cpp
mv spid.h aipid.h
mv spid.c aipid.c
mv spilcaps.h aipilcaps.h
mv spilcaps.cpp aipilcaps.cpp
mv splncaps.h aiplncaps.h
mv splncaps.cpp aiplncaps.cpp
mv srunway.h airunway.h
mv srunway.cpp airunway.cpp
mv stact.h aitact.h
mv stact.cpp aitact.cpp
mv starget.h aitarget.h
mv starget.cpp aitarget.cpp
mv stime.h aitime.h
mv stime.c aitime.c
mv swaypnt.h aiwaypnt.h
mv sweapon.h aiweapon.h
mv sweapon.cpp aiweapon.cpp

for i in `ls *.cpp`
do
    echo $i
    sed -f sed2.scr $i >tmp
    mv tmp $i
done
for i in `ls *.h`
do
    echo $i
    sed -f sed2.scr $i >tmp
    mv tmp $i
done
for i in `ls *.c`
do
    echo $i
    sed -f sed2.scr $i >tmp
    mv tmp $i
done

