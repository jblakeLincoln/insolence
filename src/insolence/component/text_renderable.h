#ifndef COMPONENT_TEXT_RENDERABLE_H
#define COMPONENT_TEXT_RENDERABLE_H

#include "../insolence_dll.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <string>

#include "component.h"
#include "../game/font.h"

struct INSOLENCE_API TextRenderable : Component {
	struct Modifiers {
		Modifiers() {}

		enum AlignH {
			H_LEFT,
			H_RIGHT,
			H_CENTRE,
		} halign = H_LEFT;

		enum AlignV {
			V_TOP,
			V_BOTTOM,
			V_CENTRE
		} valign = V_BOTTOM;

		glm::vec2 scale = glm::vec2(1.f);
		glm::vec2 offset = glm::vec2(0.f);
		float line_spacing = 1.f;
	} modifiers;

	Font *font;
	std::string text;
	glm::vec4 colour;
	bool hidden;

	TextRenderable(Font *font,
			const std::string &str   = "",
			const glm::vec4& colour  = glm::vec4(1.f),
			const Modifiers &mod = Modifiers())
		: font(font)
		, text(str)
		, colour(colour)
		, modifiers(mod)
		, hidden(false)
	{
	}
};

#endif
