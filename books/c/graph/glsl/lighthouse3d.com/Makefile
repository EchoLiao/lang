
TOP = .
CC := g++
CFLAGS := -g -Wall
CPPFLAGS := -g -Wall
INCLUDE := -I/usr/include -I$(TOP)



TARGETS = \
		  ogl \

GLUT = -lglut
#LLDLIBS = -lGLEW $(GLUT) -lGLU -lGL -lXmu -lXext -lX11 -lm
LLDLIBS = $(GLUT) -lGLU -lGL -lXmu -lXext -lX11 -lm -lGLEW


targets: textfile.o $(TARGETS)


def:
	find . -name '*.c' | xargs $(CC) -MM | \
	sed 's/\(^[^ :]*\).o:/\1:/' > Makefile.d;


# 使用"静态模式"书写规则
$(TARGETS): %: %.o textfile.o
	$(CC) $(INCLUDE) $< textfile.o $(LLDLIBS) -o $@

textfile.o: textfile.cpp textfile.h
	$(CC) $(CPPFLAGS) -c $(INCLUDE) $< -o $@


-include Makefile.d


clean:
	-rm -f *.o $(TARGETS)
