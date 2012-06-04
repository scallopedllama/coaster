# needed on the Suns
CC = gcc

# set to wherever you want to install the library files
CS418 = ./
INCDIR = -I$(CS418)/include
LIBDIR = -L$(CS418)/lib

CFLAGS = $(INCDIR)

OBJS = pic.o ppm.o jpeg.o 
#tiff.o

LIB = libpicio.a

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

install: libpicio.a
	 cp $(LIB) $(CS418)/lib
	 cp pic.h $(CS418)/include


clean:
	/bin/rm -f $(LIB) $(OBJS)
