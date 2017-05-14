#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "../component/transform.h"
#include "system.h"
#include "../game/gametime.h"

#include <bitset>
#include <typeindex>
#include <unordered_map>
#include <vector>

#ifndef INSOLENCE_INSOLENCE_MAX_COMPONENTS
	#define INSOLENCE_MAX_COMPONENTS 1024
#endif

/**
 * The EntityManager is in charge of holding all entities and associated
 * systems, which store the components attached to entities.
 *
 * The "entities" and "systems" are internal interfaces for managing the
 * lifetimes of and interactions with entities and systems, some of which are
 * made public through EntityManager functions.
 */
struct Entity;
struct INSOLENCE_API EntityManager {
friend Entity;
protected:
	/**
	 * The EntityManager holds an instance of a system for each type that gets
	 * attached to an entity at any point during the runtime of the application.
	 * Systems are created when first required (e.g. an entity adding a
	 * component of a new type for the first time).
	 */
	struct INSOLENCE_API Systems {
	private:
		/**
		 * Dynamically allocated IDs to components as systems are created.
		 * Store systems as generic ISystem pointers, and allow looking up the
		 * component ID through the component type.
		 *
		 * We keep a separate vector that is sorted by priority, pointing to
		 * systems deemed "owned" by the systems array - though this doesn't
		 * actually come into play at any point.
		 */
		uint64_t component_ids = 0;
		ISystem* systems[INSOLENCE_MAX_COMPONENTS];
		std::unordered_map<std::type_index, uint64_t> id_lookup;
		std::vector<ISystem*> systems_prioritised;

	public:
		Systems();
		~Systems();

		/**
		 * Called on EntityManager Manage to call each System update.
		 */
		void Manage(const GameTime&);

		/**
		 * Store an ISystem pointer for a specific component type.
		 */
		void Add(EntityManager *mgr, const std::type_index&, ISystem*);

		/**
		 * Check if a system exists for a specific component type.
		 */
		bool Has(const std::type_index&);

		/** Retrieve systems by:
		 *    - component type
		 *    - component ID
		 */
		ISystem* Get(const std::type_index &type);
		ISystem* Get(uint64_t id);

		/**
		 * Retrieve a component ID for a component type.
		 */
		uint64_t GetComponentID(const std::type_index &type);
	} systems;

	struct INSOLENCE_API Entities {
	private:
		/**
		 * Allocate entity IDs as they are requested. Heap allocate entities
		 * so that pointers to them can remain valid, even though it's safest
		 * to look them up by ID to ensure they exist.
		 */
		uint64_t entity_ids = 0;
		std::unordered_map<uint64_t, Entity*> entities;

	public:
		~Entities();

		/**
		 * Request a new Entity from the manager.
		 */
		Entity* Create(EntityManager*);

		/**
		 * Retrieve an entity by its ID.
		 */
		Entity* Get(uint64_t);

		/**
		 * Destroy a provided entity and all attached components.
		 */
		void Destroy(Systems&, Entity *e);
	} entities;

	/**
	 * Retrieve a system for a specified type. If it doesn't exist, it is
	 * created.
	 */
	template<typename T>
	System<T>* GetSystem();

public:
	EntityManager() {}
	~EntityManager() {}

	/**
	 * Create and return a single entity.
	 */
	Entity *CreateEntity() {
		return entities.Create(this);
	}

	/**
	 * Retrieve an entity by its ID.
	 */
	Entity *GetByID(uint64_t id) {
		return entities.Get(id);
	}

	/**
	 * Initialise a list of entity pointers. Useful with curly brace syntax.
	 */
	void CreateEntities(std::vector<Entity**> e) {
		for(size_t i = 0; i < e.size(); ++i)
			*(e[i]) = entities.Create(this);
	}

	/**
	 * Construct a new system. Used when the default constructor for a system
	 * is unavailable for the system to be created automatically.
	 */
	template<typename T, typename ... Args>
	void AddSystem(const Args &...args);

	/**
	 * Calls the Update function for all systems.
	 */
	void Manage(const GameTime &gametime) {
		systems.Manage(gametime);
	}
};

/**
 * An Entity is a object that owns components - sets of data that are operated.
 * The entity is not a specific thing, but is a general container which is
 * composed of different properties.
 */
struct INSOLENCE_API Entity {
friend EntityManager;
private:
	/**
	 * Destruction happens through the Destroy function, handled by the
	 * manager.
	 */
	~Entity() {}

