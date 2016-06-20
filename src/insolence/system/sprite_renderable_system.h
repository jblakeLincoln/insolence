#ifndef SYSTEM_SPRITE_RENDERABLE_H
#define SYSTEM_SPRITE_RENDERABLE_H

#include "../insolence_dll.h"

#include "../component/sprite_renderable.h"

struct RenderManager2D;
struct INSOLENCE_API SpriteRenderableSystem : System<SpriteRenderable> {
private:
	RenderManager2D* renderer;

public:
	SpriteRenderableSystem(RenderManager2D*);
	void Manage(const GameTime&);
};

#endif
