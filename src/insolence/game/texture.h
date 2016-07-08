#ifndef TEXTURE_H
#define TEXTURE_H

#include "../insolence_dll.h"
#include "../game/log.h"

#include <climits>
#include <cstdio>
#include <GL/glew.h>
#include <glm/vec4.hpp>


/* TODO Issue #12: Logging success/error. */

struct INSOLENCE_API Texture {
private:
	/**
	 * Initialises ID to zero;
	 */
	Texture();

	static Texture *tex_default;

public:
	GLuint id;
	int width;
	int height;

	static Texture* GetDefault();

	/**
	 * Call glDeleteTextures and frees memory.
	 */
	~Texture();

	/**
	 * Calls glBindTexture with texture ID.
	 */
	void Bind();

	GLuint GetWidth() {
		return width;
	}

	GLuint GetHeight() {
		return height;
	}

	GLint GetID() {
		return id;
	}

	/**
	 * Load a 1x1 texture with an RGBA colour.
	 */
	static Texture* LoadColour(const glm::vec4& colour)
	{
		Texture* output = new Texture();
		glGenTextures(1, &output->id);
		output->Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT,
				&colour[0]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Log success

		return output;
	}

	/**
	 * Load an image from file with DevIL.
	 *
	 * \param path	Path to file.
	 * \return		Texture, ID set to INT_MAX on load fail.
	 */

	static Texture* LoadFile(const char *path);
};

#endif
