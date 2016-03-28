#ifndef COMPONENT_TEXT_RENDERABLE_H
#define COMPONENT_TEXT_RENDERABLE_H

#include <string>

struct TextRenderable : Component {
	Font *font;
	std::string text;
	glm::vec4 colour;
	FontAlign align;
	glm::vec2 scale;

	TextRenderable() :
		colour(glm::vec4(1.f)),
		align(FontAlign::LEFT),
		scale(glm::vec2(1.f))
	{}

};

#endif
