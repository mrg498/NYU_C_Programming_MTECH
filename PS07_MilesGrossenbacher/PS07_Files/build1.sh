# Compile and link program to play a tone

gcc -Wall tone_generator.c -o tone_generator \
	-I/usr/local/include -L/usr/local/lib \
	-lsndfile -lncurses -lportaudio

