#include <stdio.h>
#include <map>
#include <vector>

#include "../../game/game_world.h"

#include "../../render/render_manager_3d.h"
#include "../../render/render_manager_2d.h"
#include "../../component/animation.h"
#include "../../component/camera.h"
#include "../../component/entity.h"

#include "3d_entity.h"
#include "entity_2d.h"

#include "../../physics/physics_manager.h"

#define ENTITY_NUM 100
#define ENTITY_SPACING 1.5

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

	std::vector<TestEntity*> entities;
	TestEntity2D *entity_2d;

	Texture *textures[3];
	Texture *tex_mega;

	void Initialise()
	{
		/* Init. */
		Texture::Init();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		renderer_3d = new RenderManager3D();
		renderer_2d = new RenderManager2D();
		phys = PhysicsManager::Create(glm::vec3(0.f, 0.5f, 0.f));


		/* Load assets. */
		textures[0] =	Texture::LoadColour(glm::vec4(1.f, 0.3f, 0.9f, 1.f));
		textures[1] =	Texture::LoadColour(glm::vec4(0.9f, 0.8f, 0.3f, 1.f));
		textures[2] =	Texture::LoadColour(glm::vec4(0.1f, 0.7f, 0.8f, 1.f));
		tex_mega	=	Texture::LoadFile("assets/mega_run.png");

		mesh_crate = Mesh::LoadFile("assets/boxy.obj",
				renderer_3d->shader_program);
		mesh_tri = Mesh::LoadFile("assets/tri.obj",
				renderer_3d->shader_program);

		entity_2d = new TestEntity2D(tex_mega, glm::vec3(20.f, 2.f, 1.f),
				renderer_2d);

		/* Set up a load of test entities. */

		int i = 0;
		for(i = 0; i < ENTITY_NUM; ++i)
		{
			Mesh *m = i % 2 ? mesh_crate : mesh_tri;

			entities.push_back(new TestEntity(phys, m, textures[i % 3],
						glm::vec3(i * ENTITY_SPACING, 0.f, 0.f), renderer_3d));
		}
		printf("Count: %d\n", i);

		camera = new Camera();
	//	camera->PanX(ENTITY_SPACING * (ENTITY_NUM / 2));
		camera->PanX(20);
		camera->pos.Translate(glm::vec3(0.f, 10.f, 24.f));
		camera->pos.Rotate(10.f, glm::vec3(0.f, 1.f, 0.f));
	}

	void Update(const GameTime& gametime)
	{
		phys->StepSimulation(1.0/60.0);
		for(int i = 0; i < entities.size(); ++i)
			entities[i]->Update();

		entity_2d->Update(gametime.GetFrameTime());
		//camera->pos.Translate(glm::vec3(0.f, -4.f, -6.f));
		//camera->pos.Rotate(0.00f, glm::vec3(0.f, 12.0 + 1.5.f, 0.f));
		//camera->pos.Translate(glm::vec3(0.f, 4.f, 6.f));
		//camera->PanX(0.1f);

		renderer_3d->SetViewPosition(camera->pos.GetPosition());
	}

	void Draw()
	{
		//entity_2d->Draw();

		for(int i = 2; i < entities.size(); ++i)
			entities[i]->Draw();


		camera->UpdateRenderer(renderer_2d);
		camera->UpdateRenderer(renderer_3d);
		renderer_2d->Manage();
		renderer_3d->Manage();
	}

	void Unload()
	{
		for(int i = 0; i < entities.size(); ++i)
			delete entities[i];

		for(int i = 0; i < 3; ++i)
			delete textures[i];

		delete mesh_crate;
		delete mesh_tri;
		delete camera;
		delete phys;
		delete renderer_3d;
	}
};

