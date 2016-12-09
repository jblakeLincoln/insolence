#ifndef COMPONENT_SPRITE_RENDERABLE_H
#define COMPONENT_SPRITE_RENDERABLE_H

#include "../insolence_dll.h"

#include "component.h"
#include "transform.h"
#include "../game/animation.h"
#include "../game/texture.h"

struct INSOLENCE_API SpriteRenderable : Component {
	Texture *texture;
	Animation animation;
	glm::vec4 colour;
	glm::vec2 origin;
	float rot;
	int layer;
	bool hidden;

	SpriteRenderable(Texture* texture=Texture::GetDefault(),
			const glm::vec4& colour=glm::vec4(1.f),
			const glm::vec2 &origin=glm::vec2(0.f),
			float rot=0,
			int layer=0,
			bool hidden=false)
	:
		texture(texture),
		colour(colour),
		origin(origin),
		rot(rot),
		layer(layer),
		hidden(hidden)
	{
		animation.Setup(texture);
	}
};

#endif
