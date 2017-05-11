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
	static ShaderProgram *FromPair(const char *vert_path, const char *frag_path);

	ShaderProgram();
	~ShaderProgram();

	GLuint GetID() const {
		return id;
	}

	void AttachShader(Shader *shader);

	GLint GetUniformLocation(const char *name);
	GLint GetAttribLocation(const char *name);
};

#endif
