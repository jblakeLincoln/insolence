#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "../insolence_dll.h"

#include "graphics.h"
#include "shader.h"

struct INSOLENCE_API ShaderProgram
{
private:
	GLuint id;

public:
	ShaderProgram();
	~ShaderProgram();

	void AttachShader(Shader *shader);

	GLuint GetID() const {
		return id;
	}

	GLint GetUniformLocation(const char *name);
	GLint GetAttribLocation(const char *name);
};

#endif
