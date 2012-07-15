CFILES   = makelist.c args.c config.c
OFILES   = $(CFILES:.c=.o)

CFLAGS = -O -Wall -Wno-deprecated
CC = g++

CPPFLAGS = -I../belgolib
LDFLAGS   = ../belgolib/belgolib.a

OEXE = makelist

CPP_HEADERS = -I/usr/include/g++-3/ $(CPPFLAGS)

default: test

test: $(OFILES)
	$(CC) -o $(OEXE) $(OFILES) $(LDFLAGS)
#	strip $(OEXE)

clean:
	-/bin/rm -f $(OFILES) $(OEXE)

depend: 
	makedepend $(CPP_HEADERS)  -D__make_dep__ -- $(CFLAGS) -- $(CFILES)

# DO NOT DELETE
