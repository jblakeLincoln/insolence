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

		if(s->hidden == true || m == NULL || s->texture == 0)
			continue;

		glm::mat4 model;

		s->animation.Progress(gametime.GetFrameTime());

		model = m->GetModelMatrix();
		model = glm::translate(model,
				glm::vec3(-s->origin.x, -s->origin.y, 0.f));
		model = glm::rotate(m->GetModelMatrix(), glm::radians(360 - s->rot),
				glm::vec3(0.f, 0.f, 1.f));
		model = glm::translate(model,
				glm::vec3(s->origin.x, s->origin.y, 0.f));

		renderer->Add(s->texture, model, s->colour,
				s->animation.GetAnimationRectangle(s->texture), s->layer);
	}
}
