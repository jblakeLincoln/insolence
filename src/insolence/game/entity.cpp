#include "entity.h"
#include "entity_manager.cpp"

Entity::Entity(EntityManager *m)
:
	id(id_count++),
	manager(m)
{}

Component* Entity::SendToManager(const Component *c, size_t hash)
{
	return manager->Add(this, c, hash);
}

bool Entity::CheckSystemExists(size_t hash)
{
	return manager->HasSystem(hash);
}

void Entity::RemoveFromManager(size_t hash)
{
	return manager->Remove(this, hash);
}

void Entity::RemoveEntityFromManager(Entity*)
{
	manager->Remove(this);
}

void Entity::SendSystemToManager(ISystem *sys, size_t hash)
{
	manager->AddSystem(sys, hash);
}
