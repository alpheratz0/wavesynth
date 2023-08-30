.POSIX:
.PHONY: clean all install uninstall

include config.mk

all: wavesynth

wav.o: wav.c wav.h
note.o: note.c note.h
utils.o: utils.c utils.h
wavesynth.o: wavesynth.c
wavesynth: wav.o note.o utils.o wavesynth.o

clean:
	rm -f wavesynth *.o

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f wavesynth $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/wavesynth

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/wavesynth
