g++ -o room_acousticsB \
	-Wno-deprecated-declarations \
	-D USE_OPENGL=1 \
	-framework GLUT -framework OpenGL \
	room_acoustics.cpp calc_images.cpp parametric_reverb.cpp para_reverb.cpp \
	display_opengl.cpp \
	-I/usr/local/include -L/usr/local/lib \
	-lsndfile -lncurses -lportaudio
