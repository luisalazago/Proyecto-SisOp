# An admittedly primitive Makefile
# To compile, type "make" or make "all"
# To remove files, type "make clean"

CC = gcc
CFLAGS = -Wall
OBJS = wserver.o wclient.o request.o io_helper.o 
SHARE = -lrt -lsqlite3 -pthread

.SUFFIXES: .c .o 

all: wserver wclient spin.cgi

wserver: wserver.o request.o io_helper.o
	$(CC) $(CFLAGS) -o wserver wserver.o request.o io_helper.o $(SHARE)

wclient: wclient.o io_helper.o
	$(CC) $(CFLAGS) -o wclient wclient.o io_helper.o $(SHARE)

spin.cgi: spin.c
	$(CC) $(CFLAGS) -o spin.cgi spin.c $(SHARE)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $< $(SHARE)

clean:
	-rm -f $(OBJS) wserver wclient spin.cgi
