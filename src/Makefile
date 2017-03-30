CXX = g++

INCLUDES = -I ../include -I /usr/include/freetype2

LIBS = -L /usr/lib/x86_64 -lGLEW -lGL -lGLU
LIBS+= -lSDL2 -lassimp -lopenal -lfreetype
LIBS+= -lX11 -lXxf86vm -lXinerama -lXcursor -lXrandr -lpthread -lXi

DEFINES = -D GLM_FORCE_RADIANS
DEFINES+= -D INSOLENCE_OPENGL_DESKTOP -D LINUX

CPP_FLAGS = --std=c++11 -fPIC -shared
CPP_FILES = insolence/stu.cpp

.PHONY: all

all:
	$(CXX) $(INCLUDES) $(CPP_FILES) $(CPP_FLAGS) $(LIBS) $(DEFINES) -o ../bin/libinsolence.so
