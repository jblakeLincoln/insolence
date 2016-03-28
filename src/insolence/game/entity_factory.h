#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "entity.h"

#include "../physics/physics_manager.h"
#include "../systems/rigid_body.h"

#include "../render/render_manager_2d.h"
#include "../render/render_manager_3d.h"

#include "../component/mesh_renderable.h"
#include "../component/sprite_renderable.h"
#include "../component/text_renderable.h"

#include <map>
#include <typeinfo>
#include <typeindex>

struct EntityManager {
private:
	std::vector<Entity*> entities;

public:
	RenderManager2D *renderer_2d;
	RenderManager3D *renderer_3d;
	PhysicsManager *physics;

	EntityManager() {
		renderer_2d = new RenderManager2D();
		renderer_3d = new RenderManager3D();
		physics = PhysicsManager::Create(glm::vec3(0.f, -9.f, 0.f));
	}

	Entity *CreateEntity()
	{
		Entity *e = new Entity();
		entities.push_back(e);

		return e;
	}

	void ManageEntities()
	{
		physics->StepSimulation(1.f/60.f);

		for(int i = 0; i < entities.size(); ++i)
			for(int j = 0; j < entities[i]->NumComponents(); ++j)
				ManageComponent(entities[i], entities[i]->GetComponentByNum(j));

	}

	void FlushDraw()
	{
		renderer_2d->Flush();
		renderer_3d->Flush();
	}

	/**
	 * Manage individual insolence components.
	 * This is some bullshit I never should have gotten into.
	 * Hopefully this can be replaced with systems that get passed the
	 * GameWorld that can just touch the renderer themselves and be called
	 * through a virtual function of the Entity. We'll get round to it.
	 *
	 * If statement should be non-pointer, because it's comparing to a
	 * dereferenced type. Actual call to Manage should cast to a pointer.
	 *
	 * \param e	Entity to manage.
	 * \param c	Component to handle.
	 */
	template<typename T>
	void ManageComponent(Entity *e, T *c)
	{
		const std::type_info& info = typeid(*c);

		/*e*/if(info == typeid(MeshRenderable))
			Manage(e, (MeshRenderable*)c);
		else if(info == typeid(SpriteRenderable))
			Manage(e, (SpriteRenderable*)c);
		else if(info == typeid(RigidBody))
			Manage(e, (RigidBody*)c);
	}

	void Manage(Entity*, MeshRenderable*);
	void Manage(Entity*, SpriteRenderable*);
	void Manage(Entity*, TextRenderable*);
	void Manage(Entity*, RigidBody*);
};

#endif
