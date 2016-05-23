#ifndef COMPONENT_TEXT_RENDERABLE_H
#define COMPONENT_TEXT_RENDERABLE_H

#include "../insolence_dll.h"

#include "../game/font.h"
#include "../util/insolence_string.h"

struct INSOLENCE_API TextRenderable : Component {
	Font *font;
	char *text;
	glm::vec4 colour;
	FontAlign align;
	glm::vec2 scale;

	TextRenderable(Font *font,
			const glm::vec4& colour=glm::vec4(1.f),
			FontAlign align=FontAlign::LEFT,
			const glm::vec2& scale=glm::vec2(1.f))
	:
		font(font),
		colour(colour),
		align(align),
		scale(scale),
		text(NULL)
	{
	}

	~TextRenderable()
	{
		if(text != NULL)
			delete[] text;
	}

	TextRenderable(const TextRenderable &other)
	{
		font = other.font;
		colour = other.colour;
		align = other.align;
		scale = other.scale;

		if(other.text != NULL)
			text = strdup(other.text);
		else
			text = NULL;
	}

	void Text(const char *format, ...)
	{
		if(text != NULL)
			delete[] text;

		va_list args;
		va_start(args, format);
		vasprintf(&text, format, args);
		va_end(args);
	}
};

#endif
