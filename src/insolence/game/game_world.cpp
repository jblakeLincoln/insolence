#include "game_world.h"

#include "camera.h"
#include "input.h"
#include "entity.h"
#include "entity_manager.h"
#include "window.h"

void BaseGameWorld::BaseInitialise()
{
	window = Window::Create(
			initialise_properties.window_width,
			initialise_properties.window_height,
			initialise_properties.window_title,
			initialise_properties.resizable);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	SetClearColour(glm::vec3(0.f));
	is_running = true;

	entity_manager = new EntityManager();

	Initialise();

	if(Camera::GetActiveCamera() == NULL)
		log(Log::WARN, "GameWorld - No camera set");
}

void BaseGameWorld::BaseUpdate()
{
	Window::PollEvents();
	Update(gametime);
	entity_manager->Manage(gametime);
}

void BaseGameWorld::BaseDraw()
{
	glClearColor(clear_colour.x, clear_colour.y, clear_colour.z, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(Camera::GetActiveCamera() != NULL)
		Camera::GetActiveCamera()->Post();

	entity_manager->FlushDraw(gametime);

	window->SwapBuffers();
}

void BaseGameWorld::BaseUnload()
{
	Unload();

	delete entity_manager;
	Window::Destroy(window);
}

#ifdef INSOLENCE_WEBGL
void BaseGameWorld::RunWebGL(void *gw)
{
	BaseGameWorld *world = (BaseGameWorld*)gw;
	world->BaseUpdate();
	world->BaseDraw();
	world->gametime.Update();
}
#endif

void BaseGameWorld::Run(int width, int height, const char *title,
		bool resizable)
{
	initialise_properties.window_width = width;
	initialise_properties.window_height = height;
	initialise_properties.window_title = title;
	initialise_properties.resizable = resizable;

#ifdef INSOLENCE_WEBGL
	BaseInitialise();
	emscripten_set_main_loop_arg(BaseGameWorld::RunWebGL, this, 0, 1);
	BaseUnload();
	return;
#endif

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
	e->Add<Transform>();

	return e;
}

void BaseGameWorld::SetClearColour(const glm::vec3& colour)
{
	for(int i = 0; i < 3; ++i)
		clear_colour[i] = colour[i] / 255.f;
}
