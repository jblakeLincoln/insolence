#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

/**
 * The manager handles Entities, and Systems which manage the components
 * of Entities.
 *
 * Header-only. Entity and the game world need this defined before they can
 * do anything, and so call entity_manager.cpp to ready this before they get
 * started.
 */

#include "../insolence_dll.h"

#include "gametime.h"
#include "../component/component.h"
#include "../component/transform.h"

#include <algorithm>
#include <cstddef>
#include <typeindex>
#include <unordered_map>
#include <vector>

struct Entity;
struct INSOLENCE_API EntityManager {
private:
	struct SystemsContainer {
		std::unordered_map<std::type_index, ISystem*> lookup;
		std::vector<ISystem*> prioritised;

		void Add(const std::type_index &type, ISystem *sys) {
			lookup[type] = sys;
			prioritised.push_back(sys);

			std::sort(prioritised.begin(), prioritised.end(),
					[](ISystem *a, ISystem *b) {
						return a->priority > b->priority;
					});
		}
	};

	/**
	 * Private function for getting a type passed through a tuple and adding
	 * its IP to the passed through mask.
	 */
	template<std::size_t I = 0, typename... Type>
	inline typename std::enable_if<I < sizeof...(Type), void>::type
	GetBitmaskMultiple(uint32_t *mask, const std::tuple<Type...> &t) {
		*mask |= GetComponentID(typeid(std::get<I>(t)));
	}

	/**
	 * Go to the next type in the GetBitmaskMultiple tuple.
	 */
	template<std::size_t I = 0, typename... Type>
	inline typename std::enable_if<I == sizeof...(Type), bool>::type
	GetBitmaskMultiple(uint32_t *mask, const std::tuple<Type...> &t) {
		return true;
	}

public:
	SystemsContainer logic_systems;
	SystemsContainer render_systems;

	std::unordered_map<uint32_t, Entity*> entities_lookup;
	std::unordered_map<std::type_index, uint32_t> component_bits;
	uint32_t component_bit_count = 1;

	EntityManager() {}

	/**
	 * Creates a container system that will simply store all types of
	 * components. Not entirely necessary to call since if a component is made
	 * for a system that doesn't exist, one of these will be automatically
	 * created.
	 */
	template<typename T>
	void AddSystemContainer() {
		const std::type_index type = typeid(T);
		if(logic_systems.lookup.find(type) != logic_systems.lookup.end())
			return;

		System<T> *sys = new System<T>();
		logic_systems.Add(type, sys);
	}

	template<typename T, typename... Args>
	void AddLogicSystem(Args... args) {
		AddSystem<T>(logic_systems, args...);
	}

	template<typename T, typename... Args>
	void AddRenderSystem(Args... args) {
		AddSystem<T>(render_systems, args...);
	}

	/**
	 * Adds an implemented system for a component. If the system already
	 * exists, we bail without doing anything.
	 */
	template<typename T, typename... Args>
	void AddSystem(SystemsContainer &systems,
			Args... args) {
		const std::type_index type = T::GetType();
		if(systems.lookup.find(type) != systems.lookup.end())
			return;

		T *sys = new T(args...);
		sys->manager = this;
		systems.Add(type, sys);
	}

	/**
	 * Creates an entity owned by this manager and returns a reference.
	 */
	Entity* CreateEntity()
	{
		Entity *e = new Entity(this);
		e->Add<Transform>();
		entities_lookup.insert(std::pair<uint32_t, Entity*>(e->GetID(), e));

		return e;
	}

	/**
	 * Get the bitmask identifier of a component or multiple components.
	 */
	template<typename First, typename... Types>
	uint32_t GetBitmask() {
		uint32_t mask = GetComponentID(typeid(First));

		if(sizeof...(Types) == 0)
			return mask;

		GetBitmaskMultiple(&mask, std::tuple<Types...>());
		return mask;
	}

	Entity* GetByID(uint32_t id) {
		return entities_lookup[id];
	}

