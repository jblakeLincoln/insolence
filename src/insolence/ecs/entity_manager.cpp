#include "entity_manager.h"

void Entity::Destroy() {
	mgr->entities.Destroy(mgr->systems, this);
}

/**************************************
 * EntityManager Systems functions
 **************************************/

EntityManager::Systems::Systems() {
	id_lookup.reserve(INSOLENCE_MAX_COMPONENTS);
	systems_prioritised.reserve(INSOLENCE_MAX_COMPONENTS);

	for(size_t i = 0; i < INSOLENCE_MAX_COMPONENTS; ++i)
		systems[i] = nullptr;
}

EntityManager::Systems::~Systems() {
	for(size_t i = 0; i < INSOLENCE_MAX_COMPONENTS; ++i) {
		if(systems[i] == nullptr)
			continue;

		delete systems[i];
		systems[i] = nullptr;
	}
}

void EntityManager::Systems::Add
(EntityManager *mgr, const std::type_index &type, ISystem *sys)
{
	uint64_t id = GetComponentID(type);

	if(systems[id] != nullptr)
		return;

	sys->mgr = mgr;
	systems[id] = sys;
	id_lookup[type] = id;

	size_t i;
	for(i = 0; i < systems_prioritised.size(); ++i) {
		if(systems_prioritised[i]->GetPriority() < sys->GetPriority())
			break;
	}

	systems_prioritised.insert(systems_prioritised.begin() + i, sys);
}

bool EntityManager::Systems::Has(const std::type_index &type) {
	if(id_lookup.count(type) == 0)
		return false;

	ISystem *sys = systems[id_lookup.at(type)];
	return sys != nullptr;
}

ISystem* EntityManager::Systems::Get(const std::type_index &type) {
	return systems[id_lookup.at(type)];
}

ISystem* EntityManager::Systems::Get(uint64_t id) {
	return systems[id];
}

uint64_t EntityManager::Systems::GetComponentID(const std::type_index &type) {
	if(id_lookup.count(type) == 0)
		id_lookup[type] = ++component_ids;
	return id_lookup[type];
}

void EntityManager::Systems::Manage(const GameTime &gametime) {
	for(size_t i = 0; i < systems_prioritised.size(); ++i)
		systems_prioritised[i]->Manage(gametime);
}

/**************************************
 * EntityManager Entities functions
 **************************************/

EntityManager::Entities::~Entities() {
	for(auto it = entities.begin(); it != entities.end(); ++it)
		delete it->second;
}

Entity* EntityManager::Entities::Create(EntityManager *mgr) {
	Entity *e = new Entity(mgr);
	entities[++entity_ids] = e;
	e->id = entity_ids;
	e->Add<Transform>();
	return e;
}

Entity* EntityManager::Entities::Get(uint64_t id) {
	if(entities.count(id) == 0)
		return nullptr;
	return entities[id];
}

void EntityManager::Entities::Destroy(Systems &mgr_systems, Entity *e) {
	for(size_t i = 0; i < e->components.size(); ++i) {
		if(e->components[i] == false)
			continue;
		mgr_systems.Get(i)->RemoveComponent(e);
	}

	entities.erase(e->id);
	delete e;
}

