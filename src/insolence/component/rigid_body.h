#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "movement.h"
#include "component.h"
#include "../game/entity.h"
#include "../physics/physics_manager.h"
#include <btBulletDynamicsCommon.h>

struct RigidBody : public Component
{
private:
public:
	btRigidBody *rigid_body;
	RigidBody() :
		rigid_body(0)
	{}
	~RigidBody();
};

#endif
