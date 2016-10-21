#include <insolence/insolence.h>

#include <insolence/component/mesh_renderable.h>
#include <insolence/component/sprite_renderable.h>
#include <insolence/component/text_renderable.h>
#include <insolence/render/render_manager_2d.h>

struct Game1 : BaseGameWorld
{
	Camera *camera;

	void Initialise()
	{
		camera = new Camera(GetWindow());
	}

	void Update(const GameTime& time)
	{
	}

	void Unload()
	{
		delete camera;
	}
};

int main(int argc, char **argv)
{
	Game1 *game = new Game1();
	game->Run();

	delete game;

	return 0;
}


