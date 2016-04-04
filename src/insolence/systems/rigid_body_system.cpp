#include "rigid_body_system.h"

RigidBodySystem::RigidBodySystem(PhysicsManager *p)
:	physics_manager(p)
{}

void RigidBodySystem::Manage(const GameTime& gametime)
{
	for(it = components.begin(); it != components.end(); ++it)
	{
		Entity *e = it->first;
		RigidBody *r = &it->second;
		Movement *m = e->Get<Movement>();

		if(m == NULL || r == NULL)
			return;

		if(r->rigid_body == NULL)
			r->rigid_body = physics_manager->CreateRigidBody(m);

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
}
