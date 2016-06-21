#include "texture.h"

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
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
}
