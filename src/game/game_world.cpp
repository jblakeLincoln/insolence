#include "game_world.h"

void BaseGameWorld::BaseInitialise()
{
	window = Window::CreateInsolenceWindow(500, 500, "Window Title");
	is_running = true;

	gametime = GameTime();

	Initialise();
	BaseUpdate();
}

void BaseGameWorld::BaseUpdate()
{
	gametime.Update();
	Update(gametime);
	BaseDraw();
}

void BaseGameWorld::BaseDraw()
{
	is_running = !window->ShouldClose();

	if(is_running == false)
	{
		Unload();
		return;
	}

	glClearColor(0.14f, 0.01f, 0.003f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Draw();

	window->SwapBuffers();
	Window::PollEvents();

	BaseUpdate();
}

void BaseGameWorld::BaseUnload()
{
	Unload();

	Window::DestroyInsolenceWindow(window);
}

void BaseGameWorld::Run()
{
	BaseInitialise();
}
