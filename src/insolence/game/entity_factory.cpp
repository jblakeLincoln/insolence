#include "entity_factory.h"

void EntityManager::Manage(Entity *e, MeshRenderable *c)
{
	if(c->mesh == NULL || c->material == NULL)
		return;

	renderer_3d->Add(c->mesh, c->material, c->colour,
			e->Get<Movement>()->GetModelMatrix());
}

void EntityManager::Manage(Entity *e, SpriteRenderable *c)
{
	if(c->material == NULL)
		return;

	renderer_2d->Add(c->material->diffuse,
			e->Get<Movement>()->GetModelMatrix(),
			c->colour,
			c->animation.GetAnimationRectangle(c->material->diffuse));
}

void EntityManager::Manage(Entity *e, TextRenderable *c)
{
	glm::vec3 pos = e->Get<Movement>()->GetPosition();
	if(c->font == NULL)
		return;

	renderer_2d->AddText(c->font, c->text.c_str(), glm::vec2(pos.x, pos.y),
			c->colour, c->align, c->scale);
}

void EntityManager::Manage(Entity *e, RigidBody *c)
{
	if(c->rigid_body == NULL)
		c->rigid_body = physics->CreateRigidBody(e);

	SyncMovementToRigidBody(e);
}
