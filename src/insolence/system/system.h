#ifndef SYSTEM_H
#define SYSTEM_H

#include "../insolence_dll.h"

#include <algorithm>
#include <typeindex>
#include <map>
#include <unordered_map>
#include <vector>

#include "../game/gametime.h"

struct Entity;
struct Component;

static const uint32_t DEFAULT_SYSTEM_PRIORITY = 100;

/**
 * Interface for creating vectors/maps of templated type "System".
 */
struct INSOLENCE_API ISystem {
protected:
public:
	const uint32_t priority;

	ISystem(uint32_t p)	:
		priority(p)
	{}

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

template<typename TComponent, uint32_t Priority = DEFAULT_SYSTEM_PRIORITY>
struct SystemBase : ISystem {
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
	virtual void Manage(const GameTime &gametime) {
		for(it = components.begin(); it != components.end(); ++it)
			Manage(gametime, (Entity*)it->first, &it->second);
	}

	virtual void Manage(const GameTime &gametime, Entity *e, TComponent *c) {}

	SystemBase() :
		ISystem(Priority) {
	}

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

template<typename T>
struct System : SystemBase<T> {
};

#endif
