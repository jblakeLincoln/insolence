#include "entity.h"
#include "entity_manager.cpp"

uint32_t Entity::id_count = 0;

Entity::Entity(EntityManager *m)
:
	id(id_count++),
	manager(m)
{}

Entity::~Entity()
{
	std::unordered_map<std::type_index, Component*>::iterator it;

	while(components.size() != 0)
	{
		it = components.begin();
		const std::type_index type = typeid(*it->second);
		RemoveFromManager(type);
		components.erase(type);
	}

	manager->Remove(this);
}

uint32_t Entity::GetComponentID(const std::type_index &type)
{
	return manager->GetComponentID(type);
}

Component* Entity::SendToManager(const Component *c,
		const std::type_index &type)
{
	return manager->Add(this, c, type);
}

bool Entity::CheckSystemExists(const std::type_index &type)
{
	return manager->HasSystem(type);
}

void Entity::RemoveFromManager(const std::type_index &type)
{
	return manager->Remove(this, type);
}

void Entity::SendSystemToManager(ISystem *sys, const std::type_index &type)
{
	manager->AddSystem(sys, type);
}
