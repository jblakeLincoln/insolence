#ifndef SHADER_H
#define SHADER_H

#include "../insolence_dll.h"

#include "../game/log.h"
#include "graphics.h"

#include <stdio.h>

enum ShaderFlags {
	NONE =               1 << 0,
	ADD_HEADER =         1 << 1,
};

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
	static char glsl_version[];
	static const char *vert_header;
	static const char *frag_header;

	static void Initialise() {
		return;

		/* Placeholder - dynamically get GLSL version. */
		if(strcmp(glsl_version, "") != 0)
			return;

		char *glsl_getstr;
		const char* p_index;

		if(asprintf(&glsl_getstr, "%s",
					glGetString(GL_SHADING_LANGUAGE_VERSION)) < 0)
			return;
		if((p_index = strchr(glsl_getstr, '.')) == NULL)
			return;

		int index = p_index - glsl_getstr;

		glsl_version[0] = glsl_getstr[index - 1];
		glsl_version[1] = glsl_getstr[index + 1];
		glsl_version[2] = glsl_getstr[index + 2];
		glsl_version[3] = '\0';

		free(glsl_getstr);
	}

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
	 * Calls glGetShaderiv with the GL_COMPILE_STATUS parameter.
	 */
	bool GetCompileStatus();

	/**
	 * Logs shader compile status to Log::ERR. If shader compilation
	 * is successful, this log with a blank message.
	 *
	 * \param prepend	Variable formatted string to prepend to log message
	 */
	void LogCompileInfo(const char *prepend, ...);

	/**
	 * Create shader from string.
	 *
	 * \param source    String to use as shader source.
	 * \param type      Shader type to create.
	 * \param flags     Optional flags to modify compiled shader.
	 * \return          Pointer to shader object.
	 */
	static Shader *CreateFromString(const char *source, GLenum type,
			ShaderFlags flags = ShaderFlags::NONE)
	{
		Shader *output = new Shader(type);

		const char* arr[3] = { "", "", source };
		GLint lengths[] = { 0, 0, (GLint)strlen(source) };

		/*
		 * Add a header with version number and OpenGL desktop/ES conversions..
		 */
		if((flags & ShaderFlags::ADD_HEADER) == ShaderFlags::ADD_HEADER)
		{
			arr[0] = Shader::glsl_version;
			arr[1] = type == GL_VERTEX_SHADER ? vert_header : frag_header;
			lengths[0] = (GLint)strlen(arr[0]);
			lengths[1] = (GLint)strlen(arr[1]);
		}

		glShaderSource(output->id, 3, arr, lengths);
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
	static Shader *CreateFromFile(const char *path, GLenum type,
			ShaderFlags flags = ShaderFlags::NONE)
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

		output = Shader::CreateFromString(buffer, type, flags);

		delete[] buffer;

		log(Log::INFO, "Shader (%s) - Successfully created %s",
				__FUNCTION__, path);

		return output;
	}
};

#endif
