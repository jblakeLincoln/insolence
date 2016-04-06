#ifndef SYSTEM_SPRITE_RENDERABLE_H
#define SYSTEM_SPRITE_RENDERABLE_H

#include "../game/entity.h"
#include "../component/sprite_renderable.h"
#include "../render/render_manager_2d.h"

struct SpriteRenderableSystem : System<SpriteRenderable> {
private:
	RenderManager2D* renderer;

public:
	SpriteRenderableSystem(RenderManager2D*);
	void Manage(const GameTime&);
};

#endif
