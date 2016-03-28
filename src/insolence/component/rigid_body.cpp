#include "rigid_body.h"

RigidBody::RigidBody(PhysicsManager *phys, Entity *e)
{
	rigid_body = phys->CreateRigidBody(e);
}

RigidBody::~RigidBody()
{
	delete rigid_body;
}
