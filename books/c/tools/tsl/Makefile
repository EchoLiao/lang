CC=gcc
LD=gcc
AS=as
AR=ar ruv
RANLIB=ranlib
DIFF=diff
CD=cd
RM=rm -f
CP=cp -rf

A=.a
O=.o
E=

INCLUDE=-I./ -I./src
LIBDIRS=-L./lib
LIBS=

CFLAGS=-c -g -Wall $(INCLUDE)
LDFLAGS=-g $(LIBDIRS) $(LIBS)


DST_OBJECTS= \
			 ./src/tsl.o \

OBJECTS= \
		 ./test/tsl_test.o \

TARGETS= \
		 ./test/tsl_test \


all: $(TARGETS)



$(DST_OBJECTS): %.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

$(OBJECTS): %.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

$(TARGETS): %: %.o $(DST_OBJECTS)
	$(LD) -o $@ $< $(DST_OBJECTS) $(LDFLAGS)



clean:
	$(RM) $(DST_OBJECTS) $(OBJECTS) $(TARGETS)


