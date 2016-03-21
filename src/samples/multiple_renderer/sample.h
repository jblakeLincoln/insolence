#include <stdio.h>
#include <map>
#include <vector>

#include "../../game/game_world.h"

#include "../../render/render_manager_3d.h"
#include "../../render/render_manager_2d.h"
#include "../../component/animation.h"
#include "../../component/camera.h"
#include "../../component/colour.h"
#include "../../component/entity.h"
#include "../../component/font.h"
#include "../../component/material.h"
#include "../../systems/animation.h"
#include "../../systems/rigid_body.h"
#include "../../physics/physics_manager.h"

#define ENTITY_NUM 20
#define ENTITY_SPACING 0.9

struct SampleMultipleRenderer : BaseGameWorld
{
	SampleMultipleRenderer() {}
private:
	PhysicsManager *phys;
	RenderManager2D *renderer_2d;
	RenderManager3D *renderer_3d;
	Camera *camera;

	Mesh *mesh_crate;
	Mesh *mesh_tri;
	Material *material_crate;
	Material *material_white;
	Texture *tex_white;
	glm::vec4 colours[4];
	Texture *tex_mega;

	std::vector<Entity*> entities_3d;
	Entity *entity_2d;

	Entity *plane;

	Font *f;

	void Initialise()
	{
		/* Init. */
		Texture::Init();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		renderer_3d = new RenderManager3D();
		renderer_2d = new RenderManager2D();
		phys = PhysicsManager::Create(glm::vec3(0.f, -9.8f, 0.f));

		/* Load assets. */
		colours[0]	=	glm::vec4(1.f, 0.3f, 0.9f, 1.f);
		colours[1]	=	glm::vec4(0.9f, 0.8f, 0.3f, 1.f);
		colours[2]	=	glm::vec4(0.1f, 0.7f, 0.8f, 1.f);
		colours[3]	=	glm::vec4(0.f, 0.7f, 0.f, 1.f);
		tex_white	=	Texture::LoadColour(glm::vec4(1.f));
		tex_mega	=	Texture::LoadFile("assets/mega_run.png");

		mesh_crate = Mesh::LoadFile("assets/crate.obj",
				renderer_3d->shader_program);
		mesh_tri = Mesh::LoadFile("assets/tri.obj",
				renderer_3d->shader_program);

		tex_mega->owner = 0;
		tex_white->owner = 0;
		mesh_tri->owner = 0;
		mesh_crate->owner = 0;

		entity_2d = CreateEntity2D(tex_mega, glm::vec3(20.f, 2.f, 1.f));

		material_crate = new Material();
		material_crate->owner = 0;
		material_crate->diffuse = Texture::LoadFile("assets/crate.png");
		material_crate->normal = Texture::LoadFile("assets/crate_n.png");

		material_white = new Material();
		material_white->owner = 0;
		material_white->diffuse = tex_white;
		material_white->normal = material_crate->normal;

		/* Set up a load of test entities. */

		for(int i = 0; i < ENTITY_NUM; ++i)
		{
			Mesh *m = i % 2 ? mesh_crate : mesh_crate; //mesh_tri;

			entities_3d.push_back(CreateEntity3D(
						glm::vec3(i * ENTITY_SPACING, i*0.1f, i*0.5f),
						glm::vec3(1.f, 1.f, 1.f),
						colours[i % 3],
						m, material_crate, phys));
			phys->Make2D(entities_3d[i]);
		}

		plane = CreateEntity3D(glm::vec3(-5.f), glm::vec3(50.f, 1.f, 50.f),
				colours[3], mesh_crate, material_white, phys);
		plane->Get<RigidBody>()->rigid_body->setMassProps(0.f,
				btVector3(0.f, 0.f, 0.f));
		plane->Get<Colour>()->colour = glm::vec4(0.6f, 0.1f, 0.2f, 1.f);

		phys->ChangeMass(entities_3d[0], 2.f);

		/*
		phys->CreateConstraint(
				entities_3d[0], entities_3d[1],
				glm::vec3(5.f), glm::vec3(5.f));
				*/
		camera = new Camera();
		camera->pos.Translate(glm::vec3(0.f, 0.f, 16.f));
		camera->pos.Rotate(10.f, glm::vec3(0.f, 1.f, 0.f));

		f = Font::Load("/usr/share/fonts/truetype/tlwg/Kinnari-Bold.ttf", 172);
		//phys->Deactivate(entities_3d[1]);
	}

