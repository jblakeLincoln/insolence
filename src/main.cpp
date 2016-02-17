#include <stdio.h>
#include <bitset>
#include <map>

#include "game_world.cpp"
#include "entity.h"
#include "sprite_renderable.h"
#include "keys.h"
#include "window.h"

#include "mesh.h"

#include "camera.h"
#include "shader_program.h"
#include "render_manager_3d.h"

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


struct GameWorld : BaseGameWorld
{
private:
	RenderManager3D *renderer_3d;
	Mesh *mesh_crate;
	Mesh *mesh_tri;

	std::vector<TestEntity*> e;

	Camera *camera;
	//Texture *tex_red;
	//Texture *tex_blue;
	Texture *textures[3];

	void Initialise()
	{
		/* Init. */
		Texture::Init();
		glEnable(GL_DEPTH_TEST);
		renderer_3d = new RenderManager3D();

		/* Load assets. */
		//tex_red = Texture::LoadColour(glm::vec4(1.f, 0.f, 0.f, 1.f));
		//tex_blue = Texture::LoadColour(glm::vec4(0.f, 1.f, 0.f, 1.f));
		textures[0] = Texture::LoadColour(glm::vec4(1.f, 0.f, 0.f, 1.f));
		textures[1] = Texture::LoadColour(glm::vec4(0.f, 1.f, 0.f, 1.f));
		textures[2] = Texture::LoadColour(glm::vec4(0.f, 0.f, 1.f, 1.f));

		mesh_crate = Mesh::LoadFile("assets/boxy.obj",
				renderer_3d->shader_program);
		mesh_tri = Mesh::LoadFile("assets/tri.obj",
				renderer_3d->shader_program);

		/* Set up a load of test entities. */
		int max = 300;
		int t = 2.1f;

		for(int i = 0; i < max; ++i)
		{
			Mesh *m = i % 2 ? mesh_crate : mesh_tri;

			e.push_back(new TestEntity(m, textures[i % 3],
						glm::vec3(i * t, 0.f, 0.f), renderer_3d));
		}

		camera = new Camera();
		camera->PanX(t * (max / 2));
		camera->pos.Translate(glm::vec3(0.f, 4.f, 12.f));
	}

	void Update(const GameTime& gametime)
	{
		//printf("%d\n", gametime.GetElapsedTime().ElapsedMilliseconds());
		if(Input::GetKey(JKEY_KEY_W) == JKEY_PRESS)
			printf("Pressed!\n");
		else if(Input::GetKey(JKEY_KEY_W) == JKEY_RELEASE)
			printf("Released!\n");

		for(int i = 0; i < 1; ++i)
			e[i]->Update();

		camera->pos.Translate(glm::vec3(0.f, -4.f, -12.f));
		camera->pos.Rotate(0.01f, glm::vec3(0.f, 1.f, 0.f));
		camera->pos.Translate(glm::vec3(0.f, 4.f, 12.f));

		renderer_3d->SetViewPosition(camera->pos.GetPosition());
	}

	void Draw()
	{
		camera->UpdateRenderer(renderer_3d);

		for(int i = 0; i < e.size(); ++i)
			e[i]->Draw();

		renderer_3d->Manage();
	}

	void Unload()
	{
		for(int i = 0; i < e.size(); ++i)
			delete e[i];

		for(int i = 0; i < 3; ++i)
			delete textures[i];

		delete mesh_crate;
		delete mesh_tri;
		delete camera;
		delete renderer_3d;
	}
};

int main()
{
	GameWorld *world = new GameWorld();
	world->Run();

	delete world;
	return 0;
}
