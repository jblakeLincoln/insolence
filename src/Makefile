CXX = clang++

INCLUDES = -I ../include -I /usr/include/freetype2

LINUX_LIBS = -L /usr/lib/x86_64 -lGLEW -lGL -lGLU
LINUX_LIBS+= -lSDL2 -lassimp -lopenal -lfreetype
LINUX_LIBS+= -lX11 -lXxf86vm -lXinerama -lXcursor -lXrandr -lpthread -lXi

DEFINES = -D GLM_FORCE_RADIANS

CPP_FLAGS = --std=c++11
CPP_FILES = insolence/stu.cpp

LINUX_FLAGS = -D INSOLENCE_OPENGL_DESKTOP -D LINUX -o ../bin/libinsolence.so -fPIC -shared
WEBGL_FLAGS = -D INSOLENCE_WEBGL -o ../bin/libinsolence.bc

.PHONY: all debug

COMMAND_DESKTOP = $(CXX) $(INCLUDES) $(CPP_FILES) $(CPP_FLAGS) $(LINUX_LIBS) $(LINUX_FLAGS) $(DEFINES)
COMMAND_WEBGL = emcc $(INCLUDES) $(CPP_FILES) $(CPP_FLAGS) $(DEFINES) $(WEBGL_FLAGS)

debug:
	$(COMMAND_DESKTOP) -O0

release:
	$(COMMAND_DESKTOP) -O3

webgl-debug:
	$(COMMAND_WEBGL) -O0

webgl-release:
	$(COMMAND_WEBGL) -O3
