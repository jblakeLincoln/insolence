#include "shader.h"

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
