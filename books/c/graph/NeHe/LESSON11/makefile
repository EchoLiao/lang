# makefile for opengl (glut) applications
# author: milix (milix_gr@hotmail.com)

# add here the name of the project 
PROJ = lesson11
PROJ2 = lesson11-2
ENDIAN = endian_test
BMPRW = bmprw_test

# aliases
CC = gcc
RC = windres
RM = -rm -f
CP = -copy

# include and library paths
INCPATH = -I/app/include 
LIBPATH = -L/app/lib

# compiler and linker switches
CFLAGS = -g -fomit-frame-pointer -Wall
LDFLAGS = -lGLU -lGL -lXmu -lXext -lX11 -lm -lglut -L/usr/lib64 -lstdc++

# add here additional object files
OBJS = $(PROJ).o  
OBJS2 = $(PROJ2).o  
ENDIAN_OBJ = $(ENDIAN).o  
BMPRW_OBJ = $(BMPRW).o  
SHARED = bmprw.o

all: $(PROJ).exe $(PROJ2).exe $(ENDIAN).exe $(BMPRW).exe

$(PROJ).exe: $(OBJS) $(SHARED)
	$(CC) $(LIBPATH) -o $@ $(SHARED) $(OBJS) $(LDFLAGS)

$(PROJ2).exe: $(OBJS2) $(SHARED)
	$(CC) $(LIBPATH) -o $@ $(SHARED) $(OBJS2) $(LDFLAGS)

$(ENDIAN).exe: $(ENDIAN_OBJ)
	$(CC) $(LIBPATH) -o $@ $^ $(LDFLAGS)

$(BMPRW).exe: $(BMPRW_OBJ) $(SHARED)
	$(CC) $(LIBPATH) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(INCPATH) $(CFLAGS) -c $*.c

%.o: %.cpp
	$(CC) $(INCPATH) $(CFLAGS) -c $*.cpp

%.o: %.rc
	$(RC) -i $*.rc -o $*.o


bmprw.o: bmprw.cpp bmprw.h


clean:
	$(RM) *.o
	$(RM) *.exe
	$(RM) $(SHARED)
