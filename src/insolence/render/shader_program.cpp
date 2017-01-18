#include "shader_program.h"

#include "shader.h"

ShaderProgram::ShaderProgram()
{
	id = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(id);
}

void ShaderProgram::AttachShader(Shader *shader)
{
	glAttachShader(id, shader->GetID());
}

GLint ShaderProgram::GetUniformLocation(const char *name)
{
	return glGetUniformLocation(id, name);
}

GLint ShaderProgram::GetAttribLocation(const char *name)
{
	return glGetAttribLocation(id, name);
}
