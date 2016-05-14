#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "../insolence_dll.h"

#include <GL/glew.h>

#include "shader.h"

struct INSOLENCE_API ShaderProgram
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
	 * \return	Program id.
	 */
	GLuint GetID() const { return id; }

	/**
	 * Bind a buffer, set a vertex attrib pointer, and enable the attrib
	 * in one go.
	 *
	 * \param buffer		Buffer to be bound.
	 * \param attrib		Name of shader attribute.
	 * \param size			Size of attribute.
	 * \param type			Data type of attribute.
	 * \param normalised	GL_ bool to normalise.
	 * \param stride		Buffer stride.
	 * \param pointer		Buffer data position.
	 */
	void EnableAttrib(int buffer, const char *attrib_name, int size,
			GLenum type, GLboolean normalised, GLsizei stride,
			const GLvoid *pointer);
};

#endif
