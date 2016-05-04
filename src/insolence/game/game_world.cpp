#include "game_world.h"
#include "entity_manager.cpp"

void BaseGameWorld::BaseInitialise()
{
	window = Window::CreateInsolenceWindow(720, 540, "Window Title");
	is_running = true;

	gametime = GameTime();
	entity_manager = new EntityManager();

	Initialise();
	BaseUpdate();
}

void BaseGameWorld::BaseUpdate()
{
	gametime.Update();
	Update(gametime);
	entity_manager->Manage(gametime);
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

	glClearColor(224.f / 255.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Draw();

	if(Camera::GetActiveCamera() != NULL)
		Camera::GetActiveCamera()->Post();

	entity_manager->FlushDraw();

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

Entity* BaseGameWorld::CreateEntity()
{
	Entity *e = entity_manager->CreateEntity();
	e->Add<Movement>();

	return e;
}
