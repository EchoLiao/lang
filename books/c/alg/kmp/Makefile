
CFLAGS = -g -Wall

all: kmp_test


kmp.o: kmp.c kmp.h

kmp_test.o: kmp_test.c kmp_test.h

kmp_test: kmp_test.o kmp.o
	gcc $^ -o $@



clean:
	-rm -f *.o kmp_test
