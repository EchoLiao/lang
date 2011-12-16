LIBDIRS = -L/usr/X11R6/lib -L/usr/lib
LIBS =  -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm
INCDIRS = -I/usr/X11R6/include/
EXEC = Lesson27

all: Lesson27.o M3D_math3d.o
	g++ -o $(EXEC) Lesson27.o M3D_math3d.o $(INCDIRS) $(LIBDIRS) $(LIBS)

Lesson27.o : Lesson27.cpp 3Dobject.h
	g++ -Wall -g -c Lesson27.cpp


M3D_math3d.o : ../LESSON11/M3D_math3d.h
	g++ -Wall -g -c ../LESSON11/M3D_math3d.c

run: all
	./$(EXEC)

clean:
	rm -f *.o $(EXEC)
