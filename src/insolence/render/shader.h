#ifndef SHADER_H
#define SHADER_H

#include "../insolence_dll.h"

#include "../game/log.h"

#include <stdio.h>
#include <GL/glew.h>

/* TODO Issue #10: Logging. */
struct INSOLENCE_API Shader
{
private:
	/**
	 * Call glCreateShader and assigns id.
	 *
	 * \param type	Shader type.
	 */
	Shader(GLenum type);

	GLuint id;

public:
	/**
	 * Calls glDeleteShader and deletes id.
	 */
	~Shader();

	/**
	 * \return	Shader ID.
	 */
	GLuint GetID() { return id; }

	/**
	 * Calls glGetShaderiv to get compile status, shader type etc.
	 *
	 * \param param	GL parameter to query.
	 * \return		Requested object parameter.
	 */
	GLint GetShaderiv(GLenum param);

	/**
	 * Create shader from string.
	 *
	 * \param source	String to use as shader source.
	 * \param type		Shader type to create.
	 * \return			Pointer to shader object.
	 */
	static Shader *CreateFromString(const char *source, GLenum type)
	{
		Shader *output = new Shader(type);

		glShaderSource(output->id, 1, &source, NULL);
		glCompileShader(output->id);

		return output;
	}

	/**
	 * Create shader from file. Gets the string from file and calls
	 * CreateFromString.
	 *
	 * \param path	Path of file to use as source.
	 * \param type	Shader type to create.
	 * \return		Pointer to shader object.
	 */
	static Shader *CreateFromFile(const char *path, GLenum type)
	{
		Shader *output = NULL;
		char *buffer;
		FILE *file;
		int length;

		file = fopen(path, "r");

		if(file == NULL)
		{
			log(Log::ERR, "Shader (%s) - Failed to find file %s",
					__FUNCTION__, path);
			return new Shader(type);
		}

		fseek(file, 0, SEEK_END);
		length = ftell(file);
		rewind(file);

		buffer = new char[length+1];
		buffer[length] = '\0';
		int temp = fread(buffer, 1, length, file); /* Stupid unused warn. */
		fclose(file);

		output = Shader::CreateFromString(buffer, type);

		delete[] buffer;

		log(Log::INFO, "Shader (%s) - Successfully created %s",
				__FUNCTION__, path);

		return output;
	}
};

#endif
