# makefile for opengl (glut) applications
# author: milix (milix_gr@hotmail.com)

# add here the name of the project 
PROJ = lesson14

# aliases
CC = gcc
RM = -rm -f
CP = -cp -f

# include and library paths
INCPATH = -I/usr/X11R6/include 
LIBPATH = -L/usr/X11R6/lib -L/usr/lib

# compiler and linker switches
CFLAGS = -Wall -g
LDFLAGS = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm


# add here additional object files
OBJS = $(PROJ).o  

all: $(PROJ).exe

$(PROJ).exe: $(OBJS)
	$(CC) $(LIBPATH) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(INCPATH) $(CFLAGS) -c $*.c

%.o: %.cpp
	$(CC) $(INCPATH) $(CFLAGS) -c $*.cpp


clean:
	$(RM) *.o
	$(RM) $(PROJ).exe
