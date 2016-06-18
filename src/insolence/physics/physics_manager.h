#ifdef INSOLENCE_LINK_BULLET
#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "../insolence_dll.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <btBulletDynamicsCommon.h>

struct RigidBody;
struct INSOLENCE_API PhysicsManager {
	friend RigidBody;
private:
	PhysicsManager() {}

	btDynamicsWorld *dynamics_world;
	btDefaultCollisionConfiguration *collision_config;
	btAlignedObjectArray<btCollisionShape*> collision_shapes;
	btAlignedObjectArray<btPoint2PointConstraint*> constraints;
	btBroadphaseInterface *broadphase;
	btCollisionDispatcher *dispatcher;
	btConstraintSolver *solver;

public:
	PhysicsManager(const glm::vec3 &gravity);
	~PhysicsManager();

	void RemoveRigidBody(btRigidBody *r);

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
	btRigidBody* CreateRigidBody(Transform *m);

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
	//btPoint2PointConstraint* CreateConstraint(Entity *e1, Entity *e2,
	//		const glm::vec3& c1, const glm::vec3& c2);

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
#endif
