#include "sprite_renderable_system.h"

#include "../render/render_manager_2d.h"

SpriteRenderableSystem::SpriteRenderableSystem(RenderManager2D* r)
	:	renderer(r)
{
}

void SpriteRenderableSystem::Manage(const GameTime& gametime)
{
	for(it = 0; it < components.size(); ++it)
	{
		SpriteRenderable &s = components[it];

		if(s.hidden == true || s.texture == 0)
			continue;

		Transform *m = entities[it]->Get<Transform>();
		glm::mat4 model;

		s.animation.Progress(gametime.GetFrameTime());

		model = m->GetModelMatrix();
		model = glm::translate(model,
				glm::vec3(-s.origin.x, -s.origin.y, 0.f));
		model = glm::rotate(m->GetModelMatrix(), glm::radians(360 - s.rot),
				glm::vec3(0.f, 0.f, 1.f));
		model = glm::translate(model,
				glm::vec3(s.origin.x, s.origin.y, 0.f));

		renderer->Add(s.texture, model, s.colour,
				s.animation.GetAnimationRectangle(s.texture), s.layer);
	}
}
