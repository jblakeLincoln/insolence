#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <btBulletDynamicsCommon.h>
#include "../component/rigid_body.h"

struct PhysicsManager {
private:
	PhysicsManager();
	PhysicsManager(const glm::vec3 &gravity);

	btDynamicsWorld *dynamics_world;
	btDefaultCollisionConfiguration *collision_config;
	btAlignedObjectArray<btCollisionShape*> collision_shapes;
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
	 */
	btRigidBody* CreateRigidBody(const MovementComponent& ms);

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
