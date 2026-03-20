mv aimath.h ai2sbr.h
mv smath.h aimath.h
mv smath.cpp aimath.cpp
cp sobject.h aiobject.h
rm sobject.h
cp sarray.h aiarray.h
rm sarray.h

for i in `ls *.cpp`
do
    echo $i
    sed -f sed.scr $i >tmp
    mv tmp $i
done
for i in `ls *.h`
do
    echo $i
    sed -f sed.scr $i >tmp
    mv tmp $i
done
for i in `ls *.c`
do
    echo $i
    sed -f sed.scr $i >tmp
    mv tmp $i
done

