#ifndef SYSTEM_SPRITE_RENDERABLE_H
#define SYSTEM_SPRITE_RENDERABLE_H

#include "../insolence_dll.h"

#include "../game/entity.h"
#include "../component/sprite_renderable.h"
#include "../render/render_manager_2d.h"

struct INSOLENCE_API SpriteRenderableSystem : System<SpriteRenderable> {
private:
	RenderManager2D* renderer;

public:
	SpriteRenderableSystem(RenderManager2D*);
	void Manage(const GameTime&);
};

#endif
