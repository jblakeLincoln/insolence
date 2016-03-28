#ifndef COMPONENT_SPRITE_RENDERABLE_H
#define COMPONENT_SPRITE_RENDERABLE_H

#include "../game/animation.h"

struct SpriteRenderable : Component {
	Material *material;
	Animation animation;
	glm::vec4 colour;

	SpriteRenderable() :
		colour(glm::vec4(1.f))
	{}

};

#endif
