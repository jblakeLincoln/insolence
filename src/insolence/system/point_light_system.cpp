#include "point_light_system.h"

#include "../component/transform.h"

void System<PointLight>::
Manage(const GameTime &gametime, Entity &e, PointLight &c)
{
	if(c.light == NULL)
		return;

	c.light->pos = e.Get<Transform>()->GetPos();

	renderer->Add(NULL, Material::GetDefault(),
			glm::scale(e.Get<Transform>()->GetModelMatrix(), glm::vec3(0.1f)),
			glm::vec4(c.light->diffuse, 1.f));
}

void System<PointLight>::
OnAdd(Entity &e, PointLight &c)
{
	c.light = renderer->RequestPointLight();
}

void System<PointLight>::
OnRemove(Entity &e, PointLight &c)
{
	if(c.light != NULL)
		renderer->FreePointLight(c.light);
}
