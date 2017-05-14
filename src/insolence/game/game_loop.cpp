#include "game_loop.h"

#include "camera.h"
#include "input.h"
#include "game.h"
#include "window.h"

#include "../render/render_manager_2d.h"
#include "../render/render_manager_3d.h"
#include "../system/sprite_renderable_system.h"
#include "../system/mesh_renderable_system.h"
#include "../system/text_renderable_system.h"
#include "default_renderers.h"

void BaseGameLoop::BaseInitialise()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	Initialise();

	if(Camera::GetActive() == NULL)
		log(Log::WARN, "GameWorld - No camera set");
}

void BaseGameLoop::BaseUpdate()
{
	Window::PollEvents();
	Update(gametime);
	Game::Get()->entity_manager->Manage(gametime);
}

void BaseGameLoop::BaseDraw()
{
	glm::vec3& clear_colour = Game::Get()->clear_colour;
	glClearColor(clear_colour.x, clear_colour.y, clear_colour.z, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(Camera::GetActive() != NULL)
		Camera::GetActive()->Post();

	DefaultRenderer::Get()->Flush(gametime);
	Draw(gametime);
	Game::Get()->window->SwapBuffers();
}

void BaseGameLoop::BaseUnload()
{
	Unload();
}

#ifdef INSOLENCE_WEBGL
void BaseGameLoop::RunWebGL(void *gw)
{
	BaseGameLoop *world = (BaseGameLoop*)gw;
	world->BaseUpdate();
	world->BaseDraw();
	world->gametime.Update();
}
#endif

void BaseGameLoop::Start()
{
#ifdef INSOLENCE_WEBGL
	BaseInitialise();
	emscripten_set_main_loop_arg(BaseGameLoop::RunWebGL, this, 0, 1);
	BaseUnload();
	return;
#endif

	BaseInitialise();

	while(Game::Get()->window->ShouldClose() == false)
	{
		BaseUpdate();
		BaseDraw();
		gametime.Update();
	}

	BaseUnload();
}
