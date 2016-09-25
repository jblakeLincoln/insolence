#ifndef GRAPHICS_H
#define GRAPHICS_H

#if INSOLENCE_OPENGL_DESKTOP || INSOLENCE_WEBGL
	#include <GL/glew.h>
	#include <GL/gl.h>
#elif INSOLENCE_ANDROID
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
#endif

#endif
