#ifdef INSOLENCE_LINKS_BULLET
#include "rigid_body_system.h"

RigidBodySystem::RigidBodySystem(PhysicsManager *p)
:	physics_manager(p)
{}

void RigidBodySystem::EndCreation(Entity *e, RigidBody *c)
{
	Transform *m = e->Get<Transform>();

	c->manager = physics_manager;

	if(c->rigid_body == NULL && m != NULL)
		c->rigid_body = physics_manager->CreateRigidBody(m);
}

void RigidBodySystem::RemoveOverride(Entity *e, RigidBody *c)
{
	if(physics_manager != NULL && c->rigid_body != NULL)
		c->manager->RemoveRigidBody(c->rigid_body);
}

void RigidBodySystem::Manage(const GameTime& gametime)
{
	for(it = components.begin(); it != components.end(); ++it)
	{
		Entity *e = it->first;
		RigidBody *r = &it->second;
		Transform *m = e->Get<Transform>();

		if(m == NULL || r == NULL || r->rigid_body == NULL)
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
}

#endif
