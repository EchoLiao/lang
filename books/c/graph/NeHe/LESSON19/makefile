# makefile for opengl (glut) applications
# author: milix (milix_gr@hotmail.com)

# add here the name of the project 
PROJ = lesson19

# aliases
CC = gcc
RC = windres
RM = -rm -f
CP = -cp

# include and library paths
INCPATH = -I/app/include 
LIBPATH = -L/app/lib

# compiler and linker switches
CFLAGS = -O2 -fomit-frame-pointer -Wall
#LDFLAGS = -mwindows -lwinmm -lglut32 -lglu32 -lopengl32 
LDFLAGS = -lglut -lGLU -lGL -lstdc++ -lrt -lm

# add here additional object files
OBJS = bmprw.o nal_fps.o

all: $(PROJ).exe burst.exe texEnvf.exe


texEnvf.exe: texEnvf.o $(OBJS)
	$(CC) $(LIBPATH) -o $@ $^ $(LDFLAGS)

burst.exe: burst.o $(OBJS)
	$(CC) $(LIBPATH) -o $@ $^ $(LDFLAGS)

$(PROJ).exe: $(PROJ).o $(OBJS)
	$(CC) $(LIBPATH) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(INCPATH) $(CFLAGS) -c $*.c

%.o: %.cpp
	$(CC) $(INCPATH) $(CFLAGS) -c $*.cpp

%.o: %.rc
	$(RC) -i $*.rc -o $*.o

clean:
	$(RM) *.o
	$(RM) $(PROJ).exe *.exe
