#include "physics_manager.h"

PhysicsManager::PhysicsManager(const glm::vec3& gravity)
{
	collision_config = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_config);
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	dynamics_world = new btDiscreteDynamicsWorld(dispatcher,
			broadphase,
			solver,
			collision_config);

	dynamics_world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

PhysicsManager::~PhysicsManager()
{
	for (int i = dynamics_world->getNumCollisionObjects()-1; i >= 0; --i)
	{
		btCollisionObject *obj = dynamics_world->getCollisionObjectArray()[i];
		btRigidBody *body = btRigidBody::upcast(obj);

		if (body && body->getMotionState())
			delete body->getMotionState();

		dynamics_world->removeCollisionObject(obj);

		delete obj;
	}

	for (int i = 0; i < collision_shapes.size(); ++i)
		delete collision_shapes[i];

	collision_shapes.clear();

	delete dynamics_world;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collision_config;
}

void PhysicsManager::StepSimulation(float step)
{
	dynamics_world->stepSimulation(step);
}

btRigidBody* PhysicsManager::CreateRigidBody(Entity *e)
{
	Movement* m = e->Get<Movement>();

	if(m == NULL)
		m = new Movement();

	btCollisionShape *box_shape = new btBoxShape(btVector3
			(m->GetScale().x, m->GetScale().y, m->GetScale().z));

	collision_shapes.push_back(box_shape);

	/* Set body values. */
	btScalar mass = 1.f;
	btVector3 local_inertia = btVector3(0.f, 0.f, 0.f);
	box_shape->calculateLocalInertia(mass, local_inertia);

	/* Set body positions */
	btTransform box_transform;
	box_transform.setIdentity();
	box_transform.setOrigin(btVector3(m->GetPosition().x,
				m->GetPosition().y, m->GetPosition().z));

	btDefaultMotionState *box_motion_state =
		new btDefaultMotionState(box_transform);

	/* Finish up body creation. */
	btRigidBody::btRigidBodyConstructionInfo box_rb_info(mass,
			box_motion_state, box_shape, local_inertia);

	box_rb_info.m_friction = 1.f;
	btRigidBody *rigid_body = new btRigidBody(box_rb_info);

	if(!rigid_body)
	{
		// log error.
	}

	dynamics_world->addRigidBody(rigid_body);

	return rigid_body;
}

