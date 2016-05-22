#include "game_world.h"
#include "entity_manager.cpp"

void BaseGameWorld::BaseInitialise()
{
	window = Window::CreateInsolenceWindow(720, 540, "Window Title");
	is_running = true;

	gametime = GameTime();
	entity_manager = new EntityManager();

	Initialise();
}

void BaseGameWorld::BaseUpdate()
{
	gametime.Update();
	Update(gametime);
	entity_manager->Manage(gametime);
}

void BaseGameWorld::BaseDraw()
{
	glClearColor(224.f / 255.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Draw();

	if(Camera::GetActiveCamera() != NULL)
		Camera::GetActiveCamera()->Post();

	entity_manager->FlushDraw();

	window->SwapBuffers();
	Window::PollEvents();
}

void BaseGameWorld::BaseUnload()
{
	Unload();

	Window::DestroyInsolenceWindow(window);
}

void BaseGameWorld::Run()
{
	BaseInitialise();
	BaseUpdate();

	while(window->ShouldClose() == false)
	{
		BaseUpdate();
		BaseDraw();
	}

	BaseUnload();
}

Entity* BaseGameWorld::CreateEntity()
{
	Entity *e = entity_manager->CreateEntity();
	e->Add<Movement>();

	return e;
}
