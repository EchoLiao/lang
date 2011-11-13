
TOP = .

CXX := g++
CPPFLAGS := -g -Wall -I/usr/include -I$(TOP)

GLUT = -lglut
LDLIBS = $(GLUT) -lGLU -lGL -lXmu -lXext -lX11 -lm


TARGETS = \
		  prog6OGLViewingProg2D.P258 \


all: $(TARGETS)


# 使用"静态模式"书写规则
$(TARGETS): %: %.o
	$(CXX) $(CPPFLAGS) $< $(LDLIBS) -o $@


clean:
	-rm -f *.o $(TARGETS)
