#ifndef COMPONENT_TEXT_RENDERABLE_H
#define COMPONENT_TEXT_RENDERABLE_H

#include "../insolence_dll.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "component.h"
#include "../game/font.h"
#include "../util/insolence_string.h"

struct INSOLENCE_API TextRenderable : Component {
	Font *font;
	char *text;
	glm::vec4 colour;
	glm::vec2 scale;
	TextAlignH::AlignH halign;
	TextAlignV::AlignV valign;
	bool hidden;

	TextRenderable(Font *font,
			const glm::vec4& colour=glm::vec4(1.f),
			TextAlignH::AlignH halign=TextAlignH::LEFT,
			TextAlignV::AlignV valign=TextAlignV::BOTTOM,
			const glm::vec2& scale=glm::vec2(1.f))
	:
		font(font),
		colour(colour),
		halign(halign),
		valign(valign),
		scale(scale),
		hidden(false),
		text(NULL)
	{
	}

	~TextRenderable()
	{
		if(text != NULL)
			free(text);
	}

	TextRenderable(const TextRenderable &other)
	{
		font = other.font;
		colour = other.colour;
		halign = other.halign;
		valign = other.valign;
		scale = other.scale;
		hidden = other.hidden;

		if(other.text != NULL)
			text = strdup(other.text);
		else
			text = NULL;
	}

	void Text(const char *format, ...)
	{
		if(text != NULL)
			free(text);

		va_list args;
		va_start(args, format);
		if(vasprintf(&text, format, args) < 0)
			printf("**** TextRenderable vasprintf error\n");
		va_end(args);
	}
};

#endif
