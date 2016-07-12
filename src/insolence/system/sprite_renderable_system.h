#ifndef SYSTEM_SPRITE_RENDERABLE_H
#define SYSTEM_SPRITE_RENDERABLE_H

#include "../insolence_dll.h"

#include "../component/sprite_renderable.h"
#include "../game/game_world.h"
#include "../game/default_renderers.h"

struct RenderManager2D;
struct INSOLENCE_API SpriteRenderableSystem : System<SpriteRenderable> {
private:
	RenderManager2D* renderer;

public:
	SpriteRenderableSystem(RenderManager2D*);
	void Manage(const GameTime&);
};

static void DrawSprite(const glm::mat4 &model, const SpriteRenderable &s,
		RenderManager2D* r = DefaultRenderer::Get()->Renderer2D)
{
	glm::mat4 m = model;
	m[3][0] -= s.origin.x;
	m[3][1] -= s.origin.y;

	r->Add(s.texture, m, s.colour,
			s.animation.GetAnimationRectangle(s.texture), s.layer);
}

#endif
