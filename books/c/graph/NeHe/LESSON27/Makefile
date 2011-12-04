LIBDIRS = -L/usr/X11R6/lib -L/usr/lib
LIBS =  -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm
INCDIRS = -I/usr/X11R6/include/
EXEC = Lesson27

all: Lesson27.o 
	g++ -o $(EXEC) Lesson27.o $(INCDIRS) $(LIBDIRS) $(LIBS)

Lesson27.o : Lesson27.cpp 3Dobject.h
	g++ -c Lesson27.cpp

run: all
	./$(EXEC)

clean:
	rm -f *.o $(EXEC)
