CC = gcc
RM = -rm -f

CFLAGS = -g -fomit-frame-pointer -Wall
LDFLAGS = -lGLU -lGL -lm -lglut -L/usr/lib64



TARGETS = 3drace

OBJECTS = $(TARGETS).o


all: $(TARGETS)



%.o: %.c
	$(CC) $(INCPATH) $(CFLAGS) -c $*.c


$(TARGETS): $(OBJECTS)
	$(CC) $< $(LDFLAGS) -o $@


clean:
	$(RM) $(TARGETS) $(OBJECTS)

