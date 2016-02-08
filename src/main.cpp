#include <stdio.h>
#include <bitset>
#include <map>

#include "game_world.cpp"
#include "entity.h"
#include "sprite_renderable.h"
#include "keys.h"
#include "window.h"

#include "camera.h"

struct TestEntity : Entity
{
	Texture *tex;
	SpriteRenderable *renderable;
	RenderManager2D *renderer;
	MovementComponent move;

	TestEntity(Texture *t, const glm::vec3 &pos, RenderManager2D *r)
	{
		tex = t;
		renderer = r;
		renderable = new SpriteRenderable(this);
		move.Move(pos);
	}

	virtual void Update()
	{
		move.Rotate(0.1f, glm::vec3(1.f, 0.f, 0.f));
	}

	virtual void Draw()
	{
		renderer->Add(tex, move.GetModelMatrix(), glm::vec4(1.f));
	}
};


struct GameWorld : BaseGameWorld
{
private:
	RenderManager2D *renderer_2d;

	std::vector<TestEntity*> e;
	Camera *camera;

	Texture *tex_red;
	Texture *tex_blue;

	void Initialise()
	{
		Texture::Init();

		tex_red = Texture::LoadColour(glm::vec4(1.f, 0.f, 0.f, 1.f));
		tex_blue = Texture::LoadColour(glm::vec4(0.f, 1.f, 0.f, 1.f));

		renderer_2d = new RenderManager2D();

		e.push_back(new TestEntity(tex_red, glm::vec3(0, 0.f, 0.f),
					renderer_2d));
		e.push_back(new TestEntity(tex_blue, glm::vec3(1, 0.f, 0.f),
					renderer_2d));

		camera = new Camera();
	}

	void Update()
	{
		if(Input::GetKey(JKEY_KEY_W) == JKEY_PRESS)
			printf("Pressed!\n");
		else if(Input::GetKey(JKEY_KEY_W) == JKEY_RELEASE)
			printf("Released!\n");

		for(int i = 0; i < 1; ++i)
			e[i]->Update();
	}

	void Draw()
	{
		camera->move.MoveX(0.1f);

		for(int i = 0; i < e.size(); ++i)
			e[i]->Draw();

		camera->UpdateRenderer(renderer_2d);

		renderer_2d->Manage();
	}

	void Unload()
	{
		for(int i = 0; i < e.size(); ++i)
			delete e[i];

		delete camera;
	}
};

int main()
{
	GameWorld *world = new GameWorld();
	world->Run();

	delete world;
	return 0;
}
