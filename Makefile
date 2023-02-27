.POSIX:
.PHONY: clean all

CC=cc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -Os
LDLIBS=-lm
LDFLAGS=-s

all: wavesynth

wav.o: wav.c wav.h
utils.o: utils.c utils.h
wavesynth.o: wavesynth.c
wavesynth: wav.o utils.o wavesynth.o

clean:
	rm -f wavesynth *.o