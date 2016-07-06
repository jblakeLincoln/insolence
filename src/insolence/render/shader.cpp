#include "shader.h"

#ifdef INSOLENCE_OPENGL_DESKTOP
char Shader::glsl_version[15] = "#version 150\n\0";
#elif INSOLENCE_OPENGL_ES
char Shader::glsl_version[15] = "#version 100\n\0";
#endif

const char* Shader::vert_header =
	/* Version replaced */
	"#if __VERSION__ < 120 || __VERSION__ == 300\n" /* OpenGL ES */
	"	#define INSOLENCE_GLSL_ES\n"
	"	#define IN attribute\n"
	"	#define OUT varying\n"
	"#else\n" /* OpenGL Desktop */
	"	#define INSOLENCE_GLSL_DESKTOP\n"
	"	#define IN in\n"
	"	#define OUT out\n"
	"#endif\n";

const char *Shader::frag_header =
	"#if __VERSION__ < 120 || __VERSION__ == 300\n" /* OpenGL ES */
	"	#define INSOLENCE_GLSL_ES\n"
	"	precision highp float;\n"
	"	#define texture texture2D\n"
	"	#define IN varying \n"
	"#else\n" /* OpenGL Desktop */
	"	#define INSOLENCE_GLSL_DESKTOP\n"
	"	#define IN in\n"
	"#endif\n";



Shader::Shader(GLenum type)
{
	id = glCreateShader(type);
}

Shader::~Shader()
{
	glDeleteShader(id);
}

GLint Shader::GetShaderiv(GLenum param)
{
	GLint output;
	glGetShaderiv(id, param, &output);
	return output;
}
