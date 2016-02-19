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

#define ENTITY_NUM 300
#define ENTITY_SPACING 2.1

struct SampleMultipleRenderer : BaseGameWorld
{
private:
	RenderManager2D *renderer_2d;
	RenderManager3D *renderer_3d;
	Camera *camera;

	Mesh *mesh_crate;
	Mesh *mesh_tri;

	std::vector<TestEntity*> entities;

	Texture *textures[3];

	void Initialise()
	{
		/* Init. */
		Texture::Init();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		renderer_3d = new RenderManager3D();
		renderer_2d = new RenderManager2D();

		/* Load assets. */
		textures[0] =	Texture::LoadColour(glm::vec4(1.f, 0.3f, 0.9f, 1.f));
		textures[1] =	Texture::LoadColour(glm::vec4(0.9f, 0.8f, 0.3f, 1.f));
		textures[2] =	Texture::LoadColour(glm::vec4(0.1f, 0.7f, 0.8f, 1.f));

		mesh_crate = Mesh::LoadFile("assets/boxy.obj",
				renderer_3d->shader_program);
		mesh_tri = Mesh::LoadFile("assets/tri.obj",
				renderer_3d->shader_program);

		/* Set up a load of test entities. */

		for(int i = 0; i < ENTITY_NUM; ++i)
		{
			Mesh *m = i % 2 ? mesh_crate : mesh_tri;

			entities.push_back(new TestEntity(m, textures[i % 3],
						glm::vec3(i * ENTITY_SPACING, 0.f, 0.f), renderer_3d));
		}

		camera = new Camera();
		camera->PanX(ENTITY_SPACING * (ENTITY_NUM / 2));
		camera->pos.Translate(glm::vec3(0.f, 0.f, 12.f));
	}

	void Update(const GameTime& gametime)
	{
		for(int i = 0; i < 1; ++i)
			entities[i]->Update();

		camera->pos.Translate(glm::vec3(0.f, -4.f, -6.f));
		camera->pos.Rotate(0.00f, glm::vec3(0.f, 1.f, 0.f));
		camera->pos.Translate(glm::vec3(0.f, 4.f, 6.f));
		camera->PanX(0.1f);

		renderer_3d->SetViewPosition(camera->pos.GetPosition());
	}

	void Draw()
	{
		for(int i = 0; i < entities.size(); ++i)
			entities[i]->Draw();

		camera->UpdateRenderer(renderer_2d);
		camera->UpdateRenderer(renderer_3d);
		renderer_3d->Manage();
		renderer_2d->Manage();
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
		delete renderer_3d;
	}
};

