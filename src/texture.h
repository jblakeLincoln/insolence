#ifndef TEXTURE_H
#define TEXTURE_H
#define ILUT_USE_OPENGL

#include <stdio.h>
#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <IL/ilut.h>

/* TODO Issue #12: Logging success/error. */

struct Texture
{
private:
	/**
	 * Initialises ID to zero;
	 */
	Texture();
public:
	GLuint id;
	GLuint width;
	GLuint height;

	/**
	 * Call glDeleteTextures and frees memory.
	 */
	~Texture();

	/**
	 * Calls glBindTexture with texture ID.
	 */
	void Bind();

	/**
	 * return width.
	 */
	GLuint GetWidth() { return width; }

	/**
	 * return height.
	 */
	GLuint GetHeight() { return height; }

	/**
	 * return ID.
	 */
	GLint GetID() { return id; }

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
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Log success

		return output;
	}

	/**
	 * Load an image from file with DevIL.
	 *
	 * \param path	Path to file.
	 * \return		Texture, ID set to INT_MAX on load fail.
	 */
	static Texture* LoadFile(const char *path)
	{
		Texture *output = new Texture();
		ILuint image = ilGenImage();

		if(FILE *f = fopen(path, "r"))
			fclose(f);
		else
		{
			// Log file does not exist.
			return output;
		}

		ilBindImage(image);
		ilLoadImage(path);

		output->width = ilGetInteger(IL_IMAGE_WIDTH);
		output->height = ilGetInteger(IL_IMAGE_WIDTH);

		glGenTextures(1, &output->id);
		output->Bind();

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT),
				output->width, output->height, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), ilGetData());

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Log success

		return output;
	}

	/**
	 * Initialise DevIL for texture loading.
	 */
	static void Init()
	{
		ilInit();
		iluInit();
		ilutInit();
		ilutRenderer(ILUT_OPENGL);
	}
};

#endif