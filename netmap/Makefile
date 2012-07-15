CFILES   = misc.c hostclass.c netmap.c drawmap.c config.c args.c lookup.c
OFILES   = $(CFILES:.c=.o)

CFLAGS = -O -Wall -Wno-deprecated
CC = g++

CPPFLAGS = -I../belgolib

# On most systems this should work
LDFLAGS   = ../belgolib/belgolib.a
# On Solaris-like systems you need (Thanks to Kevin)
# LDFLAGS = ../belgolib/belgolib.a -L /usr/local/lib -lnsl 

# This only works with Gnu-make, if you use another one, comment out the ifeq, 
# else and endif commands 
ifeq ($(OSTYPE), solaris)
  LDFLAGS = ../belgolib/belgolib.a -L /usr/local/lib -lnsl 
else
  LDFLAGS   = ../belgolib/belgolib.a
endif


OEXE = netmap

CPP_HEADERS = -I/usr/include/g++-3/ $(CPPFLAGS)

default: test

test: $(OFILES)
	$(CC) -o $(OEXE) $(OFILES) $(LDFLAGS)
	strip $(OEXE)

clean:
	-/bin/rm -f $(OFILES) $(OEXE)

depend: 
	makedepend $(CPP_HEADERS)  -D__make_dep__ -- $(CFLAGS) -- $(CFILES)


# DO NOT DELETE