	void Update(const GameTime& gametime)
	{
		phys->StepSimulation(1.0/60.0);
		plane->Get<RigidBody>()->rigid_body->setLinearVelocity(
				btVector3(0.f, 0.f, 0.f));

		glm::vec3 force;

		if(Input::GetKey(JKEY_KEY_W) >= JKEY_PRESS)
			force.z = -30.f;
		if(Input::GetKey(JKEY_KEY_S) >= JKEY_PRESS)
			force.z = 30.f;
		if(Input::GetKey(JKEY_KEY_A) >= JKEY_PRESS)
			force.x = -30.f;
		if(Input::GetKey(JKEY_KEY_D) >= JKEY_PRESS)
			force.x = 30.f;
		if(Input::GetKey(JKEY_KEY_E) >= JKEY_PRESS)
			force.y = 30.f;

		force *= 1.1f;

		if(force != glm::vec3(0))
		{
			entities_3d[0]->Get<RigidBody>()->rigid_body->
				setActivationState(ACTIVE_TAG);
			entities_3d[0]->Get<RigidBody>()->rigid_body->
				applyCentralForce(btVector3(force.x, force.y, force.z));
		}

		phys->IsSleeping(entities_3d[0]);

		for(int i = 0; i < entities_3d.size(); ++i)
			SyncMovementToRigidBody(entities_3d[i]);

		for(int i = 0; i < entities_3d.size(); ++i)
		{
			SyncMovementToRigidBody(plane);

			if(phys->IsSleeping(entities_3d[i]))
				entities_3d[i]->Get<Colour>()->colour =
					glm::vec4(0.8f, 0.1f, 0.f, 1.f);
			else
				entities_3d[i]->Get<Colour>()->colour =
					glm::vec4(0.1f, 0.8f, 0.f, 1.f);
		}

		ProgressAnimation(entity_2d, gametime.GetFrameTime());

		renderer_3d->SetViewPosition(camera->pos.GetPosition());
	}

	void Draw()
	{
		for(int i = 0; i < entities_3d.size(); ++i)
			DrawEntity3D(entities_3d[i]);

		DrawEntity3D(plane);
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0, 0, 1.009f));

		renderer_2d->AddText(f, "MrotavatorW", glm::vec2(-10, 0), glm::vec4(0.f,
					0.f, 0.f, 1.f), FontAlign::LEFT, glm::vec2(0.05f));
		renderer_2d->Add(tex_mega, model, glm::vec4(1.f),
				GetAnimationRectangle(entity_2d, tex_mega));

		camera->Post();
		renderer_3d->Flush();
		renderer_2d->Flush();
	}

	void DrawEntity3D(Entity *e)
	{
		renderer_3d->Add(e->Get<Mesh>(),
				e->Get<Material>(),
				e->Get<Colour>()->colour,
				e->Get<Movement>()->GetModelMatrix());
	}

	void Unload()
	{
		for(int i = 0; i < entities_3d.size(); ++i)
			delete entities_3d[i];

		delete mesh_crate;
		delete mesh_tri;
		delete camera;
		delete phys;
		delete renderer_3d;
	}

	static Entity* CreateEntity2D(Texture *t, const glm::vec3& pos)
	{
		Entity *e = new Entity();
		e->Add(new Animation(3, 3, glm::vec4(30, 64, 180, 180), 100));

		return e;
	}

	static Entity* CreateEntity3D(const glm::vec3& pos, const glm::vec3& scale,
			const glm::vec4& color, Mesh *m, Material *mat, PhysicsManager *p)
	{
		glm::vec4 c = glm::vec4((color.x + color.y + color.z)/3.f);
		c.w = color.w;

		Entity *e = new Entity();
		e->Add(new Movement());
		e->Add(m);
		e->Add(mat);
		e->Add(new Colour());
		e->Get<Movement>()->Move(pos);
		e->Get<Movement>()->SetScale(scale);
		e->Get<Colour>()->colour = c;
		e->Add(new RigidBody(p, e));
		return e;
	}
};

