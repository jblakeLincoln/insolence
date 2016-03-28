#ifndef SYSTEM_RIGID_BODY_H
#define SYSTEM_RIGID_BODY_H

#include "../component/rigid_body.h"

void SyncMovementToRigidBody(Entity *e)
{
	Movement *m = e->Get<Movement>();
	RigidBody *r = e->Get<RigidBody>();

	if(m == NULL || r == NULL)
		return;

	glm::vec3 position = glm::vec3(
			r->rigid_body->getCenterOfMassPosition().x(),
			r->rigid_body->getCenterOfMassPosition().y(),
			r->rigid_body->getCenterOfMassPosition().z());

	btQuaternion o = r->rigid_body->getOrientation();
	glm::quat rotation(-o.getZ(), o.getY(), -o.getX(), o.getW());

	m->Move(-m->GetPosition());
	m->Move(position);
	m->SetOrientation(rotation);
}

#endif
