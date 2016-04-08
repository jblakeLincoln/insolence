#ifndef ENTITY_H
#define ENTITY_H

/**
 * Defines Systems and Entities. They have to all be in the same file because
 * of the templated functions and the way we compile - a bit messy, but the
 * functionality is worth it.
 */

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
struct ISystem {
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
struct System : ISystem {
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

		/* Square bracket overload wants a constructor. */
		//return &(components[e] = (TComponent)(*comp));
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

	virtual void EndCreation(Entity* e, TComponent *c) {}
};

struct Entity
{
private:
	std::unordered_map<size_t, Component*> components;
	EntityManager *manager;

	/**
	 * Check if a System has already been added/created for a particular
	 * type of component.
	 *
	 * \param hash	Hash of the component type.
	 * \return		True for an existing system, otherwise false.
	 */
	bool CheckSystemExists(size_t hash);

	/**
	 * For the creation of a new system to store a particular component
	 * type.Adds the new system to the manager, which retains the pointer and
	 * controls its lifetime.
	 *
	 * \param sys	System to hand over to manager.
	 * \param hash	Hash of component type.
	 */
	void SendSystemToManager(ISystem*, size_t hash);

	/**
	 * Send a component to the manager, which will then be copied to the
	 * relevant system. We ensure the manager already has the system by
	 * calling SendSystemToManager because the manager isn't capable of
	 * creating a system itself.
	 *
	 * \param c		Pointer to component to be copied to system.
	 * \param hash	Hash of component/system type.
	 */
	Component* SendToManager(const Component *T, size_t hash);

	/**
	 * Removes a component from the system it is attached to in the manager.
	 *
	 * \param hash	Hash of component type.
	 */
	void RemoveFromManager(size_t);

	void RemoveEntityFromManager(Entity*);

	/**
	 * Removes this entity from the manager - the manager will handle
	 * getting rid of all the components from the relevant systems.
	 *
	 * TODO
	 */

	Entity() {}

	uint32_t id;
public:

	uint32_t GetID() { return id; }

	void Destroy()
	{
		delete this;
	}

	Entity(EntityManager *m);
	/**
	 * Create an Entity with systems managed by a particular EntityManager.
	 */
	~Entity();

	/**
	 * Functions for interacting with components.
	 * Declared and commented at the bottom of the file.
	 */
	template<typename T, typename... Args>
	void Add(Args... args);

	template <typename T>
	T* Get();

	template<typename T>
	void Remove();
};

/**
 * Adds a type of component to the relevant system. If no system exists to
 * handle it, create one. The attached component must implement at least one
 * constructor and be using it. An object of the component will be created,
 * then passed through and copied to the system.
 *
 * \param T		Type of component to attach.
 * \param Args	Arguments for the desired constructor of the component.
 */
template<typename T, typename... Args>
void Entity::Add(Args... args)
{
	const size_t hash = typeid(T).hash_code();
	T component(args...);

	if(CheckSystemExists(hash) == false)
		SendSystemToManager((ISystem*)new System<T>(), hash);

	components[hash] = (T*)SendToManager(&component, hash);
}

/**
 * If the desired component exists, return it. Otherwise return 0;
 */
template <typename T>
T* Entity::Get()
{
	return (T*)components[typeid(T).hash_code()];
}

/**
 * Remove the desired component from its system, and the entity.
 */
template<typename T>
void Entity::Remove()
{
	const size_t hash = typeid(T).hash_code();
	printf("Removing from manager\n");
	RemoveFromManager(hash);
	printf("Removed from manager\n");
	components.erase(hash);
	printf("Erased\n");
}

#endif
