#ifndef ENTITY_H
#define ENTITY_H

#include "../insolence_dll.h"

#include <cstddef>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "system.h"

struct Component;
struct EntityManager;

struct INSOLENCE_API Entity {
private:
	static uint32_t id_count;

	EntityManager *manager;
	std::unordered_map<std::type_index, Component*> components;
	uint32_t id;
	uint32_t component_mask = 0;

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

	virtual ~Entity();

	/**
	 * Private check function for getting a type out of a passed through tuple
	 * and checking whether we hold a component for it.
	 */
	template<std::size_t I = 0, typename... Type>
	inline typename std::enable_if<I < sizeof...(Type), bool>::type
	HasComponent(const std::tuple<Type...> &t) {
		bool ret = component_mask & GetComponentID(typeid(std::get<I>(t)));

		if(ret == true)
			ret = HasComponent<I + 1, Type...>(t);

		return ret;
	}

	/**
	 * Go to the next type in the HasComponent tuple.
	 */
	template<std::size_t I = 0, typename... Type>
	inline typename std::enable_if<I == sizeof...(Type), bool>::type
	HasComponent(const std::tuple<Type...> &t) {
		return true;
	}

public:

	/**
	 * Create an Entity with systems managed by a particular EntityManager.
	 */
	Entity(EntityManager *m);

	/**
	 * Returns a unique identifier for the entity within its manager.
	 */
	const uint32_t GetID() const {
		return id;
	}

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
	template<typename T>
	T* Get();

	/**
	 * Remove the desired component from its system, and the entity.
	 */
	template<typename T>
	void Remove();

	template<typename First, typename... Types>
	bool Has() {
		bool ret = component_mask & GetComponentID(typeid(First));

		if(ret == 0 || sizeof...(Types) == 0)
			return ret;

		return HasComponent(std::tuple<Types...>());
	}



		/**
	 * Get the bit field of the component from the manager.
	 */
	uint32_t GetComponentID(const std::type_index &type);
};

template<typename T, typename... Args>
T* Entity::Add(Args... args)
{
	const std::type_index type = typeid(T);
	T component(args...);

	if(CheckSystemExists(type) == false)
		SendSystemToManager((ISystem*)new System<T>(), type);

	component_mask |= GetComponentID(type);

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
	component_mask &= ~(GetComponentID(type));
}

#endif
