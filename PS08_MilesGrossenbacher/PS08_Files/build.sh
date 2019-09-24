CPP=c++
CFLAGS=" -Wall -I/usr/local/include"
LIBS="-L/usr/local/lib -lsndfile -lportaudio -lncurses"

EXE=audioEffects
SRC="audioEffects.cpp baseEffect.cpp passThrough.cpp sigFilter.cpp sigDRC.cpp"

$CPP $CFLAGS -o $EXE $SRC $LIBS
