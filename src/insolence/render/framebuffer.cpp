#include "framebuffer.h"

#include "../game/log.h"

#include "../game/texture.h"
#include "../game/window.h"


Framebuffer::~Framebuffer()
{
	delete tex;
	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &rbo);
}

Framebuffer* Framebuffer::Create(Window *w)
{
	Framebuffer *out = new Framebuffer();

	out->window = w;
	out->dimensions = glm::ivec2(
				w->GetFramebufferWidth(), w->GetFramebufferHeight());

	out->tex = Texture::CreateBlank(out->dimensions.x, out->dimensions.y);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &out->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, out->fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, out->tex->GetID(), 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_TEXTURE_2D, out->tex->GetID(), 0);

	glGenRenderbuffers(1, &out->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, out->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
			out->dimensions.x, out->dimensions.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, out->rbo);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		log(Log::ERR, "%s - Incomplete framebuffer", __FUNCTION__);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return out;
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	if(dimensions.x == window->GetFramebufferWidth() &&
			dimensions.y == window->GetFramebufferHeight())
		return;

	dimensions = glm::ivec2(
			window->GetFramebufferWidth(), window->GetFramebufferHeight());

	tex->Blank(dimensions.x, dimensions.y);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
		window->GetFramebufferWidth(), window->GetFramebufferHeight());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
