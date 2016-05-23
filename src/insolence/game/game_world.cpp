#include "game_world.h"
#include "entity_manager.cpp"

void BaseGameWorld::BaseInitialise()
{
	window = Window::Create(720, 540, "Window Title", true);

	SetClearColour(glm::vec3(30.f, 130.f, 180.f));
	is_running = true;

	entity_manager = new EntityManager();

	Initialise();
}

void BaseGameWorld::BaseUpdate()
{
	Window::PollEvents();
	Update(gametime);
}

void BaseGameWorld::BaseDraw()
{
	glClearColor(clear_colour.x, clear_colour.y, clear_colour.z, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(Camera::GetActiveCamera() != NULL)
		Camera::GetActiveCamera()->Post();

	entity_manager->Manage(gametime);
	entity_manager->FlushDraw();

	window->SwapBuffers();
}

void BaseGameWorld::BaseUnload()
{
	Unload();

	Window::Destroy(window);
}

void BaseGameWorld::Run()
{
	double accumulator = 0;

	BaseInitialise();

	while(window->ShouldClose() == false)
	{
		int frame_time_start = Time::NowMilliseconds();

		while(accumulator >= FRAME_TIME)
		{
			BaseUpdate();

			accumulator -= FRAME_TIME;
		}

		BaseDraw();
		gametime.Update();

		accumulator += Time::NowMilliseconds() - frame_time_start;
	}

	BaseUnload();
}

Entity* BaseGameWorld::CreateEntity()
{
	Entity *e = entity_manager->CreateEntity();
	e->Add<Movement>();

	return e;
}

void BaseGameWorld::SetClearColour(const glm::vec3& colour)
{
	for(int i = 0; i < 3; ++i)
		clear_colour[i] = colour[i] / 255.f;
}
