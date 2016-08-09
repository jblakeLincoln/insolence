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
	"	#define in attribute\n"
	"	#define out varying\n"
	"	uniform mat4 mat_proj;\n"
	"	uniform mat4 mat_view;\n"
	"#else\n" /* OpenGL Desktop */
	"	#define INSOLENCE_GLSL_DESKTOP\n"
	"	#define IN in\n"
	"	#define OUT out\n"
	"	layout(std140) uniform Camera {\n"
	"		mat4 mat_proj;\n"
	"		mat4 mat_view;\n"
	"	};\n"
	"#endif\n"
	"#line 0\n";

const char *Shader::frag_header =
	"#if __VERSION__ < 120 || __VERSION__ == 300\n" /* OpenGL ES */
	"	#define INSOLENCE_GLSL_ES\n"
	"	precision highp float;\n"
	"	#define texture texture2D\n"
	"	#define in varying \n"
	"#else\n" /* OpenGL Desktop */
	"	#define INSOLENCE_GLSL_DESKTOP\n"
	"	#define IN in\n"
	"	out vec4 out_colour;\n"
	"	#define gl_FragColor out_colour\n"
	"#endif\n"
	"#line 0\n";



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

bool Shader::GetCompileStatus()
{
	return GetShaderiv(GL_COMPILE_STATUS);
}

void Shader::LogCompileInfo(const char *prepend, ...)
{
	char buf[1024];
	int length = 0;
	va_list args;

	glGetShaderInfoLog(GetID(), 1024, &length, buf);

	if(length == 0)
		return;

	buf[length-1] = '\0';

	va_start(args, prepend);
	log(Log::ERR, "%sShader log: %s", prepend, buf);
	va_end(args);
}
