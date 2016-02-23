#include "rigid_body.h"

RigidBody::RigidBody(PhysicsManager *phys, const MovementComponent& m)
{
	physics_manager = phys;
	rigid_body = phys->CreateRigidBody(m);
}

RigidBody::~RigidBody()
{
	delete rigid_body;
}

void RigidBody::SyncMovement(MovementComponent *m)
{
	glm::vec3 position = glm::vec3(
			rigid_body->getCenterOfMassPosition().x(),
			rigid_body->getCenterOfMassPosition().y(),
			rigid_body->getCenterOfMassPosition().z());

	btQuaternion o = rigid_body->getOrientation();
	glm::quat rotation(-o.getZ(), o.getY(), -o.getX(), o.getW());

	m->Move(-m->GetPosition());
	m->Move(position);
	m->SetOrientation(rotation);
	m->SetScale(glm::vec3(1.f));
}
