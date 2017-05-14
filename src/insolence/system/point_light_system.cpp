#include "point_light_system.h"

#include "../game/default_renderers.h"
#include "../component/transform.h"

PointLightSystem::PointLightSystem()
{
	renderer = DefaultRenderer::Get()->Renderer3D;
}

void PointLightSystem::
Manage(const GameTime &gametime, Entity &e, PointLight &c)
{
	if(c.light == NULL)
		return;

	c.light->pos = e.Get<Transform>()->GetPos();

	renderer->Add(NULL, Material::GetDefault(),
			glm::scale(e.Get<Transform>()->GetModelMatrix(), glm::vec3(0.1f)),
			glm::vec4(c.light->diffuse, 1.f));
}

void PointLightSystem::
OnAdd(Entity &e, PointLight &c)
{
	c.light = renderer->RequestPointLight();
}

void PointLightSystem::
OnRemove(Entity &e, PointLight &c)
{
	if(c.light != NULL)
		renderer->FreePointLight(c.light);
}
