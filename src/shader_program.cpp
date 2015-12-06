#include "shader_program.h"

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
