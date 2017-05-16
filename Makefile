CC=gcc
CFLAGS= -Wall -pedantic -std=c99
SOURCES=./imgwrite.h ./imgwrite.c ./main.c
SOURCES+= ./font_types.h ./font_prop14x16.c ./font_rom8x16.c
SOURCES+= ./netcom.h ./netcom.c ./messages.h ./messages.c
CFILES=./imgwrite.c ./main.c ./font_prop14x16.c ./font_rom8x16.c ./netcom.c ./messages.c


main:$(SOURCES) 
		$(CC) $(CFLAGS) -o $@ $(CFILES)

