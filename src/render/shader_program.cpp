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

/* Untested. */
void ShaderProgram::EnableAttrib(int buffer, const char *attrib_name, int size,
		GLenum type, GLboolean normalised, GLsizei stride,
		const GLvoid *pointer)
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	GLuint index = glGetAttribLocation(id, attrib_name);

	glVertexAttribPointer(index, size, type, normalised, stride, pointer);
	glEnableVertexAttribArray(index);
}

