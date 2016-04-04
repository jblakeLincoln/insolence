#ifndef SYSTEM_RIGID_BODY_H
#define SYSTEM_RIGID_BODY_H

#include "../game/entity.h"
#include "../component/movement.h"
#include "../component/rigid_body.h"
#include "../physics/physics_manager.h"
#include <btBulletDynamicsCommon.h>

struct RigidBodySystem : System<RigidBody> {
private:
	PhysicsManager *physics_manager;

public:
	RigidBodySystem(PhysicsManager*);
	void Manage(const GameTime&);
};

#endif
