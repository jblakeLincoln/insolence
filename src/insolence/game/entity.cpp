#include "entity.h"
#include "entity_manager.cpp"

Entity::Entity(EntityManager *m)
:
	id(id_count++),
	manager(m)
{}

Entity::~Entity()
{
	std::unordered_map<size_t, Component*>::iterator it;

	while(components.size() != 0)
	{
		it = components.begin();
		const size_t hash = typeid(*it->second).hash_code();
		RemoveFromManager(hash);
		components.erase(hash);
	}

	manager->Remove(this);
}

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

void Entity::SendSystemToManager(ISystem *sys, size_t hash)
{
	manager->AddSystem(sys, hash);
}
