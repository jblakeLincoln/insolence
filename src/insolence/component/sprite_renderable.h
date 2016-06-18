#ifndef COMPONENT_SPRITE_RENDERABLE_H
#define COMPONENT_SPRITE_RENDERABLE_H

#include "../insolence_dll.h"

#include "component.h"
#include "transform.h"
#include "../game/animation.h"

struct INSOLENCE_API SpriteRenderable : Component {
	Texture *texture;
	Animation animation;
	glm::vec4 colour;
	glm::vec2 origin;

	SpriteRenderable(Texture* texture=0,
			const glm::vec4& colour=glm::vec4(1.f),
			const glm::vec2 &origin=glm::vec2(0.f),
			const Animation& animation=Animation())
	:
		texture(texture),
		colour(colour),
		animation(animation),
		origin(origin)
	{}
};

#endif