	/**
	 * Stores a pointer to the EntityManager to which it belongs, the ID by
	 * which the manager and systems refer to it, and a bitset of attached
	 * components for speedy lookup of what the entity owns.
	 */
	EntityManager *mgr;
	uint64_t id;
	std::bitset<INSOLENCE_MAX_COMPONENTS> components;

protected:
	/**
	 * Construction is performed by the EntityManager, when it passes
	 * through a pointer to itself.
	 */
	Entity(EntityManager *mgr_in)
		: mgr(mgr_in)
	{}

	/**
	 * Check the entity for having a provided list of components, by
	 * iterating over types in a tuple, provided by the public Has<>().
	 */
	template<std::size_t I = 0, typename... Type>
	inline typename std::enable_if<I < sizeof...(Type), bool>::type
	HasComponent(const std::tuple<Type...> &t);

	/**
	 * Go to the next type in the HasComponent tuple.
	 */
	template<std::size_t I = 0, typename... Type>
	inline typename std::enable_if<I == sizeof...(Type), bool>::type
	HasComponent(const std::tuple<Type...> &t);

public:
	uint64_t GetID() {
		return id;
	}

	/**
	 * Construct a new component to attach to the entity, constructed in
	 * place in the relevant system with the constructor arguments
	 * provided.
	 *
	 * Returns the newly constructed component.
	 */
	template<typename T, typename... Args>
	T* Add(const Args &...args);

	/**
	 * Retrieve a component type for the entity. Returns a component if it
	 * exists, otherwise nullptr.
	 */
	template<typename T>
	T* Get();

	/**
	 * Removes a selected type of component, or does nothing if the
	 * component type is not attached.
	 */
	template<typename T>
	void Remove();

	/**
	 * Check if the system has components of the specified types. True if
	 * the entity holds all provided types, otherwise. false.
	 *
	 * When checking typeid, we __always__ refer to the type of a pointer
	 * of a component, rather than the components themselves, which means
	 * when dealing with multiple types and creating a tuple, we don't
	 * construct components.
	 */
	template<typename First, typename... Types>
	bool Has();

	/**
	 * Destroy the entity, having it removed from the manager and all
	 * attached components destroyed.
	 */
	void Destroy();
};

/*************************************
 * EntityManager functions
 *************************************/
template<typename T>
System<T>* EntityManager::GetSystem()
{
	const std::type_index type = typeid(T*);

	if(systems.Has(type) == false)
		systems.Add(this, type, new System<T>());

	return static_cast<System<T>*>(systems.Get(type));
}

template<typename T, typename ... Args>
void EntityManager::AddSystem(const Args &...args)
{
	const std::type_index type = T::GetType();

	if(systems.Has(type) == true)
		return;

	systems.Add(this, type, new T(args...));
}

/**************************************
 * Entity functions
 **************************************/
template<typename T, typename... Args>
T* Entity::Add(const Args &...args) {
	uint64_t cid = mgr->systems.GetComponentID(typeid(T*));

	if(components[cid] == true)
		return mgr->GetSystem<T>()->GetComponent(this);

	T *ret = mgr->GetSystem<T>()->AddComponent(this, args...);
	components.set(cid, true);
	return ret;
}

template<typename T>
T* Entity::Get() {
	uint64_t cid = mgr->systems.GetComponentID(typeid(T*));
	if(components[cid] == false)
		return nullptr;

	return mgr->GetSystem<T>()->GetComponent(this);
}

template<typename T>
void Entity::Remove() {
	mgr->GetSystem<T>()->RemoveComponent(this);
	components.set(mgr->systems.GetComponentID(typeid(T*)), false);
}

template<std::size_t I, typename... Type>
inline typename std::enable_if<I < sizeof...(Type), bool>::type
Entity::HasComponent(const std::tuple<Type...> &t) {
	uint64_t cid = mgr->systems.GetComponentID(typeid(std::get<I>(t)));
	bool ret = components[cid];

	if(ret == true)
		ret = HasComponent<I + 1, Type...>(t);

	return ret;
}

template<std::size_t I, typename... Type>
inline typename std::enable_if<I == sizeof...(Type), bool>::type
Entity::HasComponent(const std::tuple<Type...> &t) {
	return true;
}

template<typename First, typename... Types>
bool Entity::Has() {
	uint64_t cid = mgr->systems.GetComponentID(typeid(First*));
	bool ret = components[cid];

	if(ret == false || sizeof...(Types) == 0)
		return ret;

	return HasComponent(std::tuple<Types*...>());
}

#endif
