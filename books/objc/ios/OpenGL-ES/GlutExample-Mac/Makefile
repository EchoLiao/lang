
CC = g++
RM = -rm -f

MACFRAMEWORK = -framework GLUT -framework OpenGL

#COMPILERFLAGS = 
#LIBRARIES = 
CFLAGS = -Wall -g
LDFLAGS = $(MACLIBPATH) $(MACFRAMEWORK)


OBJECTS = GlutExample.o
TARGETS = GlutExample


All: $(TARGETS)

#$(TARGETS): $(OBJECTS)
#    $(CC) $(FRAMEWORK) $(CFLAGS) -o $@ $(LIBPATH) $(OBJECTS)

    #$(LIBRARIES)
    #    ./GlutExample


%.o: %.c
	$(CC) $(INCPATH) $(CFLAGS) -c $*.c

$(TARGETS): $(OBJECTS)
	$(CC) $< $(LDFLAGS) -o $@


clean:
	$(RM) $(TARGETS) $(OBJECTS)

