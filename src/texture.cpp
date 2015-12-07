#include "texture.h"

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
