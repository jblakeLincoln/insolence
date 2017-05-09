#include "texture.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture* Texture::tex_default = 0;

Texture* Texture::GetDefault()
{
	if(tex_default == 0)
		tex_default = Texture::LoadColour(glm::vec4(1.f));

	return tex_default;
}

Texture::Texture()
{
	id = 0;
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

Texture* Texture::LoadFile(const char *path)
{
	Texture *output = new Texture();

	if(FILE *f = fopen(path, "r"))
		fclose(f);
	else
	{
		log(Log::FATAL, "Texture (%s) - File %s cannot be found",
				__FUNCTION__, path);
		output->id = INT_MAX;
		return output;
	}

	unsigned char *image = stbi_load(path, &output->width, &output->height,
			NULL, 4);

	glGenTextures(1, &output->id);
	output->Bind();

	/* TODO Issue #25: Try ilCopyPixels to fix alpha issues. */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			output->width, output->height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifndef INSOLENCE_ANDROID
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
#else
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);
#endif

	stbi_image_free(image);

	log(Log::INFO, "Texture (%s) - loaded %s successfully",
			__FUNCTION__, path);

	return output;
}

Texture *Texture::CreateBlank(uint32_t w, uint32_t h, GLint in_iformat,
		GLint in_format, GLenum in_type)
{
	Texture *output = new Texture();

	output->width = w;
	output->height = h;
	output->internal_format = in_iformat;
	output->format = in_format;
	output->datatype = in_type;

	glGenTextures(1, &output->id);
	output->Bind();
	output->Blank(w, h);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return output;
}

void Texture::Blank(uint32_t w, uint32_t h)
{
	void *data;
	int data_size = 0;

	width = w;
	height = h;
	Bind();

	if(datatype == GL_UNSIGNED_BYTE)
		data_size = sizeof(uint8_t);
	else if(datatype == GL_FLOAT)
		data_size = sizeof(float);

	/*
	 * Does this look dumb? It is dumb.
	 * The texture needs setting to zero to suppress WebGL errors even though
	 * the implementation always seems to make sure it's set to zero.
	 *
	 * Functions designed to zero out textures are GL4.3 onwards.
	 * Can't really stack allocate it.
	 * So here we are.
	 */
	data_size *= w * h;
	data = malloc(data_size);
	memset(data, 0, data_size);

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format,
			datatype, data);

	free(data);
}
