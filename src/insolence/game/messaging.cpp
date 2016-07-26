#include "messaging.h"
#include "entity.h"
#include "entity_manager.h"

MessageManager::MessageManager(EntityManager *mgr)
:	manager(mgr)
{}

void MessageManager::Add(uint32_t event, Entity *entity)
{
	messages[event].push_back(entity->GetID());
}

Entity* MessageManager::Get(uint32_t event)
{
	uint32_t id;
	std::vector<uint32_t> *vec;

	if(manager == NULL || (it = messages.find(event)) == messages.end())
		return NULL;

	if((vec = &it->second)->size() == 0)
		return NULL;

	Entity *entity = manager->GetByID(vec->front());

	vec->erase(vec->begin());
	if(vec->size() == 0)
		messages.erase(it);

	return entity;
}

void MessageManager::Clear()
{
	messages.clear();
}

