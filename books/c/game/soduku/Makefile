
CFLAGS = -g -Wall

all: soduku_test


soduku.o: soduku.c soduku.h

soduku_test.o: soduku_test.c soduku_test.h

soduku_test: soduku_test.o soduku.o
	gcc $^ -o $@



clean:
	-rm -f *.o soduku_test
