#include "game.h"

#include "camera.h"
#include "input.h"
#include "entity.h"
#include "entity_manager.h"
#include "game_loop.h"
#include "window.h"

#include "../util/console.h"
#include "../util/console_functions.cpp"

#include "../render/render_manager_2d.h"
#include "../render/render_manager_3d.h"
#include "../system/sprite_renderable_system.h"
#include "../system/mesh_renderable_system.h"
#include "../system/text_renderable_system.h"
#include "default_renderers.h"

void Game::Run()
{
	if(loop == nullptr)
		log(Log::FATAL, "Game loop not set");

	window = Window::Create(
			startup_properties.window_width,
			startup_properties.window_height,
			startup_properties.window_title.c_str(),
			startup_properties.is_resizable);

	entity_manager = new EntityManager();
	entity_manager->AddRenderSystem<SpriteRenderableSystem>(
			DefaultRenderer::Get()->Renderer2D);
	entity_manager->AddRenderSystem<MeshRenderableSystem>(
			DefaultRenderer::Get()->Renderer3D);
	entity_manager->AddRenderSystem<TextRenderableSystem>(
			DefaultRenderer::Get()->RendererText);


	console = new Console();
	RegisterConsoleFunctions();
	loop->Start();

	delete entity_manager;
	delete loop;
	delete console;
	Window::Destroy(window);
}

Entity* Game::CreateEntity()
{
	return Game::Get()->entity_manager->CreateEntity();
}
