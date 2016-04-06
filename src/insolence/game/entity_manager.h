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

#include "gametime.h"
#include "../physics/physics_manager.h"
#include "../render/render_manager_2d.h"
#include "../render/render_manager_3d.h"
#include "../systems/mesh_renderable_system.h"
#include "../systems/sprite_renderable_system.h"
#include "../systems/text_renderable_system.h"
#include "../systems/rigid_body_system.h"

#include <cstddef>
#include <typeindex>
#include <unordered_map>

struct Entity;
struct EntityManager {
private:
	typedef std::unordered_map<size_t, ISystem*>::iterator sys_iterator;

public:
	std::vector<Entity*> entities;
	std::unordered_map<size_t, ISystem*> systems;

	RenderManager2D *renderer_2d;
	RenderManager3D *renderer_3d;
	PhysicsManager *physics_manager;

	/**
	 * Set up our manager. By default we want 2D and 3D renderers available,
	 * as well as a physics system.
	 */
	EntityManager()
	{
		renderer_2d = new RenderManager2D();
		renderer_3d = new RenderManager3D();
		physics_manager = PhysicsManager::Create(glm::vec3(0.f, -9.8f, 0.f));

		systems[typeid(Movement).hash_code()] = new System<Movement>();

		systems[typeid(SpriteRenderable).hash_code()] =
			new SpriteRenderableSystem(renderer_2d);

		systems[typeid(TextRenderable).hash_code()] =
			new TextRenderableSystem(renderer_2d);

		systems[typeid(MeshRenderable).hash_code()] =
			new MeshRenderableSystem(renderer_3d);

		systems[typeid(RigidBody).hash_code()] =
			new RigidBodySystem(physics_manager);
	}

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
		for(sys_iterator it = systems.begin();
				it != systems.end(); ++it)
		{
			delete it->second;
		}

		delete renderer_2d;
		delete renderer_3d;
		delete physics_manager;
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
		physics_manager->StepSimulation(1.f/60.f);

		for(sys_iterator it = systems.begin();
				it != systems.end(); ++it)
		{
			/* TODO: Only call Manage for manually created systems. */
			it->second->Manage(gametime);
		}
	}

	/**
	 * Remove a component for a specified entity from its system.
	 *
	 * \param e		Entity from which the component should be removed.
	 * \param hash	Hash of component type.
	 */
	void Remove(Entity *e, size_t hash) {
		systems[hash]->Remove(e);
	}

	/**
	 * Tell all renderers to draw everything they've got.
	 */
	void FlushDraw()
	{
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
	void AddSystem(ISystem* sys, size_t hash)
	{
		systems[hash] = sys;
	}

	/**
	 * Checks if we have a system for a particular component type.
	 *
	 * \param hash	Type of component to check for.
	 * \return		True if system for component exists, otherwise false.
	 */
	bool HasSystem(size_t hash)
	{
		return systems.find(hash) != systems.end();
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
	Component* Add(Entity *e, const Component* c, size_t hash)
	{
		sys_iterator it = systems.find(hash);

		if(it == systems.end())
			return 0;

		return systems[hash]->CreateComponent(e, c);
	}
};

#endif
