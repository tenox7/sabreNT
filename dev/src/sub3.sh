sed -e s/aimath.h/ai2sbr.h/g sbfltmdl.cpp >tmp
mv tmp sbfltmdl.cpp
sed -e s/aimath.h/ai2sbr.h/g pilot.cpp >tmp
mv tmp pilot.cpp
sed -e s/aimath.h/ai2sbr.h/g hud.cpp >tmp
mv tmp hud.cpp
sed -e s/aimath.h/ai2sbr.h/g fltmngr.cpp >tmp
mv tmp fltmngr.cpp

