#include "sprite_renderable_system.h"

#include "../render/render_manager_2d.h"

SpriteRenderableSystem::SpriteRenderableSystem(RenderManager2D* r)
:	renderer(r)
{}

void SpriteRenderableSystem::Manage(const GameTime& gametime)
{
	for(it = components.begin(); it!= components.end(); ++it)
	{
		SpriteRenderable *s = &it->second;
		Transform *m = it->first->Get<Transform>();

		if(m == NULL || s->texture == 0)
			continue;

		s->animation.Progress(gametime.GetFrameTime());

		glm::mat4 model = m->GetModelMatrix();
		model[3][0] -= s->origin.x;
		model[3][1] -= s->origin.y;

		renderer->Add(s->texture, m->GetModelMatrix(), s->colour,
				s->animation.GetAnimationRectangle(s->texture), s->layer);
	}
}
