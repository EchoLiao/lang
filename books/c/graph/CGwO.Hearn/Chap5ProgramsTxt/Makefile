
TOP = .

CXX := g++
CPPFLAGS := -g -Wall -I/usr/include -I$(TOP)

GLUT = -lglut
LDLIBS = $(GLUT) -lGLU -lGL -lXmu -lXext -lX11 -lm


TARGETS = \
		  prog5CompositeTransformation2D.P203 \
		  prog5CompositeTransformation2D.P203-2 \
		  prog5RotateRoutines3D.P227 \


all: $(TARGETS)


# 使用"静态模式"书写规则
$(TARGETS): %: %.o
	$(CXX) $(CPPFLAGS) $< $(LDLIBS) -o $@


clean:
	-rm -f *.o $(TARGETS)
