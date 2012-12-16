CC = gcc
RM = -rm -f

LIBPATH =
LIBS = -lGLU -lGL -lm -lglut -L/usr/lib64

DEF =
INCLUDE =
CFLAGS = $(INCLUDE) $(DEF) -g -fomit-frame-pointer -Wall
LDFLAGS = $(LIBPATH) $(LIBS)

TARGETS = 3drace
OBJECTS = $(TARGETS).o

all: $(TARGETS)


%.o: %.c
	$(CC) $(CFLAGS) -c $*.c

$(TARGETS): $(OBJECTS)
	$(CC) $< $(LDFLAGS) -o $@


clean:
	$(RM) $(TARGETS) $(OBJECTS)

