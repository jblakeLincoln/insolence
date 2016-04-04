#ifndef COMPONENT_RIGID_BODY_H
#define COMPONENT_RIGID_BODY_H

#include "component.h"
#include <btBulletDynamicsCommon.h>

struct RigidBody : public Component
{
private:
public:
	btRigidBody *rigid_body;
	RigidBody() :
		rigid_body(0)
	{
	}
	~RigidBody() {
	//	delete rigid_body;

	}
};

#endif
