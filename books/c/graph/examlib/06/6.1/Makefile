OBJECTS= Nehe.o Texture.o Vector3D.o bmprw.o

CC= g++

all: light

%.o: %.cpp
	$(CC) -Os -Wall -ansi -pedantic -ffloat-store -c -g -o $@ $<

clean:
	rm -f *.o;rm -f light

light: $(OBJECTS)
	$(CC) -g -o light $(OBJECTS) -L/usr/X11R6/lib -lm -lGL -lglut -lGLU
