#include "sprite_renderable_system.h"

SpriteRenderableSystem::SpriteRenderableSystem(RenderManager2D* r)
:	renderer(r)
{}

void SpriteRenderableSystem::Manage(const GameTime& gametime)
{
	for(it = components.begin(); it!= components.end(); ++it)
	{
		SpriteRenderable *s = &it->second;
		Movement *m = it->first->Get<Movement>();

		if(m == NULL || s->texture == 0)
			continue;

		s->animation.Progress(gametime.GetFrameTime());

		renderer->Add(s->texture, m->GetModelMatrix(), s->colour,
				s->animation.GetAnimationRectangle(s->texture));
	}
}
