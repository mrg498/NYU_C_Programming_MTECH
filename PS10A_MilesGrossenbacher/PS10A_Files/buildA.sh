g++ -o room_acousticsA \
	-D USE_NCURSES=1 \
	room_acoustics.cpp calc_images.cpp display_ncurses.cpp \
	-I/usr/local/include -L/usr/local/lib \
	-lsndfile -lncurses -lportaudio
