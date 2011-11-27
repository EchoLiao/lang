# makefile for opengl (glut) applications
# author: milix (milix_gr@hotmail.com)

# add here the name of the project 
PROJ = lesson02

# aliases
CC = gcc
RC = windres
RM = -rm -f
CP = -copy

# include and library paths
INCPATH = -I/app/include 
LIBPATH = -L/app/lib

# compiler and linker switches
CFLAGS = -fomit-frame-pointer -Wall
LDFLAGS = -lGLU -lGL -lXmu -lXext -lX11 -lm -lglut -L/usr/lib64 -lstdc++

# add here additional object files
OBJS = $(PROJ).o  

all: $(PROJ).exe

$(PROJ).exe: $(OBJS)
	$(CC) $(LIBPATH) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(INCPATH) $(CFLAGS) -c $*.c

%.o: %.cpp
	$(CC) $(INCPATH) $(CFLAGS) -c $*.cpp

%.o: %.rc
	$(RC) -i $*.rc -o $*.o

clean:
	$(RM) *.o
	$(RM) $(PROJ).exe
