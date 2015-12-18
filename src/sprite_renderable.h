#ifndef SPRITE_RENDERABLE_H
#define SPRITE_RENDERABLE_H

#include "renderable.h"

struct SpriteRenderable : public Renderable
{
	Texture *tex;

	SpriteRenderable(Entity *e);
	~SpriteRenderable();

	/**
	 * Overload of Renderable draw that sends data to the renderer.
	 */
	void Draw();
};

#endif
