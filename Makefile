CC=gcc
CFLAGS= -Wall -pedantic -std=c99
SOURCES= ./imgwrite.h ./imgwrite.c ./graphics.c ./graphics.h
SOURCES+= ./font_types.h ./font_rom8x16.c
SOURCES+= ./netcom.h ./netcom.c ./messages.h ./messages.c ./intercom.c ./intercom.h
SOURCES+= ./screen.h ./screen.c ./control.h ./control.c
SOURCES+= ./mzapo_regs.h ./mzapo_parlcd.h ./mzapo_parlcd.c ./mzapo_phys.c ./mzapo_phys.h ./utils.c ./utils.h ./main.c
CFILES= ./imgwrite.c ./font_rom8x16.c ./netcom.c ./messages.c ./utils.c
CFILES+= ./screen.c ./control.c ./mzapo_parlcd.c ./mzapo_phys.c ./graphics.c ./intercom.c ./main.c


main:$(SOURCES) 
		$(CC) $(CFLAGS) -o $@ $(CFILES)

