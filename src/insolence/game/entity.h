#ifndef ENTITY_H
#define ENTITY_H

/**
 * Defines Systems and Entities. They have to all be in the same file because
 * of the templated functions and the way we compile - a bit messy, but the
 * functionality is worth it.
 */

#include "../insolence_dll.h"

#include <cstddef>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "gametime.h"
#include "game_world.h"
#include "../component/component.h"

static uint32_t id_count = 0;

struct Entity;
struct EntityManager;

/**
 * Interface for creating vectors/maps of templated type "System".
 */
struct INSOLENCE_API ISystem {
	virtual ~ISystem() {}

	/**
	 * Used by System to return a component to EntityManager.
	 */
	virtual Component* CreateComponent(Entity*, const Component*)=0;

	/**
	 * Can be used by any derived for interacting with components per-frame.
	 */
	virtual void Manage(const GameTime& gametime) {}

	/**
	 * Used by System to remove entity from its storage container.
	 * For specialised removal (e.g. deleting a component's pointers), see
	 * System's Remove(void).
	 */
	virtual void Remove(Entity*)=0;
};

template <typename TComponent>
struct INSOLENCE_API System : ISystem {
private:

	/**
	 * Makes a copy of the received component and adds it to our component
	 * map.
	 */
	TComponent* CreateTComponent(Entity *e, const TComponent* comp)
	{
		components.emplace(e, *comp);
		TComponent *t = &(components.find(e)->second);
		EndCreation(e, t);

		return t;
	}

protected:
	typename std::unordered_map<Entity*, TComponent>::iterator it;
	std::unordered_map<Entity*, TComponent> components;

public:

	/**
	 * We can't have a templated function here, so we take in a generic
	 * component pointer, cast it to T, and send it to be added to the map.
	 */
	Component* CreateComponent(Entity *e, const Component* c) {
		return CreateTComponent(e, (TComponent*)c);
	}

	/**
	 * Overload this function for specialised removal.
	 */
	virtual void RemoveOverride(Entity *e, TComponent *c) {}

	/**
	 *	Calls Remove and erases the component attached to the sent entity from
	 *	the storage container.
	 */
	void Remove(Entity *e) final {
		RemoveOverride(e, &components.find(e)->second);
		components.erase(e);
	}

	/**
	 * Override function for performing final actions just before adding.
	 */
	virtual void EndCreation(Entity* e, TComponent *c) {}

	/**
	 * \return	Component type.
	 */
	static const std::type_index GetType() {
		return typeid(TComponent);
	}
};

struct INSOLENCE_API Entity
{
private:
	std::unordered_map<std::type_index, Component*> components;
	uint32_t id;
	EntityManager *manager;

	/**
	 * Check if a System has already been added/created for a particular
	 * type of component.
	 */
	bool CheckSystemExists(const std::type_index &type);

	/**
	 * For the creation of a new system to store a particular component
	 * type. Adds the new system to the manager, which retains the pointer and
	 * controls its lifetime.
	 */
	void SendSystemToManager(ISystem*, const std::type_index &type);

	/**
	 * Send a component to the manager, which will then be copied to the
	 * relevant system. We ensure the manager already has the system by
	 * calling SendSystemToManager because the manager isn't capable of
	 * creating a system itself.
	 */
	Component* SendToManager(const Component *T, const std::type_index &type);

	/**
	 * Removes a component from the system it is attached to in the manager.
	 */
	void RemoveFromManager(const std::type_index &type);

	Entity() {}

	~Entity();

public:

	uint32_t GetID() { return id; }

	/**
	 * Create an Entity with systems managed by a particular EntityManager.
	 */
	Entity(EntityManager *m);

	void Destroy() {
		delete this;
	}

	/**
	 * Adds a type of component to the relevant system. If no system exists to
	 * handle it, create one. The attached component must implement at least one
	 * constructor and be using it. An object of the component will be created,
	 * then passed through and copied to the system.
	 *
	 * \param T		Type of component to attach.
	 * \param Args	Arguments for the desired constructor of the component.
	 *
	 * \return		Pointer to created component, otherwise 0.
	 */
	template<typename T, typename... Args>
	T* Add(Args... args);

	/**
	 * If the desired component exists, return it. Otherwise return 0;
	 */
	template <typename T>
	T* Get();

	/**
	 * Remove the desired component from its system, and the entity.
	 */
	template<typename T>
	void Remove();
};

template<typename T, typename... Args>
T* Entity::Add(Args... args)
{
	const std::type_index type = typeid(T);
	T component(args...);

	if(CheckSystemExists(type) == false)
		SendSystemToManager((ISystem*)new System<T>(), type);

	return (T*)(components[type] = (T*)SendToManager(&component, type));
}

template <typename T>
T* Entity::Get()
{
	return (T*)components[typeid(T)];
}

template<typename T>
void Entity::Remove()
{
	const std::type_index type = typeid(T);

	RemoveFromManager(type);
	components.erase(type);
}

#endif
