#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../insolence_dll.h"

#include "../render/graphics.h"
#include <glm/vec2.hpp>

/*
 * TODO Issue #31: Add optional depth and stencil renderbuffers.
 */

struct Window;
struct Texture;
struct INSOLENCE_API Framebuffer {
private:
	Window *window;
	GLuint fbo;
	GLuint rbo;
	glm::ivec2 dimensions;

	Framebuffer() {}
	Framebuffer(const Framebuffer&) {}

public:
	Texture *tex;

	~Framebuffer();

	static Framebuffer *Create(Window *w);
	static void Unbind();

	void Bind();
};

#endif
