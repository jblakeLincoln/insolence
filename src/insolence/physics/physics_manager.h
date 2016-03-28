#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <btBulletDynamicsCommon.h>
#include "../component/rigid_body.h"

struct PhysicsManager {
public:
	PhysicsManager();
	PhysicsManager(const glm::vec3 &gravity);

	btDynamicsWorld *dynamics_world;
	btDefaultCollisionConfiguration *collision_config;
	btAlignedObjectArray<btCollisionShape*> collision_shapes;
	btAlignedObjectArray<btPoint2PointConstraint*> constraints;
	btBroadphaseInterface *broadphase;
	btCollisionDispatcher *dispatcher;
	btConstraintSolver *solver;

public:
	~PhysicsManager();

	/**
	 * Progress the simulation.
	 *
	 * \param float	Time to step physics by (e.g. 1.0/60.0).
	 */
	void StepSimulation(float);

	/**
	 * Create a rigid body. Currently creates a box shape at the
	 * position specified by the movement component.
	 *
	 * \return	Pointer to btRigidBody on success, or NULL on fail.
	 */
	btRigidBody* CreateRigidBody(Entity *e);

	/**
	 * Change the mass of an entity. 0 makes the entity static.
	 *
	 * \param e		Entity to affect.
	 * \param mass	Mass to apply.
	 */
	void ChangeMass(Entity *e, float mass);

	/**
	 * Create a constraint between two entities. The constraint is tracked
	 * by the manager and will be deleted at manager destruction.
	 *
	 * \param e1	Entity to affect.
	 * \param e2	Other entity to affect.
	 * \param c1	Pivot A.
	 * \param c2	Pivot B.
	 * \return		Pointer to the created constraint, or NULL on failure.
	 */
	btPoint2PointConstraint* CreateConstraint(Entity *e1, Entity *e2,
			const glm::vec3& c1, const glm::vec3& c2);

	/**
	 * Activate an entity.
	 */
	void Activate(Entity *e);

	/**
	 * Deactivate an entity.
	 */
	void Deactivate(Entity *e);

	/**
	 * Constrain the rigid body to move only on the X and Y axis, and only
	 * rotate on the Y axis.
	 */
	void Make2D(Entity *e);

	/**
	 * Check if the rigid body of an entity is sleeping.
	 *
	 * \return	True if sleeping (btRigidBody state ISLAND_SLEEPING),
	 *			otherwise false.
	 */
	bool IsSleeping(Entity *e);

	/**
	 * Create a physics manager - has its own gravity and controls a set of
	 * rigid bodies.
	 *
	 * \param gravity	Strength of gravity.
	 */
	static PhysicsManager* Create(const glm::vec3 &gravity)
	{
		PhysicsManager *out = new PhysicsManager(gravity);
		return out;
	}

};

#endif
