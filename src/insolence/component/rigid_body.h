#ifdef INSOLENCE_LINK_BULLET
#ifndef COMPONENT_RIGID_BODY_H
#define COMPONENT_RIGID_BODY_H

#include "../insolence_dll.h"

#include "component.h"
#include "../physics/physics_manager.h"
#include <btBulletDynamicsCommon.h>

struct PhysicsManager;
struct RigidBodySystem;
struct INSOLENCE_API RigidBody : public Component
{
	friend PhysicsManager;
	friend RigidBodySystem;
private:
	PhysicsManager *manager;
	btRigidBody *rigid_body;
public:
	RigidBody() :
		rigid_body(0)
	{
	}
	~RigidBody() {
	//	delete rigid_body;

	}

	/**
	 * Activate an entity.
	 */
	void Activate();

	/**
	 * Deactivate an entity.
	 */
	void Deactivate();

	/**
	 * Change the mass of an entity. 0 makes the entity static.
	 *
	 * \param e		Entity to affect.
	 * \param mass	Mass to apply.
	 */
	void SetMass(float);

	/**
	 * Check if the rigid body of an entity is sleeping.
	 *
	 * \return	True if sleeping (btRigidBody state ISLAND_SLEEPING),
	 *			otherwise false.
	 */
	bool IsSleeping();

	/**
	 * Constrain the rigid body to move only on the X and Y axis, and only
	 * rotate on the Y axis.
	 */
	void Make2D();
};

#endif
#endif
