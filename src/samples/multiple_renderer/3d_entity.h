#include "../../component/entity.h"
#include "../../physics/physics_manager.h"
#include "../../component/rigid_body.h"

struct TestEntity : Entity
{
	Texture *tex;
	Mesh *mesh;
	RenderManager3D *renderer;
	MovementComponent move;
	RigidBody *rigid_body;

	TestEntity(PhysicsManager *p, Mesh *m, Texture *t, const glm::vec3 &pos,
			RenderManager3D *r)
	{
		mesh = m;
		tex = t;
		renderer = r;
		move.Move(pos);
		rigid_body = new RigidBody(p, move);
	}

	virtual void Update()
	{
		rigid_body->SyncMovement(&move);
	}

	virtual void Draw()
	{
		renderer->Add(mesh, tex, move.GetModelMatrix());
	}
};
