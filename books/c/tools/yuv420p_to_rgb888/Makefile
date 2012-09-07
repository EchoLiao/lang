
PROJ = yuv420p_to_rgb888_test

CC = gcc
RM = -rm -f
CP = -cp

# include and library paths
INCPATH = -I.
LIBPATH = -L.

# compiler and linker switches
CFLAGS = -Wall -g
LDFLAGS =

# add here additional object files
OBJS = \
	   bmprw.o \
	   yuv420p_to_rgb888.o \
	   yuv420p_to_rgb888_test.o \


all: $(PROJ)
	./yuv420p_to_rgb888_test nal.yuv420p

$(PROJ): $(OBJS)
	$(CC) $(LIBPATH) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(INCPATH) $(CFLAGS) -c $*.c


clean:
	$(RM) *.o
	$(RM) $(PROJ)
	$(RM) out.bmp

