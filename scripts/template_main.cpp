#include <insolence/insolence.h>

struct Game : BaseGameWorld
{
	void Initialise()
	{

	}

	void Update(const GameTime &gametime)
	{

	}

	void Draw()
	{

	}

	void Unload()
	{

	}
};

int main()
{
	Game *game = new Game();
	game->Run();

	delete game;
	return 0;
}
