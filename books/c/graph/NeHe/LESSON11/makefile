# makefile for opengl (glut) applications
# author: milix (milix_gr@hotmail.com)

# add here the name of the project 
PROJ = lesson11
PROJ2 = lesson11-2

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
SHARED = naldebug.o

all: $(PROJ).exe $(PROJ2).exe

$(PROJ).exe: $(OBJS) $(SHARED)
	$(CC) $(LIBPATH) -o $@ $(SHARED) $(OBJS) $(LDFLAGS)

$(PROJ2).exe: $(OBJS2) $(SHARED)
	$(CC) $(LIBPATH) -o $@ $(SHARED) $(OBJS2) $(LDFLAGS)

%.o: %.c
	$(CC) $(INCPATH) $(CFLAGS) -c $*.c

%.o: %.cpp
	$(CC) $(INCPATH) $(CFLAGS) -c $*.cpp

%.o: %.rc
	$(RC) -i $*.rc -o $*.o


naldebug.o: naldebug.cpp naldebug.h


clean:
	$(RM) *.o
	$(RM) *.exe
	$(RM) $(SHARED)
