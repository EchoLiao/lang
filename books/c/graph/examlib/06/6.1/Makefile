OBJECTS= Nehe.o Texture.o Vector3D.o bmprw.o

CC= g++

all: Nehe

%.o: %.cpp
	$(CC) -Os -Wall -ansi -pedantic -ffloat-store -c -g -o $@ $<

clean:
	rm -f *.o;rm -f Nehe

Nehe: $(OBJECTS)
	$(CC) -g -o Nehe $(OBJECTS) -L/usr/X11R6/lib -lm -lGL -lglut -lGLU
