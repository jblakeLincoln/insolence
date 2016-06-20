#ifdef INSOLENCE_LINKS_BULLET
#ifndef SYSTEM_RIGID_BODY_H
#define SYSTEM_RIGID_BODY_H

#include "../insolence_dll.h"

#include "../component/rigid_body.h"
#include <btBulletDynamicsCommon.h>

struct PhysicsManager;
struct INSOLENCE_API RigidBodySystem : System<RigidBody> {
private:
	PhysicsManager *physics_manager;

public:
	RigidBodySystem(PhysicsManager*);
	void Manage(const GameTime&);
	void RemoveOverride(Entity*, RigidBody*);
	void EndCreation(Entity*, RigidBody*);
};

#endif
#endif
