CC = gcc -Wall -ansi -g
SOURCE = lesson43.cpp FreeType.cpp
FREETYPE_INCLUDE = /usr/include/freetype2
FLAGS = -lGL -lGLU -lfreetype `sdl-config --cflags --libs` -L/usr/lib64 -lstdc++ -lm
BINARY = freetype

all:
	$(CC) $(SOURCE) -o $(BINARY) $(FLAGS) -I$(FREETYPE_INCLUDE)

clean:
	@echo Cleaning up...
	@rm -f $(BINARY)
	@echo Done.
