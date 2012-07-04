CC = gcc
CFLAGS = -g -Wall
LLDLIBS = -lglut -lGLU -lGL -lXmu -lXext -lX11 -lm

OBJ_SODUKU = soduku.o
TARGETS = soduku_test soduku_glut



all: $(TARGETS)


$(TARGETS): %: %.o $(OBJ_SODUKU)
	$(CC) $(CFLAGS) $< $(OBJ_SODUKU) $(LLDLIBS) -o $@


-include Makefile.d



clean:
	-rm -f *.o $(TARGETS) $(OBJ_SODUKU)
