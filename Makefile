CC=gcc
CFLAGS= -Wall -pedantic -std=c99
SOURCES= ./imgwrite.h ./imgwrite.c ./main.c
SOURCES+= ./font_types.h ./font_prop14x16.c ./font_rom8x16.c
SOURCES+= ./netcom.h ./netcom.c ./messages.h ./messages.c
SOURCES+= ./screen.h ./screen.c ./control.h ./control.c
SOURCES+= ./mzapo_regs.h ./mzapo_parlcd.h ./mzapo_parlcd.c ./mzapo_phys.c ./mzapo_phys.h
CFILES= ./imgwrite.c ./main.c ./font_prop14x16.c ./font_rom8x16.c ./netcom.c ./messages.c
CFILES+= ./screen.c ./control.c ./mzapo_parlcd.c ./mzapo_phys.c


main:$(SOURCES) 
		$(CC) $(CFLAGS) -o $@ $(CFILES)

