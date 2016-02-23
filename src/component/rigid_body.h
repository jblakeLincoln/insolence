#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "../component/movement_component.h"
#include "../physics/physics_manager.h"

#include <btBulletDynamicsCommon.h>

struct PhysicsManager;
struct RigidBody
{
private:
	RigidBody() {}
	PhysicsManager *physics_manager;
	btRigidBody *rigid_body;

public:
	RigidBody(PhysicsManager*, const MovementComponent&);
	~RigidBody();

	void SyncMovement(MovementComponent *m);
};

#endif
