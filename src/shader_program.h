#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>

#include "shader.h"

struct ShaderProgram
{
private:
	GLuint id;

public:
	/**
	 * Call glCreateProgram and assign id.
	 */
	ShaderProgram();

	/**
	 * Call glDeleteProgram and free memory.
	 */
	~ShaderProgram();

	/**
	 * Attach a shader to the program.
	 *
	 * \param shader	Shader to attach.
	 */
	void AttachShader(Shader *shader);

	/**
	 * return	Program id.
	 */
	GLuint GetID() { return id; }
};

#endif