	/**
	 * Getting rid of our manager means clearing out all related entities and
	 * their attached components.
	 */
	virtual ~EntityManager()
	{
		std::unordered_map<uint32_t, Entity*>::iterator entity_it;
		std::unordered_map<std::type_index, ISystem*>::iterator sys_it;

		for(sys_it = logic_systems.lookup.begin();
				sys_it != logic_systems.lookup.end(); ++sys_it)
		{
			delete sys_it->second;
		}

		for(sys_it = render_systems.lookup.begin();
				sys_it != render_systems.lookup.end(); ++sys_it)
		{
			delete sys_it->second;
		}

		for(entity_it = entities_lookup.begin();
			   entity_it != entities_lookup.end(); ++entity_it)
		{
			delete entity_it->second;
		}
	}

	/**
	 * Iterate over all known systems and call their Manage functions.
	 * Update anything necessary for functionality (such as stepping the
	 * physics).
	 *
	 * \param gametime	GameTime from game world.
	 */
	void Manage(const GameTime& gametime)
	{
		std::vector<ISystem*>::iterator it;

		for(it = logic_systems.prioritised.begin();
				it != logic_systems.prioritised.end(); ++it)
		{
			/* TODO: Only call Manage for manually created systems. */
			(*it)->Manage(gametime);
		}
	}

	void Remove(Entity *e)
	{
		std::unordered_map<uint32_t, Entity*>::iterator it;
		if(entities_lookup.size() == 0 || e == NULL ||
				(it = entities_lookup.find(e->GetID())) == entities_lookup.end())
			return;

		entities_lookup.erase(it->first);
		delete it->second;
	}

	/**
	 * Remove a component for a specified entity from its system.
	 *
	 * \param e		Entity from which the component should be removed.
	 * \param hash	Hash of component type.
	 */
	void Remove(Entity *e, const std::type_index &type) {
		if(logic_systems.lookup.find(type) != logic_systems.lookup.end())
		{
			logic_systems.lookup[type]->Remove(e);
			return;
		}

		if(render_systems.lookup.find(type) != render_systems.lookup.end())
		{
			render_systems.lookup[type]->Remove(e);
			return;
		}
	}

	/**
	 * Tell all renderers to draw everything they've got.
	 */
	void FlushDraw(const GameTime& gametime) {
		std::vector<ISystem*>::iterator it;
		for(it = render_systems.prioritised.begin();
				it != render_systems.prioritised.end(); ++it)
		{
			/* TODO: Only call Manage for manually created systems. */
			(*it)->Manage(gametime);
		}
	}

	/**
	 * Add the sent system to the list of managed systems. The system is
	 * "handed over" - the manager will control management and deallocation
	 * of systems sent through.
	 *
	 * \param sys	System to hand over.
	 * \param hash	Hash of component type of the system.
	 */
	void AddSystem(ISystem* sys, const std::type_index &type) {
		sys->manager = this;
		logic_systems.Add(type, sys);
	}

	/**
	 * Checks if we have a system for a particular component type.
	 *
	 * \param hash	Type of component to check for.
	 * \return		True if system for component exists, otherwise false.
	 */
	bool HasSystem(const std::type_index &type) {
		return (logic_systems.lookup.find(type) != logic_systems.lookup.end()) ||
			(render_systems.lookup.find(type) != render_systems.lookup.end());
	}

	uint32_t GetComponentID(const std::type_index &type) {
		uint32_t id = component_bits[type];

		if(id != 0)
			return id;

		return (component_bits[type] = component_bit_count *= 2);
	}

	/**
	 * Adds a component to an entity. We require the hash of the component type
	 * to be sent through since we don't have the means to decipher the type.
	 *
	 * \param e		Entity to which the component is attached.
	 * \param c		Pointer to component to be copied to system.
	 * \param hash	Hash of the component type.
	 *
	 * \return		Pointer to component if created successfully, otherwise 0.
	 */
	Component* Add(Entity *e, const Component* c, const std::type_index &type) {
		std::unordered_map<std::type_index, ISystem*>::iterator it;

		it = logic_systems.lookup.find(type);
		if(it != logic_systems.lookup.end())
			return logic_systems.lookup[type]->CreateComponent(e, c);

		it = render_systems.lookup.find(type);
		if(it != render_systems.lookup.end())
			return render_systems.lookup[type]->CreateComponent(e, c);

		return NULL;
	}
};

#endif
