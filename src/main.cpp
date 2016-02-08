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
	SpriteRenderable *renderable;
	RenderManager2D *renderer;
	MovementComponent move;

	TestEntity(RenderManager2D *r)
	{
		renderer = r;
		renderable = new SpriteRenderable(this);
	}

	virtual void Update()
	{
		move.Rotate(0.1f, glm::vec3(1.f, 0.f, 0.f));
	}

	virtual void Draw()
	{
		renderer->Add(move.GetModelMatrix(), glm::vec4(1.f));
	}
};


struct GameWorld : BaseGameWorld
{
private:
	RenderManager2D *renderer_2d;

	TestEntity *e;
	Camera *camera;

	void Initialise()
	{
		renderer_2d = new RenderManager2D();
		e = new TestEntity(renderer_2d);
		camera = new Camera();
	}

	void Update()
	{
		if(Input::GetKey(JKEY_KEY_W) == JKEY_PRESS)
			printf("Pressed!\n");
		else if(Input::GetKey(JKEY_KEY_W) == JKEY_RELEASE)
			printf("Released!\n");

		glm::vec2 mouse = Input::GetCursorPos();

		e->Update();
	}

	void Draw()
	{
		camera->move.MoveX(0.1f);
		e->Draw();

		camera->UpdateRenderer(renderer_2d);

		renderer_2d->Manage();
	}

	void Unload()
	{
		delete e;
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
