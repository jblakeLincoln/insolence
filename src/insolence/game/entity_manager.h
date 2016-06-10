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
//#include "../physics/physics_manager.h"
#include "../render/render_manager_2d.h"
#include "../render/render_manager_3d.h"
#include "../systems/mesh_renderable_system.h"
#include "../systems/sprite_renderable_system.h"
#include "../systems/text_renderable_system.h"
//#include "../systems/rigid_body_system.h"

#include <cstddef>
#include <typeindex>
#include <unordered_map>

struct Entity;
struct INSOLENCE_API EntityManager {
private:
	typedef std::unordered_map<std::type_index, ISystem*>::iterator sys_iterator;

public:
	std::vector<Entity*> entities;
	std::unordered_map<std::type_index, ISystem*> logic_systems;
	std::unordered_map<std::type_index, ISystem*> render_systems;

	RenderManager2D *renderer_2d;
	RenderManager3D *renderer_3d;
//	PhysicsManager *physics_manager;

	/**
	 * Set up our manager. By default we want 2D and 3D renderers available,
	 * as well as a physics system.
	 */
	EntityManager()
	{
		renderer_2d = new RenderManager2D();
		renderer_3d = new RenderManager3D();
//		physics_manager = PhysicsManager::Create(glm::vec3(0.f, -9.8f, 0.f));

		/* Default systems for this EntityManager. */
		AddSystemContainer<Transform>();
		AddRenderSystem<SpriteRenderableSystem>(renderer_2d);
		AddRenderSystem<TextRenderableSystem>(renderer_2d);
		AddRenderSystem<MeshRenderableSystem>(renderer_3d);
//		AddSystem<RigidBodySystem>(physics_manager);
	}

	/**
	 * Creates a container system that will simply store all types of
	 * components. Not entirely necessary to call since if a component is made
	 * for a system that doesn't exist, one of these will be automatically
	 * created.
	 */
	template<typename T>
	void AddSystemContainer() {
		const std::type_index type = typeid(T);
		if(logic_systems.find(type) != logic_systems.end())
			return;

		logic_systems[type] = new System<T>();
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
	void AddSystem(std::unordered_map<std::type_index, ISystem*> &systems,
			Args... args) {
		if(systems.find(T::GetType()) != systems.end())
			return;

		systems[T::GetType()] = new T(args...);
	}

	/**
	 * Creates an entity owned by this manager and returns a reference.
	 */
	Entity* CreateEntity()
	{
		entities.push_back(new Entity(this));
		return entities.back();
	}

	/**
	 * Getting rid of our manager means clearing out all related entities and
	 * their attached components.
	 */
	~EntityManager()
	{
		for(sys_iterator it = logic_systems.begin();
				it != logic_systems.end(); ++it)
		{
			delete it->second;
		}

		for(sys_iterator it = render_systems.begin();
				it != render_systems.end(); ++it)
		{
			delete it->second;
		}

		delete renderer_2d;
		delete renderer_3d;
//		delete physics_manager;
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
//		physics_manager->StepSimulation(1.f/60.f);

		for(sys_iterator it = logic_systems.begin();
				it != logic_systems.end(); ++it)
		{
			/* TODO: Only call Manage for manually created systems. */
			it->second->Manage(gametime);
		}
	}

	void Remove(Entity *e)
	{
		if(entities.size() == 0)
			return;

		entities.erase(std::remove(entities.begin(), entities.end(),
				e), entities.end());
	}

	/**
	 * Remove a component for a specified entity from its system.
	 *
	 * \param e		Entity from which the component should be removed.
	 * \param hash	Hash of component type.
	 */
	void Remove(Entity *e, const std::type_index &type) {
		if(logic_systems.find(type) != logic_systems.end())
		{
			logic_systems[type]->Remove(e);
			return;
		}

		if(render_systems.find(type) != render_systems.end())
		{
			render_systems[type]->Remove(e);
			return;
		}
	}

	/**
	 * Tell all renderers to draw everything they've got.
	 */
	void FlushDraw(const GameTime& gametime) {
		for(sys_iterator it = render_systems.begin();
				it != render_systems.end(); ++it)
		{
			/* TODO: Only call Manage for manually created systems. */
			it->second->Manage(gametime);
		}

		renderer_2d->Flush();
		renderer_3d->Flush();
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
		logic_systems[type] = sys;
	}

	/**
	 * Checks if we have a system for a particular component type.
	 *
	 * \param hash	Type of component to check for.
	 * \return		True if system for component exists, otherwise false.
	 */
	bool HasSystem(const std::type_index &type) {
		return (logic_systems.find(type) != logic_systems.end()) ||
			(render_systems.find(type) != render_systems.end());
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
		sys_iterator it = logic_systems.find(type);
		if(it != logic_systems.end())
			return logic_systems[type]->CreateComponent(e, c);

		it = render_systems.find(type);
		if(it != render_systems.end())
			return render_systems[type]->CreateComponent(e, c);
	}
};

#endif
