#include <insolence/insolence.h>

#include <insolence/component/mesh_renderable.h>
#include <insolence/component/sprite_renderable.h>
#include <insolence/component/text_renderable.h>
#include <insolence/render/render_manager_2d.h>

struct Game1 : BaseGameLoop
{
	Camera *camera;

	void Initialise()
	{
		camera = new Camera(Game::Get()->window);
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
	Game::Get()->loop = new Game1();
	Game::Get()->Run();
}
