#ifndef SYSTEM_SPRITE_RENDERABLE_H
#define SYSTEM_SPRITE_RENDERABLE_H

#include "../insolence_dll.h"

#include "../component/sprite_renderable.h"

struct RenderManager2D;
template<>
struct INSOLENCE_API System<SpriteRenderable> : SystemBase<SpriteRenderable> {
private:
	RenderManager2D* renderer;

public:
	System<SpriteRenderable>() {
		renderer = Game::Get()->renderers.sprite;
	}

	void Manage(const GameTime&);
};

#endif
