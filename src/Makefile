# Makefile for Biclique

CC = gcc
MPICC = mpicc
CFLAGS = -O3 -g #-Wall
PTHREADS = -lpthread
LFSFLAG = -D_FILE_OFFSET_BITS=64
TESTFLAGS = #-DPERFORMANCE #-DDEBUG #-DVERBOSE

OBJECTS = bigraph.o utility.o sort.o biclique.o
HEADERS = bigraph.h utility.h sort.h biclique.h
EXECUTABLE = biclique

all: $(EXECUTABLE)

biclique: biclique-driver.o $(OBJECTS) $(HEADERS) Makefile
	$(CC) $(CFLAGS) $(LFSFLAG) $(TESTFLAGS) -o $@ biclique-driver.o $(OBJECTS) $(PTHREADS)

.c.o:
	$(CC) $(CFLAGS) $(LFSFLAG) $(TESTFLAGS) -c $*.c

clean:
	rm -f core* *.o $(EXECUTABLE)

