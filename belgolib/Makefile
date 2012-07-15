CFILES   = files.c dirs.c dirs_core.c search.c
OFILES   = $(CFILES:.c=.o)

CFLAGS = -O -Wall
CC = g++

CPP_HEADERS = /usr/include/g++-3/

default: makelib

makelib : $(OFILES)
	-/bin/rm  belgolib.a
	ar qc belgolib.a $(OFILES)
	ranlib belgolib.a

clean:
	-/bin/rm -f $(OFILES)

depend: 
	makedepend -I$(CPP_HEADERS)  -D__make_dep__ -- $(CFLAGS) -- $(CFILES)



# DO NOT DELETE

