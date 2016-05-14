#ifndef COMPONENT_SPRITE_RENDERABLE_H
#define COMPONENT_SPRITE_RENDERABLE_H

#include "../insolence_dll.h"

#include "../game/animation.h"

struct INSOLENCE_API SpriteRenderable : Component {
	Texture *texture;
	Animation animation;
	glm::vec4 colour;

	SpriteRenderable(Texture* texture=0,
			const glm::vec4& colour=glm::vec4(1.f),
			const Animation& animation=Animation()) :
		texture(texture),
		colour(colour),
		animation(animation)
	{}
};

#endif
