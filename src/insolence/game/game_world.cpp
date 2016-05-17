#include "game_world.h"
#include "entity_manager.cpp"

void BaseGameWorld::BaseInitialise()
{
	window = Window::CreateInsolenceWindow(720, 540, "Window Title");
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
	glClearColor(224.f / 255.f, 1.f, 1.f, 1.f);
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

	Window::DestroyInsolenceWindow(window);
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
