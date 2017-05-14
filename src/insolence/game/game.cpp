#include "game.h"

#include "camera.h"
#include "input.h"
#include "game_loop.h"
#include "window.h"

#include "../ecs/entity_manager.h"
#include "../render/render_manager_2d.h"
#include "../render/render_manager_3d.h"
#include "../render/render_manager_simple_particles.h"
#include "../system/sprite_renderable_system.h"
#include "../system/mesh_renderable_system.h"
#include "../system/text_renderable_system.h"
#include "../util/console.h"
#include "../util/console_functions.cpp"

void Game::Run()
{
	if(loop == nullptr)
		log(Log::FATAL, "Game loop not set");

	window = Window::Create(
			startup_properties.window_width,
			startup_properties.window_height,
			startup_properties.window_title.c_str(),
			startup_properties.is_resizable);

	CreateShaderPrograms();
	CreateRenderers();

	entity_manager = new EntityManager();
	console = new Console();
	RegisterConsoleFunctions();

	loop->Start();

	delete console;
	delete entity_manager;

	delete renderers.sprite;
	delete renderers.mesh;
	delete renderers.text;
	delete renderers.particles;

	delete loop;
	Window::Destroy(window);
}

Entity* Game::CreateEntity()
{
	return Game::Get()->entity_manager->CreateEntity();
}

void Game::CreateShaderPrograms()
{
	resources.shader_programs.Add(EngineResources::ShaderProgram::DEFAULT_2D,
			ShaderProgram::FromPair("shaders/2d.vs", "shaders/2d.fs"));

	resources.shader_programs.Add(EngineResources::ShaderProgram::DEFAULT_3D,
			ShaderProgram::FromPair("shaders/3d.vs", "shaders/3d.fs"));

	resources.shader_programs.Add(EngineResources::ShaderProgram::DEFAULT_TEXT,
			ShaderProgram::FromPair("shaders/2d.vs", "shaders/text.fs"));

	resources.shader_programs.Add(EngineResources::ShaderProgram::PARTICLES,
			ShaderProgram::FromPair("shaders/particles.vs", "shaders/particles.fs"));
}

void Game::CreateRenderers()
{
	renderers.sprite =
		new RenderManager2D(resources.shader_programs.Get(
					EngineResources::ShaderProgram::DEFAULT_2D));

	renderers.mesh =
		new RenderManager3D(resources.shader_programs.Get(
					EngineResources::ShaderProgram::DEFAULT_3D));

	renderers.text =
		new RenderManager2D(resources.shader_programs.Get(
					EngineResources::ShaderProgram::DEFAULT_TEXT));

	renderers.particles =
		new RenderManagerSimpleParticles(resources.shader_programs.Get(
					EngineResources::ShaderProgram::PARTICLES));
}
