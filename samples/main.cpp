#include <insolence/game/game_world.h>
#include <insolence/game/camera.h>
#include <insolence/game/entity_manager.h>

#include <insolence/component/mesh_renderable.h>
#include <insolence/component/sprite_renderable.h>
#include <insolence/component/text_renderable.h>
#include <insolence/component/rigid_body.h>
#include <insolence/physics/physics_manager.h>
#include <insolence/render/render_manager_2d.h>
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

struct Game1 : BaseGameWorld
{
	std::vector<Entity*> entities;

	Camera *camera;
	Mesh *mesh;
	Material *material;

	int counter = 0;
	float count = 100.f;

	void Initialise()
	{
		Texture::Init();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mesh = Mesh::LoadFile("assets/crate.obj");
		material = new Material();
		material->diffuse =
			Texture::LoadColour(glm::vec4(1.f));

		camera = new Camera();
		camera->pos.MoveZ(800.f);
		camera->PanX(count / 2.f * 1.2f);

		for(int i = 0; i < count; ++i)
			Spawn(i);
	}

	void Spawn(int i)
	{
		entities.push_back(CreateEntity());
		entities.back()->Add<MeshRenderable>(mesh, material,
				glm::vec4(i % 128 / 255.f,
					i % 64 / 255.f,
					i % 32 / 255.f,
					1.f));
		entities.back()->Get<Movement>()->Move(
				glm::vec3(i * 1.2f, 1.f, 0.f));
		entities.back()->Get<Movement>()->Scale(glm::vec3(20.f));
		entities.back()->Add<RigidBody>();
		entities.back()->Get<RigidBody>()->Make2D();
	}

	void Update(const GameTime& time)
	{

		if(Input::GetKey(JKEY_KEY_S))
			Spawn(0);

		if(Input::GetKey(JKEY_KEY_D) && entities.size() > 0)
		{
			entities[0]->Destroy();
			entities.erase(entities.begin());
		}

		if(entities.size() > 0)
		{
			if(++counter > count)
				counter = 0;

			for(int i = 0; i < count; ++i)
			{
				entities[0]->Destroy();
				entities.erase(entities.begin());
				Spawn(counter);
			}
		}

		entity_manager->renderer_3d->SetViewPosition(camera->pos.GetPosition());
		camera->Post();
	}

	void Unload()
	{
		delete mesh;
		delete material;
		delete camera;
	}
};

int main()
{
	Game1 *game = new Game1();
	game->Run();

	delete game;

	return 0;
}


