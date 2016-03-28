#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "movement.h"
#include "component.h"
#include "entity.h"
#include "../physics/physics_manager.h"
#include <btBulletDynamicsCommon.h>

struct PhysicsManager;
struct RigidBody : public Component
{
private:
	RigidBody() {}

public:
	btRigidBody *rigid_body;
	RigidBody(PhysicsManager *phys, Entity *e);
	~RigidBody();
};

#endif
