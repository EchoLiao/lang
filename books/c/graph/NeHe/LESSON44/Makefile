# Project: Lesson44
# Makefile created by Dev-C++ 4.9.8.0

CPP  = g++
CC   = gcc
WINDRES = windres
RES  = 
OBJ  = glCamera.o glFont.o glPoint.o glVector.o Lesson44.o $(RES)
LINKOBJ  = glCamera.o glFont.o glPoint.o glVector.o Lesson44.o $(RES)
#LIBS =  -L"C:/Dev-Cpp/lib" -mwindows -lopengl32 -lglu32 -lwinmm -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
LIBS =  -lGL -lGLU
#INCS =  -I"C:/Dev-Cpp/include" 
#CXXINCS =  -I"C:/Dev-Cpp/include/c++"  -I"C:/Dev-Cpp/include/c++/mingw32"  -I"C:/Dev-Cpp/include/c++/backward"  -I"C:/Dev-Cpp/include" 
BIN  = Lesson44
CXXFLAGS = $(CXXINCS)
CFLAGS = $(INCS)

.PHONY: all all-before all-after clean clean-custom

all: all-before Lesson44 all-after


clean: clean-custom
	rm -f $(OBJ) $(BIN)

$(BIN): $(LINKOBJ)
	$(CPP) $(LINKOBJ) -o "Lesson44" $(LIBS)

glCamera.o: glCamera.cpp
	$(CPP) -c glCamera.cpp -o glCamera.o $(CXXFLAGS)

glFont.o: glFont.cpp
	$(CPP) -c glFont.cpp -o glFont.o $(CXXFLAGS)

glPoint.o: glPoint.cpp
	$(CPP) -c glPoint.cpp -o glPoint.o $(CXXFLAGS)

glVector.o: glVector.cpp
	$(CPP) -c glVector.cpp -o glVector.o $(CXXFLAGS)

Lesson44.o: Lesson44.cpp
	$(CPP) -c Lesson44.cpp -o Lesson44.o $(CXXFLAGS)
