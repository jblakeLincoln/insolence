#ifndef COMPONENT_SPRITE_RENDERABLE_H
#define COMPONENT_SPRITE_RENDERABLE_H

#include "../game/animation.h"

struct SpriteRenderable : Component {
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
