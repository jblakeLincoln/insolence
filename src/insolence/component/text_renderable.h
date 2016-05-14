#ifndef COMPONENT_TEXT_RENDERABLE_H
#define COMPONENT_TEXT_RENDERABLE_H

#include "../insolence_dll.h"

#include "../game/font.h"
#include <string>

struct INSOLENCE_API TextRenderable : Component {
	Font *font;
	std::string text;
	glm::vec4 colour;
	FontAlign align;
	glm::vec2 scale;

	TextRenderable(Font *font=0,
			const std::string text="",
			const glm::vec4& colour=glm::vec4(1.f),
			FontAlign align=FontAlign::LEFT,
			const glm::vec2& scale=glm::vec2(1.f))
	:
		font(font),
		text(text),
		colour(colour),
		align(align),
		scale(scale)
	{}

	void Text(const char *format, ...)
	{
		char *buf;
		va_list va;
		va_start(va, format);
		vasprintf(&buf, format, va);
		va_end(va);

		text = buf;
		delete buf;
	}
};

#endif
