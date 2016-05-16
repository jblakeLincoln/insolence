#ifdef INSOLENCE_LINKS_BULLET
#include "rigid_body.h"

void RigidBody::Activate()
{
	if(rigid_body == NULL)
		return;

	rigid_body->setActivationState(ACTIVE_TAG);
}

void RigidBody::Deactivate()
{
	SetMass(0);
}

void RigidBody::SetMass(float mass)
{
	if(rigid_body == NULL || manager == NULL)
		return;

	manager->dynamics_world->removeRigidBody(rigid_body);
	btVector3 inertia = btVector3(0.f, 0.f, 0.f);

	rigid_body->getCollisionShape()->calculateLocalInertia(mass, inertia);
	rigid_body->setMassProps(mass, inertia);
	manager->dynamics_world->addRigidBody(rigid_body);
}

bool RigidBody::IsSleeping()
{
	if(rigid_body == NULL)
		return true;

	switch(rigid_body->getActivationState())
	{
		case ISLAND_SLEEPING:
			return false;
		default:
			return true;
	}
}

void RigidBody::Make2D()
{
	if(rigid_body == NULL)
		return;

	rigid_body->setLinearFactor(btVector3(1, 1, 0));
	rigid_body->setAngularFactor(btVector3(0, 0, 1));
}

#endif
