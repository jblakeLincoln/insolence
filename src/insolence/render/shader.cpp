#include "shader.h"

#ifdef INSOLENCE_OPENGL_DESKTOP
char Shader::glsl_version[15] = "#version 150\n\0";
#elif INSOLENCE_WEBGL
char Shader::glsl_version[15] = "#version 100\n\0";
#elif INSOLENCE_ANDROID
char Shader::glsl_version[18] = "#version 300 es\n\0";
#endif

const char *Shader::vert_header =
#ifdef INSOLENCE_WEBGL
	"#define INSOLENCE_GLSL_WEB\n"
#elif INSOLENCE_ANDROID
	"#define INSOLENCE_GLSL_ANDROID\n"
#elif INSOLENCE_OPENGL_DESKTOP
	"#define INSOLENCE_GLSL_DESKTOP\n"
#endif

#if INSOLENCE_OPENGL_DESKTOP || INSOLENCE_ANDROID
	"layout(std140) uniform Camera {\n"
	"	mat4 mat_proj;\n"
	"	mat4 mat_view;\n"
	"};\n"
#elif INSOLENCE_WEBGL
	"#define in attribute\n"
	"#define out varying\n"
	"uniform mat4 mat_proj;\n"
	"uniform mat4 mat_view;\n"
#endif
	"#line 1\n";

const char *Shader::frag_header =
#ifdef INSOLENCE_WEBGL
	"#define INSOLENCE_GLSL_WEB\n"
#elif INSOLENCE_ANDROID
	"#define INSOLENCE_GLSL_ANDROID\n"
#elif INSOLENCE_OPENGL_DESKTOP
	"#define INSOLENCE_GLSL_DESKTOP\n"
#endif

#if INSOLENCE_WEBGL || INSOLENCE_ANDROID
	"precision highp float;\n"
#endif
#if INSOLENCE_WEBGL
	"#define texture texture2D\n"
	"#define in varying \n"
	"uniform mat4 mat_proj;\n"
	"uniform mat4 mat_view;\n"
#elif INSOLENCE_OPENGL_DESKTOP || INSOLENCE_ANDROID
	"out vec4 out_colour;\n"
	"#define gl_FragColor out_colour\n"
#endif
	"#line 1\n";

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
