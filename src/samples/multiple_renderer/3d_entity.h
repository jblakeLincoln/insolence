#include "../../component/entity.h"

struct TestEntity : Entity
{
	Texture *tex;
	Mesh *mesh;
	RenderManager3D *renderer;
	MovementComponent move;

	TestEntity(Mesh *m, Texture *t, const glm::vec3 &pos, RenderManager3D *r)
	{
		mesh = m;
		tex = t;
		renderer = r;
		move.Move(pos);
	}

	virtual void Update()
	{
		move.Rotate(0.07f, glm::vec3(1.f, 0.f, 0.f));
	}

	virtual void Draw()
	{
		renderer->Add(mesh, tex, move.GetModelMatrix());
	}
};
